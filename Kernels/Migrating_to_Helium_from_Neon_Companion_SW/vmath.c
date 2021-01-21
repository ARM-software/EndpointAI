/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        cmplx_dot.c
 * Description:  Floating-point complex dot product
 *
 * $Date:        18. March 2019
 * $Revision:    V1.6.0
 *
 * Target Processor: Cortex-M cores
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

#include "vmath.h"


/** Logarithm polynomial coefficients */
const float32_t log_tab[4 * 8] = {
    -2.29561495781f, -2.29561495781f, -2.29561495781f, -2.29561495781f,
    -2.47071170807f, -2.47071170807f, -2.47071170807f, -2.47071170807f,
    -5.68692588806f, -5.68692588806f, -5.68692588806f, -5.68692588806f,
    -0.165253549814f, -0.165253549814f, -0.165253549814f, -0.165253549814f,
    5.17591238022f, 5.17591238022f, 5.17591238022f, 5.17591238022f,
    0.844007015228f, 0.844007015228f, 0.844007015228f, 0.844007015228f,
    4.58445882797f, 4.58445882797f, 4.58445882797f, 4.58445882797f,
    0.0141278216615f, 0.0141278216615f, 0.0141278216615f, 0.0141278216615f
};

const float32_t exp_tab[4 * 8] = {
    1.f, 1.f, 1.f, 1.f,
    0.0416598916054f, 0.0416598916054f, 0.0416598916054f, 0.0416598916054f,
    0.500000596046f, 0.500000596046f, 0.500000596046f, 0.500000596046f,
    0.0014122662833f, 0.0014122662833f, 0.0014122662833f, 0.0014122662833f,
    1.00000011921f, 1.00000011921f, 1.00000011921f, 1.00000011921f,
    0.00833693705499f, 0.00833693705499f, 0.00833693705499f, 0.00833693705499f,
    0.166665703058f, 0.166665703058f, 0.166665703058f, 0.166665703058f,
    0.000195780929062f, 0.000195780929062f, 0.000195780929062f, 0.000195780929062f
};

#define LOG2    0.6931471805f
#define INVLOG2 1.4426950408f
#define M_PI	3.14159265358979f


/** Sin polynomial coefficients */
float           te_sin_coeff2 = 0.166666666666f;        // 1/(2*3)
float           te_sin_coeff3 = 0.05f;  // 1/(4*5)
float           te_sin_coeff4 = 0.023809523810f;        // 1/(6*7)
float           te_sin_coeff5 = 0.013888888889f;        // 1/(8*9)



#if __ARM_FEATURE_MVE

static inline float32x4_t vtaylor_polyq_f32(float32x4_t x, const float32_t * coeffs)
{
    float32x4_t     A =
        vmlaq_emu_f32(vld1q_f32(&coeffs[4 * 0]), vld1q_f32(&coeffs[4 * 4]), x);
    float32x4_t     B =
        vmlaq_emu_f32(vld1q_f32(&coeffs[4 * 2]), vld1q_f32(&coeffs[4 * 6]), x);
    float32x4_t     C =
        vmlaq_emu_f32(vld1q_f32(&coeffs[4 * 1]), vld1q_f32(&coeffs[4 * 5]), x);
    float32x4_t     D =
        vmlaq_emu_f32(vld1q_f32(&coeffs[4 * 3]), vld1q_f32(&coeffs[4 * 7]), x);
    float32x4_t     x2 = vmulq_f32(x, x);
    float32x4_t     x4 = vmulq_f32(x2, x2);
    float32x4_t     res = vmlaq_emu_f32(vmlaq_emu_f32(A, B, x2), vmlaq_emu_f32(C, D, x2), x4);
    return res;
}



float32x4_t vlogq_helium_f32_direct(float32x4_t x)
{
    // Extract exponent
    int32x4_t       m =
        vsubq_s32(vreinterpretq_s32_u32(vshrq_n_u32(vreinterpretq_u32_f32(x), 23)),
                  vdupq_n_s32(127));
    float32x4_t     val =
        vreinterpretq_f32_s32(vsubq_s32(vreinterpretq_s32_f32(x), vshlq_n_s32(m, 23)));

    // Polynomial Approximation
    float32x4_t     poly = vtaylor_polyq_f32(val, log_tab);

    // Reconstruct
    poly = vfmaq_f32(poly, vcvtq_f32_s32(m), vdupq_n_f32(LOG2));

    return poly;
}

