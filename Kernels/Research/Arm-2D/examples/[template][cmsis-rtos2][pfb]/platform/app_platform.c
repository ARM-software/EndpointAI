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


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include "platform.h"
#include "app_cfg.h"

#if defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wformat-nonliteral"
#   pragma clang diagnostic ignored "-Wsign-compare"
#   pragma clang diagnostic ignored "-Wmissing-prototypes"
#   pragma clang diagnostic ignored "-Wcast-qual"
#   pragma clang diagnostic ignored "-Wsign-conversion"
#endif


extern void SysTick_Handler(void);
extern void _ttywrch(int ch);


extern const GLCD_FONT    GLCD_Font_16x24;
extern const GLCD_FONT    GLCD_Font_6x8;


#if defined(__IS_COMPILER_GCC__)
static volatile uint32_t s_wDelayCounter = 0;
#endif

__WEAK
void platform_1ms_event_handler(void);

#ifndef RTE_CMSIS_RTOS2
__OVERRIDE_WEAK
void SysTick_Handler(void)
{
    platform_1ms_event_handler();
    
#if defined(__IS_COMPILER_GCC__)
    extern void HAL_IncTick(void);
    HAL_IncTick();
    
    extern void user_code_insert_to_systick_handler(void);
    user_code_insert_to_systick_handler();
    
    if (s_wDelayCounter) {
        s_wDelayCounter--;
    }
#endif
}
#elif defined(__IS_COMPILER_GCC__)

void __wrap_osRtxTick_Handler(void)
{
    extern void HAL_IncTick(void);
    HAL_IncTick();
    
    extern void user_code_insert_to_systick_handler(void);
    user_code_insert_to_systick_handler();
    
    if (s_wDelayCounter) {
        s_wDelayCounter--;
    }
    
    extern void __real_osRtxTick_Handler(void);
    __real_osRtxTick_Handler();
}

#if defined(__IS_COMPILER_GCC__)
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wpedantic"
#endif


_ATTRIBUTE ((__format__ (__printf__, 1, 2)))
int	__wrap_printf (const char *__restrict format, ...)
{
    va_list va;
    va_start(va, format);
    int ret = 0;
    arm_thread_safe { 
        ret = vprintf(format, va);
    }
    va_end(va);
    
    return ret;
}

#if defined(__IS_COMPILER_GCC__)
#   pragma GCC diagnostic pop
#endif

#endif

#if defined(__IS_ARM_COMPILER_5__) || defined(__IS_ARM_COMPILER_6__)

#pragma __printf_args
__attribute__((__nonnull__(1)))
_ARMABI int $Sub$$printf(const char * __restrict format, ...) 
{
    va_list va;
    va_start(va, format);
    int ret = 0;
    arm_thread_safe { 
        ret = vprintf(format, va);
    }
    va_end(va);
    
    return ret;
}

#endif




/**
  * Override default HAL_GetTick function
  */
uint32_t HAL_GetTick (void) {
  static uint32_t ticks = 0U;
         uint32_t i;

  if (osKernelGetState () == osKernelRunning) {
    return ((uint32_t)osKernelGetTickCount ());
  }

  /* If Kernel is not running wait approximately 1 ms then increment 
     and return auxiliary tick counter value */
  for (i = (SystemCoreClock >> 14U); i > 0U; i--) {
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
  }
  return ++ticks;
}

void delay_ms(uint32_t wMS)
{
#if defined(__IS_COMPILER_GCC__)
    s_wDelayCounter = wMS;
    
    while(s_wDelayCounter) { 
        __NOP(); 
    };
#else
    platform_delay_ms(wMS);
#endif
}


__WEAK 
bool device_specific_init(void)
{
    return false;
}

__attribute__((used, constructor(255)))
void app_platform_init(void)
{
    init_cycle_counter(device_specific_init());

    //__log_cycles_of("Initialize platform...\r\n") {
        //LED_Initialize();                         /* Initializ LEDs                 */
        //Buttons_Initialize();                     /* Initializ Push Buttons         */

        //Touch_Initialize();                       /* Initialize touchscreen         */
        GLCD_Initialize();                        /* Initialize the GLCD            */

        /* display initial screen */
        GLCD_SetFont(&GLCD_Font_6x8);
        GLCD_SetBackgroundColor(GLCD_COLOR_BLACK);
        GLCD_ClearScreen();
        //GLCD_SetBackgroundColor(GLCD_COLOR_BLUE);
        GLCD_SetForegroundColor(GLCD_COLOR_GREEN);
    //}
}

#if __IS_COMPILER_ARM_COMPILER_6__
__asm(".global __use_no_semihosting\n\t");
#   ifndef __MICROLIB
__asm(".global __ARM_use_no_argv\n\t");
#   endif

__NO_RETURN
void _sys_exit(int ret)
{
    ARM_2D_UNUSED(ret);
    while(1) {}
}

#endif

#if defined(__MICROLIB)
_ARMABI_NORETURN 
ARM_NONNULL(1,2)
void __aeabi_assert(const char *chCond, const char *chLine, int wErrCode) 
{
    ARM_2D_UNUSED(chCond);
    ARM_2D_UNUSED(chLine);
    ARM_2D_UNUSED(wErrCode);
    while(1) {
        __NOP();
    }
}
#endif

#if !defined(__IS_COMPILER_GCC__)
void _ttywrch(int ch)
{
    ARM_2D_UNUSED(ch);
}
#endif

#if defined(__clang__)
#   pragma clang diagnostic pop
#endif
