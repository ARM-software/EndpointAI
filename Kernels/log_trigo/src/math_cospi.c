/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        Math_cos.c
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
#include "arm_math_fast.h"

/*  x = [ -pi .. +pi]  */
float cos_pi_c(float x)
{
    float cosVal, fract;
    int32_t index;
    float findex;

    findex = x * PERIODdiv2Pi + PERIOD0p75;
    index = (int32_t)roundf(findex);       /* fractional residual = floor */
    fract = findex - (float)index;
    index = 2 * index;

    /* Linear interpolation process */
    cosVal = sin_pi_l1[index] + fract * sin_pi_l1[index + 1];
    return (cosVal);
}

//=======================================================================================
