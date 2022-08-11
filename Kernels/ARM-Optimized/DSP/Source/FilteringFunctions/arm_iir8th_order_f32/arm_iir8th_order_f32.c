/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_iir8th_order_f32.c
 * Description:  MVE 8th order IIR (experimental)
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


#include "arm_iir8th_order_f32.h"



/**
 * @brief  Nth order floating-point IIR reference.
 * @param[in,out] S             points to an instance of the floating-point IIR4 structure.
 * @param[in]     pSrc          Filter input pointer
 * @param[out]    pDst          Filter output pointer
 * @param[in]     blockSize     Points to the fwd state buffer.
 */

void arm_iir8_f32(const arm_iir8_instance_f32 * S, const float32_t * pSrc, float32_t * pDst,
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
    int32_t         order = IIR_8_ORDER;


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

/**
 * @brief  8th order floating-point IIR MVE version.
 * @param[in,out] S             points to an instance of the floating-point IIR4 structure.
 * @param[in]     pSrc          Filter input pointer
 * @param[out]    pDst          Filter output pointer
 * @param[in]     blockSize     Points to the fwd state buffer.
 */

void arm_iir8_f32_mve(const arm_iir8_instance_f32 * S, const float32_t * pSrc, float32_t * pDst,
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

    // Load configuration parameters and saved state.
    order = IIR_8_ORDER;       // fixed
    pNumCoef = S->pNumCoef;
    pDenCoef = S->pDenCoef;
    pInputHist = &(S->pNumState[order + 1]);
    pOutputHist = &(S->pDenState[order + 1]);
    pResult = pOutputHist;
    pTmpDst = pDst;

    // Copy new samples into history buffer
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
        // Preload some coefficients
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

        /* sum the partial accumultors */
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
         "q4", "q5", "q6", "q7",
         "r14", "memory");

#else                           /* MVE intrinsics */

    float32x4_t     acc0;
    float32x4_t     vecIn0;
    float32x4_t     vecCoefNum0, vecCoefDen0;
    float32x4_t     vecCoefNum1, vecCoefDen1;

    /*
     * preload coefficients
     */
    vecCoefNum0 = *(float32x4_t *) pNumCoef;
    vecCoefNum1 = *(float32x4_t *) (pNumCoef + 4);

    vecCoefDen0 = *(float32x4_t *) pDenCoef;
    vecCoefDen1 = *(float32x4_t *) (pDenCoef + 4);

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
    order = IIR_8_ORDER;
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
