/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library Extract
 * Title:        matrix.c
 * Description:  Floating-point complex dot product
 *
 * $Date:        18. March 2019
 * $Revision:    V1.6.0
 *
 * Target Processor: Cortex-M cores
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

#include "matrix.h"

/* index permutation table for 4x4 matrix transposition */
static const uint8_t mat_4x4_transp_idx_u8[16] = {
    0, 4, 8, 12,
    1, 5, 9, 13,
    2, 6, 10, 14,
    3, 7, 11, 15,
};


#if __ARM_FEATURE_MVE


void mat_transpose_inp_4x4_helium_f32_direct(float32_t * matrix)
{
    float32x4_t     row0 = vld1q_f32(matrix);
    float32x4_t     row1 = vld1q_f32(matrix + 4);
    float32x4_t     row2 = vld1q_f32(matrix + 8);
    float32x4_t     row3 = vld1q_f32(matrix + 12);

    float32x4x2_t   row01 = vtrnq_emu_f32(row0, row1);
    float32x4x2_t   row23 = vtrnq_emu_f32(row2, row3);

    vst1q_f32(matrix + 4,
              vcombine_emu_f32(vget_low_emu_f32(row01.val[1]),
                               vget_low_emu_f32(row23.val[1])));
    vst1q_f32(matrix,
              vcombine_emu_f32(vget_low_emu_f32(row01.val[0]),
                               vget_low_emu_f32(row23.val[0])));
    vst1q_f32(matrix + 8,
              vcombine_emu_f32(vget_high_emu_f32(row01.val[0]),
                               vget_high_emu_f32(row23.val[0])));
    vst1q_f32(matrix + 12,
              vcombine_emu_f32(vget_high_emu_f32(row01.val[1]),
                               vget_high_emu_f32(row23.val[1])));
}

void mat_transpose_inp_4x4_helium_f32(float32_t * matrix)
{
    float32x4x4_t   rows;

    rows.val[0] = vld1q_f32(matrix);
    rows.val[1] = vld1q_f32(matrix + 4);
    rows.val[2] = vld1q_f32(matrix + 8);
    rows.val[3] = vld1q_f32(matrix + 12);

    vst4q_f32(matrix, rows);
}


void mat_transpose_inp_4x4_helium_u8(uint8_t * matrix)
{
    uint8x16_t      mat;
    uint8x16_t      offset = vld1q_u8(mat_4x4_transp_idx_u8);

    mat = vldrbq_gather_offset_u8(matrix, offset);
    vst1q_u8(matrix, mat);
}


void mat_multiply_4x4_helium_direct(float32_t * A, float32_t * B, float32_t * C)
{
    // these are the columns A
    float32x4_t     A0;
    float32x4_t     A1;
    float32x4_t     A2;
    float32x4_t     A3;

    // these are the columns B
    float32x4_t     B0;
    float32x4_t     B1;
    float32x4_t     B2;
    float32x4_t     B3;

    // these are the columns C
    float32x4_t     C0;
    float32x4_t     C1;
    float32x4_t     C2;
    float32x4_t     C3;

    A0 = vld1q_f32(A);
    A1 = vld1q_f32(A + 4);
    A2 = vld1q_f32(A + 8);
    A3 = vld1q_f32(A + 12);

    // Zero accumulators for C values
    C0 = vmovq_n_emu_f32(0);
    C1 = vmovq_n_emu_f32(0);
    C2 = vmovq_n_emu_f32(0);
    C3 = vmovq_n_emu_f32(0);

    // Multiply accumulate in 4x1 blocks, i.e. each column in C
    B0 = vld1q_f32(B);
    C0 = vfmaq_laneq_emu_f32(C0, A0, B0, 0);
    C0 = vfmaq_laneq_emu_f32(C0, A1, B0, 1);
    C0 = vfmaq_laneq_emu_f32(C0, A2, B0, 2);
    C0 = vfmaq_laneq_emu_f32(C0, A3, B0, 3);
    vst1q_f32(C, C0);

    B1 = vld1q_f32(B + 4);
    C1 = vfmaq_laneq_emu_f32(C1, A0, B1, 0);
    C1 = vfmaq_laneq_emu_f32(C1, A1, B1, 1);
    C1 = vfmaq_laneq_emu_f32(C1, A2, B1, 2);
    C1 = vfmaq_laneq_emu_f32(C1, A3, B1, 3);
    vst1q_f32(C + 4, C1);

    B2 = vld1q_f32(B + 8);
    C2 = vfmaq_laneq_emu_f32(C2, A0, B2, 0);
    C2 = vfmaq_laneq_emu_f32(C2, A1, B2, 1);
    C2 = vfmaq_laneq_emu_f32(C2, A2, B2, 2);
    C2 = vfmaq_laneq_emu_f32(C2, A3, B2, 3);
    vst1q_f32(C + 8, C2);

    B3 = vld1q_f32(B + 12);
    C3 = vfmaq_laneq_emu_f32(C3, A0, B3, 0);
    C3 = vfmaq_laneq_emu_f32(C3, A1, B3, 1);
    C3 = vfmaq_laneq_emu_f32(C3, A2, B3, 2);
    C3 = vfmaq_laneq_emu_f32(C3, A3, B3, 3);
    vst1q_f32(C + 12, C3);
}





