/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_fir_q31.c
 * Description:  Q31 FIR filter processing function
 *               using Helium assembly kernels
 *               This version allows boosting Q.31 FIR Filter performance when using compilers having suboptimal
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
  @brief         Processing function for Q31 FIR filter.
  @param[in]     S          points to an instance of the Q31 FIR filter structure
  @param[in]     pSrc       points to the block of input data
  @param[out]    pDst       points to the block of output data
  @param[in]     blockSize  number of samples to process. /!\ Number of output samples produced is rounded to the next multiple of 4. /!\
  @return        none

  @par           Scaling and Overflow Behavior
                   The function is implemented using an internal 64-bit accumulator.
                   The accumulator has a 2.62 format and maintains full precision of the intermediate multiplication results but provides only a single guard bit.
                   Thus, if the accumulator result overflows it wraps around rather than clip.
                   In order to avoid overflows completely the input signal must be scaled down by log2(numTaps) bits.
                   After all multiply-accumulates are performed, the 2.62 accumulator is right shifted by 31 bits and saturated to 1.31 format to yield the final result.

 @remark
                   Refer to \ref arm_fir_fast_q31() for a faster but less precise implementation of this filter.
 */
#if defined(ARM_MATH_MVEI)

#include "arm_helium_utils.h"

#ifdef USE_ASM

/* Maximum number of vector registers holding Q.31 taps */
/* 8 total Helium v8.1M registers available */
/* 1 coefficient needed for loading input samples */
/* 7 coefficients free for taps storage */
#define FIR_Q31_TAPS_SPARE_QREGS            7

/* Maximum number of taps held in vector registers */
#define FIR_Q31_MAX_COEFS_BLK               (FIR_Q31_TAPS_SPARE_QREGS * 4)


/* 32-bit memcopy */
#define COPY_STATE_32(blockSize, pTempSrc, pTempDest)           \
__asm volatile (                                                \
    ".p2align 2                                            \n"  \
    "   wlstp.32         lr, %[cnt], 1f                    \n"  \
    /* low overhead loop start */                               \
    "2:                                                    \n"  \
    "   vldrw.32         q0, [%[src]], 16                  \n"  \
    "   vstrw.32         q0, [%[dst]], 16                  \n"  \
    "   letp             lr, 2b                            \n"  \
    /* low overhead loop end */                                 \
    "1:                                                    \n"  \
    : [src] "+r" (pTempSrc), [dst] "+r" (pTempDest)             \
    : [cnt] "r" (blockSize)                                     \
    : "q0", "r14");


/* incremental Q register clobbered list depending on number of coefficients */

#define FIR_Q31_4_COEFS_CLOB_QLIST      "q0", "q1"
#define FIR_Q31_8_COEFS_CLOB_QLIST      FIR_Q31_4_COEFS_CLOB_QLIST, "q2"
#define FIR_Q31_12_COEFS_CLOB_QLIST     FIR_Q31_8_COEFS_CLOB_QLIST, "q3"
#define FIR_Q31_16_COEFS_CLOB_QLIST     FIR_Q31_12_COEFS_CLOB_QLIST, "q4"
#define FIR_Q31_20_COEFS_CLOB_QLIST     FIR_Q31_16_COEFS_CLOB_QLIST, "q5"
#define FIR_Q31_24_COEFS_CLOB_QLIST     FIR_Q31_20_COEFS_CLOB_QLIST, "q6"
#define FIR_Q31_28_COEFS_CLOB_QLIST     FIR_Q31_24_COEFS_CLOB_QLIST, "q7"


/* coefficients preload into vector registers */

#define FIR_Q31_LOAD_4_COEFS(coefPtr)                                \
        /* Coefs load  */                                            \
        "coef0_3    .req q1                                    \n"   \
        "coef4_7    .req q2                                    \n"   \
        "coef8_11   .req q3                                    \n"   \
        "coef12_15  .req q4                                    \n"   \
        "coef16_19  .req q5                                    \n"   \
        "coef20_23  .req q6                                    \n"   \
        "coef24_27  .req q7                                    \n"   \
                                                                     \
        "   vldrw.32        coef0_3, ["#coefPtr"]              \n"

#define FIR_Q31_FORGET_COEFS(coefPtr)                                \
        ".unreq coef0_3                                        \n"   \
        ".unreq coef4_7                                        \n"   \
        ".unreq coef8_11                                       \n"   \
        ".unreq coef12_15                                      \n"   \
        ".unreq coef16_19                                      \n"   \
        ".unreq coef20_23                                      \n"   \
        ".unreq coef24_27                                      \n"   \
        ".unreq "#coefPtr"                                     \n"

#define FIR_Q31_LOAD_8_COEFS(coefPtr)                                \
        FIR_Q31_LOAD_4_COEFS(coefPtr)                                \
        "   vldrw.32        coef4_7, ["#coefPtr", #16]        \n"

#define FIR_Q31_LOAD_12_COEFS(coefPtr)                               \
        FIR_Q31_LOAD_8_COEFS(coefPtr)                                \
        "   vldrw.32        coef8_11, ["#coefPtr", #32]       \n"

#define FIR_Q31_LOAD_16_COEFS(coefPtr)                               \
        FIR_Q31_LOAD_12_COEFS(coefPtr)                               \
        "   vldrw.32        coef12_15, ["#coefPtr", #48]      \n"

#define FIR_Q31_LOAD_20_COEFS(coefPtr)                               \
        FIR_Q31_LOAD_16_COEFS(coefPtr)                               \
        "   vldrw.32        coef16_19, ["#coefPtr", #64]      \n"

#define FIR_Q31_LOAD_24_COEFS(coefPtr)                               \
        FIR_Q31_LOAD_20_COEFS(coefPtr)                               \
        "   vldrw.32        coef20_23, ["#coefPtr", #80]      \n"

#define FIR_Q31_LOAD_28_COEFS(coefPtr)                               \
        FIR_Q31_LOAD_24_COEFS(coefPtr)                               \
        "   vldrw.32        coef24_27, ["#coefPtr", #96]      \n"


/* VRMLALVH helium MAC + reduce accumulation mode */

/* clear accumulator before operation */
#define ACC_CLEAR
/* Accumulate with existing register contents */
#define ACC_ACCUMULATE              "a"

/* block filtering MACROs */

#define FIR_Q31_FILTER_LAST_POSTINCR(acc, pSamples, incr, accumulate)                        \
        "   vldrw.32                         q0, [%["#pSamples"]], #("#incr")            \n" \
        "   vrmlalvh"ACC_##accumulate".s32   %Q["#acc"], %R["#acc"], q0, coef0_3         \n" \

#define FIR_Q31_FILTER_LAST(acc, pSamples, offs, accumulate)                                 \
        "   vldrw.32                         q0, [%["#pSamples"], #("#offs")]            \n" \
        "   vrmlalvh"ACC_##accumulate".s32   %Q["#acc"], %R["#acc"], q0, coef0_3         \n" \

#define FIR_Q31_FILTER_8_COEFS_PARTIAL(acc, pSamples, offs, accumulate)                      \
        "   vldrw.32                          q0, [%["#pSamples"], #("#offs" + 16)]      \n" \
        "   vrmlalvh"ACC_##accumulate".s32    %Q["#acc"], %R["#acc"], q0, coef4_7        \n" \

#define FIR_Q31_FILTER_12_COEFS_PARTIAL(acc, pSamples, offs, accumulate)                     \
        "   vldrw.32                          q0, [%["#pSamples"], #("#offs" + (16*2))]  \n" \
        "   vrmlalvh"ACC_##accumulate".s32    %Q["#acc"], %R["#acc"], q0, coef8_11       \n" \
        "   vldrw.32                          q0, [%["#pSamples"], #("#offs" + 16)]      \n" \
        "   vrmlalvha.s32                     %Q["#acc"], %R["#acc"], q0, coef4_7        \n" \

#define FIR_Q31_FILTER_16_COEFS_PARTIAL(acc, pSamples, offs, accumulate)                     \
        "   vldrw.32                          q0, [%["#pSamples"], #("#offs" + (16*3))]  \n" \
        "   vrmlalvh"ACC_##accumulate".s32    %Q["#acc"], %R["#acc"], q0, coef12_15      \n" \
        "   vldrw.32                          q0, [%["#pSamples"], #("#offs" + (16*2))]  \n" \
        "   vrmlalvha.s32                     %Q["#acc"], %R["#acc"], q0, coef8_11       \n" \
        "   vldrw.32                          q0, [%["#pSamples"], #("#offs" + 16)]      \n" \
        "   vrmlalvha.s32                     %Q["#acc"], %R["#acc"], q0, coef4_7        \n" \

#define FIR_Q31_FILTER_20_COEFS_PARTIAL(acc, pSamples, offs, accumulate)                     \
        "   vldrw.32                          q0, [%["#pSamples"], #("#offs" + (16*4))]  \n" \
        "   vrmlalvh"ACC_##accumulate".s32    %Q["#acc"], %R["#acc"], q0, coef16_19      \n" \
        "   vldrw.32                          q0, [%["#pSamples"], #("#offs" + (16*3))]  \n" \
        "   vrmlalvha.s32                     %Q["#acc"], %R["#acc"], q0, coef12_15      \n" \
        "   vldrw.32                          q0, [%["#pSamples"], #("#offs" + (16*2))]  \n" \
        "   vrmlalvha.s32                     %Q["#acc"], %R["#acc"], q0, coef8_11       \n" \
        "   vldrw.32                          q0, [%["#pSamples"], #("#offs" + 16)]      \n" \
        "   vrmlalvha.s32                     %Q["#acc"], %R["#acc"], q0, coef4_7        \n" \

