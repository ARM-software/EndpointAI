/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_fir_f32.c
 * Description:  Floating-point FIR filter processing function
 *               using Helium assembly kernels
 *               This version allows boosting F32 FIR Filter performance when using compilers having suboptimal
 *               Helium intrinsic code generation.
 *
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
  @defgroup FIR Finite Impulse Response (FIR) Filters

  This set of functions implements Finite Impulse Response (FIR) filters
  for Q7, Q15, Q31, and floating-point data types.  Fast versions of Q15 and Q31 are also provided.
  The functions operate on blocks of input and output data and each call to the function processes
  <code>blockSize</code> samples through the filter.  <code>pSrc</code> and
  <code>pDst</code> points to input and output arrays containing <code>blockSize</code> values.

  @par           Algorithm
                   The FIR filter algorithm is based upon a sequence of multiply-accumulate (MAC) operations.
                   Each filter coefficient <code>b[n]</code> is multiplied by a state variable which equals a previous input sample <code>x[n]</code>.
  <pre>
      y[n] = b[0] * x[n] + b[1] * x[n-1] + b[2] * x[n-2] + ...+ b[numTaps-1] * x[n-numTaps+1]
  </pre>
  @par
                   \image html FIR.GIF "Finite Impulse Response filter"
  @par
                   <code>pCoeffs</code> points to a coefficient array of size <code>numTaps</code>.
                   Coefficients are stored in time reversed order.
  @par
  <pre>
      {b[numTaps-1], b[numTaps-2], b[N-2], ..., b[1], b[0]}
  </pre>
  @par
                   <code>pState</code> points to a state array of size <code>numTaps + blockSize - 1</code>.
                   Samples in the state buffer are stored in the following order.
  @par
  <pre>
      {x[n-numTaps+1], x[n-numTaps], x[n-numTaps-1], x[n-numTaps-2]....x[n](==pSrc[0]), x[n+1](==pSrc[1]), ..., x[n+blockSize-1](==pSrc[blockSize-1])}
  </pre>
  @par
                   Note that the length of the state buffer exceeds the length of the coefficient array by <code>blockSize-1</code>.
                   The increased state buffer length allows circular addressing, which is traditionally used in the FIR filters,
                   to be avoided and yields a significant speed improvement.
                   The state variables are updated after each block of data is processed; the coefficients are untouched.

  @par           Instance Structure
                   The coefficients and state variables for a filter are stored together in an instance data structure.
                   A separate instance structure must be defined for each filter.
                   Coefficient arrays may be shared among several instances while state variable arrays cannot be shared.
                   There are separate instance structure declarations for each of the 4 supported data types.

  @par           Initialization Functions
                   There is also an associated initialization function for each data type.
                   The initialization function performs the following operations:
                   - Sets the values of the internal structure fields.
                   - Zeros out the values in the state buffer.
                   To do this manually without calling the init function, assign the follow subfields of the instance structure:
                   numTaps, pCoeffs, pState. Also set all of the values in pState to zero.
  @par
                   Use of the initialization function is optional.
                   However, if the initialization function is used, then the instance structure cannot be placed into a const data section.
                   To place an instance structure into a const data section, the instance structure must be manually initialized.
                   Set the values in the state buffer to zeros before static initialization.
                   The code below statically initializes each of the 4 different data type filter instance structures
  <pre>
      arm_fir_instance_f32 S = {numTaps, pState, pCoeffs};
      arm_fir_instance_q31 S = {numTaps, pState, pCoeffs};
      arm_fir_instance_q15 S = {numTaps, pState, pCoeffs};
      arm_fir_instance_q7 S =  {numTaps, pState, pCoeffs};
  </pre>
                   where <code>numTaps</code> is the number of filter coefficients in the filter; <code>pState</code> is the address of the state buffer;
                   <code>pCoeffs</code> is the address of the coefficient buffer.
  @par          Initialization of Helium version
                 For Helium version the array of coefficients must be padded with zero to contain
                 a full number of lanes.

                 The array length L must be a multiple of x. L = x * a :
                 - x is 4  for f32
                 - x is 4  for q31
                 - x is 4  for f16 (so managed like the f32 version and not like the q15 one)
                 - x is 8  for q15
                 - x is 16 for q7

                 The additional coefficients
                 (x * a - numTaps) must be set to 0.
                 numTaps is still set to its right value in the init function. It means that
                 the implementation may require to read more coefficients due to the vectorization and
                 to avoid having to manage too many different cases in the code.


  @par          Helium state buffer
                 The state buffer must contain some additional temporary data
                 used during the computation but which is not the state of the FIR.
                 The first A samples are temporary data.
                 The remaining samples are the state of the FIR filter.
  @par
                 So the state buffer has size <code> numTaps + A + blockSize - 1 </code> :
                 - A is blockSize for f32
                 - A is 8*ceil(blockSize/8) for f16
                 - A is 8*ceil(blockSize/4) for q31
                 - A is 0 for other datatypes (q15 and q7)


  @par           Fixed-Point Behavior
                   Care must be taken when using the fixed-point versions of the FIR filter functions.
                   In particular, the overflow and saturation behavior of the accumulator used in each function must be considered.
                   Refer to the function specific documentation below for usage guidelines.

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

