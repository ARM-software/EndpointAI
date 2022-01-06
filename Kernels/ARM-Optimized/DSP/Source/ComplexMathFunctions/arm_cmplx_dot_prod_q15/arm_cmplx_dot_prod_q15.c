/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_cmplx_dot_prod_q15.c
 * Description:  Processing function for the Q15 Complex Dot product
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
  @addtogroup cmplx_dot_prod
  @{
 */

/**
  @brief         Q15 complex dot product.
  @param[in]     pSrcA       points to the first input vector
  @param[in]     pSrcB       points to the second input vector
  @param[in]     numSamples  number of samples in each vector
  @param[out]    realResult  real part of the result returned here
  @param[out]    imagResult  imaginary part of the result returned her
  @return        none

  @par           Scaling and Overflow Behavior
                   The function is implemented using an internal 64-bit accumulator.
                   The intermediate 1.15 by 1.15 multiplications are performed with full precision and yield a 2.30 result.
                   These are accumulated in a 64-bit accumulator with 34.30 precision.
                   As a final step, the accumulators are converted to 8.24 format.
                   The return results <code>realResult</code> and <code>imagResult</code> are in 8.24 format.
 */

#if defined(ARM_MATH_MVEI)
void arm_cmplx_dot_prod_q15_mve(
  const q15_t * pSrcA,
  const q15_t * pSrcB,
        uint32_t numSamples,
        q31_t * realResult,
        q31_t * imagResult)
{
    q63_t           accReal = 0LL;
    q63_t           accImag = 0LL;

#ifdef USE_ASM
    uint32_t  tail = numSamples & 7;

    __asm volatile (
        /* pre-load */
        "   vldrh.16            q0, [%[pA]], 16                     \n"
        "   vldrh.16            q1, [%[pB]], 16                     \n"

        /*
         * Complex vector dot-product loop
         * Process 4 complex elements / iteration
         */
        ".p2align 2                                                 \n"
        "   wls                 lr, %[cnt], 1f                      \n"
        /* low overhead loop start */
        "2:                                                         \n"
        "   vmlsldava.s16       %Q[accR], %R[accR], q0, q1          \n"
        "   vldrh.16            q2, [%[pA]], 16                     \n"
        "   vmlaldavax.s16      %Q[accI], %R[accI], q0, q1          \n"
        "   vldrh.16            q3, [%[pB]], 16                     \n"
        "   vmlsldava.s16       %Q[accR], %R[accR], q2, q3          \n"
        "   vldrh.16            q0, [%[pA]], 16                     \n"
        "   vmlaldavax.s16      %Q[accI], %R[accI], q2, q3          \n"
        "   vldrh.16            q1, [%[pB]], 16                     \n"
        "   le                  lr, 2b                              \n"
        /* low overhead loop end */
        "1:                                                         \n"

        /* tail handling */
        ".p2align 2                                                 \n"
        "   wlstp.16            lr, %[tail], 1f                     \n"
        "2:                                                         \n"
        "   vmlsldava.s16       %Q[accR], %R[accR], q0, q1          \n"
        "   vmlaldavax.s16      %Q[accI], %R[accI], q0, q1          \n"
        "   vldrh.16            q0, [%[pA]], 16                     \n"
        "   vldrh.16            q1, [%[pB]], 16                     \n"
        "   letp                lr, 2b                              \n"
        "1:                                                         \n"

        /* scale and store */
        "   asrl                %Q[accR], %R[accR], #(14 - 8)       \n"
        "   asrl                %Q[accI], %R[accI], #(14 - 8)       \n"
        "   str                 %Q[accR], [%[realResult]]           \n"
        "   str                 %Q[accI], [%[imagResult]]           \n"

        :[pA] "+r"(pSrcA),[pB] "+r"(pSrcB),
         [accR] "+r" (accReal), [accI] "+r" (accImag)
        :[cnt] "r"(numSamples / 8), [tail] "r" (CMPLX_DIM * tail),
        [realResult] "r"(realResult), [imagResult] "r"(imagResult)
        : "q0", "q1", "q2", "q3",
          "lr", "memory");
#else

    /* CMSIS DSP MVE intrinsic reference */
    int32_t         blkCnt;
    q15x8_t         vecSrcA, vecSrcB;
    q15x8_t         vecSrcC, vecSrcD;

    blkCnt = (numSamples >> 3);
    blkCnt -= 1;
    if (blkCnt > 0) {
        /* should give more freedom to generate stall free code */
        vecSrcA = vld1q(pSrcA);
        vecSrcB = vld1q(pSrcB);
        pSrcA += 8;
        pSrcB += 8;

        while (blkCnt > 0) {

            accReal = vmlsldavaq(accReal, vecSrcA, vecSrcB);
            vecSrcC = vld1q(pSrcA);
            pSrcA += 8;

            accImag = vmlaldavaxq(accImag, vecSrcA, vecSrcB);
            vecSrcD = vld1q(pSrcB);
            pSrcB += 8;

            accReal = vmlsldavaq(accReal, vecSrcC, vecSrcD);
            vecSrcA = vld1q(pSrcA);
            pSrcA += 8;

            accImag = vmlaldavaxq(accImag, vecSrcC, vecSrcD);
            vecSrcB = vld1q(pSrcB);
            pSrcB += 8;
            /*
             * Decrement the blockSize loop counter
             */
            blkCnt--;
        }

        /* process last elements out of the loop avoid the armclang breaking the SW pipeline */
        accReal = vmlsldavaq(accReal, vecSrcA, vecSrcB);
        vecSrcC = vld1q(pSrcA);

        accImag = vmlaldavaxq(accImag, vecSrcA, vecSrcB);
        vecSrcD = vld1q(pSrcB);

        accReal = vmlsldavaq(accReal, vecSrcC, vecSrcD);
        vecSrcA = vld1q(pSrcA);

        accImag = vmlaldavaxq(accImag, vecSrcC, vecSrcD);
        vecSrcB = vld1q(pSrcB);

        /*
         * tail
         */
        blkCnt = CMPLX_DIM * (numSamples & 7);
        do {
            mve_pred16_t    p = vctp16q(blkCnt);

            pSrcA += 8;
            pSrcB += 8;

            vecSrcA = vldrhq_z_s16(pSrcA, p);
            vecSrcB = vldrhq_z_s16(pSrcB, p);

            accReal = vmlsldavaq_p(accReal, vecSrcA, vecSrcB, p);
            accImag = vmlaldavaxq_p(accImag, vecSrcA, vecSrcB, p);

            blkCnt -= 8;
        }
        while ((int32_t) blkCnt > 0);
    } else {
        blkCnt = numSamples * CMPLX_DIM;
        while (blkCnt > 0) {
            mve_pred16_t    p = vctp16q(blkCnt);

            vecSrcA = vldrhq_z_s16(pSrcA, p);
            vecSrcB = vldrhq_z_s16(pSrcB, p);

            accReal = vmlsldavaq_p(accReal, vecSrcA, vecSrcB, p);
            accImag = vmlaldavaxq_p(accImag, vecSrcA, vecSrcB, p);

            /*
             * Decrement the blkCnt loop counter
             * Advance vector source and destination pointers
             */
            pSrcA += 8;
            pSrcB += 8;
            blkCnt -= 8;
        }
    }
    *realResult = asrl(accReal, (14 - 8));
    *imagResult = asrl(accImag, (14 - 8));
#endif
}

#endif

/**
  @} end of cmplx_dot_prod group
 */
