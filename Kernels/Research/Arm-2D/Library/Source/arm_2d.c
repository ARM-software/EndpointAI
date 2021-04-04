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
 * Title:        arm-2d.c
 * Description:  Basic Tile operations
 *
 * $Date:        01. December 2020
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
#ifndef __ARM_2D_DEFAULT_SUB_TASK_POOL_SIZE
#   define __ARM_2D_DEFAULT_SUB_TASK_POOL_SIZE        3
#endif

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/

__WEAK
arm_2d_runtime_feature_t ARM_2D_RUNTIME_FEATURE = {
    .TREAT_OUT_OF_RANGE_AS_COMPLETE         = 1,
    .HAS_DEDICATED_THREAD_FOR_2D_TASK       = 0,
};
/*============================ PROTOTYPES ====================================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ IMPLEMENTATION ================================*/

ARM_NOINIT
struct __arm_2d_op_control ARM_2D_CTRL;

#define __arm_2d_get_address_and_region_from_tile(  __TILE_PTR,                 \
                                                    __VALID_REGION_NAME,        \
                                                    __LOCATION_OFFSET_PTR,      \
                                                    __TYPE,                     \
                                                    __BUF_PTR_NAME)             \
    arm_2d_region_t __VALID_REGION_NAME;                                        \
    assert(NULL != (__TILE_PTR));                                               \
    __TYPE *(__BUF_PTR_NAME) = NULL;                                            \
                                                                                \
    (__TILE_PTR) = arm_2d_tile_get_root((__TILE_PTR),                           \
                                        &__VALID_REGION_NAME,                   \
                                         __LOCATION_OFFSET_PTR);                \
                                                                                \
    if (NULL != (__TILE_PTR)) {                                                 \
        (__BUF_PTR_NAME) = ((__TYPE *)((__TILE_PTR)->pchBuffer))                \
                        + (__TILE_PTR)->tRegion.tSize.iWidth *                  \
                                __VALID_REGION_NAME.tLocation.iY                \
                        + __VALID_REGION_NAME.tLocation.iX;                     \
    }


/*----------------------------------------------------------------------------*
 * Region Calculation                                                         *
 *----------------------------------------------------------------------------*/

__WEAK
arm_fsm_rt_t __arm_2d_issue_sub_task_tile_process(  
                                        arm_2d_op_t *ptThis,
                                        void *__RESTRICT pTargetBase,
                                        int16_t iTargetStride,
                                        arm_2d_size_t *__RESTRICT ptTargetSize)
{
    arm_fsm_rt_t tResult = (arm_fsm_rt_t)ARM_2D_ERR_NOT_SUPPORT;
    __arm_2d_sub_task_t *ptTask = &(__arm_2d_sub_task_t){
        .ptOP = (arm_2d_op_core_t *)ptThis,
    };

    /* call default software implementation */
    ARM_2D_RUN_DEFAULT(   
                    OP_CORE.ptOp->Info.LowLevelInterfaceIndex.TileProcessLike, 
                    __arm_2d_io_tile_default_tile_process_t,                
                    pTargetBase,                               
                    iTargetStride,         
                    ptTargetSize);
    return tResult;
}


__WEAK
arm_fsm_rt_t __arm_2d_issue_sub_task_fill(arm_2d_op_cp_t *ptThis,
                                    void *__RESTRICT pSourceBase,
                                    int16_t iSourceStride,
                                    arm_2d_region_t *__RESTRICT ptSourceRegion,
                                    void *__RESTRICT pTargetBase,
                                    int16_t iTargetStride,
                                    arm_2d_region_t *__RESTRICT ptTargetRegion)
{
    arm_fsm_rt_t tResult = (arm_fsm_rt_t)ARM_2D_ERR_NOT_SUPPORT;
    __arm_2d_sub_task_t *ptTask = &(__arm_2d_sub_task_t){
        .ptOP = (arm_2d_op_core_t *)ptThis,
        .Param.tFill = {
            .pSource = pSourceBase,                               
            .iSourceStride = iSourceStride,         
            .tSourceRegion = *ptSourceRegion,                 
            .pTarget = pTargetBase,                               
            .iTargetStride = iTargetStride,         
            .tTargetRegion = *ptTargetRegion,
        },
    };

    /* call default software implementation */
    ARM_2D_RUN_DEFAULT( OP_CORE.ptOp->Info.LowLevelInterfaceIndex.FillLike, 
                          __arm_2d_io_func_t );
    return tResult;
}