#if defined(ARM_MATH_MVEF)

#define FIR_F32_MAX_COEF_BLK        8


#ifdef USE_ASM

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


/* FIR coef preload */

#define FIR_F32_LOAD_8_COEFS(pCoeffs)                           \
    "C8n_0   .req   r2                                     \n"  \
    "C8n_1   .req   r3                                     \n"  \
    "C8n_2   .req   r4                                     \n"  \
    "C8n_3   .req   r5                                     \n"  \
    "C8n_4   .req   r6                                     \n"  \
    "C8n_5   .req   r10                                    \n"  \
    "C8n_6   .req   r8                                     \n"  \
    "C8n_7   .req   r9                                     \n"  \
                                                                \
    "   ldrd        C8n_6, C8n_7, [%["#pCoeffs"], #24]     \n"  \
    "   ldrd        C8n_4, C8n_5, [%["#pCoeffs"], #16]     \n"  \
    "   ldrd        C8n_2, C8n_3, [%["#pCoeffs"], #8]      \n"  \
    "   ldrd        C8n_0, C8n_1, [%["#pCoeffs"]], #32     \n"

#define FIR_F32_FORGET_8_COEFS()                                \
    " .unreq C8n_0                                         \n"  \
    " .unreq C8n_1                                         \n"  \
    " .unreq C8n_2                                         \n"  \
    " .unreq C8n_3                                         \n"  \
    " .unreq C8n_4                                         \n"  \
    " .unreq C8n_5                                         \n"  \
    " .unreq C8n_6                                         \n"  \
    " .unreq C8n_7                                         \n"  \

#define FIR_F32_LOAD_4_COEFS(pCoeffs)                           \
    "C4n_0   .req   r2                                     \n"  \
    "C4n_1   .req   r3                                     \n"  \
    "C4n_2   .req   r4                                     \n"  \
    "C4n_3   .req   r5                                     \n"  \
                                                                \
    "   ldrd        C4n_0, C4n_1, [%["#pCoeffs"]]          \n"  \
    "   ldrd        C4n_2, C4n_3, [%["#pCoeffs"], #8]      \n"

#define FIR_F32_FORGET_4_COEFS(pCoeffs)                         \
    " .unreq C4n_0                                         \n"  \
    " .unreq C4n_1                                         \n"  \
    " .unreq C4n_2                                         \n"  \
    " .unreq C4n_3                                         \n"  \


/* block filter 4 samples with 4 taps */
#define FIR_F32_FILTER_4_SAMPLS_4_COEFS(pSamples)              \
    "   vldrw.32            q0, [%["#pSamples"], #12]      \n" \
    "   vmul.f32            q1, q0, C4n_3                  \n" \
    "   vldrw.32            q0, [%["#pSamples"], #8]       \n" \
    "   vfma.f32            q1, q0, C4n_2                  \n" \
    "   vldrw.32            q0, [%["#pSamples"], #4]       \n" \
    "   vfma.f32            q1, q0, C4n_1                  \n" \
    "   vldrw.32            q0, [%["#pSamples"]], #16      \n" \
    "   vfma.f32            q1, q0, C4n_0                  \n"


