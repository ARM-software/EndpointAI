/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_cfft_q31.c
 * Description:  Combined Radix Decimation in Frequency CFFT fixed point processing function
 *               using Helium assembly kernels
 *               This version allows boosting CFFT Q31 performance when using compilers having suboptimal
 *               Helium intrinsic code generation.
 *
 * $Date:        Mar 2023
 * $Revision:    V1.0.2
 *
 * Target Processor: Cortex-M with Helium
 * -------------------------------------------------------------------- */
/*
 * Copyright (C) 2010-2023 ARM Limited or its affiliates. All rights reserved.
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

#if defined(ARM_MATH_MVEI) && !defined(ARM_MATH_AUTOVECTORIZE)

#include "arm_vec_fft.h"

#ifdef USE_ASM

#include "../arm_cfft_utils.h"

/*
 Helium CFFT Radix 2

  for (i = 0; i < n2; i++)
  {
     cosVal = pCoef[2 * i];
     sinVal = pCoef[2 * i + 1];

     l = i + n2;

     xt =           (pSrc[2 * i] >> 1U) - (pSrc[2 * l] >> 1U);
     pSrc[2 * i] = ((pSrc[2 * i] >> 1U) + (pSrc[2 * l] >> 1U)) >> 1U;

     yt =               (pSrc[2 * i + 1] >> 1U) - (pSrc[2 * l + 1] >> 1U);
     pSrc[2 * i + 1] = ((pSrc[2 * l + 1] >> 1U) + (pSrc[2 * i + 1] >> 1U)) >> 1U;

     pSrc[2 * l]     = (((int16_t) (((q31_t) xt * cosVal) >> 16U)) +
                        ((int16_t) (((q31_t) yt * sinVal) >> 16U))  );

     pSrc[2 * l + 1] = (((int16_t) (((q31_t) yt * cosVal) >> 16U)) -
                        ((int16_t) (((q31_t) xt * sinVal) >> 16U))   );
  }
*/

#define RAD2_BFLY_FX32_MVE(conj)                                       \
    ".p2align 2                                                            \n"\
    "   wls                 lr, %[count], 1f                               \n"\
    "   vldrw.32            q0, [%[pIn0]]                                  \n"\
    /* low overhead loop start */                                             \
    "2:                                                                    \n"\
    "   vshr.s32            q0, q0, #1                                     \n"\
    "   vldrw.32            q1, [%[pIn1]]                                  \n"\
    "   vshr.s32            q1, q1, #1                                     \n"\
    /* a0,a1 */                                                               \
    "   vhadd.s32           q2, q0, q1                                     \n"\
    /* twiddle */                                                             \
    "   vldrw.32            q7, [%[pCoef]], #16                            \n"\
    /* xt,yt */                                                               \
    "   vhsub.s32           q3, q0, q1                                     \n"\
    "   vldrw.32            q0, [%[pIn0], #16]                             \n"\
    cmplx_fx_mul_r_##conj(32, q4, q3, q7)                                     \
    "   vstrw.32            q2, [%[pIn0]], 16                              \n"\
    cmplx_fx_mul_i_##conj(32, q4, q3, q7)                                     \
    "   vstrw.32            q4, [%[pIn1]], 16                              \n"\
    /* low overhead loop end */                                               \
    "   le                  lr, 2b                                         \n"\
    "1:                                                                    \n"



