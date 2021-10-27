/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library Extract
 * Title:        cmplx_dot.h
 * Description:  Single Floating-point and Q.15 Fixed point complex dot products
 *
 * $Date:        Jul 2020
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
#include "cmplx_dot.h"




/**
  @ingroup groupCmplxMath
 */

/**
  @defgroup cmplx_dot_prod Complex Dot Product

  Computes the dot product of two complex vectors.
  The vectors are multiplied element-by-element and then summed.

  The <code>pSrcA</code> points to the first complex input vector and
  <code>pSrcB</code> points to the second complex input vector.
  <code>numSamples</code> specifies the number of complex samples
  and the data in each array is stored in an interleaved fashion
  (real, imag, real, imag, ...).
  Each array has a total of <code>2*numSamples</code> values.

  The underlying algorithm is used:

  <pre>
  realResult = 0;
  imagResult = 0;
  for (n = 0; n < numSamples; n++) {
      realResult += pSrcA[(2*n)+0] * pSrcB[(2*n)+0] - pSrcA[(2*n)+1] * pSrcB[(2*n)+1];
      imagResult += pSrcA[(2*n)+0] * pSrcB[(2*n)+1] + pSrcA[(2*n)+1] * pSrcB[(2*n)+0];
  }
  </pre>

  There are separate functions for floating-point, Q15, and Q31 data types.
 */

/**
  @addtogroup cmplx_dot_prod
  @{
 */

/**
  @brief         Floating-point complex dot product.
  @param[in]     pSrcA       points to the first input vector
  @param[in]     pSrcB       points to the second input vector
  @param[in]     numSamples  number of samples in each vector
  @param[out]    realResult  real part of the result returned here
  @param[out]    imagResult  imaginary part of the result returned here
  @return        none
 */


void arm_cmplx_dot_prod_q15(const q15_t * pSrcA,
                            const q15_t * pSrcB,
                            uint32_t numSamples, q31_t * realResult, q31_t * imagResult)
{
    uint32_t        blkCnt;     /* Loop counter */
    q63_t           real_sum = 0, imag_sum = 0; /* Temporary result variables */
    q15_t           a0, b0, c0, d0;


    /* Initialize blkCnt with number of samples */
    blkCnt = numSamples;
    while (blkCnt > 0U) {
        a0 = *pSrcA++;
        b0 = *pSrcA++;
        c0 = *pSrcB++;
        d0 = *pSrcB++;

        real_sum += (q31_t) a0 *c0;
        imag_sum += (q31_t) a0 *d0;
        real_sum -= (q31_t) b0 *d0;
        imag_sum += (q31_t) b0 *c0;

        /* Decrement loop counter */
        blkCnt--;
    }

    /* Store real and imaginary result in 8.24 format  */
    /* Convert real data in 34.30 to 8.24 by 6 right shifts */
    *realResult = (q31_t) (real_sum >> 6);
    /* Convert imaginary data in 34.30 to 8.24 by 6 right shifts */
    *imagResult = (q31_t) (imag_sum >> 6);
}

void arm_cmplx_dot_prod_f32(const float32_t * pSrcA,
                            const float32_t * pSrcB,
                            uint32_t numSamples,
                            float32_t * realResult, float32_t * imagResult)
{
    uint32_t        blkCnt;     /* Loop counter */
    float32_t       real_sum = 0.0f, imag_sum = 0.0f;   /* Temporary result variables */
    float32_t       a0, b0, c0, d0;

    /* Initialize blkCnt with number of samples */
    blkCnt = numSamples;
    while (blkCnt > 0U) {
        a0 = *pSrcA++;
        b0 = *pSrcA++;
        c0 = *pSrcB++;
        d0 = *pSrcB++;

        real_sum += a0 * c0;
        imag_sum += a0 * d0;
        real_sum -= b0 * d0;
        imag_sum += b0 * c0;

        /* Decrement loop counter */
        blkCnt--;
    }

    /* Store real and imaginary result in destination buffer. */
    *realResult = real_sum;
    *imagResult = imag_sum;
}


#if __ARM_FEATURE_MVE

