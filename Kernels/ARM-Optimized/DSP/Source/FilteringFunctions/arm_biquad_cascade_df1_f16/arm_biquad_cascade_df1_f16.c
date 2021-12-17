/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_biquad_cascade_df1_f16.c
 * Description:  Processing function for the floating-point Biquad cascade DirectFormI(DF1) filter
 *               using Helium assembly kernels
 *               This version allows boosting DF1 F16 Biquad performance when using compilers having suboptimal
 *               Helium intrinsic code generation.

 * $Date:        Dec 2021
 * $Revision:    V1.0.0
 *
 * Target Processor: Cortex-M and Cortex-A cores
 * -------------------------------------------------------------------- */
/*
 * Copyright (C) 2010-2021 ARM Limited or its affiliates. All rights reserved.
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

#include "dsp/filtering_functions_f16.h"

#if defined(ARM_FLOAT16_SUPPORTED)
/**
  @ingroup groupFilters
 */


/**
  @addtogroup BiquadCascadeDF1
  @{
 */

/**
  @brief         Processing function for the floating-point Biquad cascade filter.
  @param[in]     S         points to an instance of the floating-point Biquad cascade structure
  @param[in]     pSrc      points to the block of input data
  @param[out]    pDst      points to the block of output data
  @param[in]     blockSize  number of samples to process.
  @return        none
 */
#if defined(ARM_MATH_MVE_FLOAT16) && !defined(ARM_MATH_AUTOVECTORIZE)

#include "arm_helium_utils.h"

