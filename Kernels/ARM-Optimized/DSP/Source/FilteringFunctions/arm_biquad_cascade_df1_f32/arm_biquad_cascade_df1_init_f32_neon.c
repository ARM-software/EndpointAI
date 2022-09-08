/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_biquad_cascade_df1_init_f32_neon.c
 * Description:  Floating-point Biquad cascade DirectFormI(DF1) filter initialization function for Neon
 *               (Duplicated from the MVE version as CMSIS requires explicit Helium ARM_MATH_MVEF compile option)
 *
 * $Date:        Sep 2022
 * $Revision:    V1.0.0
 *
 * Target Processor: Cortex-M and Cortex-A cores
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

#include "dsp/filtering_functions.h"
#include "arm_biquad_cascade_df1_f32.h"


#if defined(ARM_MATH_NEON) && !defined(ARM_MATH_AUTOVECTORIZE)

static void generateCoefsFastBiquadF32(float32_t b0, float32_t b1, float32_t b2, float32_t a1, float32_t a2,
                                arm_biquad_mod_coef_f32 * newCoef)
{
    float32_t coeffs[4][8] = {
        {0, 0, 0, b0, b1, b2, a1, a2},
        {0, 0, b0, b1, b2, 0, a2, 0},
        {0, b0, b1, b2, 0, 0, 0, 0},
        {b0, b1, b2, 0, 0, 0, 0, 0},
    };

    for (int i = 0; i < 8; i++)
    {
        coeffs[1][i] += a1 * coeffs[0][i];
        coeffs[2][i] += a1 * coeffs[1][i] + a2 * coeffs[0][i];
        coeffs[3][i] += a1 * coeffs[2][i] + a2 * coeffs[1][i];

        /*
         * transpose
         */
        newCoef->coeffs[i][0] = (float32_t) coeffs[0][i];
        newCoef->coeffs[i][1] = (float32_t) coeffs[1][i];
        newCoef->coeffs[i][2] = (float32_t) coeffs[2][i];
        newCoef->coeffs[i][3] = (float32_t) coeffs[3][i];
    }
}

void arm_biquad_cascade_df1_mve_init_f32_neon(
      arm_biquad_casd_df1_inst_f32 * S,
      uint8_t numStages,
      const float32_t * pCoeffs,
      arm_biquad_mod_coef_f32 * pCoeffsMod,
      float32_t * pState)
{
    arm_biquad_cascade_df1_init_f32(S, numStages, (float32_t *)pCoeffsMod, pState);

    /* Generate SIMD friendly modified coefs */
    for (int i = 0; i < numStages; i++)
    {
        generateCoefsFastBiquadF32(pCoeffs[0], pCoeffs[1], pCoeffs[2], pCoeffs[3], pCoeffs[4], pCoeffsMod);
        pCoeffs += 5;
        pCoeffsMod++;
    }
}
#endif

/**
  @} end of BiquadCascadeDF1 group
 */
