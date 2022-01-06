/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_fir_q15.c
 * Description:  Q15 FIR filter processing function
 *               using Helium assembly kernels
 *               This version allows boosting Q.15 FIR Filter performance when using compilers having suboptimal
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

#include "dsp/filtering_functions.h"

/**
  @ingroup groupFilters
 */

/**
  @addtogroup FIR
  @{
 */

/**
  @brief         Processing function for the Q15 FIR filter.
  @param[in]     S          points to an instance of the Q15 FIR filter structure
  @param[in]     pSrc       points to the block of input data
  @param[out]    pDst       points to the block of output data
  @param[in]     blockSize  number of samples to process. /!\ Number of output samples produced is rounded to the next multiple of 8. /!\
  @return        none

  @par           Scaling and Overflow Behavior
                   The function is implemented using a 64-bit internal accumulator.
                   Both coefficients and state variables are represented in 1.15 format and multiplications yield a 2.30 result.
                   The 2.30 intermediate results are accumulated in a 64-bit accumulator in 34.30 format.
                   There is no risk of internal overflow with this approach and the full precision of intermediate multiplications is preserved.
                   After all additions have been performed, the accumulator is truncated to 34.15 format by discarding low 15 bits.
                   Lastly, the accumulator is saturated to yield a result in 1.15 format.

  @remark
                   Refer to \ref arm_fir_fast_q15() for a faster but less precise implementation of this function.

                   /!\ When filter size is greater than 56, extra storage space is required to perform partial filtering accumulation.
                   The size needed for this area is equal to the number of sample to process in _64-bit_ word size. (blockSize * sizeof(q63_t))
                   Unlike Floating point and Q.31 FIR filters, there is no extra room inside the FIR instance, so storage is reserved directly
                   on the stack to avoid changing the API.

 */
#if defined(ARM_MATH_MVEI)


#ifdef USE_ASM

/* Maximum number of vector registers holding Q.15 taps */
/* 8 total Helium v8.1M registers available             */
/* 1 coefficient needed for loading input samples       */
/* 7 coefficients free for taps storage                 */
#define FIR_Q15_TAPS_SPARE_QREGS            7

/* Maximum number of taps held in vector registers = 56 */
#define FIR_Q15_MAX_COEFS_BLK               (FIR_Q15_TAPS_SPARE_QREGS * 8)


/* 16-bit memcopy */
#define COPY_STATE_16(blockSize, pTempSrc, pTempDest)           \
__asm volatile (                                                \
    ".p2align 2                                            \n"  \
    "   wlstp.16         lr, %[cnt], 1f                    \n"  \
    /* low overhead loop start */                               \
    "2:                                                    \n"  \
    "   vldrh.16         q0, [%[src]], 16                  \n"  \
    "   vstrh.16         q0, [%[dst]], 16                  \n"  \
    "   letp             lr, 2b                            \n"  \
    /* low overhead loop end */                                 \
    "1:                                                    \n"  \
    : [src] "+r" (pTempSrc), [dst] "+r" (pTempDest)             \
    : [cnt] "r" (blockSize)                                     \
    : "q0", "r14");


/* incremental Q register clobbered list depending on number of coefficients */

#define FIR_Q15_8_COEFS_CLOB_QLIST      "q0", "q1"
#define FIR_Q15_16_COEFS_CLOB_QLIST     FIR_Q15_8_COEFS_CLOB_QLIST, "q2"
#define FIR_Q15_24_COEFS_CLOB_QLIST     FIR_Q15_16_COEFS_CLOB_QLIST, "q3"
#define FIR_Q15_32_COEFS_CLOB_QLIST     FIR_Q15_24_COEFS_CLOB_QLIST, "q4"
#define FIR_Q15_40_COEFS_CLOB_QLIST     FIR_Q15_32_COEFS_CLOB_QLIST, "q5"
#define FIR_Q15_48_COEFS_CLOB_QLIST     FIR_Q15_40_COEFS_CLOB_QLIST, "q6"
#define FIR_Q15_56_COEFS_CLOB_QLIST     FIR_Q15_48_COEFS_CLOB_QLIST, "q7"


/* coefficients preload into vector registers */

#define FIR_Q15_LOAD_8_COEFS(coefPtr)                                \
        /* Coefs load  */                                            \
        "coef0_7    .req q1                                    \n"   \
        "coef8_15   .req q2                                    \n"   \
        "coef16_23  .req q3                                    \n"   \
        "coef24_31  .req q4                                    \n"   \
        "coef32_39  .req q5                                    \n"   \
        "coef40_47  .req q6                                    \n"   \
        "coef48_55  .req q7                                    \n"   \
                                                                     \
        "   vldrh.16        coef0_7, ["#coefPtr"]              \n"


#define FIR_Q15_LOAD_16_COEFS(coefPtr)                               \
        FIR_Q15_LOAD_8_COEFS(coefPtr)                                \
        "   vldrh.16        coef8_15, ["#coefPtr", #16]        \n"

#define FIR_Q15_LOAD_24_COEFS(coefPtr)                               \
        FIR_Q15_LOAD_16_COEFS(coefPtr)                               \
        "   vldrh.16        coef16_23, ["#coefPtr", #32]       \n"

#define FIR_Q15_LOAD_32_COEFS(coefPtr)                               \
        FIR_Q15_LOAD_24_COEFS(coefPtr)                               \
        "   vldrh.16        coef24_31, ["#coefPtr", #48]      \n"

#define FIR_Q15_LOAD_40_COEFS(coefPtr)                               \
        FIR_Q15_LOAD_32_COEFS(coefPtr)                               \
        "   vldrh.16        coef32_39, ["#coefPtr", #64]      \n"

#define FIR_Q15_LOAD_48_COEFS(coefPtr)                               \
        FIR_Q15_LOAD_40_COEFS(coefPtr)                               \
        "   vldrh.16        coef40_47, ["#coefPtr", #80]      \n"

#define FIR_Q15_LOAD_56_COEFS(coefPtr)                               \
        FIR_Q15_LOAD_48_COEFS(coefPtr)                               \
        "   vldrh.16        coef48_55, ["#coefPtr", #96]      \n"


/* VMLALDAV Helium MAC + reduce operartor accumulation mode */

/* clear accumulator before operation */
#define ACC_CLEAR
/* Accumulate with existing register contents */
#define ACC_ACCUMULATE              "a"

/* block filtering MACROs */

/* vector load (post-indexed) + vector MAC */
#define FIR_Q15_FILTER_LAST_POSTINCR(acc, pSamples, incr, accumulate)                       \
        "   vldrh.16                         q0, [%["#pSamples"]], #("#incr")           \n" \
        "   vmlaldav"ACC_##accumulate".s16   %Q["#acc"], %R["#acc"], q0, coef0_7        \n" \

/* vector load (pre-indexed) + vector MAC */
#define FIR_Q15_FILTER_LAST(acc, pSamples, offs, accumulate)                                \
        "   vldrh.16                         q0, [%["#pSamples"], #("#offs")]           \n" \
        "   vmlaldav"ACC_##accumulate".s16   %Q["#acc"], %R["#acc"], q0, coef0_7        \n" \

#define FIR_Q15_FILTER_16_COEFS_PARTIAL(acc, pSamples, offs, accumulate)                    \
        "   vldrh.16                         q0, [%["#pSamples"], #("#offs" + 16)]      \n" \
        "   vmlaldav"ACC_##accumulate".s16   %Q["#acc"], %R["#acc"], q0, coef8_15       \n" \

#define FIR_Q15_FILTER_24_COEFS_PARTIAL(acc, pSamples, offs, accumulate)                    \
        "   vldrh.16                         q0, [%["#pSamples"], #("#offs" + (16*2))]  \n" \
        "   vmlaldav"ACC_##accumulate".s16   %Q["#acc"], %R["#acc"], q0, coef16_23      \n" \
        "   vldrh.16                         q0, [%["#pSamples"], #("#offs" + 16)]      \n" \
        "   vmlaldava.s16                    %Q["#acc"], %R["#acc"], q0, coef8_15       \n" \

