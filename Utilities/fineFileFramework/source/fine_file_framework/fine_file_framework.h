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

#ifndef __FINE_FILE_FRAMEWORK_H__
#define __FINE_FILE_FRAMEWORK_H__

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#ifdef   __cplusplus
extern "C" {
#endif


/*============================ INCLUDES ======================================*/

#ifndef __FFF_CFG_SKIP_CFG_FILE__
#include "fff_cfg.h"
#endif

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


/* suppress some warnings for user applications when using arm-2d.
 */
#if defined(__clang__)
#   pragma clang diagnostic push
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
#   pragma clang diagnostic ignored "-Wpadded"
#   pragma clang diagnostic ignored "-Wmissing-declarations"
#   pragma clang diagnostic ignored "-Wmicrosoft-anon-tag"
#elif defined(__IS_COMPILER_ARM_COMPILER_5__)
#   pragma diag_suppress 1296,174,64
#endif


/*============================ MACROFIED FUNCTIONS ===========================*/
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

#ifndef dimof
#   define dimof(__array)               (sizeof(__array) / sizeof(__array[0]))
#endif

#ifndef FFF_UNUSED
#   define FFF_UNUSED(__VAR)            (void)(__VAR)
#endif

/*!
 * \brief a wrapper for __attribute__((nonnull))
 */
#ifndef ARM_NONNULL
#   if  defined(__IS_COMPILER_ARM_COMPILER_5__) ||\
        defined(__IS_COMPILER_ARM_COMPILER_6__) ||\
        defined(__IS_COMPILER_GCC__)            ||\
        defined(__IS_COMPILER_LLVM__)
#       define ARM_NONNULL(...)     __attribute__((nonnull(__VA_ARGS__)))
#   else
#       define ARM_NONNULL(...)
#   endif
#endif

/*!
 * \brief an attribute for static variables that no initialisation is required 
 *        in the C startup process.
 */
#ifndef ARM_NOINIT
#   if     defined(__IS_COMPILER_ARM_COMPILER_5__)
#       define ARM_NOINIT   __attribute__(( section( ".bss.noinit"),zero_init))
#   elif   defined(__IS_COMPILER_ARM_COMPILER_6__)
#       define ARM_NOINIT   __attribute__(( section( ".bss.noinit")))
#   elif   defined(__IS_COMPILER_IAR__)
#       define ARM_NOINIT   __no_init
#   elif   defined(__IS_COMPILER_GCC__) || defined(__IS_COMPILER_LLVM__)
#       define ARM_NOINIT   __attribute__(( section( ".bss.noinit")))
#   else
#       define ARM_NOINIT
#   endif
#endif

/*!
 * \brief a wrapper for __attribute__((nonnull))
 */
#ifndef ARM_NONNULL
#   if  defined(__IS_COMPILER_ARM_COMPILER_5__) ||\
        defined(__IS_COMPILER_ARM_COMPILER_6__) ||\
        defined(__IS_COMPILER_GCC__)            ||\
        defined(__IS_COMPILER_LLVM__)
#       define ARM_NONNULL(...)     __attribute__((nonnull(__VA_ARGS__)))
#   else
#       define ARM_NONNULL(...)
#   endif
#endif

/*!
 * \brief an attribute for static variables that no initialisation is required 
 *        in the C startup process.
 */
#ifndef ARM_NOINIT
#   if     defined(__IS_COMPILER_ARM_COMPILER_5__)
#       define ARM_NOINIT   __attribute__(( section( ".bss.noinit"),zero_init))
#   elif   defined(__IS_COMPILER_ARM_COMPILER_6__)
#       define ARM_NOINIT   __attribute__(( section( ".bss.noinit")))
#   elif   defined(__IS_COMPILER_IAR__)
#       define ARM_NOINIT   __no_init
#   elif   defined(__IS_COMPILER_GCC__) || defined(__IS_COMPILER_LLVM__)
#       define ARM_NOINIT   __attribute__(( section( ".bss.noinit")))
#   else
#       define ARM_NOINIT
#   endif
#endif

#ifndef MAX
#   define MAX(a,b) ((a) > (b) ? (a) : (b))
#endif

#ifndef MIN
#   define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif


/*!
 * \note do NOT use this macro directly
 */
#define __FFF_VA_NUM_ARGS_IMPL( _0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,      \
                                _13,_14,_15,_16,__N,...)      __N