static void arm_bitreversal_q31_inpl_mve_asm(
        uint32_t *pSrc,
  const uint16_t bitRevLen,
  const uint16_t *pBitRevTab)
{

     __asm volatile(
        "   vmov.i32        q0, #0x1                     \n"
        /* convert 16-bit bit-reverse index into 64-bit */
        /* complex scatter store offsets */
        "   vldrh.u32       q5, [%[pBitRevTab]], #8      \n"
        "   vmullb.u32      q2, q5, q0                   \n"
        "   vmullt.u32      q1, q5, q0                   \n"
        /* low overhead loop start */
        ".p2align 2                                      \n"
        "   wls             lr, %[bitRevLen], 1f         \n"
        "2:                                              \n"
        "   vldrh.u32       q5, [%[pBitRevTab]], #8      \n"
        "   vldrd.u64       q3, [%[pSrc], q2]            \n"
        "   vmullb.u32      q6, q5, q0                   \n"
        "   vldrd.u64       q4, [%[pSrc], q1]            \n"
        "   vmullt.u32      q7, q5, q0                   \n"
        /* scatter store */
        "   vstrd.64        q4, [%[pSrc], q2]            \n"
        "   vstrd.64        q3, [%[pSrc], q1]            \n"
        "   vldrh.u32       q5, [%[pBitRevTab]], #8      \n"
        "   vldrd.u64       q3, [%[pSrc], q6]            \n"
        "   vmullb.u32      q2, q5, q0                   \n"
        "   vldrd.u64       q4, [%[pSrc], q7]            \n"
        "   vmullt.u32      q1, q5, q0                   \n"
        "   vstrd.64        q4, [%[pSrc], q6]            \n"
        "   vstrd.64        q3, [%[pSrc], q7]            \n"
        /* low overhead loop end */
        "   le              lr, 2b                       \n"
        "1:                                              \n"

        /*  tail handling as core loop was unrolled */
        "   wlstp.32        lr, %[bitRevLenRem], 1f      \n"
        "2:                                              \n"
        "   vldrd.u64       q3, [%[pSrc], q2]            \n"
        "   vldrd.u64       q4, [%[pSrc], q1]            \n"
        "   vstrd.64        q4, [%[pSrc], q2]            \n"
        "   vstrd.64        q3, [%[pSrc], q1]            \n"
        "   vldrh.u32       q5, [%[pBitRevTab]], #8      \n"
        "   vmullb.u32      q2, q5, q0                   \n"
        "   vmullt.u32      q1, q5, q0                   \n"
        "   letp            lr, 2b                       \n"
        "1:                                              \n"
        : [pBitRevTab] "+r" (pBitRevTab)
        : [bitRevLen] "r" (bitRevLen/8),
          [bitRevLenRem] "r" (bitRevLen&7), [pSrc] "r" (pSrc)
        : "q0", "q1", "q2", "q3",
          "q4", "q5", "q6", "q7",
          "lr", "memory"
        );
}
#endif

