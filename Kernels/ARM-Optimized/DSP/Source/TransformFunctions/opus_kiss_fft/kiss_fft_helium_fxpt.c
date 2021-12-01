/*Copyright (c) 2003-2004, Mark Borgerding
  Lots of modifications by Jean-Marc Valin
  Copyright (c) 2005-2007, Xiph.Org Foundation
  Copyright (c) 2008,      Xiph.Org Foundation, CSIRO

  All rights reserved.

  Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
       this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.*/

/* This code is originally from Mark Borgerding's KISS-FFT but has been
   heavily modified to better suit Opus */


/* ARM CM55 with helium updates under apache-2.0 license */
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


#include "_kiss_fft_guts.h"

#if !__ARM_FEATURE_MVE
#error "MVE support not enabled"
#else
#include <arm_mve.h>

#define MAX_TWIDDLE_ARR_LEN     (480)
#define MAX_RADX_STAGES         6

#define __ALIGN8(addr)          (((uintptr_t)addr + (8 - 1)) & -8)

/**
 * @brief helium specific context to be attached to private kissfft context part.
 *
 * contains :
 *  - vector friendly rearranged twiddles
 *  - 64-bit expanded bitreveral offsets for scatter store
 */
typedef struct
{
    kiss_fft_cpx *rearranged_twiddle_stride1;  /**< reordered twiddle offset 1 storage */
    kiss_fft_cpx *rearranged_twiddle_stride2;  /**< reordered twiddle offset 2 storage */
    kiss_fft_cpx *rearranged_twiddle_stride3;  /**< reordered twiddle offset 3 storage */
    kiss_fft_cpx *rearranged_twiddle_stride4;  /**< reordered twiddle offset 4 storage */

    kiss_fft_cpx *rearranged_twiddle_tab_stride1_arr[MAX_RADX_STAGES]; /**< Per stage reordered twiddle pointer (offset 1) */
    kiss_fft_cpx *rearranged_twiddle_tab_stride2_arr[MAX_RADX_STAGES]; /**< Per stage reordered twiddle pointer (offset 2) */
    kiss_fft_cpx *rearranged_twiddle_tab_stride3_arr[MAX_RADX_STAGES]; /**< Per stage reordered twiddle pointer (offset 3) */
    kiss_fft_cpx *rearranged_twiddle_tab_stride4_arr[MAX_RADX_STAGES]; /**< Per stage reordered twiddle pointer (offset 4) */

    kiss_fft_cpx *p_cur_tw1;
    kiss_fft_cpx *p_cur_tw2;
    kiss_fft_cpx *p_cur_tw3;
    kiss_fft_cpx *p_cur_tw4;

    uint64_t * bitrev64;    /**< 64-bit bitreversed index array. */
    opus_val32 bitrev_scal; /**< FWD FFT scaling = inverse FFT length. */
} kf_helium;


/* see kiss_fft.c for C reference */
void kf_bfly2_mve(
                     kiss_fft_cpx * Fout,
                     int m,
                     int N,
                     int stage
                    )
{
   (void)m;

#define INVSQRT2_Q31  1518500250
    opus_val32 tw = INVSQRT2_Q31;
    /* We know that m==4 here because the radix-2 is just after a radix-4 */
    celt_assert(m==4);
    static const int32_t strides8cplx[4] = { (0)*4, (1)*4, (16)*4, (17)*4 };

__asm volatile (
        // setup vector of pointers for gather load
        "scatgathvec    .req q6                                    \n"
        "   vldrw.s32       q1, [%[strides8cplx]]                  \n"
        "   vadd.s32        scatgathvec, q1, %[Fout]               \n"

        ".p2align 2                                                \n"

        "   wlstp.32        lr, %[N], 1f                           \n"
        /* low overhead loop start */
        "   2:                                                     \n"
        "   vldrw.u32       q0, [scatgathvec, #0]                  \n"
        "   vldrw.u32       q1, [scatgathvec, #32]                 \n"
        "   vadd.s32        q2, q0, q1                             \n"
        "   vstrw.s32       q2, [scatgathvec, #0]                  \n"
        "   vsub.s32        q0, q0, q1                             \n"
        "   vstrw.s32       q0, [scatgathvec, #32]                 \n"
        "   vldrw.u32       q1, [scatgathvec, #40]                 \n"
        "   vcadd.s32       q2, q1, q1, #270                       \n"
        "   vqdmulh.s32     q1, q2, %[tw]                          \n"
        "   vldrw.u32       q0, [scatgathvec, #8]                  \n"
        "   vadd.s32        q2, q0, q1                             \n"
        "   vstrw.s32       q2, [scatgathvec, #8]                  \n"
        "   vsub.s32        q0, q0, q1                             \n"
        "   vstrw.s32       q0, [scatgathvec, #40]                 \n"
        "   vldrw.u32       q0, [scatgathvec, #16]                 \n"
        "   vldrw.u32       q1, [scatgathvec, #48]                 \n"
        "   vcadd.s32       q2, q0, q1, #270                       \n"
        "   vstrw.s32       q2, [scatgathvec, #16]                 \n"
        "   vcadd.s32       q0, q0, q1, #90                        \n"
        "   vstrw.s32       q0, [scatgathvec, #48]                 \n"
        "   vldrw.u32       q1, [scatgathvec, #56]                 \n"
        "   vcadd.s32       q2, q1, q1, #270                       \n"
        "   vqdmulh.s32     q1, q2, %[tw]                          \n"
        "   vldrw.u32       q0, [scatgathvec, #24]                 \n"
        "   vcadd.s32       q2, q0, q1, #270                       \n"
        "   vstrw.s32       q2, [scatgathvec, #24]                 \n"
        "   vcadd.s32       q0, q0, q1, #90                        \n"
        "   vstrw.s32       q0, [scatgathvec, #56]                 \n"
        "   vadd.s32        scatgathvec, scatgathvec, %[cst128]    \n"
        /* low overhead loop end */
        "   letp            lr, 2b                                 \n"
        "   1:                                                     \n"

        : [Fout] "+r"(Fout)
        : [strides8cplx] "r" (strides8cplx),
          [tw] "r" (tw), [N] "r" (N*2), [cst128] "r" (16*8)
        :"lr", "q0", "q1", "q2", "q6" );

}

