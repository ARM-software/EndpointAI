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
#include <stdio.h>
#include "platform.h"
#include "example_gui.h"
#include "./controls/controls.h"

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
#   pragma clang diagnostic ignored "-Wmissing-prototypes"
#endif

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/

#define arm_2d_layer(__TILE_ADDR, __TRANS, __X, __Y, ...)                       \
    {                                                                           \
        .ptTile = (__TILE_ADDR),                                                \
        .tRegion.tLocation.iX = (__X),                                          \
        .tRegion.tLocation.iY = (__Y),                                          \
        .chTransparency = (__TRANS),                                            \
        __VA_ARGS__                                                             \
    }

/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
/*============================ LOCAL VARIABLES ===============================*/


extern const uint8_t c_bmpWhiteDot[19*20*sizeof(uint16_t)];
extern const arm_2d_tile_t c_tPictureWhiteDot;


/*============================ IMPLEMENTATION ================================*/

static volatile uint32_t s_wSystemTimeInMs = 0;
static volatile bool s_bTimeout = false;
extern void platform_1ms_event_handler(void);

void platform_1ms_event_handler(void)
{
    s_wSystemTimeInMs++; 
    if (!(s_wSystemTimeInMs & (_BV(10) - 1))) {
        s_bTimeout = true;
    }
}

void example_gui_init(void)
{
    controls_init();
    
}


void example_gui_do_events(void)
{

}


__WEAK 
void example_gui_on_refresh_evt_handler(const arm_2d_tile_t *ptFrameBuffer)
{
     ARM_2D_UNUSED(ptFrameBuffer);
}

extern const arm_2d_tile_t c_tileArrow;
extern
const arm_2d_tile_t c_tileGear01;
extern 
const arm_2d_tile_t c_tileGear02;

typedef struct {
    const arm_2d_tile_t *ptTile;
    float fAngle;
    float fAngleSpeed;
    arm_2d_location_t tCentre;
    arm_2d_region_t tRegion;
} demo_gears_t;

static demo_gears_t s_tGears[] = {

    {
        .ptTile = &c_tileGear02,
        .fAngleSpeed = -4,
        .tCentre = {
            .iX = 20,
            .iY = 20,
        },
        .tRegion = {
            .tLocation = {
                .iX = ((APP_SCREEN_WIDTH - 41) >> 1) + 10,
                .iY = ((APP_SCREEN_HEIGHT - 40) >>1) + 10,
            },
            .tSize = {
                .iWidth = 41,
                .iHeight = 41,
            },
        },
    },

    {
        .ptTile = &c_tileGear01,
        .fAngleSpeed = 2,
        .tCentre = {
            .iX = 61,
            .iY = 60,
        },
        .tRegion = {
            .tLocation = {
                .iX = ((APP_SCREEN_WIDTH - 120) >> 1) - 20,
                .iY = ((APP_SCREEN_HEIGHT - 120) >>1) - 20,
            },
            .tSize = {
                .iWidth = 120,
                .iHeight = 120,
            },
        },
    },
    

    
};

void example_gui_refresh(const arm_2d_tile_t *ptTile, bool bIsNewFrame)
{
    static float s_fAngle = 0;

    arm_2d_rgb16_fill_colour(ptTile, NULL, GLCD_COLOR_BLACK);
    
    arm_foreach (demo_gears_t, s_tGears) {
    
        if (bIsNewFrame) {
            _->fAngle += ARM_2D_ANGLE(_->fAngleSpeed);
            if (s_fAngle >= ARM_2D_ANGLE(360)) {
                s_fAngle -= ARM_2D_ANGLE(360);
            }
        }
    
        arm_2d_rgb565_tile_rotation_prepare(_->ptTile, 
                                            ptTile, 
                                            &(_->tRegion),
                                            _->tCentre,
                                            _->fAngle,
                                            GLCD_COLOR_BLACK);
        
        arm_2d_tile_rotate();
    }

    example_gui_on_refresh_evt_handler(ptTile);
}


#if defined(__clang__)
#   pragma clang diagnostic pop
#endif


