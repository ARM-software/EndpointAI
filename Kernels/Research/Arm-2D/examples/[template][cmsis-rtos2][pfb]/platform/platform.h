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
#include "lcd_printf.h"

//#include "Device.h"                     // Keil::Board Support:V2M-MPS2:Common
#include "RTE_Components.h"             // Component selection
//#include "Board_LED.h"                  // ::Board Support:LED
//#include "Board_Buttons.h"              // ::Board Support:Buttons
//#include "Board_Touch.h"                // ::Board Support:Touchscreen
#include "Board_GLCD.h"                 // ::Board Support:Graphic LCD
#include "GLCD_Config.h"                // Keil.SAM4E-EK::Board Support:Graphic LCD

#ifdef RTE_CMSIS_RTOS2                  // when RTE component CMSIS RTOS2 is used
#   include "cmsis_os2.h"                  // ::CMSIS:RTOS2
#endif


#ifdef __cplusplus
extern "C" {
#endif

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
#if defined(RTE_CMSIS_RTOS2)

#   define arm_thread_safe  \
            arm_using(  int32_t ARM_CONNECT2(temp,__LINE__) =                   \
            ({uint32_t temp = osKernelLock();temp;}),                           \
            osKernelRestoreLock(ARM_CONNECT2(temp,__LINE__)))

#endif



/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/

extern uint32_t SystemCoreClock;
/*============================ PROTOTYPES ====================================*/

#if defined(__IS_COMPILER_GCC__)
extern void app_platform_init(void);
#endif

extern void delay_ms(uint32_t wMS);


#ifdef __cplusplus
}
#endif

#endif
