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
 * Title:        arm-2d_cde.c
 * Description:  Basic Tile operations
 *
 * $Date:        18. Mar 2021
 * $Revision:    V.0.1.0
 *
 * Target Processor:  Cortex-M cores
 *
 * -------------------------------------------------------------------- */

#define __ARM_2D_IMPL__

#include "arm_2d.h"

#if defined(__ARM_2D_HAS_CDE__) && __ARM_2D_HAS_CDE__

#include <arm_cde.h>

#ifdef   __cplusplus
extern "C" {
#endif

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
#endif



#ifdef EXPERIMENTAL

void __arm_enable_cde(void)
{
    volatile * uint32_t        cde_reg = (volatile * uint32_t)0xE000ED88;

    *cde_reg |= 0xffff;
    __builtin_arm_isb(0xf);
}


/*! \brief initialise the helium service service
 *! \param none
 *! \return none
 */
void __arm_2d_cde_init(void)
{
    __arm_enable_cde();

}

/*----------------------------------------------------------------------------*
 * alpha blending                                                             *
 *----------------------------------------------------------------------------*/

/* Vector RGB16 Mix */
#ifdef USE_MVE_INTRINSICS
#define VRGB16MIX(In1Out, In2, Ratio)                           \
        __arm_vcx3qa(0, In1Out, In2, Ratio, 0b01)

#else
#define VRGB16MIX_ASM(Qd, Qn, Qm)                               \
        "    vcx3a                    p0,"#Qd","#Qn","#Qm",#0x1 \n"

#endif

__OVERRIDE_WEAK
void __arm_2d_impl_rgb565_alpha_blending(   uint16_t *phwSourceBase,
                                            int16_t iSourceStride,
                                            uint16_t *phwTargetBase,
                                            int16_t iTargetStride,
                                            arm_2d_size_t *ptCopySize,
                                            uint_fast8_t chRatio)
{
#if defined (ARM_MATH_HELIUM) || defined(ARM_MATH_MVEI)
    int32_t         row = ptCopySize->iHeight;
    int16x8_t       vecRatio = vdupq_n_s16(chRatio >> 1);
    do {
        const uint16_t *phwSource = phwSourceBase;
        uint16_t       *phwTarget = phwTargetBase;
        int32_t         loopCnt = ptCopySize->iWidth;

#ifdef USE_MVE_INTRINSICS
        do {
            mve_pred16_t    p = vctp16q(loopCnt);
            uint16x8_t      vecIn1 = vld1q_z(phwTarget, p);
            uint16x8_t      vecIn2 = vld1q_z(phwSource, p);

            vst1q_p_u16(phwTarget, VRGB16MIX(vecIn1, vecIn2, vecRatio), p);

            phwTarget += 8;
            phwSource += 8;
            loopCnt -= 8;
        }
        while (loopCnt > 0);
#else
        __asm volatile  (
            ".p2align 2                                                  \n"
            "   wlstp.16                lr, %[cnt], 1f                   \n"
            "2:                                                          \n"
            "   vldrh.u16               q1,[%[phwTarget]]                \n"
            "   vldrh.u16               q2,[%[phwSource]],#16            \n"

            /* Blend the result with CDE */
            VRGB16MIX_ASM   (q1, q2, %[ratio])
            "   vstrh.16                q1,[%[phwTarget]],#16            \n"
            "   letp                    lr,#2b                           \n"
            "1:                                                          \n"
            :[phwTarget] "+r"(phwTarget),[phwSource] "+r"(phwSource)
            :[cnt] "r"      (loopCnt),[ratio] "t"(vecRatio)
            :"q1", "q2", "memory", "r14");
#endif
        phwSourceBase += iSourceStride;
        phwTargetBase += iTargetStride;
    } while (--row);
#endif
}




__OVERRIDE_WEAK
void __arm_2d_impl_rgb565_alpha_blending_colour_masking(
                                                uint16_t * __RESTRICT phwSource,
                                                int16_t         iSourceStride,
                                                uint16_t * __RESTRICT phwTarget,
                                                int16_t         iTargetStride,
                                                arm_2d_size_t * __RESTRICT ptCopySize,
                                                uint_fast8_t    chRatio,
                                                uint_fast16_t   hwColour)
{
#if defined (ARM_MATH_HELIUM) || defined(ARM_MATH_MVEI)
    uint32_t        iHeight = ptCopySize->iHeight;
    uint32_t        iWidth = ptCopySize->iWidth;
    int16x8_t       vecRatio = vdupq_n_s16(chRatio >> 1);
    do {
        const uint16_t *phwSourceCur = phwSource;
        uint16_t       *phwTargetCur = phwTarget;
        int32_t         loopCnt = iWidth;
#ifdef USE_MVE_INTRINSICS
        do {
            mve_pred16_t    p = vctp16q(loopCnt);
            uint16x8_t      vecHwDst = vld1q_z(phwTargetCur, p);
            uint16x8_t      vecHwSrc = vld1q_z(phwSourceCur, p);

            /* Blend the result with CDE */
            vecHwDst = VRGB16MIX(vecHwDst, vecHwSrc, vecRatio);

            p = vcmpneq_m_n_u16(vecHwSrc, hwColour, p);
            vst1q_p_u16(phwTargetCur, vecHwDst, p);

            phwTargetCur += 8;
            phwSourceCur += 8;
            loopCnt -= 8;
        }
        while (loopCnt > 0);
#else
        __asm volatile  (
            ".p2align 2                                                  \n"
            "   wlstp.16                lr, %[cnt], 1f                   \n"
            "   vdup.i16                q0,%[ratio]                      \n"
            "2:                                                          \n"
            "   vldrh.u16               q1,[%[phwTarget]]                \n"
            "   vldrh.u16               q2,[%[phwSource]],#16            \n"
            /* Blend the result with CDE */
            VRGB16MIX_ASM   (q1, q2, q0)
            "   vpt.I16                 NE, q2, %[hwColour]              \n"
            "   vstrht.16               q1,[%[phwTarget]],#16            \n"
            "   letp                    lr,#2b                           \n"
            "1:                                                          \n"
            :[phwTarget] "+r"(phwTargetCur),[phwSource] "+r"(phwSourceCur)
            :[cnt] "r"      (loopCnt),[ratio] "r"(ratio),
            [hwColour] "r"(hwColour)
            :"q0", "q1", "q2", "memory", "r14");
#endif
        phwSource += iSourceStride;
        phwTarget += iTargetStride;
    } while (--iHeight);
#endif
}




__OVERRIDE_WEAK
void __arm_2d_impl_rgb565_alpha_blending_direct(const uint16_t *phwSource,
                                                const uint16_t *phwBackground,
                                                uint16_t *phwDestination,
                                                uint32_t wPixelCount,
                                                uint_fast8_t chRatio)
{
#if defined (ARM_MATH_HELIUM) || defined(ARM_MATH_MVEI)
    int16x8_t       vecRatio = vdupq_n_s16(chRatio >> 1);
#ifdef USE_MVE_INTRINSICS
    int32_t         loopCnt = wPixelCount;
    do {
        mve_pred16_t    p = vctp16q(loopCnt);
        uint16x8_t      vecSrc = vld1q_z(phwSource, p);
        uint16x8_t      vecBckg = vld1q_z(phwBackground, p);

        vst1q_p_u16(phwDestination, VRGB16MIX(vecSrc, vecBckg, vecRatio), p);

        phwDestination += 8;
        phwBackground += 8;
        phwSource += 8;
        loopCnt -= 8;
    }
    while (loopCnt > 0);
#else
    __asm volatile  (
        ".p2align 2                                                  \n"
        "   wlstp.16                lr, %[cnt], 1f                   \n"
        "   vdup.i16                q0,%[ratio]                      \n"
        "2:                                                          \n"
        "   vldrh.u16               q1,[%[phwSource]],#16            \n"
        "   vldrh.u16               q2,[%[phwBackground]],#16        \n"

        /* Blend the result with CDE */
        VRGB16MIX_ASM               (q1, q2, q0)
        "   vstrh.16                q1,[%[phwDestination]],#16       \n"
        "   letp                    lr,#2b                           \n"
        "1:                                                          \n"
        :[phwDestination] "+r"(phwDestination),[phwSource] "+r"(phwSource),
         [phwBackground] "+r"(phwBackground)
        :[cnt] "r" (wPixelCount),[ratio] "r"(ratio)
        :"q0", "q1", "q2", "memory", "r14");
#endif
#endif
}

#endif //EXPERIMENTAL

#if defined(__clang__)
#   pragma clang diagnostic pop
#endif

#ifdef   __cplusplus
}
#endif

#endif // #if defined (__ARM_2D_HAS_ACI__)

