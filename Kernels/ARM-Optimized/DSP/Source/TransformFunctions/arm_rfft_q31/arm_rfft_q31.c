/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_rfft_q31.c
 * Description:  FFT & RIFFT Q31 process function
 *               using Helium assembly kernels
 *               This version allows boosting RFFT Q31 performance when using compilers having suboptimal
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

/* ----------------------------------------------------------------------
 * Internal functions prototypes
 * -------------------------------------------------------------------- */

#if defined(ARM_MATH_MVEI)

static void arm_split_rfft_q31_mve(
        q31_t * pSrc,
        uint32_t fftLen,
  const q31_t * pATable,
  const q31_t * pBTable,
        q31_t * pDst,
        uint32_t modifier);

static void arm_split_rifft_q31_mve(
        q31_t * pSrc,
        uint32_t fftLen,
  const q31_t * pATable,
  const q31_t * pBTable,
        q31_t * pDst,
        uint32_t modifier);

void            arm_cfft_q31_mve(const arm_cfft_instance_q31 * S,
                                 q31_t * pSrc, uint8_t ifftFlag, uint8_t bitReverseFlag);

/**
  @addtogroup RealFFT
  @{
 */

/**
  @brief         Processing function for the Q31 RFFT/RIFFT.
  @param[in]     S     points to an instance of the Q31 RFFT/RIFFT structure
  @param[in]     pSrc  points to input buffer (Source buffer is modified by this function)
  @param[out]    pDst  points to output buffer
  @return        none

  @par           Input an output formats
                   Internally input is downscaled by 2 for every stage to avoid saturations inside CFFT/CIFFT process.
                   Hence the output format is different for different RFFT sizes.
                   The input and output formats for different RFFT sizes and number of bits to upscale are mentioned in the tables below for RFFT and RIFFT:
  @par
                   \image html RFFTQ31.gif "Input and Output Formats for Q31 RFFT"
  @par
                   \image html RIFFTQ31.gif "Input and Output Formats for Q31 RIFFT"
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

void arm_rfft_q31_mve(
  const arm_rfft_instance_q31 * S,
        q31_t * pSrc,
        q31_t * pDst)
{
#if defined(ARM_MATH_MVEI) && !defined(ARM_MATH_AUTOVECTORIZE)
  const arm_cfft_instance_q31 *S_CFFT = &(S->cfftInst);
#else
  const arm_cfft_instance_q31 *S_CFFT = S->pCfft;
#endif
        uint32_t L2 = S->fftLenReal >> 1U;

  /* Calculation of RIFFT of input */
  if (S->ifftFlagR == 1U)
  {
     /*  Real IFFT core process */
     arm_split_rifft_q31_mve (pSrc, L2, S->pTwiddleAReal, S->pTwiddleBReal, pDst, S->twidCoefRModifier);

     /* Complex IFFT process */
     arm_cfft_q31_mve (S_CFFT, pDst, S->ifftFlagR, S->bitReverseFlagR);

     arm_shift_q31(pDst, 1, pDst, S->fftLenReal);
  }
  else
  {
     /* Calculation of RFFT of input */

     /* Complex FFT process */
     arm_cfft_q31_mve (S_CFFT, pSrc, S->ifftFlagR, S->bitReverseFlagR);

     /*  Real FFT core process */
     arm_split_rfft_q31_mve (pSrc, L2, S->pTwiddleAReal, S->pTwiddleBReal, pDst, S->twidCoefRModifier);
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
 */



#include "arm_helium_utils.h"
#include "arm_vec_fft.h"

#if defined(__CMSIS_GCC_H)

#define MVE_CMPLX_MULT_FX_AxB_S32(A,B)          vqdmladhxq_s32(vqdmlsdhq_s32((__typeof(A))vuninitializedq_s32(), A, B), A, B)
#define MVE_CMPLX_MULT_FX_AxConjB_S32(A,B)      vqdmladhq_s32(vqdmlsdhxq_s32((__typeof(A))vuninitializedq_s32(), A, B), A, B)

#endif



static void arm_split_rfft_q31_mve(
        q31_t * pSrc,
        uint32_t fftLen,
  const q31_t * pATable,
  const q31_t * pBTable,
        q31_t * pDst,
        uint32_t modifier)
{
    uint32_t        i;          /* Loop Counter */
    const q31_t    *pCoefA, *pCoefB;    /* Temporary pointers for twiddle factors */
    q31_t          *pOut1 = &pDst[2];
    q31_t          *pIn1;
    uint32x4_t      offsetIdx1 = { 2, 3, 0, 1 };
    uint32x4_t      offsetIdx2 = { 0, 1, modifier * 2, modifier * 2 + 1 };

    offsetIdx1 = offsetIdx1 + (2 * fftLen - 4);


    /* Init coefficient pointers */
    pCoefA = &pATable[modifier * 2];
    pCoefB = &pBTable[modifier * 2];

    const q31_t    *pCoefAb, *pCoefBb;
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

        "   wlstp.32            lr, %[count], 1f                        \n"
        "   vldrw.32            q2, [%[pIn1]], #16                      \n"

        /* low overhead loop start */
        "2:                                                             \n"
        "   vsub.i32            offsetIdx1, offsetIdx1, %[four]         \n"
        "   vldrw.32            q4, [%[pCoefAb], offsetIdx2, uxtw #2]   \n"
        "   vqdmlsdh.s32        q5, q2, q4                              \n"
        "   vldrw.32            q3, [%[pSrc], offsetIdx1, uxtw #2]      \n"
        "   vqdmladhx.s32       q5, q2, q4                              \n"
        "   vldrw.32            q2, [%[pCoefBb], offsetIdx2, uxtw #2]   \n"
        "   vqdmlsdhx.s32       q4, q2, q3                              \n"
        "   vadd.i32            offsetIdx2, offsetIdx2, %[modif4]       \n"
        "   vqdmladh.s32        q4, q2, q3                              \n"
        "   vldrw.32            q2, [%[pIn1]], #16                      \n"
        "   vhadd.s32           q3, q5, q4                              \n"
        "   vstrw.32            q3, [%[pOut1]], #16                     \n"
        "   letp                lr, 2b                                  \n"

        /* low overhead loop end */
        "1:                                                             \n"

        " .unreq offsetIdx1                                             \n"
        " .unreq offsetIdx2                                             \n"

     : [pIn1] "+&r" (pIn1), [pOut1] "+&r" (pOut1)
     : [count] "r" (i), [pSrc] "r" (pSrc), [pCoefAb] "r" (pCoefAb),
       [pCoefBb] "r" (pCoefBb), [modif4] "r" (modifier * 4),
       [four] "r" (4), [scratch] "r" (scratch)
      : "q0", "q1", "q2", "q3",
        "q4", "q5",
        "r14", "memory"
    );


#else

    i = fftLen - 1U;
    i = i / 2 + 1;
    while (i > 0U) {
        q31x4_t         in1 = vld1q_s32(pIn1);
        q31x4_t         in2 = vldrwq_gather_shifted_offset_s32(pSrc, offsetIdx1);
        q31x4_t         coefA = vldrwq_gather_shifted_offset_s32(pCoefAb, offsetIdx2);
        q31x4_t         coefB = vldrwq_gather_shifted_offset_s32(pCoefBb, offsetIdx2);
#if defined(__CMSIS_GCC_H)
        q31x4_t         out = vhaddq_s32(MVE_CMPLX_MULT_FX_AxB_S32(in1, coefA),MVE_CMPLX_MULT_FX_AxConjB_S32(coefB, in2));
#else
        q31x4_t         out = vhaddq_s32(MVE_CMPLX_MULT_FX_AxB(in1, coefA, q31x4_t),
                                         MVE_CMPLX_MULT_FX_AxConjB(coefB, in2, q31x4_t));
#endif
        vst1q(pOut1, out);
        pOut1 += 4;

        offsetIdx2 += modifier * 4;
        offsetIdx1 -= 4;

        pIn1 += 4;
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
 */



static void arm_split_rifft_q31_mve(
        q31_t * pSrc,
        uint32_t fftLen,
  const q31_t * pATable,
  const q31_t * pBTable,
        q31_t * pDst,
        uint32_t modifier)
{
    const q31_t    *pCoefA, *pCoefB;    /* Temporary pointers for twiddle factors */
    q31_t          *pIn1;
    uint32x4_t      offsetIdx1 = { 2, 3, 0, 1 };
    uint32x4_t      offsetIdx2 = { 0, 1, modifier * 2, modifier * 2 + 1 };
    int32x4_t       conj = { 1, -1, 1, -1 };

    offsetIdx1 = offsetIdx1 + (2 * fftLen - 2);

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
    scratch[2] = conj;

    __asm volatile(
        ".p2align 2                                                     \n"

        " offsetIdx1    .req  q0                                        \n"
        " offsetIdx2    .req  q1                                        \n"
        " conj          .req  q2                                        \n"

        "   vldrw.32            offsetIdx1, [%[scratch], #(0*16)]       \n"
        "   vldrw.32            offsetIdx2, [%[scratch], #(1*16)]       \n"
        "   vldrw.32            conj, [%[scratch], #(2*16)]             \n"

        "   wlstp.32            lr, %[count], 1f                        \n"
        "   vldrw.32            q6, [%[pIn1]], #16                      \n"

        /* low overhead loop start */
        "2:                                                             \n"
        "   vsub.i32            offsetIdx1, offsetIdx1, %[four]         \n"
        "   vldrw.32            q3, [%[pCoefA], offsetIdx2, uxtw #2]    \n"
        "   vqdmlsdhx.s32       q5, q6, q3                              \n"
        "   vldrw.32            q4, [%[pSrc], offsetIdx1, uxtw #2]      \n"
        "   vqdmladh.s32        q5, q6, q3                              \n"
        "   vldrw.32            q6, [%[pCoefB], offsetIdx2, uxtw #2]    \n"
        "   vqdmlsdh.s32        q3, q4, q6                              \n"
        "   vadd.i32            offsetIdx2, offsetIdx2, %[modif4]       \n"
        "   vqdmladhx.s32       q3, q4, q6                              \n"
        "   vldrw.32            q6, [%[pIn1]], #16                      \n"
        "   vmul.s32            q3, q3, conj                            \n"
        "   vhadd.s32           q4, q5, q3                              \n"
        "   vstrw.32            q4, [%[pDst]], #16                      \n"
        "  letp                 lr, 2b                                  \n"

        /* low overhead loop end */
        "1:                                                             \n"

        " .unreq offsetIdx1                                             \n"
        " .unreq offsetIdx2                                             \n"
        " .unreq conj                                                   \n"

     : [pIn1] "+&r" (pIn1), [pDst] "+&r" (pDst)
     : [count] "r" (fftLen*2), [pSrc] "r" (pSrc),
       [pCoefA] "r" (pCoefA), [pCoefB] "r" (pCoefB),
       [modif4] "r" (modifier * 4), [four] "r" (4),
       [scratch] "r" (scratch)
     : "q0", "q1", "q2", "q3",
       "q4", "q5", "q6",
       "r14", "memory"
    );

#else
    uint32_t        i;          /* Loop Counter */
    const q31_t    *pCoefAb, *pCoefBb;

    pCoefAb = pCoefA;
    pCoefBb = pCoefB;

    i = fftLen;
    i = i >> 1;
    while (i > 0U) {
        q31x4_t         in1 = vld1q_s32(pIn1);
        q31x4_t         in2 = vldrwq_gather_shifted_offset_s32(pSrc, offsetIdx1);
        q31x4_t         coefA = vldrwq_gather_shifted_offset_s32(pCoefAb, offsetIdx2);
        q31x4_t         coefB = vldrwq_gather_shifted_offset_s32(pCoefBb, offsetIdx2);

#if defined(__CMSIS_GCC_H)
        q31x4_t         out = vhaddq_s32(MVE_CMPLX_MULT_FX_AxConjB_S32(in1, coefA),
                                     vmulq_s32(conj, MVE_CMPLX_MULT_FX_AxB_S32(in2, coefB)));
#else
        q31x4_t         out = vhaddq_s32(MVE_CMPLX_MULT_FX_AxConjB(in1, coefA, q31x4_t),
                                         vmulq_s32(conj, MVE_CMPLX_MULT_FX_AxB(in2, coefB, q31x4_t)));
#endif
        vst1q_s32(pDst, out);
        pDst += 4;

        offsetIdx2 += modifier * 4;
        offsetIdx1 -= 4;

        pIn1 += 4;
        i -= 1;
    }
#endif
}

#endif
