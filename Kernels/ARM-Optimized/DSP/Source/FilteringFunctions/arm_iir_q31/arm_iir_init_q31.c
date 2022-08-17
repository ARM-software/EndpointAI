/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_iir_init_q31.c
 * Description:  MVE IIR init stage (experimental)
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


#include "arm_iir_q31.h"


/*
 * @brief  Initialization function for Q.31 IIRs.
 * @param[in,out] S             points to an instance of the Q.31 IIR structure.
 * @param[in]     pNumCoef      points to the filter coefficients numerator.
 * @param[in]     pDenCoef      points to the filter coefficients denominator.
 * @param[in]     pNumState     points to the IIR sample history.
 * @param[in]     pDenState     points to the IIR output history.
 * @param[in]     blockSize     IIR block size
 * @param[in]     postShift     filter coefficients scaling
 */

void arm_iir_init_q31(arm_iir_instance_q31 * S, uint16_t order,
                      const q31_t * pNumCoef, const q31_t * pDenCoef,
                      q31_t * pNumState, q31_t * pDenState, uint32_t blockSize, int8_t postShift)
{
    S->order = order;
    S->pNumState = pNumState;
    S->pDenState = pDenState;
    S->pNumCoef = pNumCoef;
    S->pDenCoef = pDenCoef;
    S->postShift = postShift;

    memset(pNumState, 0, (blockSize + order) * sizeof(q31_t));
    memset(pDenState, 0, (blockSize + order - 1) * sizeof(q31_t));
}


#if defined(ARM_MATH_MVEF)

/*
 * @brief  Initialization function for Q.31 IIRs (MVE specific)
 * @param[in,out] S             points to an instance of the Q.31 IIR structure.
 * @param[in]     pNumCoef      points to the filter coefficients numerator.
 * @param[in]     pDenCoef      points to the filter coefficients denominator.
 * @param[in]     pNumState     points to the IIR sample history.
 * @param[in]     pDenState     points to the IIR output history.
 * @param[in]     pModCoef      points to the MVE reshuffled * 0-padded coefficient storage
 * @param[in]     blockSize     IIR block size
 * @param[in]     postShift     filter coefficients scaling
 */

void arm_iir_init_q31_mve(arm_iir_instance_q31 * S, uint16_t order,
                          const q31_t * pNumCoef, const q31_t * pDenCoef,
                          q31_t * pNumState, q31_t * pDenState, q31_t * pModCoef,
                          uint32_t blockSize, int8_t postShift)
{
    S->order = order;
    S->pNumState = pNumState;
    S->pDenState = pDenState;
    S->postShift = postShift;

    /* Reorganize coef for vector processing */
    /* pad with 0 if not size if noy aligned with vector size */

    /* copy numerator coefs */
    for (int i = 0; i < order + 1; i++)
        pModCoef[i] = pNumCoef[order - i];

    S->pNumCoef = pModCoef;

    /* round to vector size and 0-pad */
    int             round_up = ((order + 1 + 3) / 4) * 4;
    for (int i = order + 1; i < round_up; i++)
        pModCoef[i] = 0;

    pModCoef += round_up;

    S->pDenCoef = pModCoef;
    for (int i = 0; i < order; i++)
        /* negate */
        pModCoef[i] = __QSUB(0, pDenCoef[order - 1 - i]);
//        pModCoef[i] = pDenCoef[order - 1 - i];

    memset(pNumState, 0, (blockSize + order) * sizeof(q31_t));
    memset(pDenState, 0, (blockSize + order - 1) * sizeof(q31_t));

}

#endif
