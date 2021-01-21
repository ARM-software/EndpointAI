
#ifndef VP10_AVG
#define VP10_AVG

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

#define INLINE inline

#if __ARM_FEATURE_NEON
unsigned int vp10_avg_8x8_neon(const uint8_t *s, int p); 
#endif

#if __ARM_FEATURE_MVE
unsigned int vp10_avg_8x8_helium(const uint8_t *s, int p);
#endif

#endif