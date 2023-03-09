/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_cfft_q15.c
 * Description:  Combined Radix Decimation in Frequency CFFT fixed point processing function
 *               using Helium assembly kernels
 *               This version allows boosting CFFT Q15 performance when using compilers having suboptimal
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
#define RAD2_BFLY_FX16_MVE(conj)                                       \
    ".p2align 2                                                            \n"\
    "   wls                 lr, %[count], 1f                               \n"\
    "   vldrh.16            q0, [%[pIn0]]                                  \n"\
    /* low overhead loop start */                                             \
    "2:                                                                    \n"\
    "   vshr.s16            q0, q0, #1                                     \n"\
    "   vldrh.16            q1, [%[pIn1]]                                  \n"\
    "   vshr.s16            q1, q1, #1                                     \n"\
    /* a0,a1 */                                                               \
    "   vhadd.s16           q2, q0, q1                                     \n"\
    /* twiddle */                                                             \
    "   vldrh.16            q7, [%[pCoef]], #16                            \n"\
    /* xt,yt */                                                               \
    "   vhsub.s16           q3, q0, q1                                     \n"\
    "   vldrh.16            q0, [%[pIn0], #16]                             \n"\
    cmplx_fx_mul_r_##conj(16, q4, q3, q7)                                     \
    "   vstrh.16            q2, [%[pIn0]], 16                              \n"\
    cmplx_fx_mul_i_##conj(16, q4, q3, q7)                                     \
    "   vstrh.16            q4, [%[pIn1]], 16                              \n"\
    /* low overhead loop end */                                               \
    "   le                  lr, 2b                                         \n"\
    "1:                                                                    \n"




static void arm_bitreversal_q15_inpl_mve_asm(
        uint16_t *pSrc,
  const uint16_t bitRevLen,
  const uint16_t *pBitRevTab)
{
     __asm volatile(
        "   vmov.i16        q0, #0x1                     \n"

        /* load scheduling */
        /* read 16-bit src/dst bit-reversal pairs */
        "   vldrh.u16       q5, [%[pBitRevTab]], #16     \n"
        /* expand source indexes (odd) */
        "   vmullb.u16      q2, q5, q0                   \n"
        /* convert into complex pair index */
        "   vshr.u32        q2, q2, #3                   \n"
        /* expand destination indexes (even) */
        "   vmullt.u16      q1, q5, q0                   \n"
        /* convert into complex pair index */
        "   vshr.u32        q1, q1, #3                   \n"

        /* core loop unrolled */
        ".p2align 2                                      \n"
        "   wls             lr, %[bitRevLen], 1f         \n"
        /* low overhead loop start */
        "2:                                              \n"
        "   vldrh.u16       q5, [%[pBitRevTab]], #16     \n"
        "   vmullb.u16      q6, q5, q0                   \n"
        "   vldrw.u32       q3, [%[pSrc], q2, uxtw #2]   \n"
        "   vshr.u32        q6, q6, #3                   \n"
        "   vldrw.u32       q4, [%[pSrc], q1, uxtw #2]   \n"
        "   vmullt.u16      q7, q5, q0                   \n"
        "   vstrw.32        q4, [%[pSrc], q2, uxtw #2]   \n"
        "   vshr.u32        q7, q7, #3                   \n"
        "   vstrw.32        q3, [%[pSrc], q1, uxtw #2]   \n"

        "   vldrh.u16       q5, [%[pBitRevTab]], #16     \n"
        "   vmullb.u16      q2, q5, q0                   \n"
        "   vldrw.u32       q3, [%[pSrc], q6, uxtw #2]   \n"
        "   vshr.u32        q2, q2, #3                   \n"
        "   vldrw.u32       q4, [%[pSrc], q7, uxtw #2]   \n"
        "   vmullt.u16      q1, q5, q0                   \n"
        "   vstrw.32        q4, [%[pSrc], q6, uxtw #2]   \n"
        "   vshr.u32        q1, q1, #3                   \n"
        "   vstrw.32        q3, [%[pSrc], q7, uxtw #2]   \n"
        /* low overhead loop end */
        "   le              lr, 2b                       \n"
        "1:                                              \n"

        /* tail */
        "   wlstp.16        lr, %[bitRevLenRem], 1f      \n"
        "2:                                              \n"
        "   vldrw.u32       q3, [%[pSrc], q2, uxtw #2]   \n"
        "   vldrw.u32       q4, [%[pSrc], q1, uxtw #2]   \n"
        "   vstrw.32        q4, [%[pSrc], q2, uxtw #2]   \n"
        "   vstrw.32        q3, [%[pSrc], q1, uxtw #2]   \n"

        "   vldrh.u16       q1, [%[pBitRevTab]], #16     \n"
        "   vmullb.u16      q2, q1, q0                   \n"
        "   vshr.u32        q2, q2, #3                   \n"
        "   vmullt.u16      q1, q1, q0                   \n"
        "   vshr.u32        q1, q1, #3                   \n"
        "   letp            lr, 2b                       \n"
        "1:                                              \n"

        : [pBitRevTab] "+r" (pBitRevTab)
        : [bitRevLen] "r" (bitRevLen/16), [bitRevLenRem] "r" (bitRevLen&0xf), [pSrc] "r" (pSrc)
        : "q0", "q1", "q2", "q3",
          "q4", "q5", "q6", "q7",
          "memory");
}