#define FIR_Q31_FILTER_24_COEFS_PARTIAL(acc, pSamples, offs, accumulate)                     \
        "   vldrw.32                          q0, [%["#pSamples"], #("#offs" + (16*5))]  \n" \
        "   vrmlalvh"ACC_##accumulate".s32    %Q["#acc"], %R["#acc"], q0, coef20_23      \n" \
        "   vldrw.32                          q0, [%["#pSamples"], #("#offs" + (16*4))]  \n" \
        "   vrmlalvha.s32                     %Q["#acc"], %R["#acc"], q0, coef16_19      \n" \
        "   vldrw.32                          q0, [%["#pSamples"], #("#offs" + (16*3))]  \n" \
        "   vrmlalvha.s32                     %Q["#acc"], %R["#acc"], q0, coef12_15      \n" \
        "   vldrw.32                          q0, [%["#pSamples"], #("#offs" + (16*2))]  \n" \
        "   vrmlalvha.s32                     %Q["#acc"], %R["#acc"], q0, coef8_11       \n" \
        "   vldrw.32                          q0, [%["#pSamples"], #("#offs" + 16)]      \n" \
        "   vrmlalvha.s32                     %Q["#acc"], %R["#acc"], q0, coef4_7        \n" \

#define FIR_Q31_FILTER_28_COEFS_PARTIAL(acc, pSamples, offs, accumulate)                     \
        "   vldrw.32                          q0, [%["#pSamples"], #("#offs" + (16*6))]  \n" \
        "   vrmlalvh"ACC_##accumulate".s32    %Q["#acc"], %R["#acc"], q0, coef24_27      \n" \
        "   vldrw.32                          q0, [%["#pSamples"], #("#offs" + (16*5))]  \n" \
        "   vrmlalvha.s32                     %Q["#acc"], %R["#acc"], q0, coef20_23      \n" \
        "   vldrw.32                          q0, [%["#pSamples"], #("#offs" + (16*4))]  \n" \
        "   vrmlalvha.s32                     %Q["#acc"], %R["#acc"], q0, coef16_19      \n" \
        "   vldrw.32                          q0, [%["#pSamples"], #("#offs" + (16*3))]  \n" \
        "   vrmlalvha.s32                     %Q["#acc"], %R["#acc"], q0, coef12_15      \n" \
        "   vldrw.32                          q0, [%["#pSamples"], #("#offs" + (16*2))]  \n" \
        "   vrmlalvha.s32                     %Q["#acc"], %R["#acc"], q0, coef8_11       \n" \
        "   vldrw.32                          q0, [%["#pSamples"], #("#offs" + 16)]      \n" \
        "   vrmlalvha.s32                     %Q["#acc"], %R["#acc"], q0, coef4_7        \n" \


#define FIR_Q31_FILTER_4_COEFS(acc, pSamples, offs, accumulate)                              \
        FIR_Q31_FILTER_LAST(acc, pSamples, offs, accumulate)

#define FIR_Q31_FILTER_4_COEFS_POSTINCR(acc, pSamples, incr, accumulate)                     \
        FIR_Q31_FILTER_LAST_POSTINCR(acc, pSamples, incr, accumulate)

#define FIR_Q31_FILTER_8_COEFS(acc, pSamples, offs, accumulate)                              \
        FIR_Q31_FILTER_8_COEFS_PARTIAL(acc, pSamples, offs, accumulate)                      \
        FIR_Q31_FILTER_LAST(acc, pSamples, offs, ACCUMULATE)

#define FIR_Q31_FILTER_8_COEFS_POSTINCR(acc, pSamples, incr, accumulate)                     \
        FIR_Q31_FILTER_8_COEFS_PARTIAL(acc, pSamples, 0, accumulate)                         \
        FIR_Q31_FILTER_LAST_POSTINCR(acc, pSamples, incr, ACCUMULATE)

#define FIR_Q31_FILTER_12_COEFS(acc, pSamples, offs, accumulate)                             \
        FIR_Q31_FILTER_12_COEFS_PARTIAL(acc, pSamples, offs, accumulate)                     \
        FIR_Q31_FILTER_LAST(acc, pSamples, offs, ACCUMULATE)

#define FIR_Q31_FILTER_12_COEFS_POSTINCR(acc, pSamples, incr, accumulate)                    \
        FIR_Q31_FILTER_12_COEFS_PARTIAL(acc, pSamples, 0, accumulate)                        \
        FIR_Q31_FILTER_LAST_POSTINCR(acc, pSamples, incr, ACCUMULATE)

#define FIR_Q31_FILTER_16_COEFS(acc, pSamples, offs, accumulate)                             \
        FIR_Q31_FILTER_16_COEFS_PARTIAL(acc, pSamples, offs, accumulate)                     \
        FIR_Q31_FILTER_LAST(acc, pSamples, offs, ACCUMULATE)

#define FIR_Q31_FILTER_16_COEFS_POSTINCR(acc, pSamples, incr, accumulate)                    \
        FIR_Q31_FILTER_16_COEFS_PARTIAL(acc, pSamples, 0, accumulate)                        \
        FIR_Q31_FILTER_LAST_POSTINCR(acc, pSamples, incr, ACCUMULATE)

#define FIR_Q31_FILTER_20_COEFS(acc, pSamples, offs, accumulate)                             \
        FIR_Q31_FILTER_20_COEFS_PARTIAL(acc, pSamples, offs, accumulate)                     \
        FIR_Q31_FILTER_LAST(acc, pSamples, offs, ACCUMULATE)

#define FIR_Q31_FILTER_20_COEFS_POSTINCR(acc, pSamples, incr, accumulate)                    \
        FIR_Q31_FILTER_20_COEFS_PARTIAL(acc, pSamples, 0, accumulate)                        \
        FIR_Q31_FILTER_LAST_POSTINCR(acc, pSamples, incr, ACCUMULATE)

#define FIR_Q31_FILTER_24_COEFS(acc, pSamples, offs, accumulate)                             \
        FIR_Q31_FILTER_24_COEFS_PARTIAL(acc, pSamples, offs, accumulate)                     \
        FIR_Q31_FILTER_LAST(acc, pSamples, offs, ACCUMULATE)

#define FIR_Q31_FILTER_24_COEFS_POSTINCR(acc, pSamples, incr, accumulate)                    \
        FIR_Q31_FILTER_24_COEFS_PARTIAL(acc, pSamples, 0, accumulate)                        \
        FIR_Q31_FILTER_LAST_POSTINCR(acc, pSamples, incr, ACCUMULATE)

#define FIR_Q31_FILTER_28_COEFS(acc, pSamples, offs, accumulate)                             \
        FIR_Q31_FILTER_28_COEFS_PARTIAL(acc, pSamples, offs, accumulate)                     \
        FIR_Q31_FILTER_LAST(acc, pSamples, offs, ACCUMULATE)

#define FIR_Q31_FILTER_28_COEFS_POSTINCR(acc, pSamples, incr, accumulate)                    \
        FIR_Q31_FILTER_28_COEFS_PARTIAL(acc, pSamples, 0, accumulate)                        \
        FIR_Q31_FILTER_LAST_POSTINCR(acc, pSamples, incr, ACCUMULATE)


/* filtering macro for filters with number of taps lower or equal than 28 */

#define FIR_Q31_MAIN_CORE(TAPS)                                                             \
    q31_t       *pRefStatePtr = S->pState + 2*ROUND_UP(blockSize, 4);                       \
    q31_t       *pState = pRefStatePtr; /* State pointer */                                 \
    const q31_t *pCoeffs = S->pCoeffs; /* Coefficient pointer */                            \
    q31_t       *pStateCur;        /* Points to the current sample of the state */          \
    const q31_t *pSamples;         /* Temporary pointer to the sample buffer */             \
    q31_t       *pOutput;          /* Temporary pointer to the output buffer */             \
    const q31_t *pTempSrc;         /* Temporary pointer to the source data */               \
    q31_t       *pTempDest;        /* Temporary pointer to the destination buffer */        \
    uint32_t     numTaps = S->numTaps; /* Number of filter coefficients in the filter */    \
    q63_t        acc0, acc1, acc2, acc3;                                                    \
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
    scratch[0] = ((blockSize + 3) / 4);                                                     \
    scratch[1] = (uint32_t)pCoeffs;                                                         \
                                                                                            \
    __asm volatile(                                                                         \
        /* Coefs load  */                                                                   \
        "coefPtr   .req lr                                              \n"                 \
        "   ldr             coefPtr, [%[scratch], #4]                   \n"                 \
                                                                                            \
        FIR_Q31_LOAD_##TAPS##_COEFS(coefPtr)                                                \
                                                                                            \
        /* preload state */                                                                 \
        "   vldrw.32        q0, [%[pTempSrc]], #16                     \n"                  \
        "   vstrw.32        q0, [%[pTempDest]], #16                    \n"                  \
                                                                                            \
        "   ldr             lr, [%[scratch], #0]                       \n"                  \
                                                                                            \
        /*                                                                                  \
         * Filtering Core loop                                                              \
         * Produces 4 elements / loop                                                       \
         */                                                                                 \
        ".p2align 2                                                    \n"                  \
        "   wls             lr, lr, 1f                                 \n"                  \
        /* low overhead loop start */                                                       \
        "2:                                                            \n"                  \
                                                                                            \
        /* block filtering starting &pSample[3] */                                          \
        FIR_Q31_FILTER_##TAPS##_COEFS(acc0, pSamples, 12, CLEAR)                            \
                                                                                            \
        /* block filtering starting &pSample[2] */                                          \
        FIR_Q31_FILTER_##TAPS## _COEFS(acc1, pSamples, 8, CLEAR)                            \
                                                                                            \
        /* block filtering starting &pSample[1] */                                          \
        FIR_Q31_FILTER_##TAPS##_COEFS(acc2, pSamples, 4, CLEAR)                             \
                                                                                            \
        /* block filtering starting &pSample[0] + advance to &pSample[4] */                 \
        FIR_Q31_FILTER_##TAPS##_COEFS_POSTINCR(acc3, pSamples, 16, CLEAR)                   \
                                                                                            \
        /* update state */                                                                  \
        "   vldrw.32       q0, [%[pTempSrc]], #16                     \n"                   \
        /* .54-> .31 conversion and store accumulators */                                   \
        "   asrl            %Q[acc0], %R[acc0], #23                   \n"                   \
        "   asrl            %Q[acc1], %R[acc1], #23                   \n"                   \
        "   vstrw.32        q0, [%[pTempDest]], #16                   \n"                   \
        "   asrl            %Q[acc2], %R[acc2], #23                   \n"                   \
        "   asrl            %Q[acc3], %R[acc3], #23                   \n"                   \
        /*                                                                                  \
         * store 4 elements                                                                 \
         */                                                                                 \
        "   strd            %Q[acc3], %Q[acc2], [%[pOutput]], #8      \n"                   \
        "   strd            %Q[acc1], %Q[acc0], [%[pOutput]], #8      \n"                   \
                                                                                            \
        /* low overhead loop end */                                                         \
        "   le              lr, 2b                                    \n"                   \
        "1:                                                           \n"                   \
                                                                                            \
        FIR_Q31_FORGET_COEFS(coefPtr)                                                       \
                                                                                            \
    : [pSamples] "+r" (pSamples), [pOutput] "+r" (pOutput),                                 \
      [pTempSrc] "+r" (pTempSrc),[pTempDest] "+r" (pTempDest),                              \
      [acc0] "=&r"(acc0),[acc1] "=&r"(acc1),                                                \
      [acc2] "=&r"(acc2),[acc3] "=&r"(acc3)                                                 \
    : [scratch] "r" (scratch)                                                               \
    : FIR_Q31_##TAPS##_COEFS_CLOB_QLIST,                                                    \
      "memory", "r14");                                                                     \
                                                                                            \
    /*                                                                                      \
     * Copy the samples back into the history buffer start                                  \
     */                                                                                     \
    pTempSrc = &pState[blockSize];                                                          \
    pTempDest = pState;                                                                     \
                                                                                            \
    COPY_STATE_32(numTaps, pTempSrc, pTempDest);



