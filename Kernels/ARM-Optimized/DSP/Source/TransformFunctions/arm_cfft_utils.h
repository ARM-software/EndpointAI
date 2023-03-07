/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_cfft_utils.h
 * Description:  CFFT Macros
 *
 * $Date:        07 Mar 2023
 * $Revision:    V1.0.1
 *
 * Target Processor: Cortex-M with helium
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


#ifndef ARM_CFFT_UTILS_H
#define ARM_CFFT_UTILS_H

#include "dsp/fast_math_functions.h"


#if defined(ARM_MATH_MVEF)


#ifdef USE_ASM

/*
 * Floating point multiplication, 1st elt conjugated
 * Qd = Qn' * Qm
 */

#undef cmplx_flt_mul_r_conj
#undef cmplx_flt_mul_i_conj
#define cmplx_flt_mul_r_conj(sz, qd, qn, qm) " vcmul.f"#sz"    " #qd "," #qn "," #qm ", #0 \n"
#define cmplx_flt_mul_i_conj(sz, qd, qn, qm) " vcmla.f"#sz"    " #qd "," #qn "," #qm ", #270 \n"

/*
 * Floating point multiplication
 * Qd = Qn * Qm
 */
#undef cmplx_flt_mul_r_
#undef cmplx_flt_mul_i_
#define cmplx_flt_mul_r_(sz, qd, qn, qm)    " vcmul.f"#sz"    " #qd "," #qn "," #qm ", #0 \n"
#define cmplx_flt_mul_i_(sz, qd, qn, qm)    " vcmla.f"#sz"    " #qd "," #qn "," #qm ", #90 \n"


#define RAD2_BFLY_FLT_MVE(sz, conj)                            \
    ".p2align 2                                             \n"\
    "   wls                 lr, %[count], 1f                \n"\
    "2:                                                     \n"\
    /* low overhead loop start */                              \
    "   vldrw.32            q0, [%[pIn0]]                   \n"\
    "   vldrw.32            q1, [%[pIn1]]                   \n"\
    /* a0,a1 */                                                \
    "   vadd.f"#sz"            q2, q0, q1                   \n"\
    /* twiddle */                                              \
    "   vldrw.32            q7, [%[pCoef]], 16              \n"\
    /* xt,yt */                                                \
    "   vsub.f"#sz"            q3, q0, q1                   \n"\
    cmplx_flt_mul_r_##conj(sz, q4, q7, q3)                     \
    "   vstrw.32            q2, [%[pIn0]], 16               \n"\
    cmplx_flt_mul_i_##conj(sz, q4, q7, q3)                     \
    "   vstrw.32            q4, [%[pIn1]], 16               \n"\
    /* low overhead loop end */                                \
    "   le                  lr, 2b                          \n"\
    "1:                                                     \n"



#if defined(ARM_CM85_OPT)

/* original hand-optimized loop kept for reference */
/* obsoleted by a better slothy optimized variant  */

