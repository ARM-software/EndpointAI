/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_rfft_fast_f16.c
 * Description:  RFFT & RIFFT Floating point process function
 *               using Helium assembly kernels
 *               This version allows boosting RFFT F16 performance when using compilers having suboptimal
 *               Helium intrinsic code generation.
 *
 * $Date:        Jan 2022
 * $Revision:    V1.0.0
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

#include "dsp/transform_functions_f16.h"
#include "arm_common_tables_f16.h"

#if defined(ARM_FLOAT16_SUPPORTED)


#if defined(ARM_MATH_MVE_FLOAT16)

void arm_cfft_f16_mve(
    const arm_cfft_instance_f16 * S,
    float16_t * p1,
    uint8_t ifftFlag,
    uint8_t bitReverseFlag);


#ifndef USE_ASM
#define MVE_CMPLX_MULT_FLT_AxB_Pred(A,B,p)        vcmlaq_rot90_m(vcmulq_x(A, B, p), A, B, p)
#define MVE_CMPLX_MULT_FLT_Conj_AxB_Pred(A,B,p)   vcmlaq_rot270_m(vcmulq_x(A, B, p), A, B, p)
#endif


static void stage_rfft_f16_mve(const arm_rfft_fast_instance_f16 * S,
                        float16_t * p, float16_t * pOut)
{
    const float16_t *pCoeff = S->pTwiddleRFFT;  /* Points to RFFT Twiddle factors */
    float16_t      *pA = p;     /* increasing pointer */
    float16_t      *pB = p;     /* decreasing pointer */
    float16_t       xAR, xAI, xBR, xBI; /* temporary variables */
    float16_t       t1a, t1b;   /* temporary variables */
#ifndef USE_ASM
    float16x8_t     tw, xA, xB;
    float16x8_t     tmp1, tmp2, res;
#endif
    uint32x4_t      vecStridesBkwd;
    float16x8_t     conj = vcvtq_f16_s16(vsubq_n_s16(vdwdupq_u16(2, 4, 2), 1));
    int32_t         blockCnt;
    uint32_t        fftSize;


    fftSize = (S->Sint).fftLen - 1;

    vecStridesBkwd[0] = fftSize;
    vecStridesBkwd[1] = fftSize - 1;
    vecStridesBkwd[2] = fftSize - 2;
    vecStridesBkwd[3] = fftSize - 3;


    /* Pack first and last sample of the frequency domain together */
    xBR = pB[0];
    xBI = pB[1];
    xAR = pA[0];
    xAI = pA[1];

    pCoeff+=2 ;

    // U1 = XA(1) + XB(1); % It is real
    t1a = (_Float16)xBR + (_Float16)xAR  ;

    // U2 = XB(1) - XA(1); % It is imaginary
    t1b = (_Float16)xBI + (_Float16)xAI  ;

    // real(tw * (xB - xA)) = twR * (xBR - xAR) - twI * (xBI - xAI);
    // imag(tw * (xB - xA)) = twI * (xBR - xAR) + twR * (xBI - xAI);
    *pOut++ = 0.5f16 * ( (_Float16)t1a + (_Float16)t1b );
    *pOut++ = 0.5f16 * ( (_Float16)t1a - (_Float16)t1b );

    // XA(1) = 1/2*( U1 - imag(U2) +  i*( U1 +imag(U2) ));
    pB = p;
    pA += 2;

    blockCnt = fftSize * CMPLX_DIM;


#ifndef USE_ASM
    while (blockCnt > 0) {
        /*
           function X = my_split_rfft(X, ifftFlag)
           % X is a series of real numbers
           L  = length(X);
           XC = X(1:2:end) +i*X(2:2:end);
           XA = fft(XC);
           XB = conj(XA([1 end:-1:2]));
           TW = i*exp(-2*pi*i*[0:L/2-1]/L).';
           for l = 2:L/2
           XA(l) = 1/2 * (XA(l) + XB(l) + TW(l) * (XB(l) - XA(l)));
           end
           XA(1) = 1/2* (XA(1) + XB(1) + TW(1) * (XB(1) - XA(1))) + i*( 1/2*( XA(1) + XB(1) + i*( XA(1) - XB(1))));
           X = XA;
         */

        mve_pred16_t    p = vctp16q(blockCnt);

        xA = vld1q_z(pA, p);        pA += 8;

        xB = (float16x8_t)vldrwq_gather_shifted_offset_z_f32((float32_t *)pB, vecStridesBkwd, p);
        /* conjugate */
        xB = vmulq_x(xB, conj, p);
        vecStridesBkwd = vsubq_x_n_u32(vecStridesBkwd, 4, p);

        tw = vld1q_z(pCoeff, p);    pCoeff += 8;

        tmp1 = vaddq_x(xA, xB, p);
        tmp2 = vsubq_x(xB, xA, p);

        /* multiply with twiddle */
        res = MVE_CMPLX_MULT_FLT_AxB_Pred(tw, tmp2, p);

        res = vaddq_x(res, tmp1, p);
        res = vmulq_x_n_f16(res, 0.5f16, p);

        vst1q_p(pOut, res, p);      pOut += 8;

        blockCnt -= 8;
    }
#else

    /* use memory as "t" / "w" constraints do not allow to convey
     MVE vectors to inline asm block with GCC */
    uint16x8_t scratch[2];

    scratch[0] = vreinterpretq_u16_u32(vecStridesBkwd);
    scratch[1] = vreinterpretq_u16_f16(conj);

    __asm volatile(
        ".p2align 2                                             \n"

        " vecStrd       .req  q0                                \n"
        " conj          .req  q1                                \n"

        "   vldrw.32            vecStrd, [%[scratch], #(0*16)]  \n"
        "   vldrh.16            conj, [%[scratch], #(1*16)]     \n"

        /* preload Xa/Xb */
        "  vldrh.16             q2, [%[pA]], #16                \n"
        "  vldrw.32             q3, [%[pB], vecStrd, uxtw #2]   \n"

        "   wlstp.16            lr, %[count], 1f                \n"

        /* low overhead loop start */
        "2:                                                     \n"
        "  vmul.f16             q3, q3, conj                    \n"
        "  vsub.f16             q5, q3, q2                      \n"
        /* load twiddle */
        "  vldrh.16             q6, [%[pCoeff]], #16            \n"
        "  vcmul.f16            q4, q6, q5, #0                  \n"
        /* decrement gather load index */
        "  vsub.i32             vecStrd, vecStrd, %[four]       \n"
        "  vcmla.f16            q4, q6, q5, #90               \n"
        "  vadd.f16             q2, q2, q3                      \n"
        "  vldrw.32             q3, [%[pB], vecStrd, uxtw #2]   \n"
        "  vadd.f16             q4, q4, q2                      \n"
        "  vldrh.16             q2, [%[pA]], #16                \n"
        "  vmul.f16             q4, q4, %[half]                 \n"
        "  vstrh.16             q4, [%[pOut]], #16              \n"
        "  letp                 lr, 2b                          \n"

        /* low overhead loop end */
        "1:                                                     \n"
         : [pOut] "+r" (pOut), [pA] "+&r" (pA), [pB] "+r" (pB),
           [pCoeff] "+&r" (pCoeff)
         : [count] "r" (blockCnt), [four] "r" (4), [half] "r" (0.5f16),
           [scratch] "r" (scratch)
         : "q0", "q1", "q2", "q3",
           "q4", "q5", "q6",
           "r14", "memory"
    );
#endif
}