#define vmlaq_f32 vfmaq_f32
#define vmlsq_f32 vfmsq_f32

void arm_cmplx_dot_prod_helium_f32_direct_conversion(const float32_t * pSrcA,
                                                     const float32_t * pSrcB,
                                                     uint32_t numSamples,
                                                     float32_t * realResult,
                                                     float32_t * imagResult)
{
    uint32_t        blkCnt;     /* Loop counter */
    float32_t       real_sum = 0.0f, imag_sum = 0.0f;   /* Temporary result variables */
    float32_t       a0, b0, c0, d0;

    float32x4x2_t   vec1, vec2, vec3, vec4;
    float32x4_t     accR, accI;

    /* float32x2_t accum = vdup_n_f32(0); */

    accR = vdupq_n_f32(0.0f);
    accI = vdupq_n_f32(0.0f);

    /* Loop unrolling: Compute 8 outputs at a time */
    blkCnt = numSamples >> 3U;

    while (blkCnt > 0U) {
        /* C = (A[0]+jA[1])*(B[0]+jB[1]) + ...  */
        /* Calculate dot product and then store the result in a temporary buffer. */

        vec1 = vld2q_f32(pSrcA);
        vec2 = vld2q_f32(pSrcB);

        /* Increment pointers */
        pSrcA += 8;
        pSrcB += 8;

        /* Re{C} = Re{A}*Re{B} - Im{A}*Im{B} */
        accR = vfmaq_f32(accR, vec1.val[0], vec2.val[0]);
        accR = vfmsq_f32(accR, vec1.val[1], vec2.val[1]);

        /* Im{C} = Re{A}*Im{B} + Im{A}*Re{B} */
        accI = vfmaq_f32(accI, vec1.val[1], vec2.val[0]);
        accI = vfmaq_f32(accI, vec1.val[0], vec2.val[1]);

        vec3 = vld2q_f32(pSrcA);
        vec4 = vld2q_f32(pSrcB);

        /* Increment pointers */
        pSrcA += 8;
        pSrcB += 8;

        /* Re{C} = Re{A}*Re{B} - Im{A}*Im{B} */
        accR = vfmaq_f32(accR, vec3.val[0], vec4.val[0]);
        accR = vfmsq_f32(accR, vec3.val[1], vec4.val[1]);

        /* Im{C} = Re{A}*Im{B} + Im{A}*Re{B} */
        accI = vfmaq_f32(accI, vec3.val[1], vec4.val[0]);
        accI = vfmaq_f32(accI, vec3.val[0], vec4.val[1]);

        /* Decrement the loop counter */
        blkCnt--;
    }

    /* no pairwise add vpadd_f32 */
    real_sum += vgetq_lane_f32(accR, 0) + vgetq_lane_f32(accR, 1) +
        vgetq_lane_f32(accR, 2) + vgetq_lane_f32(accR, 3);

    imag_sum += vgetq_lane_f32(accI, 0) + vgetq_lane_f32(accI, 1) +
        vgetq_lane_f32(accI, 2) + vgetq_lane_f32(accI, 3);

    /* Tail */
    blkCnt = numSamples & 0x7;
    while (blkCnt > 0U) {
        a0 = *pSrcA++;
        b0 = *pSrcA++;
        c0 = *pSrcB++;
        d0 = *pSrcB++;

        real_sum += a0 * c0;
        imag_sum += a0 * d0;
        real_sum -= b0 * d0;
        imag_sum += b0 * c0;

        /* Decrement loop counter */
        blkCnt--;
    }

    /* Store real and imaginary result in destination buffer. */
    *realResult = real_sum;
    *imagResult = imag_sum;
}