#define RAD4_BFLY_STG_FLT_MVE_SUBOPT(sz, rot1, rot2)           \
    ".p2align 2                                             \n"\
    "   wls                     lr, %[count], 1f            \n"\
    "   vldrw.32                q0, [%[pSrcA0]]             \n"\
    "   vldrw.32                q1, [%[pSrcA2]]             \n"\
    "2:                                                     \n"\
    /*  = R(xa + xc), I(ya + yc) */                            \
    "   vadd.f"#sz"             q6, q0, q1                  \n"\
    /*  q4 = yb2, xb2, yb1, xb1 */                             \
    "   vldrw.32                q2, [%[pSrcA1]]             \n"\
    /*  = R(xa - xc), I(ya - yc) */                            \
    "   vsub.f"#sz"             q7, q0, q1                  \n"\
    /*  q5 = yd2, xd2, yd1, xd1 */                             \
    "   vldrw.32                q3, [%[pSrcA3]]             \n"\
    /*  = R(xb + xd), I(yb + yd) */                            \
    "   vadd.f"#sz"             q0, q2, q3                  \n"\
    /*  load twiddle factors */                                \
    "   vldrw.32                q4, [%[t0]], #16            \n"\
    /*  = R(xb - xd), I(yb - yd) */                            \
    "   vsub.f"#sz"             q1, q2, q3                  \n"\
    /*  load twiddle factors */                                \
    "   vldrw.32                q5, [%[t1]], #16            \n"\
    /*  xa' = (xa + xc) + (xb + xd) */                         \
    /*  ya' = (ya + yc) + (yb + yd) */                         \
    "   vadd.f"#sz"             q2, q6, q0                  \n"\
    "   vstrw.32                q2, [%[pSrcA0]], #16        \n"\
    /*  xc' = (xa + xc) - (xb + xd) */                         \
    /*  yc' = (ya + yc) - (yb + yd) */                         \
    "   vsub.f"#sz"             q2, q6, q0                  \n"\
    "   vldrw.32                q0, [%[pSrcA0]]             \n"\
    /*  xc' = (xa-xb+xc-xd)co2 + (ya-yb+yc-yd)(si2) */         \
    /*  yc' = (ya-yb+yc-yd)co2 - (xa-xb+xc-xd)(si2) */         \
    "   vcmul.f"#sz"            q3, q4, q2, #0              \n"\
    /*  xb' = (xa - xc) + (yb - yd) */                         \
    /*  yb' = (ya - yc) - (xb - xd) */                         \
    "   vcadd.f"#sz"            q6, q7, q1, #"#rot1"        \n"\
    "   vcmla.f"#sz"            q3, q4, q2, #"#rot1"        \n"\
    "   vstrw.u32               q3, [%[pSrcA1]], #16        \n"\
    /*  xd' = (xa - xc) - (yb - yd) */                         \
    /*  yd' = (ya - yc) + (xb - xd) */                         \
    "   vcmul.f"#sz"            q4, q5, q6, #0              \n"\
    "   vcadd.f"#sz"            q2, q7, q1, #"#rot2"        \n"\
    /*  xb' = (xa+yb-xc-yd)co1 + (ya-xb-yc+xd)(si1) */         \
    /*  yb' = (ya-xb-yc+xd)co1 - (xa+yb-xc-yd)(si1) */         \
    /*  load twiddle factors */                                \
    "   vldrw.32                q7, [%[t2]], #16            \n"\
    "   vcmla.f"#sz"            q4, q5, q6, #"#rot1"        \n"\
    "   vstrw.32                q4, [%[pSrcA2]], #16        \n"\
    /*  xd' = (xa-yb-xc+yd)* co3 + (ya+xb-yc-xd)* (sa3) */     \
    /*  yd' = (ya+xb-yc-xd)* co3 - (xa-yb-xc+yd)* (sa3) */     \
    "   vcmul.f"#sz"            q3, q7, q2, #0              \n"\
    "   vldrw.32                q1, [%[pSrcA2], #0]         \n"\
    "   vcmla.f"#sz"            q3, q7, q2, #"#rot1"        \n"\
    "   vstrw.32                q3, [%[pSrcA3]], #16        \n"\
    "   le                      lr, 2b                      \n"\
    "1:                                                     \n"


/* slothy / helight optimized version */

