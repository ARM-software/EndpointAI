#ifndef __CUSTOM_FILE_H__
#define __CUSTOM_FILE_H__


/****************************************************************************
*  Copyright 2020 Gabriel Wang (Email:gabriel.wang@arm.com)                 *
*                                                                           *
*  Licensed under the Apache License, Version 2.0 (the "License");          *
*  you may not use this file except in compliance with the License.         *
*  You may obtain a copy of the License at                                  *
*                                                                           *
*     http://www.apache.org/licenses/LICENSE-2.0                            *
*                                                                           *
*  Unless required by applicable law or agreed to in writing, software      *
*  distributed under the License is distributed on an "AS IS" BASIS,        *
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. *
*  See the License for the specific language governing permissions and      *
*  limitations under the License.                                           *
*                                                                           *
****************************************************************************/

/*============================ INCLUDES ======================================*/

//#include "fine_file_framework.h"
#include <stdint.h>
#include <stdbool.h>
/*============================ MACROS ========================================*/

// for IAR
#undef __IS_COMPILER_IAR__
#if defined(__IAR_SYSTEMS_ICC__)
#   define __IS_COMPILER_IAR__                  1
#endif

// for arm compiler 5
#undef __IS_COMPILER_ARM_COMPILER_5__
#if ((__ARMCC_VERSION >= 5000000) && (__ARMCC_VERSION < 6000000))
#   define __IS_COMPILER_ARM_COMPILER_5__       1
#endif


//for arm compiler 6

#undef __IS_COMPILER_ARM_COMPILER_6__
#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
#   define __IS_COMPILER_ARM_COMPILER_6__       1
#endif
#undef __IS_COMPILER_ARM_COMPILER__
#if defined(__IS_COMPILER_ARM_COMPILER_5__) && __IS_COMPILER_ARM_COMPILER_5__   \
||  defined(__IS_COMPILER_ARM_COMPILER_6__) && __IS_COMPILER_ARM_COMPILER_6__
#   define __IS_COMPILER_ARM_COMPILER__         1
#endif

// for clang
#undef  __IS_COMPILER_LLVM__
#if defined(__clang__) && !__IS_COMPILER_ARM_COMPILER_6__
#   define __IS_COMPILER_LLVM__                 1
#else

// for gcc
#   undef __IS_COMPILER_GCC__
#   if defined(__GNUC__) && !(  defined(__IS_COMPILER_ARM_COMPILER__)           \
                            ||  defined(__IS_COMPILER_LLVM__)                   \
                            ||  defined(__IS_COMPILER_IAR__))
#       define __IS_COMPILER_GCC__              1
#   endif
#endif


#if !__IS_COMPILER_ARM_COMPILER__
/*
 * openmode is a bitmap, whose bits are given below. They
 * correspond directly to the ANSI mode specification.
 */
#define OPEN_R 0
#define OPEN_W 4
#define OPEN_A 8
#define OPEN_B 1
#define OPEN_PLUS 2

#define  _ARMABI

#endif

#undef __implement_ex
#undef __implement
#undef implement
#undef implement_ex

#define __implement_ex(__type, __name)                                          \
            union {                                                             \
                __type  __name;                                                 \
                __type;                                                         \
            };

#define __implement(__type)             __implement_ex( __type,                 \
                                                        use_as__##__type)
                                                        
#define implement(__type)               __implement(__type)
#define implement_ex(__type, __name)    __implement_ex(__type, __name)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/

typedef struct arm_file_node_t arm_file_node_t;

struct arm_file_node_t {
    uint8_t chID;
    union {
        struct {
            uint8_t bIsValid        : 1;
            uint8_t bIsVisible      : 1;
            uint8_t                 : 2;
            uint8_t bCanRead        : 1;
            uint8_t bCanWrite       : 1;
            uint8_t bCanSeek        : 1;
            uint8_t bIsStream       : 1; /* whether we know size or not */
        };
        uint8_t chAttribute;
    };
    
    uint8_t    chAliasCount;
    const char *const* ppchPathString;
    
    const arm_file_node_t *ptParent;        //!< parent
    const arm_file_node_t *ptNext;          //!< next file in the same folder
    const arm_file_node_t *ptList;          //!< !NULL means folder
} ;

typedef struct arm_folder_node_t {
    implement(arm_file_node_t)
    const arm_file_node_t ptFirstNode[];
} arm_folder_node_t;


typedef struct __FILE {
    implement(arm_file_node_t)
}__FILE;

typedef __FILE FILE;



/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/






#endif