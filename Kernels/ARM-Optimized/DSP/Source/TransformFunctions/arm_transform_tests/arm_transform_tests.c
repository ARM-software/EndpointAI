/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_transform_tests.c
 * Description:  test optimized transform functions against CMSIS DSP reference
 *
 * $Date:        06. Mar 2023
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

#include "_utils.h"
#include "arm_math.h"
#include "arm_math_f16.h"
#include "../transform_functions_mve.h"
#include "perf_counter.h"

#define MAX_FFT_LENGTH          1024

typedef struct {
    int64_t         ref, opt;
} prof;

typedef int     (*testfct) (int size, int dump, int ifft, prof * p);

typedef struct {
    testfct         f;
    char           *name;
} test;


/* for perf_counter */
void SysTick_Handler(void)
{
#if __IS_COMPILER_GCC__ || __IS_COMPILER_IAR__
    user_code_insert_to_systick_handler();
#endif
}




int test_cfft_q31(int size, int dump, int ifft, prof * p)
{
    int             errorCnt = 0;
    SETUP_RADMIXED_FFT(q31, size, ifft, 0);

    SETUP_SINGLE_VECTORS_CMPLX(q31_t, size, 2);
    set_input(SINE, vectorSize * CMPLX_DIM, pA, powf(2.0, 8.0), q31_t);

    if (dump)
        dump_buf_matlab(pA, vectorSize * CMPLX_DIM, 16, "%d");

    memcpy(pdstScal, pA, vectorSize * CMPLX_DIM * sizeof(q31_t));
    memcpy(pdstVec, pA, vectorSize * CMPLX_DIM * sizeof(q31_t));

    RUN_Scal_VS_Vec_FFT_WITH_FLAGS(arm_cfft_q31, 16, ifftFlag, bitReverseFlag, "%d");

    DUMP_ON_ERROR_CMPLX("%d");

    return errorCnt;
}

int test_cfft_q15(int size, int dump, int ifft, prof * p)
{
    int             errorCnt = 0;
    SETUP_RADMIXED_FFT(q15, size, ifft, 0);

    SETUP_SINGLE_VECTORS_CMPLX(q15_t, size, 4);
    set_input(SINE, vectorSize * CMPLX_DIM, pA, 1, q15_t);

    if (dump)
        dump_buf_matlab(pA, vectorSize * CMPLX_DIM, 16, "%d");

    memcpy(pdstScal, pA, vectorSize * CMPLX_DIM * sizeof(q15_t));
    memcpy(pdstVec, pA, vectorSize * CMPLX_DIM * sizeof(q15_t));

    RUN_Scal_VS_Vec_FFT_WITH_FLAGS(arm_cfft_q15, 16, ifftFlag, bitReverseFlag, "%d");

    DUMP_ON_ERROR_CMPLX("%d");

    return errorCnt;
}




int test_cfft_f16(int size, int dump, int ifft, prof * p)
{
    int             errorCnt = 0;
    SETUP_RADMIXED_FFT(f16, size, ifft, 0);

    SETUP_SINGLE_VECTORS_CMPLX(float16_t, size, 4);
    set_input(SINE, vectorSize * CMPLX_DIM, pA, powf(2.0, -8.0), float16_t);

    if (dump)
        dump_buf_matlab(pA, vectorSize * CMPLX_DIM, 16, "%f");

    memcpy(pdstScal, pA, vectorSize * CMPLX_DIM * sizeof(float16_t));
    memcpy(pdstVec, pA, vectorSize * CMPLX_DIM * sizeof(float16_t));

    RUN_Scal_VS_Vec_FFT_WITH_FLAGS(arm_cfft_f16, 16, ifftFlag, bitReverseFlag, "%f");

    DUMP_ON_ERROR_CMPLX("%f");

    return errorCnt;
}

