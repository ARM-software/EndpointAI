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

#ifndef __PLATFORM_H__
#define __PLATFORM_H__

/*============================ INCLUDES ======================================*/

#include ".\perf_counter.h"
#include "cmsis_compiler.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "arm_2d.h"

//#include "Device.h"                     // Keil::Board Support:V2M-MPS2:Common
#include "RTE_Components.h"             // Component selection
//#include "Board_LED.h"                  // ::Board Support:LED
//#include "Board_Buttons.h"              // ::Board Support:Buttons
//#include "Board_Touch.h"                // ::Board Support:Touchscreen
#include "Board_GLCD.h"                 // ::Board Support:Graphic LCD
#include "GLCD_Config.h"                // Keil.SAM4E-EK::Board Support:Graphic LCD


#ifdef __cplusplus
extern "C" {
#endif

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/

#define SAFE_ATOM_CODE(...)             \
{                                       \
    uint32_t wOrig = __disable_irq();   \
    __VA_ARGS__;                        \
    __set_PRIMASK(wOrig);               \
}

#ifndef _BV
#   define _BV(__N)                ((uint32_t)1 << (__N))
#endif


#define __PRINT_BANNER(__STR)                                                   \
        do {                                                                    \
            lcd_text_location(  (GLCD_HEIGHT / 8) / 2 - 1,                      \
                                ((GLCD_WIDTH / 6) - sizeof(__STR)) / 2);        \
            lcd_puts(__STR);                                                    \
        } while(0)

/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/

extern const GLCD_FONT    GLCD_Font_16x24;
extern const GLCD_FONT    GLCD_Font_6x8;

extern uint32_t SystemCoreClock;
/*============================ PROTOTYPES ====================================*/

extern void delay_ms(uint32_t wMS);

extern
int lcd_printf(const char *format, ...);

extern
void lcd_puts(const char *str);

extern 
void lcd_text_location(uint8_t chY, uint8_t chX);

#ifdef __cplusplus
}
#endif

#endif