/* specialized filtering routines for filters with number of taps lower or equal than 28 */

static void arm_fir_q31_1_4_mve(const arm_fir_instance_q31 * S,
    const q31_t * __restrict pSrc,
    q31_t * __restrict pDst, uint32_t blockSize)
{
    FIR_Q31_MAIN_CORE(4)
}

static void arm_fir_q31_5_8_mve(const arm_fir_instance_q31 * S,
    const q31_t * __restrict pSrc,
    q31_t * __restrict pDst, uint32_t blockSize)
{
    FIR_Q31_MAIN_CORE(8)
}

static void arm_fir_q31_9_12_mve(const arm_fir_instance_q31 * S,
    const q31_t * __restrict pSrc,
    q31_t * __restrict pDst, uint32_t blockSize)
{
    FIR_Q31_MAIN_CORE(12)
}

static void arm_fir_q31_13_16_mve(const arm_fir_instance_q31 * S,
    const q31_t * __restrict pSrc,
    q31_t * __restrict pDst, uint32_t blockSize)
{
    FIR_Q31_MAIN_CORE(16)
}

static void arm_fir_q31_17_20_mve(const arm_fir_instance_q31 * S,
    const q31_t * __restrict pSrc,
    q31_t * __restrict pDst, uint32_t blockSize)
{
    FIR_Q31_MAIN_CORE(20)
}

static void arm_fir_q31_21_24_mve(const arm_fir_instance_q31 * S,
    const q31_t * __restrict pSrc,
    q31_t * __restrict pDst, uint32_t blockSize)
{
    FIR_Q31_MAIN_CORE(24)
}

static void arm_fir_q31_25_28_mve(const arm_fir_instance_q31 * S,
    const q31_t * __restrict pSrc,
    q31_t * __restrict pDst, uint32_t blockSize)
{
    FIR_Q31_MAIN_CORE(28)
}


/* filtering routines for filters with number of taps greater than 28 */

