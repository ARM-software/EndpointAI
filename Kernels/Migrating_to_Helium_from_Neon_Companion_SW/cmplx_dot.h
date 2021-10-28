/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library Extract
 * Title:        cmplx_dot.h
 * Description:  Single Floating-point and Q.15 Fixed point complex dot products
 *
 * $Date:        Jul 2020
 * $Revision:    V.0.0.1
 *
 * Target Processor: Cortex-M cores with MVE support
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

#ifndef CMPLX_DOT_H
#define CMPLX_DOT_H

#include <stdint.h>
#if __ARM_FEATURE_NEON
#include <arm_neon.h>
#endif
#if __ARM_FEATURE_MVE
#include <arm_mve.h>
#include "helium_neon_helpers.h"
#endif

#define CMPLX_PAIR              2

 /**
   * @brief 8-bit fractional data type in 1.7 format.
   */
  typedef int8_t q7_t;

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
  
#if __ARM_FEATURE_NEON


  
void arm_cmplx_dot_prod_neon_f32(const float32_t * pSrcA,
                            const float32_t * pSrcB,
                            uint32_t numSamples,
                            float32_t * realResult, float32_t * imagResult);

void arm_cmplx_dot_prod_neon_q15(const q15_t * pSrcA,
                            const q15_t * pSrcB,
                            uint32_t numSamples, q31_t * realResult, q31_t * imagResult);
#endif    


#if __ARM_FEATURE_MVE


void arm_cmplx_dot_prod_helium_f32(const float32_t * pSrcA,
                            const float32_t * pSrcB,
                            uint32_t numSamples,
                            float32_t * realResult, float32_t * imagResult);

void arm_cmplx_dot_prod_helium_q15(const q15_t * pSrcA,
                            const q15_t * pSrcB,
                            uint32_t numSamples, q31_t * realResult, q31_t * imagResult);
#endif

#endif                          /*  VECTOR_SAMPLE_H */