void arm_cmplx_dot_prod_helium_f32_vcmla(const float32_t * pSrcA,
                                         const float32_t * pSrcB,
                                         uint32_t numSamples,
                                         float32_t * realResult, float32_t * imagResult)
{
    uint32_t        blockSize = numSamples * CMPLX_PAIR;        /* loop counters */
    uint32_t        blkCnt;
    float32_t       real_sum, imag_sum;
    float32x4_t     vecSrcA, vecSrcB;
    float32x4_t     vec_acc = vdupq_n_f32(0.0f);
    float32_t       a0, b0, c0, d0;

    /* Compute 2 complex samples at a time */
    blkCnt = blockSize >> 2U;

    while (blkCnt > 0U) {
        vecSrcA = vld1q((const float32_t *) pSrcA);
        vecSrcB = vld1q((const float32_t *) pSrcB);
        /* Re{C} = Re{A}*Re{B} - Im{A}*Im{B} */
        /* Im{C} = Re{A}*Im{B} + Im{A}*Re{B} */
        vec_acc = vcmlaq(vec_acc, vecSrcA, vecSrcB);
        vec_acc = vcmlaq_rot90(vec_acc, vecSrcA, vecSrcB);
        /*
         * Decrement the blkCnt loop counter
         * Advance vector source and destination pointers
         */
        pSrcA += 4;
        pSrcB += 4;
        blkCnt--;
    }

    /* lane summation */
    real_sum = vgetq_lane(vec_acc, 0) + vgetq_lane(vec_acc, 2);
    imag_sum = vgetq_lane(vec_acc, 1) + vgetq_lane(vec_acc, 3);

    /* Tail */
    blkCnt = (blockSize & 3) >> 1;
    while (blkCnt > 0U) {
        a0 = *pSrcA++;
        b0 = *pSrcA++;
        c0 = *pSrcB++;
        d0 = *pSrcB++;

        real_sum += a0 * c0;
        imag_sum += a0 * d0;
        real_sum -= b0 * d0;
        imag_sum += b0 * c0;

        /* Decrement loop counter */
        blkCnt--;
    }

    /*
     * Store the real and imaginary results in the destination buffers
     */
    *realResult = real_sum;
    *imagResult = imag_sum;
}



void arm_cmplx_dot_prod_helium_f32_vcmla_tp(const float32_t * pSrcA,
                                            const float32_t * pSrcB,
                                            uint32_t numSamples,
                                            float32_t * realResult,
                                            float32_t * imagResult)
{
    int32_t         blkCnt = numSamples * CMPLX_PAIR;   /* loop counters */
    float32_t       real_sum, imag_sum;
    float32x4_t     vecSrcA, vecSrcB;
    float32x4_t     vec_acc = vdupq_n_f32(0.0f);


    do {
        mve_pred16_t    p = vctp32q(blkCnt);

        vecSrcA = vld1q_z_f32(pSrcA, p);
        vecSrcB = vld1q_z_f32(pSrcB, p);
        vec_acc = vcmlaq_m(vec_acc, vecSrcA, vecSrcB, p);
        vec_acc = vcmlaq_rot90_m(vec_acc, vecSrcA, vecSrcB, p);
        /*
         * Decrement the blkCnt loop counter
         * Advance vector source and destination pointers
         */
        pSrcA += 4;
        pSrcB += 4;
        blkCnt -= 4;
    }
    while (blkCnt > 0);

    real_sum = vgetq_lane(vec_acc, 0) + vgetq_lane(vec_acc, 2);
    imag_sum = vgetq_lane(vec_acc, 1) + vgetq_lane(vec_acc, 3);

    /*
     * Store the real and imaginary results in the destination buffers
     */
    *realResult = real_sum;
    *imagResult = imag_sum;
}