float32x4_t vlogq_helium_f32(float32x4_t x)
{
    // Extract exponent
    int32x4_t       m =
        vsubq_n_s32(vreinterpretq_s32_u32(vshrq_n_u32(vreinterpretq_u32_f32(x), 23)),
                    127);
    float32x4_t     val =
        vreinterpretq_f32_s32(vsubq_s32(vreinterpretq_s32_f32(x), vshlq_n_s32(m, 23)));

    // Polynomial Approximation
    float32x4_t     poly = vtaylor_polyq_f32(val, log_tab);

    // Reconstruct
    poly = vfmaq_n_f32(poly, vcvtq_f32_s32(m), LOG2);

    return poly;
}

float32x4_t vexpq_helium_f32_direct(float32x4_t x)
{
    // Perform range reduction [-log(2),log(2)]
    int32x4_t       m = vcvtq_s32_f32(vmulq_f32(x, vdupq_n_f32(INVLOG2)));
    float32x4_t     val = vmlsq_emu_f32(x, vcvtq_f32_s32(m), vdupq_n_f32(LOG2));

    // Polynomial Approximation
    float32x4_t     poly = vtaylor_polyq_f32(val, exp_tab);

    // Reconstruct
    poly =
        vreinterpretq_f32_s32(vqaddq_s32
                              (vreinterpretq_s32_f32(poly), vqshlq_n_s32(m, 23)));
    poly = vpselq(vdupq_n_f32(0.0f), poly, vcmpltq_s32(m, vdupq_n_s32(-126)));

    return poly;
}

float32x4_t vexpq_helium_f32(float32x4_t x)
{
    // Perform range reduction [-log(2),log(2)]
    int32x4_t       m = vcvtq_s32_f32(vmulq_n_f32(x, INVLOG2));
    float32x4_t     val = vfmsq_f32(x, vcvtq_f32_s32(m), vdupq_n_f32(LOG2));

    // Polynomial Approximation
    float32x4_t     poly = vtaylor_polyq_f32(val, exp_tab);

    // Reconstruct
    poly =
        vreinterpretq_f32_s32(vqaddq_s32
                              (vreinterpretq_s32_f32(poly), vqshlq_n_s32(m, 23)));
    poly = vdupq_m_n_f32(poly, 0.0f, vcmpltq_n_s32(m, -126));

    return poly;
}



float32x4_t vsinq_helium_f32_direct(float32x4_t val)
{
    const float32x4_t pi_v = vdupq_n_f32(M_PI);
    const float32x4_t pio2_v = vdupq_n_f32(M_PI / 2);
    const float32x4_t ipi_v = vdupq_n_f32(1 / M_PI);

    //Find positive or negative
    const int32x4_t c_v = vabsq_s32(vcvtq_s32_f32(vmulq_f32(val, ipi_v)));
    mve_pred16_t    pred = vcmpleq_f32(val, vdupq_n_f32(0));
    const uint32x4_t sign_v = vpselq_u32(vdupq_n_u32(0xffffffff), vdupq_n_u32(0), pred);
    const uint32x4_t odd_v = vandq_u32(vreinterpretq_u32_s32(c_v), vdupq_n_u32(1));

    uint32x4_t      neg_v = veorq_u32(odd_v, sign_v);

    //Modulus a - (n * int(a*(1/n)))
    float32x4_t     ma = vsubq_f32(vabsq_f32(val), vmulq_f32(pi_v, vcvtq_f32_s32(c_v)));
    mve_pred16_t    reb_v = vcmpgeq_f32(ma, pio2_v);

    //Rebase a between 0 and pi/2
    ma = vpselq_f32(vsubq_f32(pi_v, ma), ma, reb_v);

    //Taylor series
    const float32x4_t ma2 = vmulq_f32(ma, ma);

    //2nd elem: x^3 / 3!
    float32x4_t     elem = vmulq_f32(vmulq_f32(ma, ma2), vdupq_n_f32(te_sin_coeff2));
    float32x4_t     res = vsubq_f32(ma, elem);

    //3rd elem: x^5 / 5!
    elem = vmulq_f32(vmulq_f32(elem, ma2), vdupq_n_f32(te_sin_coeff3));
    res = vaddq_f32(res, elem);

    //4th elem: x^7 / 7!float32x2_t vsin_f32(float32x2_t val)
    elem = vmulq_f32(vmulq_f32(elem, ma2), vdupq_n_f32(te_sin_coeff4));
    res = vsubq_f32(res, elem);

    //5th elem: x^9 / 9!
    elem = vmulq_f32(vmulq_f32(elem, ma2), vdupq_n_f32(te_sin_coeff5));
    res = vaddq_f32(res, elem);

    //Change of sign
    neg_v = vshlq_n_u32(neg_v, 31);
    res = vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(res), neg_v));
    return res;
}




