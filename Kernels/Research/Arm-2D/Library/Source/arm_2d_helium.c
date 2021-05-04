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
 * $Date:        12. Jan 2021
 * $Revision:    V.0.5.0
 *
 * Target Processor:  Cortex-M cores
 *
 * -------------------------------------------------------------------- */

#define __ARM_2D_IMPL__

#include "arm_2d.h"

#if defined(__ARM_2D_HAS_HELIUM__) && __ARM_2D_HAS_HELIUM__

#include "__arm_2d_paving_helium.h"

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


/*! \brief initialise the helium service service
 *! \param none
 *! \return none
 */
void __arm_2d_helium_init(void)
{
    /* even if this is empty, do not remove it */
}


void __arm_2d_rgb16_2x2_paving(const uint16_t *__RESTRICT pSourceBase,
                                int16_t iSourceStride,
                                const arm_2d_size_t * ptSourceSize,
                                uint16_t * __RESTRICT pTargetBase,
                                int16_t iTargetStride,
                                uint16_t tilePairRows,
                                uint16_t tilePairCols)
{
    __ARM_2D_PAVING_2x2(16,
                        PAVING_DIRECT_START_OFFS(_, _),
                        PAVING_DIRECT_READ_DIR,
                        PAVING_DIRECT_SETUP_COPY(16),
                        PAVING_DIRECT_LOAD_PATTERN(16));
}


void __arm_2d_rgb16_2x2_paving_x_mirror(const uint16_t *__RESTRICT pSourceBase,
                                        int16_t iSourceStride,
                                        const arm_2d_size_t * ptSourceSize,
                                        uint16_t * __RESTRICT pTargetBase,
                                        int16_t iTargetStride,
                                        uint16_t tilePairRows,
                                        uint16_t tilePairCols)
{
    __ARM_2D_PAVING_2x2(16,
                        PAVING_X_MIRROR_START_OFFS(_, _),
                        PAVING_X_MIRROR_READ_DIR,
                        PAVING_X_MIRROR_SETUP_COPY(16),
                        PAVING_X_MIRROR_LOAD_PATTERN(16));
}


void __arm_2d_rgb16_2x2_paving_y_mirror(const uint16_t *__RESTRICT pSourceBase,
                                        int16_t iSourceStride,
                                        const arm_2d_size_t * ptSourceSize,
                                        uint16_t * __RESTRICT pTargetBase,
                                        int16_t iTargetStride,
                                        uint16_t tilePairRows,
                                        uint16_t tilePairCols)
{
    __ARM_2D_PAVING_2x2(16,
                        PAVING_Y_MIRROR_START_OFFS(iSourceStride, ptSourceSize->iHeight),
                        PAVING_Y_MIRROR_READ_DIR,
                        PAVING_Y_MIRROR_SETUP_COPY(16),
                        PAVING_Y_MIRROR_LOAD_PATTERN(16));
}

void __arm_2d_rgb16_2x2_paving_xy_mirror(const uint16_t *__RESTRICT pSourceBase,
                                            int16_t iSourceStride,
                                            const arm_2d_size_t * ptSourceSize,
                                            uint16_t * __RESTRICT pTargetBase,
                                            int16_t iTargetStride,
                                            uint16_t tilePairRows,
                                            uint16_t tilePairCols)
{
    __ARM_2D_PAVING_2x2(16,
                        PAVING_XY_MIRROR_START_OFFS(iSourceStride, ptSourceSize->iHeight),
                        PAVING_XY_MIRROR_READ_DIR,
                        PAVING_XY_MIRROR_SETUP_COPY(16),
                        PAVING_XY_MIRROR_LOAD_PATTERN(16));
}


void __arm_2d_rgb16_1x2_paving(const uint16_t * __RESTRICT pSourceBase,
                                       int16_t iSourceStride,
                                       const arm_2d_size_t * ptSourceSize,
                                       uint16_t * __RESTRICT pTargetBase,
                                       int16_t iTargetStride,
                                       uint32_t destWidth,
                                       uint16_t tilePairRows)
{
    __ARM_2D_PAVING_1x2(16,
                        PAVING_DIRECT_START_OFFS(_, _),
                        PAVING_DIRECT_READ_DIR,
                        PAVING_DIRECT_SETUP_COPY(16),
                        PAVING_DIRECT_LOAD_PATTERN(16));
}

void __arm_2d_rgb16_1x2_paving_x_mirror(const uint16_t * __RESTRICT pSourceBase,
                                        int16_t iSourceStride,
                                        const arm_2d_size_t * ptSourceSize,
                                        uint16_t * __RESTRICT pTargetBase,
                                        int16_t iTargetStride,
                                        uint32_t destWidth,
                                        uint16_t tilePairRows)
{
    __ARM_2D_PAVING_1x2(16,
                        PAVING_X_MIRROR_START_OFFS(_, _),
                        PAVING_X_MIRROR_READ_DIR,
                        PAVING_X_MIRROR_SETUP_COPY(16),
                        PAVING_X_MIRROR_LOAD_PATTERN(16));
}

