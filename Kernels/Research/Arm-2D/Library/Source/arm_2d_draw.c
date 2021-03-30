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
 * Title:        arm-2d_draw.c
 * Description:  Basic Tile operations
 *
 * $Date:        22. Febrary 2020
 * $Revision:    V.0.5.0
 *
 * Target Processor:  Cortex-M cores
 *
 * -------------------------------------------------------------------- */


/*============================ INCLUDES ======================================*/
#define __ARM_2D_IMPL__

#include "arm_2d.h"
#include "__arm_2d_impl.h"

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
#   pragma clang diagnostic ignored "-Wsign-compare"
#   pragma clang diagnostic ignored "-Wmissing-prototypes"
#   pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#   pragma clang diagnostic ignored "-Wswitch-enum"
#   pragma clang diagnostic ignored "-Wswitch"
#endif

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/

void __arm_2d_impl_rgb16_colour_filling(uint16_t *__RESTRICT phwTarget,
                                        int16_t iTargetStride,
                                        arm_2d_size_t *__RESTRICT ptCopySize,
                                        uint16_t hwColour);

void __arm_2d_impl_rgb32_colour_filling(uint32_t *__RESTRICT pwTarget,
                                        int16_t iTargetStride,
                                        arm_2d_size_t *__RESTRICT ptCopySize,
                                        uint32_t hwColour);
/*============================ LOCAL VARIABLES ===============================*/
/*============================ IMPLEMENTATION ================================*/


/*----------------------------------------------------------------------------*
 * Draw a point with specified colour                                         *
 *----------------------------------------------------------------------------*/

ARM_NONNULL(1)
arm_fsm_rt_t arm_2d_rgb16_draw_point(   const arm_2d_tile_t *ptTarget,
                                        const arm_2d_location_t tLocation,
                                        uint_fast16_t hwColour)
{
    assert(NULL != ptTarget);

    ARM_2D_IMPL(arm_2d_op_drw_pt_t);
    memset(ptThis, 0, sizeof(*ptThis));
    
    arm_2d_region_t tPointRegion = {
        .tLocation = tLocation,
        .tSize = {1,1},
    };

    OP_CORE.ptOp = &ARM_2D_OP_TABLE[ARM_2D_OP_DRAW_POINT_RGB16];
    OP_CORE.Preference.u2ACCMethods = ARM_2D_PREF_ACC_SW_ONLY;

    this.Target.ptTile = ptTarget;
    this.Target.ptRegion = &tPointRegion;
    this.hwColour = hwColour;

    return __arm_2d_op_invoke(NULL);
}

                                       
ARM_NONNULL(1)
arm_fsm_rt_t arm_2d_rgb32_draw_point(   const arm_2d_tile_t *ptTarget,
                                        const arm_2d_location_t tLocation,
                                        uint32_t wColour)
{
    assert(NULL != ptTarget);

    ARM_2D_IMPL(arm_2d_op_drw_pt_t);
    memset(ptThis, 0, sizeof(*ptThis));
    
    arm_2d_region_t tPointRegion = {
        .tLocation = tLocation,
        .tSize = {1,1},
    };

    OP_CORE.ptOp = &ARM_2D_OP_TABLE[ARM_2D_OP_DRAW_POINT_RGB32];
    OP_CORE.Preference.u2ACCMethods = ARM_2D_PREF_ACC_SW_ONLY;
    
    this.Target.ptTile = ptTarget;
    this.Target.ptRegion = &tPointRegion;
    this.wColour = wColour;

    return __arm_2d_op_invoke(NULL);
}

arm_fsm_rt_t __arm_2d_sw_draw_point(__arm_2d_sub_task_t *ptTask,
                                    void *__RESTRICT pTarget,
                                    int16_t iStride,
                                    arm_2d_size_t *__RESTRICT ptSize)
{
    ARM_2D_IMPL(arm_2d_op_drw_pt_t, ptTask->ptOP)
    
    ARM_2D_UNUSED(ptSize);
    ARM_2D_UNUSED(iStride);

    switch (OP_CORE.ptOp->Info.Colour.u3ColourSZ) {
        case ARM_2D_COLOUR_SZ_16BIT:
            (*(uint16_t *)pTarget) = this.hwColour;
            break;
        case ARM_2D_COLOUR_SZ_32BIT:
            (*(uint32_t *)pTarget) = this.wColour;
            break;
        default:
            return (arm_fsm_rt_t)ARM_2D_ERR_NOT_SUPPORT;
    }

    return arm_fsm_rt_cpl;
}


