/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_iir4th_order_cascade_f32.c
 * Description:  Cascade of 4th Order Single Precision IIR (experimental)
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


#include "arm_iir4th_order_cascade_f32.h"


/**
 * @brief  4th order floating-point IIR.
 * @param[in,out] S             points to an instance of the floating-point IIR4 structure.
 * @param[in]     pSrc          input sample pointer
 * @param[out]    pDst          Filter output pointer
 * @param[in]     blockSize     Number of samples to filter
 * @par           (temporary) restriction : blockSize must be even
 */

void arm_iir4_f32(float32_t * pState, const float32_t * pCoef, const float32_t * pSrc, float32_t * pDst,
                  uint32_t blockSize)
{
    const float32_t *pIn = pSrc;                            /*  source pointer            */
    float32_t       *pOut = pDst;                           /*  destination pointer       */
    float32_t       *pStateNum = pState;                    /*  pState pointer            */
    float32_t       *pStateDen = pStateNum + IIR_4_ORDER;   /*  pState pointer            */
    const float32_t *pNum = pCoef;                          /*  coefficient pointer       */
    const float32_t *pDen = pCoef + (IIR_4_ORDER + 1);      /*  coefficient pointer       */
    float32_t        Xn1, Xn2, Xn3, Xn4;
    float32_t        Yn1, Yn2, Yn3, Yn4;                    /*  Filter pState variables   */
    float32_t        X0, X1;
    float32_t        acc0, acc1;                            /*  Accumulators              */
    int32_t          sample;

    /*
     * Reading the pState values
     */
    Xn1 = pStateNum[0];
    Xn2 = pStateNum[1];
    Xn3 = pStateNum[2];
    Xn4 = pStateNum[3];

    Yn1 = pStateDen[0];
    Yn2 = pStateDen[1];
    Yn3 = pStateDen[2];
    Yn4 = pStateDen[3];

    sample = blockSize;

    while (sample > 0) {
        X0 = *pIn++;
        X1 = *pIn++;

        acc0 =
            X0 * pNum[0] + Xn1 * pNum[1] + Xn2 * pNum[2] + Xn3 * pNum[3] + Xn4 * pNum[4]
            - (Yn1 * pDen[0] + Yn2 * pDen[1] + Yn3 * pDen[2] + Yn4 * pDen[3]);

        acc1 = X1 * pNum[0] + X0 * pNum[1] + Xn1 * pNum[2] + Xn2 * pNum[3] + Xn3 * pNum[4]
            - (acc0 * pDen[0] + Yn1 * pDen[1] + Yn2 * pDen[2] + Yn3 * pDen[3]);

        /*
         * Store the result in the accumulator in the destination buffer.
         */
        *pOut++ = acc0;
        *pOut++ = acc1;
        /*
         * update recurrence
         */
        Xn4 = Xn2;
        Xn3 = Xn1;
        Xn2 = X0;
        Xn1 = X1;

        Yn4 = Yn2;
        Yn3 = Yn1;
        Yn2 = acc0;
        Yn1 = acc1;

        /*
         * decrement the loop counter
         */
        sample -= 2;
    }

    /*
     * Store the updated state variables back into the pState array
     */
    *pStateNum++ = Xn1;
    *pStateNum++ = Xn2;
    *pStateNum++ = Xn3;
    *pStateNum++ = Xn4;

    *pStateDen++ = Yn1;
    *pStateDen++ = Yn2;
    *pStateDen++ = Yn3;
    *pStateDen++ = Yn4;
}


/**
 * @brief  cascade of 4th order floating-point IIR.
 * @param[in,out] S             Points to an instance of the cascaded IIR4 structure.
 * @param[in]     pSrc          Filter input sample pointer
 * @param[out]    pDst          Filter output pointer
 * @param[in]     blockSize     Number of samples to filter
 * @par           (temporary) restriction : blockSize must be even
 */

void arm_iir4_cascade_f32(const arm_iir4_casd_inst_f32 * S, const float32_t * pSrc,
                          float32_t * pDst, uint32_t blockSize)
{
    int32_t          numStages = S->numStages;
    const float32_t *pIn = pSrc;         /* Source pointer */
    float32_t       *pOut = pDst;        /* Destination pointer */
    float32_t       *pState = S->pState;
    const float32_t *pCoeffs = S->pCoeffs;

    for (int curStage = 0; curStage < numStages; curStage++) {

        arm_iir4_f32(pState, pCoeffs, pIn, pOut, blockSize);

        /* Retrieve next stage context & coeffs */
        pState += (2 * IIR_4_ORDER);
        pCoeffs += (2 * IIR_4_ORDER + 1);

        /* The first stage goes from the input buffer to the output buffer. */
        /* Subsequent numStages occur in-place in the output buffer */
        pIn = pDst;

        /* Reset output pointer */
        pOut = pDst;
    }
}


