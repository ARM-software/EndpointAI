/*
 * Copyright (C) 2010-2020 Arm Limited or its affiliates. All rights reserved.
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
 * Title:        arm-2d_tile.c
 * Description:  Basic Tile operations
 *
 * $Date:        01. December 2020
 * $Revision:    V.0.9.0
 *
 * Target Processor:  Cortex-M cores
 *
 * -------------------------------------------------------------------- */

#define __ARM_2D_IMPL__

#include "arm_2d.h"
#include "__arm_2d_impl.h"
#include "__arm_2d_paving.h"

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
#endif


/*----------------------------------------------------------------------------*
 * Accelerable Low Level APIs                                                 *
 *----------------------------------------------------------------------------*/


void __arm_2d_impl_rgb16_copy(   uint16_t *__RESTRICT phwSource,
                                int16_t iSourceStride,
                                uint16_t *__RESTRICT phwTarget,
                                int16_t iTargetStride,
                                arm_2d_size_t *__RESTRICT ptCopySize);

void __arm_2d_impl_rgb32_copy(   uint32_t *__RESTRICT pwSource,
                                int16_t iSourceStride,
                                uint32_t *__RESTRICT pwTarget,
                                int16_t iTargetStride,
                                arm_2d_size_t *__RESTRICT ptCopySize);


void __arm_2d_impl_rgb16_copy_colour_masking(   
                                uint16_t *__RESTRICT phwSource,
                                int16_t iSourceStride,
                                uint16_t *__RESTRICT phwTarget,
                                int16_t iTargetStride,
                                arm_2d_size_t *__RESTRICT ptCopySize,
                                uint16_t hwColour);

void __arm_2d_impl_rgb32_copy_colour_masking(   
                                uint32_t *__RESTRICT pwSource,
                                int16_t iSourceStride,
                                uint32_t *__RESTRICT pwTarget,
                                int16_t iTargetStride,
                                arm_2d_size_t *__RESTRICT ptCopySize,
                                uint32_t wColour);
                                

void __arm_2d_impl_rgb16_fill(   uint16_t *__RESTRICT phwSourceBase,
                                int16_t iSourceStride,
                                arm_2d_size_t *__RESTRICT ptSourceSize,
                                uint16_t *__RESTRICT phwTargetBase,
                                int16_t iTargetStride,
                                arm_2d_size_t *__RESTRICT ptTargetSize);


void __arm_2d_impl_rgb32_fill(   uint32_t *__RESTRICT pwSourceBase,
                                int16_t iSourceStride,
                                arm_2d_size_t *__RESTRICT ptSourceSize,
                                uint32_t *__RESTRICT pwTargetBase,
                                int16_t iTargetStride,
                                arm_2d_size_t *__RESTRICT ptTargetSize);

void __arm_2d_impl_rgb16_fill_mirror(   uint16_t *__RESTRICT phwSourceBase,
                                            int16_t iSourceStride,
                                            arm_2d_size_t *__RESTRICT ptSourceSize,
                                            uint16_t *__RESTRICT phwTargetBase,
                                            int16_t iTargetStride,
                                            arm_2d_size_t *__RESTRICT ptTargetSize,
                                            uint32_t wMode);

void __arm_2d_impl_rgb32_fill_mirror(   uint32_t *__RESTRICT pwSourceBase,
                                            int16_t iSourceStride,
                                            arm_2d_size_t *__RESTRICT ptSourceSize,
                                            uint32_t *__RESTRICT pwTargetBase,
                                            int16_t iTargetStride,
                                            arm_2d_size_t *__RESTRICT ptTargetSize,
                                            uint32_t wMode);

void __arm_2d_impl_rgb16_copy_mirror(   uint16_t *__RESTRICT phwSource,
                                            int16_t iSourceStride,
                                            uint16_t *__RESTRICT phwTarget,
                                            int16_t iTargetStride,
                                            arm_2d_size_t *__RESTRICT ptCopySize,
                                            uint32_t wMode);

void __arm_2d_impl_rgb32_copy_mirror(   uint32_t *__RESTRICT pwSource,
                                            int16_t iSourceStride,
                                            uint32_t *__RESTRICT pwTarget,
                                            int16_t iTargetStride,
                                            arm_2d_size_t *__RESTRICT ptCopySize,
                                            uint32_t wMode);

void __arm_2d_impl_rgb16_fill_x_mirror(  uint16_t * __RESTRICT phwSourceBase,
                                        int16_t iSourceStride,
                                        arm_2d_size_t * __RESTRICT ptSourceSize,
                                        uint16_t * __RESTRICT phwTargetBase,
                                        int16_t iTargetStride,
                                        arm_2d_size_t * __RESTRICT ptTargetSize);

void __arm_2d_impl_rgb16_fill_y_mirror(  uint16_t * __RESTRICT phwSourceBase,
                                        int16_t iSourceStride,
                                        arm_2d_size_t * __RESTRICT ptSourceSize,
                                        uint16_t * __RESTRICT phwTargetBase,
                                        int16_t iTargetStride,
                                        arm_2d_size_t * __RESTRICT ptTargetSize);

void __arm_2d_impl_rgb16_fill_xy_mirror( uint16_t * __RESTRICT phwSourceBase,
                                        int16_t iSourceStride,
                                        arm_2d_size_t * __RESTRICT ptSourceSize,
                                        uint16_t * __RESTRICT phwTargetBase,
                                        int16_t iTargetStride,
                                        arm_2d_size_t * __RESTRICT ptTargetSize);
                                       
void __arm_2d_impl_rgb16_copy_x_mirror(  uint16_t *__RESTRICT phwSource,
                                        int16_t iSourceStride,
                                        uint16_t *__RESTRICT phwTarget,
                                        int16_t iTargetStride,
                                        arm_2d_size_t *__RESTRICT ptCopySize);   

void __arm_2d_impl_rgb32_copy_x_mirror(  uint32_t *__RESTRICT pwSource,
                                        int16_t iSourceStride,
                                        uint32_t *__RESTRICT pwTarget,
                                        int16_t iTargetStride,
                                        arm_2d_size_t *__RESTRICT ptCopySize);
                                        
void __arm_2d_impl_rgb16_copy_y_mirror(  uint16_t *__RESTRICT phwSource,
                                        int16_t iSourceStride,
                                        uint16_t *__RESTRICT phwTarget,
                                        int16_t iTargetStride,
                                        arm_2d_size_t *__RESTRICT ptCopySize);
                                        
void __arm_2d_impl_rgb32_copy_y_mirror(  uint32_t *__RESTRICT pwSource,
                                        int16_t iSourceStride,
                                        uint32_t *__RESTRICT pwTarget,
                                        int16_t iTargetStride,
                                        arm_2d_size_t *__RESTRICT ptCopySize);

void __arm_2d_impl_rgb16_copy_xy_mirror( uint16_t *__RESTRICT phwSource,
                                        int16_t iSourceStride,
                                        uint16_t *__RESTRICT phwTarget,
                                        int16_t iTargetStride,
                                        arm_2d_size_t *__RESTRICT ptCopySize);
                                        
void __arm_2d_impl_rgb32_copy_xy_mirror( uint32_t *__RESTRICT pwSource,
                                        int16_t iSourceStride,
                                        uint32_t *__RESTRICT pwTarget,
                                        int16_t iTargetStride,
                                        arm_2d_size_t *__RESTRICT ptCopySize);

void __arm_2d_impl_rgb32_fill_x_mirror(  uint32_t *__RESTRICT pwSourceBase,
                                        int16_t iSourceStride,
                                        arm_2d_size_t *__RESTRICT ptSourceSize,
                                        uint32_t *__RESTRICT pwTargetBase,
                                        int16_t iTargetStride,
                                        arm_2d_size_t *__RESTRICT ptTargetSize);

void __arm_2d_impl_rgb32_fill_y_mirror(  uint32_t *__RESTRICT pwSourceBase,
                                        int16_t iSourceStride,
                                        arm_2d_size_t *__RESTRICT ptSourceSize,
                                        uint32_t *__RESTRICT pwTargetBase,
                                        int16_t iTargetStride,
                                        arm_2d_size_t *__RESTRICT ptTargetSize);

void __arm_2d_impl_rgb32_fill_xy_mirror( uint32_t *__RESTRICT pwSourceBase,
                                        int16_t iSourceStride,
                                        arm_2d_size_t *__RESTRICT ptSourceSize,
                                        uint32_t *__RESTRICT pwTargetBase,
                                        int16_t iTargetStride,
                                        arm_2d_size_t *__RESTRICT ptTargetSize);

void __arm_2d_impl_rgb16_fill_colour_masking(
                                        uint16_t * __RESTRICT pSourceBase,
                                        int16_t iSourceStride,
                                        arm_2d_size_t * __RESTRICT ptSourceSize,
                                        uint16_t * __RESTRICT pTargetBase,
                                        int16_t iTargetStride,
                                        arm_2d_size_t * __RESTRICT ptTargetSize,
                                        uint_fast16_t hwColour);

void __arm_2d_impl_rgb32_fill_colour_masking(
                                        uint32_t * __RESTRICT pSourceBase,
                                        int16_t iSourceStride,
                                        arm_2d_size_t * __RESTRICT ptSourceSize,
                                        uint32_t * __RESTRICT pTargetBase,
                                        int16_t iTargetStride,
                                        arm_2d_size_t * __RESTRICT ptTargetSize,
                                        uint_fast32_t wColour);

                                        
/*----------------------------------------------------------------------------*
 * Tile Operations                                                            *
 *----------------------------------------------------------------------------*/

/*
  HOW IT WORKS:

    Input Region 0
  +------------------------------------------------------+
  |                                                      |
  |                                                      |
  |                                                      |
  |                       +------------------------------+---------+
  |                       |                              |/////////|
  |                       |        Output Region         |/////////|
  |                       |                              |/////////|
  +-----------------------+------------------------------+/////////|
                          |////////////////////////////////////////|
                          |////////////////////////////////////////|
                          +----------------------------------------+
                                                   Input Region 1
 */