#endif

static void _arm_radix4_butterfly_q15_mve(
    const arm_cfft_instance_q15 * S,
    q15_t   *pSrc,
    uint32_t fftLen)
{
#ifndef USE_ASM
    q15x8_t vecTmp0, vecTmp1;
    q15x8_t vecSum0, vecDiff0, vecSum1, vecDiff1;
    q15x8_t vecA, vecB, vecC, vecD;
    q15x8_t vecW;
    uint32_t  blkCnt;
#endif
    uint32_t  n1, n2;
    uint32_t  stage = 0;
    int32_t  iter = 1;
    static const int32_t strides[4] = {
        (0 - 16) * (int32_t)sizeof(q15_t *), (4 - 16) * (int32_t)sizeof(q15_t *),
        (8 - 16) * (int32_t)sizeof(q15_t *), (12 - 16) * (int32_t)sizeof(q15_t *)
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
        q15_t const *p_rearranged_twiddle_tab_stride2 =
                &S->rearranged_twiddle_stride2[
                S->rearranged_twiddle_tab_stride2_arr[stage]];
        q15_t const *p_rearranged_twiddle_tab_stride3 = &S->rearranged_twiddle_stride3[
                S->rearranged_twiddle_tab_stride3_arr[stage]];
        q15_t const *p_rearranged_twiddle_tab_stride1 =
                &S->rearranged_twiddle_stride1[
                S->rearranged_twiddle_tab_stride1_arr[stage]];


        q15_t * pBase = pSrc;
#pragma nounroll
        for (int i = 0; i < iter; i++)
        {
            q15_t    *inA = pBase;
            q15_t    *inB = inA + n2 * CMPLX_DIM;
            q15_t    *inC = inB + n2 * CMPLX_DIM;
            q15_t    *inD = inC + n2 * CMPLX_DIM;

#ifdef USE_ASM

            register unsigned count  __asm("lr") = (n2 / 4);
            __asm volatile(

                 RAD4_BFLY_STG_FX_FWD_MVE(16)

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

            blkCnt = n2 / 4;
            /*
             * load 4 x q15 complex pair
             */
            vecA = vldrhq_s16(inA);
            vecC = vldrhq_s16(inC);
            while (blkCnt > 0U)
            {
                vecB = vldrhq_s16(inB);
                vecD = vldrhq_s16(inD);

                vecSum0 = vhaddq(vecA, vecC);
                vecDiff0 = vhsubq(vecA, vecC);

                vecSum1 = vhaddq(vecB, vecD);
                vecDiff1 = vhsubq(vecB, vecD);
                /*
                 * [ 1 1 1 1 ] * [ A B C D ]' .* 1
                 */
                vecTmp0 = vhaddq(vecSum0, vecSum1);
                vst1q(inA, vecTmp0);
                inA += 8;
                /*
                 * [ 1 -1 1 -1 ] * [ A B C D ]'
                 */
                vecTmp0 = vhsubq(vecSum0, vecSum1);
                /*
                 * [ 1 -1 1 -1 ] * [ A B C D ]'.* W2
                 */
                vecW = vld1q(p_rearranged_twiddle_tab_stride2);
                p_rearranged_twiddle_tab_stride2 += 8;
                vecTmp1 = MVE_CMPLX_MULT_FX_AxB(vecW, vecTmp0, q15x8_t);

                vst1q(inB, vecTmp1);
                inB += 8;
                /*
                 * [ 1 -i -1 +i ] * [ A B C D ]'
                 */
                vecTmp0 = MVE_CMPLX_SUB_FX_A_ixB(vecDiff0, vecDiff1);
                /*
                 * [ 1 -i -1 +i ] * [ A B C D ]'.* W1
                 */
                vecW = vld1q(p_rearranged_twiddle_tab_stride1);
                p_rearranged_twiddle_tab_stride1 += 8;
                vecTmp1 = MVE_CMPLX_MULT_FX_AxB(vecW, vecTmp0, q15x8_t);
                vst1q(inC, vecTmp1);
                inC += 8;

                /*
                 * [ 1 +i -1 -i ] * [ A B C D ]'
                 */
                vecTmp0 = MVE_CMPLX_ADD_FX_A_ixB(vecDiff0, vecDiff1);
                /*
                 * [ 1 +i -1 -i ] * [ A B C D ]'.* W3
                 */
                vecW = vld1q(p_rearranged_twiddle_tab_stride3);
                p_rearranged_twiddle_tab_stride3 += 8;
                vecTmp1 = MVE_CMPLX_MULT_FX_AxB(vecW, vecTmp0, q15x8_t);
                vst1q(inD, vecTmp1);
                inD += 8;

                vecA = vldrhq_s16(inA);
                vecC = vldrhq_s16(inC);

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
     * start of Last stage process
     */
#ifdef USE_ASM

    __asm volatile(
        ".set incr,                 16/4                        \n"
        ".p2align 2                                             \n"
        "   wls                     lr, %[count], 1f            \n"
        "   vldrw.32                q6, [%[strides]]            \n"
        "   vadd.u32                q6, q6, %[pout]             \n"
        "   vldrw.32                q7, [q6, #64]!              \n"
        /* Read xc (real), yc(imag) input */
        "   vldrw.32                q5, [q6, #(2*incr)]         \n"
        "2:                                                     \n"
        /* R(xa + xc), I(ya + yc) */
        "   vhadd.s16               q0, q7, q5                  \n"
        /* Read xb (real), yb(imag) input. */
        "   vldrw.32                q4, [q6, #(1*incr)]         \n"
        /* R(xa - xc), I(ya - yc) */
        "   vhsub.s16               q1, q7, q5                  \n"
        /* Read xc (real), yc(imag) input */
        "   vldrw.32                q5, [q6, #(3*incr)]         \n"
        /* R(xb + xd), I(yb + yd) */
        "   vhadd.s16               q2, q4, q5                  \n"
        "   vldrw.32                q7, [q6, #64]!              \n"
        /* R(xb - xd), I(yb - yd) */
        "   vhsub.s16               q3, q4, q5                  \n"
        "   vldrw.32                q5, [q6, #(2*incr)]         \n"
        /* xa' = (xa + xc) + (xb + xd) */
        /* ya' = (ya + yc) + (yb + yd) */
        "   vhadd.s16               q4, q0, q2                  \n"
        "   vstrw.32                q4, [q6, #-64]              \n"
        /* xa' = (xa + xc) - (xb + xd) */
        /* ya' = (ya + yc) - (yb + yd) */
        "   vhsub.s16               q4, q0, q2                  \n"
        "   vstrw.32                q4, [q6, #(-64 + incr)]     \n"
        /* xb' = (xa - xc) + (yb - yd) */
        /* yb' = (ya - yc) - (xb - xd) */
        "   vhcadd.s16              q4, q1, q3, #270            \n"
        "   vstrw.32                q4, [q6, #(-64 + 2*incr)]   \n"
        /* xb' = (xa - xc) - (yb - yd) */
        /* yb' = (ya - yc) + (xb - xd) */
        "   vhcadd.s16              q4, q1, q3, #90             \n"
        "   vstrw.32                q4, [q6, #(-64 + 3*incr)]   \n"
        "   le                      lr, 2b                      \n"
        "1:                                                     \n"

        : [pout] "+r"(pSrc)
        : [count] "r"(fftLen >> 4), [strides] "r"(strides)
       : "q0", "q1", "q2", "q3",
         "q4", "q5", "q6", "q7",
         "memory", "lr");
#else
    uint32x4_t vecScGathAddr = vld1q_u32 ((uint32_t*)strides);
    vecScGathAddr = vecScGathAddr + (uint32_t) pSrc;

    /*
     * load scheduling
     */
    vecA = (q15x8_t) vldrwq_gather_base_wb_s32(&vecScGathAddr, 64);
    vecC = (q15x8_t) vldrwq_gather_base_s32(vecScGathAddr, 8);

    blkCnt = (fftLen >> 4);
    while (blkCnt > 0U)
    {
        vecSum0 = vhaddq(vecA, vecC);
        vecDiff0 = vhsubq(vecA, vecC);

        vecB = (q15x8_t) vldrwq_gather_base_s32(vecScGathAddr, 4);
        vecD = (q15x8_t) vldrwq_gather_base_s32(vecScGathAddr, 12);

        vecSum1 = vhaddq(vecB, vecD);
        vecDiff1 = vhsubq(vecB, vecD);
        /*
         * pre-load for next iteration
         */
        vecA = (q15x8_t) vldrwq_gather_base_wb_s32(&vecScGathAddr, 64);
        vecC = (q15x8_t) vldrwq_gather_base_s32(vecScGathAddr, 8);

        vecTmp0 = vhaddq(vecSum0, vecSum1);
        vstrwq_scatter_base_s32(vecScGathAddr, -64, (int32x4_t) vecTmp0);

        vecTmp0 = vhsubq(vecSum0, vecSum1);
        vstrwq_scatter_base_s32(vecScGathAddr, -64 + 4, (int32x4_t) vecTmp0);

        vecTmp0 = MVE_CMPLX_SUB_FX_A_ixB(vecDiff0, vecDiff1);
        vstrwq_scatter_base_s32(vecScGathAddr, -64 + 8, (int32x4_t) vecTmp0);

        vecTmp0 = MVE_CMPLX_ADD_FX_A_ixB(vecDiff0, vecDiff1);
        vstrwq_scatter_base_s32(vecScGathAddr, -64 + 12, (int32x4_t) vecTmp0);

        blkCnt--;
    }
#endif
}


static void arm_cfft_radix4by2_q15_mve(const arm_cfft_instance_q15 *S, q15_t *pSrc, uint32_t fftLen)
{
    uint32_t n2;
    q15_t *pIn0;
    q15_t *pIn1;
    const q15_t *pCoef = S->pTwiddle;
#ifndef USE_ASM
    uint32_t     blkCnt;
    q15x8_t    vecIn0, vecIn1, vecSum, vecDiff;
    q15x8_t    vecCmplxTmp, vecTw;
#endif

    n2 = fftLen >> 1;

    pIn0 = pSrc;
    pIn1 = pSrc + fftLen;

#ifdef USE_ASM
    __asm volatile(
        RAD2_BFLY_FX16_MVE(conj)
        : [pIn0] "+r"(pIn0), [pIn1] "+r"(pIn1), [pCoef] "+r"(pCoef)
        : [count] "r"(n2 / 4)
        : "lr", "memory",
          "q0", "q1", "q2", "q3", "q4", "q7");
#else

    blkCnt = n2 / 4;

    while (blkCnt > 0U)
    {
        vecIn0 = *(q15x8_t *) pIn0;
        vecIn1 = *(q15x8_t *) pIn1;

        vecIn0 = vecIn0 >> 1;
        vecIn1 = vecIn1 >> 1;
        vecSum = vhaddq(vecIn0, vecIn1);
        vst1q(pIn0, vecSum);
        pIn0 += 8;

        vecTw = vld1q(pCoef);
        pCoef += 8;

        vecDiff = vhsubq(vecIn0, vecIn1);
        vecCmplxTmp = MVE_CMPLX_MULT_FX_AxConjB(vecDiff, vecTw, q15x8_t);
        vst1q(pIn1, vecCmplxTmp);
        pIn1 += 8;

        blkCnt--;
    }
#endif

    _arm_radix4_butterfly_q15_mve(S, pSrc, n2);

    _arm_radix4_butterfly_q15_mve(S, pSrc + fftLen, n2);


    pIn0 = pSrc;
#ifdef USE_ASM
    __asm volatile(
        ".p2align 2                                                \n"
        "   wlstp.16             lr, %[count], 1f                  \n"
        "2:                                                        \n"
        "   vldrh.16             q0, [%[p]]                        \n"
        "   vshl.s16             q0, q0, #1                        \n"
        "   vstrh.16             q0, [%[p]],16                     \n"
        "   letp                 lr, 2b                            \n"
        "1:                                                        \n"
        : [p] "+r"(pIn0)
        : [count] "r"(fftLen * CMPLX_DIM)
        : "q0", "lr", "memory");
#else

    blkCnt = (fftLen << 1);
    while (blkCnt > 0U)
    {
        mve_pred16_t p0 = vctp16q(blkCnt);
        vecIn0 = vld1q_z(pIn0, p0);
        vecIn0 = vecIn0 << 1;
        vst1q_p(pIn0, vecIn0, p0);
        pIn0 += 8;
        blkCnt-=8;
    }
#endif
}

static void _arm_radix4_butterfly_inverse_q15_mve(const arm_cfft_instance_q15 *S,q15_t *pSrc, uint32_t fftLen)
{
#ifndef USE_ASM
    q15x8_t vecTmp0, vecTmp1;
    q15x8_t vecSum0, vecDiff0, vecSum1, vecDiff1;
    q15x8_t vecA, vecB, vecC, vecD;
    q15x8_t vecW;
    uint32_t  blkCnt;
#endif
    uint32_t  n1, n2;
    uint32_t  stage = 0;
    int32_t  iter = 1;
    static const int32_t strides[4] = {
        (0 - 16) * (int32_t)sizeof(q15_t *), (4 - 16) * (int32_t)sizeof(q15_t *),
        (8 - 16) * (int32_t)sizeof(q15_t *), (12 - 16) * (int32_t)sizeof(q15_t *)
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
        q15_t const *p_rearranged_twiddle_tab_stride2 =
                &S->rearranged_twiddle_stride2[
                S->rearranged_twiddle_tab_stride2_arr[stage]];
        q15_t const *p_rearranged_twiddle_tab_stride3 = &S->rearranged_twiddle_stride3[
                S->rearranged_twiddle_tab_stride3_arr[stage]];
        q15_t const *p_rearranged_twiddle_tab_stride1 =
                &S->rearranged_twiddle_stride1[
                S->rearranged_twiddle_tab_stride1_arr[stage]];

        q15_t * pBase = pSrc;
        for (int i = 0; i < iter; i++)
        {
            q15_t    *inA = pBase;
            q15_t    *inB = inA + n2 * CMPLX_DIM;
            q15_t    *inC = inB + n2 * CMPLX_DIM;
            q15_t    *inD = inC + n2 * CMPLX_DIM;

#ifdef USE_ASM
            register unsigned count  __asm("lr") = (n2 / 4);
            __asm volatile(

                RAD4_BFLY_STG_FX_BKWD_MVE(16)

                : [pSrcA0] "+r"(inA), [pSrcA1] "+r"(inB),
                  [pSrcA2] "+r"(inC), [pSrcA3] "+r"(inD),
                  [count] "+r" (count),
                  [t0] "+r"(p_rearranged_twiddle_tab_stride2) , [t1] "+r"(p_rearranged_twiddle_tab_stride1), [t2] "+r"(p_rearranged_twiddle_tab_stride3)
                :
                : "q0", "q1", "q2", "q3",
                  "q4", "q5", "q6", "q7",
                  "memory");
#else
            blkCnt = n2 / 4;
            /*
             * load 4 x q15 complex pair
             */
            vecA = vldrhq_s16(inA);
            vecC = vldrhq_s16(inC);
            while (blkCnt > 0U)
            {
                vecB = vldrhq_s16(inB);
                vecD = vldrhq_s16(inD);

                vecSum0 = vhaddq(vecA, vecC);
                vecDiff0 = vhsubq(vecA, vecC);

                vecSum1 = vhaddq(vecB, vecD);
                vecDiff1 = vhsubq(vecB, vecD);
                /*
                 * [ 1 1 1 1 ] * [ A B C D ]' .* 1
                 */
                vecTmp0 = vhaddq(vecSum0, vecSum1);
                vst1q(inA, vecTmp0);
                inA += 8;
                /*
                 * [ 1 -1 1 -1 ] * [ A B C D ]'
                 */
                vecTmp0 = vhsubq(vecSum0, vecSum1);
                /*
                 * [ 1 -1 1 -1 ] * [ A B C D ]'.* W2
                 */
                vecW = vld1q(p_rearranged_twiddle_tab_stride2);
                p_rearranged_twiddle_tab_stride2 += 8;
                vecTmp1 = MVE_CMPLX_MULT_FX_AxConjB(vecTmp0, vecW, q15x8_t);

                vst1q(inB, vecTmp1);
                inB += 8;
                /*
                 * [ 1 -i -1 +i ] * [ A B C D ]'
                 */
                vecTmp0 = MVE_CMPLX_ADD_FX_A_ixB(vecDiff0, vecDiff1);
                /*
                 * [ 1 -i -1 +i ] * [ A B C D ]'.* W1
                 */
                vecW = vld1q(p_rearranged_twiddle_tab_stride1);
                p_rearranged_twiddle_tab_stride1 += 8;
                vecTmp1 = MVE_CMPLX_MULT_FX_AxConjB(vecTmp0, vecW, q15x8_t);
                vst1q(inC, vecTmp1);
                inC += 8;
                /*
                 * [ 1 +i -1 -i ] * [ A B C D ]'
                 */
                vecTmp0 = MVE_CMPLX_SUB_FX_A_ixB(vecDiff0, vecDiff1);
                /*
                 * [ 1 +i -1 -i ] * [ A B C D ]'.* W3
                 */
                vecW = vld1q(p_rearranged_twiddle_tab_stride3);
                p_rearranged_twiddle_tab_stride3 += 8;
                vecTmp1 = MVE_CMPLX_MULT_FX_AxConjB(vecTmp0, vecW, q15x8_t);
                vst1q(inD, vecTmp1);
                inD += 8;

                vecA = vldrhq_s16(inA);
                vecC = vldrhq_s16(inC);

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
     * start of Last stage process
     */
#ifdef USE_ASM
    q15_t *in0 = pSrc;
    __asm volatile(
        ".set incr,                 16/4                        \n"
        ".p2align 2                                             \n"
        "   wls                     lr, %[count], 1f            \n"
        "   vldrw.32                q6, [%[strides]]            \n"
        "   vadd.u32                q6, q6, %[pout]             \n"
        "   vldrw.32                q7, [q6, #64]!              \n"
        /* Read xc (real), yc(imag) input */
        "   vldrw.32                q5, [q6, #(2*incr)]         \n"
        "2:                                                     \n"
        /* R(xa + xc), I(ya + yc) */
        "   vhadd.s16               q0, q7, q5                  \n"
        /* Read xb (real), yb(imag) input. */
        "   vldrw.32                q4, [q6, #(1*incr)]         \n"
        /* R(xa - xc), I(ya - yc) */
        "   vhsub.s16               q1, q7, q5                  \n"
        /* Read xc (real), yc(imag) input */
        "   vldrw.32                q5, [q6, #(3*incr)]         \n"
        /* R(xb + xd), I(yb + yd) */
        "   vhadd.s16               q2, q4, q5                  \n"
        "   vldrw.32                q7, [q6, #64]!              \n"
        /* R(xb - xd), I(yb - yd) */
        "   vhsub.s16               q3, q4, q5                  \n"
        "   vldrw.32                q5, [q6, #(2*incr)]         \n"
        /* xa' = (xa + xc) + (xb + xd) */
        /* ya' = (ya + yc) + (yb + yd) */
        "   vhadd.s16               q4, q0, q2                  \n"
        "   vstrw.32                q4, [q6, #-64]              \n"
        /* xa' = (xa + xc) - (xb + xd) */
        /* ya' = (ya + yc) - (yb + yd) */
        "   vhsub.s16               q4, q0, q2                  \n"
        "   vstrw.32                q4, [q6, #(-64 + incr)]     \n"
        /* xb' = (xa - xc) + (yb - yd) */
        /* yb' = (ya - yc) - (xb - xd) */
        "   vhcadd.s16              q4, q1, q3, #90             \n"
        "   vstrw.32                q4, [q6, #(-64 + 2*incr)]   \n"
        /* xb' = (xa - xc) - (yb - yd) */
        /* yb' = (ya - yc) + (xb - xd) */
        "   vhcadd.s16              q4, q1, q3, #270            \n"
        "   vstrw.32                q4, [q6, #(-64 + 3*incr)]   \n"
        "   le                      lr, 2b                      \n"
        "1:                                                     \n"

        : [pout] "+r"(in0)
        : [count] "r"(fftLen >> 4), [strides] "r"(strides)
       : "q0", "q1", "q2", "q3",
         "q4", "q5", "q6", "q7",
         "memory", "lr");
#else
    uint32x4_t vecScGathAddr = vld1q_u32((uint32_t*)strides);
    vecScGathAddr = vecScGathAddr + (uint32_t) pSrc;

    /*
     * load scheduling
     */
    vecA = (q15x8_t) vldrwq_gather_base_wb_s32(&vecScGathAddr, 64);
    vecC = (q15x8_t) vldrwq_gather_base_s32(vecScGathAddr, 8);

    blkCnt = (fftLen >> 4);
    while (blkCnt > 0U)
    {
        vecSum0 = vhaddq(vecA, vecC);
        vecDiff0 = vhsubq(vecA, vecC);

        vecB = (q15x8_t) vldrwq_gather_base_s32(vecScGathAddr, 4);
        vecD = (q15x8_t) vldrwq_gather_base_s32(vecScGathAddr, 12);

        vecSum1 = vhaddq(vecB, vecD);
        vecDiff1 = vhsubq(vecB, vecD);
        /*
         * pre-load for next iteration
         */
        vecA = (q15x8_t) vldrwq_gather_base_wb_s32(&vecScGathAddr, 64);
        vecC = (q15x8_t) vldrwq_gather_base_s32(vecScGathAddr, 8);

        vecTmp0 = vhaddq(vecSum0, vecSum1);
        vstrwq_scatter_base_s32(vecScGathAddr, -64, (int32x4_t) vecTmp0);

        vecTmp0 = vhsubq(vecSum0, vecSum1);
        vstrwq_scatter_base_s32(vecScGathAddr, -64 + 4, (int32x4_t) vecTmp0);

        vecTmp0 = MVE_CMPLX_ADD_FX_A_ixB(vecDiff0, vecDiff1);
        vstrwq_scatter_base_s32(vecScGathAddr, -64 + 8, (int32x4_t) vecTmp0);

        vecTmp0 = MVE_CMPLX_SUB_FX_A_ixB(vecDiff0, vecDiff1);
        vstrwq_scatter_base_s32(vecScGathAddr, -64 + 12, (int32x4_t) vecTmp0);

        blkCnt--;
    }
#endif
}

static void arm_cfft_radix4by2_inverse_q15_mve(const arm_cfft_instance_q15 *S, q15_t *pSrc, uint32_t fftLen)
{
    uint32_t n2;
    q15_t *pIn0;
    q15_t *pIn1;
    const q15_t *pCoef = S->pTwiddle;
#ifndef USE_ASM
    uint32_t     blkCnt;
    q15x8_t    vecIn0, vecIn1, vecSum, vecDiff;
    q15x8_t    vecCmplxTmp, vecTw;
#endif
    n2 = fftLen >> 1;

    pIn0 = pSrc;
    pIn1 = pSrc + fftLen;

#ifdef USE_ASM
    __asm volatile(
          RAD2_BFLY_FX16_MVE(/*no conjugate*/)
        : [pIn0] "+r"(pIn0), [pIn1] "+r"(pIn1), [pCoef] "+r"(pCoef)
        : [count] "r"(n2 / 4)
        : "lr", "memory",
          "q0", "q1", "q2", "q3", "q4", "q7");

#else

    blkCnt = n2 / 4;

    while (blkCnt > 0U)
    {
        vecIn0 = *(q15x8_t *) pIn0;
        vecIn1 = *(q15x8_t *) pIn1;

        vecIn0 = vecIn0 >> 1;
        vecIn1 = vecIn1 >> 1;
        vecSum = vhaddq(vecIn0, vecIn1);
        vst1q(pIn0, vecSum);
        pIn0 += 8;

        vecTw = vld1q(pCoef);
        pCoef += 8;

        vecDiff = vhsubq(vecIn0, vecIn1);
        vecCmplxTmp = vqdmlsdhq(vuninitializedq_s16() , vecDiff, vecTw);
        vecCmplxTmp = vqdmladhxq(vecCmplxTmp, vecDiff, vecTw);
        vst1q(pIn1, vecCmplxTmp);
        pIn1 += 8;

        blkCnt--;
    }
#endif

    _arm_radix4_butterfly_inverse_q15_mve(S, pSrc, n2);

    _arm_radix4_butterfly_inverse_q15_mve(S, pSrc + fftLen, n2);

    pIn0 = pSrc;

#ifdef USE_ASM
    __asm volatile(
        ".p2align 2                                                \n"
        "   wlstp.16             lr, %[count], 1f                  \n"
        "2:                                                        \n"
        "   vldrh.16             q0, [%[p]]                        \n"
        "   vshl.s16             q0, q0, #1                        \n"
        "   vstrh.16             q0, [%[p]],16                     \n"
        "   letp                 lr, 2b                            \n"
        "1:                                                        \n"
        : [p] "+r"(pIn0)
        : [count] "r"(fftLen << 1)
        : "q0", "lr", "memory");
#else
    blkCnt = (fftLen << 1);
    while (blkCnt > 0U)
    {
        mve_pred16_t p0 = vctp16q(blkCnt);
        vecIn0 = vld1q_z(pIn0, p0);
        vecIn0 = vecIn0 << 1;
        vst1q_p(pIn0, vecIn0, p0);
        pIn0 += 8;
        blkCnt -=8;
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
  @brief         Processing function for Q15 complex FFT.
  @param[in]     S               points to an instance of Q15 CFFT structure
  @param[in,out] p1              points to the complex data buffer of size <code>2*fftLen</code>. Processing occurs in-place
  @param[in]     ifftFlag       flag that selects transform direction
                   - value = 0: forward transform
                   - value = 1: inverse transform
  @param[in]     bitReverseFlag flag that enables / disables bit reversal of output
                   - value = 0: disables bit reversal of output
                   - value = 1: enables bit reversal of output
  @return        none
 */
void arm_cfft_q15_mve(
  const arm_cfft_instance_q15 * S,
        q15_t * pSrc,
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
                _arm_radix4_butterfly_inverse_q15_mve(S, pSrc, fftLen);
                break;

            case 32:
            case 128:
            case 512:
            case 2048:
                arm_cfft_radix4by2_inverse_q15_mve(S, pSrc, fftLen);
                break;
            }
        } else {
            switch (fftLen) {
            case 16:
            case 64:
            case 256:
            case 1024:
            case 4096:
                _arm_radix4_butterfly_q15_mve(S, pSrc, fftLen);
                break;

            case 32:
            case 128:
            case 512:
            case 2048:
                arm_cfft_radix4by2_q15_mve(S, pSrc, fftLen);
                break;
            }
        }


        if (bitReverseFlag)
        {
#ifdef USE_ASM
            arm_bitreversal_q15_inpl_mve_asm((uint16_t*)pSrc, S->bitRevLength, S->pBitRevTable);
#else
            arm_bitreversal_16_inpl_mve((uint16_t*)pSrc, S->bitRevLength, S->pBitRevTable);
#endif
        }
}

#endif