#define RAD4_BFLY_STG_FLT_MVE(sz, rot1, rot2)                  \
    ".p2align 2                                             \n"\
                                                               \
    "    vldrw.32        q4, [%[pSrcA0]]                    \n"\
    "    vldrw.32        q5, [%[pSrcA3]]                    \n"\
    "    vldrw.32        q1, [%[pSrcA2]]                    \n"\
    "    vsub.f"#sz"     q3, q4, q1                         \n"\
    "    vldrw.32        q0, [%[pSrcA1]]                    \n"\
    "    vsub.f"#sz"     q6, q0, q5                         \n"\
    "    wls             lr, %[count], 1f                   \n"\
    "2:                                                     \n"\
    "    vcadd.f"#sz"    q7, q3, q6, #"#rot2"               \n"\
    "    vldrw.32        q2, [%[t2]] , #16                  \n"\
    "    vadd.f"#sz"     q1, q4, q1                         \n"\
    "    vcmul.f"#sz"    q4, q2, q7, #0                     \n"\
    "    vadd.f"#sz"     q5, q0, q5                         \n"\
    "    vldrw.32        q0, [%[pSrcA1], #16]               \n"\
    "    vcmla.f"#sz"    q4, q2, q7, #"#rot1"               \n"\
    "    vldrw.32        q2, [%[t1]] , #16                  \n"\
    "    vstrw.u32       q4, [%[pSrcA3]] , #16              \n"\
    "    vcadd.f"#sz"    q4, q3, q6, #"#rot1"               \n"\
    "    vldrw.32        q6, [%[t0]] , #16                  \n"\
    "    vsub.f"#sz"     q3, q1, q5                         \n"\
    "    vcmul.f"#sz"    q7, q2, q4, #0                     \n"\
    "    vadd.f"#sz"     q5, q1, q5                         \n"\
    "    vcmul.f"#sz"    q1, q6, q3, #0                     \n"\
    "    vstrw.u32       q5, [%[pSrcA0]] , #16              \n"\
    "    vcmla.f"#sz"    q7, q2, q4, #"#rot1"               \n"\
    "    vldrw.32        q4, [%[pSrcA0]]                    \n"\
    "    vcmla.f"#sz"    q1, q6, q3, #"#rot1"               \n"\
    "    vldrw.32        q5, [%[pSrcA3]]                    \n"\
    "    vstrw.u32       q1, [%[pSrcA1]] , #16              \n"\
    "    vsub.f"#sz"     q6, q0, q5                         \n"\
    "    vldrw.32        q1, [%[pSrcA2], #16]               \n"\
    "    vsub.f"#sz"     q3, q4, q1                         \n"\
    "    vstrw.u32       q7, [%[pSrcA2]] , #16              \n"\
    "    le              lr, 2b                             \n"\
    "1:                                                     \n"\
    "    vcadd.f"#sz"    q7, q3, q6, #"#rot1"               \n"\
    "    vldrw.32        q2, [%[t1]] , #16                  \n"\
    "    vadd.f"#sz"     q1, q4, q1                         \n"\
    "    vcmul.f"#sz"    q4, q2, q7, #0                     \n"\
    "    vadd.f"#sz"     q0, q0, q5                         \n"\
    "    vldrw.32        q5, [%[t0]] , #16                  \n"\
    "    vcmla.f"#sz"    q4, q2, q7, #"#rot1"               \n"\
    "    vstrw.u32       q4, [%[pSrcA2]] , #16              \n"\
    "    vcadd.f"#sz"    q7, q3, q6, #"#rot2"               \n"\
    "    vldrw.32        q3, [%[t2]] , #16                  \n"\
    "    vsub.f"#sz"     q4, q1, q0                         \n"\
    "    vcmul.f"#sz"    q6, q3, q7, #0                     \n"\
    "    vadd.f"#sz"     q2, q1, q0                         \n"\
    "    vcmul.f"#sz"    q1, q5, q4, #0                     \n"\
    "    vstrw.u32       q2, [%[pSrcA0]] , #16              \n"\
    "    vcmla.f"#sz"    q6, q3, q7, #"#rot1"               \n"\
    "    vstrw.u32       q6, [%[pSrcA3]] , #16              \n"\
    "    vcmla.f"#sz"    q1, q5, q4, #"#rot1"               \n"\
    "    vstrw.u32       q1, [%[pSrcA1]] , #16              \n"\

#else

/* original hand-optimized loop kept for reference */
/* obsoleted by a better slothy optimized variant  */