void arm_cmplx_dot_prod_helium_f32(const float32_t * pSrcA,
                                   const float32_t * pSrcB,
                                   uint32_t numSamples,
                                   float32_t * realResult, float32_t * imagResult)
{
    int32_t         blkCnt;
    float32_t       real_sum, imag_sum;
    float32x4_t     vecSrcA, vecSrcB;
    float32x4_t     vec_acc = vdupq_n_f32(0.0f);
    float32x4_t     vecSrcC, vecSrcD;

    blkCnt = numSamples / 4;
    blkCnt -= 1;

    if (blkCnt > 0) {
        /* will give more freedom to the compiler to generate stall-free code */
        vecSrcA = vld1q(pSrcA);
        vecSrcB = vld1q(pSrcB);
        pSrcA += 4;
        pSrcB += 4;

        while (blkCnt > 0U) {
            vec_acc = vcmlaq(vec_acc, vecSrcA, vecSrcB);
            vecSrcC = vld1q(pSrcA);
            pSrcA += 4;

            vec_acc = vcmlaq_rot90(vec_acc, vecSrcA, vecSrcB);
            vecSrcD = vld1q(pSrcB);
            pSrcB += 4;

            vec_acc = vcmlaq(vec_acc, vecSrcC, vecSrcD);
            vecSrcA = vld1q(pSrcA);
            pSrcA += 4;

            vec_acc = vcmlaq_rot90(vec_acc, vecSrcC, vecSrcD);
            vecSrcB = vld1q(pSrcB);
            pSrcB += 4;
            /*
             * Decrement the blockSize loop counter
             */
            blkCnt--;
        }

        /* process last elements out of loop to keep the SW pipeline */
        vec_acc = vcmlaq(vec_acc, vecSrcA, vecSrcB);
        vecSrcC = vld1q(pSrcA);

        vec_acc = vcmlaq_rot90(vec_acc, vecSrcA, vecSrcB);
        vecSrcD = vld1q(pSrcB);

        vec_acc = vcmlaq(vec_acc, vecSrcC, vecSrcD);
        vec_acc = vcmlaq_rot90(vec_acc, vecSrcC, vecSrcD);

        /*
         * tail
         */
        blkCnt = CMPLX_PAIR * (numSamples & 3);
        while (blkCnt > 0) {
            mve_pred16_t    p = vctp32q(blkCnt);
            pSrcA += 4;
            pSrcB += 4;
            vecSrcA = vld1q_z_f32(pSrcA, p);
            vecSrcB = vld1q_z_f32(pSrcB, p);
            vec_acc = vcmlaq_m(vec_acc, vecSrcA, vecSrcB, p);
            vec_acc = vcmlaq_rot90_m(vec_acc, vecSrcA, vecSrcB, p);
            blkCnt -= 4;
        }
    } else {
        /* small vector */
        blkCnt = numSamples * CMPLX_PAIR;
        vec_acc = vdupq_n_f32(0.0f);

        do {
            mve_pred16_t    p = vctp32q(blkCnt);

            vecSrcA = vld1q_z_f32(pSrcA, p);
            vecSrcB = vld1q_z_f32(pSrcB, p);
            vec_acc = vcmlaq_m(vec_acc, vecSrcA, vecSrcB, p);
            vec_acc = vcmlaq_rot90_m(vec_acc, vecSrcA, vecSrcB, p);
            /*
             * Decrement the blkCnt loop counter
             * Advance vector source and destination pointers
             */
            pSrcA += 4;
            pSrcB += 4;
            blkCnt -= 4;
        }
        while (blkCnt > 0);
    }

    real_sum = vgetq_lane(vec_acc, 0) + vgetq_lane(vec_acc, 2);
    imag_sum = vgetq_lane(vec_acc, 1) + vgetq_lane(vec_acc, 3);

    /*
     * Store the real and imaginary results in the destination buffers
     */
    *realResult = real_sum;
    *imagResult = imag_sum;
}





