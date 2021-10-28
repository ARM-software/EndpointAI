/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library Extract
 * Title:        matrix.h
 * Description:  matrix manupulations illustrating Neon to Helium comversion
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

#ifndef MATRIX_H
#define MATRIX_H

#include <stdint.h>
#include <limits.h>
#include <float.h>
#include <math.h>
#if __ARM_FEATURE_NEON
#include <arm_neon.h>
#endif
#if __ARM_FEATURE_MVE
#include <arm_mve.h>
#include <arm_math_types.h>
#include "helium_neon_helpers.h"
#endif



#if __ARM_FEATURE_NEON
void mat_transpose_inp_4x4_neon_f32(    float32_t * mat);
void mat_transpose_inp_4x4_neon_u8(uint8_t * matrix);
void mat_multiply_4x4_neon(float32_t * A, float32_t * B, float32_t * C);
#endif

#if __ARM_FEATURE_MVE
void mat_transpose_inp_4x4_helium_f32(    float32_t * mat);
void mat_transpose_inp_4x4_helium_u8(uint8_t * matrix);
void mat_multiply_4x4_helium(float32_t * A, float32_t * B, float32_t * C);
#endif

#endif