/*----------------------------------------------------------------------------*
 * Fill tile with specified colour                                            *
 *----------------------------------------------------------------------------*/
extern
ARM_NONNULL(1)
arm_fsm_rt_t arm_2d_rgb16_fill_colour(  const arm_2d_tile_t *ptTarget,
                                        const arm_2d_region_t *ptRegion,
                                        uint_fast16_t hwColour)
{
    assert(NULL != ptTarget);

    ARM_2D_IMPL(arm_2d_op_fill_cl_t);
    memset(ptThis, 0, sizeof(*ptThis));
    
    OP_CORE.ptOp = &ARM_2D_OP_TABLE[ARM_2D_OP_COLOUR_FILL_RGB16];

    this.Target.ptTile = ptTarget;
    this.Target.ptRegion = ptRegion;
    this.hwColour = hwColour;

    return __arm_2d_op_invoke(NULL);
}
                                       
extern
ARM_NONNULL(1)
arm_fsm_rt_t arm_2d_rgb32_fill_colour(  const arm_2d_tile_t *ptTarget,
                                        const arm_2d_region_t *ptRegion,
                                        uint32_t wColour)
{
    assert(NULL != ptTarget);

    ARM_2D_IMPL(arm_2d_op_fill_cl_t);
    memset(ptThis, 0, sizeof(*ptThis));
    
    OP_CORE.ptOp = &ARM_2D_OP_TABLE[ARM_2D_OP_COLOUR_FILL_RGB32];

    this.Target.ptTile = ptTarget;
    this.Target.ptRegion = ptRegion;
    this.wColour = wColour;

    return __arm_2d_op_invoke(NULL);
}



arm_fsm_rt_t __arm_2d_sw_colour_filling(__arm_2d_sub_task_t *ptTask,
                                        void *__RESTRICT pTarget,
                                        int16_t iStride,
                                        arm_2d_size_t *__RESTRICT ptSize)
{
    ARM_2D_IMPL(arm_2d_op_fill_cl_t, ptTask->ptOP)

    switch (OP_CORE.ptOp->Info.Colour.u3ColourSZ) {
        case ARM_2D_COLOUR_SZ_16BIT:
            __arm_2d_impl_rgb16_colour_filling( pTarget,
                                                iStride,
                                                ptSize,
                                                this.hwColour);
            break;
        case ARM_2D_COLOUR_SZ_32BIT:
            __arm_2d_impl_rgb32_colour_filling( pTarget,
                                                iStride,
                                                ptSize,
                                                this.wColour);
            break;
        default:
            return (arm_fsm_rt_t)ARM_2D_ERR_NOT_SUPPORT;
    }

    return arm_fsm_rt_cpl;
}

/*----------------------------------------------------------------------------*
 * Accelerable Low Level APIs                                                 *
 *----------------------------------------------------------------------------*/


__WEAK
void __arm_2d_impl_rgb16_colour_filling(uint16_t *__RESTRICT phwTarget,
                                        int16_t iTargetStride,
                                        arm_2d_size_t *__RESTRICT ptCopySize,
                                        uint16_t hwColour)
{
    for (int_fast16_t y = 0; y < ptCopySize->iHeight; y++) {
        for (int_fast16_t x = 0; x < ptCopySize->iWidth; x++){
            phwTarget[x] = hwColour;
        }
        phwTarget += iTargetStride;
    }
}


__WEAK
void __arm_2d_impl_rgb32_colour_filling(uint32_t *__RESTRICT pwTarget,
                                        int16_t iTargetStride,
                                        arm_2d_size_t *__RESTRICT ptCopySize,
                                        uint32_t wColour)
{
    for (int_fast16_t y = 0; y < ptCopySize->iHeight; y++) {
        for (int_fast16_t x = 0; x < ptCopySize->iWidth; x++){
            pwTarget[x] = wColour;
        }
        pwTarget += iTargetStride;
    }
}


#if defined(__clang__)
#   pragma clang diagnostic pop
#endif

#ifdef   __cplusplus
}
#endif
