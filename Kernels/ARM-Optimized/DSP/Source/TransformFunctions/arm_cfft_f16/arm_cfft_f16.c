/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_cfft_f16.c
 * Description:  Combined Radix Decimation in Frequency CFFT floating point processing function
 *               using Helium assembly kernels
 *               This version allows boosting CFFT F16 performance when using compilers having suboptimal
 *               Helium intrinsic code generation.
 *
 * $Date:        Jan 2022
 * $Revision:    V1.0.1
 *
 * Target Processor: Cortex-M with Helium
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

#include "dsp/transform_functions_f16.h"
#include "arm_common_tables_f16.h"


#if defined(ARM_MATH_MVE_FLOAT16) && !defined(ARM_MATH_AUTOVECTORIZE)

#include "arm_helium_utils.h"
#include "arm_vec_fft.h"
#include "arm_mve_tables_f16.h"


#ifdef USE_ASM
/*
 * Floating point multiplication, 1st elt conjugated
 * Qd = Qn' * Qm
 */
#undef cmplx_fx_mul_r_
#undef cmplx_fx_mul_i_
#define cmplx_flt_mul_r_conj(qd, qn, qm) " vcmul.f16    " #qd "," #qn "," #qm ", #0 \n"
#define cmplx_flt_mul_i_conj(qd, qn, qm) " vcmla.f16    " #qd "," #qn "," #qm ", #270 \n"

/*
 * Floating point multiplication
 * Qd = Qn * Qm
 */
#undef cmplx_fx_mul_r_
#undef cmplx_fx_mul_i_
#define cmplx_flt_mul_r_(qd, qn, qm)    " vcmul.f16    " #qd "," #qn "," #qm ", #0 \n"
#define cmplx_flt_mul_i_(qd, qn, qm)    " vcmla.f16    " #qd "," #qn "," #qm ", #90 \n"


#define RAD2_BFLY_FLT16_MVE(conj)                              \
    ".p2align 2                                             \n"\
    "   wls                 lr, %[count], 1f                \n"\
    "2:                                                     \n"\
    /* low overhead loop start */                              \
    "   vldrh.16            q0, [%[pIn0]]                   \n"\
    "   vldrh.16            q1, [%[pIn1]]                   \n"\
    /* a0,a1 */                                                \
    "   vadd.f16            q2, q0, q1                      \n"\
    /* twiddle */                                              \
    "   vldrh.16            q7, [%[pCoef]], 16              \n"\
    /* xt,yt */                                                \
    "   vsub.f16            q3, q0, q1                      \n"\
    cmplx_flt_mul_r_##conj(q4, q7, q3)                         \
    "   vstrw.32            q2, [%[pIn0]], 16               \n"\
    cmplx_flt_mul_i_##conj(q4, q7, q3)                         \
    "   vstrw.32            q4, [%[pIn1]], 16               \n"\
    /* low overhead loop end */                                \
    "   le                  lr, 2b                          \n"\
    "1:                                                     \n"



