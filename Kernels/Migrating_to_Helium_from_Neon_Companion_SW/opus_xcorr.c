/* opus source code */
/* Copyright (c) 2014-2015 Xiph.Org Foundation */

/* Helium version
 * Copyright (C) 2010-2020 ARM Limited or its affiliates. All rights reserved.
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

#include "opus_xcorr.h"


void xcorr_kernel_c(const opus_val16 * x, const opus_val16 * y, opus_val32 sum[4],
                    int len)
{
    int             j;
    opus_val16      y_0, y_1, y_2, y_3;
    celt_assert(len >= 3);

    y_3 = 0;                    /* gcc doesn't realize that y_3 can't be used uninitialized */
    y_0 = *y++;
    y_1 = *y++;
    y_2 = *y++;
    for (j = 0; j < len - 3; j += 4) {
        opus_val16      tmp;
        tmp = *x++;
        y_3 = *y++;
        sum[0] = MAC16_16(sum[0], tmp, y_0);
        sum[1] = MAC16_16(sum[1], tmp, y_1);
        sum[2] = MAC16_16(sum[2], tmp, y_2);
        sum[3] = MAC16_16(sum[3], tmp, y_3);
        tmp = *x++;
        y_0 = *y++;
        sum[0] = MAC16_16(sum[0], tmp, y_1);
        sum[1] = MAC16_16(sum[1], tmp, y_2);
        sum[2] = MAC16_16(sum[2], tmp, y_3);
        sum[3] = MAC16_16(sum[3], tmp, y_0);
        tmp = *x++;
        y_1 = *y++;
        sum[0] = MAC16_16(sum[0], tmp, y_2);
        sum[1] = MAC16_16(sum[1], tmp, y_3);
        sum[2] = MAC16_16(sum[2], tmp, y_0);
        sum[3] = MAC16_16(sum[3], tmp, y_1);
        tmp = *x++;
        y_2 = *y++;
        sum[0] = MAC16_16(sum[0], tmp, y_3);
        sum[1] = MAC16_16(sum[1], tmp, y_0);
        sum[2] = MAC16_16(sum[2], tmp, y_1);
        sum[3] = MAC16_16(sum[3], tmp, y_2);
    }
    if (j++ < len) {
        opus_val16      tmp = *x++;
        y_3 = *y++;
        sum[0] = MAC16_16(sum[0], tmp, y_0);
        sum[1] = MAC16_16(sum[1], tmp, y_1);
        sum[2] = MAC16_16(sum[2], tmp, y_2);
        sum[3] = MAC16_16(sum[3], tmp, y_3);
    }
    if (j++ < len) {
        opus_val16      tmp = *x++;
        y_0 = *y++;
        sum[0] = MAC16_16(sum[0], tmp, y_1);
        sum[1] = MAC16_16(sum[1], tmp, y_2);
        sum[2] = MAC16_16(sum[2], tmp, y_3);
        sum[3] = MAC16_16(sum[3], tmp, y_0);
    }
    if (j < len) {
        opus_val16      tmp = *x++;
        y_1 = *y++;
        sum[0] = MAC16_16(sum[0], tmp, y_2);
        sum[1] = MAC16_16(sum[1], tmp, y_3);
        sum[2] = MAC16_16(sum[2], tmp, y_0);
        sum[3] = MAC16_16(sum[3], tmp, y_1);
    }
}

#if __ARM_FEATURE_MVE
#include <arm_mve_intrinsics.h>
#define vmlal_lane_emu_s16(a, b, c, idx) vmlaq_n_s32(a, b, vgetq_lane(c, idx))

