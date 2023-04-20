/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        _utils.h
 * Description:  Test optimized filtering functions against CMSIS DSP reference
 *
 * $Date:        20. Apr 2023
 * $Revision:    V1.0.0
 *
 * Target Processor: Cortex-M cores
 * -------------------------------------------------------------------- */
/*
 * Copyright (C) 2023 ARM Limited or its affiliates. All rights reserved.
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

#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>

#define CCAT(A, B)              A ## B
#define CAT(A, B)               CCAT(A, B)
#define ADD_FCT_SUFFIX(x,y)     CAT(x,y)

#define PRINT(fmt, args...)

#define MAX_BUF_SZ              (1024*1)

#define NO_DIFF_ALLOWED         0
#define MAX_ABSDIFF_ALLOWED     20
#define EXTRA_TAIL_ALIGN        8

#define ALIGN16 __attribute__((aligned(16)))
#define ALIGN8  __attribute__((aligned(8)))
#define ALIGN4  __attribute__((aligned(4)))

// extra length to ensure vector operation did not corrupt the tail
#define EXTRA_TAIL_CHK          8

typedef enum {
    RAMP,
    RAND,
    SINE,
    CONST,
    FILE0,
    FILE1,
    RAND2
} inputTyp;

static int      seed = 0xdeadbeef;

static inline void set_pseudo_rand_seed(int32_t the_seed)
{
    seed = the_seed;
}

static inline int32_t pseudo_rand(int32_t seed)
{
    return 1764525 * seed + 2013904223;
}

#define maxi(a,b) \
  ({ __typeof__ (a) _a = (a); \
      __typeof__ (b) _b = (b); \
    _a > _b ? _a : _b; })

#define ABS(a) \
  ({ __typeof__ (a) _a = (a); \
    _a <0 ? -_a : _a; })


#define SIN250_48_PERIOD 96*2

static short    sin250_48[SIN250_48_PERIOD] = {
    0, 536, 1072, 1606, 2139, 2669, 3196, 3720,
    4240, 4756, 5266, 5771, 6270, 6762, 7246, 7723,
    8192, 8652, 9102, 9543, 9974, 10394, 10803, 11200,
    11585, 11958, 12318, 12665, 12998, 13318, 13623, 13913,
    14189, 14449, 14694, 14924, 15137, 15334, 15515, 15679,
    15826, 15956, 16069, 16165, 16244, 16305, 16349, 16375,
    16384, 16375, 16349, 16305, 16244, 16165, 16069, 15956,
    15826, 15679, 15515, 15334, 15137, 14924, 14694, 14449,
    14189, 13913, 13623, 13318, 12998, 12665, 12318, 11958,
    11585, 11200, 10803, 10394, 9974, 9543, 9102, 8652,
    8192, 7723, 7246, 6762, 6270, 5771, 5266, 4756,
    4240, 3720, 3196, 2669, 2139, 1606, 1072, 536,
    0, -536, -1072, -1606, -2139, -2669, -3196, -3720,
    -4240, -4756, -5266, -5771, -6270, -6762, -7246, -7723,
    -8192, -8652, -9102, -9543, -9974, -10394, -10803, -11200,
    -11585, -11958, -12318, -12665, -12998, -13318, -13623, -13913,
    -14189, -14449, -14694, -14924, -15137, -15334, -15515, -15679,
    -15826, -15956, -16069, -16165, -16244, -16305, -16349, -16375,
    -16384, -16375, -16349, -16305, -16244, -16165, -16069, -15956,
    -15826, -15679, -15515, -15334, -15137, -14924, -14694, -14449,
    -14189, -13913, -13623, -13318, -12998, -12665, -12318, -11958,
    -11585, -11200, -10803, -10394, -9974, -9543, -9102, -8652,
    -8192, -7723, -7246, -6762, -6270, -5771, -5266, -4756,
    -4240, -3720, -3196, -2669, -2139, -1606, -1072, -536,
};



#define TO_FLT(X,Y) ((float)X*(float)Y);

#define set_input(test, size, in_out, scale, format)                              \
{                                                                                 \
    switch (test)                                                                 \
    {                                                                             \
    case RAMP:                                                                    \
        {                                                                         \
            for (int i = 0; i < size; i++)                                        \
                in_out[i] = (format)TO_FLT(i,scale);                              \
            break;                                                                \
        }                                                                         \
    case CONST:                                                                   \
        {                                                                         \
            for (int i = 0; i < size; i++)                                        \
                in_out[i] = (format)TO_FLT(1,scale)                               \
            break;                                                                \
        }                                                                         \
    case RAND:                                                                    \
        {                                                                         \
            for (int i = 0; i < size; i++)                                        \
            {                                                                     \
                seed = pseudo_rand(seed);                                         \
                in_out[i] = (format)TO_FLT(seed,scale)                            \
            }                                                                     \
        }                                                                         \
        break;                                                                    \
    case RAND2:                                                                   \
        {                                                                         \
            for (int i = 0; i < size; i++)                                        \
            {                                                                     \
                float64_t x = (float64_t)rand()/(float64_t)(RAND_MAX);            \
                in_out[i] = x*scale;                                              \
            }                                                                     \
        }                                                                         \
        break;                                                                    \
    case SINE:                                                                    \
        {                                                                         \
            for (int i = 0; i < size; i++)  {                                     \
                in_out[i] =(format) TO_FLT(sin250_48[i % SIN250_48_PERIOD],scale) \
            }                                                                     \
            break;                                                                \
        }                                                                         \
                                                                                  \
    default:                                                                      \
        break;                                                                    \
    }                                                                             \
}



#define check_for_error(a, b,buf_sz, thresh )                                     \
{                                                                                 \
    for (int i = 0; i < buf_sz; i++)                                              \
        if (ABS(a[i] - b[i]) > thresh)                                            \
        {                                                                         \
            printf("[%s] error idx %d %.2f %.2f\n",                               \
            #a, i,(float)a[i],(float)b[i]);                                       \
            errorCnt++;                                                           \
            break;                                                                \
        }                                                                         \
}

#define dump_buf_matlab(a, buf_sz, wrap, format )                                 \
{                                                                                 \
    printf("%s=[\n", #a);                                                         \
    for (int i = 0; i < buf_sz; i++)                                              \
        printf(i % wrap == wrap - 1 ? format",...\n":format", ", a[i]);           \
    printf("];\n");                                                               \
}


#define check_error(a, b,buf_sz, thresh,format )                                    \
{                                                                                   \
    __typeof__(*a) maxdiff = 0;                                                     \
        for (int i = 0; i < buf_sz; i++) {                                          \
            if (ABS(a[i] - b[i]) > maxdiff)                                         \
                maxdiff = ABS(a[i] - b[i]);                                         \
            if (ABS(a[i] - b[i]) > thresh)                                          \
            {                                                                       \
                PRINT("[%d] : %s = "#format" %s = "#format"\n", i,#a,a[i],#b,b[i]); \
                errorCnt++;                                                         \
                break;                                                              \
            }                                                                       \
        }                                                                           \
       PRINT("maxdiff %d\n",(int)maxdiff);                                          \
}

/* Scaling for intermediate accumulating buffer */
/* Q31 FIR is using 64-bit accumulation, requiring 2x amount of blocksze for storage */