void kf_bfly3_mve(kiss_fft_cpx * Fout, const size_t fstride,
                const kiss_fft_state * st, int m, int N, int mm, int stage)
{
    int       i;
    const size_t m2 = 2 * m;
    const kiss_fft_cpx *tw1, *tw2;
    kf_helium      *st_helium = (kf_helium *) st->arch_fft->priv;
#ifdef FIXED_POINT
#define SIN_2PIBY3_Q31      -1859775393 // round(sin(2*pi/3)*2**31)
    kiss_fft_scalar sin2piBy3 = SIN_2PIBY3_Q31;
#else
    epi3 = st->twiddles[fstride * m];
#endif
    const int ptr_post_upd = mm - m;
    kiss_fft_cpx *ptrA = Fout;
    kiss_fft_cpx *ptrB = Fout + m;
    kiss_fft_cpx *ptrC = Fout + m2;


    tw1 = st_helium->rearranged_twiddle_tab_stride1_arr[stage];
    tw2 = st_helium->rearranged_twiddle_tab_stride2_arr[stage];


#pragma nounroll

    for (i = 0; i < N; i++)
    {
        __asm volatile (
            // load scheduling

            "   vldrw.u32       q3, [%[tw1]], 16               \n"
            "   vldrw.u32       q6, [%[ptrB]]                  \n"

            ".p2align 2                                        \n"
            /* low overhead loop start */
            "  wls              lr, %[count], 1f               \n"
            "2:                                                \n"
            "   vqdmlsdh.s32    q4, q6, q3                     \n"
            "   vldrw.u32       q2, [%[ptrC]]                  \n"
            "   vqdmladhx.s32   q4, q6, q3                     \n"
            "   vldrw.u32       q3, [%[tw2]], 16               \n"
            "   vqdmlsdh.s32    q5, q2, q3                     \n"
            "   vqdmladhx.s32   q5, q2, q3                     \n"
            "   vldrw.u32       q3, [%[tw1]], 16               \n"

            // ** sub-stage1 **:
            "   vhadd.s32       q6, q4, q5                     \n"
            "   vsub.s32        q7, q4, q5                     \n"
            "   vldrw.u32       q0, [%[ptrA]]                  \n"
            // ** sub-stage2 **:
            "   vsub.s32        q1, q0, q6                     \n"
            "   vmla.s32        q0, q6, %[two]                 \n"
            "   vldrw.u32       q6, [%[ptrB], #0x10]           \n"
            "   vqdmulh.s32     q7, q7, %[sin2piBy3]           \n"
            // ** sub-stage3 **:
            "   vstrw.u32       q0, [%[ptrA]], #16             \n"
            "   vcadd.s32       q2, q1, q7, #90                \n"
            "   vstrw.u32       q2, [%[ptrB]], #16             \n"
            "   vcadd.s32       q2, q1, q7, #270               \n"
            "   vstrw.u32       q2, [%[ptrC]], #16             \n"
            /* low overhead loop end */
            "   le              lr, 2b                         \n"
            "1:                                                \n"

            :[ptrA] "+r"(ptrA),[ptrB] "+r"(ptrB),[ptrC] "+r"(ptrC),
             [tw1] "+r"(tw1),[tw2] "+r"(tw2)
            :[count] "r"(m >> 1),[two] "r"(2),[sin2piBy3] "r"(sin2piBy3)
            :"memory", "r14","lr",
             "q0", "q1", "q2", "q3",
             "q4", "q5", "q6", "q7" );

        // rewind twiddles ptrs

        tw1 = tw1 - m - 2 ; /* -2 because of initial preload */
        tw2 = tw2 - m;

        ptrA += ptr_post_upd;
        ptrB += ptr_post_upd;
        ptrC += ptr_post_upd;
    }
}

