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

#ifndef __PERFORMANCE_COUNTER_H__
#define __PERFORMANCE_COUNTER_H__

/*============================ INCLUDES ======================================*/
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/


#ifndef __TPASTE2
#   define __TPASTE2(a, b)                            a##b
#endif

#define ____noreturn_cyclesOf(__STR,...)                                        \
        {                                                                       \
            int32_t nCycles;                                                    \
            start_cycle_counter();                                              \
            __VA_ARGS__;                                                        \
            nCycles = stop_cycle_counter();                                     \
            printf( "\r\n-[Cycle Report]"                                       \
                    "--------------------------------------------\r\n"          \
                    __STR                                                       \
                    " total cycle count: %d [%08x]\r\n", nCycles, nCycles);     \
        }
#define __noreturn_CyclesOf(__STR, ...)         \
            ____noreturn_cyclesOf(__STR, __VA_ARGS__)


#define __log_cycles_of(__STR)                                                  \
            for (int32_t nCycles = 0,                                           \
                    __TPASTE2(__cycle_count_s_, __LINE__) = 1;                  \
                 __TPASTE2(__cycle_count_s_, __LINE__)-- ?                      \
                    (start_cycle_counter(),1) :                                 \
                    (                                                           \
                    printf( "\r\n-[Cycle Report]"                               \
                        "--------------------------------------------\r\n"      \
                        __STR                                                   \
                        " total cycle count: %d [%08x], %3.3f ms\r\n", nCycles, nCycles, (float)nCycles/25000.0f)  \
                    ,0);                                                        \
                    nCycles = stop_cycle_counter()                              \
                 )
                    
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/

extern void cycle_counter_overflow(void);

extern void platform_delay_ms(uint32_t wMS);

extern void init_cycle_counter(bool bTimerOccupied);

/* Function : start_time
	This function will be called right before starting the timed portion of the benchmark.
	Implementation may be capturing a system timer (as implemented in the example code) 
	or zeroing some system parameters - e.g. setting the cpu clocks cycles to 0.
*/
extern void start_cycle_counter(void);

/* Function : stop_time
	This function will be called right after ending the timed portion of the benchmark.
	Implementation may be capturing a system timer (as implemented in the example code) 
	or other system parameters - e.g. reading the current value of cpu cycles counter.
*/
extern int32_t stop_cycle_counter(void);

#ifdef __cplusplus
}
#endif

#endif