static void _arm_radix4_butterfly_q31_mve(
    const arm_cfft_instance_q31 * S,
    q31_t   *pSrc,
    uint32_t fftLen)
{
#ifndef USE_ASM
    q31x4_t vecTmp0, vecTmp1;
    q31x4_t vecSum0, vecDiff0, vecSum1, vecDiff1;
    q31x4_t vecA, vecB, vecC, vecD;
    q31x4_t vecW;
    uint32_t  blkCnt;
#endif
    uint32_t  n1, n2;
    uint32_t  stage = 0;
    int32_t  iter = 1;
    static const int32_t strides[4] = {
        (0 - 16) * (int32_t)sizeof(q31_t *), (1 - 16) * (int32_t)sizeof(q31_t *),
        (8 - 16) * (int32_t)sizeof(q31_t *), (9 - 16) * (int32_t)sizeof(q31_t *)
    };


    /*
     * Process first stages
     * Each stage in middle stages provides two down scaling of the input
     */
    n2 = fftLen;
    n1 = n2;
    n2 >>= 2u;

#pragma nounroll
    for (int k = fftLen / 4u; k > 1; k >>= 2u)
    {
        q31_t const *p_rearranged_twiddle_tab_stride2 =
            &S->rearranged_twiddle_stride2[
            S->rearranged_twiddle_tab_stride2_arr[stage]];
        q31_t const *p_rearranged_twiddle_tab_stride3 = &S->rearranged_twiddle_stride3[
            S->rearranged_twiddle_tab_stride3_arr[stage]];
        q31_t const *p_rearranged_twiddle_tab_stride1 =
            &S->rearranged_twiddle_stride1[
            S->rearranged_twiddle_tab_stride1_arr[stage]];


        q31_t * pBase = pSrc;
#pragma nounroll
        for (int i = 0; i < iter; i++)
        {
            q31_t    *inA = pBase;
            q31_t    *inB = inA + n2 * CMPLX_DIM;
            q31_t    *inC = inB + n2 * CMPLX_DIM;
            q31_t    *inD = inC + n2 * CMPLX_DIM;

#ifdef USE_ASM
            register unsigned count  __asm("lr") = (n2 / 2);
            __asm volatile(

                 RAD4_BFLY_STG_FX_FWD_MVE(32)

                : [pSrcA0] "+r"(inA), [pSrcA1] "+r"(inB),
                  [pSrcA2] "+r"(inC), [pSrcA3] "+r"(inD),
                  [count] "+r" (count),
                  [t0] "+r"(p_rearranged_twiddle_tab_stride2),
                  [t1] "+r"(p_rearranged_twiddle_tab_stride1),
                  [t2] "+r"(p_rearranged_twiddle_tab_stride3)
                :
                : "q0", "q1", "q2", "q3",
                  "q4", "q5", "q6", "q7",
                  "memory");
#else

            blkCnt = n2 / 2;
            /*
             * load 2 x q31 complex pair
             */
            vecA = vldrwq_s32(inA);
            vecC = vldrwq_s32(inC);
            while (blkCnt > 0U)
            {
                vecB = vldrwq_s32(inB);
                vecD = vldrwq_s32(inD);

                vecSum0 = vhaddq(vecA, vecC);
                vecDiff0 = vhsubq(vecA, vecC);

                vecSum1 = vhaddq(vecB, vecD);
                vecDiff1 = vhsubq(vecB, vecD);
                /*
                 * [ 1 1 1 1 ] * [ A B C D ]' .* 1
                 */
                vecTmp0 = vhaddq(vecSum0, vecSum1);
                vst1q(inA, vecTmp0);
                inA += 4;
                /*
                 * [ 1 -1 1 -1 ] * [ A B C D ]'
                 */
                vecTmp0 = vhsubq(vecSum0, vecSum1);
                /*
                 * [ 1 -1 1 -1 ] * [ A B C D ]'.* W2
                 */
                vecW = vld1q(p_rearranged_twiddle_tab_stride2);
                p_rearranged_twiddle_tab_stride2 += 4;
                vecTmp1 = MVE_CMPLX_MULT_FX_AxB(vecW, vecTmp0, q31x4_t);

                vst1q(inB, vecTmp1);
                inB += 4;
                /*
                 * [ 1 -i -1 +i ] * [ A B C D ]'
                 */
                vecTmp0 = MVE_CMPLX_SUB_FX_A_ixB(vecDiff0, vecDiff1);
                /*
                 * [ 1 -i -1 +i ] * [ A B C D ]'.* W1
                 */
                vecW = vld1q(p_rearranged_twiddle_tab_stride1);
                p_rearranged_twiddle_tab_stride1 += 4;
                vecTmp1 = MVE_CMPLX_MULT_FX_AxB(vecW, vecTmp0, q31x4_t);
                vst1q(inC, vecTmp1);
                inC += 4;
                /*
                 * [ 1 +i -1 -i ] * [ A B C D ]'
                 */
                vecTmp0 = MVE_CMPLX_ADD_FX_A_ixB(vecDiff0, vecDiff1);
                /*
                 * [ 1 +i -1 -i ] * [ A B C D ]'.* W3
                 */
                vecW = vld1q(p_rearranged_twiddle_tab_stride3);
                p_rearranged_twiddle_tab_stride3 += 4;
                vecTmp1 = MVE_CMPLX_MULT_FX_AxB(vecW, vecTmp0, q31x4_t);
                vst1q(inD, vecTmp1);
                inD += 4;

                vecA = vldrwq_s32(inA);
                vecC = vldrwq_s32(inC);

                blkCnt--;
            }
#endif

            p_rearranged_twiddle_tab_stride1 -= n2 * CMPLX_DIM;
            p_rearranged_twiddle_tab_stride2 -= n2 * CMPLX_DIM;
            p_rearranged_twiddle_tab_stride3 -= n2 * CMPLX_DIM;

            pBase +=  CMPLX_DIM * n1;
        }

        n1 = n2;
        n2 >>= 2u;
        iter = iter << 2;
        stage++;
    }

    /*
     * End of 1st stages process
     * data is in 11.21(q21) format for the 1024 point as there are 3 middle stages
     * data is in 9.23(q23) format for the 256 point as there are 2 middle stages
     * data is in 7.25(q25) format for the 64 point as there are 1 middle stage
     * data is in 5.27(q27) format for the 16 point as there are no middle stages
     */

    /*
     * start of Last stage process
     */
#ifdef USE_ASM

    __asm volatile(
        ".set incr,                 32/4                        \n"
        ".p2align 2                                             \n"
        "   wls                     lr, %[count], 1f            \n"
        "   vldrw.32                q6, [%[strides]]            \n"
        "   vadd.u32                q6, q6, %[pout]             \n"
        "   vldrd.64                q7, [q6, #64]!              \n"
        /* Read xc (real), yc(imag) input */
        "   vldrd.64                q5, [q6, #(2*incr)]         \n"
        "2:                                                     \n"
        /* R(xa + xc), I(ya + yc) */
        "   vhadd.s32               q0, q7, q5                  \n"
        /* Read xb (real), yb(imag) input. */
        "   vldrd.64                q4, [q6, #(1*incr)]         \n"
        /* R(xa - xc), I(ya - yc) */
        "   vhsub.s32               q1, q7, q5                  \n"
        /* Read xc (real), yc(imag) input */
        "   vldrd.64                q5, [q6, #(3*incr)]         \n"
        /* R(xb + xd), I(yb + yd) */
        "   vhadd.s32               q2, q4, q5                  \n"
        "   vldrd.64                q7, [q6, #64]!              \n"
        /* R(xb - xd), I(yb - yd) */
        "   vhsub.s32               q3, q4, q5                  \n"
        "   vldrd.64                q5, [q6, #(2*incr)]         \n"
        /* xa' = (xa + xc) + (xb + xd) */
        /* ya' = (ya + yc) + (yb + yd) */
        "   vhadd.s32               q4, q0, q2                  \n"
        "   vstrd.64                q4, [q6, #-64]              \n"
        /* xa' = (xa + xc) - (xb + xd) */
        /* ya' = (ya + yc) - (yb + yd) */
        "   vhsub.s32               q4, q0, q2                  \n"
        "   vstrd.64                q4, [q6, #(-64 + incr)]     \n"
        /* xb' = (xa - xc) + (yb - yd) */
        /* yb' = (ya - yc) - (xb - xd) */
        "   vhcadd.s32              q4, q1, q3, #270            \n"
        "   vstrd.64                q4, [q6, #(-64 + 2*incr)]   \n"
        /* xb' = (xa - xc) - (yb - yd) */
        /* yb' = (ya - yc) + (xb - xd) */
        "   vhcadd.s32              q4, q1, q3, #90             \n"
        "   vstrd.64                q4, [q6, #(-64 + 3*incr)]   \n"
        "   le                      lr, 2b                      \n"
        "1:                                                     \n"

        : [pout] "+r"(pSrc)
        : [count] "r"(fftLen >> 3), [strides] "r"(strides)
       : "q0", "q1", "q2", "q3",
         "q4", "q5", "q6", "q7",
         "memory", "lr");
#else
    uint32x4_t vecScGathAddr = vld1q_u32((uint32_t*)strides);
    vecScGathAddr = vecScGathAddr + (uint32_t) pSrc;

    /*
     * load scheduling
     */
    vecA = vldrwq_gather_base_wb_s32(&vecScGathAddr, 64);
    vecC = vldrwq_gather_base_s32(vecScGathAddr, 16);

    blkCnt = (fftLen >> 3);
    while (blkCnt > 0U)
    {
        vecSum0 = vhaddq(vecA, vecC);
        vecDiff0 = vhsubq(vecA, vecC);

        vecB = vldrwq_gather_base_s32(vecScGathAddr, 8);
        vecD = vldrwq_gather_base_s32(vecScGathAddr, 24);

        vecSum1 = vhaddq(vecB, vecD);
        vecDiff1 = vhsubq(vecB, vecD);
        /*
         * pre-load for next iteration
         */
        vecA = vldrwq_gather_base_wb_s32(&vecScGathAddr, 64);
        vecC = vldrwq_gather_base_s32(vecScGathAddr, 16);

        vecTmp0 = vhaddq(vecSum0, vecSum1);
        vstrwq_scatter_base_s32(vecScGathAddr, -64, vecTmp0);

        vecTmp0 = vhsubq(vecSum0, vecSum1);
        vstrwq_scatter_base_s32(vecScGathAddr, -64 + 8, vecTmp0);

        vecTmp0 = MVE_CMPLX_SUB_FX_A_ixB(vecDiff0, vecDiff1);
        vstrwq_scatter_base_s32(vecScGathAddr, -64 + 16, vecTmp0);

        vecTmp0 = MVE_CMPLX_ADD_FX_A_ixB(vecDiff0, vecDiff1);
        vstrwq_scatter_base_s32(vecScGathAddr, -64 + 24, vecTmp0);

        blkCnt--;
    }
#endif
    /*
     * output is in 11.21(q21) format for the 1024 point
     * output is in 9.23(q23) format for the 256 point
     * output is in 7.25(q25) format for the 64 point
     * output is in 5.27(q27) format for the 16 point
     */
}