void __arm_2d_rgb16_1x2_paving_y_mirror(const uint16_t * __RESTRICT pSourceBase,
                                       int16_t iSourceStride,
                                       const arm_2d_size_t * ptSourceSize,
                                       uint16_t * __RESTRICT pTargetBase,
                                       int16_t iTargetStride,
                                       uint32_t destWidth,
                                       uint16_t tilePairRows)
{
    __ARM_2D_PAVING_1x2(16,
                        PAVING_Y_MIRROR_START_OFFS(iSourceStride, ptSourceSize->iHeight),
                        PAVING_Y_MIRROR_READ_DIR,
                        PAVING_Y_MIRROR_SETUP_COPY(16),
                        PAVING_Y_MIRROR_LOAD_PATTERN(16));
}


void __arm_2d_rgb16_1x2_paving_xy_mirror(const uint16_t * __RESTRICT pSourceBase,
                                       int16_t iSourceStride,
                                       const arm_2d_size_t * ptSourceSize,
                                       uint16_t * __RESTRICT pTargetBase,
                                       int16_t iTargetStride,
                                       uint32_t destWidth,
                                       uint16_t tilePairRows)
{
    __ARM_2D_PAVING_1x2(16,
                        PAVING_XY_MIRROR_START_OFFS(iSourceStride, ptSourceSize->iHeight),
                        PAVING_XY_MIRROR_READ_DIR,
                        PAVING_XY_MIRROR_SETUP_COPY(16),
                        PAVING_XY_MIRROR_LOAD_PATTERN(16));
}


void __arm_2d_rgb16_2x1_paving(const uint16_t * __RESTRICT pSourceBase,
                                           int16_t iSourceStride,
                                           const arm_2d_size_t * __RESTRICT ptSourceSize,
                                           uint16_t * pTargetBase,
                                           int16_t iTargetStride,
                                           uint16_t tilePairCols,
                                           uint16_t destHeight)
{
    __ARM_2D_PAVING_2x1(16,
                        PAVING_DIRECT_START_OFFS(_, _),
                        PAVING_DIRECT_READ_DIR,
                        PAVING_DIRECT_SETUP_COPY(16),
                        PAVING_DIRECT_LOAD_PATTERN(16));
}

void __arm_2d_rgb16_2x1_paving_x_mirror(const uint16_t * __RESTRICT pSourceBase,
                                        int16_t iSourceStride,
                                        const arm_2d_size_t * __RESTRICT ptSourceSize,
                                        uint16_t * pTargetBase,
                                        int16_t iTargetStride,
                                        uint16_t tilePairCols,
                                        uint16_t destHeight)
{
    __ARM_2D_PAVING_2x1(16,
                        PAVING_X_MIRROR_START_OFFS(_, _),
                        PAVING_X_MIRROR_READ_DIR,
                        PAVING_X_MIRROR_SETUP_COPY(16),
                        PAVING_X_MIRROR_LOAD_PATTERN(16));
}

void __arm_2d_rgb16_2x1_paving_y_mirror(const uint16_t * __RESTRICT pSourceBase,
                                        int16_t iSourceStride,
                                        const arm_2d_size_t * __RESTRICT ptSourceSize,
                                        uint16_t * pTargetBase,
                                        int16_t iTargetStride,
                                        uint16_t tilePairCols,
                                        uint16_t destHeight)
{
    __ARM_2D_PAVING_2x1(16,
                        PAVING_Y_MIRROR_START_OFFS(iSourceStride, ptSourceSize->iHeight),
                        PAVING_Y_MIRROR_READ_DIR,
                        PAVING_Y_MIRROR_SETUP_COPY(16),
                        PAVING_Y_MIRROR_LOAD_PATTERN(16));
}


void __arm_2d_rgb16_2x1_paving_xy_mirror(const uint16_t * __RESTRICT pSourceBase,
                                        int16_t iSourceStride,
                                        const arm_2d_size_t * __RESTRICT ptSourceSize,
                                        uint16_t * pTargetBase,
                                        int16_t iTargetStride,
                                        uint16_t tilePairCols,
                                        uint16_t destHeight)
{
    __ARM_2D_PAVING_2x1(16,
                        PAVING_XY_MIRROR_START_OFFS(iSourceStride, ptSourceSize->iHeight),
                        PAVING_XY_MIRROR_READ_DIR,
                        PAVING_XY_MIRROR_SETUP_COPY(16),
                        PAVING_XY_MIRROR_LOAD_PATTERN(16));
}



void __arm_2d_rgb16_1x1_paving(const uint16_t * __RESTRICT pSource,
                               int16_t iSourceStride,
                               uint16_t * __RESTRICT pTarget,
                               int16_t iTargetStride,
                               const arm_2d_size_t * ptSrcCopySize,
                               const arm_2d_size_t * ptDstCopySize)
{
    __ARM_2D_PAVING_1x1(16,
                        PAVING_DIRECT_START_OFFS(_, _),
                        PAVING_DIRECT_READ_DIR,
                        PAVING_DIRECT_SETUP_COPY(16),
                        PAVING_DIRECT_LOAD_PATTERN(16));
}

