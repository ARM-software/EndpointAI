/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library Extract
 * Title:        vmin.h
 * Description:  Minimum value of a floating-point vector
 *
 * $Date:        18. March 2019
 * $Revision:    V1.6.0
 *
 * Target Processor: Cortex-M cores
 * -------------------------------------------------------------------- */
/*
 * Copyright (C) 2010-2021 ARM Limited or its affiliates. All rights reserved.
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

#ifndef MIN_H
#define MIN_H

#include <stdint.h>
#include <limits.h>
#include <float.h>
#if __ARM_FEATURE_NEON
#include <arm_neon.h>
#endif
#if __ARM_FEATURE_MVE
#include <arm_mve.h>
#include "helium_neon_helpers.h"
#endif

#define F32_MAX   ((float32_t)FLT_MAX)

 /**
   * @brief 8-bit fractional data type in 1.7 format.
   */
typedef int8_t  q7_t;

  /**
   * @brief 16-bit fractional data type in 1.15 format.
   */
typedef int16_t q15_t;

  /**
   * @brief 32-bit fractional data type in 1.31 format.
   */
typedef int32_t q31_t;

  /**
   * @brief 64-bit fractional data type in 1.63 format.
   */
typedef int64_t q63_t;


void            arm_min_f32(const float32_t * pSrc,
                            uint32_t blockSize, float32_t * pResult, uint32_t * pIndex);

#if __ARM_FEATURE_NEON
void            arm_min_neon_f32(const float32_t * pSrc,
                                 uint32_t blockSize,
                                 float32_t * pResult, uint32_t * pIndex);
#endif


#if __ARM_FEATURE_MVE
void            arm_min_helium_f32(const float32_t * pSrc,
                                   uint32_t blockSize,
                                   float32_t * pResult, uint32_t * pIndex);
#endif

#endif                          /*  VECTOR_SAMPLE_H */
