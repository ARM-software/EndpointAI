/* ----------------------------------------------------------------------
 * Title:        arm_mat_vec_mult_unscaled_q7_q15.c
 * Description:  Q7 matrix by Q15 vector multiplication
 *               Unscaled 32-bit output vector (Q22)
 *
 * $Date:        10 August 2021
 *
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

#include "dsp/matrix_functions.h"

/**
 * @ingroup groupMatrix
 */



/**
 * @addtogroup MatrixVectMult
 * @{
 */

/**
 * @brief Q7 matrix and Q15 vector multiplication.
 * @param[in]       *pSrcMat points to the input matrix structure
 * @param[in]       *pVec points to the input vector
 * @param[out]      *pDst points to the output vector
 */
#if defined(ARM_MATH_MVEI) && !defined(ARM_MATH_AUTOVECTORIZE)



#ifdef USE_ASM


/* unroll x 5 in full asm for a more efficient handling of the outer loop */

static void __attribute__((naked))
arm_mat_vec_mult_unscaled_q7_q15_unroll5(
    const int16_t     *pVec,
    const int8_t      *pMat,
    int32_t           *pOut,
    uint16_t           loopx5Cnt,
    uint16_t           rows,
    uint16_t           cols)
{

    __asm__ volatile (
        "   push        {r4-r11, lr}                  \n"
        /* reserve scratch space (3 x 32-bit words) */
        "   sub         sp, #12                       \n"

        /* retrieve rows & cols */
        "   ldrd        r11, r12, [sp, #(12+36)]      \n"
        /* destination pointer */
        "   mov         r9, r2                        \n"

        /* scratch layout
            scratch[0] = pIn;
            scratch[1] = cols;
            scratch[2] = loopx5Cnt;
        */
        "   strd        r0, r12, [sp]                 \n"
        "   str.w       r3, [sp, #8]                  \n"

        /* init current matrix row pointer */
        "   mov         r11, r1                       \n"

        ".p2align 2                                   \n"
        /* use wide instr. variants to keep alignment */

        /* outer loop start */
        "loop_unrollx5_start:                         \n"

        /* reload matrix row pointer & cols */
        "   ldrd        r7, lr, [sp]                  \n"

        /* derive the 5 curent row pointers */
        "   mov.w       r0, r11                       \n"
        "   add.w       r1, r0, lr                    \n"
        "   add.w       r2, r1, lr                    \n"
        "   add.w       r3, r2, lr                    \n"
        "   add.w       r5, r3, lr                    \n"

        /* row start for next iteration */
        "   add.w       r11, r5, lr                   \n"

        /* batch clear accumulators (ascending order) */
        "   clrm        {r4, r6, r8, r10, r12}        \n"

        /* inner loop start */
        "   wlstp.16    lr, lr, 1f                    \n"
        "2:                                           \n"
        /* 16-bit vector load */
        "   vldrh.16     q0, [r7], #16                \n"
        /* 8-bit matrix row load with 16-bit widening */
        "   vldrb.s16    q1, [r0], #8                 \n"
        "   vmlava.s16   r4, q0, q1                   \n"
        "   vldrb.s16    q1, [r1], #8                 \n"
        "   vmlava.s16   r6, q0, q1                   \n"
        "   vldrb.s16    q1, [r2], #8                 \n"
        "   vmlava.s16   r8, q0, q1                   \n"
        "   vldrb.s16    q1, [r3], #8                 \n"
        "   vmlava.s16   r10, q0, q1                  \n"
        "   vldrb.s16    q1, [r5], #8                 \n"
        "   vmlava.s16   r12, q0, q1                  \n"
        "   letp         lr, 2b                       \n"
        "1:                                           \n"

        /* store the 5 32-bit unscaled accumulators */
        /* into 32-bit destination vector  */
        "   strd         r4, r6, [r9], #8             \n"
        "   strd         r8, r10, [r9], #8            \n"
        "   str          r12, [r9], #4                \n"

        /* reload outer loop counter & decrement */
        "   ldr         r0, [sp, #8]                  \n"
        "   subs        r0, #1                        \n"
        "   str         r0, [sp, #8]                  \n"
        "   bne         loop_unrollx5_start           \n"

        /* restore stack and exit */
        "   add         sp, #12                       \n"
        "   pop.w       {r4- r11, pc}                 \n"
    );
}

void arm_mat_vec_mult_unscaled_q7_q15(
    const arm_matrix_instance_q7 * pSrcMat,
    const q15_t      *pSrcVec,
    q31_t            *pDstVec)