void xcorr_kernel_mve_fixed_direct(const opus_val16 * x, const opus_val16 * y, opus_val32 sum[4],
                            int len)
{
    int             j;
    int32x4_t       a = vld1q_s32(sum);
    /* Load y[0...3] */
    /* This requires len>0 to always be valid (which we assert in the C code). */
    int32x4_t       y0 = vldrhq_s32(y);
    y += 4;

    for (j = 0; j + 8 <= len; j += 8) {
        /* Load x[0...7] */
        int32x4_t       x0 = vldrhq_s32(x);
        int32x4_t       x4 = vldrhq_s32(x + 4);
        /* Load y[4...11] */
        int32x4_t       y4 = vldrhq_s32(y);
        int32x4_t       y8 = vldrhq_s32(y + 4);
        int32x4_t       a0 = vmlaq_n_s32(a, y0, vgetq_lane(x0, 0));
        int32x4_t       a1 = vmlaq_n_s32(a0, y4, vgetq_lane(x4, 0));

        int32x4_t       y1 = vext_emu_s16(y0, y4, 1);
        int32x4_t       y5 = vext_emu_s16(y4, y8, 1);
        int32x4_t       a2 = vmlaq_n_s32(a1, y1, vgetq_lane(x0, 1));
        int32x4_t       a3 = vmlaq_n_s32(a2, y5, vgetq_lane(x4, 1));

        int32x4_t       y2 = vext_emu_s16(y0, y4, 2);
        int32x4_t       y6 = vext_emu_s16(y4, y8, 2);
        int32x4_t       a4 = vmlaq_n_s32(a3, y2, vgetq_lane(x0, 2));
        int32x4_t       a5 = vmlaq_n_s32(a4, y6, vgetq_lane(x4, 2));

        int32x4_t       y3 = vext_emu_s16(y0, y4, 3);
        int32x4_t       y7 = vext_emu_s16(y4, y8, 3);
        int32x4_t       a6 = vmlaq_n_s32(a5, y3, vgetq_lane(x0, 3));
        int32x4_t       a7 = vmlaq_n_s32(a6, y7, vgetq_lane(x4, 3));

        y0 = y8;
        a = a7;
        x += 8;
        y += 8;
    }

    for (; j < len; j++) {
        int16_t         x0 = *x;        /* load next x */
        int32x4_t       a0 = vmlaq_n_s32(a, y0, (int32_t) x0);

        y0 = vldrhq_s32(y - 3); /* load next y */
        a = a0;
        x++;
        y++;
    }

    vst1q_s32(sum, a);
}


void xcorr_kernel_mve_fixed_improv_1(const opus_val16 * x, const opus_val16 * y,
                             opus_val32 sum[4], int len)
{
    int             j;
    int32x4_t       a = vld1q_s32(sum);
    /* Load y[0...3] */
    /* This requires len>0 to always be valid (which we assert in the C code). */
    int32x4_t       y0 = vldrhq_s32(y);
    y += 4;

    for (j = 0; j + 8 <= len; j += 8) {
        /* Load x[0...7] */
        int32x4_t       x0 = vldrhq_s32(x);
        int32x4_t       x4 = vldrhq_s32(x + 4);
        /* Load y[4...11] */
        int32x4_t       y4 = vldrhq_s32(y);
        int32x4_t       y8 = vldrhq_s32(y + 4);
        int32x4_t       a0 = vmlaq_n_s32(a, y0, vgetq_lane(x0, 0));
        int32x4_t       a1 = vmlaq_n_s32(a0, y4, vgetq_lane(x4, 0));

        int32x4_t       y1 = vldrhq_s32(y - 3);
        int32x4_t       y5 = vldrhq_s32(y + 1);
        int32x4_t       a2 = vmlaq_n_s32(a1, y1, vgetq_lane(x0, 1));
        int32x4_t       a3 = vmlaq_n_s32(a2, y5, vgetq_lane(x4, 1));

        int32x4_t       y2 = vldrhq_s32(y - 2);
        int32x4_t       y6 = vldrhq_s32(y + 2);
        int32x4_t       a4 = vmlaq_n_s32(a3, y2, vgetq_lane(x0, 2));
        int32x4_t       a5 = vmlaq_n_s32(a4, y6, vgetq_lane(x4, 2));

        int32x4_t       y3 = vldrhq_s32(y - 1);
        int32x4_t       y7 = vldrhq_s32(y + 3);
        int32x4_t       a6 = vmlaq_n_s32(a5, y3, vgetq_lane(x0, 3));
        int32x4_t       a7 = vmlaq_n_s32(a6, y7, vgetq_lane(x4, 3));

        y0 = y8;
        a = a7;

        x += 8;
        y += 8;
    }

    for (; j < len; j++) {
        int16_t         x0 = *x;        /* load next x */
        int32x4_t       a0 = vmlaq_n_s32(a, y0, (int32_t) x0);

        y0 = vldrhq_s32(y - 3); /* load next y */
        a = a0;
        x++;
        y++;
    }

    vst1q_s32(sum, a);
}