static void arm_cfft_radix4by2_q31_mve(const arm_cfft_instance_q31 *S, q31_t *pSrc, uint32_t fftLen)
{
    uint32_t     n2;
    q31_t       *pIn0;
    q31_t       *pIn1;
    const q31_t *pCoef = S->pTwiddle;
#ifndef USE_ASM
    uint32_t     blkCnt;
    q31x4_t    vecIn0, vecIn1, vecSum, vecDiff;
    q31x4_t    vecCmplxTmp, vecTw;
#endif

    n2 = fftLen >> 1;
    pIn0 = pSrc;
    pIn1 = pSrc + fftLen;

#ifdef USE_ASM
    __asm volatile(
        RAD2_BFLY_FX32_MVE(conj)
        : [pIn0] "+r"(pIn0), [pIn1] "+r"(pIn1), [pCoef] "+r"(pCoef)
        : [count] "r"(n2 / 2)
        : "lr", "memory",
          "q0", "q1", "q2", "q3", "q4", "q7");
#else

    blkCnt = n2 / 2;
    while (blkCnt > 0U)
    {
        vecIn0 = vld1q_s32(pIn0);
        vecIn1 = vld1q_s32(pIn1);

        vecIn0 = vecIn0 >> 1;
        vecIn1 = vecIn1 >> 1;
        vecSum = vhaddq(vecIn0, vecIn1);
        vst1q(pIn0, vecSum);
        pIn0 += 4;

        vecTw = vld1q_s32(pCoef);
        pCoef += 4;
        vecDiff = vhsubq(vecIn0, vecIn1);

        vecCmplxTmp = MVE_CMPLX_MULT_FX_AxConjB(vecDiff, vecTw, q31x4_t);
        vst1q(pIn1, vecCmplxTmp);
        pIn1 += 4;

        blkCnt--;
    }
#endif

   _arm_radix4_butterfly_q31_mve(S, pSrc, n2);

   _arm_radix4_butterfly_q31_mve(S, pSrc + fftLen, n2);

    pIn0 = pSrc;

#ifdef USE_ASM
    __asm volatile(
        ".p2align 2                                                \n"
        "   wlstp.32             lr, %[count], 1f                  \n"
        "2:                                                        \n"
        "   vldrw.32             q0, [%[p]]                        \n"
        "   vshl.s32             q0, q0, #1                        \n"
        "   vstrw.32             q0, [%[p]],16                     \n"
        "   letp                 lr, 2b                            \n"
        "1:                                                        \n"
        : [p] "+r"(pIn0)
        : [count] "r"(fftLen << 1)
        : "q0", "lr", "memory");
#else
    blkCnt = (fftLen << 1);
    while (blkCnt > 0U)
    {
        mve_pred16_t p0 = vctp32q(blkCnt);
        vecIn0 = vld1q_z(pIn0, p0);
        vecIn0 = vecIn0 << 1;
        vst1q_p(pIn0, vecIn0, p0);
        pIn0 += 4;
        blkCnt -= 4;
    }
#endif
}