#define RAD4_BFLY_STG_FLT_MVE_SUBOPT(sz, rot1, rot2)           \
    ".p2align 2                                             \n"\
    "   wls                     lr, %[count], 1f            \n"\
    "   vldrw.32                q1, [%[pSrcA0]]             \n"\
    "   vldrw.32                q6, [%[pSrcA2]]             \n"\
    "2:                                                     \n"\
    /*  = R(xa + xc), I(ya + yc) */                            \
    "   vadd.f"#sz"             q0, q1, q6                  \n"\
    /*  q4 = yb2, xb2, yb1, xb1 */                             \
    "   vldrw.32                q4, [%[pSrcA1]]             \n"\
    /*  = R(xa - xc), I(ya - yc) */                            \
    "   vsub.f"#sz"             q2, q1, q6                  \n"\
    /*  q5 = yd2, xd2, yd1, xd1 */                             \
    "   vldrw.32                q5, [%[pSrcA3]]             \n"\
    /*  = R(xb + xd), I(yb + yd) */                            \
    "   vadd.f"#sz"             q1, q4, q5                  \n"\
    /*  = R(xb - xd), I(yb - yd) */                            \
    "   vsub.f"#sz"             q3, q4, q5                  \n"\
    /*  xa' = (xa + xc) + (xb + xd) */                         \
    /*  ya' = (ya + yc) + (yb + yd) */                         \
    "   vadd.f"#sz"             q4, q0, q1                  \n"\
    "   vstrw.32                q4, [%[pSrcA0]], #16        \n"\
    /*  xc' = (xa + xc) - (xb + xd) */                         \
    /*  yc' = (ya + yc) - (yb + yd) */                         \
    "   vsub.f"#sz"             q4, q0, q1                  \n"\
    /*  load twiddle factors */                                \
    "   vldrw.32                q5, [%[t0]], #16            \n"\
    /*  xc' = (xa-xb+xc-xd)co2 + (ya-yb+yc-yd)(si2) */         \
    /*  yc' = (ya-yb+yc-yd)co2 - (xa-xb+xc-xd)(si2) */         \
    "   vcmul.f"#sz"            q0, q5, q4, #0              \n"\
    "   vcmla.f"#sz"            q0, q5, q4, #"#rot1"        \n"\
    "   vstrw.32                q0, [%[pSrcA1]], #16        \n"\
    /*  xb' = (xa - xc) + (yb - yd) */                         \
    /*  yb' = (ya - yc) - (xb - xd) */                         \
    "   vcadd.f"#sz"            q4, q2, q3, #"#rot1"        \n"\
    /*  load twiddle factors */                                \
    "   vldrw.32                q5, [%[t1]], #16            \n"\
    /*  xb' = (xa+yb-xc-yd)co1 + (ya-xb-yc+xd)(si1) */         \
    /*  yb' = (ya-xb-yc+xd)co1 - (xa+yb-xc-yd)(si1) */         \
    "   vcmul.f"#sz"            q0, q5, q4, #0              \n"\
    "   vldrw.32                q1, [%[pSrcA0]]             \n"\
    "   vcmla.f"#sz"            q0, q5, q4, #"#rot1"        \n"\
    "   vstrw.u32               q0, [%[pSrcA2]], #16        \n"\
    /*  xd' = (xa - xc) - (yb - yd) */                         \
    /*  yd' = (ya - yc) + (xb - xd) */                         \
    "   vcadd.f"#sz"            q4, q2, q3, #"#rot2"        \n"\
    /*  load twiddle factors */                                \
    "   vldrw.32                q5, [%[t2]], #16            \n"\
    /*  xd' = (xa-yb-xc+yd)* co3 + (ya+xb-yc-xd)* (sa3) */     \
    /*  yd' = (ya+xb-yc-xd)* co3 - (xa-yb-xc+yd)* (sa3) */     \
    "   vcmul.f"#sz"            q0, q5, q4, #0              \n"\
    "   vldrw.32                q6, [%[pSrcA2]]             \n"\
    "   vcmla.f"#sz"            q0, q5, q4, #"#rot1"        \n"\
    "   vstrw.32                q0, [%[pSrcA3]], #16        \n"\
    "   le                      lr, 2b                      \n"\
    "1:                                                     \n"

