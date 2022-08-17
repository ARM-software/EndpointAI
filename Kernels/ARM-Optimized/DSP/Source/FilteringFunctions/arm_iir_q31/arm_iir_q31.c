/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_iir_q31.c
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


#include "arm_iir_q31.h"



/**
 * @brief  Nth order Q.31 IIR reference.
 * @param[in,out] S             points to an instance of the Q.31 IIR structure.
 * @param[in]     pSrc          Filter input pointer
 * @param[out]    pDst          Filter output pointer
 * @param[in]     blockSize     Points to the fwd state buffer.
 */

void arm_iir_q31(const arm_iir_instance_q31 * S, const q31_t * pSrc, q31_t * pDst,
                 uint32_t blockSize)
{
    const q31_t    *pIn = pSrc;                 /*  source pointer            */
    q31_t          *pOut = pDst;                /*  destination pointer       */
    q31_t          *pNumState = S->pNumState;   /*  pState pointer            */
    q31_t          *pDenState = S->pDenState;   /*  pState pointer            */
    const q31_t    *pA = S->pDenCoef;           /*  coefficient pointer       */
    const q31_t    *pB = S->pNumCoef;           /*  coefficient pointer       */
    q31_t           X0;
    q63_t           acc0;                       /* accumulator vector */
    int32_t         sample;
    int32_t         order = S->order;
    int32_t         uShift = (S->postShift + 1U);
    int32_t         lShift = 32U - uShift;      /* Shift to be applied to the output */


    sample = blockSize;

    while (sample > 0) {
        X0 = *pIn++;

        acc0 = ((q63_t)X0 * pB[0]);
        for (int i = 0; i < order; i++) {
            acc0 += ((q63_t)pNumState[i] * pB[1 + i]);
        }
        for (int i = 0; i < order; i++) {
            acc0 -= ((q63_t)pDenState[i] * pA[i]);
        }

        /*
         * Store the result in the accumulator in the destination buffer.
         */
        acc0 >>= lShift;
        *pOut++ = (q31_t) (acc0);

        /*
         * update recurrence
         */
        for (int i = order - 1; i >= 1; i--)
            pNumState[i] = pNumState[i - 1];
        pNumState[0] = X0;

        for (int i = order - 1; i >= 1; i--)
            pDenState[i] = pDenState[i - 1];
        pDenState[0] = (q31_t) (acc0);

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
 * @brief  4th order Q.31 IIR MVE version.
 * @param[in,out] S             points to an instance of the Q.31 IIR structure.
 * @param[in]     pSrc          Filter input pointer
 * @param[out]    pDst          Filter output pointer
 * @param[in]     blockSize     Points to the fwd state buffer.
 */



void arm_iir4_q31_mve(const arm_iir_instance_q31 * S, const q31_t * pSrc, q31_t * pDst,
                       uint32_t blockSize)
{
    uint32_t         order;         /* Number of coefficients */
    const q31_t     *pNumCoef;      /* Numerator Coefficients */
    const q31_t     *pDenCoef;      /* Denominator Coefficients */
    q31_t           *pInputHist;    /* Pointer to the input history */
    q31_t           *pOutputHist;   /* Pointer to the output history */
    q31_t           *pResult;
    q31_t           *pTmpDst;
    const q31_t     *pS;
    q31_t           *pD;
    int32_t          blkCnt;
    int32_t          pShift = S->postShift;
    q31_t            lastNumCoef;

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

    lastNumCoef = pNumCoef[IIR_4_ORDER];

    q31_t      *fwd = &(S->pNumState[1]);
    q31_t      *bck = &(S->pDenState[1]);
    q63_t       sum;

#ifdef USE_ASM
    register unsigned count  __asm("lr") = blockSize;

    __asm volatile  (
        /* Preload 4x2 coefficients */
        "   vldrw.32        q2, [%[coef_fwd]], #16                      \n"
        "   vldrw.32        q5, [%[coef_bck]], #16                      \n"

        "   vldrw.u32       q0, [%[input_hist]], #4                     \n"
        "   ldr             %Q[sum], [%[input_hist], %[order], lsl #2]  \n"

        ".p2align 2                                                     \n"
        "   wls             lr, %[count], 1f                            \n"
        "2:                                                             \n"
        /* fwd[order] * pNumCoef[order] */
        "   smull           %Q[sum], %R[sum], %Q[sum], %[lastNumC]      \n"
        "   vmlaldava.s32   %Q[sum], %R[sum], q0, q2                    \n"
        /* backward (coefs are negated at init stage */
        "   vldrw.u32       q0, [%[output_hist]], #4                    \n"
        "   vmlaldava.s32   %Q[sum], %R[sum], q0, q5                    \n"

        /* load next samples */
        "   vldrw.u32       q0, [%[input_hist]], #4                     \n"

        /* scale output and extract high part */
        "   sqrshrl         %Q[sum], %R[sum], #64, %[shift]             \n"
        "   ldr             %Q[sum], [%[input_hist], %[order], lsl #2]  \n"

        /* populate destination and output history */
        "   str             %R[sum], [%[pDst]], #4                      \n"
        "   str             %R[sum], [%[pResult]], #4                   \n"
        "   le              lr, 2b                                      \n"
        "1:                                                             \n"
        : [input_hist] "+r"(fwd),[output_hist] "+r"(bck),
          [count] "+r"(count),[pDst] "+r"(pTmpDst),
          [pResult] "+r"(pResult),
          [sum] "=&r"(sum)
        :[coef_fwd] "r" (pNumCoef),[coef_bck] "r"(pDenCoef),
         [lastNumC] "r" (lastNumCoef), [order] "r" (order-1),
         [shift] "r" (-(1+pShift))
        :"q0", "q1", "q2", "q5",
         "memory");

#else                           /* MVE intrinsics */

    q31x4_t     vecIn0;
    q31x4_t     vecCoefNum0, vecCoefDen0;

    /*
     * preload 4 x 2 num/den coefficients
     */
    vecCoefNum0 = *(q31x4_t *) pNumCoef;

    vecCoefDen0 = *(q31x4_t *) pDenCoef;

    blkCnt = blockSize;
    while (blkCnt > 0) {

        sum = ((q63_t)fwd[IIR_4_ORDER] * lastNumCoef);

        vecIn0 = vld1q(fwd);
        sum = vmlaldavaq(sum, vecIn0, vecCoefNum0);

        vecIn0 = vld1q(bck);
        sum = vmlaldavaq(sum, vecIn0, vecCoefDen0);

        sum = sqrshrl(sum, -(1+pShift));
        bck = bck + 1;
        fwd = fwd + 1;

        *pTmpDst++ = (q31_t) (sum >> 32U);
        *pResult++ = (q31_t) (sum >> 32U);
        /*
         * Decrement the sample block loop counter
         */
        blkCnt--;
    }

#endif                          // USE_ASM

    /* save state */
    q31_t      *r = &(pOutputHist[blockSize - order]);
    const q31_t *s = &(pSrc[blockSize - order]);
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
 * @brief  8th order Q.31 IIR MVE version.
 * @param[in,out] S             points to an instance of the Q.31 IIR structure.
 * @param[in]     pSrc          Filter input pointer
 * @param[out]    pDst          Filter output pointer
 * @param[in]     blockSize     Points to the fwd state buffer.
 */



void arm_iir8_q31_mve(const arm_iir_instance_q31 * S, const q31_t * pSrc, q31_t * pDst,
                       uint32_t blockSize)
{
    uint32_t         order;         /* Number of coefficients */
    const q31_t     *pNumCoef;      /* Numerator Coefficients */
    const q31_t     *pDenCoef;      /* Denominator Coefficients */
    q31_t           *pInputHist;    /* Pointer to the input history */
    q31_t           *pOutputHist;   /* Pointer to the output history */
    q31_t           *pResult;
    q31_t           *pTmpDst;
    const q31_t     *pS;
    q31_t           *pD;
    int32_t          blkCnt;
    int32_t          pShift = S->postShift;
    q31_t            lastNumCoef;

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
    lastNumCoef = pNumCoef[IIR_8_ORDER];

    q31_t      *fwd = &(S->pNumState[1]);
    q31_t      *bck = &(S->pDenState[1]);
    q63_t       sum;

#ifdef USE_ASM

    register unsigned count  __asm("lr") = blockSize;

    __asm volatile  (
        /* Preload 8x2 coefficients */
        "   vldrw.32        q2, [%[coef_fwd]], #16                      \n"
        "   vldrw.32        q3, [%[coef_fwd]], #16                      \n"

        "   vldrw.32        q5, [%[coef_bck]], #16                      \n"
        "   vldrw.32        q6, [%[coef_bck]], #16                      \n"

        "   vldrw.u32       q0, [%[input_hist], #16]                    \n"
        "   ldr             %Q[sum], [%[input_hist], %[order], lsl #2]  \n"

        ".p2align 2                                                     \n"
        "   wls             lr, %[count], 1f                            \n"
        "2:                                                             \n"
        /* fwd[order] * pNumCoef[order] */
        "   smull           %Q[sum], %R[sum], %Q[sum], %[lastNumC]      \n"
        "   vmlaldava.s32   %Q[sum], %R[sum], q0, q3                    \n"
        "   vldrw.u32       q0, [%[input_hist]], #4                     \n"
        "   vmlaldava.s32   %Q[sum], %R[sum], q0, q2                    \n"

        /* backward (coefs are negated at init stage */
        "   vldrw.u32       q0, [%[output_hist], #16]                   \n"
        "   vmlaldava.s32   %Q[sum], %R[sum], q0, q6                    \n"
        "   vldrw.u32       q0, [%[output_hist]], #4                    \n"
        "   vmlaldava.s32   %Q[sum], %R[sum], q0, q5                    \n"

        /* load next samples */
        "   vldrw.u32       q0, [%[input_hist], #16]                    \n"

        /* scale output and extract high part */
        "   sqrshrl         %Q[sum], %R[sum], #64, %[shift]             \n"
        "   ldr             %Q[sum], [%[input_hist], %[order], lsl #2]  \n"

        /* populate destination and output history */
        "   str             %R[sum], [%[pDst]], #4                      \n"
        "   str             %R[sum], [%[pResult]], #4                   \n"
        "   le              lr, 2b                                      \n"
        "1:                                                             \n"
        : [input_hist] "+r"(fwd),[output_hist] "+r"(bck),
          [count] "+r"(count),[pDst] "+r"(pTmpDst),
          [pResult] "+r"(pResult),
          [sum] "=&r"(sum)
        :[coef_fwd] "r" (pNumCoef),[coef_bck] "r"(pDenCoef),
         [lastNumC] "r" (lastNumCoef), [order] "r" (order),
         [shift] "r" (-(1+pShift))
        :"q0", "q1", "q2", "q3",
         "q5", "q6",
         "memory");

#else                           /* MVE intrinsics */

    q31x4_t     vecIn0;
    q31x4_t     vecCoefNum0, vecCoefDen0;
    q31x4_t     vecCoefNum1, vecCoefDen1;

    /*
     * preload 8 x 2 num/den coefficients
     */
    vecCoefNum0 = *(q31x4_t *) pNumCoef;
    vecCoefNum1 = *(q31x4_t *) (pNumCoef + 4);

    vecCoefDen0 = *(q31x4_t *) pDenCoef;
    vecCoefDen1 = *(q31x4_t *) (pDenCoef + 4);

    blkCnt = blockSize;
    while (blkCnt > 0) {

        sum = ((q63_t)fwd[IIR_8_ORDER] * lastNumCoef);

        vecIn0 = vld1q(fwd);
        sum = vmlaldavaq(sum, vecIn0, vecCoefNum0);

        vecIn0 = vld1q(&fwd[4]);
        sum = vmlaldavaq(sum, vecIn0, vecCoefNum1);

        vecIn0 = vld1q(bck);
        sum = vmlaldavaq(sum, vecIn0, vecCoefDen0);

        vecIn0 = vld1q(&bck[4]);
        sum = vmlaldavaq(sum, vecIn0, vecCoefDen1);

        sum = sqrshrl(sum, -(1+pShift));
        bck = bck + 1;
        fwd = fwd + 1;

        *pTmpDst++ = (q31_t) (sum >> 32U);
        *pResult++ = (q31_t) (sum >> 32U);
        /*
         * Decrement the sample block loop counter
         */
        blkCnt--;
    }

#endif                          // USE_ASM

    /* save state */
    q31_t      *r = &(pOutputHist[blockSize - order]);
    const q31_t *s = &(pSrc[blockSize - order]);
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
 * @brief 12th order Q.31 IIR MVE version.
 * @param[in,out] S             points to an instance of the Q.31 IIR structure.
 * @param[in]     pSrc          Filter input pointer
 * @param[out]    pDst          Filter output pointer
 * @param[in]     blockSize     Points to the fwd state buffer.
 */

void arm_iir12_q31_mve(const arm_iir_instance_q31 * S, const q31_t * pSrc, q31_t * pDst,
                       uint32_t blockSize)
{
    uint32_t         order;         /* Number of coefficients */
    const q31_t     *pNumCoef;      /* Numerator Coefficients */
    const q31_t     *pDenCoef;      /* Denominator Coefficients */
    q31_t           *pInputHist;    /* Pointer to the input history */
    q31_t           *pOutputHist;   /* Pointer to the output history */
    q31_t           *pResult;
    q31_t           *pTmpDst;
    const q31_t     *pS;
    q31_t           *pD;
    int32_t          blkCnt;
    int32_t          pShift = S->postShift;
    q31_t            lastNumCoef;

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
    lastNumCoef = pNumCoef[IIR_12_ORDER];

    q31_t      *fwd = &(S->pNumState[1]);
    q31_t      *bck = &(S->pDenState[1]);
    q63_t       sum;

#ifdef USE_ASM

    register unsigned count  __asm("lr") = blockSize;

    __asm volatile  (
        /* Preload 12x2 coefficients */
        "   vldrw.32        q2, [%[coef_fwd]], #16                      \n"
        "   vldrw.32        q3, [%[coef_fwd]], #16                      \n"
        "   vldrw.32        q4, [%[coef_fwd]], #16                      \n"

        "   vldrw.32        q5, [%[coef_bck]], #16                      \n"
        "   vldrw.32        q6, [%[coef_bck]], #16                      \n"
        "   vldrw.32        q7, [%[coef_bck]], #16                      \n"

        "   vldrw.u32       q0, [%[input_hist], #32]                    \n"
        "   ldr             %Q[sum], [%[input_hist], %[order], lsl #2]  \n"

        ".p2align 2                                                     \n"
        "   wls             lr, %[count], 1f                            \n"
        "2:                                                             \n"
        /* fwd[order] * pNumCoef[order] */
        "   smull           %Q[sum], %R[sum], %Q[sum], %[lastNumC]      \n"
        "   vmlaldava.s32   %Q[sum], %R[sum], q0, q4                    \n"
        "   vldrw.u32       q0, [%[input_hist], #16]                    \n"
        "   vmlaldava.s32   %Q[sum], %R[sum], q0, q3                    \n"
        "   vldrw.u32       q0, [%[input_hist]], #4                     \n"
        "   vmlaldava.s32   %Q[sum], %R[sum], q0, q2                    \n"

        /* backward (coefs are negated at init stage */
        "   vldrw.u32       q0, [%[output_hist], #32]                   \n"
        "   vmlaldava.s32   %Q[sum], %R[sum], q0, q7                    \n"
        "   vldrw.u32       q0, [%[output_hist], #16]                   \n"
        "   vmlaldava.s32   %Q[sum], %R[sum], q0, q6                    \n"
        "   vldrw.u32       q0, [%[output_hist]], #4                    \n"
        "   vmlaldava.s32   %Q[sum], %R[sum], q0, q5                    \n"

        /* load next samples */
        "   vldrw.u32       q0, [%[input_hist], #32]                    \n"

        /* scale output and extract high part */
        "   sqrshrl         %Q[sum], %R[sum], #64, %[shift]             \n"
        "   ldr             %Q[sum], [%[input_hist], %[order], lsl #2]  \n"

        /* populate destination and output history */
        "   str             %R[sum], [%[pDst]], #4                      \n"
        "   str             %R[sum], [%[pResult]], #4                   \n"
        "   le              lr, 2b                                      \n"
        "1:                                                             \n"
        : [input_hist] "+r"(fwd),[output_hist] "+r"(bck),
          [count] "+r"(count),[pDst] "+r"(pTmpDst),
          [pResult] "+r"(pResult),
          [sum] "=&r"(sum)
        :[coef_fwd] "r" (pNumCoef),[coef_bck] "r"(pDenCoef),
         [lastNumC] "r" (lastNumCoef), [order] "r" (order),
         [shift] "r" (-(1+pShift))
        :"q0", "q1", "q2", "q3",
         "q5", "q6",
         "memory");


#else                           /* MVE intrinsics */

    q31x4_t     vecIn0;
    q31x4_t     vecCoefNum0, vecCoefDen0;
    q31x4_t     vecCoefNum1, vecCoefDen1;
    q31x4_t     vecCoefNum2, vecCoefDen2;

    /*
     * preload 12 x 2 num/den coefficients
     */
    vecCoefNum0 = *(q31x4_t *) pNumCoef;
    vecCoefNum1 = *(q31x4_t *) (pNumCoef + 4);
    vecCoefNum2 = *(q31x4_t *) (pNumCoef + 8);

    vecCoefDen0 = *(q31x4_t *) pDenCoef;
    vecCoefDen1 = *(q31x4_t *) (pDenCoef + 4);
    vecCoefDen2 = *(q31x4_t *) (pDenCoef + 8);

    blkCnt = blockSize;
    while (blkCnt > 0) {

        sum = ((q63_t)fwd[IIR_12_ORDER] * lastNumCoef);

        vecIn0 = vld1q(fwd);
        sum = vmlaldavaq(sum, vecIn0, vecCoefNum0);

        vecIn0 = vld1q(&fwd[4]);
        sum = vmlaldavaq(sum, vecIn0, vecCoefNum1);

        vecIn0 = vld1q(&fwd[8]);
        sum = vmlaldavaq(sum, vecIn0, vecCoefNum2);

        vecIn0 = vld1q(bck);
        sum = vmlaldavaq(sum, vecIn0, vecCoefDen0);

        vecIn0 = vld1q(&bck[4]);
        sum = vmlaldavaq(sum, vecIn0, vecCoefDen1);

        vecIn0 = vld1q(&bck[8]);
        sum = vmlaldavaq(sum, vecIn0, vecCoefDen2);

        sum = sqrshrl(sum, -(1+pShift));
        bck = bck + 1;
        fwd = fwd + 1;

        *pTmpDst++ = (q31_t) (sum >> 32U);
        *pResult++ = (q31_t) (sum >> 32U);
        /*
         * Decrement the sample block loop counter
         */
        blkCnt--;
    }

#endif                          // USE_ASM

    /* save state */
    q31_t      *r = &(pOutputHist[blockSize - order]);
    const q31_t *s = &(pSrc[blockSize - order]);
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
 * @brief Nth order Q.31 IIR MVE version.
 * @param[in,out] S             points to an instance of the Q.31 IIR structure.
 * @param[in]     pSrc          Filter input pointer
 * @param[out]    pDst          Filter output pointer
 * @param[in]     blockSize     Points to the fwd state buffer.
 */

void arm_iir_q31_mve(const arm_iir_instance_q31 * S, const q31_t * pSrc, q31_t * pDst,
                       uint32_t blockSize)
{
    uint32_t         order;         /* Number of coefficients */
    const q31_t     *pNumCoef;      /* Numerator Coefficients */
    const q31_t     *pDenCoef;      /* Denominator Coefficients */
    q31_t           *pInputHist;    /* Pointer to the input history */
    q31_t           *pOutputHist;   /* Pointer to the output history */
    q31_t           *pResult;
    q31_t           *pTmpDst;
    const q31_t     *pS;
    q31_t           *pD;
    int32_t          blkCnt;
    int32_t          pShift = S->postShift;
    q31_t            lastNumCoef;

    order = S->order;

    /* [1 to 12 taps] specialized routines */
    if (order <= 4)
        return arm_iir4_q31_mve(S, pSrc, pDst, blockSize);

    else if (order <= 8)
         return arm_iir8_q31_mve(S, pSrc, pDst, blockSize);

    else if (order <= 12)
        return arm_iir12_q31_mve(S, pSrc, pDst, blockSize);


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
    lastNumCoef = pNumCoef[order];

    q31_t      *fwd = &(S->pNumState[1]);
    q31_t      *bck = &(S->pDenState[1]);
    q63_t       sum;

#if 0 //def USE_ASM
    /* to be completed */
    __asm volatile  (
        /* Preload 8x2 coefficients */
        "   vldrw.32        q2, [%[coef_fwd]], #16                              \n"
        "   vldrw.32        q3, [%[coef_fwd]], #16                              \n"

        "   vldrw.32        q5, [%[coef_bck]], #16                              \n"
        "   vldrw.32        q6, [%[coef_bck]], #16                              \n"


        ".p2align 2                                                             \n"
        "iir_loop_%=:                                                           \n"
        /* fwd[order] * pNumCoef[order] */
        "   smull           %Q[sum], %R[sum], %Q[sum], %[lastNumC]              \n"

        "   vldrw.u32       q0, [%[input_hist]], #16                            \n"
        "   vmlalva.s32     %Q[sum], %R[sum], q0, q2                            \n"
        "   vldrw.u32       q0, [%[input_hist]], #16                            \n"
        "   vmlalva.s32     %Q[sum], %R[sum], q0, q3                            \n"

        "   vldrw.u32       q0, [%[output_hist]], #16                           \n"
        "   vmlalva.s32     %Q[sum], %R[sum], q0, q5                            \n"
        "   vldrw.u32       q0, [%[output_hist]], #16                           \n"
        "   vmlalva.s32     %Q[sum], %R[sum], q0, q6                            \n"

        /* num/den coefs >= 8 */
        "   wls             lr, %[loop_cnt], 1f                                 \n"
        "2:                                                                     \n"

        "   vldrw.u32       q0, [%[input_hist]], #16                            \n"
        "   vldrw.32        q4, [%[coef_fwd]], #16                              \n"
        "   vmlalva.s32     %Q[sum], %R[sum], , q0, q4                          \n"
        "   vldrw.u32       q0, [%[output_hist]], #16                           \n"
        "   vldrw.32        q4, [%[coef_bck]], #16                              \n"
        "   vmlalva.s32     %Q[sum], %R[sum], q0, q4                            \n"
        "   le              lr, 2b                                              \n"
        "1:                                                                     \n"



        /* scale output and extract high part */
        "   sqrshrl         %Q[sum], %R[sum], #64, %[shift]                     \n"
        "   ldr             %Q[sum], [%[input_hist], %[order], lsl #2]          \n"

        /* populate destination and output history */
        "   str             %R[sum], [%[pDst]], #4                              \n"
        "   str             %R[sum], [%[pResult]], #4                           \n"

        /* rewind coef and history pointers */
        "   sub             %[input_hist], %[rew_hist]                          \n"
        "   sub             %[output_hist], %[rew_hist]                         \n"
        "   sub             %[coef_fwd], %[rew_coef]                            \n"
        "   sub             %[coef_bck], %[rew_coef]                            \n"

        "   subs            %[sample_cnt], %[sample_cnt], #1                    \n"
        "   bne             iir_loop_%=                                         \n"

        : [input_hist] "+r"(fwd),[output_hist] "+r"(bck),
          [sample_cnt] "+r"(blockSize),[pDst] "+r"(pTmpDst),
          [pResult] "+r"(pResult)
        :[coef_fwd] "r" (pNumCoef),[coef_bck] "r"(pDenCoef),
          [loop_cnt] "r" (((order + 3) / 4) - 2),
          [rew_hist] "r" ((order-1)*sizeof(q31_t)),
          [rew_coef] "r" ((order-8)*sizeof(q31_t))
        :"q0", "q1", "q2", "q3",
         "q4", "q5", "q6",
         "r14", "memory", "cc");

#else                           /* MVE intrinsics */

    q31x4_t     vecIn0;
    q31x4_t     vecCoefNum0, vecCoefDen0;
    q31x4_t     vecCoefNum1, vecCoefDen1;


    /*
     * preload 8 x 2 first coefficients
     */
    vecCoefNum0 = *(q31x4_t *) pNumCoef;
    pNumCoef += 4;
    vecCoefNum1 = *(q31x4_t *) pNumCoef;
    pNumCoef += 4;

    vecCoefDen0 = *(q31x4_t *) pDenCoef;
    pDenCoef += 4;
    vecCoefDen1 = *(q31x4_t *) pDenCoef;
    pDenCoef += 4;

    blkCnt = blockSize;
    while (blkCnt > 0U)
    {
        const q31_t   *pDenCoefCur = pDenCoef;
        const q31_t   *pNumCoefCur = pNumCoef;
        q31_t   *pFwdCur = fwd;
        q31_t   *pBckCur = bck;


        sum = ((q63_t)fwd[order] * lastNumCoef);

        vecIn0 = vld1q(pFwdCur);
        pFwdCur += 4;
        sum = vmlaldavaq(sum, vecIn0, vecCoefNum0);

        vecIn0 = vld1q(pFwdCur);
        pFwdCur += 4;
        sum = vmlaldavaq(sum, vecIn0, vecCoefNum1);

        vecIn0 = vld1q(pBckCur);
        pBckCur += 4;
        sum = vmlaldavaq(sum, vecIn0, vecCoefDen0);

        vecIn0 = vld1q(pBckCur);
        pBckCur += 4;
        sum = vmlaldavaq(sum, vecIn0, vecCoefDen1);

        /*
         * num/den coefs >= 8
         */
        uint32_t  i = (((order + 3) / 4) - 2);
        while (i > 0U)
        {
            q31x4_t vecCoef;

            vecIn0 = vld1q(pFwdCur);
            pFwdCur += 4;

            vecCoef = vld1q(pNumCoefCur);
            pNumCoefCur += 4;

            sum = vmlaldavaq(sum, vecIn0, vecCoef);

            vecCoef = vld1q(pDenCoefCur);
            pDenCoefCur += 4;

            vecIn0 = vld1q(pBckCur);
            pBckCur += 4;
            sum = vmlaldavaq(sum, vecIn0, vecCoef);

            i--;
        }

        sum = sqrshrl(sum, -(1+pShift));
        bck = bck + 1;
        fwd = fwd + 1;

        *pTmpDst++ = (q31_t) (sum >> 32U);
        *pResult++ = (q31_t) (sum >> 32U);
        /*
         * Decrement the sample block loop counter
         */
        blkCnt--;
    }

#endif                          // USE_ASM

    /* save state */
    q31_t      *r = &(pOutputHist[blockSize - order]);
    const q31_t *s = &(pSrc[blockSize - order]);
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