static void _arm_radix4_butterfly_inverse_q31_mve(
    const arm_cfft_instance_q31 *S,
    q31_t   *pSrc,
    uint32_t fftLen)
{
#ifndef USE_ASM
    q31x4_t vecTmp0, vecTmp1;
    q31x4_t vecSum0, vecDiff0, vecSum1, vecDiff1;
    q31x4_t vecA, vecB, vecC, vecD;
    q31x4_t vecW;
    uint32_t  blkCnt;
#endif
    uint32_t  n1, n2;
    uint32_t  stage = 0;
    int32_t  iter = 1;
    static const int32_t strides[4] = {
        (0 - 16) * (int32_t)sizeof(q31_t *), (1 - 16) * (int32_t)sizeof(q31_t *),
        (8 - 16) * (int32_t)sizeof(q31_t *), (9 - 16) * (int32_t)sizeof(q31_t *)
    };

    /*
     * Process first stages
     * Each stage in middle stages provides two down scaling of the input
     */
    n2 = fftLen;
    n1 = n2;
    n2 >>= 2u;

    for (int k = fftLen / 4u; k > 1; k >>= 2u)
    {
        q31_t const *p_rearranged_twiddle_tab_stride2 =
            &S->rearranged_twiddle_stride2[
            S->rearranged_twiddle_tab_stride2_arr[stage]];
        q31_t const *p_rearranged_twiddle_tab_stride3 = &S->rearranged_twiddle_stride3[
            S->rearranged_twiddle_tab_stride3_arr[stage]];
        q31_t const *p_rearranged_twiddle_tab_stride1 =
            &S->rearranged_twiddle_stride1[
            S->rearranged_twiddle_tab_stride1_arr[stage]];

        q31_t * pBase = pSrc;
        for (int i = 0; i < iter; i++)
        {
            q31_t    *inA = pBase;
            q31_t    *inB = inA + n2 * CMPLX_DIM;
            q31_t    *inC = inB + n2 * CMPLX_DIM;
            q31_t    *inD = inC + n2 * CMPLX_DIM;

#ifdef USE_ASM
            register unsigned count  __asm("lr") = (n2 / 2);

            __asm volatile(

                RAD4_BFLY_STG_FX_BKWD_MVE(32)

                : [pSrcA0] "+r"(inA), [pSrcA1] "+r"(inB),
                  [pSrcA2] "+r"(inC), [pSrcA3] "+r"(inD),
                  [count] "+r" (count),
                  [t0] "+r"(p_rearranged_twiddle_tab_stride2) , [t1] "+r"(p_rearranged_twiddle_tab_stride1), [t2] "+r"(p_rearranged_twiddle_tab_stride3)
                :
                : "q0", "q1", "q2", "q3",
                  "q4", "q5", "q6", "q7",
                  "memory");
#else
            blkCnt = n2 / 2;
            /*
             * load 2 x q31 complex pair
             */
            vecA = vldrwq_s32(inA);
            vecC = vldrwq_s32(inC);
            while (blkCnt > 0U)
            {
                vecB = vldrwq_s32(inB);
                vecD = vldrwq_s32(inD);

                vecSum0 = vhaddq(vecA, vecC);
                vecDiff0 = vhsubq(vecA, vecC);

                vecSum1 = vhaddq(vecB, vecD);
                vecDiff1 = vhsubq(vecB, vecD);
                /*
                 * [ 1 1 1 1 ] * [ A B C D ]' .* 1
                 */
                vecTmp0 = vhaddq(vecSum0, vecSum1);
                vst1q(inA, vecTmp0);
                inA += 4;
                /*
                 * [ 1 -1 1 -1 ] * [ A B C D ]'
                 */
                vecTmp0 = vhsubq(vecSum0, vecSum1);
                /*
                 * [ 1 -1 1 -1 ] * [ A B C D ]'.* W2
                 */
                vecW = vld1q(p_rearranged_twiddle_tab_stride2);
                p_rearranged_twiddle_tab_stride2 += 4;
                vecTmp1 = MVE_CMPLX_MULT_FX_AxConjB(vecTmp0, vecW, q31x4_t);

                vst1q(inB, vecTmp1);
                inB += 4;
                /*
                 * [ 1 -i -1 +i ] * [ A B C D ]'
                 */
                vecTmp0 = MVE_CMPLX_ADD_FX_A_ixB(vecDiff0, vecDiff1);
                /*
                 * [ 1 -i -1 +i ] * [ A B C D ]'.* W1
                 */
                vecW = vld1q(p_rearranged_twiddle_tab_stride1);
                p_rearranged_twiddle_tab_stride1 += 4;
                vecTmp1 = MVE_CMPLX_MULT_FX_AxConjB(vecTmp0, vecW, q31x4_t);
                vst1q(inC, vecTmp1);
                inC += 4;
                /*
                 * [ 1 +i -1 -i ] * [ A B C D ]'
                 */
                vecTmp0 = MVE_CMPLX_SUB_FX_A_ixB(vecDiff0, vecDiff1);
                /*
                 * [ 1 +i -1 -i ] * [ A B C D ]'.* W3
                 */
                vecW = vld1q(p_rearranged_twiddle_tab_stride3);
                p_rearranged_twiddle_tab_stride3 += 4;
                vecTmp1 = MVE_CMPLX_MULT_FX_AxConjB(vecTmp0, vecW, q31x4_t);
                vst1q(inD, vecTmp1);
                inD += 4;

                vecA = vldrwq_s32(inA);
                vecC = vldrwq_s32(inC);

                blkCnt--;
            }
#endif
            p_rearranged_twiddle_tab_stride1 -= n2 * CMPLX_DIM;
            p_rearranged_twiddle_tab_stride2 -= n2 * CMPLX_DIM;
            p_rearranged_twiddle_tab_stride3 -= n2 * CMPLX_DIM;
            pBase +=  CMPLX_DIM * n1;
        }
        n1 = n2;
        n2 >>= 2u;
        iter = iter << 2;
        stage++;
    }

    /*
     * End of 1st stages process
     * data is in 11.21(q21) format for the 1024 point as there are 3 middle stages
     * data is in 9.23(q23) format for the 256 point as there are 2 middle stages
     * data is in 7.25(q25) format for the 64 point as there are 1 middle stage
     * data is in 5.27(q27) format for the 16 point as there are no middle stages
     */

    /*
     * start of Last stage process
     */
#ifdef USE_ASM
    q31_t *in0 = pSrc;
    __asm volatile(
        ".set incr,                 32/4                        \n"
        ".p2align 2                                             \n"
        "   wls                     lr, %[count], 1f            \n"
        "   vldrw.32                q6, [%[strides]]            \n"
        "   vadd.u32                q6, q6, %[pout]             \n"
        "   vldrd.64                q7, [q6, #64]!              \n"
        /* Read xc (real), yc(imag) input */
        "   vldrd.64                q5, [q6, #(2*incr)]         \n"
        "2:                                                     \n"
        /* R(xa + xc), I(ya + yc) */
        "   vhadd.s32               q0, q7, q5                  \n"
        /* Read xb (real), yb(imag) input. */
        "   vldrd.64                q4, [q6, #(1*incr)]         \n"
        /* R(xa - xc), I(ya - yc) */
        "   vhsub.s32               q1, q7, q5                  \n"
        /* Read xc (real), yc(imag) input */
        "   vldrd.64                q5, [q6, #(3*incr)]         \n"
        /* R(xb + xd), I(yb + yd) */
        "   vhadd.s32               q2, q4, q5                  \n"
        "   vldrd.64                q7, [q6, #64]!              \n"
        /* R(xb - xd), I(yb - yd) */
        "   vhsub.s32               q3, q4, q5                  \n"
        "   vldrd.64                q5, [q6, #(2*incr)]         \n"
        /* xa' = (xa + xc) + (xb + xd) */
        /* ya' = (ya + yc) + (yb + yd) */
        "   vhadd.s32               q4, q0, q2                  \n"
        "   vstrd.64                q4, [q6, #-64]              \n"
        /* xa' = (xa + xc) - (xb + xd) */
        /* ya' = (ya + yc) - (yb + yd) */
        "   vhsub.s32               q4, q0, q2                  \n"
        "   vstrd.64                q4, [q6, #(-64 + incr)]     \n"
        /* xb' = (xa - xc) + (yb - yd) */
        /* yb' = (ya - yc) - (xb - xd) */
        "   vhcadd.s32              q4, q1, q3, #90             \n"
        "   vstrd.64                q4, [q6, #(-64 + 2*incr)]   \n"
        /* xb' = (xa - xc) - (yb - yd) */
        /* yb' = (ya - yc) + (xb - xd) */
        "   vhcadd.s32              q4, q1, q3, #270            \n"
        "   vstrd.64                q4, [q6, #(-64 + 3*incr)]   \n"
        "   le                      lr, 2b                      \n"
        "1:                                                     \n"

        : [pout] "+r"(in0)
        : [count] "r"(fftLen >> 3), [strides] "r"(strides)
       : "q0", "q1", "q2", "q3",
         "q4", "q5", "q6", "q7",
         "memory", "lr");
#else
    uint32x4_t vecScGathAddr = vld1q_u32((uint32_t*)strides);
    vecScGathAddr = vecScGathAddr + (uint32_t) pSrc;

    /*
     * load scheduling
     */
    vecA = vldrwq_gather_base_wb_s32(&vecScGathAddr, 64);
    vecC = vldrwq_gather_base_s32(vecScGathAddr, 16);

    blkCnt = (fftLen >> 3);
    while (blkCnt > 0U)
    {
        vecSum0 = vhaddq(vecA, vecC);
        vecDiff0 = vhsubq(vecA, vecC);

        vecB = vldrwq_gather_base_s32(vecScGathAddr, 8);
        vecD = vldrwq_gather_base_s32(vecScGathAddr, 24);

        vecSum1 = vhaddq(vecB, vecD);
        vecDiff1 = vhsubq(vecB, vecD);
        /*
         * pre-load for next iteration
         */
        vecA = vldrwq_gather_base_wb_s32(&vecScGathAddr, 64);
        vecC = vldrwq_gather_base_s32(vecScGathAddr, 16);

        vecTmp0 = vhaddq(vecSum0, vecSum1);
        vstrwq_scatter_base_s32(vecScGathAddr, -64, vecTmp0);

        vecTmp0 = vhsubq(vecSum0, vecSum1);
        vstrwq_scatter_base_s32(vecScGathAddr, -64 + 8, vecTmp0);

        vecTmp0 = MVE_CMPLX_ADD_FX_A_ixB(vecDiff0, vecDiff1);
        vstrwq_scatter_base_s32(vecScGathAddr, -64 + 16, vecTmp0);

        vecTmp0 = MVE_CMPLX_SUB_FX_A_ixB(vecDiff0, vecDiff1);
        vstrwq_scatter_base_s32(vecScGathAddr, -64 + 24, vecTmp0);

        blkCnt--;
    }
#endif
    /*
     * output is in 11.21(q21) format for the 1024 point
     * output is in 9.23(q23) format for the 256 point
     * output is in 7.25(q25) format for the 64 point
     * output is in 5.27(q27) format for the 16 point
     */
}

