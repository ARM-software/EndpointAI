/* ----------------------------------------------------------------------
 * Title:        main.c
 * Description:  Test wrapper for the Helium Programmer's Guide:
 *               Migrating to Helium from Neon
 *               Code samples
 *
 * $Date:        Dec 2020
 *
 * $Revision:    V1.0.0
 *
 * Target Processor: Cortex-M and Cortex-A cores
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

#include <stdio.h>
#include <stdint.h>
#include "arm_math.h"
#include "utils.h"
#include "opus_xcorr.h"
#include "cmplx_dot.h"
#include "vmin.h"
#include "vmath.h"
#include "matrix.h"
#include "vp10_avg.h"
#include "rgb.h"

#ifdef FVP
#include <common.h>
#endif


#define FOREACH_TEST(TEST)                                 \
                /*  0 */ TEST(opus_xcorr_kernel_fx_scal)   \
                /*  1 */ TEST(opus_xcorr_kernel_fx_vec)    \
                /*  2 */ TEST(opus_xcorr_kernel_flt_vec)   \
                /*  3 */ TEST(cmplx_dot_flt_vec)           \
                /*  4 */ TEST(cmplx_dot_fx_vec)            \
                /*  5 */ TEST(min_flt)                     \
                /*  6 */ TEST(log_flt)                     \
                /*  7 */ TEST(exp_flt)                     \
                /*  8 */ TEST(sin_flt)                     \
                /*  9 */ TEST(invsqrt_flt)                 \
                /*  10 */ TEST(inv_flt)                    \
                /*  11 */ TEST(mat_transp_flt)             \
                /*  12 */ TEST(mat_transp_u8)              \
                /*  13 */ TEST(mat_mult4x4_flt)            \
                /*  14 */ TEST(vp10_avg_8x8)               \
                /*  15 */ TEST(rgb2gray)                   \
                /*  16 */ TEST(webrtc_aec_scal_err_sig)    \
                /* last */ TEST(last_test)

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,
enum TEST_ENUM {
    FOREACH_TEST(GENERATE_ENUM)
};

static const char *TEST_STRING[] = {
    FOREACH_TEST(GENERATE_STRING)
};

#define ALIGN16 __attribute__((aligned(16)))

