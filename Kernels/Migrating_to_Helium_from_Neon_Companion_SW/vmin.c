/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library Extract
 * Title:        vmin.c
 * Description:  Minimum value of a floating-point vector
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

#include "vmin.h"


void arm_min_f32(const float32_t * pSrc,
                 uint32_t blockSize, float32_t * pResult, uint32_t * pIndex)
{
    float32_t       minVal, out;        /* Temporary variables to store the output value. */
    uint32_t        blkCnt, outIndex;   /* Loop counter */


    /* Initialise index value to zero. */
    outIndex = 0U;

    /* Load first input value that act as reference value for comparision */
    out = *pSrc++;


    /* Initialize blkCnt with number of samples */
    blkCnt = (blockSize - 1U);


    while (blkCnt > 0U) {
        /* Initialize minVal to the next consecutive values one by one */
        minVal = *pSrc++;

        /* compare for the minimum value */
        if (out > minVal) {
            /* Update the minimum value and it's index */
            out = minVal;
            outIndex = blockSize - blkCnt;
        }

        /* Decrement loop counter */
        blkCnt--;
    }

    /* Store the minimum value and it's index into destination pointers */
    *pResult = out;
    *pIndex = outIndex;
}


#if __ARM_FEATURE_MVE

void arm_min_helium_f32_direct(const float32_t * pSrc,
                               uint32_t blockSize, float32_t * pResult, uint32_t * pIndex)
{
    float32_t       maxVal1, out;       /* Temporary variables to store the output value. */
    uint32_t        blkCnt, outIndex;   /* loop counter */
    float32x4_t     outV, srcV;
    static const uint32_t indexInit[4] = { 4, 5, 6, 7 };
    static const uint32_t countVInit[4] = { 0, 1, 2, 3 };
    uint32x4_t      maxIdx;
    uint32x4_t      index;
    uint32x4_t      delta;
    uint32x4_t      countV;
    mve_pred16_t    pred;
    float32_t       minValue = F32_MAX;

    maxIdx = vdupq_n_u32(ULONG_MAX);
    delta = vdupq_n_u32(4);
    index = vld1q_u32(indexInit);
    countV = vld1q_u32(countVInit);

    /* Initialise the index value to zero. */
    outIndex = 0U;

    /* Load first input value that act as reference value for comparison */
    if (blockSize <= 3) {
        out = *pSrc++;

        blkCnt = blockSize - 1;

        while (blkCnt > 0U) {
            /* Initialize maxVal to the next consecutive values one by one */
            maxVal1 = *pSrc++;

            /* compare for the maximum value */
            if (out > maxVal1) {
                /* Update the maximum value and it's index */
                out = maxVal1;
                outIndex = blockSize - blkCnt;
            }

            /* Decrement the loop counter */
            blkCnt--;
        }
    } else {
        outV = vld1q_f32(pSrc);
        pSrc += 4;

        /* Compute 4 outputs at a time */
        blkCnt = (blockSize - 4) >> 2U;

        while (blkCnt > 0U) {
            srcV = vld1q_f32(pSrc);
            pSrc += 4;

            pred = vcmpleq_f32(srcV, outV);
            outV = vpselq_f32(srcV, outV, pred);
            countV = vpselq_f32(index, countV, pred);

            index = vaddq_u32(index, delta);

            /* Decrement the loop counter */
            blkCnt--;
        }

        out = vminnmvq_f32(minValue, outV);

        pred = vcmpleq_n_f32(outV, out);
        countV = vpselq(countV, maxIdx, pred);
        /*
         * Get min index which is thus for a max value
         */
        outIndex = vminvq_u32(ULONG_MAX, countV);

        /* if (blockSize - 1U) is not multiple of 4 */
        blkCnt = (blockSize - 4) % 4U;

        while (blkCnt > 0U) {
            /* Initialize maxVal to the next consecutive values one by one */
            maxVal1 = *pSrc++;

            /* compare for the maximum value */
            if (out > maxVal1) {
                /* Update the maximum value and it's index */
                out = maxVal1;
                outIndex = blockSize - blkCnt;
            }

            /* Decrement the loop counter */
            blkCnt--;
        }
    }

    /* Store the maximum value and it's index into destination pointers */
    *pResult = out;
    *pIndex = outIndex;
}