void kf_bfly4_mve(kiss_fft_cpx * Fout, const size_t fstride,
            const kiss_fft_state * st, int m, int N, int mm, int stage)
{
    int       i;

    if (m == 1)
    {
        int32_t  *pSrc = (int32_t *) Fout;

        static const int32_t strides8cplx[4] = {
            (0 - 16) * 4, (1 - 16) * 4, (8 - 16) * 4, (9 - 16) * 4
        };
        // Complex input organized as :
        // A0, B0, C0, D0, A1, B1, C1, D1, A2, B2 etc..
        // generates:
        //    AAx = (Ax + Cx) +  (Bx + Dx)
        //    BBx = (Ax - Cx) -i*(Bx - Dx)
        //    CCx = (Ax + Cx) -  (Bx + Dx)
        //    DDx = (Ax - Cx) +i*(Bx - Dx)
        //    ...


        __asm volatile (
            "scatgathvec    .req q6                                     \n"
            // setup gather load pointers
            "   vldrw.32        scatgathvec, [%[strides8cplx]]          \n"
            "   vadd.s32        scatgathvec, scatgathvec, %[pOut]       \n"

            /* preload */
            "   vldrw.u32       q7, [scatgathvec, #64]!                 \n"
            "   vldrw.u32       q5, [scatgathvec, #16]                  \n"


            ".p2align 2                                                 \n"
            "  wlstp.s32        lr, %[count], 1f                        \n"
            /* low overhead loop start */
            "2:                                                         \n"
            "   vadd.s32        q0, q7, q5                              \n"
            "   vldrw.u32       q4, [scatgathvec, #8]                   \n"
            "   vsub.s32        q1, q7, q5                              \n"
            "   vldrw.u32       q5, [scatgathvec, #24]                  \n"
            "   vadd.s32        q2, q4, q5                              \n"
            "   vldrw.u32       q7, [scatgathvec, #64]!                 \n"
            "   vsub.s32        q3, q4, q5                              \n"
            "   vldrw.u32       q5, [scatgathvec, #16]                  \n"
            "   vadd.s32        q4, q0, q2                              \n"
            "   vstrw.32        q4, [scatgathvec, #-64]                 \n"
            "   vsub.s32        q4, q0, q2                              \n"
            "   vstrw.32        q4, [scatgathvec, #-48]                 \n"
            "   vcadd.s32       q4, q1, q3, #270                        \n"
            "   vstrw.32        q4, [scatgathvec, #-56]                 \n"
            "   vcadd.s32       q4, q1, q3, #90                         \n"
            "   vstrw.32        q4, [scatgathvec, #-40]                 \n"
            /* low overhead loop end */
            "   letp            lr, 2b                                  \n"
            "1:                                                         \n"
           :[pOut] "+r"(pSrc)
           :[count] "r"(N*2),
            [strides8cplx] "r"(strides8cplx)
           :"memory", "r14",
           "q0", "q1", "q2", "q3", "q4", "q5", "q6", "q7");

    }
    else
    {
        const kiss_fft_cpx *tw1, *tw2, *tw3;
        /* m is guaranteed to be a multiple of 4. */
        const int m2 = 2 * m;
        const int m3 = 3 * m;
        // pre-calculate offset for fast ptrs update between each stage
        // next stage input pointers are given by:
        //  ptrA =  Fout_beg + i*mm;
        //  ptrB =  Fout_beg + i*mm + m;
        //  ptrC =  Fout_beg + i*mm + m2;
        //  ptrD =  Fout_beg + i*mm + m3;
        //
        //  Input ptrs are updated in the inner loop by m with
        //  the exception of ptr A & C which are 2 steps forward
        //  because of loop scheduling optimization
        const int ptrBandD_post_upd = mm - m;
        const int ptrAandC_post_upd = mm - m - 2;
        kf_helium      *st_helium = (kf_helium *) st->arch_fft->priv;


        tw1 = st_helium->rearranged_twiddle_tab_stride1_arr[stage];
        tw2 = st_helium->rearranged_twiddle_tab_stride2_arr[stage];
        tw3 = st_helium->rearranged_twiddle_tab_stride3_arr[stage];


        kiss_fft_cpx *ptrA = Fout;
        kiss_fft_cpx *ptrB = Fout + m;
        kiss_fft_cpx *ptrC = Fout + m2;
        kiss_fft_cpx *ptrD = Fout + m3;

#pragma nounroll

        for (i = 0; i < N; i++)
        {
            // Complex input organized as :
            // A0, B0, C0, D0, A1, B1, C1, D1, A2, B2 etc..
            // generates:
            //      AAx = (Ax + Cx') +  (Bx' + Dx')
            //      BBx = (Ax - Cx') -i*(Bx' - Dx')
            //      CCx = (Ax + Cx') -  (Bx' + Dx')
            //      DDx = (Ax - Cx') +i*(Bx' - Dx')
            //    ...
            // where
            //      Bx' = Bx * TwBx,
            //      Cx' = Cx * TwCx
            //      Dx' = Dx * TwDx


            __asm volatile (
                /* preload */
                "   vldrw.u32       q7, [%[ptrA]], #16        \n"
                "   vldrw.u32       q6, [%[ptrC]], #16        \n"
                ".p2align 2                                   \n"
                "  wls              lr, %[count], 1f          \n"
                /* low overhead loop start */
                "2:                                           \n"
                "   vldrw.u32       q1, [%[tw2]], 16          \n"
                "   vqdmlsdh.s32    q5, q6, q1                \n"
                "   vqdmladhx.s32   q5, q6, q1                \n"
                "   vldrw.u32       q1, [%[tw1]], 16          \n"
                "   vadd.s32        q0, q7, q5                \n"
                "   vldrw.u32       q6, [%[ptrB]], #16        \n"
                "   vqdmlsdh.s32    q4, q6, q1                \n"
                "   vldrw.u32       q2, [%[tw3]], #16         \n"
                "   vqdmladhx.s32   q4, q6, q1                \n"
                "   vsub.s32        q1, q7, q5                \n"
                "   vldrw.u32       q6, [%[ptrD]], #16        \n"
                "   vqdmlsdh.s32    q5, q6, q2                \n"
                "   vqdmladhx.s32   q5, q6, q2                \n"
                "   vadd.s32        q2, q4, q5                \n"
                "   vldrw.u32       q7, [%[ptrA]], #16        \n"
                "   vsub.s32        q3, q4, q5                \n"
                "   vldrw.u32       q6, [%[ptrC]], #16        \n"
                "   vadd.s32        q4, q0, q2                \n"
                "   vstrw.32        q4, [%[ptrA], #-32]       \n"
                "   vsub.s32        q4, q0, q2                \n"
                "   vstrw.32        q4, [%[ptrC], #-32]       \n"
                "   vcadd.s32       q4, q1, q3, #270          \n"
                "   vstrw.32        q4, [%[ptrB], #-16]       \n"
                "   vcadd.s32       q4, q1, q3, #90           \n"
                "   vstrw.32        q4, [%[ptrD], #-16]       \n"
                /* low overhead loop end */
                "   le              lr, 2b                    \n"
                "1:                                           \n"

                :[ptrA] "+r"(ptrA), [ptrB] "+r"(ptrB),
                 [ptrC] "+r"(ptrC), [ptrD] "+r"(ptrD),
                 [tw1] "+r"(tw1), [tw2] "+r"(tw2),
                 [tw3] "+r"(tw3)
                :[count] "r"(m >> 1)
                :"memory", "r14",
                 "q0", "q1", "q2", "q3",
                 "q4", "q5", "q6", "q7" );

            // rewind twiddles ptrs
            tw1 = tw1 - m;
            tw2 = tw2 - m;
            tw3 = tw3 - m;

            // increment original input pointers by mm
            // taking into account updates from the inner loop
            ptrA += ptrAandC_post_upd;
            ptrC += ptrAandC_post_upd;
            ptrB += ptrBandD_post_upd;
            ptrD += ptrBandD_post_upd;
        }
    }
}