void __arm_2d_rgb16_1x1_paving_x_mirror(const uint16_t * __RESTRICT pSource,
                                       int16_t iSourceStride,
                                       uint16_t * __RESTRICT pTarget,
                                       int16_t iTargetStride,
                                       const arm_2d_size_t * ptSrcCopySize,
                                       const arm_2d_size_t * ptDstCopySize)
{
    __ARM_2D_PAVING_1x1(16,
                        PAVING_X_MIRROR_START_OFFS(_, _),
                        PAVING_X_MIRROR_READ_DIR, PAVING_X_MIRROR_SETUP_COPY(16),
                        PAVING_X_MIRROR_LOAD_PATTERN(16));
}


void __arm_2d_rgb16_1x1_paving_y_mirror(const uint16_t * __RESTRICT pSource,
                                       int16_t iSourceStride,
                                       uint16_t * __RESTRICT pTarget,
                                       int16_t iTargetStride,
                                       const arm_2d_size_t * ptSrcCopySize,
                                       const arm_2d_size_t * ptDstCopySize)
{
    __ARM_2D_PAVING_1x1(16,
                        PAVING_Y_MIRROR_START_OFFS(iSourceStride, ptSrcCopySize->iHeight),
                        PAVING_Y_MIRROR_READ_DIR,
                        PAVING_Y_MIRROR_SETUP_COPY(16),
                        PAVING_Y_MIRROR_LOAD_PATTERN(16));
}


void __arm_2d_rgb16_1x1_paving_xy_mirror(const uint16_t * __RESTRICT pSource,
                                           int16_t iSourceStride,
                                           uint16_t * __RESTRICT pTarget,
                                           int16_t iTargetStride,
                                           const arm_2d_size_t * ptSrcCopySize,
                                           const arm_2d_size_t * ptDstCopySize)
{
    __ARM_2D_PAVING_1x1(16,
                        PAVING_XY_MIRROR_START_OFFS(iSourceStride, ptSrcCopySize->iHeight),
                        PAVING_XY_MIRROR_READ_DIR,
                        PAVING_XY_MIRROR_SETUP_COPY(16),
                        PAVING_XY_MIRROR_LOAD_PATTERN(16));
}




void __arm_2d_rgb32_2x2_paving(const uint32_t *__RESTRICT pSourceBase,
                                int16_t iSourceStride,
                                const arm_2d_size_t * ptSourceSize,
                                uint32_t * __RESTRICT pTargetBase,
                                int16_t iTargetStride,
                                uint16_t tilePairRows,
                                uint16_t tilePairCols)
{
    __ARM_2D_PAVING_2x2(32,
                        PAVING_DIRECT_START_OFFS(_, _),
                        PAVING_DIRECT_READ_DIR,
                        PAVING_DIRECT_SETUP_COPY(32),
                        PAVING_DIRECT_LOAD_PATTERN(32));
}


void __arm_2d_rgb32_2x2_paving_x_mirror(const uint32_t *__RESTRICT pSourceBase,
                                        int16_t iSourceStride,
                                        const arm_2d_size_t * ptSourceSize,
                                        uint32_t * __RESTRICT pTargetBase,
                                        int16_t iTargetStride,
                                        uint16_t tilePairRows,
                                        uint16_t tilePairCols)
{
    __ARM_2D_PAVING_2x2(32,
                        PAVING_X_MIRROR_START_OFFS(_, _),
                        PAVING_X_MIRROR_READ_DIR,
                        PAVING_X_MIRROR_SETUP_COPY(32),
                        PAVING_X_MIRROR_LOAD_PATTERN(32));
}


void __arm_2d_rgb32_2x2_paving_y_mirror(const uint32_t *__RESTRICT pSourceBase,
                                        int16_t iSourceStride,
                                        const arm_2d_size_t * ptSourceSize,
                                        uint32_t * __RESTRICT pTargetBase,
                                        int16_t iTargetStride,
                                        uint16_t tilePairRows,
                                        uint16_t tilePairCols)
{
    __ARM_2D_PAVING_2x2(32,
                        PAVING_Y_MIRROR_START_OFFS(iSourceStride, ptSourceSize->iHeight),
                        PAVING_Y_MIRROR_READ_DIR,
                        PAVING_Y_MIRROR_SETUP_COPY(32),
                        PAVING_Y_MIRROR_LOAD_PATTERN(32));
}

void __arm_2d_rgb32_2x2_paving_xy_mirror(const uint32_t *__RESTRICT pSourceBase,
                                        int16_t iSourceStride,
                                        const arm_2d_size_t * ptSourceSize,
                                        uint32_t * __RESTRICT pTargetBase,
                                        int16_t iTargetStride,
                                        uint16_t tilePairRows,
                                        uint16_t tilePairCols)
{
    __ARM_2D_PAVING_2x2(32,
                        PAVING_XY_MIRROR_START_OFFS(iSourceStride, ptSourceSize->iHeight),
                        PAVING_XY_MIRROR_READ_DIR,
                        PAVING_XY_MIRROR_SETUP_COPY(32),
                        PAVING_XY_MIRROR_LOAD_PATTERN(32));
}


void __arm_2d_rgb32_1x2_paving(const uint32_t * __RESTRICT pSourceBase,
                               int16_t iSourceStride,
                               const arm_2d_size_t * ptSourceSize,
                               uint32_t * __RESTRICT pTargetBase,
                               int16_t iTargetStride,
                               uint32_t destWidth,
                               uint16_t tilePairRows)
{
    __ARM_2D_PAVING_1x2(32,
                        PAVING_DIRECT_START_OFFS(_, _),
                        PAVING_DIRECT_READ_DIR,
                        PAVING_DIRECT_SETUP_COPY(32),
                        PAVING_DIRECT_LOAD_PATTERN(32));
}