#define SZ_SCL_f32 1
#define SZ_SCL_f16 1
#define SZ_SCL_q31 2
#define SZ_SCL_q15 1

#define SETUP_FIR(TYP, EXT, blockSize, numTaps)                                        \
    arm_fir_instance_##EXT SOPT, SREF;                                                 \
    ALIGN16 TYP     Coeffs[numTaps + 16], *pCoeffs;                                    \
    ALIGN16 TYP     Input[MAX_BUF_SZ], *pInput;                                        \
    ALIGN16 TYP     firStateOPT[CAT(SZ_SCL_,EXT)*MAX_BUF_SZ +                          \
                                               MAX_BUF_SZ + EXTRA_TAIL_CHK] = {0},     \
        *pfirStOPT;                                                                    \
    ALIGN16 TYP     firStateREF[CAT(SZ_SCL_,EXT)*MAX_BUF_SZ +                          \
                                               MAX_BUF_SZ + EXTRA_TAIL_CHK] = {0},     \
        *pfirStREF;                                                                    \
    ALIGN16 TYP     dstOPT[MAX_BUF_SZ + EXTRA_TAIL_CHK] = {0}, *pdstOPT;               \
    ALIGN16 TYP     dstREF[MAX_BUF_SZ + EXTRA_TAIL_CHK] = {0}, *pdstREF;               \
                                                                                       \
    if (!numTaps || !blockSize) {                                                      \
        printf("invalid taps / blocksz\n"); exit(1);}                                  \
    PRINT("Test: blockSize %d Taps %d off %d\n", blockSize, numTaps, alignOffs);  \
    seed += blockSize + numTaps;                                                       \
    pCoeffs     = Coeffs;                                                              \
    pfirStOPT   = firStateOPT + alignOffs;                                             \
    pfirStREF = firStateREF + alignOffs ;                                              \
    pInput      = Input;                                                               \
    pdstOPT     = dstOPT + alignOffs;                                                  \
    pdstREF   = dstREF+ alignOffs;                                                     \
    arm_fir_init_##EXT(&SOPT, numTaps, pCoeffs, pfirStOPT, blockSize);                 \
    arm_fir_init_##EXT(&SREF, numTaps, pCoeffs, pfirStREF, blockSize);


