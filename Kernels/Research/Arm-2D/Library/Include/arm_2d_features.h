/*
 * Copyright (C) 2021 Arm Limited or its affiliates. All rights reserved.
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

/* ----------------------------------------------------------------------
 * Project:      Arm-2D Library
 * Title:        #include "arm_2d_features.h"
 * Description:  Public header file to indicate features avaialble for this
 *               arm-2d library variant.
 *
 * $Date:        29. Jan 2020
 * $Revision:    V.0.5.0
 *
 * Target Processor:  Cortex-M cores
 * -------------------------------------------------------------------- */
 
#ifndef __ARM_2D_FEATURES_H__
#define __ARM_2D_FEATURES_H__

/*============================ INCLUDES ======================================*/


#ifdef   __cplusplus
extern "C" {
#endif

/*============================ MACROS ========================================*/

#undef __ARM_2D_HAS_HELIUM__
#if defined (ARM_MATH_HELIUM) || defined(ARM_MATH_MVEF) || defined(ARM_MATH_MVEI)
#   define __ARM_2D_HAS_HELIUM__                        1
#else
#   define __ARM_2D_HAS_HELIUM__                        0
#endif

#ifndef __ARM_2D_HAS_CDE__
#   define __ARM_2D_HAS_CDE__                           0
#endif

#ifndef __ARM_2D_HAS_HW_ACC__
#   define __ARM_2D_HAS_HW_ACC__                        0
#endif
#if defined(__ARM_2D_HAS_HW_ACC__) && __ARM_2D_HAS_HW_ACC__
#   if defined(__ARM_2D_HAS_ASYNC__) && !__ARM_2D_HAS_ASYNC__
#       warning As long as __ARM_2D_HAS_HW_ACC__ is set to 1,\
 __ARM_2D_HAS_ASYNC__ is forced to 1. Since you set __ARM_2D_HAS_ASYNC__ to\
 0, please remove your macro definition for __ARM_2D_HAS_ASYNC__ to avoid this\
 warning.
#   endif
#   undef __ARM_2D_HAS_ASYNC__
#   define __ARM_2D_HAS_ASYNC__                         1
#endif

#ifndef __ARM_2D_HAS_ASYNC__
#   define __ARM_2D_HAS_ASYNC__                         1
#endif
#if defined(__ARM_2D_HAS_ASYNC__) &&  __ARM_2D_HAS_ASYNC__
#   if  !defined(__ARM_2D_DEFAULT_SUB_TASK_POOL_SIZE) ||                        \
        __ARM_2D_DEFAULT_SUB_TASK_POOL_SIZE < 4
#       define __ARM_2D_DEFAULT_SUB_TASK_POOL_SIZE      4
#   endif
#endif

#ifndef __ARM_2D_HAS_INTERPOLATION_ROTATION__
#   define __ARM_2D_HAS_INTERPOLATION_ROTATION__        0
#endif

/*============================ MACROFIED FUNCTIONS ===========================*/

#if defined(__ARM_2D_HAS_CDE__) && !__ARM_2D_HAS_CDE__
#   define __arm_2d_cde_init()
#endif

#if defined(__ARM_2D_HAS_HELIUM__) && !__ARM_2D_HAS_HELIUM__
#   define __arm_2d_helium_init()
#endif

#if defined(__ARM_2D_HAS_HW_ACC__) && !__ARM_2D_HAS_HW_ACC__
#   define __arm_2d_acc_init()
#endif

#if defined(__ARM_2D_HAS_ASYNC__) && !__ARM_2D_HAS_ASYNC__
#   define __arm_2d_async_init()
#endif


#undef arm_2d_init
#define arm_2d_init()                                                           \
        do {                                                                    \
            __arm_2d_init();                                                    \
            __arm_2d_async_init();                                              \
            __arm_2d_helium_init();                                             \
            __arm_2d_cde_init();                                                \
            __arm_2d_acc_init();                                                \
        } while(0)

/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/

#ifdef   __cplusplus
}
#endif

#endif
