/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        Math_sin.c
 * Description:  sinus
 *
 * $Date:        January 2025
 * $Revision:    V.0.0.1
 *
 * Target Processor: 
 * -------------------------------------------------------------------- */
/*
 * Copyright (C) 2010-2025 ARM Limited or its affiliates. All rights reserved.
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
#include "arm_math_types.h"


#ifdef _MSC_VER 
#define _CRT_SECURE_NO_DEPRECATE
uint32_t as_uint(const float x) { return *(uint32_t*)&x;}
float as_float(const uint32_t x) { return *(float*)&x; }

float half_to_float(const uint16_t x) { // IEEE-754 16-bit floating-point format (without infinity): 1-5-10, exp-15, +-131008.0, +-6.1035156E-5, +-5.9604645E-8, 3.311 digits
    const uint32_t e = (x & 0x7C00) >> 10; // exponent
    const uint32_t m = (x & 0x03FF) << 13; // mantissa
    const uint32_t v = as_uint((float)m) >> 23; // evil log2 bit hack to count leading zeros in denormalized format
    return as_float((x & 0x8000) << 16 | (e != 0) * ((e + 112) << 23 | m) | ((e == 0) & (m != 0)) * ((v - 37) << 23 | ((m << (150 - v)) & 0x007FE000))); // sign : normalized : denormalized
}
uint16_t float_to_half(const float x) { // IEEE-754 16-bit floating-point format (without infinity): 1-5-10, exp-15, +-131008.0, +-6.1035156E-5, +-5.9604645E-8, 3.311 digits
    const uint32_t b = as_uint(x) + 0x00001000; // round-to-nearest-even: add last bit after truncated mantissa
    const uint32_t e = (b & 0x7F800000) >> 23; // exponent
    const uint32_t m = b & 0x007FFFFF; // mantissa; in line below: 0x007FF000 = 0x00800000-0x00001000 = decimal indicator flag - initial rounding
    return (b & 0x80000000) >> 16 | (e > 112) * ((((e - 112) << 10) & 0x7C00) | m >> 13) | ((e < 113) & (e > 101)) * ((((0x007FF000 + m) >> (125 - e)) + 1) >> 1) | (e > 143) * 0x7FFF; // sign : normalized : denormalized : saturate
}
float invsqrt_l1_c(float x)
{
    x = half_to_float(float_to_half(x));
    x = 1 / sqrtf(x);
    x = half_to_float(float_to_half(x));

    return (x);
}
#else



#include <math.h>

float invsqrt_l1_c(float x) // 22 cycles
{
    __asm volatile(
    "  vcvtb.f16.f32 s0, s0 \n"
    "  vmov.f16 s2, #1.0    \n"
    "  vsqrt.f16 s0, s0     \n"
    "  vdiv.f16 s2,s0       \n"
    "  vcvtb.f32.f16 s0,s2  \n"
        :
        :
        :
    );
    return x;
}
#endif



//=======================================================================================