void __arm_2d_rgb32_1x2_paving_x_mirror(const uint32_t * __RESTRICT pSourceBase,
                                        int16_t iSourceStride,
                                        const arm_2d_size_t * ptSourceSize,
                                        uint32_t * __RESTRICT pTargetBase,
                                        int16_t iTargetStride,
                                        uint32_t destWidth,
                                        uint16_t tilePairRows)
{
    __ARM_2D_PAVING_1x2(32,
                        PAVING_X_MIRROR_START_OFFS(_, _),
                        PAVING_X_MIRROR_READ_DIR,
                        PAVING_X_MIRROR_SETUP_COPY(32),
                        PAVING_X_MIRROR_LOAD_PATTERN(32));
}

void __arm_2d_rgb32_1x2_paving_y_mirror(const uint32_t * __RESTRICT pSourceBase,
                                       int16_t iSourceStride,
                                       const arm_2d_size_t * ptSourceSize,
                                       uint32_t * __RESTRICT pTargetBase,
                                       int16_t iTargetStride,
                                       uint32_t destWidth,
                                       uint16_t tilePairRows)
{
    __ARM_2D_PAVING_1x2(32,
                        PAVING_Y_MIRROR_START_OFFS(iSourceStride, ptSourceSize->iHeight),
                        PAVING_Y_MIRROR_READ_DIR,
                        PAVING_Y_MIRROR_SETUP_COPY(32),
                        PAVING_Y_MIRROR_LOAD_PATTERN(32));
}


void __arm_2d_rgb32_1x2_paving_xy_mirror(const uint32_t * __RESTRICT pSourceBase,
                                       int16_t iSourceStride,
                                       const arm_2d_size_t * ptSourceSize,
                                       uint32_t * __RESTRICT pTargetBase,
                                       int16_t iTargetStride,
                                       uint32_t destWidth,
                                       uint16_t tilePairRows)
{
    __ARM_2D_PAVING_1x2(32,
                        PAVING_XY_MIRROR_START_OFFS(iSourceStride, ptSourceSize->iHeight),
                        PAVING_XY_MIRROR_READ_DIR,
                        PAVING_XY_MIRROR_SETUP_COPY(32),
                        PAVING_XY_MIRROR_LOAD_PATTERN(32));
}


void __arm_2d_rgb32_2x1_paving(const uint32_t * __RESTRICT pSourceBase,
                               int16_t iSourceStride,
                               const arm_2d_size_t * __RESTRICT ptSourceSize,
                               uint32_t * pTargetBase,
                               int16_t iTargetStride,
                               uint16_t tilePairCols,
                               uint16_t destHeight)
{
    __ARM_2D_PAVING_2x1(32,
                        PAVING_DIRECT_START_OFFS(_, _),
                        PAVING_DIRECT_READ_DIR,
                        PAVING_DIRECT_SETUP_COPY(32),
                        PAVING_DIRECT_LOAD_PATTERN(32));
}

void __arm_2d_rgb32_2x1_paving_x_mirror(const uint32_t * __RESTRICT pSourceBase,
                                        int16_t iSourceStride,
                                        const arm_2d_size_t * __RESTRICT ptSourceSize,
                                        uint32_t * pTargetBase,
                                        int16_t iTargetStride,
                                        uint16_t tilePairCols,
                                        uint16_t destHeight)
{
    __ARM_2D_PAVING_2x1(32,
                        PAVING_X_MIRROR_START_OFFS(_, _),
                        PAVING_X_MIRROR_READ_DIR,
                        PAVING_X_MIRROR_SETUP_COPY(32),
                        PAVING_X_MIRROR_LOAD_PATTERN(32));
}

void __arm_2d_rgb32_2x1_paving_y_mirror(const uint32_t * __RESTRICT pSourceBase,
                                        int16_t iSourceStride,
                                        const arm_2d_size_t * __RESTRICT ptSourceSize,
                                        uint32_t * pTargetBase,
                                        int16_t iTargetStride,
                                        uint16_t tilePairCols,
                                        uint16_t destHeight)
{
    __ARM_2D_PAVING_2x1(32,
                        PAVING_Y_MIRROR_START_OFFS(iSourceStride, ptSourceSize->iHeight),
                        PAVING_Y_MIRROR_READ_DIR,
                        PAVING_Y_MIRROR_SETUP_COPY(32),
                        PAVING_Y_MIRROR_LOAD_PATTERN(32));
}


void __arm_2d_rgb32_2x1_paving_xy_mirror(const uint32_t * __RESTRICT pSourceBase,
                                        int16_t iSourceStride,
                                        const arm_2d_size_t * __RESTRICT ptSourceSize,
                                        uint32_t * pTargetBase,
                                        int16_t iTargetStride,
                                        uint16_t tilePairCols,
                                        uint16_t destHeight)
{
    __ARM_2D_PAVING_2x1(32,
                        PAVING_XY_MIRROR_START_OFFS(iSourceStride, ptSourceSize->iHeight),
                        PAVING_XY_MIRROR_READ_DIR,
                        PAVING_XY_MIRROR_SETUP_COPY(32),
                        PAVING_XY_MIRROR_LOAD_PATTERN(32));
}