/* slothy / helight optimized version */
#define RAD4_BFLY_STG_FLT_MVE(sz, rot1, rot2)                  \
    "   inA        .req %[pSrcA0]                           \n"\
    "   inB        .req %[pSrcA1]                           \n"\
    "   inC        .req %[pSrcA2]                           \n"\
    "   inD        .req %[pSrcA3]                           \n"\
    "   pW0        .req %[t0]                               \n"\
    "   pW1        .req %[t1]                               \n"\
    "   pW3        .req %[t2]                               \n"\
    "                                                       \n"\
    ".p2align 2                                             \n"\
    "   vldrw.32            q0, [inA]                       \n"\
    "   vldrw.32            q1, [inD]                       \n"\
    "   vldrw.32            q2, [inC]                       \n"\
    "   wls                 lr, %[count], 1f                \n"\
    "   2:                                                  \n"\
    "   vadd.f"#sz"         q6, q0, q2                      \n"\
    "   vsub.f"#sz"         q3, q0, q2                      \n"\
    "   vldrw.32            q0, [inB]                       \n"\
    "   vadd.f"#sz"         q7, q0, q1                      \n"\
    "   vldrw.32            q2, [inC, #16]                  \n"\
    "   vsub.f"#sz"         q4, q6, q7                      \n"\
    "   vadd.f"#sz"         q5, q6, q7                      \n"\
    "   vldrw.32            q7, [pW1] , #16                 \n"\
    "   vsub.f"#sz"         q1, q0, q1                      \n"\
    "   vldrw.32            q0, [pW0] , #16                 \n"\
    "   vcmul.f"#sz"        q6, q0, q4, #0                  \n"\
    "   vstrw.32            q5, [inA] , #16                 \n"\
    "   vcmla.f"#sz"        q6, q0, q4, #"#rot1"            \n"\
    "   vstrw.32            q6, [inB] , #16                 \n"\
    "   vcadd.f"#sz"        q5, q3, q1, #"#rot1"            \n"\
    "   vcmul.f"#sz"        q6, q7, q5, #0                  \n"\
    "   vldrw.32            q0, [inA]                       \n"\
    "   vcmla.f"#sz"        q6, q7, q5, #"#rot1"            \n"\
    "   vldrw.32            q5, [pW3] , #16                 \n"\
    "   vcadd.f"#sz"        q4, q3, q1, #"#rot2"            \n"\
    "   vldrw.32            q1, [inD, #16]                  \n"\
    "   vcmul.f"#sz"        q7, q5, q4, #0                  \n"\
    "   vstrw.32            q6, [inC] , #16                 \n"\
    "   vcmla.f"#sz"        q7, q5, q4, #"#rot1"            \n"\
    "   vstrw.32            q7, [inD] , #16                 \n"\
    "   le                  lr, 2b                          \n"\
    "   1:                                                  \n"\
    "                                                       \n"\
    "   vsub.f"#sz"         q5, q0, q2                      \n"\
    "   vldrw.32            q4, [pW3] , #16                 \n"\
    "   vadd.f"#sz"         q6, q0, q2                      \n"\
    "   vldrw.32            q0, [inB]                       \n"\
    "   vadd.f"#sz"         q3, q0, q1                      \n"\
    "   vadd.f"#sz"         q7, q6, q3                      \n"\
    "   vstrw.32            q7, [inA] , #16                 \n"\
    "   vsub.f"#sz"         q1, q0, q1                      \n"\
    "   vsub.f"#sz"         q7, q6, q3                      \n"\
    "   vldrw.32            q0, [pW0] , #16                 \n"\
    "   vcmul.f"#sz"        q6, q0, q7, #0                  \n"\
    "   vldrw.32            q3, [pW1] , #16                 \n"\
    "   vcmla.f"#sz"        q6, q0, q7, #"#rot1"            \n"\
    "   vcadd.f"#sz"        q7, q5, q1, #"#rot2"            \n"\
    "   vstrw.32            q6, [inB] , #16                 \n"\
    "   vcmul.f"#sz"        q6, q4, q7, #0                  \n"\
    "   vcmla.f"#sz"        q6, q4, q7, #"#rot1"            \n"\
    "   vstrw.32            q6, [inD] , #16                 \n"\
    "   vcadd.f"#sz"        q7, q5, q1, #"#rot1"            \n"\
    "   vcmul.f"#sz"        q6, q3, q7, #0                  \n"\
    "   vcmla.f"#sz"        q6, q3, q7, #"#rot1"            \n"\
    "   vstrw.32            q6, [inC] , #16                 \n"

#endif // ARM_CM85_OPT

#define RAD4_BFLY_STG_FLT_FWD_MVE(sz)       RAD4_BFLY_STG_FLT_MVE_SUBOPT(sz, 270, 90)
#define RAD4_BFLY_STG_FLT_BKWD_MVE(sz)      RAD4_BFLY_STG_FLT_MVE_SUBOPT(sz, 90, 270)

#endif  // USE_ASM

#endif  // ARM_MATH_MVEF


#if defined(ARM_MATH_MVEI) && !defined(ARM_MATH_AUTOVECTORIZE)

#include "arm_vec_fft.h"

#ifdef USE_ASM

/*
 * Fx point multiplication, 2nd elt conjugated
 * Qd = Qn * Qm'
 */

#undef cmplx_fx_mul_r_conj
#undef cmplx_fx_mul_i_conj
#define cmplx_fx_mul_r_conj(qd, qn, qm) " vqrdmladh.s32    " #qd "," #qn "," #qm " \n"
#define cmplx_fx_mul_i_conj(qd, qn, qm) " vqrdmlsdhx.s32   " #qd "," #qn "," #qm " \n"

/*
 * Fx point multiplication
 * Qd = Qn * Qm
 */
#undef cmplx_fx_mul_r_
#undef cmplx_fx_mul_i_
#define cmplx_fx_mul_r_(qd, qn, qm)     " vqrdmlsdh.s32    " #qd "," #qm "," #qn " \n"
#define cmplx_fx_mul_i_(qd, qn, qm)     " vqrdmladhx.s32   " #qd "," #qm "," #qn " \n"