void xcorr_kernel_mve_fixed_improv_2(const opus_val16 * x, const opus_val16 * y,
                                 opus_val32 sum[4], int len)
{
    int             j;

    for (j = 0; j + 8 <= len; j += 8) {
        /* Load x[0...7] */
        int16x8_t       x0 = vld1q(x);
        /* Load y[0...7] */
        int32x4_t       y0 = vld1q(y);
        int32x4_t       y1 = vld1q(y + 1);
        int32x4_t       y2 = vld1q(y + 2);
        int32x4_t       y3 = vld1q(y + 3);

        sum[0] = vmladavaq_s16(sum[0], x0, y0);
        sum[1] = vmladavaq_s16(sum[1], x0, y1);
        sum[2] = vmladavaq_s16(sum[2], x0, y2);
        sum[3] = vmladavaq_s16(sum[3], x0, y3);

        x += 8;
        y += 8;
    }


    if (j < len) {
        mve_pred16_t    pred = vctp16q(len - j);
        /* Load x[0...7] */
        int16x8_t       x0 = vld1q(x);
        /* Load y[0...7] */
        int16x8_t       y0 = vld1q(y);
        int16x8_t       y1 = vld1q(y + 1);
        int16x8_t       y2 = vld1q(y + 2);
        int16x8_t       y3 = vld1q(y + 3);

        sum[0] = vmladavaq_p_s16(sum[0], x0, y0, pred);
        sum[1] = vmladavaq_p_s16(sum[1], x0, y1, pred);
        sum[2] = vmladavaq_p_s16(sum[2], x0, y2, pred);
        sum[3] = vmladavaq_p_s16(sum[3], x0, y3, pred);

    }
}


void xcorr_kernel_mve_fixed(const opus_val16 * x, const opus_val16 * y,
                                    opus_val32 sum[4], int len)
{

    do {
        mve_pred16_t    pred = vctp16q(len);
        /* Load x[0...7] */
        int16x8_t       x0 = vldrhq_z_s16(x, pred);
        /* Load y[0...7] */
        int16x8_t       y0 = vldrhq_z_s16(y, pred);
        int16x8_t       y1 = vldrhq_z_s16(y + 1, pred);
        int16x8_t       y2 = vldrhq_z_s16(y + 2, pred);
        int16x8_t       y3 = vldrhq_z_s16(y + 3, pred);

        sum[0] = vmladavaq_p_s16(sum[0], x0, y0, pred);
        sum[1] = vmladavaq_p_s16(sum[1], x0, y1, pred);
        sum[2] = vmladavaq_p_s16(sum[2], x0, y2, pred);
        sum[3] = vmladavaq_p_s16(sum[3], x0, y3, pred);

        x += 8;
        y += 8;
        len -= 8;
    }
    while (len > 0);
}



