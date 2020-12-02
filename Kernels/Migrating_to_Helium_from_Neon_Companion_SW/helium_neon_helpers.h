/* ----------------------------------------------------------------------
 * Project:      mve vs neon conversion
 * Title:        opus_xcorr.h
 * Description:  opus xcorr sample to illustrate Neon to Helium comversion
 *
 * $Date:        Jul 2020
 * $Revision:    V.0.0.1
 *
 * Target Processor: Cortex-M cores with MVE support
 * -------------------------------------------------------------------- */
/*
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

#ifndef NEON_TO_HELIUM_HELPER
#define NEON_TO_HELIUM_HELPER


typedef struct {
    int16x8_t       val[3];
} int16x8x3_t;

typedef struct {
    int8x16_t       val[3];
} int8x16x3_t;


#define vmovq_n_emu_f32         vdupq_n_f32
#define vfmaq_laneq_emu_f32(acc, A, B, idx)  vfmaq_n_f32(acc, A, vgetq_lane_f32(B, idx))

#define vpaddlq_emu_u16(x) vaddq_u32(vmovlbq_u16(v_16x8), vmovltq_u16(v_16x8))
#define vpaddlq_emu_u32(b, x) \
            b = vsetq_lane_u64(vgetq_lane_u32(x, 0) + vgetq_lane_u32(x, 1), b, 0); \
            b = vsetq_lane_u64(vgetq_lane_u32(x, 2) + vgetq_lane_u32(x, 3), b, 1); \

#define vld1_emu_u8         vldrbq_u16
#define uint8x8_emu_t       uint16x8_t

#define vaddl_emu_u8        vaddq_u16
#define vaddw_emu_u8        vaddq_u16

#define vmlaq_emu_f32       vfmaq_f32
#define vmlsq_emu_f32       vfmsq_f32


#define DONT_CARE (-1)


#define GENERIC_EXT_1(dst, a, b)                \
    dst = vsetq_lane(vgetq_lane(a, 1), dst, 0); \
    dst = vsetq_lane(vgetq_lane(a, 2), dst, 1); \
    dst = vsetq_lane(vgetq_lane(a, 3), dst, 2); \
    dst = vsetq_lane(vgetq_lane(b, 0), dst, 3);

#define GENERIC_EXT_2(dst, a, b)                \
    dst = vsetq_lane(vgetq_lane(a, 2), dst, 0); \
    dst = vsetq_lane(vgetq_lane(a, 3), dst, 1); \
    dst = vsetq_lane(vgetq_lane(b, 0), dst, 2); \
    dst = vsetq_lane(vgetq_lane(b, 1), dst, 3);

#define GENERIC_EXT_3(dst, a, b)                \
    dst = vsetq_lane(vgetq_lane(a, 3), dst, 0); \
    dst = vsetq_lane(vgetq_lane(b, 0), dst, 1); \
    dst = vsetq_lane(vgetq_lane(b, 1), dst, 2); \
    dst = vsetq_lane(vgetq_lane(b, 2), dst, 3);

static inline float32x4_t vextq_f32_1_heavy(float32x4_t a, float32x4_t b)
{
    float32x4_t     dst = vuninitializedq_f32();
    GENERIC_EXT_1(dst, a, b);
    return dst;
}

static inline float32x4_t vextq_f32_1(float32x4_t a, float32x4_t b)
{
    return __builtin_shufflevector(a, b, 1, 2, 3, 4);
}

static inline int32x4_t vextq_s32_1(int32x4_t a, int32x4_t b)
{
    return __builtin_shufflevector(a, b, 1, 2, 3, 4);
}

static inline float32x4_t vextq_f32_2(float32x4_t a, float32x4_t b)
{
    return __builtin_shufflevector(a, b, 2, 3, 4, 5);
}

static inline int32x4_t vextq_s32_2(int32x4_t a, int32x4_t b)
{
    return __builtin_shufflevector(a, b, 2, 3, 4, 5);
}

static inline float32x4_t vextq_f32_3(float32x4_t a, float32x4_t b)
{
    return __builtin_shufflevector(a, b, 3, 4, 5, 6);
}

static inline int32x4_t vextq_s32_3(int32x4_t a, int32x4_t b)
{
    return __builtin_shufflevector(a, b, 3, 4, 5, 6);
}


static inline float32x4_t vextq_emu_f32(float32x4_t a, float32x4_t b, const int32_t imm)
{
    float32x4_t     dst;

    switch (imm) {
      case 1:
          dst = vextq_f32_1(a, b);
          break;
      case 2:
          dst = vextq_f32_2(a, b);
          break;
      case 3:
          dst = vextq_f32_3(a, b);
          break;
    }
    return dst;
}

static inline int32x4_t vext_emu_s16(int32x4_t a, int32x4_t b, const int32_t imm)
{
    int32x4_t       dst;

    switch (imm) {
      case 1:
          dst = vextq_s32_1(a, b);
          break;
      case 2:
          dst = vextq_s32_2(a, b);
          break;
      case 3:
          dst = vextq_s32_3(a, b);
          break;
    }
    return dst;
}


static inline float32x4x2_t vtrnq_emu_f32(float32x4_t a, float32x4_t b)
{
    float32x4x2_t   dst;

    dst.val[0] = __builtin_shufflevector(a, b, 0, 4, 2, 6);
    dst.val[1] = __builtin_shufflevector(a, b, 1, 5, 3, 7);
    return dst;
}

static inline float32x4_t vcombine_emu_f32(float32x4_t a, float32x4_t b)
{
    return __builtin_shufflevector(a, b, 0, 1, 4, 5);
}

static inline float32x4_t vget_low_emu_f32(float32x4_t a)
{
    return __builtin_shufflevector(a, a, 0, 1, DONT_CARE, DONT_CARE);
}

static inline float32x4_t vget_high_emu_f32(float32x4_t a)
{
    return __builtin_shufflevector(a, a, 2, 3, DONT_CARE, DONT_CARE);
}

static inline float32x4x2_t vzipq_emu_f32(float32x4_t x, float32x4_t y)
{
    float32x4x2_t   dst;
    dst.val[0] = __builtin_shufflevector(x, y, 0, 4, 1, 5);
    dst.val[1] = __builtin_shufflevector(x, y, 2, 6, 3, 7);
    return dst;
}

static inline float32x4x2_t vuzpq_emu_f32(float32x4_t x, float32x4_t y)
{
    float32x4x2_t   dst;
    dst.val[0] = __builtin_shufflevector(x, y, 0, 2, 4, 6);
    dst.val[1] = __builtin_shufflevector(x, y, 1, 3, 5, 7);
    return dst;
}


static inline int16x8x3_t vld3_emu_u8(const uint8_t * base)
{
    int16x8x3_t     dst;
    int16x8_t       index;

    index = vidupq_n_u16(0, 1);
    index = vmulq_n_u16(index, 3);

    dst.val[0] = vldrbq_gather_offset_u16(base, index);
    dst.val[1] = vldrbq_gather_offset_u16(base + 1, index);
    dst.val[2] = vldrbq_gather_offset_u16(base + 2, index);

    return dst;
}

static inline int8x16x3_t vld3q_emu_u8(const uint8_t * base)
{
    int8x16x3_t     dst;
    int16x8_t       index;

    index = vidupq_n_u16(0, 1);
    index = vmulq_n_u16(index, 3);

    dst.val[0] = vldrbq_gather_offset_u8(base, index);
    dst.val[1] = vldrbq_gather_offset_u8(base + 1, index);
    dst.val[2] = vldrbq_gather_offset_u8(base + 2, index);

    return dst;
}

#endif
