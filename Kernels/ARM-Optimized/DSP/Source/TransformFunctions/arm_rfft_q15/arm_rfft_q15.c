/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_rfft_q15.c
 * Description:  RFFT & RIFFT Q15 process function
 *               using Helium assembly kernels
 *               This version allows boosting RFFT Q15 performance when using compilers having suboptimal
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

#include "dsp/transform_functions.h"

#if defined(ARM_MATH_MVEI)

/* ----------------------------------------------------------------------
 * Internal functions prototypes
 * -------------------------------------------------------------------- */

static void arm_split_rfft_q15_mve(
        q15_t * pSrc,
        uint32_t fftLen,
  const q15_t * pATable,
  const q15_t * pBTable,
        q15_t * pDst,
        uint32_t modifier);

static void arm_split_rifft_q15_mve(
        q15_t * pSrc,
        uint32_t fftLen,
  const q15_t * pATable,
  const q15_t * pBTable,
        q15_t * pDst,
        uint32_t modifier);

void arm_cfft_q15_mve(
  const arm_cfft_instance_q15 * S,
        q15_t * pSrc,
        uint8_t ifftFlag,
        uint8_t bitReverseFlag);

/**
  @addtogroup RealFFT
  @{
 */

/**
  @brief         Processing function for the Q15 RFFT/RIFFT.
  @param[in]     S     points to an instance of the Q15 RFFT/RIFFT structure
  @param[in]     pSrc  points to input buffer (Source buffer is modified by this function.). Must be 32-bit aligned
  @param[out]    pDst  points to output buffer
  @return        none

  @par           Input an output formats
                   Internally input is downscaled by 2 for every stage to avoid saturations inside CFFT/CIFFT process.
                   Hence the output format is different for different RFFT sizes.
                   The input and output formats for different RFFT sizes and number of bits to upscale are mentioned in the tables below for RFFT and RIFFT:
  @par
                   \image html RFFTQ15.gif "Input and Output Formats for Q15 RFFT"
  @par
                   \image html RIFFTQ15.gif "Input and Output Formats for Q15 RIFFT"
  @par
                   If the input buffer is of length N, the output buffer must have length 2*N.
                   The input buffer is modified by this function.
  @par
                   For the RIFFT, the source buffer must at least have length
                   fftLenReal + 2.
                   The last two elements must be equal to what would be generated
                   by the RFFT:
                     (pSrc[0] - pSrc[1]) >> 1 and 0
 */

void arm_rfft_q15_mve(
  const arm_rfft_instance_q15 * S,
        q15_t * pSrc,
        q15_t * pDst)
{
#if defined(ARM_MATH_MVEI) && !defined(ARM_MATH_AUTOVECTORIZE)
  const arm_cfft_instance_q15 *S_CFFT = &(S->cfftInst);
#else
  const arm_cfft_instance_q15 *S_CFFT = S->pCfft;
#endif
        uint32_t L2 = S->fftLenReal >> 1U;

  /* Calculation of RIFFT of input */
  if (S->ifftFlagR == 1U)
  {
     /*  Real IFFT core process */
     arm_split_rifft_q15_mve (pSrc, L2, S->pTwiddleAReal, S->pTwiddleBReal, pDst, S->twidCoefRModifier);

     /* Complex IFFT process */
     arm_cfft_q15_mve (S_CFFT, pDst, S->ifftFlagR, S->bitReverseFlagR);

     arm_shift_q15(pDst, 1, pDst, S->fftLenReal);
  }
  else
  {
     /* Calculation of RFFT of input */

     /* Complex FFT process */
     arm_cfft_q15_mve (S_CFFT, pSrc, S->ifftFlagR, S->bitReverseFlagR);

     /*  Real FFT core process */
     arm_split_rfft_q15_mve (pSrc, L2, S->pTwiddleAReal, S->pTwiddleBReal, pDst, S->twidCoefRModifier);
  }

}

