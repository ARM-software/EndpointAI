/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_vcos_q15.c
 * Description:  Fast vectorized cosine calculation for Q.15 values
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

#include "arm_vsin_helium_q15.h"

/**
  @ingroup groupFastMath
 */


/**
  @addtogroup cos
  @{
 */

#if defined(ARM_MATH_MVEI)

/**
  @brief         Fast approximation to the trigonometric cosine function for Q.15 data.
  @param[in]     ptheta Scaled input value in radians
  @param[out     pCosVal cos(pVal[]) vector

  The Q15 input value is in the range [0 +0.9999] and is mapped to a radian value in the range [0 2*PI).
 */

void arm_vcos_q15_mve(const q15_t * ptheta, q15_t * pCosVal, uint32_t blockSize)
{
#ifdef USE_ASM

    VCOS_Q15(ptheta, pCosVal, blockSize);

#else                           /* Helium intrinsic version */

    int32_t         loopCnt = blockSize;

    do {
        mve_pred16_t    p = vctp16q(loopCnt);
        q15x8_t         vecAngle;

        vecAngle = vld1q_z(ptheta, p);

        vst1q_p(pCosVal, vcos_q15(vecAngle), p);
        /*
         * Decrement the blockSize loop counter
         * Advance vector source and destination pointers
         */
        ptheta += 8;
        pCosVal += 8;
        loopCnt -= 8;
    }
    while (loopCnt > 0);

#endif                          // USE_ASM

}

#endif                          // ARM_MATH_MVEI

/**
  @} end of cos group
 */
