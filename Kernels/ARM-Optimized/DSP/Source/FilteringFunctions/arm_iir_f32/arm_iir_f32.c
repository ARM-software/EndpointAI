/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_iir_f32.c
 * Description:  MVE IIR (experimental)
 *
 * $Date:        Aug 2022
 * $Revision:    V.1.0.0
 *
 * Target Processor: Cortex-M cores with MVE support
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


#include "arm_iir_f32.h"



/**
 * @brief  Nth order floating-point IIR reference.
 * @param[in,out] S             points to an instance of the floating-point IIR4 structure.
 * @param[in]     pSrc          Filter input pointer
 * @param[out]    pDst          Filter output pointer
 * @param[in]     blockSize     Points to the fwd state buffer.
 */

void arm_iir_f32(const arm_iir_instance_f32 * S, const float32_t * pSrc, float32_t * pDst,
                 uint32_t blockSize)
{
    const float32_t *pIn = pSrc;        /*  source pointer            */
    float32_t      *pOut = pDst;        /*  destination pointer       */
    float32_t      *pNumState = S->pNumState;   /*  pState pointer            */
    float32_t      *pDenState = S->pDenState;   /*  pState pointer            */
    const float32_t *pA = S->pDenCoef;  /*  coefficient pointer       */
    const float32_t *pB = S->pNumCoef;  /*  coefficient pointer       */
    float32_t       X0;
    float32_t       acc0;       /* accumulator vector */
    int32_t         sample;
    int32_t         order = S->order;


    sample = blockSize;

    while (sample > 0) {
        X0 = *pIn++;

        acc0 = X0 * pB[0];
        for (int i = 0; i < order; i++) {
            acc0 += pNumState[i] * pB[1 + i];
        }
        for (int i = 0; i < order; i++) {
            acc0 -= pDenState[i] * pA[i];
        }
        /*
         * Store the result in the accumulator in the destination buffer.
         */
        *pOut++ = acc0;

        /*
         * update recurrence
         */
        for (int i = order - 1; i >= 1; i--)
            pNumState[i] = pNumState[i - 1];
        pNumState[0] = X0;

        for (int i = order - 1; i >= 1; i--)
            pDenState[i] = pDenState[i - 1];
        pDenState[0] = acc0;

        /*
         * decrement the loop counter
         */
        sample -= 1;
    }
}


#if defined(ARM_MATH_MVEF)


#define IIR_4_ORDER     4
#define IIR_8_ORDER     8
#define IIR_12_ORDER    12



/**
 * @brief  4th order floating-point IIR MVE version.
 * @param[in,out] S             points to an instance of the floating-point IIR4 structure.
 * @param[in]     pSrc          Filter input pointer
 * @param[out]    pDst          Filter output pointer
 * @param[in]     blockSize     Points to the fwd state buffer.
 */