float32x4_t vsinq_helium_f32(float32x4_t val)
{
    const float32_t pi_v = M_PI;
    const float32_t pio2_v = M_PI / 2;
    const float32_t ipi_v = 1 / M_PI;

    //Find positive or negative
    const int32x4_t c_v = vabsq_s32(vcvtq_s32_f32(vmulq_n_f32(val, ipi_v)));
    mve_pred16_t    pred = vcmpleq_f32(val, vdupq_n_f32(0));
    const uint32x4_t sign_v = vpselq_u32(vdupq_n_u32(0xffffffff), vdupq_n_u32(0), pred);
    const uint32x4_t odd_v = vandq_u32(vreinterpretq_u32_s32(c_v), vdupq_n_u32(1));

    uint32x4_t      neg_v = veorq_u32(odd_v, sign_v);

    //Modulus a - (n * int(a*(1/n)))
    float32x4_t     ma = vsubq_f32(vabsq_f32(val), vmulq_n_f32(vcvtq_f32_s32(c_v), pi_v));
    mve_pred16_t    reb_v = vcmpgeq_n_f32(ma, pio2_v);

    //Rebase a between 0 and pi/2
    ma = vpselq_f32(vsubq_f32(vdupq_n_f32(pi_v), ma), ma, reb_v);

    //Taylor series
    const float32x4_t ma2 = vmulq_f32(ma, ma);

    //2nd elem: x^3 / 3!
    float32x4_t     elem = vmulq_n_f32(vmulq_f32(ma, ma2), te_sin_coeff2);
    float32x4_t     res = vsubq_f32(ma, elem);

    //3rd elem: x^5 / 5!
    elem = vmulq_n_f32(vmulq_f32(elem, ma2), te_sin_coeff3);
    res = vaddq_f32(res, elem);

    //4th elem: x^7 / 7!float32x2_t vsin_f32(float32x2_t val)
    elem = vmulq_n_f32(vmulq_f32(elem, ma2), te_sin_coeff4);
    res = vsubq_f32(res, elem);

    //5th elem: x^9 / 9!
    elem = vmulq_n_f32(vmulq_f32(elem, ma2), te_sin_coeff5);
    res = vaddq_f32(res, elem);

    //Change of sign
    neg_v = vshlq_n_u32(neg_v, 31);
    res = vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(res), neg_v));
    return res;
}

float32_t __invSqrtf32(float32_t x)
{
    float32_t       xhalf = 0.5f * x;
    int             i = *(int *) &x;

    /* gives initial guess */
    i = INVSQRT_MAGIC_F32 - (i >> 1);
    x = *(float32_t *) & i;
    /* Newton */
    x = x * (1.5f - xhalf * x * x);
    /* Newton    */
    x = x * (1.5f - xhalf * x * x);
    /* Newton */
    x = x * (1.5f - xhalf * x * x);
    /* etc .. */
    return x;
}