static void arm_fir_q31_gt_28_mve(const arm_fir_instance_q31 * S,
    const q31_t * __restrict pSrc,
    q31_t * __restrict pDst, uint32_t blockSize)
{
    q31_t       *pRefStatePtr = S->pState + 2*ROUND_UP(blockSize, 4);
    q31_t       *pState = pRefStatePtr; /* State pointer */
    const q31_t *pCoeffs = S->pCoeffs; /* Coefficient pointer */
    q31_t       *pStateCur;        /* Points to the current sample of the state */
    const q31_t *pSamples;         /* Temporary pointer to the sample buffer */
    q31_t       *pOutput;          /* Temporary pointer to the output buffer */
    const q31_t *pTempSrc;         /* Temporary pointer to the source data */
    q31_t       *pTempDest;        /* Temporary pointer to the destination buffer */
    uint32_t     numTaps = S->numTaps; /* Number of filter coefficients in the filter */
    q63_t        acc0, acc1, acc2, acc3;


    /*
     * pState points to state array which contains previous frame (numTaps - 1) samples
     * pStateCur points to the location where the new input data should be written
     */
    pStateCur = &(pState[(numTaps - 1u)]);
    pTempDest = pStateCur;
    pTempSrc = pSrc;
    pSamples = pState;
    pOutput = pDst;

    COPY_STATE_32(blockSize, pTempSrc, pTempDest);


    /* use a scratch to reduce register constraints in the following */
    /* inline asm block when using GCC */
    uint32_t    scratch[2];
    /* rounded low overhead loop size */
    scratch[0] = ((blockSize + 3) / 4);
    scratch[1] = (uint32_t)pCoeffs;


    /*
     * 1st part : start from cleared accumulators and process 28 = FIR_Q31_MAX_COEFS_BLK
     *            taps and store partial accumulators in the temporary buffer
     */
    q63_t      *partial_accu_ptr = (q63_t *)S->pState;
    __asm volatile(
        /* Coefs load  */
        "coefPtr   .req lr                                          \n"
        "   ldr             coefPtr, [%[scratch], #4]               \n"

        FIR_Q31_LOAD_28_COEFS(coefPtr)

        "   ldr             lr, [%[scratch], #0]                    \n"

        /*
         * Filtering Core loop
         */
        ".p2align 2                                                 \n"
        "   wls             lr, lr, 1f                              \n"
        /* low overhead loop start */
        "2:                                                         \n"

        FIR_Q31_FILTER_28_COEFS(acc0, pSamples, 12, CLEAR)

        FIR_Q31_FILTER_28_COEFS(acc1, pSamples, 8, CLEAR)

        FIR_Q31_FILTER_28_COEFS(acc2, pSamples, 4, CLEAR)

        /* advance sample ptr to next block */
        FIR_Q31_FILTER_28_COEFS_POSTINCR(acc3, pSamples, 16, CLEAR)

        /*
         * store partial accumulators
         */
        "   strd            %Q[acc0], %R[acc0], [%[pap]], #8        \n"
        "   strd            %Q[acc1], %R[acc1], [%[pap]], #8        \n"
        "   strd            %Q[acc2], %R[acc2], [%[pap]], #8        \n"
        "   strd            %Q[acc3], %R[acc3], [%[pap]], #8        \n"

        /* low overhead loop end */
        "   le              lr, 2b                                  \n"
        "1:                                                         \n"

        FIR_Q31_FORGET_COEFS(coefPtr)

    : [pSamples] "+r" (pSamples), [pap] "+r" (partial_accu_ptr),
      [acc0] "=&r"(acc0),[acc1] "=&r"(acc1),
      [acc2] "=&r"(acc2),[acc3] "=&r"(acc3)
    : [scratch] "r" (scratch)
    : FIR_Q31_28_COEFS_CLOB_QLIST,
      "memory", "r14");


    /*
     * 2nd part : reload partial accumulators and process next block of 28 taps (=FIR_Q31_MAX_COEFS_BLK)
     *            and keep on storing partial accumulators in the temporary buffer
     */
    int     localTaps = numTaps - FIR_Q31_MAX_COEFS_BLK;
    int     sample_offset = FIR_Q31_MAX_COEFS_BLK;

    while(localTaps > FIR_Q31_MAX_COEFS_BLK) {
        pSamples = pState + sample_offset;
        partial_accu_ptr = (q63_t *)S->pState;

        pCoeffs += FIR_Q31_MAX_COEFS_BLK;
        scratch[1] = (uint32_t)pCoeffs;

        __asm volatile(
            /* Coefs load  */
            "coefPtr   .req lr                                          \n"
            "   ldr             coefPtr, [%[scratch], #4]               \n"

            FIR_Q31_LOAD_28_COEFS(coefPtr)

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
            "   ldrd            %Q[acc2], %R[acc2], [%[pap], #16]       \n"
            "   ldrd            %Q[acc3], %R[acc3], [%[pap], #24]       \n"

            FIR_Q31_FILTER_28_COEFS(acc0, pSamples, 12, ACCUMULATE)

            FIR_Q31_FILTER_28_COEFS(acc1, pSamples, 8, ACCUMULATE)

            FIR_Q31_FILTER_28_COEFS(acc2, pSamples, 4, ACCUMULATE)

            /* advance sample ptr to next block */
            FIR_Q31_FILTER_28_COEFS_POSTINCR(acc3, pSamples, 16, ACCUMULATE)

            /*
             * store partial accumulators
             */
            "   strd            %Q[acc0], %R[acc0], [%[pap]], #8        \n"
            "   strd            %Q[acc1], %R[acc1], [%[pap]], #8        \n"
            "   strd            %Q[acc2], %R[acc2], [%[pap]], #8        \n"
            "   strd            %Q[acc3], %R[acc3], [%[pap]], #8        \n"

            /* low overhead loop end */
            "   le              lr, 2b                                  \n"
            "1:                                                         \n"

            FIR_Q31_FORGET_COEFS(coefPtr)

        : [pSamples] "+r" (pSamples), [pap] "+r" (partial_accu_ptr),
          [acc0] "=&r"(acc0),[acc1] "=&r"(acc1),
          [acc2] "=&r"(acc2),[acc3] "=&r"(acc3)
        : [scratch] "r" (scratch)
        : FIR_Q31_28_COEFS_CLOB_QLIST,
          "memory", "r14");

        localTaps -= FIR_Q31_MAX_COEFS_BLK;
        sample_offset += FIR_Q31_MAX_COEFS_BLK;

    }

    /*
     * last part : Process remainders (modulo 28)
     *             Reload partial accumulators + perform last block filtering then
     *             scale before storing to destination.
     */
    pOutput  = pDst;
    pSamples = pState + sample_offset;
    partial_accu_ptr = (q63_t *)S->pState;

    pCoeffs += FIR_Q31_MAX_COEFS_BLK;
    scratch[1] = (uint32_t)pCoeffs;

   if (localTaps > 24) {
        __asm volatile(
            /* Coefs load  */
            "coefPtr   .req lr                                          \n"
            "   ldr             coefPtr, [%[scratch], #4]               \n"

            FIR_Q31_LOAD_28_COEFS(coefPtr)

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
            "   ldrd            %Q[acc2], %R[acc2], [%[pap]], #8        \n"
            "   ldrd            %Q[acc3], %R[acc3], [%[pap]], #8        \n"

            FIR_Q31_FILTER_28_COEFS(acc0, pSamples, 12, ACCUMULATE)

            FIR_Q31_FILTER_28_COEFS(acc1, pSamples, 8, ACCUMULATE)

            FIR_Q31_FILTER_28_COEFS(acc2, pSamples, 4, ACCUMULATE)

            /* advance sample ptr to next block */
            FIR_Q31_FILTER_28_COEFS_POSTINCR(acc3, pSamples, 16, ACCUMULATE)

            /* .54-> .31 conversion and store accumulators */
            "   asrl            %Q[acc0], %R[acc0], #23                 \n"
            "   asrl            %Q[acc1], %R[acc1], #23                 \n"
            "   asrl            %Q[acc2], %R[acc2], #23                 \n"
            "   asrl            %Q[acc3], %R[acc3], #23                 \n"
            /*
             * store 4 elements
             */
            "   strd            %Q[acc3], %Q[acc2], [%[pOutput]], #8    \n"
            "   strd            %Q[acc1], %Q[acc0], [%[pOutput]], #8    \n"

            /* low overhead loop end */
            "   le              lr, 2b                                  \n"
            "1:                                                         \n"

            FIR_Q31_FORGET_COEFS(coefPtr)

        : [pSamples] "+r" (pSamples), [pap] "+r" (partial_accu_ptr),
          [pOutput] "+r" (pOutput),
          [acc0] "=&r"(acc0),[acc1] "=&r"(acc1),
          [acc2] "=&r"(acc2),[acc3] "=&r"(acc3)
        : [scratch] "r" (scratch)
        : FIR_Q31_28_COEFS_CLOB_QLIST,
          "memory", "r14");
    }
    else if (localTaps > 20) {
        __asm volatile(
            /* Coefs load  */
            "coefPtr   .req lr                                          \n"
            "   ldr             coefPtr, [%[scratch], #4]               \n"

            FIR_Q31_LOAD_24_COEFS(coefPtr)

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
            "   ldrd            %Q[acc2], %R[acc2], [%[pap]], #8        \n"
            "   ldrd            %Q[acc3], %R[acc3], [%[pap]], #8        \n"

            FIR_Q31_FILTER_24_COEFS(acc0, pSamples, 12, ACCUMULATE)

            FIR_Q31_FILTER_24_COEFS(acc1, pSamples, 8, ACCUMULATE)

            FIR_Q31_FILTER_24_COEFS(acc2, pSamples, 4, ACCUMULATE)

            /* advance sample ptr to next block */
            FIR_Q31_FILTER_24_COEFS_POSTINCR(acc3, pSamples, 16, ACCUMULATE)

            /* .54-> .31 conversion and store accumulators */
            "   asrl            %Q[acc0], %R[acc0], #23                 \n"
            "   asrl            %Q[acc1], %R[acc1], #23                 \n"
            "   asrl            %Q[acc2], %R[acc2], #23                 \n"
            "   asrl            %Q[acc3], %R[acc3], #23                 \n"
            /*
             * store 4 elements
             */
            "   strd            %Q[acc3], %Q[acc2], [%[pOutput]], #8    \n"
            "   strd            %Q[acc1], %Q[acc0], [%[pOutput]], #8    \n"

            /* low overhead loop end */
            "   le              lr, 2b                                  \n"
            "1:                                                         \n"

            FIR_Q31_FORGET_COEFS(coefPtr)

        : [pSamples] "+r" (pSamples), [pap] "+r" (partial_accu_ptr),
          [pOutput] "+r" (pOutput),
          [acc0] "=&r"(acc0),[acc1] "=&r"(acc1),
          [acc2] "=&r"(acc2),[acc3] "=&r"(acc3)
        : [scratch] "r" (scratch)
        : FIR_Q31_24_COEFS_CLOB_QLIST,
          "memory", "r14");
    } else if (localTaps > 16) {
        __asm volatile(
            /* Coefs load  */
            "coefPtr   .req lr                                          \n"
            "   ldr             coefPtr, [%[scratch], #4]               \n"

            FIR_Q31_LOAD_20_COEFS(coefPtr)

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
            "   ldrd            %Q[acc2], %R[acc2], [%[pap]], #8        \n"
            "   ldrd            %Q[acc3], %R[acc3], [%[pap]], #8        \n"

            FIR_Q31_FILTER_20_COEFS(acc0, pSamples, 12, ACCUMULATE)

            FIR_Q31_FILTER_20_COEFS(acc1, pSamples, 8, ACCUMULATE)

            FIR_Q31_FILTER_20_COEFS(acc2, pSamples, 4, ACCUMULATE)

            /* advance sample ptr to next block */
            FIR_Q31_FILTER_20_COEFS_POSTINCR(acc3, pSamples, 16, ACCUMULATE)

            /* .54-> .31 conversion and store accumulators */
            "   asrl            %Q[acc0], %R[acc0], #23                 \n"
            "   asrl            %Q[acc1], %R[acc1], #23                 \n"
            "   asrl            %Q[acc2], %R[acc2], #23                 \n"
            "   asrl            %Q[acc3], %R[acc3], #23                 \n"
            /*
             * store 4 elements
             */
            "   strd            %Q[acc3], %Q[acc2], [%[pOutput]], #8    \n"
            "   strd            %Q[acc1], %Q[acc0], [%[pOutput]], #8    \n"

            /* low overhead loop end */
            "   le              lr, 2b                                  \n"
            "1:                                                         \n"

            FIR_Q31_FORGET_COEFS(coefPtr)

        : [pSamples] "+r" (pSamples), [pap] "+r" (partial_accu_ptr),
          [pOutput] "+r" (pOutput),
          [acc0] "=&r"(acc0),[acc1] "=&r"(acc1),
          [acc2] "=&r"(acc2),[acc3] "=&r"(acc3)
        : [scratch] "r" (scratch)
        : FIR_Q31_20_COEFS_CLOB_QLIST,
          "memory", "r14");
    } else if (localTaps > 12) {
        __asm volatile(
            /* Coefs load  */
            "coefPtr   .req lr                                          \n"
            "   ldr             coefPtr, [%[scratch], #4]               \n"

            FIR_Q31_LOAD_16_COEFS(coefPtr)

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
            "   ldrd            %Q[acc2], %R[acc2], [%[pap]], #8        \n"
            "   ldrd            %Q[acc3], %R[acc3], [%[pap]], #8        \n"

            FIR_Q31_FILTER_16_COEFS(acc0, pSamples, 12, ACCUMULATE)

            FIR_Q31_FILTER_16_COEFS(acc1, pSamples, 8, ACCUMULATE)

            FIR_Q31_FILTER_16_COEFS(acc2, pSamples, 4, ACCUMULATE)

            /* advance sample ptr to next block */
            FIR_Q31_FILTER_16_COEFS_POSTINCR(acc3, pSamples, 16, ACCUMULATE)

            /* .54-> .31 conversion and store accumulators */
            "   asrl            %Q[acc0], %R[acc0], #23                 \n"
            "   asrl            %Q[acc1], %R[acc1], #23                 \n"
            "   asrl            %Q[acc2], %R[acc2], #23                 \n"
            "   asrl            %Q[acc3], %R[acc3], #23                 \n"
            /*
             * store 4 elements
             */
            "   strd            %Q[acc3], %Q[acc2], [%[pOutput]], #8    \n"
            "   strd            %Q[acc1], %Q[acc0], [%[pOutput]], #8    \n"

            /* low overhead loop end */
            "   le              lr, 2b                                  \n"
            "1:                                                         \n"

            FIR_Q31_FORGET_COEFS(coefPtr)

        : [pSamples] "+r" (pSamples), [pap] "+r" (partial_accu_ptr),
          [pOutput] "+r" (pOutput),
          [acc0] "=&r"(acc0),[acc1] "=&r"(acc1),
          [acc2] "=&r"(acc2),[acc3] "=&r"(acc3)
        : [scratch] "r" (scratch)
        : FIR_Q31_16_COEFS_CLOB_QLIST,
          "memory", "r14");
    } else if (localTaps > 8) {
        __asm volatile(
            /* Coefs load  */
            "coefPtr   .req lr                                          \n"
            "   ldr             coefPtr, [%[scratch], #4]               \n"

            FIR_Q31_LOAD_12_COEFS(coefPtr)

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
            "   ldrd            %Q[acc2], %R[acc2], [%[pap]], #8        \n"
            "   ldrd            %Q[acc3], %R[acc3], [%[pap]], #8        \n"

            FIR_Q31_FILTER_12_COEFS(acc0, pSamples, 12, ACCUMULATE)

            FIR_Q31_FILTER_12_COEFS(acc1, pSamples, 8, ACCUMULATE)

            FIR_Q31_FILTER_12_COEFS(acc2, pSamples, 4, ACCUMULATE)

            /* advance sample ptr to next block */
            FIR_Q31_FILTER_12_COEFS_POSTINCR(acc3, pSamples, 16, ACCUMULATE)

            /* .54-> .31 conversion and store accumulators */
            "   asrl            %Q[acc0], %R[acc0], #23                 \n"
            "   asrl            %Q[acc1], %R[acc1], #23                 \n"
            "   asrl            %Q[acc2], %R[acc2], #23                 \n"
            "   asrl            %Q[acc3], %R[acc3], #23                 \n"
            /*
             * store 4 elements
             */
            "   strd            %Q[acc3], %Q[acc2], [%[pOutput]], #8    \n"
            "   strd            %Q[acc1], %Q[acc0], [%[pOutput]], #8    \n"

            /* low overhead loop end */
            "   le              lr, 2b                                  \n"
            "1:                                                         \n"

            FIR_Q31_FORGET_COEFS(coefPtr)

        : [pSamples] "+r" (pSamples), [pap] "+r" (partial_accu_ptr),
          [pOutput] "+r" (pOutput),
          [acc0] "=&r"(acc0),[acc1] "=&r"(acc1),
          [acc2] "=&r"(acc2),[acc3] "=&r"(acc3)
        : [scratch] "r" (scratch)
        : FIR_Q31_12_COEFS_CLOB_QLIST,
          "memory", "r14");
    } else if (localTaps > 4) {
        __asm volatile(
            /* Coefs load  */
            "coefPtr   .req lr                                          \n"
            "   ldr             coefPtr, [%[scratch], #4]               \n"

            FIR_Q31_LOAD_8_COEFS(coefPtr)

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
            "   ldrd            %Q[acc2], %R[acc2], [%[pap]], #8        \n"
            "   ldrd            %Q[acc3], %R[acc3], [%[pap]], #8        \n"

            FIR_Q31_FILTER_8_COEFS(acc0, pSamples, 12, ACCUMULATE)

            FIR_Q31_FILTER_8_COEFS(acc1, pSamples, 8, ACCUMULATE)

            FIR_Q31_FILTER_8_COEFS(acc2, pSamples, 4, ACCUMULATE)

            /* advance sample ptr to next block */
            FIR_Q31_FILTER_8_COEFS_POSTINCR(acc3, pSamples, 16, ACCUMULATE)

            /* .54-> .31 conversion and store accumulators */
            "   asrl            %Q[acc0], %R[acc0], #23                 \n"
            "   asrl            %Q[acc1], %R[acc1], #23                 \n"
            "   asrl            %Q[acc2], %R[acc2], #23                 \n"
            "   asrl            %Q[acc3], %R[acc3], #23                 \n"
            /*
             * store 4 elements
             */
            "   strd            %Q[acc3], %Q[acc2], [%[pOutput]], #8    \n"
            "   strd            %Q[acc1], %Q[acc0], [%[pOutput]], #8    \n"

            /* low overhead loop end */
            "   le              lr, 2b                                  \n"
            "1:                                                         \n"

            FIR_Q31_FORGET_COEFS(coefPtr)

        : [pSamples] "+r" (pSamples), [pap] "+r" (partial_accu_ptr),
          [pOutput] "+r" (pOutput),
          [acc0] "=&r"(acc0),[acc1] "=&r"(acc1),
          [acc2] "=&r"(acc2),[acc3] "=&r"(acc3)
        : [scratch] "r" (scratch)
        : FIR_Q31_8_COEFS_CLOB_QLIST,
          "memory", "r14");
    }    else if (localTaps > 0) {
        __asm volatile(
            /* Coefs load  */
            "coefPtr   .req lr                                          \n"
            "   ldr             coefPtr, [%[scratch], #4]               \n"

            FIR_Q31_LOAD_4_COEFS(coefPtr)

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
            "   ldrd            %Q[acc2], %R[acc2], [%[pap]], #8        \n"
            "   ldrd            %Q[acc3], %R[acc3], [%[pap]], #8        \n"

            FIR_Q31_FILTER_4_COEFS(acc0, pSamples, 12, ACCUMULATE)

            FIR_Q31_FILTER_4_COEFS(acc1, pSamples, 8, ACCUMULATE)

            FIR_Q31_FILTER_4_COEFS(acc2, pSamples, 4, ACCUMULATE)

            /* advance sample ptr to next block */
            FIR_Q31_FILTER_4_COEFS_POSTINCR(acc3, pSamples, 16, ACCUMULATE)

            /* .54-> .31 conversion and store accumulators */
            "   asrl            %Q[acc0], %R[acc0], #23                 \n"
            "   asrl            %Q[acc1], %R[acc1], #23                 \n"
            "   asrl            %Q[acc2], %R[acc2], #23                 \n"
            "   asrl            %Q[acc3], %R[acc3], #23                 \n"
            /*
             * store 4 elements
             */
            "   strd            %Q[acc3], %Q[acc2], [%[pOutput]], #8    \n"
            "   strd            %Q[acc1], %Q[acc0], [%[pOutput]], #8    \n"

            /* low overhead loop end */
            "   le              lr, 2b                                  \n"
            "1:                                                         \n"

            FIR_Q31_FORGET_COEFS(coefPtr)

        : [pSamples] "+r" (pSamples), [pap] "+r" (partial_accu_ptr),
          [pOutput] "+r" (pOutput),
          [acc0] "=&r"(acc0),[acc1] "=&r"(acc1),
          [acc2] "=&r"(acc2),[acc3] "=&r"(acc3)
        : [scratch] "r" (scratch)
        : FIR_Q31_4_COEFS_CLOB_QLIST,
          "memory", "r14");
    }

    /*
     * Copy the samples back into the history buffer start
     */
    pTempSrc = &pState[blockSize];
    pTempDest = pState;

    COPY_STATE_32(numTaps, pTempSrc, pTempDest);
}


