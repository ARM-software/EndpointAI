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

#ifndef OPUS_XCORR_H
#define OPUS_XCORR_H

#include <stdint.h>
#if __ARM_FEATURE_NEON
#include <arm_neon.h>
#endif
#if __ARM_FEATURE_MVE
#include <arm_mve.h>
#include "helium_neon_helpers.h"
#endif

/* opus types */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
typedef int16_t opus_int16;
typedef uint16_t opus_uint16;
typedef int     opus_int;
typedef int32_t opus_int32;
typedef uint32_t opus_uint32;
typedef opus_int16 opus_val16;
typedef opus_int32 opus_val32;
typedef opus_val32 celt_sig;
typedef opus_val32 celt_ener;
typedef opus_val16 celt_norm;

#define opus_int64       long long
#define opus_uint64      unsigned long long


/** Add two 16-bit values */
#define ADD16(a,b) ((opus_val16)((opus_val16)(a)+(opus_val16)(b)))
/** Subtract two 16-bit values */
#define SUB16(a,b) ((opus_val16)(a)-(opus_val16)(b))
/** Add two 32-bit values */
#define ADD32(a,b) ((opus_val32)(a)+(opus_val32)(b))
/** Subtract two 32-bit values */
#define SUB32(a,b) ((opus_val32)(a)-(opus_val32)(b))
/** 16x16 multiplication where the result fits in 16 bits */
#define MULT16_16_16(a,b)     ((((opus_val16)(a))*((opus_val16)(b))))

/* (opus_val32)(opus_val16) gives TI compiler a hint that it's 16x16->32 multiply */
/** 16x16 multiplication where the result fits in 32 bits */
#define MULT16_16(a,b)     (((opus_val32)(opus_val16)(a))*((opus_val32)(opus_val16)(b)))

/** 16x16 multiply-add where the result fits in 32 bits */
#define MAC16_16(c,a,b) (ADD32((c),MULT16_16((a),(b))))

#define celt_assert(cond)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void            xcorr_kernel_c(const opus_val16 * x, const opus_val16 * y,
                               opus_val32 sum[4], int len);

#if __ARM_FEATURE_NEON
void xcorr_kernel_neon_fixed(const opus_val16 * x, const opus_val16 * y, opus_val32 sum[4], int len);
void xcorr_kernel_neon_float(const float32_t *x, const float32_t *y,
      float32_t sum[4], int len); 
#endif    


#if __ARM_FEATURE_MVE
void xcorr_kernel_mve_fixed(const opus_val16 * x, const opus_val16 * y, opus_val32 sum[4], int len);
void xcorr_kernel_mve_float(const float32_t *x, const float32_t *y,
      float32_t sum[4], int len); 

#endif

#endif                          /*  VECTOR_SAMPLE_H */