#if defined(ARM_MATH_MVEF)

/**
 * @brief  4th order MVE floating-point IIR.
 * @param[in,out] S             points to an instance of the floating-point IIR4 structure.
 * @param[in]     pSrc          input sample pointer
 * @param[out]    pDst          Filter output pointer
 * @param[in]     blockSize     Number of samples to filter
 * @par           (temporary) restriction : blockSize must be multiple of 4
 */

void arm_iir4_f32_mve(float32_t * pState, const float32_t * pSrc, float32_t * pDst,
                      uint32_t blockSize)
{
    const float32_t *pIn = pSrc;                 /*  source pointer            */
    float32_t       *pOut = pDst;                /*  destination pointer       */
    float32_t       *pCoeffs = pState;           /*  coefficient pointer       */
    float32_t       *pStateNum = pState + IIR4_F32_COEF_MATRIX_SZ;     /*  pState pointer            */
    float32_t       *pStateDen = pStateNum + IIR_4_ORDER;     /*  pState pointer            */
    float32_t       Xn1, Xn2, Xn3, Xn4;
    float32_t       Yn1, Yn2, Yn3, Yn4;          /*  Filter pState variables   */
    float32x4_t     accVec;                      /* accumulator vector */


    /*
     * Reading the pState values
     */
    Xn1 = pStateNum[0];
    Xn2 = pStateNum[1];
    Xn3 = pStateNum[2];
    Xn4 = pStateNum[3];

    Yn1 = pStateDen[0];
    Yn2 = pStateDen[1];
    Yn3 = pStateDen[2];
    Yn4 = pStateDen[3];

#ifndef USE_ASM

    int32_t         sample = blockSize >> 2;

    /* MVE Intrinsics */
    while (sample > 0) {
        float32x4_t        coeffs;
        float32_t          X0, X1, X2, X3;

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
        accVec = vfmaq(accVec, coeffs, Xn3);

        coeffs = vld1q(&pCoeffs[28]);
        accVec = vfmaq(accVec, coeffs, Xn4);

        coeffs = vld1q(&pCoeffs[32]);
        accVec = vfmaq(accVec, coeffs, Yn1);

        coeffs = vld1q(&pCoeffs[36]);
        accVec = vfmaq(accVec, coeffs, Yn2);

        coeffs = vld1q(&pCoeffs[40]);
        accVec = vfmaq(accVec, coeffs, Yn3);

        coeffs = vld1q(&pCoeffs[44]);
        accVec = vfmaq(accVec, coeffs, Yn4);

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
        Xn3 = X1;
        Xn4 = X0;

        Yn1 = vgetq_lane(accVec, 3);
        Yn2 = vgetq_lane(accVec, 2);
        Yn3 = vgetq_lane(accVec, 1);
        Yn4 = vgetq_lane(accVec, 0);

        /*
         * decrement the loop counter
         */
        sample--;
    }

    /*
     * Store the updated state variables back into the pState array
     */

    *pStateDen++ = Yn1;
    *pStateDen++ = Yn2;
    *pStateDen++ = Yn3;
    *pStateDen++ = Yn4;

#else /* inline Asm */

    float32x4_t     YVec;
    register unsigned loloopCnt  __asm("lr") = ((blockSize) >> 2U);

    YVec[0] = Yn4;
    YVec[1] = Yn3;
    YVec[2] = Yn2;
    YVec[3] = Yn1;

 __asm volatile (
        ".p2align 2                                               \n"
        "    wls             lr, lr, 1f                           \n"
        "2:                                                       \n"
        "    ldrd            r0, r1, [%[pIn]], 8                  \n"
        "    ldrd            r2, r3, [%[pIn]], 8                  \n"

        "    vldrw.32        q0, [%[pCoeffs]]                     \n"
        "    vmul.f32        %[accVec], q0, r3                    \n"

        "    vldrw.32        q0, [%[pCoeffs], #(4*4)]             \n"
        "    vfma.f32        %[accVec], q0, r2                    \n"

        "    vldrw.32        q0, [%[pCoeffs], #(8*4)]             \n"
        "    vfma.f32        %[accVec], q0, r1                    \n"

        "    vldrw.32        q0, [%[pCoeffs], #(12*4)]            \n"
        "    vfma.f32        %[accVec], q0, r0                    \n"

        "    vldrw.32        q0, [%[pCoeffs], #(16*4)]            \n"
        "    vfma.f32        %[accVec], q0, %[Xn1]                \n"

        "    vmov            q1[2], q1[0], r0, r2                 \n"
        "    vmov            r2, r0, %[YVec][3], %[YVec][1]       \n"

        "    vldrw.32        q0, [%[pCoeffs], #(20*4)]            \n"
        "    vfma.f32        %[accVec], q0, %[Xn2]                \n"

        "    vldrw.32        q0, [%[pCoeffs], #(24*4)]            \n"
        "    vfma.f32        %[accVec], q0, %[Xn3]                \n"

        "    vmov            q1[3], q1[1], r1, r3                 \n"
        "    vmov            r3, r1, %[YVec][2], %[YVec][0]       \n"

        "    vldrw.32        q0, [%[pCoeffs], #(28*4)]            \n"
        "    vfma.f32        %[accVec], q0, %[Xn4]                \n"


        "    vldrw.32        q0, [%[pCoeffs], #(32*4)]            \n"
        "    vfma.f32        %[accVec], q0, r0                    \n"

        "    vldrw.32        q0, [%[pCoeffs], #(36*4)]            \n"
        "    vfma.f32        %[accVec], q0, r1                    \n"

        "    vldrw.32        q0, [%[pCoeffs], #(40*4)]            \n"
        "    vfma.f32        %[accVec], q0, r2                    \n"

        "    vldrw.32        q0, [%[pCoeffs], #(44*4)]            \n"
        "    vfma.f32        %[accVec], q0, r3                    \n"

        "    vstrw.32        %[accVec], [%[pOut]], #16            \n"

       /*
        * update recurrence
        */
        //        Xn1 = X3;
        //        Xn2 = X2;
        //        Xn3 = X1;
        //        Xn4 = X0;
        "     vmov           %[Xn4], %[Xn2], q1[2], q1[0]        \n"
        "     vmov           %[Xn3], %[Xn1], q1[3], q1[1]        \n"

        // Yn1 = vgetq_lane(accVec, 3);
        // Yn2 = vgetq_lane(accVec, 2);
        // Yn3 = vgetq_lane(accVec, 1);
        // Yn4 = vgetq_lane(accVec, 0);
        "     vmov          %[YVec], %[accVec]                   \n"

        "     le            lr, 2b                               \n"
        "1:                                                      \n"
       : [loloopCnt] "+r" (loloopCnt), [pIn] "+r" (pIn),
         [accVec] "+t" (accVec), [pOut] "+r" (pOut),
         [Xn1] "+r" (Xn1), [Xn2] "+r" (Xn2),
         [Xn3] "+r" (Xn3), [Xn4] "+r" (Xn4),
         [YVec] "+t" (YVec)
       : [pCoeffs] "r" (pCoeffs)
       : "r0", "r1", "r2", "r3",
         "q0", "q1", "memory" );

    /*
     * Store the updated state variables back into the pState array
     */
    *pStateDen++ = YVec[3];
    *pStateDen++ = YVec[2];
    *pStateDen++ = YVec[1];
    *pStateDen++ = YVec[0];

#endif // USE_ASM

    *pStateNum++ = Xn1;
    *pStateNum++ = Xn2;
    *pStateNum++ = Xn3;
    *pStateNum++ = Xn4;
}