void kf_bfly5_mve(kiss_fft_cpx * Fout, const size_t fstride,
                const kiss_fft_state * st, int m, int N, int mm, int stage)
{
    kiss_fft_cpx *Fout0, *Fout1, *Fout2, *Fout3, *Fout4;
    int       i;
    kiss_fft_cpx ya, yb;
    const int ptr_post_upd = mm - m;
    const kiss_fft_cpx *tw1, *tw2, *tw3, *tw4;
    kf_helium      *st_helium = (kf_helium *) st->arch_fft->priv;

    tw1 = st_helium->rearranged_twiddle_tab_stride1_arr[stage];
    tw2 = st_helium->rearranged_twiddle_tab_stride2_arr[stage];
    tw3 = st_helium->rearranged_twiddle_tab_stride3_arr[stage];
    tw4 = st_helium->rearranged_twiddle_tab_stride4_arr[stage];


#ifdef FIXED_POINT
    /* e^i(2.pi/5), e^i(pi/5) */
    ya.r = 663608942;
    ya.i = -(2042378317);
    yb.r = -(1737350766);
    yb.i = -(1262259218);
#else
    ya = st->twiddles[fstride * m];
    yb = st->twiddles[fstride * 2 * m];
#endif

    //Fout = Fout_beg + i*mm;
    Fout0 = Fout;
    Fout1 = Fout0 + m;
    Fout2 = Fout0 + 2 * m;
    Fout3 = Fout0 + 3 * m;
    Fout4 = Fout0 + 4 * m;

#pragma nounroll

    for (i = 0; i < N; i++)
    {
         register unsigned count  __asm("lr") = (m >> 1);

        __asm volatile (
            ".p2align 2                                 \n"
            "   vldrw.u32       q1, [%[ptrB]]           \n"
            "   vqdmlsdh.s32    q4, q1, q3              \n"
            "   vldrw.u32       q3, [%[tw1]], #16       \n"

            "  wls              lr, lr, 1f              \n"
            /* low overhead loop start */
            "2:                                         \n"
            "   vqdmladhx.s32   q4, q1, q3              \n"
            "   vldrw.u32       q6, [%[ptrE]]           \n"
            "   vldrw.u32       q3, [%[tw4]], #16       \n"
            "   vqdmlsdh.s32    q7, q6, q3              \n"
            "   vldrw.u32       q1, [%[ptrC]]           \n"
            "   vqdmladhx.s32   q7, q6,q3               \n"
            "   vldrw.u32       q6, [%[tw2]], #16       \n"
            "   vqdmlsdh.s32    q5, q1, q6              \n"
            "   vldrw.u32       q3, [%[tw3]], #16       \n"
            "   vqdmladhx.s32   q5, q1, q6              \n"
            "   vldrw.u32       q1, [%[ptrD]]           \n"
            "   vqdmlsdh.s32    q6, q1, q3              \n"
            "   vadd.s32        q0, q4, q7              \n"
            "   vqdmladhx.s32   q6, q1, q3              \n"
            "   vsub.s32        q3, q4, q7              \n"
            "   vadd.s32        q1, q5, q6              \n"
            "   vsub.s32        q2, q5, q6              \n"
            "   vldrw.u32       q4, [%[ptrA]]           \n"
            "   vadd.s32        q5, q4, q0              \n"
            "   vqdmulh.s32     q6, q0, %[yar]          \n"
            "   vadd.s32        q5, q5, q1              \n"
            "   vstrw.u32       q5, [%[ptrA]], #16      \n"
            "   vqdmulh.s32     q7, q1, %[ybr]          \n"
            "   vadd.s32        q5, q4, q6              \n"
            "   vqdmulh.s32     q6, q3, %[yai]          \n"
            "   vadd.s32        q5, q5, q7              \n"
            "   vqdmulh.s32     q7, q2, %[ybi]          \n"
            "   vadd.s32        q6, q6, q7              \n"
            "   vcadd.s32       q7, q5, q6, #90         \n"
            "   vstrw.u32       q7, [%[ptrB]], #16      \n"
            "   vcadd.s32       q7, q5, q6, #270        \n"
            "   vqdmulh.s32     q6, q0, %[ybr]          \n"
            "   vstrw.u32       q7, [%[ptrE]], #16      \n"
            "   vqdmulh.s32     q7, q1, %[yar]          \n"
            "   vadd.s32        q5, q4, q6              \n"
            "   vqdmulh.s32     q6, q2, %[yai]          \n"
            "   vadd.s32        q5, q5, q7              \n"
            "   vqdmulh.s32     q7, q3, %[ybi]          \n"
            "   vldrw.u32       q1, [%[ptrB]]           \n"
            "   vsub.s32        q6, q6, q7              \n"
            "   vldrw.u32       q3, [%[tw1]], #16       \n"
            "   vqdmlsdh.s32    q4, q1, q3              \n"
            "   vcadd.s32       q7, q5, q6, #270        \n"
            "   vstrw.u32       q7, [%[ptrC]], #16      \n"
            "   vcadd.s32       q7, q5, q6, #90         \n"
            "   vstrw.u32       q7, [%[ptrD]], #16      \n"
            /* low overhead loop end */
            "   le              lr, 2b                  \n"
            "1:                                         \n"

            :[ptrA] "+r"(Fout0),[ptrB] "+r"(Fout1),
             [ptrC] "+r"(Fout2),[ptrD] "+r"(Fout3),[ptrE] "+r"(Fout4),
             [tw1] "+r"(tw1),[tw2] "+r"(tw2),[tw3] "+r"(tw3),[tw4] "+r"(tw4),
             [count] "+r" (count)
            :[yar] "r"(ya.r),[yai] "r"(ya.i),
             [ybr] "r"(yb.r),[ybi] "r"(yb.i)
            :"memory",
             "q0", "q1", "q2", "q3",
             "q4", "q5", "q6", "q7");


        // rewind twiddles ptrs
        tw1 = tw1 - m;
        tw2 = tw2 - m;
        tw3 = tw3 - m;
        tw4 = tw4 - m;

        Fout0 += ptr_post_upd;
        Fout1 += ptr_post_upd;
        Fout2 += ptr_post_upd;
        Fout3 += ptr_post_upd;
        Fout4 += ptr_post_upd;
    }

}