#define FIR_Q15_FILTER_32_COEFS_PARTIAL(acc, pSamples, offs, accumulate)                    \
        "   vldrh.16                         q0, [%["#pSamples"], #("#offs" + (16*3))]  \n" \
        "   vmlaldav"ACC_##accumulate".s16   %Q["#acc"], %R["#acc"], q0, coef24_31      \n" \
        "   vldrh.16                         q0, [%["#pSamples"], #("#offs" + (16*2))]  \n" \
        "   vmlaldava.s16                    %Q["#acc"], %R["#acc"], q0, coef16_23      \n" \
        "   vldrh.16                         q0, [%["#pSamples"], #("#offs" + 16)]      \n" \
        "   vmlaldava.s16                    %Q["#acc"], %R["#acc"], q0, coef8_15       \n" \

#define FIR_Q15_FILTER_40_COEFS_PARTIAL(acc, pSamples, offs, accumulate)                    \
        "   vldrh.16                         q0, [%["#pSamples"], #("#offs" + (16*4))]  \n" \
        "   vmlaldav"ACC_##accumulate".s16   %Q["#acc"], %R["#acc"], q0, coef32_39      \n" \
        "   vldrh.16                         q0, [%["#pSamples"], #("#offs" + (16*3))]  \n" \
        "   vmlaldava.s16                    %Q["#acc"], %R["#acc"], q0, coef24_31      \n" \
        "   vldrh.16                         q0, [%["#pSamples"], #("#offs" + (16*2))]  \n" \
        "   vmlaldava.s16                    %Q["#acc"], %R["#acc"], q0, coef16_23      \n" \
        "   vldrh.16                         q0, [%["#pSamples"], #("#offs" + 16)]      \n" \
        "   vmlaldava.s16                    %Q["#acc"], %R["#acc"], q0, coef8_15       \n" \

#define FIR_Q15_FILTER_48_COEFS_PARTIAL(acc, pSamples, offs, accumulate)                    \
        "   vldrh.16                         q0, [%["#pSamples"], #("#offs" + (16*5))]  \n" \
        "   vmlaldav"ACC_##accumulate".s16   %Q["#acc"], %R["#acc"], q0, coef40_47      \n" \
        "   vldrh.16                         q0, [%["#pSamples"], #("#offs" + (16*4))]  \n" \
        "   vmlaldava.s16                    %Q["#acc"], %R["#acc"], q0, coef32_39      \n" \
        "   vldrh.16                         q0, [%["#pSamples"], #("#offs" + (16*3))]  \n" \
        "   vmlaldava.s16                    %Q["#acc"], %R["#acc"], q0, coef24_31      \n" \
        "   vldrh.16                         q0, [%["#pSamples"], #("#offs" + (16*2))]  \n" \
        "   vmlaldava.s16                    %Q["#acc"], %R["#acc"], q0, coef16_23      \n" \
        "   vldrh.16                         q0, [%["#pSamples"], #("#offs" + 16)]      \n" \
        "   vmlaldava.s16                    %Q["#acc"], %R["#acc"], q0, coef8_15       \n" \

#define FIR_Q15_FILTER_56_COEFS_PARTIAL(acc, pSamples, offs, accumulate)                    \
        "   vldrh.16                         q0, [%["#pSamples"], #("#offs" + (16*6))]  \n" \
        "   vmlaldav"ACC_##accumulate".s16   %Q["#acc"], %R["#acc"], q0, coef48_55      \n" \
        "   vldrh.16                         q0, [%["#pSamples"], #("#offs" + (16*5))]  \n" \
        "   vmlaldava.s16                    %Q["#acc"], %R["#acc"], q0, coef40_47      \n" \
        "   vldrh.16                         q0, [%["#pSamples"], #("#offs" + (16*4))]  \n" \
        "   vmlaldava.s16                    %Q["#acc"], %R["#acc"], q0, coef32_39      \n" \
        "   vldrh.16                         q0, [%["#pSamples"], #("#offs" + (16*3))]  \n" \
        "   vmlaldava.s16                    %Q["#acc"], %R["#acc"], q0, coef24_31      \n" \
        "   vldrh.16                         q0, [%["#pSamples"], #("#offs" + (16*2))]  \n" \
        "   vmlaldava.s16                    %Q["#acc"], %R["#acc"], q0, coef16_23      \n" \
        "   vldrh.16                         q0, [%["#pSamples"], #("#offs" + 16)]      \n" \
        "   vmlaldava.s16                    %Q["#acc"], %R["#acc"], q0, coef8_15       \n" \


#define FIR_Q15_FILTER_8_COEFS(acc, pSamples, offs, accumulate)                             \
        FIR_Q15_FILTER_LAST(acc, pSamples, offs, accumulate)

#define FIR_Q15_FILTER_8_COEFS_POSTINCR(acc, pSamples, incr, accumulate)                    \
        FIR_Q15_FILTER_LAST_POSTINCR(acc, pSamples, incr, accumulate)

#define FIR_Q15_FILTER_16_COEFS(acc, pSamples, offs, accumulate)                            \
        FIR_Q15_FILTER_16_COEFS_PARTIAL(acc, pSamples, offs, accumulate)                    \
        FIR_Q15_FILTER_LAST(acc, pSamples, offs, ACCUMULATE)

#define FIR_Q15_FILTER_16_COEFS_POSTINCR(acc, pSamples, incr, accumulate)                   \
        FIR_Q15_FILTER_16_COEFS_PARTIAL(acc, pSamples, 0, accumulate)                       \
        FIR_Q15_FILTER_LAST_POSTINCR(acc, pSamples, incr, ACCUMULATE)

#define FIR_Q15_FILTER_24_COEFS(acc, pSamples, offs, accumulate)                            \
        FIR_Q15_FILTER_24_COEFS_PARTIAL(acc, pSamples, offs, accumulate)                    \
        FIR_Q15_FILTER_LAST(acc, pSamples, offs, ACCUMULATE)

#define FIR_Q15_FILTER_24_COEFS_POSTINCR(acc, pSamples, incr, accumulate)                   \
        FIR_Q15_FILTER_24_COEFS_PARTIAL(acc, pSamples, 0, accumulate)                       \
        FIR_Q15_FILTER_LAST_POSTINCR(acc, pSamples, incr, ACCUMULATE)

#define FIR_Q15_FILTER_32_COEFS(acc, pSamples, offs, accumulate)                            \
        FIR_Q15_FILTER_32_COEFS_PARTIAL(acc, pSamples, offs, accumulate)                    \
        FIR_Q15_FILTER_LAST(acc, pSamples, offs, ACCUMULATE)

#define FIR_Q15_FILTER_32_COEFS_POSTINCR(acc, pSamples, incr, accumulate)                   \
        FIR_Q15_FILTER_32_COEFS_PARTIAL(acc, pSamples, 0, accumulate)                       \
        FIR_Q15_FILTER_LAST_POSTINCR(acc, pSamples, incr, ACCUMULATE)

#define FIR_Q15_FILTER_40_COEFS(acc, pSamples, offs, accumulate)                            \
        FIR_Q15_FILTER_40_COEFS_PARTIAL(acc, pSamples, offs, accumulate)                    \
        FIR_Q15_FILTER_LAST(acc, pSamples, offs, ACCUMULATE)

#define FIR_Q15_FILTER_40_COEFS_POSTINCR(acc, pSamples, incr, accumulate)                   \
        FIR_Q15_FILTER_40_COEFS_PARTIAL(acc, pSamples, 0, accumulate)                       \
        FIR_Q15_FILTER_LAST_POSTINCR(acc, pSamples, incr, ACCUMULATE)