__WEAK
arm_fsm_rt_t __arm_2d_issue_sub_task_copy(arm_2d_op_cp_t *ptThis,
                                    void *__RESTRICT pSource,
                                    int16_t iSourceStride,
                                    arm_2d_region_t *__RESTRICT ptSourceRegion,
                                    void *__RESTRICT pTarget,
                                    int16_t iTargetStride,
                                    arm_2d_region_t *__RESTRICT ptTargetRegion,
                                    arm_2d_size_t * __RESTRICT ptCopySize)
{
    arm_fsm_rt_t tResult = (arm_fsm_rt_t)ARM_2D_ERR_NOT_SUPPORT;
    __arm_2d_sub_task_t *ptTask = &(__arm_2d_sub_task_t){
        .ptOP = (arm_2d_op_core_t *)ptThis,
        .Param.tCopy = {
            .pSource = pSource,                               
            .iSourceStride = iSourceStride,         
            .tSourceRegion = *ptSourceRegion,                 
            .pTarget = pTarget,                               
            .iTargetStride = iTargetStride,         
            .tTargetRegion = *ptTargetRegion,
            .tCopySize = *ptCopySize,
        },
    };
    
    /* call default software implementation */
    ARM_2D_RUN_DEFAULT(   OP_CORE.ptOp->Info.LowLevelInterfaceIndex.CopyLike, 
                            __arm_2d_io_func_t );
    
    return tResult;
}



ARM_NONNULL(1,2)
static
arm_fsm_rt_t __arm_2d_tile_process( arm_2d_op_t *ptThis,
                                    const arm_2d_tile_t *ptTarget)
{
    assert(NULL != ptTarget);
    assert(OP_CORE.ptOp->Info.Colour.u3ColourSZ >= ARM_2D_COLOUR_SZ_8BIT);
    
    arm_fsm_rt_t tResult = (arm_fsm_rt_t)ARM_2D_ERR_NOT_SUPPORT;
    uint_fast8_t chPixelLen = _BV(  OP_CORE.ptOp->Info.Colour.u3ColourSZ 
                                 -  ARM_2D_COLOUR_SZ_8BIT);
    
    /* process target region, tile and buffer address */
    arm_2d_region_t tTargetRegion;                                         
    uint8_t *pchTarget = NULL;                                            
                                                                                
    ptTarget = arm_2d_tile_get_root(ptTarget, &tTargetRegion, NULL);                
                                                                                
    if (NULL != ptTarget) {                                                 
        pchTarget = ptTarget->pchBuffer               
                    + ( ptTarget->tRegion.tSize.iWidth 
                      * tTargetRegion.tLocation.iY                
                      + tTargetRegion.tLocation.iX) * chPixelLen;                     
    }


    if (NULL == ptTarget) {
        return (arm_fsm_rt_t)ARM_2D_ERR_OUT_OF_REGION;
    }


    tResult = __arm_2d_issue_sub_task_tile_process( 
                                                ptThis,                
                                                pchTarget,                               
                                                ptTarget->tRegion.tSize.iWidth,         
                                                &(tTargetRegion.tSize));

    return tResult;
}