void arm_cmplx_dot_prod_helium_q15_direct_conversion(const q15_t * pSrcA,
                                                     const q15_t * pSrcB,
                                                     uint32_t numSamples,
                                                     q31_t * realResult,
                                                     q31_t * imagResult)
{
    uint32_t        blkCnt;     /* Loop counter */
    q63_t           real_sum = 0, imag_sum = 0; /* Temporary result variables */
    q15_t           a0, b0, c0, d0;

    int16x8x2_t     vec1, vec2;
    int32x4_t       tempL, tempH;
    int64_t         resr1, resr2, resi;

    resr1 = 0LL;
    resr2 = 0LL;
    resi = 0LL;

    /* loop Unrolling */

    blkCnt = numSamples >> 3U;

    while (blkCnt > 0U) {
        vec1 = vld2q_s16(pSrcA);
        vec2 = vld2q_s16(pSrcB);
        pSrcA += 16;
        pSrcB += 16;

        tempL = vmullbq_int_s16(vec1.val[0], vec2.val[0]);
        tempH = vmulltq_int_s16(vec1.val[0], vec2.val[0]);

        resr1 = vaddlvaq_s32(resr1, tempL);
        resr1 = vaddlvaq_s32(resr1, tempH);

        tempL = vmullbq_int_s16(vec1.val[1], vec2.val[1]);
        tempH = vmulltq_int_s16(vec1.val[1], vec2.val[0]);

        resr2 = vaddlvaq_s32(resr2, tempL);
        resr2 = vaddlvaq_s32(resr2, tempH);

        tempL = vmullbq_int_s16(vec1.val[0], vec2.val[1]);
        tempH = vmulltq_int_s16(vec1.val[0], vec2.val[1]);

        resi = vaddlvaq_s32(resi, tempL);
        resi = vaddlvaq_s32(resi, tempH);

        tempL = vmullbq_int_s16(vec1.val[1], vec2.val[0]);
        tempH = vmulltq_int_s16(vec1.val[1], vec2.val[0]);

        resi = vaddlvaq_s32(resi, tempL);
        resi = vaddlvaq_s32(resi, tempH);

        /* Decrement the blockSize loop counter */
        blkCnt--;
    }
    real_sum = resr1;
    real_sum -= resr2;
    imag_sum = resi;

    /* tail */
    blkCnt = numSamples & 0x7;

    while (blkCnt > 0U) {
        a0 = *pSrcA++;
        b0 = *pSrcA++;
        c0 = *pSrcB++;
        d0 = *pSrcB++;

        real_sum += (q31_t) a0 *c0;
        imag_sum += (q31_t) a0 *d0;
        real_sum -= (q31_t) b0 *d0;
        imag_sum += (q31_t) b0 *c0;

        /* Decrement loop counter */
        blkCnt--;
    }

    /* Store real and imaginary result in 8.24 format  */
    /* Convert real data in 34.30 to 8.24 by 6 right shifts */
    *realResult = (q31_t) (real_sum >> 6);
    /* Convert imaginary data in 34.30 to 8.24 by 6 right shifts */
    *imagResult = (q31_t) (imag_sum >> 6);
}



void arm_cmplx_dot_prod_helium_q15_ntp(const q15_t * pSrcA,
                                       const q15_t * pSrcB,
                                       uint32_t numSamples, q31_t * realResult,
                                       q31_t * imagResult)
{
    int32_t         blkCnt;     /* Loop counter */
    q63_t           real_sum = 0, imag_sum = 0; /* Temporary result variables */
    q15_t           a0, b0, c0, d0;
    int16x8_t       vec1, vec2;

    /* loop Unrolling */

    blkCnt = (numSamples * CMPLX_PAIR) >> 3U;

    while (blkCnt > 0) {
        vec1 = vld1q(pSrcA);
        vec2 = vld1q(pSrcB);

        real_sum = vmlsldavaq_s16(real_sum, vec1, vec2);
        imag_sum = vmlaldavaxq_s16(imag_sum, vec1, vec2);

        /*
         * Decrement the blkCnt loop counter
         * Advance vector source and destination pointers
         */
        pSrcA += 8;
        pSrcB += 8;
        blkCnt--;
    }

    /* tail */
    blkCnt = ((numSamples * CMPLX_PAIR) & 0x7) >> 1;

    while (blkCnt > 0U) {
        a0 = *pSrcA++;
        b0 = *pSrcA++;
        c0 = *pSrcB++;
        d0 = *pSrcB++;

        real_sum += (q31_t) a0 *c0;
        imag_sum += (q31_t) a0 *d0;
        real_sum -= (q31_t) b0 *d0;
        imag_sum += (q31_t) b0 *c0;

        /* Decrement loop counter */
        blkCnt--;
    }

    /* Store real and imaginary result in 8.24 format  */
    /* Convert real data in 34.30 to 8.24 by 6 right shifts */
    *realResult = (q31_t) asrl(real_sum, 6);
    /* Convert imaginary data in 34.30 to 8.24 by 6 right shifts */
    *imagResult = (q31_t) asrl(imag_sum, 6);
}



