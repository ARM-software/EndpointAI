/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_vsin_q31.c
 * Description:  Fast vectorized sine calculation for Q.31
 *               fixed-point values
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

#include "arm_vsin_helium_q31.h"

/**
  @ingroup groupFastMath
 */


/**
  @addtogroup sin
  @{
 */

#if defined(ARM_MATH_MVEI)

/**
  @brief         Fast approximation to the trigonometric sine function for Q31 data.
  @param[in]     pVal  Scaled input value in radians
  @param[out     pSinVal sin(pVal[]) vector

  The Q31 input value is in the range [0 +0.9999] and is mapped to a radian value in the range [0 2*PI).
 */

void arm_vsin_q31_mve(const q31_t * ptheta, q31_t * pSinVal, uint32_t blockSize)
{
#ifdef USE_ASM

    VSIN_Q31(ptheta, pSinVal, blockSize);

#else                           /* Helium intrinsic version */

    int32_t         loopCnt = blockSize;

    do {
        mve_pred16_t    p = vctp32q(loopCnt);
        q31x4_t         vecAngle;

        vecAngle = vld1q_z(ptheta, p);

        vst1q_p(pSinVal, vsin_q31(vecAngle), p);
        /*
         * Decrement the blockSize loop counter
         * Advance vector source and destination pointers
         */
        ptheta += 4;
        pSinVal += 4;
        loopCnt -= 4;
    }
    while (loopCnt > 0);

#endif                          // USE_ASM

}

#endif                          // ARM_MATH_MVEI

/**
  @} end of sin group
 */