void arm_biquad_cascade_df1_f16_mve(
  const arm_biquad_casd_df1_inst_f16 * S,
  const float16_t * pSrc,
        float16_t * pDst,
        uint32_t blockSize)
{
    float16_t *pIn = (float16_t *)pSrc;     /*  source pointer            */
    float16_t *pOut = pDst;                 /*  destination pointer       */
    float16_t *pState = S->pState;          /*  pState pointer            */
    const float16_t *pCoeffs = S->pCoeffs;  /*  coefficient pointer       */
    uint32_t  stage = S->numStages;         /*  loop counters             */

#ifdef USE_ASM

    #pragma nounroll
    do
    {
        __asm volatile (
            "Xn1                       .req r4                      \n"
            "Xn2                       .req r5                      \n"
            "Yn1                       .req r6                      \n"
            "Yn2                       .req r7                      \n"


            /* restore state for current biquad */
            "   ldrh                Xn1, [%[state]]                 \n"
            "   ldrh                Xn2, [%[state],2]               \n"
            "   ldrh                Yn1, [%[state],4]               \n"
            "   ldrh                Yn2, [%[state],6]               \n"


            /* load input sample N-2 */
            "   sub                 r9, %[cnt], #2                  \n"
            "   ldrh                r8, [%[src], r9, LSL #1]        \n"
            /* load input sample N-1 */
            "   sub                 r9, %[cnt], #1                  \n"
            "   ldrh                r10, [%[src], r9, LSL #1]       \n"
            /* save fwd state */
            "   strh                r10, [%[state]], #2             \n"
            "   strh                r8, [%[state]], #2              \n"


            /* main biquad loop */
            /* 1st coef preloaded to avoid structural hazard  */
            /* after input samples loading */
            ".p2align 2                                             \n"
            "   wlstp.16            lr, %[cnt], 1f                 \n"
            "   vldrh.16            q2, [%[coefs], 16]              \n"
            "2:                                                     \n"
            /* load 4 x packed inputs                   */
            /* r8 = (Xn|Xn+1) r9 = (Xn+2|Xn+3) etc...   */
            "   ldrd                r10, r11, [%[src], #8]          \n"
            "   ldrd                r8, r9,   [%[src]], #16         \n"
            /* start consuming half low parts (Xn, Xn+2, Xn+4, Xn+6) */

            "   vmul.f16            q0, q2, r11                     \n"
            "   vldrh.16            q1, [%[coefs], 48]              \n"
            "   vfma.f16            q0, q1, r10                     \n"
            "   vldrh.16            q1, [%[coefs], 80]              \n"
            "   vfma.f16            q0, q1, r9                      \n"
            "   vldrh.16            q1, [%[coefs], 112]             \n"
            "   vfma.f16            q0, q1, r8                      \n"
            /* consumes Xn-2 */
            "   vldrh.16            q1, [%[coefs], 144]             \n"
            "   vfma.f16            q0, q1, Xn2                     \n"

            /* move Xn+1, Xn+3, Xn+5, Xn+7 in half low position */
            /* by (dual) shifting r8-r11 */
            "   lsrl                r8, r9, #16                     \n"
            /* update Xn-2 */
            "   mov                 Xn2, r11                        \n"
            "   lsrl                r10, r11, #16                   \n"

            "   vldrh.16            q1, [%[coefs], 0]               \n"
            "   vfma.f16            q0, q1, r11                     \n"
            "   vldrh.16            q1, [%[coefs], 32]              \n"
            "   vfma.f16            q0, q1, r10                     \n"
            "   vldrh.16            q1, [%[coefs], 64]              \n"
            "   vfma.f16            q0, q1, r9                      \n"
            "   vldrh.16            q1, [%[coefs], 96]              \n"
            "   vfma.f16            q0, q1, r8                      \n"
            /* consumes Xn-1 */
            "   vldrh.16            q1, [%[coefs], 128]             \n"
            "   vfma.f16            q0, q1, Xn1                     \n"
            /* update Xn-2 */
            "   mov                 Xn1, r11                        \n"
            /* recursive parts */
            "   vldrh.16            q1, [%[coefs], 160]             \n"
            "   vfma.f16            q0, q1, Yn1                     \n"
            "   vldrh.16            q1, [%[coefs], 176]             \n"
            "   vfma.f16            q0, q1, Yn2                     \n"
            /* store 8 outputs */
            "   vstrh.16            q0, [%[dst]], #16               \n"
            /* update Yn-1, Yn-2 */
            "   vmov.u16            Yn2, q0[6]                      \n"
            "   vmov.u16            Yn1, q0[7]                      \n"
            "   letp                lr, 2b                          \n"
            "1:                                                     \n"

            /* tail handling (modulo 8) */
            "   and                 r8, %[cnt], #7                  \n"
            "   tbb                 [pc, R8]                        \n"
            /* -----------------------------------------------------*/
            /* branch table for 0-7 case remainders */
            "brTab:                                                 \n"
            ".byte      ((rem0 - brTab)/2), ((rem1 - brTab)/2)      \n"
            ".byte      ((rem2 - brTab)/2), ((rem3 - brTab)/2)      \n"
            ".byte      ((rem4 - brTab)/2), ((rem5 - brTab)/2)      \n"
            ".byte      ((rem6 - brTab)/2), ((rem7 - brTab)/2)      \n"
            /* -----------------------------------------------------*/

            "rem1:                                                  \n"
            /* save {Yn1, q0[0]} 16-bit vector elts */
            "   vstr.16             s0, [%[state]]                  \n"
            "   add                 %[state], #2                    \n"
            "   strh                Yn1, [%[state]], #2             \n"
            "   b                   cont                            \n"

            "rem2:                                                  \n"
            /* save {q0[0], q0[1]} 16-bit vector elts */
            "   vmov.u32            r8, q0[0]                       \n"
            "   ror                 r8, r8, #16                     \n"
            "   str                 r8, [%[state]], #4              \n"
            "   b                   cont                            \n"

            "rem3:                                                  \n"
            /* save {q0[1], q0[2]} 16-bit vector elts */
            "   vmov.u16            r9, q0[2]                       \n"
            "   vmov.u16            r8, q0[1]                       \n"
            "   strh                r9, [%[state]], #2              \n"
            "   strh                r8, [%[state]], #2              \n"
            "   b                   cont                            \n"

            "rem4 :                                                 \n"
            /* save {q0[2], q0[3]} 16-bit vector elts */
            "   vmov.u32            r8, q0[1]                       \n"
            "   ror                 r8, r8, #16                     \n"
            "   str                 r8, [%[state]], #4              \n"
            "   b                   cont                            \n"

            "rem5:                                                  \n"
            /* save {q0[3], q0[4]} 16-bit vector elts */
            "   vmov.u16            r9, q0[4]                       \n"
            "   vmov.u16            r8, q0[3]                       \n"
            "   strh                r9, [%[state]], #2              \n"
            "   strh                r8, [%[state]], #2              \n"
            "   b                   cont                            \n"

            "rem6 :                                                 \n"
            /* save {q0[4], q0[5]} 16-bit vector elts */
            "   vmov.u32            r8, q0[2]                       \n"
            "   ror                 r8, r8, #16                     \n"
            "   str                 r8, [%[state]], #4              \n"
            "   b                   cont                            \n"

            "rem7:                                                  \n"
            /* save {q0[5], q0[6]} 16-bit vector elts */
            "   vmov.u16            r9, q0[6]                       \n"
            "   vmov.u16            r8, q0[5]                       \n"
            "   strh                r9, [%[state]], #2              \n"
            "   strh                r8, [%[state]], #2              \n"
            "   b                   cont                            \n"

            "rem0 :                                                 \n"
            /* save state (blockSize multiple of 8)*/
            "   strh                Yn1, [%[state]], #2             \n"
            "   strh                Yn2, [%[state]], #2             \n"

            "cont:                                                  \n"

            :[state] "+r" (pState), [src] "+r"(pIn),[dst] "+r"(pOut)
            :[coefs] "r"(pCoeffs), [cnt] "r" (blockSize)
            :"q0", "q1", "q2",
             "r4", "r5", "r6", "r7",
             "r8", "r9", "r10", "r11",
             "lr", "memory", "cc");

        pCoeffs += sizeof(arm_biquad_mod_coef_f16) / sizeof(float16_t);

        // Stage 1 takes data from src->dst.  Subsequent stages go from dst->dst.
        pIn = pDst;
        pOut = pDst;
    }
    while (--stage);
#else
    float16_t Xn1, Xn2, Yn1, Yn2;   /*  Filter pState variables     */
    float16_t X0, X1, X2, X3;       /*  temporary input             */
    float16_t X4, X5, X6, X7;       /*  temporary input             */
    _Float16 lastX, lastY;          /*  X,Y history for tail handling */
    f16x8_t coeffs;
    f16x8_t accVec;                 /* accumulator vector            */
    uint32_t  sample;               /*  loop counters               */

    do
    {
        /*
         * Reading the pState values
         */
        Xn1 = pState[0];
        Xn2 = pState[1];
        Yn1 = pState[2];
        Yn2 = pState[3];

        sample = blockSize >> 3U;

        /*
         * First part of the processing with loop unrolling.  Compute 8 outputs at a time.
         */
        while (sample > 0U)
        {
            X0 = *pIn++;
            X1 = *pIn++;
            X2 = *pIn++;
            X3 = *pIn++;
            X4 = *pIn++;
            X5 = *pIn++;
            X6 = *pIn++;
            X7 = *pIn++;

            coeffs = vld1q(pCoeffs);
            accVec = vmulq(coeffs, X7);

            coeffs = vld1q(&pCoeffs[8]);
            accVec = vfmaq(accVec, coeffs, X6);

            coeffs = vld1q(&pCoeffs[16]);
            accVec = vfmaq(accVec, coeffs, X5);

            coeffs = vld1q(&pCoeffs[24]);
            accVec = vfmaq(accVec, coeffs, X4);

            coeffs = vld1q(&pCoeffs[32]);
            accVec = vfmaq(accVec, coeffs, X3);

            coeffs = vld1q(&pCoeffs[40]);
            accVec = vfmaq(accVec, coeffs, X2);

            coeffs = vld1q(&pCoeffs[48]);
            accVec = vfmaq(accVec, coeffs, X1);

            coeffs = vld1q(&pCoeffs[56]);
            accVec = vfmaq(accVec, coeffs, X0);

            coeffs = vld1q(&pCoeffs[64]);
            accVec = vfmaq(accVec, coeffs, Xn1);

            coeffs = vld1q(&pCoeffs[72]);
            accVec = vfmaq(accVec, coeffs, Xn2);

            coeffs = vld1q(&pCoeffs[80]);
            accVec = vfmaq(accVec, coeffs, Yn1);

            coeffs = vld1q(&pCoeffs[88]);
            accVec = vfmaq(accVec, coeffs, Yn2);
            /*
             * Store the result in the accumulator in the destination buffer.
             */
            vst1q(pOut, accVec);
            pOut += 8;

            /*
             * update recurrence
             */
            Xn1 = X7;
            Xn2 = X6;
            Yn1 = vgetq_lane(accVec, 7);
            Yn2 = vgetq_lane(accVec, 6);
            /*
             * decrement the loop counter
             */
            sample--;
        }

        /*
         * If the blockSize is not a multiple of 8,
         * compute any remaining output samples here.
         */
        sample = blockSize & 0x7U;
        if (sample)
        {
            /* save previous X, Y for modulo 1 length case */
            lastX = X7;
            lastY = Yn1;

            X0 = *pIn++;
            X1 = *pIn++;
            X2 = *pIn++;
            X3 = *pIn++;
            X4 = *pIn++;
            X5 = *pIn++;
            X6 = *pIn++;
            X7 = *pIn++;

            coeffs = vld1q(pCoeffs);
            accVec = vmulq(coeffs, X7);

            coeffs = vld1q(&pCoeffs[8]);
            accVec = vfmaq(accVec, coeffs, X6);

            coeffs = vld1q(&pCoeffs[16]);
            accVec = vfmaq(accVec, coeffs, X5);

            coeffs = vld1q(&pCoeffs[24]);
            accVec = vfmaq(accVec, coeffs, X4);

            coeffs = vld1q(&pCoeffs[32]);
            accVec = vfmaq(accVec, coeffs, X3);

            coeffs = vld1q(&pCoeffs[40]);
            accVec = vfmaq(accVec, coeffs, X2);

            coeffs = vld1q(&pCoeffs[48]);
            accVec = vfmaq(accVec, coeffs, X1);

            coeffs = vld1q(&pCoeffs[56]);
            accVec = vfmaq(accVec, coeffs, X0);

            coeffs = vld1q(&pCoeffs[64]);
            accVec = vfmaq(accVec, coeffs, Xn1);

            coeffs = vld1q(&pCoeffs[72]);
            accVec = vfmaq(accVec, coeffs, Xn2);

            coeffs = vld1q(&pCoeffs[80]);
            accVec = vfmaq(accVec, coeffs, Yn1);

            coeffs = vld1q(&pCoeffs[88]);
            accVec = vfmaq(accVec, coeffs, Yn2);

            switch(sample)
            {
               case 1:
                 *pOut++ = vgetq_lane(accVec, 0);
                  Xn1 = X0;
                  Xn2 = lastX;
                  Yn1 = vgetq_lane(accVec, 0);
                  Yn2 = lastY;
               break;
               case 2:
                 *pOut++ = vgetq_lane(accVec, 0);
                 *pOut++ = vgetq_lane(accVec, 1);
                 Xn1 = X1;
                 Xn2 = X0;
                 Yn1 = vgetq_lane(accVec, 1);
                 Yn2 = vgetq_lane(accVec, 0);
               break;
               case 3:
                *pOut++ = vgetq_lane(accVec, 0);
                *pOut++ = vgetq_lane(accVec, 1);
                *pOut++ = vgetq_lane(accVec, 2);
                Xn1 = X2;
                Xn2 = X1;
                Yn1 = vgetq_lane(accVec, 2);
                Yn2 = vgetq_lane(accVec, 1);
               break;

               case 4:
                *pOut++ = vgetq_lane(accVec, 0);
                *pOut++ = vgetq_lane(accVec, 1);
                *pOut++ = vgetq_lane(accVec, 2);
                *pOut++ = vgetq_lane(accVec, 3);
                Xn1 = X3;
                Xn2 = X2;
                Yn1 = vgetq_lane(accVec, 3);
                Yn2 = vgetq_lane(accVec, 2);
               break;

               case 5:
                *pOut++ = vgetq_lane(accVec, 0);
                *pOut++ = vgetq_lane(accVec, 1);
                *pOut++ = vgetq_lane(accVec, 2);
                *pOut++ = vgetq_lane(accVec, 3);
                *pOut++ = vgetq_lane(accVec, 4);
                Xn1 = X4;
                Xn2 = X3;
                Yn1 = vgetq_lane(accVec, 4);
                Yn2 = vgetq_lane(accVec, 3);
               break;

               case 6:
                *pOut++ = vgetq_lane(accVec, 0);
                *pOut++ = vgetq_lane(accVec, 1);
                *pOut++ = vgetq_lane(accVec, 2);
                *pOut++ = vgetq_lane(accVec, 3);
                *pOut++ = vgetq_lane(accVec, 4);
                *pOut++ = vgetq_lane(accVec, 5);
                Xn1 = X5;
                Xn2 = X4;
                Yn1 = vgetq_lane(accVec, 5);
                Yn2 = vgetq_lane(accVec, 4);
               break;

               case 7:
                *pOut++ = vgetq_lane(accVec, 0);
                *pOut++ = vgetq_lane(accVec, 1);
                *pOut++ = vgetq_lane(accVec, 2);
                *pOut++ = vgetq_lane(accVec, 3);
                *pOut++ = vgetq_lane(accVec, 4);
                *pOut++ = vgetq_lane(accVec, 5);
                *pOut++ = vgetq_lane(accVec, 6);
                Xn1 = X6;
                Xn2 = X5;
                Yn1 = vgetq_lane(accVec, 6);
                Yn2 = vgetq_lane(accVec, 5);
               break;
            }
        }
        /*
         * Store the updated state variables back into the pState array
         */
        *pState++ = Xn1;
        *pState++ = Xn2;
        *pState++ = Yn1;
        *pState++ = Yn2;

        pCoeffs += sizeof(arm_biquad_mod_coef_f16) / sizeof(float16_t);
        /*
         * The first stage goes from the input buffer to the output buffer.
         * Subsequent numStages  occur in-place in the output buffer
         */
        pIn = pDst;
        /*
         * Reset the output pointer
         */
        pOut = pDst;
        /*
         * decrement the loop counter
         */
        stage--;
    }
    while (stage > 0U);
#endif
}

#endif

#endif /*#if defined(ARM_FLOAT16_SUPPORTED)*/
