/*****************************************************************************
 *   Copyright(C)2009-2019 by GorgonMeducer<embedded_zhuoran@hotmail.com>    *
 *                       and  SimonQian<simonqian@simonqian.com>             *
 *         with support from  HenryLong<henry_long@163.com>                  *
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


#if defined(__OOC_RELEASE__)
#   undef __OOC_DEBUG__
#   define __OOC_DEBUG__        1
#endif


#if     !defined(__PLOOC_CLASS_USE_STRICT_TEMPLATE__)                           \
    &&  !defined(__PLOOC_CLASS_USE_SIMPLE_TEMPLATE__)                           \
    &&  !defined(__PLOOC_CLASS_USE_BLACK_BOX_TEMPLATE__)
#   define __PLOOC_CLASS_USE_SIMPLE_TEMPLATE__          1
#endif


#if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 199901L

#ifndef __OOC_DEBUG__
#   define __OOC_DEBUG__
#   warning For C89/90, __OOC_DEBUG__ is enforced. 
#endif

#if defined(__PLOOC_CLASS_USE_STRICT_TEMPLATE__) 
#   undef __PLOOC_CLASS_USE_STRICT_TEMPLATE__
#   define __PLOOC_CLASS_USE_SIMPLE_TEMPLATE__
#endif

#if defined(__PLOOC_CLASS_USE_BLACK_BOX_TEMPLATE__) 
#   undef __PLOOC_CLASS_USE_BLACK_BOX_TEMPLATE__
#   define __PLOOC_CLASS_USE_SIMPLE_TEMPLATE__
#endif

#   if !defined(__OOC_DEBUG__) || !defined(__PLOOC_CLASS_USE_SIMPLE_TEMPLATE__)
#       error \
You must use __OOC_DEBUG__ (or __OOC_RELEASE__) together with the\
__PLOOC_CLASS_USE_SIMPLE_TEMPLATE__ in ANSI-C89/90. 
#   endif
#endif

#ifndef __PLOOC_CLASS_H__           
#define __PLOOC_CLASS_H__           

 
/******************************************************************************
 * HOW TO USE                                                                 *
 ******************************************************************************/
 //! TODO: Add How to use

   

/*============================ INCLUDES ======================================*/
#include <stdint.h>
/*! \NOTE the uint_fast8_t used in this header file is defined in stdint.h 
          if you don't have stdint.h supported in your toolchain, you should
          define uint_fast8_t all by yourself with following rule:
          a. if the target processor is 8 bits, define it as uint8_t
          b. if the target processor is 16 bits, define it as uint16_t 
          c. if the target processor is 32 bits, define it as uint32_t
          d. if the target processor is 64 bits, define it as either uint32_t or
             uint64_t
 */

/*============================ MACROS ========================================*/
/*!\ node if you want your code more "elegent", say you want to use "this" with 
 *        "." rather than a pointer with "->", you can add following macros to
 *        your code, assuming the variable name of the object pointer is "ptThis".
 *        If your object pointer has a different name, please feel free to change
 *        the macro by yourself
 
#undef this
#define this         (*ptThis)

*/
#include "./plooc.h"   



/*============================ MACROFIED FUNCTIONS ===========================*/

//! @{
#ifndef __PLOOC_CONNECT2
#   define __PLOOC_CONNECT2( a, b)              a##b
#endif
#ifndef __PLOOC_CONNECT3
#   define __PLOOC_CONNECT3( a, b, c)           a##b##c
#endif
#ifndef __PLOOC_CONNECT4
#   define __PLOOC_CONNECT4( a, b, c, d)        a##b##c##d
#endif
//! @}

#ifndef __PLOOC_ALIGN
#   define __PLOOC_ALIGN(__N)           __attribute__((aligned(__N)))
#endif
#ifndef PLOOC_ALIGN
#   define PLOOC_ALIGN(__N)             __PLOOC_ALIGN(__N)
#endif
/*
#ifndef PLOOC_DEFAULT_OBJ_ALIGN
#   define PLOOC_DEFAULT_OBJ_ALIGN      sizeof(uint_fast8_t)
#endif
*/
#ifndef PLOOC_PACKED
#   define PLOOC_PACKED                 __attribute__((packed))
#endif

