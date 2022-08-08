/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_vsin_helium_f32.h
 * Description:  Fast vectorized sin/cos calculation macros for
 *               single-precision floating-point values
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


#if defined(ARM_MATH_MVEF)


#ifdef USE_ASM

#define VSIN_CORE_F32(pTheta, pVal, offset, len)                               \
    register unsigned loopCnt     __asm("lr") = len;                           \
                                                                               \
    __asm volatile (                                                           \
        ".p2align 2                                                     \n"    \
        "   wlstp.32            lr, lr, 1f                              \n"    \
                                                                               \
        "   vdup.32             q7, %[c0x1fff]                          \n"    \
        "   vdup.32             q6, %[onef32]                           \n"    \
                                                                               \
        /* preload */                                                          \
        "   vldrw.32            q0, [%[ptheta]], #16                    \n"    \
        "   vdup.32             q1, %[c0]                               \n"    \
        "2:                                                             \n"    \
        /* input x is in radians */                                            \
        /* Scale the input to [0 1] range from [0 2*PI] */                     \
        /* divide input by 2*pi (add 0.25 (pi/2) to read sine table) */        \
        /* in = x * 0.159154943092f + 0.25f */                                 \
        "   vfmas.f32           q1, q0, %[c1]                           \n"    \
                                                                               \
        /* Calculation of floor value of input */                              \
        /* n = (int32_t) in; */                                                \
        /* Make negative values towards -infinity */                           \
        /* if (in < 0.0f) */                                                   \
        /*   n--; */                                                           \
        "   vcvtm.s32.f32       q3, q1                                  \n"    \
        /* convert back to float */                                            \
        "   vcvt.f32.s32        q2, q3                                  \n"    \
        /* Map input value to [0 1] */                                         \
        /* in = in - (float32_t) n; */                                         \
        "   vsub.f32             q2, q1, q2                             \n"    \
                                                                               \
        /* Calculation of index of the table */                                \
        /* findex = (float32_t) FAST_MATH_TABLE_SIZE * in; */                  \
        "   vmul.f32            q3, q2, %[tblSzf32]                     \n"    \
                                                                               \
        /* index = ((uint16_t)findex) & 0x1ff; */                              \
        "   vcvt.s32.f32        q5, q3                                  \n"    \
        "   vand.s32            q5, q5, q7                              \n"    \
                                                                               \
        /* fractional value calculation */                                     \
        /* q3 = fract = findex - (float32_t) index; */                         \
        "   vcvt.f32.s32        q1, q5                                  \n"    \
        "   vsub.f32            q3, q3, q1                              \n"    \
                                                                               \
        /* Read two nearest values of input value from the cos table */        \
        /* q1 = a = sinTable_f32[index]; */                                    \
        "   vldrw.32            q1, [%[psinTable_f32], q5, UXTW #2]     \n"    \
        /* q2 = b = sinTable_f32[index+1]; */                                  \
        /* (q4 = 1.0f-fract) */                                                \
        "   vsub.f32            q4, q6, q3                              \n"    \
        "   vldrw.32            q2, [%[psinTable_f32_1], q5, UXTW #2]   \n"    \
                                                                               \
        /* q2 = fract*b; */                                                    \
        "   vmul.f32            q2, q3, q2                              \n"    \
        "   vldrw.32            q0, [%[ptheta]], #16                    \n"    \
        /* Linear interpolation process */                                     \
        /* cosVal = (1.0f-fract)*a + fract*b; */                               \
        "   vfma.f32            q2, q4, q1                              \n"    \
                                                                               \
        /* schedule next load */                                               \
        "   vstrw.32            q2, [%[pval]], #16                      \n"    \
        "   vdup.32             q1, %[c0]                               \n"    \
                                                                               \
        "   letp                lr, 2b                                  \n"    \
        "1:                                                             \n"    \
        :[ptheta] "+r"(pTheta),                                                \
        [pval] "+r"(pVal), [loopCnt] "+r"(loopCnt)                             \
        :[c0] "r"((float)(1.0 / (2.0 * PI))),[c1] "r"(offset),                 \
        [tblSzf32] "r"( (float32_t) FAST_MATH_TABLE_SIZE),                     \
        [c0x1fff] "r" (0x1ff), [psinTable_f32] "r" (sinTable_f32),             \
        [psinTable_f32_1] "r" (&sinTable_f32[1]),                              \
        [onef32] "r" (1.0f)                                                    \
        :"q0", "q1", "q2", "q3",                                               \
         "q4", "q5", "q6", "q7",                                               \
         "memory");



#define VSIN_F32(pTheta, pVal, len)   VSIN_CORE_F32(pTheta, pVal, 0.0f, len)

#define VCOS_F32(pTheta, pVal, len)   VSIN_CORE_F32(pTheta, pVal, 0.25f, len)


#else                           /* MVE intrinsics */


__STATIC_FORCEINLINE f32x4_t vsin_core_f32(f32x4_t vecIn, float32_t offset)
{
    q31x4_t         vMsk0x1ff = vdupq_n_s32(0x1ff);
    f32x4_t         vOne = vdupq_n_f32(1.0f);
    f32x4_t         vTmp0, vTmp1;
    f32x4_t         vSinLo, vSinHi;
    int32x4_t       vTmpInt;

    /*
     * input x is in radians
     * Scale the input to [0 1] range from [0 2*PI]
     * divide input by 2*pi (add 0.25 (pi/2) to read sine table)
     */

    vTmp0 = vdupq_n_f32(1.0 / (2.0 * PI));
    vTmp0 = vfmasq_n_f32(vTmp0, vecIn, offset);
    /*
     * Calculation of floor value of input
     */
    vTmpInt = vcvtmq_s32_f32(vTmp0);
    vTmp1 = vcvtq_f32_s32(vTmpInt);
    /*
     * Map input value to [0 1]
     */
    vTmp1 = vTmp0 - vTmp1;
    /*
     * Calculation of index of the table
     */
    vTmp0 = vTmp1 * (float32_t) FAST_MATH_TABLE_SIZE;
    /*
     * index = ((uint16_t)findex) & 0x1ff;
     */
    vTmpInt = vcvtq_s32_f32(vTmp0);
    vTmpInt = vTmpInt & vMsk0x1ff;
    /*
     * fractional value calculation
     * fract = findex - (float32_t) index;
     */
    vTmp1 = vcvtq_f32_s32(vTmpInt);
    vTmp0 = vTmp0 - vTmp1;
    /*
     * Read two nearest values of input value from the cos table
     * a = sinTable_f32[index];
     */
    vSinLo = vldrwq_gather_shifted_offset_f32(sinTable_f32, (uint32x4_t)vTmpInt);
    /*
     * b = sinTable_f32[index+1];
     */
    vTmpInt = vTmpInt + 1;
    vSinHi = vldrwq_gather_shifted_offset_f32(sinTable_f32, (uint32x4_t)vTmpInt);
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


__STATIC_FORCEINLINE f32x4_t vsin_f32(f32x4_t vecIn)
{
    return vsin_core_f32(vecIn, 0.0f);
}


__STATIC_FORCEINLINE f32x4_t vcos_f32(f32x4_t vecIn)
{
    /* add 0.25 offset (pi/2) to sine table */
    return vsin_core_f32(vecIn, 0.25f);
}


#endif                          // USE_ASM

#endif                          // ARM_MATH_MVEF

#endif                          // ARM_VSIN_HELIUM_F32_H
