/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_filter_tests.c
 * Description:  Test optimized filtering functions against CMSIS DSP reference
 *
 * $Date:        20. Apr 2023
 * $Revision:    V1.0.1
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

#include "_utils.h"
#include "arm_math.h"
#include "arm_math_f16.h"
#include "../filtering_functions_mve.h"
#include "perf_counter.h"



typedef struct {
    int64_t         ref, opt;
} prof;

typedef int     (*testfct) (int blockSize, int filter_sz, int dump, prof * p);

typedef struct {
    testfct         f;
    char           *name;
    int             minBlkSize;
    int             maxBlkSize;
    int             minFltSize;
    int             maxFltSize;

} test;


/* for perf_counter */
void SysTick_Handler(void)
{
#if __IS_COMPILER_GCC__ || __IS_COMPILER_IAR__
    user_code_insert_to_systick_handler();
#endif
}




int test_biq_df1_f32(int blockSize, int nbCascade, int dump, prof * p)
{
    int             errorCnt = 0;
#if defined(ARM_MATH_MVEF)
    int             alignOffs = 0;

    SETUP_BIQUAD_DF1(float32_t, f32, nbCascade, blockSize, alignOffs);

    pcoefs = coefs;
    GEN_BIQUAD_LPF_FLT(float32_t, nbCascade, pcoefs, 0);


    ALIGN16 arm_biquad_mod_coef_f32 coefsModRef[nbCascade];
    ALIGN16 arm_biquad_mod_coef_f32 coefsModOpt[nbCascade];

    arm_biquad_cascade_df1_mve_init_f32(&SRef, nbCascade, coefs, coefsModRef, pstateRef);
    arm_biquad_cascade_df1_mve_init_f32(&SOpt, nbCascade, coefs, coefsModOpt, pstateOpt);

    set_input(RAND, blockSize, pInput, powf(2.0f, -30.0f), float32_t);

    RUN_BIQUAD_DF1(arm_biquad_cascade_df1_f32, 1e-6f);

    DUMP_ON_BIQUAD_DF1_ERROR("%f");

    hasNonNormalElts(pdstRef, blockSize);
    hasNonNormalElts(pdstOpt, blockSize);
#else
    printf("skipped\n");
#endif
    return errorCnt;
}




int test_biq_df1_f16(int blockSize, int nbCascade, int dump, prof * p)
{
    int             errorCnt = 0;
#if defined(ARM_MATH_MVEF)
    int             alignOffs = 0;

    SETUP_BIQUAD_DF1(float16_t, f16, nbCascade, blockSize, alignOffs);

    pcoefs = coefs;
    GEN_BIQUAD_LPF_FLT(float16_t, nbCascade, pcoefs, 0);


    ALIGN16 arm_biquad_mod_coef_f16 coefsModRef[nbCascade];
    ALIGN16 arm_biquad_mod_coef_f16 coefsModOpt[nbCascade];

    arm_biquad_cascade_df1_mve_init_f16(&SRef, nbCascade, coefs, coefsModRef, pstateRef);
    arm_biquad_cascade_df1_mve_init_f16(&SOpt, nbCascade, coefs, coefsModOpt, pstateOpt);

    set_input(RAND, blockSize, pInput, powf(2.0f, -30.0f), float16_t);

    RUN_BIQUAD_DF1(arm_biquad_cascade_df1_f16, 1e-2f);

    DUMP_ON_BIQUAD_DF1_ERROR("%f");

    hasNonNormalElts(pdstRef, blockSize);
    hasNonNormalElts(pdstOpt, blockSize);
#else
    printf("skipped\n");
#endif
    return errorCnt;
}



int test_biq_df1_q31(int blockSize, int nbCascade, int dump, prof * p)
{
    int             errorCnt = 0;
#if defined(ARM_MATH_MVEF)
    int             alignOffs = 0;

    SETUP_BIQUAD_DF1(q31_t, q31, nbCascade, blockSize, alignOffs);

    arm_biquad_cascade_df1_init_q31(&SRef, nbCascade, pcoefs, pstateRef POSTSHIFT_q31);
    arm_biquad_cascade_df1_init_q31(&SOpt, nbCascade, pcoefs, pstateOpt POSTSHIFT_q31);

    pcoefs = coefs;
    GEN_BIQUAD_LPF_FX(q31_t, nbCascade, pcoefs, 20);

    set_input(RAND, blockSize, pInput, powf(2.0, -10.0), q31_t);

    RUN_BIQUAD_DF1(arm_biquad_cascade_df1_q31, 1);

    DUMP_ON_BIQUAD_DF1_ERROR("%d");

#else
    printf("skipped\n");
#endif
    return errorCnt;
}


