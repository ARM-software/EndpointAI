/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_vsin_helium_f16.h
 * Description:  Fast vectorized sin/cos calculation macros for
 *               half-precision floating-point values
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

#ifndef ARM_VSIN_HELIUM_F16_H
#define ARM_VSIN_HELIUM_F16_H

#include "dsp/fast_math_functions_f16.h"
#include "arm_common_tables_f16.h"


#if defined(ARM_FLOAT16_SUPPORTED)

/* Tables for Fast Math Sine and Cosine */
#if !defined(ARM_DSP_CONFIG_TABLES) || defined(ARM_ALL_FAST_TABLES) || defined(ARM_TABLE_SIN_F16)
extern const float16_t sinTable_f16[FAST_MATH_TABLE_SIZE + 1];
#endif                          /* !defined(ARM_DSP_CONFIG_TABLES) defined(ARM_ALL_FAST_TABLES) */


#if defined(ARM_MATH_MVE_FLOAT16)


#ifdef USE_ASM

#define VSIN_CORE_F16(pTheta, pVal, offset, len)                               \
    register unsigned loopCnt     __asm("lr") = len;                           \
                                                                               \
    __asm volatile (                                                           \
        ".p2align 2                                                     \n"    \
        "   wlstp.16            lr, lr, 1f                              \n"    \
                                                                               \
        "   vdup.16             q7, %[c0x1fff]                          \n"    \
        "   vdup.16             q6, %[onef16]                           \n"    \
                                                                               \
        /* preload */                                                          \
        "   vldrh.16            q0, [%[ptheta]], #16                    \n"    \
        "   vdup.16             q1, %[c0]                               \n"    \
        "2:                                                             \n"    \
        /* input x is in radians */                                            \
        /* Scale the input to [0 1] range from [0 2*PI] */                     \
        /* divide input by 2*pi (add 0.25 (pi/2) to read sine table) */        \
        /* in = x * 0.159154943092f + 0.25f */                                 \
        "   vfmas.f16           q1, q0, %[c1]                           \n"    \
                                                                               \
        /* Calculation of floor value of input */                              \
        /* n = (int16_t) in; */                                                \
        /* Make negative values towards -infinity */                           \
        /* if (in < 0.0f) */                                                   \
        /*   n--; */                                                           \
        "   vcvtm.s16.f16       q3, q1                                  \n"    \
        /* convert back to float */                                            \
        "   vcvt.f16.s16        q2, q3                                  \n"    \
        /* Map input value to [0 1] */                                         \
        /* in = in - (float16_t) n; */                                         \
        "   vsub.f16             q2, q1, q2                             \n"    \
                                                                               \
        /* Calculation of index of the table */                                \
        /* findex = (float16_t) FAST_MATH_TABLE_SIZE * in; */                  \
        "   vmul.f16            q3, q2, %[tblSzf16]                     \n"    \
                                                                               \
        /* index = ((uint16_t)findex) & 0x1ff; */                              \
        "   vcvt.s16.f16        q5, q3                                  \n"    \
        "   vand.s16            q5, q5, q7                              \n"    \
                                                                               \
        /* fractional value calculation */                                     \
        /* q3 = fract = findex - (float16_t) index; */                         \
        "   vcvt.f16.s16        q1, q5                                  \n"    \
        "   vsub.f16            q3, q3, q1                              \n"    \
                                                                               \
        /* Read two nearest values of input value from the cos table */        \
        /* q1 = a = sinTable_f16[index]; */                                    \
        "   vldrh.16            q1, [%[psinTable_f16], q5, UXTW #1]     \n"    \
        /* q2 = b = sinTable_f16[index+1]; */                                  \
        /* (q4 = 1.0f-fract) */                                                \
        "   vsub.f16            q4, q6, q3                              \n"    \
        "   vldrh.16            q2, [%[psinTable_f16_1], q5, UXTW #1]   \n"    \
                                                                               \
        /* q2 = fract*b; */                                                    \
        "   vmul.f16            q2, q3, q2                              \n"    \
        "   vldrh.16            q0, [%[ptheta]], #16                    \n"    \
        /* Linear interpolation process */                                     \
        /* cosVal = (1.0f-fract)*a + fract*b; */                               \
        "   vfma.f16            q2, q4, q1                              \n"    \
                                                                               \
        /* schedule next load */                                               \
        "   vstrh.16            q2, [%[pval]], #16                      \n"    \
        "   vdup.16             q1, %[c0]                               \n"    \
                                                                               \
        "   letp                lr, 2b                                  \n"    \
        "1:                                                             \n"    \
        :[ptheta] "+r"(pTheta),                                                \
        [pval] "+r"(pVal)                                                      \
        :[c0] "r"((float16_t)(1.0 / (2.0 * PI))),[c1] "r"(offset),             \
        [tblSzf16] "r"( (float16_t) FAST_MATH_TABLE_SIZE),                     \
        [c0x1fff] "r" (0x1ff), [psinTable_f16] "r" (sinTable_f16),             \
        [loopCnt] "r"(loopCnt), [psinTable_f16_1] "r" (&sinTable_f16[1]),      \
        [onef16] "r" (1.0f16)                                                  \
        :"q0", "q1", "q2", "q3",                                               \
         "q4", "q5", "q6", "q7",                                               \
         "memory");