#define CLEAR_COEF_TAIL(TYP, numTaps)                                                  \
    TYP * ptail =&pCoeffs[numTaps];                                                    \
    for(int i = 0;i < 16 - (numTaps & 0xf);i++)                                        \
        *ptail++ = 0;



#define RUN_FIR_VS_REF(fct, thresh)                                                    \
    int64_t         ref, opt;                                                          \
    ref = get_system_ticks();                                                          \
    fct(&SREF, pInput, pdstREF, blockSize);                                            \
    ref = get_system_ticks() - ref;                                                    \
                                                                                       \
    opt = get_system_ticks();                                                          \
    ADD_FCT_SUFFIX(fct,_mve)(&SOPT, pInput, pdstOPT, blockSize);                       \
    opt = get_system_ticks() - opt;                                                    \
                                                                                       \
    check_for_error(pdstOPT, pdstREF, (blockSize + EXTRA_TAIL_CHK),                    \
        ( __typeof__ (Input[0]))(thresh));                                             \
    p->ref = ref;                                                                      \
    p->opt = opt;


#define DUMP_ON_FIR_ERROR(format, numTaps)                                             \
    if (errorCnt || dump)                                                              \
    {                                                                                  \
        dump_buf_matlab(dstOPT, blockSize, 8, format);                                 \
        dump_buf_matlab(dstREF, blockSize, 8, format);                                 \
        dump_buf_matlab(firStateOPT, (numTaps - 1), 8, format);                        \
        dump_buf_matlab(firStateREF, (numTaps - 1), 8, format);                        \
    }



#define min(a,b) (a < b ? a : b)