float32x4_t vinvsqrtq_helium_f32(float32x4_t vecIn)
{
    int32x4_t       vecNewtonInit = vdupq_n_s32(INVSQRT_MAGIC_F32);
    float32x4_t     vecOneHandHalf = vdupq_n_f32(1.5f);
    float32x4_t     sqrt_reciprocal;
    float32x4_t     vecHalf;
    int32x4_t       vecTmpInt;
    float32x4_t     vecTmpFlt1;


    vecHalf = vmulq(vecIn, 0.5f);
    /*
     * cast input float vector to integer and right shift by 1
     */
    vecTmpInt = vshrq_n_s32((int32x4_t) vecIn, 1);
    /*
     * INVSQRT_MAGIC - ((vec_q32_t)vecIn >> 1)
     */
    sqrt_reciprocal = vreinterpretq_f32_s32(vsubq(vecNewtonInit, vecTmpInt));
    /*
     * 1st iteration
     * (1.5f-xhalf*x*x)
     */
    vecTmpFlt1 = vmulq(sqrt_reciprocal, sqrt_reciprocal);
    vecTmpFlt1 = vfmsq_f32(vecOneHandHalf, vecTmpFlt1, vecHalf);
    /*
     * x = x*(1.5f-xhalf*x*x);
     */
    sqrt_reciprocal = vmulq(sqrt_reciprocal, vecTmpFlt1);

    /*
     * 2nd iteration
     */
    vecTmpFlt1 = vmulq(sqrt_reciprocal, sqrt_reciprocal);
    vecTmpFlt1 = vfmsq_f32(vecOneHandHalf, vecTmpFlt1, vecHalf);
    sqrt_reciprocal = vmulq(sqrt_reciprocal, vecTmpFlt1);

    /*
     * 3rd iteration
     */
    vecTmpFlt1 = vmulq(sqrt_reciprocal, sqrt_reciprocal);
    vecTmpFlt1 = vfmsq_f32(vecOneHandHalf, vecTmpFlt1, vecHalf);
    sqrt_reciprocal = vmulq(sqrt_reciprocal, vecTmpFlt1);

    /*
     * set negative values to NAN
     */
    sqrt_reciprocal = vdupq_m(sqrt_reciprocal, NAN, vcmpltq(vecIn, 0.0f));
    sqrt_reciprocal = vdupq_m(sqrt_reciprocal, INFINITY, vcmpeqq(vecIn, 0.0f));
    return sqrt_reciprocal;
}


static inline float32x4_t vinvsqrtq_helium_step_f32(float32x4_t sqrt_reciprocal,
                                                    float32x4_t vecHalf)
{
    float32x4_t     vecOneHandHalf = vdupq_n_f32(1.5f);
    float32x4_t     tmp;
    /*
     * 1st iteration
     * (1.5f-xhalf*x*x)
     */
    tmp = vmulq(sqrt_reciprocal, sqrt_reciprocal);
    tmp = vfmsq_f32(vecOneHandHalf, tmp, vecHalf);
    /*
     * x = x*(1.5f-xhalf*x*x);
     */
    sqrt_reciprocal = vmulq(sqrt_reciprocal, tmp);
    return sqrt_reciprocal;

}

static inline float32x4_t vinvsqrtq_helium_init_f32(float32x4_t vecIn)
{
    int32x4_t       vecNewtonInit = vdupq_n_s32(INVSQRT_MAGIC_F32);
    float32x4_t     sqrt_reciprocal;
    int32x4_t       vecTmpInt;

    /*
     * cast input float vector to integer and right shift by 1
     */
    vecTmpInt = vshrq_n_s32((int32x4_t) vecIn, 1);
    /*
     * INVSQRT_MAGIC - ((vec_q32_t)vecIn >> 1)
     */
    sqrt_reciprocal = vreinterpretq_f32_s32(vsubq(vecNewtonInit, vecTmpInt));

    return sqrt_reciprocal;
}


