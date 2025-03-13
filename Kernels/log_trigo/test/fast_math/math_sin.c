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
#include "arm_math_fast.h"


float sin_l1_c(float x)
{
    float sinVal, fract, in;
    int32_t index;
    float findex;
    
    x = x * 0.159154943092f;
    in = floorf(x);
    in = x - in;                            /*  to 0 .. 2 */
    in = PERIOD * in;
    findex = PERIOD + in;
    index = (int32_t)roundf(findex);       /* fractional residual = floor */
    fract = findex - (float)index;
    index = 2 * index;
    
    /* Linear interpolation process */
    sinVal = sin_l1[index] + fract * sin_l1[index + 1];
    return (sinVal);
}

//=======================================================================================

