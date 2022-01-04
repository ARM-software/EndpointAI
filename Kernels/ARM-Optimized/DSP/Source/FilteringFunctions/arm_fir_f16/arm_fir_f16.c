/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_fir_f16.c
 * Description:  Floating-point FIR filter processing function
 *               using Helium assembly kernels
 *               This version allows boosting F16 FIR Filter performance when using compilers having suboptimal
 *               Helium intrinsic code generation.
 *
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
  @addtogroup FIR
  @{
 */

/**
  @brief         Processing function for floating-point FIR filter.
  @param[in]     S          points to an instance of the floating-point FIR filter structure
  @param[in]     pSrc       points to the block of input data
  @param[out]    pDst       points to the block of output data
  @param[in]     blockSize  number of samples to process
  @return        none
 */

#if defined(ARM_MATH_MVE_FLOAT16)

#define FIR_F16_MAX_COEF_BLK        8

#ifdef USE_ASM

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


/* FIR coefficients preload */

#define FIR_F16_LOAD_8_COEFS(pCoeffs)                           \
    "C8n_0   .req   r2                                     \n"  \
    "C8n_1   .req   r3                                     \n"  \
    "C8n_2   .req   r4                                     \n"  \
    "C8n_3   .req   r5                                     \n"  \
    "C8n_4   .req   r6                                     \n"  \
    "C8n_5   .req   r10                                    \n"  \
    "C8n_6   .req   r8                                     \n"  \
    "C8n_7   .req   r9                                     \n"  \
                                                                \
    "   ldrh           C8n_0, [%[pCoeffs]], #2             \n"  \
    "   ldrh           C8n_1, [%[pCoeffs]], #2             \n"  \
    "   ldrh           C8n_2, [%[pCoeffs]], #2             \n"  \
    "   ldrh           C8n_3, [%[pCoeffs]], #2             \n"  \
    "   ldrh           C8n_4, [%[pCoeffs]], #2             \n"  \
    "   ldrh           C8n_5, [%[pCoeffs]], #2             \n"  \
    "   ldrh           C8n_6, [%[pCoeffs]], #2             \n"  \
    "   ldrh           C8n_7, [%[pCoeffs]], #2             \n"


#define FIR_F16_LOAD_4_COEFS(pCoeffs)                           \
    "C4n_0   .req   r2                                     \n"  \
    "C4n_1   .req   r3                                     \n"  \
    "C4n_2   .req   r4                                     \n"  \
    "C4n_3   .req   r5                                     \n"  \
                                                                \
    "   ldrh           C4n_0, [%[pCoeffs],#0]              \n"  \
    "   ldrh           C4n_1, [%[pCoeffs],#2]              \n"  \
    "   ldrh           C4n_2, [%[pCoeffs],#4]              \n"  \
    "   ldrh           C4n_3, [%[pCoeffs],#6]              \n"



/* block filter 8 samples with 4 taps */
#define FIR_F16_FILTER_8_SAMPLS_4_COEFS(pSamples)              \
    "   vldrh.16            q0, [%["#pSamples"], #6]      \n" \
    "   vmul.f16            q1, q0, C4n_3                  \n" \
    "   vldrh.16            q0, [%["#pSamples"], #4]       \n" \
    "   vfma.f16            q1, q0, C4n_2                  \n" \
    "   vldrh.16            q0, [%["#pSamples"], #2]       \n" \
    "   vfma.f16            q1, q0, C4n_1                  \n" \
    "   vldrh.16            q0, [%["#pSamples"]], #16      \n" \
    "   vfma.f16            q1, q0, C4n_0                  \n"


/* block filter 8 samples with 8 taps */
#define FIR_F16_FILTER_8_SAMPLS_8_COEFS(pSamples)              \
    "   vldrh.16            q0, [%["#pSamples"], #14]      \n" \
    "   vmul.f16            q1, q0, C8n_7                  \n" \
    "   vldrh.16            q0, [%["#pSamples"], #12]      \n" \
    "   vfma.f16            q1, q0, C8n_6                  \n" \
    "   vldrh.16            q0, [%["#pSamples"], #10]      \n" \
    "   vfma.f16            q1, q0, C8n_5                  \n" \
    "   vldrh.16            q0, [%["#pSamples"], #8]       \n" \
    "   vfma.f16            q1, q0, C8n_4                  \n" \
    "   vldrh.16            q0, [%["#pSamples"], #6]       \n" \
    "   vfma.f16            q1, q0, C8n_3                  \n" \
    "   vldrh.16            q0, [%["#pSamples"], #4]       \n" \
    "   vfma.f16            q1, q0, C8n_2                  \n" \
    "   vldrh.16            q0, [%["#pSamples"], #2]       \n" \
    "   vfma.f16            q1, q0, C8n_1                  \n" \
    "   vldrh.16            q0, [%["#pSamples"]], #16      \n" \
    "   vfma.f16            q1, q0, C8n_0                  \n"



__STATIC_INLINE void arm_fir_f16_1_4_mve(const arm_fir_instance_f16 * S,
                      const float16_t   * __restrict pSrc,
                      float16_t         * __restrict pDst,
                      uint32_t          blockSize)
{
    float16_t       *pRefStatePtr = S->pState + blockSize;
    float16_t       *pState = pRefStatePtr;     /* State pointer */
    const float16_t *pCoeffs = S->pCoeffs;      /* Coefficient pointer */
    const float16_t *pSamples;      /* Temporary pointer to the sample buffer */
    float16_t       *pOutput;       /* Temporary pointer to the output buffer */
    const float16_t *pTempSrc;      /* Temporary pointer to the source data */
    float16_t       *pTempDest;     /* Temporary pointer to the destination buffer */


    pSamples = pState;
    pOutput = pDst;

    __asm volatile (
       /*
        * Coefs load
        */
        FIR_F16_LOAD_4_COEFS(pCoeffs)

        ".p2align 2                                                        \n"
        "   wlstp.16            lr, %[cnt], 1f                             \n"
        /* low overhead loop start */
        "2:                                                                \n"

        FIR_F16_FILTER_8_SAMPLS_4_COEFS(pSamples)
        /*
         * store 8 elements
         */
        "   vstrh.16            q1, [%[pOutput]], #16                      \n"
        "   letp                lr, 2b                                     \n"

        /* low overhead loop end */
        "1:                                                                \n"

       :[pSamples] "+r"(pSamples),[pOutput] "+r"(pOutput)
       :[cnt] "r"(blockSize), [pCoeffs] "r"(pCoeffs)
       :"q0", "q1",
        "r2", "r3", "r4", "r5",
        "r14", "memory");

    /*
     * Copy the samples back into the history buffer start
     */
    pTempSrc  = &(pState[blockSize]);
    pTempDest = pState;

    COPY_STATE_16(4, pTempSrc, pTempDest);
}



__STATIC_INLINE void arm_fir_f16_5_8_mve(const arm_fir_instance_f16 * S,
                      const float16_t   * __restrict pSrc,
                      float16_t         * __restrict pDst,
                      uint32_t          blockSize)
{
    float16_t       *pRefStatePtr = S->pState + blockSize;
    float16_t       *pState = pRefStatePtr;     /* State pointer */
    const float16_t *pCoeffs = S->pCoeffs;      /* Coefficient pointer */
    const float16_t *pSamples;      /* Temporary pointer to the sample buffer */
    float16_t       *pOutput;       /* Temporary pointer to the output buffer */
    const float16_t *pTempSrc;      /* Temporary pointer to the source data */
    float16_t       *pTempDest;     /* Temporary pointer to the destination buffer */


    pSamples = pState;
    pOutput = pDst;

    __asm volatile (
       /*
        * Coefs load
        */
        FIR_F16_LOAD_8_COEFS(pCoeffs)

        ".p2align 2                                                 \n"
        "   wlstp.16            lr, %[cnt], 1f                      \n"
        /* low overhead loop start */
        "2:                                                         \n"

        FIR_F16_FILTER_8_SAMPLS_8_COEFS(pSamples)
        /*
         * store 8 elements
         */
        "   vstrh.16            q1, [%[pOutput]], #16               \n"
        "   letp                lr, 2b                              \n"
        /* low overhead loop end */
        "1:                                                         \n"

       :[pSamples] "+r"(pSamples),[pOutput] "+r"(pOutput),
        [pCoeffs] "+r"(pCoeffs)
       :[cnt] "r"(blockSize)
       :"q0", "q1",
        "r2", "r3", "r4", "r5",
        "r6", "r10", "r8", "r9",
        "r14", "memory");

    /*
     * Copy the samples back into the history buffer start
     */
    pTempSrc  = &(pState[blockSize]);
    pTempDest = pState;

    COPY_STATE_16(8, pTempSrc, pTempDest);
}



void arm_fir_f16_mve(const arm_fir_instance_f16 * S,
                        const float16_t * pSrc,
                        float16_t       * pDst,
                        uint32_t          blockSize)
{
    float16_t       *pRefStatePtr = S->pState + blockSize;
    float16_t       *pState = pRefStatePtr ;    /* State pointer */
    const float16_t *pCoeffs = S->pCoeffs;      /* Coefficient pointer */
    const float16_t *pSamples;              /* Temporary pointer to the sample buffer */
    float16_t       *pOutput;               /* Temporary pointer to the output buffer */
    const float16_t *pTempSrc;              /* Temporary pointer to the source data */
    float16_t       *pTempDest;             /* Temporary pointer to the destination buffer */
    uint32_t         numTaps = S->numTaps;  /* Number of filter coefficients in the filter */


    pTempSrc = pSrc;
    pTempDest = &(pState[(numTaps - 1u)]);

    COPY_STATE_16(blockSize, pTempSrc, pTempDest);

    /*
     * [1 to 8 taps] specialized routines
     */
    if (numTaps <= 4)
    {
        arm_fir_f16_1_4_mve(S, pSrc, pDst, blockSize);
        return;
    }
    else if (numTaps <= 8)
    {
        arm_fir_f16_5_8_mve(S, pSrc, pDst, blockSize);
        return;
    }

    pSamples = pState;
    float16_t *partial_accu_ptr = S->pState;


    /*
     * 1st part : start from cleared accumulators and process 8 = FIR_F16_MAX_COEF_BLK
     *            taps and store partial accumulators in a temp buffer
     */

    __asm volatile (
       /*
        * Coefs load
        */
        FIR_F16_LOAD_8_COEFS(pCoeffs)

        ".p2align 2                                                     \n"
        "   wlstp.16            lr, %[cnt], 1f                          \n"

        /* low overhead loop start */
        "2:                                                             \n"

        FIR_F16_FILTER_8_SAMPLS_8_COEFS(pSamples)
        /*
         * store 8 elements
         */
        "   vstrh.16            q1, [%[partial_accu_ptr]], #16          \n"
        "   letp                lr, 2b                                  \n"
        /* low overhead loop end */
        "1:                                                             \n"

       :[pSamples] "+r"(pSamples),[partial_accu_ptr] "+r"(partial_accu_ptr),
        [pCoeffs] "+r"(pCoeffs)
       :[cnt] "r"(blockSize)
       :"q0", "q1",
        "r2", "r3", "r4", "r5",
        "r6", "r10", "r8", "r9",
        "r14", "memory");


        /*
         * 2nd part : reload partial accumulators and process next block of 8 = FIR_F16_MAX_COEF_BLK
         *            taps and keep on storign partial accumulators in temp buffer
         */

        int       localTaps = numTaps - FIR_F16_MAX_COEF_BLK;
        int       sample_offset = FIR_F16_MAX_COEF_BLK;

        while (localTaps > FIR_F16_MAX_COEF_BLK)
        {
            pSamples = pState + sample_offset;
            partial_accu_ptr =  S->pState;


            __asm volatile (
                /*
                * Coefs load
                */
                FIR_F16_LOAD_8_COEFS(pCoeffs)

                ".p2align 2                                                     \n"
                "   wlstp.16            lr, %[cnt], 1f                          \n"

                /* low overhead loop start */
                "2:                                                             \n"

                FIR_F16_FILTER_8_SAMPLS_8_COEFS(pSamples)

                /* reload partial accumulators */
                "   vldrh.16            q2, [%[partial_accu_ptr]]               \n"
                "   vadd.f16            q1, q1, q2                              \n"

                /*
                * store 8 elements
                */
                "   vstrh.16            q1, [%[partial_accu_ptr]], #16          \n"
                "   letp                lr, 2b                                  \n"

                /* low overhead loop end */
                "1:                                                             \n"

                :[pSamples] "+r"(pSamples),[partial_accu_ptr] "+r"(partial_accu_ptr),
                [pCoeffs] "+r"(pCoeffs)
                :[cnt] "r"(blockSize)
                :"q0", "q1", "q2",
                "r2", "r3", "r4", "r5",
                "r6", "r10", "r8", "r9",
                "r14", "memory");


            localTaps -= FIR_F16_MAX_COEF_BLK;
            sample_offset += FIR_F16_MAX_COEF_BLK;

        }

        /*
         * last part : Process remainders (modulo 8)
         *             Reload partial accumulators +
         *             perform last MACs and scale before storing to dest.
         */
        pOutput = pDst;
        pSamples = pState + sample_offset;
        partial_accu_ptr = S->pState;

        if (localTaps > 4)
        {
            register unsigned count  __asm("lr") = blockSize;

            __asm volatile (
               /*
                * Coefs load
                */
                FIR_F16_LOAD_8_COEFS(pCoeffs)

                ".p2align 2                                                  \n"
                "   wlstp.16            lr, lr, 1f                           \n"
                /* low overhead loop start */
                "2:                                                          \n"

                FIR_F16_FILTER_8_SAMPLS_8_COEFS(pSamples)

               /* reload partial accumulators */
                "   vldrh.16            q2, [%[partial_accu_ptr]], #16       \n"
                "   vadd.f16            q1, q1, q2                           \n"

                /*
                 * store 8 elements in filter output
                 */
                "   vstrh.16            q1, [%[pOutput]], #16                \n"
                "   letp                lr, 2b                               \n"

                /* low overhead loop end */
                "1:                                                          \n"

               :[pSamples] "+r"(pSamples),[partial_accu_ptr] "+r"(partial_accu_ptr),
                [pOutput] "+r"(pOutput), [count] "+r" (count), [pCoeffs] "+r"(pCoeffs)
               :
               :"q0", "q1", "q2",
                "r2", "r3", "r4", "r5",
                "r6", "r10", "r8", "r9",
                "memory");

        }
        else
        {
            register unsigned count  __asm("lr") = blockSize;

            __asm volatile (
                /*
                 * Coefs load
                 */
                FIR_F16_LOAD_4_COEFS(pCoeffs)

                ".p2align 2                                                  \n"
                "   wlstp.16            lr, lr, 1f                           \n"

                /* low overhead loop start */
                "2:                                                          \n"

                FIR_F16_FILTER_8_SAMPLS_4_COEFS(pSamples)

                /* reload partial accumulators */
                "   vldrh.16            q2, [%[partial_accu_ptr]], #16       \n"
                "   vadd.f16            q1, q1, q2                           \n"

                /*
                 * store 8 elements in filter output
                 */
                "   vstrh.16            q1, [%[pOutput]], #16                \n"
                "   letp                lr, 2b                               \n"

                /* low overhead loop end */
                "1:                                                          \n"

            :[pSamples] "+r"(pSamples),[partial_accu_ptr] "+r"(partial_accu_ptr),
            [pOutput] "+r"(pOutput), [count] "+r" (count)
            :[pCoeffs] "r"(pCoeffs)
            :"q0", "q1", "q2",
            "r2", "r3", "r4", "r5",
            "memory");
        }

    /*
     * Copy the samples back into the history buffer start
     */
    pTempSrc  = &(pState[blockSize]);
    pTempDest = pState;

    COPY_STATE_16(numTaps, pTempSrc, pTempDest);
}


#else

/* CMSIS DSP MVE intrinsic reference */

#define FIR_F16_CORE(pSamples, c, NB_TAPS)                                 \
        vecAcc0 = vdupq_n_f16(0.0f16);                                     \
        for (int i = 0; i < NB_TAPS; i++) {                                \
            vecIn0 = vld1q(&pSamples[i]);                                  \
            vecAcc0 = vfmaq(vecAcc0, vecIn0, c[i]);                        \
        }

#define NB_TAPS 4
__STATIC_INLINE void arm_fir_f16_1_4_mve(const arm_fir_instance_f16 * S,
    const float16_t * __restrict pSrc,
    float16_t * __restrict pDst, uint32_t blockSize)
{
    float16_t      *pState = S->pState;     /* State pointer */
    const float16_t *pCoeffs = S->pCoeffs;  /* Coefficient pointer */
    float16_t      *pStateCur;              /* Points to the current sample of the state */
    const float16_t *pSamples;              /* Temporary pointer to the sample buffer */
    float16_t      *pOutput;                /* Temporary pointer to the output buffer */
    const float16_t *pTempSrc;              /* Temporary pointer to the source data */
    float16_t      *pTempDest;              /* Temporary pointer to the destination buffer */
    uint32_t        numTaps = S->numTaps;   /* Number of filter coefficients in the filter */
    int32_t         blkCnt;
    float16x8_t         vecIn0;
    float16x8_t         vecAcc0;
    float16_t       c[NB_TAPS];


    /*
     * pState points to state array which contains previous frame (numTaps - 1) samples
     * pStateCur points to the location where the new input data should be written
     */
    pStateCur = &(pState[(numTaps - 1u)]);
    /*
     * Copy new data into state so that we obtain a continuous sample buffer
     * containing both the tail end of the old data and the new data.
     */
    pSamples = pState;
    pTempSrc = pSrc;
    pOutput = pDst;

    for (int i = 0; i < NB_TAPS; i++)
        c[i] = pCoeffs[i];

    blkCnt = blockSize >> 3;
    while (blkCnt > 0) {
        /*
         * Save 8 input samples in the history buffer
         */
        vst1q(pStateCur, vld1q(pTempSrc));
        pStateCur += 8;
        pTempSrc += 8;

        FIR_F16_CORE(pSamples, c, NB_TAPS);

        vst1q(pOutput, vecAcc0);

        pOutput += 8;
        pSamples += 8;

        blkCnt--;
    }

    blkCnt = blockSize & 7;
    if (blkCnt)
    {
        mve_pred16_t    p0 = vctp16q(blkCnt);

        vst1q(pStateCur, vld1q(pTempSrc));
        pStateCur += 8;
        pTempSrc += 8;

        FIR_F16_CORE(pSamples, c, NB_TAPS);

        vstrhq_p_f16(pOutput, vecAcc0, p0);
    }

    /*
     * Copy the samples back into the history buffer start
     */
    pTempSrc = &pState[blockSize];
    pTempDest = pState;

    blkCnt = numTaps >> 3;
    while (blkCnt > 0) {
        vst1q(pTempDest, vld1q(pTempSrc));
        pTempSrc += 8;
        pTempDest += 8;
        blkCnt--;
    }
    blkCnt = numTaps & 7;
    if (blkCnt > 0) {
        mve_pred16_t    p0 = vctp16q(blkCnt);
        vstrhq_p_f16(pTempDest, vld1q(pTempSrc), p0);
    }

}
#undef NB_TAPS

#define NB_TAPS 8
__STATIC_INLINE void arm_fir_f16_5_8_mve(const arm_fir_instance_f16 * S,
    const float16_t * __restrict pSrc,
    float16_t * __restrict pDst, uint32_t blockSize)
{
    float16_t      *pState = S->pState;     /* State pointer */
    const float16_t *pCoeffs = S->pCoeffs;  /* Coefficient pointer */
    float16_t      *pStateCur;              /* Points to the current sample of the state */
    const float16_t *pSamples;              /* Temporary pointer to the sample buffer */
    float16_t      *pOutput;                /* Temporary pointer to the output buffer */
    const float16_t *pTempSrc;              /* Temporary pointer to the source data */
    float16_t      *pTempDest;              /* Temporary pointer to the destination buffer */
    uint32_t        numTaps = S->numTaps;   /* Number of filter coefficients in the filter */
    int32_t         blkCnt;
    float16x8_t         vecIn0;
    float16x8_t         vecAcc0;
    float16_t       c[NB_TAPS];


    /*
     * pState points to state array which contains previous frame (numTaps - 1) samples
     * pStateCur points to the location where the new input data should be written
     */
    pStateCur = &(pState[(numTaps - 1u)]);
    /*
     * Copy new data into state so that we obtain a continuous sample buffer
     * containing both the tail end of the old data and the new data.
     */
    pSamples = pState;
    pTempSrc = pSrc;
    pOutput = pDst;

    for (int i = 0; i < NB_TAPS; i++)
        c[i] = pCoeffs[i];

    blkCnt = blockSize >> 3;
    while (blkCnt > 0) {
        /*
         * Save 8 input samples in the history buffer
         */
        vst1q(pStateCur, vld1q(pTempSrc));
        pStateCur += 8;
        pTempSrc += 8;

        FIR_F16_CORE(pSamples, c, NB_TAPS);

        vst1q(pOutput, vecAcc0);

        pOutput += 8;
        pSamples += 8;

        blkCnt--;
    }

    blkCnt = blockSize & 7;
    if (blkCnt)
    {
        mve_pred16_t    p0 = vctp16q(blkCnt);

        vst1q(pStateCur, vld1q(pTempSrc));
        pStateCur += 8;
        pTempSrc += 8;

        FIR_F16_CORE(pSamples, c, NB_TAPS);

        vstrhq_p_f16(pOutput, vecAcc0, p0);
    }

    /*
     * Copy the samples back into the history buffer start
     */
    pTempSrc = &pState[blockSize];
    pTempDest = pState;

    blkCnt = numTaps >> 3;
    while (blkCnt > 0) {
        vst1q(pTempDest, vld1q(pTempSrc));
        pTempSrc += 8;
        pTempDest += 8;
        blkCnt--;
    }
    blkCnt = numTaps & 7;
    if (blkCnt > 0) {
        mve_pred16_t    p0 = vctp16q(blkCnt);
        vstrhq_p_f16(pTempDest, vld1q(pTempSrc), p0);
    }

}
#undef NB_TAPS

void arm_fir_f16_mve(const arm_fir_instance_f16 * S,
  const float16_t * pSrc,
  float16_t * pDst,
  uint32_t blockSize)
{
    float16_t *pRefStatePtr = S->pState + ROUND_UP(blockSize, 8);
    float16_t *pState = pRefStatePtr ;      /* State pointer */
    const float16_t *pCoeffs = S->pCoeffs;      /* Coefficient pointer */
    const float16_t *pSamples;  /* Temporary pointer to the sample buffer */
    float16_t      *pOutput;    /* Temporary pointer to the output buffer */
    const float16_t *pTempSrc;  /* Temporary pointer to the source data */
    float16_t      *pTempDest;  /* Temporary pointer to the destination buffer */
    uint32_t        numTaps = S->numTaps;       /* Number of filter coefficients in the filter */
    uint32_t        blkCnt;
    float16_t       c0, c1, c2, c3;
    float16_t       c4, c5, c6, c7;

    /*
     * [1 to 8 taps] specialized routines
     */
    if (numTaps <= 4) {
        arm_fir_f16_1_4_mve(S, pSrc, pDst, blockSize);
        return;
    } else if (numTaps <= 8) {
        arm_fir_f16_5_8_mve(S, pSrc, pDst, blockSize);
        return;
    }

    pTempSrc = pSrc;
    pTempDest = &(pState[(numTaps - 1u)]);
    int             cnt = blockSize;
    do {
        mve_pred16_t    p0 = vctp16q(cnt);
        vstrhq_p_f16(pTempDest, vld1q(pTempSrc), p0);
        pTempDest += 8;
        pTempSrc += 8;
        cnt -= 8;
    } while (cnt > 0);

    float16_t      *partial_accu_ptr = S->pState;

    pSamples = pState;
    c0 = *pCoeffs++;
    c1 = *pCoeffs++;
    c2 = *pCoeffs++;
    c3 = *pCoeffs++;
    c4 = *pCoeffs++;
    c5 = *pCoeffs++;
    c6 = *pCoeffs++;
    c7 = *pCoeffs++;

    cnt = blockSize >> 3;
    while (cnt > 0) {
        float16x8_t     vecAcc0;
        float16x8_t     vecIn0;

        vecIn0 = vld1q(pSamples);
        vecAcc0 = vmulq(vecIn0, c0);
        vecIn0 = vld1q(&pSamples[1]);
        vecAcc0 = vfmaq(vecAcc0, vecIn0, c1);
        vecIn0 = vld1q(&pSamples[2]);
        vecAcc0 = vfmaq(vecAcc0, vecIn0, c2);
        vecIn0 = vld1q(&pSamples[3]);
        vecAcc0 = vfmaq(vecAcc0, vecIn0, c3);
        vecIn0 = vld1q(&pSamples[4]);
        vecAcc0 = vfmaq(vecAcc0, vecIn0, c4);
        vecIn0 = vld1q(&pSamples[5]);
        vecAcc0 = vfmaq(vecAcc0, vecIn0, c5);
        vecIn0 = vld1q(&pSamples[6]);
        vecAcc0 = vfmaq(vecAcc0, vecIn0, c6);
        vecIn0 = vld1q(&pSamples[7]);
        vecAcc0 = vfmaq(vecAcc0, vecIn0, c7);
        pSamples += 8;
        vst1q(partial_accu_ptr, vecAcc0);
        cnt--;
        partial_accu_ptr += 8;
    }

    cnt = blockSize & 7;
    if (cnt > 0) {
        float16x8_t     vecAcc0;
        float16x8_t     vecIn0;

        mve_pred16_t p0 = vctp16q(cnt);


        vecIn0 = vld1q(pSamples);
        vecAcc0 = vmulq(vecIn0, c0);
        vecIn0 = vld1q(&pSamples[1]);
        vecAcc0 = vfmaq(vecAcc0, vecIn0, c1);
        vecIn0 = vld1q(&pSamples[2]);
        vecAcc0 = vfmaq(vecAcc0, vecIn0, c2);
        vecIn0 = vld1q(&pSamples[3]);
        vecAcc0 = vfmaq(vecAcc0, vecIn0, c3);
        vecIn0 = vld1q(&pSamples[4]);
        vecAcc0 = vfmaq(vecAcc0, vecIn0, c4);
        vecIn0 = vld1q(&pSamples[5]);
        vecAcc0 = vfmaq(vecAcc0, vecIn0, c5);
        vecIn0 = vld1q(&pSamples[6]);
        vecAcc0 = vfmaq(vecAcc0, vecIn0, c6);
        vecIn0 = vld1q(&pSamples[7]);
        vecAcc0 = vfmaq(vecAcc0, vecIn0, c7);
        vstrhq_p_f16(partial_accu_ptr, vecAcc0,p0);
    }

    int             localTaps = numTaps - FIR_F16_MAX_COEF_BLK;
    int             sample_offset = FIR_F16_MAX_COEF_BLK;
    while (localTaps > FIR_F16_MAX_COEF_BLK) {
        c0 = *pCoeffs++;
        c1 = *pCoeffs++;
        c2 = *pCoeffs++;
        c3 = *pCoeffs++;
        c4 = *pCoeffs++;
        c5 = *pCoeffs++;
        c6 = *pCoeffs++;
        c7 = *pCoeffs++;

        partial_accu_ptr = S->pState;
        pSamples = pState + sample_offset;
        int  cnt = blockSize >> 3;
        while (cnt > 0) {
            float16x8_t     vecAcc0;
            float16x8_t     vecIn0;


            vecIn0 = vld1q(pSamples);
            vecAcc0 = vmulq(vecIn0, c0);
            vecIn0 = vld1q(&pSamples[1]);
            vecAcc0 = vfmaq(vecAcc0, vecIn0, c1);
            vecIn0 = vld1q(&pSamples[2]);
            vecAcc0 = vfmaq(vecAcc0, vecIn0, c2);
            vecIn0 = vld1q(&pSamples[3]);
            vecAcc0 = vfmaq(vecAcc0, vecIn0, c3);
            vecIn0 = vld1q(&pSamples[4]);
            vecAcc0 = vfmaq(vecAcc0, vecIn0, c4);
            vecIn0 = vld1q(&pSamples[5]);
            vecAcc0 = vfmaq(vecAcc0, vecIn0, c5);
            vecIn0 = vld1q(&pSamples[6]);
            vecAcc0 = vfmaq(vecAcc0, vecIn0, c6);
            vecIn0 = vld1q(&pSamples[7]);
            vecAcc0 = vfmaq(vecAcc0, vecIn0, c7);
            pSamples += 8;
            vecAcc0 += vld1q_f16(partial_accu_ptr);
            vst1q(partial_accu_ptr, vecAcc0);
            cnt--;
            partial_accu_ptr += 8;
        }

        cnt = blockSize & 7;
        if (cnt > 0) {
            float16x8_t     vecAcc0;
            float16x8_t     vecIn0;

            mve_pred16_t p0 = vctp16q(cnt);

            vecIn0 = vld1q(pSamples);
            vecAcc0 = vmulq(vecIn0, c0);
            vecIn0 = vld1q(&pSamples[1]);
            vecAcc0 = vfmaq(vecAcc0, vecIn0, c1);
            vecIn0 = vld1q(&pSamples[2]);
            vecAcc0 = vfmaq(vecAcc0, vecIn0, c2);
            vecIn0 = vld1q(&pSamples[3]);
            vecAcc0 = vfmaq(vecAcc0, vecIn0, c3);
            vecIn0 = vld1q(&pSamples[4]);
            vecAcc0 = vfmaq(vecAcc0, vecIn0, c4);
            vecIn0 = vld1q(&pSamples[5]);
            vecAcc0 = vfmaq(vecAcc0, vecIn0, c5);
            vecIn0 = vld1q(&pSamples[6]);
            vecAcc0 = vfmaq(vecAcc0, vecIn0, c6);
            vecIn0 = vld1q(&pSamples[7]);
            vecAcc0 = vfmaq(vecAcc0, vecIn0, c7);
            vecAcc0 += vld1q_f16(partial_accu_ptr);
            vstrhq_p_f16(partial_accu_ptr, vecAcc0,p0);
        }

        localTaps -= FIR_F16_MAX_COEF_BLK;
        sample_offset += FIR_F16_MAX_COEF_BLK;
    }

    pSamples = pState + sample_offset;

    if (localTaps > 4) {
        c0 = *pCoeffs++;
        c1 = *pCoeffs++;
        c2 = *pCoeffs++;
        c3 = *pCoeffs++;
        c4 = *pCoeffs++;
        c5 = *pCoeffs++;
        c6 = *pCoeffs++;
        c7 = *pCoeffs++;
        pOutput = pDst;

        partial_accu_ptr = S->pState;
        cnt = blockSize >> 3;
        while (cnt > 0) {
            float16x8_t     vecAcc0;
            float16x8_t     vecIn0;

            vecIn0 = vld1q(pSamples);
            vecAcc0 = vmulq(vecIn0, c0);
            vecIn0 = vld1q(&pSamples[1]);
            vecAcc0 = vfmaq(vecAcc0, vecIn0, c1);
            vecIn0 = vld1q(&pSamples[2]);
            vecAcc0 = vfmaq(vecAcc0, vecIn0, c2);
            vecIn0 = vld1q(&pSamples[3]);
            vecAcc0 = vfmaq(vecAcc0, vecIn0, c3);
            vecIn0 = vld1q(&pSamples[4]);
            vecAcc0 = vfmaq(vecAcc0, vecIn0, c4);
            vecIn0 = vld1q(&pSamples[5]);
            vecAcc0 = vfmaq(vecAcc0, vecIn0, c5);
            vecIn0 = vld1q(&pSamples[6]);
            vecAcc0 = vfmaq(vecAcc0, vecIn0, c6);
            vecIn0 = vld1q(&pSamples[7]);
            vecAcc0 = vfmaq(vecAcc0, vecIn0, c7);
            pSamples += 8;
            float16x8_t     pap = vld1q_f16(partial_accu_ptr);
            vst1q(pOutput, vecAcc0 + pap);
            cnt--;
            partial_accu_ptr += 8;
            pOutput += 8;
        }

        cnt = blockSize & 7;
        if (cnt > 0) {
            float16x8_t     vecAcc0;
            float16x8_t     vecIn0;

            mve_pred16_t p0 = vctp16q(cnt);

            vecIn0 = vld1q(pSamples);
            vecAcc0 = vmulq(vecIn0, c0);
            vecIn0 = vld1q(&pSamples[1]);
            vecAcc0 = vfmaq(vecAcc0, vecIn0, c1);
            vecIn0 = vld1q(&pSamples[2]);
            vecAcc0 = vfmaq(vecAcc0, vecIn0, c2);
            vecIn0 = vld1q(&pSamples[3]);
            vecAcc0 = vfmaq(vecAcc0, vecIn0, c3);
            vecIn0 = vld1q(&pSamples[4]);
            vecAcc0 = vfmaq(vecAcc0, vecIn0, c4);
            vecIn0 = vld1q(&pSamples[5]);
            vecAcc0 = vfmaq(vecAcc0, vecIn0, c5);
            vecIn0 = vld1q(&pSamples[6]);
            vecAcc0 = vfmaq(vecAcc0, vecIn0, c6);
            vecIn0 = vld1q(&pSamples[7]);
            vecAcc0 = vfmaq(vecAcc0, vecIn0, c7);
            float16x8_t     pap = vld1q_f16(partial_accu_ptr);
            vstrhq_p_f16(pOutput, vecAcc0 + pap, p0);
            pOutput += cnt;
        }

    } else {
        c0 = *pCoeffs++;
        c1 = *pCoeffs++;
        c2 = *pCoeffs++;
        c3 = *pCoeffs++;
        pOutput = pDst;

        partial_accu_ptr = S->pState;
        cnt = blockSize >> 3;
        while (cnt > 0) {
            float16x8_t     vecAcc0;
            float16x8_t     vecIn0;

            vecIn0 = vld1q(pSamples);
            vecAcc0 = vmulq(vecIn0, c0);
            vecIn0 = vld1q(&pSamples[1]);
            vecAcc0 = vfmaq(vecAcc0, vecIn0, c1);
            vecIn0 = vld1q(&pSamples[2]);
            vecAcc0 = vfmaq(vecAcc0, vecIn0, c2);
            vecIn0 = vld1q(&pSamples[3]);
            vecAcc0 = vfmaq(vecAcc0, vecIn0, c3);
            pSamples += 8;
            float16x8_t     pap = vld1q_f16(partial_accu_ptr);
            vst1q(pOutput, vecAcc0 + pap);
            cnt--;
            partial_accu_ptr += 8;
            pOutput += 8;
        }

        cnt = blockSize & 7;
        if (cnt > 0) {
            float16x8_t     vecAcc0;
            float16x8_t     vecIn0;

            mve_pred16_t p0 = vctp16q(cnt);

            vecIn0 = vld1q(pSamples);
            vecAcc0 = vmulq(vecIn0, c0);
            vecIn0 = vld1q(&pSamples[1]);
            vecAcc0 = vfmaq(vecAcc0, vecIn0, c1);
            vecIn0 = vld1q(&pSamples[2]);
            vecAcc0 = vfmaq(vecAcc0, vecIn0, c2);
            vecIn0 = vld1q(&pSamples[3]);
            vecAcc0 = vfmaq(vecAcc0, vecIn0, c3);
            float16x8_t     pap = vld1q_f16(partial_accu_ptr);
            vstrhq_p_f16(pOutput, vecAcc0 + pap, p0);
            pOutput += cnt;
        }
    }

    /*
     * Copy the samples back into the history buffer start
     */
    pTempSrc = &pState[blockSize];
    pTempDest = pState;

    blkCnt = numTaps >> 3;
    while (blkCnt > 0U) {
        vst1q(pTempDest, vld1q(pTempSrc));
        pTempSrc += 8;
        pTempDest += 8;
        blkCnt--;
    }
    blkCnt = numTaps & 7;
    if (blkCnt > 0U) {
        mve_pred16_t    p0 = vctp16q(blkCnt);
        vstrhq_p_f16(pTempDest, vld1q(pTempSrc), p0);
    }
}

#endif

#endif

/**
* @} end of FIR group
*/

#endif /* #if defined(ARM_FLOAT16_SUPPORTED) */