static void arm_cfft_radix4by2_inverse_q31_mve(const arm_cfft_instance_q31 *S, q31_t *pSrc, uint32_t fftLen)
{
    uint32_t     n2;
    q31_t       *pIn0;
    q31_t       *pIn1;
    const q31_t *pCoef = S->pTwiddle;
#ifndef USE_ASM
    uint32_t     blkCnt;
    q31x4_t     vecIn0, vecIn1, vecSum, vecDiff;
    q31x4_t     vecCmplxTmp, vecTw;
#endif

    n2 = fftLen >> 1;

    pIn0 = pSrc;
    pIn1 = pSrc + fftLen;

#ifdef USE_ASM
    __asm volatile(
          RAD2_BFLY_FX32_MVE(/*no conjugate*/)
        : [pIn0] "+r"(pIn0), [pIn1] "+r"(pIn1), [pCoef] "+r"(pCoef)
        : [count] "r"(n2 / 2)
        : "lr", "memory",
          "q0", "q1", "q2", "q3", "q4", "q7");

#else
    blkCnt = n2 / 2;
    while (blkCnt > 0U)
    {
        vecIn0 = vld1q_s32(pIn0);
        vecIn1 = vld1q_s32(pIn1);

        vecIn0 = vecIn0 >> 1;
        vecIn1 = vecIn1 >> 1;
        vecSum = vhaddq(vecIn0, vecIn1);
        vst1q(pIn0, vecSum);
        pIn0 += 4;

        //vecTw = (q31x4_t) vldrdq_gather_offset_s64(pCoef, vecOffs);
        vecTw = vld1q_s32(pCoef);
        pCoef += 4;
        vecDiff = vhsubq(vecIn0, vecIn1);

        vecCmplxTmp = MVE_CMPLX_MULT_FX_AxB(vecDiff, vecTw, q31x4_t);
        vst1q(pIn1, vecCmplxTmp);
        pIn1 += 4;

        blkCnt--;
    }
#endif
    _arm_radix4_butterfly_inverse_q31_mve(S, pSrc, n2);

    _arm_radix4_butterfly_inverse_q31_mve(S, pSrc + fftLen, n2);

    pIn0 = pSrc;

#ifdef USE_ASM
    __asm volatile(
        ".p2align 2                                                \n"
        "   wlstp.32             lr, %[count], 1f                  \n"
        "2:                                                        \n"
        "   vldrw.32             q0, [%[p]]                        \n"
        "   vshl.s32             q0, q0, #1                        \n"
        "   vstrw.32             q0, [%[p]],16                     \n"
        "   letp                 lr, 2b                            \n"
        "1:                                                        \n"
        : [p] "+r"(pIn0)
        : [count] "r"(fftLen << 1)
        : "q0", "lr", "memory");
#else

    blkCnt = (fftLen << 1);
    while (blkCnt > 0U)
    {
        mve_pred16_t p0 = vctp32q(blkCnt);
        vecIn0 = vld1q_z(pIn0, p0);
        vecIn0 = vecIn0 << 1;
        vst1q_p(pIn0, vecIn0, p0);
        pIn0 += 4;
        blkCnt -= 4;
    }
#endif
}