/* original hand-optimized loop kept for reference */
/* obsoleted by a better slothy optimized variant  */
#define RAD4_BFLY_STG_FX_MVE_SUBOPT(sz, rot1, mls, rot2, mla)           \
    ".p2align 2                                                      \n"\
    "   wls                     lr, %[count], 1f                     \n"\
    "   vldrw.32                q1, [%[pSrcA0]]                      \n"\
    "   vldrw.32                q6, [%[pSrcA2]]                      \n"\
    /* low overhead loop start */                                       \
    "2:                                                              \n"\
    /* = R(xa + xc), I(ya + yc) */                                      \
    "   vhadd.s"#sz"            q0, q1, q6                           \n"\
    /* q4 = yb2, xb2, yb1, xb1 */                                       \
    "    vldrw.32               q4, [%[pSrcA1]]                      \n"\
    /* = R(xa - xc), I(ya - yc)  */                                     \
    "   vhsub.s"#sz"            q2, q1, q6                           \n"\
    /* q5 = yd2, xd2, yd1, xd1  */                                      \
    "   vldrw.32                q5, [%[pSrcA3]]                      \n"\
    /* = R(xb + xd), I(yb + yd)  */                                     \
    "   vhadd.s"#sz"            q1, q4, q5                           \n"\
    /* = R(xb - xd), I(yb - yd)  */                                     \
    "   vhsub.s"#sz"            q3, q4, q5                           \n"\
    /* xa' = (xa + xc) + (xb + xd)  */                                  \
    /* ya' = (ya + yc) + (yb + yd)   */                                 \
    /* load twiddle factors  */                                         \
    "   vldrw.32                q7, [%[t1]], #16                     \n"\
    "   vhadd.s"#sz"            q4, q0, q1                           \n"\
    "   vstrw.32                q4, [%[pSrcA0]], #16                 \n"\
    /* xc' = (xa + xc) - (xb + xd)  */                                  \
    /* yc' = (ya + yc) - (yb + yd)  */                                  \
    "   vhsub.s"#sz"            q4, q0, q1                           \n"\
    /* load twiddle factors  */                                         \
    "   vldrw.32                q5, [%[t0]], #16                     \n"\
    /* xc' = (xa-xb+xc-xd)co2 + (ya-yb+yc-yd)(si2)  */                  \
    /* yc' = (ya-yb+yc-yd)co2 - (xa-xb+xc-xd)(si2)  */                  \
    "   vqd"#mls"dh.s"#sz"      q0, q4, q5                           \n"\
    "   vhcadd.s"#sz"           q6, q2, q3, #"#rot1"                 \n"\
    "   vqd"#mla"dhx.s"#sz"     q0, q4, q5                           \n"\
    "   vstrw.32                q0, [%[pSrcA1]], #16                 \n"\
    /* xb' = (xa - xc) + (yb - yd)  */                                  \
    /* yb' = (ya - yc) - (xb - xd)  */                                  \
    /* xb' = (xa+yb-xc-yd)co1 + (ya-xb-yc+xd)(si1)  */                  \
    /* yb' = (ya-xb-yc+xd)co1 - (xa+yb-xc-yd)(si1)  */                  \
    "   vqd"#mls"dh.s"#sz"      q0, q6, q7                           \n"\
    "   vldrw.32                q1, [%[pSrcA0]]                      \n"\
    "   vqd"#mla"dhx.s"#sz"     q0, q6, q7                           \n"\
    "   vstrw.32                q0, [%[pSrcA2]], #16                 \n"\
    /* xd' = (xa - xc) - (yb - yd)  */                                  \
    /* yd' = (ya - yc) + (xb - xd)  */                                  \
    "   vhcadd.s"#sz"           q4, q2, q3, #"#rot2"                 \n"\
    /* load twiddle factors  */                                         \
    "   vldrw.32                q5, [%[t2]], #16                     \n"\
    /* xd' = (xa-yb-xc+yd)* co3 + (ya+xb-yc-xd)* (sa3)  */              \
    /* yd' = (ya+xb-yc-xd)* co3 - (xa-yb-xc+yd)* (sa3)  */              \
    "   vqd"#mls"dh.s"#sz"      q0, q4, q5                           \n"\
    "   vldrw.32                q6, [%[pSrcA2]]                      \n"\
    "   vqd"#mla"dhx.s"#sz"     q0, q4, q5                           \n"\
    "   vstrw.32                q0, [%[pSrcA3]], #16                 \n"\
    "   le                      lr, 2b                               \n"\
    /* low overhead loop end */                                         \
    "1:                                                              \n"







#if defined(ARM_CM85_OPT)

