/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_iir_q31.h
 * Description:  MVE IIR (experimental)
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

#ifndef ARM_IIR_H
#define ARM_IIR_H

#include "dsp/filtering_functions.h"


/**
 * @brief Instance structure for the Q.31 IIR.
 */

typedef struct {
    uint32_t        order;                   /** order of the filter. */
    q31_t          *pNumState;               /** points input sample history array. The array is of length numStages + blockSize. */
    q31_t          *pDenState;               /** points to output history array. The array is of length numStages + blockSize. */
    const q31_t    *pNumCoef;                /** points to the Numerator coefficients array. The array is of length order + 1. */
    const q31_t    *pDenCoef;                /** points to the Denominator coefficients array. The array is of length order */
    int8_t          postShift;               /**< Additional shift, in bits, applied to each output sample. */
} arm_iir_instance_q31;


void            arm_iir_init_q31(arm_iir_instance_q31 * S, uint16_t order,
                                 const q31_t * pNumCoef, const q31_t * pDenCoef,
                                 q31_t * pNumState, q31_t * pDenState, uint32_t blockSize, int8_t postShift);

void            arm_iir_q31(const arm_iir_instance_q31 * S, const q31_t * pSrc,
                            q31_t * pDst, uint32_t blockSize);


#if defined(ARM_MATH_MVEF)

/* Modified coefficient storage for Helium implementation */
/* Numerator & Denomiator storage requires extra 0-padding to align with Q31 vector length */
#define     IIR_COEF_BUF_SZ_Q31(order)          ((((order + 1) + 3) / 4) * 8)


void            arm_iir_init_q31_mve(arm_iir_instance_q31 * S, uint16_t order,
                                     const q31_t * pNumCoef, const q31_t * pDenCoef,
                                     q31_t * pNumState, q31_t * pDenState,
                                     q31_t * pModCoef, uint32_t blockSize, int8_t postShift);

/* Exported for optimizion purpose, but recommend using arm_iir_q31_mve only */
void            arm_iir4_q31_mve(const arm_iir_instance_q31 * S, const q31_t * pSrc,
                                 q31_t * pDst, uint32_t blockSize);


void            arm_iir8_q31_mve(const arm_iir_instance_q31 * S, const q31_t * pSrc,
                                 q31_t * pDst, uint32_t blockSize);


void            arm_iir12_q31_mve(const arm_iir_instance_q31 * S, const q31_t * pSrc,
                                  q31_t * pDst, uint32_t blockSize);


void            arm_iir_q31_mve(const arm_iir_instance_q31 * S, const q31_t * pSrc,
                                q31_t * pDst, uint32_t blockSize);

#endif

#endif                          // ARM_IIR_ORDER_H