//! @{
#ifndef PLOOC_CONNECT2
#   define PLOOC_CONNECT2( a, b)        __PLOOC_CONNECT2( a, b)
#endif
#ifndef PLOOC_CONNECT3
#   define PLOOC_CONNECT3( a, b, c)     __PLOOC_CONNECT3( a, b, c)
#endif
#ifndef PLOOC_CONNECT4
#   define PLOOC_CONNECT4( a, b, c, d)  __PLOOC_CONNECT4( a, b, c, d)
#endif
//! @}
                             
#ifndef PLOOC_UNUSED_PARAM
#   define PLOOC_UNUSED_PARAM(__N)      do {(__N) = (__N);}while(0)
#endif


#   if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 199901L

#   ifndef PLOOC_ALIGNOF
#       define PLOOC_ALIGNOF(__TYPE)           __alignof__(__TYPE)
#   endif

#   define PLOOC_ALIGNOF_STRUCT(__TYPE)       PLOOC_ALIGNOF(struct {__TYPE})
#   define PLOOC_SIZEOF_STRUCT(__TYPE)        sizeof(struct {__TYPE})

#else

#   ifndef PLOOC_ALIGNOF
#       define PLOOC_ALIGNOF(...)           __alignof__(__VA_ARGS__)
#   endif

#   define PLOOC_ALIGNOF_STRUCT(...)       PLOOC_ALIGNOF(struct {__VA_ARGS__})
#   define PLOOC_SIZEOF_STRUCT(...)        sizeof(struct {__VA_ARGS__})

#   define PLOOC_VISIBLE(...)                                                   \
        struct  {                                                               \
            __VA_ARGS__                                                         \
        }PLOOC_ALIGN(PLOOC_ALIGNOF_STRUCT(__VA_ARGS__));


#   if  !defined (__PLOOC_CLASS_USE_NO_STRUCT_MASK__)
        
#       define PLOOC_INVISIBLE(...)                                             \
        uint8_t PLOOC_CONNECT4(_,__LINE__,__COUNTER__,_chMask)                  \
                [PLOOC_SIZEOF_STRUCT(__VA_ARGS__)]                              \
                PLOOC_ALIGN(PLOOC_ALIGNOF_STRUCT(__VA_ARGS__));

#   else
#       define PLOOC_INVISIBLE(...)                                             \
            struct {                                                            \
                __VA_ARGS__                                                     \
            }   PLOOC_CONNECT3(zzz_, __LINE__,__COUNTER__)                      \
                PLOOC_ALIGN(PLOOC_ALIGNOF_STRUCT(__VA_ARGS__));
#   endif   /* __PLOOC_CLASS_USE_NO_STRUCT_MASK__ */

#   define __PLOOC_PRO_struct                          struct
#   define __PLOOC_PRI_struct                          struct 
#   define __PLOOC_EXT_struct                          struct
#   define __PLOOC_PRO_union                           union
#   define __PLOOC_PRI_union                           union 
#   define __PLOOC_EXT_union                           union
#   define __PLOOC_EXT_uint8_t                         uint8_t
#   define __PLOOC_PRI_uint8_t                         uint8_t
#   define __PLOOC_PRO_uint8_t                         uint8_t
#   define __PLOOC_EXT_
#   define __PLOOC_PRI_
#   define __PLOOC_PRO_



#   ifdef __OOC_DEBUG__

//! \brief wrapper for shell type
#       define __PLOOC_EXT__public_member(...)      PLOOC_VISIBLE(__VA_ARGS__)
#       define __PLOOC_EXT__private_member(...)     PLOOC_VISIBLE(__VA_ARGS__)
#       define __PLOOC_EXT__protected_member(...)   PLOOC_VISIBLE(__VA_ARGS__)
#       define __PLOOC_EXT__which(...)              PLOOC_VISIBLE(__VA_ARGS__)