static void opus_fft_impl_mve(const kiss_fft_state * st, kiss_fft_cpx * fout)
{
    int             m2, m;
    int             p;
    int             L;
    int             fstride[MAXFACTORS];
    int             i;
    int             shift;
    int             stage = 0;

    /* st->shift can be -1 */
    shift = st->shift > 0 ? st->shift : 0;

    fstride[0] = 1;
    L = 0;
    do {
        p = st->factors[2 * L];
        m = st->factors[2 * L + 1];
        fstride[L + 1] = fstride[L] * p;
        L++;
    }
    while (m != 1);
    m = st->factors[2 * L - 1];
    for (i = L - 1; i >= 0; i--) {
        if (i != 0)
            m2 = st->factors[2 * i - 1];
        else
            m2 = 1;

        switch (st->factors[2 * i]) {
          case 2:
              kf_bfly2_mve(fout, m, fstride[i], stage);
              break;
          case 4:
              kf_bfly4_mve(fout, fstride[i] << shift, st, m, fstride[i], m2, stage);
              break;
          case 3:
              kf_bfly3_mve(fout, fstride[i] << shift, st, m, fstride[i], m2, stage);
              break;
          case 5:
              kf_bfly5_mve(fout, fstride[i] << shift, st, m, fstride[i], m2, stage);
              break;
        }
        m = m2;
        stage++;
    }
}