/*!
 * \brief A macro to count the number of parameters
 * 
 * \note if GNU extension is not supported or enabled, the following express will
 *       be false:  (__FFF_VA_NUM_ARGS() != 0)
 *       This might cause problems when in this library.
 */
#define __FFF_VA_NUM_ARGS(...)                                                  \
            __FFF_VA_NUM_ARGS_IMPL( 0,##__VA_ARGS__,16,15,14,13,12,11,10,9,     \
                                      8,7,6,5,4,3,2,1,0)

/*!
 * \note do NOT use this macro directly
 */
#define __FFF_CONNECT2(__A, __B)                        __A##__B

/*!
 * \note do NOT use this macro directly
 */
#define __FFF_CONNECT3(__A, __B, __C)                   __A##__B##__C


/*! 
 * \brief connect three symbol names as one
 */
#define FFF_CONNECT3(__A, __B, __C)             __FFF_CONNECT3(__A, __B, __C)

/*! 
 * \brief connect two symbol names as one
 */
#define FFF_CONNECT2(__A, __B)                  __FFF_CONNECT2(__A, __B)

/*!
 * \note do NOT use this macro directly
 */
#define __FFF_USING1(__declare)                                                 \
            for (__declare, *FFF_CONNECT3(__FFF_USING_, __LINE__,_ptr) = NULL;  \
                 FFF_CONNECT3(__FFF_USING_, __LINE__,_ptr)++ == NULL;           \
                )

/*!
 * \note do NOT use this macro directly
 */
#define __FFF_USING2(__declare, __on_leave_expr)                                \
            for (__declare, *FFF_CONNECT3(__FFF_USING_, __LINE__,_ptr) = NULL;  \
                 FFF_CONNECT3(__FFF_USING_, __LINE__,_ptr)++ == NULL;           \
                 (__on_leave_expr)                                              \
                )

/*!
 * \note do NOT use this macro directly
 */
#define __FFF_USING3(__declare, __on_enter_expr, __on_leave_expr)               \
            for (__declare, *FFF_CONNECT3(__FFF_USING_, __LINE__,_ptr) = NULL;  \
                 FFF_CONNECT3(__FFF_USING_, __LINE__,_ptr)++ == NULL ?          \
                    ((__on_enter_expr),1) : 0;                                  \
                 (__on_leave_expr)                                              \
                )

/*!
 * \note do NOT use this macro directly
 */
#define __FFF_USING4(__dcl1, __dcl2, __on_enter_expr, __on_leave_expr)          \
            for (__dcl1,__dcl2,*FFF_CONNECT3(__FFF_USING_, __LINE__,_ptr)= NULL;\
                 FFF_CONNECT3(__FFF_USING_, __LINE__,_ptr)++ == NULL ?          \
                    ((__on_enter_expr),1) : 0;                                  \
                 (__on_leave_expr)                                              \
                )

/*!
 * \brief create a code segment with up to two local variables and 
 *        entering/leaving operations
 * \note prototype 1
 *       fff_using(local variable declaration) {
 *           code body 
 *       }
 * 
 * \note prototype 2
 *       fff_using(local variable declaration, {code segment before leaving the body}) {
 *           code body
 *       }
 *
 * \note prototype 3
 *       fff_using( local variable declaration, 
 *                 {code segment before entering the body},
 *                 {code segment before leaving the body}
 *                 ) {
 *           code body 
 *       }
 *
 * \note prototype 4
 *       fff_using( local variable1 declaration,
                    local variable2 with the same type as the local variable 1,
 *                 {code segment before entering the body},
 *                 {code segment before leaving the body}
 *                 ) {
 *           code body 
 *       }
 */