#define FIR_Q15_FILTER_48_COEFS(acc, pSamples, offs, accumulate)                            \
        FIR_Q15_FILTER_48_COEFS_PARTIAL(acc, pSamples, offs, accumulate)                    \
        FIR_Q15_FILTER_LAST(acc, pSamples, offs, ACCUMULATE)

#define FIR_Q15_FILTER_48_COEFS_POSTINCR(acc, pSamples, incr, accumulate)                   \
        FIR_Q15_FILTER_48_COEFS_PARTIAL(acc, pSamples, 0, accumulate)                       \
        FIR_Q15_FILTER_LAST_POSTINCR(acc, pSamples, incr, ACCUMULATE)

#define FIR_Q15_FILTER_56_COEFS(acc, pSamples, offs, accumulate)                            \
        FIR_Q15_FILTER_56_COEFS_PARTIAL(acc, pSamples, offs, accumulate)                    \
        FIR_Q15_FILTER_LAST(acc, pSamples, offs, ACCUMULATE)

#define FIR_Q15_FILTER_56_COEFS_POSTINCR(acc, pSamples, incr, accumulate)                   \
        FIR_Q15_FILTER_56_COEFS_PARTIAL(acc, pSamples, 0, accumulate)                       \
        FIR_Q15_FILTER_LAST_POSTINCR(acc, pSamples, incr, ACCUMULATE)


/* filtering macro for filters with number of taps lower or equal than 56 */

#define FIR_Q15_MAIN_CORE(TAPS)                                                             \
    q15_t       *pRefStatePtr = S->pState;                                                  \
    q15_t       *pState = pRefStatePtr; /* State pointer */                                 \
    const q15_t *pCoeffs = S->pCoeffs; /* Coefficient pointer */                            \
    q15_t       *pStateCur;        /* Points to the current sample of the state */          \
    const q15_t *pSamples;         /* Temporary pointer to the sample buffer */             \
    q15_t       *pOutput;          /* Temporary pointer to the output buffer */             \
    const q15_t *pTempSrc;         /* Temporary pointer to the source data */               \
    q15_t       *pTempDest;        /* Temporary pointer to the destination buffer */        \
    uint32_t     numTaps = S->numTaps; /* Number of filter coefficients in the filter */    \
    q63_t        acc0, acc1;                                                                \
    uint32_t     low, high;                                                                 \
                                                                                            \
                                                                                            \
    /*                                                                                      \
     * pState points to state array which contains previous frame (numTaps - 1) samples     \
     * pStateCur points to the location where the new input data should be written          \
     */                                                                                     \
    pStateCur = &(pState[(numTaps - 1u)]);                                                  \
    pTempDest = pStateCur;                                                                  \
    pTempSrc = pSrc;                                                                        \
    pSamples = pState;                                                                      \
    pOutput = pDst;                                                                         \
                                                                                            \
    /* use a scratch to reduce register constraints in the following */                     \
    /* inline asm block when using GCC */                                                   \
    uint32_t    scratch[2];                                                                 \
    /* rounded low overhead loop size */                                                    \
    scratch[0] = ((blockSize + 7) / 8);                                                     \
    scratch[1] = (uint32_t)pCoeffs;                                                         \
                                                                                            \
    __asm volatile(                                                                         \
        /* Coefs load  */                                                                   \
        "coefPtr   .req lr                                              \n"                 \
        "   ldr             coefPtr, [%[scratch], #4]                   \n"                 \
                                                                                            \
        FIR_Q15_LOAD_##TAPS##_COEFS(coefPtr)                                                \
                                                                                            \
        /* preload state */                                                                 \
        "   vldrh.16        q0, [%[pTempSrc]], #16                     \n"                  \
        "   vstrh.16        q0, [%[pTempDest]], #16                    \n"                  \
                                                                                            \
        "   ldr             lr, [%[scratch], #0]                       \n"                  \
                                                                                            \
        /*                                                                                  \
         * Filtering Core loop                                                              \
         * Produces 8 elements / loop                                                       \
         */                                                                                 \
        ".p2align 2                                                    \n"                  \
        "   wls             lr, lr, 1f                                 \n"                  \
        /* low overhead loop start */                                                       \
        "2:                                                            \n"                  \
                                                                                            \
        /* block filtering starting &pSample[1] */                                          \
        FIR_Q15_FILTER_##TAPS##_COEFS(acc0, pSamples, 2, CLEAR)                             \
        /* block filtering starting &pSample[0] + advance to &pSample[2] */                 \
        FIR_Q15_FILTER_##TAPS##_COEFS_POSTINCR(acc1, pSamples, 4, CLEAR)                    \
                                                                                            \
        /* .54-> .15 conversion and pack 2 x Q15 in a register */                           \
        "   sqrshrl          %Q[acc0], %R[acc0], #48, %[q15shift]      \n"                  \
        "   sqrshrl          %Q[acc1], %R[acc1], #48, %[q15shift]      \n"                  \
        "   pkhbt            %[low], %R[acc1], %R[acc0], LSL #16       \n"                  \
                                                                                            \
        FIR_Q15_FILTER_##TAPS##_COEFS(acc0, pSamples, 2, CLEAR)                             \
        FIR_Q15_FILTER_##TAPS##_COEFS_POSTINCR(acc1, pSamples, 4, CLEAR)                    \
                                                                                            \
        /* .54-> .15 conversion and pack 2 x Q15 in a register */                           \
        "   sqrshrl          %Q[acc0], %R[acc0], #48, %[q15shift]      \n"                  \
        "   sqrshrl          %Q[acc1], %R[acc1], #48, %[q15shift]      \n"                  \
        "   pkhbt            %[high], %R[acc1], %R[acc0], LSL #16      \n"                  \
                                                                                            \
        FIR_Q15_FILTER_##TAPS##_COEFS(acc0, pSamples, 2, CLEAR)                             \
        FIR_Q15_FILTER_##TAPS##_COEFS_POSTINCR(acc1, pSamples, 4, CLEAR)                    \
                                                                                            \
        /* store 4 x Q.15 elements */                                                       \
        "   strd             %[low], %[high], [%[pOutput]], #8         \n"                  \
                                                                                            \
        /* .54-> .15 conversion and pack 2 x Q15 in a register */                           \
        "   sqrshrl          %Q[acc0], %R[acc0], #48, %[q15shift]      \n"                  \
        "   sqrshrl          %Q[acc1], %R[acc1], #48, %[q15shift]      \n"                  \
        "   pkhbt            %[low], %R[acc1], %R[acc0], LSL #16       \n"                  \
                                                                                            \
        FIR_Q15_FILTER_##TAPS##_COEFS(acc0, pSamples, 2, CLEAR)                             \
        FIR_Q15_FILTER_##TAPS##_COEFS_POSTINCR(acc1, pSamples, 4, CLEAR)                    \
                                                                                            \
        /* preload state */                                                                 \
        "   vldrh.16         q0, [%[pTempSrc]], #16                    \n"                  \
                                                                                            \
        /* .54-> .15 conversion and pack 2 x Q15 in a register */                           \
        "   sqrshrl          %Q[acc0], %R[acc0], #48, %[q15shift]      \n"                  \
        "   sqrshrl          %Q[acc1], %R[acc1], #48, %[q15shift]      \n"                  \
        "   vstrh.16         q0, [%[pTempDest]], #16                   \n"                  \
        "   pkhbt            %[high], %R[acc1], %R[acc0], LSL #16      \n"                  \
                                                                                            \
        /* store 4 x Q.15 elements */                                                       \
        "   strd            %[low], %[high], [%[pOutput]], #8          \n"                  \
                                                                                            \
        /* low overhead loop end */                                                         \
        "   le              lr, 2b                                     \n"                  \
        "1:                                                            \n"                  \
                                                                                            \
    : [pSamples] "+r" (pSamples), [pOutput] "+r" (pOutput),                                 \
      [pTempSrc] "+r" (pTempSrc),[pTempDest] "+r" (pTempDest),                              \
      [acc0] "=&r"(acc0),[acc1] "=&r"(acc1),                                                \
      [low] "=&r"(low),[high] "=&r"(high)                                                   \
    : [scratch] "r" (scratch), [q15shift] "r" (-(32-15))                                    \
    : FIR_Q15_##TAPS##_COEFS_CLOB_QLIST,                                                    \
      "memory", "r14");                                                                     \
                                                                                            \
    /*                                                                                      \
     * Copy the samples back into the history buffer start                                  \
     */                                                                                     \
    pTempSrc = &pState[blockSize];                                                          \
    pTempDest = pState;                                                                     \
                                                                                            \
    COPY_STATE_16(numTaps, pTempSrc, pTempDest);


