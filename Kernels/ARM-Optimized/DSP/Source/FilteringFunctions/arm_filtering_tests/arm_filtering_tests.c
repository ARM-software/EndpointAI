/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_filter_tests.c
 * Description:  Test optimized filtering functions against CMSIS DSP reference
 *
 * $Date:        06. Apr 2023
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

    RUN_FIR_VS_REF(arm_fir_f16, 0.5f);

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
        {test_fir_f32, "firf32", 256, 256, 32, 32},
        {test_fir_f16, "firf16", 256, 256, 32, 32},
        {test_fir_q31, "firq31", 256, 256, 32, 32},
        {test_fir_q15, "firq15", 256, 256, 32, 32},
        {NULL, NULL}
    };
    test           *curtest = testlist;


    init_cycle_counter(false);

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