float32x4_t vinvsqrtq_helium_f32_neon_like(float32x4_t vecIn)
{
    float32x4_t     sqrt_reciprocal;
    float32x4_t     vecHalf;

    vecHalf = vmulq(vecIn, 0.5f);

    sqrt_reciprocal = vinvsqrtq_helium_init_f32(vecIn); /* vrsqrteq_f32 equiv. */

    sqrt_reciprocal = vinvsqrtq_helium_step_f32(sqrt_reciprocal, vecHalf);      /* vrsqrtsq_f32 equiv. */
    sqrt_reciprocal = vinvsqrtq_helium_step_f32(sqrt_reciprocal, vecHalf);
    sqrt_reciprocal = vinvsqrtq_helium_step_f32(sqrt_reciprocal, vecHalf);

    /*
     * handle negative & 0 values
     */
    sqrt_reciprocal = vdupq_m(sqrt_reciprocal, NAN, vcmpltq(vecIn, 0.0f));
    sqrt_reciprocal = vdupq_m(sqrt_reciprocal, INFINITY, vcmpeqq(vecIn, 0.0f));
    return sqrt_reciprocal;
}


static inline float32x4_t vrecpq_step_f32(float32x4_t x, float32x4_t ax)
{
    float32x4_t     b;
    float32x4_t     v2f = vdupq_n_f32(2.0f);

    b = vfmsq(v2f, x, ax);
    x = vmulq_f32(x, b);
    return x;
}


static inline float32x4_t vrecpq_init_f32(float32x4_t ax)
{
    any32x4_t       xinv;
    int32x4_t       m;

    xinv.f = ax;
    m = vsubq(vdupq_n_s32(0x3F800000), vandq_s32(xinv.i, vdupq_n_s32(0x7F800000)));
    xinv.i = vaddq_s32(xinv.i, m);
    xinv.f = vfmsq_f32(vdupq_n_f32(1.41176471f), vdupq_n_f32(0.47058824f), xinv.f);
    xinv.i = vaddq_s32(xinv.i, m);

    return xinv.f;
}



float32x4_t vinvq_helium_f32(float32x4_t x)
{
    int32x4_t       m;
    float32x4_t     b;
    any32x4_t       xinv;
    float32x4_t     ax = vabsq(x);
    float32x4_t     v2f = vdupq_n_f32(2.0f);

    xinv.f = ax;
    m = vsubq(vdupq_n_s32(0x3F800000), vandq_s32(xinv.i, vdupq_n_s32(0x7F800000)));
    xinv.i = vaddq_s32(xinv.i, m);
    xinv.f = vfmsq_f32(vdupq_n_f32(1.41176471f), vdupq_n_f32(0.47058824f), xinv.f);
    xinv.i = vaddq_s32(xinv.i, m);

    b = vfmsq(v2f, xinv.f, ax);
    xinv.f = vmulq_f32(xinv.f, b);

    b = vfmsq(v2f, xinv.f, ax);
    xinv.f = vmulq_f32(xinv.f, b);

    b = vfmsq(v2f, xinv.f, ax);
    xinv.f = vmulq_f32(xinv.f, b);

    /*
     * restore sign + handle division by 0
     */
    xinv.f = vdupq_m(xinv.f, INFINITY, vcmpeqq(x, 0.0f));
    xinv.f = vnegq_m(xinv.f, xinv.f, vcmpltq(x, 0.0f));
    return xinv.f;
}


float32x4_t vinvq_helium_f32_neon_like(float32x4_t x)
{
    float32x4_t     recip;
    float32x4_t     ax = vabsq(x);

    recip = vrecpq_init_f32(ax);        /* vrecpeq_f32 eq */

    recip = vrecpq_step_f32(recip, ax); /* vrecpsq_f32 */
    recip = vrecpq_step_f32(recip, ax);
    recip = vrecpq_step_f32(recip, ax);

    /*
     * restore sign + handle division by 0
     */
    recip = vdupq_m(recip, INFINITY, vcmpeqq(x, 0.0f));
    recip = vnegq_x_f32(recip, vcmpltq(x, 0.0f));
    return recip;
}

static inline float32x4_t vdiv_helium_f32( float32x4_t num, float32x4_t den)
{
    return vmulq_f32(num, vinvq_helium_f32_neon_like(den));
}