void arm_iir4_f32_mve(const arm_iir_instance_f32 * S, const float32_t * pSrc, float32_t * pDst,
                       uint32_t blockSize)
{
    uint32_t         order;         /* Number of coefficients */
    const float32_t *pNumCoef;      /* Numerator Coefficients */
    const float32_t *pDenCoef;      /* Denominator Coefficients */
    float32_t       *pInputHist;    /* Pointer to the input history */
    float32_t       *pOutputHist;   /* Pointer to the output history */
    float32_t       *pResult;
    float32_t       *pTmpDst;
    const float32_t *pS;
    float32_t       *pD;
    int32_t          blkCnt;

    /* Load configuration parameters and saved state. */
    order = IIR_4_ORDER;
    pNumCoef = S->pNumCoef;
    pDenCoef = S->pDenCoef;
    pInputHist = &(S->pNumState[order + 1]);
    pOutputHist = &(S->pDenState[order + 1]);
    pResult = pOutputHist;
    pTmpDst = pDst;

    /* Copy new samples into history buffer */
    pS = pSrc;
    pD = pInputHist;
    blkCnt = blockSize;
    do {
        mve_pred16_t    p = vctp32q(blkCnt);

        vst1q_p(pD, vld1q_z(pS, p), p);
        pS += 4;
        pD += 4;
        blkCnt -= 4;
    }
    while (blkCnt > 0);

    /* main loop */

    float32_t      *fwd = &(S->pNumState[1]);
    float32_t      *bck = &(S->pDenState[1]);

#ifdef USE_ASM

    __asm volatile  (
        /* Preload 4x2 coefficients */
        "   vldrw.32        q2, [%[coef_fwd]], #16              \n"
        "   vldrw.32        q5, [%[coef_bck]], #16              \n"


        ".p2align 2                                             \n"
        "   vldrw.u32       q0, [%[input_hist]], #4             \n"
        "   wls             lr, %[sample_cnt], 1f               \n"
        "2:                                                     \n"
        "   vmul.f32        q1, q0, q2                          \n"
        "   vldrw.u32       q0, [%[output_hist]], #4            \n"
        "   vfms.f32        q1, q0, q5                          \n"
        "   vldrw.u32       q0, [%[input_hist]], #4             \n"

        /* sum the partial accumulators */
        "   vadd.f32        s4, s4, s5                          \n"
        "   vadd.f32        s6, s6, s7                          \n"
        "   vldr            s5, [%[coef_fwd]]                   \n"
        "   vadd.f32        s4, s4, s6                          \n"

        /*  re-use fwd[4] in the last element of Q0 */
        "   vfma.f32        s4, s5, s3                          \n"

        /* populate destination and output history */
        "   vstm            %[pDst]!, {s4}                      \n"
        "   vstm            %[pResult]!, {s4}                   \n"
        "   le              lr, 2b                              \n"
        "1:                                                     \n"
        : [input_hist] "+r"(fwd),[output_hist] "+r"(bck),
          [sample_cnt] "+r"(blockSize),[pDst] "+r"(pTmpDst),
          [pResult] "+r"(pResult)
        :[coef_fwd] "r" (pNumCoef),[coef_bck] "r"(pDenCoef)
        :"q0", "q1", "q2", "q5",
         "r14", "memory");

#else                           /* MVE intrinsics */

    f32x4_t     acc0;
    f32x4_t     vecIn0;
    f32x4_t     vecCoefNum0, vecCoefDen0;

    /*
     * preload 4 x 2 num/den coefficients
     */
    vecCoefNum0 = *(f32x4_t *) pNumCoef;

    vecCoefDen0 = *(f32x4_t *) pDenCoef;

    blkCnt = blockSize;
    while (blkCnt > 0) {
        float32_t       sum;

        vecIn0 = vld1q(fwd);
        acc0 = vmulq(vecIn0, vecCoefNum0);

        vecIn0 = vld1q(bck);
        acc0 = vfmsq(acc0, vecIn0, vecCoefDen0);

        sum = acc0[0] + acc0[1] + acc0[2] + acc0[3];
        sum += fwd[IIR_4_ORDER] * pNumCoef[IIR_4_ORDER];
        bck = bck + 1;
        fwd = fwd + 1;

        *pTmpDst++ = sum;
        *pResult++ = sum;
        /*
         * Decrement the sample block loop counter
         */
        blkCnt--;
    }

#endif                          // USE_ASM

    /* save state */
    float32_t      *r = &(pOutputHist[blockSize - order]);
    const float32_t *s = &(pSrc[blockSize - order]);
    fwd = S->pNumState + 1;
    bck = S->pDenState + 1;

    blkCnt = order;
    while (blkCnt > 0U) {
        mve_pred16_t    p0 = vctp32q(blkCnt);

        vst1q_p(bck, vld1q(r), p0);
        vst1q_p(fwd, vld1q(s), p0);

        r += 4;
        bck += 4;
        s += 4;
        fwd += 4;
        blkCnt -= 4;
    }
}


/**
 * @brief  8th order floating-point IIR MVE version.
 * @param[in,out] S             points to an instance of the floating-point IIR4 structure.
 * @param[in]     pSrc          Filter input pointer
 * @param[out]    pDst          Filter output pointer
 * @param[in]     blockSize     Points to the fwd state buffer.
 */



