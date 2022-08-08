/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        FastMathFunctions.c
 * Description:  Combination of all Helium optimized fast math function source files.
  *
 * $Date:        5. Aug 2022
 * $Revision:    V1.0.0
 *
 * Target Processor: Cortex-M cores with Helium
 * -------------------------------------------------------------------- */
/*
 * Copyright (C) 2019-2022 ARM Limited or its affiliates. All rights reserved.
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

#if !defined(ARM_DSP_CONFIG_TABLES) || defined(ARM_FAST_ALLOW_TABLES)

#if !defined(ARM_DSP_CONFIG_TABLES) || defined(ARM_ALL_FAST_TABLES) || defined(ARM_TABLE_SIN_F32)
#include "arm_vcos_f32/arm_vcos_f32.c"
#endif

#if !defined(ARM_DSP_CONFIG_TABLES) || defined(ARM_ALL_FAST_TABLES) || defined(ARM_TABLE_SIN_Q15)
#include "arm_vcos_q15/arm_vcos_q15.c"
#endif

#if !defined(ARM_DSP_CONFIG_TABLES) || defined(ARM_ALL_FAST_TABLES) || defined(ARM_TABLE_SIN_Q31)
#include "arm_vcos_q31/arm_vcos_q31.c"
#endif

#if !defined(ARM_DSP_CONFIG_TABLES) || defined(ARM_ALL_FAST_TABLES) || defined(ARM_TABLE_SIN_F32)
#include "arm_vsin_f32/arm_vsin_f32.c"
#endif

#if !defined(ARM_DSP_CONFIG_TABLES) || defined(ARM_ALL_FAST_TABLES) || defined(ARM_TABLE_SIN_Q15)
#include "arm_vsin_q15/arm_vsin_q15.c"
#endif

#if !defined(ARM_DSP_CONFIG_TABLES) || defined(ARM_ALL_FAST_TABLES) || defined(ARM_TABLE_SIN_Q31)
#include "arm_vsin_q31/arm_vsin_q31.c"
#endif


#endif