#define check_relative_abs_error(a,b,buf_sz,abseps,releps,format)                               \
    {                                                                                           \
        int hasError=0;                                                                         \
        float maxeps = 0.0f;                                                                    \
        for (int i = 0; i < buf_sz; i+=1)                                                       \
        {                                                                                       \
            if ((!isfinite(a[i])) || (!isfinite(b[i])))                                         \
            {                                                                                   \
                PRINT("[%d] : %s = "#format" %s = "#format"\n", i,#a,a[i],#b,b[i]);             \
                hasError=1;                                                                     \
            }                                                                                   \
            else                                                                                \
            {                                                                                   \
                float32_t delta = ABS((float32_t)(a[i]-b[i]));                                  \
                float32_t moda = ABS(a[i]);                                                     \
                float32_t modb = ABS(b[i]);                                                     \
                if(delta > maxeps)                                                              \
                    maxeps = delta;                                                             \
                if ((moda!=0) && (modb!=0) && (delta > abseps))                                 \
                {                                                                               \
                    float32_t relerr=delta / min((moda + modb), FLT_MAX);                       \
                    if (relerr >= releps)                                                       \
                    {                                                                           \
                       PRINT("REL [%d] : %s = "#format" %s = "#format" ERR = %.5f\n",           \
                                                                 i,#a,a[i],#b,b[i],relerr);     \
                       hasError=1;                                                              \
                    }                                                                           \
                }                                                                               \
                else if (delta > abseps)                                                        \
                {                                                                               \
                    PRINT("ABS [%d] : %s = "#format" %s = "#format" ERR = %f\n",                \
                                                                i,#a,a[i],#b,b[i],delta);       \
                    hasError=1;                                                                 \
                }                                                                               \
            }                                                                                   \
        }                                                                                       \
        if (hasError)                                                                           \
        {                                                                                       \
            errorCnt++;                                                                         \
        }                                                                                       \
        PRINT("max eps : %f\n", maxeps);                                                        \
    }


#define DUMP_ON_ERROR_CMPLX(format)                                                             \
        if (errorCnt || dump)                                                                   \
        {                                                                                       \
            dump_buf_matlab(pdstScal, CMPLX_DIM*vectorSize + EXTRA_TAIL_CHK, 16, format);       \
            dump_buf_matlab(pdstVec, CMPLX_DIM*vectorSize + EXTRA_TAIL_CHK, 16, format);        \
        }

#define POSTSHIFT_q31 ,10
#define POSTSHIFT_q15 ,1

#define POSTSHIFT_f32
#define POSTSHIFT_f16

#define MAX_BIQ_STAGES      16

#define SETUP_BIQUAD_DF1(TYP, EXT,nbCascade, blockSize,alignOffs)                               \
    ALIGN16 TYP     stateRef[4 * MAX_BIQ_STAGES] = { 0 }, *pstateRef;                           \
    ALIGN16 TYP     stateOpt[4 * MAX_BIQ_STAGES] = { 0 }, *pstateOpt;                           \
    ALIGN16 TYP     coefs[5 * nbCascade], *pcoefs = coefs;                                      \
    arm_biquad_casd_df1_inst_##EXT SRef, SOpt;                                                  \
    ALIGN16 TYP Input[MAX_BUF_SZ], *pInput = Input;                                             \
    ALIGN16 TYP dstRef[MAX_BUF_SZ + EXTRA_TAIL_CHK] = { 0 },                                    \
     *pdstRef = dstRef + alignOffs;                                                             \
    ALIGN16 TYP dstOpt[MAX_BUF_SZ + EXTRA_TAIL_CHK] = { 0 },                                    \
    *pdstOpt = dstOpt + alignOffs;                                                              \
    pstateRef = stateRef + alignOffs;                                                           \
    pstateOpt = stateOpt + alignOffs;




#define GEN_BIQUAD_LPF_FLT(TYP, nbCascade, pcoefs, scale)                                       \
    for (int i = 0; i < nbCascade; i++) {                                                       \
        float32_t w, f0, alpha;                                                                 \
        float32_t a0, a1, a2, b0, b1, b2, Q;                                                    \
                                                                                                \
        /* low pass filter, fci = 200*i/8000 */                                                 \
        Q = 2.0f;                                                                               \
        f0 = 200.f * (float32_t)(i + 1);                                                        \
        w = 2 * PI * f0 / 8000.0f;                                                              \
        alpha = sinf(w) / 2 * Q;                                                                \
                                                                                                \
        a0 = 1.0f + alpha;                                                                      \
        b0 = ((1 - cosf(w)) / 2.f) / a0;                                                        \
        b1 = (2.0f * b0);                                                                       \
        b2 = b0;                                                                                \
                                                                                                \
        a0 = 1.0f + alpha;                                                                      \
        a1 = -2.0f * cosf(w) / a0;                                                              \
        a2 = (1.0f - alpha) / a0;                                                               \
                                                                                                \
        PRINT("coef a1=%f a2=%f b0=%f b1=%f b2=%f \n", a1, a2, b0, b1, b0);                     \
                                                                                                \
        *pcoefs++ = (TYP)b0;                                                                    \
        *pcoefs++ = (TYP)b1;                                                                    \
        *pcoefs++ = (TYP)b2;                                                                    \
        *pcoefs++ = -(TYP)a1;                                                                   \
        *pcoefs++ = -(TYP)a2;                                                                   \
    }



#define INSERT_0_q31_t
#define INSERT_0_q15_t    *pcoefs++ = 0;


/* Biquad Low pass filter Gen for fixed point  */
#define GEN_BIQUAD_LPF_FX(TYP, nbCascade, pcoefs,scale)                                             \
    for (int i = 0; i < nbCascade; i++) {                                                           \
        float32_t w, f0, alpha;                                                                     \
        float32_t a0, a1, a2, b0, b1, b2, Q;                                                        \
        TYP a1fx, a2fx, b0fx, b1fx, b2fx;                                                           \
                                                                                                    \
        /* low pass filter, fci = 200*i/8000 */                                                     \
        Q = 2.0f;                                                                                   \
        f0 = 400.0f + 100.0f * (float32_t)(i + 1);                                                  \
        w = 2 * PI * f0 / 8000.0f;                                                                  \
        alpha = sinf(w) / 2.0f * Q;                                                                 \
                                                                                                    \
        a0 = 1.0f + alpha;                                                                          \
        b0 = ((1 - cosf(w)) / 2.0f) / a0;                                                           \
        b1 = (2.0f * b0);                                                                           \
        b2 = b0;                                                                                    \
                                                                                                    \
        a0 = (1.0f + alpha);                                                                        \
        a1 = -2.0f * cosf(w) / a0;                                                                  \
        a2 = (1.0f - alpha) / a0;                                                                   \
                                                                                                    \
        a1fx = (TYP)(roundf(a1 * powf(2.0, scale)));                                                \
        a2fx = (TYP)(roundf(a2 * powf(2.0, scale)));                                                \
        b0fx = (TYP)(roundf(b0 * powf(2.0, scale)));                                                \
        b1fx = (TYP)(roundf(b1 * powf(2.0, scale)));                                                \
        b2fx = (TYP)(roundf(b2 * powf(2.0, scale)));                                                \
                                                                                                    \
        PRINT("coef a1=%f a2=%f b0=%f b1=%f b2=%f : %d %d %d %d %d\n",                              \
        a1, a2, b0, b1, b0, a1fx, a2fx, b0fx, b1fx, b2fx);                                          \
                                                                                                    \
        *pcoefs++ = b0fx;                                                                           \
        /* special case for Q15 */                                                                  \
        /* Coefficients are stored in the following order:  */                                      \
        /* {b10, 0, b11, b12, a11, a12, b20, 0, b21,...}    */                                      \
        INSERT_0_##TYP                                                                              \
        *pcoefs++ = b1fx;                                                                           \
        *pcoefs++ = b2fx;                                                                           \
        *pcoefs++ = a1fx;                                                                           \
        *pcoefs++ = a2fx;                                                                           \
    }



#define hasNonNormalElts(a, buf_sz )                                                            \
{                                                                                               \
    int cntNonNormal = 0;                                                                       \
    for (int i = 0; i < buf_sz; i++) {                                                          \
        float32_t __a = (float32_t)a[i];                                                        \
        /*neither infinity, NaN, zero or subnormal */                                           \
        if(!isnormal(__a))   {                                                                  \
            printf("idx %d %f \n",i, a[i]);                                                     \
            cntNonNormal++; }                                                                   \
    }                                                                                           \
    if(cntNonNormal) printf("!!! %s unexpected elts cnt = %d\n",                                \
                #a, cntNonNormal);                                                              \
}




#define RUN_BIQUAD_DF1(fct, thresh)                                                             \
    int64_t         ref, opt;                                                                   \
    ref = get_system_ticks();                                                                   \
    fct(&SRef, pInput, pdstRef, blockSize);                                                     \
    ref = get_system_ticks() - ref;                                                             \
                                                                                                \
    opt = get_system_ticks();                                                                   \
    ADD_FCT_SUFFIX(fct,_mve)(&SOpt, pInput, pdstOpt, blockSize);                                \
    opt = get_system_ticks() - opt;                                                             \
                                                                                                \
    p->ref = ref;                                                                               \
    p->opt = opt;                                                                               \
                                                                                                \
    check_for_error(pdstRef, pdstOpt, (blockSize + EXTRA_TAIL_CHK),                             \
    ( __typeof__ (Input[0]))(thresh));                                                          \
    check_for_error(stateRef, stateOpt, (4 * nbCascade + EXTRA_TAIL_CHK),                       \
    ( __typeof__ (Input[0]))(thresh));


#define DUMP_ON_BIQUAD_DF1_ERROR(format)                                                        \
    if (errorCnt || dump)                                                                       \
    {                                                                                           \
        dump_buf_matlab(pInput, blockSize, 8, format);                                          \
        dump_buf_matlab(pdstRef, blockSize, 8, format);                                         \
        dump_buf_matlab(pdstOpt, blockSize, 8, format);                                         \
        dump_buf_matlab(stateRef, (4 * nbCascade), 8, format);                                  \
        dump_buf_matlab(stateOpt, (4 * nbCascade), 8, format);                                  \
    }




#endif