void mat_multiply_4x4_helium(float32_t * A, float32_t * B, float32_t * C)
{
    float32_t const *pSrBVec;
    float32_t *pInB = B;
    float32_t *pInA = A;
    float32_t *pOut = C;
    float32_t *pInA0, *pInA1, *pInA2, *pInA3;
    float32x4_t vecMac0, vecMac1, vecMac2, vecMac3;
    float32x4_t vecInB;
    const uint32_t MATRIX_DIM = 4;

    pSrBVec = (float32_t const *) pInB;

    pInA0 = pInA;
    pInA1 = pInA0 + MATRIX_DIM;
    pInA2 = pInA1 + MATRIX_DIM;
    pInA3 = pInA2 + MATRIX_DIM;
    /*
     * load {b0,0, b0,1, b0,2, b0,3}
     */
    vecInB = vld1q(pSrBVec);  pSrBVec += MATRIX_DIM;

    vecMac0 = vmulq(vecInB, *pInA0++);
    vecMac1 = vmulq(vecInB, *pInA1++);
    vecMac2 = vmulq(vecInB, *pInA2++);
    vecMac3 = vmulq(vecInB, *pInA3++);
    /*
     * load {b1,0, b1,1, b1,2, b1,3}
     */
    vecInB = vld1q(pSrBVec);  pSrBVec += MATRIX_DIM;

    vecMac0 = vfmaq(vecMac0, vecInB, *pInA0++);
    vecMac1 = vfmaq(vecMac1, vecInB, *pInA1++);
    vecMac2 = vfmaq(vecMac2, vecInB, *pInA2++);
    vecMac3 = vfmaq(vecMac3, vecInB, *pInA3++);
    /*
     * load {b2,0, b2,1, b2,2, b2,3}
     */
    vecInB = vld1q(pSrBVec);  pSrBVec += MATRIX_DIM;

    vecMac0 = vfmaq(vecMac0, vecInB, *pInA0++);
    vecMac1 = vfmaq(vecMac1, vecInB, *pInA1++);
    vecMac2 = vfmaq(vecMac2, vecInB, *pInA2++);
    vecMac3 = vfmaq(vecMac3, vecInB, *pInA3++);
    /*
     * load {b3,0, b3,1, b3,2, b3,3}
     */
    vecInB = vld1q(pSrBVec);  pSrBVec += MATRIX_DIM;

    vecMac0 = vfmaq(vecMac0, vecInB, *pInA0++);
    vecMac1 = vfmaq(vecMac1, vecInB, *pInA1++);
    vecMac2 = vfmaq(vecMac2, vecInB, *pInA2++);
    vecMac3 = vfmaq(vecMac3, vecInB, *pInA3++);

    vst1q(pOut, vecMac0);  pOut += MATRIX_DIM;
    vst1q(pOut, vecMac1);  pOut += MATRIX_DIM;
    vst1q(pOut, vecMac2);  pOut += MATRIX_DIM;
    vst1q(pOut, vecMac3);
}

#endif

#if __ARM_FEATURE_NEON
void mat_transpose_inp_4x4_neon_f32(float32_t * matrix)
{
    float32x4_t     row0 = vld1q_f32(matrix);
    float32x4_t     row1 = vld1q_f32(matrix + 4);
    float32x4_t     row2 = vld1q_f32(matrix + 8);
    float32x4_t     row3 = vld1q_f32(matrix + 12);

    float32x4x2_t   row01 = vtrnq_f32(row0, row1);
    float32x4x2_t   row23 = vtrnq_f32(row2, row3);

    vst1q_f32(matrix,
              vcombine_f32(vget_low_f32(row01.val[0]), vget_low_f32(row23.val[0])));
    vst1q_f32(matrix + 4,
              vcombine_f32(vget_low_f32(row01.val[1]), vget_low_f32(row23.val[1])));
    vst1q_f32(matrix + 8,
              vcombine_f32(vget_high_f32(row01.val[0]), vget_high_f32(row23.val[0])));
    vst1q_f32(matrix + 12,
              vcombine_f32(vget_high_f32(row01.val[1]), vget_high_f32(row23.val[1])));
}


void mat_transpose_inp_4x4_neon_u8(uint8_t * matrix)
{
    uint8x16_t      mat = vld1q_u8(matrix);
    uint8x16_t      offset = vld1q_u8(mat_4x4_transp_idx_u8);
    uint8x8_t       bot, top;
    uint8x8x2_t     tmp;

    tmp.val[0] = vget_low_u8(mat);
    tmp.val[1] = vget_high_u8(mat);

    bot = vtbl2_u8(tmp, vget_low_u8(offset));
    top = vtbl2_u8(tmp, vget_high_u8(offset));

    vst1q_u8(matrix, vcombine_u8(bot, top));
}