void arm_iir8_f32_mve(const arm_iir_instance_f32 * S, const float32_t * pSrc, float32_t * pDst,
                       uint32_t blockSize)
{
    uint32_t         order;         /* Number of coefficients */
    const float32_t *pNumCoef;      /* Numerator Coefficients */
    const float32_t *pDenCoef;      /* Denominator Coefficients */
    float32_t       *pInputHist;    /* Pointer to the input history */
    float32_t       *pOutputHist;   /* Pointer to the output history */
    float32_t       *pResult;
    float32_t       *pTmpDst;
    const float32_t *pS;
    float32_t       *pD;
    int32_t          blkCnt;

    /* Load configuration parameters and saved state. */
    order = IIR_8_ORDER;
    pNumCoef = S->pNumCoef;
    pDenCoef = S->pDenCoef;
    pInputHist = &(S->pNumState[order + 1]);
    pOutputHist = &(S->pDenState[order + 1]);
    pResult = pOutputHist;
    pTmpDst = pDst;

    /* Copy new samples into history buffer */
    pS = pSrc;
    pD = pInputHist;
    blkCnt = blockSize;
    do {
        mve_pred16_t    p = vctp32q(blkCnt);

        vst1q_p(pD, vld1q_z(pS, p), p);
        pS += 4;
        pD += 4;
        blkCnt -= 4;
    }
    while (blkCnt > 0);

    /* main loop */

    float32_t      *fwd = &(S->pNumState[1]);
    float32_t      *bck = &(S->pDenState[1]);

#ifdef USE_ASM

    __asm volatile  (
        /* Preload 8x2 coefficients */
        "   vldrw.32        q2, [%[coef_fwd]], #16              \n"
        "   vldrw.32        q3, [%[coef_fwd]], #16              \n"

        "   vldrw.32        q5, [%[coef_bck]], #16              \n"
        "   vldrw.32        q6, [%[coef_bck]], #16              \n"


        ".p2align 2                                             \n"
        "   vldrw.u32       q0, [%[input_hist], #16]            \n"
        "   wls             lr, %[sample_cnt], 1f               \n"
        "2:                                                     \n"
        "   vmul.f32        q1, q0, q3                          \n"
        "   vldrw.u32       q0, [%[input_hist]], #4             \n"
        "   vfma.f32        q1, q0, q2                          \n"
        "   vldrw.u32       q0, [%[output_hist], #16]           \n"
        "   vfms.f32        q1, q0, q6                          \n"
        "   vldrw.u32       q0, [%[output_hist]], #4            \n"
        "   vfms.f32        q1, q0, q5                          \n"
        "   vldrw.u32       q0, [%[input_hist], #16]            \n"

        /* sum the partial accumulators */
        "   vadd.f32        s4, s4, s5                          \n"
        "   vadd.f32        s6, s6, s7                          \n"
        "   vldr            s5, [%[coef_fwd]]                   \n"
        "   vadd.f32        s4, s4, s6                          \n"

        /*  re-use fwd[8] in the last element of Q0 */
        "   vfma.f32        s4, s5, s3                          \n"

        /* populate destination and output history */
        "   vstm            %[pDst]!, {s4}                      \n"
        "   vstm            %[pResult]!, {s4}                   \n"
        "   le              lr, 2b                              \n"
        "1:                                                     \n"
        : [input_hist] "+r"(fwd),[output_hist] "+r"(bck),
          [sample_cnt] "+r"(blockSize),[pDst] "+r"(pTmpDst),
          [pResult] "+r"(pResult)
        :[coef_fwd] "r" (pNumCoef),[coef_bck] "r"(pDenCoef)
        :"q0", "q1", "q2", "q3",
         "q5", "q6",
         "r14", "memory");

#else                           /* MVE intrinsics */

    f32x4_t     acc0;
    f32x4_t     vecIn0;
    f32x4_t     vecCoefNum0, vecCoefDen0;
    f32x4_t     vecCoefNum1, vecCoefDen1;

    /*
     * preload 8 x 2 num/den coefficients
     */
    vecCoefNum0 = *(f32x4_t *) pNumCoef;
    vecCoefNum1 = *(f32x4_t *) (pNumCoef + 4);

    vecCoefDen0 = *(f32x4_t *) pDenCoef;
    vecCoefDen1 = *(f32x4_t *) (pDenCoef + 4);

    blkCnt = blockSize;
    while (blkCnt > 0) {
        float32_t       sum;

        vecIn0 = vld1q(fwd);
        acc0 = vmulq(vecIn0, vecCoefNum0);

        vecIn0 = vld1q(&fwd[4]);
        acc0 = vfmaq(acc0, vecIn0, vecCoefNum1);

        vecIn0 = vld1q(bck);
        acc0 = vfmsq(acc0, vecIn0, vecCoefDen0);

        vecIn0 = vld1q(&bck[4]);
        acc0 = vfmsq(acc0, vecIn0, vecCoefDen1);

        sum = acc0[0] + acc0[1] + acc0[2] + acc0[3];
        sum += fwd[IIR_8_ORDER] * pNumCoef[IIR_8_ORDER];
        bck = bck + 1;
        fwd = fwd + 1;

        *pTmpDst++ = sum;
        *pResult++ = sum;
        /*
         * Decrement the sample block loop counter
         */
        blkCnt--;
    }

#endif                          // USE_ASM

    /* save state */
    float32_t      *r = &(pOutputHist[blockSize - order]);
    const float32_t *s = &(pSrc[blockSize - order]);
    fwd = S->pNumState + 1;
    bck = S->pDenState + 1;

    blkCnt = order;
    while (blkCnt > 0U) {
        mve_pred16_t    p0 = vctp32q(blkCnt);

        vst1q_p(bck, vld1q(r), p0);
        vst1q_p(fwd, vld1q(s), p0);

        r += 4;
        bck += 4;
        s += 4;
        fwd += 4;
        blkCnt -= 4;
    }
}


