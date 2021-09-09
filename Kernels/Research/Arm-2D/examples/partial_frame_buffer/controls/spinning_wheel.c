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


#include "arm_2d.h"
#include "arm_2d_helper.h"
#include "platform.h"
#include <math.h>

#include "./spinning_wheel.h"
#include "./shape_round_corner_box.h"

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
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
extern uint32_t SystemCoreClock;


/*============================ PROTOTYPES ====================================*/
__attribute__((nothrow)) 
extern int64_t clock(void);

/*============================ LOCAL VARIABLES ===============================*/


/*============================ IMPLEMENTATION ================================*/


void spinning_wheel_init(void)
{

}

void spinning_wheel_show(const arm_2d_tile_t *ptTarget, bool bIsNewFrame)
{
    ASSERT(NULL != ptTarget);
    ARM_2D_UNUSED(ptTarget);
    ARM_2D_UNUSED(bIsNewFrame);

}


#if defined(__clang__)
#   pragma clang diagnostic pop
#endif
