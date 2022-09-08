/******************************************************************************
 * @file     arm_biquad_cascade_df1_f32.h
 * @brief    arm_biquad_cascade_df1_f32 neon and helium optimized versions related
 * @version  V1.0.0
 * @date     Sept 2022
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


#ifndef _ARM_BIQUAD_CASCADE_DF1_F32_H_
#define _ARM_BIQUAD_CASCADE_DF1_F32_H_


#include "dsp/filtering_functions.h"

#if defined(ARM_MATH_NEON) && !defined(ARM_MATH_AUTOVECTORIZE)
  /**
   * @brief Instance structure for the modified Biquad coefs required by vectorized code.
   */
typedef struct {
    float32_t       coeffs[8][4];
                              /**< Points to the array of modified coefficients.
                              The array is of length 32. There is one per stage */
} arm_biquad_mod_coef_f32;
#endif


#if defined(ARM_MATH_MVEF) && !defined(ARM_MATH_AUTOVECTORIZE)

void            arm_biquad_cascade_df1_f32_mve(const arm_biquad_casd_df1_inst_f32 * S,
                                               const float32_t * pSrc,
                                               float32_t * pDst, uint32_t blockSize);



#else // NEON

#if defined(ARM_MATH_NEON)  && !defined(ARM_MATH_AUTOVECTORIZE)

void            arm_biquad_cascade_df1_mve_init_f32_neon(arm_biquad_casd_df1_inst_f32 * S,
                                                         uint8_t numStages,
                                                         const float32_t * pCoeffs,
                                                         arm_biquad_mod_coef_f32 * pCoeffsMod,
                                                         float32_t * pState);

void            arm_biquad_cascade_df1_f32_neon(const arm_biquad_casd_df1_inst_f32 * S,
                                                const float32_t * pSrc, float32_t * pDst,
                                                uint32_t blockSize);
#endif

#endif   // defined(ARM_MATH_MVEF) && !defined(ARM_MATH_AUTOVECTORIZE)
#endif                          // _ARM_BIQUAD_CASCADE_DF1_F32_H_