ARM_NONNULL(1,2)
bool arm_2d_region_intersect(   const arm_2d_region_t *ptRegionIn0,
                                const arm_2d_region_t *ptRegionIn1,
                                arm_2d_region_t *ptRegionOut)
{
    assert(ptRegionIn0 != NULL);
    assert(ptRegionIn1 != NULL);

    do {
        arm_2d_location_t tLocationIn0End = {
                                .iX = ptRegionIn0->tLocation.iX
                                    + ptRegionIn0->tSize.iWidth
                                    - 1,
                                .iY = ptRegionIn0->tLocation.iY
                                    + ptRegionIn0->tSize.iHeight
                                    - 1,
                            };

        arm_2d_location_t tLocationIn1End = {
                                .iX = ptRegionIn1->tLocation.iX
                                    + ptRegionIn1->tSize.iWidth
                                    - 1,
                                .iY = ptRegionIn1->tLocation.iY
                                    + ptRegionIn1->tSize.iHeight
                                    - 1,
                            };

        arm_2d_location_t tLocationOutStart = {
                                .iX = MAX(  ptRegionIn0->tLocation.iX,
                                            ptRegionIn1->tLocation.iX),

                                .iY = MAX(  ptRegionIn0->tLocation.iY,
                                            ptRegionIn1->tLocation.iY),
                            };

        arm_2d_location_t tLocationOutEnd = {
                                .iX = MIN(  tLocationIn0End.iX,
                                            tLocationIn1End.iX),
                                .iY = MIN(  tLocationIn0End.iY,
                                            tLocationIn1End.iY),
                            };

        if (    (tLocationOutStart.iX > tLocationOutEnd.iX)
            ||  (tLocationOutStart.iY > tLocationOutEnd.iY)) {
            return false;
        }

        if (NULL != ptRegionOut) {
            ptRegionOut->tLocation = tLocationOutStart;
            ptRegionOut->tSize.iWidth = tLocationOutEnd.iX
                                      - tLocationOutStart.iX
                                      + 1;

            ptRegionOut->tSize.iHeight = tLocationOutEnd.iY
                                       - tLocationOutStart.iY
                                       + 1;
        }
    } while(0);

    return true;
}

ARM_NONNULL(1,2)
bool arm_2d_is_point_inside_region( const arm_2d_region_t *ptRegion,
                                    const arm_2d_location_t *ptPoint)
{
    assert(ptRegion != NULL);
    assert(ptPoint != NULL);
    
    do {
        if          (ptPoint->iX < ptRegion->tLocation.iX) {
            break;
        } else if   (ptPoint->iY < ptRegion->tLocation.iY) {
            break;
        } else if   (ptPoint->iX >= ptRegion->tLocation.iX + ptRegion->tSize.iWidth) {
            break;
        } else if   (ptPoint->iY >= ptRegion->tLocation.iY + ptRegion->tSize.iHeight) {
            break;
        }
        
        return true;
    } while(0);

    return false;
}

/*
  HOW IT WORKS:

   Root Tile (Output Tile)
  +------------------------------------------------------------------------+
  |     ... ...                                                            |
  |                                                                        |
  |                   Parent Tile                                          |
  |                 +------------------------------------+                 |
  |                 |        Child Tile                  |                 |
  |                 |     +------------------------------+---------+       |
  |                 |     |                              |/////////|       |
  |                 |     |       Valid Region           |/////////|       |
  |                 |     |                              |/////////|       |
  |                 +-----+------------------------------+/////////|       |
  |                       |////////////////////////////////////////|       |
  |                       |////////////////////////////////////////|       |
  |                       +----------------------------------------+       |
  |                                                                        |
  +------------------------------------------------------------------------+
 */
ARM_NONNULL(1,2)
const arm_2d_tile_t *arm_2d_tile_get_root(  const arm_2d_tile_t *ptTile,
                                            arm_2d_region_t *ptValidRegion,
                                            arm_2d_location_t *ptOffset)
{
    assert(NULL != ptTile);
    assert(NULL != ptValidRegion);

    *ptValidRegion = ptTile->tRegion;

    if (NULL != ptOffset) {
        ptOffset->iX = 0;
        ptOffset->iY = 0;
    }

    if (arm_2d_is_root_tile(ptTile)) {
        return ptTile;
    }

    do {
        //! get parent
        ptTile = (const arm_2d_tile_t *)ptTile->ptParent;
        if (NULL == ptTile) {
            break;
        }

        /*! \note Calculate the relative position between valid region and
         *!       the tile's original region. Usually, the tile's region
         *!       is inside the parent tile, but when the tile's location is
         *!       out of the parent's region with one or more negative axies,
         *!       the offset will be non-zero.
         *!       The offset is used to indicate the tile's view, and the
         *!       valid region is seen as inside the tile's region.
         *!
         *!   Figure: What's the meaning of offset location
         *!
         *!   The special case, where the child tile has a negative coordinates,
         *!   hence, the offset is (a,b) **as if** the valid region is inside
         *!   the child tile.
         *!
         *!    (-a,-b) Child Tile
         *!       +------------------------------------+
         *!       |///(0,0) Parent Tile ///////////////|
         *!       |/////+------------------------------+---------+
         *!       |/////|                              |         |
         *!       |/////|       Valid Region           |         |
         *!       |/////|                              |         |
         *!       +-----+------------------------------+         |
         *!             |                                        |
         *!             |                                        |
         *!             +----------------------------------------+
         *!
         */
        if (NULL != ptOffset) {
            arm_2d_location_t tOffset = ptValidRegion->tLocation;
            tOffset.iX = MAX(0, -tOffset.iX);
            tOffset.iY = MAX(0, -tOffset.iY);

            ptOffset->iX += tOffset.iX;
            ptOffset->iY += tOffset.iY;
        }

        /*! calculate the valid range in parent tile
         *!
         *! \note the location of the parent tile is used to indicate its
         *!       relative location between the it and its parent.
         *!       when calculate the valid range in parent, we have to assume
         *!       that the location is always (0,0)
         *!
         *! \note the location of a root tile is always (0,0)
         */
        arm_2d_region_t tParentRegion = {
            .tSize = ptTile->tRegion.tSize,
        };

        /*! make sure the output region is valid */
        if (!arm_2d_region_intersect(   &tParentRegion,
                                        ptValidRegion,
                                        ptValidRegion)) {
            /* out of range */
            return NULL;
        }

        if (arm_2d_is_root_tile(ptTile)) {
            break;
        }

        ptValidRegion->tLocation.iX += ptTile->tRegion.tLocation.iX;
        ptValidRegion->tLocation.iY += ptTile->tRegion.tLocation.iY;

    } while(true);

    return ptTile;
}


/*
  HOW IT WORKS:

   Parent Tile (Are NOT necessarily a ROOT tile )
  +------------------------------------------------------+
  |                                                      |
  |                                                      |
  |                         Target Region                |
  |                       +------------------------------+---------+
  |                       |                              |/////////|
  |                       |    New Child Tile (Output)   |/////////|
  |                       |                              |/////////|
  +-----------------------+------------------------------+/////////|
                          |////////////////////////////////////////|
                          |////////////////////////////////////////|
                          +----------------------------------------+

 */
ARM_NONNULL(1,2,3)
arm_2d_tile_t *arm_2d_tile_generate_child(
                                        const arm_2d_tile_t *ptParentTile,
                                        const arm_2d_region_t *ptRegion,
                                        arm_2d_tile_t *ptOutput,
                                        bool bClipRegion)
{
    assert(NULL != ptParentTile);
    assert(NULL != ptRegion);
    assert(NULL != ptOutput);

    memset(ptOutput, 0, sizeof(arm_2d_tile_t));
    ptOutput->tRegion = *ptRegion;

    arm_2d_region_t tParentRegion = {
        .tSize = ptParentTile->tRegion.tSize,
    };

    if (bClipRegion) {
        if (!arm_2d_region_intersect(   &tParentRegion,
                                        &(ptOutput->tRegion),
                                        &(ptOutput->tRegion)
                                        )) {
            /* out of range */
            return NULL;
        }
    } else {
        /*! \note since you are allowed to generate a child tile whose region is
         *!       bigger than its parent, so we don't have to clip it, see **note**
         *!       below.
         */
        if (!arm_2d_region_intersect(   &tParentRegion,
                                        &(ptOutput->tRegion),
                                        NULL //&(ptOutput->tRegion) //!< **note**
                                        )) {
            /* out of range */
            return NULL;
        }
    }

    ptOutput->ptParent = (arm_2d_tile_t *)ptParentTile;

    return ptOutput;
}


/*----------------------------------------------------------------------------*
 * Copy tile to destination directly                                          *
 *----------------------------------------------------------------------------*/

ARM_NONNULL(1,2)
arm_fsm_rt_t arm_2d_rgb16_tile_copy( const arm_2d_tile_t *ptSource,
                                     const arm_2d_tile_t *ptTarget,
                                     const arm_2d_region_t *ptRegion,
                                     uint32_t wMode)
{
    assert(NULL != ptSource);
    assert(NULL != ptTarget);

    ARM_2D_IMPL(arm_2d_op_cp_t);
    //memset(ptThis, 0, sizeof(*ptThis));
    
    OP_CORE.ptOp = &ARM_2D_OP_TABLE[ARM_2D_OP_TILE_COPY_RGB16];
    
    this.Target.ptTile = ptTarget;
    this.Target.ptRegion = ptRegion;
    this.Source.ptTile = ptSource;
    this.wMode = wMode;

    return __arm_2d_op_invoke(NULL);
}


ARM_NONNULL(1,2)
arm_fsm_rt_t arm_2d_rgb32_tile_copy( const arm_2d_tile_t *ptSource,
                                     const arm_2d_tile_t *ptTarget,
                                     const arm_2d_region_t *ptRegion,
                                     uint32_t wMode)
{

    assert(NULL != ptSource);
    assert(NULL != ptTarget);

    ARM_2D_IMPL(arm_2d_op_cp_t);
    //memset(ptThis, 0, sizeof(*ptThis));
    
    OP_CORE.ptOp = &ARM_2D_OP_TABLE[ARM_2D_OP_TILE_COPY_RGB32];
    
    this.Target.ptTile = ptTarget;
    this.Target.ptRegion = ptRegion;
    this.Source.ptTile = ptSource;
    this.wMode = wMode;

    return __arm_2d_op_invoke(NULL);
}

/*! \brief copy source tile to destination tile and use destination tile as 
 *!        background. When encountering specified mask colour, the background
 *!        pixel should be used, otherwise the foreground pixel from source tile
 *!        is used. 
 *!         
 *! \note  All color formats which using 16bits per pixel are treated equally.
 *! 
 *! \note  alpha channel is not handled, i.e. rgba5551
 */

ARM_NONNULL(1,2)
arm_fsm_rt_t arm_2d_rgb16_tile_copy_with_colour_masking(
                                            const arm_2d_tile_t *ptSource, 
                                            const arm_2d_tile_t *ptTarget,
                                            const arm_2d_region_t *ptRegion,
                                            uint16_t hwMaskColour,
                                            uint32_t wMode)
{
    assert(NULL != ptSource);
    assert(NULL != ptTarget);

    ARM_2D_IMPL(arm_2d_op_cp_cl_msk_t);
    //memset(ptThis, 0, sizeof(*ptThis));
    
    //! not support mirror
    if (wMode & (ARM_2D_CP_MODE_Y_MIRROR | ARM_2D_CP_MODE_X_MIRROR)) {
        return (arm_fsm_rt_t)ARM_2D_ERR_NOT_SUPPORT;
    }
    
    OP_CORE.ptOp = 
        &ARM_2D_OP_TABLE[ARM_2D_OP_TILE_COPY_WITH_COLOUR_MASKING_RGB16];
    
    this.Target.ptTile = ptTarget;
    this.Target.ptRegion = ptRegion;
    this.Source.ptTile = ptSource;
    this.wMode = wMode;
    this.hwColour = hwMaskColour;
    
    return __arm_2d_op_invoke(NULL);
}