/**
 * @brief 12th order floating-point IIR MVE version.
 * @param[in,out] S             points to an instance of the floating-point IIR4 structure.
 * @param[in]     pSrc          Filter input pointer
 * @param[out]    pDst          Filter output pointer
 * @param[in]     blockSize     Points to the fwd state buffer.
 */

void arm_iir12_f32_mve(const arm_iir_instance_f32 * S, const float32_t * pSrc, float32_t * pDst,
                       uint32_t blockSize)
{
    uint32_t         order;         /* Number of coefficients */
    const float32_t *pNumCoef;      /* Numerator Coefficients */
    const float32_t *pDenCoef;      /* Denominator Coefficients */
    float32_t       *pInputHist;    /* Pointer to the input history */
    float32_t       *pOutputHist;   /* Pointer to the output history */
    float32_t       *pResult;
    float32_t       *pTmpDst;
    const float32_t *pS;
    float32_t       *pD;
    int32_t          blkCnt;

    /* Load configuration parameters and saved state. */
    order = IIR_12_ORDER;
    pNumCoef = S->pNumCoef;
    pDenCoef = S->pDenCoef;
    pInputHist = &(S->pNumState[order + 1]);
    pOutputHist = &(S->pDenState[order + 1]);
    pResult = pOutputHist;
    pTmpDst = pDst;

    /* Copy new samples into history buffer */
    pS = pSrc;
    pD = pInputHist;
    blkCnt = blockSize;
    do {
        mve_pred16_t    p = vctp32q(blkCnt);

        vst1q_p(pD, vld1q_z(pS, p), p);
        pS += 4;
        pD += 4;
        blkCnt -= 4;
    }
    while (blkCnt > 0);

    /* main loop */

    float32_t      *fwd = &(S->pNumState[1]);
    float32_t      *bck = &(S->pDenState[1]);

#ifdef USE_ASM

    __asm volatile  (
        /* Preload 12x2 coefficients */
        "   vldrw.32        q2, [%[coef_fwd]], #16              \n"
        "   vldrw.32        q3, [%[coef_fwd]], #16              \n"
        "   vldrw.32        q4, [%[coef_fwd]], #16              \n"
        "   vldrw.32        q5, [%[coef_bck]], #16              \n"
        "   vldrw.32        q6, [%[coef_bck]], #16              \n"
        "   vldrw.32        q7, [%[coef_bck]], #16              \n"

        ".p2align 2                                             \n"
        "   vldrw.u32       q0, [%[input_hist], #32]            \n"
        "   wls             lr, %[sample_cnt], 1f               \n"
        "2:                                                     \n"
        "   vmul.f32        q1, q0, q4                          \n"
        "   vldrw.u32       q0, [%[input_hist], #16]            \n"
        "   vfma.f32        q1, q0, q3                          \n"
        "   vldrw.u32       q0, [%[input_hist]], #4             \n"
        "   vfma.f32        q1, q0, q2                          \n"
        "   vldrw.u32       q0, [%[output_hist], #32]           \n"
        "   vfms.f32        q1, q0, q7                          \n"
        "   vldrw.u32       q0, [%[output_hist], #16]           \n"
        "   vfms.f32        q1, q0, q6                          \n"
        "   vldrw.u32       q0, [%[output_hist]], #4            \n"
        "   vfms.f32        q1, q0, q5                          \n"
        "   vldrw.u32       q0, [%[input_hist], #32]            \n"

        /* sum the partial accumultors */
        "   vadd.f32        s4, s4, s5                          \n"
        "   vadd.f32        s6, s6, s7                          \n"
        "   vldr            s5, [%[coef_fwd]]                   \n"
        "   vadd.f32        s4, s4, s6                          \n"

        /*  re-use fwd[12] in the last element of Q0 */
        "   vfma.f32        s4, s5, s3                          \n"

        /* populate destination and output history */
        "   vstm            %[pDst]!, {s4}                      \n"
        "   vstm            %[pResult]!, {s4}                   \n"
        "   le              lr, 2b                              \n"
        "1:                                                     \n"
        : [input_hist] "+r"(fwd),[output_hist] "+r"(bck),
          [sample_cnt] "+r"(blockSize),[pDst] "+r"(pTmpDst),
          [pResult] "+r"(pResult)
        :[coef_fwd] "r" (pNumCoef),[coef_bck] "r"(pDenCoef)
        :"q0", "q1", "q2", "q3",
         "q4", "q5", "q6", "q7",
         "r14", "memory");

#else                           /* MVE intrinsics */

    f32x4_t     acc0;
    f32x4_t     vecIn0;
    f32x4_t     vecCoefNum0, vecCoefDen0;
    f32x4_t     vecCoefNum1, vecCoefDen1;
    f32x4_t     vecCoefNum2, vecCoefDen2;

    /*
     * preload 12 x 2 num/den coefficients
     */
    vecCoefNum0 = *(f32x4_t *) pNumCoef;
    vecCoefNum1 = *(f32x4_t *) (pNumCoef + 4);
    vecCoefNum2 = *(f32x4_t *) (pNumCoef + 8);

    vecCoefDen0 = *(f32x4_t *) pDenCoef;
    vecCoefDen1 = *(f32x4_t *) (pDenCoef + 4);
    vecCoefDen2 = *(f32x4_t *) (pDenCoef + 8);

    blkCnt = blockSize;
    while (blkCnt > 0) {
        float32_t       sum;

        vecIn0 = vld1q(fwd);
        acc0 = vmulq(vecIn0, vecCoefNum0);

        vecIn0 = vld1q(&fwd[4]);
        acc0 = vfmaq(acc0, vecIn0, vecCoefNum1);

        vecIn0 = vld1q(&fwd[8]);
        acc0 = vfmaq(acc0, vecIn0, vecCoefNum2);

        vecIn0 = vld1q(bck);
        acc0 = vfmsq(acc0, vecIn0, vecCoefDen0);

        vecIn0 = vld1q(&bck[4]);
        acc0 = vfmsq(acc0, vecIn0, vecCoefDen1);

        vecIn0 = vld1q(&bck[8]);
        acc0 = vfmsq(acc0, vecIn0, vecCoefDen2);

        sum = acc0[0] + acc0[1] + acc0[2] + acc0[3];
        sum += fwd[IIR_12_ORDER] * pNumCoef[IIR_12_ORDER];
        bck = bck + 1;
        fwd = fwd + 1;

        *pTmpDst++ = sum;
        *pResult++ = sum;
        /*
         * Decrement the sample block loop counter
         */
        blkCnt--;
    }

#endif                          // USE_ASM

    /* save state */
    float32_t      *r = &(pOutputHist[blockSize - order]);
    const float32_t *s = &(pSrc[blockSize - order]);
    fwd = S->pNumState + 1;
    bck = S->pDenState + 1;

    blkCnt = order;
    while (blkCnt > 0U) {
        mve_pred16_t    p0 = vctp32q(blkCnt);

        vst1q_p(bck, vld1q(r), p0);
        vst1q_p(fwd, vld1q(s), p0);

        r += 4;
        bck += 4;
        s += 4;
        fwd += 4;
        blkCnt -= 4;
    }
}