/* specialized filtering routines for filters with number of taps lower or equal than 56 */

static void arm_fir_q15_1_8_mve(const arm_fir_instance_q15 * S,
  const q15_t * __restrict pSrc,
  q15_t * __restrict pDst, uint32_t blockSize)
{
    FIR_Q15_MAIN_CORE(8)
}

static void arm_fir_q15_9_16_mve(const arm_fir_instance_q15 * S,
  const q15_t * __restrict pSrc,
  q15_t * __restrict pDst, uint32_t blockSize)
{
    FIR_Q15_MAIN_CORE(16)
}

static void arm_fir_q15_17_24_mve(const arm_fir_instance_q15 * S,
  const q15_t * __restrict pSrc,
  q15_t * __restrict pDst, uint32_t blockSize)
{
    FIR_Q15_MAIN_CORE(24)
}

static void arm_fir_q15_25_32_mve(const arm_fir_instance_q15 * S,
  const q15_t * __restrict pSrc,
  q15_t * __restrict pDst, uint32_t blockSize)
{
    FIR_Q15_MAIN_CORE(32)
}

static void arm_fir_q15_33_40_mve(const arm_fir_instance_q15 * S,
  const q15_t * __restrict pSrc,
  q15_t * __restrict pDst, uint32_t blockSize)
{
    FIR_Q15_MAIN_CORE(40)
}

static void arm_fir_q15_41_48_mve(const arm_fir_instance_q15 * S,
  const q15_t * __restrict pSrc,
  q15_t * __restrict pDst, uint32_t blockSize)
{
    FIR_Q15_MAIN_CORE(48)
}

static void arm_fir_q15_49_56_mve(const arm_fir_instance_q15 * S,
  const q15_t * __restrict pSrc,
  q15_t * __restrict pDst, uint32_t blockSize)
{
    FIR_Q15_MAIN_CORE(56)
}


/* filtering routines for filters with number of taps greater than 56 */

