/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_cmplx_dot_prod_q31.c
 * Description:  Q31 complex dot product
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
  @brief         Q31 complex dot product.
  @param[in]     pSrcA       points to the first input vector
  @param[in]     pSrcB       points to the second input vector
  @param[in]     numSamples  number of samples in each vector
  @param[out]    realResult  real part of the result returned here
  @param[out]    imagResult  imaginary part of the result returned here
  @return        none

  @par           Scaling and Overflow Behavior
                   The function is implemented using an internal 64-bit accumulator.
                   The intermediate 1.31 by 1.31 multiplications are performed with 64-bit precision and then shifted to 16.48 format.
                   The internal real and imaginary accumulators are in 16.48 format and provide 15 guard bits.
                   Additions are nonsaturating and no overflow will occur as long as <code>numSamples</code> is less than 32768.
                   The return results <code>realResult</code> and <code>imagResult</code> are in 16.48 format.
                   Input down scaling is not required.
 */

#if defined(ARM_MATH_MVEI)

void arm_cmplx_dot_prod_q31_mve(
  const q31_t * pSrcA,
  const q31_t * pSrcB,
        uint32_t numSamples,
        q63_t * realResult,
        q63_t * imagResult)
{
    q63_t           accReal = 0LL;
    q63_t           accImag = 0LL;

#ifdef USE_ASM
    uint32_t  tail = numSamples & 3;

    __asm volatile (
        /* pre-load */
        "   vldrw.32            q0, [%[pA]], 16                     \n"
        "   vldrw.32            q1, [%[pB]], 16                     \n"

        /*
         * Complex vector dot-product loop
         * Process 4 complex elements / iteration
         */
        ".p2align 2                                                 \n"
        "   wls                 lr, %[cnt], 1f                      \n"
        /* low overhead loop start */
        "2:                                                         \n"
        "   vrmlsldavha.s32     %Q[accR], %R[accR], q0, q1          \n"
        "   vldrw.32            q2, [%[pA]], 16                     \n"
        "   vrmlaldavhax.s32    %Q[accI], %R[accI], q0, q1          \n"
        "   vldrw.32            q3, [%[pB]], 16                     \n"
        "   vrmlsldavha.s32     %Q[accR], %R[accR], q2, q3          \n"
        "   vldrw.32            q0, [%[pA]], 16                     \n"
        "   vrmlaldavhax.s32    %Q[accI], %R[accI], q2, q3          \n"
        "   vldrw.32            q1, [%[pB]], 16                     \n"
        "   le                  lr, 2b                              \n"
        /* low overhead loop end */
        "1:                                                         \n"

        /* tail handling */
        ".p2align 2                                                 \n"
        "   wlstp.32            lr, %[tail], 1f                     \n"
        "2:                                                         \n"
        "   vrmlsldavha.s32     %Q[accR], %R[accR], q0, q1          \n"
        "   vrmlaldavhax.s32    %Q[accI], %R[accI], q0, q1          \n"
        "   vldrw.32            q0, [%[pA]], 16                     \n"
        "   vldrw.32            q1, [%[pB]], 16                     \n"
        "   letp                lr, 2b                              \n"
        "1:                                                         \n"

        /* scale and store */
        "   asrl                %Q[accR], %R[accR], #(14 - 8)       \n"
        "   asrl                %Q[accI], %R[accI], #(14 - 8)       \n"
        "   strd                %Q[accR], %R[accR], [%[realResult]] \n"
        "   strd                %Q[accI], %R[accI], [%[imagResult]] \n"

        :[pA] "+r"(pSrcA),[pB] "+r"(pSrcB),
         [accR] "+r" (accReal), [accI] "+r" (accImag)
        :[cnt] "r"(numSamples / 4), [tail] "r" (CMPLX_DIM * tail),
        [realResult] "r"(realResult), [imagResult] "r"(imagResult)
        : "q0", "q1", "q2", "q3",
          "lr", "memory");
#else
    /* CMSIS DSP MVE intrinsic reference */

    int32_t         blkCnt;
    q31x4_t         vecSrcA, vecSrcB;
    q31x4_t         vecSrcC, vecSrcD;

    blkCnt = numSamples >> 2;
    blkCnt -= 1;
    if (blkCnt > 0) {
        /* should give more freedom to generate stall free code */
        vecSrcA = vld1q(pSrcA);
        vecSrcB = vld1q(pSrcB);
        pSrcA += 4;
        pSrcB += 4;

        while (blkCnt > 0) {

            accReal = vrmlsldavhaq(accReal, vecSrcA, vecSrcB);
            vecSrcC = vld1q(pSrcA);
            pSrcA += 4;

            accImag = vrmlaldavhaxq(accImag, vecSrcA, vecSrcB);
            vecSrcD = vld1q(pSrcB);
            pSrcB += 4;

            accReal = vrmlsldavhaq(accReal, vecSrcC, vecSrcD);
            vecSrcA = vld1q(pSrcA);
            pSrcA += 4;

            accImag = vrmlaldavhaxq(accImag, vecSrcC, vecSrcD);
            vecSrcB = vld1q(pSrcB);
            pSrcB += 4;
            /*
             * Decrement the blockSize loop counter
             */
            blkCnt--;
        }

        /* process last elements out of the loop avoid the armclang breaking the SW pipeline */
        accReal = vrmlsldavhaq(accReal, vecSrcA, vecSrcB);
        vecSrcC = vld1q(pSrcA);

        accImag = vrmlaldavhaxq(accImag, vecSrcA, vecSrcB);
        vecSrcD = vld1q(pSrcB);

        accReal = vrmlsldavhaq(accReal, vecSrcC, vecSrcD);
        vecSrcA = vld1q(pSrcA);

        accImag = vrmlaldavhaxq(accImag, vecSrcC, vecSrcD);
        vecSrcB = vld1q(pSrcB);

        /*
         * tail
         */
        blkCnt = CMPLX_DIM * (numSamples & 3);
        do {
            mve_pred16_t    p = vctp32q(blkCnt);

            pSrcA += 4;
            pSrcB += 4;

            vecSrcA = vldrwq_z_s32(pSrcA, p);
            vecSrcB = vldrwq_z_s32(pSrcB, p);

            accReal = vrmlsldavhaq_p(accReal, vecSrcA, vecSrcB, p);
            accImag = vrmlaldavhaxq_p(accImag, vecSrcA, vecSrcB, p);

            blkCnt -= 4;
        }
        while ((int32_t) blkCnt > 0);
    } else {
        blkCnt = numSamples * CMPLX_DIM;
        while (blkCnt > 0) {
            mve_pred16_t    p = vctp32q(blkCnt);

            vecSrcA = vldrwq_z_s32(pSrcA, p);
            vecSrcB = vldrwq_z_s32(pSrcB, p);

            accReal = vrmlsldavhaq_p(accReal, vecSrcA, vecSrcB, p);
            accImag = vrmlaldavhaxq_p(accImag, vecSrcA, vecSrcB, p);

            /*
             * Decrement the blkCnt loop counter
             * Advance vector source and destination pointers
             */
            pSrcA += 4;
            pSrcB += 4;
            blkCnt -= 4;
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
