/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_cfft_f32.c
 * Description:  Combined Radix Decimation in Frequency CFFT Floating point processing function
 *               using Helium assembly kernels
 *               This version allows boosting CFFT F32 performance when using compilers having suboptimal
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
#include "arm_common_tables.h"

#if defined(ARM_MATH_MVEF) && !defined(ARM_MATH_AUTOVECTORIZE)

#include "arm_helium_utils.h"
#include "arm_vec_fft.h"
#include "arm_mve_tables.h"


#ifdef USE_ASM

#include "../arm_cfft_utils.h"


static void arm_bitreversal_f32_inpl_mve_asm(
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
        : [bitRevLen] "r" (bitRevLen/8), [bitRevLenRem] "r" (bitRevLen&7), [pSrc] "r" (pSrc)
        : "q0", "q1", "q2", "q3",
          "q4", "q5", "q6", "q7",
          "lr", "memory"
        );
}
#endif

static float32_t arm_inverse_fft_length_f32(uint16_t fftLen)
{
  float32_t retValue=1.0f;

  switch (fftLen)
  {

  case 4096U:
    retValue = 0.000244140625f;
    break;

  case 2048U:
    retValue = 0.00048828125f;
    break;

  case 1024U:
    retValue = 0.0009765625f;
    break;

  case 512U:
    retValue = 0.001953125;
    break;

  case 256U:
    retValue = 0.00390625f;
    break;

  case 128U:
    retValue = 0.0078125;
    break;

  case 64U:
    retValue = 0.015625f;
    break;

  case 32U:
    retValue = 0.03125;
    break;

  case 16U:
    retValue = 0.0625f;
    break;


  default:
    break;
  }
  return(retValue);
}