#define fff_using(...)                                                          \
            FFF_CONNECT2(__FFF_USING, __FFF_VA_NUM_ARGS(__VA_ARGS__))(__VA_ARGS__)


/*!
 * \brief A macro to generate a safe name, usually used in macro template as the 
 *        name of local variables
 * 
 */
#define FFF_SAFE_NAME(...)    FFF_CONNECT3(__,__LINE__,##__VA_ARGS__)

#define __fff_foreach1(__PATH)              __fff_foreach3(__PATH, -1, _)

#define __fff_foreach2(__PATH, __LEVEL)     __fff_foreach3(__PATH, __LEVEL, _)

#define __fff_foreach3(__PATH, __LEVEL, __ITEM_NAME)                            \
    fff_using(  int_fast16_t FFF_SAFE_NAME(iLevel) = (__LEVEL),                 \
                FFF_SAFE_NAME(iCurrentLevel) = 0,                               \
                {if (FFF_SAFE_NAME(iLevel) < 0) {                               \
                    FFF_SAFE_NAME(iLevel) = (INT16_MAX);                        \
                }},                                                             \
                (void)FFF_SAFE_NAME(iCurrentLevel))                             \
        for(const arm_file_node_t                                               \
                *FFF_SAFE_NAME(ptStart) = arm_fff_helper_find_path((__PATH)),   \
                *FFF_SAFE_NAME(ptNode) = FFF_SAFE_NAME(ptStart),                \
                *__ITEM_NAME = FFF_SAFE_NAME(ptNode);                           \
                                                                                \
                /* loop end condition */                                        \
                ({                                                              \
                    __ITEM_NAME = FFF_SAFE_NAME(ptNode);                        \
                    ( NULL != FFF_SAFE_NAME(ptNode));                           \
                 });                                                            \
                                                                                \
        ({                                                                      \
        if (    (NULL != FFF_SAFE_NAME(ptNode)->ptList)                         \
            &&  (FFF_SAFE_NAME(iCurrentLevel) < FFF_SAFE_NAME(iLevel))) {       \
            FFF_SAFE_NAME(ptNode) = FFF_SAFE_NAME(ptNode)->ptList;              \
            FFF_SAFE_NAME(iCurrentLevel)++;                                     \
        } else do {                                                             \
            /*! the final node  */                                              \
            if (   (NULL == FFF_SAFE_NAME(ptNode)->ptNext)                      \
                || (FFF_SAFE_NAME(ptNode) == FFF_SAFE_NAME(ptNode)->ptNext)) {  \
                                                                                \
                if (    (NULL == FFF_SAFE_NAME(ptNode)->ptParent)               \
                    ||  (0 == FFF_SAFE_NAME(iCurrentLevel))) {                  \
                    FFF_SAFE_NAME(ptNode) = NULL;                               \
                    break ;                                                     \
                } else if (FFF_SAFE_NAME(ptNode) == FFF_SAFE_NAME(ptStart)) {   \
                    FFF_SAFE_NAME(ptNode) = NULL;                               \
                    break ;                                                     \
                }                                                               \
                                                                                \
                FFF_SAFE_NAME(iCurrentLevel)--;                                 \
                FFF_SAFE_NAME(ptNode) = FFF_SAFE_NAME(ptNode)->ptParent;        \
            } else {                                                            \
                /*! check next file */                                          \
                FFF_SAFE_NAME(ptNode) = FFF_SAFE_NAME(ptNode)->ptNext;          \
                break;                                                          \
            }                                                                   \
        } while(true);                                                          \
        })) 