ARM_NONNULL(1,2)
static
arm_fsm_rt_t __arm_2d_big_pixel_tile_pave(  arm_2d_op_cp_t *ptThis,
                                            const arm_2d_tile_t *ptSource,
                                            const arm_2d_tile_t *ptTarget,
                                            uint32_t wMode)
{
    assert(NULL != ptSource);
    assert(NULL != ptTarget);
    assert(OP_CORE.ptOp->Info.Colour.u3ColourSZ >= ARM_2D_COLOUR_SZ_8BIT);
    
    arm_fsm_rt_t tResult = (arm_fsm_rt_t)ARM_2D_ERR_NOT_SUPPORT;
    uint_fast8_t chSourcePixelLen = _BV(  OP_CORE.ptOp->Info.Colour.u3ColourSZ 
                                 -  ARM_2D_COLOUR_SZ_8BIT);
    uint_fast8_t chTargetPixelLen = chSourcePixelLen;
    
    //! check if enforced colour is allowed
    if (OP_CORE.ptOp->Info.Param.bAllowEnforcedColour) {
        if (ptSource->tInfo.bHasEnforcedColour) {
            chSourcePixelLen = _BV(  ptSource->tInfo.tColourInfo.u3ColourSZ 
                                    -  ARM_2D_COLOUR_SZ_8BIT);
        }
    }
    
    /* process source region, tile and buffer address */
    arm_2d_region_t tSourceRegion;                                                     
    
    uint8_t *pchSource = NULL;                                            
                                                                                
    ptSource = arm_2d_tile_get_root(ptSource,                           
                                    &tSourceRegion,                   
                                    NULL);                
                                                                                
    if (NULL != (ptSource)) {                                                 
        arm_2d_location_t tOffset = tSourceRegion.tLocation;              
        if ((wMode) & ARM_2D_CP_MODE_X_MIRROR) {                             
            tOffset.iX = 0;                                                     
        }                                                                       
        if ((wMode) & ARM_2D_CP_MODE_Y_MIRROR) {                             
            tOffset.iY = 0;                                                     
        }                                                                       
        pchSource = ptSource->pchBuffer               
                        + ( ptSource->tRegion.tSize.iWidth * tOffset.iY   
                          + tOffset.iX) * chSourcePixelLen;                                       
    }


    //! check if enforced colour is allowed
    if (OP_CORE.ptOp->Info.Param.bAllowEnforcedColour) {
        if (ptTarget->tInfo.bHasEnforcedColour) {
            chTargetPixelLen = _BV(  ptTarget->tInfo.tColourInfo.u3ColourSZ 
                                    -  ARM_2D_COLOUR_SZ_8BIT);
        }
    }

    /* process target region, tile and buffer address */
    arm_2d_region_t tTargetRegion;                                         
    uint8_t *pchTarget = NULL;                                            
                                                                                
    ptTarget = arm_2d_tile_get_root(ptTarget, &tTargetRegion, NULL);                
                                                                                
    if (NULL != ptTarget) {                                                 
        pchTarget = ptTarget->pchBuffer               
                    + ( ptTarget->tRegion.tSize.iWidth 
                      * tTargetRegion.tLocation.iY                
                      + tTargetRegion.tLocation.iX) * chTargetPixelLen;                     
    }


    if (NULL == ptSource || NULL == ptTarget) {
        return (arm_fsm_rt_t)ARM_2D_ERR_OUT_OF_REGION;
    }

    if (wMode & ARM_2D_CP_MODE_FILL) {
        tResult = __arm_2d_issue_sub_task_fill( ptThis, 
                                                pchSource,                               
                                                ptSource->tRegion.tSize.iWidth,         
                                                &(tSourceRegion),                 
                                                pchTarget,                               
                                                ptTarget->tRegion.tSize.iWidth,         
                                                &(tTargetRegion));
    
    } else {
        arm_2d_size_t tActualSize = {
            .iWidth = MIN(  tSourceRegion.tSize.iWidth, 
                            tTargetRegion.tSize.iWidth),
            .iHeight = MIN( tSourceRegion.tSize.iHeight, 
                            tTargetRegion.tSize.iHeight),
        };
        
        //! right and/or bottom alignment
        do {
            arm_2d_size_t tOffset = {0};
            
            //! right alignment 
            if (wMode & ARM_2D_CP_MODE_X_MIRROR) {
                tOffset.iWidth  = tSourceRegion.tSize.iWidth 
                                - tActualSize.iWidth;
            }
            
            //! bottom alignment 
            if (wMode & ARM_2D_CP_MODE_Y_MIRROR) {
                tOffset.iHeight = tSourceRegion.tSize.iHeight 
                                - tActualSize.iHeight;
            }
            
            pchSource +=(   tOffset.iHeight * ptSource->tRegion.tSize.iWidth 
                        +   tOffset.iWidth) * chSourcePixelLen;
        } while(0);
    
        tResult = __arm_2d_issue_sub_task_copy( ptThis, 
                                                pchSource,                               
                                                ptSource->tRegion.tSize.iWidth,         
                                                &(tSourceRegion),                 
                                                pchTarget,                               
                                                ptTarget->tRegion.tSize.iWidth,         
                                                &(tTargetRegion),
                                                &tActualSize);
    }

    return tResult;
}