void opus_fft_kf_reshuf_twidl(const size_t fstride, const kiss_fft_state * st, int m, int stage,
                     int radix)
{
    kf_helium      *st_helium = (kf_helium *) st->arch_fft->priv;
    const kiss_twiddle_cpx *tw1, *tw2, *tw3, *tw4;
    const int       scale = 16; //
    kiss_fft_cpx   *p_cur_tw1 = st_helium->p_cur_tw1;
    kiss_fft_cpx   *p_cur_tw2 = st_helium->p_cur_tw2;
    kiss_fft_cpx   *p_cur_tw3 = st_helium->p_cur_tw3;
    kiss_fft_cpx   *p_cur_tw4 = st_helium->p_cur_tw4;

    p_cur_tw1 = (kiss_fft_cpx *) __ALIGN8(p_cur_tw1);
    p_cur_tw2 = (kiss_fft_cpx *) __ALIGN8(p_cur_tw2);
    p_cur_tw3 = (kiss_fft_cpx *) __ALIGN8(p_cur_tw3);
    p_cur_tw4 = (kiss_fft_cpx *) __ALIGN8(p_cur_tw4);

    st_helium->rearranged_twiddle_tab_stride1_arr[stage] = p_cur_tw1;
    st_helium->rearranged_twiddle_tab_stride2_arr[stage] = p_cur_tw2;
    st_helium->rearranged_twiddle_tab_stride3_arr[stage] = p_cur_tw3;
    st_helium->rearranged_twiddle_tab_stride4_arr[stage] = p_cur_tw4;


    tw4 = tw3 = tw2 = tw1 = st->twiddles;
    for (int j = 0; j < m; j++) {
        kiss_fft_cpx    x;

        x.r = tw1->r << scale;
        x.i = tw1->i << scale;
        *p_cur_tw1++ = x;
        tw1 += fstride;

        x.r = tw2->r << scale;
        x.i = tw2->i << scale;
        *p_cur_tw2++ = x;
        tw2 += fstride * 2;

        if (radix > 3) {
            x.r = tw3->r << scale;
            x.i = tw3->i << scale;
            *p_cur_tw3++ = x;
            tw3 += fstride * 3;
        }
        if (radix > 4) {
            x.r = tw4->r << scale;
            x.i = tw4->i << scale;
            *p_cur_tw4++ = x;
            tw4 += fstride * 4;
        }
    }

    st_helium->p_cur_tw1 = p_cur_tw1;
    st_helium->p_cur_tw2 = p_cur_tw2;
    st_helium->p_cur_tw3 = p_cur_tw3;
    st_helium->p_cur_tw4 = p_cur_tw4;
}



 /**
  * @brief opus FFT vector-freindly twiddle expansion
  * @param[in, out]    st   KissFFT state
  * @return            none

  * Widening of the 16-bit twiddles into 32-bit
  * and re-arrangement allowing to use contigous vector load (stride removed)
  */