/*
 * HOW TO USE
 
    A example to list all the files and folers, and open all the files
 
    fff_foreach("/", -1, ptItem) {
        
        if (ptItem->chID == 0) {
            printf("Folder: %s\r\n", ptItem->ppchPathString[0]);
        } else {
            printf("Open File: %s...", ptItem->ppchPathString[0]);
            
            const arm_file_node_t *ptNode = NULL;
            if (ptItem->bCanRead && ptItem->bCanWrite) {
                ptNode = arm_fff_open_node((arm_file_node_t *)ptItem, "r+");
            } else if (ptItem->bCanRead){
                ptNode = arm_fff_open_node((arm_file_node_t *)ptItem, "r");
            } else if (ptItem->bCanWrite){
                ptNode = arm_fff_open_node((arm_file_node_t *)ptItem, "w");
            }
            
            if (NULL == ptNode) {
                printf("Failed\r\n");
            } else {
                printf("OK\r\n");
            }
            
            arm_fff_close((arm_file_node_t *)ptItem);
        }
    }

    printf("\r\n\r\n Use stdio.h\r\n");
    fff_foreach("/", -1, ptItem) {
        
        char chPathBuffer[84];
        arm_fff_helper_get_path_string(ptItem, chPathBuffer, sizeof(chPathBuffer));

        if (ptItem->chID == 0) {
            printf("Folder: %s\r\n", chPathBuffer);
        } else {
            printf("Open File: %s...", chPathBuffer);

            FILE *ptFile = NULL;
            
            if (ptItem->bCanRead && ptItem->bCanWrite) {
                ptFile = fopen(chPathBuffer, "r+");
            } else if (ptItem->bCanRead){
                ptFile = fopen(chPathBuffer, "r");
            } else if (ptItem->bCanWrite){
                ptFile = fopen(chPathBuffer, "w");
            }
            
            if (NULL == ptFile) {
                printf("Failed\r\n");
            } else {
                printf("OK\r\n");
            }
            
            fclose(ptFile);
        }
    }
 
 
 */

/*!
 * \brief access all the file node, including folders and files
 *
 * \note prototype 1
 *       fff_using(path string) {
 *           code body 
 *       }
 * 
 * \note prototype 2
 *       fff_using(path string, level) {
 *           code body
 *       }
 *
 * \note prototype 3
 *       fff_using( path string, level, item variable name) {
 *           code body 
 *       }
 *
 */
#define fff_foreach(...)                                                        \
            FFF_CONNECT2(   __fff_foreach,                                      \
                            __FFF_VA_NUM_ARGS(__VA_ARGS__))(__VA_ARGS__)


/*----------------------------------------------------------------------------*
 * Common Wrapper                                                             *
 *----------------------------------------------------------------------------*/
#define __fff_attribute(__NAME, ...)      .__NAME = __VA_ARGS__
#define fff_attribute(__NAME, ...)      __fff_attribute(__NAME, __VA_ARGS__)

#define __fff_path(...)                                                         \
            .use_as__arm_file_node_t                                            \
                .ppchPathString = (const char *const []){__VA_ARGS__},          \
            .use_as__arm_file_node_t                                            \
                .chAliasCount = (sizeof((const char *[]){__VA_ARGS__})          \
                            /   sizeof(const char *))                          

#define fff_path(...)           __fff_path(__VA_ARGS__)

                            
#define fff_disk_path(...)      __fff_path(__VA_ARGS__)
#define fff_folder_path(...)    __fff_path(__VA_ARGS__)

#define fff_access(...)         __VA_ARGS__

#define fff_list(...)           .tList = {__VA_ARGS__}


#define FFF_READ_ONLY                                                           \
        fff_attribute(use_as__arm_file_node_t.bCanRead,true),                   \
        fff_attribute(use_as__arm_file_node_t.bCanWrite,false)
      
#define FFF_READ_WRITE                                                          \
        fff_attribute(use_as__arm_file_node_t.bCanRead,true),                   \
        fff_attribute(use_as__arm_file_node_t.bCanWrite,true)
      