{
    uint32_t        numRows = pSrcMat->numRows;
    uint32_t        numCols = pSrcMat->numCols;
    const q7_t     *pSrcA = pSrcMat->pData;
    const q7_t     *pInA1;
    const q7_t     *pInA2;
    const q15_t    *pInVec = pSrcVec;
    uint32_t        loopx5Cnt = numRows / 5;

    if(loopx5Cnt ) {
        /* process blocks of 5 matrix rows */
        arm_mat_vec_mult_unscaled_q7_q15_unroll5(pSrcVec,
                                    pSrcA,
                                    pDstVec,
                                    loopx5Cnt,
                                    numRows,
                                    numCols);

        /* update for residual rows */
        pSrcA += loopx5Cnt * numCols * 5;
        numRows = numRows - loopx5Cnt * 5;
        pDstVec += loopx5Cnt * 5;
    }

    /* residual rows handling (4, 3, 2 & 1 specific cases) */

    if (numRows >= 4) {
        const q7_t     *pInA3;
        const q7_t     *pInA4;

        /* Initialize the 4 row-pointers */
        pInVec = pSrcVec;
        pInA1 = pSrcA;
        pInA2 = pInA1 + numCols;
        pInA3 = pInA2 + numCols;
        pInA4 = pInA3 + numCols;

        /* Enforce registers assignement for :                                              */
        /* - avoiding consuming extra intermediate variable for low overhead loop counter   */
        /* - avoiding assembler warnings regarding ascending order during CLRM call.        */
        /* VMLADAVA accumulators have to be even. This condition can be already be          */
        /* controled with inline asm constraints "Te", but registers ordering cannot        */
        /* without explicit assignement                                                     */

        register  uint32_t loopCnt  __asm("lr") = numCols;
        register  q31_t sum0  __asm("r4");
        register  q31_t sum1  __asm("r6");
        register  q31_t sum2  __asm("r8");
        register  q31_t sum3  __asm("r10");

        __asm volatile  (
            /* reset accumulators */
            "   clrm                {%[sum0], %[sum1], %[sum2], %[sum3]}            \n"
            ".p2align 2                                                             \n"
            "   wlstp.16            lr, %[colCnt] , 1f                              \n"
            "   2:                                                                  \n"
            "   vldrh.16            q0, [%[pInVec]], #16                            \n"
            "   vldrb.s16           q1, [%[pInA1]], #8                              \n"
            "   vmladava.s16        %[sum0], q0, q1                                 \n"
            "   vldrb.s16           q1, [%[pInA2]], #8                              \n"
            "   vmladava.s16        %[sum1], q0, q1                                 \n"
            "   vldrb.s16           q1, [%[pInA3]], #8                              \n"
            "   vmladava.s16        %[sum2], q0, q1                                 \n"
            "   vldrb.s16           q1, [%[pInA4]], #8                              \n"
            "   vmladava.s16        %[sum3], q0, q1                                 \n"
            "   letp                lr, 2b                                          \n"
            "   1:                                                                  \n"

            /* store output vector */
            "   strd                %[sum0], %[sum1], [%[px]],8                     \n"
            "   strd                %[sum2], %[sum3], [%[px]]                       \n"

            :[pInVec] "+r"(pInVec),[pInA1] "+l"(pInA1),[pInA2] "+l"(pInA2),
             [pInA3] "+l"(pInA3),[pInA4] "+l"(pInA4),
             [colCnt] "+r"(loopCnt),[px] "+r"(pDstVec),[sum0] "=Te"(sum0),
             [sum1] "=Te"(sum1),[sum2] "=Te"(sum2),[sum3] "=Te"(sum3)            :
            :"q0", "q1", "memory");


    } else if (numRows >= 3) {
        const q7_t     *pInA3;
        pInVec = pSrcVec;

        /* Initialize the 3 row-pointers */
        pInA1 = pSrcA;
        pInA2 = pInA1 + numCols;
        pInA3 = pInA2 + numCols;


        register  uint32_t loopCnt  __asm("lr") = numCols;
        register  q31_t sum0  __asm("r4");
        register  q31_t sum1  __asm("r6");
        register  q31_t sum2  __asm("r8");

        __asm volatile  (
            /* reset accumulators */
            "   clrm                {%[sum0], %[sum1], %[sum2]}                     \n"
            ".p2align 2                                                             \n"
            "   wlstp.16            lr, %[colCnt] , 1f                              \n"
            "   2:                                                                  \n"
            "   vldrh.16            q0, [%[pInVec]], #16                            \n"
            "   vldrb.s16           q1, [%[pInA1]], #8                              \n"
            "   vmladava.s16        %[sum0], q0, q1                                 \n"
            "   vldrb.s16           q1, [%[pInA2]], #8                              \n"
            "   vmladava.s16        %[sum1], q0, q1                                 \n"
            "   vldrb.s16           q1, [%[pInA3]], #8                              \n"
            "   vmladava.s16        %[sum2], q0, q1                                 \n"
            "   letp                lr, 2b                                          \n"
            "   1:                                                                  \n"

            /* store output vector */
            "   strd                %[sum0], %[sum1], [%[px]],8                     \n"
            "   str                 %[sum2], [%[px]]                                \n"
            :[pInVec] "+r"(pInVec),[pInA1] "+l"(pInA1),[pInA2] "+l"(pInA2),
             [pInA3] "+l"(pInA3),
             [colCnt] "+r"(loopCnt),[px] "+r"(pDstVec),[sum0] "=Te"(sum0),
             [sum1] "=Te"(sum1),[sum2] "=Te"(sum2)
            :
            :"q0", "q1", "memory");

    } else if (numRows >= 2) {
        pInVec = pSrcVec;
        /* Initialize the 2 row-pointers */
        pInA1 = pSrcA;
        pInA2 = pInA1 + numCols;

        register  uint32_t loopCnt  __asm("lr") = numCols;
        register  q31_t sum0  __asm("r4");
        register  q31_t sum1  __asm("r6");

        __asm volatile  (
            /* reset accumulators */
            "   clrm                {%[sum0], %[sum1]}                              \n"
            ".p2align 2                                                             \n"
            "   wlstp.16            lr, %[colCnt] , 1f                              \n"
            "   2:                                                                  \n"
            "   vldrh.16            q0, [%[pInVec]], #16                            \n"
            "   vldrb.s16           q1, [%[pInA1]], #8                              \n"
            "   vmladava.s16        %[sum0], q0, q1                                 \n"
            "   vldrb.s16           q1, [%[pInA2]], #8                              \n"
            "   vmladava.s16        %[sum1], q0, q1                                 \n"
            "   letp                lr, 2b                                          \n"
            "   1:                                                                  \n"

            /* store output vector */
            "   strd                %[sum0], %[sum1], [%[px]],8                     \n"
            :[pInVec] "+r"(pInVec),[pInA1] "+l"(pInA1),[pInA2] "+l"(pInA2),
             [colCnt] "+r"(loopCnt),[px] "+r"(pDstVec),[sum0] "=Te"(sum0),
             [sum1] "=Te"(sum1)
             :
            :"q0", "q1", "memory");

    }  else if (numRows >= 1) {
        pInVec = pSrcVec;
        pInA1 = pSrcA;

        register  uint32_t loopCnt  __asm("lr") = numCols;
        register  q31_t sum0  __asm("r4");
        __asm volatile  (
            /* reset accumulators */
            "   clrm                {%[sum0]}                                       \n"
            ".p2align 2                                                             \n"
            "   wlstp.16            lr, %[colCnt] , 1f                              \n"
            "   2:                                                                  \n"
            "   vldrh.16            q0, [%[pInVec]], #16                            \n"
            "   vldrb.s16           q1, [%[pInA1]], #8                              \n"
            "   vmladava.s16        %[sum0], q0, q1                                 \n"
            "   letp                lr, 2b                                          \n"
            "   1:                                                                  \n"

            /* store output vector */
            "   str                 %[sum0], [%[px]]                                \n"
            :[pInVec] "+r"(pInVec),[pInA1] "+l"(pInA1),
             [colCnt] "+r"(loopCnt),[px] "+r"(pDstVec),[sum0] "=Te"(sum0)
            :
            :"q0", "q1", "memory");
    }
}