void __arm_2d_rgb32_1x1_paving(const uint32_t * __RESTRICT pSource,
                                   int16_t iSourceStride,
                                   uint32_t * __RESTRICT pTarget,
                                   int16_t iTargetStride,
                                   const arm_2d_size_t * ptSrcCopySize,
                                   const arm_2d_size_t * ptDstCopySize)
{
    __ARM_2D_PAVING_1x1(32,
                        PAVING_DIRECT_START_OFFS(_, _),
                        PAVING_DIRECT_READ_DIR,
                        PAVING_DIRECT_SETUP_COPY(32),
                        PAVING_DIRECT_LOAD_PATTERN(32));
}

void __arm_2d_rgb32_1x1_paving_x_mirror(const uint32_t * __RESTRICT pSource,
                                       int16_t iSourceStride,
                                       uint32_t * __RESTRICT pTarget,
                                       int16_t iTargetStride,
                                       const arm_2d_size_t * ptSrcCopySize,
                                       const arm_2d_size_t * ptDstCopySize)
{
    __ARM_2D_PAVING_1x1(32,
                        PAVING_X_MIRROR_START_OFFS(_, _),
                        PAVING_X_MIRROR_READ_DIR, PAVING_X_MIRROR_SETUP_COPY(32),
                        PAVING_X_MIRROR_LOAD_PATTERN(32));
}


void __arm_2d_rgb32_1x1_paving_y_mirror(const uint32_t * __RESTRICT pSource,
                                       int16_t iSourceStride,
                                       uint32_t * __RESTRICT pTarget,
                                       int16_t iTargetStride,
                                       const arm_2d_size_t * ptSrcCopySize,
                                       const arm_2d_size_t * ptDstCopySize)
{
    __ARM_2D_PAVING_1x1(32,
                        PAVING_Y_MIRROR_START_OFFS(iSourceStride, ptSrcCopySize->iHeight),
                        PAVING_Y_MIRROR_READ_DIR,
                        PAVING_Y_MIRROR_SETUP_COPY(32),
                        PAVING_Y_MIRROR_LOAD_PATTERN(32));
}


void __arm_2d_rgb32_1x1_paving_xy_mirror(const uint32_t * __RESTRICT pSource,
                                       int16_t iSourceStride,
                                       uint32_t * __RESTRICT pTarget,
                                       int16_t iTargetStride,
                                       const arm_2d_size_t * ptSrcCopySize,
                                       const arm_2d_size_t * ptDstCopySize)
{
    __ARM_2D_PAVING_1x1(32,
                        PAVING_XY_MIRROR_START_OFFS(iSourceStride, ptSrcCopySize->iHeight),
                        PAVING_XY_MIRROR_READ_DIR,
                        PAVING_XY_MIRROR_SETUP_COPY(32),
                        PAVING_XY_MIRROR_LOAD_PATTERN(32));
}

/* paving with color masking */

__OVERRIDE_WEAK
void __arm_2d_rgb16_2x2_cl_msk_paving(  const uint16_t *__RESTRICT pSourceBase,
                                        int16_t iSourceStride,
                                        const arm_2d_size_t * __RESTRICT ptSourceSize,
                                        uint16_t * __RESTRICT pTargetBase,
                                        int16_t iTargetStride,
                                        uint16_t tilePairRows,
                                        uint16_t tilePairCols,
                                        uint16_t hwColour)
{
    __ARM_2D_PAVING_2x2(16,
                        PAVING_DIRECT_START_OFFS(_, _),
                        PAVING_DIRECT_READ_DIR,
                        PAVING_DIRECT_SETUP_COPY(16),
                        PAVING_DIRECT_LOAD_PATTERN(16),
                        CMP_CL_MSK(16, hwColour));
}


__OVERRIDE_WEAK
void __arm_2d_rgb16_1x2_cl_msk_paving(  const uint16_t * __RESTRICT pSourceBase,
                                        int16_t iSourceStride,
                                        const arm_2d_size_t * __RESTRICT ptSourceSize,
                                        uint16_t * __RESTRICT pTargetBase,
                                        int16_t iTargetStride,
                                        uint32_t destWidth,
                                        uint16_t tilePairRows,
                                        uint16_t hwColour)
{
    __ARM_2D_PAVING_1x2(16,
                        PAVING_DIRECT_START_OFFS(_, _),
                        PAVING_DIRECT_READ_DIR,
                        PAVING_DIRECT_SETUP_COPY(16),
                        PAVING_DIRECT_LOAD_PATTERN(16),
                        CMP_CL_MSK(16, hwColour));
}

__OVERRIDE_WEAK
void __arm_2d_rgb16_2x1_cl_msk_paving(  const uint16_t * __RESTRICT pSourceBase,
                                        int16_t iSourceStride,
                                        const arm_2d_size_t * __RESTRICT ptSourceSize,
                                        uint16_t * __RESTRICT pTargetBase,
                                        int16_t iTargetStride,
                                        uint16_t tilePairCols,
                                        uint16_t destHeight,
                                        uint16_t hwColour)
{
    __ARM_2D_PAVING_2x1(16,
                        PAVING_DIRECT_START_OFFS(_, _),
                        PAVING_DIRECT_READ_DIR,
                        PAVING_DIRECT_SETUP_COPY(16),
                        PAVING_DIRECT_LOAD_PATTERN(16),
                        CMP_CL_MSK(16, hwColour));
}