static
arm_fsm_rt_t __tile_clipped_pave(
                            arm_2d_op_cp_t *ptThis,
                            const arm_2d_region_t *ptRegion,
                            arm_2d_region_t *ptClippedRegion,
                            uint32_t wMode)
{
    arm_2d_region_t tempRegion = this.Source.ptTile->tRegion;
    arm_2d_tile_t tClippedTile = {0};
    arm_2d_tile_t tTargetTile = {0};
    tempRegion.tLocation.iX = -ptRegion->tLocation.iX;
    tempRegion.tLocation.iY = -ptRegion->tLocation.iY;
    //tempRegion.tSize.iWidth += ptRegion->tLocation.iX > 0 ? ptRegion->tLocation.iX : 0;
    //tempRegion.tSize.iHeight += ptRegion->tLocation.iY > 0 ? ptRegion->tLocation.iY : 0;
    tempRegion.tSize.iWidth += ptRegion->tLocation.iX;
    tempRegion.tSize.iHeight += ptRegion->tLocation.iY;

    arm_fsm_rt_t tResult = (arm_fsm_rt_t)ARM_2D_ERR_OUT_OF_REGION;
    do {
        if (NULL == arm_2d_tile_generate_child( this.Target.ptTile, 
                                                ptRegion, 
                                                &tTargetTile, 
                                                true)) {
            break;
        }

        if (NULL == arm_2d_tile_generate_child( this.Source.ptTile, 
                                                &tempRegion, 
                                                &tClippedTile, 
                                                true)) {
            break;
        }
        
        if (NULL != ptClippedRegion) {
            *ptClippedRegion = tClippedTile.tRegion;
        }
    

    #if 0
        switch (OP_CORE.ptOp->Info.Colour.u3ColourSZ) {
            case ARM_2D_COLOUR_SZ_8BIT:
            case ARM_2D_COLOUR_SZ_16BIT:
            case ARM_2D_COLOUR_SZ_32BIT:
                tResult = __arm_2d_big_pixel_tile_pave( ptThis, 
                                                    &tClippedTile, 
                                                    &tTargetTile, 
                                                    wMode);
                break;
            default:
                tResult = (arm_fsm_rt_t)ARM_2D_ERR_NOT_SUPPORT;
                break;
        }
    #else
        if (OP_CORE.ptOp->Info.Colour.u3ColourSZ >= ARM_2D_COLOUR_SZ_8BIT) {
            tResult = __arm_2d_big_pixel_tile_pave( ptThis, 
                                    &tClippedTile, 
                                    &tTargetTile, 
                                    wMode);
        } else {
            tResult = (arm_fsm_rt_t)ARM_2D_ERR_NOT_SUPPORT;
        }
    #endif
    } while(0);

    return tResult;
}

static arm_fsm_rt_t __tile_non_negtive_location_pave(
                                                arm_2d_op_cp_t *ptThis,
                                                const arm_2d_tile_t *ptSource,
                                                const arm_2d_tile_t *ptTarget,
                                                const arm_2d_region_t *ptRegion,
                                                uint32_t wMode)
{
    arm_fsm_rt_t tResult = (arm_fsm_rt_t)ARM_2D_ERR_OUT_OF_REGION;
    do {
        arm_2d_tile_t tTile = {0};
        if (NULL == arm_2d_tile_generate_child(ptTarget, ptRegion, &tTile, true)) {
            break;
        }
    #if 0
        switch (OP_CORE.ptOp->Info.Colour.u3ColourSZ) {
            case ARM_2D_COLOUR_SZ_8BIT:
            case ARM_2D_COLOUR_SZ_16BIT:
            case ARM_2D_COLOUR_SZ_32BIT:
                tResult = __arm_2d_big_pixel_tile_pave(ptThis, ptSource, &tTile, wMode);
                break;
            default:
                tResult = (arm_fsm_rt_t)ARM_2D_ERR_NOT_SUPPORT;
                break;
        }
    #else
        if (OP_CORE.ptOp->Info.Colour.u3ColourSZ >= ARM_2D_COLOUR_SZ_8BIT) {
            tResult = __arm_2d_big_pixel_tile_pave( ptThis, 
                                                    ptSource, 
                                                    &tTile, 
                                                    wMode);
        } else {
            tResult = (arm_fsm_rt_t)ARM_2D_ERR_NOT_SUPPORT;
        }
    #endif
    } while(0);

    return tResult;
}


