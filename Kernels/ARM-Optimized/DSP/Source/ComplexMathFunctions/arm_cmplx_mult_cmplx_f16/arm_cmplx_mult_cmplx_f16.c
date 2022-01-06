/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_cmplx_mult_cmplx_f16.c
 * Description:  Floating-point complex-by-complex multiplication
 *               using Helium assembly kernels
 *               This version allows boosting F16 complex-by-complex performance when using compilers having suboptimal
 *               Helium intrinsic code generation.
 *
 * $Date:        Jan 2022
 * $Revision:    V1.0.0
 *
 * Target Processor: Cortex-M cores with Helium
 * -------------------------------------------------------------------- */
/*
 * Copyright (C) 2010-2022 ARM Limited or its affiliates. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "dsp/complex_math_functions_f16.h"

#if defined(ARM_FLOAT16_SUPPORTED)

/**
  @ingroup groupCmplxMath
 */

/**
  @defgroup CmplxByCmplxMult Complex-by-Complex Multiplication

  Multiplies a complex vector by another complex vector and generates a complex result.
  The data in the complex arrays is stored in an interleaved fashion
  (real, imag, real, imag, ...).
  The parameter <code>numSamples</code> represents the number of complex
  samples processed.  The complex arrays have a total of <code>2*numSamples</code>
  real values.

  The underlying algorithm is used:

  <pre>
  for (n = 0; n < numSamples; n++) {
      pDst[(2*n)+0] = pSrcA[(2*n)+0] * pSrcB[(2*n)+0] - pSrcA[(2*n)+1] * pSrcB[(2*n)+1];
      pDst[(2*n)+1] = pSrcA[(2*n)+0] * pSrcB[(2*n)+1] + pSrcA[(2*n)+1] * pSrcB[(2*n)+0];
  }
  </pre>

  There are separate functions for floating-point, Q15, and Q31 data types.
 */

/**
  @addtogroup CmplxByCmplxMult
  @{
 */

/**
  @brief         Floating-point complex-by-complex multiplication.
  @param[in]     pSrcA       points to first input vector
  @param[in]     pSrcB       points to second input vector
  @param[out]    pDst        points to output vector
  @param[in]     numSamples  number of samples in each vector
  @return        none
 */

#if defined(ARM_MATH_MVE_FLOAT16)