void arm_fir_q31_mve(
  const arm_fir_instance_q31 * S,
  const q31_t * pSrc,
        q31_t * pDst,
        uint32_t blockSize)
{
    uint32_t     numTaps = S->numTaps; /* Number of filter coefficients in the filter */

    /*
     * 1 to 28 taps specialized routines
     */
    if (numTaps <= 4)
    {
        arm_fir_q31_1_4_mve(S, pSrc, pDst, blockSize);
        return;
    }
    else if (numTaps <= 8)
    {
        arm_fir_q31_5_8_mve(S, pSrc, pDst, blockSize);
        return;
    }
    else if (numTaps <= 12)
    {
        arm_fir_q31_9_12_mve(S, pSrc, pDst, blockSize);
        return;
    }
    else if (numTaps <= 16)
    {
        arm_fir_q31_13_16_mve(S, pSrc, pDst, blockSize);
        return;
    }
    else if (numTaps <= 20)
    {
        arm_fir_q31_17_20_mve(S, pSrc, pDst, blockSize);
        return;
    }
    else if (numTaps <= 24)
    {
        arm_fir_q31_21_24_mve(S, pSrc, pDst, blockSize);
        return;
    }
    else if (numTaps <= 28)
    {
        arm_fir_q31_25_28_mve(S, pSrc, pDst, blockSize);
        return;
    }
    /*
     * Generic version using 28 coefs block filtering
     */
    else
    {
        arm_fir_q31_gt_28_mve(S, pSrc, pDst, blockSize);
    }
}

#else

/* CMSIS DSP MVE intrinsic reference */

#define FIR_Q31_CORE(nbAcc, nbVecTaps, pSample, vecCoeffs)                 \
        for (int j = 0; j < nbAcc; j++) {                                  \
            const q31_t    *pSmp = &pSamples[j];                           \
            q31x4_t         vecIn0;                                        \
            q63_t           acc[4];                                        \
                                                                           \
            acc[j] = 0;                                                    \
            for (int i = 0; i < nbVecTaps; i++) {                          \
                vecIn0 = vld1q(pSmp + 4 * i);                  \
                acc[j] = vrmlaldavhaq(acc[j], vecIn0, vecCoeffs[i]);       \
            }                                                              \
            *pOutput++ = (q31_t)asrl(acc[j], 23);                          \
        }


#define FIR_Q31_CORE_STR_PARTIAL(nbAcc, nbVecTaps, pSample, vecCoeffs)     \
        for (int j = 0; j < nbAcc; j++) {                                  \
            const q31_t    *pSmp = &pSamples[j];                           \
            q31x4_t         vecIn0;                                        \
                                                                           \
            acc[j] = 0;                                                    \
            for (int i = 0; i < nbVecTaps; i++) {                          \
                vecIn0 = vld1q(pSmp + 4 * i);                  \
                acc[j] = vrmlaldavhaq(acc[j], vecIn0, vecCoeffs[i]);       \
            }                                                              \
            *arm_fir_partial_accu_ptr++ = acc[j];                          \
        }


#define FIR_Q31_CORE_LD_PARTIAL(nbAcc, nbVecTaps, pSample, vecCoeffs)      \
        for (int j = 0; j < nbAcc; j++) {                                  \
            const q31_t    *pSmp = &pSamples[j];                           \
            q31x4_t         vecIn0;                                        \
                                                                           \
            acc[j] = *arm_fir_partial_accu_ptr++;                          \
                                                                           \
            for (int i = 0; i < nbVecTaps; i++) {                          \
                vecIn0 = vld1q(pSmp + 4 * i);                  \
                acc[j] = vrmlaldavhaq(acc[j], vecIn0, vecCoeffs[i]);       \
            }                                                              \
            *pOutput++ = (q31_t)asrl(acc[j], 23);                          \
        }