#define FFF_NO_ACCESS                                                           \
        fff_attribute(use_as__arm_file_node_t.bCanRead,false),                  \
        fff_attribute(use_as__arm_file_node_t.bCanWrite,false)

/*----------------------------------------------------------------------------*
 * Folder Wrapper                                                             *
 *----------------------------------------------------------------------------*/
//#define __use_fff_folder(__NAME, ...)                                           \
//    struct {                                                                    \
//        /*implement(arm_folder_node_t)*/                                        \
//        implement_ex(                                                           \
//            struct {                                                            \
//                /* implement(arm_file_node_t) */                                \
//                implement_ex(                                                   \
//                    struct {                                                    \
//                        uint8_t chID;                                           \
//                        union {                                                 \
//                            struct {                                            \
//                                uint8_t bIsValid        : 1;                    \
//                                uint8_t bIsVisible      : 1;                    \
//                                uint8_t                 : 2;                    \
//                                uint8_t bCanRead        : 1;                    \
//                                uint8_t bCanWrite       : 1;                    \
//                                uint8_t bCanSeek        : 1;                    \
//                                uint8_t bIsStream       : 1;                    \
//                            };                                                  \
//                            uint8_t chAttribute;                                \
//                        };                                                      \
//                                                                                \
//                        uint8_t    chAliasCount;                                \
//                        const char *const* ppchPathString;                      \
//                                                                                \
//                        const arm_file_node_t *ptParent;                        \
//                        const arm_file_node_t *ptNext;                          \
//                        const arm_file_node_t *ptList;                          \
//                    },                                                          \
//                use_as__arm_file_node_t)                                        \
//                const arm_file_node_t ptFirstNode[];                            \
//            },                                                                  \
//        use_as__arm_folder_node_t)                                              \
//        implement_ex(                                                           \
//            struct {                                                            \
//                __VA_ARGS__                                                     \
//            },                                                                  \
//        tList)                                                                  \
//    } __NAME
//#define use_fff_folder(__NAME, ...)     __use_fff_folder(__NAME, __VA_ARGS__)


#define __use_fff_folder_begin(__NAME)                                          \
    struct {                                                                    \
        /*implement(arm_folder_node_t)*/                                        \
        implement_ex(                                                           \
            struct {                                                            \
                /* implement(arm_file_node_t) */                                \
                implement_ex(                                                   \
                    struct {                                                    \
                        uint8_t chID;                                           \
                        union {                                                 \
                            struct {                                            \
                                uint8_t bIsValid        : 1;                    \
                                uint8_t bIsVisible      : 1;                    \
                                uint8_t                 : 2;                    \
                                uint8_t bCanRead        : 1;                    \
                                uint8_t bCanWrite       : 1;                    \
                                uint8_t bCanSeek        : 1;                    \
                                uint8_t bIsStream       : 1;                    \
                            };                                                  \
                            uint8_t chAttribute;                                \
                        };                                                      \
                                                                                \
                        uint8_t    chAliasCount;                                \
                        const char *const* ppchPathString;                      \
                                                                                \
                        const arm_file_node_t *ptParent;                        \
                        const arm_file_node_t *ptNext;                          \
                        const arm_file_node_t *ptList;                          \
                    },                                                          \
                use_as__arm_file_node_t)                                        \
                const arm_file_node_t ptFirstNode[];                            \
            },                                                                  \
        use_as__arm_folder_node_t)                                              \
        struct {

#define __use_fff_folder_end(__NAME)                                            \
        } /*tList*/;                                                            \
    } __NAME

#define use_fff_folder_begin(__NAME)    __use_fff_folder_begin(__NAME)
#define use_fff_folder_end(__NAME)      __use_fff_folder_end(__NAME)

