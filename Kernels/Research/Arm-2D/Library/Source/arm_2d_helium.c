/*
 * Copyright (C) 2010-2021 Arm Limited or its affiliates. All rights reserved.
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

/* ----------------------------------------------------------------------
 * Project:      Arm-2D Library
 * Title:        arm-2d_helium.c
 * Description:  Acceleration extensions using Helium.
 *
 * $Date:        16. Sep 2021
 * $Revision:    V.0.10.0
 *
 * Target Processor:  Cortex-M cores with Helium
 *
 * -------------------------------------------------------------------- */

#define __ARM_2D_IMPL__

#include "arm_2d.h"
#include "__arm_2d_impl.h"

#if defined(__ARM_2D_HAS_HELIUM__) && __ARM_2D_HAS_HELIUM__

#if defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wincompatible-pointer-types-discards-qualifiers"
#   pragma clang diagnostic ignored "-Wcast-qual"
#   pragma clang diagnostic ignored "-Wcast-align"
#   pragma clang diagnostic ignored "-Wextra-semi-stmt"
#   pragma clang diagnostic ignored "-Wsign-conversion"
#   pragma clang diagnostic ignored "-Wunused-function"
#   pragma clang diagnostic ignored "-Wimplicit-int-float-conversion"
#   pragma clang diagnostic ignored "-Wdouble-promotion"
#   pragma clang diagnostic ignored "-Wunused-parameter"
#   pragma clang diagnostic ignored "-Wimplicit-float-conversion"
#   pragma clang diagnostic ignored "-Wimplicit-int-conversion"
#   pragma clang diagnostic ignored "-Wtautological-pointer-compare"
#   pragma clang diagnostic ignored "-Wmissing-prototypes"
#   pragma clang diagnostic ignored "-Wsign-compare"
#   pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#   pragma clang diagnostic ignored "-Wpadded"
#   pragma clang diagnostic ignored "-Wvector-conversion"
#endif



#include "__arm_2d_paving_helium.h"
#include "__arm_2d_math_helium.h"
#include "__arm_2d_utils_helium.h"
#ifdef   __cplusplus
extern "C" {
#endif




/*! \brief initialise the helium service service
 *! \param none
 *! \return none
 */
void __arm_2d_helium_init(void)
{
    /* even if this is empty, do not remove it */
}


/*----------------------------------------------------------------------------*
 * Code Template                                                              *
 *----------------------------------------------------------------------------*/

#define __API_COLOUR                rgb16
#define __API_INT_TYPE              uint16_t
#define __API_INT_TYPE_BIT_NUM      16

#include "__arm_2d_copy_helium.inc"


#define __API_COLOUR                rgb32
#define __API_INT_TYPE              uint32_t
#define __API_INT_TYPE_BIT_NUM      32

#include "__arm_2d_copy_helium.inc"


/*----------------------------------------------------------------------------*
 * Copy, Fill and Mirroring                                                   *
 *----------------------------------------------------------------------------*/


void __arm_copy_16_mve(uint16_t * pDst, const uint16_t * pSrc, uint32_t blockSize)
{
    do {
        mve_pred16_t    p = vctp16q(blockSize);

        vstrhq_p_u16(pDst, vldrhq_z_u16(pSrc, p), p);
        /*
         * Decrement the blockSize loop counter
         * Advance vector source and destination pointers
         */
        pSrc += 8;
        pDst += 8;
        blockSize -= 8;
    }
    while ((int32_t) blockSize > 0);
}

void __arm_copy_32_mve(uint32_t * pDst, const uint32_t * pSrc, uint32_t blockSize)
{
    do {
        mve_pred16_t    p = vctp32q(blockSize);

        vstrwq_p_u32(pDst, vldrwq_z_u32(pSrc, p), p);
        /*
         * Decrement the blockSize loop counter
         * Advance vector source and destination pointers
         */
        pSrc += 4;
        pDst += 4;
        blockSize -= 4;
    }
    while ((int32_t) blockSize > 0);
}

void __arm_copy_32_x_mirror_mve(uint32_t * pDst,
                                const uint32_t * pSrc, uint32_t width, uint32_t blockSize)
{
    uint32x4_t      offset;
    uint32_t        curOffsetIdx = width - 1;

    offset = vddupq_wb_u32(&curOffsetIdx, 1);
    do {
        mve_pred16_t    p = vctp32q(blockSize);

        uint32x4_t      reversedPixVec =
            vldrwq_gather_shifted_offset_z_u32(pSrc, offset, p);

        offset = vddupq_x_wb_u32(&curOffsetIdx, 1, p);

        vstrwq_p_u32(pDst, reversedPixVec, p);
        /*
         * Decrement the blockSize loop counter
         * Advance destination pointers
         */
        pDst += 4;
        blockSize -= 4;
    }
    while ((int32_t) blockSize > 0);
}


 void __arm_copy_16_mve_narrow(  uint16_t *phwSource,
                                                int16_t iSourceStride,
                                                uint16_t *phwTarget,
                                                int16_t iTargetStride,
                                                arm_2d_size_t *ptCopySize)
{
#ifdef USE_MVE_INTRINSICS
    for (int32_t x = 0; x < ptCopySize->iWidth; x++) {
        uint16x8_t      srcStr = vidupq_u16((uint32_t) 0, 1);
        uint16x8_t      dstStr = vidupq_u16((uint32_t) 0, 1);

        srcStr = srcStr * iSourceStride;
        dstStr = dstStr * iTargetStride;

        for (int32_t y = 0; y < ptCopySize->iHeight / 8; y++) {
            uint16x8_t      in = vldrhq_gather_shifted_offset_u16(phwSource, srcStr);
            srcStr = vaddq_n_u16(srcStr, (8 * iSourceStride));

            vstrhq_scatter_shifted_offset_u16(phwTarget, dstStr, in);
            dstStr = vaddq_n_u16(dstStr, (8 * iTargetStride));
        }

        phwSource++;
        phwTarget++;
    }
#else
    __asm volatile(
        "   clrm                {r2, r4}                               \n"
        "   vidup.u16           q0, r2, #1                             \n"
        "   vmul.i16            q2, q0, %[iSourceStride]               \n"
        "   vidup.u16           q1, r4, #1                             \n"
        "   vmul.i16            q3, q1, %[iTargetStride]               \n"

        "3:                                                            \n"
        /* outer loop, iterates over columns */
        /* size = ptCopySize->iWidth */
        "   vmov                q0, q2                                 \n"
        "   vmov                q1, q3                                 \n"

        /* inner loop, iterates over rows (size = ptCopySize->iHeight) */
        "   wlstp.16            lr, %[iHeight], 1f                     \n"
        ".p2align 2                                                    \n"
        "2:                                                            \n"
        "   vldrh.u16           q4, [%[phwSource], q0, uxtw #1]        \n"
        "   vadd.i16            q0, q0, %[iSourceStridex8]             \n"
        "   vstrh.16            q4, [%[phwTarget], q1, uxtw #1]        \n"
        "   vadd.i16            q1, q1, %[iTargetStridex8]             \n"
        "   letp                lr, 2b                                 \n"
        "1:                                                            \n"

        "   add.n               %[phwSource], #2                       \n"
        "   add.n               %[phwTarget], #2                       \n"
        "   subs                %[iWidth], #1                          \n"
        "   bne                 3b                                     \n"

        : [phwTarget] "+r"(phwTarget),  [phwSource] "+r"(phwSource)
        : [iHeight] "r"(ptCopySize->iHeight), [iWidth] "r" (ptCopySize->iWidth),
          [iSourceStride] "r" (iSourceStride),[iSourceStridex8] "r" (iSourceStride*8),
          [iTargetStride] "r" (iTargetStride),[iTargetStridex8] "r" (iTargetStride*8)
        : "r2", "r4", "q0", "q1", "q2", "q3", "q4", "memory", "r14", "cc");
#endif
}

 void __arm_copy_32_mve_narrow(   uint32_t *pwSource,
                                                int32_t iSourceStride,
                                                uint32_t *pwTarget,
                                                int32_t iTargetStride,
                                                arm_2d_size_t *ptCopySize)
{
#ifdef USE_MVE_INTRINSICS
    for (int_fast32_t x = 0; x < ptCopySize->iWidth; x++) {
        uint32x4_t      srcStr = vidupq_u32((uint32_t) 0, 1);
        uint32x4_t      dstStr = vidupq_u32((uint32_t) 0, 1);

        srcStr = srcStr * iSourceStride;
        dstStr = dstStr * iTargetStride;

        for (int_fast32_t y = 0; y < ptCopySize->iHeight / 4; y++) {
            uint32x4_t      in = vldrwq_gather_shifted_offset_u32(pwSource, srcStr);
            srcStr = vaddq_n_u32(srcStr, (4 * iSourceStride));

            vstrwq_scatter_shifted_offset_u32(pwTarget, dstStr, in);
            dstStr = vaddq_n_u32(dstStr, (4 * iTargetStride));
        }

        pwSource++;
        pwTarget++;
    }
#else
    __asm volatile(

        "   clrm                {r2, r4}                              \n"
        "   vidup.u32           q0, r2, #1                            \n"
        "   vmul.i32            q2, q0, %[iSourceStride]              \n"
        "   vidup.u32           q1, r4, #1                            \n"
        "   vmul.i32            q3, q1, %[iTargetStride]              \n"

        "3:                                                           \n"
        /* outer loop, iterates over columns */
        /* size = ptCopySize->iWidth */
        "   vmov                q0, q2                                \n"
        "   vmov                q1, q3                                \n"

        /* inner loop, iterates over rows (size = ptCopySize->iHeight) */
        "   wlstp.32            lr, %[iHeight], 1f                    \n"
        ".p2align 2                                                   \n"
        "2:                                                           \n"
        "   vldrw.u32           q4, [%[pwSource], q0, uxtw #2]        \n"
        "   vadd.i32            q0, q0, %[iSourceStridex4]            \n"
        "   vstrw.32            q4, [%[pwTarget], q1, uxtw #2]        \n"
        "   vadd.i32            q1, q1, %[iTargetStridex4]            \n"
        "   letp                lr, 2b                                \n"
        "1:                                                           \n"

        "   add.n               %[pwSource], #4                       \n"
        "   add.n               %[pwTarget], #4                       \n"
        "   subs                %[iWidth], #1                         \n"
        "   bne                 3b                                    \n"

        : [pwTarget] "+r"(pwTarget),  [pwSource] "+r"(pwSource)
        : [iHeight] "r"(ptCopySize->iHeight), [iWidth] "r" (ptCopySize->iWidth),
          [iSourceStride] "r" (iSourceStride),[iSourceStridex4] "r" (iSourceStride*4),
          [iTargetStride] "r" (iTargetStride),[iTargetStridex4] "r" (iTargetStride*4)
        : "r2", "r4", "q0", "q1", "q2", "q3", "q4", "memory", "r14", "cc");
#endif
}




__OVERRIDE_WEAK
void __arm_2d_impl_rgb16_copy(   uint16_t *phwSource,
                                int16_t iSourceStride,
                                uint16_t *phwTarget,
                                int16_t iTargetStride,
                                arm_2d_size_t *ptCopySize)
{
    /*
     * 16-bit Narrow copy case:
     * use column copy with scatter / gather
     */
    if(ptCopySize->iWidth <= 4) {
        __arm_copy_16_mve_narrow(phwSource,
                                    iSourceStride,
                                    phwTarget,
                                    iTargetStride,
                                    ptCopySize);

    } else  if((((uint32_t)phwSource & 3) == 0) && (((uint32_t)phwTarget & 3) == 0)
        && ((iSourceStride & 3) == 0) && ((iTargetStride & 3) ==0)) {
        /*
         * source / dst & strides are 64-bit aligned
         * use scalar LDRD/STRD, faster than back to back vector VLDR/VSTR on M55
         */
        __asm volatile(
            "3:                                                          \n"

            "   mov                 r0, %[phwSource]                     \n"
            "   mov                 r1, %[phwTarget]                     \n"

            /* scalar version faster (no DTCM bank conflict)*/
            "   wls                 lr, %[iWidth], 1f                    \n"
            ".p2align 2                                                  \n"
            "2:                                                          \n"
            "   ldrd                r2, r3, [r0], #8                     \n"
            "   strd                r2, r3, [r1], #8                     \n"
            "   le                  lr, 2b                               \n"
            "1:                                                          \n"
           // tail
            "   wls                 lr, %[iWidthTail], 1f                \n"
            ".p2align 2                                                  \n"
            "2:                                                          \n"
            "   ldrh                r2, [r0], #2                         \n"
            "   strh                r2, [r1], #2                         \n"
            "   le                  lr, 2b                               \n"
            "1:                                                          \n"

            "   add                 %[phwSource], %[iSourceStride]       \n"
            "   add                 %[phwTarget], %[iTargetStride]       \n"
            "   subs                %[iHeight], #1                       \n"
            "   bne                 3b                                   \n"

            : [phwTarget] "+r"(phwTarget),  [phwSource] "+r"(phwSource)
            : [iHeight] "r"(ptCopySize->iHeight), [iWidth] "r" (ptCopySize->iWidth/4),
              [iWidthTail] "r" (ptCopySize->iWidth & 3),
              [iSourceStride] "r" (iSourceStride*sizeof(uint16_t)),
              [iTargetStride] "r" (iTargetStride*sizeof(uint16_t))
            : "r0", "r1", "r2", "r3", "q0", "memory", "r14", "cc"
            );
        }
        else
        {
        /*
         * generic column major 16-bit 2D copy
         */
        int32_t iWidth = ptCopySize->iWidth;
        int32_t iHeight = ptCopySize->iHeight;

        __asm volatile(
            "   mov                 r2, %[iHeight]                  \n"
            "3:                                                     \n"

            "   mov                 r0, %[phwSource]                \n"
            "   mov                 r1, %[phwTarget]                \n"

            "   wlstp.16            lr, %[iWidth], 1f               \n"
            ".p2align 2                                             \n"
            "2:                                                     \n"
            "   vldrh.u16           q0, [r0], #16                   \n"
            "   vstrh.16            q0, [r1], #16                   \n"
            "   letp                lr, 2b                          \n"
            "1:                                                     \n"
            "   add                 %[phwSource], %[iSourceStride]  \n"
            "   add                 %[phwTarget], %[iTargetStride]  \n"
            "   subs                r2, #1                          \n"
            "   bne                 3b                              \n"

            : [phwTarget] "+r"(phwTarget),  [phwSource] "+r"(phwSource)
            : [iHeight] "r"(iHeight), [iWidth] "r" (iWidth),
              [iSourceStride] "r" (iSourceStride*sizeof(uint16_t)),
              [iTargetStride] "r" (iTargetStride*sizeof(uint16_t))
            : "r0", "r1", "r2", "q0", "memory", "r14", "cc");
    }
}

__OVERRIDE_WEAK
 void __arm_2d_impl_rgb32_copy(   uint32_t *pwSource,
                                                int16_t iSourceStride,
                                                uint32_t *pwTarget,
                                                int16_t iTargetStride,
                                                arm_2d_size_t *ptCopySize)
{
    if(ptCopySize->iWidth <= 2) {
        /*
         * 32-bit Narrow copy case:
         * use column copy with scatter / gather
         */
        __arm_copy_32_mve_narrow(pwSource,
                                    iSourceStride,
                                    pwTarget,
                                    iTargetStride,
                                    ptCopySize);

    } else  if((((uint32_t)pwSource & 3) == 0) && (((uint32_t)pwTarget & 3) == 0)
        && ((iSourceStride & 3) == 0) && ((iTargetStride & 3) ==0)) {
        /*
         * source / dst & strides are 64-bit aligned
         * use scalar LDRD/STRD, faster than back to back vector VLDR/VSTR on M55
         */
        __asm volatile(
            "3:                                                          \n"
            "   mov                 r0, %[pwSource]                      \n"
            "   mov                 r1, %[pwTarget]                      \n"

            /* scalar version faster (no DTCM bank conflict)*/
            "   wls                 lr, %[iWidth], 1f                    \n"
            ".p2align 2                                                  \n"
            "2:                                                          \n"
            "   ldrd                r2, r3, [r0], #8                     \n"
            "   strd                r2, r3, [r1], #8                     \n"
            "   le                  lr, 2b                               \n"
            "1:                                                          \n"
           // tail
            "   wls                 lr, %[iWidthTail], 1f                \n"
            ".p2align 2                                                  \n"
            "2:                                                          \n"
            "   ldr                 r2, [r0], #4                         \n"
            "   str                 r2, [r1], #4                         \n"
            "   le                  lr, 2b                               \n"
            "1:                                                          \n"

            "   add                 %[pwSource], %[iSourceStride]        \n"
            "   add                 %[pwTarget], %[iTargetStride]        \n"
            "   subs                %[iHeight], #1                       \n"
            "   bne                 3b                                   \n"

            : [pwTarget] "+r"(pwTarget),  [pwSource] "+r"(pwSource)
            : [iHeight] "r"(ptCopySize->iHeight), [iWidth] "r" (ptCopySize->iWidth/2),
              [iWidthTail] "r" (ptCopySize->iWidth & 1),
              [iSourceStride] "r" (iSourceStride*sizeof(uint32_t)),
              [iTargetStride] "r" (iTargetStride*sizeof(uint32_t))
            : "r0", "r1", "r2", "r3", "q0", "memory", "r14", "cc"
            );
        }
        else
        {
        /*
         * generic column major 32-bit 2D copy
         */
        __asm volatile(
            "3:                                                     \n"

            "   mov                 r0, %[pwSource]                 \n"
            "   mov                 r1, %[pwTarget]                 \n"

            "   wlstp.32            lr, %[iWidth], 1f               \n"
            ".p2align 2                                             \n"
            "2:                                                     \n"
            "   vldrw.32           q0, [r0], #16                    \n"
            "   vstrw.32            q0, [r1], #16                   \n"
            "   letp                lr, 2b                          \n"
            "1:                                                     \n"
            "   add                 %[pwSource], %[iSourceStride]   \n"
            "   add                 %[pwTarget], %[iTargetStride]   \n"
            "   subs                %[iHeight], #1                  \n"
            "   bne                 3b                              \n"

            : [pwTarget] "+r"(pwTarget),  [pwSource] "+r"(pwSource)
            : [iHeight] "r"(ptCopySize->iHeight), [iWidth] "r" (ptCopySize->iWidth),
              [iSourceStride] "r" (iSourceStride*sizeof(uint32_t)),
              [iTargetStride] "r" (iTargetStride*sizeof(uint32_t))
            : "r0", "r1", "q0", "memory", "r14", "cc");
        }
}

/*----------------------------------------------------------------------------*
 * alpha blending                                                             *
 *----------------------------------------------------------------------------*/
/* RGB16 Mix */


__OVERRIDE_WEAK
void __arm_2d_impl_rgb565_alpha_blending(   uint16_t *phwSourceBase,
                                            int16_t iSourceStride,
                                            uint16_t *phwTargetBase,
                                            int16_t iTargetStride,
                                            arm_2d_size_t *ptCopySize,
                                            uint_fast8_t chRatio)
{
#ifdef USE_MVE_INTRINSICS
    int32_t         blkCnt;
    uint16_t        ratio1x8 = chRatio * 8;
    uint16_t        ratio1x4 = chRatio * 4;
    uint16_t        ratio2x8 = (256 - chRatio) * 8;
    uint16_t        ratio2x4 = (256 - chRatio) * 4;

    uint16x8_t      vecMaskR = vdupq_n_u16(0x001f);
    uint16x8_t      vecMaskG = vdupq_n_u16(0x003f);
    uint16x8_t      vecMaskBpck = vdupq_n_u16(0x00f8);
    uint16x8_t      vecMaskGpck = vdupq_n_u16(0x00fc);


    for (int32_t y = 0; y < ptCopySize->iHeight; y++) {
        const uint16_t *phwSource = phwSourceBase;
        uint16_t       *phwTarget = phwTargetBase;

        blkCnt = ptCopySize->iWidth;
        do {

            uint16x8_t      vecIn;
            uint16x8_t      vecR0, vecB0, vecG0;
            uint16x8_t      vecR1, vecB1, vecG1;

            /* unpack 1st stream */
            vecIn = vld1q(phwSource);
            vecR0 = vecIn & vecMaskR;

            vecB0 = vecIn >> 11;

            vecG0 = vecIn >> 5;
            vecG0 = vecG0 & vecMaskG;


            /* unpack 2nd stream */
            vecIn = vld1q(phwTarget);
            vecR1 = vecIn & vecMaskR;

            vecB1 = vecIn >> 11;

            vecG1 = vecIn >> 5;
            vecG1 = vecG1 & vecMaskG;


            /* merge */
            vecR0 = vecR0 * ratio1x8 + vecR1 * ratio2x8;
            vecR0 = vecR0 >> 8;

            vecG0 = vecG0 * ratio1x4 + vecG1 * ratio2x4;
            vecG0 = vecG0 >> 8;

            vecB0 = vecB0 * ratio1x8 + vecB1 * ratio2x8;
            vecB0 = vecB0 >> 8;


            /* pack */
            uint16x8_t      vOut = vecR0 >> 3 | vmulq((vecG0 & vecMaskGpck), 8)
                | vmulq((vecB0 & vecMaskBpck), 256);


            vst1q(phwTarget, vOut);

            phwSource += 8;
            phwTarget += 8;
            blkCnt -= 8;
        }
        while (blkCnt > 0);

        phwSourceBase += iSourceStride;
        phwTargetBase += iTargetStride;
    }

#else /* USE_MVE_INTRINSICS  */


    uint16_t        ratio1x8 = chRatio * 8;
    uint16_t        ratio1x4 = chRatio * 4;
    uint16_t        ratio2x8 = (256 - chRatio) * 8;
    uint16_t        ratio2x4 = (256 - chRatio) * 4;
    uint16x8_t      vecMaskR = vdupq_n_u16(0x001f);
    uint16x8_t      vecMaskG = vdupq_n_u16(0x003f);
    uint16x8_t      vecMaskBpck = vdupq_n_u16(0x00f8);
    uint32_t        iWidth = ptCopySize->iWidth;
    int32_t         row = ptCopySize->iHeight;
    uint16x8_t      scratch[1];

    vst1q((uint16_t *)scratch, vdupq_n_u16(0x00fc));

    do {
        const uint16_t *pSource = phwSourceBase;
        uint16_t       *pTarget = phwTargetBase;
        register unsigned loopCnt  __asm("lr");
        loopCnt = iWidth;

    __asm volatile(
        ".p2align 2                                              \n"
        "   vldrh.u16               q4, [%[pTarget]]             \n"
        "   vldrh.u16               q5, [%[pSource]], #16        \n"

        "   wlstp.16                lr, %[loopCnt], 1f           \n"
        "2:                                                      \n"
        // B target extraction
        // right shift by 5 (x 1/32) for M55 friendly
        // IV / Mul pipe interleaving
        "   vqdmulh.s16             q2, q4, %[rshft5]            \n"
        "   vand                    q7, q4, %[vecMaskR]          \n"

        "   vmul.i16                q6, q7, %[ratio2x8]          \n"
        // B source extraction
        "   vand                    q7, q5, %[vecMaskR]          \n"
        // B mix
        "   vmla.u16                q6, q7, %[ratio1x8]          \n"
        // G extraction
        "   vand                    q2, q2, %[vecMaskG]          \n"
        "   vshr.u16                q7, q5, #5                   \n"
        "   vmul.i16                q2, q2, %[ratio2x4]          \n"
        // G extraction
        "   vand                    q7, q7, %[vecMaskG]          \n"
        // G mix
        "   vmla.u16                q2, q7, %[ratio1x4]          \n"
        // R extraction
        "   vshr.u16                q4, q4, #11                  \n"
        "   vmul.i16                q7, q4, %[ratio2x8]          \n"
        // R extraction
        "   vshr.u16                q5, q5, #11                  \n"
        // R mix
        "   vmla.u16                q7, q5, %[ratio1x8]          \n"

        "   vshr.u16                q2, q2, #8                   \n"
        "   vldrh.16                q5, [%[scratch]]             \n"

        "   vand                    q2, q2, q5                   \n"
        // vmulq((vecG0 & 0x00fc), 8)
        "   vmul.i16                q2, q2, %[eight]             \n"
        "   vshr.u16                q4, q7, #8                   \n"
        // schedule next source load
        "   vldrh.u16               q5, [%[pSource]], #16        \n"
        "   vand                    q7, q4, %[vecMaskBpck]       \n"
        // pack R & G
        // vmulq((vecG0 & vecMaskGpck), 8) + vmulq((vecR0 & vecMaskRpck), 256)
        "   vmla.u16                q2, q7, %[twofiftysix]       \n"
        // downshift B ((vecB0 >> 8) >> 3)
        "   vshr.u16                q7, q6, #11                  \n"
        // schedule next target load (pre offset as target not imcrementred so far)
        "   vldrh.u16               q4, [%[pTarget], #16]        \n"
        // pack blue with R&G
        "   vorr                    q2, q2, q7                   \n"

        "   vstrh.16                q2, [%[pTarget]], #16        \n"
        "   letp                    lr, 2b                       \n"
        "1:                                                      \n"

        : [pSource] "+r"(pSource),  [pTarget] "+r" (pTarget), [loopCnt] "+r"(loopCnt)
        : [vecMaskR] "t" (vecMaskR), [vecMaskG] "t" (vecMaskG),
          [vecMaskBpck] "t" (vecMaskBpck),
          [ratio1x8] "r" (ratio1x8), [ratio2x8] "r" (ratio2x8),
          [ratio1x4] "r" (ratio1x4), [ratio2x4] "r" (ratio2x4),
          [eight] "r" (8), [twofiftysix] "r" (256), [rshft5] "r" (1024), [scratch] "r" (scratch)
        : "q2", "q4", "q5", "q6", "q7", "memory" );

        phwSourceBase += iSourceStride;
        phwTargetBase += iTargetStride;
    } while (--row);
#endif /* USE_MVE_INTRINSICS */
}




__OVERRIDE_WEAK
void __arm_2d_impl_rgb565_colour_filling_with_opacity(
                                        uint16_t *__RESTRICT pTargetBase,
                                        int16_t iTargetStride,
                                        arm_2d_size_t *__RESTRICT ptCopySize,
                                        uint16_t Colour,
                                        uint_fast8_t chRatio)
{
#ifdef USE_MVE_INTRINSICS
    int32_t         blkCnt;
    uint16_t        ratio1x8 = chRatio * 8;
    uint16_t        ratio1x4 = chRatio * 4;
    uint16_t        ratio2x8 = (256 - chRatio) * 8;
    uint16_t        ratio2x4 = (256 - chRatio) * 4;

    uint16x8_t      vecMaskR = vdupq_n_u16(0x001f);
    uint16x8_t      vecMaskG = vdupq_n_u16(0x003f);
    uint16x8_t      vecMaskBpck = vdupq_n_u16(0x00f8);
    uint16x8_t      vecMaskGpck = vdupq_n_u16(0x00fc);
    uint16x8_t      vecIn;
    uint16x8_t      vecColorR, vecColorB, vecColorG;

    /* unpack color & scale */
    vecIn = vdupq_n_u16(Colour);
    vecColorR = (vecIn & vecMaskR) * ratio1x8;

    vecColorB = (vecIn >> 11) * ratio1x8;

    vecColorG = vecIn >> 5;
    vecColorG = (vecColorG & vecMaskG) * ratio1x4;

    for (int32_t y = 0; y < ptCopySize->iHeight; y++) {
        uint16_t       *phwTarget = pTargetBase;

        blkCnt = ptCopySize->iWidth;
        do {
            uint16x8_t      vecR0, vecB0, vecG0;
            uint16x8_t      vecR1, vecB1, vecG1;

            /* unpack stream */
            vecIn = vld1q(phwTarget);
            vecR1 = vecIn & vecMaskR;

            vecB1 = vecIn >> 11;

            vecG1 = vecIn >> 5;
            vecG1 = vecG1 & vecMaskG;


            /* merge */
            vecR0 = vecColorR + vecR1 * ratio2x8;
            vecR0 = vecR0 >> 8;

            vecG0 = vecColorG + vecG1 * ratio2x4;
            vecG0 = vecG0 >> 8;

            vecB0 = vecColorB + vecB1 * ratio2x8;
            vecB0 = vecB0 >> 8;

            /* pack */
            uint16x8_t      vOut = vecR0 >> 3 | vmulq((vecG0 & vecMaskGpck), 8)
                | vmulq((vecB0 & vecMaskBpck), 256);

            vst1q(phwTarget, vOut);

            phwTarget += 8;
            blkCnt -= 8;
        }
        while (blkCnt > 0);

        pTargetBase += iTargetStride;
    }

#else /* USE_MVE_INTRINSICS  */

    uint16_t        ratio1x8 = chRatio * 8;
    uint16_t        ratio1x4 = chRatio * 4;
    uint16_t        ratio2x8 = (256 - chRatio) * 8;
    uint16_t        ratio2x4 = (256 - chRatio) * 4;
    uint16x8_t      vecMaskR = vdupq_n_u16(0x001f);
    uint16x8_t      vecMaskG = vdupq_n_u16(0x003f);
    uint16x8_t      vecMaskBpck = vdupq_n_u16(0x00f8);
    uint16x8_t      vecColorR, vecColorB, vecColorG;
    uint16x8_t      scratch[4];

    /* unpack color */
    uint16x8_t vecIn = vdupq_n_u16(Colour);
    vecColorR = vecIn & vecMaskR;
    vecColorB = vecIn >> 11;
    vecColorG = vecIn >> 5;
    vecColorG = vecColorG & vecMaskG;
    vst1q((uint16_t*)scratch, vecColorR * ratio1x8);
    vst1q((uint16_t*)&scratch[1], vecColorB * ratio1x8);
    vst1q((uint16_t*)&scratch[2], vecColorG * ratio1x4);
    vst1q((uint16_t*)&scratch[3], vdupq_n_u16(0x00fc));

    int32_t         row = ptCopySize->iHeight;
    do {

        uint16_t       *phwTarget = pTargetBase;
        register unsigned loopCnt  __asm("lr");
        loopCnt = ptCopySize->iWidth;

    __asm volatile(
        "   vldrh.u16               q4, [%[phwTarget]]           \n"

        "   wlstp.16                lr, %[loopCnt], 1f           \n"
        ".p2align 2                                              \n"
        "2:                                                      \n"
        // B target extraction
        "   vand                    q7, q4, %[vecMaskR]          \n"
        "   vldrh.u16               q6, [%[scratch]]             \n"
        "   vshr.u16                q2, q4, #5                   \n"

        // B mix
        "   vmla.u16                q6, q7, %[ratio2x8]          \n"
        // G extraction
        "   vand                    q7, q2, %[vecMaskG]          \n"

        // G extraction
        "   vldrh.u16               q2, [%[scratch], #32]        \n"
        // G mix
        "   vmla.u16                q2, q7, %[ratio2x4]          \n"

        "   vshr.u16                q4, q4, #11                  \n"
        // R extraction
        "   vldrh.u16               q7, [%[scratch], #16]        \n"
        "   vshr.u16                q2, q2, #8                   \n"
        // R mix
        "   vmla.u16                q7, q4, %[ratio2x8]          \n"
        "   vshr.u16                q4, q7, #8                   \n"

        // load duplicated 0xfc mask
        "   vldrh.u16               q7, [%[scratch], #48]        \n"
        "   vand                    q2, q2, q7                   \n"

        "   vmul.i16                q2, q2, %[eight]             \n"
        "   vand                    q7, q4, %[vecMaskBpck]       \n"

        // pack R & G
        "   vmla.u16                q2, q7, %[twofiftysix]       \n"
        // downshift B ((vecB0 >> 8) >> 3)
        "   vshr.u16                q7, q6, #11                  \n"
        // schedule next target load
        "   vldrh.u16               q4, [%[phwTarget], #16]      \n"
        // pack blue with R&G
        "   vorr                    q2, q2, q7                   \n"
        "   vstrh.16                q2, [%[phwTarget]], #16      \n"
        "   letp                    lr, 2b                       \n"
        "1:                                                      \n"
        : [phwTarget] "+r" (phwTarget), [loopCnt] "+r"(loopCnt)
        : [vecMaskR] "t" (vecMaskR), [vecMaskG] "t" (vecMaskG),
          [vecMaskBpck] "t" (vecMaskBpck),
          [ratio2x8] "r" (ratio2x8), [ratio2x4] "r" (ratio2x4),
          [eight] "r" (8), [twofiftysix] "r" (256), [scratch] "r" (scratch)
        : "q2", "q4", "q5", "q6", "q7", "memory" );

        pTargetBase += iTargetStride;
    } while (--row);

#endif /* USE_MVE_INTRINSICS */

}


__OVERRIDE_WEAK
void __arm_2d_impl_rgb565_alpha_blending_colour_keying(
                                                uint16_t * __RESTRICT phwSource,
                                                int16_t         iSourceStride,
                                                uint16_t * __RESTRICT phwTarget,
                                                int16_t         iTargetStride,
                                                arm_2d_size_t * __RESTRICT ptCopySize,
                                                uint_fast8_t    chRatio,
                                                uint32_t   hwColour)
{
#ifdef USE_MVE_INTRINSICS
    uint32_t        iHeight = ptCopySize->iHeight;
    uint32_t        iWidth = ptCopySize->iWidth;

    int32_t         blkCnt;
    uint16_t        ratio1x8 = chRatio * 8;
    uint16_t        ratio1x4 = chRatio * 4;
    uint16_t        ratio2x8 = (256 - chRatio) * 8;
    uint16_t        ratio2x4 = (256 - chRatio) * 4;

    uint16x8_t      vecMaskR = vdupq_n_u16(0x001f);
    uint16x8_t      vecMaskG = vdupq_n_u16(0x003f);
    uint16x8_t      vecMaskBpck = vdupq_n_u16(0x00f8);
    uint16x8_t      vecMaskGpck = vdupq_n_u16(0x00fc);

    for (uint32_t y = 0; y < iHeight; y++) {
        // - inconditional blending + predicated dst update
        const uint16_t *pSource = phwSource;
        uint16_t       *pTarget = phwTarget;
        blkCnt = iWidth >> 3;

        while (blkCnt > 0) {
            uint16x8_t      vecInSrc, vecInDst;
            uint16x8_t      vecR0, vecB0, vecG0;
            uint16x8_t      vecR1, vecB1, vecG1;

            /* unpack 1st stream */
            vecInSrc = vld1q(pSource);
            vecR0 = vandq(vecInSrc, vecMaskR);
            vecB0 = vshrq(vecInSrc, 11);
            vecG0 = vshrq(vecInSrc, 5);
            vecG0 = vandq(vecG0, vecMaskG);

            /* unpack 2nd stream */
            vecInDst = vld1q(pTarget);
            vecR1 = vandq(vecInDst, vecMaskR);
            vecB1 = vshrq(vecInDst, 11);
            vecG1 = vshrq(vecInDst, 5);
            vecG1 = vandq(vecG1, vecMaskG);

            /* merge */
            vecR0 = vmlaq(vmulq(vecR0, ratio1x8), vecR1, ratio2x8);
            vecR0 = vshrq(vecR0, 8);
            vecG0 = vmlaq(vmulq(vecG0, ratio1x4), vecG1, ratio2x4);
            vecG0 = vshrq(vecG0, 8);
            vecB0 = vmlaq(vmulq(vecB0, ratio1x8), vecB1, ratio2x8);
            vecB0 = vshrq(vecB0, 8);

            /* pack */
            uint16x8_t      vOut = vorrq(vshrq(vecR0, 3),
                                         vmulq(vandq(vecG0, vecMaskGpck), 8));

            vOut = vorrq(vOut, vmulq(vandq(vecB0, vecMaskBpck), 256));

            vst1q_p(pTarget, vOut, vcmpneq_n_s16(vecInSrc, hwColour));

            pSource += 8;
            pTarget += 8;
            blkCnt--;

        }

        blkCnt = iWidth & 7;
        if (blkCnt > 0U) {
            uint16x8_t      vecInSrc, vecInDst;
            uint16x8_t      vecR0, vecB0, vecG0;
            uint16x8_t      vecR1, vecB1, vecG1;

            /* unpack 1st stream */
            vecInSrc = vld1q(pSource);
            vecR0 = vandq(vecInSrc, vecMaskR);
            vecB0 = vshrq(vecInSrc, 11);
            vecG0 = vshrq(vecInSrc, 5);
            vecG0 = vandq(vecG0, vecMaskG);

            /* unpack 2nd stream */
            vecInDst = vld1q(pTarget);
            vecR1 = vandq(vecInDst, vecMaskR);
            vecB1 = vshrq(vecInDst, 11);
            vecG1 = vshrq(vecInDst, 5);
            vecG1 = vandq(vecG1, vecMaskG);

            /* merge */
            vecR0 = vmlaq(vmulq(vecR0, ratio1x8), vecR1, ratio2x8);
            vecR0 = vshrq(vecR0, 8);
            vecG0 = vmlaq(vmulq(vecG0, ratio1x4), vecG1, ratio2x4);
            vecG0 = vshrq(vecG0, 8);
            vecB0 = vmlaq(vmulq(vecB0, ratio1x8), vecB1, ratio2x8);
            vecB0 = vshrq(vecB0, 8);

            /* pack */
            uint16x8_t      vOut = vorrq(vshrq(vecR0, 3),
                                         vmulq(vandq(vecG0, vecMaskGpck), 8));

            vOut = vorrq(vOut,
                    vmulq(vandq(vecB0, vecMaskBpck), 256));

            vst1q_p(pTarget, vOut,
                    vcmpneq_m_n_s16(vecInSrc, hwColour, vctp16q(blkCnt)));

        }

        phwSource += iSourceStride;
        phwTarget += iTargetStride;
    }
#else
    uint32_t        iHeight = ptCopySize->iHeight;
    uint32_t        iWidth = ptCopySize->iWidth;

    uint16_t        ratio1x8 = chRatio * 8;
    uint16_t        ratio1x4 = chRatio * 4;
    uint16_t        ratio2x8 = (256 - chRatio) * 8;
    uint16_t        ratio2x4 = (256 - chRatio) * 4;

    uint16x8_t      vecMaskR = vdupq_n_u16(0x001f);
    uint16x8_t      vecMaskG = vdupq_n_u16(0x003f);
    uint16x8_t      vecMaskBpck = vdupq_n_u16(0x00f8);
    uint16x8_t      scratch[1];

    vst1q((uint16_t *)scratch, vdupq_n_u16(0x00fc));

    for (uint32_t y = 0; y < iHeight; y++) {

        const uint16_t *pSource = phwSource;
        uint16_t       *pTarget = phwTarget;
        register unsigned loopCnt  __asm("lr");
        loopCnt = iWidth;

    __asm volatile(
        ".p2align 2                                              \n"
        "   vldrh.u16               q4, [%[pTarget]]             \n"
        "   vldrh.u16               q5, [%[pSource]], #16        \n"
        "   vand                    q7, q4, %[vecMaskR]          \n"
        "   wlstp.16                lr, %[loopCnt], 1f           \n"
        "2:                                                      \n"
        // B target extraction
        "   vshr.u16                q2, q4, #5                   \n"
        "   vmul.i16                q6, q7, %[ratio2x8]          \n"
        // B source extraction
        "   vand                    q7, q5, %[vecMaskR]          \n"
        // B mix
        "   vmla.u16                q6, q7, %[ratio1x8]          \n"
        // G extraction
        "   vand                    q2, q2, %[vecMaskG]          \n"
        "   vshr.u16                q7, q5, #5                   \n"
        "   vmul.i16                q2, q2, %[ratio2x4]          \n"
        // G extraction
        "   vand                    q7, q7, %[vecMaskG]          \n"
        // G mix
        "   vmla.u16                q2, q7, %[ratio1x4]          \n"
        // R extraction
        "   vshr.u16                q4, q4, #11                  \n"
        "   vmul.i16                q7, q4, %[ratio2x8]          \n"
        // R extraction
        "   vshr.u16                q5, q5, #11                  \n"
        // R mix
        "   vmla.u16                q7, q5, %[ratio1x8]          \n"

        "   vshr.u16                q2, q2, #8                   \n"
        "   vldrh.16                q5, [%[scratch]]             \n"

        "   vand                    q2, q2, q5                   \n"
        // vmulq((vecG0 & 0x00fc), 8)
        "   vmul.i16                q2, q2, %[eight]             \n"
        "   vshr.u16                q4, q7, #8                   \n"
        // schedule next source load
        "   vldrh.u16               q5, [%[pSource]], #16        \n"
        "   vand                    q7, q4, %[vecMaskBpck]       \n"
        // pack R & G
        // vmulq((vecG0 & vecMaskGpck), 8) + vmulq((vecR0 & vecMaskRpck), 256)
        "   vmla.u16                q2, q7, %[twofiftysix]       \n"
        // downshift B ((vecB0 >> 8) >> 3)
        "   vshr.u16                q7, q6, #11                  \n"
        // schedule next target load (pre offset as target not imcrementred so far)
        "   vldrh.u16               q4, [%[pTarget], #16]        \n"
        // pack blue with R&G
        "   vorr                    q2, q2, q7                   \n"
        "   vldrh.u16               q6, [%[pSource], #-32]       \n"
        "   vand                    q7, q4, %[vecMaskR]          \n"
        "   vpt.u16                 ne, q6, %[hwColour]          \n"
        "   vstrht.16               q2, [%[pTarget]], #16        \n"
        "   letp                    lr, 2b                       \n"
        "1:                                                      \n"

        : [pSource] "+r"(pSource),  [pTarget] "+r" (pTarget), [loopCnt] "+r"(loopCnt)
        : [vecMaskR] "t" (vecMaskR), [vecMaskG] "t" (vecMaskG),
          [vecMaskBpck] "t" (vecMaskBpck),
          [ratio1x8] "r" (ratio1x8), [ratio2x8] "r" (ratio2x8),
          [ratio1x4] "r" (ratio1x4), [ratio2x4] "r" (ratio2x4),
          [eight] "r" (8), [twofiftysix] "r" (256), [hwColour] "r" (hwColour), [scratch] "r" (scratch)
        : "q2", "q4", "q5", "q6", "q7", "memory" );

        phwSource += (iSourceStride);
        phwTarget += (iTargetStride);
    }
#endif
}




__OVERRIDE_WEAK
void __arm_2d_impl_cccn888_alpha_blending(   uint32_t *pwSourceBase,
                                            int16_t iSourceStride,
                                            uint32_t *pwTargetBase,
                                            int16_t iTargetStride,
                                            arm_2d_size_t *ptCopySize,
                                            uint_fast8_t chRatio)
{
#ifdef USE_MVE_INTRINSICS
    uint16_t        chRatioCompl = 256 - (uint16_t) chRatio;
    int32_t         blkCnt;
    int32_t         row = ptCopySize->iHeight;

    while (row > 0) {

        const uint32_t *pwSource = pwSourceBase;
        uint32_t       *pwTarget = pwTargetBase;
        /* byte extraction into 16-bit vector */
        uint16x8_t      vecSrc = vldrbq_u16(pwSource);
        uint16x8_t      vecTrg = vldrbq_u16(pwTarget);

        pwSource += 2;
        blkCnt = ptCopySize->iWidth;

        while (blkCnt > 0) {
            vstrbq_u16(pwTarget,
                       vmlaq(vmulq(vecSrc, chRatio), vecTrg, chRatioCompl) >> 8);

            pwTarget += 2;

            vecSrc = vldrbq_u16(pwSource);
            vecTrg = vldrbq_u16(pwTarget);
            pwSource += 2;
            blkCnt -= 2;
        }

        pwSourceBase += iSourceStride;
        pwTargetBase += iTargetStride;
        row--;
    }
#else
    uint16_t        chRatioCompl = 256 - (uint16_t) chRatio;
    register unsigned blkCnt  __asm("lr");
    int32_t row = ptCopySize->iHeight;

    while(row > 0)
    {
        blkCnt = ptCopySize->iWidth*4;
        const uint32_t *pwSource = pwSourceBase;
        uint32_t *pwTarget = pwTargetBase;

        __asm volatile(
            "   vldrb.u16               q0, [%[pwSource]], #8          \n"
            "   vldrb.u16               q1, [%[pwTarget]]              \n"

            "   wlstp.16                lr, %[loopCnt], 1f             \n"
            "2:                                                        \n"
            "   vmul.u16                q2, q0, %[chRatio]             \n"
            "   vldrb.u16               q0, [%[pwSource]], #8          \n"
            "   vmla.u16                q2, q1, %[chRatioCompl]        \n"
            "   vldrb.u16               q1, [%[pwTarget], #8]          \n"
            "   vshr.u16                q2, q2, #8                     \n"
            "   vstrb.16                q2, [%[pwTarget]], #8          \n"
            "   letp                    lr, 2b                         \n"
            "1:                                                        \n"

            : [pwSource] "+l"(pwSource),  [pwTarget] "+l"(pwTarget),
              [loopCnt] "+r"(blkCnt)
            : [chRatio] "r" (chRatio), [chRatioCompl] "r" (chRatioCompl)
            : "q0", "q1", "q2", "memory" );

        pwSourceBase += iSourceStride;
        pwTargetBase += iTargetStride;
        row--;
    }
#endif
}




__OVERRIDE_WEAK
void __arm_2d_impl_cccn888_colour_filling_with_opacity(
                                        uint32_t *__RESTRICT pTargetBase,
                                        int16_t iTargetStride,
                                        arm_2d_size_t *__RESTRICT ptCopySize,
                                        uint32_t Colour,
                                        uint_fast8_t chRatio)
{
#ifdef USE_MVE_INTRINSICS
    uint16_t        chRatioCompl = 256 - (uint16_t) chRatio;
    int32_t         blkCnt;
    int32_t         row = ptCopySize->iHeight;
    uint32_t        scratch[2];
    uint16x8_t      vColor;

    scratch[0] = scratch[1] = Colour;
    vColor = vldrbq_u16((uint8_t *) scratch);
    vColor = vColor * (uint16_t)chRatio;

    while (row > 0) {
        uint32_t       *pTarget = pTargetBase;
        blkCnt = ptCopySize->iWidth;

        while (blkCnt > 0) {
            /* byte extraction into 16-bit vector */
            uint16x8_t      vecTrg = vldrbq_u16((uint8_t *)pTarget);

            vstrbq_u16((uint8_t *)pTarget, vmlaq(vColor, vecTrg, chRatioCompl) >> 8);

            pTarget += 2;
            blkCnt -= 2;
        }
        pTargetBase += iTargetStride;
        row--;
    }
#else /* USE_MVE_INTRINSICS  */

    uint16_t        chRatioCompl = 256 - (uint16_t) chRatio;
    int32_t         blkCnt;
    int32_t         row = ptCopySize->iHeight;
    uint32_t        scratch[2];
    uint16x8_t      vColor;

    scratch[0] = scratch[1] = Colour;
    vColor = vldrbq_u16((uint8_t *) scratch);
    vColor = vColor * (uint16_t)chRatio;

    while (row > 0) {
        uint32_t       *pTarget = pTargetBase;
        blkCnt = ptCopySize->iWidth*4;

    __asm volatile(
        /* preload */
        "   vldrb.u16               q1, [%[pTarget]]               \n"

        "   wlstp.16                lr, %[loopCnt], 1f             \n"
        ".p2align 2                                                \n"
        "2:                                                        \n"
        "   vmov                    q2, %[vColor]                  \n"
        "   vmla.u16                q2, q1, %[chRatioCompl]        \n"
        "   vldrb.u16               q1, [%[pTarget], #8]           \n"
        "   vshr.u16                q2, q2, #8                     \n"
        "   vstrb.16                q2, [%[pTarget]], #8           \n"
        "   letp                    lr, 2b                         \n"
        "1:                                                        \n"
        : [pTarget] "+l"(pTarget)
        : [loopCnt] "r"(blkCnt), [chRatioCompl] "r" (chRatioCompl), [vColor] "t" (vColor)
        : "q0", "q1", "q2", "memory" );

        pTargetBase += iTargetStride;
        row--;
    }

#endif /* USE_MVE_INTRINSICS */
}

__OVERRIDE_WEAK
void __arm_2d_impl_cccn888_alpha_blending_colour_keying(uint32_t * __RESTRICT pSourceBase,
                                                       int16_t iSourceStride,
                                                       uint32_t * __RESTRICT pTargetBase,
                                                       int16_t iTargetStride,
                                                       arm_2d_size_t *
                                                       __RESTRICT ptCopySize,
                                                       uint_fast8_t chRatio,
                                                       uint32_t Colour)
{
    int_fast16_t    iHeight = ptCopySize->iHeight;
    int_fast16_t    iWidth = ptCopySize->iWidth;
    uint16_t        chRatioCompl = 256 - chRatio;


    for (int_fast16_t y = 0; y < iHeight; y++) {
        const uint32_t *pSource = pSourceBase;
        uint32_t       *pTarget = pTargetBase;

#ifdef USE_MVE_INTRINSICS
        int32_t         blkCnt = iWidth;
        uint16x8_t      vecSrc, vecTrg;

        do {
            mve_pred16_t    p = vctp32q(blkCnt);

            uint8x16_t      vSrc8 = vld1q_z(pSource, p);
            uint8x16_t      vTrg8 = vld1q_z(pTarget, p);

            /* 16-bit expansion A/G src pixels */
            uint16x8_t      vSrc16b = vmovlbq_x(vSrc8, p);
            /* 16-bit expansion R/B src pixels */
            uint16x8_t      vSrc16t = vmovltq_x(vSrc8, p);

            /* 16-bit expansion A/G target pixels */
            uint16x8_t      vTrg16b = vmovlbq_x(vTrg8, p);
            /* 16-bit expansion R/B target pixels */
            uint16x8_t      vTrg16t = vmovltq_x(vTrg8, p);

            /* A/G blending */
            int16x8_t       vecOutb = vmlaq_m(vmulq_x(vSrc16b, chRatio, p), vTrg16b, chRatioCompl, p);
            /* R/B blending */
            int16x8_t       vecOutt = vmlaq_m(vmulq_x(vSrc16t, chRatio, p), vTrg16t, chRatioCompl, p);

            /* merge into 8-bit vector */
            int8x16_t       vecOut8 = vuninitializedq_s8();

            vecOut8 = vqshrnbq_m_n_s16(vecOut8, vecOutb, 8, p);
            vecOut8 = vqshrntq_m_n_s16(vecOut8, vecOutt, 8, p);

            // update if (*pSourceBase != Colour)
            vst1q_p_u32((uint8_t *) pTarget, (uint32x4_t) vecOut8,
                        vcmpneq_m_n_u32((uint32x4_t) vSrc8, Colour, p));

            pSource += 4;
            pTarget += 4;
            blkCnt -= 4;
        }
        while (blkCnt > 0);

#else // USE_MVE_INTRINSICS

    __asm volatile (
        ".p2align 2                                 \n"
        /* preload uint32x4_t target vector */
        "   vldrw.u32       q2, [%[targ]]           \n"

        "   wlstp.32        lr, %[loopCnt], 1f      \n"
        "2:                                         \n"
        /* 16-bit expansion A/G target pixels */
        "   vmovlb.u8       q3, q2                  \n"
        "   vldrw.u32       q0, [%[src]], #16       \n"
        /* 16-bit expansion A/G source pixels */
        "   vmovlb.u8       q1, q0                  \n"
        "   vmul.i16        q1, q1, %[ratio]        \n"
        /* 16-bit expansion R/B target pixels */
        "   vmovlt.u8       q2, q2                  \n"
        /* A/G blending */
        "   vmla.u16        q1, q3, %[ratioCmp]     \n"
        /* 16-bit expansion R/B source pixels */
        "   vmovlt.u8       q3, q0                  \n"
        "   vmul.i16        q3, q3, %[ratio]        \n"
        /* merge A/G into 8-bit vector */
        "   vqshrnb.s16     q1, q1, #8              \n"
        /* R/B blending */
        "   vmla.u16        q3, q2, %[ratioCmp]     \n"
        /* preload next target */
        "   vldrw.u32       q2, [%[targ], #16]      \n"
        /* merge R/B into 8-bit vector */
        "   vqshrnt.s16     q1, q3, #8              \n"
        /* update if (*pSourceBase != Colour) */
        "   vpt.i32         ne, q0, %[color]        \n"
        "   vstrwt.32       q1, [%[targ]], #16      \n"
        "   letp            lr, 2b                  \n"
        "1:                                         \n"
        :[targ] "+r" (pTarget), [src] "+r" (pSource)
        :[loopCnt] "r" (iWidth), [ratio] "r" (chRatio),
         [ratioCmp] "r" (chRatioCompl), [color] "r" (Colour)
        :"r14", "q0", "q1", "q2", "q3", "memory");
#endif
        pSourceBase += (iSourceStride);
        pTargetBase += (iTargetStride);
    }
}



__OVERRIDE_WEAK
void __arm_2d_impl_rgb565_alpha_blending_direct(const uint16_t *phwSource,
                                                const uint16_t *phwBackground,
                                                uint16_t *phwDestination,
                                                uint32_t wPixelCount,
                                                uint_fast8_t chRatio)
{
#ifdef USE_MVE_INTRINSICS
    int32_t         blkCnt;
    uint16_t        ratio1x8 = chRatio * 8;
    uint16_t        ratio1x4 = chRatio * 4;
    uint16_t        ratio2x8 = (256 - chRatio) * 8;
    uint16_t        ratio2x4 = (256 - chRatio) * 4;

    uint16x8_t      vecMaskR = vdupq_n_u16(0x001f);
    uint16x8_t      vecMaskG = vdupq_n_u16(0x003f);
    uint16x8_t      vecMaskBpck = vdupq_n_u16(0x00f8);
    uint16x8_t      vecMaskGpck = vdupq_n_u16(0x00fc);

    blkCnt = wPixelCount;
    do {

        uint16x8_t      vecIn;
        uint16x8_t      vecR0, vecB0, vecG0;
        uint16x8_t      vecR1, vecB1, vecG1;

        /* unpack 1st stream */
        vecIn = vld1q(phwSource);
        phwSource += 8;

        vecR0 = vecIn & vecMaskR;

        vecB0 = vecIn >> 11;

        vecG0 = vecIn >> 5;
        vecG0 = vecG0 & vecMaskG;


        /* unpack 2nd stream */
        vecIn = vld1q(phwBackground);
        phwBackground += 8;

        vecR1 = vecIn & vecMaskR;

        vecB1 = vecIn >> 11;

        vecG1 = vecIn >> 5;
        vecG1 = vecG1 & vecMaskG;


        /* merge */
        vecR0 = vecR0 * ratio1x8 + vecR1 * ratio2x8;
        vecR0 = vecR0 >> 8;

        vecG0 = vecG0 * ratio1x4 + vecG1 * ratio2x4;
        vecG0 = vecG0 >> 8;

        vecB0 = vecB0 * ratio1x8 + vecB1 * ratio2x8;
        vecB0 = vecB0 >> 8;


        /* pack */
        uint16x8_t      vOut =
            vecR0 >> 3 | vmulq((vecG0 & vecMaskGpck), 8)
            | vmulq((vecB0 & vecMaskBpck), 256);

        vst1q(phwDestination, vOut);
        phwDestination += 8;

        blkCnt -= 8;
    }
    while (blkCnt > 0);

#else /* USE_MVE_INTRINSICS */

    uint16_t        ratio1x8 = chRatio * 8;
    uint16_t        ratio1x4 = chRatio * 4;
    uint16_t        ratio2x8 = (256 - chRatio) * 8;
    uint16_t        ratio2x4 = (256 - chRatio) * 4;
    uint16x8_t      vecMaskR = vdupq_n_u16(0x001f);
    uint16x8_t      vecMaskG = vdupq_n_u16(0x003f);
    uint16x8_t      vecMaskBpck = vdupq_n_u16(0x00f8);

     register unsigned loopCnt  __asm("lr") = (wPixelCount);

    __asm volatile(

        "   vldrh.u16               q4, [%[in2]], #16            \n"
        "   vmov.i16                q6, #0x00fc                  \n"
        "   vstrw.32                q6, [sp]                     \n"
        "   vldrh.u16               q5, [%[in1]], #16            \n"
        "   wlstp.16                lr, %[loopCnt], 1f           \n"
        "2:                                                      \n"

        "   vand                    q6, q4, %[vecMaskR]          \n"
        "   vmul.i16                q6, q6, %[ratio2x8]          \n"
        "   vshr.u16                q2, q4, #5                   \n"
        "   vand                    q7, q5, %[vecMaskR]          \n"
        "   vmla.u16                q6, q7, %[ratio1x8]          \n"
        "   vand                    q2, q2, %[vecMaskG]          \n"
        "   vshr.u16                q7, q5, #5                   \n"
        "   vmul.i16                q2, q2, %[ratio2x4]          \n"
        "   vand                    q7, q7, %[vecMaskG]          \n"
        "   vmla.u16                q2, q7, %[ratio1x4]          \n"
        "   vshr.u16                q4, q4, #11                  \n"
        "   vmul.i16                q7, q4, %[ratio2x8]          \n"
        "   vshr.u16                q5, q5, #11                  \n"
        "   vshr.u16                q2, q2, #8                   \n"
        "   vmla.u16                q7, q5, %[ratio1x8]          \n"

        //  "   vmov.i16                 q6, #0x00fc  \n"
        "   vshr.u16                q7, q7, #8                   \n"
        //  "   vmov.i16                 q6, #0x00fc  \n"
        /* load 0x00fc instead of mov for better overlap opportunity */
        "   vldrw.u32               q4, [sp]                     \n"
        "   vand                    q2, q2, q4                   \n"
        "   vmul.i16                q2, q2, %[eight]             \n"
        "   vand                    q4, q7, %[vecMaskBpck]       \n" // Q7 = vecB0
        "   vldrh.u16               q5, [%[in1]], #16            \n"
        "   vmla.u16                q2, q4, %[twofiftysix]       \n"
        // (vecR0 >> 3) >> 8
        "   vshr.u16                q6, q6, #11                  \n"
        "   vldrh.u16               q4, [%[in2]], #16            \n"
        "   vorr                    q2, q2, q6                   \n"
        "   vstrh.16                q2, [%[out]], #16            \n"
        "   letp                    lr, 2b                       \n"
        "1:                                                      \n"

        : [in1] "+r"(phwSource),  [in2] "+r"(phwBackground),
          [out] "+r" (phwDestination), [loopCnt] "+r"(loopCnt)
        : [vecMaskR] "t" (vecMaskR), [vecMaskG] "t" (vecMaskG),
          [vecMaskBpck] "t" (vecMaskBpck),
          [ratio1x8] "r" (ratio1x8), [ratio2x8] "r" (ratio2x8),
          [ratio1x4] "r" (ratio1x4), [ratio2x4] "r" (ratio2x4),
          [eight] "r" (8), [twofiftysix] "r" (256)
        : "q2", "q4", "q5", "q6", "q7", "memory" );

#endif
}

__OVERRIDE_WEAK
void __arm_2d_impl_cccn888_alpha_blending_direct(const uint32_t *pwSource,
                                                const uint32_t *pwBackground,
                                                uint32_t *pwDestination,
                                                uint32_t wPixelCount,
                                                uint_fast8_t chRatio)
{
#ifdef USE_MVE_INTRINSICS
    int32_t         blkCnt;
    uint16_t        chRatioCompl = 256 - (uint16_t) chRatio;
    uint16x8_t      vecSrc, vecBckg;


    vecSrc = vldrbq_u16((uint8_t const *) pwSource);
    pwSource += 2;
    vecBckg = vldrbq_u16((uint8_t const *) pwBackground);
    pwBackground += 2;


    blkCnt = wPixelCount;
    do {
        uint16x8_t      vecOut;

        vecOut = vmulq_n_u16(vecSrc, (uint16_t) chRatio);
        vecSrc = vldrbq_u16((uint8_t const *) pwSource);
        pwSource += 2;

        vecOut = vmlaq_n_u16(vecOut, vecBckg, chRatioCompl);
        vecBckg = vldrbq_u16((uint8_t const *) pwBackground);
        pwBackground += 2;

        vecOut = vecOut >> 8;

        vstrbq_u16((uint8_t *) pwDestination, vecOut);
        pwDestination += 2;

        blkCnt -= 2;
    }
    while (blkCnt > 0);

#else /* USE_MVE_INTRINSICS */
    uint16_t        chRatioCompl = 256 - (uint16_t) chRatio;
    register unsigned blkCnt  __asm("lr") = (wPixelCount * 4);

    __asm volatile(
        "   vldrb.u16               q0, [%[pwSource]], #8          \n"
        "   vldrb.u16               q1, [%[pwBackg]], #8           \n"

        "   wlstp.16                lr, %[loopCnt], 1f             \n"
        "2:                                                        \n"
        "   vmul.u16                q2, q0, %[chRatio]             \n"
        "   vldrb.u16               q0, [%[pwSource]], #8          \n"
        "   vmla.u16                q2, q1, %[chRatioCompl]        \n"
        "   vldrb.u16               q1, [%[pwBackg]], #8           \n"
        "   vshr.u16                q2, q2, #8                     \n"
        "   vstrb.16                q2, [%[pwDest]], #8            \n"
        "   letp                    lr, 2b                         \n"
        "1:                                                        \n"

        : [pwSource] "+l"(pwSource),  [pwBackg] "+l"(pwBackground),
          [pwDest] "+l" (pwDestination), [loopCnt] "+r"(blkCnt)
        : [chRatio] "r" (chRatio), [chRatioCompl] "r" (chRatioCompl)
        : "q0", "q1", "q2", "memory" );
#endif

}


static
mve_pred16_t arm_2d_is_point_vec_inside_region_s16(const arm_2d_region_t * ptRegion,
                                               const arm_2d_point_s16x8_t * ptPoint)
{
    mve_pred16_t    p0 = vcmpgeq(ptPoint->X, ptRegion->tLocation.iX);
    p0 = vcmpgeq_m(ptPoint->Y, ptRegion->tLocation.iY, p0);
    p0 = vcmpltq_m(ptPoint->X, ptRegion->tLocation.iX + ptRegion->tSize.iWidth, p0);
    p0 = vcmpltq_m(ptPoint->Y, ptRegion->tLocation.iY + ptRegion->tSize.iHeight, p0);

    return p0;
}

static
mve_pred16_t arm_2d_is_point_vec_inside_region_s32(const arm_2d_region_t * ptRegion,
                                               const arm_2d_point_s32x4_t * ptPoint)
{
    mve_pred16_t    p0 = vcmpgeq_n_s32(ptPoint->X, ptRegion->tLocation.iX);
    p0 = vcmpgeq_m_n_s32(ptPoint->Y, ptRegion->tLocation.iY, p0);
    p0 = vcmpltq_m_n_s32(ptPoint->X, ptRegion->tLocation.iX + ptRegion->tSize.iWidth, p0);
    p0 = vcmpltq_m_n_s32(ptPoint->Y, ptRegion->tLocation.iY + ptRegion->tSize.iHeight, p0);

    return p0;
}


/**
  @brief         return 3 vector of 16-bit channels (8-bit widened) taken from a memory reference
  @param[in]     pMem           pointer to packed 8-bit channel
  @param[out]    R              vector of 16-bit widened R channel
  @param[out]    G              vector of 16-bit widened G channel
  @param[out]    B              vector of 16-bit widened B channel
 */
void __arm_2d_unpack_rgb888_from_mem(const uint8_t * pMem, uint16x8_t * R, uint16x8_t * G,
                                     uint16x8_t * B)
{
    uint16x8_t      sg = vidupq_n_u16(0, 4);

    *R = vldrbq_gather_offset_u16(pMem, sg);
    *G = vldrbq_gather_offset_u16(pMem + 1, sg);
    *B = vldrbq_gather_offset_u16(pMem + 2, sg);
}

/**
  @brief         interleave 3 x 16-bit widened vectors into 8-bit memory reference
                 (4th channel untouched)
  @param[in]     pMem           pointer to packed 8-bit channel
  @param[in]     R              vector of 16-bit widened R channel
  @param[in]     G              vector of 16-bit widened G channel
  @param[in]     B              vector of 16-bit widened B channel
 */
void __arm_2d_pack_rgb888_to_mem(uint8_t * pMem, uint16x8_t R, uint16x8_t G, uint16x8_t B)
{
    uint16x8_t      sg = vidupq_n_u16(0, 4);

    vstrbq_scatter_offset_u16(pMem, sg, R);
    vstrbq_scatter_offset_u16(pMem + 1, sg, G);
    vstrbq_scatter_offset_u16(pMem + 2, sg, B);
    //vstrbq_scatter_offset_u16(pMem + 3, sg, vdupq_n_u16(0));
}


/**
  unpack vectors of pixels read from a input 2D coordinates if fits inside the region of
  interest or alternative target pixel if content matches color mask
  (vector of packed pixels & region of interest name implicit and fixed to respectively
  vTarget and ptOrigValidRegion)
  Update global predictor tracking region fit & color mask comparison.
 */

#define __ARM_2D_RGB565_GET_RGBVEC_FROM_POINT(vecX, vecY, R, G, B, MaskColour, pGlb)                       \
        arm_2d_point_s16x8_t vPoint = {.X = vecX,.Y = vecY };                                              \
        /* set vector predicate if point is inside the region */                                           \
        mve_pred16_t    p =                                                                                \
            arm_2d_is_point_vec_inside_region_s16(ptOrigValidRegion, &vPoint);                             \
        pGlb |= p;                                                                                         \
                                                                                                           \
        /* prepare vector of point offsets */                                                              \
        uint16x8_t      ptOffs = vPoint.X + vPoint.Y * iOrigStride;                                        \
        /* retrieve all point values */                                                                    \
        uint16x8_t      ptVal =                                                                            \
            vldrhq_gather_shifted_offset_z_u16(pOrigin, ptOffs, predTail);                                 \
                                                                                                           \
        /* combine 2 predicates set to true if point is in the region & values different from color mask */\
        p = vcmpneq_m_n_u16(ptVal, MaskColour, p);                                                         \
        pGlb |= p;                                                                                         \
        ptVal = vpselq_u16(ptVal, vTarget, p);                                                             \
                                                                                                           \
        /* expand channels */                                                                              \
        __arm_2d_rgb565_unpack_single_vec(ptVal, &R, &G, &B);


/**
  Same as above but use offset compensation during gather load.
  unpack vectors of pixels read from a input 2D coordinates if fits inside the region of
  interest or alternative target pixel if content matches color mask
  (vector of packed pixels & region of interest name implicit and fixed to respectively
  vTarget and ptOrigValidRegion)
  Update global predictor tracking region fit & color mask comparison.
 */

#define __ARM_2D_RGB565_GET_RGBVEC_FROM_POINT_FAR(vecX, vecY, R, G, B, MaskColour, pGlb)                   \
        arm_2d_point_s16x8_t vPoint = {.X = vecX,.Y = vecY };                                              \
        /* set vector predicate if point is inside the region */                                           \
        mve_pred16_t    p =                                                                                \
            arm_2d_is_point_vec_inside_region_s16(ptOrigValidRegion, &vPoint);                             \
        pGlb |= p;                                                                                         \
        /* prepare vector of point offsets */                                                              \
        int16_t         correctionOffset = vminvq_s16(0x7fff, vPoint.Y) - 1;                               \
        uint16x8_t      ptOffs = vPoint.X + (vPoint.Y - correctionOffset) * iOrigStride;                   \
                                                                                                           \
        /* base pointer update to compensate offset */                                                     \
        uint16_t       *pOriginCorrected = pOrigin + (correctionOffset * iOrigStride);                     \
        /* retrieve all point values */                                                                    \
        uint16x8_t      ptVal =                                                                            \
            vldrhq_gather_shifted_offset_z_u16(pOriginCorrected, ptOffs, predTail);                        \
                                                                                                           \
        /* combine 2 predicates set to true if point is in the region & values different from color mask */\
        p = vcmpneq_m_n_u16(ptVal, MaskColour, p);                                                         \
        pGlb |= p;                                                                                         \
        ptVal = vpselq_u16(ptVal, vTarget, p);                                                             \
                                                                                                           \
        /* expand channels */                                                                              \
        __arm_2d_rgb565_unpack_single_vec(ptVal, &R, &G, &B);


/**
  unpack vectors of 32-bit pixels read from a input 2D coordinates if fits inside the region of
  interest or alternative target pixel if content matches color mask
  16-bit vector processed in 2 parts because of 32-bit requirements, so handles 8 x 32-bit vectors
  (vectors of packed pixels & region of interest name implicit and fixed to respectively
  vTargetLo, vectorHi and ptOrigValidRegion)
  Update 2 global predictors tracking region fit & color mask comparison for 1st and 2nd half.
 */

#define __ARM_2D_RGB888_GET_RGBVEC_FROM_POINT(vecX, vecY, R, G, B, MaskColour, pGlbLo, pGlbHi)             \
        arm_2d_point_s16x8_t vPoint = {.X = vecX,.Y = vecY };                                              \
        arm_2d_point_s32x4_t tPointLo, tPointHi;                                                           \
                                                                                                           \
        /* split 16-bit point vector into 2 x 32-bit vectors */                                            \
        vst1q(pscratch16, vPoint.X);                                                                       \
        tPointLo.X = vldrhq_s32(pscratch16);                                                               \
        tPointHi.X = vldrhq_s32(pscratch16 + 4);                                                           \
                                                                                                           \
        vst1q(pscratch16, vPoint.Y);                                                                       \
        tPointLo.Y = vldrhq_s32(pscratch16);                                                               \
        tPointHi.Y = vldrhq_s32(pscratch16 + 4);                                                           \
                                                                                                           \
        /* 1st half */                                                                                     \
                                                                                                           \
        /* set vector predicate if point is inside the region */                                           \
        mve_pred16_t    p = arm_2d_is_point_vec_inside_region_s32(ptOrigValidRegion, &tPointLo);           \
        /* prepare vector of point offsets */                                                              \
        uint32x4_t      ptOffs = tPointLo.X + tPointLo.Y * iOrigStride;                                    \
                                                                                                           \
        /* retrieve all point values */                                                                    \
        uint32x4_t      ptVal = vldrwq_gather_shifted_offset_z_u32(pOrigin, ptOffs, predTailLow);          \
                                                                                                           \
        /* combine 2 predicates set to true if point is in the region & values different from color mask */\
        p = vcmpneq_m_n_u32(ptVal, MaskColour, p);                                                         \
        pGlbLo |= p;                                                                                       \
        ptVal = vpselq_u32(ptVal, vTargetLo, p);                                                           \
                                                                                                           \
        vst1q(scratch32, ptVal);                                                                           \
                                                                                                           \
        /* 2nd half */                                                                                     \
                                                                                                           \
        /* set vector predicate if point is inside the region */                                           \
        p = arm_2d_is_point_vec_inside_region_s32(ptOrigValidRegion, &tPointHi);                           \
        /* prepare vector of point offsets */                                                              \
        ptOffs = tPointHi.X + tPointHi.Y * iOrigStride;                                                    \
                                                                                                           \
        /* retrieve all point values */                                                                    \
        ptVal = vldrwq_gather_shifted_offset_z_u32(pOrigin, ptOffs, predTailHigh);                         \
                                                                                                           \
        /* combine 2 predicates set to true if point is in the region & values different from color mask */\
        p = vcmpneq_m_n_u32(ptVal, MaskColour, p);                                                         \
        pGlbHi |= p;                                                                                       \
        ptVal = vpselq_u32(ptVal, vTargetHi, p);                                                           \
                                                                                                           \
        vst1q(scratch32 + 4, ptVal);                                                                       \
                                                                                                           \
        /* expand channels */                                                                              \
        __arm_2d_unpack_rgb888_from_mem((uint8_t *) scratch32, &R, &G, &B);


/**
  Alpha blending of a packed vector with 3 vectors of single R, G & B channels
 */

#define __ARM_2D_BLEND_RGB565_TARGET_RGBVEC(chOpacity, vPackedTarget, vAvgR, vAvgG, vAvgB, vBlended)       \
    uint16x8_t      vTargetR, vTargetG, vTargetB;                                                          \
                                                                                                           \
    __arm_2d_rgb565_unpack_single_vec(vTarget, &vTargetR, &vTargetG, &vTargetB);                           \
                                                                                                           \
    uint16_t        chOpacityCompl = (256 - chOpacity);                                                    \
                                                                                                           \
    /* merge */                                                                                            \
    vAvgR = vAvgR * chOpacityCompl + vTargetR * chOpacity;                                                 \
    vAvgR = vAvgR >> 8;                                                                                    \
                                                                                                           \
    vAvgG = vAvgG * chOpacityCompl + vTargetG * chOpacity;                                                 \
    vAvgG = vAvgG >> 8;                                                                                    \
                                                                                                           \
    vAvgB = vAvgB * chOpacityCompl + vTargetB * chOpacity;                                                 \
    vAvgB = vAvgB >> 8;                                                                                    \
                                                                                                           \
    vBlended = __arm_2d_rgb565_pack_single_vec(vAvgR, vAvgG, vAvgB);


#if     __ARM_2D_HAS_HELIUM_FLOAT__                                             \
    && !__ARM_2D_CFG_FORCED_FIXED_POINT_ROTATION__

#define __CALIB             0.009f16

/**
  Scale R, G & B channels
 */
#define __ARM_2D_SCALE_RGBVEC(vAvgPixelR, vAvgPixelG, vAvgPixelB, R, G, B, vScal)                           \
        vAvgPixelR = vScal * vcvtq_f16_s16(R);                                                              \
        vAvgPixelG = vScal * vcvtq_f16_s16(G);                                                              \
        vAvgPixelB = vScal * vcvtq_f16_s16(B);

/**
  Scale R, G & B channels with accumulation
 */

#define __ARM_2D_SCALE_RGBVEC_ACC(vAvgPixelR, vAvgPixelG, vAvgPixelB, R, G, B, vScal)                       \
        vAvgPixelR += vScal * vcvtq_f16_s16(R);                                                             \
        vAvgPixelG += vScal * vcvtq_f16_s16(G);                                                             \
        vAvgPixelB += vScal * vcvtq_f16_s16(B);


static
bool __arm_2d_rotate_regression(arm_2d_size_t * __RESTRICT ptCopySize,
                                    arm_2d_location_t * pSrcPoint,
                                    float fAngle,
                                    arm_2d_location_t * tOffset,
                                    arm_2d_location_t * center,
                                    int32_t             iOrigStride,
                                    arm_2d_rot_linear_regr_t regrCoefs[]
    )
{
    int32_t    iHeight = ptCopySize->iHeight;
    int32_t    iWidth = ptCopySize->iWidth;
    float           invHeight = iHeight > 1 ? 1.0f / (float) (iHeight - 1) : __LARGEINVF32;
    arm_2d_point_s32x4_t vPointCornerI;
    int32x4_t       vCornerX = { 0, 1, 0, 1 };
    int32x4_t       vCornerY = { 0, 0, 1, 1 };
    float           cosAngle = arm_cos_f32(fAngle);
    float           sinAngle = arm_sin_f32(fAngle);
    arm_2d_point_float_t centerf;
    float           slopeX, slopeY;
    bool            gatherLoadIdxOverflow = 0;

    //printf("invHeight %f\n",invHeight);


    centerf.fX = (float) center->iX;
    centerf.fY = (float) center->iY;

    vPointCornerI.X = vdupq_n_s32(pSrcPoint->iX + tOffset->iX);
    vPointCornerI.X = vPointCornerI.X + vmulq_n_s32(vCornerX, (iWidth - 1));

    vPointCornerI.Y = vdupq_n_s32(pSrcPoint->iY + tOffset->iY);
    vPointCornerI.Y = vPointCornerI.Y + vmulq_n_s32(vCornerY, (iHeight - 1));

    /*
        Vector version of:

        int16_t         iX = ptLocation->iX - ptCenter->iX;
        int16_t         iY = ptLocation->iY - ptCenter->iY;

        float           cosAngle = arm_cos_f32(fAngle);
        float           sinAngle = arm_sin_f32(fAngle);

        ptOutBuffer->fY = (iY * cosAngle + iX * sinAngle + ptCenter->iY);
        ptOutBuffer->fX = (-iY * sinAngle + iX * cosAngle + ptCenter->iX);
    */

    arm_2d_point_f32x4_t vTmp, vPointCornerF;

    vTmp.X = vsubq_n_f32(vcvtq_f32_s32(vPointCornerI.X), centerf.fX);
    vTmp.Y = vsubq_n_f32(vcvtq_f32_s32(vPointCornerI.Y), centerf.fY);

    vPointCornerF.X = vmulq_n_f32(vTmp.X, cosAngle) - vmulq_n_f32(vTmp.Y, sinAngle);
    vPointCornerF.X = vaddq_n_f32(vPointCornerF.X, centerf.fX);

    vPointCornerF.Y = vmulq_n_f32(vTmp.X, sinAngle) + vmulq_n_f32(vTmp.Y, cosAngle);
    vPointCornerF.Y = vaddq_n_f32(vPointCornerF.Y, centerf.fY);

    /*
       Check whether rotated index offsets could exceed 16-bit limits
       used in subsequent gather loads
       This will occur for parts of large images (e.g. 320*200)
       To avoid unconditional penalties for small/medium images,
       returns a speculative overflow allowing to handle large offsets.
    */
    float32_t maxY = vmaxnmvq(0.0f, vPointCornerF.Y);

    if((iOrigStride * maxY) > (float)(UINT16_MAX))
        gatherLoadIdxOverflow = true;


    /* interpolation in Y direction for 1st elements column */
    slopeX = (vPointCornerF.X[2] - vPointCornerF.X[0]) * invHeight;
    slopeY = (vPointCornerF.Y[2] - vPointCornerF.Y[0]) * invHeight;

    regrCoefs[0].slopeY = slopeY;
    regrCoefs[0].slopeX = slopeX;
    regrCoefs[0].interceptY = vPointCornerF.Y[0];
    regrCoefs[0].interceptX = vPointCornerF.X[0];


    /* interpolation in Y direction for the last elements column */
    slopeX = (vPointCornerF.X[3] - vPointCornerF.X[1]) * invHeight;
    slopeY = (vPointCornerF.Y[3] - vPointCornerF.Y[1]) * invHeight;

    regrCoefs[1].slopeY = slopeY;
    regrCoefs[1].slopeX = slopeX;
    regrCoefs[1].interceptY = vPointCornerF.Y[1];
    regrCoefs[1].interceptX = vPointCornerF.X[1];

    return gatherLoadIdxOverflow;
}



static
void __arm_2d_impl_rgb565_get_pixel_colour(arm_2d_point_f16x8_t * ptPoint,
                                           arm_2d_region_t * ptOrigValidRegion,
                                           uint16_t * pOrigin,
                                           int16_t iOrigStride,
                                           uint16_t * pTarget,
                                           uint16_t MaskColour, uint32_t elts)
{
    mve_pred16_t    predTail = vctp16q(elts);
    uint16x8_t      vTarget = vld1q(pTarget);

#if     defined(__ARM_2D_HAS_INTERPOLATION_ROTATION__) &&  __ARM_2D_HAS_INTERPOLATION_ROTATION__
    float16x8_t     vOne = vdupq_n_f16(1.0f);
    int16x8_t       vXi = vcvtq_s16_f16(ptPoint->X);
    int16x8_t       vYi = vcvtq_s16_f16(ptPoint->Y);

    vXi = vsubq_m_n_s16(vXi, vXi, 1, vcmpltq_n_f16(ptPoint->X, 0));
    vYi = vsubq_m_n_s16(vYi, vYi, 1, vcmpltq_n_f16(ptPoint->Y, 0));

    float16x8_t     vWX = ptPoint->X - vcvtq_f16_s16(vXi);
    float16x8_t     vWY = ptPoint->Y - vcvtq_f16_s16(vYi);


    /* combination of Bottom / Top & Left / Right areas contributions */
    float16x8_t     vAreaTR = vWX * vWY;
    float16x8_t     vAreaTL = (vOne - vWX) * vWY;
    float16x8_t     vAreaBR = vWX * (vOne - vWY);
    float16x8_t     vAreaBL = (vOne - vWX) * (vOne - vWY);

    /* accumulated pixel vectors */
    float16x8_t     vAvgPixelR, vAvgPixelG, vAvgPixelB;

    /* predicate accumulator */
    /* tracks all predications conditions for selecting final */
    /* averaged pixed / target pixel */
    /* can probably optimized away since averaged target pixel is */
    /* equivalent to original pixel, but precision limitations would introduce small errors */
    mve_pred16_t    predGlb = 0;

    /*
     * accumulate / average over the 4 neigbouring pixels
     */
    uint16x8_t      R, G, B;
    /* Bottom Left averaging */
    {
        __ARM_2D_RGB565_GET_RGBVEC_FROM_POINT(vXi, vYi, R, G, B, MaskColour, predGlb);

        __ARM_2D_SCALE_RGBVEC(vAvgPixelR, vAvgPixelG, vAvgPixelB, R, G, B, vAreaBL);
    }


    /* Bottom Right averaging */
    {
        __ARM_2D_RGB565_GET_RGBVEC_FROM_POINT(vaddq_n_s16(vXi, 1), vYi, R, G, B, MaskColour, predGlb);

        __ARM_2D_SCALE_RGBVEC_ACC(vAvgPixelR, vAvgPixelG, vAvgPixelB, R, G, B, vAreaBR);
    }

    /* Top Left averaging */
    {
        __ARM_2D_RGB565_GET_RGBVEC_FROM_POINT(vXi, vaddq_n_s16(vYi, 1), R, G, B, MaskColour, predGlb);

        __ARM_2D_SCALE_RGBVEC_ACC(vAvgPixelR, vAvgPixelG, vAvgPixelB, R, G, B, vAreaTL);
    }

    /* Top Right averaging */
    {
        __ARM_2D_RGB565_GET_RGBVEC_FROM_POINT(vaddq_n_s16(vXi, 1), vaddq_n_s16(vYi, 1),
                                                                R, G, B, MaskColour, predGlb);

        __ARM_2D_SCALE_RGBVEC_ACC(vAvgPixelR, vAvgPixelG, vAvgPixelB, R, G, B, vAreaTR);
    }


    /* pack */
    uint16x8_t      TempPixel = __arm_2d_rgb565_pack_single_vec(vcvtq_s16_f16(vAvgPixelR),
                                                                vcvtq_s16_f16(vAvgPixelG),
                                                                vcvtq_s16_f16(vAvgPixelB));

    /* select between target pixel, averaged pixed */
    vTarget = vpselq_u16(TempPixel, vTarget, predGlb);

#else
    arm_2d_point_s16x8_t vPoint = {
            .X = vcvtq_s16_f16(ptPoint->X),
            .Y = vcvtq_s16_f16(ptPoint->Y) };

    /* set vector predicate if point is inside the region */
    mve_pred16_t    p = arm_2d_is_point_vec_inside_region_s16(ptOrigValidRegion, &vPoint);
    /* prepare vector of point offsets */
    uint16x8_t      ptOffs = vPoint.X + vPoint.Y * iOrigStride;
    /* retrieve all point values */
    uint16x8_t      ptVal = vldrhq_gather_shifted_offset_z_u16(pOrigin, ptOffs, predTail);

    /* combine 2 predicates set to true if point is in the region & values different from color mask */
    vTarget = vpselq_u16(ptVal, vTarget, vcmpneq_m_n_u16(ptVal, MaskColour, p));

#endif

    /* update target pixels */
    vst1q_p(pTarget, vTarget, predTail);
}


static
void __arm_2d_impl_rgb565_get_pixel_colour_offs_compensated(arm_2d_point_f16x8_t *
                                                            ptPoint,
                                                            arm_2d_region_t *
                                                            ptOrigValidRegion,
                                                            uint16_t * pOrigin,
                                                            int16_t iOrigStride,
                                                            uint16_t * pTarget,
                                                            uint16_t MaskColour,
                                                            uint32_t elts)
{
    mve_pred16_t    predTail = vctp16q(elts);
    uint16x8_t      vTarget = vld1q(pTarget);

#if defined(__ARM_2D_HAS_INTERPOLATION_ROTATION__) &&  __ARM_2D_HAS_INTERPOLATION_ROTATION__
    float16x8_t     vOne = vdupq_n_f16(1.0f);
    int16x8_t       vXi = vcvtq_s16_f16(ptPoint->X);
    int16x8_t       vYi = vcvtq_s16_f16(ptPoint->Y);

    vXi = vsubq_m_n_s16(vXi, vXi, 1, vcmpltq_n_f16(ptPoint->X, 0));
    vYi = vsubq_m_n_s16(vYi, vYi, 1, vcmpltq_n_f16(ptPoint->Y, 0));

    float16x8_t     vWX = ptPoint->X - vcvtq_f16_s16(vXi);
    float16x8_t     vWY = ptPoint->Y - vcvtq_f16_s16(vYi);

    /* combination of Bottom / Top & Left / Right areas contributions */
    float16x8_t     vAreaTR = vWX * vWY;
    float16x8_t     vAreaTL = (vOne - vWX) * vWY;
    float16x8_t     vAreaBR = vWX * (vOne - vWY);
    float16x8_t     vAreaBL = (vOne - vWX) * (vOne - vWY);

    /* accumulated pixel vectors */
    float16x8_t     vAvgPixelR, vAvgPixelG, vAvgPixelB;

    /* predicate accumulator */
    /* tracks all predications conditions for selecting final */
    /* averaged pixed / target pixel */
    /* can probably optimized away since averaged target pixel is */
    /* equivalent to original pixel, but precision limitations would introduce small errors */
    mve_pred16_t    predGlb = 0;

    /*
     * accumulate / average over the 4 neigbouring pixels
     */

    uint16x8_t      R, G, B;
    /* Bottom Left averaging */
    {
        __ARM_2D_RGB565_GET_RGBVEC_FROM_POINT_FAR(vXi, vYi, R, G, B, MaskColour, predGlb);

        __ARM_2D_SCALE_RGBVEC(vAvgPixelR, vAvgPixelG, vAvgPixelB, R, G, B, vAreaBL);
    }

    /* Bottom Right averaging */
    {
        __ARM_2D_RGB565_GET_RGBVEC_FROM_POINT_FAR(vaddq_n_s16(vXi, 1), vYi,
            R, G, B, MaskColour, predGlb);

        __ARM_2D_SCALE_RGBVEC_ACC(vAvgPixelR, vAvgPixelG, vAvgPixelB, R, G, B, vAreaBR);
    }

    /* Top Left averaging */
    {
        __ARM_2D_RGB565_GET_RGBVEC_FROM_POINT_FAR(vXi, vaddq_n_s16(vYi, 1),
                R, G, B, MaskColour, predGlb);

        __ARM_2D_SCALE_RGBVEC_ACC(vAvgPixelR, vAvgPixelG, vAvgPixelB, R, G, B, vAreaTL);
    }

    /* Top Right averaging */
    {
        __ARM_2D_RGB565_GET_RGBVEC_FROM_POINT_FAR(vaddq_n_s16(vXi, 1), vaddq_n_s16(vYi, 1),
                R, G, B, MaskColour, predGlb);

        __ARM_2D_SCALE_RGBVEC_ACC(vAvgPixelR, vAvgPixelG, vAvgPixelB, R, G, B, vAreaTR);
    }


    /* pack */
    uint16x8_t      TempPixel = __arm_2d_rgb565_pack_single_vec(vcvtq_s16_f16(vAvgPixelR),
                                                                vcvtq_s16_f16(vAvgPixelG),
                                                                vcvtq_s16_f16(vAvgPixelB));

    /* select between target pixel, averaged pixed */
    vTarget = vpselq_u16(TempPixel, vTarget, predGlb);

#else
    arm_2d_point_s16x8_t vPoint = {
            .X = vcvtq_s16_f16(ptPoint->X),
            .Y = vcvtq_s16_f16(ptPoint->Y) };
    /* set vector predicate if point is inside the region */
    mve_pred16_t    p = arm_2d_is_point_vec_inside_region_s16(ptOrigValidRegion, &vPoint);

    /* prepare vector of point offsets */
    /* correctionOffset avoid 16-bit overflow */
    int16_t         correctionOffset = vminvq_s16(0x7fff, vPoint.Y) - 1;
    uint16x8_t      ptOffs = vPoint.X + (vPoint.Y - correctionOffset) * iOrigStride;

    /* base pointer update to compensate offset */
    pOrigin += (correctionOffset * iOrigStride);

    /* retrieve all point values */
    uint16x8_t      ptVal = vldrhq_gather_shifted_offset_z_u16(pOrigin, ptOffs, predTail);

    /* combine 2 predicates set to true if point is in the region & values different from color mask */
    vTarget = vpselq_u16(ptVal, vTarget, vcmpneq_m_n_u16(ptVal, MaskColour, p));

#endif

    vst1q_p(pTarget, vTarget, predTail);
}

static
void __arm_2d_impl_rgb565_get_pixel_colour_with_alpha(
                                            arm_2d_point_f16x8_t    *ptPoint,
                                            arm_2d_region_t         *ptOrigValidRegion,
                                            uint16_t                *pOrigin,
                                            int16_t                  iOrigStride,
                                            uint16_t                *pTarget,
                                            uint16_t                 MaskColour,
                                            uint8_t                  chOpacity,
                                            uint32_t                 elts)
{
    mve_pred16_t    predTail = vctp16q(elts);
    uint16x8_t      vTarget = vld1q(pTarget);

#if defined(__ARM_2D_HAS_INTERPOLATION_ROTATION__) &&  __ARM_2D_HAS_INTERPOLATION_ROTATION__

    float16x8_t     vOne = vdupq_n_f16(1.0f);
    int16x8_t       vXi = vcvtq_s16_f16(ptPoint->X);
    int16x8_t       vYi = vcvtq_s16_f16(ptPoint->Y);

    vXi = vsubq_m_n_s16(vXi, vXi, 1, vcmpltq_n_f16(ptPoint->X, 0));
    vYi = vsubq_m_n_s16(vYi, vYi, 1, vcmpltq_n_f16(ptPoint->Y, 0));

    float16x8_t     vWX = ptPoint->X - vcvtq_f16_s16(vXi);
    float16x8_t     vWY = ptPoint->Y - vcvtq_f16_s16(vYi);

    /* combination of Bottom / Top & Left / Right areas contributions */
    float16x8_t     vAreaTR = vWX * vWY;
    float16x8_t     vAreaTL = (vOne - vWX) * vWY;
    float16x8_t     vAreaBR = vWX * (vOne - vWY);
    float16x8_t     vAreaBL = (vOne - vWX) * (vOne - vWY);

    /* accumulated pixel vectors */
    float16x8_t     vAvgPixelR, vAvgPixelG, vAvgPixelB;

    /* predicate accumulator */
    /* tracks all predications conditions for selecting final */
    /* averaged pixed / target pixel */
    /* can probably optimized away since averaged target pixel is */
    /* equivalent to original pixel, but precision limitations would introduce small errors */
    mve_pred16_t    predGlb = 0;

    /*
     * accumulate / average over the 4 neigbouring pixels
     */

    uint16x8_t      R, G, B;
    /* Bottom Left averaging */
    {
        __ARM_2D_RGB565_GET_RGBVEC_FROM_POINT(vXi, vYi, R, G, B, MaskColour, predGlb);

        __ARM_2D_SCALE_RGBVEC(vAvgPixelR, vAvgPixelG, vAvgPixelB, R, G, B, vAreaBL);
    }


    /* Bottom Right averaging */
    {
        __ARM_2D_RGB565_GET_RGBVEC_FROM_POINT(vaddq_n_s16(vXi, 1), vYi, R, G, B, MaskColour, predGlb);

        __ARM_2D_SCALE_RGBVEC_ACC(vAvgPixelR, vAvgPixelG, vAvgPixelB, R, G, B, vAreaBR);
    }

    /* Top Left averaging */
    {
        __ARM_2D_RGB565_GET_RGBVEC_FROM_POINT(vXi, vaddq_n_s16(vYi, 1), R, G, B, MaskColour, predGlb);

        __ARM_2D_SCALE_RGBVEC_ACC(vAvgPixelR, vAvgPixelG, vAvgPixelB, R, G, B, vAreaTL);
    }

    /* Top Right averaging */
    {
        __ARM_2D_RGB565_GET_RGBVEC_FROM_POINT(vaddq_n_s16(vXi, 1), vaddq_n_s16(vYi, 1),
                                                                R, G, B, MaskColour, predGlb);

        __ARM_2D_SCALE_RGBVEC_ACC(vAvgPixelR, vAvgPixelG, vAvgPixelB, R, G, B, vAreaTR);
    }


    /* blending */
    uint16x8_t      vAvgR, vAvgG, vAvgB;

    vAvgR = vcvtq_s16_f16(vAvgPixelR);
    vAvgG = vcvtq_s16_f16(vAvgPixelG);
    vAvgB = vcvtq_s16_f16(vAvgPixelB);

    uint16x8_t      vBlended;

    __ARM_2D_BLEND_RGB565_TARGET_RGBVEC(chOpacity, vTarget, vAvgR, vAvgG, vAvgB, vBlended);

    /* select between target pixel, averaged pixed */
    vTarget = vpselq_u16(vBlended, vTarget, predGlb);

#else
    /* set vector predicate if point is inside the region */
    arm_2d_point_s16x8_t vPoint = {
            .X = vcvtq_s16_f16(ptPoint->X),
            .Y = vcvtq_s16_f16(ptPoint->Y) };

    mve_pred16_t    p = arm_2d_is_point_vec_inside_region_s16(ptOrigValidRegion, &vPoint);
    /* prepare vector of point offsets */
    uint16x8_t      ptOffs = vPoint.X + vPoint.Y * iOrigStride;
    /* retrieve all point values */
    uint16x8_t      ptVal = vldrhq_gather_shifted_offset_z_u16(pOrigin, ptOffs, predTail);

    /* alpha blending */
    uint16x8_t      vBlended =
        __arm_2d_rgb565_alpha_blending_single_vec(ptVal, vTarget, chOpacity);

    /* combine 2 predicates, set to true, if point is in the region & values different from color mask */
    vTarget = vpselq_u16(vBlended, vTarget, vcmpneq_m_n_u16(ptVal, MaskColour, p));
#endif

    vst1q_p(pTarget, vTarget, predTail);
}


static
void __arm_2d_impl_rgb565_get_pixel_colour_with_alpha_offs_compensated(
                                            arm_2d_point_f16x8_t    *ptPoint,
                                            arm_2d_region_t         *ptOrigValidRegion,
                                            uint16_t                *pOrigin,
                                            int16_t                  iOrigStride,
                                            uint16_t                *pTarget,
                                            uint16_t                 MaskColour,
                                            uint8_t                  chOpacity,
                                            uint32_t                 elts)
{
    mve_pred16_t    predTail = vctp16q(elts);
    uint16x8_t      vTarget = vld1q(pTarget);

#if defined(__ARM_2D_HAS_INTERPOLATION_ROTATION__) &&  __ARM_2D_HAS_INTERPOLATION_ROTATION__

    float16x8_t     vOne = vdupq_n_f16(1.0f);
    int16x8_t       vXi = vcvtq_s16_f16(ptPoint->X);
    int16x8_t       vYi = vcvtq_s16_f16(ptPoint->Y);

    vXi = vsubq_m_n_s16(vXi, vXi, 1, vcmpltq_n_f16(ptPoint->X, 0));
    vYi = vsubq_m_n_s16(vYi, vYi, 1, vcmpltq_n_f16(ptPoint->Y, 0));

    float16x8_t     vWX = ptPoint->X - vcvtq_f16_s16(vXi);
    float16x8_t     vWY = ptPoint->Y - vcvtq_f16_s16(vYi);

    /* combination of Bottom / Top & Left / Right areas contributions */
    float16x8_t     vAreaTR = vWX * vWY;
    float16x8_t     vAreaTL = (vOne - vWX) * vWY;
    float16x8_t     vAreaBR = vWX * (vOne - vWY);
    float16x8_t     vAreaBL = (vOne - vWX) * (vOne - vWY);

    /* accumulated pixel vectors */
    float16x8_t     vAvgPixelR, vAvgPixelG, vAvgPixelB;

    /* predicate accumulator */
    /* tracks all predications conditions for selecting final */
    /* averaged pixed / target pixel */
    /* can probably optimized away since averaged target pixel is */
    /* equivalent to original pixel, but precision limitations would introduce small errors */
    mve_pred16_t    predGlb = 0;

    /*
     * accumulate / average over the 4 neigbouring pixels
     */

    uint16x8_t      R, G, B;
    /* Bottom Left averaging */
    {
        __ARM_2D_RGB565_GET_RGBVEC_FROM_POINT_FAR(vXi, vYi, R, G, B, MaskColour, predGlb);

        __ARM_2D_SCALE_RGBVEC(vAvgPixelR, vAvgPixelG, vAvgPixelB, R, G, B, vAreaBL);
    }


    /* Bottom Right averaging */
    {
        __ARM_2D_RGB565_GET_RGBVEC_FROM_POINT_FAR(vaddq_n_s16(vXi, 1), vYi, R, G, B, MaskColour, predGlb);

        __ARM_2D_SCALE_RGBVEC_ACC(vAvgPixelR, vAvgPixelG, vAvgPixelB, R, G, B, vAreaBR);
    }

    /* Top Left averaging */
    {
        __ARM_2D_RGB565_GET_RGBVEC_FROM_POINT_FAR(vXi, vaddq_n_s16(vYi, 1), R, G, B, MaskColour, predGlb);

        __ARM_2D_SCALE_RGBVEC_ACC(vAvgPixelR, vAvgPixelG, vAvgPixelB, R, G, B, vAreaTL);
    }

    /* Top Right averaging */
    {
        __ARM_2D_RGB565_GET_RGBVEC_FROM_POINT_FAR(vaddq_n_s16(vXi, 1), vaddq_n_s16(vYi, 1),
                                                                R, G, B, MaskColour, predGlb);

        __ARM_2D_SCALE_RGBVEC_ACC(vAvgPixelR, vAvgPixelG, vAvgPixelB, R, G, B, vAreaTR);
    }


    /* blending */
    uint16x8_t      vAvgR, vAvgG, vAvgB;

    vAvgR = vcvtq_s16_f16(vAvgPixelR);
    vAvgG = vcvtq_s16_f16(vAvgPixelG);
    vAvgB = vcvtq_s16_f16(vAvgPixelB);

    uint16x8_t      vBlended;

    __ARM_2D_BLEND_RGB565_TARGET_RGBVEC(chOpacity, vTarget, vAvgR, vAvgG, vAvgB, vBlended);

    /* select between target pixel, averaged pixed */
    vTarget = vpselq_u16(vBlended, vTarget, predGlb);
#else
    /* set vector predicate if point is inside the region */
    arm_2d_point_s16x8_t vPoint = {
            .X = vcvtq_s16_f16(ptPoint->X),
            .Y = vcvtq_s16_f16(ptPoint->Y) };
    /* set vector predicate if point is inside the region */
    mve_pred16_t    p = arm_2d_is_point_vec_inside_region_s16(ptOrigValidRegion, &vPoint);
    /* prepare vector of point offsets */
    /* correctionOffset avoid 16-bit overflow */
    int16_t         correctionOffset = vminvq_s16(0x7fff, vPoint.Y) - 1;

    uint16x8_t      ptOffs =
        vPoint.X + (vPoint.Y - correctionOffset) * iOrigStride;

    /* retrieve all point values */
    /* base pointer update to compensate offset */
    pOrigin += (correctionOffset * iOrigStride);

    uint16x8_t      ptVal = vldrhq_gather_shifted_offset_z_u16(pOrigin, ptOffs, predTail);


    /* alpha blending */
    uint16x8_t      vBlended =
        __arm_2d_rgb565_alpha_blending_single_vec(ptVal, vTarget, chOpacity);

    /* combine 2 predicates, set to true, if point is in the region & values different from color mask */
    vTarget = vpselq_u16(vBlended, vTarget, vcmpneq_m_n_u16(ptVal, MaskColour, p));

#endif

    vst1q_p(pTarget, vTarget, predTail);
}




static
void __arm_2d_impl_cccn888_get_pixel_colour_mve(   arm_2d_point_f16x8_t *ptPoint,
                                            arm_2d_region_t *ptOrigValidRegion,
                                            uint32_t *pOrigin,
                                            int16_t iOrigStride,
                                            uint32_t *pTarget,
                                            uint32_t MaskColour,
                                            int16_t elts)
{
#if     defined(__ARM_2D_HAS_INTERPOLATION_ROTATION__) &&  __ARM_2D_HAS_INTERPOLATION_ROTATION__
    ARM_ALIGN(8) uint32_t scratch32[32];
    int16_t        *pscratch16 = (int16_t *) scratch32;
    uint32x4_t      vTargetLo = vld1q(pTarget);
    uint32x4_t      vTargetHi = vld1q(pTarget + 4);
    mve_pred16_t    predTailLow = vctp32q(elts);
    mve_pred16_t    predTailHigh = elts - 4 > 0 ? vctp32q(elts - 4) : 0;
    float16x8_t     vOne = vdupq_n_f16(1.0f);

    int16x8_t       vXi = vcvtq_s16_f16(ptPoint->X);
    int16x8_t       vYi = vcvtq_s16_f16(ptPoint->Y);

    vXi = vsubq_m_n_s16(vXi, vXi, 1, vcmpltq_n_f16(ptPoint->X, 0));
    vYi = vsubq_m_n_s16(vYi, vYi, 1, vcmpltq_n_f16(ptPoint->Y, 0));

    float16x8_t     vWX = ptPoint->X - vcvtq_f16_s16(vXi);
    float16x8_t     vWY = ptPoint->Y - vcvtq_f16_s16(vYi);

    /* combination of Bottom / Top & Left / Right areas contributions */
    float16x8_t     vAreaTR = vWX * vWY;
    float16x8_t     vAreaTL = (vOne - vWX) * vWY;
    float16x8_t     vAreaBR = vWX * (vOne - vWY);
    float16x8_t     vAreaBL = (vOne - vWX) * (vOne - vWY);

    /* accumulated pixel vectors */
    float16x8_t     vAvgPixelR, vAvgPixelG, vAvgPixelB;

    /* predicate accumulators */
    /* tracks all predications conditions for selecting final */
    /* averaged pixed / target pixel */
    /* can probably optimized away since averaged target pixel is */
    /* equivalent to original pixel, but precision limitations would introduce small errors */
    mve_pred16_t    predGlbLo = 0, predGlbHi = 0;

    /*
     * accumulate / average over the 4 neigbouring pixels
     */

    uint16x8_t      R, G, B;

    /* Bottom Left averaging */
    {
        __ARM_2D_RGB888_GET_RGBVEC_FROM_POINT(vXi, vYi, R, G, B, MaskColour, predGlbLo, predGlbHi);

        __ARM_2D_SCALE_RGBVEC(vAvgPixelR, vAvgPixelG, vAvgPixelB, R, G, B, vAreaBL);
    }

    /* Bottom Right averaging */
    {
        __ARM_2D_RGB888_GET_RGBVEC_FROM_POINT(vaddq_n_s16(vXi, 1), vYi,
                            R, G, B, MaskColour, predGlbLo, predGlbHi);

        __ARM_2D_SCALE_RGBVEC_ACC(vAvgPixelR, vAvgPixelG, vAvgPixelB, R, G, B, vAreaBR);
    }

    /* Top Left averaging */
    {
        __ARM_2D_RGB888_GET_RGBVEC_FROM_POINT(vXi, vaddq_n_s16(vYi, 1),
                                                R, G, B, MaskColour, predGlbLo, predGlbHi);

        __ARM_2D_SCALE_RGBVEC_ACC(vAvgPixelR, vAvgPixelG, vAvgPixelB, R, G, B, vAreaTL);
    }

    /* Top Right averaging */
    {
        __ARM_2D_RGB888_GET_RGBVEC_FROM_POINT(vaddq_n_s16(vXi, 1), vaddq_n_s16(vYi, 1),
                                                R, G, B, MaskColour, predGlbLo, predGlbHi);

        __ARM_2D_SCALE_RGBVEC_ACC(vAvgPixelR, vAvgPixelG, vAvgPixelB, R, G, B, vAreaTR);
    }

    /* pack */
    __arm_2d_pack_rgb888_to_mem((uint8_t *) scratch32, vcvtq_s16_f16(vAvgPixelR),
                                vcvtq_s16_f16(vAvgPixelG), vcvtq_s16_f16(vAvgPixelB));

    uint32x4_t      TempPixel = vld1q(scratch32);

    /* select between target pixel, averaged pixed */
    TempPixel = vpselq_u32(TempPixel, vTargetLo, predGlbLo);

    vst1q_p(pTarget, TempPixel, predTailLow);

    TempPixel = vld1q(scratch32 + 4);

    /* select between target pixel, averaged pixed */
    TempPixel = vpselq_u32(TempPixel, vTargetHi, predGlbHi);

    vst1q_p(pTarget + 4, TempPixel, predTailHigh);
#else

    arm_2d_point_s32x4_t    tPointLo, tPointHi;
    ARM_ALIGN(8)  int16_t   scratch[8];
    mve_pred16_t            p;

    /* split 16-bit point vector into 2 x 32-bit vectors */
    vst1q(scratch, vcvtq_s16_f16(ptPoint->X));
    tPointLo.X = vldrhq_s32(scratch);
    tPointHi.X = vldrhq_s32(scratch + 4);

    vst1q(scratch, vcvtq_s16_f16(ptPoint->Y));
    tPointLo.Y = vldrhq_s32(scratch);
    tPointHi.Y = vldrhq_s32(scratch + 4);

    /* 1st half */

    /* set vector predicate if point is inside the region */
    p = arm_2d_is_point_vec_inside_region_s32(ptOrigValidRegion, &tPointLo);
    /* prepare vector of point offsets */
    uint32x4_t      ptOffs = tPointLo.X + tPointLo.Y * iOrigStride;
    uint32x4_t      vPixel = vld1q(pTarget);
    /* retrieve all point values */
    uint32x4_t      ptVal = vldrwq_gather_shifted_offset_u32(pOrigin, ptOffs);

    /* combine 2 predicates set to true if point is in the region & values different from color mask */
    vPixel = vpselq_u32(ptVal, vPixel, vcmpneq_m_n_u32(ptVal, MaskColour, p));

    vst1q_p(pTarget, vPixel, vctp32q(elts));

    elts -= 4;
    if (elts > 0) {

        /* second half */
        p = arm_2d_is_point_vec_inside_region_s32(ptOrigValidRegion, &tPointHi);
        ptOffs = tPointHi.X + tPointHi.Y * iOrigStride;
        vPixel = vld1q(pTarget + 4);

        ptVal = vldrwq_gather_shifted_offset_u32(pOrigin, ptOffs);
        vPixel = vpselq_u32(ptVal, vPixel, vcmpneq_m_n_u32(ptVal, MaskColour, p));
        vst1q_p(pTarget + 4, vPixel, vctp32q(elts));
    }
#endif
}



static
void __arm_2d_impl_cccn888_get_pixel_colour_with_alpha_mve(
                                            arm_2d_point_s16x8_t    *ptPoint,
                                            arm_2d_region_t         *ptOrigValidRegion,
                                            uint32_t                *pOrigin,
                                            int16_t                  iOrigStride,
                                            uint32_t                *pTarget,
                                            uint32_t                 MaskColour,
                                            uint8_t                  chOpacity,
                                            int16_t                  elts)
{
#if     defined(__ARM_2D_HAS_INTERPOLATION_ROTATION__)  &&  __ARM_2D_HAS_INTERPOLATION_ROTATION__
    ARM_ALIGN(8) uint32_t scratch32[32];
    int16_t        *pscratch16 = (int16_t *) scratch32;
    uint32x4_t      vTargetLo = vld1q(pTarget);
    uint32x4_t      vTargetHi = vld1q(pTarget + 4);
    mve_pred16_t    predTailLow = vctp32q(elts);
    mve_pred16_t    predTailHigh = elts - 4 > 0 ? vctp32q(elts - 4) : 0;
    float16x8_t     vOne = vdupq_n_f16(1.0f);

    int16x8_t       vXi = vcvtq_s16_f16(ptPoint->X);
    int16x8_t       vYi = vcvtq_s16_f16(ptPoint->Y);

    vXi = vsubq_m_n_s16(vXi, vXi, 1, vcmpltq_n_f16(ptPoint->X, 0));
    vYi = vsubq_m_n_s16(vYi, vYi, 1, vcmpltq_n_f16(ptPoint->Y, 0));

    float16x8_t     vWX = ptPoint->X - vcvtq_f16_s16(vXi);
    float16x8_t     vWY = ptPoint->Y - vcvtq_f16_s16(vYi);

    /* combination of Bottom / Top & Left / Right areas contributions */
    float16x8_t     vAreaTR = vWX * vWY;
    float16x8_t     vAreaTL = (vOne - vWX) * vWY;
    float16x8_t     vAreaBR = vWX * (vOne - vWY);
    float16x8_t     vAreaBL = (vOne - vWX) * (vOne - vWY);

    /* accumulated pixel vectors */
    float16x8_t     vAvgPixelR, vAvgPixelG, vAvgPixelB;


    /* predicate accumulators */
    /* tracks all predications conditions for selecting final */
    /* averaged pixed / target pixel */
    /* can probably optimized away since averaged target pixel is */
    /* equivalent to original pixel, but precision limitations would introduce small errors */
    mve_pred16_t    predGlbLo = 0, predGlbHi = 0;

    /*
     * accumulate / average over the 4 neigbouring pixels
     */
    uint16x8_t      R, G, B;

    /* Bottom Left averaging */
    {
        __ARM_2D_RGB888_GET_RGBVEC_FROM_POINT(vXi, vYi, R, G, B, MaskColour, predGlbLo, predGlbHi);

        __ARM_2D_SCALE_RGBVEC(vAvgPixelR, vAvgPixelG, vAvgPixelB, R, G, B, vAreaBL);
    }

    /* Bottom Right averaging */
    {
        __ARM_2D_RGB888_GET_RGBVEC_FROM_POINT(vaddq_n_s16(vXi, 1), vYi,
                                            R, G, B, MaskColour, predGlbLo, predGlbHi);

        __ARM_2D_SCALE_RGBVEC_ACC(vAvgPixelR, vAvgPixelG, vAvgPixelB, R, G, B, vAreaBR);
    }

    /* Top Left averaging */
    {
        __ARM_2D_RGB888_GET_RGBVEC_FROM_POINT(vXi, vaddq_n_s16(vYi, 1),
                                        R, G, B, MaskColour, predGlbLo, predGlbHi);

        __ARM_2D_SCALE_RGBVEC_ACC(vAvgPixelR, vAvgPixelG, vAvgPixelB, R, G, B, vAreaTL);
    }

    /* Top Right averaging */
    {
        __ARM_2D_RGB888_GET_RGBVEC_FROM_POINT(vaddq_n_s16(vXi, 1), vaddq_n_s16(vYi, 1),
            R, G, B, MaskColour, predGlbLo, predGlbHi);

        __ARM_2D_SCALE_RGBVEC_ACC(vAvgPixelR, vAvgPixelG, vAvgPixelB, R, G, B, vAreaTR);
    }


    /* alpha blending */
    uint16x8_t      vTargetR, vTargetG, vTargetB;

    __arm_2d_unpack_rgb888_from_mem((const uint8_t *) pTarget, &vTargetR, &vTargetG, &vTargetB);

    uint16_t        chOpacityCompl = (256 - chOpacity);

    uint16x8_t      vAvgR, vAvgG, vAvgB;

    vAvgR = vcvtq_s16_f16(vAvgPixelR);
    vAvgG = vcvtq_s16_f16(vAvgPixelG);
    vAvgB = vcvtq_s16_f16(vAvgPixelB);

    /* merge */
    vAvgR = vAvgR * chOpacityCompl + vTargetR * chOpacity;
    vAvgR = vAvgR >> 8;

    vAvgG = vAvgG * chOpacityCompl + vTargetG * chOpacity;
    vAvgG = vAvgG >> 8;

    vAvgB = vAvgB * chOpacityCompl + vTargetB * chOpacity;
    vAvgB = vAvgB >> 8;


    /* pack */
    __arm_2d_pack_rgb888_to_mem((uint8_t *) scratch32, vAvgR, vAvgG, vAvgB);

    uint32x4_t      TempPixel = vld1q(scratch32);

    /* select between target pixel, averaged pixed */
    TempPixel = vpselq_u32(TempPixel, vTargetLo, predGlbLo);

    vst1q_p(pTarget, TempPixel, predTailLow);

    TempPixel = vld1q(scratch32 + 4);

    /* select between target pixel, averaged pixed */
    TempPixel = vpselq_u32(TempPixel, vTargetHi, predGlbHi);

    vst1q_p(pTarget + 4, TempPixel, predTailHigh);
#else
    arm_2d_point_s32x4_t    tPointLo, tPointHi;
    ARM_ALIGN(8) int16_t          scratch[8];
    ARM_ALIGN(8) uint32_t         blendled[4];
    mve_pred16_t            p;

    /* split 16-bit point vector into 2 x 32-bit vectors */
    vst1q(scratch, ptPoint->X);
    tPointLo.X = vldrhq_s32(scratch);
    tPointHi.X = vldrhq_s32(scratch + 4);

    vst1q(scratch, ptPoint->Y);
    tPointLo.Y = vldrhq_s32(scratch);
    tPointHi.Y = vldrhq_s32(scratch + 4);

    /* 1st half */

    /* set vector predicate if point is inside the region */
    p = arm_2d_is_point_vec_inside_region_s32(ptOrigValidRegion, &tPointLo);
    /* prepare vector of point offsets */
    uint32x4_t      ptOffs = tPointLo.X + tPointLo.Y * iOrigStride;
    uint32x4_t      vPixel = vld1q(pTarget);
    /* retrieve all point values */
    uint32x4_t      ptVal = vldrwq_gather_shifted_offset_u32(pOrigin, ptOffs);

    vstrwq_u32((uint32_t *) scratch, ptVal);

    /* alpha-blending (requires widened inputs) */
    vstrbq_u16((uint8_t *) blendled,
               __rgb888_alpha_blending_direct_single_vec(vldrbq_u16((uint8_t const *) scratch),
                                                         vldrbq_u16((uint8_t const *) pTarget), chOpacity));

    vstrbq_u16((uint8_t *) blendled + 2,
               __rgb888_alpha_blending_direct_single_vec(vldrbq_u16((uint8_t const *)scratch + 4),
                                                         vldrbq_u16((uint8_t const *)pTarget + 2), chOpacity));

    uint32x4_t      vBlended = vld1q(blendled);

    /* combine 2 predicates, set to true, if point is in the region & values different from color mask */
    vPixel = vpselq_u32(vBlended, vPixel, vcmpneq_m_n_u32(ptVal, MaskColour, p));

    vst1q_p(pTarget, vPixel, vctp32q(elts));

    elts -= 4;
    if(elts > 0) {
        /* second half */

        p = arm_2d_is_point_vec_inside_region_s32(ptOrigValidRegion, &tPointHi);
        ptOffs = tPointHi.X + tPointHi.Y * iOrigStride;
        vPixel = vld1q(pTarget);
        ptVal = vldrwq_gather_shifted_offset_u32(pOrigin, ptOffs);

        vstrwq_u32((uint32_t *) scratch, ptVal);

        /* alpha-blending (requires widened inputs) */
        vstrbq_u16((uint8_t *) blendled,
                   __rgb888_alpha_blending_direct_single_vec(vldrbq_u16((uint8_t const *) scratch),
                                                             vldrbq_u16((uint8_t const *) pTarget), chOpacity));
        vstrbq_u16((uint8_t *) blendled + 2,
                   __rgb888_alpha_blending_direct_single_vec(vldrbq_u16((uint8_t const *)scratch + 4),
                                                             vldrbq_u16((uint8_t const *)pTarget + 2), chOpacity));

        vBlended = vld1q(blendled);

        /* combine 2 predicates, set to true, if point is in the region & values different from color mask */
        vPixel = vpselq_u32(vBlended, vPixel, vcmpneq_m_n_u32(ptVal, MaskColour, p));

        vst1q_p(pTarget + 4, vPixel, vctp32q(elts));
    }
#endif
}


__OVERRIDE_WEAK
void __arm_2d_impl_rgb565_rotate( __arm_2d_param_copy_orig_t *ptParam,
                            __arm_2d_rotate_info_t *ptInfo)
{
    int32_t    iHeight = ptParam->use_as____arm_2d_param_copy_t.tCopySize.iHeight;
    int32_t    iWidth = ptParam->use_as____arm_2d_param_copy_t.tCopySize.iWidth;

    int32_t    iTargetStride =
        ptParam->use_as____arm_2d_param_copy_t.tTarget.iStride;
    uint16_t       *pTargetBase = ptParam->use_as____arm_2d_param_copy_t.tTarget.pBuffer;
    uint16_t       *pOrigin = ptParam->tOrigin.pBuffer;
    int32_t    iOrigStride = ptParam->tOrigin.iStride;
    uint16_t        MaskColour = ptInfo->Mask.hwColour;
    float           fAngle = -ptInfo->fAngle;
    arm_2d_location_t tOffset =
        ptParam->use_as____arm_2d_param_copy_t.tSource.tValidRegion.tLocation;
    arm_2d_location_t *pCenter = &(ptInfo->tCenter);

    float           invIWidth = iWidth > 1 ? 1.0f / (float) (iWidth - 1) : __LARGEINVF32;
    arm_2d_rot_linear_regr_t regrCoefs[2];
    arm_2d_location_t SrcPt = ptInfo->tDummySourceOffset;
    bool            gatherLoadIdxOverflow;

    /* get regression parameters over 1st and last column */
    gatherLoadIdxOverflow =
        __arm_2d_rotate_regression(&ptParam->use_as____arm_2d_param_copy_t.tCopySize,
                                   &SrcPt, fAngle, &tOffset, pCenter, iOrigStride,
                                   regrCoefs);

    /* slopes between 1st and last columns */
    float16_t       slopeY, slopeX;

    slopeY = (float16_t) (regrCoefs[1].interceptY - regrCoefs[0].interceptY) * invIWidth;
    slopeX = (float16_t) (regrCoefs[1].interceptX - regrCoefs[0].interceptX) * invIWidth;


    if (!gatherLoadIdxOverflow) {
        for (int32_t y = 0; y < iHeight; y++) {

            /* 1st column estimates (intercepts for regression in X direction */
            float16_t       colFirstY = regrCoefs[0].slopeY * y + regrCoefs[0].interceptY;
            float16_t       colFirstX = regrCoefs[0].slopeX * y + regrCoefs[0].interceptX;

            int32_t         nbVecElts = iWidth;
            float16x8_t     vX = vcvtq_f16_s16((int16x8_t) vidupq_n_u16(0, 1));
            uint16_t       *pTargetBaseCur = pTargetBase;

            while (nbVecElts > 0) {
                arm_2d_point_f16x8_t tPointV;

                    tPointV.X =
                        vfmaq_n_f16(vdupq_n_f16(colFirstX), vX, slopeX);
                    tPointV.Y =
                        vfmaq_n_f16(vdupq_n_f16(colFirstY), vX, slopeY);

#if !defined(__ARM_2D_CFG_UNSAFE_IGNORE_CALIB_IN_ROTATION_FOR_PERFORMANCE__)
                    tPointV.X = vaddq_m_n_f16(tPointV.X, tPointV.X, __CALIB, vcmpgtq(tPointV.X, 0));
                    tPointV.X = vsubq_m_n_f16(tPointV.X, tPointV.X, __CALIB, vcmpleq(tPointV.X, 0));

                    tPointV.Y = vaddq_m_n_f16(tPointV.Y, tPointV.Y, __CALIB, vcmpgtq(tPointV.Y, 0));
                    tPointV.Y = vsubq_m_n_f16(tPointV.Y, tPointV.Y, __CALIB, vcmpleq(tPointV.Y, 0));
#endif
                    __arm_2d_impl_rgb565_get_pixel_colour(&tPointV,
                                                          &ptParam->tOrigin.tValidRegion,
                                                          pOrigin,
                                                          iOrigStride,
                                                          pTargetBaseCur, MaskColour,
                                                          nbVecElts);

                pTargetBaseCur += 8;
                vX += 8.0f16;
                nbVecElts -= 8;
            }
            pTargetBase += iTargetStride;
        }
    } else {
        for (int32_t y = 0; y < iHeight; y++) {

            /* 1st column estimates (intercepts for regression in X direction */
            float16_t       colFirstY = regrCoefs[0].slopeY * y + regrCoefs[0].interceptY;
            float16_t       colFirstX = regrCoefs[0].slopeX * y + regrCoefs[0].interceptX;
            int32_t         nbVecElts = iWidth;
            float16x8_t     vX = vcvtq_f16_s16((int16x8_t) vidupq_n_u16(0, 1));
            uint16_t       *pTargetBaseCur = pTargetBase;

            while (nbVecElts > 0) {
                arm_2d_point_f16x8_t tPointV;

                tPointV.X =
                    vfmaq_n_f16(vdupq_n_f16(colFirstX), vX, slopeX);
                tPointV.Y =
                    vfmaq_n_f16(vdupq_n_f16(colFirstY), vX, slopeY);

#if !defined(__ARM_2D_CFG_UNSAFE_IGNORE_CALIB_IN_ROTATION_FOR_PERFORMANCE__)
                    tPointV.X = vaddq_m_n_f16(tPointV.X, tPointV.X, __CALIB, vcmpgtq(tPointV.X, 0));
                    tPointV.X = vsubq_m_n_f16(tPointV.X, tPointV.X, __CALIB, vcmpleq(tPointV.X, 0));

                    tPointV.Y = vaddq_m_n_f16(tPointV.Y, tPointV.Y, __CALIB, vcmpgtq(tPointV.Y, 0));
                    tPointV.Y = vsubq_m_n_f16(tPointV.Y, tPointV.Y, __CALIB, vcmpleq(tPointV.Y, 0));
#endif
                __arm_2d_impl_rgb565_get_pixel_colour_offs_compensated(&tPointV,
                                                                       &ptParam->tOrigin.
                                                                       tValidRegion,
                                                                       pOrigin,
                                                                       iOrigStride,
                                                                       pTargetBaseCur,
                                                                       MaskColour,
                                                                       nbVecElts);

                pTargetBaseCur += 8;
                vX += 8.0f16;
                nbVecElts -= 8;
            }
            pTargetBase += iTargetStride;
        }
    }
}


/* untested */
__OVERRIDE_WEAK
void __arm_2d_impl_cccn888_rotate(   __arm_2d_param_copy_orig_t *ptParam,
                                    __arm_2d_rotate_info_t *ptInfo)
{
    int32_t         iHeight = ptParam->use_as____arm_2d_param_copy_t.tCopySize.iHeight;
    int32_t         iWidth = ptParam->use_as____arm_2d_param_copy_t.tCopySize.iWidth;

    int32_t         iTargetStride =
        ptParam->use_as____arm_2d_param_copy_t.tTarget.iStride;
    uint32_t       *pTargetBase = ptParam->use_as____arm_2d_param_copy_t.tTarget.pBuffer;
    uint32_t       *pOrigin = ptParam->tOrigin.pBuffer;
    int32_t    iOrigStride = ptParam->tOrigin.iStride;
    uint32_t        MaskColour = ptInfo->Mask.hwColour;
    float           fAngle = -ptInfo->fAngle;
    arm_2d_location_t tOffset =
        ptParam->use_as____arm_2d_param_copy_t.tSource.tValidRegion.tLocation;
    arm_2d_location_t *pCenter = &(ptInfo->tCenter);
    float           invIWidth = 1.0f / (float) (iWidth - 1);
    arm_2d_rot_linear_regr_t    regrCoefs[2];
    arm_2d_location_t           SrcPt = ptInfo->tDummySourceOffset;

    /* get regression parameters over 1st and last column */
    __arm_2d_rotate_regression(&ptParam->use_as____arm_2d_param_copy_t.tCopySize,
                                   &SrcPt, fAngle, &tOffset, pCenter, iOrigStride,
                                   regrCoefs);

    /* slopes between 1st and last columns */
    float16_t           slopeY, slopeX;

    slopeY = (float16_t)(regrCoefs[1].interceptY - regrCoefs[0].interceptY) * invIWidth;
    slopeX = (float16_t)(regrCoefs[1].interceptX - regrCoefs[0].interceptX) * invIWidth;

    for (int32_t y = 0; y < iHeight; y++) {

        /* 1st column estimates (intercepts for regression in X direction */
        float16_t       colFirstY = regrCoefs[0].slopeY * y + regrCoefs[0].interceptY;
        float16_t       colFirstX = regrCoefs[0].slopeX * y + regrCoefs[0].interceptX;
        int32_t         nbVecElts = iWidth;
        float16x8_t     vX = vcvtq_f16_s16((int16x8_t) vidupq_n_u16(0, 1));
        uint32_t       *pTargetBaseCur = pTargetBase;

        while (nbVecElts > 0) {
            arm_2d_point_f16x8_t tPointV;

            tPointV.X =
                vfmaq_n_f16(vdupq_n_f16(colFirstX), vX, slopeX);
            tPointV.Y =
                vfmaq_n_f16(vdupq_n_f16(colFirstY), vX, slopeY);

#if !defined(__ARM_2D_CFG_UNSAFE_IGNORE_CALIB_IN_ROTATION_FOR_PERFORMANCE__)
            tPointV.X = vaddq_m_n_f16(tPointV.X, tPointV.X, __CALIB, vcmpgtq(tPointV.X, 0));
            tPointV.X = vsubq_m_n_f16(tPointV.X, tPointV.X, __CALIB, vcmpleq(tPointV.X, 0));

            tPointV.Y = vaddq_m_n_f16(tPointV.Y, tPointV.Y, __CALIB, vcmpgtq(tPointV.Y, 0));
            tPointV.Y = vsubq_m_n_f16(tPointV.Y, tPointV.Y, __CALIB, vcmpleq(tPointV.Y, 0));
#endif

            __arm_2d_impl_cccn888_get_pixel_colour_mve(&tPointV,
                                                      &ptParam->tOrigin.tValidRegion,
                                                      pOrigin,
                                                      iOrigStride,
                                                      pTargetBase, MaskColour, nbVecElts);

            pTargetBaseCur += 8;
            vX += 8.0f16;
            nbVecElts -= 8;
        }
        pTargetBase += iTargetStride;
    }
}

__OVERRIDE_WEAK
void __arm_2d_impl_rgb565_rotate_alpha(   __arm_2d_param_copy_orig_t *ptParam,
                                    __arm_2d_rotate_info_t *ptInfo,
                                    uint_fast8_t chRatio)
{
    int32_t         iHeight = ptParam->use_as____arm_2d_param_copy_t.tCopySize.iHeight;
    int32_t         iWidth = ptParam->use_as____arm_2d_param_copy_t.tCopySize.iWidth;

    int32_t         iTargetStride =
        ptParam->use_as____arm_2d_param_copy_t.tTarget.iStride;
    uint16_t       *pTargetBase = ptParam->use_as____arm_2d_param_copy_t.tTarget.pBuffer;
    uint16_t       *pOrigin = ptParam->tOrigin.pBuffer;
    int32_t         iOrigStride = ptParam->tOrigin.iStride;
    uint16_t        MaskColour = ptInfo->Mask.hwColour;
    float           fAngle = -ptInfo->fAngle;
    arm_2d_location_t tOffset =
        ptParam->use_as____arm_2d_param_copy_t.tSource.tValidRegion.tLocation;
    arm_2d_location_t *pCenter = &(ptInfo->tCenter);

    uint16_t        hwRatioCompl = 256 - chRatio;
    float           invIWidth = iWidth > 1 ? 1.0f / (float) (iWidth - 1) : __LARGEINVF32;
    arm_2d_rot_linear_regr_t regrCoefs[2];
    arm_2d_location_t SrcPt = ptInfo->tDummySourceOffset;
    bool            gatherLoadIdxOverflow;

    /* get regression parameters over 1st and last column */
    gatherLoadIdxOverflow =
        __arm_2d_rotate_regression(&ptParam->use_as____arm_2d_param_copy_t.tCopySize,
                                   &SrcPt, fAngle, &tOffset, pCenter, iOrigStride,
                                   regrCoefs);

    /* slopes between 1st and last columns */
    float16_t       slopeY, slopeX;

    slopeY = (float16_t) (regrCoefs[1].interceptY - regrCoefs[0].interceptY) * invIWidth;
    slopeX = (float16_t) (regrCoefs[1].interceptX - regrCoefs[0].interceptX) * invIWidth;

    if (!gatherLoadIdxOverflow) {
        for (int32_t y = 0; y < iHeight; y++) {
            /* 1st column estimates (intercepts for regression in X direction */
            float16_t       colFirstY =
                (float16_t) (regrCoefs[0].slopeY * y + regrCoefs[0].interceptY);
            float16_t       colFirstX =
                (float16_t) (regrCoefs[0].slopeX * y + regrCoefs[0].interceptX);

            int32_t         nbVecElts = iWidth;
            float16x8_t     vX = vcvtq_f16_s16((int16x8_t) vidupq_n_u16(0, 1));
            uint16_t       *pTargetBaseCur = pTargetBase;

            while (nbVecElts > 0) {
                arm_2d_point_f16x8_t tPointV;

                /* linear interpolation thru first & last columns */
                tPointV.X =
                    vfmaq_n_f16(vdupq_n_f16(colFirstX), vX, slopeX);
                tPointV.Y =
                    vfmaq_n_f16(vdupq_n_f16(colFirstY), vX, slopeY);

#if !defined(__ARM_2D_CFG_UNSAFE_IGNORE_CALIB_IN_ROTATION_FOR_PERFORMANCE__)
                tPointV.X = vaddq_m_n_f16(tPointV.X, tPointV.X, __CALIB, vcmpgtq(tPointV.X, 0));
                tPointV.X = vsubq_m_n_f16(tPointV.X, tPointV.X, __CALIB, vcmpleq(tPointV.X, 0));

                tPointV.Y = vaddq_m_n_f16(tPointV.Y, tPointV.Y, __CALIB, vcmpgtq(tPointV.Y, 0));
                tPointV.Y = vsubq_m_n_f16(tPointV.Y, tPointV.Y, __CALIB, vcmpleq(tPointV.Y, 0));
#endif
                __arm_2d_impl_rgb565_get_pixel_colour_with_alpha(&tPointV,
                                                                     &ptParam->tOrigin.
                                                                     tValidRegion,
                                                                     pOrigin, iOrigStride,
                                                                     pTargetBaseCur,
                                                                     MaskColour,
                                                                     hwRatioCompl,
                                                                     nbVecElts);
                pTargetBaseCur += 8;
                vX += 8.0f16;
                nbVecElts -= 8;
            }
            pTargetBase += iTargetStride;
        }
    } else {

        /*
            Large image / Large origin offsets
            Gather load 16-bit could overflow
                - Y offset needs to be shifted down to avoid overflow
                - 16-bit gather loads base address is incremented

            Needs to be done in the inner loop.
            In the case of steep slopes, taking the minimum between the Y extrema could still generate overflows
        */
        for (int32_t y = 0; y < iHeight; y++) {
            /* 1st column estimates (intercepts for regression in X direction */
            float16_t       colFirstY =
                (float16_t) (regrCoefs[0].slopeY * y + regrCoefs[0].interceptY);
            float16_t       colFirstX =
                (float16_t) (regrCoefs[0].slopeX * y + regrCoefs[0].interceptX);

            int32_t         nbVecElts = iWidth;
            float16x8_t     vX = vcvtq_f16_s16((int16x8_t) vidupq_n_u16(0, 1));
            uint16_t       *pTargetBaseCur = pTargetBase;

            while (nbVecElts > 0) {
                arm_2d_point_f16x8_t tPointV;

                /* linear interpolation thru first & last columns */
                tPointV.X =
                    vfmaq_n_f16(vdupq_n_f16(colFirstX), vX, slopeX);
                tPointV.Y =
                    vfmaq_n_f16(vdupq_n_f16(colFirstY), vX, slopeY);

#if !defined(__ARM_2D_CFG_UNSAFE_IGNORE_CALIB_IN_ROTATION_FOR_PERFORMANCE__)
                tPointV.X = vaddq_m_n_f16(tPointV.X, tPointV.X, __CALIB, vcmpgtq(tPointV.X, 0));
                tPointV.X = vsubq_m_n_f16(tPointV.X, tPointV.X, __CALIB, vcmpleq(tPointV.X, 0));

                tPointV.Y = vaddq_m_n_f16(tPointV.Y, tPointV.Y, __CALIB, vcmpgtq(tPointV.Y, 0));
                tPointV.Y = vsubq_m_n_f16(tPointV.Y, tPointV.Y, __CALIB, vcmpleq(tPointV.Y, 0));
#endif
                __arm_2d_impl_rgb565_get_pixel_colour_with_alpha_offs_compensated(&tPointV,
                                                                        &ptParam->tOrigin.
                                                                        tValidRegion,
                                                                        pOrigin,
                                                                        iOrigStride,
                                                                        pTargetBaseCur,
                                                                        MaskColour,
                                                                        hwRatioCompl,
                                                                        nbVecElts);
                pTargetBaseCur += 8;
                vX += 8.0f16;
                nbVecElts -= 8;
            }
            pTargetBase += iTargetStride;
        }
    }
}


/* untested */
__OVERRIDE_WEAK
void __arm_2d_impl_cccn888_rotate_alpha(   __arm_2d_param_copy_orig_t *ptParam,
                                    __arm_2d_rotate_info_t *ptInfo,
                                    uint_fast8_t chRatio)
{
    int32_t    iHeight = ptParam->use_as____arm_2d_param_copy_t.tCopySize.iHeight;
    int32_t    iWidth = ptParam->use_as____arm_2d_param_copy_t.tCopySize.iWidth;

    int32_t    iTargetStride =
        ptParam->use_as____arm_2d_param_copy_t.tTarget.iStride;
    uint32_t       *pTargetBase = ptParam->use_as____arm_2d_param_copy_t.tTarget.pBuffer;
    uint32_t       *pOrigin = ptParam->tOrigin.pBuffer;
    int32_t         iOrigStride = ptParam->tOrigin.iStride;
    uint32_t        MaskColour = ptInfo->Mask.hwColour;
    float           fAngle = -ptInfo->fAngle;
    arm_2d_location_t tOffset =
        ptParam->use_as____arm_2d_param_copy_t.tSource.tValidRegion.tLocation;
    uint16_t        wRatioCompl = 256 - chRatio;
    arm_2d_location_t *pCenter = &(ptInfo->tCenter);
    float                       invIWidth = 1.0f / (float) (iWidth - 1);
    arm_2d_rot_linear_regr_t    regrCoefs[2];
    arm_2d_location_t           SrcPt = ptInfo->tDummySourceOffset;

    /* get regression parameters over 1st and last column */
     __arm_2d_rotate_regression(&ptParam->use_as____arm_2d_param_copy_t.tCopySize,
                                   &SrcPt, fAngle, &tOffset, pCenter, iOrigStride,
                                   regrCoefs);


    /* slopes between 1st and last columns */
    float16_t           slopeY, slopeX;

    slopeY = (float16_t)(regrCoefs[1].interceptY - regrCoefs[0].interceptY) * invIWidth;
    slopeX = (float16_t)(regrCoefs[1].interceptX - regrCoefs[0].interceptX) * invIWidth;

    for (int32_t y = 0; y < iHeight; y++) {

        /* 1st column estimates (intercepts for regression in X direction */
        float16_t       colFirstY = regrCoefs[0].slopeY * y + regrCoefs[0].interceptY;
        float16_t       colFirstX = regrCoefs[0].slopeX * y + regrCoefs[0].interceptX;
        int32_t         nbVecElts = iWidth;
        float16x8_t     vX = vcvtq_f16_s16((int16x8_t) vidupq_n_u16(0, 1));
        uint32_t       *pTargetBaseCur = pTargetBase;

        while (nbVecElts > 0) {
            arm_2d_point_s16x8_t tPointV;

            tPointV.X = vcvtq_s16_f16(
                vfmaq_n_f16(vdupq_n_f16(colFirstX), vX, slopeX));
            tPointV.Y = vcvtq_s16_f16(
                vfmaq_n_f16(vdupq_n_f16(colFirstY), vX, slopeY));

#if !defined(__ARM_2D_CFG_UNSAFE_IGNORE_CALIB_IN_ROTATION_FOR_PERFORMANCE__)
            tPointV.X = vaddq_m_n_f16(tPointV.X, tPointV.X, __CALIB, vcmpgtq(tPointV.X, 0));
            tPointV.X = vsubq_m_n_f16(tPointV.X, tPointV.X, __CALIB, vcmpleq(tPointV.X, 0));

            tPointV.Y = vaddq_m_n_f16(tPointV.Y, tPointV.Y, __CALIB, vcmpgtq(tPointV.Y, 0));
            tPointV.Y = vsubq_m_n_f16(tPointV.Y, tPointV.Y, __CALIB, vcmpleq(tPointV.Y, 0));
#endif

            __arm_2d_impl_cccn888_get_pixel_colour_with_alpha_mve(&tPointV,
                                                                 &ptParam->
                                                                 tOrigin.tValidRegion,
                                                                 pOrigin, iOrigStride,
                                                                 pTargetBase, MaskColour,
                                                                 wRatioCompl, nbVecElts);
            pTargetBaseCur += 8;
            vX += 8.0f16;
            nbVecElts -= 8;
        }
        pTargetBase += iTargetStride;
    }
}

#else /* __ARM_2D_HAS_HELIUM_FLOAT__ && ! __ARM_2D_CFG_FORCED_FIXED_POINT_ROTATION__ */


/* extra calibration removed in fixed-point code since offset is lower than Q9.6 representation */

#define ONE_BY_2PI_Q31      341782637.0f
#define ARSHIFT(x, shift)   (shift > 0 ? x >> shift : x << (-shift))
#define TO_Q16(x)           ((x) << 16)
#define GET_Q6INT(x)        ((x) >> 6)
#define SET_Q6INT(x)        ((x) << 6)

/**
  Scale R, G & B channels
 */
#define __ARM_2D_SCALE_RGBVEC(vAvgPixelR, vAvgPixelG, vAvgPixelB, R, G, B, vScal)                       \
        vAvgPixelR = vmulhq_u16(vScal, R);                                                              \
        vAvgPixelG = vmulhq_u16(vScal, G);                                                              \
        vAvgPixelB = vmulhq_u16(vScal, B);
/**
  Scale R, G & B channels with accumulation
 */
#define __ARM_2D_SCALE_RGBVEC_ACC(vAvgPixelR, vAvgPixelG, vAvgPixelB, R, G, B, vScal)                    \
        vAvgPixelR += vmulhq_u16(vScal, R);                                                              \
        vAvgPixelG += vmulhq_u16(vScal, G);                                                              \
        vAvgPixelB += vmulhq_u16(vScal, B);


#ifdef VECTORIZED_ROTATION_REGR
/* disabled as slower than scalar */
static
bool __arm_2d_rotate_regression(arm_2d_size_t * __RESTRICT ptCopySize,
                                    arm_2d_location_t * pSrcPoint,
                                    float fAngle,
                                    arm_2d_location_t * tOffset,
                                    arm_2d_location_t * center,
                                    int32_t             iOrigStride,
                                    arm_2d_rot_linear_regr_t regrCoefs[]
    )
{
    int32_t         iHeight = ptCopySize->iHeight;
    int32_t         iWidth = ptCopySize->iWidth;
    q31_t           invHeightFx = 0x7fffffff / (iHeight - 1);
    arm_2d_point_s32x4_t vPointCornerI;
    int32_t         AngleFx = (int32_t) roundf(fAngle * ONE_BY_2PI_Q31);
    q31_t           cosAngleFx = arm_cos_q31(AngleFx);
    q31_t           sinAngleFx = arm_sin_q31(AngleFx);
    int32x4_t       vCornerX = { 0, 1, 0, 1 };
    int32x4_t       vCornerY = { 0, 0, 1, 1 };
    bool            gatherLoadIdxOverflow = 0;

    vPointCornerI.X = vdupq_n_s32(pSrcPoint->iX + tOffset->iX);
    vPointCornerI.X = vPointCornerI.X + vmulq_n_s32(vCornerX, (iWidth - 1));

    vPointCornerI.Y = vdupq_n_s32(pSrcPoint->iY + tOffset->iY);
    vPointCornerI.Y = vPointCornerI.Y + vmulq_n_s32(vCornerY, (iHeight - 1));

    /*
       Vector version of:

       int16_t         iX = ptLocation->iX - ptCenter->iX;
       int16_t         iY = ptLocation->iY - ptCenter->iY;

       q31_t           cosAngleFx = arm_cos_q31(fAngle);
       q31_t           sinAngleFx = arm_sin_q31(fAngle);
       tPointCornerFx[0][0].Y =
       __QDADD(__QDADD(centerQ16.Y, MULTFX(iYQ16, cosAngleFx)), MULTFX(iXQ16, sinAngleFx));
       tPointCornerFx[0][0].X =
       __QDSUB(__QDADD(centerQ16.X, MULTFX(iXQ16, cosAngleFx)), MULTFX(iYQ16, sinAngleFx));

     */

    arm_2d_point_s32x4_t vTmp1;

    vTmp1.X = vsubq_n_s16(vPointCornerI.X, center->iX);
    vTmp1.Y = vsubq_n_s16(vPointCornerI.Y, center->iY);
    vTmp1.X <<= 16;
    vTmp1.Y <<= 16;


    vPointCornerI.X =
        vqsubq(vqdmulhq_n_s32(vTmp1.X, cosAngleFx), vqdmulhq_n_s32(vTmp1.Y, sinAngleFx));
    vPointCornerI.X = vqaddq_n_s32(vPointCornerI.X, (center->iX << 16));

    vPointCornerI.Y = vqdmlahq(vqdmulhq_n_s32(vTmp1.X, sinAngleFx), vTmp1.Y, cosAngleFx);
    vPointCornerI.Y = vqaddq_n_s32(vPointCornerI.Y, (center->iY << 16));

    /*
       Check whether rotated index offsets could exceed 16-bit limits
       used in subsequent gather loads
       This will occur for parts of large images (e.g. 320*200)
       To avoid unconditional penalties for small/medium images,
       returns a speculative overflow allowing to handle large offsets.
    */
    int32_t maxY = vmaxvq(0.0f, vPointCornerI.Y);

    if(MULTFX(TO_Q16(iOrigStride), maxY) > UINT16_MAX)
        gatherLoadIdxOverflow = true;


    /* regression parameters */

    vTmp1.X[0] = vPointCornerI.X[0];
    vTmp1.X[1] = vPointCornerI.X[1];
    vTmp1.X[2] = vPointCornerI.Y[0];
    vTmp1.X[3] = vPointCornerI.Y[1];

    vTmp1.Y[0] = vPointCornerI.X[2];
    vTmp1.Y[1] = vPointCornerI.X[3];
    vTmp1.Y[2] = vPointCornerI.Y[2];
    vTmp1.Y[3] = vPointCornerI.Y[3];

    /* slopes */
    vTmp1.X = vqdmulhq_n_s32(vTmp1.Y - vTmp1.X, invHeightFx);

    regrCoefs[0].slopeY = vTmp1.X[2];
    regrCoefs[0].slopeX = vTmp1.X[0];
    regrCoefs[0].interceptY = vPointCornerI.Y[0];
    regrCoefs[0].interceptX = vPointCornerI.X[0];

    regrCoefs[1].slopeY = vTmp1.X[3];
    regrCoefs[1].slopeX = vTmp1.X[1];
    regrCoefs[1].interceptY = vPointCornerI.Y[1];
    regrCoefs[1].interceptX = vPointCornerI.X[1];

    return gatherLoadIdxOverflow;
}

#else

static
bool __arm_2d_rotate_regression(arm_2d_size_t * __RESTRICT ptCopySize,
                                            arm_2d_location_t * pSrcPoint,
                                            float fAngle,
                                            arm_2d_location_t * tOffset,
                                            arm_2d_location_t * center,
                                            int32_t             iOrigStride,
                                            arm_2d_rot_linear_regr_t regrCoefs[]
    )
{
    int_fast16_t        iHeight = ptCopySize->iHeight;
    int_fast16_t        iWidth = ptCopySize->iWidth;
    q31_t               invHeightFx = 0x7fffffff / (iHeight - 1);
    int32_t             AngleFx = lroundf(fAngle * ONE_BY_2PI_Q31);
    q31_t               cosAngleFx = arm_cos_q31(AngleFx);
    q31_t               sinAngleFx = arm_sin_q31(AngleFx);
    arm_2d_point_fx_t   tPointCornerFx[2][2];
    arm_2d_point_fx_t   centerQ16;
    arm_2d_point_fx_t   srcPointQ16;
    arm_2d_point_fx_t   tOffsetQ16;
    arm_2d_point_fx_t   tmp;
    int32_t             iXQ16, iYQ16;
    bool                gatherLoadIdxOverflow = 0;

    /* Q16 conversion */
    centerQ16.X = TO_Q16(center->iX);
    centerQ16.Y = TO_Q16(center->iY);

    srcPointQ16.X = TO_Q16(pSrcPoint->iX);
    srcPointQ16.Y = TO_Q16(pSrcPoint->iY);

    tOffsetQ16.X = TO_Q16(tOffset->iX);
    tOffsetQ16.Y = TO_Q16(tOffset->iY);


    /* (0,0) corner */
    tmp.X = srcPointQ16.X + 0 + tOffsetQ16.X;
    tmp.Y = srcPointQ16.Y + 0 + tOffsetQ16.Y;

    iXQ16 = tmp.X - centerQ16.X;
    iYQ16 = tmp.Y - centerQ16.Y;

    tPointCornerFx[0][0].Y =
        __QDADD(__QDADD(centerQ16.Y, MULTFX(iYQ16, cosAngleFx)), MULTFX(iXQ16, sinAngleFx));
    tPointCornerFx[0][0].X =
        __QDSUB(__QDADD(centerQ16.X, MULTFX(iXQ16, cosAngleFx)), MULTFX(iYQ16, sinAngleFx));


    /* ((iWidth - 1),0) corner */
    tmp.X = srcPointQ16.X + 0 + tOffsetQ16.X + TO_Q16(iWidth - 1);
    iXQ16 = tmp.X - centerQ16.X;

    tPointCornerFx[1][0].Y =
        __QDADD(__QDADD(centerQ16.Y, MULTFX(iYQ16, cosAngleFx)), MULTFX(iXQ16, sinAngleFx));
    tPointCornerFx[1][0].X =
        __QDSUB(__QDADD(centerQ16.X, MULTFX(iXQ16, cosAngleFx)), MULTFX(iYQ16, sinAngleFx));


    /* ((iWidth - 1),(iHeight - 1)) corner */
    tmp.Y = srcPointQ16.Y + tOffsetQ16.Y + TO_Q16(iHeight - 1);
    iYQ16 = tmp.Y - centerQ16.Y;

    tPointCornerFx[1][1].Y =
        __QDADD(__QDADD(centerQ16.Y, MULTFX(iYQ16, cosAngleFx)), MULTFX(iXQ16, sinAngleFx));
    tPointCornerFx[1][1].X =
        __QDSUB(__QDADD(centerQ16.X, MULTFX(iXQ16, cosAngleFx)), MULTFX(iYQ16, sinAngleFx));


    /* (0,(iHeight - 1)) corner */
    tmp.X = srcPointQ16.X + 0 + tOffsetQ16.X;
    iXQ16 = tmp.X - centerQ16.X;

    tPointCornerFx[0][1].Y =
        __QDADD(__QDADD(centerQ16.Y, MULTFX(iYQ16, cosAngleFx)), MULTFX(iXQ16, sinAngleFx));
    tPointCornerFx[0][1].X =
        __QDSUB(__QDADD(centerQ16.X, MULTFX(iXQ16, cosAngleFx)), MULTFX(iYQ16, sinAngleFx));
    /*
       Check whether rotated index offsets could exceed 16-bit limits
       used in subsequent gather loads
       This will occur for parts of large images (e.g. 320*200)
       To avoid unconditional penalties for small/medium images,
       returns a speculative overflow allowing to handle large offsets.
    */
    int32_t maxY = MAX(MAX
                        (MAX(tPointCornerFx[0][0].Y, tPointCornerFx[0][1].Y),
                            tPointCornerFx[1][0].Y),
                                tPointCornerFx[1][1].Y);

    if(MULTFX(TO_Q16(iOrigStride), maxY) > UINT16_MAX)
        gatherLoadIdxOverflow = true;


    /* regression */
    int32_t           slopeXFx, slopeYFx;

    /* interpolation in Y direction for 1st elements column */
    slopeXFx = MULTFX((tPointCornerFx[0][1].X - tPointCornerFx[0][0].X), invHeightFx);
    slopeYFx = MULTFX((tPointCornerFx[0][1].Y - tPointCornerFx[0][0].Y), invHeightFx);

    regrCoefs[0].slopeY = slopeYFx * 2;
    regrCoefs[0].slopeX = slopeXFx * 2;
    regrCoefs[0].interceptY = tPointCornerFx[0][0].Y;
    regrCoefs[0].interceptX = tPointCornerFx[0][0].X;


    /* interpolation in Y direction for the last elements column */
    slopeXFx = MULTFX((tPointCornerFx[1][1].X - tPointCornerFx[1][0].X), invHeightFx);
    slopeYFx = MULTFX((tPointCornerFx[1][1].Y - tPointCornerFx[1][0].Y), invHeightFx);

    regrCoefs[1].slopeY = slopeYFx* 2;
    regrCoefs[1].slopeX = slopeXFx* 2;
    regrCoefs[1].interceptY = tPointCornerFx[1][0].Y;
    regrCoefs[1].interceptX = tPointCornerFx[1][0].X;

    return gatherLoadIdxOverflow;
}

#endif

static
void __arm_2d_impl_rgb565_get_pixel_colour(   arm_2d_point_s16x8_t *ptPoint,
                                            arm_2d_region_t *ptOrigValidRegion,
                                            uint16_t *pOrigin,
                                            int16_t iOrigStride,
                                            uint16_t *pTarget,
                                            uint16_t MaskColour,
                                            uint32_t elts)
{
    mve_pred16_t    predTail = vctp16q(elts);
    uint16x8_t      vTarget = vld1q(pTarget);

#if defined(__ARM_2D_HAS_INTERPOLATION_ROTATION__) &&  __ARM_2D_HAS_INTERPOLATION_ROTATION__
    int16x8_t       vOne = vdupq_n_s16(SET_Q6INT(1));
    int16x8_t       vXi = GET_Q6INT(ptPoint->X);
    int16x8_t       vYi = GET_Q6INT(ptPoint->Y);

    vXi = vsubq_m_n_s16(vXi, vXi, 1, vcmpltq_n_s16(ptPoint->X, 0));
    vYi = vsubq_m_n_s16(vYi, vYi, 1, vcmpltq_n_s16(ptPoint->Y, 0));

    int16x8_t       vWX = ptPoint->X - SET_Q6INT(vXi);
    int16x8_t       vWY = ptPoint->Y - SET_Q6INT(vYi);

    /* combination of Bottom / Top & Left / Right areas contributions */
    uint16x8_t      vAreaTR = vmulq_u16(vWX, vWY);
    uint16x8_t      vAreaTL = vmulq_u16((vOne - vWX), vWY);
    uint16x8_t      vAreaBR = vmulq_u16(vWX, (vOne - vWY));
    uint16x8_t      vAreaBL = vmulq_u16((vOne - vWX), (vOne - vWY));

    /* Q16 conversion */
    vAreaTR = vqshlq_n_u16(vAreaTR, 4);
    vAreaTL = vqshlq_n_u16(vAreaTL, 4);
    vAreaBR = vqshlq_n_u16(vAreaBR, 4);
    vAreaBL = vqshlq_n_u16(vAreaBL, 4);


    /* accumulated pixel vectors */
    uint16x8_t       vAvgPixelR, vAvgPixelG, vAvgPixelB;

    /* predicate accumulator */
    /* tracks all predications conditions for selecting final */
    /* averaged pixed / target pixel */
    /* can probably optimized away since averaged target pixel is */
    /* equivalent to original pixel, but precision limitations would introduce small errors */
    mve_pred16_t    predGlb = 0;

    /*
     * accumulate / average over the 4 neigbouring pixels
     */

    uint16x8_t      R, G, B;
    /* Bottom Left averaging */
    {
        __ARM_2D_RGB565_GET_RGBVEC_FROM_POINT(vXi, vYi, R, G, B, MaskColour, predGlb);

        __ARM_2D_SCALE_RGBVEC(vAvgPixelR, vAvgPixelG, vAvgPixelB, R, G, B, vAreaBL);
    }


    /* Bottom Right averaging */
    {
        __ARM_2D_RGB565_GET_RGBVEC_FROM_POINT(vaddq_n_s16(vXi, 1), vYi, R, G, B, MaskColour, predGlb);

        __ARM_2D_SCALE_RGBVEC_ACC(vAvgPixelR, vAvgPixelG, vAvgPixelB, R, G, B, vAreaBR);
    }

    /* Top Left averaging */
    {
        __ARM_2D_RGB565_GET_RGBVEC_FROM_POINT(vXi, vaddq_n_s16(vYi, 1), R, G, B, MaskColour, predGlb);

        __ARM_2D_SCALE_RGBVEC_ACC(vAvgPixelR, vAvgPixelG, vAvgPixelB, R, G, B, vAreaTL);
    }

    /* Top Right averaging */
    {
        __ARM_2D_RGB565_GET_RGBVEC_FROM_POINT(vaddq_n_s16(vXi, 1), vaddq_n_s16(vYi, 1),
                                                                R, G, B, MaskColour, predGlb);

        __ARM_2D_SCALE_RGBVEC_ACC(vAvgPixelR, vAvgPixelG, vAvgPixelB, R, G, B, vAreaTR);
    }

    /* pack */
    uint16x8_t      TempPixel = __arm_2d_rgb565_pack_single_vec(vAvgPixelR,
                                                              vAvgPixelG,
                                                              vAvgPixelB);

    /* select between target pixel, averaged pixed */
    vTarget = vpselq_u16(TempPixel, vTarget, predGlb);


#else
    /* extract integer part */
    arm_2d_point_s16x8_t vPoint = {
            .X = GET_Q6INT(ptPoint->X),
            .Y = GET_Q6INT(ptPoint->Y)};

    /* set vector predicate if point is inside the region */
    mve_pred16_t    p = arm_2d_is_point_vec_inside_region_s16(ptOrigValidRegion, &vPoint);
    /* prepare vector of point offsets */
    uint16x8_t      ptOffs = vPoint.X + vPoint.Y * iOrigStride;
    /* retrieve all point values */
    uint16x8_t      ptVal = vldrhq_gather_shifted_offset_z_u16(pOrigin, ptOffs, predTail);

    /* combine 2 predicates set to true if point is in the region & values different from color mask */
    vTarget = vpselq_u16(ptVal, vTarget, vcmpneq_m_n_u16(ptVal, MaskColour, p));

#endif
    vst1q_p(pTarget, vTarget, predTail);
}

static
void __arm_2d_impl_rgb565_get_pixel_colour_offs_compensated(   arm_2d_point_s16x8_t *ptPoint,
                                            arm_2d_region_t    *ptOrigValidRegion,
                                            uint16_t           *pOrigin,
                                            int16_t             iOrigStride,
                                            uint16_t           *pTarget,
                                            uint16_t            MaskColour,
                                            uint32_t            elts)
{
    mve_pred16_t    predTail = vctp16q(elts);
    uint16x8_t      vTarget = vld1q(pTarget);

#if defined(__ARM_2D_HAS_INTERPOLATION_ROTATION__) &&  __ARM_2D_HAS_INTERPOLATION_ROTATION__
    int16x8_t       vOne = vdupq_n_s16(SET_Q6INT(1));
    int16x8_t       vXi = GET_Q6INT(ptPoint->X);
    int16x8_t       vYi = GET_Q6INT(ptPoint->Y);

    vXi = vsubq_m_n_s16(vXi, vXi, 1, vcmpltq_n_s16(ptPoint->X, 0));
    vYi = vsubq_m_n_s16(vYi, vYi, 1, vcmpltq_n_s16(ptPoint->Y, 0));

    int16x8_t       vWX = ptPoint->X - SET_Q6INT(vXi);
    int16x8_t       vWY = ptPoint->Y - SET_Q6INT(vYi);

    /* combination of Bottom / Top & Left / Right areas contributions */
    uint16x8_t      vAreaTR = vmulq_u16(vWX, vWY);
    uint16x8_t      vAreaTL = vmulq_u16((vOne - vWX), vWY);
    uint16x8_t      vAreaBR = vmulq_u16(vWX, (vOne - vWY));
    uint16x8_t      vAreaBL = vmulq_u16((vOne - vWX), (vOne - vWY));

    /* Q16 conversion */
    vAreaTR = vqshlq_n_u16(vAreaTR, 4);
    vAreaTL = vqshlq_n_u16(vAreaTL, 4);
    vAreaBR = vqshlq_n_u16(vAreaBR, 4);
    vAreaBL = vqshlq_n_u16(vAreaBL, 4);


    /* accumulated pixel vectors */
    uint16x8_t       vAvgPixelR, vAvgPixelG, vAvgPixelB;

    /* predicate accumulator */
    /* tracks all predications conditions for selecting final */
    /* averaged pixed / target pixel */
    /* can probably optimized away since averaged target pixel is */
    /* equivalent to original pixel, but precision limitations would introduce small errors */
    mve_pred16_t    predGlb = 0;

    /*
     * accumulate / average over the 4 neigbouring pixels
     */

    uint16x8_t      R, G, B;
    /* Bottom Left averaging */
    {
        __ARM_2D_RGB565_GET_RGBVEC_FROM_POINT_FAR(vXi, vYi, R, G, B, MaskColour, predGlb);

        __ARM_2D_SCALE_RGBVEC(vAvgPixelR, vAvgPixelG, vAvgPixelB, R, G, B, vAreaBL);
    }


    /* Bottom Right averaging */
    {
        __ARM_2D_RGB565_GET_RGBVEC_FROM_POINT_FAR(vaddq_n_s16(vXi, 1), vYi, R, G, B, MaskColour, predGlb);

        __ARM_2D_SCALE_RGBVEC_ACC(vAvgPixelR, vAvgPixelG, vAvgPixelB, R, G, B, vAreaBR);
    }

    /* Top Left averaging */
    {
        __ARM_2D_RGB565_GET_RGBVEC_FROM_POINT_FAR(vXi, vaddq_n_s16(vYi, 1), R, G, B, MaskColour, predGlb);

        __ARM_2D_SCALE_RGBVEC_ACC(vAvgPixelR, vAvgPixelG, vAvgPixelB, R, G, B, vAreaTL);
    }

    /* Top Right averaging */
    {
        __ARM_2D_RGB565_GET_RGBVEC_FROM_POINT_FAR(vaddq_n_s16(vXi, 1), vaddq_n_s16(vYi, 1),
                                                                R, G, B, MaskColour, predGlb);

        __ARM_2D_SCALE_RGBVEC_ACC(vAvgPixelR, vAvgPixelG, vAvgPixelB, R, G, B, vAreaTR);
    }

    /* pack */
    uint16x8_t      TempPixel = __arm_2d_rgb565_pack_single_vec(vAvgPixelR,
                                                              vAvgPixelG,
                                                              vAvgPixelB);

    /* select between target pixel, averaged pixed */
    vTarget = vpselq_u16(TempPixel, vTarget, predGlb);


#else
    /* extract integer part */
    arm_2d_point_s16x8_t vPoint = {
            .X = GET_Q6INT(ptPoint->X),
            .Y = GET_Q6INT(ptPoint->Y)};

    /* set vector predicate if point is inside the region */
    mve_pred16_t    p = arm_2d_is_point_vec_inside_region_s16(ptOrigValidRegion, &vPoint);

    /* prepare vector of point offsets */
    /* correctionOffset avoid 16-bit overflow */
    int16_t         correctionOffset = vminvq_s16(0x7fff, vPoint.Y) - 1;
    uint16x8_t      ptOffs =
        vPoint.X + (vPoint.Y - correctionOffset) * iOrigStride;

    /* base pointer update to compensate offset */
    pOrigin += (correctionOffset * iOrigStride);

    /* retrieve all point values */
    uint16x8_t      ptVal = vldrhq_gather_shifted_offset_z_u16(pOrigin, ptOffs, predTail);

    /* combine 2 predicates set to true if point is in the region & values different from color mask */
    vTarget = vpselq_u16(ptVal, vTarget, vcmpneq_m_n_u16(ptVal, MaskColour, p));
#endif

    vst1q_p(pTarget, vTarget, predTail);
}



static
void __arm_2d_impl_rgb565_get_pixel_colour_with_alpha(
                                            arm_2d_point_s16x8_t    *ptPoint,
                                            arm_2d_region_t         *ptOrigValidRegion,
                                            uint16_t                *pOrigin,
                                            int16_t                  iOrigStride,
                                            uint16_t                *pTarget,
                                            uint16_t                 MaskColour,
                                            uint8_t                  chOpacity,
                                            uint32_t                 elts)
{
    mve_pred16_t    predTail = vctp16q(elts);
    uint16x8_t      vTarget = vld1q(pTarget);

#if defined(__ARM_2D_HAS_INTERPOLATION_ROTATION__) &&  __ARM_2D_HAS_INTERPOLATION_ROTATION__
    int16x8_t       vOne = vdupq_n_s16(SET_Q6INT(1));
    int16x8_t       vXi = GET_Q6INT(ptPoint->X);
    int16x8_t       vYi = GET_Q6INT(ptPoint->Y);

    vXi = vsubq_m_n_s16(vXi, vXi, 1, vcmpltq_n_s16(ptPoint->X, 0));
    vYi = vsubq_m_n_s16(vYi, vYi, 1, vcmpltq_n_s16(ptPoint->Y, 0));

    int16x8_t       vWX = ptPoint->X - SET_Q6INT(vXi);
    int16x8_t       vWY = ptPoint->Y - SET_Q6INT(vYi);

    /* combination of Bottom / Top & Left / Right areas contributions */
    uint16x8_t      vAreaTR = vmulq_u16(vWX, vWY);
    uint16x8_t      vAreaTL = vmulq_u16((vOne - vWX), vWY);
    uint16x8_t      vAreaBR = vmulq_u16(vWX, (vOne - vWY));
    uint16x8_t      vAreaBL = vmulq_u16((vOne - vWX), (vOne - vWY));

    /* Q16 conversion */
    vAreaTR = vqshlq_n_u16(vAreaTR, 4);
    vAreaTL = vqshlq_n_u16(vAreaTL, 4);
    vAreaBR = vqshlq_n_u16(vAreaBR, 4);
    vAreaBL = vqshlq_n_u16(vAreaBL, 4);


    /* accumulated pixel vectors */
    uint16x8_t       vAvgPixelR, vAvgPixelG, vAvgPixelB;

    /* predicate accumulator */
    /* tracks all predications conditions for selecting final */
    /* averaged pixed / target pixel */
    /* can probably optimized away since averaged target pixel is */
    /* equivalent to original pixel, but precision limitations would introduce small errors */
    mve_pred16_t    predGlb = 0;

    /*
     * accumulate / average over the 4 neigbouring pixels
     */

    uint16x8_t      R, G, B;
    /* Bottom Left averaging */
    {
        __ARM_2D_RGB565_GET_RGBVEC_FROM_POINT(vXi, vYi, R, G, B, MaskColour, predGlb);

        __ARM_2D_SCALE_RGBVEC(vAvgPixelR, vAvgPixelG, vAvgPixelB, R, G, B, vAreaBL);
    }


    /* Bottom Right averaging */
    {
        __ARM_2D_RGB565_GET_RGBVEC_FROM_POINT(vaddq_n_s16(vXi, 1), vYi, R, G, B, MaskColour, predGlb);

        __ARM_2D_SCALE_RGBVEC_ACC(vAvgPixelR, vAvgPixelG, vAvgPixelB, R, G, B, vAreaBR);
    }

    /* Top Left averaging */
    {
        __ARM_2D_RGB565_GET_RGBVEC_FROM_POINT(vXi, vaddq_n_s16(vYi, 1), R, G, B, MaskColour, predGlb);

        __ARM_2D_SCALE_RGBVEC_ACC(vAvgPixelR, vAvgPixelG, vAvgPixelB, R, G, B, vAreaTL);
    }

    /* Top Right averaging */
    {
        __ARM_2D_RGB565_GET_RGBVEC_FROM_POINT(vaddq_n_s16(vXi, 1), vaddq_n_s16(vYi, 1),
                                                                R, G, B, MaskColour, predGlb);

        __ARM_2D_SCALE_RGBVEC_ACC(vAvgPixelR, vAvgPixelG, vAvgPixelB, R, G, B, vAreaTR);
    }


    /* alpha blending */
    uint16x8_t      vBlended;

    __ARM_2D_BLEND_RGB565_TARGET_RGBVEC(chOpacity, vTarget, vAvgPixelR, vAvgPixelG, vAvgPixelB, vBlended);


    /* select between target pixel, averaged pixed */
    vTarget = vpselq_u16(vBlended, vTarget, predGlb);

#else
    /* extract integer part */
    arm_2d_point_s16x8_t vPoint = {
            .X = GET_Q6INT(ptPoint->X),
            .Y = GET_Q6INT(ptPoint->Y)};

    /* set vector predicate if point is inside the region */
    mve_pred16_t    p = arm_2d_is_point_vec_inside_region_s16(ptOrigValidRegion, &vPoint);
    /* prepare vector of point offsets */
    uint16x8_t      ptOffs = vPoint.X + vPoint.Y * iOrigStride;
    /* retrieve all point values */
    uint16x8_t      ptVal = vldrhq_gather_shifted_offset_u16(pOrigin, ptOffs);

    /* alpha blending */
    uint16x8_t      vBlended =
        __arm_2d_rgb565_alpha_blending_single_vec(ptVal, vTarget, chOpacity);

    /* combine 2 predicates, set to true, if point is in the region & values different from color mask */
    vTarget = vpselq_u16(vBlended, vTarget, vcmpneq_m_n_u16(ptVal, MaskColour, p));
#endif

    vst1q_p(pTarget, vTarget, predTail);
}


static
void __arm_2d_impl_rgb565_get_pixel_colour_with_alpha_offs_compensated(
                                            arm_2d_point_s16x8_t    *ptPoint,
                                            arm_2d_region_t         *ptOrigValidRegion,
                                            uint16_t                *pOrigin,
                                            int16_t                  iOrigStride,
                                            uint16_t                *pTarget,
                                            uint16_t                 MaskColour,
                                            uint8_t                  chOpacity,
                                            uint32_t                 elts)
{
    mve_pred16_t    predTail = vctp16q(elts);
    uint16x8_t      vTarget = vld1q(pTarget);

#if defined(__ARM_2D_HAS_INTERPOLATION_ROTATION__) &&  __ARM_2D_HAS_INTERPOLATION_ROTATION__
    int16x8_t       vOne = vdupq_n_s16(SET_Q6INT(1));
    int16x8_t       vXi = GET_Q6INT(ptPoint->X);
    int16x8_t       vYi = GET_Q6INT(ptPoint->Y);

    vXi = vsubq_m_n_s16(vXi, vXi, 1, vcmpltq_n_s16(ptPoint->X, 0));
    vYi = vsubq_m_n_s16(vYi, vYi, 1, vcmpltq_n_s16(ptPoint->Y, 0));

    int16x8_t       vWX = ptPoint->X - SET_Q6INT(vXi);
    int16x8_t       vWY = ptPoint->Y - SET_Q6INT(vYi);

    /* combination of Bottom / Top & Left / Right areas contributions */
    uint16x8_t      vAreaTR = vmulq_u16(vWX, vWY);
    uint16x8_t      vAreaTL = vmulq_u16((vOne - vWX), vWY);
    uint16x8_t      vAreaBR = vmulq_u16(vWX, (vOne - vWY));
    uint16x8_t      vAreaBL = vmulq_u16((vOne - vWX), (vOne - vWY));

    /* Q16 conversion */
    vAreaTR = vqshlq_n_u16(vAreaTR, 4);
    vAreaTL = vqshlq_n_u16(vAreaTL, 4);
    vAreaBR = vqshlq_n_u16(vAreaBR, 4);
    vAreaBL = vqshlq_n_u16(vAreaBL, 4);


    /* accumulated pixel vectors */
    uint16x8_t       vAvgPixelR, vAvgPixelG, vAvgPixelB;

    /* predicate accumulator */
    /* tracks all predications conditions for selecting final */
    /* averaged pixed / target pixel */
    /* can probably optimized away since averaged target pixel is */
    /* equivalent to original pixel, but precision limitations would introduce small errors */
    mve_pred16_t    predGlb = 0;

    /*
     * accumulate / average over the 4 neigbouring pixels
     */

    uint16x8_t      R, G, B;
    /* Bottom Left averaging */
    {
        __ARM_2D_RGB565_GET_RGBVEC_FROM_POINT_FAR(vXi, vYi, R, G, B, MaskColour, predGlb);

        __ARM_2D_SCALE_RGBVEC(vAvgPixelR, vAvgPixelG, vAvgPixelB, R, G, B, vAreaBL);
    }


    /* Bottom Right averaging */
    {
        __ARM_2D_RGB565_GET_RGBVEC_FROM_POINT_FAR(vaddq_n_s16(vXi, 1), vYi, R, G, B, MaskColour, predGlb);

        __ARM_2D_SCALE_RGBVEC_ACC(vAvgPixelR, vAvgPixelG, vAvgPixelB, R, G, B, vAreaBR);
    }

    /* Top Left averaging */
    {
        __ARM_2D_RGB565_GET_RGBVEC_FROM_POINT_FAR(vXi, vaddq_n_s16(vYi, 1), R, G, B, MaskColour, predGlb);

        __ARM_2D_SCALE_RGBVEC_ACC(vAvgPixelR, vAvgPixelG, vAvgPixelB, R, G, B, vAreaTL);
    }

    /* Top Right averaging */
    {
        __ARM_2D_RGB565_GET_RGBVEC_FROM_POINT_FAR(vaddq_n_s16(vXi, 1), vaddq_n_s16(vYi, 1),
                                                                R, G, B, MaskColour, predGlb);

        __ARM_2D_SCALE_RGBVEC_ACC(vAvgPixelR, vAvgPixelG, vAvgPixelB, R, G, B, vAreaTR);
    }

    /* alpha blending */
    uint16x8_t      vBlended;

    __ARM_2D_BLEND_RGB565_TARGET_RGBVEC(chOpacity, vTarget, vAvgPixelR, vAvgPixelG, vAvgPixelB, vBlended);



    /* select between target pixel, averaged pixed */
    vTarget = vpselq_u16(vBlended, vTarget, predGlb);

#else
    /* extract integer part */
    arm_2d_point_s16x8_t vPoint = {
            .X = GET_Q6INT(ptPoint->X),
            .Y = GET_Q6INT(ptPoint->Y)};

    /* set vector predicate if point is inside the region */
    mve_pred16_t    p = arm_2d_is_point_vec_inside_region_s16(ptOrigValidRegion, &vPoint);
    /* prepare vector of point offsets */
    /* correctionOffset avoid 16-bit overflow */
    int16_t         correctionOffset = vminvq_s16(0x7fff, vPoint.Y) - 1;
    uint16x8_t      ptOffs =
        vPoint.X + (vPoint.Y - correctionOffset) * iOrigStride;

    /* retrieve all point values */
    /* base pointer update to compensate offset */
    pOrigin += (correctionOffset * iOrigStride);

    uint16x8_t      ptVal = vldrhq_gather_shifted_offset_z_u16(pOrigin, ptOffs, predTail);

    /* alpha blending */
    uint16x8_t      vBlended =
        __arm_2d_rgb565_alpha_blending_single_vec(ptVal, vTarget, chOpacity);

    /* combine 2 predicates, set to true, if point is in the region & values different from color mask */
    vTarget = vpselq_u16(vBlended, vTarget, vcmpneq_m_n_u16(ptVal, MaskColour, p));

#endif

    vst1q_p(pTarget, vTarget, predTail);
}

static
void __arm_2d_impl_cccn888_get_pixel_colour_mve(   arm_2d_point_s16x8_t *ptPoint,
                                            arm_2d_region_t *ptOrigValidRegion,
                                            uint32_t *pOrigin,
                                            int16_t iOrigStride,
                                            uint32_t *pTarget,
                                            uint32_t MaskColour,
                                            int16_t elts)
{
#if     defined(__ARM_2D_HAS_INTERPOLATION_ROTATION__) &&  __ARM_2D_HAS_INTERPOLATION_ROTATION__
    ARM_ALIGN(8) uint32_t scratch32[32];
    int16_t        *pscratch16 = (int16_t *) scratch32;
    uint32x4_t      vTargetLo = vld1q(pTarget);
    uint32x4_t      vTargetHi = vld1q(pTarget + 4);
    mve_pred16_t    predTailLow = vctp32q(elts);
    mve_pred16_t    predTailHigh = elts - 4 > 0 ? vctp32q(elts - 4) : 0;
    int16x8_t       vOne = vdupq_n_s16(SET_Q6INT(1));

    int16x8_t       vXi = GET_Q6INT(ptPoint->X);
    int16x8_t       vYi = GET_Q6INT(ptPoint->Y);

    vXi = vsubq_m_n_s16(vXi, vXi, 1, vcmpltq_n_s16(ptPoint->X, 0));
    vYi = vsubq_m_n_s16(vYi, vYi, 1, vcmpltq_n_s16(ptPoint->Y, 0));

    int16x8_t       vWX = ptPoint->X - (SET_Q6INT(vXi));
    int16x8_t       vWY = ptPoint->Y - (SET_Q6INT(vYi));

    /* combination of Bottom / Top & Left / Right areas contributions */
    uint16x8_t      vAreaTR = vmulq_u16(vWX, vWY);
    uint16x8_t      vAreaTL = vmulq_u16((vOne - vWX), vWY);
    uint16x8_t      vAreaBR = vmulq_u16(vWX, (vOne - vWY));
    uint16x8_t      vAreaBL = vmulq_u16((vOne - vWX), (vOne - vWY));

    /* Q16 conversion */
    vAreaTR = vqshlq_n_u16(vAreaTR, 4);
    vAreaTL = vqshlq_n_u16(vAreaTL, 4);
    vAreaBR = vqshlq_n_u16(vAreaBR, 4);
    vAreaBL = vqshlq_n_u16(vAreaBL, 4);


    /* accumulated pixel vectors */
    uint16x8_t      vAvgPixelR, vAvgPixelG, vAvgPixelB;

    /* predicate accumulators */
    /* tracks all predications conditions for selecting final */
    /* averaged pixed / target pixel */
    /* can probably optimized away since averaged target pixel is */
    /* equivalent to original pixel, but precision limitations would introduce small errors */
    mve_pred16_t    predGlbLo = 0, predGlbHi = 0;

    /*
     * accumulate / average over the 4 neigbouring pixels
     */

    uint16x8_t      R, G, B;

    /* Bottom Left averaging */
    {
        __ARM_2D_RGB888_GET_RGBVEC_FROM_POINT(vXi, vYi, R, G, B, MaskColour, predGlbLo, predGlbHi);

        __ARM_2D_SCALE_RGBVEC(vAvgPixelR, vAvgPixelG, vAvgPixelB, R, G, B, vAreaBL);
    }

    /* Bottom Right averaging */
    {
        __ARM_2D_RGB888_GET_RGBVEC_FROM_POINT(vaddq_n_s16(vXi, 1), vYi,
                            R, G, B, MaskColour, predGlbLo, predGlbHi);

        __ARM_2D_SCALE_RGBVEC_ACC(vAvgPixelR, vAvgPixelG, vAvgPixelB, R, G, B, vAreaBR);
    }

    /* Top Left averaging */
    {
        __ARM_2D_RGB888_GET_RGBVEC_FROM_POINT(vXi, vaddq_n_s16(vYi, 1),
                                                R, G, B, MaskColour, predGlbLo, predGlbHi);

        __ARM_2D_SCALE_RGBVEC_ACC(vAvgPixelR, vAvgPixelG, vAvgPixelB, R, G, B, vAreaTL);
    }

    /* Top Right averaging */
    {
        __ARM_2D_RGB888_GET_RGBVEC_FROM_POINT(vaddq_n_s16(vXi, 1), vaddq_n_s16(vYi, 1),
                                                R, G, B, MaskColour, predGlbLo, predGlbHi);

        __ARM_2D_SCALE_RGBVEC_ACC(vAvgPixelR, vAvgPixelG, vAvgPixelB, R, G, B, vAreaTR);
    }


    /* pack */
    __arm_2d_pack_rgb888_to_mem((uint8_t *) scratch32, vAvgPixelR, vAvgPixelG, vAvgPixelB);

    uint32x4_t      TempPixel = vld1q(scratch32);

    /* select between target pixel, averaged pixed */
    TempPixel = vpselq_u32(TempPixel, vTargetLo, predGlbLo);

    vst1q_p(pTarget, TempPixel, predTailLow);

    TempPixel = vld1q(scratch32 + 4);

    /* select between target pixel, averaged pixed */
    TempPixel = vpselq_u32(TempPixel, vTargetHi, predGlbHi);

    vst1q_p(pTarget + 4, TempPixel, predTailHigh);
#else

    arm_2d_point_s32x4_t    tPointLo, tPointHi;
    ARM_ALIGN(8)  int16_t         scratch[8];
    mve_pred16_t            p;

    /* split 16-bit point vector into 2 x 32-bit vectors */
    vst1q(scratch, GET_Q6INT(ptPoint->X));
    tPointLo.X = vldrhq_s32(scratch);
    tPointHi.X = vldrhq_s32(scratch + 4);

    vst1q(scratch, GET_Q6INT(ptPoint->Y));
    tPointLo.Y = vldrhq_s32(scratch);
    tPointHi.Y = vldrhq_s32(scratch + 4);

    /* 1st half */

    /* set vector predicate if point is inside the region */
    p = arm_2d_is_point_vec_inside_region_s32(ptOrigValidRegion, &tPointLo);
    /* prepare vector of point offsets */
    uint32x4_t      ptOffs = tPointLo.X + tPointLo.Y * iOrigStride;
    uint32x4_t      vPixel = vld1q(pTarget);
    /* retrieve all point values */
    uint32x4_t      ptVal = vldrwq_gather_shifted_offset_u32(pOrigin, ptOffs);

    /* combine 2 predicates set to true if point is in the region & values different from color mask */
    vPixel = vpselq_u32(ptVal, vPixel, vcmpneq_m_n_u32(ptVal, MaskColour, p));

    vst1q_p(pTarget, vPixel, vctp32q(elts));

    elts -= 4;
    if (elts > 0) {

        /* second half */
        p = arm_2d_is_point_vec_inside_region_s32(ptOrigValidRegion, &tPointHi);
        ptOffs = tPointHi.X + tPointHi.Y * iOrigStride;
        vPixel = vld1q(pTarget + 4);

        ptVal = vldrwq_gather_shifted_offset_u32(pOrigin, ptOffs);
        vPixel = vpselq_u32(ptVal, vPixel, vcmpneq_m_n_u32(ptVal, MaskColour, p));
        vst1q_p(pTarget + 4, vPixel, vctp32q(elts));
    }
#endif
}

static
void __arm_2d_impl_cccn888_get_pixel_colour_with_alpha_mve(
                                            arm_2d_point_s16x8_t    *ptPoint,
                                            arm_2d_region_t         *ptOrigValidRegion,
                                            uint32_t                *pOrigin,
                                            int16_t                  iOrigStride,
                                            uint32_t                *pTarget,
                                            uint32_t                 MaskColour,
                                            uint8_t                  chOpacity,
                                            int16_t                  elts)
{
#if     defined(__ARM_2D_HAS_INTERPOLATION_ROTATION__)  &&  __ARM_2D_HAS_INTERPOLATION_ROTATION__
    ARM_ALIGN(8) uint32_t scratch32[32];
    int16_t        *pscratch16 = (int16_t *) scratch32;
    uint32x4_t      vTargetLo = vld1q(pTarget);
    uint32x4_t      vTargetHi = vld1q(pTarget + 4);
    mve_pred16_t    predTailLow = vctp32q(elts);
    mve_pred16_t    predTailHigh = elts - 4 > 0 ? vctp32q(elts - 4) : 0;
    int16x8_t       vOne = vdupq_n_s16(SET_Q6INT(1));

    int16x8_t       vXi = GET_Q6INT(ptPoint->X);
    int16x8_t       vYi = GET_Q6INT(ptPoint->Y);

    vXi = vsubq_m_n_s16(vXi, vXi, 1, vcmpltq_n_s16(ptPoint->X, 0));
    vYi = vsubq_m_n_s16(vYi, vYi, 1, vcmpltq_n_s16(ptPoint->Y, 0));

    int16x8_t       vWX = ptPoint->X - (SET_Q6INT(vXi));
    int16x8_t       vWY = ptPoint->Y - (SET_Q6INT(vYi));

    /* combination of Bottom / Top & Left / Right areas contributions */
    uint16x8_t      vAreaTR = vmulq_u16(vWX, vWY);
    uint16x8_t      vAreaTL = vmulq_u16((vOne - vWX), vWY);
    uint16x8_t      vAreaBR = vmulq_u16(vWX, (vOne - vWY));
    uint16x8_t      vAreaBL = vmulq_u16((vOne - vWX), (vOne - vWY));

    /* Q16 conversion */
    vAreaTR = vqshlq_n_u16(vAreaTR, 4);
    vAreaTL = vqshlq_n_u16(vAreaTL, 4);
    vAreaBR = vqshlq_n_u16(vAreaBR, 4);
    vAreaBL = vqshlq_n_u16(vAreaBL, 4);


    /* accumulated pixel vectors */
    uint16x8_t      vAvgPixelR, vAvgPixelG, vAvgPixelB;

    /* predicate accumulators */
    /* tracks all predications conditions for selecting final */
    /* averaged pixed / target pixel */
    /* can probably optimized away since averaged target pixel is */
    /* equivalent to original pixel, but precision limitations would introduce small errors */
    mve_pred16_t    predGlbLo = 0, predGlbHi = 0;

    /*
     * accumulate / average over the 4 neigbouring pixels
     */

    uint16x8_t      R, G, B;

    /* Bottom Left averaging */
    {
        __ARM_2D_RGB888_GET_RGBVEC_FROM_POINT(vXi, vYi, R, G, B, MaskColour, predGlbLo, predGlbHi);

        __ARM_2D_SCALE_RGBVEC(vAvgPixelR, vAvgPixelG, vAvgPixelB, R, G, B, vAreaBL);
    }

    /* Bottom Right averaging */
    {
        __ARM_2D_RGB888_GET_RGBVEC_FROM_POINT(vaddq_n_s16(vXi, 1), vYi,
                            R, G, B, MaskColour, predGlbLo, predGlbHi);

        __ARM_2D_SCALE_RGBVEC_ACC(vAvgPixelR, vAvgPixelG, vAvgPixelB, R, G, B, vAreaBR);
    }

    /* Top Left averaging */
    {
        __ARM_2D_RGB888_GET_RGBVEC_FROM_POINT(vXi, vaddq_n_s16(vYi, 1),
                                                R, G, B, MaskColour, predGlbLo, predGlbHi);

        __ARM_2D_SCALE_RGBVEC_ACC(vAvgPixelR, vAvgPixelG, vAvgPixelB, R, G, B, vAreaTL);
    }

    /* Top Right averaging */
    {
        __ARM_2D_RGB888_GET_RGBVEC_FROM_POINT(vaddq_n_s16(vXi, 1), vaddq_n_s16(vYi, 1),
                                                R, G, B, MaskColour, predGlbLo, predGlbHi);

        __ARM_2D_SCALE_RGBVEC_ACC(vAvgPixelR, vAvgPixelG, vAvgPixelB, R, G, B, vAreaTR);
    }

    /* alpha blending */
    uint16x8_t      vTargetR, vTargetG, vTargetB;

    __arm_2d_unpack_rgb888_from_mem((const uint8_t *) pTarget, &vTargetR, &vTargetG, &vTargetB);

    uint16_t        chOpacityCompl = (256 - chOpacity);

    /* merge */
    vAvgPixelR = vmlaq_n_u16(vmulq_n_u16(vAvgPixelR, chOpacityCompl), vTargetR, chOpacity);
    vAvgPixelR = vAvgPixelR >> 8;

    vAvgPixelG = vmlaq_n_u16(vmulq_n_u16(vAvgPixelG, chOpacityCompl), vTargetG, chOpacity);
    vAvgPixelG = vAvgPixelG >> 8;

    vAvgPixelB = vmlaq_n_u16(vmulq_n_u16(vAvgPixelB, chOpacityCompl), vTargetB, chOpacity);
    vAvgPixelB = vAvgPixelB >> 8;

    /* pack */
    __arm_2d_pack_rgb888_to_mem((uint8_t *) scratch32, vAvgPixelR, vAvgPixelG, vAvgPixelB);

    uint32x4_t      TempPixel = vld1q(scratch32);

    /* select between target pixel, averaged pixed */
    TempPixel = vpselq_u32(TempPixel, vTargetLo, predGlbLo);

    vst1q_p(pTarget, TempPixel, predTailLow);

    TempPixel = vld1q(scratch32 + 4);

    /* select between target pixel, averaged pixed */
    TempPixel = vpselq_u32(TempPixel, vTargetHi, predGlbHi);

    vst1q_p(pTarget + 4, TempPixel, predTailHigh);
#else
    arm_2d_point_s32x4_t    tPointLo, tPointHi;
    ARM_ALIGN(8) int16_t          scratch[8];
    ARM_ALIGN(8) uint32_t         blendled[4];
    mve_pred16_t            p;

    /* split 16-bit point vector into 2 x 32-bit vectors */
    vst1q(scratch, GET_Q6INT(ptPoint->X));
    tPointLo.X = vldrhq_s32(scratch);
    tPointHi.X = vldrhq_s32(scratch + 4);

    vst1q(scratch, GET_Q6INT(ptPoint->Y));
    tPointLo.Y = vldrhq_s32(scratch);
    tPointHi.Y = vldrhq_s32(scratch + 4);

    /* 1st half */

    /* set vector predicate if point is inside the region */
    p = arm_2d_is_point_vec_inside_region_s32(ptOrigValidRegion, &tPointLo);
    /* prepare vector of point offsets */
    uint32x4_t      ptOffs = tPointLo.X + tPointLo.Y * iOrigStride;
    uint32x4_t      vPixel = vld1q(pTarget);
    /* retrieve all point values */
    uint32x4_t      ptVal = vldrwq_gather_shifted_offset_u32(pOrigin, ptOffs);

    vstrwq_u32((uint32_t *) scratch, ptVal);

    /* alpha-blending (requires widened inputs) */
    vstrbq_u16((uint8_t *) blendled,
               __rgb888_alpha_blending_direct_single_vec(vldrbq_u16((uint8_t const *) scratch),
                                                         vldrbq_u16((uint8_t const *) pTarget), chOpacity));

    vstrbq_u16((uint8_t *) blendled + 2,
               __rgb888_alpha_blending_direct_single_vec(vldrbq_u16((uint8_t const *)scratch + 4),
                                                         vldrbq_u16((uint8_t const *)pTarget + 2), chOpacity));

    uint32x4_t      vBlended = vld1q(blendled);

    /* combine 2 predicates, set to true, if point is in the region & values different from color mask */
    vPixel = vpselq_u32(vBlended, vPixel, vcmpneq_m_n_u32(ptVal, MaskColour, p));

    vst1q_p(pTarget, vPixel, vctp32q(elts));

    elts -= 4;
    if(elts > 0) {
        /* second half */

        p = arm_2d_is_point_vec_inside_region_s32(ptOrigValidRegion, &tPointHi);
        ptOffs = tPointHi.X + tPointHi.Y * iOrigStride;
        vPixel = vld1q(pTarget);
        ptVal = vldrwq_gather_shifted_offset_u32(pOrigin, ptOffs);

        vstrwq_u32((uint32_t *) scratch, ptVal);

        /* alpha-blending (requires widened inputs) */
        vstrbq_u16((uint8_t *) blendled,
                   __rgb888_alpha_blending_direct_single_vec(vldrbq_u16((uint8_t const *) scratch),
                                                             vldrbq_u16((uint8_t const *) pTarget), chOpacity));
        vstrbq_u16((uint8_t *) blendled + 2,
                   __rgb888_alpha_blending_direct_single_vec(vldrbq_u16((uint8_t const *)scratch + 4),
                                                             vldrbq_u16((uint8_t const *)pTarget + 2), chOpacity));

        vBlended = vld1q(blendled);

        /* combine 2 predicates, set to true, if point is in the region & values different from color mask */
        vPixel = vpselq_u32(vBlended, vPixel, vcmpneq_m_n_u32(ptVal, MaskColour, p));

        vst1q_p(pTarget + 4, vPixel, vctp32q(elts));
    }
#endif
}



__OVERRIDE_WEAK
void __arm_2d_impl_rgb565_rotate( __arm_2d_param_copy_orig_t *ptParam,
                            __arm_2d_rotate_info_t *ptInfo)
{
    int32_t         iHeight = ptParam->use_as____arm_2d_param_copy_t.tCopySize.iHeight;
    int32_t         iWidth = ptParam->use_as____arm_2d_param_copy_t.tCopySize.iWidth;
    int32_t         iTargetStride =
        ptParam->use_as____arm_2d_param_copy_t.tTarget.iStride;
    uint16_t       *pTargetBase = ptParam->use_as____arm_2d_param_copy_t.tTarget.pBuffer;
    uint16_t       *pOrigin = ptParam->tOrigin.pBuffer;
    int32_t         iOrigStride = ptParam->tOrigin.iStride;
    uint16_t        MaskColour = ptInfo->Mask.hwColour;
    float           fAngle = -ptInfo->fAngle;
    arm_2d_location_t tOffset =
        ptParam->use_as____arm_2d_param_copy_t.tSource.tValidRegion.tLocation;
    arm_2d_location_t *pCenter = &(ptInfo->tCenter);
    q31_t           invIWidth = (iWidth > 1) ? 0x7fffffff / (iWidth - 1) : 0x7fffffff;
    arm_2d_rot_linear_regr_t    regrCoefs[2];
    arm_2d_location_t           SrcPt = ptInfo->tDummySourceOffset;
    bool            gatherLoadIdxOverflow;

    /* get regression parameters over 1st and last column */
    gatherLoadIdxOverflow =
        __arm_2d_rotate_regression(&ptParam->use_as____arm_2d_param_copy_t.tCopySize,
                                   &SrcPt, fAngle, &tOffset, pCenter, iOrigStride,
                                   regrCoefs);


    /* slopes between 1st and last columns */
    int32_t         slopeY, slopeX;

    slopeY =
        MULTFX((regrCoefs[1].interceptY - regrCoefs[0].interceptY), invIWidth);
    slopeX =
        MULTFX((regrCoefs[1].interceptX - regrCoefs[0].interceptX), invIWidth);

    int32_t         nrmSlopeX = 17 - __CLZ(ABS(slopeX));
    int32_t         nrmSlopeY = 17 - __CLZ(ABS(slopeY));

    slopeX = ARSHIFT(slopeX, nrmSlopeX);
    slopeY = ARSHIFT(slopeY, nrmSlopeY);

    if (!gatherLoadIdxOverflow) {
    for (int32_t y = 0; y < iHeight; y++) {

        /* 1st column estimates */
        int32_t         colFirstY =
            __QADD((regrCoefs[0].slopeY * y), regrCoefs[0].interceptY);
        int32_t         colFirstX =
            __QADD((regrCoefs[0].slopeX * y), regrCoefs[0].interceptX);

        /* Q6 conversion */
        colFirstX = colFirstX >> 10;
        colFirstY = colFirstY >> 10;

        int32_t         nbVecElts = iWidth;
        int16x8_t       vX = (int16x8_t) vidupq_n_u16(0, 1);
        uint16_t       *pTargetBaseCur = pTargetBase;

        /* Q9.6 coversion */
        vX = vX * (1<<6);

        while (nbVecElts > 0) {
                arm_2d_point_s16x8_t tPointV;

                tPointV.X = vqdmulhq_n_s16(vX, slopeX);
                tPointV.X = vaddq_n_s16(vqrshlq_n_s16(tPointV.X, nrmSlopeX), colFirstX);

                tPointV.Y = vqdmulhq_n_s16(vX, slopeY);
                tPointV.Y = vaddq_n_s16(vqrshlq_n_s16(tPointV.Y, nrmSlopeY), colFirstY);

                __arm_2d_impl_rgb565_get_pixel_colour(&tPointV,
                                                      &ptParam->tOrigin.tValidRegion,
                                                      pOrigin,
                                                      iOrigStride,
                                                      pTargetBaseCur, MaskColour,
                                                      nbVecElts);

            pTargetBaseCur += 8;
            vX += ((1<<6) * 8);
            nbVecElts -= 8;
        }
        pTargetBase += iTargetStride;
        }
    } else {
        for (int32_t y = 0; y < iHeight; y++) {

            /* 1st column estimates */
            int32_t         colFirstY =
                __QADD((regrCoefs[0].slopeY * y), regrCoefs[0].interceptY);
            int32_t         colFirstX =
                __QADD((regrCoefs[0].slopeX * y), regrCoefs[0].interceptX);

            /* Q6 conversion */
            colFirstX = colFirstX >> 10;
            colFirstY = colFirstY >> 10;

            int32_t         nbVecElts = iWidth;
            int16x8_t       vX = (int16x8_t) vidupq_n_u16(0, 1);
            uint16_t       *pTargetBaseCur = pTargetBase;

            /* Q9.6 coversion */
            vX = SET_Q6INT(vX);

            while (nbVecElts > 0) {
                arm_2d_point_s16x8_t tPointV;

                tPointV.X = vqdmulhq_n_s16(vX, slopeX);
                tPointV.X = vaddq_n_s16(vqrshlq_n_s16(tPointV.X, nrmSlopeX), colFirstX);

                tPointV.Y = vqdmulhq_n_s16(vX, slopeY);
                tPointV.Y = vaddq_n_s16(vqrshlq_n_s16(tPointV.Y, nrmSlopeY), colFirstY);

                __arm_2d_impl_rgb565_get_pixel_colour_offs_compensated(&tPointV,
                                                                       &ptParam->tOrigin.
                                                                       tValidRegion,
                                                                       pOrigin,
                                                                       iOrigStride,
                                                                       pTargetBaseCur,
                                                                       MaskColour,
                                                                       nbVecElts);

                pTargetBaseCur += 8;
                vX += SET_Q6INT(8);
                nbVecElts -= 8;
            }
            pTargetBase += iTargetStride;
        }
    }
}




__OVERRIDE_WEAK
void __arm_2d_impl_rgb565_rotate_alpha(   __arm_2d_param_copy_orig_t *ptParam,
                                    __arm_2d_rotate_info_t *ptInfo,
                                    uint_fast8_t chRatio)
{
    int32_t         iHeight = ptParam->use_as____arm_2d_param_copy_t.tCopySize.iHeight;
    int32_t         iWidth = ptParam->use_as____arm_2d_param_copy_t.tCopySize.iWidth;

    int32_t         iTargetStride =
        ptParam->use_as____arm_2d_param_copy_t.tTarget.iStride;
    uint16_t       *pTargetBase = ptParam->use_as____arm_2d_param_copy_t.tTarget.pBuffer;
    uint16_t       *pOrigin = ptParam->tOrigin.pBuffer;
    int32_t         iOrigStride = ptParam->tOrigin.iStride;
    uint16_t        MaskColour = ptInfo->Mask.hwColour;
    float           fAngle = -ptInfo->fAngle;
    arm_2d_location_t tOffset =
        ptParam->use_as____arm_2d_param_copy_t.tSource.tValidRegion.tLocation;
    arm_2d_location_t *pCenter = &(ptInfo->tCenter);

    uint16_t        hwRatioCompl = 256 - chRatio;
    q31_t           invIWidth = iWidth > 1 ? 0x7fffffff / (iWidth - 1) : 0x7fffffff;
    arm_2d_rot_linear_regr_t regrCoefs[2];
    arm_2d_location_t SrcPt = ptInfo->tDummySourceOffset;
    bool            gatherLoadIdxOverflow;

    /* get regression parameters over 1st and last column */
    gatherLoadIdxOverflow =
        __arm_2d_rotate_regression(&ptParam->use_as____arm_2d_param_copy_t.tCopySize,
                                   &SrcPt, fAngle, &tOffset, pCenter, iOrigStride,
                                   regrCoefs);


    /* slopes between 1st and last columns */
    int32_t         slopeY, slopeX;

    slopeY = MULTFX((regrCoefs[1].interceptY - regrCoefs[0].interceptY), invIWidth);
    slopeX = MULTFX((regrCoefs[1].interceptX - regrCoefs[0].interceptX), invIWidth);

    int32_t         nrmSlopeX = 17 - __CLZ(ABS(slopeX));
    int32_t         nrmSlopeY = 17 - __CLZ(ABS(slopeY));

    slopeX = ARSHIFT(slopeX, nrmSlopeX);
    slopeY = ARSHIFT(slopeY, nrmSlopeY);

    if (!gatherLoadIdxOverflow) {
        for (int32_t y = 0; y < iHeight; y++) {
            /* 1st column estimates */
            int32_t         colFirstY =
                __QADD((regrCoefs[0].slopeY * y), regrCoefs[0].interceptY);
            int32_t         colFirstX =
                __QADD((regrCoefs[0].slopeX * y), regrCoefs[0].interceptX);

            /* Q6 conversion */
            colFirstX = colFirstX >> 10;
            colFirstY = colFirstY >> 10;

            int32_t         nbVecElts = iWidth;
            int16x8_t       vX = (int16x8_t) vidupq_n_u16(0, 1);
            uint16_t       *pTargetBaseCur = pTargetBase;

            /* Q9.6 coversion */
            vX = SET_Q6INT(vX);

            while (nbVecElts > 0) {
                /* interpolation */
                arm_2d_point_s16x8_t tPointV;

                tPointV.X = vqdmulhq_n_s16(vX, slopeX);
                tPointV.X = vaddq_n_s16(vqrshlq_n_s16(tPointV.X, nrmSlopeX), colFirstX);

                tPointV.Y = vqdmulhq_n_s16(vX, slopeY);
                tPointV.Y = vaddq_n_s16(vqrshlq_n_s16(tPointV.Y, nrmSlopeY), colFirstY);

                __arm_2d_impl_rgb565_get_pixel_colour_with_alpha(&tPointV,
                                                                 &ptParam->
                                                                 tOrigin.tValidRegion,
                                                                 pOrigin, iOrigStride,
                                                                 pTargetBaseCur,
                                                                 MaskColour, hwRatioCompl,
                                                                 nbVecElts);
                pTargetBaseCur += 8;
                vX += SET_Q6INT(8);
                nbVecElts -= 8;
            }
            pTargetBase += iTargetStride;
        }
    } else {
        /*
           Large image / Large origin offsets
           Gather load 16-bit could overflow
           - Y offset needs to be shifted down to avoid overflow
           - 16-bit gather loads base address is incremented

           Needs to be done in the inner loop.
           In the case of steep slopes, taking the minimum between the Y extrema could still generate overflows
         */
        for (int32_t y = 0; y < iHeight; y++) {
            /* 1st column estimates */
            int32_t         colFirstY =
                __QADD((regrCoefs[0].slopeY * y), regrCoefs[0].interceptY);
            int32_t         colFirstX =
                __QADD((regrCoefs[0].slopeX * y), regrCoefs[0].interceptX);

            /* Q6 conversion */
            colFirstX = colFirstX >> 10;
            colFirstY = colFirstY >> 10;

            int32_t         nbVecElts = iWidth;
            int16x8_t       vX = (int16x8_t) vidupq_n_u16(0, 1);
            uint16_t       *pTargetBaseCur = pTargetBase;

            /* Q9.6 coversion */
            vX = SET_Q6INT(vX);

            while (nbVecElts > 0) {
                /* interpolation */
                arm_2d_point_s16x8_t tPointV;

                tPointV.X = vqdmulhq_n_s16(vX, slopeX);
                tPointV.X = vaddq_n_s16(vqrshlq_n_s16(tPointV.X, nrmSlopeX), colFirstX);

                tPointV.Y = vqdmulhq_n_s16(vX, slopeY);
                tPointV.Y = vaddq_n_s16(vqrshlq_n_s16(tPointV.Y, nrmSlopeY), colFirstY);

                __arm_2d_impl_rgb565_get_pixel_colour_with_alpha_offs_compensated
                    (&tPointV, &ptParam->tOrigin.tValidRegion, pOrigin, iOrigStride,
                     pTargetBaseCur, MaskColour, hwRatioCompl, nbVecElts);

                pTargetBaseCur += 8;
                vX += SET_Q6INT(8);
                nbVecElts -= 8;
            }
            pTargetBase += iTargetStride;
        }
    }
}

/* untested */
__OVERRIDE_WEAK
void __arm_2d_impl_cccn888_rotate(   __arm_2d_param_copy_orig_t *ptParam,
                                    __arm_2d_rotate_info_t *ptInfo)
{
    int32_t    iHeight = ptParam->use_as____arm_2d_param_copy_t.tCopySize.iHeight;
    int32_t    iWidth = ptParam->use_as____arm_2d_param_copy_t.tCopySize.iWidth;

    int32_t    iTargetStride =
        ptParam->use_as____arm_2d_param_copy_t.tTarget.iStride;
    uint32_t       *pTargetBase = ptParam->use_as____arm_2d_param_copy_t.tTarget.pBuffer;
    uint32_t       *pOrigin = ptParam->tOrigin.pBuffer;
    int32_t    iOrigStride = ptParam->tOrigin.iStride;
    uint32_t        MaskColour = ptInfo->Mask.hwColour;
    float           fAngle = -ptInfo->fAngle;
    arm_2d_location_t tOffset =
        ptParam->use_as____arm_2d_param_copy_t.tSource.tValidRegion.tLocation;
    arm_2d_location_t *pCenter = &(ptInfo->tCenter);
	q31_t           invIWidth = 0x7fffffff / (iWidth - 1);
    arm_2d_rot_linear_regr_t    regrCoefs[2];
    arm_2d_location_t           SrcPt = ptInfo->tDummySourceOffset;

    /* get regression parameters over 1st and last column */
    __arm_2d_rotate_regression(&ptParam->use_as____arm_2d_param_copy_t.tCopySize,
                                   &SrcPt, fAngle, &tOffset, pCenter, iOrigStride,
                                   regrCoefs);


    /* slopes between 1st and last columns */
    int32_t         slopeY, slopeX;

    slopeY =
        MULTFX((regrCoefs[1].interceptY - regrCoefs[0].interceptY), invIWidth);
    slopeX =
        MULTFX((regrCoefs[1].interceptX - regrCoefs[0].interceptX), invIWidth);

    int32_t         nrmSlopeX = 17 - __CLZ(ABS(slopeX));
    int32_t         nrmSlopeY = 17 - __CLZ(ABS(slopeY));

    slopeX = ARSHIFT(slopeX, nrmSlopeX);
    slopeY = ARSHIFT(slopeY, nrmSlopeY);

    for (int32_t y = 0; y < iHeight; y++) {

        /* 1st column estimates */
        int32_t         colFirstY =
            __QADD((regrCoefs[0].slopeY * y), regrCoefs[0].interceptY);
        int32_t         colFirstX =
            __QADD((regrCoefs[0].slopeX * y), regrCoefs[0].interceptX);

        /* Q6 conversion */
        colFirstX = colFirstX >> 10;
        colFirstY = colFirstY >> 10;

        int32_t         nbVecElts = iWidth;
        int16x8_t       vX = (int16x8_t) vidupq_n_u16(0, 1);
        uint32_t       *pTargetBaseCur = pTargetBase;

        /* Q9.6 coversion */
        vX = vX * (1<<6);

        while (nbVecElts > 0) {
            arm_2d_point_s16x8_t tPointV;;

            tPointV.X = vqdmulhq_n_s16(vX, slopeX);
            tPointV.X = vaddq_n_s16(vqrshlq_n_s16(tPointV.X, nrmSlopeX), colFirstX);

            tPointV.Y = vqdmulhq_n_s16(vX, slopeY);
            tPointV.Y = vaddq_n_s16(vqrshlq_n_s16(tPointV.Y, nrmSlopeY), colFirstY);


            __arm_2d_impl_cccn888_get_pixel_colour_mve(&tPointV,
                                                      &ptParam->tOrigin.tValidRegion,
                                                      pOrigin,
                                                      iOrigStride,
                                                      pTargetBase, MaskColour, nbVecElts);

            pTargetBaseCur += 8;
            vX += ((1<<6) * 8);
            nbVecElts -= 8;
        }
        pTargetBase += iTargetStride;
    }
}

/* untested */
__OVERRIDE_WEAK
void __arm_2d_impl_cccn888_rotate_alpha(   __arm_2d_param_copy_orig_t *ptParam,
                                    __arm_2d_rotate_info_t *ptInfo,
                                    uint_fast8_t chRatio)
{
    int32_t    iHeight = ptParam->use_as____arm_2d_param_copy_t.tCopySize.iHeight;
    int32_t    iWidth = ptParam->use_as____arm_2d_param_copy_t.tCopySize.iWidth;

    int32_t    iTargetStride =
        ptParam->use_as____arm_2d_param_copy_t.tTarget.iStride;
    uint32_t       *pTargetBase = ptParam->use_as____arm_2d_param_copy_t.tTarget.pBuffer;
    uint32_t       *pOrigin = ptParam->tOrigin.pBuffer;
    int32_t         iOrigStride = ptParam->tOrigin.iStride;
    uint32_t        MaskColour = ptInfo->Mask.hwColour;
    float           fAngle = -ptInfo->fAngle;
    arm_2d_location_t tOffset =
        ptParam->use_as____arm_2d_param_copy_t.tSource.tValidRegion.tLocation;
    uint16_t        wRatioCompl = 256 - chRatio;
    arm_2d_location_t *pCenter = &(ptInfo->tCenter);
	q31_t           invIWidth = 0x7fffffff / (iWidth - 1);
    arm_2d_rot_linear_regr_t    regrCoefs[2];
    arm_2d_location_t           SrcPt = ptInfo->tDummySourceOffset;

    /* get regression parameters over 1st and last column */
    __arm_2d_rotate_regression(&ptParam->use_as____arm_2d_param_copy_t.tCopySize,
                                   &SrcPt, fAngle, &tOffset, pCenter, iOrigStride,
                                   regrCoefs);


    /* slopes between 1st and last columns */
    int32_t         slopeY, slopeX;

    slopeY =
        MULTFX((regrCoefs[1].interceptY - regrCoefs[0].interceptY), invIWidth);
    slopeX =
        MULTFX((regrCoefs[1].interceptX - regrCoefs[0].interceptX), invIWidth);

    int32_t         nrmSlopeX = 17 - __CLZ(ABS(slopeX));
    int32_t         nrmSlopeY = 17 - __CLZ(ABS(slopeY));

    slopeX = ARSHIFT(slopeX, nrmSlopeX);
    slopeY = ARSHIFT(slopeY, nrmSlopeY);

    for (int32_t y = 0; y < iHeight; y++) {

        /* 1st column estimates */
        int32_t         colFirstY =
            __QADD((regrCoefs[0].slopeY * y), regrCoefs[0].interceptY);
        int32_t         colFirstX =
            __QADD((regrCoefs[0].slopeX * y), regrCoefs[0].interceptX);

        /* Q6 conversion */
        colFirstX = colFirstX >> 10;
        colFirstY = colFirstY >> 10;

        int32_t         nbVecElts = iWidth;
        int16x8_t       vX = (int16x8_t) vidupq_n_u16(0, 1);
        uint32_t       *pTargetBaseCur = pTargetBase;

        /* Q9.6 coversion */
        vX = vX * (1<<6);

        while (nbVecElts > 0) {
            arm_2d_point_s16x8_t tPointV;;

            tPointV.X = vqdmulhq_n_s16(vX, slopeX);
            tPointV.X = vaddq_n_s16(vqrshlq_n_s16(tPointV.X, nrmSlopeX), colFirstX);

            tPointV.Y = vqdmulhq_n_s16(vX, slopeY);
            tPointV.Y = vaddq_n_s16(vqrshlq_n_s16(tPointV.Y, nrmSlopeY), colFirstY);


            __arm_2d_impl_cccn888_get_pixel_colour_with_alpha_mve(&tPointV,
                                                                 &ptParam->
                                                                 tOrigin.tValidRegion,
                                                                 pOrigin, iOrigStride,
                                                                 pTargetBase, MaskColour,
                                                                 wRatioCompl, nbVecElts);
            pTargetBaseCur += 8;
            vX += ((1<<6) * 8);
            nbVecElts -= 8;
        }
        pTargetBase += iTargetStride;
    }
}

#endif






/* rgb16_draw_pattern helpers */

/*
 * enable to pick gather load offset based on initial offset
 * e.g. if iOffset = 3
 * will get {0, 0, 0, 0, 0, 1, 1, 1}
 */
static uint16_t __rgb16_draw_pattern_src_incr[16] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1
};

/*
 * enable to pick vector bitmask based on initial offset
 * e.g. if iOffset = 3
 * will get {8, 16, 32, 64, 128, 1, 2, 4}
 */

static  uint16_t __rgb16_draw_pattern_src_bitmask[16] = {
    1, 2, 4, 8, 16, 32, 64, 128,
    1, 2, 4, 8, 16, 32, 64, 128,
};


/* rgb32_draw_pattern helpers */

static uint32_t __rgb32_draw_pattern_src_incr[16] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1
};


static  uint32_t __rgb32_draw_pattern_src_bitmask[16] = {
    1, 2, 4, 8, 16, 32, 64, 128,
    1, 2, 4, 8, 16, 32, 64, 128,
};


__OVERRIDE_WEAK
void __arm_2d_impl_rgb16_draw_pattern_fg_only(uint8_t *__RESTRICT pchSourceBase,
                                         int32_t  iOffset,
                                         int16_t iSourceStride,
                                         uint16_t *__RESTRICT pTargetBase,
                                         int16_t iTargetStride,
                                         arm_2d_size_t *__RESTRICT ptCopySize,
                                         uint16_t hwForeColour)
{
    //! get in byte offset
    iOffset &= 0x07;
    iSourceStride = (iSourceStride + 7) & ~0x07;

    /* deduces offset vector from iOffset for gather loading */
    uint16x8_t      offS = vld1q(__rgb16_draw_pattern_src_incr + iOffset);
    /* deduces bitmask vector with wrap from iOffset */
    uint16x8_t      vBitMask = vld1q(__rgb16_draw_pattern_src_bitmask + iOffset);

    if (ptCopySize->iWidth <= 8) {
        /* small width specialization */
        /* no inner loop */
        mve_pred16_t    p = vctp16q(ptCopySize->iWidth);

        for (int32_t y = 0; y < ptCopySize->iHeight; y++) {
            uint16x8_t      vchSrc = vldrbq_gather_offset_u16(pchSourceBase, offS);
            uint16x8_t      vTarg = vld1q(pTargetBase);

            vchSrc = vchSrc & vBitMask;
            vTarg = vdupq_m_n_u16(vTarg, hwForeColour, vcmpneq_n_u16(vchSrc, 0));

            vst1q_p(pTargetBase, vTarg, p);

            pchSourceBase += (iSourceStride >> 3);
            pTargetBase += iTargetStride;
        }

    } else {
        for (int32_t y = 0; y < ptCopySize->iHeight; y++) {
            int32_t         cnt = ptCopySize->iWidth;
            uint8_t        *pchSourceBaseCur = pchSourceBase;
            uint16_t       *pTargetBaseCur = pTargetBase;

            while (cnt > 0) {
                mve_pred16_t    p = vctp16q(cnt);
                uint16x8_t      vchSrc =
                    vldrbq_gather_offset_z_u16(pchSourceBaseCur, offS, p);
                uint16x8_t      vTarg = vld1q_z(pTargetBaseCur, p);

                vchSrc = vandq_x(vchSrc, vBitMask, p);
                vTarg = vdupq_m_n_u16(vTarg, hwForeColour, vcmpneq_m_n_u16(vchSrc, 0, p));

                vst1q_p(pTargetBaseCur, vTarg, p);

                pTargetBaseCur += 8;
                pchSourceBaseCur += 1;
                cnt -= 8;
            }
            pchSourceBase += (iSourceStride >> 3);
            pTargetBase += iTargetStride;
        }
    }
}



__OVERRIDE_WEAK
void __arm_2d_impl_rgb16_draw_pattern_no_bg_comp(uint8_t * __RESTRICT pchSourceBase,
                                                 int32_t iOffset,
                                                 int16_t iSourceStride,
                                                 uint16_t * __RESTRICT pTargetBase,
                                                 int16_t iTargetStride,
                                                 arm_2d_size_t * __RESTRICT ptCopySize)
{
    //! get in byte offset
    iOffset &= 0x07;
    iSourceStride = (iSourceStride + 7) & ~0x07;

    /* deduces offset vector from iOffset for gather loading */
    uint16x8_t      offS = vld1q(__rgb16_draw_pattern_src_incr + iOffset);
    /* deduces bitmask vector with wrap from iOffset */
    uint16x8_t      vBitMask = vld1q(__rgb16_draw_pattern_src_bitmask + iOffset);

    if (ptCopySize->iWidth <= 8) {
        /* small width specialization */
        /* no inner loop */
        mve_pred16_t    p = vctp16q(ptCopySize->iWidth);

        for (int32_t y = 0; y < ptCopySize->iHeight; y++) {
            uint16x8_t      vchSrc = vldrbq_gather_offset_u16(pchSourceBase, offS);
            uint16x8_t      vTarg = vld1q(pTargetBase);

            vchSrc = vchSrc & vBitMask;
            vTarg = vpselq(~vTarg, vTarg, vcmpneq_n_u16(vchSrc, 0));//vTarg = vpselq(vTarg, ~vTarg, vcmpneq_n_u16(vchSrc, 0));

            vst1q_p(pTargetBase, vTarg, p);

            pchSourceBase += (iSourceStride >> 3);
            pTargetBase += iTargetStride;
        }

    } else {
        for (int32_t y = 0; y < ptCopySize->iHeight; y++) {
            int32_t         cnt = ptCopySize->iWidth;
            uint8_t        *pchSourceBaseCur = pchSourceBase;
            uint16_t       *pTargetBaseCur = pTargetBase;

            while (cnt > 0) {
                mve_pred16_t    p = vctp16q(cnt);
                uint16x8_t      vchSrc =
                    vldrbq_gather_offset_z_u16(pchSourceBaseCur, offS, p);
                uint16x8_t      vTarg = vld1q_z(pTargetBaseCur, p);

                vchSrc = vandq_x(vchSrc, vBitMask, p);
                vTarg = vpselq(vmvnq_x(vTarg, p), vTarg, vcmpneq_m_n_u16(vchSrc, 0, p));//vTarg = vpselq(vTarg, vmvnq_x(vTarg, p), vcmpneq_m_n_u16(vchSrc, 0, p));

                vst1q_p(pTargetBaseCur, vTarg, p);

                pTargetBaseCur += 8;
                pchSourceBaseCur += 1;
                cnt -= 8;
            }
            pchSourceBase += (iSourceStride >> 3);
            pTargetBase += iTargetStride;
        }
    }
}




__OVERRIDE_WEAK
void __arm_2d_impl_rgb16_draw_pattern_bg_only(uint8_t *__RESTRICT pchSourceBase,
                                         int32_t  iOffset,
                                         int16_t iSourceStride,
                                         uint16_t *__RESTRICT pTargetBase,
                                         int16_t iTargetStride,
                                         arm_2d_size_t *__RESTRICT ptCopySize,
                                         uint16_t hwBackColour)
{
    //! get in byte offset
    iOffset &= 0x07;
    iSourceStride = (iSourceStride + 7) & ~0x07;

    /* deduces offset vector from iOffset for gather loading */
    uint16x8_t      offS = vld1q(__rgb16_draw_pattern_src_incr + iOffset);
    /* deduces bitmask vector with wrap from iOffset */
    uint16x8_t      vBitMask = vld1q(__rgb16_draw_pattern_src_bitmask + iOffset);

    if (ptCopySize->iWidth <= 8) {
        /* small width specialization */
        /* no inner loop */
        mve_pred16_t    p = vctp16q(ptCopySize->iWidth);

        for (int32_t y = 0; y < ptCopySize->iHeight; y++) {
            uint16x8_t      vchSrc = vldrbq_gather_offset_u16(pchSourceBase, offS);
            uint16x8_t      vTarg = vld1q(pTargetBase);

            vchSrc = vchSrc & vBitMask;
            vTarg = vdupq_m_n_u16(vTarg, hwBackColour, vcmpeqq_n_u16(vchSrc, 0));

            vst1q_p(pTargetBase, vTarg, p);

            pchSourceBase += (iSourceStride >> 3);
            pTargetBase += iTargetStride;
        }

    } else {
        for (int32_t y = 0; y < ptCopySize->iHeight; y++) {
            int32_t         cnt = ptCopySize->iWidth;
            uint8_t        *pchSourceBaseCur = pchSourceBase;
            uint16_t       *pTargetBaseCur = pTargetBase;

            while (cnt > 0) {
                mve_pred16_t    p = vctp16q(cnt);
                uint16x8_t      vchSrc =
                    vldrbq_gather_offset_z_u16(pchSourceBaseCur, offS, p);
                uint16x8_t      vTarg = vld1q_z(pTargetBaseCur, p);

                vchSrc = vandq_x(vchSrc, vBitMask, p);
                vTarg = vdupq_m_n_u16(vTarg, hwBackColour, vcmpeqq_m_n_u16(vchSrc, 0, p));

                vst1q_p(pTargetBaseCur, vTarg, p);

                pTargetBaseCur += 8;
                pchSourceBaseCur += 1;
                cnt -= 8;
            }
            pchSourceBase += (iSourceStride >> 3);
            pTargetBase += iTargetStride;
        }
    }
}




__OVERRIDE_WEAK
void __arm_2d_impl_rgb16_draw_pattern_bg_fg(uint8_t *__RESTRICT pchSourceBase,
                                         int32_t  iOffset,
                                         int16_t iSourceStride,
                                         uint16_t *__RESTRICT pTargetBase,
                                         int16_t iTargetStride,
                                         arm_2d_size_t *__RESTRICT ptCopySize,
                                         uint16_t hwForeColour,
                                         uint16_t hwBackColour)
{
    //! get in byte offset
    iOffset &= 0x07;
    iSourceStride = (iSourceStride + 7) & ~0x07;

    /* deduces offset vector from iOffset for gather loading */
    uint16x8_t      offS = vld1q(__rgb16_draw_pattern_src_incr + iOffset);
    /* deduces bitmask vector with wrap from iOffset */
    uint16x8_t      vBitMask = vld1q(__rgb16_draw_pattern_src_bitmask + iOffset);
    uint16x8_t      vFgColor = vdupq_n_u16(hwForeColour);
    uint16x8_t      vBgColor = vdupq_n_u16(hwBackColour);

    if (ptCopySize->iWidth <= 8) {
        /* small width specialization */
        /* no inner loop */
        mve_pred16_t    p = vctp16q(ptCopySize->iWidth);

        for (int32_t y = 0; y < ptCopySize->iHeight; y++) {
            uint16x8_t      vchSrc = vldrbq_gather_offset_u16(pchSourceBase, offS);
            uint16x8_t      vTarg;

            vchSrc = vchSrc & vBitMask;
            vTarg =
                vpselq(vFgColor, vBgColor, vcmpneq_n_u16(vchSrc, 0));

            vst1q_p(pTargetBase, vTarg, p);

            pchSourceBase += (iSourceStride >> 3);
            pTargetBase += iTargetStride;
        }

    } else {
        for (int32_t y = 0; y < ptCopySize->iHeight; y++) {
            int32_t         cnt = ptCopySize->iWidth;
            uint8_t        *pchSourceBaseCur = pchSourceBase;
            uint16_t       *pTargetBaseCur = pTargetBase;

            while (cnt > 0) {
                mve_pred16_t    p = vctp16q(cnt);
                uint16x8_t      vchSrc =
                    vldrbq_gather_offset_z_u16(pchSourceBaseCur, offS, p);
                uint16x8_t      vTarg;

                vchSrc = vandq_x(vchSrc, vBitMask, p);
                vTarg =
                    vpselq(vFgColor, vBgColor, vcmpneq_m_n_u16(vchSrc, 0, p));

                vst1q_p(pTargetBaseCur, vTarg, p);

                pTargetBaseCur += 8;
                pchSourceBaseCur += 1;
                cnt -= 8;
            }
            pchSourceBase += (iSourceStride >> 3);
            pTargetBase += iTargetStride;
        }
    }
}





__OVERRIDE_WEAK
void __arm_2d_impl_rgb16_draw_pattern_bg_comp(uint8_t *__RESTRICT pchSourceBase,
                                         int32_t  iOffset,
                                         int16_t iSourceStride,
                                         uint16_t *__RESTRICT pTargetBase,
                                         int16_t iTargetStride,
                                         arm_2d_size_t *__RESTRICT ptCopySize,
                                         uint16_t hwBackColour)
{
    //! get in byte offset
    iOffset &= 0x07;
    iSourceStride = (iSourceStride + 7) & ~0x07;

    /* deduces offset vector from iOffset for gather loading */
    uint16x8_t      offS = vld1q(__rgb16_draw_pattern_src_incr + iOffset);
    /* deduces bitmask vector with wrap from iOffset */
    uint16x8_t      vBitMask = vld1q(__rgb16_draw_pattern_src_bitmask + iOffset);
    uint16x8_t      vBgColor = vdupq_n_u16(hwBackColour);

    if (ptCopySize->iWidth <= 8) {
        /* small width specialization */
        /* no inner loop */
        mve_pred16_t    p = vctp16q(ptCopySize->iWidth);

        for (int32_t y = 0; y < ptCopySize->iHeight; y++) {
            uint16x8_t      vchSrc = vldrbq_gather_offset_u16(pchSourceBase, offS);
            uint16x8_t      vTarg = vld1q(pTargetBase);

            /*
                if ((*pchSrc) & chBitMask)
                    *pTarget = ~(*pTarget);
                else
                    *pTarget = hwBackColour;
            */
            vchSrc = vchSrc & vBitMask;
            vTarg = vpselq(vmvnq(vTarg), vBgColor, vcmpneq_n_u16(vchSrc, 0));

            vst1q_p(pTargetBase, vTarg, p);

            pchSourceBase += (iSourceStride >> 3);
            pTargetBase += iTargetStride;
        }

    } else {
        for (int32_t y = 0; y < ptCopySize->iHeight; y++) {
            int32_t         cnt = ptCopySize->iWidth;
            uint8_t        *pchSourceBaseCur = pchSourceBase;
            uint16_t       *pTargetBaseCur = pTargetBase;

            while (cnt > 0) {
                mve_pred16_t    p = vctp16q(cnt);
                uint16x8_t      vchSrc =
                    vldrbq_gather_offset_z_u16(pchSourceBaseCur, offS, p);
                uint16x8_t      vTarg = vld1q_z(pTargetBase, p);

                vchSrc = vandq_x(vchSrc, vBitMask, p);
                vTarg =
                    vpselq(vmvnq_x(vTarg, p), vBgColor, vcmpneq_m_n_u16(vchSrc, 0, p));

                vst1q_p(pTargetBaseCur, vTarg, p);

                pTargetBaseCur += 8;
                pchSourceBaseCur += 1;
                cnt -= 8;
            }
            pchSourceBase += (iSourceStride >> 3);
            pTargetBase += iTargetStride;
        }
    }
}



__OVERRIDE_WEAK
void __arm_2d_impl_rgb32_draw_pattern_fg_only(uint8_t *__RESTRICT pchSourceBase,
                                         int32_t  iOffset,
                                         int16_t iSourceStride,
                                         uint32_t *__RESTRICT pTargetBase,
                                         int16_t iTargetStride,
                                         arm_2d_size_t *__RESTRICT ptCopySize,
                                         uint32_t hwForeColour)
{
    //! get in byte offset
    iOffset &= 0x07;
    iSourceStride = (iSourceStride + 7) & ~0x07;

    /* deduces offset vector from iOffset for gather loading */
    /* hold  8 contiguous values into 2 32-bit vector pair */
    uint32x4_t      offSLo = vld1q(__rgb32_draw_pattern_src_incr + iOffset);
    uint32x4_t      offSHi = vld1q(__rgb32_draw_pattern_src_incr + iOffset + 4);
    /* deduces bitmask vector with wrap from iOffset */
    /* hold  8 contiguous values into 2 32-bit vector pair */
    uint32x4_t      vBitMaskLo = vld1q(__rgb32_draw_pattern_src_bitmask + iOffset);
    uint32x4_t      vBitMaskHi = vld1q(__rgb32_draw_pattern_src_bitmask + iOffset + 4);


    if (ptCopySize->iWidth <= 4) {
        /* very tall width case */
        /* only bottom parts of gather load and bitmask needed */
        /* no inner loop */
        for (int32_t y = 0; y < ptCopySize->iHeight; y++) {
            mve_pred16_t    p = vctp32q(ptCopySize->iWidth);
            uint32x4_t      vchSrc = vldrbq_gather_offset_u32(pchSourceBase, offSLo);
            uint32x4_t      vTarg = vld1q(pTargetBase);

            vchSrc = vandq(vchSrc, vBitMaskLo);
            vTarg = vdupq_m_n_u32(vTarg, hwForeColour, vcmpneq_n_u32(vchSrc, 0));

            vst1q_p(pTargetBase, vTarg, p);

            pchSourceBase += (iSourceStride >> 3);
            pTargetBase += iTargetStride;
        }
    } else if (ptCopySize->iWidth <= 8) {
        /* bottom and partial upper parts of gather load and bitmask needed */
        /* no inner loop */
        for (int32_t y = 0; y < ptCopySize->iHeight; y++) {
            mve_pred16_t    p = vctp32q(ptCopySize->iWidth - 4);
            uint32x4_t      vchSrc = vldrbq_gather_offset_u32(pchSourceBase, offSLo);
            uint32x4_t      vTarg = vld1q(pTargetBase);

            vchSrc = vandq(vchSrc, vBitMaskLo);
            vTarg = vdupq_m_n_u32(vTarg, hwForeColour, vcmpneq_n_u32(vchSrc, 0));

            vst1q(pTargetBase, vTarg);

            vchSrc = vldrbq_gather_offset_u32(pchSourceBase, offSHi);
            vTarg = vld1q(pTargetBase + 4);

            vchSrc = vandq(vchSrc, vBitMaskHi);
            vTarg = vdupq_m_n_u32(vTarg, hwForeColour, vcmpneq_n_u32(vchSrc, 0));

            vst1q_p(pTargetBase + 4, vTarg, p);

            pchSourceBase += (iSourceStride >> 3);
            pTargetBase += iTargetStride;
        }
    } else {
        /* generic case */
        for (int32_t y = 0; y < ptCopySize->iHeight; y++) {
            int32_t         cnt = ptCopySize->iWidth;
            uint8_t        *pchSourceBaseCur = pchSourceBase;
            uint32_t       *pTargetBaseCur = pTargetBase;

            while (cnt >= 8) {
                uint32x4_t      vchSrc =
                    vldrbq_gather_offset_u32(pchSourceBaseCur, offSLo);
                uint32x4_t      vTarg = vld1q(pTargetBaseCur);

                vchSrc = vandq(vchSrc, vBitMaskLo);
                vTarg = vdupq_m_n_u32(vTarg, hwForeColour, vcmpneq_n_u32(vchSrc, 0));

                vst1q(pTargetBaseCur, vTarg);
                pTargetBaseCur += 4;

                vchSrc = vldrbq_gather_offset_u32(pchSourceBaseCur, offSHi);
                vTarg = vld1q(pTargetBaseCur);

                vchSrc = vandq(vchSrc, vBitMaskHi);
                vTarg = vdupq_m_n_u32(vTarg, hwForeColour, vcmpneq_n_u32(vchSrc, 0));

                vst1q(pTargetBaseCur, vTarg);
                pTargetBaseCur += 4;

                pchSourceBaseCur += 1;
                cnt -= 8;
            }

            /* tail */
            if (cnt > 4) {
                /* bottom part + upper residual parts */
                uint32x4_t      vchSrc =
                    vldrbq_gather_offset_u32(pchSourceBaseCur, offSLo);
                uint32x4_t      vTarg = vld1q(pTargetBaseCur);
                cnt -= 4;
                mve_pred16_t    p = vctp32q(cnt);

                vchSrc = vandq(vchSrc, vBitMaskLo);
                vTarg = vdupq_m_n_u32(vTarg, hwForeColour, vcmpneq_n_u32(vchSrc, 0));

                vst1q(pTargetBaseCur, vTarg);
                pTargetBaseCur += 4;

                vchSrc = vldrbq_gather_offset_u32(pchSourceBaseCur, offSHi);
                vTarg = vld1q(pTargetBaseCur);

                vchSrc = vandq(vchSrc, vBitMaskHi);
                vTarg = vdupq_m_n_u32(vTarg, hwForeColour, vcmpneq_n_u32(vchSrc, 0));

                vst1q_p(pTargetBaseCur, vTarg, p);
            } else if (cnt > 0) {
                /* bottom part residual */
                uint32x4_t      vchSrc =
                    vldrbq_gather_offset_u32(pchSourceBaseCur, offSLo);
                uint32x4_t      vTarg = vld1q(pTargetBaseCur);
                mve_pred16_t    p = vctp32q(cnt);

                vchSrc = vandq(vchSrc, vBitMaskLo);
                vTarg = vdupq_m_n_u32(vTarg, hwForeColour, vcmpneq_n_u32(vchSrc, 0));

                vst1q_p(pTargetBaseCur, vTarg, p);
            }

            pchSourceBase += (iSourceStride >> 3);
            pTargetBase += iTargetStride;
        }
    }
}



__OVERRIDE_WEAK
void __arm_2d_impl_rgb32_draw_pattern_no_bg_comp(uint8_t *__RESTRICT pchSourceBase,
                                         int32_t  iOffset,
                                         int16_t iSourceStride,
                                         uint32_t *__RESTRICT pTargetBase,
                                         int16_t iTargetStride,
                                         arm_2d_size_t *__RESTRICT ptCopySize)
{
    //! get in byte offset
    iOffset &= 0x07;
    iSourceStride = (iSourceStride + 7) & ~0x07;

    /* deduces offset vector from iOffset for gather loading */
    /* hold  8 contiguous values into 2 32-bit vector pair */
    uint32x4_t      offSLo = vld1q(__rgb32_draw_pattern_src_incr + iOffset);
    uint32x4_t      offSHi = vld1q(__rgb32_draw_pattern_src_incr + iOffset + 4);
    /* deduces bitmask vector with wrap from iOffset */
    /* hold  8 contiguous values into 2 32-bit vector pair */
    uint32x4_t      vBitMaskLo = vld1q(__rgb32_draw_pattern_src_bitmask + iOffset);
    uint32x4_t      vBitMaskHi = vld1q(__rgb32_draw_pattern_src_bitmask + iOffset + 4);


    if (ptCopySize->iWidth <= 4) {
        /* very tall width case */
        /* only bottom parts of gather load and bitmask needed */
        /* no inner loop */
        for (int32_t y = 0; y < ptCopySize->iHeight; y++) {
            mve_pred16_t    p = vctp32q(ptCopySize->iWidth);
            uint32x4_t      vchSrc = vldrbq_gather_offset_u32(pchSourceBase, offSLo);
            uint32x4_t      vTarg = vld1q(pTargetBase);

            vchSrc = vandq(vchSrc, vBitMaskLo);
            vTarg = vpselq(vTarg, ~vTarg, vcmpneq_n_u32(vchSrc, 0));

            vst1q_p(pTargetBase, vTarg, p);

            pchSourceBase += (iSourceStride >> 3);
            pTargetBase += iTargetStride;
        }
    } else if (ptCopySize->iWidth <= 8) {
        /* bottom and partial upper parts of gather load and bitmask needed */
        /* no inner loop */
        for (int32_t y = 0; y < ptCopySize->iHeight; y++) {
            mve_pred16_t    p = vctp32q(ptCopySize->iWidth - 4);
            uint32x4_t      vchSrc = vldrbq_gather_offset_u32(pchSourceBase, offSLo);
            uint32x4_t      vTarg = vld1q(pTargetBase);

            vchSrc = vandq(vchSrc, vBitMaskLo);
            vTarg = vpselq(vTarg, ~vTarg, vcmpneq_n_u32(vchSrc, 0));

            vst1q(pTargetBase, vTarg);

            vchSrc = vldrbq_gather_offset_u32(pchSourceBase, offSHi);
            vTarg = vld1q(pTargetBase + 4);

            vchSrc = vandq(vchSrc, vBitMaskHi);
            vTarg = vpselq(vTarg, ~vTarg, vcmpneq_n_u32(vchSrc, 0));

            vst1q_p(pTargetBase + 4, vTarg, p);

            pchSourceBase += (iSourceStride >> 3);
            pTargetBase += iTargetStride;
        }
    } else {
        /* generic case */
        for (int32_t y = 0; y < ptCopySize->iHeight; y++) {
            int32_t         cnt = ptCopySize->iWidth;
            uint8_t        *pchSourceBaseCur = pchSourceBase;
            uint32_t       *pTargetBaseCur = pTargetBase;

            while (cnt >= 8) {
                uint32x4_t      vchSrc =
                    vldrbq_gather_offset_u32(pchSourceBaseCur, offSLo);
                uint32x4_t      vTarg = vld1q(pTargetBaseCur);

                vchSrc = vandq(vchSrc, vBitMaskLo);
                vTarg = vpselq(vTarg, ~vTarg, vcmpneq_n_u32(vchSrc, 0));

                vst1q(pTargetBaseCur, vTarg);
                pTargetBaseCur += 4;

                vchSrc = vldrbq_gather_offset_u32(pchSourceBaseCur, offSHi);
                vTarg = vld1q(pTargetBaseCur);

                vchSrc = vandq(vchSrc, vBitMaskHi);
                vTarg = vpselq(vTarg, ~vTarg, vcmpneq_n_u32(vchSrc, 0));

                vst1q(pTargetBaseCur, vTarg);
                pTargetBaseCur += 4;

                pchSourceBaseCur += 1;
                cnt -= 8;
            }

            /* tail */
            if (cnt > 4) {
                /* bottom part + upper residual parts */
                uint32x4_t      vchSrc =
                    vldrbq_gather_offset_u32(pchSourceBaseCur, offSLo);
                uint32x4_t      vTarg = vld1q(pTargetBaseCur);
                cnt -= 4;
                mve_pred16_t    p = vctp32q(cnt);

                vchSrc = vandq(vchSrc, vBitMaskLo);
                vTarg = vpselq(vTarg, ~vTarg, vcmpneq_n_u32(vchSrc, 0));

                vst1q(pTargetBaseCur, vTarg);
                pTargetBaseCur += 4;

                vchSrc = vldrbq_gather_offset_u32(pchSourceBaseCur, offSHi);
                vTarg = vld1q(pTargetBaseCur);

                vchSrc = vandq(vchSrc, vBitMaskHi);
                vTarg = vpselq(vTarg, ~vTarg, vcmpneq_n_u32(vchSrc, 0));

                vst1q_p(pTargetBaseCur, vTarg, p);
            } else if (cnt > 0) {
                /* bottom part residual */
                uint32x4_t      vchSrc =
                    vldrbq_gather_offset_u32(pchSourceBaseCur, offSLo);
                uint32x4_t      vTarg = vld1q(pTargetBaseCur);
                mve_pred16_t    p = vctp32q(cnt);

                vchSrc = vandq(vchSrc, vBitMaskLo);
                vTarg = vpselq(vTarg, ~vTarg, vcmpneq_n_u32(vchSrc, 0));

                vst1q_p(pTargetBaseCur, vTarg, p);
            }

            pchSourceBase += (iSourceStride >> 3);
            pTargetBase += iTargetStride;
        }
    }
}





__OVERRIDE_WEAK
void __arm_2d_impl_rgb32_draw_pattern_bg_only(uint8_t *__RESTRICT pchSourceBase,
                                         int32_t  iOffset,
                                         int16_t iSourceStride,
                                         uint32_t *__RESTRICT pTargetBase,
                                         int16_t iTargetStride,
                                         arm_2d_size_t *__RESTRICT ptCopySize,
                                         uint32_t hwBackColour)
{
    //! get in byte offset
    iOffset &= 0x07;
    iSourceStride = (iSourceStride + 7) & ~0x07;

    /* deduces offset vector from iOffset for gather loading */
    /* hold  8 contiguous values into 2 32-bit vector pair */
    uint32x4_t      offSLo = vld1q(__rgb32_draw_pattern_src_incr + iOffset);
    uint32x4_t      offSHi = vld1q(__rgb32_draw_pattern_src_incr + iOffset + 4);
    /* deduces bitmask vector with wrap from iOffset */
    /* hold  8 contiguous values into 2 32-bit vector pair */
    uint32x4_t      vBitMaskLo = vld1q(__rgb32_draw_pattern_src_bitmask + iOffset);
    uint32x4_t      vBitMaskHi = vld1q(__rgb32_draw_pattern_src_bitmask + iOffset + 4);


    if (ptCopySize->iWidth <= 4) {
        /* very tall width case */
        /* only bottom parts of gather load and bitmask needed */
        /* no inner loop */
        for (int32_t y = 0; y < ptCopySize->iHeight; y++) {
            mve_pred16_t    p = vctp32q(ptCopySize->iWidth);
            uint32x4_t      vchSrc = vldrbq_gather_offset_u32(pchSourceBase, offSLo);
            uint32x4_t      vTarg = vld1q(pTargetBase);

            vchSrc = vandq(vchSrc, vBitMaskLo);
            vTarg = vdupq_m_n_u32(vTarg, hwBackColour, vcmpeqq_n_u32(vchSrc, 0));

            vst1q_p(pTargetBase, vTarg, p);

            pchSourceBase += (iSourceStride >> 3);
            pTargetBase += iTargetStride;
        }
    } else if (ptCopySize->iWidth <= 8) {
        /* bottom and partial upper parts of gather load and bitmask needed */
        /* no inner loop */
        for (int32_t y = 0; y < ptCopySize->iHeight; y++) {
            mve_pred16_t    p = vctp32q(ptCopySize->iWidth - 4);
            uint32x4_t      vchSrc = vldrbq_gather_offset_u32(pchSourceBase, offSLo);
            uint32x4_t      vTarg = vld1q(pTargetBase);

            vchSrc = vandq(vchSrc, vBitMaskLo);
            vTarg = vdupq_m_n_u32(vTarg, hwBackColour, vcmpeqq_n_u32(vchSrc, 0));

            vst1q(pTargetBase, vTarg);

            vchSrc = vldrbq_gather_offset_u32(pchSourceBase, offSHi);
            vTarg = vld1q(pTargetBase + 4);

            vchSrc = vandq(vchSrc, vBitMaskHi);
            vTarg = vdupq_m_n_u32(vTarg, hwBackColour, vcmpeqq_n_u32(vchSrc, 0));

            vst1q_p(pTargetBase + 4, vTarg, p);

            pchSourceBase += (iSourceStride >> 3);
            pTargetBase += iTargetStride;
        }
    } else {
        /* generic case */
        for (int32_t y = 0; y < ptCopySize->iHeight; y++) {
            int32_t         cnt = ptCopySize->iWidth;
            uint8_t        *pchSourceBaseCur = pchSourceBase;
            uint32_t       *pTargetBaseCur = pTargetBase;

            while (cnt >= 8) {
                uint32x4_t      vchSrc =
                    vldrbq_gather_offset_u32(pchSourceBaseCur, offSLo);
                uint32x4_t      vTarg = vld1q(pTargetBaseCur);

                vchSrc = vandq(vchSrc, vBitMaskLo);
                vTarg = vdupq_m_n_u32(vTarg, hwBackColour, vcmpeqq_n_u32(vchSrc, 0));

                vst1q(pTargetBaseCur, vTarg);
                pTargetBaseCur += 4;

                vchSrc = vldrbq_gather_offset_u32(pchSourceBaseCur, offSHi);
                vTarg = vld1q(pTargetBaseCur);

                vchSrc = vandq(vchSrc, vBitMaskHi);
                vTarg = vdupq_m_n_u32(vTarg, hwBackColour, vcmpeqq_n_u32(vchSrc, 0));

                vst1q(pTargetBaseCur, vTarg);
                pTargetBaseCur += 4;

                pchSourceBaseCur += 1;
                cnt -= 8;
            }

            /* tail */
            if (cnt > 4) {
                /* bottom part + upper residual parts */
                uint32x4_t      vchSrc =
                    vldrbq_gather_offset_u32(pchSourceBaseCur, offSLo);
                uint32x4_t      vTarg = vld1q(pTargetBaseCur);
                cnt -= 4;
                mve_pred16_t    p = vctp32q(cnt);

                vchSrc = vandq(vchSrc, vBitMaskLo);
                vTarg = vdupq_m_n_u32(vTarg, hwBackColour, vcmpeqq_n_u32(vchSrc, 0));

                vst1q(pTargetBaseCur, vTarg);
                pTargetBaseCur += 4;

                vchSrc = vldrbq_gather_offset_u32(pchSourceBaseCur, offSHi);
                vTarg = vld1q(pTargetBaseCur);

                vchSrc = vandq(vchSrc, vBitMaskHi);
                vTarg = vdupq_m_n_u32(vTarg, hwBackColour, vcmpeqq_n_u32(vchSrc, 0));

                vst1q_p(pTargetBaseCur, vTarg, p);
            } else if (cnt > 0) {
                /* bottom part residual */
                uint32x4_t      vchSrc =
                    vldrbq_gather_offset_u32(pchSourceBaseCur, offSLo);
                uint32x4_t      vTarg = vld1q(pTargetBaseCur);
                mve_pred16_t    p = vctp32q(cnt);

                vchSrc = vandq(vchSrc, vBitMaskLo);
                vTarg = vdupq_m_n_u32(vTarg, hwBackColour, vcmpeqq_n_u32(vchSrc, 0));

                vst1q_p(pTargetBaseCur, vTarg, p);
            }

            pchSourceBase += (iSourceStride >> 3);
            pTargetBase += iTargetStride;
        }
    }
}




__OVERRIDE_WEAK
void __arm_2d_impl_rgb32_draw_pattern_bg_fg(uint8_t *__RESTRICT pchSourceBase,
                                         int32_t  iOffset,
                                         int16_t iSourceStride,
                                         uint32_t *__RESTRICT pTargetBase,
                                         int16_t iTargetStride,
                                         arm_2d_size_t *__RESTRICT ptCopySize,
                                         uint32_t hwForeColour,
                                         uint32_t hwBackColour)
{
    //! get in byte offset
    iOffset &= 0x07;
    iSourceStride = (iSourceStride + 7) & ~0x07;

    /* deduces offset vector from iOffset for gather loading */
    /* hold  8 contiguous values into 2 32-bit vector pair */
    uint32x4_t      offSLo = vld1q(__rgb32_draw_pattern_src_incr + iOffset);
    uint32x4_t      offSHi = vld1q(__rgb32_draw_pattern_src_incr + iOffset + 4);
    /* deduces bitmask vector with wrap from iOffset */
    /* hold  8 contiguous values into 2 32-bit vector pair */
    uint32x4_t      vBitMaskLo = vld1q(__rgb32_draw_pattern_src_bitmask + iOffset);
    uint32x4_t      vBitMaskHi = vld1q(__rgb32_draw_pattern_src_bitmask + iOffset + 4);
    uint32x4_t      vFgColor = vdupq_n_u32(hwForeColour);
    uint32x4_t      vBgColor = vdupq_n_u32(hwBackColour);

    if (ptCopySize->iWidth <= 4) {
        /* very tall width case */
        /* only bottom parts of gather load and bitmask needed */
        /* no inner loop */
        for (int32_t y = 0; y < ptCopySize->iHeight; y++) {
            mve_pred16_t    p = vctp32q(ptCopySize->iWidth);
            uint32x4_t      vchSrc = vldrbq_gather_offset_u32(pchSourceBase, offSLo);
            uint32x4_t      vTarg;

            vchSrc = vandq(vchSrc, vBitMaskLo);
            vTarg =
                vpselq(vFgColor, vBgColor, vcmpneq_n_u32(vchSrc, 0));

            vst1q_p(pTargetBase, vTarg, p);

            pchSourceBase += (iSourceStride >> 3);
            pTargetBase += iTargetStride;
        }
    } else if (ptCopySize->iWidth <= 8) {
        /* bottom and partial upper parts of gather load and bitmask needed */
        /* no inner loop */
        for (int32_t y = 0; y < ptCopySize->iHeight; y++) {
            mve_pred16_t    p = vctp32q(ptCopySize->iWidth - 4);
            uint32x4_t      vchSrc = vldrbq_gather_offset_u32(pchSourceBase, offSLo);
            uint32x4_t      vTarg;

            vchSrc = vandq(vchSrc, vBitMaskLo);
            vTarg =
                vpselq(vFgColor, vBgColor, vcmpneq_n_u32(vchSrc, 0));

            vst1q(pTargetBase, vTarg);

            vchSrc = vldrbq_gather_offset_u32(pchSourceBase, offSHi);

            vchSrc = vandq(vchSrc, vBitMaskHi);
            vTarg =
                vpselq(vFgColor, vBgColor, vcmpneq_n_u32(vchSrc, 0));

            vst1q_p(pTargetBase + 4, vTarg, p);

            pchSourceBase += (iSourceStride >> 3);
            pTargetBase += iTargetStride;
        }
    } else {
        /* generic case */
        for (int32_t y = 0; y < ptCopySize->iHeight; y++) {
            int32_t         cnt = ptCopySize->iWidth;
            uint8_t        *pchSourceBaseCur = pchSourceBase;
            uint32_t       *pTargetBaseCur = pTargetBase;

            while (cnt >= 8) {
                uint32x4_t      vchSrc =
                    vldrbq_gather_offset_u32(pchSourceBaseCur, offSLo);
                uint32x4_t      vTarg;

                vchSrc = vandq(vchSrc, vBitMaskLo);
                vTarg =
                    vpselq(vFgColor, vBgColor, vcmpneq_n_u32(vchSrc, 0));

                vst1q(pTargetBaseCur, vTarg);
                pTargetBaseCur += 4;

                vchSrc = vldrbq_gather_offset_u32(pchSourceBaseCur, offSHi);

                vchSrc = vandq(vchSrc, vBitMaskHi);
                vTarg =
                    vpselq(vFgColor, vBgColor, vcmpneq_n_u32(vchSrc, 0));

                vst1q(pTargetBaseCur, vTarg);
                pTargetBaseCur += 4;

                pchSourceBaseCur += 1;
                cnt -= 8;
            }

            /* tail */
            if (cnt > 4) {
                /* bottom part + upper residual parts */
                uint32x4_t      vchSrc =
                    vldrbq_gather_offset_u32(pchSourceBaseCur, offSLo);
                uint32x4_t      vTarg;
                cnt -= 4;
                mve_pred16_t    p = vctp32q(cnt);

                vchSrc = vandq(vchSrc, vBitMaskLo);
                vTarg =
                    vpselq(vFgColor, vBgColor, vcmpneq_n_u32(vchSrc, 0));

                vst1q(pTargetBaseCur, vTarg);
                pTargetBaseCur += 4;

                vchSrc = vldrbq_gather_offset_u32(pchSourceBaseCur, offSHi);

                vchSrc = vandq(vchSrc, vBitMaskHi);
                vTarg =
                    vpselq(vFgColor, vBgColor, vcmpneq_n_u32(vchSrc, 0));

                vst1q_p(pTargetBaseCur, vTarg, p);
            } else if (cnt > 0) {
                /* bottom part residual */
                uint32x4_t      vchSrc =
                    vldrbq_gather_offset_u32(pchSourceBaseCur, offSLo);
                uint32x4_t      vTarg;
                mve_pred16_t    p = vctp32q(cnt);

                vchSrc = vandq(vchSrc, vBitMaskLo);
                vTarg =
                    vpselq(vFgColor, vBgColor, vcmpneq_n_u32(vchSrc, 0));

                vst1q_p(pTargetBaseCur, vTarg, p);
            }

            pchSourceBase += (iSourceStride >> 3);
            pTargetBase += iTargetStride;
        }
    }
}




__OVERRIDE_WEAK
void __arm_2d_impl_rgb32_draw_pattern_bg_comp(uint8_t *__RESTRICT pchSourceBase,
                                         int32_t  iOffset,
                                         int16_t iSourceStride,
                                         uint32_t *__RESTRICT pTargetBase,
                                         int16_t iTargetStride,
                                         arm_2d_size_t *__RESTRICT ptCopySize,
                                         uint32_t hwBackColour)
{
    //! get in byte offset
    iOffset &= 0x07;
    iSourceStride = (iSourceStride + 7) & ~0x07;

    /* deduces offset vector from iOffset for gather loading */
    /* hold  8 contiguous values into 2 32-bit vector pair */
    uint32x4_t      offSLo = vld1q(__rgb32_draw_pattern_src_incr + iOffset);
    uint32x4_t      offSHi = vld1q(__rgb32_draw_pattern_src_incr + iOffset + 4);
    /* deduces bitmask vector with wrap from iOffset */
    /* hold  8 contiguous values into 2 32-bit vector pair */
    uint32x4_t      vBitMaskLo = vld1q(__rgb32_draw_pattern_src_bitmask + iOffset);
    uint32x4_t      vBitMaskHi = vld1q(__rgb32_draw_pattern_src_bitmask + iOffset + 4);
    uint32x4_t      vBgColor = vdupq_n_u32(hwBackColour);


    if (ptCopySize->iWidth <= 4) {
        /* very tall width case */
        /* only bottom parts of gather load and bitmask needed */
        /* no inner loop */
        for (int32_t y = 0; y < ptCopySize->iHeight; y++) {
            mve_pred16_t    p = vctp32q(ptCopySize->iWidth);
            uint32x4_t      vchSrc = vldrbq_gather_offset_u32(pchSourceBase, offSLo);
            uint32x4_t      vTarg = vld1q(pTargetBase);

            vchSrc = vandq(vchSrc, vBitMaskLo);
            vTarg = vpselq(vmvnq(vTarg), vBgColor, vcmpneq_n_u32(vchSrc, 0));

            vst1q_p(pTargetBase, vTarg, p);

            pchSourceBase += (iSourceStride >> 3);
            pTargetBase += iTargetStride;
        }
    } else if (ptCopySize->iWidth <= 8) {
        /* bottom and partial upper parts of gather load and bitmask needed */
        /* no inner loop */
        for (int32_t y = 0; y < ptCopySize->iHeight; y++) {
            mve_pred16_t    p = vctp32q(ptCopySize->iWidth - 4);
            uint32x4_t      vchSrc = vldrbq_gather_offset_u32(pchSourceBase, offSLo);
            uint32x4_t      vTarg = vld1q(pTargetBase);

            vchSrc = vandq(vchSrc, vBitMaskLo);
            vTarg = vpselq(vmvnq(vTarg), vBgColor, vcmpneq_n_u32(vchSrc, 0));

            vst1q(pTargetBase, vTarg);

            vchSrc = vldrbq_gather_offset_u32(pchSourceBase, offSHi);
            vTarg = vld1q(pTargetBase + 4);

            vchSrc = vandq(vchSrc, vBitMaskHi);
            vTarg = vpselq(vmvnq(vTarg), vBgColor, vcmpneq_n_u32(vchSrc, 0));

            vst1q_p(pTargetBase + 4, vTarg, p);

            pchSourceBase += (iSourceStride >> 3);
            pTargetBase += iTargetStride;
        }
    } else {
        /* generic case */
        for (int32_t y = 0; y < ptCopySize->iHeight; y++) {
            int32_t         cnt = ptCopySize->iWidth;
            uint8_t        *pchSourceBaseCur = pchSourceBase;
            uint32_t       *pTargetBaseCur = pTargetBase;

            while (cnt >= 8) {
                uint32x4_t      vchSrc =
                    vldrbq_gather_offset_u32(pchSourceBaseCur, offSLo);
                uint32x4_t      vTarg = vld1q(pTargetBaseCur);

                vchSrc = vandq(vchSrc, vBitMaskLo);
                vTarg = vpselq(vmvnq(vTarg), vBgColor, vcmpneq_n_u32(vchSrc, 0));

                vst1q(pTargetBaseCur, vTarg);
                pTargetBaseCur += 4;

                vchSrc = vldrbq_gather_offset_u32(pchSourceBaseCur, offSHi);
                vTarg = vld1q(pTargetBaseCur);

                vchSrc = vandq(vchSrc, vBitMaskHi);
                vTarg = vpselq(vmvnq(vTarg), vBgColor, vcmpneq_n_u32(vchSrc, 0));

                vst1q(pTargetBaseCur, vTarg);
                pTargetBaseCur += 4;

                pchSourceBaseCur += 1;
                cnt -= 8;
            }

            /* tail */
            if (cnt > 4) {
                /* bottom part + upper residual parts */
                uint32x4_t      vchSrc =
                    vldrbq_gather_offset_u32(pchSourceBaseCur, offSLo);
                uint32x4_t      vTarg = vld1q(pTargetBaseCur);
                cnt -= 4;
                mve_pred16_t    p = vctp32q(cnt);

                vchSrc = vandq(vchSrc, vBitMaskLo);
                vTarg = vpselq(vmvnq(vTarg), vBgColor, vcmpneq_n_u32(vchSrc, 0));

                vst1q(pTargetBaseCur, vTarg);
                pTargetBaseCur += 4;

                vchSrc = vldrbq_gather_offset_u32(pchSourceBaseCur, offSHi);
                vTarg = vld1q(pTargetBaseCur);

                vchSrc = vandq(vchSrc, vBitMaskHi);
                vTarg = vpselq(vmvnq(vTarg), vBgColor, vcmpneq_n_u32(vchSrc, 0));

                vst1q_p(pTargetBaseCur, vTarg, p);
            } else if (cnt > 0) {
                /* bottom part residual */
                uint32x4_t      vchSrc =
                    vldrbq_gather_offset_u32(pchSourceBaseCur, offSLo);
                uint32x4_t      vTarg = vld1q(pTargetBaseCur);
                mve_pred16_t    p = vctp32q(cnt);

                vchSrc = vandq(vchSrc, vBitMaskLo);
                vTarg = vpselq(vmvnq(vTarg), vBgColor, vcmpneq_n_u32(vchSrc, 0));

                vst1q_p(pTargetBaseCur, vTarg, p);
            }

            pchSourceBase += (iSourceStride >> 3);
            pTargetBase += iTargetStride;
        }
    }
}


/**
  8-bit pixel color fill alpha/channel mask with/without opacity MVE intrinsic generator
   - TRGT_LOAD is a contigous / strided target load function
        C8BIT_TRGT_LOAD / C8BIT_TRGT_LOAD_STRIDE
  - STRIDE is an optional vector of offset for gather load
  - SCAL_OPACITY is extra alpha scaling function
        C8BIT_SCAL_OPACITY_NONE / C8BIT_SCAL_OPACITY
  - OPACITY is an optinal 8-bit vector with duplicated opacity values
  (need vector format to be used with VMULH.U8)
  - ALPHA_SZ, alpha chan width (1 or 4 for resp. 8 or 32-bit type)

  Macro assumes pTarget8/ pAlpha are already setup
 */

#define C8BIT_COLOUR_FILLING_MASK_INNER_MVE(TRGT_LOAD, STRIDE, SCAL_OPACITY,                   \
                                                                      OPACITY, ALPHA_SZ)       \
        int32_t         blkCnt = iWidth;                                                       \
        do {                                                                                   \
            mve_pred16_t    tailPred = vctp16q(blkCnt);                                        \
                                                                                               \
            uint16x8_t      vecTarget = vldrbq_z_u16(pTarget8, tailPred);                      \
            uint16x8_t      vecTransp = TRGT_LOAD(pAlpha, STRIDE, tailPred);                   \
                                                                                               \
            vecTransp = SCAL_OPACITY(vecTransp, OPACITY, tailPred);                            \
            uint16x8_t      vecAlpha = vsubq_x_u16(v256, vecTransp, tailPred);                 \
                                                                                               \
            vecTarget = vmulq_x(vecTarget, vecAlpha, tailPred);                                \
            vecTarget = vmlaq_m(vecTarget, vecTransp, (uint16_t) Colour, tailPred);            \
            vecTarget = vecTarget >> 8;                                                        \
                                                                                               \
            vstrbq_p_u16(pTarget8, vecTarget, tailPred);                                       \
                                                                                               \
            pAlpha += (8 * ALPHA_SZ);                                                          \
            pTarget8 += 8;                                                                     \
            blkCnt -= 8;                                                                       \
        }                                                                                      \
        while (blkCnt > 0);


/**
  RGB565 pixel color fill alpha/channel mask with/without opacity MVE intrinsic generator
   - TRGT_LOAD is a contigous / strided target load function
        RGB565_TRGT_LOAD / RGB565_TRGT_LOAD_STRIDE
  - STRIDE is an optional vector of offset for gather load
  - SCAL_OPACITY is extra alpha scaling function
        RGB565_SCAL_OPACITY_NONE / RGB565_SCAL_OPACITY
  - OPACITY is an optinal 8-bit vector with duplicated opacity values
  (need vector format to be used with VMULH.U8)
  - P_ALPHA, 8-bit or 32-bit alpha chan. pointer
  - ALPHA_SZ, alpha chan width (1 or 4 for resp. 8 or 32-bit type)

  Does not generate a tail-predicated loop as relying on pack/unpack functions.
  Predication is only applied in the final stage during pixel store.
 */

#define RGB565_COLOUR_FILLING_MASK_MVE(TRGT_LOAD, STRIDE, SCAL_OPACITY, OPACITY,               \
                                                                 P_ALPHA,  ALPHA_SZ)           \
    uint16x8_t      v256 = vdupq_n_u16(256);                                                   \
                                                                                               \
    for (int_fast16_t y = 0; y < iHeight; y++) {                                               \
        const uint8_t  *pAlpha = (const uint8_t *)P_ALPHA;                                     \
        uint16_t       *pCurTarget = pTarget;                                                  \
        int32_t         blkCnt = iWidth;                                                       \
                                                                                               \
        do {                                                                                   \
            uint16x8_t      vecTarget = vld1q(pCurTarget);                                     \
            uint16x8_t      vecTransp = TRGT_LOAD(pAlpha, STRIDE);                             \
            vecTransp = SCAL_OPACITY(vecTransp, OPACITY);                                      \
            uint16x8_t      vecAlpha = vsubq_u16(v256, vecTransp);                             \
            uint16x8_t      vecR, vecG, vecB;                                                  \
                                                                                               \
            arm_2d_unpack_rgb565_single_vec(vecTarget, &vecR, &vecG, &vecB);                   \
                                                                                               \
            /* blending using alpha vector weights */                                          \
            vecR = vmulq(vecR, vecAlpha);                                                      \
            vecR = vmlaq(vecR, vecTransp, (uint16_t) tSrcPix.R);                               \
            vecR = vecR >> 8;                                                                  \
                                                                                               \
            vecG = vmulq(vecG, vecAlpha);                                                      \
            vecG = vmlaq(vecG, vecTransp, (uint16_t) tSrcPix.G);                               \
            vecG = vecG >> 8;                                                                  \
                                                                                               \
            vecB = vmulq(vecB, vecAlpha);                                                      \
            vecB = vmlaq(vecB, vecTransp, (uint16_t) tSrcPix.B);                               \
            vecB = vecB >> 8;                                                                  \
                                                                                               \
            vecTarget = arm_2d_pack_rgb565_single_vec(vecR, vecG, vecB);                       \
                                                                                               \
            /* tail predication */                                                             \
            vst1q_p_u16(pCurTarget, vecTarget, vctp16q(blkCnt));                               \
                                                                                               \
            pAlpha += (8 *  ALPHA_SZ);                                                         \
            pCurTarget += 8;                                                                   \
            blkCnt -= 8;                                                                       \
        }                                                                                      \
        while (blkCnt > 0);                                                                    \
                                                                                               \
        P_ALPHA += (iAlphaStride);                                                             \
        pTarget += (iTargetStride);                                                            \
    }


/**
  CCCN888 pixel color fill alpha/channel mask with/without opacity MVE intrinsic generator
   - TRGT_LOAD is a contigous / strided target load function
        CCCN888_TRGT_LOAD / CCCN888_TRGT_LOAD_STRIDE
  - STRIDE is an optional vector of offset for gather load
  - SCAL_OPACITY is extra alpha scaling function
        CCCN888_SCAL_OPACITY_NONE / CCCN888_SCAL_OPACITY
  - OPACITY is an optinal 8-bit vector with duplicated opacity values
  (need vector format to be used with VMULH.U8)
  - ALPHA_SZ, alpha chan width (1 or 4 for resp. 8 or 32-bit type)

  Macro assumes pTargetCh0/1/2 & pAlpha are already setup
 */

#define CCCN888_COLOUR_FILLING_MASK_INNER_MVE(TRGT_LOAD, STRIDE, SCAL_OPACITY,                 \
                                                                           OPACITY, ALPHA_SZ)  \
        int32_t         blkCnt = iWidth;                                                       \
                                                                                               \
        do {                                                                                   \
            mve_pred16_t    tailPred = vctp16q(blkCnt);                                        \
                                                                                               \
            /* expand chan0, chan1, chan2 */                                                   \
            uint16x8_t      vecTargetC0 = vldrbq_gather_offset_z_u16(pTargetCh0, vStride4Offs, \
                tailPred);                                                                     \
            uint16x8_t      vecTargetC1 = vldrbq_gather_offset_z_u16(pTargetCh1, vStride4Offs, \
                tailPred);                                                                     \
            uint16x8_t      vecTargetC2 = vldrbq_gather_offset_z_u16(pTargetCh2, vStride4Offs, \
                tailPred);                                                                     \
            uint16x8_t      vecTransp = TRGT_LOAD(pAlpha, STRIDE, tailPred);                   \
                                                                                               \
            vecTransp = SCAL_OPACITY(vecTransp, OPACITY, tailPred);                            \
                                                                                               \
            uint16x8_t      vecAlpha = vsubq_x_u16(v256, vecTransp, tailPred);                 \
                                                                                               \
            /*  scale ch0 vector with alpha vector */                                          \
            vecTargetC0 = vmulq_x(vecTargetC0, vecAlpha, tailPred);                            \
            /*  blend ch0 vector with input ch0 color*/                                        \
            vecTargetC0 = vmlaq_m(vecTargetC0, vecTransp, (uint16_t) c0, tailPred);            \
            vecTargetC0 = vecTargetC0 >> 8;                                                    \
                                                                                               \
            /* repeat for ch1 and ch2 */                                                       \
            vecTargetC1 = vmulq_x(vecTargetC1, vecAlpha, tailPred);                            \
            vecTargetC1 = vmlaq_m(vecTargetC1, vecTransp, (uint16_t) c1, tailPred);            \
            vecTargetC1 = vecTargetC1 >> 8;                                                    \
                                                                                               \
            vecTargetC2 = vmulq_x(vecTargetC2, vecAlpha, tailPred);                            \
            vecTargetC2 = vmlaq_m(vecTargetC2, vecTransp, (uint16_t) c2, tailPred);            \
            vecTargetC2 = vecTargetC2 >> 8;                                                    \
                                                                                               \
            /* store and merge chan0, chan1, chan2 */                                          \
            vstrbq_scatter_offset_p_u16(pTargetCh0, vStride4Offs, vecTargetC0, tailPred);      \
            vstrbq_scatter_offset_p_u16(pTargetCh1, vStride4Offs, vecTargetC1, tailPred);      \
            vstrbq_scatter_offset_p_u16(pTargetCh2, vStride4Offs, vecTargetC2, tailPred);      \
                                                                                               \
            pAlpha += 8 * ALPHA_SZ;                                                            \
            pTargetCh0 += 8*4;                                                                 \
            pTargetCh1 += 8*4;                                                                 \
            pTargetCh2 += 8*4;                                                                 \
            blkCnt -= 8;                                                                       \
        }                                                                                      \
        while (blkCnt > 0);



#define C8BIT_TRGT_LOAD(base, stride, pred)             vldrbq_z_u16(base, pred)
#define C8BIT_TRGT_LOAD_STRIDE(base, stride, pred)      vldrbq_gather_offset_z_u16(base, stride, pred);
#define C8BIT_SCAL_OPACITY_NONE(transp, opac, pred)     transp
#define C8BIT_SCAL_OPACITY(transp, opac, pred)          (uint16x8_t) vmulhq_x((uint8x16_t) transp, opac, pred)

#define RGB565_TRGT_LOAD(base, stride)                  vldrbq_u16(base)
#define RGB565_TRGT_LOAD_STRIDE(base, stride)           vldrbq_gather_offset_u16(base, stride);
#define RGB565_SCAL_OPACITY_NONE(transp, opac)          transp
#define RGB565_SCAL_OPACITY(transp, opac)               (uint16x8_t) vmulhq((uint8x16_t) transp, opac)

#define CCCN888_TRGT_LOAD(base, stride, pred)           vldrbq_z_u16(base, pred)
#define CCCN888_TRGT_LOAD_STRIDE(base, stride, pred)    vldrbq_gather_offset_z_u16(base, stride, pred);
#define CCCN888_SCAL_OPACITY_NONE(transp, opac, pred)   transp
#define CCCN888_SCAL_OPACITY(transp, opac, pred)        (uint16x8_t) vmulhq_x((uint8x16_t) transp, opac, pred)



__OVERRIDE_WEAK
void __arm_2d_impl_c8bit_colour_filling_mask(uint8_t * __RESTRICT pTarget,
                                                       int16_t iTargetStride,
                                                       uint8_t * __RESTRICT pchAlpha,
                                                       int16_t iAlphaStride,
                                                       arm_2d_size_t * __RESTRICT ptCopySize,
                                                       uint8_t Colour)
{
    int_fast16_t    iHeight = ptCopySize->iHeight;
    int_fast16_t    iWidth = ptCopySize->iWidth;
    uint16x8_t      v256 = vdupq_n_u16(256);

    for (int_fast16_t y = 0; y < iHeight; y++) {
        const uint8_t * pAlpha = pchAlpha;
        uint8_t *       pTarget8 = pTarget;

#ifdef USE_MVE_INTRINSICS
        C8BIT_COLOUR_FILLING_MASK_INNER_MVE(C8BIT_TRGT_LOAD, _,
                                        C8BIT_SCAL_OPACITY_NONE, _, 1);
#else
        register unsigned blkCnt __asm("lr");
        blkCnt = iWidth;

    __asm volatile(
            ".p2align 2                                              \n"
            "   vldrb.u16               q0, [%[pTarget]]             \n"
            "   vldrb.u16               q1, [%[pAlpha]], #8          \n"
            "   wlstp.16                lr, %[loopCnt], 1f           \n"
            "2:                                                      \n"
            "   vsub.i16                q2, %[vec256], q1            \n"
            "   vmul.u16                q3, q0, q2                   \n"
            "   vldrb.u16               q0, [%[pTarget], #8]         \n"
            "   vmla.u16                q3, q1, %[Colour]            \n"
            "   vldrb.u16               q1, [%[pAlpha]], #8          \n"
            "   vshr.u16                q3, q3, #8                   \n"
            "   vstrb.u16               q3, [%[pTarget]], #8         \n"
            "   letp                    lr, 2b                       \n"
            "1:                                                      \n"

            : [pTarget] "+r"(pTarget8),  [pAlpha] "+r" (pAlpha), [loopCnt] "+r"(blkCnt)
            :[vec256] "t"   (v256),[Colour] "r"(Colour)
            :"q0", "q1", "q2", "q3", "memory");

#endif
        pchAlpha += (iAlphaStride);
        pTarget += (iTargetStride);
    }
}

__OVERRIDE_WEAK
void __arm_2d_impl_c8bit_colour_filling_mask_opacity(uint8_t * __RESTRICT pTarget,
                                                               int16_t iTargetStride,
                                                               uint8_t * __RESTRICT pchAlpha,
                                                               int16_t iAlphaStride,
                                                               arm_2d_size_t *
                                                               __RESTRICT ptCopySize,
                                                               uint8_t Colour, uint8_t chOpacity)
{
    int_fast16_t    iHeight = ptCopySize->iHeight;
    int_fast16_t    iWidth = ptCopySize->iWidth;
    uint8x16_t      vOpacity = vdupq_n_u8(chOpacity);
    uint16x8_t      v256 = vdupq_n_u16(256);

    for (int_fast16_t y = 0; y < iHeight; y++) {
        const uint8_t * pAlpha = pchAlpha;
        uint8_t *       pTarget8 = pTarget;

#ifdef USE_MVE_INTRINSICS
        C8BIT_COLOUR_FILLING_MASK_INNER_MVE(C8BIT_TRGT_LOAD, _,
                                        C8BIT_SCAL_OPACITY, vOpacity, 1);
#else
        register unsigned blkCnt __asm("lr");
        blkCnt = iWidth;

    __asm volatile(
        ".p2align 2                                              \n"
        "   vldrb.u16               q0, [%[pTarget]]             \n"
        "   vldrb.u16               q1, [%[pAlpha]], #8          \n"
        "   vmulh.u8                q1, q1, %[vOpacity]          \n"
        "   wlstp.16                lr, %[loopCnt], 1f           \n"
        "2:                                                      \n"
        "   vsub.i16                q2, %[vec256], q1            \n"
        "   vmul.u16                q3, q0, q2                   \n"
        "   vldrb.u16               q0, [%[pTarget], #8]         \n"
        "   vmla.u16                q3, q1, %[Colour]            \n"
        "   vldrb.u16               q1, [%[pAlpha]], #8          \n"
        "   vmulh.u8                q1, q1, %[vOpacity]          \n"
        "   vshr.u16                q3, q3, #8                   \n"
        "   vstrb.u16               q3, [%[pTarget]], #8         \n"
        "   letp                    lr, 2b                       \n"
        "1:                                                      \n"

        : [pTarget] "+r"(pTarget8),  [pAlpha] "+r" (pAlpha), [loopCnt] "+r"(blkCnt)
        :[vec256] "t"   (v256),[Colour] "r"(Colour),[vOpacity] "t"(vOpacity)
        :"q0", "q1", "q2", "q3", "memory");

#endif
        pchAlpha += (iAlphaStride);
        pTarget += (iTargetStride);
    }
}


__OVERRIDE_WEAK
void __arm_2d_impl_c8bit_colour_filling_channel_mask(uint8_t * __RESTRICT pTarget,
                                                         int16_t iTargetStride,
                                                         uint32_t * __RESTRICT pwAlpha,
                                                         int16_t iAlphaStride,
                                                         arm_2d_size_t * __RESTRICT ptCopySize,
                                                         uint8_t Colour)
{
    int_fast16_t    iHeight = ptCopySize->iHeight;
    int_fast16_t    iWidth = ptCopySize->iWidth;
    uint16x8_t      v256 = vdupq_n_u16(256);
    uint16x8_t      vStride4Offs = vidupq_n_u16(0, 4);

    for (int_fast16_t y = 0; y < iHeight; y++) {
        const uint8_t  *pAlpha = (const uint8_t *)pwAlpha;
        uint8_t *       pTarget8 = pTarget;

#ifdef USE_MVE_INTRINSICS
        C8BIT_COLOUR_FILLING_MASK_INNER_MVE(C8BIT_TRGT_LOAD_STRIDE, vStride4Offs,
                                        C8BIT_SCAL_OPACITY_NONE, _, 4);
#else
        register unsigned blkCnt __asm("lr");
        blkCnt = iWidth;

    __asm volatile(
        ".p2align 2                                              \n"
        "   vldrb.u16               q0, [%[pTarget]]             \n"
        "   vldrb.u16               q1, [%[pAlpha], %[str4Offs]] \n"
        "   wlstp.16                lr, %[loopCnt], 1f           \n"
        "2:                                                      \n"
        "   vsub.i16                q2, %[vec256], q1            \n"
        "   vmul.u16                q3, q0, q2                   \n"
        "   add                     %[pAlpha], %[pAlpha], #(8*4) \n"
        "   vldrb.u16               q0, [%[pTarget], #8]         \n"
        "   vmla.u16                q3, q1, %[Colour]            \n"
        "   vldrb.u16               q1, [%[pAlpha], %[str4Offs]] \n"
        "   vshr.u16                q3, q3, #8                   \n"
        "   vstrb.u16               q3, [%[pTarget]], #8         \n"
        "   letp                    lr, 2b                       \n"
        "1:                                                      \n"

        : [pTarget] "+r"(pTarget8),  [pAlpha] "+r" (pAlpha), [loopCnt] "+r"(blkCnt)
        :[vec256] "t"   (v256),[Colour] "r"(Colour),[str4Offs] "t"(vStride4Offs)
        :"q0", "q1", "q2", "q3", "memory");

#endif
        pwAlpha += (iAlphaStride);
        pTarget += (iTargetStride);
    }
}




__OVERRIDE_WEAK
void __arm_2d_impl_c8bit_colour_filling_channel_mask_opacity(uint8_t * __RESTRICT pTarget,
                                                                 int16_t iTargetStride,
                                                                 uint32_t * __RESTRICT pwAlpha,
                                                                 int16_t iAlphaStride,
                                                                 arm_2d_size_t *
                                                                 __RESTRICT ptCopySize,
                                                                 uint8_t Colour, uint8_t chOpacity)
{
    int_fast16_t    iHeight = ptCopySize->iHeight;
    int_fast16_t    iWidth = ptCopySize->iWidth;
    uint8x16_t      vOpacity = vdupq_n_u8(chOpacity);
    uint16x8_t      v256 = vdupq_n_u16(256);
    uint16x8_t      vStride4Offs = vidupq_n_u16(0, 4);

    for (int_fast16_t y = 0; y < iHeight; y++) {
        const uint8_t  *pAlpha = (const uint8_t *)pwAlpha;
        uint8_t        *pTarget8 = pTarget;

#ifdef USE_MVE_INTRINSICS
        C8BIT_COLOUR_FILLING_MASK_INNER_MVE(C8BIT_TRGT_LOAD_STRIDE, vStride4Offs,
                                        C8BIT_SCAL_OPACITY, vOpacity, 4);
#else
        register unsigned blkCnt __asm("lr");
        blkCnt = iWidth;

    __asm volatile(
        ".p2align 2                                              \n"
        "   vldrb.u16               q0, [%[pTarget]]             \n"
        "   vldrb.u16               q1, [%[pAlpha], %[str4Offs]] \n"
        "   vmulh.u8                q1, q1, %[vOpacity]          \n"
        "   wlstp.16                lr, %[loopCnt], 1f           \n"
        "2:                                                      \n"
        "   vsub.i16                q2, %[vec256], q1            \n"
        "   vmul.u16                q3, q0, q2                   \n"
        "   vldrb.u16               q0, [%[pTarget], #8]         \n"
        "   add                     %[pAlpha], %[pAlpha], #(8*4) \n"
        "   vmla.u16                q3, q1, %[Colour]            \n"
        "   vldrb.u16               q1, [%[pAlpha], %[str4Offs]] \n"
        "   vmulh.u8                q1, q1, %[vOpacity]          \n"
        "   vshr.u16                q3, q3, #8                   \n"
        "   vstrb.u16               q3, [%[pTarget]], #8         \n"
        "   letp                    lr, 2b                       \n"
        "1:                                                      \n"

        : [pTarget] "+r"(pTarget8),  [pAlpha] "+r" (pAlpha), [loopCnt] "+r"(blkCnt)
        :[vec256] "t"   (v256),[Colour] "r"(Colour),[vOpacity] "t"(vOpacity),
         [str4Offs] "t"(vStride4Offs)
        :"q0", "q1", "q2", "q3", "memory");

#endif
        pwAlpha += (iAlphaStride);
        pTarget += (iTargetStride);
    }
}


__OVERRIDE_WEAK
void __arm_2d_impl_rgb565_colour_filling_mask(uint16_t * __RESTRICT pTarget,
                                                    int16_t iTargetStride,
                                                    uint8_t * __RESTRICT pchAlpha,
                                                    int16_t iAlphaStride,
                                                    arm_2d_size_t * __RESTRICT ptCopySize,
                                                    uint16_t Colour)
{
    int_fast16_t    iHeight = ptCopySize->iHeight;
    int_fast16_t    iWidth = ptCopySize->iWidth;
    __arm_2d_color_fast_rgb_t tSrcPix;

    __arm_2d_rgb565_unpack(*(&Colour), &tSrcPix);

#ifdef USE_MVE_INTRINSICS
    RGB565_COLOUR_FILLING_MASK_MVE(RGB565_TRGT_LOAD, _,
                                            RGB565_SCAL_OPACITY_NONE, _, pchAlpha, 1);
#else
    /* RGB565 pack/unpack Masks */
    /* use memory rather than vmov to optimize Helium operations interleaving */
    uint16x8_t scratch[4];

    // Unpacking Mask Red
    vst1q((uint16_t*)&scratch[0], vdupq_n_u16(0x1f));
    // Unpacking Mask Green
    vst1q((uint16_t*)&scratch[1], vdupq_n_u16(0x3f));
    // packing Mask Green
    vst1q((uint16_t*)&scratch[2], vdupq_n_u16(0xfc));
    // packing Mask Blue
    vst1q((uint16_t*)&scratch[3], vdupq_n_u16(0xf8));

    for (int_fast16_t y = 0; y < iHeight; y++) {
        const uint8_t  *pAlpha = pchAlpha;
        uint16_t       *pCurTarget = pTarget;
        register unsigned blkCnt __asm("lr");
        blkCnt = iWidth;

       __asm volatile  (
            ".p2align 2                                            \n"
            /* load scheduling */
            "   vldrh.u16               q0, [%[pTarget]]           \n"
            "   vmov.i16                q7, #0x0100                \n"
            "   vldrb.u16               q1, [%[pAlpha]], #8        \n"

            "   wlstp.16                lr, %[loopCnt], 1f         \n"
            "2:                                                    \n"

            // vecAlpha
            "   vsub.i16                q2, q7, q1                 \n"

            /*  RGB565 unpack          */

            /* vecAlpha * 4 for G channel upscale */
            "   vmul.i16                q2, q2, %[four]            \n"
            /* G channel extract */
            "   vshr.u16                q5, q0, #5                 \n"

            /* load Unpacking Mask for R channel */
            "   vldrh.u16               q7, [%[scratch], #(0*16)]  \n"
            "   vand                    q4, q0, q7                 \n"

            /* load Unpacking Mask for G channel */
            "   vldrh.u16               q7, [%[scratch], #(1*16)]  \n"
            "   vand                    q5, q5, q7                 \n"
            /*  scale G vector with alpha vector */
            "   vmul.u16                q5, q5, q2                 \n"

            /* B channel */
            "   vshr.u16                q6, q0, #11                \n"

            /*  blend G vector with input G color*/
            "   vmla.u16                q5, q1, %[G]               \n"

            /* vecAlpha * 8 for R & B  upscale */
            "   vshl.i16                q2, q2, #1                 \n"

            /*  scale R vector with alpha vec */
            "   vmul.u16                q4, q4, q2                 \n"

            "   vshr.u16                q5, q5, #8                 \n"

            /*  blend R vector with input R color*/
            "   vmla.u16                q4, q1, %[R]               \n"

            /* load packing Mask for G channel */
            "   vldrh.u16               q7, [%[scratch], #(2*16)]  \n"

            /*  scale B vector with alpha vector */
            "   vmul.u16                q6, q6, q2                 \n"
            "   vand                    q5, q5, q7                 \n"

            /*  blend B vector with input B color*/
            "   vmla.u16                q6, q1, %[B]               \n"

            /* load packing Mask for B channel */
            "   vldrh.u16               q7, [%[scratch], #(3*16)]  \n"

            "   vshr.u16                q6, q6, #8                 \n"

           /* RGB 565 pack */

            /* (G & 0x00fc), 8) */
            "   vmul.i16                q5, q5, %[eight]           \n"

            /* (B & 0x00f8) */
            "   vand                    q6, q6, q7                 \n"

            /* load next alpha vector */
            "   vldrb.u16               q1, [%[pAlpha]], #8        \n"
            "   vmov.i16                q7, #0x0100                \n"

            /* pack G & B */
            "   vmla.u16                q5, q6, %[twofiftysix]     \n"
            /* combined (R >> 8) >> 3 */
            "   vshr.u16                q4, q4, #11                \n"

            /* load next target */
            "   vldrh.u16               q0, [%[pTarget], #16]      \n"

            /*  pack R */
            "   vorr                    q4, q4, q5                 \n"
            "   vstrh.16                q4, [%[pTarget]], #16      \n"
            "   letp                    lr, 2b                     \n"
            "1:                                                    \n"
            :[pTarget]"+r"(pCurTarget),[pAlpha] "+r"(pAlpha),[loopCnt] "+r"(blkCnt)
            :[Colour] "r"(Colour), [eight] "r" (8), [four] "r" (4),
            [R] "r" (tSrcPix.R), [G] "r" (tSrcPix.G), [B] "r" (tSrcPix.B),
            [twofiftysix] "r" (256), [scratch] "r" (scratch)
            :"q0", "q1", "q2", "q3", "q4", "q5", "q6", "q7", "memory");

        pchAlpha += (iAlphaStride);
        pTarget += (iTargetStride);
    }
#endif
}



__OVERRIDE_WEAK
void __arm_2d_impl_rgb565_colour_filling_mask_opacity(uint16_t * __RESTRICT pTarget,
                                                    int16_t iTargetStride,
                                                    uint8_t * __RESTRICT pchAlpha,
                                                    int16_t iAlphaStride,
                                                    arm_2d_size_t * __RESTRICT ptCopySize,
                                                    uint16_t Colour, uint8_t chOpacity)
{
    int_fast16_t    iHeight = ptCopySize->iHeight;
    int_fast16_t    iWidth = ptCopySize->iWidth;
    uint8x16_t      vOpacity = vdupq_n_u8(chOpacity);

    __arm_2d_color_fast_rgb_t tSrcPix;

    __arm_2d_rgb565_unpack(*(&Colour), &tSrcPix);

#ifdef USE_MVE_INTRINSICS
    RGB565_COLOUR_FILLING_MASK_MVE(RGB565_TRGT_LOAD, _,
                                            RGB565_SCAL_OPACITY, vOpacity, pchAlpha, 1);
#else
    /* RGB565 pack/unpack Masks + opacity */
    /* use memory rather than vmov to optimize Helium operations interleaving */
    uint16x8_t scratch[5];

    // Unpacking Mask Red
    vst1q((uint16_t*)&scratch[0], vdupq_n_u16(0x1f));
    // Unpacking Mask Green
    vst1q((uint16_t*)&scratch[1], vdupq_n_u16(0x3f));
    // packing Mask Green
    vst1q((uint16_t*)&scratch[2], vdupq_n_u16(0xfc));
    // packing Mask Blue
    vst1q((uint16_t*)&scratch[3], vdupq_n_u16(0xf8));
    // opacity
    vst1q((uint16_t*)&scratch[4], (uint16x8_t)vOpacity);


    for (int_fast16_t y = 0; y < iHeight; y++) {
        const uint8_t  *pAlpha = pchAlpha;
        uint16_t       *pCurTarget = pTarget;
        register unsigned blkCnt __asm("lr");
        blkCnt = iWidth;

       __asm volatile  (
            ".p2align 2                                            \n"
            /* load scheduling */
            "   vldrh.u16               q0, [%[pTarget]]           \n"
            "   vmov.i16                q7, #0x0100                \n"
            "   vldrb.u16               q1, [%[pAlpha]], #8        \n"
            /* opacity vector */
            "   vldrh.u16               q6, [%[scratch], #(4*16)]  \n"
            "   vmulh.u8                q1, q1, q6                 \n"

            "   wlstp.16                lr, %[loopCnt], 1f         \n"
            "2:                                                    \n"

            // vecAlpha
            "   vsub.i16                q2, q7, q1                 \n"

            /*  RGB565 unpack          */

            /* vecAlpha * 4 for G channel upscale */
            "   vmul.i16                q2, q2, %[four]            \n"
            /* G channel extract */
            "   vshr.u16                q5, q0, #5                 \n"

            /* load Unpacking Mask for R channel */
            "   vldrh.u16               q7, [%[scratch], #(0*16)]  \n"
            "   vand                    q4, q0, q7                 \n"

            /* load Unpacking Mask for G channel */
            "   vldrh.u16               q7, [%[scratch], #(1*16)]  \n"
            "   vand                    q5, q5, q7                 \n"
            /*  scale G vector with alpha vector */
            "   vmul.u16                q5, q5, q2                 \n"

            /* B channel */
            "   vshr.u16                q6, q0, #11                \n"

            /*  blend G vector with input G color*/
            "   vmla.u16                q5, q1, %[G]               \n"

            /* vecAlpha * 8 for R & B  upscale */
            "   vshl.i16                q2, q2, #1                 \n"

            /*  scale R vector with alpha vec */
            "   vmul.u16                q4, q4, q2                 \n"

            "   vshr.u16                q5, q5, #8                 \n"

            /*  blend R vector with input R color*/
            "   vmla.u16                q4, q1, %[R]               \n"

            /* load packing Mask for G channel */
            "   vldrh.u16               q7, [%[scratch], #(2*16)]  \n"

            /*  scale B vector with alpha vector */
            "   vmul.u16                q6, q6, q2                 \n"
            "   vand                    q5, q5, q7                 \n"

            /*  blend B vector with input B color*/
            "   vmla.u16                q6, q1, %[B]               \n"

            /* load packing Mask for B channel */
            "   vldrh.u16               q7, [%[scratch], #(3*16)]  \n"

            "   vshr.u16                q6, q6, #8                 \n"

           /* RGB 565 pack */

            /* (G & 0x00fc), 8) */
            "   vmul.i16                q5, q5, %[eight]           \n"

            /* (B & 0x00f8) */
            "   vand                    q6, q6, q7                 \n"

            /* load next alpha vector */
            "   vldrb.u16               q1, [%[pAlpha]], #8        \n"
            "   vmov.i16                q7, #0x0100                \n"

            /* pack G & B */
            "   vmla.u16                q5, q6, %[twofiftysix]     \n"
            /* reload opacity and scale alpha */
            "   vldrh.u16               q6, [%[scratch], #(4*16)]  \n"
            "   vmulh.u8                q1, q1, q6                 \n"

            /* combined (R >> 8) >> 3 */
            "   vshr.u16                q4, q4, #11                \n"

            /* load next target */
            "   vldrh.u16               q0, [%[pTarget], #16]      \n"

            /*  pack R */
            "   vorr                    q4, q4, q5                 \n"
            "   vstrh.16                q4, [%[pTarget]], #16      \n"
            "   letp                    lr, 2b                     \n"
            "1:                                                    \n"
            :[pTarget]"+r"(pCurTarget),[pAlpha] "+r"(pAlpha),[loopCnt] "+r"(blkCnt)
            :[Colour] "r"(Colour), [eight] "r" (8), [four] "r" (4),
            [R] "r" (tSrcPix.R), [G] "r" (tSrcPix.G), [B] "r" (tSrcPix.B),
            [twofiftysix] "r" (256), [scratch] "r" (scratch)
            :"q0", "q1", "q2", "q3", "q4", "q5", "q6", "q7", "memory");

        pchAlpha += (iAlphaStride);
        pTarget += (iTargetStride);
    }
#endif
}


__OVERRIDE_WEAK
void __arm_2d_impl_rgb565_colour_filling_channel_mask(uint16_t * __RESTRICT pTarget,
                                                      int16_t iTargetStride,
                                                      uint32_t * __RESTRICT pwAlpha,
                                                      int16_t iAlphaStride,
                                                      arm_2d_size_t * __RESTRICT ptCopySize,
                                                      uint16_t Colour)
{
    int_fast16_t    iHeight = ptCopySize->iHeight;
    int_fast16_t    iWidth = ptCopySize->iWidth;
    uint16x8_t      vStride4Offs = vidupq_n_u16(0, 4);
    __arm_2d_color_fast_rgb_t tSrcPix;

    __arm_2d_rgb565_unpack(*(&Colour), &tSrcPix);

#ifdef USE_MVE_INTRINSICS
    RGB565_COLOUR_FILLING_MASK_MVE(RGB565_TRGT_LOAD_STRIDE, vStride4Offs,
                                            RGB565_SCAL_OPACITY_NONE, _, pwAlpha, 4);
#else
    /* RGB565 pack/unpack Masks  */
    /* use memory rather than vmov to optimize Helium operations interleaving */
    uint16x8_t scratch[4];

    // Unpacking Mask Red
    vst1q((uint16_t*)&scratch[0], vdupq_n_u16(0x1f));
    // Unpacking Mask Green
    vst1q((uint16_t*)&scratch[1], vdupq_n_u16(0x3f));
    // packing Mask Green
    vst1q((uint16_t*)&scratch[2], vdupq_n_u16(0xfc));
    // packing Mask Blue
    vst1q((uint16_t*)&scratch[3], vdupq_n_u16(0xf8));

    for (int_fast16_t y = 0; y < iHeight; y++) {
        const uint32_t  *pAlpha = pwAlpha;
        uint16_t       *pCurTarget = pTarget;
        register unsigned blkCnt __asm("lr");
        blkCnt = iWidth;

       __asm volatile  (
            ".p2align 2                                            \n"
            /* load scheduling */
            "   vldrh.u16               q0, [%[pTarget]]           \n"

            "   vmov.i16                q7, #0x0100                \n"
            "   vldrb.u16               q1, [%[pAlpha],%[str4Offs]]\n"

            "   wlstp.16                lr, %[loopCnt], 1f         \n"
            "2:                                                    \n"
            "   add                     %[pAlpha], %[pAlpha],#(8*4)\n"
            // vecAlpha
            "   vsub.i16                q2, q7, q1                 \n"

            /*  RGB565 unpack          */

            /* vecAlpha * 4 for G channel upscale */
            "   vmul.i16                q2, q2, %[four]            \n"
            /* G channel extract */
            "   vshr.u16                q5, q0, #5                 \n"

            /* load Unpacking Mask for R channel */
            "   vldrh.u16               q7, [%[scratch], #(0*16)]  \n"
            "   vand                    q4, q0, q7                 \n"

            /* load Unpacking Mask for G channel */
            "   vldrh.u16               q7, [%[scratch], #(1*16)]  \n"
            "   vand                    q5, q5, q7                 \n"
            /*  scale G vector with alpha vector */
            "   vmul.u16                q5, q5, q2                 \n"

            /* B channel */
            "   vshr.u16                q6, q0, #11                \n"

            /*  blend G vector with input G color*/
            "   vmla.u16                q5, q1, %[G]               \n"

            /* vecAlpha * 8 for R & B  upscale */
            "   vshl.i16                q2, q2, #1                 \n"

            /*  scale R vector with alpha vec */
            "   vmul.u16                q4, q4, q2                 \n"

            "   vshr.u16                q5, q5, #8                 \n"

            /*  blend R vector with input R color*/
            "   vmla.u16                q4, q1, %[R]               \n"

            /* load packing Mask for G channel */
            "   vldrh.u16               q7, [%[scratch], #(2*16)]  \n"

            /*  scale B vector with alpha vector */
            "   vmul.u16                q6, q6, q2                 \n"
            "   vand                    q5, q5, q7                 \n"

            /*  blend B vector with input B color*/
            "   vmla.u16                q6, q1, %[B]               \n"

            /* load packing Mask for B channel */
            "   vldrh.u16               q7, [%[scratch], #(3*16)]  \n"

            "   vshr.u16                q6, q6, #8                 \n"

           /* RGB 565 pack */

            /* (G & 0x00fc), 8) */
            "   vmul.i16                q5, q5, %[eight]           \n"

            /* (B & 0x00f8) */
            "   vand                    q6, q6, q7                 \n"

            /* load next alpha vector */
            "   vldrb.u16               q1, [%[pAlpha],%[str4Offs]]\n"

            "   vmov.i16                q7, #0x0100                \n"

            /* pack G & B */
            "   vmla.u16                q5, q6, %[twofiftysix]     \n"
            /* combined (R >> 8) >> 3 */
            "   vshr.u16                q4, q4, #11                \n"

            /* load next target */
            "   vldrh.u16               q0, [%[pTarget], #16]      \n"

            /*  pack R */
            "   vorr                    q4, q4, q5                 \n"
            "   vstrh.16                q4, [%[pTarget]], #16      \n"
            "   letp                    lr, 2b                     \n"
            "1:                                                    \n"
            :[pTarget]"+r"(pCurTarget),[pAlpha] "+r"(pAlpha),[loopCnt] "+r"(blkCnt)
            :[Colour] "r"(Colour), [eight] "r" (8), [four] "r" (4),
            [R] "r" (tSrcPix.R), [G] "r" (tSrcPix.G), [B] "r" (tSrcPix.B),
            [twofiftysix] "r" (256), [scratch] "r" (scratch), [str4Offs] "t"(vStride4Offs)
            :"q0", "q1", "q2", "q4", "q5", "q6", "q7", "memory");

        pwAlpha += (iAlphaStride);
        pTarget += (iTargetStride);
    }
#endif
}


__OVERRIDE_WEAK
void __arm_2d_impl_rgb565_colour_filling_channel_mask_opacity(uint16_t * __RESTRICT pTarget,
                                                              int16_t iTargetStride,
                                                              uint32_t * __RESTRICT pwAlpha,
                                                              int16_t iAlphaStride,
                                                              arm_2d_size_t * __RESTRICT ptCopySize,
                                                              uint16_t Colour, uint8_t chOpacity)
{
    int_fast16_t    iHeight = ptCopySize->iHeight;
    int_fast16_t    iWidth = ptCopySize->iWidth;
    uint16x8_t      vStride4Offs = vidupq_n_u16(0, 4);
    uint8x16_t      vOpacity = vdupq_n_u8(chOpacity);
    __arm_2d_color_fast_rgb_t tSrcPix;

    __arm_2d_rgb565_unpack(*(&Colour), &tSrcPix);

#ifdef USE_MVE_INTRINSICS

    RGB565_COLOUR_FILLING_MASK_MVE(RGB565_TRGT_LOAD_STRIDE, vStride4Offs,
                                            RGB565_SCAL_OPACITY, vOpacity, pwAlpha, 4);
#else
    /* RGB565 pack/unpack Masks + opacity */
    /* use memory rather than vmov to optimize Helium operations interleaving */
    uint16x8_t scratch[5];

    // Unpacking Mask Red
    vst1q((uint16_t*)&scratch[0], vdupq_n_u16(0x1f));
    // Unpacking Mask Green
    vst1q((uint16_t*)&scratch[1], vdupq_n_u16(0x3f));
    // packing Mask Green
    vst1q((uint16_t*)&scratch[2], vdupq_n_u16(0xfc));
    // packing Mask Blue
    vst1q((uint16_t*)&scratch[3], vdupq_n_u16(0xf8));
    // opacity
    vst1q((uint16_t*)&scratch[4], (uint16x8_t)vOpacity);

    for (int_fast16_t y = 0; y < iHeight; y++) {
        const uint32_t  *pAlpha = pwAlpha;
        uint16_t       *pCurTarget = pTarget;
        register unsigned blkCnt __asm("lr");
        blkCnt = iWidth;

       __asm volatile  (
            ".p2align 2                                            \n"
            /* load scheduling */
            "   vldrh.u16               q0, [%[pTarget]]           \n"

            "   vmov.i16                q7, #0x0100                \n"
            "   vldrb.u16               q1, [%[pAlpha],%[str4Offs]]\n"
            /* opacity vector */
            "   vldrh.u16               q6, [%[scratch], #(4*16)]  \n"
            "   vmulh.u8                q1, q1, q6                 \n"

            "   wlstp.16                lr, %[loopCnt], 1f         \n"
            "2:                                                    \n"
            "   add                     %[pAlpha], %[pAlpha],#(8*4)\n"
            // vecAlpha
            "   vsub.i16                q2, q7, q1                 \n"

            /*  RGB565 unpack          */

            /* vecAlpha * 4 for G channel upscale */
            "   vmul.i16                q2, q2, %[four]            \n"
            /* G channel extract */
            "   vshr.u16                q5, q0, #5                 \n"

            /* load Unpacking Mask for R channel */
            "   vldrh.u16               q7, [%[scratch], #(0*16)]  \n"
            "   vand                    q4, q0, q7                 \n"

            /* load Unpacking Mask for G channel */
            "   vldrh.u16               q7, [%[scratch], #(1*16)]  \n"
            "   vand                    q5, q5, q7                 \n"
            /*  scale G vector with alpha vector */
            "   vmul.u16                q5, q5, q2                 \n"

            /* B channel */
            "   vshr.u16                q6, q0, #11                \n"

            /*  blend G vector with input G color*/
            "   vmla.u16                q5, q1, %[G]               \n"

            /* vecAlpha * 8 for R & B  upscale */
            "   vshl.i16                q2, q2, #1                 \n"

            /*  scale R vector with alpha vec */
            "   vmul.u16                q4, q4, q2                 \n"

            "   vshr.u16                q5, q5, #8                 \n"

            /*  blend R vector with input R color*/
            "   vmla.u16                q4, q1, %[R]               \n"

            /* load packing Mask for G channel */
            "   vldrh.u16               q7, [%[scratch], #(2*16)]  \n"

            /*  scale B vector with alpha vector */
            "   vmul.u16                q6, q6, q2                 \n"
            "   vand                    q5, q5, q7                 \n"

            /*  blend B vector with input B color*/
            "   vmla.u16                q6, q1, %[B]               \n"

            /* load packing Mask for B channel */
            "   vldrh.u16               q7, [%[scratch], #(3*16)]  \n"

            "   vshr.u16                q6, q6, #8                 \n"

           /* RGB 565 pack */

            /* (G & 0x00fc), 8) */
            "   vmul.i16                q5, q5, %[eight]           \n"

            /* (B & 0x00f8) */
            "   vand                    q6, q6, q7                 \n"

            /* load next alpha vector */
            "   vldrb.u16               q1, [%[pAlpha],%[str4Offs]]\n"

            "   vmov.i16                q7, #0x0100                \n"

            /* pack G & B */
            "   vmla.u16                q5, q6, %[twofiftysix]     \n"
            /* combined (R >> 8) >> 3 */
            "   vldrh.u16               q6, [%[scratch], #(4*16)]  \n"
            "   vmulh.u8                q1, q1, q6                 \n"
            "   vshr.u16                q4, q4, #11                \n"

            /* load next target */
            "   vldrh.u16               q0, [%[pTarget], #16]      \n"

            /*  pack R */
            "   vorr                    q4, q4, q5                 \n"
            "   vstrh.16                q4, [%[pTarget]], #16      \n"
            "   letp                    lr, 2b                     \n"
            "1:                                                    \n"
            :[pTarget]"+r"(pCurTarget),[pAlpha] "+r"(pAlpha),[loopCnt] "+r"(blkCnt)
            :[Colour] "r"(Colour), [eight] "r" (8), [four] "r" (4),
            [R] "r" (tSrcPix.R), [G] "r" (tSrcPix.G), [B] "r" (tSrcPix.B),
            [twofiftysix] "r" (256), [scratch] "r" (scratch), [str4Offs] "t"(vStride4Offs)
            :"q0", "q1", "q2", "q4", "q5", "q6", "q7", "memory");

        pwAlpha += (iAlphaStride);
        pTarget += (iTargetStride);
    }
#endif
}



__OVERRIDE_WEAK
void __arm_2d_impl_cccn888_colour_filling_mask(uint32_t * __RESTRICT pTarget,
                                                     int16_t iTargetStride,
                                                     uint8_t * __RESTRICT pchAlpha,
                                                     int16_t iAlphaStride,
                                                     arm_2d_size_t * __RESTRICT ptCopySize,
                                                     uint32_t Colour)
{
    int_fast16_t    iHeight = ptCopySize->iHeight;
    int_fast16_t    iWidth = ptCopySize->iWidth;
    uint16x8_t      v256 = vdupq_n_u16(256);
    uint16x8_t      vStride4Offs = vidupq_n_u16(0, 4);
    uint16_t        c0, c1, c2;

    c0 = Colour & 0xff;
    c1 = (Colour >> 8) & 0xff;
    c2 = (Colour >> 16) & 0xff;

    for (int_fast16_t y = 0; y < iHeight; y++) {
        const uint8_t * pAlpha = pchAlpha;
        uint8_t *       pTargetCh0 = (uint8_t*)pTarget;
        uint8_t *       pTargetCh1 = pTargetCh0 + 1;
        uint8_t *       pTargetCh2 = pTargetCh0 + 2;

#ifdef USE_MVE_INTRINSICS

        CCCN888_COLOUR_FILLING_MASK_INNER_MVE(CCCN888_TRGT_LOAD, _,
                                        CCCN888_SCAL_OPACITY_NONE, _, 1);
#else

        register unsigned blkCnt __asm("lr");
        blkCnt = iWidth;

    __asm volatile(
            ".p2align 2                                                   \n"
            /* expand chan0 */
            "   vldrb.u16               q0, [%[pTargetCh0], %[str4Offs]]  \n"
            "   vldrb.u16               q1, [%[pAlpha]], #8               \n"

            "   wlstp.16                lr, %[loopCnt], 1f                \n"
            "2:                                                           \n"

            "   vsub.i16                q2, %[vec256], q1                 \n"

            /*  scale ch0 vector with alpha vector */
            "   vmul.u16                q3, q0, q2                        \n"

            /* expand chan1 */
            "   vldrb.u16               q0, [%[pTargetCh1], %[str4Offs]]  \n"
            /*  blend ch0 vector with input ch0 color*/
            "   vmla.u16                q3, q1, %[c0]                     \n"
            "   vshr.u16                q3, q3, #8                        \n"

            "   vstrb.u16               q3, [%[pTargetCh0], %[str4Offs]]  \n"

            /*  scale ch1 vector with alpha vector */
            "   vmul.u16                q3, q0, q2                        \n"

            /* expand chan2 */
            "   vldrb.u16               q0, [%[pTargetCh2], %[str4Offs]]  \n"
            /*  blend ch1 vector with input ch1 color*/
            "   vmla.u16                q3, q1, %[c1]                     \n"
            "   vshr.u16                q3, q3, #8                        \n"
            "   vstrb.u16               q3, [%[pTargetCh1], %[str4Offs]]  \n"

            "   adds                    %[pTargetCh0], #32                \n"
            "   adds                    %[pTargetCh1], #32                \n"

            /*  scale ch2 vector with alpha vector */
            "   vmul.u16                q3, q0, q2                        \n"
            "   vldrb.u16               q0, [%[pTargetCh0], %[str4Offs]]  \n"

            /*  blend ch2 vector with input ch2 color*/
            "   vmla.u16                q3, q1, %[c2]                     \n"
            "   vldrb.u16               q1, [%[pAlpha]], #8               \n"

            "   vshr.u16                q3, q3, #8                        \n"
            "   vstrb.u16               q3, [%[pTargetCh2], %[str4Offs]]  \n"

            "   add.w                   %[pTargetCh2], %[pTargetCh2], #32 \n"

            "   letp                    lr, 2b                            \n"
            "1:                                                           \n"

            :[pTargetCh0] "+r"(pTargetCh0),  [pTargetCh1] "+r"(pTargetCh1),
             [pTargetCh2] "+r"(pTargetCh2), [pAlpha] "+r" (pAlpha), [loopCnt] "+r"(blkCnt)
            :[vec256] "t" (v256),[str4Offs] "t" (vStride4Offs),
             [c0] "r"(c0), [c1] "r"(c1), [c2] "r"(c2)
            :"q0", "q1", "q2", "q3", "memory");

#endif
        pchAlpha += (iAlphaStride);
        pTarget += (iTargetStride);
    }
}



__OVERRIDE_WEAK
void __arm_2d_impl_cccn888_colour_filling_mask_opacity(uint32_t * __RESTRICT pTarget,
                                                             int16_t iTargetStride,
                                                             uint8_t * __RESTRICT pchAlpha,
                                                             int16_t iAlphaStride,
                                                             arm_2d_size_t * __RESTRICT ptCopySize,
                                                             uint32_t Colour, uint8_t chOpacity)
{
    int_fast16_t    iHeight = ptCopySize->iHeight;
    int_fast16_t    iWidth = ptCopySize->iWidth;
    uint16x8_t      v256 = vdupq_n_u16(256);
    uint16x8_t      vStride4Offs = vidupq_n_u16(0, 4);
    uint8x16_t      vOpacity = vdupq_n_u8(chOpacity);
    uint16_t        c0, c1, c2;

    c0 = Colour & 0xff;
    c1 = (Colour >> 8) & 0xff;
    c2 = (Colour >> 16) & 0xff;

    for (int_fast16_t y = 0; y < iHeight; y++) {
        const uint8_t * pAlpha = pchAlpha;
        uint8_t *       pTargetCh0 = (uint8_t*)pTarget;
        uint8_t *       pTargetCh1 = pTargetCh0 + 1;
        uint8_t *       pTargetCh2 = pTargetCh0 + 2;

#ifdef USE_MVE_INTRINSICS

        CCCN888_COLOUR_FILLING_MASK_INNER_MVE(CCCN888_TRGT_LOAD, _,
                                        CCCN888_SCAL_OPACITY, vOpacity, 1);
#else

        register unsigned blkCnt __asm("lr");
        blkCnt = iWidth;

    __asm volatile(
            ".p2align 2                                                   \n"
            /* expand chan0 */
            "   vldrb.u16               q0, [%[pTargetCh0], %[str4Offs]]  \n"
            "   vldrb.u16               q1, [%[pAlpha]], #8               \n"
            "   vmulh.u8                q1, q1, %[vOpacity]               \n"

            "   wlstp.16                lr, %[loopCnt], 1f                \n"
            "2:                                                           \n"

            "   vsub.i16                q2, %[vec256], q1                 \n"

            /*  scale ch0 vector with alpha vector */
            "   vmul.u16                q3, q0, q2                        \n"

            /* expand chan1 */
            "   vldrb.u16               q0, [%[pTargetCh1], %[str4Offs]]  \n"
            /*  blend ch0 vector with input ch0 color*/
            "   vmla.u16                q3, q1, %[c0]                     \n"
            "   vshr.u16                q3, q3, #8                        \n"

            "   vstrb.u16               q3, [%[pTargetCh0], %[str4Offs]]  \n"

            /*  scale ch1 vector with alpha vector */
            "   vmul.u16                q3, q0, q2                        \n"

            /* expand chan2 */
            "   vldrb.u16               q0, [%[pTargetCh2], %[str4Offs]]  \n"
            /*  blend ch1 vector with input ch1 color*/
            "   vmla.u16                q3, q1, %[c1]                     \n"
            "   vshr.u16                q3, q3, #8                        \n"
            "   vstrb.u16               q3, [%[pTargetCh1], %[str4Offs]]  \n"

            "   adds                    %[pTargetCh0], #32                \n"
            "   adds                    %[pTargetCh1], #32                \n"

            /*  scale ch2 vector with alpha vector */
            "   vmul.u16                q3, q0, q2                        \n"
            "   vldrb.u16               q0, [%[pTargetCh0], %[str4Offs]]  \n"

            /*  blend ch2 vector with input ch2 color*/
            "   vmla.u16                q3, q1, %[c2]                     \n"
            "   vldrb.u16               q1, [%[pAlpha]], #8               \n"
            "   vmulh.u8                q1, q1, %[vOpacity]               \n"

            "   vshr.u16                q3, q3, #8                        \n"
            "   vstrb.u16               q3, [%[pTargetCh2], %[str4Offs]]  \n"

            "   add.w                   %[pTargetCh2], %[pTargetCh2], #32 \n"

            "   letp                    lr, 2b                            \n"
            "1:                                                           \n"

            :[pTargetCh0] "+r"(pTargetCh0),  [pTargetCh1] "+r"(pTargetCh1),
             [pTargetCh2] "+r"(pTargetCh2), [pAlpha] "+r" (pAlpha), [loopCnt] "+r"(blkCnt)
            :[vec256] "t" (v256),[str4Offs] "t" (vStride4Offs),
             [vOpacity] "t"(vOpacity),
             [c0] "r"(c0), [c1] "r"(c1), [c2] "r"(c2)
            :"q0", "q1", "q2", "q3", "memory");

#endif
        pchAlpha += (iAlphaStride);
        pTarget += (iTargetStride);
    }
}


__OVERRIDE_WEAK
void __arm_2d_impl_cccn888_colour_filling_channel_mask(uint32_t * __RESTRICT pTarget,
                                                       int16_t iTargetStride,
                                                       uint32_t * __RESTRICT pwAlpha,
                                                       int16_t iAlphaStride,
                                                       arm_2d_size_t * __RESTRICT ptCopySize,
                                                       uint32_t Colour)
{
    int_fast16_t    iHeight = ptCopySize->iHeight;
    int_fast16_t    iWidth = ptCopySize->iWidth;
    uint16x8_t      v256 = vdupq_n_u16(256);
    uint16x8_t      vStride4Offs = vidupq_n_u16(0, 4);
    uint16_t        c0, c1, c2;

    c0 = Colour & 0xff;
    c1 = (Colour >> 8) & 0xff;
    c2 = (Colour >> 16) & 0xff;

    for (int_fast16_t y = 0; y < iHeight; y++) {
        const uint8_t *pAlpha = (const uint8_t *)pwAlpha;
        uint8_t *       pTargetCh0 = (uint8_t*)pTarget;
        uint8_t *       pTargetCh1 = pTargetCh0 + 1;
        uint8_t *       pTargetCh2 = pTargetCh0 + 2;

#ifdef USE_MVE_INTRINSICS

        CCCN888_COLOUR_FILLING_MASK_INNER_MVE(CCCN888_TRGT_LOAD_STRIDE, vStride4Offs,
                                        CCCN888_SCAL_OPACITY_NONE, _, 4);
#else

        register unsigned blkCnt __asm("lr");
        blkCnt = iWidth;

    __asm volatile(
            ".p2align 2                                                   \n"
            /* expand chan0 */
            "   vldrb.u16               q0, [%[pTargetCh0], %[str4Offs]]  \n"
            "   vldrb.u16               q1, [%[pAlpha], %[str4Offs]]      \n"

            "   wlstp.16                lr, %[loopCnt], 1f                \n"
            "2:                                                           \n"

            "   vsub.i16                q2, %[vec256], q1                 \n"

            /*  scale ch0 vector with alpha vector */
            "   vmul.u16                q3, q0, q2                        \n"

            /* expand chan1 */
            "   vldrb.u16               q0, [%[pTargetCh1], %[str4Offs]]  \n"
            /*  blend ch0 vector with input ch0 color*/
            "   vmla.u16                q3, q1, %[c0]                     \n"
            "   vshr.u16                q3, q3, #8                        \n"

            "   vstrb.u16               q3, [%[pTargetCh0], %[str4Offs]]  \n"

            /*  scale ch1 vector with alpha vector */
            "   vmul.u16                q3, q0, q2                        \n"

            /* expand chan2 */
            "   vldrb.u16               q0, [%[pTargetCh2], %[str4Offs]]  \n"
            /*  blend ch1 vector with input ch1 color*/
            "   vmla.u16                q3, q1, %[c1]                     \n"
            "   vshr.u16                q3, q3, #8                        \n"
            "   vstrb.u16               q3, [%[pTargetCh1], %[str4Offs]]  \n"

            "   adds                    %[pAlpha], #32                    \n"
            "   adds                    %[pTargetCh0], #32                \n"


            /*  scale ch2 vector with alpha vector */
            "   vmul.u16                q3, q0, q2                        \n"
            "   vldrb.u16               q0, [%[pTargetCh0], %[str4Offs]]  \n"

            /*  blend ch2 vector with input ch2 color*/
            "   vmla.u16                q3, q1, %[c2]                     \n"
            "   vldrb.u16               q1, [%[pAlpha], %[str4Offs]]      \n"

            "   vshr.u16                q3, q3, #8                        \n"
            "   vstrb.u16               q3, [%[pTargetCh2], %[str4Offs]]  \n"

            "   adds                    %[pTargetCh1], #32                \n"
            "   adds                    %[pTargetCh2], #32                \n"

            "   letp                    lr, 2b                            \n"
            "1:                                                           \n"

            :[pTargetCh0] "+r"(pTargetCh0),  [pTargetCh1] "+r"(pTargetCh1),
             [pTargetCh2] "+r"(pTargetCh2), [pAlpha] "+r" (pAlpha), [loopCnt] "+r"(blkCnt)
            :[vec256] "t" (v256),[str4Offs] "t" (vStride4Offs),
             [c0] "r"(c0), [c1] "r"(c1), [c2] "r"(c2)
            :"q0", "q1", "q2", "q3", "memory");

#endif
        pwAlpha += (iAlphaStride);
        pTarget += (iTargetStride);
    }
}



__OVERRIDE_WEAK
void __arm_2d_impl_cccn888_colour_filling_channel_mask_opacity(uint32_t * __RESTRICT pTarget,
                                                               int16_t iTargetStride,
                                                               uint32_t * __RESTRICT pwAlpha,
                                                               int16_t iAlphaStride,
                                                               arm_2d_size_t *
                                                               __RESTRICT ptCopySize,
                                                               uint32_t Colour, uint8_t chOpacity)
{
    int_fast16_t    iHeight = ptCopySize->iHeight;
    int_fast16_t    iWidth = ptCopySize->iWidth;
    uint16x8_t      v256 = vdupq_n_u16(256);
    uint16x8_t      vStride4Offs = vidupq_n_u16(0, 4);
    uint8x16_t      vOpacity = vdupq_n_u8(chOpacity);
    uint16_t        c0, c1, c2;

    c0 = Colour & 0xff;
    c1 = (Colour >> 8) & 0xff;
    c2 = (Colour >> 16) & 0xff;

    for (int_fast16_t y = 0; y < iHeight; y++) {
        const uint8_t  *pAlpha = (const uint8_t *)pwAlpha;
        uint8_t *       pTargetCh0 = (uint8_t*)pTarget;
        uint8_t *       pTargetCh1 = pTargetCh0 + 1;
        uint8_t *       pTargetCh2 = pTargetCh0 + 2;

#ifdef USE_MVE_INTRINSICS

        CCCN888_COLOUR_FILLING_MASK_INNER_MVE(CCCN888_TRGT_LOAD_STRIDE, vStride4Offs,
                                        CCCN888_SCAL_OPACITY, vOpacity, 4);
#else

        register unsigned blkCnt __asm("lr");
        blkCnt = iWidth;

    __asm volatile(
            ".p2align 2                                                   \n"
            /* expand chan0 */
            "   vldrb.u16               q0, [%[pTargetCh0], %[str4Offs]]  \n"
            "   vldrb.u16               q1, [%[pAlpha], %[str4Offs]]      \n"
            "   vmulh.u8                q1, q1, %[vOpacity]               \n"

            "   wlstp.16                lr, %[loopCnt], 1f                \n"
            "2:                                                           \n"

            "   vsub.i16                q2, %[vec256], q1                 \n"

            /*  scale ch0 vector with alpha vector */
            "   vmul.u16                q3, q0, q2                        \n"

            /* expand chan1 */
            "   vldrb.u16               q0, [%[pTargetCh1], %[str4Offs]]  \n"
            /*  blend ch0 vector with input ch0 color*/
            "   vmla.u16                q3, q1, %[c0]                     \n"
            "   vshr.u16                q3, q3, #8                        \n"

            "   vstrb.u16               q3, [%[pTargetCh0], %[str4Offs]]  \n"

            /*  scale ch1 vector with alpha vector */
            "   vmul.u16                q3, q0, q2                        \n"

            /* expand chan2 */
            "   vldrb.u16               q0, [%[pTargetCh2], %[str4Offs]]  \n"
            /*  blend ch1 vector with input ch1 color*/
            "   vmla.u16                q3, q1, %[c1]                     \n"
            "   vshr.u16                q3, q3, #8                        \n"
            "   vstrb.u16               q3, [%[pTargetCh1], %[str4Offs]]  \n"

            "   adds                    %[pAlpha], #32                    \n"
            "   adds                    %[pTargetCh0], #32                \n"


            /*  scale ch2 vector with alpha vector */
            "   vmul.u16                q3, q0, q2                        \n"
            "   vldrb.u16               q0, [%[pTargetCh0], %[str4Offs]]  \n"

            /*  blend ch2 vector with input ch2 color*/
            "   vmla.u16                q3, q1, %[c2]                     \n"
            "   vldrb.u16               q1, [%[pAlpha], %[str4Offs]]      \n"
            "   vmulh.u8                q1, q1, %[vOpacity]               \n"

            "   vshr.u16                q3, q3, #8                        \n"
            "   vstrb.u16               q3, [%[pTargetCh2], %[str4Offs]]  \n"

            "   adds                    %[pTargetCh1], #32                \n"
            "   adds                    %[pTargetCh2], #32                \n"

            "   letp                    lr, 2b                            \n"
            "1:                                                           \n"

            :[pTargetCh0] "+r"(pTargetCh0),  [pTargetCh1] "+r"(pTargetCh1),
             [pTargetCh2] "+r"(pTargetCh2), [pAlpha] "+r" (pAlpha), [loopCnt] "+r"(blkCnt)
            :[vec256] "t" (v256),[str4Offs] "t" (vStride4Offs), [vOpacity] "t"(vOpacity),
             [c0] "r"(c0), [c1] "r"(c1), [c2] "r"(c2)
            :"q0", "q1", "q2", "q3", "memory");

#endif
        pwAlpha += (iAlphaStride);
        pTarget += (iTargetStride);
    }
}

#ifdef EXPERIMENTAL


/*----------------------------------------------------------------------------*
 * Misc & Experimental                                                        *
 *----------------------------------------------------------------------------*/
#if __ARM_2D_HAS_HELIUM_FLOAT__


int16_t __arm_2d_bilinear_interp_rgb16_f16(
           const uint16_t   *phwSourceBase,
           int16_t           iSourceStride,
           arm_2d_size_t *ptSourceSize,
           float16_t         X,
           float16_t         Y)
{
    float16_t       out;
    int16_t         xIndex, yIndex, index;
    float16_t       xdiff, ydiff;
    float16_t       b1, b2, b3, b4;
    __arm_2d_color_fast_rgb_t    packed00, packed01, packed10, packed11;

    xIndex = (int16_t) X;
    yIndex = (int16_t) Y;

#ifdef INTERP_BOUND_CHECK
    /* Care taken for table outside boundary */
    /* Returns zero output when values are outside table boundary */
    if (xIndex < 0 || xIndex > (ptSourceSize->iWidth - 2) || yIndex < 0
        || yIndex > (ptSourceSize->iHeight - 2)) {
        return (0);
    }
#endif

    /* Calculation of index for two nearest points in X-direction */
    index = (xIndex) + (yIndex) * iSourceStride;

    /* Read two nearest points in X-direction */

    __arm_2d_rgb565_unpack(phwSourceBase[index], &packed00);
    __arm_2d_rgb565_unpack(phwSourceBase[index + 1], &packed01);

    /* Calculation of index for two nearest points in Y-direction */
    index = (xIndex) + (yIndex + 1) * iSourceStride;

    /* Read two nearest points in Y-direction */
    __arm_2d_rgb565_unpack(phwSourceBase[index], &packed10);
    __arm_2d_rgb565_unpack(phwSourceBase[index + 1], &packed11);


    const uint16_t  *p800 = (uint16_t *) & packed00;
    const uint16_t  *p801 = (uint16_t *) & packed01;
    const uint16_t  *p810 = (uint16_t *) & packed10;
    const uint16_t  *p811 = (uint16_t *) & packed11;
    __arm_2d_color_fast_rgb_t  wTargetPixel;
    uint_fast8_t    n = sizeof(uint32_t) - 1; /* remove alpha */
    uint16_t        *pchDes = (uint16_t *) & wTargetPixel;

    /* interpolate individual component */
    do {
        float16_t       f00, f01, f10, f11;

        f00 = (float16_t) * p800++;
        f01 = (float16_t) * p801++;
        f10 = (float16_t) * p810++;
        f11 = (float16_t) * p811++;

        /* Calculation of intermediate values */
        b1 = f00;
        b2 = f01 - f00;
        b3 = f10 - f00;
        b4 = f00 - f01 - f10 + f11;

        /* Calculation of fractional part in X */
        xdiff = X - xIndex;

        /* Calculation of fractional part in Y */
        ydiff = Y - yIndex;

        /* Calculation of bi-linear interpolated output */
        out = b1 + b2 * xdiff + b3 * ydiff + b4 * xdiff * ydiff;

        /* convert back in byte */
        *pchDes++ = (uint16_t) out;
    } while (--n);

    return (__arm_2d_rgb565_pack(&wTargetPixel));
}


  int32_t __arm_2d_bilinear_interp_rgb32_f16(
           const uint32_t   *phwSourceBase,
           int16_t           iSourceStride,
           arm_2d_size_t *ptSourceSize,
           float16_t         X,
           float16_t         Y)
{
    float16_t       out;
    int16_t         xIndex, yIndex, index;
    float16_t       xdiff, ydiff;
    float16_t       b1, b2, b3, b4;
    uint32_t        packed00, packed01, packed10, packed11;

    xIndex = (int16_t) X;
    yIndex = (int16_t) Y;

#ifdef INTERP_BOUND_CHECK
    /* Care taken for table outside boundary */
    /* Returns zero output when values are outside table boundary */
    if (xIndex < 0 || xIndex > (ptSourceSize->iWidth - 2) || yIndex < 0
        || yIndex > (ptSourceSize->iHeight - 2)) {
        return (0);
    }
#endif

    /* Calculation of index for two nearest points in X-direction */
    index = (xIndex) + (yIndex) * iSourceStride;

    /* Read two nearest points in X-direction */
    packed00 = phwSourceBase[index];
    packed01 = phwSourceBase[index + 1];

    /* Calculation of index for two nearest points in Y-direction */
    index = (xIndex) + (yIndex + 1) * iSourceStride;

    /* Read two nearest points in Y-direction */
    packed10 = phwSourceBase[index];
    packed11 = phwSourceBase[index + 1];


    const uint8_t  *p800 = (uint8_t *) & packed00;
    const uint8_t  *p801 = (uint8_t *) & packed01;
    const uint8_t  *p810 = (uint8_t *) & packed10;
    const uint8_t  *p811 = (uint8_t *) & packed11;
    uint32_t        wTargetPixel;
    uint_fast8_t    n = sizeof(uint32_t);
    uint8_t        *pchDes = (uint8_t *) & wTargetPixel;

    /* interpolate individual component */
    do {
        float16_t       f00, f01, f10, f11;

        f00 = (float16_t) * p800++;
        f01 = (float16_t) * p801++;
        f10 = (float16_t) * p810++;
        f11 = (float16_t) * p811++;

        /* Calculation of intermediate values */
        b1 = f00;
        b2 = f01 - f00;
        b3 = f10 - f00;
        b4 = f00 - f01 - f10 + f11;

        /* Calculation of fractional part in X */
        xdiff = X - xIndex;

        /* Calculation of fractional part in Y */
        ydiff = Y - yIndex;

        /* Calculation of bi-linear interpolated output */
        out = b1 + b2 * xdiff + b3 * ydiff + b4 * xdiff * ydiff;

        /* convert back in byte */
        *pchDes++ = (uint8_t) out;
    } while (--n);

    return wTargetPixel;
}




void __arm_2d_rgb16_scale(uint16_t * phwSourceBase,
                                          int16_t iSourceStride,
                                          arm_2d_size_t * ptSourceSize,
                                          uint16_t * phwTargetBase,
                                          int16_t iTargetStride,
                                          arm_2d_size_t * ptTargetSize)
{
#if !defined(TESTING)
//! todo
#else

    /* horizontal & vertical scale factors to create interpolated from source dimension grid */
    /* limit border dimension to ensure source neighbouring points do not exceed limit */
    float16_t       scaleX =
        (float16_t) (ptSourceSize->iWidth - 2) / (float16_t) (ptTargetSize->iWidth - 1);
    float16_t       scaleY =
        (float16_t) (ptSourceSize->iHeight - 2) / (float16_t) (ptTargetSize->iHeight - 1);

    for (int32_t y = 0; y < ptTargetSize->iHeight; y++) {
        for (int32_t x = 0; x < ptTargetSize->iWidth; x++) {
            phwTargetBase[x] =
                __arm_2d_bilinear_interp_rgb16_f16(phwSourceBase,
                                                 iSourceStride,
                                                 ptSourceSize,
                                                 x * scaleX, y * scaleY);
        }
        phwTargetBase += iTargetStride;
    }
#endif
}


void __arm_2d_rgb32_scale(uint32_t * phwSourceBase,
                                          int16_t iSourceStride,
                                          arm_2d_size_t * ptSourceSize,
                                          uint32_t * phwTargetBase,
                                          int16_t iTargetStride,
                                          arm_2d_size_t * ptTargetSize)
{
#if !defined(TESTING)
//! todo
#else

    /* horizontal & vertical scale factors to create interpolated from source dimension grid */
    /* limit border dimension to ensure source neighbouring points do not exceed limit */
    float16_t       scaleX =
        (float16_t) (ptSourceSize->iWidth - 2) / (float16_t) (ptTargetSize->iWidth - 1);
    float16_t       scaleY =
        (float16_t) (ptSourceSize->iHeight - 2) / (float16_t) (ptTargetSize->iHeight - 1);

    for (int32_t y = 0; y < ptTargetSize->iHeight; y++) {
        for (int32_t x = 0; x < ptTargetSize->iWidth; x++) {
            phwTargetBase[x] =
                __arm_2d_bilinear_interp_rgb32_f16(phwSourceBase,
                                                 iSourceStride,
                                                 ptSourceSize,
                                                 x * scaleX, y * scaleY);
        }
        phwTargetBase += iTargetStride;
    }
#endif
}


#endif

/*
 * rotation trial
 */


#define PIBy180_Q30             18740330
#define Q0_TO_Q16(x)            ((x) << 16)
#define CEIL_Q16_TO_Q0(x)       ((x >> 16) + (x & 0xffff ? 1 : 0))
#define ROUND_Q16_TO_Q0(x)      ((x + (1 << 15)) >> 16)

#define PRINTDBG(x,...)

 int32_t __arm_fxpt_mult32(int32_t x, int32_t y, const int32_t scal)
{
    int32_t tmp;

    tmp = (q31_t) (((q63_t) x * y ) >> 32);
    return  tmp << scal;
}


typedef struct arm_2d_shear_rotate_info {
    int32_t tanHalfAngle;
    int32_t sinAngle;
    int32_t corner[3];
    int32_t newDim[3];
} arm_2d_shear_rotate_info;


void __arm_2d_rgb32_fill_line(      uint32_t * pTargetBase, uint32_t len, uint32_t filler)
{
    while(len > 0) {
        *pTargetBase++ = filler;
        len--;
    }
}

#ifdef MVE
void __arm_2d_rgb32_fill_col(
        uint32_t * pBase,
        uint32_t stride,
        uint32_t blockSize,
        uint32_t filler)
{

    uint32x4_t      idx = vidupq_u32((uint32_t) 0, 1);
    uint32x4_t      pattern = vdupq_n_u32(filler);
    uint32_t        incr = stride * 4;

    idx = vmulq_n_u32(idx, stride);

    do {
        mve_pred16_t    p = vctp32q(blockSize);

        vstrwq_scatter_shifted_offset_p_u32(pBase, idx, pattern, p);
        idx = vaddq(idx, incr);
        blockSize -= 4;
    }
    while ((int32_t) blockSize > 0);
}

#else

void __arm_2d_rgb32_fill_col(
        uint32_t * pTargetBase,
        uint32_t stride,
        uint32_t len,
        uint32_t filler)
{
    while(len > 0) {
        *pTargetBase = filler;
        pTargetBase += stride;
        len--;
    }
}
#endif


#ifdef MVE
void __arm_2d_rgb32_move_col(
        uint32_t * pBase,
        uint32_t stride,
        uint32_t offset,
        uint32_t blockSize)
{
    if (offset == 0)
        return;

    if (offset > 0) {
        // need to operate on the reverse direction
        // to avoid write corrupting unread samples
        uint32x4_t      idx = vddupq_u32((uint32_t) 3, 1);
        uint32_t        incr = stride * 4; /* jump 4 rows */
        uint32_t       *pDst = pBase + stride * offset;

        /* build vector with consective row index */
        idx = vmulq_n_u32(idx, stride);

        /* move index to the bottom */
        /* move from bottom in backward direction */
        idx = idx + ((blockSize - 4) * stride);

        do {
            mve_pred16_t    p = vctp32q(blockSize);
            uint32x4_t      item = vldrwq_gather_shifted_offset_z_u32(pBase, idx, p);

            vstrwq_scatter_shifted_offset_p_u32(pDst, idx, item, p);
            idx = vsubq(idx, incr);
            blockSize -= 4;
        }
        while ((int32_t) blockSize > 0);
    }
}

#else
void __arm_2d_rgb32_move_col(
        uint32_t * pTargetBase,
        uint32_t stride,
        uint32_t offset,
        uint32_t len)
{
    uint32_t * pSrc = pTargetBase + (len-1)*stride;
    uint32_t * pDst = pSrc + offset*stride;

    while(len > 0) {
        *pDst = *pSrc;
        pDst-= stride;
        pSrc-= stride;
        len--;
    }
}
#endif


void __arm_2d_rgb32_move_line(      uint32_t * pTargetBase,  uint32_t offset, uint32_t len)
{
    uint32_t * pSrc = pTargetBase + (len-1);
    uint32_t * pDst = pSrc + offset;

    while(len > 0) {
        *pDst = *pSrc;
        pDst--;
        pSrc--;
        len--;
    }
}



int32_t __tanFxQ30(int32_t in)
{
#define TANC3   98947010  /* 0.092151584 */
#define TANC2  126772778  /* 0.118066350 */
#define TANC1  359662342  /* 0.334961658 */
#define TANC0 1073741824  /* 1 */

    static const int32_t tanFxTab[4] = {TANC0, TANC1, TANC2, TANC3};

    int32_t in2 = __arm_fxpt_mult32(in,in, 2);
    int32_t acc;

    acc = __arm_fxpt_mult32(tanFxTab[3], in2, 2);
    acc = tanFxTab[2] + acc;

    acc = __arm_fxpt_mult32(acc, in2, 2);
    acc = tanFxTab[1] + acc;

    acc = __arm_fxpt_mult32(acc, in2, 2);
    acc = tanFxTab[0] + acc;

    return __arm_fxpt_mult32(acc, in, 2);
}




int32_t __sinFxQ30(int32_t in)
{
#define SINC3    -209544  /* 0.000195152832 */
#define SINC2    8946589  /* 0.00833216076 */
#define SINC1 -178956841  /* 0.166666546 */
#define SINC0 1073741824  /* 1 */

    //sin(x) approx x - 0.166666546x^3 + 0.00833216076x^5 -0.000195152832x^7

    static const int32_t sinFxTab[4] = {SINC0, SINC1, SINC2, SINC3};

    int32_t in2 = __arm_fxpt_mult32(in,in, 2);
    int32_t acc;

    acc = __arm_fxpt_mult32(sinFxTab[3], in2, 2);
    acc = sinFxTab[2] + acc;

    acc = __arm_fxpt_mult32(acc, in2, 2);
    acc = sinFxTab[1] + acc;

    acc = __arm_fxpt_mult32(acc, in2, 2);
    acc = sinFxTab[0] + acc;

    return __arm_fxpt_mult32(acc, in, 2);
}




void __arm_2d_3shear_rotate_get_target_dim_flt(
                                          arm_2d_size_t * ptSourceSize,
                                          arm_2d_size_t * ptTargetSize,
                                          int16_t angle)
{
    float32_t       angleR = (float32_t) angle / 180.0f * PI;
    float32_t       tanAngleHalf = tanf(angleR / 2);
    float32_t       sinAngle = sinf(angleR);
    float32_t       colF = (float32_t) ptSourceSize->iWidth;
    float32_t       rowF = (float32_t) ptSourceSize->iHeight;
    float32_t       centerX = (float32_t) (ptSourceSize->iWidth / 2);
    float32_t       centerY = (float32_t) (ptSourceSize->iHeight / 2);

    // X shearing effects
    float32_t       topRightX = colF - centerX - tanAngleHalf * (1.0f - centerY);
    float32_t       bottomLX = 1.0f - centerX - tanAngleHalf * (rowF - centerY);
    float32_t       newWidth = ceilf(topRightX - bottomLX + 1.0f);

    PRINTDBG(" ** iWidth %d iHeight %d ***\n", ptSourceSize->iWidth, ptSourceSize->iHeight);

    for (int i = 0; i < ptSourceSize->iHeight; i++) {
        int32_t         posYsrc = 1 + i - (int) centerY;
        int32_t         startxI;
        float32_t       startxF = 1.0f - centerX - tanAngleHalf * (float32_t) posYsrc;

        startxF = startxF - bottomLX;
        startxF = floorf(startxF + 0.5f);
        startxI = (int) startxF;
        PRINTDBG("startxI %d\n", startxI);
    }

    /* Y shearing */
    colF = newWidth;
    /* rowF source unchanged */

    /* get source image center */
    centerX = centerY;
    centerY = floorf(colF / 2.0f);

    float32_t       topLeftX = (1.0f - centerX) * sinAngle + (1.0f - centerY);
    float32_t       bottomRX = (colF - centerX) * sinAngle + (rowF - centerY);
    float32_t       newHeight = ceilf(bottomRX - topLeftX + 1.0f);

    PRINTDBG(" -- yshear %f --\n", newHeight);
    for (int i = 0; i < newWidth; i++) {
        int32_t         posXsrc = 1 + i - (int) centerX;
        int32_t         startyI;
        float32_t       startyF = 1.0f + sinAngle * (float32_t) posXsrc - centerY;

        startyF = startyF - topLeftX;
        startyF = floorf(startyF + 0.5f);
        startyI = (int) startyF;
        PRINTDBG("startyF %d\n", startyI);
    }

    /* X shearing */
    colF = newWidth;
    rowF = newHeight;

    /* get source image center */
    centerX = floorf(newWidth / 2.0f);
    centerY = floorf(newHeight / 2.0f);

    topRightX = colF - centerX - tanAngleHalf * (1.0f - centerY);
    bottomLX = 1.0f - centerX - tanAngleHalf * (rowF - centerY);
    float32_t       finalWidth = ceilf(topRightX - bottomLX + 1.0f);

    PRINTDBG(" -- xshear %f --\n", finalWidth);
    for (int i = 0; i < (int) newHeight; i++) {
        int32_t         posYsrc = 1 + i - (int) centerY;
        int32_t         startxI;
        float32_t       startxF = 1.0f - centerX - tanAngleHalf * (float32_t) posYsrc;

        startxF = startxF - bottomLX;
        startxF = floorf(startxF + 0.5f);
        startxI = (int) startxF;
        PRINTDBG("startx %d \n", startxI);
    }

    ptTargetSize->iHeight = (int) newHeight;
    ptTargetSize->iWidth = (int) finalWidth;
}


void __arm_2d_3shear_rotate_get_target_dim_fx(
                                          arm_2d_size_t * ptSourceSize,
                                          arm_2d_size_t * ptTargetSize,
                                          arm_2d_shear_rotate_info * ptShearRot,
                                          int16_t angle)
{
    int32_t         angleR = angle * PIBy180_Q30;
    int32_t         tanHalfAngle = __tanFxQ30(angleR >> 1);
    int32_t         sinAngle = __sinFxQ30(angleR);
    int32_t         srcIWidth = ptSourceSize->iWidth;
    int32_t         srcIWidthHalf = ptSourceSize->iWidth / 2;
    int32_t         srcIHeight = ptSourceSize->iHeight;
    int32_t         srcIHeightHalf = ptSourceSize->iHeight / 2;
    int32_t         Right, Left;

    PRINTDBG(" ** iWidth %d iHeight %d ***\n", ptSourceSize->iWidth, ptSourceSize->iHeight);

    Right = Q0_TO_Q16(1 - srcIHeightHalf);
    Right = __arm_fxpt_mult32(Right, tanHalfAngle, 2);
    Right = Q0_TO_Q16(srcIWidth - srcIWidthHalf) - Right;

    Left = Q0_TO_Q16(srcIHeight - srcIHeightHalf);
    Left = __arm_fxpt_mult32(Left, tanHalfAngle, 2);
    Left = Q0_TO_Q16(1 - srcIWidthHalf) - Left;

    int32_t         newWidth = Right - Left + Q0_TO_Q16(1);
    newWidth = CEIL_Q16_TO_Q0(newWidth);

    ptShearRot->tanHalfAngle = tanHalfAngle;
    ptShearRot->sinAngle = sinAngle;

    ptShearRot->corner[0] = Left;
    ptShearRot->newDim[0] = newWidth;

#if DBG
    printf("--newWidth %d \n", newWidth);
    for (int i = 0; i < ptSourceSize->iHeight; i++) {
        int32_t         posYsrc = 1 + i - srcIHeightHalf;
        int32_t         startxF =
            Q0_TO_Q16(1 - srcIWidthHalf) -
            __arm_fxpt_mult32(tanHalfAngle, Q0_TO_Q16(posYsrc), 2);

        startxF = startxF - Left;
        startxF = ROUND_Q16_TO_Q0(startxF);
        printf("startxI %d\n", startxF);
    }
#endif

    /* Y shearing */
    Left = Q0_TO_Q16(1 - srcIHeightHalf);
    Left = __arm_fxpt_mult32(Left, sinAngle, 2);
    Left = Left + Q0_TO_Q16(1 - newWidth / 2);

    Right = Q0_TO_Q16(newWidth - srcIHeightHalf);
    Right = __arm_fxpt_mult32(Right, sinAngle, 2);
    Right = Right + Q0_TO_Q16(srcIHeight - newWidth / 2);

    int32_t         newHeight = Right - Left + Q0_TO_Q16(1);
    newHeight = CEIL_Q16_TO_Q0(newHeight);

    ptShearRot->corner[1] = Left;
    ptShearRot->newDim[1] = newHeight;

#if DBG
    printf("--newHeight %d \n", newHeight);
    for (int i = 0; i < newWidth; i++) {
        int32_t         posXsrc = 1 + i - srcIHeightHalf;
        int32_t         startyF =
            Q0_TO_Q16(1 - newWidth / 2) +
            __arm_fxpt_mult32(sinAngle, Q0_TO_Q16(posXsrc), 2);

        startyF = startyF - Left;
        startyF = ROUND_Q16_TO_Q0(startyF);
        printf(" startxI %d\n", startyF);
    }
#endif

    /* X shearing */
    int32_t         newHeightHalf = newHeight / 2;
    int32_t         newWidthHalf = newWidth / 2;

    Right = Q0_TO_Q16(1 - newHeightHalf);
    Right = __arm_fxpt_mult32(Right, tanHalfAngle, 2);
    Right = Q0_TO_Q16(newWidth - newWidthHalf) - Right;

    Left = Q0_TO_Q16(newHeight - newHeightHalf);
    Left = __arm_fxpt_mult32(Left, tanHalfAngle, 2);
    Left = Q0_TO_Q16(1 - newWidthHalf) - Left;

    int32_t         finalWidth = Right - Left + Q0_TO_Q16(1);
    finalWidth = CEIL_Q16_TO_Q0(finalWidth);

    ptShearRot->corner[2] = Left;
    ptShearRot->newDim[2] = finalWidth;


    ptTargetSize->iHeight = newHeight;
    ptTargetSize->iWidth = finalWidth;
}




void __arm_2d_rgb32_rotate_fx(uint32_t * phwSourceBase,
                                           int16_t iSourceStride,
                                           arm_2d_size_t * ptSourceSize,
                                           uint32_t * phwTargetBase,
                                           int16_t iTargetStride,
                                           arm_2d_size_t * ptTargetSize,
                                           arm_2d_shear_rotate_info * ptShearRot,
                                           uint32_t        filler)
{
    int32_t         srcIWidthHalf = ptSourceSize->iWidth / 2;
    int32_t         srcIHeightHalf = ptSourceSize->iHeight / 2;
    int32_t         tanHalfAngle = ptShearRot->tanHalfAngle;
    int32_t         sinAngle = ptShearRot->sinAngle;
    int32_t         corner;
    uint32_t       *pTarget;

    // x shearing
    PRINTDBG("X shear \n");

    int8_t base[16*4];
    int8_t kernel[16*4];
    for(int i=0;i<64;i++) {
        base[i]=i;
        kernel[i]=64+i;
        }


    corner = ptShearRot->corner[0];
    pTarget = phwTargetBase;
    for (int i = 0; i < ptSourceSize->iHeight; i++) {
        uint32_t       *pDst;
        int32_t         posYsrc = 1 + i - srcIHeightHalf;
        int32_t         start =
            Q0_TO_Q16(1 - srcIWidthHalf) -
            __arm_fxpt_mult32(tanHalfAngle, Q0_TO_Q16(posYsrc), 2);

        start = start - corner;
        start = ROUND_Q16_TO_Q0(start);
        PRINTDBG("startxI %d\n", start);

        int32_t         residual = ptTargetSize->iWidth - start - ptSourceSize->iWidth;

        pDst = pTarget;
        __arm_2d_rgb32_fill_line(pDst, start, filler);


        pDst += start;
        memcpy(pDst, phwSourceBase, ptSourceSize->iWidth * sizeof(uint32_t));


        pDst += ptSourceSize->iWidth;
        __arm_2d_rgb32_fill_line(pDst, residual, filler);

        pTarget += iTargetStride;
        phwSourceBase += iSourceStride;
    }


    // Y shearing
    PRINTDBG("Y shear \n");
    int32_t         newWidth = ptShearRot->newDim[0];

    corner = ptShearRot->corner[1];
    pTarget = phwTargetBase;
    for (int i = 0; i < newWidth; i++) {
        int32_t         posXsrc = 1 + i - srcIHeightHalf;
        int32_t         start =
            Q0_TO_Q16(1 - newWidth / 2) +
            __arm_fxpt_mult32(sinAngle, Q0_TO_Q16(posXsrc), 2);

        start = start - corner;
        start = ROUND_Q16_TO_Q0(start);
        PRINTDBG(" startxI %d\n", start);

        int32_t         residual = ptTargetSize->iHeight - start - ptSourceSize->iHeight;
        __arm_2d_rgb32_move_col(pTarget, iTargetStride, start, ptSourceSize->iHeight);

        __arm_2d_rgb32_fill_col(pTarget, iTargetStride, start, filler);

        __arm_2d_rgb32_fill_col(pTarget + (start + ptSourceSize->iHeight) * iTargetStride,
                                iTargetStride, residual, filler);

        pTarget++;
    }

    PRINTDBG("X shear \n");
    // X shearing
    int32_t         newHeight = ptShearRot->newDim[1];
    int32_t         newHeightHalf = newHeight / 2;
    int32_t         newWidthHalf = newWidth / 2;
    int32_t         finalWidth = ptShearRot->newDim[2];

    corner = ptShearRot->corner[2];
    pTarget = phwTargetBase;
    for (int i = 0; i < newHeight; i++) {
        int32_t         posYsrc = 1 + i - newHeightHalf;
        int32_t         start =
            Q0_TO_Q16(1 - newWidthHalf) -
            __arm_fxpt_mult32(tanHalfAngle, Q0_TO_Q16(posYsrc), 2);

        start = start - corner;
        start = ROUND_Q16_TO_Q0(start);
        PRINTDBG(" startxI %d\n", start);

        int32_t         residual = finalWidth - start - newWidth;

        __arm_2d_rgb32_move_line(pTarget, start, newWidth);

        __arm_2d_rgb32_fill_line(pTarget, start, filler);

        __arm_2d_rgb32_fill_line(pTarget + start + newWidth, residual, filler);

        pTarget += iTargetStride;
    }
}





#endif

#if defined(__clang__)
#   pragma clang diagnostic pop
#endif

#ifdef   __cplusplus
}
#endif

#endif // __ARM_2D_HAS_HELIUM__