//! \brief wrapper for internal private type
#       define __PLOOC_PRI__public_member(...)      PLOOC_VISIBLE(__VA_ARGS__)
#       define __PLOOC_PRI__private_member(...)     PLOOC_VISIBLE(__VA_ARGS__)
#       define __PLOOC_PRI__protected_member(...)   PLOOC_VISIBLE(__VA_ARGS__)
#       define __PLOOC_PRI__which(...)              PLOOC_VISIBLE(__VA_ARGS__)

//! \brief wrapper for internal protected type
#       define __PLOOC_PRO__public_member(...)      PLOOC_VISIBLE(__VA_ARGS__)
#       define __PLOOC_PRO__private_member(...)     PLOOC_VISIBLE(__VA_ARGS__)
#       define __PLOOC_PRO__protected_member(...)   PLOOC_VISIBLE(__VA_ARGS__)
#       define __PLOOC_PRO__which(...)              PLOOC_VISIBLE(__VA_ARGS__)
#   else

//! \brief wrapper for shell type
#       define __PLOOC_EXT__public_member(...)      PLOOC_VISIBLE(__VA_ARGS__)
#       define __PLOOC_EXT__private_member(...)     PLOOC_INVISIBLE(__VA_ARGS__)
#       define __PLOOC_EXT__protected_member(...)   PLOOC_INVISIBLE(__VA_ARGS__)
#       define __PLOOC_EXT__which(...)              PLOOC_VISIBLE(__VA_ARGS__)

//! \brief wrapper for internal private type
#       define __PLOOC_PRI__public_member(...)      PLOOC_VISIBLE(__VA_ARGS__)
#       define __PLOOC_PRI__private_member(...)     PLOOC_VISIBLE(__VA_ARGS__)
#       define __PLOOC_PRI__protected_member(...)   PLOOC_VISIBLE(__VA_ARGS__)
#       define __PLOOC_PRI__which(...)              PLOOC_VISIBLE(__VA_ARGS__)

//! \brief wrapper for internal protected type
#       define __PLOOC_PRO__public_member(...)      PLOOC_VISIBLE(__VA_ARGS__)
#       define __PLOOC_PRO__private_member(...)     PLOOC_INVISIBLE(__VA_ARGS__)
#       define __PLOOC_PRO__protected_member(...)   PLOOC_VISIBLE(__VA_ARGS__)
#       define __PLOOC_PRO__which(...)              PLOOC_VISIBLE(__VA_ARGS__)
#   endif


#endif

#endif

#   if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 199901L
#   undef which
#   define which(__DECLARE)                     ,_which(__DECLARE)
#else
#   undef which
#   define which(...)                           ,_which(__VA_ARGS__)
#endif

#undef private_member
#define private_member                          ,_private_member

#undef protected_member
#define protected_member                        ,_protected_member

#undef public_member
#define public_member                           ,_public_member


/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
/*============================ INCLUDES ======================================*/
#if     defined(__PLOOC_CLASS_USE_STRICT_TEMPLATE__)
#   include "./plooc_class_strict.h"
#elif   defined(__PLOOC_CLASS_USE_SIMPLE_TEMPLATE__)
#   if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 199901L
#       include "./plooc_class_simple_c90.h"
#   else
#       include "./plooc_class_simple.h"
#   endif
#elif   defined(__PLOOC_CLASS_USE_BLACK_BOX_TEMPLATE__)
#   include "./plooc_class_black_box.h"
#else
#   include "./plooc_class_simple.h"
#endif

#undef __PLOOC_CLASS_USE_STRICT_TEMPLATE__
#undef __PLOOC_CLASS_USE_SIMPLE_TEMPLATE__
#undef __PLOOC_CLASS_USE_BLACK_BOX_TEMPLATE__
#undef __PLOOC_CLASS_IMPLEMENT
#undef __PLOOC_CLASS_INHERIT