/**
 * @brief  cascade of 4th order floating-point IIR (MVE version)
 * @param[in,out] S             Points to an instance of the cascaded IIR4 structure.
 * @param[in]     pSrc          Filter input sample pointer
 * @param[out]    pDst          Filter output pointer
 * @param[in]     blockSize     Number of samples to filter
 * @par           (temporary)   restriction : blockSize must be a multiple of 4
 */

void arm_iir4_cascade_f32_mve(const arm_iir4_casd_inst_f32 * S, const float32_t * pSrc,
                          float32_t * pDst, uint32_t blockSize)
{
    int32_t          numStages = S->numStages;
    const float32_t *pIn = pSrc;        /* Source pointer */
    float32_t       *pOut = pDst;        /* Destination pointer */
    float32_t       *pState = S->pState;

    for (int curStage = 0; curStage < numStages; curStage++) {

        arm_iir4_f32_mve(pState, pIn, pOut, blockSize);

        /* Retrieve next stage context */
        pState += IIR4_F32_CASC_MVE_CONTEXT_SZ;

        /* The first stage goes from the input buffer to the output buffer. */
        /* Subsequent numStages occur in-place in the output buffer */
        pIn = pDst;

        /* Reset output pointer */
        pOut = pDst;
    }
}

#endif // ARM_MATH_MVEF

