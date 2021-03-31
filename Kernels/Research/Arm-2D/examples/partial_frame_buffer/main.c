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
#include <stdio.h>
#include "platform.h"
#include "./display_adapter/display_adapter.h"
#include "example_gui.h"

#if defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wsign-conversion"
#   pragma clang diagnostic ignored "-Wpadded"
#   pragma clang diagnostic ignored "-Wcast-qual"
#   pragma clang diagnostic ignored "-Wcast-align"
#   pragma clang diagnostic ignored "-Wmissing-field-initializers"
#   pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#   pragma clang diagnostic ignored "-Wmissing-prototypes"
#endif

/*============================ MACROS ========================================*/
#ifndef __STR
#   define __STR(__A)      #__A
#endif

#ifndef STR
#   define STR(__A)         __STR(__A)
#endif

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ IMPLEMENTATION ================================*/

void display_task(void) 
{   
    int32_t nTotalCyclCount = 0;
    int32_t nTotalLCDCycCount = 0;
    
    example_gui_do_events();
    
    start_cycle_counter(); 
    do {
        nTotalLCDCycCount += stop_cycle_counter(); 
        arm_2d_tile_t * ptFrameBuffer;
        /*! begin of the drawing iteration, 
         *! try to request the tile of frame buffer
         */
        start_cycle_counter(); 
        do {
            //! request to draw the whole LCD
            ptFrameBuffer = drawing_iteration_begin(NULL);
            if (NULL == ptFrameBuffer) {
                //wait_for_lcd_ready();    //! wait until lcd is ready
                continue;
            } else if (-1 == (intptr_t)ptFrameBuffer) {
                /* display driver wants to end the drawing */
                return ;
            }
        } while(NULL == ptFrameBuffer);

        //! draw all the gui elements on target frame buffer
        example_gui_refresh(ptFrameBuffer);
        
        nTotalCyclCount += stop_cycle_counter(); 
        
        start_cycle_counter(); 
    } while(drawing_iteration_end());
    

    lcd_printf("\r\nFPS %3d:%2dms (LCD Latency %2dms) " 
                STR(APP_SCREEN_WIDTH) "*"
                STR(APP_SCREEN_HEIGHT) " %dMHz", 
                SystemCoreClock / nTotalCyclCount, 
                nTotalCyclCount / (SystemCoreClock / 1000ul),
                nTotalLCDCycCount / (SystemCoreClock / 1000ul),
                SystemCoreClock / 1000000ul);
    
}        



/*----------------------------------------------------------------------------
  Main function
 *----------------------------------------------------------------------------*/
int main (void) 
{
    arm_irq_safe {
        arm_2d_init();
        /* put your code here */
        platform_disp_init();
        example_gui_init();
    }
    
    while (1) {
        display_task();
    }
}

#if defined(__clang__)
#   pragma clang diagnostic pop
#endif


