/******************************************************************************
 * @file     main.c
 * @brief    opus FFT test application
 * @version  V0.0.1
 * @date     Nov 2021
 * Target Processor: Cortex-M and Cortex-A cores
 ******************************************************************************/
/*
 * Copyright (c) 2010-2021 Arm Limited or its affiliates. All rights reserved.
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
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "opus_lib.h"
#ifdef ARMSIM
#include <common.h>
#else
#define getArg(x)       atoi(argv[x+1])
#define startSection(x) __asm volatile("dbg %0 \n" ::"i" (((x) & 0x7)| 0x8) :"memory")
#define stopSection(x)  __asm volatile("dbg %0 \n" ::"i" (((x) & 0x7)) :"memory")
#endif

#define EXTRA_FFT_TAIL_CHK  8

#define dump_buf_matlab_cplx(a, buf_sz, wrap, format )    \
{                                                         \
    printf("%s=[\n", #a);                                 \
    for (int i = 0; i < buf_sz; i++)                      \
        printf(i % wrap == wrap - 1 ? format"+i*"format   \
        ",...\n":format"+i*"format", ", a[i].r, a[i].i);  \
    printf("];\n");                                       \
}

#define check_for_error_cplx(a, b,buf_sz, thresh )          \
{                                                           \
    for (int i = 0; i < buf_sz; i++) {                      \
        float ref = sqrtf((powf((float)(a[i].r - b[i].r),2)\
            +                                               \
            powf((float)(a[i].i - b[i].i),2)) /             \
            (powf((float)(a[i].r),2) +                      \
                powf((float)(a[i].i),2)));                  \
            if(ref > thresh)                                \
            {                                               \
                printf("[%s] error idx %d %.2f+i* %.2f %.2f+i*%.2f ref %f\n",\
                              #a,                           \
                              i,(float)a[i].r,(float)a[i].i,\
                              (float)b[i].r,(float)b[i].i, ref); \
                errorCnt++;                                 \
                break;                                      \
            }                                               \
        }                                                   \
}

#define MAX_MDCT_SZ             1920
#define MAX_VEC_SZ              MAX_MDCT_SZ
#define MAX_MDCT_BUF_SZ         (MAX_MDCT_SZ + EXTRA_FFT_TAIL_CHK)

static int      _seed = 0xdeadbeef;

void opus_celt_test_randomize_input(int size, kiss_fft_cpx * buf, int scale)
{

    for (int i = 0; i < size; i++) {
        _seed = celt_lcg_rand(_seed);
        buf->i = _seed >> scale;
        _seed = celt_lcg_rand(_seed);
        buf->r = _seed >> scale;
        buf++;
    }
}

/* pick static fft context assocated to 60, 120, 240 or 480 size used in CELT MDCT */
const kiss_fft_state *opus_get_fft_ctx(int fft_size)
{
    /* CELT (non-custom) MDCT sizes context */
    return mode48000_960_120.mdct.kfft[celt_get_fft_shift(fft_size)];
}


int main(int argc, char **argv)
{
    int             fft_size = getArg(0);
    int             dump = getArg(1);
    int             scale = getArg(2);
    int             direction = getArg(3);
    int             errorCnt = 0;

    kiss_fft_cpx    fft_in[MAX_MDCT_BUF_SZ] = { 0 };
    kiss_fft_cpx    fft_out[MAX_MDCT_BUF_SZ] = { 0 };
    kiss_fft_cpx    fft_out_mve[MAX_MDCT_BUF_SZ] = { 0 };

    /* pick static fft context assocated to 60, 120, 240 or 480 size */
    const kiss_fft_state *kfstate = opus_get_fft_ctx(fft_size);

    opus_celt_test_randomize_input(fft_size, fft_in, scale);



    /* allocate Helium FFT context extension */
    int             heliumCtxSz = opus_fft_get_mve_ctx_size(kfstate);
    void           *heliumCtx = malloc(heliumCtxSz);
    if (heliumCtx == NULL) {
        printf("out of memory\n");
        exit(1);
    }
    memset(heliumCtx, 0, heliumCtxSz);

    /* initialize Helium FFT context extension */
    opus_fft_init_mve(kfstate, heliumCtx);


    if (direction == 0) {
        /* run opus FFT c reference */
        startSection(1);
        opus_fft_c(kfstate, fft_in, fft_out);
        stopSection(1);


        /* run opus FFT Helium reference */
        startSection(2);
        opus_fft_mve(kfstate, fft_in, fft_out_mve);
        stopSection(2);
    } else {
        /* run opus FFT c reference */
        startSection(1);
        opus_ifft_c(kfstate, fft_in, fft_out);
        stopSection(1);


        /* run opus FFT Helium reference */
        startSection(2);
        opus_ifft_mve(kfstate, fft_in, fft_out_mve);
        stopSection(2);
    }
    check_for_error_cplx(fft_out_mve, fft_out, (fft_size + EXTRA_FFT_TAIL_CHK), 1e-3f);
    if (errorCnt || dump) {
        dump_buf_matlab_cplx(fft_in, fft_size, 8, "%d");
        dump_buf_matlab_cplx(fft_out, fft_size, 8, "%d");
        dump_buf_matlab_cplx(fft_out_mve, fft_size, 8, "%d");
    }

    if (!errorCnt)
        printf("Status : OK\n");
    else
        printf("Status : KO\n");

    return errorCnt;
}
