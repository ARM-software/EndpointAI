/*
 * Copyright (c) 2009-2021 Arm Limited. All rights reserved.
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
#include "./busy_wheel.h"
#include "arm_2d.h"
#include "arm_2d_helper.h"
#include "platform.h"
#include <math.h>

#if defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wsign-conversion"
#   pragma clang diagnostic ignored "-Wpadded"
#   pragma clang diagnostic ignored "-Wcast-qual"
#   pragma clang diagnostic ignored "-Wcast-align"
#   pragma clang diagnostic ignored "-Wmissing-field-initializers"
#   pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#   pragma clang diagnostic ignored "-Wmissing-braces"
#   pragma clang diagnostic ignored "-Wunused-const-variable"
#   pragma clang diagnostic ignored "-Wmissing-declarations"
#   pragma clang diagnostic ignored "-Wmissing-variable-declarations"
#   pragma clang diagnostic ignored "-Winitializer-overrides"
#endif

/*============================ MACROS ========================================*/

#define PANEL_WIDTH         100
#define PANEL_HEIGHT        100


/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
extern uint32_t SystemCoreClock;
extern
const arm_2d_tile_t c_tileWhiteDot;
extern
const arm_2d_tile_t c_tileWhiteDotAlphaQuarter;

/*============================ PROTOTYPES ====================================*/
__attribute__((nothrow)) 
extern int64_t clock(void);

/*============================ LOCAL VARIABLES ===============================*/

declare_tile(c_tPanel)
implement_tile(c_tPanel, PANEL_WIDTH, PANEL_HEIGHT, uint8_t,
    .tInfo = {
        .bIsRoot = true,
        .bHasEnforcedColour = true,
        .tColourInfo = {
            .chScheme = ARM_2D_COLOUR_8BIT,
        },
    },
)

/*============================ IMPLEMENTATION ================================*/


void spinning_wheel_init(void)
{
    arm_2d_region_t tRegion = {
        .tSize = c_tileWhiteDotAlphaQuarter.tRegion.tSize,
    };
    
    //! fill the alpha mask
    arm_2d_c8bit_fill_colour(&c_tPanel, NULL, 0xFF);
    
    //! copy the top left corner
    arm_2d_c8bit_tile_copy( &c_tileWhiteDotAlphaQuarter, 
                            &c_tPanel, 
                            NULL, 
                            ARM_2D_CP_MODE_COPY);
                            
    //! copy the top right corner
    tRegion.tLocation.iX = PANEL_WIDTH - tRegion.tSize.iWidth;
    arm_2d_c8bit_tile_copy( &c_tileWhiteDotAlphaQuarter, 
                            &c_tPanel, 
                            &tRegion, 
                            ARM_2D_CP_MODE_COPY |
                            ARM_2D_CP_MODE_X_MIRROR);
                            
    //! copy the bottom right corner 
    tRegion.tLocation.iY = PANEL_HEIGHT - tRegion.tSize.iHeight;
    arm_2d_c8bit_tile_copy( &c_tileWhiteDotAlphaQuarter, 
                            &c_tPanel, 
                            &tRegion, 
                            ARM_2D_CP_MODE_COPY     |
                            ARM_2D_CP_MODE_X_MIRROR |
                            ARM_2D_CP_MODE_Y_MIRROR );
                            
    //! copy the bottom left corner 
    tRegion.tLocation.iX = 0;
    arm_2d_c8bit_tile_copy( &c_tileWhiteDotAlphaQuarter, 
                            &c_tPanel, 
                            &tRegion, 
                            ARM_2D_CP_MODE_COPY     |
                            ARM_2D_CP_MODE_Y_MIRROR );
}

void spinning_wheel_show(const arm_2d_tile_t *ptTarget, bool bIsNewFrame)
{
    ASSERT(NULL != ptTarget);

    arm_2d_region_t tRegion = {
        .tSize = {PANEL_WIDTH, PANEL_HEIGHT},
        .tLocation = {
            .iX = (ptTarget->tRegion.tSize.iWidth - PANEL_WIDTH) >> 1,
            .iY = (ptTarget->tRegion.tSize.iHeight - PANEL_HEIGHT) >> 1,
        },
    };

    //arm_2d_rgb16_fill_colour(ptTarget, &tRegion, GLCD_COLOR_WHITE);
    arm_2d_rgb16_fill_colour_with_alpha_mask(   ptTarget, 
                                                &tRegion, 
                                                &c_tPanel, 
                                                GLCD_COLOR_YELLOW);
}


#if defined(__clang__)
#   pragma clang diagnostic pop
#endif
