/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_biquad_cascade_df1_q31.c
 * Description:  Processing function for the Q31 Biquad cascade DirectFormI(DF1) filter
 *               using Helium assembly kernels
 *               This version allows boosting DF1 Q.31 Biquad performance when using compilers having suboptimal
 *               Helium intrinsic code generation.

 * $Date:        Dec 2021
 * $Revision:    V1.0.0
 *
 * Target Processor: Cortex-M with Helium
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

#include "dsp/filtering_functions.h"

/**
  @ingroup groupFilters
 */

/**
  @addtogroup BiquadCascadeDF1
  @{
 */

/**
  @brief         Processing function for the Q31 Biquad cascade filter.
  @param[in]     S         points to an instance of the Q31 Biquad cascade structure
  @param[in]     pSrc      points to the block of input data
  @param[out]    pDst      points to the block of output data
  @param[in]     blockSize  number of samples to process
  @return        none

  @par           Scaling and Overflow Behavior
                   The function is implemented using an internal 64-bit accumulator.
                   The accumulator has a 2.62 format and maintains full precision of the intermediate multiplication results but provides only a single guard bit.
                   Thus, if the accumulator result overflows it wraps around rather than clip.
                   In order to avoid overflows completely the input signal must be scaled down by 2 bits and lie in the range [-0.25 +0.25).
                   After all 5 multiply-accumulates are performed, the 2.62 accumulator is shifted by <code>postShift</code> bits and the result truncated to
                   1.31 format by discarding the low 32 bits.
  @remark
                   Refer to \ref arm_biquad_cascade_df1_fast_q31() for a faster but less precise implementation of this filter.
 */
#if defined(ARM_MATH_MVEI) && !defined(ARM_MATH_AUTOVECTORIZE)

