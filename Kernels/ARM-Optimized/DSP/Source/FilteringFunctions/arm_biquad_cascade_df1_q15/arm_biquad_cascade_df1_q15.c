/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_biquad_cascade_df1_q15.c
 * Description:  Processing function for the Q15 Biquad cascade DirectFormI(DF1) filter
 *               using Helium assembly kernels
 *               This version allows boosting DF1 Q.15 Biquad performance when using compilers having suboptimal
 *               Helium intrinsic code generation.

 * $Date:        Jan 2022
 * $Revision:    V1.0.1
 *
 * Target Processor: Cortex-M and Cortex-A cores
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

#include "dsp/filtering_functions.h"

/**
  @ingroup groupFilters
 */

/**
  @addtogroup BiquadCascadeDF1
  @{
 */

/**
  @brief         Processing function for the Q15 Biquad cascade filter.
  @param[in]     S         points to an instance of the Q15 Biquad cascade structure
  @param[in]     pSrc      points to the block of input data
  @param[out]    pDst      points to the location where the output result is written
  @param[in]     blockSize number of samples to process
  @return        none

  @par           Scaling and Overflow Behavior
                   The function is implemented using a 64-bit internal accumulator.
                   Both coefficients and state variables are represented in 1.15 format and multiplications yield a 2.30 result.
                   The 2.30 intermediate results are accumulated in a 64-bit accumulator in 34.30 format.
                   There is no risk of internal overflow with this approach and the full precision of intermediate multiplications is preserved.
                   The accumulator is then shifted by <code>postShift</code> bits to truncate the result to 1.15 format by discarding the low 16 bits.
                   Finally, the result is saturated to 1.15 format.
  @remark
                   Refer to \ref arm_biquad_cascade_df1_fast_q15() for a faster but less precise implementation of this filter.
 */

#if defined(ARM_MATH_MVEI) && !defined(ARM_MATH_AUTOVECTORIZE)