/**
 * @brief Nth order floating-point IIR MVE version.
 * @param[in,out] S             points to an instance of the floating-point IIR4 structure.
 * @param[in]     pSrc          Filter input pointer
 * @param[out]    pDst          Filter output pointer
 * @param[in]     blockSize     Points to the fwd state buffer.
 */

void arm_iir_f32_mve(const arm_iir_instance_f32 * S, const float32_t * pSrc, float32_t * pDst,
                       uint32_t blockSize)
{
    uint32_t         order;         /* Number of coefficients */
    const float32_t *pNumCoef;      /* Numerator Coefficients */
    const float32_t *pDenCoef;      /* Denominator Coefficients */
    float32_t       *pInputHist;    /* Pointer to the input history */
    float32_t       *pOutputHist;   /* Pointer to the output history */
    float32_t       *pResult;
    float32_t       *pTmpDst;
    const float32_t *pS;
    float32_t       *pD;
    int32_t          blkCnt;

    order = S->order;

    /* [1 to 12 taps] specialized routines */
    if (order <= 4)
        return arm_iir4_f32_mve(S, pSrc, pDst, blockSize);

    else if (order <= 8)
         return arm_iir8_f32_mve(S, pSrc, pDst, blockSize);

    else if (order <= 12)
        return arm_iir12_f32_mve(S, pSrc, pDst, blockSize);


    /* generic case for orders > 12 */

    /* Load configuration parameters and saved state. */
    pNumCoef = S->pNumCoef;
    pDenCoef = S->pDenCoef;
    pInputHist = &(S->pNumState[order + 1]);
    pOutputHist = &(S->pDenState[order + 1]);
    pResult = pOutputHist;
    pTmpDst = pDst;

    /* Copy new samples into history buffer */
    pS = pSrc;
    pD = pInputHist;
    blkCnt = blockSize;
    do {
        mve_pred16_t    p = vctp32q(blkCnt);

        vst1q_p(pD, vld1q_z(pS, p), p);
        pS += 4;
        pD += 4;
        blkCnt -= 4;
    }
    while (blkCnt > 0);

    /* main loop */

    float32_t      *fwd = &(S->pNumState[1]);
    float32_t      *bck = &(S->pDenState[1]);

#ifdef USE_ASM

    __asm volatile  (
        /* Preload 8x2 coefficients */
        "   vldrw.32        q2, [%[coef_fwd]], #16              \n"
        "   vldrw.32        q3, [%[coef_fwd]], #16              \n"

        "   vldrw.32        q5, [%[coef_bck]], #16              \n"
        "   vldrw.32        q6, [%[coef_bck]], #16              \n"


        ".p2align 2                                             \n"
        "iir_loop_%=:                                           \n"

        "   vldrw.u32       q0, [%[input_hist]], #16            \n"
        "   vmul.f32        q1, q0, q2                          \n"
        "   vldrw.u32       q0, [%[input_hist]], #16            \n"
        "   vfma.f32        q1, q0, q3                          \n"

        "   vldrw.u32       q0, [%[output_hist]], #16           \n"
        "   vfms.f32        q1, q0, q5                          \n"
        "   vldrw.u32       q0, [%[output_hist]], #16           \n"
        "   vfms.f32        q1, q0, q6                          \n"

        /* num/den coefs >= 8 */
        "   wls             lr, %[loop_cnt], 1f                 \n"
        "2:                                                     \n"

        "   vldrw.u32       q0, [%[input_hist]], #16            \n"
        "   vldrw.32        q4, [%[coef_fwd]], #16              \n"
        "   vfma.f32        q1, q0, q4                          \n"
        "   vldrw.u32       q0, [%[output_hist]], #16           \n"
        "   vldrw.32        q4, [%[coef_bck]], #16              \n"
        "   vfms.f32        q1, q0, q4                          \n"
        "   le              lr, 2b                              \n"
        "1:                                                     \n"


        /* sum the partial accumulators */
        "   vadd.f32        s4, s4, s5                          \n"
        "   vadd.f32        s6, s6, s7                          \n"
        "   vldr            s5, [%[coef_fwd]]                   \n"
        "   vadd.f32        s4, s4, s6                          \n"

        /*  re-use fwd[order] in the last element of Q0 */
        "   vfma.f32        s4, s5, s3                          \n"

        /* populate destination and output history */
        "   vstm            %[pDst]!, {s4}                      \n"
        "   vstm            %[pResult]!, {s4}                   \n"

        /* rewind coef and history pointers */
        "   sub             %[input_hist], %[rew_hist]          \n"
        "   sub             %[output_hist], %[rew_hist]         \n"
        "   sub             %[coef_fwd], %[rew_coef]            \n"
        "   sub             %[coef_bck], %[rew_coef]            \n"

        "   subs            %[sample_cnt], %[sample_cnt], #1    \n"
        "   bne             iir_loop_%=                         \n"

        : [input_hist] "+r"(fwd),[output_hist] "+r"(bck),
          [sample_cnt] "+r"(blockSize),[pDst] "+r"(pTmpDst),
          [pResult] "+r"(pResult)
        :[coef_fwd] "r" (pNumCoef),[coef_bck] "r"(pDenCoef),
          [loop_cnt] "r" (((order + 3) / 4) - 2),
          [rew_hist] "r" ((order-1)*sizeof(float32_t)),
          [rew_coef] "r" ((order-8)*sizeof(float32_t))
        :"q0", "q1", "q2", "q3",
         "q4", "q5", "q6",
         "r14", "memory", "cc");

#else                           /* MVE intrinsics */

    f32x4_t     acc0;
    f32x4_t     vecIn0;
    f32x4_t     vecCoefNum0, vecCoefDen0;
    f32x4_t     vecCoefNum1, vecCoefDen1;


    /*
     * preload 8 x 2 first coefficients
     */
    vecCoefNum0 = *(f32x4_t *) pNumCoef;
    pNumCoef += 4;
    vecCoefNum1 = *(f32x4_t *) pNumCoef;
    pNumCoef += 4;

    vecCoefDen0 = *(f32x4_t *) pDenCoef;
    pDenCoef += 4;
    vecCoefDen1 = *(f32x4_t *) pDenCoef;
    pDenCoef += 4;

    blkCnt = blockSize;
    while (blkCnt > 0U)
    {
        const float32_t   *pDenCoefCur = pDenCoef;
        const float32_t   *pNumCoefCur = pNumCoef;
        float32_t   *pFwdCur = fwd;
        float32_t   *pBckCur = bck;
        float32_t    sum;


        vecIn0 = vld1q_f32(pFwdCur);
        pFwdCur += 4;
        acc0 = vmulq(vecIn0, vecCoefNum0);

        vecIn0 = vld1q_f32(pFwdCur);
        pFwdCur += 4;
        acc0 = vfmaq(acc0, vecIn0, vecCoefNum1);

        vecIn0 = vld1q_f32(pBckCur);
        pBckCur += 4;
        acc0 = vfmsq(acc0, vecIn0, vecCoefDen0);

        vecIn0 = vld1q_f32(pBckCur);
        pBckCur += 4;
        acc0 = vfmsq(acc0, vecIn0, vecCoefDen1);

        /*
         * num/den coefs >= 8
         */
        uint32_t  i = (((order + 3) / 4) - 2);
        while (i > 0U)
        {
            f32x4_t vecCoef;

            vecIn0 = vld1q_f32(pFwdCur);
            pFwdCur += 4;

            vecCoef = vld1q_f32(pNumCoefCur);
            pNumCoefCur += 4;

            acc0 = vfmaq(acc0, vecIn0, vecCoef);

            vecCoef = vld1q_f32(pDenCoefCur);
            pDenCoefCur += 4;

            vecIn0 = vld1q_f32(pBckCur);
            pBckCur += 4;
            acc0 = vfmsq(acc0, vecIn0, vecCoef);

            i--;
        }

        sum = acc0[0] + acc0[1] + acc0[2] + acc0[3];
        sum += fwd[order] * pNumCoef[order];
        bck = bck + 1;
        fwd = fwd + 1;

        *pTmpDst++ = sum;
        *pResult++ = sum;
        /*
         * Decrement the sample block loop counter
         */
        blkCnt--;
    }

#endif                          // USE_ASM

    /* save state */
    float32_t      *r = &(pOutputHist[blockSize - order]);
    const float32_t *s = &(pSrc[blockSize - order]);
    fwd = S->pNumState + 1;
    bck = S->pDenState + 1;

    blkCnt = order;
    while (blkCnt > 0U) {
        mve_pred16_t    p0 = vctp32q(blkCnt);

        vst1q_p(bck, vld1q(r), p0);
        vst1q_p(fwd, vld1q(s), p0);

        r += 4;
        bck += 4;
        s += 4;
        fwd += 4;
        blkCnt -= 4;
    }
}



#endif                          // ARM_MATH_MVEF
