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
#include "arm_2d.h"
#include "benchmark_generic.h"


/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
/*============================ LOCAL VARIABLES ===============================*/


/*============================ IMPLEMENTATION ================================*/

__OVERRIDE_WEAK
void arm_2d_helper_perf_counter_start(void)
{
    start_cycle_counter();
}

__OVERRIDE_WEAK
int32_t arm_2d_helper_perf_counter_stop(void)
{
    return stop_cycle_counter();
}

/*----------------------------------------------------------------------------
  Main function
 *----------------------------------------------------------------------------*/

int main (void)
{
    printf("Hello Arm-2D\r\n");

    arm_irq_safe {
        arm_2d_init();
        /* put your code here */
        example_gui_init();
    }

    run_benchmark_generic();    
    
    while(true) {
        
    }
}