#define FIR_Q31_MAIN_CORE()                                                              \
{                                                                                        \
    q31_t *pRefStatePtr = S->pState + 2*ROUND_UP(blockSize, 4);                          \
    q31_t      *pState = pRefStatePtr; /* State pointer */                               \
    const q31_t *pCoeffs = S->pCoeffs;  /* Coefficient pointer */                        \
    q31_t       *pStateCur;             /* Points to the current sample of the state */  \
    const q31_t *pSamples;              /* Temporary pointer to the sample buffer */     \
    q31_t       *pOutput;               /* Temporary pointer to the output buffer */     \
    const q31_t *pTempSrc;              /* Temporary pointer to the source data */       \
    q31_t       *pTempDest;             /* Temporary pointer to the destination buffer */\
    uint32_t     numTaps = S->numTaps;  /* Number of filter coefficients in the filter */\
    int32_t      blkCnt;                                                                 \
                                                                                         \
    /*                                                                                   \
     * load coefs                                                                        \
     */                                                                                  \
    q31x4_t         vecCoeffs[NBVECTAPS];                                                \
                                                                                         \
    for (int i = 0; i < NBVECTAPS; i++)                                                  \
        vecCoeffs[i] = vld1q(pCoeffs + 4 * i);                                           \
                                                                                         \
    /*                                                                                   \
     * pState points to state array which contains previous frame (numTaps - 1) samples  \
     * pStateCur points to the location where the new input data should be written       \
     */                                                                                  \
    pStateCur = &(pState[(numTaps - 1u)]);                                               \
    pTempSrc = pSrc;                                                                     \
    pSamples = pState;                                                                   \
    pOutput = pDst;                                                                      \
                                                                                         \
    blkCnt = blockSize >> 2;                                                             \
    while (blkCnt > 0) {                                                                 \
        /*                                                                               \
         * Save 4 input samples in the history buffer                                    \
         */                                                                              \
        vstrwq_s32(pStateCur, vldrwq_s32(pTempSrc));                                     \
        pStateCur += 4;                                                                  \
        pTempSrc += 4;                                                                   \
                                                                                         \
        FIR_Q31_CORE(4, NBVECTAPS, pSamples, vecCoeffs);                                 \
                                                                                         \
        pSamples += 4;                                                                   \
        /*                                                                               \
         * Decrement the sample block loop counter                                       \
         */                                                                              \
        blkCnt--;                                                                        \
    }                                                                                    \
                                                                                         \
    /* tail */                                                                           \
    int32_t        residual = blockSize & 3;                                             \
    switch (residual) {                                                                  \
      case 3:                                                                            \
          {                                                                              \
              for (int i = 0; i < residual; i++)                                         \
                  *pStateCur++ = *pTempSrc++;                                            \
                                                                                         \
              FIR_Q31_CORE(3, NBVECTAPS, pSamples, vecCoeffs);                           \
          }                                                                              \
          break;                                                                         \
                                                                                         \
      case 2:                                                                            \
          {                                                                              \
              for (int i = 0; i < residual; i++)                                         \
                  *pStateCur++ = *pTempSrc++;                                            \
                                                                                         \
               FIR_Q31_CORE(2, NBVECTAPS, pSamples, vecCoeffs);                          \
          }                                                                              \
          break;                                                                         \
                                                                                         \
      case 1:                                                                            \
          {                                                                              \
              for (int i = 0; i < residual; i++)                                         \
                  *pStateCur++ = *pTempSrc++;                                            \
                                                                                         \
              FIR_Q31_CORE(1, NBVECTAPS, pSamples, vecCoeffs);                           \
          }                                                                              \
          break;                                                                         \
    }                                                                                    \
                                                                                         \
    /*                                                                                   \
     * Copy the samples back into the history buffer start                               \
     */                                                                                  \
    pTempSrc = &pState[blockSize];                                                       \
    pTempDest = pState;                                                                  \
                                                                                         \
    blkCnt =(numTaps - 1) >> 2;                                                          \
    while (blkCnt > 0)                                                                   \
    {                                                                                    \
        vstrwq_s32(pTempDest, vldrwq_s32(pTempSrc));                                     \
        pTempSrc += 4;                                                                   \
        pTempDest += 4;                                                                  \
        blkCnt--;                                                                        \
    }                                                                                    \
    blkCnt = (numTaps - 1) & 3;                                                          \
    if (blkCnt > 0)                                                                      \
    {                                                                                    \
        mve_pred16_t p0 = vctp32q(blkCnt);                                               \
        vstrwq_p_s32(pTempDest, vldrwq_z_s32(pTempSrc, p0), p0);                         \
    }                                                                                    \
}

static void arm_fir_q31_1_4_mve(const arm_fir_instance_q31 * S,
    const q31_t * __restrict pSrc,
    q31_t * __restrict pDst, uint32_t blockSize)
{
    q31_t *pRefStatePtr = S->pState + 2*ROUND_UP(blockSize, 4);
    q31_t      *pState = pRefStatePtr; /* State pointer */
    const q31_t    *pCoeffs = S->pCoeffs; /* Coefficient pointer */
    q31_t    *pStateCur;        /* Points to the current sample of the state */
    const q31_t    *pSamples;         /* Temporary pointer to the sample buffer */
    q31_t    *pOutput;          /* Temporary pointer to the output buffer */
    const q31_t    *pTempSrc;         /* Temporary pointer to the source data */
    q31_t    *pTempDest;        /* Temporary pointer to the destination buffer */
    uint32_t  numTaps = S->numTaps; /* Number of filter coefficients in the filter */
    uint32_t  blkCnt;
    q31x4_t vecIn0;


    /*
     * pState points to state array which contains previous frame (numTaps - 1) samples
     * pStateCur points to the location where the new input data should be written
     */
    pStateCur = &(pState[(numTaps - 1u)]);
    pTempSrc = pSrc;
    pSamples = pState;
    pOutput = pDst;

    q63_t     acc0=0, acc1=0, acc2=0, acc3=0;
    /*
     * load 4 coefs
     */
    q31x4_t vecCoeffs = *(q31x4_t *) pCoeffs;

    blkCnt = blockSize >> 2;
    while (blkCnt > 0U)
    {
        const q31_t    *pSamplesTmp = pSamples;

        /*
         * Save 4 input samples in the history buffer
         */
        vst1q(pStateCur, vld1q(pTempSrc));
        pStateCur += 4;
        pTempSrc += 4;

        vecIn0 = vld1q(pSamplesTmp);
        acc0 = vrmlaldavhq(vecIn0, vecCoeffs);

        vecIn0 = vld1q(&pSamplesTmp[1]);
        acc1 = vrmlaldavhq(vecIn0, vecCoeffs);

        vecIn0 = vld1q(&pSamplesTmp[2]);
        acc2 = vrmlaldavhq(vecIn0, vecCoeffs);

        vecIn0 = vld1q(&pSamplesTmp[3]);
        acc3 = vrmlaldavhq(vecIn0, vecCoeffs);

        acc0 = asrl(acc0, 23);
        acc1 = asrl(acc1, 23);
        acc2 = asrl(acc2, 23);
        acc3 = asrl(acc3, 23);

        *pOutput++ = (q31_t) acc0;
        *pOutput++ = (q31_t) acc1;
        *pOutput++ = (q31_t) acc2;
        *pOutput++ = (q31_t) acc3;

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
            /*
             * Save 4 input samples in the history buffer
             */
            *(q31x4_t *) pStateCur = *(q31x4_t *) pTempSrc;
            pStateCur += 4;
            pTempSrc += 4;

            vecIn0 = vld1q(pSamples);
            acc0 = vrmlaldavhq(vecIn0, vecCoeffs);

            vecIn0 = vld1q(&pSamples[1]);
            acc1 = vrmlaldavhq(vecIn0, vecCoeffs);

            vecIn0 = vld1q(&pSamples[2]);
            acc2 = vrmlaldavhq(vecIn0, vecCoeffs);

            acc0 = asrl(acc0, 23);
            acc1 = asrl(acc1, 23);
            acc2 = asrl(acc2, 23);

            *pOutput++ = (q31_t) acc0;
            *pOutput++ = (q31_t) acc1;
            *pOutput++ = (q31_t) acc2;
        }
        break;

    case 2:
        {
            /*
             * Save 4 input samples in the history buffer
             */
            vst1q(pStateCur, vld1q(pTempSrc));
            pStateCur += 4;
            pTempSrc += 4;

            vecIn0 = vld1q(pSamples);
            acc0 = vrmlaldavhq(vecIn0, vecCoeffs);

            vecIn0 = vld1q(&pSamples[1]);
            acc1 = vrmlaldavhq(vecIn0, vecCoeffs);

            acc0 = asrl(acc0, 23);
            acc1 = asrl(acc1, 23);

            *pOutput++ = (q31_t) acc0;
            *pOutput++ = (q31_t) acc1;
        }
        break;

    case 1:
        {
            /*
             * Save 4 input samples in the history buffer
             */
            vst1q(pStateCur, vld1q(pTempSrc));
            pStateCur += 4;
            pTempSrc += 4;

            vecIn0 = vld1q(pSamples);
            acc0 = vrmlaldavhq(vecIn0, vecCoeffs);

            acc0 = asrl(acc0, 23);

            *pOutput++ = (q31_t) acc0;
        }
        break;
    }

    /*
     * Copy the samples back into the history buffer start
     */
    pTempSrc = &pState[blockSize];
    pTempDest = pState;

    blkCnt = (numTaps-1) >> 2;
    while (blkCnt > 0U)
    {
        vst1q(pTempDest, vld1q(pTempSrc));
        pTempSrc += 4;
        pTempDest += 4;
        blkCnt--;
    }
    blkCnt = (numTaps-1) & 3;
    if (blkCnt > 0U)
    {
        mve_pred16_t p0 = vctp32q(blkCnt);
        vstrwq_p_s32(pTempDest, vld1q(pTempSrc), p0);
    }
}



static void arm_fir_q31_5_8_mve(const arm_fir_instance_q31 * S,
    const q31_t * __restrict pSrc,
    q31_t * __restrict pDst, uint32_t blockSize)
{
    #define NBTAPS 8
    #define NBVECTAPS (NBTAPS / 4)
    FIR_Q31_MAIN_CORE();
    #undef NBVECTAPS
    #undef NBTAPS
}


static void arm_fir_q31_9_12_mve(const arm_fir_instance_q31 * S,
    const q31_t * __restrict pSrc,
    q31_t * __restrict pDst, uint32_t blockSize)
{
    #define NBTAPS 12
    #define NBVECTAPS (NBTAPS / 4)
    FIR_Q31_MAIN_CORE();
    #undef NBVECTAPS
    #undef NBTAPS
}


