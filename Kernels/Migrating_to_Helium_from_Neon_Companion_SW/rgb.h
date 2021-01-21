
#ifndef RGB2GR
#define RGB2GR

#include <stdint.h>
#include <limits.h>
#include <float.h>
#include <math.h>
#if __ARM_FEATURE_NEON
#include <arm_neon.h>
#endif
#if __ARM_FEATURE_MVE
#include <arm_mve.h>
#include <arm_math_types.h>
#include "helium_neon_helpers.h"
#endif


#if __ARM_FEATURE_NEON
void rgb_to_gray_neon(const uint8_t * rgb, uint8_t * gray, int num_pixels);
void rgb_deinterleave_neon(uint8_t * r, uint8_t * g, uint8_t * b, uint8_t * rgb,
                             int len_color);
#endif

#if __ARM_FEATURE_MVE
void rgb_to_gray_helium(const uint8_t * rgb, uint8_t * gray, int num_pixels);
void rgb_deinterleave_helium(uint8_t * r, uint8_t * g, uint8_t * b, uint8_t * rgb,
                             int len_color);
#endif

#endif