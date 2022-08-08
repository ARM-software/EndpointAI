/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_vsin_f16.c
 * Description:  Fast vectorized sine calculation for half-precision
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


#include "arm_vsin_helium_f16.h"

#if defined(ARM_FLOAT16_SUPPORTED)


/**
  @ingroup groupFastMath
 */


/**
  @addtogroup sin
  @{
 */

#if defined(ARM_MATH_MVE_FLOAT16)

/**
  @brief         Fast approximation to the trigonometric sine function for Half-precision floating-point data.
  @param[in]     pVal input value vector in radians
  @param[in]     numSamples  number of samples in each vector
  @param[out     pSinVal sin(pVal[]) vector
 */


void arm_vsin_f16_mve(const float16_t * ptheta, float16_t * pSinVal, uint32_t blockSize)
{

#ifdef USE_ASM

    VSIN_F16(ptheta, pSinVal, blockSize);

#else                           /* Helium intrinsic version */

    int32_t         loopCnt = blockSize;

    do {
        mve_pred16_t    p = vctp16q(loopCnt);
        f16x8_t         vecAngle;

        vecAngle = vld1q_z_f16(ptheta, p);

        vst1q_p(pSinVal, vsin_f16(vecAngle), p);
        /*
         * Decrement the blockSize loop counter
         * Advance vector source and destination pointers
         */
        ptheta += 8;
        pSinVal += 8;
        loopCnt -= 8;
    }
    while (loopCnt > 0);

#endif                          // USE_ASM

}

#endif                          // ARM_MATH_MVE_FLOAT16

#endif                          // ARM_FLOAT16_SUPPORTED

/**
  @} end of sin group
 */