static inline float32x4_t vsqrtq_helium_f32( float32x4_t in)
{
    float32x4_t         dst;
    
    dst = vinvsqrtq_helium_f32_neon_like(in);
    dst = vdupq_m_n_f32(dst, 0.0f, vcmpeqq_n_f32(in, 0.0f));
    dst = vmulq_f32(dst, in);
    return dst;
}



/* webrtc */
/*
 *  Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */
void ScaleErrorSignalHELIUM_partial( float * aecxPow, float ef[2][PART_LEN1]) 
{
  const float mu = 1.0f;
  const float error_threshold = 1e-5f;
  const float32x4_t k1e_10f = vdupq_n_f32(1e-10f);
  const float32x4_t kMu = vmovq_n_emu_f32(mu);
  const float32x4_t kThresh = vmovq_n_emu_f32(error_threshold);
  int i;
  // vectorized code (four at once)
  for (i = 0; i + 3 < PART_LEN1; i += 4) {
    const float32x4_t xPow = vld1q_f32(&aecxPow[i]);
    const float32x4_t ef_re_base = vld1q_f32(&ef[0][i]);
    const float32x4_t ef_im_base = vld1q_f32(&ef[1][i]);
    const float32x4_t xPowPlus = vaddq_f32(xPow, k1e_10f);
    float32x4_t ef_re = vdiv_helium_f32(ef_re_base, xPowPlus);
    float32x4_t ef_im = vdiv_helium_f32(ef_im_base, xPowPlus);
    const float32x4_t ef_re2 = vmulq_f32(ef_re, ef_re);
    const float32x4_t ef_sum2 = vfmaq_f32(ef_re2, ef_im, ef_im);
    const float32x4_t absEf = vsqrtq_helium_f32(ef_sum2);
    mve_pred16_t bigger_pred = vcmpgtq_f32(absEf, kThresh); 
    const uint32x4_t bigger_mask = vpselq_u32(vdupq_n_u32(0xffffffff), vdupq_n_u32(0), bigger_pred);
    const float32x4_t absEfPlus = vaddq_f32(absEf, k1e_10f);
    const float32x4_t absEfInv = vdiv_helium_f32(kThresh, absEfPlus);
    uint32x4_t ef_re_if = vreinterpretq_u32_f32(vmulq_f32(ef_re, absEfInv));
    uint32x4_t ef_im_if = vreinterpretq_u32_f32(vmulq_f32(ef_im, absEfInv));
    uint32x4_t ef_re_u32 = vandq_u32(vmvnq_u32(bigger_mask),
                                     vreinterpretq_u32_f32(ef_re));
    uint32x4_t ef_im_u32 = vandq_u32(vmvnq_u32(bigger_mask),
                                     vreinterpretq_u32_f32(ef_im));
    ef_re_if = vandq_u32(bigger_mask, ef_re_if);
    ef_im_if = vandq_u32(bigger_mask, ef_im_if);
    ef_re_u32 = vorrq_u32(ef_re_u32, ef_re_if);
    ef_im_u32 = vorrq_u32(ef_im_u32, ef_im_if);
    ef_re = vmulq_f32(vreinterpretq_f32_u32(ef_re_u32), kMu);
    ef_im = vmulq_f32(vreinterpretq_f32_u32(ef_im_u32), kMu);
    vst1q_f32(&ef[0][i], ef_re);
    vst1q_f32(&ef[1][i], ef_im);
  }
}


#endif

#if __ARM_FEATURE_NEON

static inline float32x4_t vtaylor_polyq_f32(float32x4_t x, const float32_t * coeffs)
{
    float32x4_t     A =
        vmlaq_f32(vld1q_f32(&coeffs[4 * 0]), vld1q_f32(&coeffs[4 * 4]), x);
    float32x4_t     B =
        vmlaq_f32(vld1q_f32(&coeffs[4 * 2]), vld1q_f32(&coeffs[4 * 6]), x);
    float32x4_t     C =
        vmlaq_f32(vld1q_f32(&coeffs[4 * 1]), vld1q_f32(&coeffs[4 * 5]), x);
    float32x4_t     D =
        vmlaq_f32(vld1q_f32(&coeffs[4 * 3]), vld1q_f32(&coeffs[4 * 7]), x);
    float32x4_t     x2 = vmulq_f32(x, x);
    float32x4_t     x4 = vmulq_f32(x2, x2);
    float32x4_t     res = vmlaq_f32(vmlaq_f32(A, B, x2), vmlaq_f32(C, D, x2), x4);
    return res;
}


