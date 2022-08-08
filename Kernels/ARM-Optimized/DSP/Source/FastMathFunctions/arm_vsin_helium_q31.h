/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_vsin_helium_q31.h
 * Description:  Fast vectorized sin/cos calculation macros for Q.31 values
 *
 * $Date:        05 Aug 2022
 * $Revision:    V1.0.0
 *
 * Target Processor: Cortex-M with helium
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

#ifndef ARM_VSIN_HELIUM_F32_H
#define ARM_VSIN_HELIUM_F32_H

#include "dsp/fast_math_functions.h"
#include "arm_common_tables.h"


#if defined(ARM_MATH_MVEI)


#ifdef USE_ASM


/* for Cosine, add 0.25 offset (pi/2) to read sine table */
#define ADD_OFFS_COS(vecTheta)     "vadd.i32  " vecTheta ", " vecTheta ", %[cst0x20000000] \n"
/* for Sine, use sine table without offset */
#define ADD_OFFS_SIN(vecTheta)

#define ADD_OFF(vecTheta, SIN_COS)    ADD_OFFS_##SIN_COS(vecTheta)



#define VSIN_CORE_Q31(pTheta, pVal, SIN_COS, len)                                   \
    register unsigned loopCnt     __asm("lr") = len;                                \
                                                                                    \
    __asm volatile (                                                                \
        "vecTheta            .req q3                                    \n"         \
                                                                                    \
        "   vmov.i32        q0, #0x1ff                                  \n"         \
        "   vdup.32         q1, %[cst7ffffe00]                          \n"         \
        "   vmvn.i32        q2, #0x80000000                             \n"         \
                                                                                    \
        "   vldrw.u32       vecTheta, [%[ptheta]], #16                  \n"         \
        "   wlstp.32        lr, lr, 1f                                  \n"         \
                                                                                    \
        "2:                                                             \n"         \
        /* adjust sin table offset */                                               \
        ADD_OFF("vecTheta", SIN_COS)                                                \
        /* Calculate the nearest index */                                           \
        /* index = (uint32_t)x >> FAST_MATH_Q31_SHIFT; */                           \
        "   vshr.u32        q4, vecTheta, %[q31shift]                   \n"         \
        /* Calculation of fractional value */                                       \
        /* fract = (x - (index << FAST_MATH_Q31_SHIFT)) << 9; */                    \
        "   vand            q4, q4, q0                                  \n"         \
        "   vmul.i32        q3, q3, %[twoPow9]                          \n"         \
        "   vand            q3, q3, q1                                  \n"         \
        /* Read two nearest values of input value from the sin table */             \
        /* a = sinTable_q31[index]; */                                              \
        "   vldrw.u32       q6, [%[psinTable_q31], q4, uxtw #2]         \n"         \
        "   veor            q5, q3, q2                                  \n"         \
        /* Linear interpolation process */                                          \
        /* sincosVal = (0x7fffffff-fract)*a >> 32; */                               \
        "   vqdmulh.s32     q5, q6, q5                                  \n"         \
        "   vldrw.u32       q6, [%[sinTable_q31_plus1], q4, uxtw #2]    \n"         \
        /* sincosVal +=  */                                                         \
        /*         ((q63_t)fract*b)) >> 32); */                                     \
        "   vqdmulh.s32     q6, q6, q3                                  \n"         \
        /* schedule next load */                                                    \
        "   vldrw.u32       vecTheta, [%[ptheta]], #16                  \n"         \
        "   vqadd.s32       q5, q5, q6                                  \n"         \
        "   vstrw.32        q5, [%[pval]], #16                          \n"         \
        "   letp            lr, 2b                                      \n"         \
        "1:                                                             \n"         \
        ".unreq vecTheta                                                \n"         \
        :[ptheta] "+r"(pTheta),                                                     \
        [pval] "+r"(pVal), [loopCnt] "+r"(loopCnt)                                  \
        :[cst7ffffe00] "r" (0x7ffffe00), [cst0x20000000] "r"(0x20000000),           \
        [q31shift] "i"( FAST_MATH_Q31_SHIFT),                                       \
        [psinTable_q31] "r" (sinTable_q31),                                         \
        [sinTable_q31_plus1] "r" (&sinTable_q31[1]),                                \
        [twoPow9] "r" (1<<9)                                                        \
        :"q0", "q1", "q2", "q3",                                                    \
         "q4", "q5", "q6", "memory");



#define VSIN_Q31(pTheta, pVal, len)   VSIN_CORE_Q31(pTheta, pVal, SIN, len)
#define VCOS_Q31(pTheta, pVal, len)   VSIN_CORE_Q31(pTheta, pVal, COS, len)


#else                           /* MVE intrinsics */


__STATIC_FORCEINLINE q31x4_t vsin_core_q31(q31x4_t vecIn)
{
    q31x4_t         vecOne = vdupq_n_s32(0x7fffffff);
    int32x4_t       vecIndex, vecTmp, vecTmp1;
    q31x4_t         vecSin0, vecSin1;

    /*
     * convert negative numbers to corresponding positive ones
     */
    vecIn = vandq(vecIn, vecOne);
    /*
     * Calculate the nearest index
     */
    vecIndex = vecIn >> FAST_MATH_Q31_SHIFT;
    /*
     * Calculation of fractional value
     * fract = (x - (index << FAST_MATH_Q31_SHIFT)) << 9;
     */
    vecTmp = vecIndex << FAST_MATH_Q31_SHIFT;
    vecTmp = vecIn - vecTmp;
    vecTmp = vecTmp << 9;
    /*
     * Read two nearest values of input value from the sin table
     * a = sinTable_q31[index];
     */
    vecSin0 = vldrwq_gather_shifted_offset_s32(sinTable_q31, (uint32x4_t) vecIndex);
    /*
     * b = sinTable_f32[index+1];
     */
    vecIndex = vecIndex + 1;
    vecSin1 = vldrwq_gather_shifted_offset_s32(sinTable_q31, (uint32x4_t) vecIndex);
    /*
     * Linear interpolation process
     * sinVal = (0x7fffffff-fract)*a >> 32;
     */
    vecTmp1 = vecOne - vecTmp;

    return vqaddq(vqdmulhq(vecSin0, vecTmp1), vqdmulhq(vecSin1, vecTmp));
}


__STATIC_FORCEINLINE q31x4_t vsin_q31(q31x4_t vecIn)
{

    return vsin_core_q31(vecIn);

}

__STATIC_FORCEINLINE q31x4_t vcos_q31(q31x4_t vecIn)
{
    /*
     *
     */
    return vsin_core_q31(vecIn + 0x20000000);
}


#endif                          // USE_ASM

#endif                          // ARM_MATH_MVEI

#endif                          // ARM_VSIN_HELIUM_F32_H