void arm_cmplx_dot_prod_helium_q15_tp(const q15_t * pSrcA,
                                      const q15_t * pSrcB,
                                      uint32_t numSamples, q31_t * realResult,
                                      q31_t * imagResult)
{
    int32_t         blkCnt;     /* Loop counter */
    q63_t           real_sum = 0, imag_sum = 0; /* Temporary result variables */
    int16x8_t       vec1, vec2;

    /* loop Unrolling */

    blkCnt = numSamples * CMPLX_PAIR;

    while (blkCnt > 0) {
        mve_pred16_t    p = vctp16q(blkCnt);
        vec1 = vld1q_z_s16(pSrcA, p);
        vec2 = vld1q_z_s16(pSrcB, p);

        real_sum = vmlsldavaq_p(real_sum, vec1, vec2, p);
        imag_sum = vmlaldavaxq_p(imag_sum, vec1, vec2, p);

        /*
         * Decrement the blkCnt loop counter
         * Advance vector source and destination pointers
         */
        pSrcA += 8;
        pSrcB += 8;
        blkCnt -= 8;
    }

    /* Store real and imaginary result in 8.24 format  */
    /* Convert real data in 34.30 to 8.24 by 6 right shifts */
    *realResult = (q31_t) asrl(real_sum, 6);
    /* Convert imaginary data in 34.30 to 8.24 by 6 right shifts */
    *imagResult = (q31_t) asrl(imag_sum, 6);
}


void arm_cmplx_dot_prod_helium_q15(const q15_t * pSrcA,
                                   const q15_t * pSrcB,
                                   uint32_t numSamples,
                                   q31_t * realResult, q31_t * imagResult)
{
    int32_t         blkCnt;
    q63_t           accReal = 0LL;
    q63_t           accImag = 0LL;
    int16x8_t       vecSrcA, vecSrcB;
    int16x8_t       vecSrcC, vecSrcD;

    blkCnt = numSamples >> 3;
    blkCnt -= 1;
    if (blkCnt > 0) {
        /* should give more freedom to generate stall free code */
        vecSrcA = vld1q(pSrcA);
        vecSrcB = vld1q(pSrcB);
        pSrcA += 8;
        pSrcB += 8;

        while (blkCnt > 0U) {

            accReal = vmlsldavaq(accReal, vecSrcA, vecSrcB);
            vecSrcC = vld1q(pSrcA);
            pSrcA += 8;

            accImag = vmlaldavaxq(accImag, vecSrcA, vecSrcB);
            vecSrcD = vld1q(pSrcB);
            pSrcB += 8;

            accReal = vmlsldavaq(accReal, vecSrcC, vecSrcD);
            vecSrcA = vld1q(pSrcA);
            pSrcA += 8;

            accImag = vmlaldavaxq(accImag, vecSrcC, vecSrcD);
            vecSrcB = vld1q(pSrcB);
            pSrcB += 8;
            /*
             * Decrement the blockSize loop counter
             */
            blkCnt--;
        }

        /* process last elements out of the loop avoid the armclang breaking the SW pipeline */
        accReal = vmlsldavaq(accReal, vecSrcA, vecSrcB);
        vecSrcC = vld1q(pSrcA);

        accImag = vmlaldavaxq(accImag, vecSrcA, vecSrcB);
        vecSrcD = vld1q(pSrcB);

        accReal = vmlsldavaq(accReal, vecSrcC, vecSrcD);
        vecSrcA = vld1q(pSrcA);

        accImag = vmlaldavaxq(accImag, vecSrcC, vecSrcD);
        vecSrcB = vld1q(pSrcB);

        /*
         * tail
         */
        blkCnt = CMPLX_PAIR * (numSamples & 7);
        do {
            mve_pred16_t    p = vctp16q(blkCnt);

            pSrcA += 8;
            pSrcB += 8;

            vecSrcA = vld1q_z_s16(pSrcA, p);
            vecSrcB = vld1q_z_s16(pSrcB, p);

            accReal = vmlsldavaq_p(accReal, vecSrcA, vecSrcB, p);
            accImag = vmlaldavaxq_p(accImag, vecSrcA, vecSrcB, p);
            blkCnt -= 8;
        }
        while ((int32_t) blkCnt > 0);
    } else {
        blkCnt = numSamples * CMPLX_PAIR;
        while (blkCnt > 0) {
            mve_pred16_t    p = vctp16q(blkCnt);

            vecSrcA = vld1q_z_s16(pSrcA, p);
            vecSrcB = vld1q_z_s16(pSrcB, p);

            accReal = vmlsldavaq_p(accReal, vecSrcA, vecSrcB, p);
            accImag = vmlaldavaxq_p(accImag, vecSrcA, vecSrcB, p);

            /*
             * Decrement the blkCnt loop counter
             * Advance vector source and destination pointers
             */
            pSrcA += 8;
            pSrcB += 8;
            blkCnt -= 8;
        }
    }
    *realResult = asrl(accReal, (14 - 8));
    *imagResult = asrl(accImag, (14 - 8));
}