/* Prepares data for inverse cfft */
void merge_rfft_f16_mve(
  const arm_rfft_fast_instance_f16 * S,
        float16_t * p,
        float16_t * pOut)
{
    const float16_t *pCoeff = S->pTwiddleRFFT;  /* Points to RFFT Twiddle factors */
    float16_t      *pA = p;     /* increasing pointer */
    float16_t      *pB = p;     /* decreasing pointer */
    float16_t       xAR, xAI;   /* temporary variables */
#ifndef USE_ASM
    float16x8_t     tw, xA, xB;
    float16x8_t     tmp1, tmp2, res;
#endif
    uint32x4_t      vecStridesBkwd;
    float16x8_t     conj = vcvtq_f16_s16(vsubq_n_s16(vdwdupq_u16(2, 4, 2), 1));
    int32_t         blockCnt;
    uint32_t        fftSize;

    fftSize = (S->Sint).fftLen - 1;

    vecStridesBkwd[0] = fftSize;
    vecStridesBkwd[1] = fftSize - 1;
    vecStridesBkwd[2] = fftSize - 2;
    vecStridesBkwd[3] = fftSize - 3;

    xAR = pA[0];
    xAI = pA[1];

    pCoeff += 2;

    *pOut++ = 0.5f * (xAR + xAI);
    *pOut++ = 0.5f * (xAR - xAI);

    pB = p;
    pA += 2;

    blockCnt = fftSize * CMPLX_DIM;


#ifndef USE_ASM
    while (blockCnt > 0) {

        mve_pred16_t    p = vctp16q(blockCnt);

        xA = vld1q_z(pA, p);            pA += 8;

        xB = (float16x8_t)vldrwq_gather_shifted_offset_z_f32((float32_t *)pB, vecStridesBkwd, p);
        /* conjugate */
        xB = vmulq_x(xB, conj, p);
        vecStridesBkwd = vsubq_x_n_u32(vecStridesBkwd, 4, p);

        tw = vld1q_z(pCoeff, p);        pCoeff += 8;

        tmp1 = vaddq_x(xA, xB, p);
        tmp2 = vsubq_x(xB, xA, p);

        /* multiply with twiddle conjugate */
        res = MVE_CMPLX_MULT_FLT_Conj_AxB_Pred(tw, tmp2, p);

        res = vaddq_x(res, tmp1, p);
        res = vmulq_x_n_f16(res, 0.5f16, p);

        vst1q_p(pOut, res, p);          pOut += 8;

        blockCnt -= 8;
    }
#else
    /* use memory as "t" / "w" constraints do not allow to convey
     MVE vectors to inline asm block with GCC */
    uint16x8_t scratch[2];

    scratch[0] = vreinterpretq_u16_u32(vecStridesBkwd);
    scratch[1] = vreinterpretq_u16_f16(conj);

    __asm volatile(
        ".p2align 2                                             \n"

        " vecStrd       .req  q0                                \n"
        " conj          .req  q1                                \n"

        "   vldrw.32            vecStrd, [%[scratch], #(0*16)]  \n"
        "   vldrh.16            conj, [%[scratch], #(1*16)]     \n"

        /* preload Xa/Xb */
        "  vldrh.16             q2, [%[pA]], #16                \n"
        "  vldrw.32             q3, [%[pB], vecStrd, uxtw #2]   \n"

        "   wlstp.16            lr, %[count], 1f                \n"

        /* low overhead loop start */
        "2:                                                     \n"
        "  vmul.f16             q3, q3, conj                    \n"
        "  vsub.f16             q5, q3, q2                      \n"
        /* load twiddle */
        "  vldrh.16             q6, [%[pCoeff]], #16            \n"
        "  vcmul.f16            q4, q6, q5, #0                  \n"
        /* decrement gather load index */
        "  vsub.i32             vecStrd, vecStrd, %[four]       \n"
        "  vcmla.f16            q4, q6, q5, #270                \n"
        "  vadd.f16             q2, q2, q3                      \n"
        "  vldrw.32             q3, [%[pB], vecStrd, uxtw #2]   \n"
        "  vadd.f16             q4, q4, q2                      \n"
        "  vldrh.16             q2, [%[pA]], #16                \n"
        "  vmul.f16             q4, q4, %[half]                 \n"
        "  vstrh.16             q4, [%[pOut]], #16              \n"
        "  letp                 lr, 2b                          \n"

        /* low overhead loop end */
        "1:                                                     \n"
         : [pOut] "+r" (pOut), [pA] "+&r" (pA), [pB] "+r" (pB),
           [pCoeff] "+&r" (pCoeff)
         : [count] "r" (blockCnt), [four] "r" (4), [half] "r" (0.5f16),
           [scratch] "r" (scratch)
         : "q0", "q1", "q2", "q3",
           "q4", "q5", "q6",
           "r14", "memory"
    );

#endif

}