int test_cfft_f32(int size, int dump, int ifft, prof * p)
{
    int             errorCnt = 0;
    SETUP_RADMIXED_FFT(f32, size, ifft, 0);

    SETUP_SINGLE_VECTORS_CMPLX(float32_t, size, 2);
    set_input(SINE, vectorSize * CMPLX_DIM, pA, powf(2.0, -8.0), float32_t);

    if (dump)
        dump_buf_matlab(pA, vectorSize * CMPLX_DIM, 16, "%f");

    memcpy(pdstScal, pA, vectorSize * CMPLX_DIM * sizeof(float32_t));
    memcpy(pdstVec, pA, vectorSize * CMPLX_DIM * sizeof(float32_t));

    RUN_Scal_VS_Vec_FFT_WITH_FLAGS(arm_cfft_f32, 16, ifftFlag, bitReverseFlag, "%f");

    DUMP_ON_ERROR_CMPLX("%f");

    return errorCnt;
}

int test_rfft_q15(int size, int dump, int ifft, prof * p)
{
    int             errorCnt = 0;

    SETUP_FIXED_RFFT(q15, size, ifft, 0);
    SETUP_SINGLE_VECTORS_CMPLX(q15_t, size, 2);

    set_input(RAND, vectorSize, pA, powf(2.0, -20.0), q15_t);

    memcpy(pdstScal, pA, vectorSize * sizeof(q15_t));
    memcpy(pdstVec, pA, vectorSize * sizeof(q15_t));

    int64_t         ref, opt;
    ref = get_system_ticks();
    arm_rfft_q15(&rfftInstanceScal, pA, pdstScal);
    ref = get_system_ticks() - ref;

    // pA corrupted, restore
    memcpy(pA, pdstVec, vectorSize * sizeof(q15_t));
    opt = get_system_ticks();
    ADD_FCT_SUFFIX(arm_rfft_q15, _mve) (&rfftInstanceVec, pA, pdstVec);
    opt = get_system_ticks() - opt;

    int             max_diff_allowed = 40;
    if (vectorSize > 1024)
        max_diff_allowed = max_diff_allowed * 2;
    vectorSize = vectorSize / 2 + 1;
    check_error(pdstScal, pdstVec, vectorSize, max_diff_allowed, "%d");

    DUMP_ON_ERROR_CMPLX("%d");
    p->ref = ref;
    p->opt = opt;

    return errorCnt;
}


int test_rfft_q31(int size, int dump, int ifft, prof * p)
{
    int             errorCnt = 0;

    SETUP_FIXED_RFFT(q31, size, ifft, 0);
    SETUP_SINGLE_VECTORS_CMPLX(q31_t, size, 2);

    set_input(RAND, vectorSize, pA, powf(2.0, -10.0), q31_t);

    memcpy(pdstScal, pA, vectorSize * sizeof(q31_t));
    memcpy(pdstVec, pA, vectorSize * sizeof(q31_t));

    int64_t         ref, opt;
    ref = get_system_ticks();
    arm_rfft_q31(&rfftInstanceScal, pA, pdstScal);
    ref = get_system_ticks() - ref;

    // pA corrupted, restore
    memcpy(pA, pdstVec, vectorSize * sizeof(q31_t));
    opt = get_system_ticks();
    ADD_FCT_SUFFIX(arm_rfft_q31, _mve) (&rfftInstanceVec, pA, pdstVec);
    opt = get_system_ticks() - opt;

    int             max_diff_allowed = 40;
    if (vectorSize > 1024)
        max_diff_allowed = max_diff_allowed * 2;
    vectorSize = vectorSize / 2 + 1;
    check_error(pdstScal, pdstVec, vectorSize, max_diff_allowed, "%d");

    DUMP_ON_ERROR_CMPLX("%d");
    p->ref = ref;
    p->opt = opt;

    return errorCnt;
}