/**
  @} end of RealFFT group
 */

/**
  @brief         Core Real FFT process
  @param[in]     pSrc      points to input buffer
  @param[in]     fftLen    length of FFT
  @param[in]     pATable   points to twiddle Coef A buffer
  @param[in]     pBTable   points to twiddle Coef B buffer
  @param[out]    pDst      points to output buffer
  @param[in]     modifier  twiddle coefficient modifier that supports different size FFTs with the same twiddle factor table
  @return        none

  @par
                   The function implements a Real FFT
 */



#include "arm_helium_utils.h"
#include "arm_vec_fft.h"

#if defined(__CMSIS_GCC_H)
#define MVE_CMPLX_MULT_FX_AxB_S16(A,B)          vqdmladhxq_s16(vqdmlsdhq_s16((__typeof(A))vuninitializedq_s16(), A, B), A, B)
#define MVE_CMPLX_MULT_FX_AxConjB_S16(A,B)      vqdmladhq_s16(vqdmlsdhxq_s16((__typeof(A))vuninitializedq_s16(), A, B), A, B)

#endif


void arm_split_rfft_q15_mve(
        q15_t * pSrc,
        uint32_t fftLen,
  const q15_t * pATable,
  const q15_t * pBTable,
        q15_t * pDst,
        uint32_t modifier)
{
   uint32_t        i;                   /* Loop Counter */
    const q15_t    *pCoefA, *pCoefB;    /* Temporary pointers for twiddle factors */
    q15_t          *pOut1 = &pDst[2];
    q15_t          *pIn1 = &pSrc[2];
    uint32x4_t      offsetIdx1 = vddupq_n_u32(3, 1);
    uint32x4_t      offsetIdx2 = vidupq_n_u32(0, 1);


    offsetIdx2 = offsetIdx2 * modifier;
    offsetIdx1 = vaddq_n_u32(offsetIdx1, (fftLen - 4));

    /* Init coefficient pointers */
    pCoefA = &pATable[modifier * 2];
    pCoefB = &pBTable[modifier * 2];

    const q15_t    *pCoefAb, *pCoefBb;
    pCoefAb = pCoefA;
    pCoefBb = pCoefB;

    pIn1 = &pSrc[2];

#ifdef USE_ASM
    /* use memory as "t" / "w" constraints do not allow to convey
     MVE vectors to inline asm block with GCC */
    uint32x4_t scratch[2];

    offsetIdx1 += 4;
    i = 2*(fftLen - 1U);

    scratch[0] = offsetIdx1;
    scratch[1] = offsetIdx2;

    __asm volatile(
        ".p2align 2                                                     \n"

        " offsetIdx1    .req  q0                                        \n"
        " offsetIdx2    .req  q1                                        \n"

        "   vldrw.32            offsetIdx1, [%[scratch]]                \n"
        "   vldrw.32            offsetIdx2, [%[scratch], #16]           \n"

        "   wlstp.16            lr, %[count], 1f                        \n"
        "   vldrh.u16           q3, [%[pIn1]], #16                      \n"

        /* low overhead loop start */
        "2:                                                             \n"
        "   vsub.i32            offsetIdx1, offsetIdx1, %[four]         \n"
        "   vldrw.u32           q4, [%[pCoefAb], offsetIdx2, uxtw #2]   \n"
        "   vqdmlsdh.s16        q5, q3, q4                              \n"
        "   vldrw.u32           q6, [%[pSrc], offsetIdx1, uxtw #2]      \n"
        "   vqdmladhx.s16       q5, q3, q4                              \n"
        "   vldrw.u32           q3, [%[pCoefBb], offsetIdx2, uxtw #2]   \n"
        "   vqdmlsdhx.s16       q4, q3, q6                              \n"
        "   vadd.i32            offsetIdx2, offsetIdx2, %[mod4]         \n"
        "   vqdmladh.s16        q4, q3, q6                              \n"
        "   vldrh.u16           q3, [%[pIn1]], #16                      \n"
        "   vhadd.s16           q6, q5, q4                              \n"
        "   vstrh.16            q6, [%[pOut1]], #16                     \n"
        "   letp                lr, 2b                                  \n"

        /* low overhead loop end */
        "1:                                                             \n"
     : [pIn1] "+&r" (pIn1), [pOut1] "+r" (pOut1)
     : [count] "r" (i), [pSrc] "r" (pSrc), [pCoefAb] "r" (pCoefAb),
       [pCoefBb] "r" (pCoefBb), [mod4] "r" (modifier * 4),
       [four] "r" (4), [scratch] "r" (scratch)
     : "q0", "q1", "q3", "q4", "q5", "q6",
       "r14","memory"
    );

#else

    i = fftLen - 1U;
    i = i / 4 + 1;
    while (i > 0U) {
        q15x8_t         in1 = vld1q_s16(pIn1);
        q15x8_t         in2 = (q15x8_t)vldrwq_gather_shifted_offset(
                                                        (const q31_t *)pSrc, offsetIdx1);
        q15x8_t         coefA = (q15x8_t)vldrwq_gather_shifted_offset(
                                                        (const q31_t *)pCoefAb, offsetIdx2);
        q15x8_t         coefB = (q15x8_t)vldrwq_gather_shifted_offset(
                                                        (const q31_t *)pCoefBb, offsetIdx2);

#if defined(__CMSIS_GCC_H)
        q15x8_t         out = vhaddq_s16(MVE_CMPLX_MULT_FX_AxB_S16(in1, coefA),
                                     MVE_CMPLX_MULT_FX_AxConjB_S16(coefB, in2));
#else
        q15x8_t         out = vhaddq_s16(MVE_CMPLX_MULT_FX_AxB(in1, coefA, q15x8_t),
                                         MVE_CMPLX_MULT_FX_AxConjB(coefB, in2, q15x8_t));
#endif
        vst1q_s16(pOut1, out);
        pOut1 += 8;

        offsetIdx2 = vaddq_n_u32(offsetIdx2, modifier * 4);
        offsetIdx1 -= 4;
        pIn1 += 8;
        i -= 1;
    }
#endif

    pDst[2 * fftLen] = (pSrc[0] - pSrc[1]) >> 1U;
    pDst[2 * fftLen + 1] = 0;

    pDst[0] = (pSrc[0] + pSrc[1]) >> 1U;
    pDst[1] = 0;
}