void xcorr_kernel_mve_float_direct(const float32_t * x, const float32_t * y,
                             float32_t sum[4], int len)
{
    float32x4_t     YY[3];
    float32x4_t     YEXT[3];
    float32x4_t     XX[2];
    float32_t       XX_2;
    float32x4_t     SUMM;
    const float32_t *xi = x;
    const float32_t *yi = y;

    celt_assert(len > 0);

    YY[0] = vld1q_f32(yi);
    SUMM = vdupq_n_f32(0);

    /* Consume 8 elements in x vector and 12 elements in y
     * vector. However, the 12'th element never really gets
     * touched in this loop. So, if len == 8, then we only
     * must access y[0] to y[10]. y[11] must not be accessed
     * hence make sure len > 8 and not len >= 8
     */
    while (len > 8) {
        yi += 4;
        YY[1] = vld1q_f32(yi);
        yi += 4;
        YY[2] = vld1q_f32(yi);

        XX[0] = vld1q_f32(xi);
        xi += 4;
        XX[1] = vld1q_f32(xi);
        xi += 4;

        SUMM = vfmaq_n_f32(SUMM, YY[0], vgetq_lane_f32(XX[0], 0));
        YEXT[0] = vextq_emu_f32(YY[0], YY[1], 1);
        SUMM = vfmaq_n_f32(SUMM, YEXT[0], vgetq_lane_f32(XX[0], 1));
        YEXT[1] = vextq_emu_f32(YY[0], YY[1], 2);
        SUMM = vfmaq_n_f32(SUMM, YEXT[1], vgetq_lane_f32(XX[0], 2));
        YEXT[2] = vextq_emu_f32(YY[0], YY[1], 3);
        SUMM = vfmaq_n_f32(SUMM, YEXT[2], vgetq_lane_f32(XX[0], 3));

        SUMM = vfmaq_n_f32(SUMM, YY[1], vgetq_lane_f32(XX[1], 0));
        YEXT[0] = vextq_emu_f32(YY[1], YY[2], 1);
        SUMM = vfmaq_n_f32(SUMM, YEXT[0], vgetq_lane_f32(XX[1], 1));
        YEXT[1] = vextq_emu_f32(YY[1], YY[2], 2);
        SUMM = vfmaq_n_f32(SUMM, YEXT[1], vgetq_lane_f32(XX[1], 2));
        YEXT[2] = vextq_emu_f32(YY[1], YY[2], 3);
        SUMM = vfmaq_n_f32(SUMM, YEXT[2], vgetq_lane_f32(XX[1], 3));

        YY[0] = YY[2];

        len -= 8;
    }

    /* Consume 4 elements in x vector and 8 elements in y
     * vector. However, the 8'th element in y never really gets
     * touched in this loop. So, if len == 4, then we only
     * must access y[0] to y[6]. y[7] must not be accessed
     * hence make sure len>4 and not len>=4
     */
    if (len > 4) {
        yi += 4;
        YY[1] = vld1q_f32(yi);

        XX[0] = vld1q_f32(xi);
        xi += 4;

        SUMM = vfmaq_n_f32(SUMM, YY[0], vgetq_lane_f32(XX[0], 0));
        YEXT[0] = vextq_emu_f32(YY[0], YY[1], 1);
        SUMM = vfmaq_n_f32(SUMM, YEXT[0], vgetq_lane_f32(XX[0], 1));
        YEXT[1] = vextq_emu_f32(YY[0], YY[1], 2);
        SUMM = vfmaq_n_f32(SUMM, YEXT[1], vgetq_lane_f32(XX[0], 2));
        YEXT[2] = vextq_emu_f32(YY[0], YY[1], 3);
        SUMM = vfmaq_n_f32(SUMM, YEXT[2], vgetq_lane_f32(XX[0], 3));

        YY[0] = YY[1];
        len -= 4;
    }

    while (--len > 0) {
        XX_2 = *xi++;
        SUMM = vfmaq_n_f32(SUMM, YY[0], XX_2);
        YY[0] = vld1q_f32(++yi);
    }

    XX_2 = *xi++;
    SUMM = vfmaq_n_f32(SUMM, YY[0], XX_2);

    vst1q_f32(sum, SUMM);
}