static void arm_fir_q15_gt_56_mve(const arm_fir_instance_q15 * S,
  const q15_t * __restrict pSrc,
  q15_t * __restrict pDst, uint32_t blockSize)
{
    q15_t       *pRefStatePtr = S->pState;
    q15_t       *pState = pRefStatePtr; /* State pointer */
    const q15_t *pCoeffs = S->pCoeffs; /* Coefficient pointer */
    q15_t       *pStateCur;        /* Points to the current sample of the state */
    const q15_t *pSamples;         /* Temporary pointer to the sample buffer */
    q15_t       *pOutput;          /* Temporary pointer to the output buffer */
    const q15_t *pTempSrc;         /* Temporary pointer to the source data */
    q15_t       *pTempDest;        /* Temporary pointer to the destination buffer */
    uint32_t     numTaps = S->numTaps; /* Number of filter coefficients in the filter */
    q63_t        acc0, acc1;


    /* allocate storage for partial accumulation on the stack */
    q63_t       arm_fir_partial_acc_storage[ROUND_UP(blockSize,4)];

    /*
     * pState points to state array which contains previous frame (numTaps - 1) samples
     * pStateCur points to the location where the new input data should be written
     */
    pStateCur = &(pState[(numTaps - 1u)]);
    pTempDest = pStateCur;
    pTempSrc = pSrc;
    pSamples = pState;
    pOutput = pDst;

    COPY_STATE_16(blockSize, pTempSrc, pTempDest);


    /* use a scratch to reduce register constraints in the following */
    /* inline asm block when using GCC */
    uint32_t    scratch[2];
    /* rounded low overhead loop size */
    scratch[0] = ((blockSize + 1) / 2);
    scratch[1] = (uint32_t)pCoeffs;


    /*
     * 1st part : start from cleared accumulators and process 56 = FIR_Q15_MAX_COEFS_BLK
     *            taps and store partial accumulators in the temporary buffer
     */
    q63_t      *partial_accu_ptr = arm_fir_partial_acc_storage;
    __asm volatile(
        /* Coefs load  */
        "coefPtr   .req lr                                          \n"
        "   ldr             coefPtr, [%[scratch], #4]               \n"

        FIR_Q15_LOAD_56_COEFS(coefPtr)

        "   ldr             lr, [%[scratch], #0]                    \n"

        /*
         * Filtering Core loop
         */
        ".p2align 2                                                 \n"
        "   wls             lr, lr, 1f                              \n"
        /* low overhead loop start */
        "2:                                                         \n"

        /* block filtering starting &pSample[1] */
        FIR_Q15_FILTER_56_COEFS(acc0, pSamples, 2, CLEAR)
        /* block filtering starting &pSample[0] + advance to &pSample[2] */
        FIR_Q15_FILTER_56_COEFS_POSTINCR(acc1, pSamples, 4, CLEAR)

        "   strd             %Q[acc0], %R[acc0], [%[pap]], #8       \n"
        "   strd             %Q[acc1], %R[acc1], [%[pap]], #8       \n"

        /* low overhead loop end */
        "   le              lr, 2b                                  \n"
        "1:                                                         \n"

    : [pSamples] "+r" (pSamples), [pap] "+r" (partial_accu_ptr),
      [acc0] "=&r"(acc0),[acc1] "=&r"(acc1)
    : [scratch] "r" (scratch)
    : FIR_Q15_56_COEFS_CLOB_QLIST,
      "memory", "r14");


    /*
     * 2nd part : reload partial accumulators and process next block of 56 taps (=FIR_Q15_MAX_COEFS_BLK)
     *            and keep on storing partial accumulators in the temporary buffer
     */
    int     localTaps = numTaps - FIR_Q15_MAX_COEFS_BLK;
    int     sample_offset = FIR_Q15_MAX_COEFS_BLK;

    while(localTaps > FIR_Q15_MAX_COEFS_BLK) {
        pSamples = pState + sample_offset;
        partial_accu_ptr = arm_fir_partial_acc_storage;

        pCoeffs += FIR_Q15_MAX_COEFS_BLK;
        scratch[1] = (uint32_t)pCoeffs;

        __asm volatile(
            /* Coefs load  */
            "coefPtr   .req lr                                          \n"
            "   ldr             coefPtr, [%[scratch], #4]               \n"

            FIR_Q15_LOAD_56_COEFS(coefPtr)

            "   ldr             lr, [%[scratch], #0]                    \n"

            /*
             * Filtering Core loop
             */
            ".p2align 2                                                 \n"
            "   wls             lr, lr, 1f                              \n"
            /* low overhead loop start */
            "2:                                                         \n"

            /*
             * load partial accumulators
             */
            "   ldrd            %Q[acc0], %R[acc0], [%[pap], #0]        \n"
            "   ldrd            %Q[acc1], %R[acc1], [%[pap], #8]        \n"

            /* block filtering starting &pSample[1] */
            FIR_Q15_FILTER_56_COEFS(acc0, pSamples, 2, ACCUMULATE)
            /* block filtering starting &pSample[0] + advance to &pSample[2] */
            FIR_Q15_FILTER_56_COEFS_POSTINCR(acc1, pSamples, 4, ACCUMULATE)

            "   strd             %Q[acc0], %R[acc0], [%[pap]], #8       \n"
            "   strd             %Q[acc1], %R[acc1], [%[pap]], #8       \n"

            /* low overhead loop end */
            "   le              lr, 2b                                  \n"
            "1:                                                         \n"

        : [pSamples] "+r" (pSamples), [pap] "+r" (partial_accu_ptr),
          [acc0] "=&r"(acc0),[acc1] "=&r"(acc1)
        : [scratch] "r" (scratch)
        : FIR_Q15_56_COEFS_CLOB_QLIST,
          "memory", "r14");

        localTaps -= FIR_Q15_MAX_COEFS_BLK;
        sample_offset += FIR_Q15_MAX_COEFS_BLK;

    }

    /*
     * last part : Process remainders (modulo 56)
     *             Reload partial accumulators + perform last block filtering then
     *             scale before storing to destination.
     */
    pOutput  = pDst;
    pSamples = pState + sample_offset;
    partial_accu_ptr = arm_fir_partial_acc_storage;

    pCoeffs += FIR_Q15_MAX_COEFS_BLK;
    scratch[1] = (uint32_t)pCoeffs;

   if (localTaps > 48) {
        __asm volatile(
            /* Coefs load  */
            "coefPtr   .req lr                                          \n"
            "   ldr             coefPtr, [%[scratch], #4]               \n"

            FIR_Q15_LOAD_56_COEFS(coefPtr)

            "   ldr             lr, [%[scratch], #0]                    \n"

            /*
             * Filtering Core loop
             */
            ".p2align 2                                                 \n"
            "   wls             lr, lr, 1f                              \n"
            /* low overhead loop start */
            "2:                                                         \n"

            /*
             * load partial accumulators
             */
            "   ldrd            %Q[acc0], %R[acc0], [%[pap]], #8        \n"
            "   ldrd            %Q[acc1], %R[acc1], [%[pap]], #8        \n"

            /* block filtering starting &pSample[1] */
            FIR_Q15_FILTER_56_COEFS(acc0, pSamples, 2, ACCUMULATE)
            /* block filtering starting &pSample[0] + advance to &pSample[2] */
            FIR_Q15_FILTER_56_COEFS_POSTINCR(acc1, pSamples, 4, ACCUMULATE)

            /* .54-> .15 conversion and store into destination */
            "   sqrshrl          %Q[acc0], %R[acc0], #48, %[q15shift]   \n"
            "   sqrshrl          %Q[acc1], %R[acc1], #48, %[q15shift]   \n"

            "   strh            %R[acc0], [%[pOutput], #2]              \n"
            "   strh            %R[acc1], [%[pOutput]], #4              \n"
            /* low overhead loop end */
            "   le              lr, 2b                                  \n"
            "1:                                                         \n"

        : [pSamples] "+r" (pSamples), [pap] "+r" (partial_accu_ptr),
          [pOutput] "+r" (pOutput),
          [acc0] "=&r"(acc0),[acc1] "=&r"(acc1)
        : [scratch] "r" (scratch), [q15shift] "r" (-(32-15))
        : FIR_Q15_56_COEFS_CLOB_QLIST,
          "memory", "r14");
    }
    else if (localTaps > 40) {
        __asm volatile(
            /* Coefs load  */
            "coefPtr   .req lr                                          \n"
            "   ldr             coefPtr, [%[scratch], #4]               \n"

            FIR_Q15_LOAD_48_COEFS(coefPtr)

            "   ldr             lr, [%[scratch], #0]                    \n"

            /*
             * Filtering Core loop
             */
            ".p2align 2                                                 \n"
            "   wls             lr, lr, 1f                              \n"
            /* low overhead loop start */
            "2:                                                         \n"

            /*
             * load partial accumulators
             */
            "   ldrd            %Q[acc0], %R[acc0], [%[pap]], #8        \n"
            "   ldrd            %Q[acc1], %R[acc1], [%[pap]], #8        \n"

            /* block filtering starting &pSample[1] */
            FIR_Q15_FILTER_48_COEFS(acc0, pSamples, 2, ACCUMULATE)
            /* block filtering starting &pSample[0] + advance to &pSample[2] */
            FIR_Q15_FILTER_48_COEFS_POSTINCR(acc1, pSamples, 4, ACCUMULATE)

            /* .54-> .15 conversion and store into destination */
            "   sqrshrl          %Q[acc0], %R[acc0], #48, %[q15shift]   \n"
            "   sqrshrl          %Q[acc1], %R[acc1], #48, %[q15shift]   \n"

            "   strh            %R[acc0], [%[pOutput], #2]              \n"
            "   strh            %R[acc1], [%[pOutput]], #4              \n"
            /* low overhead loop end */
            "   le              lr, 2b                                  \n"
            "1:                                                         \n"

        : [pSamples] "+r" (pSamples), [pap] "+r" (partial_accu_ptr),
          [pOutput] "+r" (pOutput),
          [acc0] "=&r"(acc0),[acc1] "=&r"(acc1)
        : [scratch] "r" (scratch), [q15shift] "r" (-(32-15))
        : FIR_Q15_48_COEFS_CLOB_QLIST,
          "memory", "r14");
    } else if (localTaps > 32) {
        __asm volatile(
            /* Coefs load  */
            "coefPtr   .req lr                                          \n"
            "   ldr             coefPtr, [%[scratch], #4]               \n"

            FIR_Q15_LOAD_40_COEFS(coefPtr)

            "   ldr             lr, [%[scratch], #0]                    \n"

            /*
             * Filtering Core loop
             */
            ".p2align 2                                                 \n"
            "   wls             lr, lr, 1f                              \n"
            /* low overhead loop start */
            "2:                                                         \n"

            /*
             * load partial accumulators
             */
            "   ldrd            %Q[acc0], %R[acc0], [%[pap]], #8        \n"
            "   ldrd            %Q[acc1], %R[acc1], [%[pap]], #8        \n"

            /* block filtering starting &pSample[1] */
            FIR_Q15_FILTER_40_COEFS(acc0, pSamples, 2, ACCUMULATE)
            /* block filtering starting &pSample[0] + advance to &pSample[2] */
            FIR_Q15_FILTER_40_COEFS_POSTINCR(acc1, pSamples, 4, ACCUMULATE)

            /* .54-> .15 conversion and store into destination */
            "   sqrshrl          %Q[acc0], %R[acc0], #48, %[q15shift]   \n"
            "   sqrshrl          %Q[acc1], %R[acc1], #48, %[q15shift]   \n"

            "   strh            %R[acc0], [%[pOutput], #2]              \n"
            "   strh            %R[acc1], [%[pOutput]], #4              \n"
            /* low overhead loop end */
            "   le              lr, 2b                                  \n"
            "1:                                                         \n"

        : [pSamples] "+r" (pSamples), [pap] "+r" (partial_accu_ptr),
          [pOutput] "+r" (pOutput),
          [acc0] "=&r"(acc0),[acc1] "=&r"(acc1)
        : [scratch] "r" (scratch), [q15shift] "r" (-(32-15))
        : FIR_Q15_40_COEFS_CLOB_QLIST,
          "memory", "r14");
    } else if (localTaps > 24) {
        __asm volatile(
            /* Coefs load  */
            "coefPtr   .req lr                                          \n"
            "   ldr             coefPtr, [%[scratch], #4]               \n"

            FIR_Q15_LOAD_32_COEFS(coefPtr)

            "   ldr             lr, [%[scratch], #0]                    \n"

            /*
             * Filtering Core loop
             */
            ".p2align 2                                                 \n"
            "   wls             lr, lr, 1f                              \n"
            /* low overhead loop start */
            "2:                                                         \n"

            /*
             * load partial accumulators
             */
            "   ldrd            %Q[acc0], %R[acc0], [%[pap]], #8        \n"
            "   ldrd            %Q[acc1], %R[acc1], [%[pap]], #8        \n"

            /* block filtering starting &pSample[1] */
            FIR_Q15_FILTER_32_COEFS(acc0, pSamples, 2, ACCUMULATE)
            /* block filtering starting &pSample[0] + advance to &pSample[2] */
            FIR_Q15_FILTER_32_COEFS_POSTINCR(acc1, pSamples, 4, ACCUMULATE)

            /* .54-> .15 conversion and store into destination */
            "   sqrshrl          %Q[acc0], %R[acc0], #48, %[q15shift]   \n"
            "   sqrshrl          %Q[acc1], %R[acc1], #48, %[q15shift]   \n"

            "   strh            %R[acc0], [%[pOutput], #2]              \n"
            "   strh            %R[acc1], [%[pOutput]], #4              \n"
            /* low overhead loop end */
            "   le              lr, 2b                                  \n"
            "1:                                                         \n"

        : [pSamples] "+r" (pSamples), [pap] "+r" (partial_accu_ptr),
          [pOutput] "+r" (pOutput),
          [acc0] "=&r"(acc0),[acc1] "=&r"(acc1)
        : [scratch] "r" (scratch), [q15shift] "r" (-(32-15))
        : FIR_Q15_32_COEFS_CLOB_QLIST,
          "memory", "r14");
    } else if (localTaps > 16) {
        __asm volatile(
            /* Coefs load  */
            "coefPtr   .req lr                                          \n"
            "   ldr             coefPtr, [%[scratch], #4]               \n"

            FIR_Q15_LOAD_24_COEFS(coefPtr)

            "   ldr             lr, [%[scratch], #0]                    \n"

            /*
             * Filtering Core loop
             */
            ".p2align 2                                                 \n"
            "   wls             lr, lr, 1f                              \n"
            /* low overhead loop start */
            "2:                                                         \n"

            /*
             * load partial accumulators
             */
            "   ldrd            %Q[acc0], %R[acc0], [%[pap]], #8        \n"
            "   ldrd            %Q[acc1], %R[acc1], [%[pap]], #8        \n"

            /* block filtering starting &pSample[1] */
            FIR_Q15_FILTER_24_COEFS(acc0, pSamples, 2, ACCUMULATE)
            /* block filtering starting &pSample[0] + advance to &pSample[2] */
            FIR_Q15_FILTER_24_COEFS_POSTINCR(acc1, pSamples, 4, ACCUMULATE)

            /* .54-> .15 conversion and store into destination */
            "   sqrshrl          %Q[acc0], %R[acc0], #48, %[q15shift]   \n"
            "   sqrshrl          %Q[acc1], %R[acc1], #48, %[q15shift]   \n"

            "   strh            %R[acc0], [%[pOutput], #2]              \n"
            "   strh            %R[acc1], [%[pOutput]], #4              \n"
            /* low overhead loop end */
            "   le              lr, 2b                                  \n"
            "1:                                                         \n"

        : [pSamples] "+r" (pSamples), [pap] "+r" (partial_accu_ptr),
          [pOutput] "+r" (pOutput),
          [acc0] "=&r"(acc0),[acc1] "=&r"(acc1)
        : [scratch] "r" (scratch), [q15shift] "r" (-(32-15))
        : FIR_Q15_24_COEFS_CLOB_QLIST,
          "memory", "r14");
    } else if (localTaps > 8) {
        __asm volatile(
            /* Coefs load  */
            "coefPtr   .req lr                                          \n"
            "   ldr             coefPtr, [%[scratch], #4]               \n"

            FIR_Q15_LOAD_16_COEFS(coefPtr)

            "   ldr             lr, [%[scratch], #0]                    \n"

            /*
             * Filtering Core loop
             */
            ".p2align 2                                                 \n"
            "   wls             lr, lr, 1f                              \n"
            /* low overhead loop start */
            "2:                                                         \n"

            /*
             * load partial accumulators
             */
            "   ldrd            %Q[acc0], %R[acc0], [%[pap]], #8        \n"
            "   ldrd            %Q[acc1], %R[acc1], [%[pap]], #8        \n"

            /* block filtering starting &pSample[1] */
            FIR_Q15_FILTER_16_COEFS(acc0, pSamples, 2, ACCUMULATE)
            /* block filtering starting &pSample[0] + advance to &pSample[2] */
            FIR_Q15_FILTER_16_COEFS_POSTINCR(acc1, pSamples, 4, ACCUMULATE)

            /* .54-> .15 conversion and store into destination */
            "   sqrshrl          %Q[acc0], %R[acc0], #48, %[q15shift]   \n"
            "   sqrshrl          %Q[acc1], %R[acc1], #48, %[q15shift]   \n"

            "   strh            %R[acc0], [%[pOutput], #2]              \n"
            "   strh            %R[acc1], [%[pOutput]], #4              \n"
            /* low overhead loop end */
            "   le              lr, 2b                                  \n"
            "1:                                                         \n"

        : [pSamples] "+r" (pSamples), [pap] "+r" (partial_accu_ptr),
          [pOutput] "+r" (pOutput),
          [acc0] "=&r"(acc0),[acc1] "=&r"(acc1)
        : [scratch] "r" (scratch), [q15shift] "r" (-(32-15))
        : FIR_Q15_16_COEFS_CLOB_QLIST,
          "memory", "r14");
    }    else if (localTaps > 0) {
        __asm volatile(
            /* Coefs load  */
            "coefPtr   .req lr                                          \n"
            "   ldr             coefPtr, [%[scratch], #4]               \n"

            FIR_Q15_LOAD_8_COEFS(coefPtr)

            "   ldr             lr, [%[scratch], #0]                    \n"

            /*
             * Filtering Core loop
             */
            ".p2align 2                                                 \n"
            "   wls             lr, lr, 1f                              \n"
            /* low overhead loop start */
            "2:                                                         \n"

            /*
             * load partial accumulators
             */
            "   ldrd            %Q[acc0], %R[acc0], [%[pap]], #8        \n"
            "   ldrd            %Q[acc1], %R[acc1], [%[pap]], #8        \n"

            /* block filtering starting &pSample[1] */
            FIR_Q15_FILTER_8_COEFS(acc0, pSamples, 2, ACCUMULATE)
            /* block filtering starting &pSample[0] + advance to &pSample[2] */
            FIR_Q15_FILTER_8_COEFS_POSTINCR(acc1, pSamples, 4, ACCUMULATE)

            /* .54-> .15 conversion and store into destination */
            "   sqrshrl          %Q[acc0], %R[acc0], #48, %[q15shift]   \n"
            "   sqrshrl          %Q[acc1], %R[acc1], #48, %[q15shift]   \n"

            "   strh            %R[acc0], [%[pOutput], #2]              \n"
            "   strh            %R[acc1], [%[pOutput]], #4              \n"
            /* low overhead loop end */
            "   le              lr, 2b                                  \n"
            "1:                                                         \n"

        : [pSamples] "+r" (pSamples), [pap] "+r" (partial_accu_ptr),
          [pOutput] "+r" (pOutput),
          [acc0] "=&r"(acc0),[acc1] "=&r"(acc1)
        : [scratch] "r" (scratch), [q15shift] "r" (-(32-15))
        : FIR_Q15_8_COEFS_CLOB_QLIST,
          "memory", "r14");
    }

    /*
     * Copy the samples back into the history buffer start
     */
    pTempSrc = &pState[blockSize];
    pTempDest = pState;

    COPY_STATE_16(numTaps, pTempSrc, pTempDest);
}