#else /* USE_ASM */

/* MVE Intrinsics variant */

void arm_mat_vec_mult_unscaled_q7_q15(
    const arm_matrix_instance_q7 * pSrcMat,
    const q15_t      *pSrcVec,
    q31_t            *pDstVec)
{
    const q7_t     *pMatSrc = pSrcMat->pData;
    uint32_t        numRows = pSrcMat->numRows;
    uint32_t        numCols = pSrcMat->numCols;
    const q7_t     *pMat0, *pMat1;
    int32_t         blkCnt;


    while (numRows >= 5) {
        /*
         * compute 5 accumulators per loop
         */
        const q7_t     *pMat2, *pMat3, *pMat4;
        q15_t const    *pVec;
        q31_t           acc0, acc1, acc2, acc3, acc4;
        q15x8_t         vecMatA0, vecMatA1, vecMatA2, vecMatA3, vecMatA4, vecIn;

        pVec = pSrcVec;
        /*
         * Initialize row pointers
         */
        pMat0 = pMatSrc;
        pMat1 = pMat0 + numCols;
        pMat2 = pMat1 + numCols;
        pMat3 = pMat2 + numCols;
        pMat4 = pMat3 + numCols;

        acc0 = 0L;
        acc1 = 0L;
        acc2 = 0L;
        acc3 = 0L;
        acc4 = 0L;

        blkCnt = numCols;
        while (blkCnt > 0) {
            mve_pred16_t    p0 = vctp16q(blkCnt);

            vecMatA0 = vldrbq_z_s16(pMat0, p0);
            vecMatA1 = vldrbq_z_s16(pMat1, p0);
            vecMatA2 = vldrbq_z_s16(pMat2, p0);
            vecMatA3 = vldrbq_z_s16(pMat3, p0);
            vecMatA4 = vldrbq_z_s16(pMat4, p0);
            vecIn = vld1q_z(pVec, p0);

            acc0 = vmladavaq_p(acc0, vecIn, vecMatA0, p0);
            acc1 = vmladavaq_p(acc1, vecIn, vecMatA1, p0);
            acc2 = vmladavaq_p(acc2, vecIn, vecMatA2, p0);
            acc3 = vmladavaq_p(acc3, vecIn, vecMatA3, p0);
            acc4 = vmladavaq_p(acc4, vecIn, vecMatA4, p0);

            pVec += 8;
            pMat0 += 8;
            pMat1 += 8;
            pMat2 += 8;
            pMat3 += 8;
            pMat4 += 8;
            blkCnt -= 8;
        }

        /* store the 5 32-bit unscaled accumulators */
        /* into 32-bit destination vector  */
        *pDstVec++ = acc0;
        *pDstVec++ = acc1;
        *pDstVec++ = acc2;
        *pDstVec++ = acc3;
        *pDstVec++ = acc4;

        pMatSrc += numCols * 5;
        /*
         * Decrement the row loop counter
         */
        numRows -= 5;
    }


    if (numRows >= 4) {
        const q7_t     *pMat2, *pMat3;
        q15_t const    *pVec;
        q31_t           acc0, acc1, acc2, acc3;
        q15x8_t         vecMatA0, vecMatA1, vecMatA2, vecMatA3, vecIn;

        pVec = pSrcVec;

        pMat0 = pMatSrc;
        pMat1 = pMat0 + numCols;
        pMat2 = pMat1 + numCols;
        pMat3 = pMat2 + numCols;

        acc0 = 0L;
        acc1 = 0L;
        acc2 = 0L;
        acc3 = 0L;

        blkCnt = numCols;
        while (blkCnt > 0) {
            mve_pred16_t    p0 = vctp16q(blkCnt);

            vecMatA0 = vldrbq_z_s16(pMat0, p0);
            vecMatA1 = vldrbq_z_s16(pMat1, p0);
            vecMatA2 = vldrbq_z_s16(pMat2, p0);
            vecMatA3 = vldrbq_z_s16(pMat3, p0);
            vecIn = vld1q_z(pVec, p0);

            acc0 = vmladavaq_p(acc0, vecIn, vecMatA0, p0);
            acc1 = vmladavaq_p(acc1, vecIn, vecMatA1, p0);
            acc2 = vmladavaq_p(acc2, vecIn, vecMatA2, p0);
            acc3 = vmladavaq_p(acc3, vecIn, vecMatA3, p0);

            pVec += 8;
            pMat0 += 8;
            pMat1 += 8;
            pMat2 += 8;
            pMat3 += 8;

            blkCnt -= 8;
        }

        *pDstVec++ = acc0;
        *pDstVec++ = acc1;
        *pDstVec++ = acc2;
        *pDstVec++ = acc3;

        pMatSrc += numCols * 4;
        numRows -= 4;
    }

    /*
     * process any remaining rows pair
     */
    if (numRows >= 2) {
        q15_t const    *pVec;
        q31_t           acc0, acc1;
        q15x8_t         vecMatA0, vecMatA1, vecIn;

        pVec = pSrcVec;
        pMat0 = pMatSrc;
        pMat1 = pMat0 + numCols;

        acc0 = 0;
        acc1 = 0;

        blkCnt = numCols;
        while (blkCnt > 0) {
            mve_pred16_t    p0 = vctp16q(blkCnt);

            vecMatA0 = vldrbq_z_s16(pMat0, p0);
            vecMatA1 = vldrbq_z_s16(pMat1, p0);
            vecIn = vld1q_z(pVec, p0);

            acc0 = vmladavaq_p(acc0, vecIn, vecMatA0, p0);
            acc1 = vmladavaq_p(acc1, vecIn, vecMatA1, p0);

            pVec += 8;
            pMat0 += 8;
            pMat1 += 8;
            blkCnt -= 8;
        }

        *pDstVec++ = acc0;
        *pDstVec++ = acc1;

        pMatSrc += numCols * 2;
        numRows -= 2;
    }

    if (numRows >= 1) {
        q15_t const    *pVec;
        q31_t           acc0;
        q15x8_t         vecMatA0, vecIn;

        pVec = pSrcVec;
        pMat0 = pMatSrc;
        acc0 = 0;

        blkCnt = numCols;
        while (blkCnt > 0) {
            mve_pred16_t    p0 = vctp16q(blkCnt);

            vecMatA0 = vldrbq_z_s16(pMat0, p0);
            vecIn = vld1q_z(pVec, p0);

            acc0 = vmladavaq_p(acc0, vecIn, vecMatA0, p0);

            pVec += 8;
            pMat0 += 8;
            blkCnt -= 8;
        }
        *pDstVec++ = acc0;
    }
}