void xcorr_kernel_mve_float(const float32_t * x, const float32_t * y,
                            float32_t sum[4], int len)
{
    float32x4_t     YY;
    float32x4_t     XX[2];
    float32_t       XX_2;
    float32x4_t     SUMM;
    const float32_t *xi = x;
    const float32_t *yi = y;

    celt_assert(len > 0);

    YY = vld1q_f32(yi);
    SUMM = vdupq_n_f32(0);

    /* Consume 8 elements in x vector and 12 elements in y
     * vector. However, the 12'th element never really gets
     * touched in this loop. So, if len == 8, then we only
     * must access y[0] to y[10]. y[11] must not be accessed
     * hence make sure len > 8 and not len >= 8
     */
    while (len > 8) {
        XX[0] = vld1q_f32(xi);
        xi += 4;
        XX[1] = vld1q_f32(xi);
        xi += 4;

        SUMM = vfmaq_n_f32(SUMM, YY, vgetq_lane_f32(XX[0], 0));
        YY = vld1q_f32(yi + 1);
        SUMM = vfmaq_n_f32(SUMM, YY, vgetq_lane_f32(XX[0], 1));
        YY = vld1q_f32(yi + 2);
        SUMM = vfmaq_n_f32(SUMM, YY, vgetq_lane_f32(XX[0], 2));
        YY = vld1q_f32(yi + 3);
        SUMM = vfmaq_n_f32(SUMM, YY, vgetq_lane_f32(XX[0], 3));

        YY = vld1q_f32(yi + 4);
        SUMM = vfmaq_n_f32(SUMM, YY, vgetq_lane_f32(XX[1], 0));
        YY = vld1q_f32(yi + 5);
        SUMM = vfmaq_n_f32(SUMM, YY, vgetq_lane_f32(XX[1], 1));
        YY = vld1q_f32(yi + 6);
        SUMM = vfmaq_n_f32(SUMM, YY, vgetq_lane_f32(XX[1], 2));
        YY = vld1q_f32(yi + 7);
        SUMM = vfmaq_n_f32(SUMM, YY, vgetq_lane_f32(XX[1], 3));

        YY = vld1q_f32(yi + 8);

        yi += 8;
        len -= 8;
    }

    /* Consume 4 elements in x vector and 8 elements in y
     * vector. However, the 8'th element in y never really gets
     * touched in this loop. So, if len == 4, then we only
     * must access y[0] to y[6]. y[7] must not be accessed
     * hence make sure len>4 and not len>=4
     */
    if (len > 4) {

        XX[0] = vld1q_f32(xi);
        xi += 4;

        SUMM = vfmaq_n_f32(SUMM, YY, vgetq_lane_f32(XX[0], 0));
        YY = vld1q_f32(yi + 1);
        SUMM = vfmaq_n_f32(SUMM, YY, vgetq_lane_f32(XX[0], 1));
        YY = vld1q_f32(yi + 2);
        SUMM = vfmaq_n_f32(SUMM, YY, vgetq_lane_f32(XX[0], 2));
        YY = vld1q_f32(yi + 3);
        SUMM = vfmaq_n_f32(SUMM, YY, vgetq_lane_f32(XX[0], 3));

        YY = vld1q_f32(yi + 4);
        yi += 4;
        len -= 4;
    }

    while (--len > 0) {
        XX_2 = *xi++;
        SUMM = vfmaq_n_f32(SUMM, YY, XX_2);
        YY = vld1q_f32(++yi);
    }

    XX_2 = *xi++;
    SUMM = vfmaq_n_f32(SUMM, YY, XX_2);

    vst1q_f32(sum, SUMM);
}





#endif

#if __ARM_FEATURE_NEON
void xcorr_kernel_neon_fixed(const opus_val16 * x, const opus_val16 * y,
                             opus_val32 sum[4], int len)
{
    int             j;
    int32x4_t       a = vld1q_s32(sum);
    /* Load y[0...3] */
    /* This requires len>0 to always be valid (which we assert in the C code). */
    int16x4_t       y0 = vld1_s16(y);
    y += 4;

    for (j = 0; j + 8 <= len; j += 8) {
        /* Load x[0...7] */
        int16x8_t       xx = vld1q_s16(x);
        int16x4_t       x0 = vget_low_s16(xx);
        int16x4_t       x4 = vget_high_s16(xx);
        /* Load y[4...11] */
        int16x8_t       yy = vld1q_s16(y);
        int16x4_t       y4 = vget_low_s16(yy);
        int16x4_t       y8 = vget_high_s16(yy);
        int32x4_t       a0 = vmlal_lane_s16(a, y0, x0, 0);
        int32x4_t       a1 = vmlal_lane_s16(a0, y4, x4, 0);

        int16x4_t       y1 = vext_s16(y0, y4, 1);
        int16x4_t       y5 = vext_s16(y4, y8, 1);
        int32x4_t       a2 = vmlal_lane_s16(a1, y1, x0, 1);
        int32x4_t       a3 = vmlal_lane_s16(a2, y5, x4, 1);

        int16x4_t       y2 = vext_s16(y0, y4, 2);
        int16x4_t       y6 = vext_s16(y4, y8, 2);
        int32x4_t       a4 = vmlal_lane_s16(a3, y2, x0, 2);
        int32x4_t       a5 = vmlal_lane_s16(a4, y6, x4, 2);

        int16x4_t       y3 = vext_s16(y0, y4, 3);
        int16x4_t       y7 = vext_s16(y4, y8, 3);
        int32x4_t       a6 = vmlal_lane_s16(a5, y3, x0, 3);
        int32x4_t       a7 = vmlal_lane_s16(a6, y7, x4, 3);

        y0 = y8;
        a = a7;
        x += 8;
        y += 8;
    }

    for (; j < len; j++) {
        int16x4_t       x0 = vld1_dup_s16(x);   /* load next x */
        int32x4_t       a0 = vmlal_s16(a, y0, x0);

        int16x4_t       y4 = vld1_dup_s16(y);   /* load next y */
        y0 = vext_s16(y0, y4, 1);
        a = a0;
        x++;
        y++;
    }

    vst1q_s32(sum, a);
}