/* Macro for checking absolute error using printf format.
   This is to avoid having to use cast into a float for testing functions
   using fixed points.

   This function is for fixed point comparison of arrays.
*/
#define check_error(a, b, thresh,format )                                     \
{                                                                             \
    if(!alignOffs) {                                                          \
        if (ABS(a - b) > thresh)                                              \
            {                                                                 \
                PRINT("[%d] : %s = "#format" %s = "#format"\n", i,#a,a,#b,b);\
                errorCnt++;                                                   \
                break;                                                        \
            }                                                                 \
    }                                                                         \
}

#define SETUP_SINGLE_VECTORS(TYP)                                                           \
        int       vectorSize = userInput1;                                                  \
        ALIGN16 TYP A[vectorSize], *pA;                                                     \
        ALIGN16 TYP dstScalar[MAX_BUF_SZ + EXTRA_TAIL_CHK] = {0}, *pdstScalar;              \
        ALIGN16 TYP dstVector[MAX_BUF_SZ + EXTRA_TAIL_CHK] = {0}, *pdstVector;              \
        pA = A + alignOffs;                                                                 \
        pdstScalar = dstScalar + alignOffs;                                                 \
        pdstVector = dstVector + alignOffs;


#define RUN_M33_VS_YAMIN_SINGLE_FCT(fct, thresh)                                            \
        startSection(1);                                                                    \
        /* some CMSIS routines do not support unaligned vectors ! */                        \
        if(!alignOffs)fct(pA, pdstScalar, vectorSize);                                      \
        stopSection(1);                                                                     \
                                                                                            \
        startSection(2);                                                                    \
        if (runScalarOnly)fct(pA, pdstVector, vectorSize);                                  \
        else ADD_FCT_SUFFIX(fct,_mve)(pA, pdstVector, vectorSize);                          \
        stopSection(2);                                                                     \
                                                                                            \
        check_for_error(dstScalar, dstVector, (vectorSize + EXTRA_TAIL_CHK), ( __typeof__ (*pA))(thresh));




/*

Comparison of floats taking into account possibilities
of inf, NaN and near zero values (denormalized).

When near zero values, the comparison is absolute and
otherwise it is relative.

*/
int sameFloats(float32_t a, float32_t b, float32_t epsilon)
{
    float           absA = fabsf(a);
    float           absB = fabsf(b);
    float           diff = fabsf(a - b);

    if (a == b) {               // shortcut, handles infinities
        return 1;
    } else if (a == 0 || b == 0 || diff < FLT_MIN) {
        if (diff >= (epsilon * FLT_MIN)) {
            PRINT("ABS ERR %f : ", diff);
        }
        // a or b is zero or both are extremely close to it
        // relative error is less meaningful here
        return diff < (epsilon * FLT_MIN);
    } else {                    // use relative error
        if (diff / MIN((absA + absB), FLT_MAX) >= epsilon) {
            PRINT("REL ERR %f : ", diff / MIN((absA + absB), FLT_MAX));
        }
        return diff / MIN((absA + absB), FLT_MAX) < epsilon;
    }
}

/*

Macro for comparing floating point values.

Comparisons is done element per element for the arrays.

*/
#define check_float_error(a, b, epsilon,format )                            \
{                                                                           \
    if(!alignOffs) {                                                        \
        int hasError=0;                                                     \
                                                                            \
        if ((!isfinite(a)) || (!isfinite(b)))                               \
        {                                                                   \
            PRINT("[%d] : %s = "#format" %s = "#format"\n", i,#a,a,#b,b);   \
            hasError=1;                                                     \
        }                                                                   \
        else                                                                \
        if (!sameFloats((float32_t)a,(float32_t)b,epsilon))                 \
        {                                                                   \
              PRINT("[%d] : %s = "#format" %s = "#format"\n", i,#a,a,#b,b);\
              hasError=1;                                                   \
        }                                                                   \
        if (hasError)                                                       \
        {                                                                   \
            errorCnt++;                                                     \
        }                                                                   \
    }                                                                       \
}


#define RUN_SCALAR_VS_VECTORIAL(fct)                                          \
                startSection(1);                                              \
                if(!alignOffs)                                                \
                {                                                             \
                    fct(src, &dstScalar);                                     \
                }                                                             \
                stopSection(1);                                               \
                                                                              \
                startSection(2);                                              \
                if (runScalarOnly)                                            \
                {                                                             \
                  fct(src,&dstVectorial);                                     \
                }                                                             \
                else                                                          \
                {                                                             \
                  ADD_FCT_SUFFIX(fct,_mve)(src,&dstVectorial);                \
                }                                                             \
                stopSection(2);                                               \
                                                                              \
                if (dstScalar != dstVectorial)                                \
                {                                                             \
                  errorCnt++;                                                 \
                }

#ifdef NODUMP
#define DUMP_ON_ERROR(format)
#else
#define DUMP_ON_ERROR(format)                                                 \
                if (errorCnt || dump)                                         \
                {                                                             \
                    dump_buf(dstScalar, vectorSize, 8, format);               \
                    dump_buf(dstVector, vectorSize, 8, format);               \
                }
#endif


void usage(void)
{
    printf("1st arg expected : testID < %d\n", last_test);
    printf("make run PROG_ARGS=\"0=2\"\n\n");
    for (int i = 0; i < last_test; i++)
        printf("\t%d :%s\n", i, TEST_STRING[i]);
}

int main(int argc, char **argv)
{
#ifdef FVP
    int             userInput0 = getArg(0);
    int             userInput1 = getArg(1);
    int             userInput2 = getArg(2);
    int             userInput3 = getArg(3);
#else
    int             userInput0;
    int             userInput1;
    int             userInput2;
    int             userInput3;

    printf("argc %d\n", argc);
    if (argc < 3)
        usage();

    userInput0 = atoi(argv[1]);
    userInput1 = atoi(argv[2]);
    if (argc > 3)
        userInput2 = atoi(argv[3]);
    else
        userInput2 = 0;
    if (argc > 4)
        userInput3 = atoi(argv[4]);
    else
        userInput3 = 0;
#endif

    int             errorCnt = 0;
    int             dump = userInput2;


#ifdef SCALAR_ONLY
    runScalarOnly = 1;
#endif


    if (userInput0 < last_test)
        PRINT("Test : %s : arg %d %d %d %d\n", TEST_STRING[userInput0],
              userInput0, userInput1, userInput2, userInput3);
    else {
        printf("Invalid test ID %d !\n", userInput0);
        usage();
        exit(0);
    }


    switch (userInput0) {
      case opus_xcorr_kernel_fx_scal:
          {
              int32_t         len = userInput1;
              int16_t         a[userInput1 + 4];
              int16_t         b[userInput1 + 4];
              int32_t         sum[4] = { 1000, 2000, 3000, 4000 };

              for (int i = 0; i < len + 4; ++i) {
                  a[i] = i;
                  b[i] = 100 + i;
              }

              xcorr_kernel_c(a, b, sum, len);

              if (dump)
                  dump_buf(sum, 4, 8, "%d");
          }
          break;

      case opus_xcorr_kernel_fx_vec:
          {
              int32_t         len = userInput1;
              int16_t         a[userInput1 + 4];
              int16_t         b[userInput1 + 4];
              int32_t         sum[4] = { 1000, 2000, 3000, 4000 };

              for (int i = 0; i < len + 4; ++i) {
                  a[i] = i;
                  b[i] = 100 + i;
              }

#if __ARM_FEATURE_NEON
              xcorr_kernel_neon_fixed(a, b, sum, len);
#endif

#if __ARM_FEATURE_MVE
              xcorr_kernel_mve_fixed(a, b, sum, len);
#endif

              if (dump)
                  dump_buf(sum, 4, 8, "%d");
          }
          break;



      case opus_xcorr_kernel_flt_vec:
          {
              float           len = userInput1;
              float           a[userInput1 + 4];
              float           b[userInput1 + 4];
              float           sum[4] = { 1000, 2000, 3000, 4000 };

              for (int i = 0; i < len + 4; ++i) {
                  a[i] = (float) i;
                  b[i] = 100.0f + (float) i;
              }

#if __ARM_FEATURE_NEON
              xcorr_kernel_neon_float(a, b, sum, len);
#endif

#if __ARM_FEATURE_MVE
              xcorr_kernel_mve_float(a, b, sum, len);
#endif

              if (dump)
                  dump_buf(sum, 4, 8, "%f");
          }
          break;

      case cmplx_dot_flt_vec:
          {
              int             len = userInput1;
              float           a[CMPLX_PAIR * userInput1];
              float           b[CMPLX_PAIR * userInput1];
              float           sum[CMPLX_PAIR] = { 0 };

              for (int i = 0; i < CMPLX_PAIR * len; ++i) {
                  a[i] = 100.0f + (float) i;
                  b[i] = 100.0f - (float) i;
              }

#if __ARM_FEATURE_NEON
              arm_cmplx_dot_prod_neon_f32(a, b, len, &sum[0], &sum[1]);
#endif

#if __ARM_FEATURE_MVE
              arm_cmplx_dot_prod_helium_f32(a, b, len, &sum[0], &sum[1]);
#endif

              if (dump)
                  dump_buf(sum, 2, 8, "%f");
          }
          break;

      case cmplx_dot_fx_vec:
          {
              int             len = userInput1;
              q15_t           a[CMPLX_PAIR * userInput1];
              q15_t           b[CMPLX_PAIR * userInput1];
              q31_t           sum[CMPLX_PAIR] = { 0 };

              for (int i = 0; i < CMPLX_PAIR * len; ++i) {
                  a[i] = 10000 + i * 100;
                  b[i] = -20000 - i * 100;
              }

#if __ARM_FEATURE_NEON
              arm_cmplx_dot_prod_neon_q15(a, b, len, &sum[0], &sum[1]);
#endif

#if __ARM_FEATURE_MVE
              arm_cmplx_dot_prod_helium_q15(a, b, len, &sum[0], &sum[1]);
#endif


              if (dump)
                  dump_buf(sum, 2, 8, "%d");
          }
          break;


      case min_flt:
          {
              int             len = userInput1;
              float           a[userInput1];
              uint32_t        index;
              float32_t       result;

              for (int i = 0; i < len; ++i) {
                  a[i] = i * 100.0f;
              }
              a[len / 2] = -10.0f;

#if __ARM_FEATURE_NEON
              arm_min_neon_f32(a, len, &result, &index);
#endif

#if __ARM_FEATURE_MVE
              arm_min_helium_f32(a, len, &result, &index);
#endif


              if (dump)
                  printf("min %f @ index %d\n", result, index);
          }
          break;

      case log_flt:
          {
              float32x4_t     a;
              float32x4_t     vlog;

              for (int i = 0; i < 4; ++i) {
                  a[i] = (float) (userInput1 * (i + 1));
              }


#if __ARM_FEATURE_NEON
              vlog = vlogq_neon_f32(a);
#endif

#if __ARM_FEATURE_MVE
              vlog = vlogq_helium_f32(a);
#endif

              if (dump)
                  printf("vlog [%f %f %f %f ] => [%f %f %f %f]\n", a[0], a[1], a[2], a[3],
                         vlog[0], vlog[1], vlog[2], vlog[3]);
          }
          break;

      case exp_flt:
          {
              float32x4_t     a;
              float32x4_t     vexp;

              for (int i = 0; i < 4; ++i) {
                  a[i] = (float) (userInput1 * (i + 1));
              }


#if __ARM_FEATURE_NEON
              vexp = vexpq_neon_f32(a);
#endif

#if __ARM_FEATURE_MVE
              vexp = vexpq_helium_f32(a);
#endif

              if (dump)
                  printf("vexp [%f %f %f %f ] => [%f %f %f %f]\n", a[0], a[1], a[2], a[3],
                         vexp[0], vexp[1], vexp[2], vexp[3]);
          }
          break;

      case sin_flt:
          {
              float32x4_t     a;
              float32x4_t     vsin;

              for (int i = 0; i < 4; ++i) {
                  a[i] = (float) (userInput1 * (i + 1));
              }


#if __ARM_FEATURE_NEON
              vsin = vsinq_neon_f32(a);
#endif

#if __ARM_FEATURE_MVE
              vsin = vsinq_helium_f32(a);
#endif

              if (dump)
                  printf("vsin [%f %f %f %f ] => [%f %f %f %f]\n", a[0], a[1], a[2], a[3],
                         vsin[0], vsin[1], vsin[2], vsin[3]);
          }
          break;


      case invsqrt_flt:
          {
              float32x4_t     a;
              float32x4_t     vinvsqrt;

              for (int i = 0; i < 4; ++i) {
                  a[i] = (float) (userInput1 * (i + 1));
                  a[i] = 1.0f / (a[i] * a[i]);
              }


#if __ARM_FEATURE_NEON
              vinvsqrt = vinvsqrtq_neon_f32(a);
#endif

#if __ARM_FEATURE_MVE
              vinvsqrt = vinvsqrtq_helium_f32(a);
#endif

              if (dump)
                  printf("vinvsqrt [%f %f %f %f ] => [%f %f %f %f]\n", a[0], a[1], a[2],
                         a[3], vinvsqrt[0], vinvsqrt[1], vinvsqrt[2], vinvsqrt[3]);
          }
          break;
      case inv_flt:
          {
              float32x4_t     a;
              float32x4_t     vinv;

              for (int i = 0; i < 4; ++i) {
                  a[i] = (float) (userInput1 * (i + 1));
                  a[i] = 1.0f / a[i];
              }


#if __ARM_FEATURE_NEON
              vinv = vinvq_neon_f32(a);
#endif

#if __ARM_FEATURE_MVE
              vinv = vinvq_helium_f32(a);
#endif

              if (dump)
                  printf("vinv [%f %f %f %f ] => [%f %f %f %f]\n", a[0], a[1], a[2], a[3],
                         vinv[0], vinv[1], vinv[2], vinv[3]);
          }
          break;


      case mat_transp_flt:
          {
              float32_t       a[4 * 4];

              for (int i = 0; i < 4 * 4; ++i) {
                  a[i] = i;
              }


#if __ARM_FEATURE_NEON
              mat_transpose_inp_4x4_neon_f32(a);
#endif

#if __ARM_FEATURE_MVE
              mat_transpose_inp_4x4_helium_f32(a);
#endif

              if (dump)
                  dump_buf(a, 16, 4, "%f");
          }
          break;

      case mat_transp_u8:
          {
              uint8_t         a[4 * 4];

              for (int i = 0; i < 4 * 4; ++i) {
                  a[i] = i;
              }


#if __ARM_FEATURE_NEON
              mat_transpose_inp_4x4_neon_u8(a);
#endif

#if __ARM_FEATURE_MVE
              mat_transpose_inp_4x4_helium_u8(a);
#endif

              if (dump)
                  dump_buf(a, 16, 4, "%d");
          }
          break;

      case mat_mult4x4_flt:
          {
              float32_t       a[4 * 4];
              float32_t       b[4 * 4];
              float32_t       c[4 * 4];

              for (int i = 0; i < 4 * 4; ++i) {
                  a[i] = i;
                  b[i] = i;
              }


#if __ARM_FEATURE_NEON
              mat_multiply_4x4_neon(a, b, c);
#endif

#if __ARM_FEATURE_MVE
              mat_multiply_4x4_helium(a, b, c);
#endif

              if (dump)
                  dump_buf(c, 16, 4, "%f");
          }
          break;

      case vp10_avg_8x8:
          {
              uint8_t         s[16 * 16];
              int32_t         tot;

              for (int i = 0; i < 16 * 16; ++i) {
                  s[i] = i;
              }


#if __ARM_FEATURE_NEON
              tot = vp10_avg_8x8_neon(s, 1);
#endif

#if __ARM_FEATURE_MVE
              tot = vp10_avg_8x8_helium(s, 1);
#endif

              if (dump)
                  printf("total %d\n", tot);
          }
          break;

      case rgb2gray:
          {
              uint8_t         rgb[16 * 16 * 3];
              uint8_t         gray[16 * 16];

              for (int i = 0; i < 16 * 16; ++i) {
                  rgb[3 * i] = i;
                  rgb[3 * i + 1] = i / 2;
                  rgb[3 * i + 2] = i / 3;
              }


#if __ARM_FEATURE_NEON
              rgb_to_gray_neon(rgb, gray, userInput1);
#endif

#if __ARM_FEATURE_MVE
              rgb_to_gray_helium(rgb, gray, userInput1);
#endif

              if (dump)
                  dump_buf(gray, userInput1, 8, "%d");
          }
          break;

      case webrtc_aec_scal_err_sig:
          {

              float           aecxPow[PART_LEN * 2];
              float           ef[2][PART_LEN1];

              for (int i = 0; i < PART_LEN1; ++i) {
                  ef[0][i] = i * 1.5f;
                  ef[1][i] = i * -2.5f;
                  aecxPow[i] = i * 0.123f;
                  aecxPow[i] = aecxPow[i] * aecxPow[i];
              }


#if __ARM_FEATURE_NEON
              ScaleErrorSignalNEON_partial(aecxPow, ef);
#endif

#if __ARM_FEATURE_MVE
              ScaleErrorSignalHELIUM_partial(aecxPow, ef);
#endif

              if (dump)
                  dump_buf(aecxPow, PART_LEN, 8, "%f");
          }
          break;



      default:
          PRINT("To Be implemented\n");
          errorCnt++;
          break;
    }


    PRINT_TB_STATUS(errorCnt);

    return errorCnt;
}