float32x4_t vlogq_neon_f32(float32x4_t x)
{
    // Extract exponent
    int32x4_t       m =
        vsubq_s32(vreinterpretq_s32_u32(vshrq_n_u32(vreinterpretq_u32_f32(x), 23)),
                  vdupq_n_s32(127));
    float32x4_t     val =
        vreinterpretq_f32_s32(vsubq_s32(vreinterpretq_s32_f32(x), vshlq_n_s32(m, 23)));

    // Polynomial Approximation
    float32x4_t     poly = vtaylor_polyq_f32(val, log_tab);

    // Reconstruct
    poly = vmlaq_f32(poly, vcvtq_f32_s32(m), vdupq_n_f32(LOG2));

    return poly;
}

float32x4_t vexpq_neon_f32(float32x4_t x)
{
    // Perform range reduction [-log(2),log(2)]
    int32x4_t       m = vcvtq_s32_f32(vmulq_f32(x, vdupq_n_f32(INVLOG2)));
    float32x4_t     val = vmlsq_f32(x, vcvtq_f32_s32(m), vdupq_n_f32(LOG2));

    // Polynomial Approximation
    float32x4_t     poly = vtaylor_polyq_f32(val, exp_tab);

    // Reconstruct
    poly =
        vreinterpretq_f32_s32(vqaddq_s32
                              (vreinterpretq_s32_f32(poly), vqshlq_n_s32(m, 23)));
    poly = vbslq_f32(vcltq_s32(m, vdupq_n_s32(-126)), vdupq_n_f32(0.0f), poly);
    return poly;
}


float32x4_t vsinq_neon_f32(float32x4_t val)
{
    const float32x4_t pi_v = vdupq_n_f32(M_PI);
    const float32x4_t pio2_v = vdupq_n_f32(M_PI / 2);
    const float32x4_t ipi_v = vdupq_n_f32(1 / M_PI);

    //Find positive or negative
    const int32x4_t c_v = vabsq_s32(vcvtq_s32_f32(vmulq_f32(val, ipi_v)));
    const uint32x4_t sign_v = vcleq_f32(val, vdupq_n_f32(0));
    const uint32x4_t odd_v = vandq_u32(vreinterpretq_u32_s32(c_v), vdupq_n_u32(1));

    uint32x4_t      neg_v = veorq_u32(odd_v, sign_v);

    //Modulus a - (n * int(a*(1/n)))
    float32x4_t     ma = vsubq_f32(vabsq_f32(val), vmulq_f32(pi_v, vcvtq_f32_s32(c_v)));
    const uint32x4_t reb_v = vcgeq_f32(ma, pio2_v);

    //Rebase a between 0 and pi/2
    ma = vbslq_f32(reb_v, vsubq_f32(pi_v, ma), ma);

    //Taylor series
    const float32x4_t ma2 = vmulq_f32(ma, ma);

    //2nd elem: x^3 / 3!
    float32x4_t     elem = vmulq_f32(vmulq_f32(ma, ma2), vdupq_n_f32(te_sin_coeff2));
    float32x4_t     res = vsubq_f32(ma, elem);

    //3rd elem: x^5 / 5!
    elem = vmulq_f32(vmulq_f32(elem, ma2), vdupq_n_f32(te_sin_coeff3));
    res = vaddq_f32(res, elem);

    //4th elem: x^7 / 7!float32x2_t vsin_f32(float32x2_t val)
    elem = vmulq_f32(vmulq_f32(elem, ma2), vdupq_n_f32(te_sin_coeff4));
    res = vsubq_f32(res, elem);

    //5th elem: x^9 / 9!
    elem = vmulq_f32(vmulq_f32(elem, ma2), vdupq_n_f32(te_sin_coeff5));
    res = vaddq_f32(res, elem);

    //Change of sign
    neg_v = vshlq_n_u32(neg_v, 31);
    res = vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(res), neg_v));
    return res;
}