static void arm_bitreversal_f16_inpl_mve_asm(
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

static float16_t arm_inverse_fft_length_f16(uint16_t fftLen)
{
  float16_t retValue=1.0;

  switch (fftLen)
  {

  case 4096U:
    retValue = (float16_t)0.000244140625f;
    break;

  case 2048U:
    retValue = (float16_t)0.00048828125f;
    break;

  case 1024U:
    retValue = (float16_t)0.0009765625f;
    break;

  case 512U:
    retValue = (float16_t)0.001953125f;
    break;

  case 256U:
    retValue = (float16_t)0.00390625f;
    break;

  case 128U:
    retValue = (float16_t)0.0078125f;
    break;

  case 64U:
    retValue = (float16_t)0.015625f;
    break;

  case 32U:
    retValue = (float16_t)0.03125f;
    break;

  case 16U:
    retValue = (float16_t)0.0625f;
    break;


  default:
    break;
  }
  return(retValue);
}


static void _arm_radix4_butterfly_f16_mve(const arm_cfft_instance_f16 * S,float16_t * pSrc, uint32_t fftLen)
{
#ifndef USE_ASM
    f16x8_t   vecTmp0, vecTmp1;
    f16x8_t   vecSum0, vecDiff0, vecSum1, vecDiff1;
    f16x8_t   vecA, vecB, vecC, vecD;
    f16x8_t   vecW;
    uint32_t  blkCnt;
#endif
    uint32_t  n1, n2;
    uint32_t  stage = 0;
    int32_t   iter = 1;
    static const int32_t strides[4] =
       { ( 0 - 16) * (int32_t)sizeof(float16_t *)
       , ( 4 - 16) * (int32_t)sizeof(float16_t *)
       , ( 8 - 16) * (int32_t)sizeof(float16_t *)
       , (12 - 16) * (int32_t)sizeof(float16_t *)};

    n2 = fftLen;
    n1 = n2;
    n2 >>= 2u;
    for (int k = fftLen / 4u; k > 1; k >>= 2)
    {
        float16_t const     *p_rearranged_twiddle_tab_stride1 =
                                &S->rearranged_twiddle_stride1[
                                S->rearranged_twiddle_tab_stride1_arr[stage]];
        float16_t const     *p_rearranged_twiddle_tab_stride2 =
                                &S->rearranged_twiddle_stride2[
                                S->rearranged_twiddle_tab_stride2_arr[stage]];
        float16_t const     *p_rearranged_twiddle_tab_stride3 =
                                &S->rearranged_twiddle_stride3[
                                S->rearranged_twiddle_tab_stride3_arr[stage]];

        float16_t * pBase = pSrc;
#pragma nounroll
        for (int i = 0; i < iter; i++)
        {
            float16_t    *inA = pBase;
            float16_t    *inB = inA + n2 * CMPLX_DIM;
            float16_t    *inC = inB + n2 * CMPLX_DIM;
            float16_t    *inD = inC + n2 * CMPLX_DIM;
            float16_t const *pW1 = p_rearranged_twiddle_tab_stride1;
            float16_t const *pW2 = p_rearranged_twiddle_tab_stride2;
            float16_t const *pW3 = p_rearranged_twiddle_tab_stride3;

#ifdef USE_ASM
            register unsigned count  __asm("lr") = (n2 / 4);
            __asm volatile(
                ".p2align 2                                             \n"
                "   wls                     lr, %[count], 1f            \n"
                "   vldrh.16                q1, [%[pSrcA0]]             \n"
                "   vldrh.16                q6, [%[pSrcA2]]             \n"
                /* low overhead loop start */
                "2:                                                     \n"
                /*  = R(xa + xc), I(ya + yc) */
                "   vadd.f16                q0, q1, q6                  \n"
                /*  q4 = yb2, xb2, yb1, xb1 */
                "   vldrh.16                q4, [%[pSrcA1]]             \n"
                /*  = R(xa - xc), I(ya - yc) */
                "   vsub.f16                q2, q1, q6                  \n"
                /*  q5 = yd2, xd2, yd1, xd1 */
                "   vldrh.16                q5, [%[pSrcA3]]             \n"
                /*  = R(xb + xd), I(yb + yd) */
                "   vadd.f16                q1, q4, q5                  \n"
                /*  = R(xb - xd), I(yb - yd) */
                "   vsub.f16                q3, q4, q5                  \n"
                /*  xa' = (xa + xc) + (xb + xd) */
                /*  ya' = (ya + yc) + (yb + yd) */
                "   vadd.f16                q4, q0, q1                  \n"
                "   vstrh.16                q4, [%[pSrcA0]], #16        \n"
                /*  xc' = (xa + xc) - (xb + xd) */
                /*  yc' = (ya + yc) - (yb + yd) */
                "   vsub.f16                q4, q0, q1                  \n"
                /*  load twiddle factors */
                "   vldrh.16                q5, [%[t0]], #16            \n"
                /*  xc' = (xa-xb+xc-xd)co2 + (ya-yb+yc-yd)(si2) */
                /*  yc' = (ya-yb+yc-yd)co2 - (xa-xb+xc-xd)(si2) */
                "   vcmul.f16               q0, q5, q4, #0              \n"
                "   vcmla.f16               q0, q5, q4, #270            \n"
                "   vstrh.16                q0, [%[pSrcA1]], #16        \n"
                /*  xb' = (xa - xc) + (yb - yd) */
                /*  yb' = (ya - yc) - (xb - xd) */
                "   vcadd.f16               q4, q2, q3, #270            \n"
                /*  load twiddle factors */
                "   vldrh.16                q5, [%[t1]], #16            \n"
                /*  xb' = (xa+yb-xc-yd)co1 + (ya-xb-yc+xd)(si1) */
                /*  yb' = (ya-xb-yc+xd)co1 - (xa+yb-xc-yd)(si1) */
                "   vcmul.f16               q0, q5, q4, #0              \n"
                "   vldrh.16                q1, [%[pSrcA0]]             \n"
                "   vcmla.f16               q0, q5, q4, #270            \n"
                "   vstrh.u16               q0, [%[pSrcA2]], #16        \n"
                /*  xd' = (xa - xc) - (yb - yd) */
                /*  yd' = (ya - yc) + (xb - xd) */
                "   vcadd.f16               q4, q2, q3, #90             \n"
                /*  load twiddle factors */
                "   vldrh.16                q5, [%[t2]], #16            \n"
                /*  xd' = (xa-yb-xc+yd)* co3 + (ya+xb-yc-xd)* (sa3) */
                /*  yd' = (ya+xb-yc-xd)* co3 - (xa-yb-xc+yd)* (sa3) */
                "   vcmul.f16               q0, q5, q4, #0              \n"
                "   vldrh.16                q6, [%[pSrcA2]]             \n"
                "   vcmla.f16               q0, q5, q4, #270            \n"
                "   vstrh.16                q0, [%[pSrcA3]], #16        \n"
                /* low overhead loop end */
                "   le                      lr, 2b                      \n"
                "1:                                                     \n"
                : [pSrcA0] "+r"(inA), [pSrcA1] "+r"(inB),
                  [pSrcA2] "+r"(inC), [pSrcA3] "+r"(inD),
                  [count] "+r" (count),
                  [t0] "+r"(pW2) , [t1] "+r"(pW1), [t2] "+r"(pW3)
                :
                : "q0", "q1", "q2", "q3",
                  "q4", "q5", "q6", "q7",
                  "memory");
#else
            blkCnt = n2 / 4;
            /*
             * load 2 f16 complex pair
             */
            vecA = vldrhq_f16(inA);
            vecC = vldrhq_f16(inC);
            while (blkCnt > 0U)
            {
                vecB = vldrhq_f16(inB);
                vecD = vldrhq_f16(inD);

                vecSum0 = vecA + vecC;  /* vecSum0 = vaddq(vecA, vecC) */
                vecDiff0 = vecA - vecC; /* vecSum0 = vsubq(vecA, vecC) */

                vecSum1 = vecB + vecD;
                vecDiff1 = vecB - vecD;
                /*
                 * [ 1 1 1 1 ] * [ A B C D ]' .* 1
                 */
                vecTmp0 = vecSum0 + vecSum1;
                vst1q(inA, vecTmp0);
                inA += 8;

                /*
                 * [ 1 -1 1 -1 ] * [ A B C D ]'
                 */
                vecTmp0 = vecSum0 - vecSum1;
                /*
                 * [ 1 -1 1 -1 ] * [ A B C D ]'.* W2
                 */
                vecW = vld1q(pW2);
                pW2 += 8;
                vecTmp1 = MVE_CMPLX_MULT_FLT_Conj_AxB(vecW, vecTmp0);
                vst1q(inB, vecTmp1);
                inB += 8;

                /*
                 * [ 1 -i -1 +i ] * [ A B C D ]'
                 */
                vecTmp0 = MVE_CMPLX_SUB_A_ixB(vecDiff0, vecDiff1);
                /*
                 * [ 1 -i -1 +i ] * [ A B C D ]'.* W1
                 */
                vecW = vld1q(pW1);
                pW1 +=8;
                vecTmp1 = MVE_CMPLX_MULT_FLT_Conj_AxB(vecW, vecTmp0);
                vst1q(inC, vecTmp1);
                inC += 8;

                /*
                 * [ 1 +i -1 -i ] * [ A B C D ]'
                 */
                vecTmp0 = MVE_CMPLX_ADD_A_ixB(vecDiff0, vecDiff1);
                /*
                 * [ 1 +i -1 -i ] * [ A B C D ]'.* W3
                 */
                vecW = vld1q(pW3);
                pW3 += 8;
                vecTmp1 = MVE_CMPLX_MULT_FLT_Conj_AxB(vecW, vecTmp0);
                vst1q(inD, vecTmp1);
                inD += 8;

                vecA = vldrhq_f16(inA);
                vecC = vldrhq_f16(inC);

                blkCnt--;
            }
#endif
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
        /* low overhead loop start */
        "2:                                                     \n"
        /* = R(xa + xc), I(ya + yc) */
        "   vadd.f16                q0, q7, q5                  \n"
        /* Read xb (real), yb(imag) input. */
        "   vldrw.32                q4, [q6, #(1*incr)]         \n"
        /* = R(xa - xc), I(ya - yc) */
        "   vsub.f16                q1, q7, q5                  \n"
        /* Read xc (real), yc(imag) input */
        "   vldrw.32                q5, [q6, #(3*incr)]         \n"
        /* = R(xb + xd), I(yb + yd) */
        "   vadd.f16                q2, q4, q5                  \n"
        "   vldrw.32                q7, [q6, #64]!              \n"
        /* = R(xb - xd), I(yb - yd) */
        "   vsub.f16                q3, q4, q5                  \n"
        "   vldrw.32                q5, [q6, #(2*incr)]         \n"
        /* xa' = (xa + xc) + (xb + xd) */
        /* ya' = (ya + yc) + (yb + yd) */
        "   vadd.f16                q4, q0, q2                  \n"
        "   vstrw.32                q4, [q6, #-64]              \n"
        /* xa' = (xa + xc) - (xb + xd) */
        /* ya' = (ya + yc) - (yb + yd) */
        "   vsub.f16                q4, q0, q2                  \n"
        "   vstrw.32                q4, [q6, #(-64 + incr)]     \n"
        /* xb' = (xa - xc) + (yb - yd) */
        /* yb' = (ya - yc) - (xb - xd) */
        "   vcadd.f16               q4, q1, q3, #270            \n"
        "   vstrw.32                q4, [q6, #(-64 + 2*incr)]   \n"
        /* xb' = (xa - xc) - (yb - yd) */
        /* yb' = (ya - yc) + (xb - xd) */
        "   vcadd.f16               q4, q1, q3, #90             \n"
        "   vstrw.32                q4, [q6, #(-64 + 3*incr)]   \n"
        "   le                      lr, 2b                      \n"
        /* low overhead loop end */
        "1:                                                     \n"
        : [pout] "+r"(pSrc)
        : [count] "r"(fftLen >> 4), [strides] "r"(strides)
        : "q0", "q1", "q2", "q3",
          "q4", "q5", "q6", "q7",
          "memory", "lr");
#else
    uint32x4_t vecScGathAddr = vld1q_u32((uint32_t*)strides);
    vecScGathAddr = vecScGathAddr + (uint32_t) pSrc;

    /* load scheduling */
    vecA = (f16x8_t)vldrwq_gather_base_wb_f32(&vecScGathAddr, 64);
    vecC = (f16x8_t)vldrwq_gather_base_f32(vecScGathAddr, 8);

    blkCnt = (fftLen >> 4);
    while (blkCnt > 0U)
    {
        vecSum0 = vecA + vecC;  /* vecSum0 = vaddq(vecA, vecC) */
        vecDiff0 = vecA - vecC; /* vecSum0 = vsubq(vecA, vecC) */

        vecB = (f16x8_t)vldrwq_gather_base_f32(vecScGathAddr, 4);
        vecD = (f16x8_t)vldrwq_gather_base_f32(vecScGathAddr, 12);

        vecSum1 = vecB + vecD;
        vecDiff1 = vecB - vecD;

        /* pre-load for next iteration */
        vecA = (f16x8_t)vldrwq_gather_base_wb_f32(&vecScGathAddr, 64);
        vecC = (f16x8_t)vldrwq_gather_base_f32(vecScGathAddr, 8);

        vecTmp0 = vecSum0 + vecSum1;
        vstrwq_scatter_base_f32(vecScGathAddr, -64, (f32x4_t)vecTmp0);

        vecTmp0 = vecSum0 - vecSum1;
        vstrwq_scatter_base_f32(vecScGathAddr, -64 + 4, (f32x4_t)vecTmp0);

        vecTmp0 = MVE_CMPLX_SUB_A_ixB(vecDiff0, vecDiff1);
        vstrwq_scatter_base_f32(vecScGathAddr, -64 + 8, (f32x4_t)vecTmp0);

        vecTmp0 = MVE_CMPLX_ADD_A_ixB(vecDiff0, vecDiff1);
        vstrwq_scatter_base_f32(vecScGathAddr, -64 + 12, (f32x4_t)vecTmp0);

        blkCnt--;
    }

#endif
    /*
     * End of last stage process
     */
}

static void arm_cfft_radix4by2_f16_mve(const arm_cfft_instance_f16 * S, float16_t *pSrc, uint32_t fftLen)
{
    float16_t const  *pCoef = S->pTwiddle;
    float16_t        *pIn0, *pIn1;
    uint32_t          n2;
#ifndef USE_ASM
    uint32_t          blkCnt;
    f16x8_t           vecIn0, vecIn1, vecSum, vecDiff;
    f16x8_t           vecCmplxTmp, vecTw;
#endif

    n2 = fftLen >> 1;
    pIn0 = pSrc;
    pIn1 = pSrc + fftLen;

#ifdef USE_ASM
    __asm volatile(RAD2_BFLY_FLT16_MVE(conj)
        : [pIn0] "+r"(pIn0), [pIn1] "+r"(pIn1), [pCoef] "+r"(pCoef)
        : [count] "r"(n2 / 4)
        : "lr", "memory",
          "q0", "q1", "q2", "q3", "q4", "q7");
#else

    blkCnt = n2 / 4;
    while (blkCnt > 0U)
    {
        vecIn0 = *(f16x8_t *) pIn0;
        vecIn1 = *(f16x8_t *) pIn1;
        vecTw = vld1q(pCoef);
        pCoef += 8;

        vecSum = vaddq(vecIn0, vecIn1);
        vecDiff = vsubq(vecIn0, vecIn1);

        vecCmplxTmp = MVE_CMPLX_MULT_FLT_Conj_AxB(vecTw, vecDiff);

        vst1q(pIn0, vecSum);
        pIn0 += 8;
        vst1q(pIn1, vecCmplxTmp);
        pIn1 += 8;

        blkCnt--;
    }
#endif

    _arm_radix4_butterfly_f16_mve(S, pSrc, n2);

    _arm_radix4_butterfly_f16_mve(S, pSrc + fftLen, n2);

    pIn0 = pSrc;
}

static void _arm_radix4_butterfly_inverse_f16_mve(const arm_cfft_instance_f16 * S,float16_t * pSrc, uint32_t fftLen, float16_t onebyfftLen)
{
#ifndef USE_ASM
    f16x8_t   vecTmp0, vecTmp1;
    f16x8_t   vecSum0, vecDiff0, vecSum1, vecDiff1;
    f16x8_t   vecA, vecB, vecC, vecD;
    f16x8_t   vecW;
    uint32_t  blkCnt;
#endif
    uint32_t  n1, n2;
    uint32_t  stage = 0;
    int32_t  iter = 1;
    static const int32_t strides[4] = {
        ( 0 - 16) * (int32_t)sizeof(q31_t *),
        ( 4 - 16) * (int32_t)sizeof(q31_t *),
        ( 8 - 16) * (int32_t)sizeof(q31_t *),
        (12 - 16) * (int32_t)sizeof(q31_t *)
    };

    n2 = fftLen;
    n1 = n2;
    n2 >>= 2u;
    for (int k = fftLen / 4; k > 1; k >>= 2)
    {
        float16_t const *p_rearranged_twiddle_tab_stride2 =
                    &S->rearranged_twiddle_stride2[
                    S->rearranged_twiddle_tab_stride2_arr[stage]];
        float16_t const *p_rearranged_twiddle_tab_stride3 = &S->rearranged_twiddle_stride3[
                    S->rearranged_twiddle_tab_stride3_arr[stage]];
        float16_t const *p_rearranged_twiddle_tab_stride1 =
            &S->rearranged_twiddle_stride1[
            S->rearranged_twiddle_tab_stride1_arr[stage]];

        float16_t * pBase = pSrc;
        for (int i = 0; i < iter; i++)
        {
            float16_t    *inA = pBase;
            float16_t    *inB = inA + n2 * CMPLX_DIM;
            float16_t    *inC = inB + n2 * CMPLX_DIM;
            float16_t    *inD = inC + n2 * CMPLX_DIM;
            float16_t const *pW1 = p_rearranged_twiddle_tab_stride1;
            float16_t const *pW2 = p_rearranged_twiddle_tab_stride2;
            float16_t const *pW3 = p_rearranged_twiddle_tab_stride3;

#ifdef USE_ASM
            register unsigned count  __asm("lr") = (n2 / 4);
            __asm volatile(
                ".p2align 2                                             \n"
                "   wls                     lr, %[count], 1f            \n"
                "   vldrh.16                q1, [%[pSrcA0]]             \n"
                "   vldrh.16                q6, [%[pSrcA2]]             \n"
                /* low overhead loop start */
                "2:                                                     \n"
                /*  = R(xa + xc), I(ya + yc) */
                "   vadd.f16                q0, q1, q6                  \n"
                /*  q4 = yb2, xb2, yb1, xb1 */
                "   vldrh.16                q4, [%[pSrcA1]]             \n"
                /*  = R(xa - xc), I(ya - yc) */
                "   vsub.f16                q2, q1, q6                  \n"
                /*  q5 = yd2, xd2, yd1, xd1 */
                "   vldrh.16                q5, [%[pSrcA3]]             \n"
                /*  = R(xb + xd), I(yb + yd) */
                "   vadd.f16                q1, q4, q5                  \n"
                /*  = R(xb - xd), I(yb - yd) */
                "   vsub.f16                q3, q4, q5                  \n"
                /*  xa' = (xa + xc) + (xb + xd) */
                /*  ya' = (ya + yc) + (yb + yd) */
                "   vadd.f16                q4, q0, q1                  \n"
                "   vstrh.16                q4, [%[pSrcA0]], #16        \n"
                /*  xc' = (xa + xc) - (xb + xd) */
                /*  yc' = (ya + yc) - (yb + yd) */
                "   vsub.f16                q4, q0, q1                  \n"
                /*  load twiddle factors */
                "   vldrh.16                q5, [%[t0]], #16            \n"
                /*  xc' = (xa-xb+xc-xd)co2 + (ya-yb+yc-yd)(si2) */
                /*  yc' = (ya-yb+yc-yd)co2 - (xa-xb+xc-xd)(si2) */
                "   vcmul.f16               q0, q5, q4, #0              \n"
                "   vcmla.f16               q0, q5, q4, #90             \n"
                "   vstrh.16                q0, [%[pSrcA1]], #16        \n"
                /*  xb' = (xa - xc) + (yb - yd) */
                /*  yb' = (ya - yc) - (xb - xd) */
                "   vcadd.f16               q4, q2, q3, #90             \n"
                /*  load twiddle factors */
                "   vldrh.16                q5, [%[t1]], #16            \n"
                /*  xb' = (xa+yb-xc-yd)co1 + (ya-xb-yc+xd)(si1) */
                /*  yb' = (ya-xb-yc+xd)co1 - (xa+yb-xc-yd)(si1) */
                "   vcmul.f16               q0, q5, q4, #0              \n"
                "   vldrh.16                q1, [%[pSrcA0]]             \n"
                "   vcmla.f16               q0, q5, q4, #90             \n"
                "   vstrh.16                q0, [%[pSrcA2]], #16        \n"
                /*  xd' = (xa - xc) - (yb - yd) */
                /*  yd' = (ya - yc) + (xb - xd) */
                "   vcadd.f16               q4, q2, q3, #270            \n"
                /*  load twiddle factors */
                "   vldrh.16                q5, [%[t2]], #16            \n"
                /*  xd' = (xa-yb-xc+yd)* co3 + (ya+xb-yc-xd)* (sa3) */
                /*  yd' = (ya+xb-yc-xd)* co3 - (xa-yb-xc+yd)* (sa3) */
                "   vcmul.f16               q0, q5, q4, #0              \n"
                "   vldrh.16                q6, [%[pSrcA2]]             \n"
                "   vcmla.f16               q0, q5, q4, #90             \n"
                "   vstrh.16                q0, [%[pSrcA3]], #16        \n"
                /* low overhead loop end */
                "   le                      lr, 2b                      \n"
                "1:                                                     \n"
                : [pSrcA0] "+r"(inA), [pSrcA1] "+r"(inB),
                  [pSrcA2] "+r"(inC), [pSrcA3] "+r"(inD),
                  [count] "+r" (count),
                  [t0] "+r"(pW2) , [t1] "+r"(pW1), [t2] "+r"(pW3)
                :
                : "q0", "q1", "q2", "q3",
                  "q4", "q5", "q6", "q7",
                  "memory");
#else
            blkCnt = n2 / 4;
            /*
             * load 2 f32 complex pair
             */
            vecA = vldrhq_f16(inA);
            vecC = vldrhq_f16(inC);
            while (blkCnt > 0U)
            {
                vecB = vldrhq_f16(inB);
                vecD = vldrhq_f16(inD);

                vecSum0 = vecA + vecC;  /* vecSum0 = vaddq(vecA, vecC) */
                vecDiff0 = vecA - vecC; /* vecSum0 = vsubq(vecA, vecC) */

                vecSum1 = vecB + vecD;
                vecDiff1 = vecB - vecD;
                /*
                 * [ 1 1 1 1 ] * [ A B C D ]' .* 1
                 */
                vecTmp0 = vecSum0 + vecSum1;
                vst1q(inA, vecTmp0);
                inA += 8;
                /*
                 * [ 1 -1 1 -1 ] * [ A B C D ]'
                 */
                vecTmp0 = vecSum0 - vecSum1;
                /*
                 * [ 1 -1 1 -1 ] * [ A B C D ]'.* W1
                 */
                vecW = vld1q(pW2);
                pW2 += 8;
                vecTmp1 = MVE_CMPLX_MULT_FLT_AxB(vecW, vecTmp0);
                vst1q(inB, vecTmp1);
                inB += 8;

                /*
                 * [ 1 -i -1 +i ] * [ A B C D ]'
                 */
                vecTmp0 = MVE_CMPLX_ADD_A_ixB(vecDiff0, vecDiff1);
                /*
                 * [ 1 -i -1 +i ] * [ A B C D ]'.* W2
                 */
                vecW = vld1q(pW1);
                pW1 += 8;
                vecTmp1 = MVE_CMPLX_MULT_FLT_AxB(vecW, vecTmp0);
                vst1q(inC, vecTmp1);
                inC += 8;

                /*
                 * [ 1 +i -1 -i ] * [ A B C D ]'
                 */
                vecTmp0 = MVE_CMPLX_SUB_A_ixB(vecDiff0, vecDiff1);
                /*
                 * [ 1 +i -1 -i ] * [ A B C D ]'.* W3
                 */
                vecW = vld1q(pW3);
                pW3 += 8;
                vecTmp1 = MVE_CMPLX_MULT_FLT_AxB(vecW, vecTmp0);
                vst1q(inD, vecTmp1);
                inD += 8;

                vecA = vldrhq_f16(inA);
                vecC = vldrhq_f16(inC);

                blkCnt--;
            }
#endif
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
    float16_t *in0 = pSrc;
    __asm volatile(
        ".set incr,                 16/4                        \n"
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
        "   vadd.f16                q0, q7, q5                  \n"
        /* Read xb (real), yb(imag) input. */
        "   vldrw.32                q4, [q6, #(1*incr)]         \n"
        /* = R(xa - xc), I(ya - yc) */
        "   vsub.f16                q1, q7, q5                  \n"
        /* Read xc (real), yc(imag) input */
        "   vldrw.32                q5, [q6, #(3*incr)]         \n"
        /* = R(xb + xd), I(yb + yd) */
        "   vadd.f16                q2, q4, q5                  \n"
        "   vldrw.32                q7, [q6, #64]!              \n"
        /* = R(xb - xd), I(yb - yd) */
        "   vsub.f16                q3, q4, q5                  \n"
        "   vldrw.32                q5, [q6, #(2*incr)]         \n"
        /* xa' = (xa + xc) + (xb + xd) */
        /* ya' = (ya + yc) + (yb + yd) */
        "   vadd.f16                q4, q0, q2                  \n"
        "   vmul.f16                q4, q4, %[scale]            \n"
        "   vstrw.32                q4, [q6, #-64]              \n"
        /* xa' = (xa + xc) - (xb + xd) */
        /* ya' = (ya + yc) - (yb + yd) */
        "   vsub.f16                q4, q0, q2                  \n"
        "   vmul.f16                q4, q4, %[scale]            \n"
        "   vstrw.32                q4, [q6, #(-64 + incr)]     \n"
        /* xb' = (xa - xc) + (yb - yd) */
        /* yb' = (ya - yc) - (xb - xd) */
        "   vcadd.f16               q4, q1, q3, #90             \n"
        "   vmul.f16                q4, q4, %[scale]            \n"
        "   vstrw.32                q4, [q6, #(-64 + 2*incr)]   \n"
        /* xb' = (xa - xc) - (yb - yd) */
        /* yb' = (ya - yc) + (xb - xd) */
        "   vcadd.f16               q4, q1, q3, #270            \n"
        "   vmul.f16                q4, q4, %[scale]            \n"
        "   vstrw.32                q4, [q6, #(-64 + 3*incr)]   \n"
        /* low overhead loop end */
        "   le                      lr, 2b                      \n"
        "1:                                                     \n"
        : [pout] "+r"(in0)
        : [count] "r"(fftLen >> 4), [strides] "r"(strides),
          [scale] "r"(onebyfftLen)
        : "q0", "q1", "q2", "q3",
          "q4", "q5", "q6", "q7",
          "memory", "lr");
#else

    uint32x4_t vecScGathAddr = vld1q_u32((uint32_t*)strides);
    vecScGathAddr = vecScGathAddr + (uint32_t) pSrc;

    /*
     * load scheduling
     */
    vecA = (f16x8_t)vldrwq_gather_base_wb_f32(&vecScGathAddr, 64);
    vecC = (f16x8_t)vldrwq_gather_base_f32(vecScGathAddr, 8);

    blkCnt = (fftLen >> 4);
    while (blkCnt > 0U)
    {
        vecSum0 = vecA + vecC;  /* vecSum0 = vaddq(vecA, vecC) */
        vecDiff0 = vecA - vecC; /* vecSum0 = vsubq(vecA, vecC) */

        vecB = (f16x8_t)vldrwq_gather_base_f32(vecScGathAddr, 4);
        vecD = (f16x8_t)vldrwq_gather_base_f32(vecScGathAddr, 12);

        vecSum1 = vecB + vecD;
        vecDiff1 = vecB - vecD;

        vecA = (f16x8_t)vldrwq_gather_base_wb_f32(&vecScGathAddr, 64);
        vecC = (f16x8_t)vldrwq_gather_base_f32(vecScGathAddr, 8);

        vecTmp0 = vecSum0 + vecSum1;
        vecTmp0 = vecTmp0 * onebyfftLen;
        vstrwq_scatter_base_f32(vecScGathAddr, -64, (f32x4_t)vecTmp0);

        vecTmp0 = vecSum0 - vecSum1;
        vecTmp0 = vecTmp0 * onebyfftLen;
        vstrwq_scatter_base_f32(vecScGathAddr, -64 + 4, (f32x4_t)vecTmp0);

        vecTmp0 = MVE_CMPLX_ADD_A_ixB(vecDiff0, vecDiff1);
        vecTmp0 = vecTmp0 * onebyfftLen;
        vstrwq_scatter_base_f32(vecScGathAddr, -64 + 8, (f32x4_t)vecTmp0);

        vecTmp0 = MVE_CMPLX_SUB_A_ixB(vecDiff0, vecDiff1);
        vecTmp0 = vecTmp0 * onebyfftLen;
        vstrwq_scatter_base_f32(vecScGathAddr, -64 + 12, (f32x4_t)vecTmp0);

        blkCnt--;
    }

#endif
    /*
     * End of last stage process
     */
}

static void arm_cfft_radix4by2_inverse_f16_mve(const arm_cfft_instance_f16 * S,float16_t *pSrc, uint32_t fftLen)
{
    float16_t const  *pCoef = S->pTwiddle;
    float16_t        *pIn0, *pIn1;
    uint32_t          n2;
    float16_t         onebyfftLen = arm_inverse_fft_length_f16(fftLen);
#ifndef USE_ASM
    uint32_t          blkCnt;
    f16x8_t           vecIn0, vecIn1, vecSum, vecDiff;
    f16x8_t           vecCmplxTmp, vecTw;
#endif

    n2 = fftLen >> 1;
    pIn0 = pSrc;
    pIn1 = pSrc + fftLen;

#ifdef USE_ASM
    __asm volatile(RAD2_BFLY_FLT16_MVE(/*no conjugate*/)
        : [pIn0] "+r"(pIn0), [pIn1] "+r"(pIn1), [pCoef] "+r"(pCoef)
        : [count] "r"(n2 / 4)
        : "lr", "memory",
          "q0", "q1", "q2", "q3", "q4", "q7");
#else
    blkCnt = n2 / 4;
    while (blkCnt > 0U)
    {
        vecIn0 = *(f16x8_t *) pIn0;
        vecIn1 = *(f16x8_t *) pIn1;
        vecTw = vld1q(pCoef);
        pCoef += 8;

        vecSum = vaddq(vecIn0, vecIn1);
        vecDiff = vsubq(vecIn0, vecIn1);

        vecCmplxTmp = MVE_CMPLX_MULT_FLT_AxB(vecTw, vecDiff);

        vst1q(pIn0, vecSum);
        pIn0 += 8;
        vst1q(pIn1, vecCmplxTmp);
        pIn1 += 8;

        blkCnt--;
    }
#endif

    _arm_radix4_butterfly_inverse_f16_mve(S, pSrc, n2, onebyfftLen);

    _arm_radix4_butterfly_inverse_f16_mve(S, pSrc + fftLen, n2, onebyfftLen);
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


void arm_cfft_f16_mve(
  const arm_cfft_instance_f16 * S,
        float16_t * pSrc,
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
                _arm_radix4_butterfly_inverse_f16_mve(S, pSrc, fftLen, arm_inverse_fft_length_f16(S->fftLen));
                break;

            case 32:
            case 128:
            case 512:
            case 2048:
                arm_cfft_radix4by2_inverse_f16_mve(S, pSrc, fftLen);
                break;
            }
        } else {
            switch (fftLen) {
            case 16:
            case 64:
            case 256:
            case 1024:
            case 4096:
                _arm_radix4_butterfly_f16_mve(S, pSrc, fftLen);
                break;

            case 32:
            case 128:
            case 512:
            case 2048:
                arm_cfft_radix4by2_f16_mve(S, pSrc, fftLen);
                break;
            }
        }


        if (bitReverseFlag)
        {
#ifdef USE_ASM
            arm_bitreversal_f16_inpl_mve_asm((uint16_t*)pSrc, S->bitRevLength, S->pBitRevTable);
#else
            arm_bitreversal_16_inpl_mve((uint16_t*)pSrc, S->bitRevLength, S->pBitRevTable);
#endif
        }
}

#endif
/**
  @} end of ComplexFFT group
 */