void mat_multiply_4x4_neon(float32_t * A, float32_t * B, float32_t * C)
{
#ifdef  __aarch64__
    // these are the columns A
    float32x4_t     A0;
    float32x4_t     A1;
    float32x4_t     A2;
    float32x4_t     A3;

    // these are the columns B
    float32x4_t     B0;
    float32x4_t     B1;
    float32x4_t     B2;
    float32x4_t     B3;

    // these are the columns C
    float32x4_t     C0;
    float32x4_t     C1;
    float32x4_t     C2;
    float32x4_t     C3;

    A0 = vld1q_f32(A);
    A1 = vld1q_f32(A + 4);
    A2 = vld1q_f32(A + 8);
    A3 = vld1q_f32(A + 12);

    // Zero accumulators for C values
    C0 = vmovq_n_f32(0);
    C1 = vmovq_n_f32(0);
    C2 = vmovq_n_f32(0);
    C3 = vmovq_n_f32(0);

    // Multiply accumulate in 4x1 blocks, i.e. each column in C
    B0 = vld1q_f32(B);
    C0 = vfmaq_laneq_f32(C0, A0, B0, 0);
    C0 = vfmaq_laneq_f32(C0, A1, B0, 1);
    C0 = vfmaq_laneq_f32(C0, A2, B0, 2);
    C0 = vfmaq_laneq_f32(C0, A3, B0, 3);
    vst1q_f32(C, C0);

    B1 = vld1q_f32(B + 4);
    C1 = vfmaq_laneq_f32(C1, A0, B1, 0);
    C1 = vfmaq_laneq_f32(C1, A1, B1, 1);
    C1 = vfmaq_laneq_f32(C1, A2, B1, 2);
    C1 = vfmaq_laneq_f32(C1, A3, B1, 3);
    vst1q_f32(C + 4, C1);

    B2 = vld1q_f32(B + 8);
    C2 = vfmaq_laneq_f32(C2, A0, B2, 0);
    C2 = vfmaq_laneq_f32(C2, A1, B2, 1);
    C2 = vfmaq_laneq_f32(C2, A2, B2, 2);
    C2 = vfmaq_laneq_f32(C2, A3, B2, 3);
    vst1q_f32(C + 8, C2);

    B3 = vld1q_f32(B + 12);
    C3 = vfmaq_laneq_f32(C3, A0, B3, 0);
    C3 = vfmaq_laneq_f32(C3, A1, B3, 1);
    C3 = vfmaq_laneq_f32(C3, A2, B3, 2);
    C3 = vfmaq_laneq_f32(C3, A3, B3, 3);
    vst1q_f32(C + 12, C3);
#else
    // these are the columns A
    float32x4_t     A0;
    float32x4_t     A1;
    float32x4_t     A2;
    float32x4_t     A3;

    // these are the columns B
    float32x4_t     B0;
    float32x4_t     B1;
    float32x4_t     B2;
    float32x4_t     B3;

    // these are the columns C
    float32x4_t     C0;
    float32x4_t     C1;
    float32x4_t     C2;
    float32x4_t     C3;

    A0 = vld1q_f32(A);
    A1 = vld1q_f32(A + 4);
    A2 = vld1q_f32(A + 8);
    A3 = vld1q_f32(A + 12);


    // Multiply accumulate in 4x1 blocks, i.e. each column in C
    B0 = vld1q_f32(B);
    C0 = vmulq_n_f32(A0, B0[0]);
    C0 = vfmaq_n_f32(C0, A1, B0[1]);
    C0 = vfmaq_n_f32(C0, A2, B0[2]);
    C0 = vfmaq_n_f32(C0, A3, B0[3]);
    vst1q_f32(C, C0);

    B1 = vld1q_f32(B + 4);
    C1 = vmulq_n_f32(A0, B1[0]);
    C1 = vfmaq_n_f32(C1, A1, B1[1]);
    C1 = vfmaq_n_f32(C1, A2, B1[2]);
    C1 = vfmaq_n_f32(C1, A3, B1[3]);
    vst1q_f32(C + 4, C1);

    B2 = vld1q_f32(B + 8);
    C2 = vmulq_n_f32(A0, B2[0]);
    C2 = vfmaq_n_f32(C2, A1, B2[1]);
    C2 = vfmaq_n_f32(C2, A2, B2[2]);
    C2 = vfmaq_n_f32(C2, A3, B2[3]);
    vst1q_f32(C + 8, C2);

    B3 = vld1q_f32(B + 12);
    C3 = vmulq_n_f32(A0, B3[0]);
    C3 = vfmaq_n_f32(C3, A1, B3[1]);
    C3 = vfmaq_n_f32(C3, A2, B3[2]);
    C3 = vfmaq_n_f32(C3, A3, B3[3]);
    vst1q_f32(C + 12, C3);
#endif
}

#endif