/* slothy / helight optimized version */
#define RAD4_BFLY_STG_FX_MVE(sz, rot1, mls, rot2, mla)          \
    "    .p2align             2                             \n" \
    "    sub.w                  %[count], %[count], #1      \n" \
    "    vldrw.32               q4, [%[pSrcA2]]             \n" \
    "    wls                    lr,  %[count], 1f           \n" \
    "    2:                                                 \n" \
    "    vldrw.32               q0, [%[pSrcA0]]             \n" \
    "    vhadd.s"#sz"           q7, q0, q4                  \n" \
    "    vldrw.32               q3, [%[pSrcA3]]             \n" \
    "    vhsub.s"#sz"           q4, q0, q4                  \n" \
    "    vldrw.32               q0, [%[pSrcA1]]             \n" \
    "    vhadd.s"#sz"           q1, q0, q3                  \n" \
    "    vldrw.32               q5, [%[t0]] , #16           \n" \
    "    vhsub.s"#sz"           q6, q7, q1                  \n" \
    "    vqd"#mls"dh.s"#sz"     q2, q6, q5                  \n" \
    "    vhadd.s"#sz"           q7, q7, q1                  \n" \
    "    vqd"#mla"dhx.s"#sz"    q2, q6, q5                  \n" \
    "    vldrw.32               q5, [%[t2]] , #16           \n" \
    "    vhsub.s"#sz"           q1, q0, q3                  \n" \
    "    vstrw.u32              q2, [%[pSrcA1]] , #16       \n" \
    "    vhcadd.s"#sz"          q2, q4, q1, #"#rot2"        \n" \
    "    vqd"#mls"dh.s"#sz"     q3, q2, q5                  \n" \
    "    vhcadd.s"#sz"          q6, q4, q1, #"#rot1"        \n" \
    "    vqd"#mla"dhx.s"#sz"    q3, q2, q5                  \n" \
    "    vstrw.u32              q3, [%[pSrcA3]] , #16       \n" \
    "    vldrw.32               q5, [%[t1]] , #16           \n" \
    "    vqd"#mls"dh.s"#sz"     q3, q6, q5                  \n" \
    "    vstrw.u32              q7, [%[pSrcA0]] , #16       \n" \
    "    vldrw.32               q4, [%[pSrcA2], #16]        \n" \
    "    vqd"#mla"dhx.s"#sz"    q3, q6, q5                  \n" \
    "    vstrw.u32              q3, [%[pSrcA2]] , #16       \n" \
    "    le                     lr, 2b                      \n" \
    "    1:                                                 \n" \
    "    vldrw.32               q7, [%[pSrcA0]]             \n" \
    "    vhadd.s"#sz"           q0, q7, q4                  \n" \
    "    vldrw.32               q3, [%[pSrcA3]]             \n" \
    "    vhsub.s"#sz"           q4, q7, q4                  \n" \
    "    vldrw.32               q7, [%[pSrcA1]]             \n" \
    "    vhadd.s"#sz"           q2, q7, q3                  \n" \
    "    vldrw.32               q6, [%[t0]] , #16           \n" \
    "    vhsub.s"#sz"           q5, q0, q2                  \n" \
    "    vqd"#mls"dh.s"#sz"     q1, q5, q6                  \n" \
    "    vhsub.s"#sz"           q7, q7, q3                  \n" \
    "    vqd"#mla"dhx.s"#sz"    q1, q5, q6                  \n" \
    "    vstrw.u32              q1, [%[pSrcA1]] , #16       \n" \
    "    vhadd.s"#sz"           q3, q0, q2                  \n" \
    "    vldrw.32               q5, [%[t2]] , #16           \n" \
    "    vhcadd.s"#sz"          q1, q4, q7, #"#rot2"        \n" \
    "    vqd"#mls"dh.s"#sz"     q2, q1, q5                  \n" \
    "    vldrw.32               q6, [%[t1]] , #16           \n" \
    "    vstrw.u32              q3, [%[pSrcA0]] , #16       \n" \
    "    vqd"#mla"dhx.s"#sz"    q2, q1, q5                  \n" \
    "    vhcadd.s"#sz"          q1, q4, q7, #"#rot1"        \n" \
    "    vqd"#mls"dh.s"#sz"     q5, q1, q6                  \n" \
    "    vstrw.u32              q2, [%[pSrcA3]] , #16       \n" \
    "    vqd"#mla"dhx.s"#sz"    q5, q1, q6                  \n" \
    "    vstrw.u32              q5, [%[pSrcA2]] , #16       \n"

