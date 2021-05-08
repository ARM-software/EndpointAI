/*
 * Copyright (C) 2020 Arm Limited or its affiliates. All rights reserved.
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
 * Title:        #include "arm_2d_rotation.h"
 * Description:  Public header file to contain the APIs for rotation
 *
 * $Date:        29 April 2021
 * $Revision:    V.0.5.0
 *
 * Target Processor:  Cortex-M cores
 * -------------------------------------------------------------------- */

#ifndef __ARM_2D_ROTATION_H__
#define __ARM_2D_ROTATION_H__

/*============================ INCLUDES ======================================*/

#include "arm_2d_types.h"

#ifdef   __cplusplus
extern "C" {
#endif

#if defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wsign-conversion"
#endif

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/

typedef struct __arm_2d_rotate_info_t {
    float                   fAngle;         //!< target angle
    arm_2d_location_t       tCenter;
    arm_2d_location_t       tDummySourceOffset;
    arm_2d_region_t         tTargetRegion;
    union {
        uint32_t            wColour;
        uint16_t            hwColour;
    } Mask;                                 //!< the colour to fill when out of range
} __arm_2d_rotate_info_t;

/*! \brief arm_2d_op_rotat_t is inherit from arm_2d_op_src_orig_t
 */
typedef struct arm_2d_op_rotate_t {
    inherit(arm_2d_op_core_t);
    struct {
        const arm_2d_tile_t     *ptTile;        //!< target tile
        const arm_2d_region_t   *ptRegion;      //!< target region
    } Target;
    struct {
        const arm_2d_tile_t     *ptTile;        //!< source tile
    }Source;
    uint32_t wMode;
    
    struct {
        const arm_2d_tile_t     *ptTile;        //!< the origin tile
        arm_2d_tile_t           tDummySource;   //!< the buffer for the source
    }Origin;
    
    __arm_2d_rotate_info_t      tRotate;
    
} arm_2d_op_rotate_t;

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/


extern
ARM_NONNULL(1,2)
arm_2d_err_t arm_2d_rgb565_tile_rotation_prepare(
                                        const arm_2d_tile_t *ptSource,
                                        const arm_2d_tile_t *ptTarget,
                                        const arm_2d_region_t *ptRegion,
                                        const arm_2d_location_t tCentre,
                                        float fAngle,
                                        uint16_t hwFillColour);
                                     
extern
ARM_NONNULL(1,2)
arm_2d_err_t arm_2d_rgb888_tile_rotation_prepare(
                                        const arm_2d_tile_t *ptSource,
                                        const arm_2d_tile_t *ptTarget,
                                        const arm_2d_region_t *ptRegion,
                                        const arm_2d_location_t tCentre,
                                        float fAngle,
                                        uint32_t wFillColour);

extern
arm_fsm_rt_t arm_2d_tile_rotate(void);

#if defined(__clang__)
#   pragma clang diagnostic pop
#endif

#ifdef   __cplusplus
}
#endif

#endif