__OVERRIDE_WEAK
void __arm_2d_rgb16_1x1_cl_msk_paving(
                                const uint16_t * __RESTRICT pSource,
                                int16_t iSourceStride,
                                uint16_t * __RESTRICT pTarget,
                                int16_t iTargetStride,
                                const arm_2d_size_t * __RESTRICT ptSrcCopySize,
                                const arm_2d_size_t * __RESTRICT ptDstCopySize,
                                uint16_t hwColour)
{
    __ARM_2D_PAVING_1x1(16,
                        PAVING_DIRECT_START_OFFS(_, _),
                        PAVING_DIRECT_READ_DIR,
                        PAVING_DIRECT_SETUP_COPY(16),
                        PAVING_DIRECT_LOAD_PATTERN(16),
                        CMP_CL_MSK(16, hwColour));
}



__OVERRIDE_WEAK
void __arm_2d_rgb32_2x2_cl_msk_paving(  const uint32_t *__RESTRICT pSourceBase,
                                        int16_t iSourceStride,
                                        const arm_2d_size_t * __RESTRICT ptSourceSize,
                                        uint32_t * __RESTRICT pTargetBase,
                                        int16_t iTargetStride,
                                        uint16_t tilePairRows,
                                        uint16_t tilePairCols,
                                        uint32_t wColour)
{
    __ARM_2D_PAVING_2x2(32,
                        PAVING_DIRECT_START_OFFS(_, _),
                        PAVING_DIRECT_READ_DIR,
                        PAVING_DIRECT_SETUP_COPY(32),
                        PAVING_DIRECT_LOAD_PATTERN(32),
                        CMP_CL_MSK(32, wColour));
}

__OVERRIDE_WEAK
void __arm_2d_rgb32_1x2_cl_msk_paving(
                                const uint32_t * __RESTRICT pSourceBase,
                                int16_t iSourceStride,
                                const arm_2d_size_t * __RESTRICT ptSourceSize,
                                uint32_t * __RESTRICT pTargetBase,
                                int16_t iTargetStride,
                                uint32_t destWidth,
                                uint16_t tilePairRows,
                                uint32_t wColour)
{
    __ARM_2D_PAVING_1x2(32,
                        PAVING_DIRECT_START_OFFS(_, _),
                        PAVING_DIRECT_READ_DIR,
                        PAVING_DIRECT_SETUP_COPY(32),
                        PAVING_DIRECT_LOAD_PATTERN(32),
                        CMP_CL_MSK(32, wColour));
}

__OVERRIDE_WEAK
void __arm_2d_rgb32_2x1_cl_msk_paving(
                                const uint32_t * __RESTRICT pSourceBase,
                                int16_t iSourceStride,
                                const arm_2d_size_t * __RESTRICT ptSourceSize,
                                uint32_t * __RESTRICT pTargetBase,
                                int16_t iTargetStride,
                                uint16_t tilePairCols,
                                uint16_t destHeight,
                                uint32_t wColour)
{
    __ARM_2D_PAVING_2x1(32,
                        PAVING_DIRECT_START_OFFS(_, _),
                        PAVING_DIRECT_READ_DIR,
                        PAVING_DIRECT_SETUP_COPY(32),
                        PAVING_DIRECT_LOAD_PATTERN(32),
                        CMP_CL_MSK(32, wColour));
}