static void opus_fft_init_twiddle_mve(const kiss_fft_state * st)
{
    int             m2, m;
    int             p;
    int             L;
    int             fstride[MAXFACTORS];
    int             i;
    int             shift;
    int             stage = 0;

    /* st->shift can be -1 */
    shift = st->shift > 0 ? st->shift : 0;

    fstride[0] = 1;
    L = 0;
    do {
        p = st->factors[2 * L];
        m = st->factors[2 * L + 1];
        fstride[L + 1] = fstride[L] * p;
        L++;
    }
    while (m != 1);
    m = st->factors[2 * L - 1];
    for (i = L - 1; i >= 0; i--) {
        if (i != 0)
            m2 = st->factors[2 * i - 1];
        else
            m2 = 1;

        if (stage > 0) {
            // no twiddle involved in 1st stage
            switch (st->factors[2 * i]) {
              case 2:
                  // no twiddle
                  break;
              case 4:
                  opus_fft_kf_reshuf_twidl(fstride[i] << shift, st, m, stage, 4);
                  break;
              case 3:
                  opus_fft_kf_reshuf_twidl(fstride[i] << shift, st, m, stage, 3);
                  break;
              case 5:
                  opus_fft_kf_reshuf_twidl(fstride[i] << shift, st, m, stage, 5);
                  break;
            }
        }
        m = m2;
        stage++;
    }
}

 /**
  * @brief convert 16-bit bitreversal indexes into 64-bit
  * @param[in, out]    st      KissFFT state
  * @param[out]        bitrev  expanded bitreveral offset for scatter store
  * @return            None

  */

static void opus_fft_init_expand_bitrev_mve(const kiss_fft_state * st, uint64_t * bitrev)
{
    for (int i = 0; i < st->nfft; i++)
        bitrev[i] = st->bitrev[i];
}


 /**
  * @brief opus FFT private context size
  * @param[in, out]    st   KissFFT state
  * @return            private context size in bytes to be allocated

  */

uint32_t opus_fft_get_mve_ctx_size(const kiss_fft_state * st)
{
    return __ALIGN8(sizeof(kf_helium)) + __ALIGN8(sizeof(uint64_t) * st->nfft)
        + 4 * __ALIGN8(sizeof(kiss_fft_cpx) * MAX_TWIDDLE_ARR_LEN);
}



 /**
  * @brief opus FFT helium init
  * @param[in, out]    st   KissFFT state
  * @param[in, out]    mem  storage to hold helium private parts

  * private part contains data for vector-friendly expanded twiddles and bitreversal
  */

void opus_fft_init_mve(const kiss_fft_state * st, void *mem)
{
    uint8_t        *mem8 = (uint8_t *) mem;
    kf_helium      *st_helium;
    st_helium = (kf_helium *) mem8;
    mem8 += __ALIGN8(sizeof(kf_helium));

    st->arch_fft->is_supported = 1;
    st->arch_fft->priv = (void *) st_helium;


    uint64_t       *bitrev64 = (uint64_t *) mem8;
    opus_fft_init_expand_bitrev_mve(st, bitrev64);
    st_helium->bitrev64 = bitrev64;
    st_helium->bitrev_scal = (opus_val32) roundf(powf(2.0f, 31) / (float) (st->nfft));

    mem8 += __ALIGN8(sizeof(uint64_t) * st->nfft);

    st_helium->rearranged_twiddle_stride1 = (kiss_fft_cpx *) mem8;
    mem8 += __ALIGN8(sizeof(kiss_fft_cpx) * MAX_TWIDDLE_ARR_LEN);

    st_helium->rearranged_twiddle_stride2 = (kiss_fft_cpx *) mem8;
    mem8 += __ALIGN8(sizeof(kiss_fft_cpx) * MAX_TWIDDLE_ARR_LEN);

    st_helium->rearranged_twiddle_stride3 = (kiss_fft_cpx *) mem8;
    mem8 += __ALIGN8(sizeof(kiss_fft_cpx) * MAX_TWIDDLE_ARR_LEN);

    st_helium->rearranged_twiddle_stride4 = (kiss_fft_cpx *) mem8;
    mem8 += __ALIGN8(sizeof(kiss_fft_cpx) * MAX_TWIDDLE_ARR_LEN);


    st_helium->p_cur_tw1 = st_helium->rearranged_twiddle_stride1;
    st_helium->p_cur_tw2 = st_helium->rearranged_twiddle_stride2;
    st_helium->p_cur_tw3 = st_helium->rearranged_twiddle_stride3;
    st_helium->p_cur_tw4 = st_helium->rearranged_twiddle_stride4;

    opus_fft_init_twiddle_mve(st);
}


 /**
  * @brief opus forward FFT entry point
  * @param[in]  st   KissFFT state
  * @param[in]  fin  points to the complex input data buffer of size <code>fftLen</code>.
  * @param[out] fout points to the complex output data buffer of size <code>fftLen</code>.

  * opus_fft_init_mve has to be called prior to popluate the helium
  * private part of the KissFFT context
  */