/* block filter 4 samples with 8 taps */
#define FIR_F32_FILTER_4_SAMPLS_8_COEFS(pSamples)              \
    "   vldrw.32            q0, [%["#pSamples"], #28]      \n" \
    "   vmul.f32            q1, q0, C8n_7                  \n" \
    "   vldrw.32            q0, [%["#pSamples"], #24]      \n" \
    "   vfma.f32            q1, q0, C8n_6                  \n" \
    "   vldrw.32            q0, [%["#pSamples"], #20]      \n" \
    "   vfma.f32            q1, q0, C8n_5                  \n" \
    "   vldrw.32            q0, [%["#pSamples"], #16]      \n" \
    "   vfma.f32            q1, q0, C8n_4                  \n" \
    "   vldrw.32            q0, [%["#pSamples"], #12]      \n" \
    "   vfma.f32            q1, q0, C8n_3                  \n" \
    "   vldrw.32            q0, [%["#pSamples"], #8]       \n" \
    "   vfma.f32            q1, q0, C8n_2                  \n" \
    "   vldrw.32            q0, [%["#pSamples"], #4]       \n" \
    "   vfma.f32            q1, q0, C8n_1                  \n" \
    "   vldrw.32            q0, [%["#pSamples"]], #16      \n" \
    "   vfma.f32            q1, q0, C8n_0                  \n"



