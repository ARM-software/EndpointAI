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

#define VSIN_CORE_Q31(pTheta, pVal)                                                 \
        /* convert negative numbers to corresponding positive ones */               \
        "   vand.i32            q0, q0, q7                                 \n"      \
                                                                                    \
        /* Calculate the nearest index */                                           \
        /* q1 = index = (uint32_t)x >> FAST_MATH_Q31_SHIFT; */                      \
        "   vshr.s32            q1, q0, %[q31shift]                        \n"      \
        /* Calculation of fractional value */                                       \
        /* q2 = fract = (x - (index << FAST_MATH_Q31_SHIFT)) << 9; */               \
        "   vshl.s32            q2, q1, %[q31shift]                        \n"      \
        "   vsub.s32            q2, q0, q2                                 \n"      \
        "   vshl.s32            q2, q2, #9                                 \n"      \
                                                                                    \
        /* Read two nearest values of input value from the sin table */             \
        /* a = sinTable_q31[index]; */                                              \
        "   vldrw.32            q3, [%[psinTable_q31], q1, UXTW #2]        \n"      \
        "   vadd.s32            q1, q1, %[one]                             \n"      \
        /* b = sinTable_q31[index+1]; */                                            \
        "   vldrw.32            q4, [%[psinTable_q31], q1, UXTW #2]        \n"      \
                                                                                    \
        /* Linear interpolation process */                                          \
        /* q3 = sincosVal = (0x7fffffff-fract)*a >> 32; */                          \
        "   vsub.s32            q6, q7, q2                                 \n"      \
        "   vqdmulh.s32         q3, q3, q6                                 \n"      \
                                                                                    \
        /* schedule next load */                                                    \
        "   vldrw.32            q0, [%[" #pTheta "]], #16                  \n"      \
                                                                                    \
        /* q4 = sincosVal =  */                                                     \
        /*        (q31_t)((((q63_t)cosVal << 32) + ((q63_t)fract*b)) >> 32); */     \
        "   vqdmulh.s32         q4, q4, q2                                 \n"      \
        "   vqadd.s32           q2, q4, q3                                 \n"      \
        "   vstrw.32            q2, [%[" #pVal "]], #16                    \n"




#define VCOS_Q31(pTheta, pVal, len)                                                 \
    register unsigned loopCnt     __asm("lr") = len;                                \
                                                                                    \
    __asm volatile (                                                                \
        ".p2align 2                                                        \n"      \
        "   wlstp.32            lr, lr, 1f                                 \n"      \
                                                                                    \
        "   vdup.32             q7, %[c0x7fffffff]                         \n"      \
        /* preload */                                                               \
        "   vldrw.32            q0, [%[ptheta]], #16                       \n"      \
        "2:                                                                \n"      \
                                                                                    \
        /* add 0.25 (pi/2) to read sine table */                                    \
        "   vadd.u32            q0, q0, %[c1]                              \n"      \
                                                                                    \
        VSIN_CORE_Q31(pTheta, pval)                                                 \
                                                                                    \
        "   letp                lr, 2b                                     \n"      \
        "1:                                                                \n"      \
        :[ptheta] "+r"(pTheta),                                                     \
        [pval] "+r"(pVal), [loopCnt] "+r"(loopCnt)                                  \
        :[c0x7fffffff] "r" (0x7fffffff), [c1] "r"(0x20000000),                      \
        [q31shift] "i"( FAST_MATH_Q31_SHIFT),                                       \
        [psinTable_q31] "r" (sinTable_q31), [one] "r" (1)                           \
        :"q0", "q1", "q2", "q3",                                                    \
         "q6", "q7", "memory");




#define VSIN_Q31(pTheta, pVal, len)                                                 \
    register unsigned loopCnt     __asm("lr") = len;                                \
                                                                                    \
    __asm volatile (                                                                \
        ".p2align 2                                                        \n"      \
        "   wlstp.32            lr, lr, 1f                                 \n"      \
                                                                                    \
        "   vdup.32             q7, %[c0x7fffffff]                         \n"      \
        /* preload */                                                               \
        "   vldrw.32            q0, [%[ptheta]], #16                       \n"      \
        "2:                                                                \n"      \
                                                                                    \
        VSIN_CORE_Q31(pTheta, pval)                                                 \
                                                                                    \
        "   letp                lr, 2b                                     \n"      \
        "1:                                                                \n"      \
        :[ptheta] "+r"(pTheta),                                                     \
        [pval] "+r"(pVal), [loopCnt] "+r"(loopCnt)                                  \
        :[c0x7fffffff] "r" (0x7fffffff),                                            \
        [q31shift] "i"( FAST_MATH_Q31_SHIFT),                                       \
        [psinTable_q31] "r" (sinTable_q31), [one] "r" (1)                           \
        :"q0", "q1", "q2", "q3", \
         "q6", "q7", "memory");


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
     * add 0.25 (pi/2) to read sine table
     */
    return vsin_core_q31(vecIn + 0x20000000);
}


#endif                          // USE_ASM

#endif                          // ARM_MATH_MVEI

#endif                          // ARM_VSIN_HELIUM_F32_H