void arm_biquad_cascade_df1_q31_mve(
  const arm_biquad_casd_df1_inst_q31 * S,
  const q31_t * pSrc,
        q31_t * pDst,
        uint32_t blockSize)
{
    const q31_t    *pIn = pSrc;                 /*  input pointer initialization  */
    q31_t          *pOut = pDst;                /*  output pointer initialization */
    int             shift;
    uint32_t        stages = S->numStages;      /*  loop counters                 */
    int             postShift = S->postShift;
    q31_t          *pState = S->pState;         /*  pState pointer initialization */
    const q31_t    *pCoeffs = S->pCoeffs;       /*  coeff pointer initialization  */


    shift = -(postShift + 1 + 8);

#ifdef USE_ASM

    int32_t         scratch[5];

    scratch[0] = shift;
    scratch[1] = (uint32_t)pCoeffs;
    scratch[2] = (uint32_t)pState;
    scratch[3] = (blockSize & 3);           /*loop remainder */
    scratch[4] = ((blockSize - 2) >> 2U);   /*loop count */

    /*
     * prologue consumes 3 history samples
     * inner loop process 4 x samples
     * loopRemainder is the amount of remaining samples
     */

    do {
        int64_t      acc0, acc1, acc2, acc3;


        /* 2 first elements are garbage but will be replaced with history */
        pIn -= 2;

        __asm volatile (
            " b0Coeffs         .req q2                                                \n"
            " b1Coeffs         .req q3                                                \n"
            " a0Coeffs         .req q4                                                \n"
            " a1Coeffs         .req q5                                                \n"

            /* generation of :
            * b0Coeffs { b2  b1  b0  0  }
            * b1Coeffs { 0   b2  b1  b0 }
            * a0Coeffs { 0   0   a2  a1 }
            * a1Coeffs { 0   0   a1  a2 }
            * (to be moved in init)
            */
            "   vmov.i32               b0Coeffs, #0                                   \n"
            /* coef ptr */
            "   ldr                    r8, [%[scratch], #(1*4)]                       \n"
            "   vmov.i32               a0Coeffs, #0                                   \n"
            /* load B0 */
            "   ldr                    r9, [r8], #4                                   \n"
            "   vmov.s32               b0Coeffs[2], r9                                \n"
            /* load B1 */
            "   ldr                    r9, [r8], #4                                   \n"
            "   vmov.s32               b0Coeffs[1], r9                                \n"
            /* load B2 */
            "   ldr                    r9, [r8], #4                                   \n"
            "   vmov.s32               b0Coeffs[0], r9                                \n"

            "   vmov                   b1Coeffs, b0Coeffs                             \n"
            "   mov                    r9, #0                                         \n"
            "   vshlc                  b1Coeffs, r9, #32                              \n"


            /* load A0 */
            "  ldr                     r9, [r8], #4                                   \n"
            "  vmov.s32                a0Coeffs[3], r9                                \n"
            /* load A1 */
            "  ldr                     r9, [r8], #4                                   \n"
            "  vmov.s32                a0Coeffs[2], r9                                \n"

            /* update coef ptr */
            "  str                     r8, [%[scratch], #(1*4)]                       \n"
            "  vrev64.32               a1Coeffs, a0Coeffs                             \n"


            " pstate         .req r8                                                  \n"
            " shift          .req r9                                                  \n"


            /* state pointer */
            "   ldr                    pstate, [%[scratch], #(2*4)]                   \n"
            /* shift */
            "   ldr                    shift, [%[scratch], #(0*4)]                    \n"
            /* loop count */
            "   ldr                    lr, [%[scratch], #(4*4)]                       \n"
            /* handle history */
            "   vmov.i32               q1, #0                                         \n"
            /* load last 2 samples */
            "   ldrd                   %Q[acc2], %R[acc2], [pstate]                   \n"
            /*
            * load input samples starting offset -2
            * 2 first elements are garbage, will be updated with history samples
            */
            "   vldrw.s32              q0, [%[pIn]], #8                               \n"
            "   vmov                   s0, s1, %R[acc2], %Q[acc2]                     \n"
            /* load last 2 outputs */
            "   ldrd                   %Q[acc2], %R[acc2], [pstate, #8]               \n"
            "   vmov                   s6, s7, %R[acc2], %Q[acc2]                     \n"

            "   vrmlaldavh.s32         %Q[acc2], %R[acc2], b0Coeffs, q0               \n"
            "   vrmlaldavha.s32        %Q[acc2], %R[acc2], a0Coeffs, q1               \n"
            "   sqrshrl                %Q[acc2], %R[acc2], #64, shift                 \n"
            "   vmov.s32               q1[2],   %R[acc2]                              \n"
            "   vrmlaldavh.s32         %Q[acc3], %R[acc3], b1Coeffs, q0               \n"
            "   vrmlaldavha.s32        %Q[acc3], %R[acc3], a1Coeffs, q1               \n"
            "   sqrshrl                %Q[acc3], %R[acc3], #64, shift                 \n"

            "   vldrw.s32              q0, [%[pIn]], #8                               \n"

            /*
             * Core loop (4 samples / loop)
             */

            ".p2align 2                                                               \n"
            /* low overhead loop start */
            "   wls                    lr, lr, 1f                                     \n"
            "2:                                                                       \n"
            /* { 0   0   yn2 yn1 } * { 0   0   a2  a1 } */
            "   vrmlaldavh.s32         %Q[acc0], %R[acc0], b0Coeffs, q0               \n"
            "   vmov.s32               q1[3],   %R[acc3]                              \n"
            /* { x0  x1  x2  x3 } * { b2  b1  b0  0 } */
            "   vrmlaldavha.s32        %Q[acc0], %R[acc0], a0Coeffs, q1               \n"
            /* store 2 outputs */
            "   strd                   %R[acc2], %R[acc3], [%[pOut]], #8              \n"

            "   sqrshrl                %Q[acc0], %R[acc0], #64, shift                 \n"

            /* { 0   0   yn  yn1 } * { 0   0   a1  a2 } */
            "   vrmlaldavh.s32         %Q[acc1], %R[acc1], b1Coeffs, q0               \n"
            "   vmov.s32               q1[2],   %R[acc0]                              \n"
            /* { x0  x1  x2  x3 } * { 0  b2  b1  b0   } */
            "   vrmlaldavha.s32        %Q[acc1], %R[acc1], a1Coeffs, q1               \n"

            /* load 2 next samples */
            "   vldrw.s32              q0, [%[pIn]], #8                               \n"
            "   sqrshrl                %Q[acc1], %R[acc1], #64, shift                 \n"
            "   vrmlaldavh.s32         %Q[acc2], %R[acc2], b0Coeffs, q0               \n"
            "   vmov.s32               q1[3],   %R[acc1]                              \n"
            "   vrmlaldavha.s32        %Q[acc2], %R[acc2], a0Coeffs, q1               \n"
            "   strd                   %R[acc0], %R[acc1], [%[pOut]], #8              \n"
            "   sqrshrl                %Q[acc2], %R[acc2], #64, shift                 \n"

            "   vrmlaldavh.s32         %Q[acc3], %R[acc3], b1Coeffs, q0               \n"
            "   vmov.s32               q1[2],  %R[acc2]                               \n"
            "   vrmlaldavha.s32        %Q[acc3], %R[acc3], a1Coeffs, q1               \n"
            "   vldrw.s32              q0, [%[pIn]], #8                               \n"
            "   sqrshrl                %Q[acc3], %R[acc3], #64, shift                 \n"

            /* low overhead loop end */
            "   le                     lr, 2b                                         \n"
            "1:                                                                       \n"

            "   strd                   %R[acc2], %R[acc3], [%[pOut]], #8              \n"

            /* Tail handling */
            /* load loop remainder */
            "   ldr                    lr, [%[scratch], #(3*4)]                       \n"

            "   cmp                    lr, #0                                         \n"
            "   beq                    rem0_%=                                        \n"
            "   cmp                    lr, #1                                         \n"
            "   beq                    rem1_%=                                        \n"
            "   cmp                    lr, #2                                         \n"
            "   beq                    rem2_%=                                        \n"

            /* 1 sample left */
            "   vrmlaldavh.s32         %Q[acc0], %R[acc0], b0Coeffs, q0               \n"
            "   vmov.s32               q1[3],   %R[acc3]                              \n"
            "   vrmlaldavha.s32        %Q[acc0], %R[acc0], a0Coeffs, q1               \n"
            "   sqrshrl                %Q[acc0], %R[acc0], #64, shift                 \n"
            "   str                    %R[acc0], [%[pOut]], #4                        \n"
            /* Store the updated state variables back into the pState array */
            "  vstr                    s2, [pstate]                                   \n"
            "  vstr                    s1, [pstate, #4]                               \n"
            "  strd                    %R[acc0], %R[acc3], [pstate, #8]               \n"
            "   b                      cont_%=                                        \n"

            /* 2 samples left */
            "rem2_%=:                                                                 \n"
            /* Store the updated state variables back into the pState array */
            "  vstr                    s1, [pstate]                                   \n"
            "  vstr                    s0, [pstate, #4]                               \n"
            "  strd                    %R[acc3], %R[acc2], [pstate, #8]               \n"
            "   b                      cont_%=                                        \n"

            /* 3 samples left */
            "rem1_%=:                                                                 \n"
            "   vrmlaldavh.s32         %Q[acc0], %R[acc0], b0Coeffs, q0               \n"
            "   vmov.s32               q1[3],   %R[acc3]                              \n"
            "   vrmlaldavha.s32        %Q[acc0], %R[acc0], a0Coeffs, q1               \n"
            "   sqrshrl                %Q[acc0], %R[acc0], #64, shift                 \n"
            "   vrmlaldavh.s32         %Q[acc1], %R[acc1], b1Coeffs, q0               \n"
            "   vmov.s32               q1[2],   %R[acc0]                              \n"
            "   vrmlaldavha.s32        %Q[acc1], %R[acc1], a1Coeffs, q1               \n"
            "   vldrw.s32              q0, [%[pIn]], #8                               \n"
            "   sqrshrl                %Q[acc1], %R[acc1], #64, shift                 \n"
            "   vrmlaldavh.s32         %Q[acc2], %R[acc2], b0Coeffs, q0               \n"
            "   vmov.s32               q1[3],   %R[acc1]                              \n"
            "   vrmlaldavha.s32        %Q[acc2], %R[acc2], a0Coeffs, q1               \n"
            "   strd                   %R[acc0], %R[acc1], [%[pOut]], #8              \n"
            "   sqrshrl                %Q[acc2], %R[acc2], #64, shift                 \n"
            "   str                    %R[acc2], [%[pOut]], #4                        \n"

            /* Store the updated state variables back into the pState array */
            "  vstr                    s2, [pstate]                                   \n"
            "  vstr                    s1, [pstate, #4]                               \n"
            "  strd                    %R[acc2], %R[acc1], [pstate, #8]               \n"
            "   b                      cont_%=                                        \n"

            "rem0_%=:                                                                 \n"

            "   vrmlaldavh.s32         %Q[acc0], %R[acc0], b0Coeffs, q0               \n"
            "   vmov.s32               q1[3],   %R[acc3]                              \n"
            "   vrmlaldavha.s32        %Q[acc0], %R[acc0], a0Coeffs, q1               \n"
            "   sqrshrl                %Q[acc0], %R[acc0], #64, shift                 \n"
            "   vrmlaldavh.s32         %Q[acc1], %R[acc1], b1Coeffs, q0               \n"
            "   vmov.s32               q1[2],   %R[acc0]                              \n"
            "   vrmlaldavha.s32        %Q[acc1], %R[acc1], a1Coeffs, q1               \n"
            "   sqrshrl                %Q[acc1], %R[acc1], #64, shift                 \n"
            "   strd                   %R[acc0], %R[acc1], [%[pOut]], #8              \n"

            /* Store the updated state variables back into the pState array */
            "  vstr                    s3, [pstate]                                   \n"
            "  vstr                    s2, [pstate, #4]                               \n"
            "  strd                    %R[acc1], %R[acc0], [pstate, #8]               \n"

            "cont_%=:                                                                 \n"
            "  add                     pstate, pstate, #(4*4)                         \n"
            "  str                     pstate, [%[scratch], #(2*4)]                   \n"

             :[pIn] "+r"(pIn), [pOut] "+r"(pOut),
              [acc0] "=&r"(acc0),[acc1] "=&r"(acc1),
              [acc2] "=&r"(acc2),[acc3] "=&r"(acc3)
             :[scratch] "r" (scratch)
             :"q0", "q1", "q2", "q3", "q4", "q5",
              "cc", "memory",
              "r8", "r9", "r14"
            );

        /*  The first stage goes from the input buffer to the output buffer. */
        /*  Subsequent stages occur in-place in the output buffer */
        pIn = pDst;

        /* Reset to destination pointer */
        pOut = pDst;
    }
    while (--stages);

#else
    /* CMSIS DSP intrinsic reference */
    q31x4_t         b0Coeffs, b1Coeffs, a0Coeffs, a1Coeffs;     /*  Coefficients vector           */

    do {
        q31x4_t         stateVec;
        q31x4_t         inVec0;
        int64_t         acc;
        q31_t           out, out1;

        /*
         * Reading the coefficients
         * generates :
         * Fwd0 { b2  b1  b0  0  }
         * Fwd1 { 0   b2  b1  b0 }
         * Bwd0 { 0   0   a2  a1 }
         * Bwd0 { 0   0   a1  a2 }
         * (can be moved in init)
         */
        b0Coeffs = vdupq_n_s32(0);
        a0Coeffs = vdupq_n_s32(0);

        b0Coeffs[0] = pCoeffs[2];       // b2
        b0Coeffs[1] = pCoeffs[1];       // b1
        b0Coeffs[2] = pCoeffs[0];       // b0

        b1Coeffs = b0Coeffs;
        uint32_t        zero = 0;
        b1Coeffs = vshlcq_s32(b1Coeffs, &zero, 32);

        a0Coeffs[2] = pCoeffs[4];
        a0Coeffs[3] = pCoeffs[3];
        a1Coeffs = vrev64q_s32(a0Coeffs);


        /*
         * prologue consumes history samples
         */

        /* 2 first elements are garbage, will be updated with history */
        inVec0 = vld1q(pIn - 2);

        inVec0[0] = pState[1];
        inVec0[1] = pState[0];

        stateVec[2] = pState[3];
        stateVec[3] = pState[2];

        acc = vrmlaldavhq(b0Coeffs, inVec0);
        acc = vrmlaldavhaq(acc, a0Coeffs, stateVec);
        acc = sqrshrl(acc, shift);
        out = (q31_t) ((acc >> 32) & 0xffffffff);

        stateVec[2] = out;
        acc = vrmlaldavhq(b1Coeffs, inVec0);
        acc = vrmlaldavhaq(acc, a1Coeffs, stateVec);

        acc = sqrshrl(acc, shift);
        out1 = (q31_t) ((acc >> 32) & 0xffffffff);


        inVec0 = vld1q(pIn);
        pIn += 2;

        /*
         * main loop
         */
        uint32_t            sample = (blockSize - 2) >> 2U;
        /*
         * First part of the processing with loop unrolling.
         * Compute 4 outputs at a time.
         */
        while (sample > 0U) {

            stateVec[3] = out1;

            *pOut++ = out;
            *pOut++ = out1;

            /*
             * in         { x0  x1  x2  x3 }
             *                    x
             * b0Coeffs   { b2  b1  b0  0  }
             */
            acc = vrmlaldavhq(b0Coeffs, inVec0);
            /*
             * out         { 0   0   yn2 yn1 }
             *                    x
             * a0Coeffs    { 0   0   a2  a1  }
             */
            acc = vrmlaldavhaq(acc, a0Coeffs, stateVec);
            acc = sqrshrl(acc, shift);
            out = (q31_t) ((acc >> 32) & 0xffffffff);

            stateVec[2] = out;

            /*
             * in         { x0  x1  x2  x3 }
             *                    x
             * b0Coeffs   {  0  b2  b1  b0 }
             */
            acc = vrmlaldavhq(b1Coeffs, inVec0);
            /*
             * out         { 0   0   y0  yn1 }
             *                    x
             * a0Coeffs    { 0   0   a1  a2  }
             */
            acc = vrmlaldavhaq(acc, a1Coeffs, stateVec);
            acc = sqrshrl(acc, shift);
            out1 = (q31_t) ((acc >> 32) & 0xffffffff);

            stateVec[3] = out1;

            inVec0 = vld1q(pIn);
            pIn += 2;

            /* unrolled part */
            *pOut++ = out;
            *pOut++ = out1;

            acc = vrmlaldavhq(b0Coeffs, inVec0);
            acc = vrmlaldavhaq(acc, a0Coeffs, stateVec);
            acc = sqrshrl(acc, shift);
            out = (q31_t) ((acc >> 32) & 0xffffffff);

            stateVec[2] = out;

            acc = vrmlaldavhq(b1Coeffs, inVec0);
            acc = vrmlaldavhaq(acc, a1Coeffs, stateVec);
            acc = sqrshrl(acc, shift);
            out1 = (q31_t) ((acc >> 32) & 0xffffffff);

            inVec0 = vld1q(pIn);
            pIn += 2;

            sample--;
        }

        *pOut++ = out;
        *pOut++ = out1;

        /*
         * Tail handling
         */
        int32_t         loopRemainder = blockSize & 3;
        if (loopRemainder == 2) {
            /*
             * Store the updated state variables back into the pState array
             */
            pState[0] = inVec0[1];
            pState[1] = inVec0[0];
            pState[3] = out;
            pState[2] = out1;
        } else if (loopRemainder == 1) {
            stateVec[3] = out1;

            acc = vrmlaldavhq(b0Coeffs, inVec0);
            acc = vrmlaldavhaq(acc, a0Coeffs, stateVec);
            acc = sqrshrl(acc, shift);
            out = (q31_t) ((acc >> 32) & 0xffffffff);

            stateVec[2] = out;

            acc = vrmlaldavhq(b1Coeffs, inVec0);
            acc = vrmlaldavhaq(acc, a1Coeffs, stateVec);
            acc = sqrshrl(acc, shift);
            out1 = (q31_t) ((acc >> 32) & 0xffffffff);

            stateVec[3] = out1;

            inVec0 = vld1q(pIn);
            pIn += 2;

            *pOut++ = out;
            *pOut++ = out1;

            acc = vrmlaldavhq(b0Coeffs, inVec0);
            acc = vrmlaldavhaq(acc, a0Coeffs, stateVec);
            acc = sqrshrl(acc, shift);
            out = (q31_t) ((acc >> 32) & 0xffffffff);

            *pOut++ = out;

            /*
             * Store the updated state variables back into the pState array
             */
            pState[0] = inVec0[2];
            pState[1] = inVec0[1];
            pState[3] = out1;
            pState[2] = out;
        } else if (loopRemainder == 0) {
            stateVec[3] = out1;

            acc = vrmlaldavhq(b0Coeffs, inVec0);
            acc = vrmlaldavhaq(acc, a0Coeffs, stateVec);
            acc = sqrshrl(acc, shift);
            out = (q31_t) ((acc >> 32) & 0xffffffff);

            stateVec[2] = out;

            acc = vrmlaldavhq(b1Coeffs, inVec0);
            acc = vrmlaldavhaq(acc, a1Coeffs, stateVec);
            acc = sqrshrl(acc, shift);
            out1 = (q31_t) ((acc >> 32) & 0xffffffff);

            *pOut++ = out;
            *pOut++ = out1;

            /*
             * Store the updated state variables back into the pState array
             */
            pState[0] = inVec0[3];
            pState[1] = inVec0[2];
            pState[3] = out;
            pState[2] = out1;
        } else {
            stateVec[3] = out1;

            acc = vrmlaldavhq(b0Coeffs, inVec0);
            acc = vrmlaldavhaq(acc, a0Coeffs, stateVec);
            acc = sqrshrl(acc, shift);
            out = (q31_t) ((acc >> 32) & 0xffffffff);

            *pOut++ = out;

            /*
             * Store the updated state variables back into the pState array
             */
            pState[0] = inVec0[2];
            pState[1] = inVec0[1];
            pState[3] = out1;
            pState[2] = out;
        }


        pCoeffs += 5;
        pState += 4;

        /*  The first stage goes from the input buffer to the output buffer. */
        /*  Subsequent stages occur in-place in the output buffer */
        pIn = pDst;

        /* Reset to destination pointer */
        pOut = pDst;
    }
    while (--stages);
#endif
}
#endif
/**
  @} end of BiquadCascadeDF1 group
 */