void opus_fft_mve(const kiss_fft_state * st, const kiss_fft_cpx * fin,
                  kiss_fft_cpx * fout)
{
    kf_helium      *st_helium = (kf_helium *) st->arch_fft->priv;
    opus_val32      scale = st_helium->bitrev_scal;
    uint64_t       *bitrev64 = st_helium->bitrev64;

    celt_assert2(fin != fout, "In-place FFT not supported");

    /* Bit-reverse the input */
    __asm volatile  (
        ".p2align 2                                     \n"
        "  wls              lr, %[cnt], 1f              \n"
        "2:                                             \n"
        "   vldrw.32        q1, [%[pbitrev]], #16       \n"
        "   vldrw.32        q0, [%[fin]], #16           \n"
        "   vqdmulh.s32     q0, q0, %[scal]             \n"
        "   vstrd.64        q0, [%[fout], q1, UXTW #3]  \n"
        "   le              lr, 2b                      \n"
        "1:                                             \n"
        :[pbitrev] "+r"(bitrev64),[fin] "+r"(fin)
        :[scal] "r" (scale),[fout] "r"(fout), [cnt] "r"(st->nfft / 2)
        :"lr", "memory", "q0", "q1");

    opus_fft_impl_mve(st, fout);
}


 /**
  * @brief opus inverse FFT entry point
  * @param[in]  st   KissFFT state
  * @param[in]  fin  points to the complex input data buffer of size <code>fftLen</code>.
  * @param[out] fout points to the complex output data buffer of size <code>fftLen</code>.

  * opus_fft_init_mve has to be called prior to popluate the helium
  * private part of the KissFFT context
  */

void opus_ifft_mve(const kiss_fft_state *st,const kiss_fft_cpx *fin,kiss_fft_cpx *fout)
{
    kf_helium      *st_helium = (kf_helium *) st->arch_fft->priv;
    uint64_t       *bitrev64 = st_helium->bitrev64;
    int32x4_t conj={1,-1,1,-1};

    celt_assert2(fin != fout, "In-place FFT not supported");

    /* Bit-reverse the input */
    __asm volatile  (
        ".p2align 2                                     \n"
        "  wls              lr, %[cnt], 1f              \n"
        "2:                                             \n"
        "   vldrw.32        q1, [%[pbitrev]], #16       \n"
        "   vldrw.32        q0, [%[fin]], #16           \n"
        /* apply conjugate */
        "   vmul.s32        q0, q0, %[conj]             \n"
        "   vstrd.64        q0, [%[fout], q1, UXTW #3]  \n"
        "   le              lr, 2b                      \n"
        "1:                                             \n"
        :[pbitrev] "+r"(bitrev64),[fin] "+r"(fin)
        :[fout] "r"(fout), [cnt] "r"(st->nfft / 2), [conj] "t" (conj)
        :"lr", "memory", "q0", "q1");


    opus_fft_impl_mve(st, fout);

    __asm volatile  (
        ".p2align 2                                     \n"
        "  wls              lr, %[cnt], 1f              \n"
        "2:                                             \n"
        "   vldrw.32        q0, [%[fout]]               \n"
        /* apply conjugate */
        "   vmul.s32        q0, q0, %[conj]             \n"
        "   vstrw.32        q0, [%[fout]], #16          \n"
        "   le              lr, 2b                      \n"
        "1:                                             \n"
        :[fout] "+r"(fout)
        :[cnt] "r"(st->nfft / 2), [conj] "t" (conj)
        :"lr", "memory", "q0", "q1");
}


#endif
