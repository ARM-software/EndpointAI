/*
 * Copyright (c) 2009-2020 Arm Limited. All rights reserved.
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

/*============================ INCLUDES ======================================*/
#include "./app_cfg.h"
#include <stdbool.h>
#include "./display_adapter.h"
#include "../platform/platform.h"
#include <assert.h>

#if defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wcast-qual"
#   pragma clang diagnostic ignored "-Wsign-conversion"
#   pragma clang diagnostic ignored "-Wpadded"
#endif

/*============================ MACROS ========================================*/
#if defined(__STDC_VERSION__) && __STDC_VERSION__>=201112L
//! C11
static_assert(  APP_SCREEN_WIDTH <= GLCD_WIDTH, 
                "APP_SCREEN_WIDTH should be no larger than GLCD_WIDTH");

static_assert(  APP_SCREEN_HEIGHT <= GLCD_HEIGHT, 
                "APP_SCREEN_HEIGHT should be no larger than GLCD_HEIGHT");
#endif

#ifndef PBF_BLOCK_SIZE
#   define PBF_BLOCK_SIZE       20
#endif

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/

static arm_2d_tile_t s_tPartialFrameBuffer;

ARM_NOINIT static arm_2d_color_rgb565_t                                            
    s_tPartialFrameBufferBuffer[(PBF_BLOCK_SIZE_X) * (PBF_BLOCK_SIZE_Y)];           
static  arm_2d_tile_t s_tPartialFrameBuffer = {                                      
    .tRegion = {                                                    
        .tSize = {(PBF_BLOCK_SIZE_X), (PBF_BLOCK_SIZE_Y)},                           
    },                                                              
    .tInfo.bIsRoot = true,                                          
    .pchBuffer = (uint8_t *)s_tPartialFrameBufferBuffer,                        
};

static const arm_2d_region_t c_tLCDRegion = {
        .tSize = {
            .iWidth     = APP_SCREEN_WIDTH,
            .iHeight    = APP_SCREEN_HEIGHT,
        },
    };
                
static struct {
    arm_2d_region_t tDrawRegion;
    arm_2d_region_t tTargetRegion;
    arm_2d_tile_t   tPFBTile;
    bool            bFirstIteration;
} s_tPFBController = {
    .tDrawRegion = {
        .tSize = {PBF_BLOCK_SIZE_X, PBF_BLOCK_SIZE_Y},
    },
};

/*============================ PROTOTYPES ====================================*/
/*============================ IMPLEMENTATION ================================*/


void platform_disp_init(void)
{
    s_tPFBController.bFirstIteration = true;
}


arm_fsm_rt_t platform_disp_buffer_refresh(void)
{
    
    GLCD_DrawBitmap(((GLCD_WIDTH - APP_SCREEN_WIDTH) >> 1) 
                     +  s_tPFBController.tDrawRegion.tLocation.iX
                     +  s_tPFBController.tTargetRegion.tLocation.iX,
                    ((GLCD_HEIGHT - APP_SCREEN_HEIGHT) >> 1) 
                     +  s_tPFBController.tDrawRegion.tLocation.iY
                     +  s_tPFBController.tTargetRegion.tLocation.iY,
                    s_tPartialFrameBuffer.tRegion.tSize.iWidth, 
                    s_tPartialFrameBuffer.tRegion.tSize.iHeight, 
                    s_tPartialFrameBuffer.pchBuffer);
    
    return arm_fsm_rt_cpl;
}

/*! \brief wait for LCD ready 
 */
void platform_wait_for_disp_ready(void)
{
    return ;
}

/*! \brief begin a iteration of drawing and request a frame buffer from 
 *!        low level display driver.
 *! \param ptTargetRegion the address of the target region in the LCD
 *!        passing NULL means we want to draw the whole LCD.
 *! \retval NULL the display driver is not ready
 *! \retval (intptr_t)-1 the display driver want to ignore this drawing 
 *!         (maybe because the target area is out of the LCD)
 *! \retval non-null a tile which contains the (partial) frame buffer
 */
arm_2d_tile_t * drawing_iteration_begin(arm_2d_region_t *ptTargetRegion)
{
    
    if (s_tPFBController.bFirstIteration) {
        
        if (NULL != ptTargetRegion) {
            
            //! calculate the valid region
            if (!arm_2d_region_intersect(   &c_tLCDRegion, 
                                            ptTargetRegion, 
                                            &s_tPFBController.tTargetRegion)) {
                //! out of lcd 
                return (arm_2d_tile_t *)-1;
            }
        } else {
            s_tPFBController.tTargetRegion = c_tLCDRegion;
        }
        
        s_tPFBController.bFirstIteration = false;
    }
    
    arm_2d_region_t tTempRegion = {
        .tSize = {APP_SCREEN_WIDTH, APP_SCREEN_HEIGHT},
        .tLocation = {
            .iX = - (s_tPFBController.tTargetRegion.tLocation.iX + s_tPFBController.tDrawRegion.tLocation.iX),
            .iY = - (s_tPFBController.tTargetRegion.tLocation.iY + s_tPFBController.tDrawRegion.tLocation.iY),
        },
    };
    
    
    s_tPartialFrameBuffer.tRegion.tSize.iWidth 
        = MIN(  PBF_BLOCK_SIZE_X, s_tPFBController.tTargetRegion.tSize.iWidth 
            -   s_tPFBController.tDrawRegion.tLocation.iX);
    s_tPartialFrameBuffer.tRegion.tSize.iHeight 
        = MIN(  PBF_BLOCK_SIZE_Y, s_tPFBController.tTargetRegion.tSize.iHeight 
            -   s_tPFBController.tDrawRegion.tLocation.iY);
            
    arm_2d_tile_generate_child( &s_tPartialFrameBuffer, 
                                &tTempRegion, 
                                &s_tPFBController.tPFBTile, 
                                false);

    return (arm_2d_tile_t *)&(s_tPFBController.tPFBTile);
}

/*! \brief end a drawing iteration and decide wether a new iteration is required
 *!        or not based on the return value
 *! \param none
 *! \retval true a new iteration is required
 *! \retval false no more iteration is required
 */
bool drawing_iteration_end(void)
{
    platform_disp_buffer_refresh();

    s_tPFBController.tDrawRegion.tLocation.iX += PBF_BLOCK_SIZE_X;
    if (s_tPFBController.tDrawRegion.tLocation.iX >= s_tPFBController.tTargetRegion.tSize.iWidth) {
        s_tPFBController.tDrawRegion.tLocation.iY += PBF_BLOCK_SIZE_Y;
        s_tPFBController.tDrawRegion.tLocation.iX = 0;
        
        if (s_tPFBController.tDrawRegion.tLocation.iY >= s_tPFBController.tTargetRegion.tSize.iHeight) {
            //! finished
            s_tPFBController.tDrawRegion.tLocation.iY = 0;
            s_tPFBController.bFirstIteration = true;
            return false;
        }
    }

    return true;
}