__WEAK 
bool __arm_2d_op_ensure_resource(   arm_2d_op_core_t *ptOP, 
                                    uint_fast16_t hwRequired)
{
    ARM_2D_UNUSED(hwRequired);
    ARM_2D_UNUSED(ptOP);
    return true;
}

static
arm_fsm_rt_t __arm_2d_op_frontend_region_process( arm_2d_op_core_t *ptOP)
{
   ARM_2D_IMPL(arm_2d_op_t, ptOP)

    arm_fsm_rt_t tResult = (arm_fsm_rt_t)ARM_2D_ERR_OUT_OF_REGION;;
    arm_2d_tile_t tTile = {0};
    arm_2d_region_t tDrawRegion = {0};
    arm_2d_region_t tTargetRegion = {0};

    if (!__arm_2d_op_ensure_resource(ptOP, 1)) {
        //! insufficient resources, ask users to try again
        return arm_fsm_rt_async;
    }
    
    
    if (NULL == this.Target.ptRegion) {
        //! use target region instead
        this.Target.ptRegion = &(this.Target.ptTile->tRegion);
        
        tTargetRegion.tSize = (*this.Target.ptRegion).tSize;
    } else {

        tTargetRegion = *this.Target.ptRegion;
    }

    //! handle the offset of the target tile
    do {
        //arm_2d_tile_t tValidTargetTile;
        arm_2d_region_t tValidRegion;
        arm_2d_location_t tOffset;
        if (NULL == arm_2d_tile_get_root(this.Target.ptTile, &tValidRegion, &tOffset)) {
            //! nothing to draw, actually
            return (arm_fsm_rt_t)ARM_2D_ERR_OUT_OF_REGION;
        }

        //! calculate the valid region in the view of the target tile
        tValidRegion.tLocation = tOffset;
        //if (tOffset.iX != 0 || tOffset.iY != 0) {
            this.Target.ptTile = arm_2d_tile_generate_child(  this.Target.ptTile,
                                                    &tValidRegion,
                                                    &tTile,
                                                    true);
            assert(NULL != this.Target.ptTile);
            tTargetRegion.tLocation.iX -= tOffset.iX;
            tTargetRegion.tLocation.iY -= tOffset.iY;
        //}

    } while(false);

    tDrawRegion.tSize = this.Target.ptTile->tRegion.tSize;
    if (!arm_2d_region_intersect(   &tDrawRegion, 
                                    &tTargetRegion, 
                                    &tDrawRegion)) {
        //! no overlapping
        if (ARM_2D_RUNTIME_FEATURE.TREAT_OUT_OF_RANGE_AS_COMPLETE) {
            //! nothing to draw
            return arm_fsm_rt_cpl;
        } 
        return (arm_fsm_rt_t)ARM_2D_ERR_OUT_OF_REGION;
    }


    //! drawing
    do {
        arm_2d_tile_t tTempTile = {0};
        if (NULL == arm_2d_tile_generate_child( this.Target.ptTile, 
                                                &tTargetRegion, 
                                                &tTempTile, true)) {
            break;
        }
    #if 0
        switch (OP_CORE.ptOp->Info.Colour.u3ColourSZ) {
            case ARM_2D_COLOUR_SZ_8BIT:
            case ARM_2D_COLOUR_SZ_16BIT:
            case ARM_2D_COLOUR_SZ_32BIT:
                tResult = __arm_2d_big_pixel_tile_pave(ptThis, ptSource, &tTile, wMode);
                break;
            default:
                tResult = (arm_fsm_rt_t)ARM_2D_ERR_NOT_SUPPORT;
                break;
        }
    #else
        if (OP_CORE.ptOp->Info.Colour.u3ColourSZ >= ARM_2D_COLOUR_SZ_8BIT) {
            tResult = __arm_2d_tile_process( ptThis,
                                            &tTempTile);
        } else {
            tResult = (arm_fsm_rt_t)ARM_2D_ERR_NOT_SUPPORT;
        }
    #endif
    } while(0);


                                            
    if (ARM_2D_ERR_OUT_OF_REGION == tResult) {
                                        
        if (ARM_2D_RUNTIME_FEATURE.TREAT_OUT_OF_RANGE_AS_COMPLETE) {
            //! nothing to draw
            return arm_fsm_rt_cpl;
        } 
        
        return tResult;
        
    } else if (tResult < 0) {
        return tResult;
    }

    return tResult;
}