int test_biq_df1_q15(int blockSize, int nbCascade, int dump, prof * p)
{
    int             errorCnt = 0;
#if defined(ARM_MATH_MVEF)
    int             alignOffs = 0;

    SETUP_BIQUAD_DF1(q15_t, q15, nbCascade, blockSize, alignOffs);

    arm_biquad_cascade_df1_init_q15(&SRef, nbCascade, pcoefs, pstateRef POSTSHIFT_q15);
    arm_biquad_cascade_df1_init_q15(&SOpt, nbCascade, pcoefs, pstateOpt POSTSHIFT_q15);

    pcoefs = coefs;
    GEN_BIQUAD_LPF_FX(q15_t, nbCascade, pcoefs, 13);

    set_input(RAND, blockSize, pInput, powf(2.0, -10.0), q15_t);

    RUN_BIQUAD_DF1(arm_biquad_cascade_df1_q15, 1);

    DUMP_ON_BIQUAD_DF1_ERROR("%d");

#else
    printf("skipped\n");
#endif
    return errorCnt;
}

int test_fir_f32(int blockSize, int nbTaps, int dump, prof * p)
{
    int             errorCnt = 0;
    int             alignOffs = 0;


    SETUP_FIR(float32_t, f32, blockSize, nbTaps);

    set_input(RAND, nbTaps, pCoeffs, powf(2.0, -24.0), float32_t);
    set_input(RAND, blockSize, pInput, powf(2.0, -24.0), float32_t);

    // clear coef end to deal with non-multiple of vector elements
    CLEAR_COEF_TAIL(float32_t, nbTaps);

    RUN_FIR_VS_REF(arm_fir_f32, 0.5f);

    DUMP_ON_FIR_ERROR("%.2f", nbTaps);

    return errorCnt;
}

int test_fir_f16(int blockSize, int nbTaps, int dump, prof * p)
{
    int             errorCnt = 0;
    int             alignOffs = 0;


    SETUP_FIR(float16_t, f16, blockSize, nbTaps);

    set_input(RAND, nbTaps, pCoeffs, powf(2.0, -28.0), float16_t);
    set_input(RAND, blockSize, pInput, powf(2.0, -28.0), float16_t);

    // clear coef end to deal with non-multiple of vector elements
    CLEAR_COEF_TAIL(float16_t, nbTaps);

    RUN_FIR_VS_REF(arm_fir_f16, 1.0f);

    DUMP_ON_FIR_ERROR("%.2f", nbTaps);

    return errorCnt;
}


int test_fir_q31(int blockSize, int nbTaps, int dump, prof * p)
{
    int             errorCnt = 0;
    int             alignOffs = 0;


    SETUP_FIR(q31_t, q31, blockSize, nbTaps);

    set_input(RAND, nbTaps, pCoeffs, powf(2.0, -8.0), q31_t);
    set_input(RAND, blockSize, pInput, powf(2.0, -8.0), q31_t);

    // clear coef end to deal with non-multiple of vector elements
    CLEAR_COEF_TAIL(q31_t, nbTaps);

    RUN_FIR_VS_REF(arm_fir_q31, 10);

    DUMP_ON_FIR_ERROR("%d", nbTaps);

    return errorCnt;
}

int test_fir_q15(int blockSize, int nbTaps, int dump, prof * p)
{
    int             errorCnt = 0;
    int             alignOffs = 0;


    SETUP_FIR(q15_t, q15, blockSize, nbTaps);

    set_input(RAND, nbTaps, pCoeffs, powf(2.0, -20.0), q15_t);
    set_input(RAND, blockSize, pInput, powf(2.0, -20.0), q15_t);

    // clear coef end to deal with non-multiple of vector elements
    CLEAR_COEF_TAIL(q15_t, nbTaps);

    RUN_FIR_VS_REF(arm_fir_q15, 10);

    DUMP_ON_FIR_ERROR("%d", nbTaps);

    return errorCnt;
}

int main(void)
{
    prof            p;
    test            testlist[] = {
        /* FIR filtering */
        {test_fir_f32, "firf32", 64, 256, 8, 32},
        {test_fir_f16, "firf16", 64, 256, 8, 32},
        {test_fir_q31, "firq31", 64, 256, 8, 32},
        {test_fir_q15, "firq15", 64, 256, 8, 32},
        /* DF1 Biquad filtering */
        {test_biq_df1_f32, "biqdf1_f32", 64, 256, 1, 4},
        {test_biq_df1_f16, "biqdf1_f16", 64, 256, 1, 4},
        {test_biq_df1_q31, "biqdf1_q31", 64, 256, 1, 4},
        {test_biq_df1_q15, "biqdf1_q15", 64, 256, 1, 4},

        {NULL, NULL}
    };
    test           *curtest = testlist;

    init_cycle_counter(false);


    printf("test, blksz, filter sz., cmsis-dsp, opt., status\n");
    while (curtest->f != NULL) {
    testfct         curfct = curtest->f;
    for (int blksz = curtest->minBlkSize; blksz <= curtest->maxBlkSize; blksz <<= 1) {
        for (int filtsz = curtest->minFltSize; filtsz <= curtest->maxFltSize; filtsz <<= 1) {
        int             errorCnt = curfct(blksz, filtsz, 0, &p);

        printf("%s, %d, %d, %d, %d, %s\n", curtest->name, blksz, filtsz, (uint32_t) p.ref,
               (uint32_t) p.opt, errorCnt == 0 ? "OK" : "KO");
        }
    }
    curtest++;
    }
}