/*
 * Function: xcorr_kernel_neon_float
 * ---------------------------------
 * Computes 4 correlation values and stores them in sum[4]
 */
void xcorr_kernel_neon_float(const float32_t * x, const float32_t * y,
                             float32_t sum[4], int len)
{
    float32x4_t     YY[3];
    float32x4_t     YEXT[3];
    float32x4_t     XX[2];
    float32x2_t     XX_2;
    float32x4_t     SUMM;
    const float32_t *xi = x;
    const float32_t *yi = y;

    celt_assert(len > 0);

    YY[0] = vld1q_f32(yi);
    SUMM = vdupq_n_f32(0);

    /* Consume 8 elements in x vector and 12 elements in y
     * vector. However, the 12'th element never really gets
     * touched in this loop. So, if len == 8, then we only
     * must access y[0] to y[10]. y[11] must not be accessed
     * hence make sure len > 8 and not len >= 8
     */
    while (len > 8) {
        yi += 4;
        YY[1] = vld1q_f32(yi);
        yi += 4;
        YY[2] = vld1q_f32(yi);

        XX[0] = vld1q_f32(xi);
        xi += 4;
        XX[1] = vld1q_f32(xi);
        xi += 4;

        SUMM = vmlaq_lane_f32(SUMM, YY[0], vget_low_f32(XX[0]), 0);
        YEXT[0] = vextq_f32(YY[0], YY[1], 1);
        SUMM = vmlaq_lane_f32(SUMM, YEXT[0], vget_low_f32(XX[0]), 1);
        YEXT[1] = vextq_f32(YY[0], YY[1], 2);
        SUMM = vmlaq_lane_f32(SUMM, YEXT[1], vget_high_f32(XX[0]), 0);
        YEXT[2] = vextq_f32(YY[0], YY[1], 3);
        SUMM = vmlaq_lane_f32(SUMM, YEXT[2], vget_high_f32(XX[0]), 1);

        SUMM = vmlaq_lane_f32(SUMM, YY[1], vget_low_f32(XX[1]), 0);
        YEXT[0] = vextq_f32(YY[1], YY[2], 1);
        SUMM = vmlaq_lane_f32(SUMM, YEXT[0], vget_low_f32(XX[1]), 1);
        YEXT[1] = vextq_f32(YY[1], YY[2], 2);
        SUMM = vmlaq_lane_f32(SUMM, YEXT[1], vget_high_f32(XX[1]), 0);
        YEXT[2] = vextq_f32(YY[1], YY[2], 3);
        SUMM = vmlaq_lane_f32(SUMM, YEXT[2], vget_high_f32(XX[1]), 1);

        YY[0] = YY[2];
        len -= 8;
    }

    /* Consume 4 elements in x vector and 8 elements in y
     * vector. However, the 8'th element in y never really gets
     * touched in this loop. So, if len == 4, then we only
     * must access y[0] to y[6]. y[7] must not be accessed
     * hence make sure len>4 and not len>=4
     */
    if (len > 4) {
        yi += 4;
        YY[1] = vld1q_f32(yi);

        XX[0] = vld1q_f32(xi);
        xi += 4;

        SUMM = vmlaq_lane_f32(SUMM, YY[0], vget_low_f32(XX[0]), 0);
        YEXT[0] = vextq_f32(YY[0], YY[1], 1);
        SUMM = vmlaq_lane_f32(SUMM, YEXT[0], vget_low_f32(XX[0]), 1);
        YEXT[1] = vextq_f32(YY[0], YY[1], 2);
        SUMM = vmlaq_lane_f32(SUMM, YEXT[1], vget_high_f32(XX[0]), 0);
        YEXT[2] = vextq_f32(YY[0], YY[1], 3);
        SUMM = vmlaq_lane_f32(SUMM, YEXT[2], vget_high_f32(XX[0]), 1);

        YY[0] = YY[1];
        len -= 4;
    }

    while (--len > 0) {
        XX_2 = vld1_dup_f32(xi++);
        SUMM = vmlaq_lane_f32(SUMM, YY[0], XX_2, 0);
        YY[0] = vld1q_f32(++yi);
    }

    XX_2 = vld1_dup_f32(xi);
    SUMM = vmlaq_lane_f32(SUMM, YY[0], XX_2, 0);

    vst1q_f32(sum, SUMM);
}

#endif