__OVERRIDE_WEAK
void __arm_2d_rgb32_1x1_cl_msk_paving(
                                const uint32_t * __RESTRICT pSource,
                                int16_t iSourceStride,
                                uint32_t * __RESTRICT pTarget,
                                int16_t iTargetStride,
                                const arm_2d_size_t * __RESTRICT ptSrcCopySize,
                                const arm_2d_size_t * __RESTRICT ptDstCopySize,
                                uint32_t wColour)
{
    __ARM_2D_PAVING_1x1(32,
                        PAVING_DIRECT_START_OFFS(_, _),
                        PAVING_DIRECT_READ_DIR,
                        PAVING_DIRECT_SETUP_COPY(32),
                        PAVING_DIRECT_LOAD_PATTERN(32),
                        CMP_CL_MSK(32, wColour));
}


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
    for (int_fast16_t x = 0; x < ptCopySize->iWidth; x++) {
        uint16x8_t      srcStr = vidupq_u16((uint32_t) 0, 1);
        uint16x8_t      dstStr = vidupq_u16((uint32_t) 0, 1);

        srcStr = srcStr * iSourceStride;
        dstStr = dstStr * iTargetStride;

        for (int_fast16_t y = 0; y < ptCopySize->iHeight / 8; y++) {
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
#if defined (ARM_MATH_HELIUM) || defined(ARM_MATH_MVEF) || defined(ARM_MATH_MVEI)
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

#else /*  defined (ARM_MATH_HELIUM) || defined(ARM_MATH_MVEF) || defined(ARM_MATH_MVEI) */
    for (int_fast16_t y = 0; y < ptCopySize->iHeight; y++) {
        memcpy(phwTarget, phwSource, ptCopySize->iWidth * sizeof(*phwSource));
        phwSource += iSourceStride;
        phwTarget += iTargetStride;
    }
#endif
}

__OVERRIDE_WEAK
 void __arm_2d_impl_rgb32_copy(   uint32_t *pwSource,
                                                int16_t iSourceStride,
                                                uint32_t *pwTarget,
                                                int16_t iTargetStride,
                                                arm_2d_size_t *ptCopySize)
{
#if defined (ARM_MATH_HELIUM) || defined(ARM_MATH_MVEF) || defined(ARM_MATH_MVEI)
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
#else
    for (int_fast16_t y = 0; y < ptCopySize->iHeight; y++) {
        memcpy(pwTarget, pwSource, ptCopySize->iWidth * sizeof(*pwSource));
        pwSource += iSourceStride;
        pwTarget += iTargetStride;
    }
#endif
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
#if defined (ARM_MATH_HELIUM) || defined(ARM_MATH_MVEF) || defined(ARM_MATH_MVEI)
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


    for (int_fast16_t y = 0; y < ptCopySize->iHeight; y++) {
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

    int32_t         row = ptCopySize->iHeight;
    do {
        const uint16_t *phwSource = phwSourceBase;
        uint16_t       *phwTarget = phwTargetBase;
        register unsigned loopCnt  __asm("lr");
        loopCnt = ptCopySize->iWidth;

    __asm volatile(
        "   vldrh.u16               q4, [%[phwTarget]]           \n"
        "   vmov.i16                q6, #0x00fc                  \n"

        "   vldrh.u16               q5, [%[phwSource]], #16      \n"
        "   wlstp.16                lr, %[loopCnt], 1f           \n"
        "2:                                                      \n"
        // B target extraction
        "   vand                    q6, q4, %[vecMaskR]          \n"
        "   vmul.i16                q6, q6, %[ratio2x8]          \n"
        "   vshr.u16                q2, q4, #5                   \n"
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
        "   vshr.u16                q2, q2, #8                   \n"
        // R mix
        "   vmla.u16                q7, q5, %[ratio1x8]          \n"
        "   vshr.u16                q4, q7, #8                   \n"

        "   vmov.i16                q7, #0x00fc                  \n"
        "   vand                    q2, q2, q7                   \n"
        // vmulq((vecG0 & 0x00fc), 8)
        "   vmul.i16                q2, q2, %[eight]             \n"
        "   vand                    q7, q4, %[vecMaskBpck]       \n"
        // schedule next source load
        "   vldrh.u16               q5, [%[phwSource]], #16      \n"
        // pack R & G
        // vmulq((vecG0 & vecMaskGpck), 8) + vmulq((vecR0 & vecMaskRpck), 256)
        "   vmla.u16                q2, q7, %[twofiftysix]       \n"
        // downshift B ((vecB0 >> 8) >> 3)
        "   vshr.u16                q7, q6, #11                  \n"
        // schedule next target load (pre offset as target not imcrementred so far)
        "   vldrh.u16               q4, [%[phwTarget], #16]      \n"
        // pack blue with R&G
        "   vorr                    q2, q2, q7                   \n"
        "   vstrh.16                q2, [%[phwTarget]], #16      \n"
        "   letp                    lr, 2b                       \n"
        "1:                                                      \n"

        : [phwSource] "+r"(phwSource),  [phwTarget] "+r" (phwTarget), [loopCnt] "+r"(loopCnt)
        : [vecMaskR] "t" (vecMaskR), [vecMaskG] "t" (vecMaskG),
          [vecMaskBpck] "t" (vecMaskBpck),
          [ratio1x8] "r" (ratio1x8), [ratio2x8] "r" (ratio2x8),
          [ratio1x4] "r" (ratio1x4), [ratio2x4] "r" (ratio2x4),
          [eight] "r" (8), [twofiftysix] "r" (256)
        : "q2", "q4", "q5", "q6", "q7", "memory" );

        phwSourceBase += iSourceStride;
        phwTargetBase += iTargetStride;
    } while (--row);
#endif /* USE_MVE_INTRINSICS */

#else
    /* auto-vectorized */
    uint32_t iHeight = ptCopySize->iHeight;
    uint32_t iWidth  = ptCopySize->iWidth;

    for (int_fast16_t y = 0; y < ptCopySize->iHeight; y++) {

        const uint16_t  *phwSource = phwSourceBase;
        uint16_t        *phwTarget = phwTargetBase;
        uint16_t         ratioCompl = 256 - chRatio;

        for (uint32_t x = 0; x < iWidth; x++) {
            __arm_2d_color_fast_rgb_t wSourcePixel, wTargetPixel;

            __arm_2d_rgb565_unpack(*phwSourceBase++, &wSourcePixel);
            __arm_2d_rgb565_unpack(*phwTargetBase, &wTargetPixel);

            for (int i = 0; i < 3; i++) {
                uint16_t        tmp =
                    (uint16_t) (wSourcePixel.RGB[i] * chRatio) +
                    (wTargetPixel.RGB[i] * ratioCompl);
                wTargetPixel.RGB[i] = (uint16_t) (tmp >> 8);
            }
            /* pack merged stream */
            *phwTargetBase++ = __arm_2d_rgb565_pack(&wTargetPixel);
        }
        phwSourceBase += iSourceStride;
        phwTargetBase += iTargetStride;
    }
#endif
}

__OVERRIDE_WEAK
void __arm_2d_impl_rgb888_alpha_blending(   uint32_t *pwSourceBase,
                                            int16_t iSourceStride,
                                            uint32_t *pwTargetBase,
                                            int16_t iTargetStride,
                                            arm_2d_size_t *ptCopySize,
                                            uint_fast8_t chRatio)
{
#if defined (ARM_MATH_HELIUM) || defined(ARM_MATH_MVEF) || defined(ARM_MATH_MVEI)
    uint16_t        chRatioCompl = 256 - (uint16_t) chRatio;
    register unsigned blkCnt  __asm("lr");
    int32_t row = ptCopySize->iHeight;

    do
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
    } while (--row);

#else /* defined (ARM_MATH_HELIUM) || defined(ARM_MATH_MVEF) || defined(ARM_MATH_MVEI) */
    for (int_fast16_t y = 0; y < ptCopySize->iHeight; y++) {

        const uint32_t *pwSource = pwSourceBase;
        uint32_t *pwTarget = pwTargetBase;

        for (int_fast16_t x = 0; x < ptCopySize->iWidth; x++) {

            uint_fast8_t n = sizeof(uint32_t);
            const uint8_t *pchSrc = (uint8_t *)(pwSource++);
            uint8_t *pchDes = (uint8_t *)(pwTarget++);

            do {
                *pchDes = ( ((uint_fast16_t)(*pchSrc++) * chRatio)
                          + (   (uint_fast16_t)(*pchDes)
                            *   (256 - (uint_fast16_t)chRatio))) >> 8;
                 pchDes++;
            } while(--n);
        }

        pwSourceBase += iSourceStride;
        pwTargetBase += iTargetStride;
    }
#endif
}

__OVERRIDE_WEAK
void __arm_2d_impl_rgb565_alpha_blending_direct(const uint16_t *phwSource,
                                                const uint16_t *phwBackground,
                                                uint16_t *phwDestination,
                                                uint32_t wPixelCount,
                                                uint_fast8_t chRatio)
{
#if defined (ARM_MATH_HELIUM) || defined(ARM_MATH_MVEF) || defined(ARM_MATH_MVEI)
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

#else
    do {
        __arm_2d_color_fast_rgb_t wSourcePixel, wBackgroundPixel, wTargetPixel;
        uint16_t           ratioCompl = 256 - chRatio;

        __arm_2d_rgb565_unpack(*phwSource++, &wSourcePixel);
        __arm_2d_rgb565_unpack(*phwBackground++, &wBackgroundPixel);

        for (int i = 0; i < 3; i++) {
            uint16_t        tmp =
                (uint16_t) (wSourcePixel.RGB[i] * chRatio) +
                (wBackgroundPixel.RGB[i] * ratioCompl);
            wTargetPixel.RGB[i] = (uint16_t) (tmp >> 8);
        }

        *phwDestination++ = __arm_2d_rgb565_pack(&wTargetPixel);
    } while (--wPixelCount);

#endif
}

__OVERRIDE_WEAK
void __arm_2d_impl_rgb888_alpha_blending_direct(const uint32_t *pwSource,
                                                const uint32_t *pwBackground,
                                                uint32_t *pwDestination,
                                                uint32_t wPixelCount,
                                                uint_fast8_t chRatio)
{
#if defined (ARM_MATH_HELIUM) || defined(ARM_MATH_MVEF) || defined(ARM_MATH_MVEI)
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

#else /* defined (ARM_MATH_HELIUM) || defined(ARM_MATH_MVEF) || defined(ARM_MATH_MVEI) */
    do {
        uint_fast8_t n = sizeof(uint32_t);
        const uint8_t *pchSrc = (uint8_t *)(pwSource++);
        const uint8_t *pchBG = (uint8_t *)(pwBackground++);
        uint8_t *pchDes = (uint8_t *)(pwDestination++);

        do {
            *pchDes++ = (   ((uint_fast16_t)(*pchSrc++) * chRatio)
                        +   (   (uint_fast16_t)(*pchBG++)
                            *   (256 - (uint_fast16_t)chRatio))) >> 8;
        } while(--n);

    } while(--wPixelCount);
#endif
}

/*----------------------------------------------------------------------------*
 * Misc & Experimental                                                        *
 *----------------------------------------------------------------------------*/

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
#if !defined(TESTING) && (defined(ARM_MATH_HELIUM) || defined(ARM_MATH_MVEF) || defined(ARM_MATH_MVEI))
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
#if !defined(TESTING) && (defined(ARM_MATH_HELIUM) || defined(ARM_MATH_MVEF) || defined(ARM_MATH_MVEI))
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


#ifdef EXPERIMENTAL

/*
 * rotation trial
 */

#include "arm_math.h"

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

#endif // #if defined (ARM_MATH_HELIUM) || defined(ARM_MATH_MVEF) || defined(ARM_MATH_MVEI)

