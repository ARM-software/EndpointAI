/* https://developer.arm.com/architectures/instruction-sets/simd-isas/neon/neon-programmers-guide-for-armv8-a/optimizing-c-code-with-neon-intrinsics/single-page */

#include "rgb.h"





#if __ARM_FEATURE_MVE



void rgb_to_gray_helium(const uint8_t * rgb, uint8_t * gray, int num_pixels)
{
    uint16_t        w_r = (77);
    uint16_t        w_g = (150);
    uint16_t        w_b = (29);
    uint16x8_t      temp;
    uint16x8_t      result;

    num_pixels /= 8;

    for (int i = 0; i < num_pixels; ++i, rgb += 8 * 3, gray += 8) {
        int16x8x3_t     src = vld3_emu_u8(rgb);

        temp = vmulq_n_u16(src.val[0], w_r);
        temp = vmlaq_n_u16(temp, src.val[1], w_g);
        temp = vmlaq_n_u16(temp, src.val[2], w_b);

        result = vshrq_n_u16(temp, 8);

        vstrbq_u16(gray, result);
    }
}


void rgb_deinterleave_helium(uint8_t * r, uint8_t * g, uint8_t * b, uint8_t * rgb,
                             int len_color)
{
    /*
     * Take the elements of "rgb" and store the individual colors "r", "g", and "b"
     */
    int             num8x16 = len_color / 16;
    int8x16x3_t     intlv_rgb;
    for (int i = 0; i < num8x16; i++) {
        intlv_rgb = vld3q_emu_u8(rgb + 3 * 16 * i);
        vst1q_u8(r + 16 * i, intlv_rgb.val[0]);
        vst1q_u8(g + 16 * i, intlv_rgb.val[1]);
        vst1q_u8(b + 16 * i, intlv_rgb.val[2]);
    }
}

#endif

#if __ARM_FEATURE_NEON

void rgb_to_gray_neon(const uint8_t * rgb, uint8_t * gray, int num_pixels)
{
    uint8x8_t       w_r = vdup_n_u8(77);
    uint8x8_t       w_g = vdup_n_u8(150);
    uint8x8_t       w_b = vdup_n_u8(29);
    uint16x8_t      temp;
    uint8x8_t       result;

    num_pixels /= 8;

    for (int i = 0; i < num_pixels; ++i, rgb += 8 * 3, gray += 8) {
        uint8x8x3_t     src = vld3_u8(rgb);

        temp = vmull_u8(src.val[0], w_r);

        temp = vmlal_u8(temp, src.val[1], w_g);
        temp = vmlal_u8(temp, src.val[2], w_b);

        result = vshrn_n_u16(temp, 8);

        vst1_u8(gray, result);
    }
}

void rgb_deinterleave_neon(uint8_t * r, uint8_t * g, uint8_t * b, uint8_t * rgb,
                           int len_color)
{
    /*
     * Take the elements of "rgb" and store the individual colors "r", "g", and "b"
     */
    int             num8x16 = len_color / 16;
    uint8x16x3_t    intlv_rgb;
    for (int i = 0; i < num8x16; i++) {
        intlv_rgb = vld3q_u8(rgb + 3 * 16 * i);
        vst1q_u8(r + 16 * i, intlv_rgb.val[0]);
        vst1q_u8(g + 16 * i, intlv_rgb.val[1]);
        vst1q_u8(b + 16 * i, intlv_rgb.val[2]);
    }
}

#endif