#endif


#else

/* scalar / autovectorized variant */

void arm_mat_vec_mult_unscaled_q7_q15(const arm_matrix_instance_q7 *pMat, const q15_t *pSrcVec, q31_t *pDstVec)
{
    uint32_t        numRows = pMat->numRows;
    uint32_t        numCols = pMat->numCols;
    const q7_t     *pMatSrc = pMat->pData;
    const q7_t     *pMat0, *pMat1;
    q15_t const    *pVec;
    int32_t         blkCnt;

    while (numRows >= 4) {
        const q7_t     *pMat2, *pMat3;
        q31_t           acc0, acc1, acc2, acc3;

        pVec = pSrcVec;
        pMat0 = pMatSrc;
        pMat1 = pMat0 + numCols;
        pMat2 = pMat1 + numCols;
        pMat3 = pMat2 + numCols;

        acc0 = 0L;
        acc1 = 0L;
        acc2 = 0L;
        acc3 = 0L;

        blkCnt = numCols;
        while (blkCnt > 0) {
            q15_t           curVec = *pVec++;

            acc0 += (q31_t) * pMat0++ * curVec;
            acc1 += (q31_t) * pMat1++ * curVec;
            acc2 += (q31_t) * pMat2++ * curVec;
            acc3 += (q31_t) * pMat3++ * curVec;
            blkCnt--;
        }

        /* store the 32-bit unscaled accumulators */
        /* into 32-bit destination vector  */
        *pDstVec++ = acc0;
        *pDstVec++ = acc1;
        *pDstVec++ = acc2;
        *pDstVec++ = acc3;

        pMatSrc += numCols * 4;
        numRows -= 4;
    }

    if (numRows >= 2) {
        q31_t           acc0, acc1;

        pVec = pSrcVec;
        pMat0 = pMatSrc;
        pMat1 = pMat0 + numCols;

        acc0 = 0L;
        acc1 = 0L;

        blkCnt = numCols;
        while (blkCnt > 0) {
            q15_t           curVec = *pVec++;

            acc0 += (q31_t) * pMat0++ * curVec;
            acc1 += (q31_t) * pMat1++ * curVec;
            blkCnt--;
        }

        *pDstVec++ = acc0;
        *pDstVec++ = acc1;

        pMatSrc += numCols * 2;
        numRows -= 2;
    }

    if (numRows > 0) {
        int32_t         colCnt = numCols;
        q31_t           sum = 0;

        pVec = pSrcVec;
        while (colCnt > 0) {
            sum += (q31_t) * pMatSrc++ * *pVec++;
            colCnt--;
        }

        *pDstVec = sum;
    }
}

#endif /* defined(ARM_MATH_MVEI) */

/**
 * @} end of MatrixMult group
 */