#endif

#if __ARM_FEATURE_NEON

void arm_cmplx_dot_prod_neon_f32(const float32_t * pSrcA,
                                 const float32_t * pSrcB,
                                 uint32_t numSamples,
                                 float32_t * realResult, float32_t * imagResult)
{
    uint32_t        blkCnt;     /* Loop counter */
    float32_t       real_sum = 0.0f, imag_sum = 0.0f;   /* Temporary result variables */
    float32_t       a0, b0, c0, d0;

    float32x4x2_t   vec1, vec2, vec3, vec4;
    float32x4_t     accR, accI;
    float32x2_t     accum = vdup_n_f32(0);

    accR = vdupq_n_f32(0.0f);
    accI = vdupq_n_f32(0.0f);

    /* Loop unrolling: Compute 8 outputs at a time */
    blkCnt = numSamples >> 3U;

    while (blkCnt > 0U) {
        /* C = (A[0]+jA[1])*(B[0]+jB[1]) + ...  */
        /* Calculate dot product and then store the result in a temporary buffer. */

        vec1 = vld2q_f32(pSrcA);
        vec2 = vld2q_f32(pSrcB);

        /* Increment pointers */
        pSrcA += 8;
        pSrcB += 8;

        /* Re{C} = Re{A}*Re{B} - Im{A}*Im{B} */
        accR = vmlaq_f32(accR, vec1.val[0], vec2.val[0]);
        accR = vmlsq_f32(accR, vec1.val[1], vec2.val[1]);

        /* Im{C} = Re{A}*Im{B} + Im{A}*Re{B} */
        accI = vmlaq_f32(accI, vec1.val[1], vec2.val[0]);
        accI = vmlaq_f32(accI, vec1.val[0], vec2.val[1]);

        vec3 = vld2q_f32(pSrcA);
        vec4 = vld2q_f32(pSrcB);

        /* Increment pointers */
        pSrcA += 8;
        pSrcB += 8;

        /* Re{C} = Re{A}*Re{B} - Im{A}*Im{B} */
        accR = vmlaq_f32(accR, vec3.val[0], vec4.val[0]);
        accR = vmlsq_f32(accR, vec3.val[1], vec4.val[1]);

        /* Im{C} = Re{A}*Im{B} + Im{A}*Re{B} */
        accI = vmlaq_f32(accI, vec3.val[1], vec4.val[0]);
        accI = vmlaq_f32(accI, vec3.val[0], vec4.val[1]);

        /* Decrement the loop counter */
        blkCnt--;
    }

    accum = vpadd_f32(vget_low_f32(accR), vget_high_f32(accR));
    real_sum += vget_lane_f32(accum, 0) + vget_lane_f32(accum, 1);

    accum = vpadd_f32(vget_low_f32(accI), vget_high_f32(accI));
    imag_sum += vget_lane_f32(accum, 0) + vget_lane_f32(accum, 1);

    /* Tail */
    blkCnt = numSamples & 0x7;


    while (blkCnt > 0U) {
        a0 = *pSrcA++;
        b0 = *pSrcA++;
        c0 = *pSrcB++;
        d0 = *pSrcB++;

        real_sum += a0 * c0;
        imag_sum += a0 * d0;
        real_sum -= b0 * d0;
        imag_sum += b0 * c0;

        /* Decrement loop counter */
        blkCnt--;
    }

    /* Store real and imaginary result in destination buffer. */
    *realResult = real_sum;
    *imagResult = imag_sum;
}



