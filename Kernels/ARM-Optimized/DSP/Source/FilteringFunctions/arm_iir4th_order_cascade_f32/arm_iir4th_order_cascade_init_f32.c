/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_iir4th_cascade_init_f32.h
 * Description:  Cascade of 4th Order IIR (experimental)
 *
 * $Date:        Aug 2022
 * $Revision:    V.1.0.0
 *
 * Target Processor: Cortex-M cores with MVE support
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

#include "arm_iir4th_order_cascade_f32.h"


/**
 * @brief  Initialization function for cascaded 4th order floating-point IIRs.
 * @param[in,out] S             points to an instance of the floating-point cascaded IIR structure.
 * @param[in]     numStages     number of 4th order stages in the filter.
 * @param[in]     pCoeffs       points to the filter coefficients.
 * @param[in]     pState        points to the state buffer.
 */

void arm_iir4_cascade_init_f32(
        arm_iir4_casd_inst_f32 * S,
        uint8_t     numStages,
  const float32_t * pCoeffs,
        float32_t * pState)
{
  /* Assign filter stages */
  S->numStages = numStages;

  /* Assign coefficient pointer */
  S->pCoeffs = pCoeffs;

  /* Clear state buffer and size is always 4 * numStages */
  memset(pState, 0, (IIR_4_ORDER * 2 * numStages) * sizeof(float32_t));

  /* Assign state pointer */
  S->pState = pState;
}


#if defined(ARM_MATH_MVEF)

/**
 * @brief  SIMDification of IIR4 coefficients
 * @param[in]     b                 Forward filter coefficients.
 * @param[in]     a                 Backward filter coefficients.
 * @param[in]     newCoef           Points to the modified coef. matrix form.
 */

/*
    compute modified coef. matrix based on

    |y[i]  |    |0    0     0     b[0]  b[1]  b[2]  b[3]  b[4] -a[0]  -a[1]  -a[2] -a[3]|       |0     |       |0     |       |0   |
    |y[i+1]| =  |0    0     b[0]  b[1]  b[2]  b[3]  b[4]  0    -a[1]  -a[2]  -a[3]  0   | -a[0].|y[i]  | -a[1].|0     | -a[2].|0   |
    |y[i+2]|    |0    b[0]  b[1]  b[2]  b[3]  b[4]  0     0    -a[2]  -a[3]   0     0   |       |y[i+1]|       |y[i]  |       |0   |
    |y[i+3]|    |b[0] b[1]  b[2]  b[3]  b[4]  0     0     0    -a[3]   0      0     0   |       |y[i+2]|       |y[i+1]|       |y[i]|

    which becomes

    |y[i]  |    |0    0     0     b[0]  b[1]  b[2]  b[3]  b[4] -a[0]  -a[1]  -a[2] -a[3] |
    |y[i+1]| =  |0    0     b[0]  c[1]  c[2]  c[3]  c[4]  c[5]  c[6]   c[7]   c[8]  c[9] |
    |y[i+2]|    |0    b[0]  d[1]  d[2]  d[3]  d[4]  d[5]  d[6]  d[7]   d[8]   d[9]  d[10]|
    |y[i+3]|    |b[0] e[1]  e[2]  e[3]  e[4]  e[5]  e[6]  e[7]  e[8]   e[9]   e[10] e[11]|

    with
            c[i] += -a[0] * b[i];
            d[i] += -a[0] * c[i] + (-a[1] * b[i]);
            e[i] += -a[0] * d[i] + (-a[1] * c[i]) + (-a[2] * b[i]);

*/

static void generateCoefsFastIIR4_F32(const float32_t * b, const float32_t * a,
                                arm_iir4_mod_coef_f32 * newCoef)
{

    double       coeffs[4][12] = {
        {0, 0, 0, b[0], b[1], b[2], b[3], b[4], -a[0], -a[1], -a[2], -a[3]},
        {0, 0, b[0], b[1], b[2], b[3], b[4], 0, -a[1], -a[2], -a[3], 0},
        {0, b[0], b[1], b[2], b[3], b[4], 0, 0, -a[2], -a[3], 0, 0},
        {b[0], b[1], b[2], b[3], b[4], 0, 0, 0, -a[3], 0, 0, 0},
    };

    for (int i = 0; i < 12; i++) {
        coeffs[1][i] += (-a[0]) * coeffs[0][i];
        coeffs[2][i] += (-a[0]) * coeffs[1][i] + (-a[1]) * coeffs[0][i];
        coeffs[3][i] +=
            (-a[0]) * coeffs[2][i] + (-a[1]) * coeffs[1][i] + (-a[2]) * coeffs[0][i];

        /*
         * transpose
         */
        newCoef->coeffs[i][0] = (float32_t) coeffs[0][i];
        newCoef->coeffs[i][1] = (float32_t) coeffs[1][i];
        newCoef->coeffs[i][2] = (float32_t) coeffs[2][i];
        newCoef->coeffs[i][3] = (float32_t) coeffs[3][i];
    }
}



/**
 * @brief  Initialization function for cascaded 4th order floating-point IIRs (MVE version).
 * @param[in,out] S             points to an instance of the floating-point cascaded IIR structure.
 * @param[in]     numStages     number of 4th order stages in the filter.
 * @param[in]     pCoeffs       points to the filter coefficients.
 * @param[in]     pState        points to the state buffer.

  @par             For MVE code, an extra storage for modified coefficients is required.
                   This one is part of the Filter context which must be extended by
                   sizeof(arm_iir4_mod_coef_f32) = 12*4 float32_t elements for each stage

 */

void arm_iir4_cascade_init_f32_mve(
        arm_iir4_casd_inst_f32 * S,
        uint8_t     numStages,
  const float32_t * pCoeffs,
        float32_t * pState)
{
    /* Assign filter stages */
    S->numStages = numStages;

    /* Assign coefficient pointer */
    /* Unused for the MVE version */
    S->pCoeffs = pCoeffs;

    /* Clear state buffer */

    /*
     *       State Layout
     *
     *  ModCoef0 | state num 0 | state den 0
     *  ModCoef1 | state num 1 | state den 1
     *  ...
     *  ModCoefn | state num n | state den n
     */
    memset(pState, 0, sizeof(arm_iir4_mod_coef_f32) * numStages +
           (IIR_4_ORDER * 2 * numStages) * sizeof(float32_t));

    /* Assign state pointer */
    S->pState = pState;

    /* Generates Vectorized coefficients in matrix form */
    arm_iir4_mod_coef_f32 *pCoeffsMod;
    const float32_t *pNum, *pDen;

    pCoeffsMod = (arm_iir4_mod_coef_f32 *) pState;
    pNum = pCoeffs;
    pDen = pNum + IIR_4_ORDER + 1;

    /* Generate SIMD friendly modified coefs */
    for (int i = 0; i < numStages; i++) {

        generateCoefsFastIIR4_F32(pNum, pDen, pCoeffsMod);

        pNum += (2 * IIR_4_ORDER + 1);
        pDen = pNum + IIR_4_ORDER + 1;
        pState += IIR4_F32_CASC_MVE_CONTEXT_SZ;
        pCoeffsMod = (arm_iir4_mod_coef_f32 *) pState;
    }
}

#endif
