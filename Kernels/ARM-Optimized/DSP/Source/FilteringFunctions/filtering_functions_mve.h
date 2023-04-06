/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        filtering_functions_mve.h
 * Description:  MVE filtering function headers.
 *
 * $Date:        06. Apr 2023
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


#ifndef _FILTERING_FUNCTIONS_MVE_H_
#define _FILTERING_FUNCTIONS_MVE_H_

#include "arm_math.h"

#ifdef   __cplusplus
extern          "C" {
#endif

#if defined(ARM_MATH_MVEI)
void            arm_fir_q7_mve(const arm_fir_instance_q7 * S,
                           const q7_t * pSrc, q7_t * pDst, uint32_t blockSize);

void            arm_fir_q15_mve(const arm_fir_instance_q15 * S,
                            const q15_t * pSrc, q15_t * pDst, uint32_t blockSize);

void            arm_fir_q31_mve(const arm_fir_instance_q31 * S,
                            const q31_t * pSrc, q31_t * pDst, uint32_t blockSize);

void            arm_biquad_cascade_df1_q15_mve(const arm_biquad_casd_df1_inst_q15 * S,
                                           const q15_t * pSrc,
                                           q15_t * pDst, uint32_t blockSize);

void            arm_biquad_cascade_df1_q31_mve(const arm_biquad_casd_df1_inst_q31 * S,
                                           const q31_t * pSrc,
                                           q31_t * pDst, uint32_t blockSize);

#endif


#if defined(ARM_MATH_MVEF)
void            arm_fir_f32_mve(const arm_fir_instance_f32 * S,
                            const float32_t * pSrc, float32_t * pDst, uint32_t blockSize);

void            arm_biquad_cascade_df1_f32_mve(const arm_biquad_casd_df1_inst_f32 * S,
                                           const float32_t * pSrc,
                                           float32_t * pDst, uint32_t blockSize);

#endif

#if defined(ARM_MATH_MVE_FLOAT16)

#include "arm_math_f16.h"

void            arm_fir_f16_mve(const arm_fir_instance_f16 * S,
                            const float16_t * pSrc, float16_t * pDst, uint32_t blockSize);

void            arm_biquad_cascade_df1_f16_mve(const arm_biquad_casd_df1_inst_f16 * S,
                                           const float16_t * pSrc,
                                           float16_t * pDst, uint32_t blockSize);
#endif


#ifdef   __cplusplus
}
#endif
#endif                          /* ifndef _FILTERING_FUNCTIONS_MVE_H_ */