void arm_fir_q15_mve(
  const arm_fir_instance_q15 * S,
  const q15_t * pSrc,
        q15_t * pDst,
        uint32_t blockSize)
{
    uint32_t     numTaps = S->numTaps; /* Number of filter coefficients in the filter */

    /*
     * 1 to 56 taps specialized routines
     */
    if (numTaps <= 8)
    {
        arm_fir_q15_1_8_mve(S, pSrc, pDst, blockSize);
        return;
    }
    else if (numTaps <= 16)
    {
        arm_fir_q15_9_16_mve(S, pSrc, pDst, blockSize);
        return;
    }
    else if (numTaps <= 24)
    {
        arm_fir_q15_17_24_mve(S, pSrc, pDst, blockSize);
        return;
    }
    else if (numTaps <= 32)
    {
        arm_fir_q15_25_32_mve(S, pSrc, pDst, blockSize);
        return;
    }
    else if (numTaps <= 40)
    {
        arm_fir_q15_33_40_mve(S, pSrc, pDst, blockSize);
        return;
    }
    else if (numTaps <= 48)
    {
        arm_fir_q15_41_48_mve(S, pSrc, pDst, blockSize);
        return;
    }
    else if (numTaps <= 56)
    {
        arm_fir_q15_49_56_mve(S, pSrc, pDst, blockSize);
        return;
    }
    else
        arm_fir_q15_gt_56_mve(S, pSrc, pDst, blockSize);
}


