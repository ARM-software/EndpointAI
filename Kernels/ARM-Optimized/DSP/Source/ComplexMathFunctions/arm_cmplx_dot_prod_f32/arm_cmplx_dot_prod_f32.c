/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_cmplx_dot_prod_f32.c
 * Description:  Floating-point complex dot product
 *               using Helium assembly kernels
 *               This version allows boosting complex dot product performance when using compilers having suboptimal
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

#include "dsp/complex_math_functions.h"

/**
  @ingroup groupCmplxMath
 */

/**
  @defgroup cmplx_dot_prod Complex Dot Product

  Computes the dot product of two complex vectors.
  The vectors are multiplied element-by-element and then summed.

  The <code>pSrcA</code> points to the first complex input vector and
  <code>pSrcB</code> points to the second complex input vector.
  <code>numSamples</code> specifies the number of complex samples
  and the data in each array is stored in an interleaved fashion
  (real, imag, real, imag, ...).
  Each array has a total of <code>2*numSamples</code> values.

  The underlying algorithm is used:

  <pre>
  realResult = 0;
  imagResult = 0;
  for (n = 0; n < numSamples; n++) {
      realResult += pSrcA[(2*n)+0] * pSrcB[(2*n)+0] - pSrcA[(2*n)+1] * pSrcB[(2*n)+1];
      imagResult += pSrcA[(2*n)+0] * pSrcB[(2*n)+1] + pSrcA[(2*n)+1] * pSrcB[(2*n)+0];
  }
  </pre>

  There are separate functions for floating-point, Q15, and Q31 data types.
 */

/**
  @addtogroup cmplx_dot_prod
  @{
 */

/**
  @brief         Floating-point complex dot product.
  @param[in]     pSrcA       points to the first input vector
  @param[in]     pSrcB       points to the second input vector
  @param[in]     numSamples  number of samples in each vector
  @param[out]    realResult  real part of the result returned here
  @param[out]    imagResult  imaginary part of the result returned here
  @return        none
 */

#if defined(ARM_MATH_MVEF)