#define __fff_folder(__NAME, __PARENT, __NEXT, ...)                             \
    .__NAME = {                                                                 \
        /*! default configuration */                                            \
        fff_attribute(use_as__arm_file_node_t .bIsVisible, true),               \
                                                                                \
        /*! user configuration */                                               \
        __VA_ARGS__                                                             \
                                                                                \
        /*! override user definition */                                         \
        fff_attribute(use_as__arm_file_node_t.chID, FFF_FILE_TYPE_ID_FOLDER),   \
        fff_attribute(use_as__arm_file_node_t.bIsValid, true),                  \
        fff_access(FFF_NO_ACCESS),                                              \
                                                                                \
        .use_as__arm_file_node_t                                                \
                .ptParent = (const arm_file_node_t *)(__PARENT),                \
        .use_as__arm_file_node_t                                                \
                .ptNext = (const arm_file_node_t *)&((__PARENT)->__NEXT),       \
        .use_as__arm_file_node_t                                                \
                .ptList = (const arm_file_node_t *)                             \
                    &((__PARENT)->__NAME.ptFirstNode),                          \
    }
#define fff_folder(__NAME, __PARENT, __NEXT, ...)                               \
            __fff_folder(__NAME, __PARENT, __NEXT, __VA_ARGS__)


/*----------------------------------------------------------------------------*
 * Disk Wrapper                                                               *
 *----------------------------------------------------------------------------*/
//#define use_fff_disk(__NAME, ...)     __use_fff_folder(__NAME, __VA_ARGS__)

#define use_fff_disk_begin(__NAME)    __use_fff_folder_begin(__NAME)
#define use_fff_disk_end(__NAME)      __use_fff_folder_end(__NAME)

#define __fff_disk(__NAME, __PARENT, __NEXT, ...)                               \
    .__NAME = {                                                                 \
        /*! default configuration */                                            \
        fff_attribute(use_as__arm_file_node_t .bIsVisible, true),               \
                                                                                \
        /*! user configuration */                                               \
        __VA_ARGS__                                                             \
                                                                                \
        /*! override user definition */                                         \
        fff_attribute(use_as__arm_file_node_t.chID, FFF_FILE_TYPE_ID_FOLDER),   \
        fff_attribute(use_as__arm_file_node_t.bIsValid, true),                  \
        fff_access(FFF_NO_ACCESS),                                              \
                                                                                \
        .use_as__arm_file_node_t.ptParent = NULL,                               \
        .use_as__arm_file_node_t                                                \
                .ptNext = (const arm_file_node_t *)&((__PARENT)->__NEXT),       \
        .use_as__arm_file_node_t                                                \
                .ptList = (const arm_file_node_t *)                             \
                    &((__PARENT)->__NAME.ptFirstNode),                          \
    }
#define fff_disk(__NAME, __PARENT, __NEXT, ...)                                 \
            __fff_disk(__NAME, __PARENT, __NEXT, __VA_ARGS__)
            
/*----------------------------------------------------------------------------*
 * Disk Wrapper                                                               *
 *----------------------------------------------------------------------------*/

//#define __def_fff(__NAME, ...)                                                  \
//    typedef use_fff_folder( fff_##__NAME##_t, __VA_ARGS__);                     \
//    extern const fff_##__NAME##_t __NAME;

//#define def_fff(__NAME, ...)           __def_fff(__NAME, __VA_ARGS__)


