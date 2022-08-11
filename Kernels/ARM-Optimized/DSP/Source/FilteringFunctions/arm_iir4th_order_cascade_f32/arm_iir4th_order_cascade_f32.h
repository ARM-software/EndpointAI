/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_iir4_mve.c
 * Description:  MVE IIR4 experimentation
 *
 * $Date:        Feb 2021
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

#ifndef ARM_IIR4TH_ORDER_CASC_H
#define ARM_IIR4TH_ORDER_CASC_H

#include "dsp/filtering_functions.h"


#define IIR_4_ORDER                     4

#define IIR4_F32_COEF_MATRIX_COLS       12
#define IIR4_F32_COEF_MATRIX_ROWS       4       /* F32 MVE vector elements */
#define IIR4_F32_COEF_MATRIX_SZ         (IIR4_F32_COEF_MATRIX_COLS * IIR4_F32_COEF_MATRIX_ROWS)
#define IIR4_F32_CASC_MVE_CONTEXT_SZ    (IIR4_F32_COEF_MATRIX_SZ + IIR_4_ORDER * 2)

/**
 * @brief Instance structure for the cascaded IIR
 */

typedef struct {
    uint32_t        numStages;     /**< number of stages in the filter */
    float32_t      *pState;        /**< Points to the array of state coefficients.  The array is of length 4*numStages. */
    const float32_t *pCoeffs;      /**< Points to the array of coefficients.  The array is of length 5*numStages. */
} arm_iir4_casd_inst_f32;



void            arm_iir4_cascade_init_f32(arm_iir4_casd_inst_f32 * S,
                                          uint8_t numStages,
                                          const float32_t * pCoeffs, float32_t * pState);

void            arm_iir4_cascade_f32(const arm_iir4_casd_inst_f32 * S, const float32_t * pSrc,
                                     float32_t * pDst, uint32_t blockSize);



#if defined(ARM_MATH_MVEF)

/**
 * @brief Instance structure for the modified IIR coefs required by vectorized code.
 *        (similar to DF1 Biquad)
 */

typedef struct {
    float32_t       coeffs[IIR4_F32_COEF_MATRIX_COLS][IIR4_F32_COEF_MATRIX_ROWS];
} arm_iir4_mod_coef_f32;




void            arm_iir4_cascade_init_f32_mve(arm_iir4_casd_inst_f32 * S,
                                              uint8_t numStages,
                                              const float32_t * pCoeffs, float32_t * pState);

void            arm_iir4_cascade_f32_mve(const arm_iir4_casd_inst_f32 * S, const float32_t * pSrc,
                                         float32_t * pDst, uint32_t blockSize);


#endif                          // ARM_MATH_MVEF

#endif                          // ARM_IIR4TH_ORDER_CASC_H
