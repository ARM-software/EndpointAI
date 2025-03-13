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


float atan2_l1_c(float y, float x)
{
    float z, fract, cadran;
    int32_t index;
    float findex;

    if (fabsf(y) >= fabsf(x))
    {   /* the second half of the table is for |arg| > 1 */
        z = x / y;
        findex = (float)ATAN_L1;
    }
    else
    {   /* x = -1 <=> index = 0
           x = 1 <=> index = ATAN_L1
           x:   -1       0        1
           x+1   0       1        2
           idx   0   ATAN_L1/2  ATAN_L1 */
        z = y / x;
        findex = 0;
    }

    if (x < 0)
    {
        if (y < 0) cadran = -PI;
        else cadran = PI;
    }
    else cadran = 0;
    
    findex += (z + 1.0f) * ATAN_L1div2;
    index = (int32_t)floorf(findex);
    fract = findex - (float)index;
    index = 2 * index;
    y = atan_l1[index] + fract * atan_l1[index + 1];
    y = y + cadran;

    return (y);
}

//=======================================================================================