int test_rfft_f16(int size, int dump, int ifft, prof * p)
{
    int             errorCnt = 0;

    SETUP_FLOAT_RFFT(f16, size, ifft, 0);
    SETUP_SINGLE_VECTORS_CMPLX(float16_t, size, 2);

    set_input(RAND, vectorSize, pA, (1.0f16 / (float16_t) vectorSize) * powf(2.0, -20.0),
              float16_t);

    memcpy(pdstScal, pA, vectorSize * sizeof(float16_t));
    memcpy(pdstVec, pA, vectorSize * sizeof(float16_t));

    int64_t         ref, opt;
    ref = get_system_ticks();
    arm_rfft_fast_f16(&rfftInstanceScal, pA, pdstScal, ifftFlag);
    ref = get_system_ticks() - ref;

    // pA corrupted, restore
    memcpy(pA, pdstVec, vectorSize * sizeof(float16_t));
    opt = get_system_ticks();
    ADD_FCT_SUFFIX(arm_rfft_fast_f16, _mve) (&rfftInstanceVec, pA, pdstVec, ifftFlag);
    opt = get_system_ticks() - opt;


    float32_t       releps = 0.05;
    if (size >= 2048)
        releps = 0.5f;

    check_relative_abs_error(pdstScal, pdstVec, fftLen, 0.08, releps, "%.5f");

    DUMP_ON_ERROR_CMPLX("%.4f");
    p->ref = ref;
    p->opt = opt;

    return errorCnt;
}


int test_rfft_f32(int size, int dump, int ifft, prof * p)
{
    int             errorCnt = 0;

    SETUP_FLOAT_RFFT(f32, size, ifft, 0);
    SETUP_SINGLE_VECTORS_CMPLX(float32_t, size, 2);

    set_input(RAND, vectorSize, pA, (1.0f / (float32_t) vectorSize) * powf(2.0, -20.0), float32_t);

    memcpy(pdstScal, pA, vectorSize * sizeof(float32_t));
    memcpy(pdstVec, pA, vectorSize * sizeof(float32_t));

    int64_t         ref, opt;
    ref = get_system_ticks();
    arm_rfft_fast_f32(&rfftInstanceScal, pA, pdstScal, ifftFlag);
    ref = get_system_ticks() - ref;

    // pA corrupted, restore
    memcpy(pA, pdstVec, vectorSize * sizeof(float32_t));
    opt = get_system_ticks();
    ADD_FCT_SUFFIX(arm_rfft_fast_f32, _mve) (&rfftInstanceVec, pA, pdstVec, ifftFlag);
    opt = get_system_ticks() - opt;


    float32_t       releps = 0.05;
    if (size >= 2048)
        releps = 0.5f;

    check_relative_abs_error(pdstScal, pdstVec, fftLen, 0.08, releps, "%.5f");

    DUMP_ON_ERROR_CMPLX("%.4f");
    p->ref = ref;
    p->opt = opt;

    return errorCnt;
}

int main(void)
{
    prof            p;
    test            testlist[] = {
        {test_cfft_q15, "cfftq15"},
        {test_cfft_q31, "cfftq31"},
        {test_cfft_f16, "cfftf16"},
        {test_cfft_f32, "cfftf32"},

        {test_rfft_q15, "rfftq15"},
        {test_rfft_q31, "rfftq31"},
        {test_rfft_f16, "rfftf16"},
        {test_rfft_f32, "rfftf32"},
        {NULL, NULL}
    };
    test           *curtest = testlist;

    init_cycle_counter(false);


    while (curtest->f != NULL) {
        testfct         curfct = curtest->f;
        for (int sz = 64; sz <= MAX_FFT_LENGTH; sz <<= 1) {
            int             errorCnt = curfct(sz, 0, 0, &p);

            printf("%s, %d, 0, 0, %d, %d, %s\n", curtest->name, sz, (uint32_t)p.ref, (uint32_t)p.opt,
                   errorCnt == 0 ? "OK" : "KO");
        }
        curtest++;
    }
}
