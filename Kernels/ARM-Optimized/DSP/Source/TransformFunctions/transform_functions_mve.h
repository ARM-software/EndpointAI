/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        transform_functions_mve.h
 * Description:  MVE transform function headers.
 *
 * $Date:        06. Mar 2023
 * $Revision:    V1.0.0
 *
 * Target Processor: Cortex-M cores
 * -------------------------------------------------------------------- */
/*
 * Copyright (C) 2023 ARM Limited or its affiliates. All rights reserved.
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


#ifndef _TRANSFORM_FUNCTIONS_MVE_H_
#define _TRANSFORM_FUNCTIONS_MVE_H_

#include "arm_math.h"

#ifdef   __cplusplus
extern "C"
{
#endif

#if defined(ARM_MATH_MVEI)
void            arm_cfft_q31_mve(const arm_cfft_instance_q31 * S,
                                 q31_t * pSrc, uint8_t ifftFlag, uint8_t bitReverseFlag);
void            arm_cfft_q15_mve(const arm_cfft_instance_q15 * S,
                                 q15_t * pSrc, uint8_t ifftFlag, uint8_t bitReverseFlag);


void            arm_rfft_q31_mve(const arm_rfft_instance_q31 * S, q31_t * pSrc, q31_t * pDst);
void            arm_rfft_q15_mve(const arm_rfft_instance_q15 * S, q15_t * pSrc, q15_t * pDst);
#endif


#if defined(ARM_MATH_MVEF)
void            arm_cfft_f32_mve(const arm_cfft_instance_f32 * S,
                                 float32_t * pSrc, uint8_t ifftFlag, uint8_t bitReverseFlag);

void            arm_rfft_fast_f32_mve(const arm_rfft_fast_instance_f32 * S,
                                      float32_t * p, float32_t * pOut, uint8_t ifftFlag);
#endif

#if defined(ARM_MATH_MVE_FLOAT16)

#include "arm_math_f16.h"

void            arm_cfft_f16_mve(const arm_cfft_instance_f16 * S,
                                 float16_t * pSrc, uint8_t ifftFlag, uint8_t bitReverseFlag);

void            arm_rfft_fast_f16_mve(const arm_rfft_fast_instance_f16 * S,
                                      float16_t * p, float16_t * pOut, uint8_t ifftFlag);
#endif


#ifdef   __cplusplus
}
#endif

#endif /* ifndef _TRANSFORM_FUNCTIONS_MVE_H_ */