/**
  @ingroup groupTransforms
*/

/**
  @defgroup RealFFT Real FFT Functions

  @par
                   The CMSIS DSP library includes specialized algorithms for computing the
                   FFT of real data sequences.  The FFT is defined over complex data but
                   in many applications the input is real.  Real FFT algorithms take advantage
                   of the symmetry properties of the FFT and have a speed advantage over complex
                   algorithms of the same length.
  @par
                   The Fast RFFT algorithm relays on the mixed radix CFFT that save processor usage.
  @par
                   The real length N forward FFT of a sequence is computed using the steps shown below.
  @par
                   \image html RFFT.gif "Real Fast Fourier Transform"
  @par
                   The real sequence is initially treated as if it were complex to perform a CFFT.
                   Later, a processing stage reshapes the data to obtain half of the frequency spectrum
                   in complex format. Except the first complex number that contains the two real numbers
                   X[0] and X[N/2] all the data is complex. In other words, the first complex sample
                   contains two real values packed.
  @par
                   The input for the inverse RFFT should keep the same format as the output of the
                   forward RFFT. A first processing stage pre-process the data to later perform an
                   inverse CFFT.
  @par
                   \image html RIFFT.gif "Real Inverse Fast Fourier Transform"
  @par
                   The algorithms for floating-point, Q15, and Q31 data are slightly different
                   and we describe each algorithm in turn.
  @par           Floating-point
                   The main functions are \ref arm_rfft_fast_f16() and \ref arm_rfft_fast_init_f16().

  @par
                   The FFT of a real N-point sequence has even symmetry in the frequency domain.
                   The second half of the data equals the conjugate of the first half flipped in frequency.
                   Looking at the data, we see that we can uniquely represent the FFT using only N/2 complex numbers.
                   These are packed into the output array in alternating real and imaginary components:
  @par
                   X = { real[0], imag[0], real[1], imag[1], real[2], imag[2] ...
                   real[(N/2)-1], imag[(N/2)-1 }
  @par
                   It happens that the first complex number (real[0], imag[0]) is actually
                   all real. real[0] represents the DC offset, and imag[0] should be 0.
                   (real[1], imag[1]) is the fundamental frequency, (real[2], imag[2]) is
                   the first harmonic and so on.
  @par
                   The real FFT functions pack the frequency domain data in this fashion.
                   The forward transform outputs the data in this form and the inverse
                   transform expects input data in this form. The function always performs
                   the needed bitreversal so that the input and output data is always in
                   normal order. The functions support lengths of [32, 64, 128, ..., 4096]
                   samples.
  @par           Q15 and Q31
                   The real algorithms are defined in a similar manner and utilize N/2 complex
                   transforms behind the scenes.
  @par
                   The complex transforms used internally include scaling to prevent fixed-point
                   overflows.  The overall scaling equals 1/(fftLen/2).
                   Due to the use of complex transform internally, the source buffer is
                   modified by the rfft.
  @par
                   A separate instance structure must be defined for each transform used but
                   twiddle factor and bit reversal tables can be reused.
  @par
                   There is also an associated initialization function for each data type.
                   The initialization function performs the following operations:
                    - Sets the values of the internal structure fields.
                    - Initializes twiddle factor table and bit reversal table pointers.
                    - Initializes the internal complex FFT data structure.
  @par
                   Use of the initialization function is optional **except for MVE versions where it is mandatory**.
                   If you don't use the initialization functions, then the structures should be initialized with code
                   similar to the one below:
  <pre>
      arm_rfft_instance_q31 S = {fftLenReal, fftLenBy2, ifftFlagR, bitReverseFlagR, twidCoefRModifier, pTwiddleAReal, pTwiddleBReal, pCfft};
      arm_rfft_instance_q15 S = {fftLenReal, fftLenBy2, ifftFlagR, bitReverseFlagR, twidCoefRModifier, pTwiddleAReal, pTwiddleBReal, pCfft};
  </pre>
                   where <code>fftLenReal</code> is the length of the real transform;
                   <code>fftLenBy2</code> length of  the internal complex transform (fftLenReal/2).
                   <code>ifftFlagR</code> Selects forward (=0) or inverse (=1) transform.
                   <code>bitReverseFlagR</code> Selects bit reversed output (=0) or normal order
                   output (=1).
                   <code>twidCoefRModifier</code> stride modifier for the twiddle factor table.
                   The value is based on the FFT length;
                   <code>pTwiddleAReal</code>points to the A array of twiddle coefficients;
                   <code>pTwiddleBReal</code>points to the B array of twiddle coefficients;
                   <code>pCfft</code> points to the CFFT Instance structure. The CFFT structure
                   must also be initialized.
@par
                   Note that with MVE versions you can't initialize instance structures directly and **must
                   use the initialization function**.
 */

