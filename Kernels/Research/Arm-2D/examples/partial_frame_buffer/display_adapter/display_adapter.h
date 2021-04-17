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

#ifndef __DISPLAY_ADAPTER_H__
#define __DISPLAY_ADAPTER_H__

/*============================ INCLUDES ======================================*/
#include "./app_cfg.h"
#include "arm_2d.h"
#include "arm_2d_helper.h"
/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/

extern 
void platform_disp_init(void);

extern
void platform_disp_fill_tile(const arm_2d_tile_t *ptTile, uint_fast16_t hwColor);
                                     
extern 
arm_fsm_rt_t platform_disp_buffer_refresh(void);
    
/*! \brief begin a iteration of drawing and request a frame buffer from 
 *!        low level display driver.
 *! \param ptTargetRegion the address of the target region in the LCD
 *!        passing NULL means we want to draw the whole LCD.
 *! \retval NULL the display driver is not ready
 *! \retval (intptr_t)-1 the display driver want to ignore this drawing 
 *!         (maybe because the target area is out of the LCD)
 *! \retval non-null a tile which contains the (partial) frame buffer
 */
extern arm_2d_tile_t * drawing_iteration_begin(arm_2d_region_t *ptTargetRegion);

/*! \brief end a drawing iteration and decide wether a new iteration is required
 *!        or not based on the return value
 *! \param none
 *! \retval true a new iteration is required
 *! \retval false no more iteration is required
 */
extern bool drawing_iteration_end(void);
              
/*! \brief wait for LCD ready 
 */
extern void platform_wait_for_disp_ready(void);

#endif