#else
#define MVE_ASRL_SAT16(acc, shift)          ((sqrshrl_sat48(acc, -(32-shift)) >> 32) & 0xffffffff)


#define FIR_Q15_CORE(pOutput, nbAcc, nbVecTaps, pSample, vecCoeffs)        \
        for (int j = 0; j < nbAcc; j++) {                                  \
            const q15_t    *pSmp = &pSample[j];                            \
            q63_t           acc[4];                                        \
                                                                           \
            acc[j] = 0;                                                    \
            for (int i = 0; i < nbVecTaps; i++) {                          \
                vecIn0 = vld1q(pSmp + 8 * i);                  \
                acc[j] = vmlaldavaq(acc[j], vecIn0, vecCoeffs[i]);         \
            }                                                              \
            *pOutput++ = (q15_t) MVE_ASRL_SAT16(acc[j], 15);               \
        }

#define FIR_Q15_MAIN_CORE()                                                                  \
{                                                                                            \
    q15_t          *pState = S->pState;     /* State pointer */                              \
    const q15_t    *pCoeffs = S->pCoeffs;   /* Coefficient pointer */                        \
    q15_t          *pStateCur;              /* Points to the current sample of the state */  \
    const q15_t    *pSamples;               /* Temporary pointer to the sample buffer */     \
    q15_t          *pOutput;                /* Temporary pointer to the output buffer */     \
    const q15_t    *pTempSrc;               /* Temporary pointer to the source data */       \
    q15_t          *pTempDest;              /* Temporary pointer to the destination buffer */\
    uint32_t        numTaps = S->numTaps;   /* Number of filter coefficients in the filter */\
    int32_t         blkCnt;                                                                  \
    q15x8_t         vecIn0;                                                                  \
                                                                                             \
    /*                                                                                       \
     * load coefs                                                                            \
     */                                                                                      \
    q15x8_t         vecCoeffs[NBVECTAPS];                                                    \
                                                                                             \
    for (int i = 0; i < NBVECTAPS; i++)                                                      \
        vecCoeffs[i] = vldrhq_s16(pCoeffs + 8 * i);                                          \
                                                                                             \
    /*                                                                                       \
     * pState points to state array which contains previous frame (numTaps - 1) samples      \
     * pStateCur points to the location where the new input data should be written           \
     */                                                                                      \
    pStateCur = &(pState[(numTaps - 1u)]);                                                   \
    pTempSrc = pSrc;                                                                         \
    pSamples = pState;                                                                       \
    pOutput = pDst;                                                                          \
                                                                                             \
    blkCnt = blockSize >> 2;                                                                 \
    while (blkCnt > 0) {                                                                     \
        /*                                                                                   \
         * Save 4 input samples in the history buffer                                        \
         */                                                                                  \
        vstrhq_s32(pStateCur, vldrhq_s32(pTempSrc));                                         \
        pStateCur += 4;                                                                      \
        pTempSrc += 4;                                                                       \
                                                                                             \
        FIR_Q15_CORE(pOutput, 4, NBVECTAPS, pSamples, vecCoeffs);                            \
        pSamples += 4;                                                                       \
                                                                                             \
        blkCnt--;                                                                            \
    }                                                                                        \
                                                                                             \
    /* tail */                                                                               \
    int32_t        residual = blockSize & 3;                                                \
                                                                                             \
    for (int i = 0; i < residual; i++)                                                       \
        *pStateCur++ = *pTempSrc++;                                                          \
                                                                                             \
    FIR_Q15_CORE(pOutput, residual, NBVECTAPS, pSamples, vecCoeffs);                         \
                                                                                             \
    /*                                                                                       \
     * Copy the samples back into the history buffer start                                   \
     */                                                                                      \
    pTempSrc = &pState[blockSize];                                                           \
    pTempDest = pState;                                                                      \
                                                                                             \
    /* current compiler limitation */                                                        \
    blkCnt = (numTaps - 1) >> 3;                                                             \
    while (blkCnt > 0)                                                                       \
    {                                                                                        \
        vstrhq_s16(pTempDest, vldrhq_s16(pTempSrc));                                         \
        pTempSrc += 8;                                                                       \
        pTempDest += 8;                                                                      \
        blkCnt--;                                                                            \
    }                                                                                        \
    blkCnt = (numTaps - 1) & 7;                                                              \
    if (blkCnt > 0)                                                                          \
    {                                                                                        \
        mve_pred16_t p = vctp16q(blkCnt);                                                    \
        vstrhq_p_s16(pTempDest, vldrhq_z_s16(pTempSrc, p), p);                               \
    }                                                                                        \
}

static void arm_fir_q15_25_32_mve(const arm_fir_instance_q15 * S,
  const q15_t * __restrict pSrc,
  q15_t * __restrict pDst, uint32_t blockSize)
{
    #define NBTAPS 32
    #define NBVECTAPS (NBTAPS / 8)
    FIR_Q15_MAIN_CORE();
    #undef NBVECTAPS
    #undef NBTAPS
}

static void arm_fir_q15_17_24_mve(const arm_fir_instance_q15 * S,
  const q15_t * __restrict pSrc,
  q15_t * __restrict pDst, uint32_t blockSize)
{
    #define NBTAPS 24
    #define NBVECTAPS (NBTAPS / 8)
    FIR_Q15_MAIN_CORE();
    #undef NBVECTAPS
    #undef NBTAPS
}


static void arm_fir_q15_9_16_mve(const arm_fir_instance_q15 * S,
  const q15_t * __restrict pSrc,
  q15_t * __restrict pDst, uint32_t blockSize)
{
    #define NBTAPS 16
    #define NBVECTAPS (NBTAPS / 8)
    FIR_Q15_MAIN_CORE();
    #undef NBVECTAPS
    #undef NBTAPS
}

static void arm_fir_q15_1_8_mve(const arm_fir_instance_q15 * S,
  const q15_t * __restrict pSrc,
  q15_t * __restrict pDst, uint32_t blockSize)
{
    #define NBTAPS 8
    #define NBVECTAPS (NBTAPS / 8)
    FIR_Q15_MAIN_CORE();
    #undef NBVECTAPS
    #undef NBTAPS
}


