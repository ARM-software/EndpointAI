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

/*============================ MACROS ========================================*/
#ifndef PBF_BLOCK_SIZE
#   define PBF_BLOCK_SIZE       20
#endif

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/

declare_tile(c_tPartialFrameBuffer)    

implement_tile(c_tPartialFrameBuffer, PBF_BLOCK_SIZE, PBF_BLOCK_SIZE, arm_2d_color_rgb565_t);
static struct {
    arm_2d_region_t tDrawRegion;
    arm_2d_tile_t   tPFBTile;
} s_tPFBController = {
    .tDrawRegion = {
        .tSize = {PBF_BLOCK_SIZE, PBF_BLOCK_SIZE},
    },
};

/*============================ PROTOTYPES ====================================*/
/*============================ IMPLEMENTATION ================================*/

void platform_disp_fill_tile(const arm_2d_tile_t *ptTile, uint_fast16_t hwColor)
{
    DISP_ADAPT_ASSERT(NULL != ptTile);
    arm_2d_region_t tValidRegion;
    ptTile = arm_2d_tile_get_root(ptTile, &tValidRegion, NULL);
    
    uint_fast32_t n = tValidRegion.tSize.iHeight * tValidRegion.tSize.iWidth;
    uint16_t *phwSrc = ptTile->phwBuffer;
    
    do {
        *phwSrc++ = (uint16_t)hwColor;
    } while(--n);
}

void platform_disp_init(void)
{
    platform_disp_fill_tile(&c_tPartialFrameBuffer, GLCD_COLOR_BLACK);
}


arm_fsm_rt_t platform_disp_buffer_refresh(void)
{
    
    GLCD_DrawBitmap(s_tPFBController.tDrawRegion.tLocation.iX,
                    s_tPFBController.tDrawRegion.tLocation.iY, 
                    s_tPFBController.tDrawRegion.tSize.iWidth, 
                    s_tPFBController.tDrawRegion.tSize.iHeight, 
                    c_tPartialFrameBuffer.pchBuffer);
    
    return arm_fsm_rt_cpl;
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
    arm_2d_region_t tTempRegion = {
        .tSize = { 320, 240 },
        .tLocation = {
            .iX = -s_tPFBController.tDrawRegion.tLocation.iX,
            .iY = -s_tPFBController.tDrawRegion.tLocation.iY,
        },
    };
    
    arm_2d_tile_generate_child( &c_tPartialFrameBuffer, 
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

    s_tPFBController.tDrawRegion.tLocation.iX += PBF_BLOCK_SIZE;
    if (s_tPFBController.tDrawRegion.tLocation.iX >= 320) {
        s_tPFBController.tDrawRegion.tLocation.iY += PBF_BLOCK_SIZE;
        s_tPFBController.tDrawRegion.tLocation.iX = 0;
        
        if (s_tPFBController.tDrawRegion.tLocation.iY >= 240) {
            //! finished
            s_tPFBController.tDrawRegion.tLocation.iY = 0;
            return false;
        }
    }

    return true;
}