static
arm_fsm_rt_t __arm_2d_op_frontend_control( arm_2d_op_core_t *ptOP)
{
    /*TODO: add support for control operations*/
    return (arm_fsm_rt_t)ARM_2D_ERR_NOT_SUPPORT;
}

static
arm_fsm_rt_t __arm_2d_op_frontend_region_process_with_src( arm_2d_op_core_t *ptOP)
{
    ARM_2D_IMPL(arm_2d_op_src_t, ptOP)

    arm_fsm_rt_t tResult;
    arm_2d_tile_t tTile = {0};
    arm_2d_region_t tDrawRegion = {0};
    arm_2d_region_t tTargetRegion = {0};

    if (!__arm_2d_op_ensure_resource(ptOP, 4)) {
        //! insufficient resources, ask users to try again
        return arm_fsm_rt_async;
    }
    
    
    if (NULL == this.Target.ptRegion) {
        //! use target region instead
        this.Target.ptRegion = &(this.Target.ptTile->tRegion);
        
        tTargetRegion.tSize = (*this.Target.ptRegion).tSize;
    } else {

        tTargetRegion = *this.Target.ptRegion;
    }

    //! handle the offset of the target tile
    do {
        //arm_2d_tile_t tValidTargetTile;
        
        arm_2d_region_t tValidRegion;
        arm_2d_location_t tOffset;
        if (NULL == arm_2d_tile_get_root(this.Target.ptTile, &tValidRegion, &tOffset)) {
            //! nothing to draw, actually
            return (arm_fsm_rt_t)ARM_2D_ERR_OUT_OF_REGION;
        }

        //! calculate the valid region in the view of the target tile
        tValidRegion.tLocation = tOffset;
        //if (tOffset.iX != 0 || tOffset.iY != 0) {
            this.Target.ptTile = arm_2d_tile_generate_child(  this.Target.ptTile,
                                                    &tValidRegion,
                                                    &tTile,
                                                    true);
            assert(NULL != this.Target.ptTile);
            tTargetRegion.tLocation.iX -= tOffset.iX;
            tTargetRegion.tLocation.iY -= tOffset.iY;
        //}

    } while(false);



    if ( this.wMode & ARM_2D_CP_MODE_FILL) {
        /* quickly ignore non visiable area, only for FILL mode */
        if (tTargetRegion.tLocation.iX < 0) {
            int_fast16_t iX = tTargetRegion.tLocation.iX;
            tTargetRegion.tLocation.iX %= this.Source.ptTile->tRegion.tSize.iWidth;

            //! calculate the delta
            iX = tTargetRegion.tLocation.iX - iX;

            //! update the width to make sure the range won't be expanded mistakenly
            tTargetRegion.tSize.iWidth -= iX;
        }

        if (tTargetRegion.tLocation.iY < 0) {
            int_fast16_t iY = tTargetRegion.tLocation.iY;
            tTargetRegion.tLocation.iY %= this.Source.ptTile->tRegion.tSize.iHeight;

            //! calculate the delta
            iY = tTargetRegion.tLocation.iY - iY;

            //! update the width to make sure the range won't be expanded mistakenly
            tTargetRegion.tSize.iHeight -= iY;
        }
    }

    tDrawRegion.tSize = this.Target.ptTile->tRegion.tSize;
    if (!arm_2d_region_intersect(   &tDrawRegion, 
                                    &tTargetRegion, 
                                    &tDrawRegion)) {
        //! no overlapping
        if (ARM_2D_RUNTIME_FEATURE.TREAT_OUT_OF_RANGE_AS_COMPLETE) {
            //! nothing to draw
            return arm_fsm_rt_cpl;
        } 
        return (arm_fsm_rt_t)ARM_2D_ERR_OUT_OF_REGION;
    }

    if (    (tTargetRegion.tLocation.iX < 0) 
        ||  (tTargetRegion.tLocation.iY < 0)) {
        /*! draw the top left corner */
        /*
          HOW IT WORKS:

           source tile (with negative relative location)
          +------------------------------------+
          |                                    |
          |     +------------------------------+-- ... --+
          |     |//////////////////////////////|         |
          |     |//ACTUAL DRAWING PART/////////|         |
          |     |//////////////////////////////|         |
          +-----+------------------------------+-- ... --|
                |                              |         |
               ...                            ...       ...
                |                              |         |
                +------------------------------+-- ... --+
         */
        arm_2d_region_t tClippdRegion;
        tResult = __tile_clipped_pave(  &this,
                                        &tTargetRegion,
                                        &tClippdRegion,
                                        this.wMode & ~ARM_2D_CP_MODE_FILL);
        //arm_2d_region_t tClippedRegion;
        if (ARM_2D_ERR_OUT_OF_REGION == tResult) {
                                            
            if (ARM_2D_RUNTIME_FEATURE.TREAT_OUT_OF_RANGE_AS_COMPLETE) {
                //! nothing to draw
                return arm_fsm_rt_cpl;
            } 
            
            return tResult;
            
        } else if (tResult < 0) {
            return tResult;
        }

        if (this.wMode & ARM_2D_CP_MODE_FILL) {                               /*!< fill mode */
            /*! draw the partial header */
            /*
            HOW IT WORKS:

            source tile (with negative relative location)
            +------------------+------------------------------------+-- ... --+
            |                  |    IGNORED HEADER PART             | repeat  |
            |  +---------------+------------------------------------+-- ... --+
            |  |               |////////////////////////////////////|/////////|
            |  |Clipped Region |////ACTUAL DRAWING PART/////////////|/repeat//|
            |  |               |////////////////////////////////////|/////////|
            +--+---------------+------------------------------------+-- ... --+
            |                                                                 |
            |                                                                 |
            +---------------------------------------------------------- ... --+

            */



            if  (tClippdRegion.tSize.iWidth < tDrawRegion.tSize.iWidth) {
                //! something left to draw

                arm_2d_region_t tHeaderRegion = tDrawRegion;
                tHeaderRegion.tSize.iWidth  = tDrawRegion.tSize.iWidth 
                                            - tClippdRegion.tSize.iWidth;
                tHeaderRegion.tLocation.iX += tClippdRegion.tSize.iWidth;
                tHeaderRegion.tSize.iHeight = tClippdRegion.tSize.iHeight 
                                            - tTargetRegion.tLocation.iY;
                tHeaderRegion.tLocation.iY = tTargetRegion.tLocation.iY;

                tResult = __tile_clipped_pave(  &this,
                                                &tHeaderRegion,
                                                NULL,
                                                this.wMode);
                                    
                //arm_2d_region_t tClippedRegion;
                if (tResult < 0) {
                    return tResult;
                }
            }



            /*! draw the partial column */
            /*
            HOW IT WORKS:

            source tile (with negative relative location)
            +------------------------------------+
            |                                    |
            |     +------------------------------+-- ... --+
            |     |                              |         |
            |     |                              |         |
            |     |                              |         |
            +-----+------------------------------+         |
            |     |//////////////////////////////|         |
            |     |//////////////////////////////|         |
            |     |////ACTUAL DRAWING COLUMN/////|         |
            |     |//////////////////////////////|         |
            |     |//////////////////////////////|         |
            +-----+------------------------------+         |
            |     |//////////////////////////////|         |
           ...   ...///repeat////////////////////|         |
            |     |//////////////////////////////|         |
            +-----+------------------------------+-- ... --+
            */


            if  (tClippdRegion.tSize.iHeight < tDrawRegion.tSize.iHeight) {
                //! something left to draw

                arm_2d_region_t tFirstColumnRegion = tDrawRegion;
                tFirstColumnRegion.tSize.iHeight = tDrawRegion.tSize.iHeight - tClippdRegion.tSize.iHeight;
                tFirstColumnRegion.tLocation.iY += tClippdRegion.tSize.iHeight;
                tFirstColumnRegion.tSize.iWidth = tClippdRegion.tSize.iWidth 
                                                - tTargetRegion.tLocation.iX;
                tFirstColumnRegion.tLocation.iX = tTargetRegion.tLocation.iX;

                tResult = __tile_clipped_pave(  &this,
                                                &tFirstColumnRegion,
                                                NULL,
                                                this.wMode);
                                    
                if (tResult < 0) {
                    return tResult;
                }
            }


            /*! draw the normal non-negitive part */
            /*
            HOW IT WORKS:

            source tile (with negative relative location)
            +------------------------------------+
            |                                    |
            |     +------------------------------+-- ... --+
            |     |                              |         |
            |     |                              |         |
            |     |                              |         |
            +-----+------------------------------+-- ... --|
                  |                              |/////////|
                  |                              |/////////|
                 ...                            ...///////...
                  |                              |/////////|
                  |                              |/////////|
                  +------------------------------+-- ... --+
            */

            if (    (tClippdRegion.tSize.iWidth < tDrawRegion.tSize.iWidth)
                &&  (tClippdRegion.tSize.iHeight < tDrawRegion.tSize.iHeight)) {

                arm_2d_region_t tNonNegRegion = tDrawRegion;
                tNonNegRegion.tSize.iWidth = tDrawRegion.tSize.iWidth - tClippdRegion.tSize.iWidth;
                tNonNegRegion.tSize.iHeight = tDrawRegion.tSize.iHeight - tClippdRegion.tSize.iHeight;
                tNonNegRegion.tLocation.iX += tClippdRegion.tSize.iWidth;
                tNonNegRegion.tLocation.iY += tClippdRegion.tSize.iHeight;

                tResult = __tile_non_negtive_location_pave( &this,
                                                            this.Source.ptTile,
                                                            this.Target.ptTile,
                                                            &tNonNegRegion,
                                                            this.wMode);
                if (tResult < 0) {
                    return tResult;
                }
            }

        }
    } else {
        tResult = __tile_non_negtive_location_pave( &this,
                                                    this.Source.ptTile,
                                                    this.Target.ptTile,
                                                    &tTargetRegion,
                                                    this.wMode);
                                            
        if (ARM_2D_ERR_OUT_OF_REGION == tResult) {
                                            
            if (ARM_2D_RUNTIME_FEATURE.TREAT_OUT_OF_RANGE_AS_COMPLETE) {
                //! nothing to draw
                return arm_fsm_rt_cpl;
            } 
            
            return tResult;
            
        } else if (tResult < 0) {
            return tResult;
        }
    }

    return tResult;
}