void arm_fir_q15(
  const arm_fir_instance_q15 * S,
  const q15_t * pSrc,
        q15_t * pDst,
        uint32_t blockSize)
{
    q15_t    *pState = S->pState;   /* State pointer */
    const q15_t    *pCoeffs = S->pCoeffs; /* Coefficient pointer */
    q15_t    *pStateCur;        /* Points to the current sample of the state */
    const q15_t    *pSamples;         /* Temporary pointer to the sample buffer */
    q15_t    *pOutput;          /* Temporary pointer to the output buffer */
    const q15_t    *pTempSrc;         /* Temporary pointer to the source data */
    q15_t    *pTempDest;        /* Temporary pointer to the destination buffer */
    uint32_t  numTaps = S->numTaps; /* Number of filter coefficients in the filter */
    uint32_t  blkCnt;
    q15x8_t vecIn0;
    uint32_t  tapsBlkCnt = (numTaps + 7) / 8;
    q63_t     acc0, acc1, acc2, acc3;


int32_t nbTaps = (numTaps + 7) >> 3;

switch(nbTaps) {

    case 1:
        arm_fir_q15_1_8_mve(S, pSrc, pDst, blockSize);
        return;
    case 2:
        arm_fir_q15_9_16_mve(S, pSrc, pDst, blockSize);
        return;
    case 3:
        arm_fir_q15_17_24_mve(S, pSrc, pDst, blockSize);
        return;
    case 4:
        arm_fir_q15_25_32_mve(S, pSrc, pDst, blockSize);
        return;
    }
    /*
     * pState points to state array which contains previous frame (numTaps - 1) samples
     * pStateCur points to the location where the new input data should be written
     */
    pStateCur   = &(pState[(numTaps - 1u)]);
    pTempSrc    = pSrc;
    pSamples    = pState;
    pOutput     = pDst;
    blkCnt      = blockSize >> 2;

    while (blkCnt > 0U)
    {
        const q15_t    *pCoeffsTmp = pCoeffs;
        const q15_t    *pSamplesTmp = pSamples;

        acc0 = 0LL;
        acc1 = 0LL;
        acc2 = 0LL;
        acc3 = 0LL;

        /*
         * Save 8 input samples in the history buffer
         */
        vst1q(pStateCur, vld1q(pTempSrc));
        pStateCur += 8;
        pTempSrc += 8;

        int       i = tapsBlkCnt;
        while (i > 0)
        {
            /*
             * load 8 coefs
             */
            q15x8_t vecCoeffs = *(q15x8_t *) pCoeffsTmp;

            vecIn0 = vld1q(pSamplesTmp);
            acc0 =  vmlaldavaq(acc0, vecIn0, vecCoeffs);

            vecIn0 = vld1q(&pSamplesTmp[1]);
            acc1 = vmlaldavaq(acc1, vecIn0, vecCoeffs);

            vecIn0 = vld1q(&pSamplesTmp[2]);
            acc2 = vmlaldavaq(acc2, vecIn0, vecCoeffs);

            vecIn0 = vld1q(&pSamplesTmp[3]);
            acc3 = vmlaldavaq(acc3, vecIn0, vecCoeffs);

            pSamplesTmp += 8;
            pCoeffsTmp += 8;
            /*
             * Decrement the taps block loop counter
             */
            i--;
        }

        *pOutput++ = (q15_t) MVE_ASRL_SAT16(acc0, 15);
        *pOutput++ = (q15_t) MVE_ASRL_SAT16(acc1, 15);
        *pOutput++ = (q15_t) MVE_ASRL_SAT16(acc2, 15);
        *pOutput++ = (q15_t) MVE_ASRL_SAT16(acc3, 15);

        pSamples += 4;
        /*
         * Decrement the sample block loop counter
         */
        blkCnt--;
    }

    uint32_t  residual = blockSize & 3;
    switch (residual)
    {
    case 3:
        {
            const q15_t    *pCoeffsTmp = pCoeffs;
            const q15_t    *pSamplesTmp = pSamples;

            acc0 = 0LL;
            acc1 = 0LL;
            acc2 = 0LL;

            /*
             * Save 8 input samples in the history buffer
             */
            *(q15x8_t *) pStateCur = *(q15x8_t *) pTempSrc;
            pStateCur += 8;
            pTempSrc += 8;

            int       i = tapsBlkCnt;
            while (i > 0)
            {
                /*
                 * load 8 coefs
                 */
                q15x8_t vecCoeffs = *(q15x8_t *) pCoeffsTmp;

                vecIn0 = vld1q(pSamplesTmp);
                acc0 = vmlaldavaq(acc0, vecIn0, vecCoeffs);

                vecIn0 = vld1q(&pSamplesTmp[2]);
                acc1 = vmlaldavaq(acc1, vecIn0, vecCoeffs);

                vecIn0 = vld1q(&pSamplesTmp[4]);
                acc2 = vmlaldavaq(acc2, vecIn0, vecCoeffs);

                pSamplesTmp += 8;
                pCoeffsTmp += 8;
                /*
                 * Decrement the taps block loop counter
                 */
                i--;
            }

            acc0 = asrl(acc0, 15);
            acc1 = asrl(acc1, 15);
            acc2 = asrl(acc2, 15);

            *pOutput++ = (q15_t) MVE_ASRL_SAT16(acc0, 15);
            *pOutput++ = (q15_t) MVE_ASRL_SAT16(acc1, 15);
            *pOutput++ = (q15_t) MVE_ASRL_SAT16(acc2, 15);
        }
        break;

    case 2:
        {
            const q15_t    *pCoeffsTmp = pCoeffs;
            const q15_t    *pSamplesTmp = pSamples;

            acc0 = 0LL;
            acc1 = 0LL;
            /*
             * Save 8 input samples in the history buffer
             */
            vst1q(pStateCur, vld1q(pTempSrc));
            pStateCur += 8;
            pTempSrc += 8;

            int       i = tapsBlkCnt;
            while (i > 0)
            {
                /*
                 * load 8 coefs
                 */
                q15x8_t vecCoeffs = *(q15x8_t *) pCoeffsTmp;

                vecIn0 = vld1q(pSamplesTmp);
                acc0 = vmlaldavaq(acc0, vecIn0, vecCoeffs);

                vecIn0 = vld1q(&pSamplesTmp[2]);
                acc1 = vmlaldavaq(acc1, vecIn0, vecCoeffs);

                pSamplesTmp += 8;
                pCoeffsTmp += 8;
                /*
                 * Decrement the taps block loop counter
                 */
                i--;
            }

            *pOutput++ = (q15_t) MVE_ASRL_SAT16(acc0, 15);
            *pOutput++ = (q15_t) MVE_ASRL_SAT16(acc1, 15);
        }
        break;

    case 1:
        {
            const q15_t    *pCoeffsTmp = pCoeffs;
            const q15_t    *pSamplesTmp = pSamples;

            acc0 = 0LL;

            /*
             * Save 8 input samples in the history buffer
             */
            vst1q(pStateCur, vld1q(pTempSrc));
            pStateCur += 8;
            pTempSrc += 8;

            int       i = tapsBlkCnt;
            while (i > 0)
            {
                /*
                 * load 8 coefs
                 */
                q15x8_t vecCoeffs = *(q15x8_t *) pCoeffsTmp;

                vecIn0 = vld1q(pSamplesTmp);
                acc0 = vmlaldavaq(acc0, vecIn0, vecCoeffs);

                pSamplesTmp += 8;
                pCoeffsTmp += 8;
                /*
                 * Decrement the taps block loop counter
                 */
                i--;
            }

            *pOutput++ = (q15_t) MVE_ASRL_SAT16(acc0, 15);
        }
        break;
    }

    /*
     * Copy the samples back into the history buffer start
     */
    pTempSrc = &pState[blockSize];
    pTempDest = pState;

    blkCnt = numTaps >> 3;
    while (blkCnt > 0U)
    {
        vst1q(pTempDest, vld1q(pTempSrc));
        pTempSrc += 8;
        pTempDest += 8;
        blkCnt--;
    }
    blkCnt = numTaps & 7;
    if (blkCnt > 0U)
    {
        mve_pred16_t p0 = vctp16q(blkCnt);
        vstrhq_p_s16(pTempDest, vld1q(pTempSrc), p0);
    }
}

#endif
#endif
/**
  @} end of FIR group
 */
