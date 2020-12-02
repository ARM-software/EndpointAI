/* ----------------------------------------------------------------------
 * Project:      mve vs neon conversion
 * Title:        opus_xcorr.h
 * Description:  opus xcorr sample to illustrate Neon to Helium comversion
 *
 * $Date:        Jul 2020
 * $Revision:    V.0.0.1
 *
 * Target Processor: Cortex-M cores with MVE support
 * -------------------------------------------------------------------- */
/*
 * Copyright (C) 2010-2020 ARM Limited or its affiliates. All rights reserved.
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

#ifndef MATH_H
#define MATH_H

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




#define INVSQRT_MAGIC_F32           0x5f3759df


/*
 *  Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */
 
#define PART_LEN        64             /* Length of partition. */
#define PART_LEN_SHIFT  7              /* Length of (PART_LEN * 2) in base 2. */
#define PART_LEN1       (PART_LEN + 1)  /* Unique fft coefficients. */



#if __ARM_FEATURE_NEON
float32x4_t     vlogq_neon_f32(float32x4_t x);
float32x4_t     vexpq_neon_f32(float32x4_t x);
float32x4_t     vsinq_neon_f32(float32x4_t val);
float32x4_t     vinvsqrtq_neon_f32(float32x4_t x);
float32x4_t     vinvq_neon_f32(float32x4_t x);
void            ScaleErrorSignalNEON_partial(float *aecxPow, float ef[2][PART_LEN1]);
#endif


#if __ARM_FEATURE_MVE
float32x4_t     vlogq_helium_f32(float32x4_t x);
float32x4_t     vexpq_helium_f32(float32x4_t x);
float32x4_t     vsinq_helium_f32(float32x4_t val);
float32x4_t     vinvsqrtq_helium_f32(float32x4_t x);
float32x4_t     vinvq_helium_f32(float32x4_t x);
void            ScaleErrorSignalHELIUM_partial(float *aecxPow, float ef[2][PART_LEN1]);
#endif

#endif                          /*  VECTOR_SAMPLE_H */