void arm_cmplx_mult_cmplx_f16_mve(
  const float16_t * pSrcA,
  const float16_t * pSrcB,
        float16_t * pDst,
        uint32_t numSamples)
{
#ifdef USE_ASM
    uint32_t  tail = numSamples & 7;

    /* Core loop is unrolled to optimize load / multiplication interleaving */
    __asm volatile (
        "curCpx                 .req q2                 \n"

        /* preload */
        "   vldrh.16        q0, [%[pA]], #16            \n"
        "   vldrh.16        q1, [%[pB]], #16            \n"

        /*
         * Complex vector Multiplication loop
         * Produces 4 complex elements
         */
        ".p2align 2                                     \n"
        "   wls             lr, %[cnt], 1f              \n"
        /* low overhead loop start */
        "2:                                             \n"
        "   vcmul.f16       curCpx, q0, q1, #0          \n"
        "   vldrh.16        q3, [%[pA]], 16             \n"
        "   vcmla.f16       curCpx, q0, q1, #90         \n"
        "   vldrh.16        q1, [%[pB]], 16             \n"
        /* store 4 complex pairs */
        "   vstrh.16        curCpx, [%[pD]], 16         \n"

        "   vcmul.f16       curCpx, q3, q1, #0          \n"
        "   vldrh.16        q0, [%[pA]], 16             \n"
        "   vcmla.f16       curCpx, q3, q1, #90         \n"
        "   vldrh.16        q1, [%[pB]], 16             \n"
        /* store 4 complex pairs */
        "   vstrh.16        curCpx, [%[pD]], 16         \n"

        /* low overhead loop end */
        "   le              lr, 2b                      \n"
        "1:                                             \n"

        /* tail handling */
        "   wlstp.16        lr, %[tail], 1f             \n"
        "2:                                             \n"
        "   vcmul.f16       curCpx, q0, q1, #0          \n"
        "   vcmla.f16       curCpx, q0, q1, #90         \n"
        "   vstrh.16        curCpx, [%[pD]], 16         \n"
        "   vldrh.16        q0, [%[pA]], 16             \n"
        "   vldrh.16        q1, [%[pB]], 16             \n"
        "   letp            lr, 2b                      \n"
        "1:                                             \n"

        :[pD] "+r"(pDst), [pA] "+r"(pSrcA), [pB] "+r"(pSrcB)
        :[cnt] "r"(numSamples / 8), [tail] "r" (tail * CMPLX_DIM)
        : "q0", "q1", "q2", "q3",
          "memory", "r14");

#else

    /* CMSIS DSP MVE intrinsic reference */
    int32_t         blkCnt;
    f16x8_t         vecSrcA, vecSrcB;
    f16x8_t         vecSrcC, vecSrcD;
    f16x8_t         vec_acc;

    blkCnt = (numSamples >> 3);
    blkCnt -= 1;
    if (blkCnt > 0) {
        /* should give more freedom to generate stall free code */
        vecSrcA = vld1q(pSrcA);
        vecSrcB = vld1q(pSrcB);
        pSrcA += 8;
        pSrcB += 8;

        while (blkCnt > 0) {
            vec_acc = vcmulq(vecSrcA, vecSrcB);
            vecSrcC = vld1q(pSrcA);
            pSrcA += 8;

            vec_acc = vcmlaq_rot90(vec_acc, vecSrcA, vecSrcB);
            vecSrcD = vld1q(pSrcB);
            pSrcB += 8;
            vst1q(pDst, vec_acc);
            pDst += 8;

            vec_acc = vcmulq(vecSrcC, vecSrcD);
            vecSrcA = vld1q(pSrcA);
            pSrcA += 8;

            vec_acc = vcmlaq_rot90(vec_acc, vecSrcC, vecSrcD);
            vecSrcB = vld1q(pSrcB);
            pSrcB += 8;
            vst1q(pDst, vec_acc);
            pDst += 8;
            /*
             * Decrement the blockSize loop counter
             */
            blkCnt--;
        }

        /* process last elements out of the loop avoid the armclang breaking the SW pipeline */
        vec_acc = vcmulq(vecSrcA, vecSrcB);
        vecSrcC = vld1q(pSrcA);

        vec_acc = vcmlaq_rot90(vec_acc, vecSrcA, vecSrcB);
        vecSrcD = vld1q(pSrcB);
        vst1q(pDst, vec_acc);
        pDst += 8;

        vec_acc = vcmulq(vecSrcC, vecSrcD);
        vec_acc = vcmlaq_rot90(vec_acc, vecSrcC, vecSrcD);
        vst1q(pDst, vec_acc);
        pDst += 8;

        /*
         * tail
         */
        blkCnt = CMPLX_DIM * (numSamples & 7);
        while (blkCnt > 0) {
            mve_pred16_t    p = vctp16q(blkCnt);
            pSrcA += 8;
            pSrcB += 8;

            vecSrcA = vldrhq_z_f16(pSrcA, p);
            vecSrcB = vldrhq_z_f16(pSrcB, p);
            vec_acc = vcmulq_m(vuninitializedq_f16(),vecSrcA, vecSrcB, p);
            vec_acc = vcmlaq_rot90_m(vec_acc, vecSrcA, vecSrcB, p);

            vstrhq_p_f16(pDst, vec_acc, p);
            pDst += 8;

            blkCnt -= 8;
        }
    } else {
        /* small vector */
        blkCnt = numSamples * CMPLX_DIM;

        do {
            mve_pred16_t    p = vctp16q(blkCnt);

            vecSrcA = vldrhq_z_f16(pSrcA, p);
            vecSrcB = vldrhq_z_f16(pSrcB, p);

            vec_acc = vcmulq_m(vuninitializedq_f16(),vecSrcA, vecSrcB, p);
            vec_acc = vcmlaq_rot90_m(vec_acc, vecSrcA, vecSrcB, p);
            vstrhq_p_f16(pDst, vec_acc, p);
            pDst += 8;

            /*
             * Decrement the blkCnt loop counter
             * Advance vector source and destination pointers
             */
            pSrcA += 8;
            pSrcB += 8;
            blkCnt -= 8;
        }
        while (blkCnt > 0);
    }
#endif
}


#endif
/**
  @} end of CmplxByCmplxMult group
 */

#endif /* #if defined(ARM_FLOAT16_SUPPORTED) */