#define VSIN_F16(pTheta, pVal, len)   VSIN_CORE_F16(pTheta, pVal, 0.0f16, len)

#define VCOS_F16(pTheta, pVal, len)   VSIN_CORE_F16(pTheta, pVal, 0.25f16, len)


#else                           /* MVE intrinsics */


__STATIC_FORCEINLINE f16x8_t vsin_core_f16(f16x8_t vecIn, float16_t offset)
{
    uint16x8_t      vMsk0x1ff = vdupq_n_s16(0x1ff);
    f16x8_t         vOne = vdupq_n_f16(1.0f16);
    f16x8_t         vTmp0, vTmp1;
    f16x8_t         vSinLo, vSinHi;
    int16x8_t       vTmpInt;

    /*
     * input x is in radians
     * Scale the input to [0 1] range from [0 2*PI]
     * divide input by 2*pi (add 0.25 (pi/2) to read sine table)
     */

    vTmp0 = vdupq_n_f16((float16_t) (1.0 / (2.0 * PI)));
    vTmp0 = vfmasq_n_f16(vTmp0, vecIn, offset);
    /*
     * Calculation of floor value of input
     */
    vTmpInt = vcvtmq_s16_f16(vTmp0);
    vTmp1 = vcvtq_f16_s16(vTmpInt);
    /*
     * Map input value to [0 1]
     */
    vTmp1 = vTmp0 - vTmp1;
    /*
     * Calculation of index of the table
     */
    vTmp0 = vTmp1 * (float16_t) FAST_MATH_TABLE_SIZE;
    /*
     * index = ((uint16_t)findex) & 0x1ff;
     */
    vTmpInt = vcvtq_s16_f16(vTmp0);
    vTmpInt = vTmpInt & vMsk0x1ff;
    /*
     * fractional value calculation
     * fract = findex - (float16_t) index;
     */
    vTmp1 = vcvtq_f16_s16(vTmpInt);
    vTmp0 = vTmp0 - vTmp1;
    /*
     * Read two nearest values of input value from the cos table
     * a = sinTable_f16[index];
     */
    vSinLo = vldrhq_gather_shifted_offset_f16(sinTable_f16, (uint16x8_t) vTmpInt);
    /*
     * b = sinTable_f16[index+1];
     */
    vTmpInt = vTmpInt + 1;
    vSinHi = vldrhq_gather_shifted_offset_f16(sinTable_f16, (uint16x8_t) vTmpInt);
    /*
     * 1.0f - fract
     */
    vTmp1 = vOne - vTmp0;
    /*
     * fract * b;
     */
    vTmp0 = vTmp0 * vSinHi;
    /*
     * Linear interpolation process
     * cosVal = (1.0f-fract)*a + fract*b;
     */
    vTmp0 = vfmaq(vTmp0, vSinLo, vTmp1);

    return vTmp0;
}


__STATIC_FORCEINLINE f16x8_t vsin_f16(f16x8_t vecIn)
{
    return vsin_core_f16(vecIn, 0.0f16);
}


__STATIC_FORCEINLINE f16x8_t vcos_f16(f16x8_t vecIn)
{
    /* add 0.25 offset (pi/2) to sine table */
    return vsin_core_f16(vecIn, 0.25f16);
}


#endif                          // USE_ASM

#endif                          // ARM_MATH_MVE_FLOAT16

#endif                          // ARM_FLOAT16_SUPPORTED

#endif                          // ARM_VSIN_HELIUM_F16_H