static void _arm_radix4_butterfly_f32_mve(const arm_cfft_instance_f32 * S,float32_t * pSrc, uint32_t fftLen)
{
#ifndef USE_ASM
    f32x4_t   vecTmp0, vecTmp1;
    f32x4_t   vecSum0, vecDiff0, vecSum1, vecDiff1;
    f32x4_t   vecA, vecB, vecC, vecD;
    f32x4_t   vecW;
    uint32_t  blkCnt;
#endif
    uint32_t  n1, n2;
    uint32_t  stage = 0;
    int32_t   iter = 1;
    static const int32_t strides[4] = {
        (0 - 16) * (int32_t)sizeof(q31_t *),
        (1 - 16) * (int32_t)sizeof(q31_t *),
        (8 - 16) * (int32_t)sizeof(q31_t *),
        (9 - 16) * (int32_t)sizeof(q31_t *)
    };

    n2 = fftLen;
    n1 = n2;
    n2 >>= 2u;
    for (int k = fftLen / 4u; k > 1; k >>= 2)
    {
        float32_t const     *p_rearranged_twiddle_tab_stride1 =
                            &S->rearranged_twiddle_stride1[
                            S->rearranged_twiddle_tab_stride1_arr[stage]];
        float32_t const     *p_rearranged_twiddle_tab_stride2 =
                            &S->rearranged_twiddle_stride2[
                            S->rearranged_twiddle_tab_stride2_arr[stage]];
        float32_t const     *p_rearranged_twiddle_tab_stride3 =
                            &S->rearranged_twiddle_stride3[
                            S->rearranged_twiddle_tab_stride3_arr[stage]];

        float32_t * pBase = pSrc;
#pragma nounroll
        for (int i = 0; i < iter; i++)
        {
            float32_t    *inA = pBase;
            float32_t    *inB = inA + n2 * CMPLX_DIM;
            float32_t    *inC = inB + n2 * CMPLX_DIM;
            float32_t    *inD = inC + n2 * CMPLX_DIM;

#ifdef USE_ASM
            register unsigned count  __asm("lr") = (n2 / 2);
            __asm volatile(

                RAD4_BFLY_STG_FLT_FWD_MVE(32)

                : [pSrcA0] "+r"(inA)
                  , [pSrcA1] "+r"(inB)
                  , [pSrcA2] "+r"(inC)
                  , [pSrcA3] "+r"(inD)
                  , [t0] "+r"(p_rearranged_twiddle_tab_stride2)
                  , [t1] "+r"(p_rearranged_twiddle_tab_stride1)
                  , [t2] "+r"(p_rearranged_twiddle_tab_stride3)
                  , [count] "+r"(count)
                  :
                : "memory",  "q0", "q1", "q2", "q3",
                  "q4", "q5", "q6", "q7");

#else
            blkCnt = n2 / 2;
            /*
             * load 2 f32 complex pair
             */
            vecA = vldrwq_f32(inA);
            vecC = vldrwq_f32(inC);
            while (blkCnt > 0U)
            {
                vecB = vldrwq_f32(inB);
                vecD = vldrwq_f32(inD);

                vecSum0 = vecA + vecC;  /* vecSum0 = vaddq(vecA, vecC) */
                vecDiff0 = vecA - vecC; /* vecSum0 = vsubq(vecA, vecC) */

                vecSum1 = vecB + vecD;
                vecDiff1 = vecB - vecD;
                /*
                 * [ 1 1 1 1 ] * [ A B C D ]' .* 1
                 */
                vecTmp0 = vecSum0 + vecSum1;
                vst1q(inA, vecTmp0);
                inA += 4;

                /*
                 * [ 1 -1 1 -1 ] * [ A B C D ]'
                 */
                vecTmp0 = vecSum0 - vecSum1;
                /*
                 * [ 1 -1 1 -1 ] * [ A B C D ]'.* W2
                 */
                vecW = vld1q(p_rearranged_twiddle_tab_stride2);
                p_rearranged_twiddle_tab_stride2 += 4;
                vecTmp1 = MVE_CMPLX_MULT_FLT_Conj_AxB(vecW, vecTmp0);
                vst1q(inB, vecTmp1);
                inB += 4;

                /*
                 * [ 1 -i -1 +i ] * [ A B C D ]'
                 */
                vecTmp0 = MVE_CMPLX_SUB_A_ixB(vecDiff0, vecDiff1);
                /*
                 * [ 1 -i -1 +i ] * [ A B C D ]'.* W1
                 */
                vecW = vld1q(p_rearranged_twiddle_tab_stride1);
                p_rearranged_twiddle_tab_stride1 +=4;
                vecTmp1 = MVE_CMPLX_MULT_FLT_Conj_AxB(vecW, vecTmp0);
                vst1q(inC, vecTmp1);
                inC += 4;

                /*
                 * [ 1 +i -1 -i ] * [ A B C D ]'
                 */
                vecTmp0 = MVE_CMPLX_ADD_A_ixB(vecDiff0, vecDiff1);
                /*
                 * [ 1 +i -1 -i ] * [ A B C D ]'.* W3
                 */
                vecW = vld1q(p_rearranged_twiddle_tab_stride3);
                p_rearranged_twiddle_tab_stride3 += 4;
                vecTmp1 = MVE_CMPLX_MULT_FLT_Conj_AxB(vecW, vecTmp0);
                vst1q(inD, vecTmp1);
                inD += 4;

                vecA = vldrwq_f32(inA);
                vecC = vldrwq_f32(inC);

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
        ".set incr,                 32/4                        \n"
        ".p2align 2                                             \n"
        "   wls                     lr, %[count], 1f            \n"
        "   vldrw.32                q6, [%[strides]]            \n"
        "   vadd.u32                q6, q6, %[pout]             \n"
        "   vldrw.32                q7, [q6, #64]!              \n"
        /* Read xc (real), yc(imag) input */
        "   vldrw.32                q5, [q6, #(2*incr)]         \n"
        /* low overhead loop start */
        "2:                                                     \n"
        /* = R(xa + xc), I(ya + yc) */
        "   vadd.f32                q0, q7, q5                  \n"
        /* Read xb (real), yb(imag) input. */
        "   vldrw.32                q4, [q6, #(1*incr)]         \n"
        /* = R(xa - xc), I(ya - yc) */
        "   vsub.f32                q1, q7, q5                  \n"
        /* Read xc (real), yc(imag) input */
        "   vldrw.32                q5, [q6, #(3*incr)]         \n"
        /* = R(xb + xd), I(yb + yd) */
        "   vadd.f32                q2, q4, q5                  \n"
        "   vldrw.32                q7, [q6, #64]!              \n"
        /* = R(xb - xd), I(yb - yd) */
        "   vsub.f32                q3, q4, q5                  \n"
        "   vldrw.32                q5, [q6, #(2*incr)]         \n"
        /* xa' = (xa + xc) + (xb + xd) */
        /* ya' = (ya + yc) + (yb + yd) */
        "   vadd.f32                q4, q0, q2                  \n"
        "   vstrw.32                q4, [q6, #-64]              \n"
        /* xa' = (xa + xc) - (xb + xd) */
        /* ya' = (ya + yc) - (yb + yd) */
        "   vsub.f32                q4, q0, q2                  \n"
        "   vstrw.32                q4, [q6, #(-64 + incr)]     \n"
        /* xb' = (xa - xc) + (yb - yd) */
        /* yb' = (ya - yc) - (xb - xd) */
        "   vcadd.f32               q4, q1, q3, #270            \n"
        "   vstrw.32                q4, [q6, #(-64 + 2*incr)]   \n"
        /* xb' = (xa - xc) - (yb - yd) */
        /* yb' = (ya - yc) + (xb - xd) */
        "   vcadd.f32               q4, q1, q3, #90             \n"
        "   vstrw.32                q4, [q6, #(-64 + 3*incr)]   \n"
        "   le                      lr, 2b                      \n"
        /* low overhead loop end */
        "1:                                                     \n"
        : [pout] "+r"(pSrc)
        : [count] "r"(fftLen >> 3), [strides] "r"(strides)
        : "q0", "q1", "q2", "q3",
          "q4", "q5", "q6", "q7",
          "memory", "lr");
#else
    uint32x4_t vecScGathAddr = vld1q_u32((uint32_t*)strides);
    vecScGathAddr = vecScGathAddr + (uint32_t) pSrc;

    /* load scheduling */
    vecA = vldrwq_gather_base_wb_f32(&vecScGathAddr, 64);
    vecC = vldrwq_gather_base_f32(vecScGathAddr, 16);

    blkCnt = (fftLen >> 3);
    while (blkCnt > 0U)
    {
        vecSum0 = vecA + vecC;  /* vecSum0 = vaddq(vecA, vecC) */
        vecDiff0 = vecA - vecC; /* vecSum0 = vsubq(vecA, vecC) */

        vecB = vldrwq_gather_base_f32(vecScGathAddr, 8);
        vecD = vldrwq_gather_base_f32(vecScGathAddr, 24);

        vecSum1 = vecB + vecD;
        vecDiff1 = vecB - vecD;

        /* pre-load for next iteration */
        vecA = vldrwq_gather_base_wb_f32(&vecScGathAddr, 64);
        vecC = vldrwq_gather_base_f32(vecScGathAddr, 16);

        vecTmp0 = vecSum0 + vecSum1;
        vstrwq_scatter_base_f32(vecScGathAddr, -64, vecTmp0);

        vecTmp0 = vecSum0 - vecSum1;
        vstrwq_scatter_base_f32(vecScGathAddr, -64 + 8, vecTmp0);

        vecTmp0 = MVE_CMPLX_SUB_A_ixB(vecDiff0, vecDiff1);
        vstrwq_scatter_base_f32(vecScGathAddr, -64 + 16, vecTmp0);

        vecTmp0 = MVE_CMPLX_ADD_A_ixB(vecDiff0, vecDiff1);
        vstrwq_scatter_base_f32(vecScGathAddr, -64 + 24, vecTmp0);

        blkCnt--;
    }
#endif
    /*
     * End of last stage process
     */
}

static void arm_cfft_radix4by2_f32_mve(const arm_cfft_instance_f32 * S, float32_t *pSrc, uint32_t fftLen)
{
    float32_t const  *pCoef = S->pTwiddle;
    float32_t        *pIn0, *pIn1;
    uint32_t          n2;
#ifndef USE_ASM
    uint32_t          blkCnt;
    f32x4_t           vecIn0, vecIn1, vecSum, vecDiff;
    f32x4_t           vecCmplxTmp, vecTw;
#endif

    n2 = fftLen >> 1;
    pIn0 = pSrc;
    pIn1 = pSrc + fftLen;


#ifdef USE_ASM
    __asm volatile(RAD2_BFLY_FLT_MVE(32, conj)
        : [pIn0] "+r"(pIn0)
          , [pIn1] "+r"(pIn1)
          , [pCoef] "+r"(pCoef)
        : [count] "r"(n2 / 2)
        : "lr", "memory",
          "q0", "q1", "q2", "q3", "q4", "q7");
#else
    blkCnt = n2 / 2;
    while (blkCnt > 0U)
    {
        vecIn0 = *(f32x4_t *) pIn0;
        vecIn1 = *(f32x4_t *) pIn1;
        vecTw = vld1q(pCoef);
        pCoef += 4;

        vecSum = vecIn0 + vecIn1;
        vecDiff = vecIn0 - vecIn1;

        vecCmplxTmp = MVE_CMPLX_MULT_FLT_Conj_AxB(vecTw, vecDiff);

        vst1q(pIn0, vecSum);
        pIn0 += 4;
        vst1q(pIn1, vecCmplxTmp);
        pIn1 += 4;

        blkCnt--;
    }
#endif

    _arm_radix4_butterfly_f32_mve(S, pSrc, n2);

    _arm_radix4_butterfly_f32_mve(S, pSrc + fftLen, n2);

    pIn0 = pSrc;
}

static void _arm_radix4_butterfly_inverse_f32_mve(const arm_cfft_instance_f32 * S,float32_t * pSrc, uint32_t fftLen, float32_t onebyfftLen)
{
#ifndef USE_ASM
    f32x4_t   vecTmp0, vecTmp1;
    f32x4_t   vecSum0, vecDiff0, vecSum1, vecDiff1;
    f32x4_t   vecA, vecB, vecC, vecD;
    f32x4_t   vecW;
    uint32_t  blkCnt;
#endif
    uint32_t  n1, n2;
    uint32_t  stage = 0;
    int32_t  iter = 1;
    static const int32_t strides[4] = {
        (0 - 16) * (int32_t)sizeof(q31_t *),
        (1 - 16) * (int32_t)sizeof(q31_t *),
        (8 - 16) * (int32_t)sizeof(q31_t *),
        (9 - 16) * (int32_t)sizeof(q31_t *)
    };

    n2 = fftLen;
    n1 = n2;
    n2 >>= 2u;
    for (int k = fftLen / 4u; k > 1; k >>= 2u)
    {
        float32_t const *p_rearranged_twiddle_tab_stride2 =
                    &S->rearranged_twiddle_stride2[
                    S->rearranged_twiddle_tab_stride2_arr[stage]];
        float32_t const *p_rearranged_twiddle_tab_stride3 = &S->rearranged_twiddle_stride3[
                    S->rearranged_twiddle_tab_stride3_arr[stage]];
        float32_t const *p_rearranged_twiddle_tab_stride1 =
            &S->rearranged_twiddle_stride1[
            S->rearranged_twiddle_tab_stride1_arr[stage]];

        float32_t * pBase = pSrc;
        for (int i = 0; i < iter; i++)
        {
            float32_t    *inA = pBase;
            float32_t    *inB = inA + n2 * CMPLX_DIM;
            float32_t    *inC = inB + n2 * CMPLX_DIM;
            float32_t    *inD = inC + n2 * CMPLX_DIM;

#ifdef USE_ASM
            register unsigned count  __asm("lr") = (n2 / 2);
            __asm volatile(

                RAD4_BFLY_STG_FLT_BKWD_MVE(32)

                : [pSrcA0] "+r"(inA)
                  , [pSrcA1] "+r"(inB)
                  , [pSrcA2] "+r"(inC)
                  , [pSrcA3] "+r"(inD)
                  , [t0] "+r"(p_rearranged_twiddle_tab_stride2)
                  , [t1] "+r"(p_rearranged_twiddle_tab_stride1)
                  , [t2] "+r"(p_rearranged_twiddle_tab_stride3)
                  , [count] "+r"(count)
                  :
                : "memory",  "q0", "q1", "q2", "q3",
                  "q4", "q5", "q6", "q7");

#else
            blkCnt = n2 / 2;
            /*
             * load 2 f32 complex pair
             */
            vecA = vldrwq_f32(inA);
            vecC = vldrwq_f32(inC);
            while (blkCnt > 0U)
            {
                vecB = vldrwq_f32(inB);
                vecD = vldrwq_f32(inD);

                vecSum0 = vecA + vecC;  /* vecSum0 = vaddq(vecA, vecC) */
                vecDiff0 = vecA - vecC; /* vecSum0 = vsubq(vecA, vecC) */

                vecSum1 = vecB + vecD;
                vecDiff1 = vecB - vecD;
                /*
                 * [ 1 1 1 1 ] * [ A B C D ]' .* 1
                 */
                vecTmp0 = vecSum0 + vecSum1;
                vst1q(inA, vecTmp0);
                inA += 4;
                /*
                 * [ 1 -1 1 -1 ] * [ A B C D ]'
                 */
                vecTmp0 = vecSum0 - vecSum1;
                /*
                 * [ 1 -1 1 -1 ] * [ A B C D ]'.* W1
                 */
                vecW = vld1q(p_rearranged_twiddle_tab_stride2);
                p_rearranged_twiddle_tab_stride2 += 4;
                vecTmp1 = MVE_CMPLX_MULT_FLT_AxB(vecW, vecTmp0);
                vst1q(inB, vecTmp1);
                inB += 4;

                /*
                 * [ 1 -i -1 +i ] * [ A B C D ]'
                 */
                vecTmp0 = MVE_CMPLX_ADD_A_ixB(vecDiff0, vecDiff1);
                /*
                 * [ 1 -i -1 +i ] * [ A B C D ]'.* W2
                 */
                vecW = vld1q(p_rearranged_twiddle_tab_stride1);
                p_rearranged_twiddle_tab_stride1 += 4;
                vecTmp1 = MVE_CMPLX_MULT_FLT_AxB(vecW, vecTmp0);
                vst1q(inC, vecTmp1);
                inC += 4;

                /*
                 * [ 1 +i -1 -i ] * [ A B C D ]'
                 */
                vecTmp0 = MVE_CMPLX_SUB_A_ixB(vecDiff0, vecDiff1);
                /*
                 * [ 1 +i -1 -i ] * [ A B C D ]'.* W3
                 */
                vecW = vld1q(p_rearranged_twiddle_tab_stride3);
                p_rearranged_twiddle_tab_stride3 += 4;
                vecTmp1 = MVE_CMPLX_MULT_FLT_AxB(vecW, vecTmp0);
                vst1q(inD, vecTmp1);
                inD += 4;

                vecA = vldrwq_f32(inA);
                vecC = vldrwq_f32(inC);

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
    float32_t *in0 = pSrc;
    __asm volatile(
        ".set incr,                 32/4                        \n"
        ".p2align 2                                             \n"
        "   wls                     lr, %[count], 1f            \n"
        "   vldrw.32                q6, [%[strides]]            \n"
        "   vadd.u32                q6, q6, %[pout]             \n"
        "   vldrw.32                q7, [q6, #64]!              \n"
        /* Read xc (real), yc(imag) input */
        "   vldrw.32                q5, [q6, #(2*incr)]         \n"
        /* low overhead loop start */
        "2:                                                     \n"
        /* = R(xa + xc), I(ya + yc) */
        "   vadd.f32                q0, q7, q5                  \n"
        /* Read xb (real), yb(imag) input. */
        "   vldrw.32                q4, [q6, #(1*incr)]         \n"
        /* = R(xa - xc), I(ya - yc) */
        "   vsub.f32                q1, q7, q5                  \n"
        /* Read xc (real), yc(imag) input */
        "   vldrw.32                q5, [q6, #(3*incr)]         \n"
        /* = R(xb + xd), I(yb + yd) */
        "   vadd.f32                q2, q4, q5                  \n"
        "   vldrw.32                q7, [q6, #64]!              \n"
        /* = R(xb - xd), I(yb - yd) */
        "   vsub.f32                q3, q4, q5                  \n"
        "   vldrw.32                q5, [q6, #(2*incr)]         \n"
        /* xa' = (xa + xc) + (xb + xd) */
        /* ya' = (ya + yc) + (yb + yd) */
        "   vadd.f32                q4, q0, q2                  \n"
        "   vmul.f32                q4, q4, %[scale]            \n"
        "   vstrw.32                q4, [q6, #-64]              \n"
        /* xa' = (xa + xc) - (xb + xd) */
        /* ya' = (ya + yc) - (yb + yd) */
        "   vsub.f32                q4, q0, q2                  \n"
        "   vmul.f32                q4, q4, %[scale]            \n"
        "   vstrw.32                q4, [q6, #(-64 + incr)]     \n"
        /* xb' = (xa - xc) + (yb - yd) */
        /* yb' = (ya - yc) - (xb - xd) */
        "   vcadd.f32               q4, q1, q3, #90             \n"
        "   vmul.f32                q4, q4, %[scale]            \n"
        "   vstrw.32                q4, [q6, #(-64 + 2*incr)]   \n"
        /* xb' = (xa - xc) - (yb - yd) */
        /* yb' = (ya - yc) + (xb - xd) */
        "   vcadd.f32               q4, q1, q3, #270            \n"
        "   vmul.f32                q4, q4, %[scale]            \n"
        "   vstrw.32                q4, [q6, #(-64 + 3*incr)]   \n"
        /* low overhead loop end */
        "   le                      lr, 2b                      \n"
        "1:                                                     \n"
        : [pout] "+r"(in0)
        : [count] "r"(fftLen >> 3), [strides] "r"(strides),
          [scale] "r"(onebyfftLen)
        : "q0", "q1", "q2", "q3",
          "q4", "q5", "q6", "q7",
          "memory", "lr");
#else
    uint32x4_t vecScGathAddr = vld1q_u32 ((uint32_t*)strides);
    vecScGathAddr = vecScGathAddr + (uint32_t) pSrc;

    /*
     * load scheduling
     */
    vecA = vldrwq_gather_base_wb_f32(&vecScGathAddr, 64);
    vecC = vldrwq_gather_base_f32(vecScGathAddr, 16);

    blkCnt = (fftLen >> 3);
    while (blkCnt > 0U)
    {
        vecSum0 = vecA + vecC;  /* vecSum0 = vaddq(vecA, vecC) */
        vecDiff0 = vecA - vecC; /* vecSum0 = vsubq(vecA, vecC) */

        vecB = vldrwq_gather_base_f32(vecScGathAddr, 8);
        vecD = vldrwq_gather_base_f32(vecScGathAddr, 24);

        vecSum1 = vecB + vecD;
        vecDiff1 = vecB - vecD;

        vecA = vldrwq_gather_base_wb_f32(&vecScGathAddr, 64);
        vecC = vldrwq_gather_base_f32(vecScGathAddr, 16);

        vecTmp0 = vecSum0 + vecSum1;
        vecTmp0 = vecTmp0 * onebyfftLen;
        vstrwq_scatter_base_f32(vecScGathAddr, -64, vecTmp0);

        vecTmp0 = vecSum0 - vecSum1;
        vecTmp0 = vecTmp0 * onebyfftLen;
        vstrwq_scatter_base_f32(vecScGathAddr, -64 + 8, vecTmp0);

        vecTmp0 = MVE_CMPLX_ADD_A_ixB(vecDiff0, vecDiff1);
        vecTmp0 = vecTmp0 * onebyfftLen;
        vstrwq_scatter_base_f32(vecScGathAddr, -64 + 16, vecTmp0);

        vecTmp0 = MVE_CMPLX_SUB_A_ixB(vecDiff0, vecDiff1);
        vecTmp0 = vecTmp0 * onebyfftLen;
        vstrwq_scatter_base_f32(vecScGathAddr, -64 + 24, vecTmp0);

        blkCnt--;
    }
#endif
    /*
     * End of last stage process
     */
}

static void arm_cfft_radix4by2_inverse_f32_mve(const arm_cfft_instance_f32 * S,float32_t *pSrc, uint32_t fftLen)
{
    float32_t const  *pCoef = S->pTwiddle;
    float32_t        *pIn0, *pIn1;
    uint32_t          n2;
    float32_t         onebyfftLen = arm_inverse_fft_length_f32(fftLen);
#ifndef USE_ASM
    uint32_t          blkCnt;
    f32x4_t           vecIn0, vecIn1, vecSum, vecDiff;
    f32x4_t           vecCmplxTmp, vecTw;
#endif


    n2 = fftLen >> 1;
    pIn0 = pSrc;
    pIn1 = pSrc + fftLen;

#ifdef USE_ASM
    __asm volatile(RAD2_BFLY_FLT_MVE(32,/*no conjugate*/)
        : [pIn0] "+r"(pIn0), [pIn1] "+r"(pIn1), [pCoef] "+r"(pCoef)
        : [count] "r"(n2 / 2)
        : "lr", "memory",
          "q0", "q1", "q2", "q3", "q4", "q7");
#else
    blkCnt = n2 / 2;
    while (blkCnt > 0U)
    {
        vecIn0 = *(f32x4_t *) pIn0;
        vecIn1 = *(f32x4_t *) pIn1;
        vecTw = vld1q(pCoef);
        pCoef += 4;

        vecSum = vecIn0 + vecIn1;
        vecDiff = vecIn0 - vecIn1;

        vecCmplxTmp = MVE_CMPLX_MULT_FLT_AxB(vecTw, vecDiff);

        vst1q(pIn0, vecSum);
        pIn0 += 4;
        vst1q(pIn1, vecCmplxTmp);
        pIn1 += 4;

        blkCnt--;
    }
#endif
    _arm_radix4_butterfly_inverse_f32_mve(S, pSrc, n2, onebyfftLen);

    _arm_radix4_butterfly_inverse_f32_mve(S, pSrc + fftLen, n2, onebyfftLen);
}


/**
  @addtogroup ComplexFFT
  @{
 */

/**
  @brief         Processing function for the floating-point complex FFT.
  @param[in]     S              points to an instance of the floating-point CFFT structure
  @param[in,out] p1             points to the complex data buffer of size <code>2*fftLen</code>. Processing occurs in-place
  @param[in]     ifftFlag       flag that selects transform direction
                   - value = 0: forward transform
                   - value = 1: inverse transform
  @param[in]     bitReverseFlag flag that enables / disables bit reversal of output
                   - value = 0: disables bit reversal of output
                   - value = 1: enables bit reversal of output
  @return        none
 */


void arm_cfft_f32_mve(
  const arm_cfft_instance_f32 * S,
        float32_t * pSrc,
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
                _arm_radix4_butterfly_inverse_f32_mve(S, pSrc, fftLen, arm_inverse_fft_length_f32(S->fftLen));
                break;

            case 32:
            case 128:
            case 512:
            case 2048:
                arm_cfft_radix4by2_inverse_f32_mve(S, pSrc, fftLen);
                break;
            }
        } else {
            switch (fftLen) {
            case 16:
            case 64:
            case 256:
            case 1024:
            case 4096:
                _arm_radix4_butterfly_f32_mve(S, pSrc, fftLen);
                break;

            case 32:
            case 128:
            case 512:
            case 2048:
                arm_cfft_radix4by2_f32_mve(S, pSrc, fftLen);
                break;
            }
        }


        if (bitReverseFlag)
        {
#ifdef USE_ASM
            arm_bitreversal_f32_inpl_mve_asm((uint32_t*)pSrc, S->bitRevLength, S->pBitRevTable);
#else
            arm_bitreversal_32_inpl_mve((uint32_t*)pSrc, S->bitRevLength, S->pBitRevTable);
#endif
        }
}

#endif

/**
  @} end of ComplexFFT group
 */
