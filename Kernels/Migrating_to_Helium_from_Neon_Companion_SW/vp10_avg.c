/*
 *  Copyright (c) 2015 The WebM project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */


#include "vp10_avg.h"




#if __ARM_FEATURE_MVE

static INLINE unsigned int horizontal_add_u16x8(const uint16x8_t v_16x8)
{
    uint32x4_t      a = vpaddlq_emu_u16(v_16x8);
    uint64x2_t      b = vuninitializedq_u32();

    vpaddlq_emu_u32(b, a);
    uint32_t        c = (uint32_t) vgetq_lane_u64(b, 0) + vgetq_lane_u64(b, 1);
    return c;
}


unsigned int vp10_avg_8x8_helium_direct(const uint8_t * s, int p)
{
    uint8x8_emu_t   v_s0 = vld1_emu_u8(s);
    const uint8x8_emu_t v_s1 = vld1_emu_u8(s + p);
    uint16x8_t      v_sum = vaddl_emu_u8(v_s0, v_s1);
    v_s0 = vld1_emu_u8(s + 2 * p);
    v_sum = vaddw_emu_u8(v_sum, v_s0);
    v_s0 = vld1_emu_u8(s + 3 * p);
    v_sum = vaddw_emu_u8(v_sum, v_s0);
    v_s0 = vld1_emu_u8(s + 4 * p);
    v_sum = vaddw_emu_u8(v_sum, v_s0);
    v_s0 = vld1_emu_u8(s + 5 * p);
    v_sum = vaddw_emu_u8(v_sum, v_s0);
    v_s0 = vld1_emu_u8(s + 6 * p);
    v_sum = vaddw_emu_u8(v_sum, v_s0);
    v_s0 = vld1_emu_u8(s + 7 * p);
    v_sum = vaddw_emu_u8(v_sum, v_s0);
    return (horizontal_add_u16x8(v_sum) + 32) >> 6;
}

unsigned int vp10_avg_8x8_helium(const uint8_t * s, int p)
{

    uint8x16_t      v_s0 = vldrbq_u16(s);
    uint32_t        v_sum = vaddvq_u16(v_s0);
    v_s0 = vldrbq_u16(s + p);
    v_sum = vaddvaq_u16(v_sum, v_s0);
    v_s0 = vldrbq_u16(s + 2 * p);
    v_sum = vaddvaq_u16(v_sum, v_s0);
    v_s0 = vldrbq_u16(s + 3 * p);
    v_sum = vaddvaq_u16(v_sum, v_s0);
    v_s0 = vldrbq_u16(s + 4 * p);
    v_sum = vaddvaq_u16(v_sum, v_s0);
    v_s0 = vldrbq_u16(s + 5 * p);
    v_sum = vaddvaq_u16(v_sum, v_s0);
    v_s0 = vldrbq_u16(s + 6 * p);
    v_sum = vaddvaq_u16(v_sum, v_s0);
    v_s0 = vldrbq_u16(s + 7 * p);
    v_sum = vaddvaq_u16(v_sum, v_s0);
    return ((v_sum) + 32) >> 6;
}



/* 0 < p < 255 */
unsigned int vp10_avg_8x8_helium_scat_gath(const uint8_t * s, int p)
{
    /* 
       build {
       0, 1, 2, 3, 4, 5, 6, 7,
       p+0, p+1, p+2, p+3, p+4, p+5, p+6, p+7
       }
     */
    uint8x16_t      v_s0;
    uint32_t        v_sum;
    uint8x16_t      idx = viwdupq_n_u8(0, 8, 1);
    mve_pred16_t    pred = 0xff00;
    idx = vaddq_x_n_u8(idx, p, pred);

    v_s0 = vldrbq_gather_offset_u8(s, idx);
    v_sum = vaddvq_u8(v_s0);

    v_s0 = vldrbq_gather_offset_u8(s + 2 * p, idx);
    v_sum = vaddvaq_u8(v_sum, v_s0);

    v_s0 = vldrbq_gather_offset_u8(s + 4 * p, idx);
    v_sum = vaddvaq_u8(v_sum, v_s0);

    v_s0 = vldrbq_gather_offset_u8(s + 6 * p, idx);
    v_sum = vaddvaq_u8(v_sum, v_s0);

    return ((v_sum) + 32) >> 6;
}


#endif

#if __ARM_FEATURE_NEON
static INLINE unsigned int horizontal_add_u16x8(const uint16x8_t v_16x8)
{
    const uint32x4_t a = vpaddlq_u16(v_16x8);
    const uint64x2_t b = vpaddlq_u32(a);
    const uint32x2_t c = vadd_u32(vreinterpret_u32_u64(vget_low_u64(b)),
                                  vreinterpret_u32_u64(vget_high_u64(b)));
    return vget_lane_u32(c, 0);
}

unsigned int vp10_avg_8x8_neon(const uint8_t * s, int p)
{
    uint8x8_t       v_s0 = vld1_u8(s);
    const uint8x8_t v_s1 = vld1_u8(s + p);
    uint16x8_t      v_sum = vaddl_u8(v_s0, v_s1);
    v_s0 = vld1_u8(s + 2 * p);
    v_sum = vaddw_u8(v_sum, v_s0);
    v_s0 = vld1_u8(s + 3 * p);
    v_sum = vaddw_u8(v_sum, v_s0);
    v_s0 = vld1_u8(s + 4 * p);
    v_sum = vaddw_u8(v_sum, v_s0);
    v_s0 = vld1_u8(s + 5 * p);
    v_sum = vaddw_u8(v_sum, v_s0);
    v_s0 = vld1_u8(s + 6 * p);
    v_sum = vaddw_u8(v_sum, v_s0);
    v_s0 = vld1_u8(s + 7 * p);
    v_sum = vaddw_u8(v_sum, v_s0);
    return (horizontal_add_u16x8(v_sum) + 32) >> 6;
}

#endif
