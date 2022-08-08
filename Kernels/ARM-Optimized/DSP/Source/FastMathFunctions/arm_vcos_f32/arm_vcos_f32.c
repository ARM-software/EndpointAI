/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_vcos_f32.c
 * Description:  Fast vectorized cosine calculation for single-precision
 *               floating-point values
 *
 * $Date:        05 Aug 2022
 * $Revision:    V1.0.0
 *
 * Target Processor: Cortex-M with helium
 * -------------------------------------------------------------------- */
/*
 * Copyright (C) 2010-2022 ARM Limited or its affiliates. All rights reserved.
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

#include "arm_vsin_helium_f32.h"

/**
  @ingroup groupFastMath
 */

/**
  @defgroup cos Cosine

  Computes the trigonometric cosine function using a combination of table lookup
  and linear interpolation.  There are separate functions for
  Q15, Q31, and floating-point data types.
  The input to the floating-point version is in radians while the
  fixed-point Q15 and Q31 have a scaled input with the range
  [0 +0.9999] mapping to [0 2*pi).  The fixed-point range is chosen so that a
  value of 2*pi wraps around to 0.

  The implementation is based on table lookup using 512 values together with linear interpolation.
  The steps used are:
   -# Calculation of the nearest integer table index
   -# Compute the fractional portion (fract) of the table index.
   -# The final result equals <code>(1.0f-fract)*a + fract*b;</code>

  where
  <pre>
     a = Table[index];
     b = Table[index+1];
  </pre>
 */

/**
  @addtogroup cos
  @{
 */

#if defined(ARM_MATH_MVEF)


/**
  @brief         Fast approximation to the trigonometric cosine function for floating-point data.
  @param[in]     pVal input value vector in radians
  @param[in]     numSamples  number of samples in each vector
  @param[out     pCosVal cos(pVal[]) vector
 */

void arm_vcos_f32_mve(const float32_t * ptheta, float32_t * pCosVal, uint32_t blockSize)
{

#ifdef USE_ASM

    VCOS_F32(ptheta, pCosVal, blockSize);

#else                           /* Helium intrinsic version */

    int32_t         loopCnt = blockSize;

    do {
        mve_pred16_t    p = vctp32q(loopCnt);
        f32x4_t         vecAngle;

        vecAngle = vld1q_z_f32(ptheta, p);

        vst1q_p(pCosVal, vcos_f32(vecAngle), p);
        /*
         * Decrement the blockSize loop counter
         * Advance vector source and destination pointers
         */
        ptheta += 4;
        pCosVal += 4;
        loopCnt -= 4;
    }
    while (loopCnt > 0);

#endif                          // USE_ASM

}

#endif                          // ARM_MATH_MVEF

/**
  @} end of cos group
 */