#define __def_fff_begin(__NAME)                                                 \
            typedef use_fff_folder_begin( fff_##__NAME##_t)

#define __def_fff_end(__NAME)                                                   \
            use_fff_folder_end( fff_##__NAME##_t);                              \
            extern const fff_##__NAME##_t __NAME;

#define def_fff_begin(__NAME)       __def_fff_begin(__NAME)
#define def_fff_end(__NAME)         __def_fff_end(__NAME)

#define __imp_fff(__NAME, ...)                                                  \
    const fff_##__NAME##_t __NAME = {                                           \
        /*! default configuration */                                            \
        fff_attribute(use_as__arm_file_node_t .bIsVisible, true),               \
        fff_path("ROOT"),                                                       \
                                                                                \
        /*! user configuration */                                               \
        __VA_ARGS__                                                             \
                                                                                \
        /*! override user definition */                                         \
        fff_attribute(use_as__arm_file_node_t.chID, FFF_FILE_TYPE_ID_FOLDER),   \
        fff_attribute(use_as__arm_file_node_t.bIsValid, true),                  \
        fff_access(FFF_NO_ACCESS),                                              \
                                                                                \
        .use_as__arm_file_node_t                                                \
                .ptParent = NULL,                                               \
        .use_as__arm_file_node_t                                                \
                .ptNext = NULL,                                                 \
        .use_as__arm_file_node_t                                                \
                .ptList = (const arm_file_node_t *)                             \
                    &((__NAME).ptFirstNode),          \
    }
#define imp_fff(__NAME, ...)           __imp_fff(__NAME, __VA_ARGS__)

/*============================ TYPES =========================================*/

#include "sys/custom_file.h"
//typedef struct arm_file_node_t arm_file_node_t;

//struct arm_file_node_t {
//    uint8_t chID;
//    union {
//        struct {
//            uint8_t bIsValid        : 1;
//            uint8_t bIsVisible      : 1;
//            uint8_t                 : 2;
//            uint8_t bCanRead        : 1;
//            uint8_t bCanWrite       : 1;
//            uint8_t bCanSeek        : 1;
//            uint8_t bIsStream       : 1; /* whether we know size or not */
//        };
//        uint8_t chAttribute;
//    };
//    
//    uint8_t    chAliasCount;
//    const char *const* ppchPathString;
//    
//    const arm_file_node_t *ptParent;        //!< parent
//    const arm_file_node_t *ptNext;          //!< next file in the same folder
//    const arm_file_node_t *ptList;          //!< !NULL means folder
//} ;

//typedef struct arm_folder_node_t {
//    implement(arm_file_node_t)
//    const arm_file_node_t ptFirstNode[];
//} arm_folder_node_t;


//typedef struct __FILE {
//    implement(arm_file_node_t)
//}__FILE;

typedef struct arm_file_status_t {
    uint32_t bIsReady          : 1;
    uint32_t                   : 31;
} arm_file_status_t;

typedef enum {
    ARM_FFF_ERR_NO_BUFFER           = -9,
    ARM_FFF_ERR_IO_ERROR            = -8,
    ARM_FFF_ERR_NOT_ENOUGH_RESOURCE = -7,
    ARM_FFF_ERR_INVALID_PATH        = -6,
    ARM_FFF_ERR_UNAVAILABLE         = -5,
    ARM_FFF_ERR_UNSUPPORTED         = -4,
    ARM_FFF_ERR_INVALID_PARAM       = -3,
    ARM_FFF_ERR_INVALID_PTR         = -2,
    ARM_FFF_ERR_UNKOWN              = -1,
    ARM_FFF_ERR_NONE = 0,
}arm_fff_err_t;

typedef struct i_arm_file_node_t {
    struct {
        arm_file_status_t   (*Status)       (const arm_file_node_t *ptNode);
        arm_fff_err_t       (*Open)         (const arm_file_node_t *ptNode, 
                                             uint_fast8_t chAttribute);
        void                (*Close)        (const arm_file_node_t *ptNode);
        arm_fff_err_t       (*Flush)        (const arm_file_node_t *ptNode);
        bool                (*EndOfStream)  (const arm_file_node_t *ptNode);
    }Control;
    
    struct {
        int_fast32_t        (*Get)          (const arm_file_node_t *ptNode);
        int_fast32_t        (*Set)          (const arm_file_node_t *ptNode, 
                                             int_fast32_t nOffset,
                                             int_fast32_t nWhence);
    }Position; 
    
    struct {
        int_fast32_t        (*Get)          (const arm_file_node_t *ptNode);
    }Size;
    
    int_fast32_t            (*Write)        (const arm_file_node_t *ptNode,
                                             void *pBuffer,
                                             int_fast32_t nSize);
    int_fast32_t            (*Read)         (const arm_file_node_t *ptNode,
                                             void *pBuffer,
                                             int_fast32_t nSize);
} i_arm_file_node_t;

typedef struct arm_user_file_type_list_t{
    const i_arm_file_node_t   *ptList;
    uint_fast8_t              chCount;
} arm_user_file_type_list_t;

typedef struct arm_fff_cfg_t {
    const arm_file_node_t       *ptRoot;
    const char                  *pchWorkingPath;
    arm_user_file_type_list_t   tTypes;
} arm_fff_cfg_t;


#if !__IS_COMPILER_ARM_COMPILER__

/*
 * This type is used by the _sys_ I/O functions to denote an open
 * file.
 */
typedef int FILEHANDLE;

typedef __FILE FILE;

#endif

/*============================ GLOBAL VARIABLES ==============================*/

/*
 * These names should be special strings which will be recognised
 * by _sys_open and will cause it to return the standard I/O
 * handles, instead of opening a real file.
 */
extern const char __stdin_name[];
extern const char __stdout_name[];
extern const char __stderr_name[];

extern FILE __stdin, __stdout, __stderr;

/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/


extern 
arm_fff_err_t arm_fff_init( const arm_fff_cfg_t *ptCFG);

extern
const arm_file_node_t *arm_fff_open(const char *pchPath, uint16_t wFeature);

extern
const arm_file_node_t *arm_fff_open_node(   arm_file_node_t *ptNode, 
                                            const char *pchMode);

extern
bool arm_fff_close(arm_file_node_t * ptNode);

extern
bool arm_fff_eof( arm_file_node_t * ptNode);

extern
bool arm_fff_flush( arm_file_node_t * ptNode);

extern
int arm_fff_seek(   arm_file_node_t * ptNode,
                    int32_t nOffset,
                    int32_t nWhence);

extern
long int arm_fff_tell(arm_file_node_t * ptNode);

extern
int_fast32_t arm_fff_read(  arm_file_node_t *ptNode,
                            uint8_t *pchBuffer,
                            int_fast32_t nSize);

extern
size_t arm_fff_helper_read_file(arm_file_node_t *ptInputFile, 
                                void *pBuffer, 
                                size_t tSize);

extern
int_fast32_t arm_fff_write( arm_file_node_t *ptNode,
                            uint8_t *pchBuffer,
                            int_fast32_t nSize);

extern
size_t arm_fff_helper_write_file(   arm_file_node_t *ptOutputFile, 
                                    void *pBuffer, 
                                    size_t tSize);

extern
int_fast32_t arm_fff_read_byte(arm_file_node_t *ptNode);

extern
bool arm_fff_write_byte(arm_file_node_t *ptNode, uint_fast8_t chByte);

/*----------------------------------------------------------------------------*
 * Helper Functions                                                           *
 *----------------------------------------------------------------------------*/
extern
arm_fff_err_t arm_fff_helper_set_working_path(const char *pchWorkingPath);

extern 
const arm_file_node_t * arm_fff_helper_get_working_path(void);

extern
const arm_file_node_t * arm_fff_helper_find_path(const char *pchPath);

extern
char *arm_fff_helper_get_path_string(   const arm_file_node_t *ptPathNode,
                                        char *pchBuffer, 
                                        size_t wBufferSize);

extern
bool arm_fff_helper_list_folder_structure(  const char *pchPath, 
                                            int_fast16_t iLevel);

/*----------------------------------------------------------------------------*
 * Dependency                                                                 *
 *----------------------------------------------------------------------------*/
extern
void *arm_fff_malloc(size_t tSize);

extern
void arm_fff_free(void *);

/*============================ INCLUDES ======================================*/
#include "./__memory_file.h"


#if defined(__clang__)
#   pragma clang diagnostic pop
#endif

#ifdef   __cplusplus
}
#endif

#endif
