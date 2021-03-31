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
#endif


extern void SysTick_Handler(void);
extern void _ttywrch(int ch);

__WEAK
void platform_1ms_event_handler(void);

void SysTick_Handler(void)
{
    platform_1ms_event_handler();
}

void delay_ms(uint32_t wMS)
{
    platform_delay_ms(wMS);
}


__WEAK 
bool device_specific_init(void)
{
    return false;
}

__attribute__((used, constructor(255)))
static void app_platform_init(void)
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
        
        GLCD_DrawString(GLCD_WIDTH / 2 - 60, GLCD_HEIGHT / 2 - 4, "    Arm-2D Benchmark     ");
    //}
}

void GLCD_Print(uint32_t x, uint32_t y, const char *str)
{
    static uint_fast16_t s_hwOffset = 0;
    while(*str) {
        if (*str == '\r' || *str == '\n') {
            s_hwOffset = 0;
        } else if (*str == '\b') {
            if (s_hwOffset >= GLCD_Font_6x8.width) {
                s_hwOffset -= GLCD_Font_6x8.width;
            }
        } else {
            GLCD_DrawChar(x + s_hwOffset, y, *str);
            s_hwOffset += GLCD_Font_6x8.width;
            if (s_hwOffset >= GLCD_SIZE_Y) {
                s_hwOffset = 0;
            }
        }
        
        str++;
    }
}


int lcd_printf(const char *format, ...)
{
    int real_size;
    static char s_chBuffer[(GLCD_WIDTH/6)+1];
    __va_list ap;
    va_start(ap, format);
        real_size = vsnprintf(s_chBuffer, sizeof(s_chBuffer)-1, format, ap);
    va_end(ap);
    real_size = MIN(sizeof(s_chBuffer)-1, real_size);
    s_chBuffer[real_size] = '\0';
    GLCD_Print(0, GLCD_HEIGHT - 16, s_chBuffer);
    return real_size;
}

#if __IS_COMPILER_ARM_COMPILER_6__
__asm(".global __use_no_semihosting\n\t");
#   ifndef __MICROLIB
__asm(".global __ARM_use_no_argv\n\t");
#   endif


void _ttywrch(int ch)
{
    ARM_2D_UNUSED(ch);
}


__NO_RETURN
void _sys_exit(int ret)
{
    ARM_2D_UNUSED(ret);
    while(1) {}
}

#endif

#if defined(__clang__)
#   pragma clang diagnostic pop
#endif