void arm_cmplx_dot_prod_f32_mve(
    const float32_t * pSrcA,
    const float32_t * pSrcB,
    uint32_t numSamples,
    float32_t * realResult,
    float32_t * imagResult)
{
#ifdef USE_ASM

    uint32_t  tail = numSamples & 3;

    __asm volatile (
        "accCpx        .req q3                              \n"

        "   vmov.i32        accCpx, #0                      \n"

        /* preload */
        "   vldrw.32        q0, [%[pA]], 16                 \n"
        "   vldrw.32        q1, [%[pB]], 16                 \n"

        /*
         * Complex vector dot-product loop
         * Process 4 complex elements / iteration
         */
        ".p2align 2                                         \n"
        "   wls             lr, %[cnt], 1f                  \n"

        /* low overhead loop start */
        "2:                                                 \n"
        "   vcmla.f32       accCpx, q0, q1, #0              \n"
        "   vldrw.32        q2, [%[pA]], 16                 \n"
        "   vcmla.f32       accCpx, q0, q1, #90             \n"
        "   vldrw.32        q1, [%[pB]], 16                 \n"
        "   vcmla.f32       accCpx, q2, q1, #0              \n"
        "   vldrw.32        q0, [%[pA]], 16                 \n"
        "   vcmla.f32       accCpx, q2, q1, #90             \n"
        "   vldrw.32        q1, [%[pB]], 16                 \n"
        "   le              lr, 2b                          \n"
        /* low overhead loop end */
        "1:                                                 \n"

        /* tail handling */
        "   wlstp.32        lr, %[tail], 1f                 \n"
        "2:                                                 \n"
        "   vcmla.f32       accCpx, q0, q1, #0              \n"
        "   vcmla.f32       accCpx, q0, q1, #90             \n"
        "   vldrw.32        q0, [%[pA]], 16                 \n"
        "   vldrw.32        q1, [%[pB]], 16                 \n"
        "   letp            lr, 2b                          \n"
        "1:                                                 \n"

        /* Sum the partial parts */
        "   vadd.f32        s0, s12, s14                    \n"
        "   vadd.f32        s1, s13, s15                    \n"
        "   vstr.32         s0, [%[realResult]]             \n"
        "   vstr.32         s1, [%[imagResult]]             \n"

        " .unreq accCpx                                     \n"

        :[pA] "+r"(pSrcA),[pB] "+r"(pSrcB)
        :[cnt] "r"(numSamples / 4), [tail] "r"(tail * CMPLX_DIM),
         [realResult] "r"(realResult),[imagResult] "r"(imagResult)
        :"q0", "q1", "q2", "q3",
         "memory", "r14");
#else
    /* CMSIS DSP MVE intrinsic reference */

    int32_t         blkCnt;
    float32_t       real_sum, imag_sum;
    f32x4_t         vecSrcA, vecSrcB;
    f32x4_t         vec_acc = vdupq_n_f32(0.0f);
    f32x4_t         vecSrcC, vecSrcD;

    blkCnt = numSamples >> 2;
    blkCnt -= 1;
    if (blkCnt > 0) {
        /* should give more freedom to generate stall free code */
        vecSrcA = vld1q(pSrcA);
        vecSrcB = vld1q(pSrcB);
        pSrcA += 4;
        pSrcB += 4;

        while (blkCnt > 0) {
            vec_acc = vcmlaq(vec_acc, vecSrcA, vecSrcB);
            vecSrcC = vld1q(pSrcA);
            pSrcA += 4;

            vec_acc = vcmlaq_rot90(vec_acc, vecSrcA, vecSrcB);
            vecSrcD = vld1q(pSrcB);
            pSrcB += 4;

            vec_acc = vcmlaq(vec_acc, vecSrcC, vecSrcD);
            vecSrcA = vld1q(pSrcA);
            pSrcA += 4;

            vec_acc = vcmlaq_rot90(vec_acc, vecSrcC, vecSrcD);
            vecSrcB = vld1q(pSrcB);
            pSrcB += 4;
            /*
             * Decrement the blockSize loop counter
             */
            blkCnt--;
        }

         /* process last elements out of the loop avoid the armclang breaking the SW pipeline */
        vec_acc = vcmlaq(vec_acc, vecSrcA, vecSrcB);
        vecSrcC = vld1q(pSrcA);

        vec_acc = vcmlaq_rot90(vec_acc, vecSrcA, vecSrcB);
        vecSrcD = vld1q(pSrcB);

        vec_acc = vcmlaq(vec_acc, vecSrcC, vecSrcD);
        vec_acc = vcmlaq_rot90(vec_acc, vecSrcC, vecSrcD);

        /*
         * tail
         */
        blkCnt = CMPLX_DIM * (numSamples & 3);
        while (blkCnt > 0) {
            mve_pred16_t    p = vctp32q(blkCnt);
            pSrcA += 4;
            pSrcB += 4;
            vecSrcA = vldrwq_z_f32(pSrcA, p);
            vecSrcB = vldrwq_z_f32(pSrcB, p);
            vec_acc = vcmlaq_m(vec_acc, vecSrcA, vecSrcB, p);
            vec_acc = vcmlaq_rot90_m(vec_acc, vecSrcA, vecSrcB, p);
            blkCnt -= 4;
        }
    } else {
        /* small vector */
        blkCnt = numSamples * CMPLX_DIM;
        vec_acc = vdupq_n_f32(0.0f);

        do {
            mve_pred16_t    p = vctp32q(blkCnt);

            vecSrcA = vldrwq_z_f32(pSrcA, p);
            vecSrcB = vldrwq_z_f32(pSrcB, p);

            vec_acc = vcmlaq_m(vec_acc, vecSrcA, vecSrcB, p);
            vec_acc = vcmlaq_rot90_m(vec_acc, vecSrcA, vecSrcB, p);

            /*
             * Decrement the blkCnt loop counter
             * Advance vector source and destination pointers
             */
            pSrcA += 4;
            pSrcB += 4;
            blkCnt -= 4;
        }
        while (blkCnt > 0);
    }

    real_sum = vgetq_lane(vec_acc, 0) + vgetq_lane(vec_acc, 2);
    imag_sum = vgetq_lane(vec_acc, 1) + vgetq_lane(vec_acc, 3);
    /*
     * Store the real and imaginary results in the destination buffers
     */
    *realResult = real_sum;
    *imagResult = imag_sum;

#endif
}

#endif
/**
  @} end of cmplx_dot_prod group
 */
