/*
 * Copyright (C) 2023 Arm Limited or its affiliates. All rights reserved.
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

#ifndef __FINE_FILE_FRAMEWORK_CFG_H__
#define __FINE_FILE_FRAMEWORK_CFG_H__

/*============================ INCLUDES ======================================*/
#include <assert.h>
#include <RTE_Components.h>
/*============================ MACROS ========================================*/

#ifndef FFF_ASSERT   
#   define FFF_ASSERT      assert
#endif

#ifndef __FFF_CFG_IGNORE_NO_SEMIHOSTING__
#   define __FFF_CFG_IGNORE_NO_SEMIHOSTING__                0
#endif

#ifndef __FFF_CFG_SEMIHOSTING_CMD_LINE_BUFFER_SIZE__
#   define  __FFF_CFG_SEMIHOSTING_CMD_LINE_BUFFER_SIZE__    80
#endif

#ifndef __FFF_CFG_GET_COMMAND_LINE_VIA_SEMIHOSTING__
#   define __FFF_CFG_GET_COMMAND_LINE_VIA_SEMIHOSTING__     0
#endif

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/


#endif
