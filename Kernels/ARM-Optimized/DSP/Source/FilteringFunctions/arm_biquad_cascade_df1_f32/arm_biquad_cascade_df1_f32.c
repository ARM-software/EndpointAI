/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_biquad_cascade_df1_f32.c
 * Description:  Processing function for the floating-point Biquad cascade DirectFormI(DF1) filter
 *               using Helium assembly kernels
 *               This version allows boosting DF1 F32 Biquad performance when using compilers having suboptimal
 *               Helium intrinsic code generation.

 * $Date:        Jan 2022
 * $Revision:    V1.0.1
 *
 * Target Processor: Cortex-M with Helium
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
  @defgroup BiquadCascadeDF1 Biquad Cascade IIR Filters Using Direct Form I Structure

  This set of functions implements arbitrary order recursive (IIR) filters.
  The filters are implemented as a cascade of second order Biquad sections.
  The functions support Q15, Q31 and floating-point data types.
  Fast version of Q15 and Q31 also available.

  The functions operate on blocks of input and output data and each call to the function
  processes <code>blockSize</code> samples through the filter.
  <code>pSrc</code> points to the array of input data and
  <code>pDst</code> points to the array of output data.
  Both arrays contain <code>blockSize</code> values.

  @par           Algorithm
                   Each Biquad stage implements a second order filter using the difference equation:
  <pre>
      y[n] = b0 * x[n] + b1 * x[n-1] + b2 * x[n-2] + a1 * y[n-1] + a2 * y[n-2]
  </pre>
                  A Direct Form I algorithm is used with 5 coefficients and 4 state variables per stage.
                  \image html Biquad.gif "Single Biquad filter stage"
                  Coefficients <code>b0, b1 and b2 </code> multiply the input signal <code>x[n]</code> and are referred to as the feedforward coefficients.
                  Coefficients <code>a1</code> and <code>a2</code> multiply the output signal <code>y[n]</code> and are referred to as the feedback coefficients.
                  Pay careful attention to the sign of the feedback coefficients.
                  Some design tools use the difference equation
  <pre>
      y[n] = b0 * x[n] + b1 * x[n-1] + b2 * x[n-2] - a1 * y[n-1] - a2 * y[n-2]
  </pre>
                  In this case the feedback coefficients <code>a1</code> and <code>a2</code>
                  must be negated when used with the CMSIS DSP Library.

  @par
                   Higher order filters are realized as a cascade of second order sections.
                   <code>numStages</code> refers to the number of second order stages used.
                   For example, an 8th order filter would be realized with <code>numStages=4</code> second order stages.
                   \image html BiquadCascade.gif "8th order filter using a cascade of Biquad stages"
                   A 9th order filter would be realized with <code>numStages=5</code> second order stages with the coefficients for one of the stages configured as a first order filter (<code>b2=0</code> and <code>a2=0</code>).

  @par
                   The <code>pState</code> points to state variables array.
                   Each Biquad stage has 4 state variables <code>x[n-1], x[n-2], y[n-1],</code> and <code>y[n-2]</code>.
                   The state variables are arranged in the <code>pState</code> array as:
  <pre>
      {x[n-1], x[n-2], y[n-1], y[n-2]}
  </pre>

  @par
                   The 4 state variables for stage 1 are first, then the 4 state variables for stage 2, and so on.
                   The state array has a total length of <code>4*numStages</code> values.
                   The state variables are updated after each block of data is processed, the coefficients are untouched.

  @par           Instance Structure
                   The coefficients and state variables for a filter are stored together in an instance data structure.
                   A separate instance structure must be defined for each filter.
                   Coefficient arrays may be shared among several instances while state variable arrays cannot be shared.
                   There are separate instance structure declarations for each of the 3 supported data types.

  @par           Init Function
                   There is also an associated initialization function for each data type.
                   The initialization function performs following operations:
                   - Sets the values of the internal structure fields.
                   - Zeros out the values in the state buffer.
                   To do this manually without calling the init function, assign the follow subfields of the instance structure:
                   numStages, pCoeffs, pState. Also set all of the values in pState to zero.

  @par
                   Use of the initialization function is optional.
                   However, if the initialization function is used, then the instance structure cannot be placed into a const data section.
                   To place an instance structure into a const data section, the instance structure must be manually initialized.
                   Set the values in the state buffer to zeros before static initialization.
                   The code below statically initializes each of the 3 different data type filter instance structures
  <pre>
      arm_biquad_casd_df1_inst_f32 S1 = {numStages, pState, pCoeffs};
      arm_biquad_casd_df1_inst_q15 S2 = {numStages, pState, pCoeffs, postShift};
      arm_biquad_casd_df1_inst_q31 S3 = {numStages, pState, pCoeffs, postShift};
  </pre>
                   where <code>numStages</code> is the number of Biquad stages in the filter;
                   <code>pState</code> is the address of the state buffer;
                   <code>pCoeffs</code> is the address of the coefficient buffer;
                   <code>postShift</code> shift to be applied.

  @par           Fixed-Point Behavior
                   Care must be taken when using the Q15 and Q31 versions of the Biquad Cascade filter functions.
                   Following issues must be considered:
                   - Scaling of coefficients
                   - Filter gain
                   - Overflow and saturation

  @par           Scaling of coefficients
                   Filter coefficients are represented as fractional values and
                   coefficients are restricted to lie in the range <code>[-1 +1)</code>.
                   The fixed-point functions have an additional scaling parameter <code>postShift</code>
                   which allow the filter coefficients to exceed the range <code>[+1 -1)</code>.
                   At the output of the filter's accumulator is a shift register which shifts the result by <code>postShift</code> bits.
                   \image html BiquadPostshift.gif "Fixed-point Biquad with shift by postShift bits after accumulator"
                   This essentially scales the filter coefficients by <code>2^postShift</code>.
                   For example, to realize the coefficients
  <pre>
     {1.5, -0.8, 1.2, 1.6, -0.9}
  </pre>
                   set the pCoeffs array to:
  <pre>
     {0.75, -0.4, 0.6, 0.8, -0.45}
  </pre>
                   and set <code>postShift=1</code>

  @par           Filter gain
                   The frequency response of a Biquad filter is a function of its coefficients.
                   It is possible for the gain through the filter to exceed 1.0 meaning that the filter increases the amplitude of certain frequencies.
                   This means that an input signal with amplitude < 1.0 may result in an output > 1.0 and these are saturated or overflowed based on the implementation of the filter.
                   To avoid this behavior the filter needs to be scaled down such that its peak gain < 1.0 or the input signal must be scaled down so that the combination of input and filter are never overflowed.

  @par           Overflow and saturation
                   For Q15 and Q31 versions, it is described separately as part of the function specific documentation below.
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
  @param[in]     blockSize  number of samples to process
  @return        none
 */