void arm_biquad_cascade_df1_q15_mve(
  const arm_biquad_casd_df1_inst_q15 * S,
  const q15_t * pSrc,
        q15_t * pDst,
        uint32_t blockSize)
{
    const q15_t    *pIn = pSrc;             /*  input pointer initialization  */
    q15_t          *pOut = pDst;            /*  output pointer initialization */
    int             shift;
    uint32_t        stages = S->numStages;  /*  loop counters                 */
    int             postShift = S->postShift;
    q15_t          *pState = S->pState;     /*  pState pointer initialization */
    const q15_t     *pCoeffs = S->pCoeffs;  /*  coeff pointer initialization  */

    shift = (15 - postShift) - 32;

#ifdef USE_ASM
    int32_t         scratch[5];

    scratch[0] = shift;
    scratch[1] = (uint32_t)pCoeffs;
    scratch[2] = (uint32_t)pState;

    uint32_t    loopCnt = ((blockSize - 4) >> 3U);
    scratch[3] = (blockSize - 4 - (loopCnt << 3));           /*loop remainder */
    scratch[4] = loopCnt;

   do {

        /* 2 first elements are garbage but will be replaced with history */
        pIn -= 2;


        __asm volatile (

            /* generation of :
             * Coeff0 {  b2  b1  b0  0   0   0   a2  a1 }
             * Coeff1 {  0   b2  b1  b0  0   0   a1  a2 }
             * Coeff2 {  0   0   b2  b1  b0  0   a2  a1 }
             * Coeff3 {  0   0   0   b2  b1  b0  a1  a2 }
             * (to be moved in init stage)
             */

            " coeff0         .req q2                                  \n"
            " coeff1         .req q3                                  \n"
            " coeff2         .req q4                                  \n"
            " coeff3         .req q5                                  \n"

            "   vmov.i16               coeff0, #0                     \n"
            /* coef ptr */
            "   ldr                    r8, [%[scratch], #(1*4)]       \n"

            /* load b0 */
            "   ldrh                   r9, [r8, #(0*2)]               \n"
            "   vmov.s16               coeff0[2], r9                  \n"
            /* load b1 */
            "   ldrh                   r9, [r8, #(2*2)]               \n"
            "   vmov.s16               coeff0[1], r9                  \n"
            /* load b2 */
            "   ldrh                   r9, [r8, #(3*2)]               \n"
            "   vmov.s16               coeff0[0], r9                  \n"

            "   vmov                   coeff1, coeff0                 \n"
            "   mov                    r9, #0                         \n"
            "   vshlc                  coeff1, r9, #16                \n"

            "   vmov                   coeff2, coeff1                 \n"
            "   mov                    r9, #0                         \n"
            "   vshlc                  coeff2, r9, #16                \n"

            "   vmov                   coeff3, coeff2                 \n"
            "   mov                    r9, #0                         \n"
            "   vshlc                  coeff3, r9, #16                \n"


            /* load a2 */
            "  ldrh                    r9, [r8, #(5*2)]               \n"
            "  vmov.s16                coeff0[6], r9                  \n"
            "  vmov.s16                coeff1[7], r9                  \n"
            "  vmov.s16                coeff2[6], r9                  \n"
            "  vmov.s16                coeff3[7], r9                  \n"

            /* load a1 */
            "  ldrh                    r9, [r8, #(4*2)]               \n"
            "  vmov.s16                coeff0[7], r9                  \n"
            "  vmov.s16                coeff1[6], r9                  \n"
            "  vmov.s16                coeff2[7], r9                  \n"
            "  vmov.s16                coeff3[6], r9                  \n"

            "   add                    r8, r8, #(6*2)                 \n"
            /* update coef ptr */
            "  str                     r8, [%[scratch], #(1*4)]       \n"

            " pstate         .req r2                                  \n"
            " shift          .req r1                                  \n"

            /* state pointer */
            "   ldr                    pstate, [%[scratch], #(2*4)]   \n"
            /* shift */
            "   ldr                    shift, [%[scratch], #(0*4)]    \n"
            /* loop count */
            "   ldr                    lr, [%[scratch], #(4*4)]       \n"


           /*
            * history processing
            */
            "   ldrh                   r0, [pstate, #2]               \n"
            "   vldrh.s16              q0, [%[pIn]], #8               \n"
            "   vmov.s16               q0[0], r0                      \n"
            "   ldrh                   r0, [pstate, #0]               \n"
            "   vmov.s16               q0[1], r0                      \n"
            "   ldrh                   r0, [pstate, #6]               \n"
            "   vmov.s16               q0[6], r0                      \n"
            "   ldrh                   r0, [pstate, #4]               \n"

            "   vmov.s16               q0[7], r0                      \n"
            "   vmlaldav.s16           r8, r9, q0, coeff0             \n"
            "   sqrshrl                r8, r9, #48, shift             \n"
            "   vmov.s16               q0[6], r9                      \n"
            "   vmlaldav.s16           r10, r11, q0, coeff1           \n"
            "   sqrshrl                r10, r11, #48, shift           \n"
            "   vmov.s16               q0[7], r11                     \n"
            "   vmlaldav.s16           r8, r9, q0, coeff2             \n"
            /* store 2 elts */
            "   vstmia                 %[pOut]!, {s3}                 \n"
            "   sqrshrl                r8, r9, #48, shift             \n"
            "   vmov.s16               q0[6], r9                      \n"
            "   vmlaldav.s16           r10, r11, q0, coeff3           \n"
            "   vldrh.s16              q0, [%[pIn]], #8               \n"
            "   vmov.s16               q0[6], r9                      \n"
            "   sqrshrl                r10, r11, #48, shift           \n"


            /*
             * Core loop (8 elements / loop)
             */
            ".p2align 2                                               \n"
            "   wls                    lr, lr, 1f                     \n"
            /* low overhead loop start */
            "2:                                                       \n"

            "   vmov.s16               q0[7], r11                     \n"
            "   vmlaldav.s16           r8, r9, q0, coeff0             \n"
            /* store 2 elts */
            "   vstmia                 %[pOut]!, {s3}                 \n"
            "   sqrshrl                r8, r9, #48, shift             \n"

            "   vmov.s16               q0[6], r9                      \n"
            "   vmlaldav.s16           r10, r11, q0, coeff1           \n"
            "   vldrh.s16              q1, [%[pIn]], #8               \n"
            "   sqrshrl                r10, r11, #48, shift           \n"
            "   vmov.s16               q0[7], r11                     \n"

            "   vmlaldav.s16           r8, r9, q0, coeff2             \n"
            /* store 2 elts */
            "   vstmia                 %[pOut]!, {s3}                 \n"
            "   sqrshrl                r8, r9, #48, shift             \n"

            "   vmov.s16               q0[6], r9                      \n"
            "   vmlaldav.s16           r10, r11, q0, coeff3           \n"

            "   vmov.s16               q1[6], r9                      \n"
            "   sqrshrl                r10, r11, #48, shift           \n"

            "   vmov.s16               q1[7], r11                     \n"
            "   vmlaldav.s16           r8, r9, q1, coeff0             \n"
            /* store 2 elts */
            "   vstmia                 %[pOut]!, {s7}                 \n"

            "   sqrshrl                r8, r9, #48, shift             \n"

            "   vmov.s16               q1[6], r9                      \n"
            "   vmlaldav.s16           r10, r11, q1, coeff1           \n"
            "   vldrh.s16              q0, [%[pIn]], #8               \n"
            "   sqrshrl                r10, r11, #48, shift           \n"
            "   vmov.s16               q1[7], r11                     \n"

            "   vmlaldav.s16           r8, r9, q1, coeff2             \n"
            /* store 2 elts */
            "   vstmia                 %[pOut]!, {s7}                 \n"
            "   sqrshrl                r8, r9, #48, shift             \n"

            "   vmov.s16               q1[6], r9                      \n"
            "   vmlaldav.s16           r10, r11, q1, coeff3           \n"

            "   vmov.s16               q0[6], r9                      \n"
            "   sqrshrl                r10, r11, #48, shift           \n"

            "   le                     lr, 2b                         \n"
            /* low overhead loop end */
            "1:                                                       \n"

            /* handle tail : 0-7 elts */
            /* load loop remainder */
            "   ldr                    r0, [%[scratch], #(3*4)]       \n"

            "   cmp                    r0, #4                         \n"
            "   blt                    remlt4_%=                      \n"

            /* handle last chunk of 4 samples */
            "   vmov.s16               q0[7], r11                     \n"
            "   vmlaldav.s16           r8, r9, q0, coeff0             \n"
            /* store 2 elts */
            "   vstmia                 %[pOut]!, {s3}                 \n"

            "   sqrshrl                r8, r9, #48, shift             \n"

            "   vmov.s16               q0[6], r9                      \n"
            "   vmlaldav.s16           r10, r11, q0, coeff1           \n"
            "   sqrshrl                r10, r11, #48, shift           \n"
            "   vmov.s16               q0[7], r11                     \n"

            "   vmlaldav.s16           r8, r9, q0, coeff2             \n"
            /* store 2 elts */
            "   vstmia                 %[pOut]!, {s3}                 \n"
            "   sqrshrl                r8, r9, #48, shift             \n"

            "   vmov.s16               q0[6], r9                      \n"
            "   vmlaldav.s16           r10, r11, q0, coeff3           \n"
            "   vldrh.s16              q0, [%[pIn]], #8               \n"
            "   vmov.s16               q0[6], r9                      \n"
            "   sqrshrl                r10, r11, #48, shift           \n"

            "   sub                    r0, #4                         \n"

            /* handle tail : 0-3 elts */

            "remlt4_%=: "
            /* store last 2 elts */
            "   strh                   r11, [%[pOut], #2]             \n"
            "   strh                   r9, [%[pOut], #0]              \n"

            /* Tail handling */
            "   cmp                    r0, #0                         \n"
            "   beq                    rem0_%=                        \n"
            "   cmp                    r0, #1                         \n"
            "   beq                    rem1_%=                        \n"
            "   cmp                    r0, #2                         \n"
            "   beq                    rem2_%=                        \n"

            /* 3 samples left to process */
            "   vmov.s16               q0[7], r11                     \n"
            "   vmlaldav.s16           r8, r9, q0, coeff0             \n"
            "   sqrshrl                r8, r9, #48, shift             \n"
            "   vmov.s16               q0[6], r9                      \n"
            "   vmlaldav.s16           r10, r11, q0, coeff1           \n"
            "   sqrshrl                r10, r11, #48, shift           \n"

            "   strh                   r9, [%[pOut], #4]              \n"
            "   strh                   r11, [%[pOut], #6]             \n"

            "   vmov.s16               q0[7], r11                     \n"
            "   vmlaldav.s16           r8, r9, q0, coeff2             \n"
            "   sqrshrl                r8, r9, #48, shift             \n"
            "   strh                   r9, [%[pOut], #8]              \n"
            /* state update */
            "   strh                   r11, [pstate, #6]              \n"
            "   strh                   r9, [pstate, #4]               \n"
            "   vmov.s16               r9, q0[4]                      \n"
            "   vmov.s16               r11, q0[3]                     \n"
            "   strh                   r9, [pstate, #0]               \n"
            "   strh                   r11, [pstate, #2]              \n"
            "   b                      cont_%=                        \n"

            "rem2_%=:                                                 \n"
            /* 2 sample left to process */
            "   vmov.s16              q0[7], r11                      \n"
            "   vmlaldav.s16          r8, r9, q0, coeff0              \n"
            "   sqrshrl               r8, r9, #48, shift              \n"
            "   vmov.s16              q0[6], r9                       \n"
            "   vmlaldav.s16          r10, r11, q0, coeff1            \n"
            "   sqrshrl               r10, r11, #48, shift            \n"

            "   strh                  r9, [%[pOut], #4]               \n"
            "   strh                  r11, [%[pOut], #6]              \n"
            /* state update */
            "   strh                  r11, [pstate, #4]               \n"
            "   strh                  r9, [pstate, #6]                \n"
            "   vmov.s16              r9, q0[3]                       \n"
            "   vmov.s16              r11, q0[2]                      \n"
            "   strh                  r9, [pstate, #0]                \n"
            "   strh                  r11, [pstate, #2]               \n"
            "   b                     cont_%=                         \n"

            "rem1_%=:                                                 \n"
            /* 1 sample left to process */
            "   vmov.s16              q0[7], r11                      \n"
            "   vmlaldav.s16          r8, r9, q0, coeff0              \n"
            "   sqrshrl               r8, r9, #48, shift              \n"

            "   strh                  r9, [%[pOut], #4]               \n"
            /* state update */
            "   strh                  r11, [pstate, #6]               \n"
            "   strh                  r9, [pstate, #4]                \n"
            "   vmov.s16              r9, q0[2]                       \n"
            "   vmov.s16              r11, q0[1]                      \n"
            "   strh                  r9, [pstate, #0]                \n"
            "   strh                  r11, [pstate, #2]               \n"
            "   b                     cont_%=                         \n"

            "rem0_%=:                                                 \n"
            /* all samples processed */
            "   strh                  r11, [pstate, #4]               \n"
            "   strh                  r9, [pstate, #6]                \n"
            "   vmov.s16              r9, q0[1]                       \n"
            "   vmov.s16              r11, q0[0]                      \n"
            "   strh                  r9, [pstate, #0]                \n"
            "   strh                  r11, [pstate, #2]               \n"

            "cont_%=:                                                 \n"
            /* increment & store state pointer */
            "  add                    pstate, pstate, #(4*2)          \n"
            "  str                    pstate, [%[scratch], #(2*4)]    \n"

            " .unreq pstate                                           \n"
            " .unreq shift                                            \n"
            " .unreq coeff0                                           \n"
            " .unreq coeff1                                           \n"
            " .unreq coeff2                                           \n"
            " .unreq coeff3                                           \n"

        : [pIn] "+r" (pIn), [pOut] "+r" (pOut)
        : [scratch] "r" (scratch)
        : "q0", "q1", "q2", "q3", "q4", "q5",
          "r0", "r1", "r2", "r8", "r9", "r10", "r11", "r14",
          "cc", "memory"
        );


        /*
         * The first stage goes from the input buffer to the output buffer.
         * Subsequent stages occur in-place in the output buffer
         */
        pIn = pDst;
        /*
         * Reset to destination pointer
         */
        pOut = pDst;
    }
    while (--stages);

#else
    /* CMSIS DSP MVE intrinsic reference */

    do {
        q15x8_t         bCoeffs0, bCoeffs1, bCoeffs2, bCoeffs3;     /*  Coefficients vector           */
        q15x8_t         inVec0;
        q31_t           out, out1;
        int64_t         acc;
        uint32_t        sample;

        q15_t           a2 = pCoeffs[5];
        q15_t           a1 = pCoeffs[4];

        bCoeffs0 = vdupq_n_s16(0);
        bCoeffs0[0] = pCoeffs[3];       // b2
        bCoeffs0[1] = pCoeffs[2];       // b1
        bCoeffs0[2] = pCoeffs[0];       // b0

        uint32_t        zero = 0;
        bCoeffs1 = bCoeffs0;
        bCoeffs1 = vshlcq_s16(bCoeffs1, &zero, 16);

        bCoeffs2 = bCoeffs1;
        bCoeffs2 = vshlcq_s16(bCoeffs2, &zero, 16);

        bCoeffs3 = bCoeffs2;
        bCoeffs3 = vshlcq_s16(bCoeffs3, &zero, 16);

        bCoeffs0[6] = a2;
        bCoeffs0[7] = a1;
        bCoeffs1[7] = a2;
        bCoeffs1[6] = a1;

        bCoeffs2 =
            vsetq_lane_s32(vgetq_lane_s32((q31x4_t) bCoeffs0, 3), (q31x4_t) bCoeffs2, 3);
        bCoeffs3 =
            vsetq_lane_s32(vgetq_lane_s32((q31x4_t) bCoeffs1, 3), (q31x4_t) bCoeffs3, 3);


        /* 2 first elements are garbage, will be updated with history */
        inVec0 = vld1q(pIn - 2);
        pIn += 2;

        inVec0[0] = pState[1];
        inVec0[1] = pState[0];
        inVec0[6] = pState[3];
        inVec0[7] = pState[2];

        acc = vmlaldavq(bCoeffs0, inVec0);
        acc = sqrshrl_sat48(acc, shift);
        out1 = (q31_t) ((acc >> 32) & 0xffffffff);

        inVec0[6] = out1;
        acc = vmlaldavq(bCoeffs1, inVec0);
        acc = sqrshrl_sat48(acc, shift);
        out = (q31_t) ((acc >> 32) & 0xffffffff);

        inVec0[7] = out;
        *pOut++ = (q15_t) out1;
        *pOut++ = (q15_t) out;

        acc = vmlaldavq(bCoeffs2, inVec0);
        acc = sqrshrl_sat48(acc, shift);
        out1 = (q31_t) ((acc >> 32) & 0xffffffff);


        inVec0[6] = out1;
        acc = vmlaldavq(bCoeffs3, inVec0);
        acc = sqrshrl_sat48(acc, shift);
        out = (q31_t) ((acc >> 32) & 0xffffffff);

        /*
         * main loop
         */
        sample = (blockSize - 4) >> 2U;
        /* preload (efficient scheduling) */
        inVec0 = vld1q(pIn);
        pIn += 4;

        /*
         * Compute 4 outputs at a time.
         */
        while (sample > 0U) {

            inVec0[6] = out1;
            inVec0[7] = out;

            /* store */
            *pOut++ = (q15_t) out1;
            *pOut++ = (q15_t) out;

            /*
             * in       { x0  x1  x2  x3  x4  x5  yn2  yn1 }
             *                          x
             * bCoeffs0 { b2  b1  b0   0   0   0   a2  a1 }
             *
             */
            acc = vmlaldavq(bCoeffs0, inVec0);
            /* shift + saturate to 16 bit */
            acc = sqrshrl_sat48(acc, shift);
            out1 = (q31_t) ((acc >> 32) & 0xffffffff);
            inVec0[6] = out1;

            /*
             * in       { x0  x1  x2  x3  x4  x5  y0  yn1 }
             *                          x
             * bCoeffs1 {  0  b2  b1  b0   0   0  a1  a2 }
             */
            acc = vmlaldavq(bCoeffs1, inVec0);
            acc = sqrshrl_sat48(acc, shift);
            out = (q31_t) ((acc >> 32) & 0xffffffff);

            *pOut++ = (q15_t) out1;
            *pOut++ = (q15_t) out;


            inVec0[7] = out;
            /*
             * in       { x0  x1  x2  x3  x4  x5  y0  yp1 }
             *                         x
             * bCoeffs2 { 0   0   b2  b1  b0   0  a2  a1 }
             */
            acc = vmlaldavq(bCoeffs2, inVec0);
            acc = sqrshrl_sat48(acc, shift);
            out1 = (q31_t) ((acc >> 32) & 0xffffffff);
            inVec0[6] = out1;

            /*
             * in       { x0  x1  x2  x3  x4  x5  y0  yp1 }
             *                         x
             * bCoeffs2 {  0   0   0  b2  b1  b0  a1  a2  }
             */
            acc = vmlaldavq(bCoeffs3, inVec0);
            acc = sqrshrl_sat48(acc, shift);
            out = (q31_t) ((acc >> 32) & 0xffffffff);

            inVec0 = vld1q(pIn);
            pIn += 4;

            /* decrement the loop counter  */
            sample--;
        }

        *pOut++ = (q15_t) out1;
        *pOut++ = (q15_t) out;

        /*
         * Tail handling
         */
        int32_t         loopRemainder = blockSize & 3;

        if (loopRemainder == 3) {
            inVec0[6] = out1;
            inVec0[7] = out;

            acc = vmlaldavq(bCoeffs0, inVec0);
            acc = sqrshrl_sat48(acc, shift);
            out1 = (q31_t) ((acc >> 32) & 0xffffffff);
            inVec0[6] = out1;

            acc = vmlaldavq(bCoeffs1, inVec0);
            acc = sqrshrl_sat48(acc, shift);
            out = (q31_t) ((acc >> 32) & 0xffffffff);

            *pOut++ = (q15_t) out1;
            *pOut++ = (q15_t) out;

            inVec0[7] = out;
            acc = vmlaldavq(bCoeffs2, inVec0);
            acc = sqrshrl_sat48(acc, shift);
            out1 = (q31_t) ((acc >> 32) & 0xffffffff);
            *pOut++ = (q15_t) out1;

            /* Store the updated state variables back into the pState array */
            pState[0] = vgetq_lane_s16(inVec0, 4);
            pState[1] = vgetq_lane_s16(inVec0, 3);
            pState[3] = out;
            pState[2] = out1;

        } else if (loopRemainder == 2) {
            inVec0[6] = out1;
            inVec0[7] = out;

            acc = vmlaldavq(bCoeffs0, inVec0);
            acc = sqrshrl_sat48(acc, shift);
            out1 = (q31_t) ((acc >> 32) & 0xffffffff);
            inVec0[6] = out1;

            acc = vmlaldavq(bCoeffs1, inVec0);
            acc = sqrshrl_sat48(acc, shift);
            out = (q31_t) ((acc >> 32) & 0xffffffff);

            *pOut++ = (q15_t) out1;
            *pOut++ = (q15_t) out;

            /* Store the updated state variables back into the pState array */
            pState[0] = vgetq_lane_s16(inVec0, 3);
            pState[1] = vgetq_lane_s16(inVec0, 2);
            pState[3] = out1;
            pState[2] = out;
        } else if (loopRemainder == 1) {

            inVec0[6] = out1;
            inVec0[7] = out;

            acc = vmlaldavq(bCoeffs0, inVec0);
            acc = sqrshrl_sat48(acc, shift);
            out1 = (q31_t) ((acc >> 32) & 0xffffffff);
            *pOut++ = (q15_t) out1;

            /* Store the updated state variables back into the pState array */
            pState[0] = vgetq_lane_s16(inVec0, 2);
            pState[1] = vgetq_lane_s16(inVec0, 1);
            pState[3] = out;
            pState[2] = out1;


        } else {
            /* Store the updated state variables back into the pState array */
            pState[0] = vgetq_lane_s16(inVec0, 1);
            pState[1] = vgetq_lane_s16(inVec0, 0);
            pState[3] = out1;
            pState[2] = out;
        }

        pState += 4;
        pCoeffs += 6;

        /*
         * The first stage goes from the input buffer to the output buffer.
         * Subsequent stages occur in-place in the output buffer
         */
        pIn = pDst;
        /*
         * Reset to destination pointer
         */
        pOut = pDst;
    }
    while (--stages);
#endif
}
#endif
/**
  @} end of BiquadCascadeDF1 group
 */
