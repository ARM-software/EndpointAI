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

/*============================ INCLUDES ======================================*/

#include "fine_file_framework.h"
#include <string.h>
#include <rt_sys.h>

#if __IS_COMPILER_ARM_COMPILER_6__
#include <arm_compat.h>
#endif
 
#ifdef   __cplusplus
extern "C" {
#endif

/* suppress some warnings for user applications when using arm-2d.
 */
#if defined(__clang__)
#   pragma clang diagnostic ignored "-Wunknown-warning-option"
#   pragma clang diagnostic ignored "-Wreserved-identifier"
#   pragma clang diagnostic ignored "-Wgnu-variable-sized-type-not-at-end"
#   pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#   pragma clang diagnostic ignored "-Wgnu-statement-expression"
#   pragma clang diagnostic ignored "-Wextra-semi-stmt"
#   pragma clang diagnostic ignored "-Wcompound-token-split-by-macro"
#   pragma clang diagnostic ignored "-Winitializer-overrides"
#   pragma clang diagnostic ignored "-Wgcc-compat"
#   pragma clang diagnostic ignored "-Wundef"
#   pragma clang diagnostic ignored "-Wdeclaration-after-statement"
#   pragma clang diagnostic ignored "-Wflexible-array-extensions"
#   pragma clang diagnostic ignored "-Wcast-qual"
#   pragma clang diagnostic ignored "-Wmissing-prototypes"
#   pragma clang diagnostic ignored "-Wmissing-field-initializers"
#   pragma clang diagnostic ignored "-Wmissing-declarations"
#   pragma clang diagnostic ignored "-Wmicrosoft-anon-tag"
#elif defined(__IS_COMPILER_ARM_COMPILER_5__)
#   pragma diag_suppress 1296,174,64,381
#endif

/*============================ MACROS ========================================*/
#ifndef NOP
#   define NOP()            __asm volatile ("nop");
#endif

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/

enum {
    FFF_FILE_TYPE_ID_FOLDER     = 0,                                            //!< folder
    FFF_FILE_TYPE_ID_MEM_FILE,                                                  //!< Memory File
    FFF_USER_TYPE_ID_START,
    /*---------- user defined file type begin -----------*/
    
    FFF_FILE_TYPE_ID_VGA = FFF_USER_TYPE_ID_START,                              //! example user file type: you can write stream to screen
    
    
    /*---------- user defined file type end -------------*/
    FFF_USER_TYPE_COUNT,
};



/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
/*============================ IMPLEMENTATION ================================*/

#undef _MSC_VER
#define __FFF_USER_DEFINE_FILE_FRAMEWORK__

#if defined(__FFF_CFG_INCLUDE_USER_DEFINE_FILE_FRAMEWORK__)
#   include __FFF_CFG_INCLUDE_USER_DEFINE_FILE_FRAMEWORK__
#else
#   include "__fff_user_def_file_framework.c"
#endif
#undef __FFF_USER_DEFINE_FILE_FRAMEWORK__


#ifndef __FFF_CFG_ROOT__    
#   define __FFF_CFG_ROOT__             MPSx_Local_Disk
#endif
#ifndef __FFF_CFG_WORKING_PATH__
#   define __FFF_CFG_WORKING_PATH__     "."
#endif

/*----------------------------------------------------------------------------*
 * Compiler Specific Code to run __vsf_main_entry() before main()             *
 *----------------------------------------------------------------------------*/
__attribute__((used))
__attribute__((constructor, noreturn))
void __platform_main_entry(void)
{
    const arm_fff_cfg_t tCFG = {
        .ptRoot = (const arm_file_node_t *)&__FFF_CFG_ROOT__,
        .pchWorkingPath = __FFF_CFG_WORKING_PATH__,
        
        /* user defined file types */
        .tTypes = {
            .ptList = NULL,
            .chCount = 0,
        },
    };

    arm_fff_init(&tCFG);
    
    /* should not run to here, error occurred */
    while(1) {
        NOP();
    }
}

#if (   (   defined(__FFF_CFG_IGNORE_NO_SEMIHOSTING__)                          \
        &&  !__FFF_CFG_IGNORE_NO_SEMIHOSTING__))                                \
    || !defined(__FFF_CFG_IGNORE_NO_SEMIHOSTING__)

/* disable semihosting */
#if defined(__IS_COMPILER_ARM_COMPILER_5__) && __IS_COMPILER_ARM_COMPILER_5__
#   pragma import(__use_no_semihosting)
#elif defined(__IS_COMPILER_ARM_COMPILER_6__) && __IS_COMPILER_ARM_COMPILER_6__
__asm(".global __use_no_semihosting\n\t");
#endif

#endif

__attribute__((used))
__attribute__((noreturn))
void _sys_exit(int ch)
{
    (void)ch;

#if __FFF_CFG_IGNORE_NO_SEMIHOSTING__
    __semihost(0x18, (const void *)0x20026);
#endif
 
    while(1) {
        NOP();
    }
}


#ifdef   __cplusplus
}
#endif