/**
  @addtogroup RealFFT
  @{
*/

/**
  @brief         Processing function for the floating-point real FFT.
  @param[in]     S         points to an arm_rfft_fast_instance_f16 structure
  @param[in]     p         points to input buffer (Source buffer is modified by this function.)
  @param[in]     pOut      points to output buffer
  @param[in]     ifftFlag
                   - value = 0: RFFT
                   - value = 1: RIFFT
  @return        none
*/

void arm_rfft_fast_f16_mve(
  const arm_rfft_fast_instance_f16 * S,
  float16_t * p,
  float16_t * pOut,
  uint8_t ifftFlag)
{
   const arm_cfft_instance_f16 * Sint = &(S->Sint);


   /* Calculation of Real FFT */
   if (ifftFlag)
   {
      /*  Real FFT compression */
      merge_rfft_f16_mve(S, p, pOut);
      /* Complex radix-4 IFFT process */
      arm_cfft_f16_mve( Sint, pOut, ifftFlag, 1);
   }
   else
   {

      /* Calculation of RFFT of input */
      arm_cfft_f16_mve( Sint, p, ifftFlag, 1);

      /*  Real FFT extraction */
      stage_rfft_f16_mve(S, p, pOut);
   }
}

/**
* @} end of RealFFT group
*/

#endif /*  #if defined(ARM_FLOAT16_SUPPORTED) */

#endif
