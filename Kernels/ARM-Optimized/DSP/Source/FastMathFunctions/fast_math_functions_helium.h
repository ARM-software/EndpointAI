/******************************************************************************
 * @file     fast_math_functions.h
 * @brief    Public header file for CMSIS DSP Library
 * @version  V1.0.0
 * @date     5 Aug 2022
 * Target Processor: Cortex-M and Cortex-A cores
 ******************************************************************************/
/*
 * Copyright (c) 2010-2022 Arm Limited or its affiliates. All rights reserved.
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


#ifndef _FAST_MATH_FUNCTIONS_MVE_H_
#define _FAST_MATH_FUNCTIONS_MVE_H_

#include "arm_math_types.h"
#include "arm_math_memory.h"

#include "dsp/none.h"
#include "dsp/utils.h"

#include "dsp/basic_math_functions.h"


#ifdef   __cplusplus
extern          "C" {
#endif



  /**
   * @ingroup groupFastMath
   */


void            arm_vsin_q15_mve(const q15_t * ptheta, q15_t * pSinVal, uint32_t blockSize);
void            arm_vsin_q31_mve(const q31_t * ptheta, q31_t * pSinVal, uint32_t blockSize);
void            arm_vsin_f32_mve(const float32_t * ptheta, float32_t * psinVal, uint32_t blockSize);

void            arm_vcos_q15_mve(const q15_t * ptheta, q15_t * pCosVal, uint32_t blockSize);
void            arm_vcos_q31_mve(const q31_t * ptheta, q31_t * pCosVal, uint32_t blockSize);
void            arm_vcos_f32_mve(const float32_t * ptheta, float32_t * pCosVal, uint32_t blockSize);


#ifdef   __cplusplus
}
#endif
#endif                          /* ifndef _FAST_MATH_FUNCTIONS_MVE_H_ */