#if defined(ARM_MATH_MVEF) && !defined(ARM_MATH_AUTOVECTORIZE)
#include "arm_helium_utils.h"

void arm_biquad_cascade_df1_f32_mve(
  const arm_biquad_casd_df1_inst_f32 * S,
  const float32_t * pSrc,
  float32_t * pDst,
  uint32_t blockSize)
{
    const float32_t *pIn;                       /*  source pointer          */
    float32_t       *pOut;                      /*  destination pointer     */
    const float32_t *pCoeffs = S->pCoeffs;      /*  coefficient pointer     */
    float32_t       *pState = S->pState;        /*  State pointer           */
    int              stage = S->numStages;
#ifdef USE_ASM
    int32_t          scratch[4];                /*  scratch for saving      */
                                                /*  block size & input      */
                                                /*  history                 */

    scratch[0] = blockSize;
#endif

    pIn = pSrc;
    pOut = pDst;

#ifdef USE_ASM

    do {
    __asm volatile (
            "Xn1                       .req r4                      \n"
            "Xn2                       .req r5                      \n"
            "Yn1                       .req r6                      \n"
            "Yn2                       .req r7                      \n"

            /* restore state */
            "   ldrd                Xn1, Xn2, [%[state]]            \n"
            "   ldrd                Yn1, Yn2, [%[state], #8]        \n"


            /* reload block size */
            "   ldr                 lr, [%[scratch]]                \n"
            "   sub                 r8, lr, #2                      \n"
            "   add                 r8, %[src], r8, lsl #2          \n"
            /*
                load the last 2 input elements for Fwd state saving
                Need to do it here as in-place processing corrupt these
            */
            "   ldrd                r8, r9, [r8]                    \n"
            "   strd                r8, r9, [%[scratch],#4]         \n"

            /* main biquad loop */
            /* process 4 samples per loop */

            /* 1st coef preloaded to avoid structural hazard  */
            /* after input samples loading */
            ".p2align 2                                             \n"
            "   wlstp.32            lr, lr, 1f                      \n"
            "   vldrw.32            q7, [%[coefs]]                  \n"

            /* low overhead loop start */
            "2:                                                     \n"
            "   ldrd                r0, r1, [%[src], 8]             \n"
            "   ldrd                r8, r9, [%[src]], #16           \n"
            "   vmul.f32            q0, q7, r1                      \n"
            "   vldrw.32            q1, [%[coefs], 16]              \n"
            "   vfma.f32            q0, q1, r0                      \n"
            "   vldrw.32            q1, [%[coefs], 32]              \n"
            "   vfma.f32            q0, q1, r9                      \n"
            "   vldrw.32            q1, [%[coefs], 48]              \n"
            "   vfma.f32            q0, q1, r8                      \n"
            "   vldrw.32            q1, [%[coefs], 64]              \n"
            "   vfma.f32            q0, q1, Xn1                     \n"
            "   vldrw.32            q1, [%[coefs], 80]              \n"
            "   vfma.f32            q0, q1, Xn2                     \n"
            "   vldrw.32            q1, [%[coefs], 96]              \n"
            "   vfma.f32            q0, q1, Yn1                     \n"
            "   vldrw.32            q1, [%[coefs], 112]             \n"
            "   vfma.f32            q0, q1, Yn2                     \n"
            "   vstrw.32            q0, [%[dst]], #16               \n"
            /* update recurrence */
            /* xn1 / xn2 */
            "   mov.n               Xn1, r1                         \n"
            "   mov.n               Xn2, r0                         \n"
            /* yn1 / yn2 */
            "   vmov                Yn2, Yn1, s2, s3                \n"
            /* low overhead loop end */
            "   letp                lr, 2b                          \n"
            "1:                                                     \n"


            /* modulo 4 tail handling for state */
            "   ldr                 r1, [%[scratch]]                \n"
            "   and                 r1, r1, #3                      \n"
            "   cmp                 r1, #0                          \n"
            "   beq                 0f                              \n"
            "   cmp                 r1, #1                          \n"
            "   beq                 1f                              \n"
            "   cmp                 r1, #2                          \n"
            "   beq                 2f                              \n"

            /* fall thru */
            "3:                                                     \n"
            /* save state for 3 remainders */
            /* last 2 valid input samples are r9 & r0 */
            "   strd                r0, r9, [%[state]], #8          \n"
            /* last 2 valid output samples are s1-s2 */
            "   vmov                r7, r6, s1, s2                  \n"
            "   strd                r6, r7, [%[state]], #8          \n"
            "   b                   cont%=                          \n"

            "2:                                                     \n"
            /* save state for 2 remainders */
            /* last 2 valid input samples are r8 & r9 */
            "   strd                r9, r8, [%[state]], #8          \n"
            /* last 2 valid output samples are s0-s1 */
            "   vmov                r7, r6, s0, s1                  \n"
            "   strd                r6, r7, [%[state]], #8          \n"
            "   b                   cont%=                          \n"

            "1:                                                     \n"
            /* save state for 1 remainder */
            /* last 2 valid input samples were pushed in the scratch */
            "   ldrd                r8, r9, [%[scratch], #4]        \n"
            "   strd                r9, r8, [%[state]], #8          \n"
            /* last 2 valid output samples are taken on the dst buffer*/
            "   ldrd                r9, r8, [%[dst], #-20]          \n"
            "   strd                r8, r9, [%[state]], #8          \n"
            "   b                   cont%=                          \n"

            "0:                                                     \n"
            /* save state if len is an exact multiple of 4 */
            "   strd                Xn1, Xn2, [%[state]], #8        \n"
            "   strd                Yn1, Yn2, [%[state]], #8        \n"
            "cont%=:                                                \n"

            " .unreq Xn1                                            \n"
            " .unreq Xn2                                            \n"
            " .unreq Yn1                                            \n"
            " .unreq Yn2                                            \n"

            :[state] "+r"(pState),[src] "+r"(pIn),[dst] "+r"(pOut)
            :[coefs] "r"(pCoeffs), [scratch] "r" (scratch)
            :"q0", "q1",
             "memory", "cc",
             "r4", "r5", "r6", "r7",
             "r8", "r9", "r0", "r1",
             "lr");

        pCoeffs += sizeof(arm_biquad_mod_coef_f32) / sizeof(float32_t);

        /* Stage 1 takes data from src->dst.  Subsequent stages go from dst->dst. */
        pIn = pDst;
        pOut = pDst;
     } while(--stage);
#else
    float32_t Xn1, Xn2, Yn1, Yn2;       /*  Filter pState variables   */
    float32_t lastX, lastY;             /*  X,Y history for tail handling */
    float32_t X0, X1, X2, X3;           /*  temporary input           */
    f32x4_t coeffs;
    f32x4_t accVec;                     /* accumulator  vector */
    uint32_t  sample;                   /*  loop counters             */

    do
    {
        /*
         * Reading the pState values
         */
        Xn1 = pState[0];
        Xn2 = pState[1];
        Yn1 = pState[2];
        Yn2 = pState[3];

        sample = blockSize >> 2U;

        /*
         * First part of the processing with loop unrolling.  Compute 4 outputs at a time.
         * second loop below computes the remaining 1 to 3 samples.
         */
        while (sample > 0U)
        {
            X0 = *pIn++;
            X1 = *pIn++;
            X2 = *pIn++;
            X3 = *pIn++;

            coeffs = vld1q(pCoeffs);
            accVec = vmulq(coeffs, X3);

            coeffs = vld1q(&pCoeffs[4]);
            accVec = vfmaq(accVec, coeffs, X2);

            coeffs = vld1q(&pCoeffs[8]);
            accVec = vfmaq(accVec, coeffs, X1);

            coeffs = vld1q(&pCoeffs[12]);
            accVec = vfmaq(accVec, coeffs, X0);

            coeffs = vld1q(&pCoeffs[16]);
            accVec = vfmaq(accVec, coeffs, Xn1);

            coeffs = vld1q(&pCoeffs[20]);
            accVec = vfmaq(accVec, coeffs, Xn2);

            coeffs = vld1q(&pCoeffs[24]);
            accVec = vfmaq(accVec, coeffs, Yn1);

            coeffs = vld1q(&pCoeffs[28]);
            accVec = vfmaq(accVec, coeffs, Yn2);
            /*
             * Store the result in the accumulator in the destination buffer.
             */
            vst1q(pOut, accVec);
            pOut += 4;

            /*
             * update recurrence
             */
            Xn1 = X3;
            Xn2 = X2;
            Yn1 = vgetq_lane(accVec, 3);
            Yn2 = vgetq_lane(accVec, 2);
            /*
             * decrement the loop counter
             */
            sample--;
        }

        /*
         * If the blockSize is not a multiple of 4,
         * compute any remaining output samples here.
         */
        sample = blockSize & 0x3U;
        if (sample)
        {
            /* save previous X, Y for modulo 1 length case */
            lastX = X3;
            lastY = Yn1;

            X0 = *pIn++;
            X1 = *pIn++;
            X2 = *pIn++;
            X3 = *pIn++;

            coeffs = vld1q(pCoeffs);
            accVec = vmulq(coeffs, X3);

            coeffs = vld1q(&pCoeffs[4]);
            accVec = vfmaq(accVec, coeffs, X2);

            coeffs = vld1q(&pCoeffs[8]);
            accVec = vfmaq(accVec, coeffs, X1);

            coeffs = vld1q(&pCoeffs[12]);
            accVec = vfmaq(accVec, coeffs, X0);

            coeffs = vld1q(&pCoeffs[16]);
            accVec = vfmaq(accVec, coeffs, Xn1);

            coeffs = vld1q(&pCoeffs[20]);
            accVec = vfmaq(accVec, coeffs, Xn2);

            coeffs = vld1q(&pCoeffs[24]);
            accVec = vfmaq(accVec, coeffs, Yn1);

            coeffs = vld1q(&pCoeffs[28]);
            accVec = vfmaq(accVec, coeffs, Yn2);

            if (sample == 1)
            {
                *pOut++ = vgetq_lane(accVec, 0);
                Xn1 = X0;
                Xn2 = lastX;
                Yn1 = vgetq_lane(accVec, 0);
                Yn2 = lastY;
            }
            else if (sample == 2)
            {
                *pOut++ = vgetq_lane(accVec, 0);
                *pOut++ = vgetq_lane(accVec, 1);
                Xn1 = X1;
                Xn2 = X0;
                Yn1 = vgetq_lane(accVec, 1);
                Yn2 = vgetq_lane(accVec, 0);
            }
            else
            {
                *pOut++ = vgetq_lane(accVec, 0);
                *pOut++ = vgetq_lane(accVec, 1);
                *pOut++ = vgetq_lane(accVec, 2);
                Xn1 = X2;
                Xn2 = X1;
                Yn1 = vgetq_lane(accVec, 2);
                Yn2 = vgetq_lane(accVec, 1);
            }
        }
        /*
         * Store the updated state variables back into the pState array
         */
        *pState++ = Xn1;
        *pState++ = Xn2;
        *pState++ = Yn1;
        *pState++ = Yn2;

        pCoeffs += sizeof(arm_biquad_mod_coef_f32) / sizeof(float32_t);
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


#else
#if defined(ARM_MATH_NEON)  && !defined(ARM_MATH_AUTOVECTORIZE)

#include <arm_neon.h>
#include "arm_biquad_cascade_df1_f32.h"

#ifndef USE_ASM

void arm_biquad_cascade_df1_f32_neon(const arm_biquad_casd_df1_inst_f32 * S,
                                     const float32_t * pSrc, float32_t * pDst,
                                     uint32_t blockSize)
{
    const float32_t *pIn = pSrc;            /*  source pointer            */
    float32_t      *pOut = pDst;            /*  destination pointer       */
    float32_t      *pState = S->pState;     /*  pState pointer            */
    const float32_t *pCoeffs = S->pCoeffs;  /*  coefficient pointer       */
    float32_t       Xn1, Xn2, Yn1, Yn2;     /*  Filter pState variables   */
    float32_t       lastX, lastY;           /*  X,Y history for tail handling */
    float32_t       X0, X1, X2, X3;         /*  temporary input           */
    uint32_t        sample, stage;          /*  loop counters             */

    stage = S->numStages;
    do {

        f32x4_t         x0Vec, x1Vec;
        f32x4_t         accSumVec, accVec0, accVec1, accVec2, accVec3;
        f32x4_t         coeffs[8];

        /*
         * Reading the pState values
         */
        x0Vec[3] = pState[0];           // Xn1
        x0Vec[2] = pState[1];           // Xn2
        accSumVec[3] = pState[2];       // Yn1
        accSumVec[2] = pState[3];       // Yn2

        /* save last input before being corrupted when running in-place */
        Xn1 = pIn[blockSize - 1];
        Xn2 = pIn[blockSize - 2];

        sample = blockSize >> 3U;

        for (int i = 0; i < 8; i++)
            coeffs[i] = vld1q_f32(&pCoeffs[4 * i]);

        /*
         * First part of the processing
         * Compute 8 outputs at a time.
         * SW design is motivated by vector MAC latency constraint
         * It allows partial interleaving of 2 x 4 elements vectors
         * allowing to add space between 2 depending MAC operations
         */
        while (sample > 0U) {

            /*
             * load next 4 inputs
             */
            x1Vec = vld1q_f32(pIn);
            pIn += 4;

            /* split processing using 2 accumulators */
            /* to absorb MAC latency (Core specific) */
            accVec0 = vmulq_n_f32(coeffs[0], x1Vec[3]);
            accVec0 = vfmaq_n_f32(accVec0, coeffs[1], x1Vec[2]);
            accVec0 = vfmaq_n_f32(accVec0, coeffs[4], x0Vec[3]);
            accVec0 = vfmaq_n_f32(accVec0, coeffs[5], x0Vec[2]);

            accVec1 = vmulq_n_f32(coeffs[2], x1Vec[1]);
            accVec1 = vfmaq_n_f32(accVec1, coeffs[3], x1Vec[0]);
            accVec1 = vfmaq_n_f32(accVec1, coeffs[6], accSumVec[3]);
            accVec1 = vfmaq_n_f32(accVec1, coeffs[7], accSumVec[2]);

            accSumVec = vaddq_f32(accVec0, accVec1);
            vst1q_f32(pOut, accSumVec);
            pOut += 4;


            /*
             * load next 4 inputs
             */
            x0Vec = vld1q_f32(pIn);
            pIn += 4;

            /* same process as before, X vectors are toggled */
            accVec2 = vmulq_n_f32(coeffs[0], x0Vec[3]);
            accVec2 = vfmaq_n_f32(accVec2, coeffs[1], x0Vec[2]);
            accVec2 = vfmaq_n_f32(accVec2, coeffs[4], x1Vec[3]);
            accVec2 = vfmaq_n_f32(accVec2, coeffs[5], x1Vec[2]);

            accVec3 = vmulq_n_f32(coeffs[2], x0Vec[1]);
            accVec3 = vfmaq_n_f32(accVec3, coeffs[3], x0Vec[0]);
            accVec3 = vfmaq_n_f32(accVec3, coeffs[6], accSumVec[3]);
            accVec3 = vfmaq_n_f32(accVec3, coeffs[7], accSumVec[2]);

            accSumVec = vaddq_f32(accVec2, accVec3);
            vst1q_f32(pOut, accSumVec);
            pOut += 4;

            /*
             * decrement the loop counter
             */
            sample--;
        }

        Yn1 = accSumVec[3];
        Yn2 = accSumVec[2];


        /*
         * If the blockSize is not a multiple of 8,
         * compute any remaining output samples here.
         */
        sample = blockSize & 0x7U;
        if (sample > 4) {

            x1Vec = vld1q_f32(pIn);
            pIn += 4;

            accVec0 = vmulq_n_f32(coeffs[0], x1Vec[3]);
            accVec0 = vfmaq_n_f32(accVec0, coeffs[1], x1Vec[2]);
            accVec0 = vfmaq_n_f32(accVec0, coeffs[4], x0Vec[3]);
            accVec0 = vfmaq_n_f32(accVec0, coeffs[5], x0Vec[2]);

            accVec1 = vmulq_n_f32(coeffs[2], x1Vec[1]);
            accVec1 = vfmaq_n_f32(accVec1, coeffs[3], x1Vec[0]);
            accVec1 = vfmaq_n_f32(accVec1, coeffs[6], accSumVec[3]);
            accVec1 = vfmaq_n_f32(accVec1, coeffs[7], accSumVec[2]);

            accSumVec = vaddq_f32(accVec0, accVec1);
            lastY = vgetq_lane_f32(accSumVec, 3);
            vst1q_f32(pOut, accSumVec);
            pOut += 4;


            x0Vec = vld1q_f32(pIn);
            pIn += 4;

            accVec2 = vmulq_n_f32(coeffs[0], x0Vec[3]);
            accVec2 = vfmaq_n_f32(accVec2, coeffs[1], x0Vec[2]);
            accVec2 = vfmaq_n_f32(accVec2, coeffs[4], x1Vec[3]);
            accVec2 = vfmaq_n_f32(accVec2, coeffs[5], x1Vec[2]);

            accVec3 = vmulq_n_f32(coeffs[2], x0Vec[1]);
            accVec3 = vfmaq_n_f32(accVec3, coeffs[3], x0Vec[0]);
            accVec3 = vfmaq_n_f32(accVec3, coeffs[6], accSumVec[3]);
            accVec3 = vfmaq_n_f32(accVec3, coeffs[7], accSumVec[2]);

            accSumVec = vaddq_f32(accVec2, accVec3);

            if (sample == 5) {
                *pOut++ = vgetq_lane_f32(accSumVec, 0);
                Yn1 = vgetq_lane_f32(accSumVec, 0);
                Yn2 = lastY;
            } else if (sample == 6) {
                *pOut++ = vgetq_lane_f32(accSumVec, 0);
                *pOut++ = vgetq_lane_f32(accSumVec, 1);
                Yn1 = vgetq_lane_f32(accSumVec, 1);
                Yn2 = vgetq_lane_f32(accSumVec, 0);
            } else {
                *pOut++ = vgetq_lane_f32(accSumVec, 0);
                *pOut++ = vgetq_lane_f32(accSumVec, 1);
                *pOut++ = vgetq_lane_f32(accSumVec, 2);
                Yn1 = vgetq_lane_f32(accSumVec, 2);
                Yn2 = vgetq_lane_f32(accSumVec, 1);
            }
        } else if (sample > 0) {

            x1Vec = vld1q_f32(pIn);
            pIn += 4;

            accVec0 = vmulq_n_f32(coeffs[0], x1Vec[3]);
            accVec0 = vfmaq_n_f32(accVec0, coeffs[1], x1Vec[2]);
            accVec0 = vfmaq_n_f32(accVec0, coeffs[4], x0Vec[3]);
            accVec0 = vfmaq_n_f32(accVec0, coeffs[5], x0Vec[2]);

            accVec1 = vmulq_n_f32(coeffs[2], x1Vec[1]);
            accVec1 = vfmaq_n_f32(accVec1, coeffs[3], x1Vec[0]);
            accVec1 = vfmaq_n_f32(accVec1, coeffs[6], accSumVec[3]);
            accVec1 = vfmaq_n_f32(accVec1, coeffs[7], accSumVec[2]);

            accSumVec = vaddq_f32(accVec0, accVec1);
            lastY = vgetq_lane_f32(accSumVec, 3);

            if (sample == 1) {
                *pOut++ = vgetq_lane_f32(accSumVec, 0);
                Yn2 = Yn1;
                Yn1 = vgetq_lane_f32(accSumVec, 0);
            } else if (sample == 2) {
                *pOut++ = vgetq_lane_f32(accSumVec, 0);
                *pOut++ = vgetq_lane_f32(accSumVec, 1);
                Yn1 = vgetq_lane_f32(accSumVec, 1);
                Yn2 = vgetq_lane_f32(accSumVec, 0);
            } else if (sample == 3) {
                *pOut++ = vgetq_lane_f32(accSumVec, 0);
                *pOut++ = vgetq_lane_f32(accSumVec, 1);
                *pOut++ = vgetq_lane_f32(accSumVec, 2);
                Yn1 = vgetq_lane_f32(accSumVec, 2);
                Yn2 = vgetq_lane_f32(accSumVec, 1);
            } else if (sample == 4) {
                vst1q_f32(pOut, accSumVec);
                Yn1 = vgetq_lane_f32(accSumVec, 3);
                Yn2 = vgetq_lane_f32(accSumVec, 2);
            }
        }

        /*
         * Store the updated state variables back into the pState array
         */
        *pState++ = Xn1;
        *pState++ = Xn2;
        *pState++ = Yn1;
        *pState++ = Yn2;


        pCoeffs += sizeof(arm_biquad_mod_coef_f32) / sizeof(float32_t);
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
}

#else   // USE_ASM


void arm_biquad_cascade_df1_f32_neon(const arm_biquad_casd_df1_inst_f32 * S,
                                     const float32_t * pSrc, float32_t * pDst, uint32_t blockSize)
{
    const float32_t *pIn = pSrc;        /*  source pointer            */
    float32_t      *pOut = pDst;        /*  destination pointer       */
    float32_t      *pState = S->pState; /*  pState pointer            */
    const float32_t *pCoeffs = S->pCoeffs;      /*  coefficient pointer       */
    float32_t       Xn1, Xn2, Yn1, Yn2; /*  Filter pState variables   */
    float32_t       lastX, lastY;       /*  X,Y history for tail handling */
    float32_t       X0, X1, X2, X3;     /*  temporary input           */
    f32x4_t         coeffs[8];
    f32x4_t         accVec;             /* accumulator vector */
    uint32_t        sample, stage = S->numStages;       /*  loop counters             */
    uint32_t tail = blockSize & 0x7U;


    do {
        /* main loop produces blocks of 8 samples */

        sample = blockSize >> 3U;
        const float32_t *pCoeffsCur =  pCoeffs;

        /* save last input before being corrupted when running in-place */
        Xn1 = pIn[blockSize - 1];
        Xn2 = pIn[blockSize - 2];

#ifdef __aarch64__
        __asm volatile (
            ".p2align 2                                           \n"

            /* restore state */

            /* Xn1 */
            "   ld1             { v2.s }[3], [%[pState]], #4      \n"
            /* Xn2 */
            "   ld1             { v2.s }[2], [%[pState]], #4      \n"

            /* Yn1 */
            "   ld1             { v3.s }[3], [%[pState]], #4      \n"
            /* Yn2 */
            "   ld1             { v3.s }[2], [%[pState]]          \n"
            "   sub             %[pState], %[pState], #12         \n"

            /* load 8 vector coefs in V4-Q11*/
            "   ld1             {v4.4s-v7.4s},[%[pCoeffsCur]],#64 \n"
            "   ld1             {v8.4s-v11.4s},[%[pCoeffsCur]]    \n"

            /* preloading + precomputing for efficient pipelining */
            "   ld1             {v0.4s},[%[pIn]],#16              \n"

            "   fmul            v14.4s,v8.4s,v2.s[3]              \n"
            "   fmla            v14.4s,v9.4s,v2.s[2]              \n"

            "   ld1             {v2.4s},[%[pIn]],#16              \n"

            "   fmul            v15.4s,v5.4s,v0.s[2]              \n"
            "   fmla            v15.4s,v7.4s,v0.s[0]              \n"
            "   fmul            v12.4s,v8.4s,v0.s[3]              \n"
            "   fmla            v12.4s,v9.4s,v0.s[2]              \n"


            /* Core loop
             * Compute 8 outputs at a time.
             * SW design is motivated by vector MAC latency constraint
             * It allows partial interleaving of 2 x 4 elements vectors
             * allowing to add space between 2 depending MAC operations
             * 1st half is doing partial accumulation in V14-V15
             * 2nd half is doing partial accumulation in V12-V13
             */
            "1:                                                   \n"
            "   fmla            v14.4s,v4.4s,v0.s[3]              \n"
            "   fmla            v15.4s,v10.4s,v3.s[3]             \n"
            "   fmul            v13.4s,v4.4s,v2.s[3]              \n"
            "   fmla            v13.4s,v5.4s,v2.s[2]              \n"
            "   fmla            v14.4s,v6.4s,v0.s[1]              \n"
            "   fmla            v15.4s,v11.4s,v3.s[2]             \n"
            "   ld1             {v0.4s},[%[pIn]],#16              \n"
            "   fmla            v12.4s,v6.4s,v2.s[1]              \n"
            "   fmla            v13.4s,v7.4s,v2.s[0]              \n"

            /* sum partial 1st half partial accumulators */
            "   fadd            v3.4s,v14.4s,v15.4s               \n"

            /* decrement loop counter */
            "   subs            %w[sample], %w[sample], #1        \n"

            "   fmul            v14.4s,v8.4s,v2.s[3]              \n"
            "   fmla            v14.4s,v9.4s,v2.s[2]              \n"
            "   fmla            v12.4s,v10.4s,v3.s[3]             \n"
            "   fmla            v13.4s,v11.4s,v3.s[2]             \n"
            "   fmul            v15.4s,v7.4s,v0.s[0]              \n"
            "   st1             {v3.4s},[%[pOut]],#16             \n"
            "   ld1             {v2.4s},[%[pIn]],#16              \n"
            "   fmla            v15.4s,v5.4s,v0.s[2]              \n"
            /* sum partial 2nd half partial accumulators */
            "   fadd            v3.4s,v12.4s,v13.4s               \n"
            "   fmul            v12.4s,v8.4s,v0.s[3]              \n"
            "   fmla            v12.4s,v9.4s,v0.s[2]              \n"
            "   st1             {v3.4s},[%[pOut]],#16             \n"
            "   bgt             1b                                \n"


            /*
             * If the blockSize is not a multiple of 8,
             * compute any remaining output samples here.
             */
            "   add             %[pState], %[pState], #8         \n"

            "   cmp             %w[tail], #5                     \n"
            "   blo             tail_le4%=                       \n"


            "   fmla            v14.4s,v4.4s,v0.s[3]             \n"
            "   fmla            v15.4s,v10.4s,v3.s[3]            \n"
            "   fmul            v13.4s,v4.4s,v2.s[3]             \n"
            "   fmla            v13.4s,v5.4s,v2.s[2]             \n"
            "   fmla            v14.4s,v6.4s,v0.s[1]             \n"
            "   fmla            v15.4s,v11.4s,v3.s[2]            \n"
            "   fmla            v12.4s,v6.4s,v2.s[1]             \n"
            "   fmla            v13.4s,v7.4s,v2.s[0]             \n"

            /* sum partial 1st half partial accumulators */
            "   fadd            v3.4s,v14.4s,v15.4s              \n"

            "   fmla            v12.4s,v10.4s,v3.s[3]            \n"
            "   fmla            v13.4s,v11.4s,v3.s[2]            \n"
            "   st1            {v3.4s},[%[pOut]],#16             \n"

            /* save Yn2 for state*/
            "   mov             v1.s[0], v3.s[3]                 \n"
            /* sum partial 2nd half partial accumulators */
            "   fadd            v3.4s,v12.4s,v13.4s              \n"


            "   cmp             %w[tail], #5                     \n"
            "   beq             5f                               \n"
            "   cmp             %w[tail], #6                     \n"
            "   beq             6f                               \n"

            /* save remaining samples + state for different residuals */
            "7:                                                  \n"
            "   st1             {v3.2s},[%[pOut]],#8             \n"
            "   st1             { v3.s }[2], [%[pOut]]           \n"
            "   st1             { v3.s }[2], [%[pState]], #4     \n"
            "   st1             { v3.s }[1], [%[pState]], #4     \n"
            "   b               cont%=                           \n"

            "5:                                                  \n"
            "   st1             { v3.s }[0], [%[pOut]]           \n"
            "   st1             { v3.s }[0], [%[pState]], #4     \n"
            "   st1             { v1.s }[0], [%[pState]], #4     \n"
            "   b               cont%=                           \n"

            "6:                                                  \n"
            "   st1             {v3.2s},[%[pOut]]                \n"
            "   rev64           v3.2s, v3.2s                     \n"
            "   st1             {v3.2s},[%[pState]], #8          \n"
            "   b               cont%=                           \n"

            /* tail lower or equal than 4 */
            "tail_le4%=:                                         \n"
            "   cmp             %w[tail], #0                     \n"
            /* exact multiple of 8 */
            "   beq             tail_0%=                         \n"

            /* save Yn2 */
            "   mov             v1.s[0], v3.s[3]                 \n"

            "   fmla            v14.4s,v4.4s,v0.s[3]             \n"
            "   fmla            v15.4s,v10.4s,v3.s[3]            \n"
            "   fmla            v14.4s,v6.4s,v0.s[1]             \n"
            "   fmla            v15.4s,v11.4s,v3.s[2]            \n"
            /* sum partial 1st half partial accumulators */
            "   fadd            v3.4s,v14.4s,v15.4s              \n"


            "   cmp             %w[tail], #1                     \n"
            "   beq             1f                               \n"
            "   cmp             %w[tail], #2                     \n"
            "   beq             2f                               \n"
            "   cmp             %w[tail], #3                     \n"
            "   beq             3f                               \n"

            /* save remaining samples + state for different residuals */
            "4:                                                  \n"
            "   st1             {v3.4s},[%[pOut]]                \n"
            "   st1             {v3.s}[3], [%[pState]], #4       \n"
            "   st1             {v3.s}[2], [%[pState]], #4       \n"
            "   b               cont%=                           \n"

            "1:                                                  \n"
            "   st1             {v3.s}[0], [%[pOut]]             \n"
            "   st1             {v3.s}[0], [%[pState]], #4       \n"
            "   st1             {v1.s}[0], [%[pState]], #4       \n"
            "   b               cont%=                           \n"

            "2:                                                  \n"
            "   st1             {v3.2s},[%[pOut]]                \n"
            "   rev64           v3.2s, v3.2s                     \n"
            "   st1             {v3.2s},[%[pState]], #8          \n"
            "   b               cont%=                           \n"

            "3:                                                  \n"
            "   st1             {v3.2s},[%[pOut]],#8             \n"
            "   st1             {v3.s}[2], [%[pOut]]             \n"
            "   st1             {v3.s}[2], [%[pState]], #4       \n"
            "   st1             {v3.s}[1], [%[pState]], #4       \n"
            "   b               cont%=                           \n"

            "tail_0%=:                                           \n"
            "   st1             {v3.s}[3], [%[pState]], #4       \n"
            "   st1             {v3.s}[2], [%[pState]], #4       \n"

            /* save last inputs + increment state pointer */
            "cont%=:                                             \n"
            "   sub             %[pState], %[pState], #16        \n"
            "   stp             %w[Xn1], %w[Xn2],[%[pState]],#16 \n"


        : [sample] "+r" (sample), [pCoeffsCur] "+r" (pCoeffsCur),
          [pIn] "+r" (pIn), [pOut] "+r" (pOut),
          [pState] "+r" (pState)
        : [Xn1] "r" (Xn1), [Xn2] "r" (Xn2), [tail] "r" (tail)
        : "v0", "v1", "v2", "v3",
          "v4", "v5", "v6", "v7",
          "v8", "v9", "v10", "v11",
          "v12", "v13", "v14", "v15",
          "cc", "memory"
        );

#else  // __aarch32__

        __asm volatile (
            ".p2align 2                                           \n"
            /* restore state */
            "   vld1.64         {d0,d1},[%[pState]]               \n"
            "   vrev64.32       d5, d0                            \n"
            "   vrev64.32       d7, d1                            \n"


            /* load 8 vector coefs in Q4-Q11*/
            "   vld1.64         {q4}, [%[pCoeffsCur]:128]!        \n"
            "   vld1.64         {q5}, [%[pCoeffsCur]:128]!        \n"
            "   vld1.64         {q6}, [%[pCoeffsCur]:128]!        \n"
            "   vld1.64         {q7}, [%[pCoeffsCur]:128]!        \n"
            "   vld1.64         {q8}, [%[pCoeffsCur]:128]!        \n"
            "   vld1.64         {q9}, [%[pCoeffsCur]:128]!        \n"
            "   vld1.64         {q10},[%[pCoeffsCur]:128]!        \n"
            "   vld1.64         {q11},[%[pCoeffsCur]:128]!        \n"

            /* preloading + precomputing for efficient pipelining */
            "   vld1.64         {d0,d1},[%[pIn]:128]!             \n"
            "   vmul.f32        q14, q8, d5[1]                    \n"
            "   vmla.f32        q14, q9, d5[0]                    \n"
            "   vld1.64         {d4,d5},[%[pIn]:128]!             \n"
            "   vmul.f32        q15,q5,d1[0]                      \n"
            "   vmla.f32        q15,q7,d0[0]                      \n"
            "   vmul.f32        q12,q8,d1[1]                      \n"
            "   vmla.f32        q12,q9,d1[0]                      \n"

            /* Core loop
             * Compute 8 outputs at a time.
             * SW design is motivated by vector MAC latency constraint
             * It allows partial interleaving of 2 x 4 elements vectors
             * allowing to add space between 2 depending MAC operations
             * 1st half is doing partial accumulation in Q14-Q15
             * 2nd half is doing partial accumulation in Q12-Q13
             */
            "1:                                                   \n"
            "   vmla.f32        q14,q4,d1[1]                      \n"
            "   vmla.f32        q15,q10,d7[1]                     \n"
            "   vmul.f32        q13,q4,d5[1]                      \n"
            "   vmla.f32        q13,q5,d5[0]                      \n"
            "   vmla.f32        q14,q6,d0[1]                      \n"
            "   vmla.f32        q15,q11,d7[0]                     \n"

            "   vld1.64         {d0,d1},[%[pIn]:128]!             \n"
            "   vmla.f32        q12,q6,d4[1]                      \n"
            "   vmla.f32        q13,q7,d4[0]                      \n"
            /* sum partial 1st half partial accumulators */
            "   vadd.f32        q3,q14,q15                        \n"

            /* decrement loop counter */
            "   subs            %[sample],#1                      \n"
            "   vmul.f32        q14,q8,d5[1]                      \n"
            "   vmla.f32        q14,q9,d5[0]                      \n"
            "   vmla.f32        q12,q10,d7[1]                     \n"
            "   vmla.f32        q13,q11,d7[0]                     \n"
            "   vmul.f32        q15,q7,d0[0]                      \n"
            "   vst1.64         {d6,d7},[%[pOut]:128]!            \n"
            "   vld1.64         {d4,d5},[%[pIn]:128]!             \n"
            "   vmla.f32        q15,q5,d1[0]                      \n"
            /* sum partial 2nd half partial accumulators */
            "   vadd.f32        q3,q12,q13                        \n"
            "   vmul.f32        q12,q8,d1[1]                      \n"
            "   vmla.f32        q12,q9,d1[0]                      \n"
            "   vst1.64         {d6,d7},[%[pOut]:128]!            \n"
            "   bgt             1b                                \n"


            /*
             * If the blockSize is not a multiple of 8,
             * compute any remaining output samples here.
             */
            "   cmp             %[tail], #5                       \n"
            "   blo             tail_le4%=                        \n"


            /* tail bigger than 4 */
            "   vmla.f32        q14,q4,d1[1]                      \n"
            "   vmla.f32        q15,q10,d7[1]                     \n"
            "   vmul.f32        q13,q4,d5[1]                      \n"
            "   vmla.f32        q13,q5,d5[0]                      \n"
            "   vmla.f32        q14,q6,d0[1]                      \n"
            "   vmla.f32        q15,q11,d7[0]                     \n"

            "   vmla.f32        q12,q6,d4[1]                      \n"
            "   vmla.f32        q13,q7,d4[0]                      \n"
            /* sum partial 1st half partial accumulators */
            "   vadd.f32        q3,q14,q15                        \n"
            "   vmla.f32        q12,q10,d7[1]                     \n"
            "   vmla.f32        q13,q11,d7[0]                     \n"
            "   vst1.64         {d6,d7},[%[pOut]:128]!            \n"
            /* save Yn2 for state*/
            "   vmov            d2, d7                            \n"
            /* sum partial 2nd half partial accumulators */
            "   vadd.f32        q3,q12,q13                        \n"


            "   cmp             %[tail], #5                       \n"
            "   beq             5f                                \n"
            "   cmp             %[tail], #6                       \n"
            "   beq             6f                                \n"

            /* save remaining samples + state for different residuals */
            "7:                                                   \n"
            "   vst1.64         d6, [%[pOut]]!                    \n"
            "   vst1.32         d7[0], [%[pOut]]                  \n"
            "   vstr            s14,[%[pState], #(2*4)]           \n"
            "   vstr            s13,[%[pState], #(3*4)]           \n"
            "   b               cont%=                            \n"

            "5:                                                   \n"
            "   vst1.32         d6[0], [%[pOut]:32]               \n"
            "   vstr            s5,[%[pState], #(3*4)]            \n"
            "   vstr            s12,[%[pState], #(2*4)]           \n"
            "   b               cont%=                            \n"

            "6:                                                   \n"
            "   vrev64.32       d0, d6                            \n"
            "   vstr            d0,[%[pState], #(2*4)]            \n"
            "   vst1.64         d6, [%[pOut]]                     \n"
            "   b               cont%=                            \n"


            /* tail lower or equal than 4 */
            "tail_le4%=:                                          \n"
            "   cmp             %[tail], #0                       \n"
            /* exact multiple of 8 */
            "   beq             tail_0%=                          \n"

            /* save Yn2 */
            "   vmov            d2, d7                            \n"
            "   vmla.f32        q14,q4,d1[1]                      \n"
            "   vmla.f32        q15,q10,d7[1]                     \n"
            "   vmla.f32        q14,q6,d0[1]                      \n"
            "   vmla.f32        q15,q11,d7[0]                     \n"
            /* sum partial 1st half partial accumulators */
            "   vadd.f32        q3,q14,q15                        \n"

            "   cmp             %[tail], #1                       \n"
            "   beq             1f                                \n"
            "   cmp             %[tail], #2                       \n"
            "   beq             2f                                \n"
            "   cmp             %[tail], #3                       \n"
            "   beq             3f                                \n"

            /* save remaining samples + state for different residuals */
            "4:                                                   \n"
            "   vst1.64         {d6,d7},[%[pOut]:128]!            \n"
            "   vrev64.32       d0, d7                            \n"
            "   vstr            d0,[%[pState], #(2*4)]            \n"
            "   b               cont%=                            \n"

            "1:                                                   \n"
            "   vst1.32         d6[0], [%[pOut]:32]               \n"
            "   vstr            s5,[%[pState], #(3*4)]            \n"
            "   vstr            s12,[%[pState], #(2*4)]           \n"
            "   b               cont%=                            \n"

            "2:                                                   \n"
            "   vrev64.32       d0, d6                            \n"
            "   vstr            d0,[%[pState], #(2*4)]            \n"
            "   vst1.64         d6, [%[pOut]]                     \n"
            "   b               cont%=                            \n"

            "3:                                                   \n"
            "   vst1.64         d6, [%[pOut]]!                    \n"
            "   vst1.32         d7[0], [%[pOut]]                  \n"
            "   vstr            s14,[%[pState], #(2*4)]           \n"
            "   vstr            s13,[%[pState], #(3*4)]           \n"
            "   b               cont%=                            \n"


            "tail_0%=:                                            \n"
            "   vrev64.32       d6, d7                            \n"
            "   vstr            d6,[%[pState], #(2*4)]            \n"

            /* save last inputs + increment state pointer */
            "cont%=:                                              \n"
            //  strd            %[Xn1], %[Xn2], [%[pState]], #16
            "   str             %[Xn1], [%[pState]], #4           \n"
            "   str             %[Xn2], [%[pState]], #12          \n"

        : [sample] "+r" (sample), [pCoeffsCur] "+r" (pCoeffsCur),
          [pIn] "+r" (pIn), [pOut] "+r" (pOut),
          [pState] "+r" (pState)
        : [Xn1] "r" (Xn1), [Xn2] "r" (Xn2), [tail] "r" (tail)
        : "q0","d2","q2","q3",
          "q4","q5","q6","q7",
          "q8","q9","q10","q11",
          "q12","q13","q14","q15",
          "cc", "memory"
        );

#endif  // __aarch64__

        pCoeffs += sizeof(arm_biquad_mod_coef_f32) / sizeof(float32_t);
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
}

#endif

#endif // ARM_MATH_NEON

#endif
/**
  @} end of BiquadCascadeDF1 group
 */