/**
  @ingroup groupTransforms
 */

/**
  @addtogroup ComplexFFT
  @{
 */

/**
  @brief         Processing function for the Q31 complex FFT.
  @param[in]     S               points to an instance of the fixed-point CFFT structure
  @param[in,out] p1              points to the complex data buffer of size <code>2*fftLen</code>. Processing occurs in-place
  @param[in]     ifftFlag       flag that selects transform direction
                   - value = 0: forward transform
                   - value = 1: inverse transform
  @param[in]     bitReverseFlag flag that enables / disables bit reversal of output
                   - value = 0: disables bit reversal of output
                   - value = 1: enables bit reversal of output
  @return        none
 */
void arm_cfft_q31_mve(
  const arm_cfft_instance_q31 * S,
        q31_t * pSrc,
        uint8_t ifftFlag,
        uint8_t bitReverseFlag)
{
        uint32_t fftLen = S->fftLen;

        if (ifftFlag == 1U) {

            switch (fftLen) {
            case 16:
            case 64:
            case 256:
            case 1024:
            case 4096:
                _arm_radix4_butterfly_inverse_q31_mve(S, pSrc, fftLen);
                break;

            case 32:
            case 128:
            case 512:
            case 2048:
                arm_cfft_radix4by2_inverse_q31_mve(S, pSrc, fftLen);
                break;
            }
        } else {
            switch (fftLen) {
            case 16:
            case 64:
            case 256:
            case 1024:
            case 4096:
                _arm_radix4_butterfly_q31_mve(S, pSrc, fftLen);
                break;

            case 32:
            case 128:
            case 512:
            case 2048:
                arm_cfft_radix4by2_q31_mve(S, pSrc, fftLen);
                break;
            }
        }


        if (bitReverseFlag)
        {
#ifdef USE_ASM
            arm_bitreversal_q31_inpl_mve_asm((uint32_t*)pSrc, S->bitRevLength, S->pBitRevTable);
#else
            arm_bitreversal_32_inpl_mve((uint32_t*)pSrc, S->bitRevLength, S->pBitRevTable);
#endif
        }
}

#endif