/**
  @brief         Core Real IFFT process
  @param[in]     pSrc      points to input buffer
  @param[in]     fftLen    length of FFT
  @param[in]     pATable   points to twiddle Coef A buffer
  @param[in]     pBTable   points to twiddle Coef B buffer
  @param[out]    pDst      points to output buffer
  @param[in]     modifier  twiddle coefficient modifier that supports different size FFTs with the same twiddle factor table
  @return        none

  @par
                   The function implements a Real IFFT
 */


void arm_split_rifft_q15_mve(
        q15_t * pSrc,
        uint32_t fftLen,
  const q15_t * pATable,
  const q15_t * pBTable,
        q15_t * pDst,
        uint32_t modifier)
{
    const q15_t    *pCoefA, *pCoefB;    /* Temporary pointers for twiddle factors */
    q15_t          *pIn1;
    uint32x4_t      offsetIdx1 = vddupq_n_u32(3, 1);
    uint32x4_t      offsetIdx2 = vidupq_n_u32(0, 1);
    int16x8_t       conj = { 1, -1, 1, -1, 1, -1, 1, -1 }; /* conjugate */

    offsetIdx2 = offsetIdx2 * modifier;
    offsetIdx1 = vaddq_n_u32(offsetIdx1, (fftLen - 3));

    /* Init coefficient pointers */
    pCoefA = &pATable[0];
    pCoefB = &pBTable[0];


    pIn1 = &pSrc[0];

#ifdef USE_ASM
    /* use memory as "t" / "w" constraints do not allow to convey
     MVE vectors to inline asm block with GCC */
    uint32x4_t scratch[3];

    offsetIdx1 += 4;

    scratch[0] = offsetIdx1;
    scratch[1] = offsetIdx2;
    scratch[2] = (uint32x4_t)conj;

    __asm volatile(
        ".p2align 2                                                     \n"

        " offsetIdx1    .req  q0                                        \n"
        " offsetIdx2    .req  q1                                        \n"
        " conj          .req  q2                                        \n"

        "   vldrw.32            offsetIdx1, [%[scratch], #(0*16)]       \n"
        "   vldrw.32            offsetIdx2, [%[scratch], #(1*16)]       \n"
        "   vldrh.16            conj, [%[scratch], #(2*16)]             \n"

        "   wlstp.16            lr, %[count], 1f                        \n"
        "   vldrh.u16           q3, [%[pIn1]], #16                      \n"

        /* low overhead loop start */
        "2:                                                             \n"
        "   vsub.i32            offsetIdx1, offsetIdx1, %[four]         \n"
        "   vldrw.u32           q4, [%[pCoefA], offsetIdx2, uxtw #2]    \n"
        "   vqdmlsdhx.s16       q5, q3, q4                              \n"
        "   vldrw.u32           q6, [%[pSrc], offsetIdx1, uxtw #2]      \n"
        "   vqdmladh.s16        q5, q3, q4                              \n"
        "   vldrw.u32           q3, [%[pCoefB], offsetIdx2, uxtw #2]    \n"
        "   vqdmlsdh.s16        q4, q3, q6                              \n"
        "   vadd.i32            offsetIdx2, offsetIdx2, %[mod4]         \n"
        "   vqdmladhx.s16       q4, q3, q6                              \n"
        "   vldrh.u16           q3, [%[pIn1]], #16                      \n"
        "   vmul.s16            q4, q4, conj                            \n"
        "   vhadd.s16           q6, q5, q4                              \n"
        "   vstrh.16            q6, [%[pDst]], #16                      \n"
        "   letp                lr, 2b                                  \n"

        /* low overhead loop end */
        "1:                                                             \n"
     : [pIn1] "+&r" (pIn1), [pDst] "+r" (pDst)
     : [count] "r" (2*fftLen), [pSrc] "r" (pSrc), [pCoefA] "r" (pCoefA),
       [pCoefB] "r" (pCoefB), [mod4] "r" (modifier * 4),
       [four] "r" (4), [scratch] "r" (scratch)
     : "q0", "q1", "q2", "q3", "q4", "q5", "q6",
       "r14","memory"
    );

#else
    uint32_t        i;                  /* Loop Counter */

    i = fftLen;
    i = i / 4;

    while (i > 0U) {
        q15x8_t         in1 = vld1q_s16(pIn1);
        q15x8_t         in2 = (q15x8_t)vldrwq_gather_shifted_offset(
                                                        (const q31_t *)pSrc, offsetIdx1);
        q15x8_t         coefA = (q15x8_t)vldrwq_gather_shifted_offset(
                                                        (const q31_t *)pCoefA, offsetIdx2);
        q15x8_t         coefB = (q15x8_t)vldrwq_gather_shifted_offset(
                                                        (const q31_t *)pCoefB, offsetIdx2);

        q15x8_t         out = vhaddq_s16(MVE_CMPLX_MULT_FX_AxConjB(in1, coefA, q15x8_t),
                                         vmulq(conj, MVE_CMPLX_MULT_FX_AxB(in2, coefB, q15x8_t)));

        vst1q_s16(pDst, out);
        pDst += 8;

        offsetIdx2 = vaddq_n_u32(offsetIdx2, modifier * 4);
        offsetIdx1 -= 4;

        pIn1 += 8;
        i -= 1;
    }
#endif
}
#endif