static void arm_fir_q31_13_16_mve(const arm_fir_instance_q31 * S,
    const q31_t * __restrict pSrc,
    q31_t * __restrict pDst, uint32_t blockSize)
{
    #define NBTAPS 16
    #define NBVECTAPS (NBTAPS / 4)
    FIR_Q31_MAIN_CORE();
    #undef NBVECTAPS
    #undef NBTAPS
}


static void arm_fir_q31_17_20_mve(const arm_fir_instance_q31 * S,
    const q31_t * __restrict pSrc,
    q31_t * __restrict pDst, uint32_t blockSize)
{
    #define NBTAPS 20
    #define NBVECTAPS (NBTAPS / 4)
    FIR_Q31_MAIN_CORE();
    #undef NBVECTAPS
    #undef NBTAPS
}


static void arm_fir_q31_21_24_mve(const arm_fir_instance_q31 * S,
    const q31_t * __restrict pSrc,
    q31_t * __restrict pDst, uint32_t blockSize)
{
    #define NBTAPS 24
    #define NBVECTAPS (NBTAPS / 4)
    FIR_Q31_MAIN_CORE();
    #undef NBVECTAPS
    #undef NBTAPS
}


static void arm_fir_q31_25_28_mve(const arm_fir_instance_q31 * S,
    const q31_t * __restrict pSrc,
    q31_t * __restrict pDst, uint32_t blockSize)
{
    #define NBTAPS 28
    #define NBVECTAPS (NBTAPS / 4)
    FIR_Q31_MAIN_CORE();
    #undef NBVECTAPS
    #undef NBTAPS
}

static void arm_fir_q31_29_32_mve(const arm_fir_instance_q31 * S,
    const q31_t * __restrict pSrc,
    q31_t * __restrict pDst,
                               uint32_t blockSize)
{
    q31_t *pRefStatePtr = S->pState + 2*ROUND_UP(blockSize, 4);
    q31_t      *pState = pRefStatePtr; /* State pointer */
    const q31_t    *pCoeffs = S->pCoeffs;       /* Coefficient pointer */
    q31_t          *pStateCur;  /* Points to the current sample of the state */
    const q31_t    *pSamples;   /* Temporary pointer to the sample buffer */
    q31_t          *pOutput;    /* Temporary pointer to the output buffer */
    const q31_t    *pTempSrc;   /* Temporary pointer to the source data */
    q31_t          *pTempDest;  /* Temporary pointer to the destination buffer */
    uint32_t        numTaps = S->numTaps;       /* Number of filter coefficients in the filter */
    int32_t         blkCnt;
    q63_t           acc0, acc1, acc2, acc3;

#define MAX_VECT_BATCH 7

    /*
     * pre-load 28 1st coefs
     */
    q31x4_t         vecCoeffs0 = vld1q(pCoeffs + 4 * 0);
    q31x4_t         vecCoeffs1 = vld1q(pCoeffs + 4 * 1);
    q31x4_t         vecCoeffs2 = vld1q(pCoeffs + 4 * 2);
    q31x4_t         vecCoeffs3 = vld1q(pCoeffs + 4 * 3);
    q31x4_t         vecCoeffs4 = vld1q(pCoeffs + 4 * 4);
    q31x4_t         vecCoeffs5 = vld1q(pCoeffs + 4 * 5);
    q31x4_t         vecCoeffs6 = vld1q(pCoeffs + 4 * 6);

    /*
     * pState points to state array which contains previous frame (numTaps - 1) samples
     * pStateCur points to the location where the new input data should be written
     */
    pStateCur = &(pState[(numTaps - 1u)]);
    pTempSrc = pSrc;
    pSamples = pState;

    q63_t          *arm_fir_partial_accu_ptr = (q63_t*)S->pState;

    blkCnt = blockSize >> 2;
    while (blkCnt > 0) {
        /*
         * Save 4 input samples in the history buffer
         */
        vstrwq_s32(pStateCur, vldrwq_s32(pTempSrc));
        pStateCur += 4;
        pTempSrc += 4;

        const q31_t    *pSmp;
        q31x4_t         vecIn0;

        pSmp = &pSamples[0];

        vecIn0 = vld1q(pSmp);
        acc0 = vrmlaldavhq(vecIn0, vecCoeffs0);
        vecIn0 = vld1q(pSmp + 4 * 1);
        acc0 = vrmlaldavhaq(acc0, vecIn0, vecCoeffs1);
        vecIn0 = vld1q(pSmp + 4 * 2);
        acc0 = vrmlaldavhaq(acc0, vecIn0, vecCoeffs2);
        vecIn0 = vld1q(pSmp + 4 * 3);
        acc0 = vrmlaldavhaq(acc0, vecIn0, vecCoeffs3);
        vecIn0 = vld1q(pSmp + 4 * 4);
        acc0 = vrmlaldavhaq(acc0, vecIn0, vecCoeffs4);
        vecIn0 = vld1q(pSmp + 4 * 5);
        acc0 = vrmlaldavhaq(acc0, vecIn0, vecCoeffs5);
        vecIn0 = vld1q(pSmp + 4 * 6);
        acc0 = vrmlaldavhaq(acc0, vecIn0, vecCoeffs6);

        *arm_fir_partial_accu_ptr++ = acc0;

        pSmp = &pSamples[1];

        vecIn0 = vld1q(pSmp);
        acc1 = vrmlaldavhq(vecIn0, vecCoeffs0);
        vecIn0 = vld1q(pSmp + 4 * 1);
        acc1 = vrmlaldavhaq(acc1, vecIn0, vecCoeffs1);
        vecIn0 = vld1q(pSmp + 4 * 2);
        acc1 = vrmlaldavhaq(acc1, vecIn0, vecCoeffs2);
        vecIn0 = vld1q(pSmp + 4 * 3);
        acc1 = vrmlaldavhaq(acc1, vecIn0, vecCoeffs3);
        vecIn0 = vld1q(pSmp + 4 * 4);
        acc1 = vrmlaldavhaq(acc1, vecIn0, vecCoeffs4);
        vecIn0 = vld1q(pSmp + 4 * 5);
        acc1 = vrmlaldavhaq(acc1, vecIn0, vecCoeffs5);
        vecIn0 = vld1q(pSmp + 4 * 6);
        acc1 = vrmlaldavhaq(acc1, vecIn0, vecCoeffs6);

        *arm_fir_partial_accu_ptr++ = acc1;

        pSmp = &pSamples[2];

        vecIn0 = vld1q(pSmp);
        acc2 = vrmlaldavhq(vecIn0, vecCoeffs0);
        vecIn0 = vld1q(pSmp + 4 * 1);
        acc2 = vrmlaldavhaq(acc2, vecIn0, vecCoeffs1);
        vecIn0 = vld1q(pSmp + 4 * 2);
        acc2 = vrmlaldavhaq(acc2, vecIn0, vecCoeffs2);
        vecIn0 = vld1q(pSmp + 4 * 3);
        acc2 = vrmlaldavhaq(acc2, vecIn0, vecCoeffs3);
        vecIn0 = vld1q(pSmp + 4 * 4);
        acc2 = vrmlaldavhaq(acc2, vecIn0, vecCoeffs4);
        vecIn0 = vld1q(pSmp + 4 * 5);
        acc2 = vrmlaldavhaq(acc2, vecIn0, vecCoeffs5);
        vecIn0 = vld1q(pSmp + 4 * 6);
        acc2 = vrmlaldavhaq(acc2, vecIn0, vecCoeffs6);
        *arm_fir_partial_accu_ptr++ = acc2;

        pSmp = &pSamples[3];

        vecIn0 = vld1q(pSmp);
        acc3 = vrmlaldavhq(vecIn0, vecCoeffs0);
        vecIn0 = vld1q(pSmp + 4 * 1);
        acc3 = vrmlaldavhaq(acc3, vecIn0, vecCoeffs1);
        vecIn0 = vld1q(pSmp + 4 * 2);
        acc3 = vrmlaldavhaq(acc3, vecIn0, vecCoeffs2);
        vecIn0 = vld1q(pSmp + 4 * 3);
        acc3 = vrmlaldavhaq(acc3, vecIn0, vecCoeffs3);
        vecIn0 = vld1q(pSmp + 4 * 4);
        acc3 = vrmlaldavhaq(acc3, vecIn0, vecCoeffs4);
        vecIn0 = vld1q(pSmp + 4 * 5);
        acc3 = vrmlaldavhaq(acc3, vecIn0, vecCoeffs5);
        vecIn0 = vld1q(pSmp + 4 * 6);
        acc3 = vrmlaldavhaq(acc3, vecIn0, vecCoeffs6);

        *arm_fir_partial_accu_ptr++ = acc3;

        pSamples += 4;
        /*
         * Decrement the sample block loop counter
         */
        blkCnt--;
    }


    /* reminder */

    /* load last 4 coef */
    vecCoeffs0 = vld1q(pCoeffs + 4 * MAX_VECT_BATCH);
    arm_fir_partial_accu_ptr = (q63_t*)S->pState;
    pOutput = pDst;
    pSamples = pState + (MAX_VECT_BATCH * 4);


    blkCnt = blockSize >> 2;
    while (blkCnt > 0) {
        q31x4_t         vecIn0;

        /* reload intermediate MAC */
        acc0 = *arm_fir_partial_accu_ptr++;
        acc1 = *arm_fir_partial_accu_ptr++;
        acc2 = *arm_fir_partial_accu_ptr++;
        acc3 = *arm_fir_partial_accu_ptr++;


        vecIn0 = vld1q(&pSamples[0]);
        acc0 = vrmlaldavhaq(acc0, vecIn0, vecCoeffs0);

        vecIn0 = vld1q(&pSamples[1]);
        acc1 = vrmlaldavhaq(acc1, vecIn0, vecCoeffs0);

        vecIn0 = vld1q(&pSamples[2]);
        acc2 = vrmlaldavhaq(acc2, vecIn0, vecCoeffs0);

        vecIn0 = vld1q(&pSamples[3]);
        acc3 = vrmlaldavhaq(acc3, vecIn0, vecCoeffs0);

        *pOutput++ = asrl(acc0, 23);
        *pOutput++ = asrl(acc1, 23);
        *pOutput++ = asrl(acc2, 23);
        *pOutput++ = asrl(acc3, 23);

        pSamples += 4;
        /*
         * Decrement the sample block loop counter
         */
        blkCnt--;
    }

    /*
     * Copy the samples back into the history buffer start
     */
    pTempSrc = &pState[blockSize];
    pTempDest = pState;

    blkCnt = numTaps - 1;
    do {
        mve_pred16_t    p = vctp32q(blkCnt);

        vstrwq_p_s32(pTempDest, vldrwq_z_s32(pTempSrc, p), p);
        pTempSrc += 4;
        pTempDest += 4;
        blkCnt -= 4;
    }
    while (blkCnt > 0);
}