/*! \brief copy source tile to destination tile and use destination tile as 
 *!        background. When encountering specified mask colour, the background
 *!        pixel should be used, otherwise the foreground pixel from source tile
 *!        is used. 
 *! 
 *! \note  All color formats which using 32bits per pixel are treated equally.
 *! 
 *! \note  alpha channel is not handled.
 */
ARM_NONNULL(1,2)
arm_fsm_rt_t arm_2d_rgb32_tile_copy_with_colour_masking(
                                            const arm_2d_tile_t *ptSource, 
                                            const arm_2d_tile_t *ptTarget,
                                            const arm_2d_region_t *ptRegion,
                                            uint32_t wMaskColour,
                                            uint32_t wMode)
{
    assert(NULL != ptSource);
    assert(NULL != ptTarget);

    ARM_2D_IMPL(arm_2d_op_cp_cl_msk_t);
    //memset(ptThis, 0, sizeof(*ptThis));

    //! not support mirror
    if (wMode & (ARM_2D_CP_MODE_Y_MIRROR | ARM_2D_CP_MODE_X_MIRROR)) {
        return (arm_fsm_rt_t)ARM_2D_ERR_NOT_SUPPORT;
    }

    OP_CORE.ptOp = 
        &ARM_2D_OP_TABLE[ARM_2D_OP_TILE_COPY_WITH_COLOUR_MASKING_RGB32];
    
    this.Target.ptTile = ptTarget;
    this.Target.ptRegion = ptRegion;
    this.Source.ptTile = ptSource;
    this.wMode = wMode;
    this.wColour = wMaskColour;

    return __arm_2d_op_invoke(NULL);
}

arm_fsm_rt_t __arm_2d_sw_tile_fill( __arm_2d_sub_task_t *ptTask)
{
    ARM_2D_IMPL(arm_2d_op_cp_t, ptTask->ptOP);
    
    uint32_t wMode = this.wMode;

    if (wMode & (ARM_2D_CP_MODE_Y_MIRROR | ARM_2D_CP_MODE_X_MIRROR)) {
    
        switch (OP_CORE.ptOp->Info.Colour.u3ColourSZ) {
            case ARM_2D_COLOUR_SZ_16BIT:
                __arm_2d_impl_rgb16_fill_mirror(ptTask->Param.tFill.pSource,
                                                ptTask->Param.tFill.iSourceStride,
                                                &ptTask->Param.tFill.tSourceRegion.tSize,
                                                ptTask->Param.tFill.pTarget,
                                                ptTask->Param.tFill.iTargetStride,
                                                &ptTask->Param.tFill.tTargetRegion.tSize,
                                                wMode);
                break;
            case ARM_2D_COLOUR_SZ_32BIT:
                __arm_2d_impl_rgb32_fill_mirror(ptTask->Param.tFill.pSource,
                                                ptTask->Param.tFill.iSourceStride,
                                                &ptTask->Param.tFill.tSourceRegion.tSize,
                                                ptTask->Param.tFill.pTarget,
                                                ptTask->Param.tFill.iTargetStride,
                                                &ptTask->Param.tFill.tTargetRegion.tSize,
                                                wMode);
                break;
            default:
                return (arm_fsm_rt_t)ARM_2D_ERR_NOT_SUPPORT;
        }
    
    } else {
        switch (OP_CORE.ptOp->Info.Colour.u3ColourSZ) {
            case ARM_2D_COLOUR_SZ_16BIT:
                __arm_2d_impl_rgb16_fill(   ptTask->Param.tFill.pSource,
                                            ptTask->Param.tFill.iSourceStride,
                                            &ptTask->Param.tFill.tSourceRegion.tSize,
                                            ptTask->Param.tFill.pTarget,
                                            ptTask->Param.tFill.iTargetStride,
                                            &ptTask->Param.tFill.tTargetRegion.tSize);
                break;
                
            case ARM_2D_COLOUR_SZ_32BIT:
                __arm_2d_impl_rgb32_fill(   ptTask->Param.tFill.pSource,
                                            ptTask->Param.tFill.iSourceStride,
                                            &ptTask->Param.tFill.tSourceRegion.tSize,
                                            ptTask->Param.tFill.pTarget,
                                            ptTask->Param.tFill.iTargetStride,
                                            &ptTask->Param.tFill.tTargetRegion.tSize);
                break;
            default:
                return (arm_fsm_rt_t)ARM_2D_ERR_NOT_SUPPORT;
        }
    }
        
    return arm_fsm_rt_cpl;
}


arm_fsm_rt_t __arm_2d_sw_tile_copy( __arm_2d_sub_task_t *ptTask)
{
    ARM_2D_IMPL(arm_2d_op_cp_t, ptTask->ptOP);
    
    uint32_t wMode = this.wMode;

    if (wMode & (ARM_2D_CP_MODE_Y_MIRROR | ARM_2D_CP_MODE_X_MIRROR)) {
        switch (OP_CORE.ptOp->Info.Colour.u3ColourSZ) {
            case ARM_2D_COLOUR_SZ_16BIT:
                __arm_2d_impl_rgb16_copy_mirror(
                                            ptTask->Param.tCopy.pSource ,
                                            ptTask->Param.tCopy.iSourceStride,
                                            ptTask->Param.tCopy.pTarget,
                                            ptTask->Param.tCopy.iTargetStride,
                                            &ptTask->Param.tCopy.tCopySize,
                                            wMode);
                break;
            case ARM_2D_COLOUR_SZ_32BIT:
                __arm_2d_impl_rgb32_copy_mirror(
                                            ptTask->Param.tCopy.pSource ,
                                            ptTask->Param.tCopy.iSourceStride,
                                            ptTask->Param.tCopy.pTarget,
                                            ptTask->Param.tCopy.iTargetStride,
                                            &ptTask->Param.tCopy.tCopySize,
                                            wMode);
                break;
            default:
                return (arm_fsm_rt_t)ARM_2D_ERR_NOT_SUPPORT;
        }
    } else {
        switch (OP_CORE.ptOp->Info.Colour.u3ColourSZ) {
            case ARM_2D_COLOUR_SZ_16BIT:
                __arm_2d_impl_rgb16_copy(   ptTask->Param.tCopy.pSource ,
                                            ptTask->Param.tCopy.iSourceStride,
                                            ptTask->Param.tCopy.pTarget,
                                            ptTask->Param.tCopy.iTargetStride,
                                            &ptTask->Param.tCopy.tCopySize);
                break;
            case ARM_2D_COLOUR_SZ_32BIT:
                __arm_2d_impl_rgb32_copy(   ptTask->Param.tCopy.pSource ,
                                            ptTask->Param.tCopy.iSourceStride,
                                            ptTask->Param.tCopy.pTarget,
                                            ptTask->Param.tCopy.iTargetStride,
                                            &ptTask->Param.tCopy.tCopySize);
                break;
            default:
                return (arm_fsm_rt_t)ARM_2D_ERR_NOT_SUPPORT;
        }
    }

    return arm_fsm_rt_cpl;
}


arm_fsm_rt_t __arm_2d_sw_tile_copy_with_colour_masking(
                                            __arm_2d_sub_task_t *ptTask)
{
    ARM_2D_IMPL(arm_2d_op_cp_cl_msk_t, ptTask->ptOP)

    switch (OP_CORE.ptOp->Info.Colour.u3ColourSZ) {
        case ARM_2D_COLOUR_SZ_16BIT:
            __arm_2d_impl_rgb16_copy_colour_masking(   
                                    ptTask->Param.tCopy.pSource ,
                                    ptTask->Param.tCopy.iSourceStride,
                                    ptTask->Param.tCopy.pTarget,
                                    ptTask->Param.tCopy.iTargetStride,
                                    &ptTask->Param.tCopy.tCopySize,
                                    this.hwColour);
            break;
        case ARM_2D_COLOUR_SZ_32BIT:
            __arm_2d_impl_rgb32_copy_colour_masking(   
                                    ptTask->Param.tCopy.pSource ,
                                    ptTask->Param.tCopy.iSourceStride,
                                    ptTask->Param.tCopy.pTarget,
                                    ptTask->Param.tCopy.iTargetStride,
                                    &ptTask->Param.tCopy.tCopySize,
                                    this.wColour);
            break;
        default:
            return (arm_fsm_rt_t)ARM_2D_ERR_NOT_SUPPORT;
    }

    return arm_fsm_rt_cpl;
}