__STATIC_INLINE void arm_fir_f32_1_4_mve(const arm_fir_instance_f32 * S,
  const float32_t * __restrict pSrc,
  float32_t * __restrict pDst, uint32_t blockSize)
{
    float32_t       *pRefStatePtr = S->pState + blockSize;
    float32_t       *pState = pRefStatePtr;     /* State pointer */
    const float32_t *pCoeffs = S->pCoeffs;      /* Coefficient pointer */
    const float32_t *pSamples;      /* Temporary pointer to the sample buffer */
    float32_t       *pOutput;       /* Temporary pointer to the output buffer */
    const float32_t *pTempSrc;      /* Temporary pointer to the source data */
    float32_t       *pTempDest;     /* Temporary pointer to the destination buffer */


    pSamples = pState;
    pOutput = pDst;

    __asm volatile (
       /*
        * Coefs load
        */
        FIR_F32_LOAD_4_COEFS(pCoeffs)

        ".p2align 2                                                        \n"
        "   wlstp.32            lr, %[cnt], 1f                             \n"
        /* low overhead loop start */
        "2:                                                                \n"

        FIR_F32_FILTER_4_SAMPLS_4_COEFS(pSamples)
        /*
         * store 4 elements
         */
        "   vstrw.32            q1, [%[pOutput]], #16                      \n"
        "   letp                lr, 2b                                     \n"

        /* low overhead loop end */
        "1:                                                                \n"

        FIR_F32_FORGET_4_COEFS()

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

    COPY_STATE_32(4, pTempSrc, pTempDest);
}



__STATIC_INLINE void arm_fir_f32_5_8_mve(const arm_fir_instance_f32 * S,
  const float32_t * __restrict pSrc,
  float32_t * __restrict pDst, uint32_t blockSize)
{
    float32_t       *pRefStatePtr = S->pState + blockSize;
    float32_t       *pState = pRefStatePtr;     /* State pointer */
    const float32_t *pCoeffs = S->pCoeffs;      /* Coefficient pointer */
    const float32_t *pSamples;      /* Temporary pointer to the sample buffer */
    float32_t       *pOutput;       /* Temporary pointer to the output buffer */
    const float32_t *pTempSrc;      /* Temporary pointer to the source data */
    float32_t       *pTempDest;     /* Temporary pointer to the destination buffer */


    pSamples = pState;
    pOutput = pDst;

    __asm volatile (
       /*
        * Coefs load
        */
        FIR_F32_LOAD_8_COEFS(pCoeffs)

        ".p2align 2                                                 \n"
        "   wlstp.32            lr, %[cnt], 1f                      \n"
        /* low overhead loop start */
        "2:                                                         \n"

        FIR_F32_FILTER_4_SAMPLS_8_COEFS(pSamples)
        /*
         * store 4 elements
         */
        "   vstrw.32            q1, [%[pOutput]], #16               \n"
        "   letp                lr, 2b                              \n"
        /* low overhead loop end */
        "1:                                                         \n"

        FIR_F32_FORGET_8_COEFS()

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

    COPY_STATE_32(8, pTempSrc, pTempDest);
}



void arm_fir_f32_mve(const arm_fir_instance_f32 * S,
                        const float32_t * pSrc,
                        float32_t       * pDst,
                        uint32_t          blockSize)
{
    float32_t *pRefStatePtr = S->pState + blockSize;
    float32_t *pState = pRefStatePtr ;      /* State pointer */
    const float32_t *pCoeffs = S->pCoeffs;    /* Coefficient pointer */
    const float32_t *pSamples;          /* Temporary pointer to the sample buffer */
    float32_t *pOutput;                 /* Temporary pointer to the output buffer */
    const float32_t *pTempSrc;          /* Temporary pointer to the source data */
    float32_t *pTempDest;               /* Temporary pointer to the destination buffer */
    uint32_t  numTaps = S->numTaps;     /* Number of filter coefficients in the filter */


    pTempSrc = pSrc;
    pTempDest = &(pState[(numTaps - 1u)]);

    COPY_STATE_32(blockSize, pTempSrc, pTempDest);

    /*
     * [1 to 8 taps] specialized routines
     */
    if (numTaps <= 4)
    {
        arm_fir_f32_1_4_mve(S, pSrc, pDst, blockSize);
        return;
    }
    else if (numTaps <= 8)
    {
        arm_fir_f32_5_8_mve(S, pSrc, pDst, blockSize);
        return;
    }

    pSamples = pState;
    float32_t *partial_accu_ptr = S->pState;


    /*
     * 1st part : start from cleared accumulators and process 8 = FIR_F32_MAX_COEF_BLK
     *            taps and store partial accumulators in a temp buffer
     */

    __asm volatile (
       /*
        * Coefs load
        */
        FIR_F32_LOAD_8_COEFS(pCoeffs)

        ".p2align 2                                                     \n"
        "   wlstp.32            lr, %[cnt], 1f                          \n"

        /* low overhead loop start */
        "2:                                                             \n"

        FIR_F32_FILTER_4_SAMPLS_8_COEFS(pSamples)
        /*
         * store 4 elements
         */
        "   vstrw.32            q1, [%[partial_accu_ptr]], #16          \n"
        "   letp                lr, 2b                                  \n"
        /* low overhead loop end */
        "1:                                                             \n"

        FIR_F32_FORGET_8_COEFS()

       :[pSamples] "+r"(pSamples),[partial_accu_ptr] "+r"(partial_accu_ptr),
        [pCoeffs] "+r"(pCoeffs)
       :[cnt] "r"(blockSize)
       :"q0", "q1",
        "r2", "r3", "r4", "r5",
        "r6", "r10", "r8", "r9",
        "r14", "memory");


        /*
         * 2nd part : reload partial accumulators and process next block of 8 = FIR_F32_MAX_COEF_BLK
         *            taps and keep on storign partial accumulators in temp buffer
         */

        int       localTaps = numTaps - FIR_F32_MAX_COEF_BLK;
        int       sample_offset = FIR_F32_MAX_COEF_BLK;

        while (localTaps > FIR_F32_MAX_COEF_BLK)
        {
            pSamples = pState + sample_offset;
            partial_accu_ptr =  S->pState;


            __asm volatile (
                /*
                * Coefs load
                */
                FIR_F32_LOAD_8_COEFS(pCoeffs)

                ".p2align 2                                                     \n"
                "   wlstp.32            lr, %[cnt], 1f                          \n"

                /* low overhead loop start */
                "2:                                                             \n"

                FIR_F32_FILTER_4_SAMPLS_8_COEFS(pSamples)

                /* reload partial accumulators */
                "   vldrw.32            q2, [%[partial_accu_ptr]]               \n"
                "   vadd.f32            q1, q1, q2                              \n"

                /*
                * store 4 elements
                */
                "   vstrw.32            q1, [%[partial_accu_ptr]], #16          \n"
                "   letp                lr, 2b                                  \n"

                /* low overhead loop end */
                "1:                                                             \n"

                FIR_F32_FORGET_8_COEFS()

                :[pSamples] "+r"(pSamples),[partial_accu_ptr] "+r"(partial_accu_ptr),
                 [pCoeffs] "+r"(pCoeffs)
                :[cnt] "r"(blockSize)
                :"q0", "q1", "q2",
                "r2", "r3", "r4", "r5",
                "r6", "r10", "r8", "r9",
                "r14", "memory");


            localTaps -= FIR_F32_MAX_COEF_BLK;
            sample_offset += FIR_F32_MAX_COEF_BLK;


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
                FIR_F32_LOAD_8_COEFS(pCoeffs)

                ".p2align 2                                                  \n"
                "   wlstp.32            lr, lr, 1f                           \n"
                /* low overhead loop start */
                "2:                                                          \n"

                FIR_F32_FILTER_4_SAMPLS_8_COEFS(pSamples)

               /* reload partial accumulators */
                "   vldrw.32            q2, [%[partial_accu_ptr]], #16       \n"
                "   vadd.f32            q1, q1, q2                           \n"

                /*
                 * store 4 elements in filter output
                 */
                "   vstrw.32            q1, [%[pOutput]], #16                \n"
                "   letp                lr, 2b                               \n"

                /* low overhead loop end */
                "1:                                                          \n"

                FIR_F32_FORGET_8_COEFS()

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
                FIR_F32_LOAD_4_COEFS(pCoeffs)

                ".p2align 2                                                  \n"
                "   wlstp.32            lr, lr, 1f                           \n"

                /* low overhead loop start */
                "2:                                                          \n"

                FIR_F32_FILTER_4_SAMPLS_4_COEFS(pSamples)

                /* reload partial accumulators */
                "   vldrw.32            q2, [%[partial_accu_ptr]], #16       \n"
                "   vadd.f32            q1, q1, q2                           \n"

                /*
                 * store 4 elements in filter output
                 */
                "   vstrw.32            q1, [%[pOutput]], #16                \n"
                "   letp                lr, 2b                               \n"

                /* low overhead loop end */
                "1:                                                          \n"

                FIR_F32_FORGET_4_COEFS()

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

    COPY_STATE_32(numTaps, pTempSrc, pTempDest);
}


#else

/* CMSIS DSP MVE intrinsic reference */

#define FIR_F32_CORE(pSamples, c, NB_TAPS)                                 \
        vecAcc0 = vdupq_n_f32(0.0f);                                       \
        for (int i = 0; i < NB_TAPS; i++) {                                \
            vecIn0 = vld1q(&pSamples[i]);                                  \
            vecAcc0 = vfmaq(vecAcc0, vecIn0, c[i]);                        \
        }


#define NB_TAPS 4
__STATIC_INLINE void arm_fir_f32_1_4_mve(const arm_fir_instance_f32 * S,
  const float32_t * __restrict pSrc,
  float32_t * __restrict pDst, uint32_t blockSize)
{
    float32_t *pRefStatePtr = S->pState + blockSize;
    float32_t      *pState = pRefStatePtr; /* State pointer */
    const float32_t *pCoeffs = S->pCoeffs;      /* Coefficient pointer */
    float32_t      *pStateCur;  /* Points to the current sample of the state */
    const float32_t *pSamples;  /* Temporary pointer to the sample buffer */
    float32_t      *pOutput;    /* Temporary pointer to the output buffer */
    const float32_t *pTempSrc;  /* Temporary pointer to the source data */
    float32_t      *pTempDest;  /* Temporary pointer to the destination buffer */
    uint32_t        numTaps = S->numTaps;       /* Number of filter coefficients in the filter */
    int32_t         blkCnt;
    float32x4_t         vecIn0;
    float32x4_t         vecAcc0;
    float32_t       c[NB_TAPS];
    const float32_t *pCoeffsCur = pCoeffs;

    /*
     * pState points to state array which contains previous frame (numTaps - 1) samples
     * pStateCur points to the location where the new input data should be written
     */
    pStateCur = &(pState[(numTaps - 1u)]);
    pTempSrc = pSrc;

    pSamples = pState;
    pOutput = pDst;

    for (int i = 0; i < NB_TAPS; i++)
        c[i] = *pCoeffsCur++;

    blkCnt = blockSize >> 2;
    while (blkCnt > 0) {
        /*
         * Save 4 input samples in the history buffer
         */
        vst1q(pStateCur, vld1q(pTempSrc));
        pStateCur += 4;
        pTempSrc += 4;

        FIR_F32_CORE(pSamples, c, NB_TAPS);

        vst1q(pOutput, vecAcc0);

        pOutput += 4;
        pSamples += 4;

        blkCnt--;
    }

    blkCnt = blockSize & 3;
    if (blkCnt)
    {
        mve_pred16_t    p0 = vctp32q(blkCnt);

        vst1q(pStateCur, vld1q(pTempSrc));
        pStateCur += 4;
        pTempSrc += 4;

        FIR_F32_CORE(pSamples, c, NB_TAPS);

        vstrwq_p_f32(pOutput, vecAcc0, p0);
    }

    /*
     * Copy the samples back into the history buffer start
     */
    pTempSrc = &pState[blockSize];
    pTempDest = pState;

    blkCnt = numTaps - 1;
    do {
        mve_pred16_t    p = vctp32q(blkCnt);

        vstrwq_p_f32(pTempDest, vldrwq_z_f32(pTempSrc, p), p);
        pTempSrc += 4;
        pTempDest += 4;
        blkCnt -= 4;
    }
    while (blkCnt > 0);
}
#undef NB_TAPS

__STATIC_INLINE void arm_fir_f32_5_8_mve(const arm_fir_instance_f32 * S,
  const float32_t * __restrict pSrc,
  float32_t * __restrict pDst, uint32_t blockSize)
{
    float32_t *pRefStatePtr = S->pState + blockSize;
    float32_t *pState = pRefStatePtr;      /* State pointer */
    const float32_t *pCoeffs = S->pCoeffs;    /* Coefficient pointer */
    const float32_t *pSamples;          /* Temporary pointer to the sample buffer */
    const float32_t *pTempSrc;          /* Temporary pointer to the source data */
    float32_t *pTempDest;               /* Temporary pointer to the destination buffer */
    uint32_t  numTaps = S->numTaps;     /* Number of filter coefficients in the filter */
    int32_t  blkCnt;
    float32_t c0, c1, c2, c3;
    float32_t c4, c5, c6, c7;


    pTempSrc = pSrc;
    pTempDest = &(pState[(numTaps - 1u)]);
    int cnt = blockSize;
    do {
        mve_pred16_t p0 = vctp32q(cnt);
        vstrwq_p_f32(pTempDest, vld1q(pTempSrc), p0);
        pTempDest += 4;
        pTempSrc += 4;
        cnt -= 4;
    } while(cnt > 0);



    pSamples = pState;
    c0 = *pCoeffs++;
    c1 = *pCoeffs++;
    c2 = *pCoeffs++;
    c3 = *pCoeffs++;
    c4 = *pCoeffs++;
    c5 = *pCoeffs++;
    c6 = *pCoeffs++;
    c7 = *pCoeffs++;

    cnt = blockSize >> 2;
    while(cnt > 0)
    {
        float32x4_t vecAcc0;
        float32x4_t vecIn0;

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
        pSamples += 4;
        vst1q(pDst, vecAcc0);
        cnt--;
        pDst += 4;
    }

    cnt = blockSize & 3;
    if (cnt > 0)
    {
        float32x4_t vecAcc0;
        float32x4_t vecIn0;

        mve_pred16_t p0 = vctp32q(cnt);

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
        vstrwq_p_f32(pDst, vecAcc0,p0);
    }


    /*
     * Copy the samples back into the history buffer start
     */
    pTempSrc = &pState[blockSize];
    pTempDest = pState;
    blkCnt = numTaps;
    while (blkCnt > 0)
    {
        *pTempDest++ = *pTempSrc++;
        blkCnt--;
    }
}



void arm_fir_f32_mve(
const arm_fir_instance_f32 * S,
const float32_t * pSrc,
float32_t * pDst,
uint32_t blockSize)
{
    /*
       S->pState is the arm_fir_partial_accu
       S->pState + blockSize is the FIR state
    */
    float32_t *pRefStatePtr = S->pState + blockSize;
    float32_t *pState = pRefStatePtr ;      /* State pointer */
    const float32_t *pCoeffs = S->pCoeffs;    /* Coefficient pointer */
    const float32_t *pSamples;          /* Temporary pointer to the sample buffer */
    float32_t *pOutput;                 /* Temporary pointer to the output buffer */
    const float32_t *pTempSrc;          /* Temporary pointer to the source data */
    float32_t *pTempDest;               /* Temporary pointer to the destination buffer */
    uint32_t  numTaps = S->numTaps;     /* Number of filter coefficients in the filter */
    uint32_t  blkCnt;
    float32_t c0, c1, c2, c3;
    float32_t c4, c5, c6, c7;

    /*
     * [1 to 8 taps] specialized routines
     */
    if (numTaps <= 4)
    {
        arm_fir_f32_1_4_mve(S, pSrc, pDst, blockSize);
        return;
    }
    else if (numTaps <= 8)
    {
        arm_fir_f32_5_8_mve(S, pSrc, pDst, blockSize);
        return;
    }

    pTempSrc = pSrc;
    pTempDest = &(pState[(numTaps - 1u)]);
    int cnt = blockSize;
    do {
        mve_pred16_t p0 = vctp32q(cnt);
        vstrwq_p_f32(pTempDest, vld1q(pTempSrc), p0);
        pTempDest += 4;
        pTempSrc += 4;
        cnt -= 4;
    } while(cnt > 0);

    float32_t *partial_accu_ptr = S->pState;

    pSamples = pState;
    c0 = *pCoeffs++;
    c1 = *pCoeffs++;
    c2 = *pCoeffs++;
    c3 = *pCoeffs++;
    c4 = *pCoeffs++;
    c5 = *pCoeffs++;
    c6 = *pCoeffs++;
    c7 = *pCoeffs++;

    cnt = blockSize >> 2;
    while(cnt > 0) {
        float32x4_t vecAcc0;
        float32x4_t vecIn0;

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
        pSamples += 4;
        vst1q(partial_accu_ptr, vecAcc0);
        cnt--;
        partial_accu_ptr += 4;
    }

    cnt = blockSize & 3;
    if (cnt > 0)
    {
        float32x4_t vecAcc0;
        float32x4_t vecIn0;

        mve_pred16_t p0 = vctp32q(cnt);

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
        vstrwq_p_f32(partial_accu_ptr, vecAcc0,p0);
    }

    int localTaps = numTaps - FIR_F32_MAX_COEF_BLK;
    int sample_offset = FIR_F32_MAX_COEF_BLK;
    while (localTaps > FIR_F32_MAX_COEF_BLK) {
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
        int cnt = blockSize >> 2;
        while(cnt > 0) {
            float32x4_t vecAcc0;
            float32x4_t vecIn0;

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
            pSamples += 4;
            vecAcc0 += vld1q_f32(partial_accu_ptr);
            vst1q(partial_accu_ptr, vecAcc0);
            cnt--;
            partial_accu_ptr += 4;
        }

        cnt = blockSize & 3;
        if (cnt > 0) {
            float32x4_t vecAcc0;
            float32x4_t vecIn0;

            mve_pred16_t p0 = vctp32q(cnt);

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
            vecAcc0 += vld1q_f32(partial_accu_ptr);
            vstrwq_p_f32(partial_accu_ptr, vecAcc0,p0);
        }

        localTaps -= FIR_F32_MAX_COEF_BLK;
        sample_offset += FIR_F32_MAX_COEF_BLK;
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
        cnt = blockSize  >> 2;
        while(cnt > 0) {
            float32x4_t vecAcc0;
            float32x4_t vecIn0;

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
            pSamples += 4;
            float32x4_t pap = vld1q_f32(partial_accu_ptr);
            vst1q(pOutput, vecAcc0+pap);
            cnt--;
            partial_accu_ptr += 4;
            pOutput += 4;
        }

        cnt = blockSize  & 3;
        if (cnt > 0) {
            float32x4_t vecAcc0;
            float32x4_t vecIn0;

            mve_pred16_t p0 = vctp32q(cnt);

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
            float32x4_t pap = vld1q_f32(partial_accu_ptr);
            vstrwq_p_f32(pOutput, vecAcc0+pap,p0);
            pOutput += cnt;
        }
    }
    else {
        c0 = *pCoeffs++;
        c1 = *pCoeffs++;
        c2 = *pCoeffs++;
        c3 = *pCoeffs++;
        pOutput = pDst;

        partial_accu_ptr = S->pState;
        cnt = blockSize >> 2;
        while(cnt > 0) {
            float32x4_t vecAcc0;
            float32x4_t vecIn0;

            vecIn0 = vld1q(pSamples);
            vecAcc0 = vmulq(vecIn0, c0);
            vecIn0 = vld1q(&pSamples[1]);
            vecAcc0 = vfmaq(vecAcc0, vecIn0, c1);
            vecIn0 = vld1q(&pSamples[2]);
            vecAcc0 = vfmaq(vecAcc0, vecIn0, c2);
            vecIn0 = vld1q(&pSamples[3]);
            vecAcc0 = vfmaq(vecAcc0, vecIn0, c3);
            pSamples += 4;
            float32x4_t pap = vld1q_f32(partial_accu_ptr);
            vst1q(pOutput, vecAcc0+pap);
            cnt--;
            partial_accu_ptr += 4;
            pOutput += 4;
        }

        cnt = blockSize & 3;
        if (cnt > 0) {
            float32x4_t vecAcc0;
            float32x4_t vecIn0;

            mve_pred16_t p0 = vctp32q(cnt);

            vecIn0 = vld1q(pSamples);
            vecAcc0 = vmulq(vecIn0, c0);
            vecIn0 = vld1q(&pSamples[1]);
            vecAcc0 = vfmaq(vecAcc0, vecIn0, c1);
            vecIn0 = vld1q(&pSamples[2]);
            vecAcc0 = vfmaq(vecAcc0, vecIn0, c2);
            vecIn0 = vld1q(&pSamples[3]);
            vecAcc0 = vfmaq(vecAcc0, vecIn0, c3);
            float32x4_t pap = vld1q_f32(partial_accu_ptr);
            vstrwq_p_f32(pOutput, vecAcc0+pap,p0);
            pOutput += cnt;
        }
    }

    /*
     * Copy the samples back into the history buffer start
     */
    pTempSrc = &pRefStatePtr[blockSize];
    pTempDest = pRefStatePtr;

    blkCnt = numTaps >> 2;
    while (blkCnt > 0)
    {
        vst1q(pTempDest, vld1q(pTempSrc));
        pTempSrc += 4;
        pTempDest += 4;
        blkCnt--;
    }
    blkCnt = numTaps & 3;
    if (blkCnt > 0)
    {
        mve_pred16_t p0 = vctp32q(blkCnt);
        vstrwq_p_f32(pTempDest, vld1q(pTempSrc), p0);
    }
}

#endif

#endif
/**
* @} end of FIR group
*/