/*----------------------------------------------------------------------------*
 * Frontend                                                                   *
 *----------------------------------------------------------------------------*/

__WEAK
arm_fsm_rt_t __arm_2d_op_frontend_on_leave( arm_2d_op_core_t *ptThis, 
                                            arm_fsm_rt_t tResult)
{
    this.Status.bIsBusy = 0;
    this.Status.bOpCpl = true;
    this.Status.bIOError = (tResult < 0);
    this.tResult = tResult;
    
    return tResult;
}
 
arm_fsm_rt_t __arm_2d_op_invoke(arm_2d_op_core_t *ptOP)
{
    ARM_2D_IMPL(arm_2d_op_core_t, ptOP)
    
    arm_fsm_rt_t tResult;
    if (this.Status.bIsBusy) {
        return arm_fsm_rt_on_going;
    }

    //! initialize operation
    do {
        this.tResult = arm_fsm_rt_async;
        this.Status.tValue = 0;                                                 //! reset status
        this.Status.bIsBusy = true;                                             //! set busy flag
    } while(0);
    
    //! decode operation
    switch (this.ptOp->Info.Param.chValue & 
                (   ARM_2D_OP_INFO_PARAM_HAS_SOURCE 
                |   ARM_2D_OP_INFO_PARAM_HAS_TARGET
                |   ARM_2D_OP_INFO_PARAM_HAS_ALPHA_MASK)) {
                
    case (   ARM_2D_OP_INFO_PARAM_HAS_SOURCE 
         |   ARM_2D_OP_INFO_PARAM_HAS_TARGET):
        /* operaion process target tile based on source tile */
        tResult = __arm_2d_op_frontend_region_process_with_src(ptThis);
        break;
        
    case ARM_2D_OP_INFO_PARAM_HAS_TARGET:
        /* operation which only processes target tile */
        tResult = __arm_2d_op_frontend_region_process(ptThis);
        break;
        
    default:
        /* control operation */
        tResult = __arm_2d_op_frontend_control(ptThis);
    }
    
    return  __arm_2d_op_frontend_on_leave(ptThis, tResult);
}


/*! \brief initialise the whole arm-2d service
 *! \param none
 *! \return none
 */

void __arm_2d_init(void)
{
    /*! initialise arm-2d control block */
    memset(&ARM_2D_CTRL, 0, sizeof(struct __arm_2d_op_control));
}


__WEAK
arm_fsm_rt_t arm_2d_task(void)
{
    return arm_fsm_rt_cpl;
}


#if defined(__clang__)
#   pragma clang diagnostic pop
#endif

#ifdef   __cplusplus
}
#endif