void arm_min_helium_f32(const float32_t * pSrc,
                        uint32_t blockSize, float32_t * pResult, uint32_t * pIndex)
{
    int32_t         blkCnt = blockSize;
    float32x4_t     vecSrc;
    float32x4_t     curExtremValVec = vdupq_n_f32(F32_MAX);
    float32_t       minValue = F32_MAX;
    uint32_t        idx = blockSize;
    uint32x4_t      indexVec;
    uint32x4_t      curExtremIdxVec;
    uint32_t        curIdx = 0;
    mve_pred16_t    pred;


    indexVec = vidupq_wb_u32(&curIdx, 1);
    curExtremIdxVec = vdupq_n_u32(0);

    do {
        mve_pred16_t    p = vctp32q(blkCnt);

        vecSrc = vld1q_z_f32(pSrc, p);
        /*
         * Get current min per lane and current index per lane
         * when a min is selected
         */
        pred = vcmpleq_m_f32(vecSrc, curExtremValVec, p);
        curExtremValVec = vorrq_m_f32(curExtremValVec, vecSrc, vecSrc, pred);
        curExtremIdxVec = vorrq_m_f32(curExtremIdxVec, indexVec, indexVec, pred);

        indexVec = vaddq_n_u32(indexVec, 4);

        pSrc += 4;
        blkCnt -= 4;
    }
    while (blkCnt > 0);

    /*
     * Get min value across the vector
     */
    minValue = vminnmvq(minValue, curExtremValVec);
    /*
     * set index for lower values to min possible index
     */
    pred = vcmpleq(curExtremValVec, minValue);
    indexVec = vpselq(curExtremIdxVec, vdupq_n_u32(blockSize), pred);
    /*
     * Get min index which is thus for a min value
     */
    idx = vminvq(idx, indexVec);
    /*
     * Save result
     */
    *pIndex = idx;
    *pResult = minValue;
}

#endif

#if __ARM_FEATURE_NEON

void arm_min_neon_f32(const float32_t * pSrc,
                      uint32_t blockSize, float32_t * pResult, uint32_t * pIndex)
{
    float32_t       maxVal1, out;       /* Temporary variables to store the output value. */
    uint32_t        blkCnt, outIndex;   /* loop counter */
    float32x4_t     outV, srcV;
    float32x2_t     outV2;
    uint32x4_t      idxV;
    static const uint32_t indexInit[4] = { 4, 5, 6, 7 };
    static const uint32_t countVInit[4] = { 0, 1, 2, 3 };
    uint32x4_t      maxIdx;
    uint32x4_t      index;
    uint32x4_t      delta;
    uint32x4_t      countV;
    uint32x2_t      countV2;

    maxIdx = vdupq_n_u32(ULONG_MAX);
    delta = vdupq_n_u32(4);
    index = vld1q_u32(indexInit);
    countV = vld1q_u32(countVInit);

    /* Initialise the index value to zero. */
    outIndex = 0U;

    /* Load first input value that act as reference value for comparison */
    if (blockSize <= 3) {
        out = *pSrc++;

        blkCnt = blockSize - 1;

        while (blkCnt > 0U) {
            /* Initialize maxVal to the next consecutive values one by one */
            maxVal1 = *pSrc++;

            /* compare for the maximum value */
            if (out > maxVal1) {
                /* Update the maximum value and it's index */
                out = maxVal1;
                outIndex = blockSize - blkCnt;
            }

            /* Decrement the loop counter */
            blkCnt--;
        }
    } else {
        outV = vld1q_f32(pSrc);
        pSrc += 4;

        /* Compute 4 outputs at a time */
        blkCnt = (blockSize - 4) >> 2U;

        while (blkCnt > 0U) {
            srcV = vld1q_f32(pSrc);
            pSrc += 4;

            idxV = vcltq_f32(srcV, outV);
            outV = vbslq_f32(idxV, srcV, outV);
            countV = vbslq_u32(idxV, index, countV);

            index = vaddq_u32(index, delta);

            /* Decrement the loop counter */
            blkCnt--;
        }

        outV2 = vpmin_f32(vget_low_f32(outV), vget_high_f32(outV));
        outV2 = vpmin_f32(outV2, outV2);
        out = vget_lane_f32(outV2, 0);

        idxV = vceqq_f32(outV, vdupq_n_f32(out));
        countV = vbslq_u32(idxV, countV, maxIdx);

        countV2 = vpmin_u32(vget_low_u32(countV), vget_high_u32(countV));
        countV2 = vpmin_u32(countV2, countV2);
        outIndex = vget_lane_u32(countV2, 0);

        /* if (blockSize - 1U) is not multiple of 4 */
        blkCnt = (blockSize - 4) % 4U;

        while (blkCnt > 0U) {
            /* Initialize maxVal to the next consecutive values one by one */
            maxVal1 = *pSrc++;

            /* compare for the maximum value */
            if (out > maxVal1) {
                /* Update the maximum value and it's index */
                out = maxVal1;
                outIndex = blockSize - blkCnt;
            }

            /* Decrement the loop counter */
            blkCnt--;
        }
    }

    /* Store the maximum value and it's index into destination pointers */
    *pResult = out;
    *pIndex = outIndex;
}

#endif