void arm_cmplx_dot_prod_neon_q15(const q15_t * pSrcA,
                                 const q15_t * pSrcB,
                                 uint32_t numSamples, q31_t * realResult,
                                 q31_t * imagResult)
{
    uint32_t        blkCnt;     /* Loop counter */
    q63_t           real_sum = 0, imag_sum = 0; /* Temporary result variables */
    q15_t           a0, b0, c0, d0;

    int16x8x2_t     vec1, vec2;
    int32x4_t       tempL, tempH;
    int64x2_t       resr1, resr2, resi;

    resr1 = vdupq_n_s64(0);
    resr2 = vdupq_n_s64(0);
    resi = vdupq_n_s64(0);

    /* loop Unrolling */

    blkCnt = numSamples >> 3U;

    while (blkCnt > 0U) {
        vec1 = vld2q_s16(pSrcA);
        vec2 = vld2q_s16(pSrcB);
        pSrcA += 16;
        pSrcB += 16;


        tempL = vmull_s16(vget_low_s16(vec1.val[0]), vget_low_s16(vec2.val[0]));
        tempH = vmull_s16(vget_high_s16(vec1.val[0]), vget_high_s16(vec2.val[0]));

        resr1 = vpadalq_s32(resr1, tempL);
        resr1 = vpadalq_s32(resr1, tempH);

        tempL = vmull_s16(vget_low_s16(vec1.val[1]), vget_low_s16(vec2.val[1]));
        tempH = vmull_s16(vget_high_s16(vec1.val[1]), vget_high_s16(vec2.val[1]));

        resr2 = vpadalq_s32(resr2, tempL);
        resr2 = vpadalq_s32(resr2, tempH);

        tempL = vmull_s16(vget_low_s16(vec1.val[0]), vget_low_s16(vec2.val[1]));
        tempH = vmull_s16(vget_high_s16(vec1.val[0]), vget_high_s16(vec2.val[1]));

        resi = vpadalq_s32(resi, tempL);
        resi = vpadalq_s32(resi, tempH);

        tempL = vmull_s16(vget_low_s16(vec1.val[1]), vget_low_s16(vec2.val[0]));
        tempH = vmull_s16(vget_high_s16(vec1.val[1]), vget_high_s16(vec2.val[0]));

        resi = vpadalq_s32(resi, tempL);
        resi = vpadalq_s32(resi, tempH);

        /* Decrement the blockSize loop counter */
        blkCnt--;
    }
    real_sum += resr1[0] + resr1[1];
    real_sum -= (resr2[0] + resr2[1]);
    imag_sum += resi[0] + resi[1];

    /* tail */
    blkCnt = numSamples & 0x7;

    while (blkCnt > 0U) {
        a0 = *pSrcA++;
        b0 = *pSrcA++;
        c0 = *pSrcB++;
        d0 = *pSrcB++;

        real_sum += (q31_t) a0 *c0;
        imag_sum += (q31_t) a0 *d0;
        real_sum -= (q31_t) b0 *d0;
        imag_sum += (q31_t) b0 *c0;

        /* Decrement loop counter */
        blkCnt--;
    }

    /* Store real and imaginary result in 8.24 format  */
    /* Convert real data in 34.30 to 8.24 by 6 right shifts */
    *realResult = (q31_t) (real_sum >> 6);
    /* Convert imaginary data in 34.30 to 8.24 by 6 right shifts */
    *imagResult = (q31_t) (imag_sum >> 6);
}


#endif