arm_fsm_rt_t __arm_2d_sw_tile_fill_with_colour_masking( 
                                        __arm_2d_sub_task_t *ptTask)
{
    ARM_2D_IMPL(arm_2d_op_cp_cl_msk_t, ptTask->ptOP);

    switch (OP_CORE.ptOp->Info.Colour.u3ColourSZ) {
        case ARM_2D_COLOUR_SZ_16BIT:
            __arm_2d_impl_rgb16_fill_colour_masking(
                                    ptTask->Param.tFill.pSource,
                                    ptTask->Param.tFill.iSourceStride,
                                    &ptTask->Param.tFill.tSourceRegion.tSize,
                                    ptTask->Param.tFill.pTarget,
                                    ptTask->Param.tFill.iTargetStride,
                                    &ptTask->Param.tFill.tTargetRegion.tSize,
                                    this.hwColour);
            break;
            
        case ARM_2D_COLOUR_SZ_32BIT:
            __arm_2d_impl_rgb32_fill_colour_masking(
                                    ptTask->Param.tFill.pSource,
                                    ptTask->Param.tFill.iSourceStride,
                                    &ptTask->Param.tFill.tSourceRegion.tSize,
                                    ptTask->Param.tFill.pTarget,
                                    ptTask->Param.tFill.iTargetStride,
                                    &ptTask->Param.tFill.tTargetRegion.tSize,
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
 
static
void __arm_2d_impl_rgb16_fill_generic(const uint16_t * __RESTRICT pSourceBase,
                                    int16_t iSourceStride,
                                    const arm_2d_size_t * __RESTRICT ptSourceSize,
                                    uint16_t * __RESTRICT pTargetBase,
                                    int16_t iTargetStride,
                                    const arm_2d_size_t * __RESTRICT ptTargetSize,
                                    const arm_2d_rgb16_paving_fct_t * __RESTRICT pPavFct)
{
    __ARM_2D_MEM_FILL_GENERIC(  pSourceBase, 
                                iSourceStride,
                                ptSourceSize, 
                                pTargetBase, 
                                iTargetStride, 
                                ptTargetSize,
                                pPavFct);
}

static
void __arm_2d_impl_rgb32_fill_generic(const uint32_t * __RESTRICT pSourceBase,
                                    int16_t iSourceStride,
                                    const arm_2d_size_t * __RESTRICT ptSourceSize,
                                    uint32_t * __RESTRICT pTargetBase,
                                    int16_t iTargetStride,
                                    const arm_2d_size_t * __RESTRICT ptTargetSize,
                                    const arm_2d_rgb32_paving_fct_t * __RESTRICT pPavFct)
{
    __ARM_2D_MEM_FILL_GENERIC(  pSourceBase, 
                                iSourceStride,
                                ptSourceSize, 
                                pTargetBase, 
                                iTargetStride, 
                                ptTargetSize,
                                pPavFct);
} 


static
void __arm_2d_rgb16_mem_fill_colour_masking_generic(const uint16_t * __RESTRICT pSourceBase,
                                    int16_t iSourceStride,
                                    const arm_2d_size_t * __RESTRICT ptSourceSize,
                                    uint16_t * __RESTRICT pTargetBase,
                                    int16_t iTargetStride,
                                    const arm_2d_size_t * __RESTRICT ptTargetSize,
                                    const arm_2d_rgb16_cl_msk_paving_fct_t * __RESTRICT pPavFct,
                                    uint_fast16_t hwColour)
{
    __ARM_2D_MEM_FILL_GENERIC(  pSourceBase, 
                                iSourceStride,
                                ptSourceSize, 
                                pTargetBase, 
                                iTargetStride, 
                                ptTargetSize,
                                pPavFct,
                                hwColour);
}

static
void __arm_2d_impl_rgb32_fill_colour_masking_generic(const uint32_t * __RESTRICT pSourceBase,
                                    int16_t iSourceStride,
                                    const arm_2d_size_t * __RESTRICT ptSourceSize,
                                    uint32_t * __RESTRICT pTargetBase,
                                    int16_t iTargetStride,
                                    const arm_2d_size_t * __RESTRICT ptTargetSize,
                                    const arm_2d_rgb32_cl_msk_paving_fct_t * __RESTRICT pPavFct,
                                    uint32_t wColour)
{
    __ARM_2D_MEM_FILL_GENERIC(  pSourceBase, 
                                iSourceStride,
                                ptSourceSize, 
                                pTargetBase, 
                                iTargetStride, 
                                ptTargetSize,
                                pPavFct,
                                wColour);
} 
 

__WEAK
void __arm_2d_impl_rgb16_fill_mirror(uint16_t * __RESTRICT phwSourceBase,
                                         int16_t iSourceStride,
                                         arm_2d_size_t * __RESTRICT ptSourceSize,
                                         uint16_t * __RESTRICT phwTargetBase,
                                         int16_t iTargetStride,
                                         arm_2d_size_t * __RESTRICT ptTargetSize,
                                         uint32_t wMode)
{
    switch (wMode & (ARM_2D_CP_MODE_Y_MIRROR | ARM_2D_CP_MODE_X_MIRROR)) {
        case ARM_2D_CP_MODE_X_MIRROR:
            __arm_2d_impl_rgb16_fill_x_mirror(
                    phwSourceBase, iSourceStride, ptSourceSize,
                    phwTargetBase, iTargetStride, ptTargetSize);
            break;
        case ARM_2D_CP_MODE_Y_MIRROR:
            __arm_2d_impl_rgb16_fill_y_mirror(
                    phwSourceBase, iSourceStride, ptSourceSize,
                    phwTargetBase, iTargetStride, ptTargetSize);
            break;
        case ARM_2D_CP_MODE_Y_MIRROR | ARM_2D_CP_MODE_X_MIRROR :
            __arm_2d_impl_rgb16_fill_xy_mirror(
                    phwSourceBase, iSourceStride, ptSourceSize, 
                    phwTargetBase, iTargetStride, ptTargetSize);
            break;
        default:
            assert(false);  /*! this should not happen */
            //break;
    }
}

__WEAK
void __arm_2d_impl_rgb32_fill_mirror(   uint32_t *__RESTRICT pwSourceBase,
                                        int16_t iSourceStride,
                                        arm_2d_size_t *__RESTRICT ptSourceSize,
                                        uint32_t *__RESTRICT pwTargetBase,
                                        int16_t iTargetStride,
                                        arm_2d_size_t *__RESTRICT ptTargetSize,
                                        uint32_t wMode)
{
    switch (wMode & (ARM_2D_CP_MODE_Y_MIRROR | ARM_2D_CP_MODE_X_MIRROR)) {
        case ARM_2D_CP_MODE_X_MIRROR:
            __arm_2d_impl_rgb32_fill_x_mirror(
                    pwSourceBase, iSourceStride, ptSourceSize,
                    pwTargetBase, iTargetStride, ptTargetSize);
            break;
        case ARM_2D_CP_MODE_Y_MIRROR:
            __arm_2d_impl_rgb32_fill_y_mirror(
                    pwSourceBase, iSourceStride, ptSourceSize,
                    pwTargetBase, iTargetStride, ptTargetSize);
            break;
        case ARM_2D_CP_MODE_Y_MIRROR | ARM_2D_CP_MODE_X_MIRROR :
            __arm_2d_impl_rgb32_fill_xy_mirror(
                    pwSourceBase, iSourceStride, ptSourceSize, 
                    pwTargetBase, iTargetStride, ptTargetSize);
            break;
        default:
            assert(false);  /*! this should not happen */
            //break;
    }
}

__WEAK
void __arm_2d_impl_rgb16_copy_mirror(uint16_t * __RESTRICT phwSource,
                                         int16_t iSourceStride,
                                         uint16_t * __RESTRICT phwTarget,
                                         int16_t iTargetStride,
                                         arm_2d_size_t * __RESTRICT ptCopySize,
                                         uint32_t wMode)
{
    switch (wMode & (ARM_2D_CP_MODE_Y_MIRROR | ARM_2D_CP_MODE_X_MIRROR)) {
      case ARM_2D_CP_MODE_X_MIRROR:
          __arm_2d_impl_rgb16_copy_x_mirror(phwSource, iSourceStride,
                                           phwTarget, iTargetStride, ptCopySize);
          break;
      case ARM_2D_CP_MODE_Y_MIRROR:
          __arm_2d_impl_rgb16_copy_y_mirror(phwSource, iSourceStride,
                                           phwTarget, iTargetStride, ptCopySize);
          break;
      case ARM_2D_CP_MODE_Y_MIRROR | ARM_2D_CP_MODE_X_MIRROR:
          __arm_2d_impl_rgb16_copy_xy_mirror(phwSource, iSourceStride,
                                            phwTarget, iTargetStride, ptCopySize);
          break;
      default:
          assert(false);        /*! this should not happen */
          //break;
    }
}

__WEAK
void __arm_2d_impl_rgb32_copy_mirror(uint32_t * __RESTRICT pwSource,
                                         int16_t iSourceStride,
                                         uint32_t * __RESTRICT pwTarget,
                                         int16_t iTargetStride,
                                         arm_2d_size_t * __RESTRICT ptCopySize,
                                         uint32_t wMode)
{
    switch (wMode & (ARM_2D_CP_MODE_Y_MIRROR | ARM_2D_CP_MODE_X_MIRROR)) {
      case ARM_2D_CP_MODE_X_MIRROR:
          __arm_2d_impl_rgb32_copy_x_mirror(pwSource, iSourceStride,
                                           pwTarget, iTargetStride, ptCopySize);
          break;
      case ARM_2D_CP_MODE_Y_MIRROR:
          __arm_2d_impl_rgb32_copy_y_mirror(pwSource, iSourceStride,
                                           pwTarget, iTargetStride, ptCopySize);
          break;
      case ARM_2D_CP_MODE_Y_MIRROR | ARM_2D_CP_MODE_X_MIRROR:
          __arm_2d_impl_rgb32_copy_xy_mirror(pwSource, iSourceStride,
                                            pwTarget, iTargetStride, ptCopySize);
          break;
      default:
          assert(false);        /*! this should not happen */
          //break;
    }
}


__WEAK
void __arm_2d_impl_rgb16_copy(   uint16_t *__RESTRICT phwSource,
                                int16_t iSourceStride,
                                uint16_t *__RESTRICT phwTarget,
                                int16_t iTargetStride,
                                arm_2d_size_t *__RESTRICT ptCopySize)
{
    for (int_fast16_t y = 0; y < ptCopySize->iHeight; y++) {
        memcpy(phwTarget, phwSource, ptCopySize->iWidth * sizeof(*phwSource));
        phwSource += iSourceStride;
        phwTarget += iTargetStride;
    }
}

__WEAK
void __arm_2d_impl_rgb32_copy(   uint32_t *__RESTRICT pwSource,
                                int16_t iSourceStride,
                                uint32_t *__RESTRICT pwTarget,
                                int16_t iTargetStride,
                                arm_2d_size_t *__RESTRICT ptCopySize)
{
    for (int_fast16_t y = 0; y < ptCopySize->iHeight; y++) {
        memcpy(pwTarget, pwSource, ptCopySize->iWidth * sizeof(*pwSource));
        pwSource += iSourceStride;
        pwTarget += iTargetStride;
    }
}

__WEAK
void __arm_2d_impl_rgb16_copy_colour_masking(   
                                uint16_t *__RESTRICT phwSource,
                                int16_t iSourceStride,
                                uint16_t *__RESTRICT phwTarget,
                                int16_t iTargetStride,
                                arm_2d_size_t *__RESTRICT ptCopySize,
                                uint16_t hwColour)
{
    for (int_fast16_t y = 0; y < ptCopySize->iHeight; y++) {
        for (int_fast16_t x = 0; x < ptCopySize->iWidth; x++){
            if (phwSource[x] == hwColour) {
                continue;
            }
            phwTarget[x] = phwSource[x];
        }
        phwSource += iSourceStride;
        phwTarget += iTargetStride;
    }
}

__WEAK
void __arm_2d_impl_rgb32_copy_colour_masking(   
                                uint32_t *__RESTRICT pwSource,
                                int16_t iSourceStride,
                                uint32_t *__RESTRICT pwTarget,
                                int16_t iTargetStride,
                                arm_2d_size_t *__RESTRICT ptCopySize,
                                uint32_t wColour)
{
    for (int_fast16_t y = 0; y < ptCopySize->iHeight; y++) {
        for (int_fast16_t x = 0; x < ptCopySize->iWidth; x++){
            if (pwSource[x] == wColour) {
                continue;
            }
            pwTarget[x] = pwSource[x];
        }
        pwSource += iSourceStride;
        pwTarget += iTargetStride;
    }
}

#if 0
__WEAK
void __arm_2d_impl_rgb16_fill(   uint16_t *phwSourceBase,
                                int16_t iSourceStride,
                                arm_2d_size_t *ptSourceSize,
                                uint16_t *phwTargetBase,
                                int16_t iTargetStride,
                                arm_2d_size_t *ptTargetSize)
{
    for (int_fast16_t iTargetY = 0; iTargetY < ptTargetSize->iHeight;) {
        uint16_t       *phwSource = phwSourceBase;      /* todo: check with GW */

        for (int_fast16_t iSourceY = 0; iSourceY < ptSourceSize->iHeight; iSourceY++) {
            uint16_t       *phwTarget = phwTargetBase;
            /*---------------- Height Loop Begin----------------*/
            uint_fast32_t   wLengthLeft = ptTargetSize->iWidth;

            do {
                uint_fast32_t   wLength = MIN(wLengthLeft, ptSourceSize->iWidth);
                /*---------------- Width Loop Begin----------------*/
                memcpy(phwTarget, phwSource, wLength * sizeof(uint16_t));
                phwTarget += wLength;
                /*---------------- Width Loop End----------------*/
                wLengthLeft -= wLength;
            } while (wLengthLeft);
            /*---------------- Height Loop End----------------*/
            phwSource += iSourceStride;
            phwTargetBase += iTargetStride;
            iTargetY++;
            if (iTargetY >= ptTargetSize->iHeight) {
                break;
            }
        }
    }
}

__WEAK
void __arm_2d_impl_rgb16_fill_x_mirror(uint16_t * phwSourceBase,
                                      int16_t iSourceStride,
                                      arm_2d_size_t * ptSourceSize,
                                      uint16_t * phwTargetBase,
                                      int16_t iTargetStride,
                                      arm_2d_size_t * ptTargetSize)
{
    for (int_fast16_t iTargetY = 0; iTargetY < ptTargetSize->iHeight;) {
        uint16_t *phwSource = phwSourceBase;

        for (int_fast16_t iSourceY = 0; iSourceY < ptSourceSize->iHeight; iSourceY++) {
            uint16_t *phwTarget = phwTargetBase;
            /*---------------- Height Loop Begin----------------*/
            uint_fast32_t wLengthLeft = ptTargetSize->iWidth;

            do {
                uint_fast32_t wLength = MIN(wLengthLeft, ptSourceSize->iWidth);
                /*---------------- Width Loop Begin----------------*/

                uint16_t *phwTargetCur = phwTarget;
                uint16_t *phwSourceCur = phwSource;

                phwSourceCur += ptSourceSize->iWidth - 1;
                for (int i = 0; i < wLength; i++) {
                    *phwTargetCur++ = *phwSourceCur--;
                }
                
                phwTarget += wLength;
                /*---------------- Width Loop End----------------*/
                wLengthLeft -= wLength;
            } while (wLengthLeft);

            /*---------------- Height Loop End----------------*/
            phwSource += iSourceStride;
            phwTargetBase += iTargetStride;
            iTargetY++;
            if (iTargetY >= ptTargetSize->iHeight) {
                break;
            }
        }
    }
}

__WEAK
void __arm_2d_impl_rgb16_fill_y_mirror(uint16_t * phwSourceBase,
                                      int16_t iSourceStride,
                                      arm_2d_size_t * ptSourceSize,
                                      uint16_t * phwTargetBase,
                                      int16_t iTargetStride,
                                      arm_2d_size_t * ptTargetSize)
{
    for (int_fast16_t iTargetY = 0; iTargetY < ptTargetSize->iHeight;) {
        uint16_t *phwSource = phwSourceBase 
                            + iSourceStride * (ptSourceSize->iHeight - 1);

        for (int_fast16_t iSourceY = 0; iSourceY < ptSourceSize->iHeight; iSourceY++) {
            uint16_t *phwTarget = phwTargetBase;
            /*---------------- Height Loop Begin----------------*/
            uint_fast32_t wLengthLeft = ptTargetSize->iWidth;

            do {
                uint_fast32_t wLength = MIN(wLengthLeft, ptSourceSize->iWidth);
                /*---------------- Width Loop Begin----------------*/
                memcpy(phwTarget, phwSource, wLength * sizeof(uint16_t));
                phwTarget += wLength;
                /*---------------- Width Loop End----------------*/
                wLengthLeft -= wLength;
            } while (wLengthLeft);
            /*---------------- Height Loop End----------------*/
            phwSource -= iSourceStride;
            phwTargetBase += iTargetStride;
            iTargetY++;
            if (iTargetY >= ptTargetSize->iHeight) {
                break;
            }
        }
    }
}

__WEAK
void __arm_2d_impl_rgb16_fill_xy_mirror(uint16_t * phwSourceBase,
                                       int16_t iSourceStride,
                                       arm_2d_size_t * ptSourceSize,
                                       uint16_t * phwTargetBase,
                                       int16_t iTargetStride,
                                       arm_2d_size_t * ptTargetSize)
{
    for (int_fast16_t iTargetY = 0; iTargetY < ptTargetSize->iHeight;) {
        uint16_t *phwSource = phwSourceBase 
                            + iSourceStride * (ptSourceSize->iHeight - 1);        

        for (int_fast16_t iSourceY = 0; iSourceY < ptSourceSize->iHeight; iSourceY++) {
            uint16_t *phwTarget = phwTargetBase;
            /*---------------- Height Loop Begin----------------*/
            uint_fast32_t wLengthLeft = ptTargetSize->iWidth;

            do {
                uint_fast32_t wLength = MIN(wLengthLeft, ptSourceSize->iWidth);
                /*---------------- Width Loop Begin----------------*/
                uint16_t *phwTargetCur = phwTarget;
                uint16_t *phwSourceCur = phwSource;

                phwSourceCur += ptSourceSize->iWidth - 1;
                for (int i = 0; i < wLength; i++) {
                    *phwTargetCur++ = *phwSourceCur--;
                }
                phwTarget += wLength;
                /*---------------- Width Loop End----------------*/
                wLengthLeft -= wLength;
            } while (wLengthLeft);

            /*---------------- Height Loop End----------------*/
            phwSource -= iSourceStride;
            phwTargetBase += iTargetStride;
            iTargetY++;
            if (iTargetY >= ptTargetSize->iHeight) {
                break;
            }
        }
    }
}

__WEAK
void __arm_2d_impl_rgb16_copy_x_mirror(uint16_t * phwSource,
                                      int16_t iSourceStride,
                                      uint16_t * phwTarget,
                                      int16_t iTargetStride,
                                      arm_2d_size_t * ptCopySize)
{
    for (int_fast16_t y = 0; y < ptCopySize->iHeight; y++) {
        uint16_t  *phwTargetCur = phwTarget;
        uint16_t  *phwSourceCur = phwSource;

        phwSourceCur += ptCopySize->iWidth - 1;
        for (int i = 0; i < ptCopySize->iWidth; i++) {
            *phwTargetCur++ = *phwSourceCur--;
        }
        phwSource += iSourceStride;
        phwTarget += iTargetStride;
    }
}

__WEAK
void __arm_2d_impl_rgb32_copy_x_mirror(  uint32_t * pwSource,
                                        int16_t iSourceStride,
                                        uint32_t * pwTarget,
                                        int16_t iTargetStride,
                                        arm_2d_size_t * ptCopySize)
{
    for (int_fast16_t y = 0; y < ptCopySize->iHeight; y++) {
        uint32_t       *pwTargetCur = pwTarget;
        uint32_t       *pwSourceCur = pwSource;
        pwSourceCur += ptCopySize->iWidth - 1;
        
        for (int i = 0; i < ptCopySize->iWidth; i++) {
            *pwTargetCur++ = *pwSourceCur--;
        }
        
        pwSource += iSourceStride;
        pwTarget += iTargetStride;
    }
}

__WEAK
void __arm_2d_impl_rgb16_copy_y_mirror(  uint16_t * phwSource,
                                        int16_t iSourceStride,
                                        uint16_t * phwTarget,
                                        int16_t iTargetStride,
                                        arm_2d_size_t * ptCopySize)
{
    phwSource += iSourceStride * (ptCopySize->iHeight - 1);

    for (int_fast16_t y = 0; y < ptCopySize->iHeight; y++) {
        memcpy(phwTarget, phwSource, ptCopySize->iWidth * sizeof(*phwSource));
        phwSource -= iSourceStride;
        phwTarget += iTargetStride;
    }
}

__WEAK
void __arm_2d_impl_rgb32_copy_y_mirror(  uint32_t * pwSource,
                                        int16_t iSourceStride,
                                        uint32_t * pwTarget,
                                        int16_t iTargetStride,
                                        arm_2d_size_t * ptCopySize)
{
    pwSource += iSourceStride * (ptCopySize->iHeight - 1);

    for (int_fast16_t y = 0; y < ptCopySize->iHeight; y++) {

        memcpy(pwTarget, pwSource, ptCopySize->iWidth * sizeof(*pwSource));
        pwSource -= iSourceStride;
        pwTarget += iTargetStride;
    }
}

__WEAK
void __arm_2d_impl_rgb16_copy_xy_mirror( uint16_t * phwSource,
                                        int16_t iSourceStride,
                                        uint16_t * phwTarget,
                                        int16_t iTargetStride,
                                        arm_2d_size_t * ptCopySize)
{
    phwSource += iSourceStride * (ptCopySize->iHeight - 1);

    for (int_fast16_t y = 0; y < ptCopySize->iHeight; y++) {

        uint16_t *phwTargetCur = phwTarget;
        uint16_t *phwSourceCur = phwSource;

        phwSourceCur += ptCopySize->iWidth - 1;
        for (int i = 0; i < ptCopySize->iWidth; i++) {
            *phwTargetCur++ = *phwSourceCur--;
        }
        
        phwSource -= iSourceStride;
        phwTarget += iTargetStride;
    }
}

__WEAK
void __arm_2d_impl_rgb32_copy_xy_mirror( uint32_t * pwSource,
                                        int16_t iSourceStride,
                                        uint32_t * pwTarget,
                                        int16_t iTargetStride,
                                        arm_2d_size_t * ptCopySize)
{
    pwSource += iSourceStride * (ptCopySize->iHeight - 1);

    for (int_fast16_t y = 0; y < ptCopySize->iHeight; y++) {
        uint32_t *pwTargetCur = pwTarget;
        uint32_t *pwSourceCur = pwSource;

        pwSourceCur += ptCopySize->iWidth - 1;
        for (int i = 0; i < ptCopySize->iWidth; i++) {
            *pwTargetCur++ = *pwSourceCur--;
        }
        pwSource -= iSourceStride;
        pwTarget += iTargetStride;
    }
}


__WEAK
void __arm_2d_impl_rgb32_fill(   uint32_t *pwSourceBase,
                                int16_t iSourceStride,
                                arm_2d_size_t *ptSourceSize,
                                uint32_t *pwTargetBase,
                                int16_t iTargetStride,
                                arm_2d_size_t *ptTargetSize)
{
    for (int_fast16_t iTargetY = 0; iTargetY < ptTargetSize->iHeight;) {
        uint32_t       *pwSource = pwSourceBase;      /* todo: check with GW */

        for (int_fast16_t iSourceY = 0; iSourceY < ptSourceSize->iHeight; iSourceY++) {
            uint32_t       *pwTarget = pwTargetBase;
            
            /*---------------- Height Loop Begin----------------*/
            uint_fast32_t   wLengthLeft = ptTargetSize->iWidth;
            do {
                uint_fast32_t   wLength = MIN(wLengthLeft, ptSourceSize->iWidth);
                
                /*---------------- Width Loop Begin----------------*/
                memcpy(pwTarget, pwSource, wLength * sizeof(uint32_t));
                pwTarget += wLength;
                /*---------------- Width Loop End----------------*/
                
                wLengthLeft -= wLength;
            } while (wLengthLeft);
            /*---------------- Height Loop End----------------*/
            
            pwSource += iSourceStride;
            pwTargetBase += iTargetStride;
            iTargetY++;
            if (iTargetY >= ptTargetSize->iHeight) {
                break;
            }
        }
    }
}



__WEAK
void __arm_2d_impl_rgb32_fill_x_mirror(  uint32_t * pwSourceBase,
                                        int16_t iSourceStride,
                                        arm_2d_size_t * ptSourceSize,
                                        uint32_t * pwTargetBase,
                                        int16_t iTargetStride,
                                        arm_2d_size_t * ptTargetSize)
{
    for (int_fast16_t iTargetY = 0; iTargetY < ptTargetSize->iHeight;) {
        uint32_t       *pwSource = pwSourceBase;
        for (int_fast16_t iSourceY = 0; iSourceY < ptSourceSize->iHeight; iSourceY++) {
        
            /*---------------- Height Loop Begin----------------*/
            uint32_t       *pwTarget = pwTargetBase;
            uint_fast32_t   wLengthLeft = ptTargetSize->iWidth;
            do {
                uint_fast32_t   wLength = MIN(wLengthLeft, ptSourceSize->iWidth);
                
                /*---------------- Width Loop Begin----------------*/
                uint32_t       *pwTargetCur = pwTarget;
                uint32_t       *pwSourceCur = pwSource;

                pwSourceCur += ptSourceSize->iWidth - 1;
                for (int i = 0; i < wLength; i++) {
                    *pwTargetCur++ = *pwSourceCur--;
                }
                pwTarget += wLength;
                /*---------------- Width Loop End----------------*/
                
                wLengthLeft -= wLength;
            } while (wLengthLeft);
            /*---------------- Height Loop End----------------*/
            
            pwSource += iSourceStride;
            pwTargetBase += iTargetStride;
            
            iTargetY++;
            if (iTargetY >= ptTargetSize->iHeight) {
                break;
            }
        }
    }
}

__WEAK
void __arm_2d_impl_rgb32_fill_y_mirror(  uint32_t * pwSourceBase,
                                        int16_t iSourceStride,
                                        arm_2d_size_t * ptSourceSize,
                                        uint32_t * pwTargetBase,
                                        int16_t iTargetStride,
                                        arm_2d_size_t * ptTargetSize)
{
    for (int_fast16_t iTargetY = 0; iTargetY < ptTargetSize->iHeight;) {
        uint32_t *pwSource  = pwSourceBase 
                            + iSourceStride * (ptSourceSize->iHeight - 1);

        for (int_fast16_t iSourceY = 0; iSourceY < ptSourceSize->iHeight; iSourceY++) {
            uint32_t *pwTarget = pwTargetBase;
            
            /*---------------- Height Loop Begin----------------*/
            uint_fast32_t wLengthLeft = ptTargetSize->iWidth;
            do {
                uint_fast32_t wLength = MIN(wLengthLeft, ptSourceSize->iWidth);
                
                /*---------------- Width Loop Begin----------------*/
                memcpy(pwTarget, pwSource, wLength * sizeof(uint32_t));
                pwTarget += wLength;
                /*---------------- Width Loop End----------------*/
                
                wLengthLeft -= wLength;
            } while (wLengthLeft);
            /*---------------- Height Loop End----------------*/
            
            pwSource -= iSourceStride;
            pwTargetBase += iTargetStride;
            /*---------------- Height Loop End----------------*/
            
            iTargetY++;
            if (iTargetY >= ptTargetSize->iHeight) {
                break;
            }
        }
    }
}

__WEAK
void __arm_2d_impl_rgb32_fill_xy_mirror( uint32_t * pwSourceBase,
                                        int16_t iSourceStride,
                                        arm_2d_size_t * ptSourceSize,
                                        uint32_t * pwTargetBase,
                                        int16_t iTargetStride,
                                        arm_2d_size_t * ptTargetSize)
{
    for (int_fast16_t iTargetY = 0; iTargetY < ptTargetSize->iHeight;) {
        uint32_t *pwSource  = pwSourceBase 
                            + iSourceStride * (ptSourceSize->iHeight - 1);

        for (int_fast16_t iSourceY = 0; iSourceY < ptSourceSize->iHeight; iSourceY++) {
        
            /*---------------- Height Loop Begin----------------*/
            uint32_t *pwTarget = pwTargetBase;
            uint_fast32_t wLengthLeft = ptTargetSize->iWidth;

            do {
                uint_fast32_t wLength = MIN(wLengthLeft, ptSourceSize->iWidth);
                
                /*---------------- Width Loop Begin----------------*/
                uint32_t *pwTargetCur = pwTarget;
                uint32_t  *pwSourceCur = pwSource;

                pwSourceCur += ptSourceSize->iWidth - 1;
                for (int i = 0; i < wLength; i++) {
                    *pwTargetCur++ = *pwSourceCur--;
                }
                pwTarget += wLength;
                /*---------------- Width Loop End----------------*/
                
                wLengthLeft -= wLength;
            } while (wLengthLeft);

            /*---------------- Height Loop End----------------*/
            
            pwSource -= iSourceStride;
            pwTargetBase += iTargetStride;
            
            iTargetY++;
            if (iTargetY >= ptTargetSize->iHeight) {
                break;
            }
        }
    }
}
#else


__WEAK 
void __arm_2d_rgb16_2x2_paving(const uint16_t *__RESTRICT pSourceBase,
                                int16_t iSourceStride,
                                const arm_2d_size_t * __RESTRICT ptSourceSize,
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

__WEAK 
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
                        (in != hwColour));
}


__WEAK void __arm_2d_rgb16_2x2_paving_x_mirror(const uint16_t *__RESTRICT pSourceBase,
                                        int16_t iSourceStride,
                                        const arm_2d_size_t * __RESTRICT ptSourceSize,
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


__WEAK void __arm_2d_rgb16_2x2_paving_y_mirror(const uint16_t *__RESTRICT pSourceBase,
                                        int16_t iSourceStride,
                                        const arm_2d_size_t * __RESTRICT ptSourceSize,
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

__WEAK void __arm_2d_rgb16_2x2_paving_xy_mirror(const uint16_t *__RESTRICT pSourceBase,
                                            int16_t iSourceStride,
                                            const arm_2d_size_t * __RESTRICT ptSourceSize,
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


__WEAK 
void __arm_2d_rgb16_1x2_paving( const uint16_t * __RESTRICT pSourceBase,
                                int16_t iSourceStride,
                                const arm_2d_size_t * __RESTRICT ptSourceSize,
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

__WEAK 
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
                        (in != hwColour));
}

__WEAK void __arm_2d_rgb16_1x2_paving_x_mirror(const uint16_t * __RESTRICT pSourceBase,
                                        int16_t iSourceStride,
                                        const arm_2d_size_t * __RESTRICT ptSourceSize,
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

__WEAK void __arm_2d_rgb16_1x2_paving_y_mirror(const uint16_t * __RESTRICT pSourceBase,
                                       int16_t iSourceStride,
                                       const arm_2d_size_t * __RESTRICT ptSourceSize,
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


__WEAK void __arm_2d_rgb16_1x2_paving_xy_mirror(const uint16_t * __RESTRICT pSourceBase,
                                       int16_t iSourceStride,
                                       const arm_2d_size_t * __RESTRICT ptSourceSize,
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


__WEAK 
void __arm_2d_rgb16_2x1_paving( const uint16_t * __RESTRICT pSourceBase,
                                int16_t iSourceStride,
                                const arm_2d_size_t * __RESTRICT ptSourceSize,
                                uint16_t * __RESTRICT pTargetBase,
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

__WEAK 
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
                        (in != hwColour));
}

__WEAK void __arm_2d_rgb16_2x1_paving_x_mirror(const uint16_t * __RESTRICT pSourceBase,
                                        int16_t iSourceStride,
                                        const arm_2d_size_t * __RESTRICT ptSourceSize,
                                        uint16_t * __RESTRICT pTargetBase,
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

__WEAK void __arm_2d_rgb16_2x1_paving_y_mirror(const uint16_t * __RESTRICT pSourceBase,
                                        int16_t iSourceStride,
                                        const arm_2d_size_t * __RESTRICT ptSourceSize,
                                        uint16_t * __RESTRICT pTargetBase,
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


__WEAK void __arm_2d_rgb16_2x1_paving_xy_mirror(const uint16_t * __RESTRICT pSourceBase,
                                        int16_t iSourceStride,
                                        const arm_2d_size_t * __RESTRICT ptSourceSize,
                                        uint16_t * __RESTRICT pTargetBase,
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



__WEAK 
void __arm_2d_rgb16_1x1_paving(const uint16_t * __RESTRICT pSource,
                               int16_t iSourceStride,
                               uint16_t * __RESTRICT pTarget,
                               int16_t iTargetStride,
                               const arm_2d_size_t * __RESTRICT ptSrcCopySize,
                               const arm_2d_size_t * __RESTRICT ptDstCopySize)
{
    __ARM_2D_PAVING_1x1(16,
                        PAVING_DIRECT_START_OFFS(_, _),
                        PAVING_DIRECT_READ_DIR,
                        PAVING_DIRECT_SETUP_COPY(16),
                        PAVING_DIRECT_LOAD_PATTERN(16));
}

__WEAK 
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
                        (in != hwColour));
}

__WEAK void __arm_2d_rgb16_1x1_paving_x_mirror(const uint16_t * __RESTRICT pSource,
                                       int16_t iSourceStride,
                                       uint16_t * __RESTRICT pTarget,
                                       int16_t iTargetStride,
                                       const arm_2d_size_t * __RESTRICT ptSrcCopySize,
                                       const arm_2d_size_t * __RESTRICT ptDstCopySize)
{
    __ARM_2D_PAVING_1x1(16,
                        PAVING_X_MIRROR_START_OFFS(_, _),
                        PAVING_X_MIRROR_READ_DIR, PAVING_X_MIRROR_SETUP_COPY(16),
                        PAVING_X_MIRROR_LOAD_PATTERN(16));
}


__WEAK void __arm_2d_rgb16_1x1_paving_y_mirror(const uint16_t * __RESTRICT pSource,
                                       int16_t iSourceStride,
                                       uint16_t * __RESTRICT pTarget,
                                       int16_t iTargetStride,
                                       const arm_2d_size_t * __RESTRICT ptSrcCopySize,
                                       const arm_2d_size_t * __RESTRICT ptDstCopySize)
{
    __ARM_2D_PAVING_1x1(16,
                        PAVING_Y_MIRROR_START_OFFS(iSourceStride, ptSrcCopySize->iHeight),
                        PAVING_Y_MIRROR_READ_DIR,
                        PAVING_Y_MIRROR_SETUP_COPY(16),
                        PAVING_Y_MIRROR_LOAD_PATTERN(16));
}


__WEAK void __arm_2d_rgb16_1x1_paving_xy_mirror(const uint16_t * __RESTRICT pSource,
                                           int16_t iSourceStride,
                                           uint16_t * __RESTRICT pTarget,
                                           int16_t iTargetStride,
                                           const arm_2d_size_t * __RESTRICT ptSrcCopySize,
                                           const arm_2d_size_t * __RESTRICT ptDstCopySize)
{
    __ARM_2D_PAVING_1x1(16,
                        PAVING_XY_MIRROR_START_OFFS(iSourceStride, ptSrcCopySize->iHeight),
                        PAVING_XY_MIRROR_READ_DIR,
                        PAVING_XY_MIRROR_SETUP_COPY(16),
                        PAVING_XY_MIRROR_LOAD_PATTERN(16));
}




__WEAK 
void __arm_2d_rgb32_2x2_paving( const uint32_t *__RESTRICT pSourceBase,
                                int16_t iSourceStride,
                                const arm_2d_size_t * __RESTRICT ptSourceSize,
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

__WEAK
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
                        (in != wColour));
}


__WEAK 
void __arm_2d_rgb32_2x2_paving_x_mirror(const uint32_t *__RESTRICT pSourceBase,
                                        int16_t iSourceStride,
                                        const arm_2d_size_t * __RESTRICT ptSourceSize,
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


__WEAK void __arm_2d_rgb32_2x2_paving_y_mirror(const uint32_t *__RESTRICT pSourceBase,
                                        int16_t iSourceStride,
                                        const arm_2d_size_t * __RESTRICT ptSourceSize,
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

__WEAK void __arm_2d_rgb32_2x2_paving_xy_mirror(const uint32_t *__RESTRICT pSourceBase,
                                        int16_t iSourceStride,
                                        const arm_2d_size_t * __RESTRICT ptSourceSize,
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


__WEAK 
void __arm_2d_rgb32_1x2_paving(const uint32_t * __RESTRICT pSourceBase,
                               int16_t iSourceStride,
                               const arm_2d_size_t * __RESTRICT ptSourceSize,
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

__WEAK 
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
                        (in != wColour));
}

__WEAK void __arm_2d_rgb32_1x2_paving_x_mirror(const uint32_t * __RESTRICT pSourceBase,
                                        int16_t iSourceStride,
                                        const arm_2d_size_t * __RESTRICT ptSourceSize,
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

__WEAK void __arm_2d_rgb32_1x2_paving_y_mirror(const uint32_t * __RESTRICT pSourceBase,
                                       int16_t iSourceStride,
                                       const arm_2d_size_t * __RESTRICT ptSourceSize,
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


__WEAK void __arm_2d_rgb32_1x2_paving_xy_mirror(const uint32_t * __RESTRICT pSourceBase,
                                       int16_t iSourceStride,
                                       const arm_2d_size_t * __RESTRICT ptSourceSize,
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


__WEAK 
void __arm_2d_rgb32_2x1_paving(const uint32_t * __RESTRICT pSourceBase,
                               int16_t iSourceStride,
                               const arm_2d_size_t * __RESTRICT ptSourceSize,
                               uint32_t * __RESTRICT pTargetBase,
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

__WEAK 
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
                        (in != wColour));
}

__WEAK void __arm_2d_rgb32_2x1_paving_x_mirror(const uint32_t * __RESTRICT pSourceBase,
                                        int16_t iSourceStride,
                                        const arm_2d_size_t * __RESTRICT ptSourceSize,
                                        uint32_t * __RESTRICT pTargetBase,
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

__WEAK void __arm_2d_rgb32_2x1_paving_y_mirror(const uint32_t * __RESTRICT pSourceBase,
                                        int16_t iSourceStride,
                                        const arm_2d_size_t * __RESTRICT ptSourceSize,
                                        uint32_t * __RESTRICT pTargetBase,
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


__WEAK void __arm_2d_rgb32_2x1_paving_xy_mirror(const uint32_t * __RESTRICT pSourceBase,
                                        int16_t iSourceStride,
                                        const arm_2d_size_t * __RESTRICT ptSourceSize,
                                        uint32_t * __RESTRICT pTargetBase,
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

__WEAK 
void __arm_2d_rgb32_1x1_paving( const uint32_t * __RESTRICT pSource,
                                int16_t iSourceStride,
                                uint32_t * __RESTRICT pTarget,
                                int16_t iTargetStride,
                                const arm_2d_size_t * __RESTRICT ptSrcCopySize,
                                const arm_2d_size_t * __RESTRICT ptDstCopySize)
{
    __ARM_2D_PAVING_1x1(32,
                        PAVING_DIRECT_START_OFFS(_, _),
                        PAVING_DIRECT_READ_DIR,
                        PAVING_DIRECT_SETUP_COPY(32),
                        PAVING_DIRECT_LOAD_PATTERN(32));
}

__WEAK 
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
                        (in != wColour));
}

__WEAK void __arm_2d_rgb32_1x1_paving_x_mirror(const uint32_t * __RESTRICT pSource,
                                       int16_t iSourceStride,
                                       uint32_t * __RESTRICT pTarget,
                                       int16_t iTargetStride,
                                       const arm_2d_size_t * __RESTRICT ptSrcCopySize,
                                       const arm_2d_size_t * __RESTRICT ptDstCopySize)
{
    __ARM_2D_PAVING_1x1(32,
                        PAVING_X_MIRROR_START_OFFS(_, _),
                        PAVING_X_MIRROR_READ_DIR, PAVING_X_MIRROR_SETUP_COPY(32),
                        PAVING_X_MIRROR_LOAD_PATTERN(32));
}


__WEAK void __arm_2d_rgb32_1x1_paving_y_mirror(const uint32_t * __RESTRICT pSource,
                                       int16_t iSourceStride,
                                       uint32_t * __RESTRICT pTarget,
                                       int16_t iTargetStride,
                                       const arm_2d_size_t * __RESTRICT ptSrcCopySize,
                                       const arm_2d_size_t * __RESTRICT ptDstCopySize)
{
    __ARM_2D_PAVING_1x1(32,
                        PAVING_Y_MIRROR_START_OFFS(iSourceStride, ptSrcCopySize->iHeight),
                        PAVING_Y_MIRROR_READ_DIR,
                        PAVING_Y_MIRROR_SETUP_COPY(32),
                        PAVING_Y_MIRROR_LOAD_PATTERN(32));
}


__WEAK void __arm_2d_rgb32_1x1_paving_xy_mirror(const uint32_t * __RESTRICT pSource,
                                       int16_t iSourceStride,
                                       uint32_t * __RESTRICT pTarget,
                                       int16_t iTargetStride,
                                       const arm_2d_size_t * __RESTRICT ptSrcCopySize,
                                       const arm_2d_size_t * __RESTRICT ptDstCopySize)
{
    __ARM_2D_PAVING_1x1(32,
                        PAVING_XY_MIRROR_START_OFFS(iSourceStride, ptSrcCopySize->iHeight),
                        PAVING_XY_MIRROR_READ_DIR,
                        PAVING_XY_MIRROR_SETUP_COPY(32),
                        PAVING_XY_MIRROR_LOAD_PATTERN(32));
}




__WEAK void __arm_2d_impl_rgb16_fill(uint16_t * __RESTRICT pSourceBase,
                             int16_t iSourceStride,
                             arm_2d_size_t * __RESTRICT ptSourceSize,
                             uint16_t * __RESTRICT pTargetBase,
                             int16_t iTargetStride,
                             arm_2d_size_t * __RESTRICT ptTargetSize)
{
    static const arm_2d_rgb16_paving_fct_t pav_func = {
        .pav_2x2 = &__arm_2d_rgb16_2x2_paving,
        .pav_1x2 = &__arm_2d_rgb16_1x2_paving,
        .pav_2x1 = &__arm_2d_rgb16_2x1_paving,
        .pav_1x1 = &__arm_2d_rgb16_1x1_paving
    };

    __arm_2d_impl_rgb16_fill_generic(pSourceBase,
                                    iSourceStride,
                                    ptSourceSize,
                                    pTargetBase,
                                    iTargetStride, 
                                    ptTargetSize, 
                                    &pav_func);
}


__WEAK void __arm_2d_impl_rgb16_fill_x_mirror(uint16_t * __RESTRICT pSourceBase,
                                      int16_t iSourceStride,
                                      arm_2d_size_t * __RESTRICT ptSourceSize,
                                      uint16_t * __RESTRICT pTargetBase,
                                      int16_t iTargetStride,
                                      arm_2d_size_t * __RESTRICT ptTargetSize)
{
    static arm_2d_rgb16_paving_fct_t pav_func = {
        .pav_2x2 = &__arm_2d_rgb16_2x2_paving_x_mirror,
        .pav_1x2 = &__arm_2d_rgb16_1x2_paving_x_mirror,
        .pav_2x1 = &__arm_2d_rgb16_2x1_paving_x_mirror,
        .pav_1x1 = &__arm_2d_rgb16_1x1_paving_x_mirror
    };

    __arm_2d_impl_rgb16_fill_generic(pSourceBase,
                                    iSourceStride,
                                    ptSourceSize,
                                    pTargetBase,
                                    iTargetStride, ptTargetSize, &pav_func);
}


__WEAK void __arm_2d_impl_rgb16_fill_y_mirror(uint16_t * __RESTRICT pSourceBase,
                                      int16_t iSourceStride,
                                      arm_2d_size_t * __RESTRICT ptSourceSize,
                                      uint16_t * __RESTRICT pTargetBase,
                                      int16_t iTargetStride,
                                      arm_2d_size_t * __RESTRICT ptTargetSize)
{
    static arm_2d_rgb16_paving_fct_t pav_func = {
        .pav_2x2 = &__arm_2d_rgb16_2x2_paving_y_mirror,
        .pav_1x2 = &__arm_2d_rgb16_1x2_paving_y_mirror,
        .pav_2x1 = &__arm_2d_rgb16_2x1_paving_y_mirror,
        .pav_1x1 = &__arm_2d_rgb16_1x1_paving_y_mirror
    };

    __arm_2d_impl_rgb16_fill_generic(pSourceBase,
                                    iSourceStride,
                                    ptSourceSize,
                                    pTargetBase,
                                    iTargetStride, ptTargetSize, &pav_func);
}


__WEAK void __arm_2d_impl_rgb16_fill_xy_mirror(uint16_t * __RESTRICT pSourceBase,
                                       int16_t iSourceStride,
                                       arm_2d_size_t * __RESTRICT ptSourceSize,
                                       uint16_t * __RESTRICT pTargetBase,
                                       int16_t iTargetStride,
                                       arm_2d_size_t * __RESTRICT ptTargetSize)
{
    static arm_2d_rgb16_paving_fct_t pav_func = {
        .pav_2x2 = &__arm_2d_rgb16_2x2_paving_xy_mirror,
        .pav_1x2 = &__arm_2d_rgb16_1x2_paving_xy_mirror,
        .pav_2x1 = &__arm_2d_rgb16_2x1_paving_xy_mirror,
        .pav_1x1 = &__arm_2d_rgb16_1x1_paving_xy_mirror
    };

    __arm_2d_impl_rgb16_fill_generic(pSourceBase,
                                    iSourceStride,
                                    ptSourceSize,
                                    pTargetBase,
                                    iTargetStride, ptTargetSize, &pav_func);
}

__WEAK void __arm_2d_impl_rgb16_copy_x_mirror(uint16_t * __RESTRICT phwSource,
                                      int16_t iSourceStride,
                                      uint16_t * __RESTRICT phwTarget,
                                      int16_t iTargetStride,
                                      arm_2d_size_t * __RESTRICT ptCopySize)
{
    __arm_2d_rgb16_1x1_paving_x_mirror( phwSource,
                                        iSourceStride,
                                        phwTarget,
                                        iTargetStride,
                                        ptCopySize,
                                        ptCopySize);
}


__WEAK void __arm_2d_impl_rgb32_copy_x_mirror(uint32_t * __RESTRICT pwSource,
                                        int16_t iSourceStride,
                                        uint32_t * __RESTRICT pwTarget,
                                        int16_t iTargetStride,
                                        arm_2d_size_t * __RESTRICT ptCopySize)
{
    __arm_2d_rgb32_1x1_paving_x_mirror( pwSource,
                                        iSourceStride,
                                        pwTarget,
                                        iTargetStride,
                                        ptCopySize,
                                        ptCopySize);
}


__WEAK void __arm_2d_impl_rgb16_copy_y_mirror(uint16_t * __RESTRICT phwSource,
                                        int16_t iSourceStride,
                                        uint16_t * __RESTRICT phwTarget,
                                        int16_t iTargetStride,
                                        arm_2d_size_t * __RESTRICT ptCopySize)
{
    __arm_2d_rgb16_1x1_paving_y_mirror( phwSource,
                                        iSourceStride,
                                        phwTarget,
                                        iTargetStride,
                                        ptCopySize,
                                        ptCopySize);
}

__WEAK void __arm_2d_impl_rgb32_copy_y_mirror(uint32_t * __RESTRICT pwSource,
                                        int16_t iSourceStride,
                                        uint32_t * __RESTRICT pwTarget,
                                        int16_t iTargetStride,
                                        arm_2d_size_t * __RESTRICT ptCopySize)
{
    __arm_2d_rgb32_1x1_paving_y_mirror( pwSource,
                                        iSourceStride,
                                        pwTarget,
                                        iTargetStride,
                                        ptCopySize,
                                        ptCopySize);
}

__WEAK void __arm_2d_impl_rgb16_copy_xy_mirror(uint16_t * __RESTRICT phwSource,
                                        int16_t iSourceStride,
                                        uint16_t * __RESTRICT phwTarget,
                                        int16_t iTargetStride,
                                        arm_2d_size_t * __RESTRICT ptCopySize)
{
    __arm_2d_rgb16_1x1_paving_xy_mirror(phwSource,
                                        iSourceStride,
                                        phwTarget,
                                        iTargetStride,
                                        ptCopySize,
                                        ptCopySize);

}


__WEAK void __arm_2d_impl_rgb32_copy_xy_mirror(uint32_t * __RESTRICT pwSource,
                                        int16_t iSourceStride,
                                        uint32_t * __RESTRICT pwTarget,
                                        int16_t iTargetStride,
                                        arm_2d_size_t * __RESTRICT ptCopySize)
{
    __arm_2d_rgb32_1x1_paving_xy_mirror(pwSource,
                                        iSourceStride,
                                        pwTarget,
                                        iTargetStride,
                                        ptCopySize,
                                        ptCopySize);
}


__WEAK void __arm_2d_impl_rgb32_fill(uint32_t * __RESTRICT pSourceBase,
                             int16_t iSourceStride,
                             arm_2d_size_t * __RESTRICT ptSourceSize,
                             uint32_t * __RESTRICT pTargetBase,
                             int16_t iTargetStride,
                             arm_2d_size_t * __RESTRICT ptTargetSize)
{
    const arm_2d_rgb32_paving_fct_t pav_func = {
        .pav_2x2 = &__arm_2d_rgb32_2x2_paving,
        .pav_1x2 = &__arm_2d_rgb32_1x2_paving,
        .pav_2x1 = &__arm_2d_rgb32_2x1_paving,
        .pav_1x1 = &__arm_2d_rgb32_1x1_paving
    };

    __arm_2d_impl_rgb32_fill_generic(pSourceBase,
                                    iSourceStride,
                                    ptSourceSize,
                                    pTargetBase,
                                    iTargetStride, 
                                    ptTargetSize,
                                    &pav_func);
}


__WEAK void __arm_2d_impl_rgb32_fill_x_mirror(uint32_t * __RESTRICT pSourceBase,
                                      int16_t iSourceStride,
                                      arm_2d_size_t * __RESTRICT ptSourceSize,
                                      uint32_t * __RESTRICT pTargetBase,
                                      int16_t iTargetStride,
                                      arm_2d_size_t * __RESTRICT ptTargetSize)
{
    const arm_2d_rgb32_paving_fct_t pav_func = {
        .pav_2x2 = &__arm_2d_rgb32_2x2_paving_x_mirror,
        .pav_1x2 = &__arm_2d_rgb32_1x2_paving_x_mirror,
        .pav_2x1 = &__arm_2d_rgb32_2x1_paving_x_mirror,
        .pav_1x1 = &__arm_2d_rgb32_1x1_paving_x_mirror
    };

    __arm_2d_impl_rgb32_fill_generic(pSourceBase,
                                    iSourceStride,
                                    ptSourceSize,
                                    pTargetBase,
                                    iTargetStride, ptTargetSize, &pav_func);
}

__WEAK 
void __arm_2d_impl_rgb32_fill_y_mirror(uint32_t * __RESTRICT pSourceBase,
                                      int16_t iSourceStride,
                                      arm_2d_size_t * __RESTRICT ptSourceSize,
                                      uint32_t * __RESTRICT pTargetBase,
                                      int16_t iTargetStride,
                                      arm_2d_size_t * __RESTRICT ptTargetSize)
{
    const arm_2d_rgb32_paving_fct_t pav_func = {
        .pav_2x2 = &__arm_2d_rgb32_2x2_paving_y_mirror,
        .pav_1x2 = &__arm_2d_rgb32_1x2_paving_y_mirror,
        .pav_2x1 = &__arm_2d_rgb32_2x1_paving_y_mirror,
        .pav_1x1 = &__arm_2d_rgb32_1x1_paving_y_mirror
    };

    __arm_2d_impl_rgb32_fill_generic(pSourceBase,
                                    iSourceStride,
                                    ptSourceSize,
                                    pTargetBase,
                                    iTargetStride, ptTargetSize, &pav_func);
}


__WEAK 
void __arm_2d_impl_rgb32_fill_xy_mirror(uint32_t * __RESTRICT pSourceBase,
                                       int16_t iSourceStride,
                                       arm_2d_size_t * __RESTRICT ptSourceSize,
                                       uint32_t * __RESTRICT pTargetBase,
                                       int16_t iTargetStride,
                                       arm_2d_size_t * __RESTRICT ptTargetSize)
{
    const arm_2d_rgb32_paving_fct_t pav_func = {
        .pav_2x2 = &__arm_2d_rgb32_2x2_paving_xy_mirror,
        .pav_1x2 = &__arm_2d_rgb32_1x2_paving_xy_mirror,
        .pav_2x1 = &__arm_2d_rgb32_2x1_paving_xy_mirror,
        .pav_1x1 = &__arm_2d_rgb32_1x1_paving_xy_mirror
    };

    __arm_2d_impl_rgb32_fill_generic(pSourceBase,
                                    iSourceStride,
                                    ptSourceSize,
                                    pTargetBase,
                                    iTargetStride, 
                                    ptTargetSize, 
                                    &pav_func);
}



__WEAK void __arm_2d_impl_rgb16_fill_colour_masking(
                                        uint16_t * __RESTRICT pSourceBase,
                                        int16_t iSourceStride,
                                        arm_2d_size_t * __RESTRICT ptSourceSize,
                                        uint16_t * __RESTRICT pTargetBase,
                                        int16_t iTargetStride,
                                        arm_2d_size_t * __RESTRICT ptTargetSize,
                                        uint_fast16_t hwColour)
{
    static const arm_2d_rgb16_cl_msk_paving_fct_t pav_func = {
        .pav_2x2 = &__arm_2d_rgb16_2x2_cl_msk_paving,
        .pav_1x2 = &__arm_2d_rgb16_1x2_cl_msk_paving,
        .pav_2x1 = &__arm_2d_rgb16_2x1_cl_msk_paving,
        .pav_1x1 = &__arm_2d_rgb16_1x1_cl_msk_paving,
    };

    __arm_2d_rgb16_mem_fill_colour_masking_generic( pSourceBase,
                                                    iSourceStride,
                                                    ptSourceSize,
                                                    pTargetBase,
                                                    iTargetStride, 
                                                    ptTargetSize, 
                                                    &pav_func,
                                                    hwColour);
}

__WEAK void __arm_2d_impl_rgb32_fill_colour_masking(
                                        uint32_t * __RESTRICT pSourceBase,
                                        int16_t iSourceStride,
                                        arm_2d_size_t * __RESTRICT ptSourceSize,
                                        uint32_t * __RESTRICT pTargetBase,
                                        int16_t iTargetStride,
                                        arm_2d_size_t * __RESTRICT ptTargetSize,
                                        uint32_t wColour)
{
    static const arm_2d_rgb32_cl_msk_paving_fct_t pav_func = {
        .pav_2x2 = &__arm_2d_rgb32_2x2_cl_msk_paving,
        .pav_1x2 = &__arm_2d_rgb32_1x2_cl_msk_paving,
        .pav_2x1 = &__arm_2d_rgb32_2x1_cl_msk_paving,
        .pav_1x1 = &__arm_2d_rgb32_1x1_cl_msk_paving,
    };

    __arm_2d_impl_rgb32_fill_colour_masking_generic( pSourceBase,
                                                    iSourceStride,
                                                    ptSourceSize,
                                                    pTargetBase,
                                                    iTargetStride, 
                                                    ptTargetSize, 
                                                    &pav_func,
                                                    wColour);
}


#endif





#if defined(__clang__)
#   pragma clang diagnostic pop
#endif

#ifdef   __cplusplus
}
#endif