void arm_fir_q31_mve(
  const arm_fir_instance_q31 * S,
  const q31_t * pSrc,
        q31_t * pDst,
        uint32_t blockSize)
{
    q31_t *pRefStatePtr = S->pState + 2*ROUND_UP(blockSize, 4);
    q31_t      *pState = pRefStatePtr; /* State pointer */
    const q31_t    *pCoeffs = S->pCoeffs; /* Coefficient pointer */
    q31_t    *pStateCur;        /* Points to the current sample of the state */
    const q31_t    *pSamples;         /* Temporary pointer to the sample buffer */
    q31_t    *pOutput;          /* Temporary pointer to the output buffer */
    const q31_t    *pTempSrc;         /* Temporary pointer to the source data */
    q31_t    *pTempDest;        /* Temporary pointer to the destination buffer */
    uint32_t  numTaps = S->numTaps; /* Number of filter coefficients in the filter */
    uint32_t  blkCnt;
    q31x4_t vecIn0;
    uint32_t  tapsBlkCnt = (numTaps + 3) / 4;
    q63_t     acc0, acc1, acc2, acc3;
    q31x4_t vecCoeffs;


    /*
     * [1 to 32 taps] specialized routines
     */
    if (numTaps <= 4)
    {
        arm_fir_q31_1_4_mve(S, pSrc, pDst, blockSize);
        return;
    }
    else if (numTaps <= 8)
    {
        arm_fir_q31_5_8_mve(S, pSrc, pDst, blockSize);
        return;
    }
    else if (numTaps <= 12)
    {
        arm_fir_q31_9_12_mve(S, pSrc, pDst, blockSize);
        return;
    }
    else if (numTaps <= 16)
    {
        arm_fir_q31_13_16_mve(S, pSrc, pDst, blockSize);
        return;
    }
    else if (numTaps <= 20)
    {
        arm_fir_q31_17_20_mve(S, pSrc, pDst, blockSize);
        return;
    }
    else if (numTaps <= 24)
    {
        arm_fir_q31_21_24_mve(S, pSrc, pDst, blockSize);
        return;
    }
    else if (numTaps <= 28)
    {
        arm_fir_q31_25_28_mve(S, pSrc, pDst, blockSize);
        return;
    }
    else if ((numTaps <= 32)  && (blockSize >= 32))
    {
        arm_fir_q31_29_32_mve(S, pSrc, pDst, blockSize);
        return;
    }

    /*
     * pState points to state array which contains previous frame (numTaps - 1) samples
     * pStateCur points to the location where the new input data should be written
     */
    pStateCur   = &(pState[(numTaps - 1u)]);
    pSamples    = pState;
    pTempSrc    = pSrc;
    pOutput     = pDst;
    blkCnt      = blockSize >> 2;
    while (blkCnt > 0)
    {
        const q31_t    *pCoeffsTmp = pCoeffs;
        const q31_t    *pSamplesTmp = pSamples;

        acc0 = 0LL;
        acc1 = 0LL;
        acc2 = 0LL;
        acc3 = 0LL;

        /*
         * Save 4 input samples in the history buffer
         */
        vst1q(pStateCur, vld1q(pTempSrc));
        pStateCur += 4;
        pTempSrc += 4;

        int       i = tapsBlkCnt;
        while (i > 0)
        {
            /*
             * load 4 coefs
             */
            vecCoeffs = *(q31x4_t *) pCoeffsTmp;

            vecIn0 = vld1q(pSamplesTmp);
            acc0 = vrmlaldavhaq(acc0, vecIn0, vecCoeffs);

            vecIn0 = vld1q(&pSamplesTmp[1]);
            acc1 = vrmlaldavhaq(acc1, vecIn0, vecCoeffs);

            vecIn0 = vld1q(&pSamplesTmp[2]);
            acc2 = vrmlaldavhaq(acc2, vecIn0, vecCoeffs);

            vecIn0 = vld1q(&pSamplesTmp[3]);
            acc3 = vrmlaldavhaq(acc3, vecIn0, vecCoeffs);

            pSamplesTmp += 4;
            pCoeffsTmp += 4;
            /*
             * Decrement the taps block loop counter
             */
            i--;
        }

        /* .54-> .31 conversion and store accumulators */
        acc0 = asrl(acc0, 23);
        acc1 = asrl(acc1, 23);
        acc2 = asrl(acc2, 23);
        acc3 = asrl(acc3, 23);

        *pOutput++ = (q31_t) acc0;
        *pOutput++ = (q31_t) acc1;
        *pOutput++ = (q31_t) acc2;
        *pOutput++ = (q31_t) acc3;

        pSamples += 4;

        /*
         * Decrement the sample block loop counter
         */
        blkCnt--;
    }

    int32_t  residual = blockSize & 3;
    switch (residual)
    {
    case 3:
        {
            const q31_t    *pCoeffsTmp = pCoeffs;
            const q31_t    *pSamplesTmp = pSamples;

            acc0 = 0LL;
            acc1 = 0LL;
            acc2 = 0LL;

            /*
             * Save 4 input samples in the history buffer
             */
            *(q31x4_t *) pStateCur = *(q31x4_t *) pTempSrc;
            pStateCur += 4;
            pTempSrc += 4;

            int       i = tapsBlkCnt;
            while (i > 0)
            {
                vecCoeffs = *(q31x4_t *) pCoeffsTmp;

                vecIn0 = vld1q(pSamplesTmp);
                acc0 = vrmlaldavhaq(acc0, vecIn0, vecCoeffs);

                vecIn0 = vld1q(&pSamplesTmp[1]);
                acc1 = vrmlaldavhaq(acc1, vecIn0, vecCoeffs);

                vecIn0 = vld1q(&pSamplesTmp[2]);
                acc2 = vrmlaldavhaq(acc2, vecIn0, vecCoeffs);

                pSamplesTmp += 4;
                pCoeffsTmp += 4;
                i--;
            }

            acc0 = asrl(acc0, 23);
            acc1 = asrl(acc1, 23);
            acc2 = asrl(acc2, 23);

            *pOutput++ = (q31_t) acc0;
            *pOutput++ = (q31_t) acc1;
            *pOutput++ = (q31_t) acc2;
        }
        break;

    case 2:
        {
            const q31_t    *pCoeffsTmp = pCoeffs;
            const q31_t    *pSamplesTmp = pSamples;

            acc0 = 0LL;
            acc1 = 0LL;

            /*
             * Save 4 input samples in the history buffer
             */
            vst1q(pStateCur, vld1q(pTempSrc));
            pStateCur += 4;
            pTempSrc += 4;

            int       i = tapsBlkCnt;
            while (i > 0)
            {
                vecCoeffs = *(q31x4_t *) pCoeffsTmp;

                vecIn0 = vld1q(pSamplesTmp);
                acc0 = vrmlaldavhaq(acc0, vecIn0, vecCoeffs);

                vecIn0 = vld1q(&pSamplesTmp[1]);
                acc1 = vrmlaldavhaq(acc1, vecIn0, vecCoeffs);

                pSamplesTmp += 4;
                pCoeffsTmp += 4;
                i--;
            }

            acc0 = asrl(acc0, 23);
            acc1 = asrl(acc1, 23);

            *pOutput++ = (q31_t) acc0;
            *pOutput++ = (q31_t) acc1;
        }
        break;

    case 1:
        {
            const q31_t    *pCoeffsTmp = pCoeffs;
            const q31_t    *pSamplesTmp = pSamples;

            acc0 = 0LL;

            /*
             * Save 4 input samples in the history buffer
             */
            vst1q(pStateCur, vld1q(pTempSrc));
            pStateCur += 4;
            pTempSrc += 4;

            int       i = tapsBlkCnt;
            while (i > 0)
            {
                vecCoeffs = *(q31x4_t *) pCoeffsTmp;

                vecIn0 = vld1q(pSamplesTmp);
                acc0 = vrmlaldavhaq(acc0, vecIn0, vecCoeffs);

                pSamplesTmp += 4;
                pCoeffsTmp += 4;
                i--;
            }

            acc0 = asrl(acc0, 23);

            *pOutput++ = (q31_t) acc0;
        }
        break;
    }

    /*
     * Copy the samples back into the history buffer start
     */
    pTempSrc = &pState[blockSize];
    pTempDest = pState;

    blkCnt = (numTaps - 1U) >> 2;
    while (blkCnt > 0)
    {
        vst1q(pTempDest, vld1q(pTempSrc));
        pTempSrc += 4;
        pTempDest += 4;
        blkCnt--;
    }
    blkCnt = (numTaps - 1U) & 3;
    if (blkCnt > 0)
    {
        mve_pred16_t p0 = vctp32q(blkCnt);
        vstrwq_p_s32(pTempDest, vld1q(pTempSrc), p0);
    }
}

#endif /* USE_ASM */
#endif /* defined(ARM_MATH_MVEI) */
/**
  @} end of FIR group
 */
