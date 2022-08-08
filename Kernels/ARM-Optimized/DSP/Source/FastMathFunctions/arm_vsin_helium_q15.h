/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_vsin_helium_q15.h
 * Description:  Fast vectorized sin/cos calculation macros for Q.15 values
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

#ifndef ARM_VSIN_HELIUM_Q15_H
#define ARM_VSIN_HELIUM_Q15_H

#include "dsp/fast_math_functions.h"
#include "arm_common_tables.h"


#if defined(ARM_MATH_MVEI)


#ifdef USE_ASM

#define VSIN_CORE_Q15(pTheta, pVal)                                                 \
        /* convert negative numbers to corresponding positive ones */               \
        "   vand.i16            q0, q0, q7                                 \n"      \
                                                                                    \
        /* Calculate the nearest index */                                           \
        /* q1 = index = (uint16_t)x >> FAST_MATH_Q15_SHIFT; */                      \
        "   vshr.s16            q1, q0, %[q15shift]                        \n"      \
        /* Calculation of fractional value */                                       \
        /* q2 = fract = (x - (index << FAST_MATH_Q15_SHIFT)) << 9; */               \
        "   vshl.s16            q2, q1, %[q15shift]                        \n"      \
        "   vsub.s16            q2, q0, q2                                 \n"      \
        "   vshl.s16            q2, q2, #9                                 \n"      \
                                                                                    \
        /* Read two nearest values of input value from the sin table */             \
        /* a = sinTable_q15[index]; */                                              \
        "   vldrh.16            q3, [%[psinTable_q15], q1, UXTW #1]        \n"      \
        "   vadd.s16            q1, q1, %[one]                             \n"      \
        /* b = sinTable_q15[index+1]; */                                            \
        "   vldrh.16            q4, [%[psinTable_q15], q1, UXTW #1]        \n"      \
                                                                                    \
        /* Linear interpolation process */                                          \
        /* q3 = sincosVal = (0x7fff-fract)*a >> 16; */                              \
        "   vsub.s16            q6, q7, q2                                 \n"      \
        "   vqdmulh.s16         q3, q3, q6                                 \n"      \
                                                                                    \
        /* schedule next load */                                                    \
        "   vldrh.16            q0, [%[" #pTheta "]], #16                  \n"      \
                                                                                    \
        /* q4 = sincosVal =  */                                                     \
        /*        (q15_t)((((q63_t)cosVal << 16) + ((q63_t)fract*b)) >> 16); */     \
        "   vqdmulh.s16         q4, q4, q2                                 \n"      \
        "   vqadd.s16           q2, q4, q3                                 \n"      \
        "   vstrh.16            q2, [%[" #pVal "]], #16                    \n"




#define VCOS_Q15(pTheta, pVal, len)                                                 \
    register unsigned loopCnt     __asm("lr") = len;                                \
                                                                                    \
    __asm volatile (                                                                \
        ".p2align 2                                                        \n"      \
        "   wlstp.16            lr, lr, 1f                                 \n"      \
                                                                                    \
        "   vdup.16             q7, %[c0x7fff]                             \n"      \
        /* preload */                                                               \
        "   vldrh.16            q0, [%[ptheta]], #16                       \n"      \
        "2:                                                                \n"      \
                                                                                    \
        /* add 0.25 (pi/2) to read sine table */                                    \
        "   vadd.u16            q0, q0, %[c1]                              \n"      \
                                                                                    \
        VSIN_CORE_Q15(pTheta, pval)                                                 \
                                                                                    \
        "   letp                lr, 2b                                     \n"      \
        "1:                                                                \n"      \
        :[ptheta] "+r"(pTheta),                                                     \
        [pval] "+r"(pVal), [loopCnt] "+r"(loopCnt)                                  \
        :[c0x7fff] "r" (0x7fff), [c1] "r"(0x2000),                                  \
        [q15shift] "i"( FAST_MATH_Q15_SHIFT),                                       \
        [psinTable_q15] "r" (sinTable_q15), [one] "r" (1)                           \
        :"q0", "q1", "q2", "q3",                                                    \
         "q6", "q7", "memory");




#define VSIN_Q15(pTheta, pVal, len)                                                 \
    register unsigned loopCnt     __asm("lr") = len;                                \
                                                                                    \
    __asm volatile (                                                                \
        ".p2align 2                                                        \n"      \
        "   wlstp.16            lr, lr, 1f                                 \n"      \
                                                                                    \
        "   vdup.16             q7, %[c0x7fff]                             \n"      \
        /* preload */                                                               \
        "   vldrh.16            q0, [%[ptheta]], #16                       \n"      \
        "2:                                                                \n"      \
                                                                                    \
        VSIN_CORE_Q15(pTheta, pval)                                                 \
                                                                                    \
        "   letp                lr, 2b                                     \n"      \
        "1:                                                                \n"      \
        :[ptheta] "+r"(pTheta),                                                     \
        [pval] "+r"(pVal), [loopCnt] "+r"(loopCnt)                                  \
        :[c0x7fff] "r" (0x7fff),                                                    \
        [q15shift] "i"( FAST_MATH_Q15_SHIFT),                                       \
        [psinTable_q15] "r" (sinTable_q15), [one] "r" (1)                           \
        :"q0", "q1", "q2", "q3", \
         "q6", "q7", "memory");


#else                           /* MVE intrinsics */


__STATIC_FORCEINLINE q15x8_t vsin_core_q15(q15x8_t vecIn)
{
    q15x8_t         vecOne = vdupq_n_s16(0x7fff);
    int16x8_t       vecIndex, vecTmp, vecTmp1;
    q15x8_t         vecSin0, vecSin1;

    /*
     * convert negative numbers to corresponding positive ones
     */
    vecIn = vandq(vecIn, vecOne);
    /*
     * Calculate the nearest index
     */
    vecIndex = vecIn >> FAST_MATH_Q15_SHIFT;
    /*
     * Calculation of fractional value
     * fract = (x - (index << FAST_MATH_Q15_SHIFT)) << 9;
     */
    vecTmp = vecIndex << FAST_MATH_Q15_SHIFT;
    vecTmp = vecIn - vecTmp;
    vecTmp = vecTmp << 9;
    /*
     * Read two nearest values of input value from the sin table
     * a = sinTable_q15[index];
     */
    vecSin0 = vldrhq_gather_shifted_offset_s16(sinTable_q15, (uint16x8_t) vecIndex);
    /*
     * b = sinTable_f32[index+1];
     */
    vecIndex = vecIndex + 1;
    vecSin1 = vldrhq_gather_shifted_offset_s16(sinTable_q15, (uint16x8_t) vecIndex);
    /*
     * Linear interpolation process
     * sinVal = (0x7fff-fract)*a >> 16;
     */
    vecTmp1 = vecOne - vecTmp;

    return vqaddq(vqdmulhq(vecSin0, vecTmp1), vqdmulhq(vecSin1, vecTmp));
}




__STATIC_FORCEINLINE q15x8_t vsin_q15(q15x8_t vecIn)
{

    return vsin_core_q15(vecIn);

}

__STATIC_FORCEINLINE q15x8_t vcos_q15(q15x8_t vecIn)
{
    /*
     * add 0.25 (pi/2) to read sine table
     */
    return vsin_core_q15(vecIn + 0x2000);
}


#endif                          // USE_ASM

#endif                          // ARM_MATH_MVEI

#endif                          // ARM_VSIN_HELIUM_Q15_H
