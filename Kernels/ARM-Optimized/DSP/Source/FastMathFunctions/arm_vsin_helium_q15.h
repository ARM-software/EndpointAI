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

/* for Cosine, add 0.25 offset (pi/2) to read sine table */
#define ADD_OFFS_COS(vecTheta)     "vadd.i16  " vecTheta ", " vecTheta ", %[cst0x2000] \n"
/* for Sine, use sine table without offset */
#define ADD_OFFS_SIN(vecTheta)

#define ADD_OFF(vecTheta, SIN_COS)    ADD_OFFS_##SIN_COS(vecTheta)



#define VSIN_CORE_Q15(pTheta, pVal, SIN_COS, len)                                   \
    register unsigned loopCnt     __asm("lr") = len;                                \
                                                                                    \
    __asm volatile (                                                                \
        "vecTheta            .req q2                                          \n"   \
                                                                                    \
        "   vmov.i16        q0, #0x7e00                                       \n"   \
        "   vmvn.i16        q1, #0x8000                                       \n"   \
                                                                                    \
        "   vldrh.u16       vecTheta, [%[ptheta]], #16                        \n"   \
        "   wlstp.16        lr, lr, 1f                                        \n"   \
        "2:                                                                   \n"   \
        /* adjust sin table offset */                                               \
        ADD_OFF("vecTheta", SIN_COS)                                                \
                                                                                    \
        /* Calculate the nearest index */                                           \
        /* index = (uint32_t)x >> FAST_MATH_Q31_SHIFT; */                           \
        "   vshr.u16        q3, vecTheta, #6                                  \n"   \
        "   vmul.i16        q2, vecTheta, %[twoPow9]                          \n"   \
        "   vbic.i16        q3, #0xfe00                                       \n"   \
        "   vand            q2, q2, q0                                        \n"   \
        /* Read two nearest values of input value from the sin table */             \
        /* a = sinTable_q15[index]; */                                              \
        "   vldrh.u16       q5, [%[psinTable_q15], q3, uxtw #1]               \n"   \
        "   veor            q4, q2, q1                                        \n"   \
        /* Linear interpolation process */                                          \
        /* sincosVal = (0x7fff-fract)*a >> 16; */                                   \
        "   vqdmulh.s16     q4, q5, q4                                        \n"   \
        "   vldrh.u16       q5, [%[sinTable_q15_plus1], q3, uxtw #1]          \n"   \
        "   vqdmulh.s16     q6, q5, q2                                        \n"   \
        /* schedule next load */                                                    \
        "   vldrh.u16       vecTheta, [%[ptheta]], #16                        \n"   \
        /* sincosVal +=  */                                                         \
        /*          ((q63_t)fract*b)) >> 16); */                                    \
        "   vqadd.s16       q6, q4, q6                                        \n"   \
        "   vstrh.16        q6, [%[pval]], #16                                \n"   \
        "   letp            lr, 2b                                            \n"   \
        "1:                                                                   \n"   \
        ".unreq vecTheta                                                      \n"   \
        :[ptheta] "+r"(pTheta),                                                     \
        [pval] "+r"(pVal), [loopCnt] "+r"(loopCnt)                                  \
        :[cst7ffffe00] "r" (0x7ffffe00), [cst0x2000] "r"(0x2000),                   \
        [q31shift] "i"( FAST_MATH_Q31_SHIFT),                                       \
        [psinTable_q15] "r" (sinTable_q15),                                         \
        [sinTable_q15_plus1] "r" (&sinTable_q15[1]),                                \
        [one] "r" (1),                                                              \
        [twoPow9] "r" (1<<9)                                                        \
        :"q0", "q1", "q2", "q3",                                                    \
         "q4", "q5", "q6", "memory");



#define VSIN_Q15(pTheta, pVal, len)   VSIN_CORE_Q15(pTheta, pVal, SIN, len)
#define VCOS_Q15(pTheta, pVal, len)   VSIN_CORE_Q15(pTheta, pVal, COS, len)


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