#else
/* slothy / helight optimized version */
#define RAD4_BFLY_STG_FX_MVE(sz, rot1, mls, rot2, mla)      \
    "    src0                   .req %[pSrcA0]             \n"\
    "    src1                   .req %[pSrcA1]             \n"\
    "    src2                   .req %[pSrcA2]             \n"\
    "    src3                   .req %[pSrcA3]             \n"\
    "    twiddle0               .req %[t0]                 \n"\
    "    twiddle1               .req %[t1]                 \n"\
    "    twiddle2               .req %[t2]                 \n"\
    "    .p2align               2                          \n"\
    "    vldrw.32               q7, [src2]                 \n"\
    "    sub.w                  %[count], %[count], #1     \n"\
    "    vldrw.32               q4, [src0]                 \n"\
    "    vhadd.s"#sz"           q2, q4, q7                 \n"\
    "    vldrw.32               q0, [src1]                 \n"\
    "    vldrw.32               q5, [src3]                 \n"\
    "    vhadd.s"#sz"           q1, q0, q5                 \n"\
    "    vldrw.32               q6, [twiddle0] , #16       \n"\
    "    wls                    lr,  %[count], 1f          \n"\
    "    2:                                                \n"\
    "    vhadd.s"#sz"           q3, q2, q1                 \n"\
    "    vstrw.u32              q3, [src0] , #16           \n"\
    "    vhsub.s"#sz"           q2, q2, q1                 \n"\
    "    vqd"#mls"dh.s"#sz"     q1, q2, q6                 \n"\
    "    vldrw.32               q3, [twiddle2] , #16       \n"\
    "    vqd"#mla"dhx.s"#sz"    q1, q2, q6                 \n"\
    "    vldrw.32               q6, [twiddle1] , #16       \n"\
    "    vhsub.s"#sz"           q0, q0, q5                 \n"\
    "    vldrw.32               q5, [src3, #16]            \n"\
    "    vhsub.s"#sz"           q2, q4, q7                 \n"\
    "    vstrw.32               q1, [src1] , #16           \n"\
    "    vhcadd.s"#sz"          q4, q2, q0, #"#rot2"       \n"\
    "    vqd"#mls"dh.s"#sz"     q1, q4, q3                 \n"\
    "    vldrw.32               q7, [src2, #16]            \n"\
    "    vqd"#mla"dhx.s"#sz"    q1, q4, q3                 \n"\
    "    vstrw.32               q1, [src3] , #16           \n"\
    "    vhcadd.s"#sz"          q1, q2, q0, #"#rot1"       \n"\
    "    vqd"#mls"dh.s"#sz"     q3, q1, q6                 \n"\
    "    vldrw.32               q4, [src0]                 \n"\
    "    vhadd.s"#sz"           q2, q4, q7                 \n"\
    "    vldrw.32               q0, [src1]                 \n"\
    "    vqd"#mla"dhx.s"#sz"    q3, q1, q6                 \n"\
    "    vldrw.32               q6, [twiddle0] , #16       \n"\
    "    vhadd.s"#sz"           q1, q0, q5                 \n"\
    "    vstrw.32               q3, [src2] , #16           \n"\
    "    le                     lr, 2b                     \n"\
    "    1:                                                \n"\
    "    vhadd.s"#sz"           q3, q2, q1                 \n"\
    "    vstrw.32               q3, [src0] , #16           \n"\
    "    vhsub.s"#sz"           q2, q2, q1                 \n"\
    "    vqd"#mls"dh.s"#sz"     q1, q2, q6                 \n"\
    "    vhsub.s"#sz"           q0, q0, q5                 \n"\
    "    vqd"#mla"dhx.s"#sz"    q1, q2, q6                 \n"\
    "    vldrw.32               q3, [twiddle2] , #16       \n"\
    "    vhsub.s"#sz"           q2, q4, q7                 \n"\
    "    vstrw.32               q1, [src1] , #16           \n"\
    "    vhcadd.s"#sz"          q4, q2, q0, #"#rot2"       \n"\
    "    vqd"#mls"dh.s"#sz"     q1, q4, q3                 \n"\
    "    vldrw.32               q6, [twiddle1] , #16       \n"\
    "    vqd"#mla"dhx.s"#sz"    q1, q4, q3                 \n"\
    "    vstrw.32               q1, [src3] , #16           \n"\
    "    vhcadd.s"#sz"          q1, q2, q0, #"#rot1"       \n"\
    "    vqd"#mls"dh.s"#sz"     q3, q1, q6                 \n"\
    "    vqd"#mla"dhx.s"#sz"    q3, q1, q6                 \n"\
    "    vstrw.32               q3, [src2] , #16           \n"

#endif

#define RAD4_BFLY_STG_FX_FWD_MVE(sz)       RAD4_BFLY_STG_FX_MVE(sz, 270, mls, 90, mla)
#define RAD4_BFLY_STG_FX_BKWD_MVE(sz)      RAD4_BFLY_STG_FX_MVE(sz, 90, mla, 270, mls)
#endif

#endif  // ARM_MATH_MVEI

#endif  // ARM_CFFT_UTILS_H