float32x4_t vinvsqrtq_neon_f32(float32x4_t x)
{
    float32x4_t     sqrt_reciprocal = vrsqrteq_f32(x);
    sqrt_reciprocal =
        vmulq_f32(vrsqrtsq_f32(vmulq_f32(x, sqrt_reciprocal), sqrt_reciprocal),
                  sqrt_reciprocal);
    sqrt_reciprocal =
        vmulq_f32(vrsqrtsq_f32(vmulq_f32(x, sqrt_reciprocal), sqrt_reciprocal),
                  sqrt_reciprocal);

    return sqrt_reciprocal;
}

float32x4_t vinvq_neon_f32(float32x4_t x)
{
    float32x4_t     recip = vrecpeq_f32(x);
    recip = vmulq_f32(vrecpsq_f32(x, recip), recip);
    recip = vmulq_f32(vrecpsq_f32(x, recip), recip);
    return recip;
}



/* webrtc : aec/aec_core_neon.c */
/*
 *  Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */
 
void ScaleErrorSignalNEON_partial( float * aecxPow, float ef[2][PART_LEN1]) 
{
  const float mu = 1.0f;
  const float error_threshold = 1e-5f;
  const float32x4_t k1e_10f = vdupq_n_f32(1e-10f);
  const float32x4_t kMu = vmovq_n_f32(mu);
  const float32x4_t kThresh = vmovq_n_f32(error_threshold);
  int i;
  // vectorized code (four at once)
  for (i = 0; i + 3 < PART_LEN1; i += 4) {
    const float32x4_t xPow = vld1q_f32(&aecxPow[i]);
    const float32x4_t ef_re_base = vld1q_f32(&ef[0][i]);
    const float32x4_t ef_im_base = vld1q_f32(&ef[1][i]);
    const float32x4_t xPowPlus = vaddq_f32(xPow, k1e_10f);
    float32x4_t ef_re = vdivq_f32(ef_re_base, xPowPlus);
    float32x4_t ef_im = vdivq_f32(ef_im_base, xPowPlus);
    const float32x4_t ef_re2 = vmulq_f32(ef_re, ef_re);
    const float32x4_t ef_sum2 = vmlaq_f32(ef_re2, ef_im, ef_im);
    const float32x4_t absEf = vsqrtq_f32(ef_sum2);
    const uint32x4_t bigger = vcgtq_f32(absEf, kThresh);
    const float32x4_t absEfPlus = vaddq_f32(absEf, k1e_10f);
    const float32x4_t absEfInv = vdivq_f32(kThresh, absEfPlus);
    uint32x4_t ef_re_if = vreinterpretq_u32_f32(vmulq_f32(ef_re, absEfInv));
    uint32x4_t ef_im_if = vreinterpretq_u32_f32(vmulq_f32(ef_im, absEfInv));
    uint32x4_t ef_re_u32 = vandq_u32(vmvnq_u32(bigger),
                                     vreinterpretq_u32_f32(ef_re));
    uint32x4_t ef_im_u32 = vandq_u32(vmvnq_u32(bigger),
                                     vreinterpretq_u32_f32(ef_im));
    ef_re_if = vandq_u32(bigger, ef_re_if);
    ef_im_if = vandq_u32(bigger, ef_im_if);
    ef_re_u32 = vorrq_u32(ef_re_u32, ef_re_if);
    ef_im_u32 = vorrq_u32(ef_im_u32, ef_im_if);
    ef_re = vmulq_f32(vreinterpretq_f32_u32(ef_re_u32), kMu);
    ef_im = vmulq_f32(vreinterpretq_f32_u32(ef_im_u32), kMu);
    vst1q_f32(&ef[0][i], ef_re);
    vst1q_f32(&ef[1][i], ef_im);
  }
}

#endif
