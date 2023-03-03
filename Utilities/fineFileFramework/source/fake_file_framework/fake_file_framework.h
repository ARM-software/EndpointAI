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

#ifndef __FAKE_FILE_FRAMEWORK_H__
#define __FAKE_FILE_FRAMEWORK_H__

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
/*============================ MACROS ========================================*/
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


/*----------------------------------------------------------------------------*
 * Common Wrapper                                                             *
 *----------------------------------------------------------------------------*/
#define __fff_attribute(__NAME, ...)      .__NAME = __VA_ARGS__
#define fff_attribute(__NAME, ...)      __fff_attribute(__NAME, __VA_ARGS__)

#define __fff_path(...)                                                         \
            .ppchPathString = (const char *const []){__VA_ARGS__},              \
            .chAliasCount = (   sizeof((const char *[]){__VA_ARGS__})           \
                            /   sizeof(const char *))                          
                            
#define fff_path(...)       __fff_path(__VA_ARGS__)

#define fff_access(...)       __VA_ARGS__

#define fff_list(...)       .tList = {__VA_ARGS__}

#define FFF_READ_ONLY                                                           \
        fff_attribute(bCanRead,true),                                           \
        fff_attribute(bCanWrite,false)
      
#define FFF_READ_WRITE                                                          \
        fff_attribute(bCanRead,true),                                           \
        fff_attribute(bCanWrite,true)
      
#define FFF_NO_ACCESS                                                           \
        fff_attribute(bCanRead,false),                                          \
        fff_attribute(bCanWrite,false)

/*----------------------------------------------------------------------------*
 * Folder Wrapper                                                             *
 *----------------------------------------------------------------------------*/
#define __use_fff_folder(__NAME, ...)                                           \
    struct {                                                                    \
        implement(arm_folder_node_t)                                            \
        implement_ex(                                                           \
            struct {                                                            \
                __VA_ARGS__                                                     \
            },                                                                  \
        tList)                                                                  \
    } __NAME
#define use_fff_folder(__NAME, ...)     __use_fff_folder(__NAME, __VA_ARGS__)


#define __fff_folder(__NAME, __PARENT, __NEXT, ...)                             \
    .__NAME = {                                                                 \
        /*! default configuration */                                            \
        fff_attribute(chID,FFF_FILE_TYPE_ID_FOLDER),                            \
        fff_attribute(bIsVisible,true),                                         \
                                                                                \
        /*! user configuration */                                               \
        __VA_ARGS__                                                             \
                                                                                \
        /*! override user definition */                                         \
        fff_attribute(bIsValid,true),                                           \
        fff_access(FFF_NO_ACCESS),                                              \
        .ptParent = (const arm_file_node_t *)(__PARENT),                        \
        .ptNext = (const arm_file_node_t *)&((__PARENT)->__NEXT),               \
        .ptList = (const arm_file_node_t *)&((__PARENT)->__NAME.ptFirstNode),   \
    }
#define fff_folder(__NAME, __PARENT, __NEXT, ...)                               \
            __fff_folder(__NAME, __PARENT, __NEXT, __VA_ARGS__)


/*----------------------------------------------------------------------------*
 * Disk Wrapper                                                               *
 *----------------------------------------------------------------------------*/
#define use_fff_disk(__NAME, ...)     __use_fff_folder(__NAME, __VA_ARGS__)

#define __fff_disk(__NAME, __PARENT, __NEXT, ...)                               \
    .__NAME = {                                                                 \
        /*! default configuration */                                            \
        fff_attribute(chID,FFF_FILE_TYPE_ID_FOLDER),                            \
        fff_attribute(bIsVisible,true),                                         \
                                                                                \
        /*! user configuration */                                               \
        __VA_ARGS__                                                             \
                                                                                \
        /*! override user definition */                                         \
        fff_attribute(bIsValid,true),                                           \
        fff_access(FFF_NO_ACCESS),                                              \
        .ptParent = NULL,                                                       \
        .ptNext = (const arm_file_node_t *)&((__PARENT)->__NEXT),               \
        .ptList = (const arm_file_node_t *)&((__PARENT)->__NAME.ptFirstNode),   \
    }
#define fff_disk(__NAME, __PARENT, __NEXT, ...)                                 \
            __fff_disk(__NAME, __PARENT, __NEXT, __VA_ARGS__)
            
/*----------------------------------------------------------------------------*
 * Disk Wrapper                                                               *
 *----------------------------------------------------------------------------*/

#define __def_fff(__NAME, ...)                                                  \
    typedef use_fff_folder( fff_##__NAME##_t, __VA_ARGS__);                     \
    extern const fff_##__NAME##_t __NAME;
    
#define def_fff(__NAME, ...)           __def_fff(__NAME, __VA_ARGS__)

#define __imp_fff(__NAME, ...)                                                  \
    const fff_##__NAME##_t __NAME = {                                           \
        /*! default configuration */                                            \
        fff_attribute(chID,FFF_FILE_TYPE_ID_FOLDER),                            \
        fff_attribute(bIsVisible,true),                                         \
                                                                                \
        /*! user configuration */                                               \
        __VA_ARGS__                                                             \
                                                                                \
        /*! override user definition */                                         \
        fff_attribute(bIsValid,true),                                           \
        fff_access(FFF_NO_ACCESS),                                              \
        .ptParent = NULL,                                                       \
        .ptNext = NULL,                                                         \
        .ptList = (const arm_file_node_t *)&(__NAME.ptFirstNode),               \
    }
#define imp_fff(__NAME, ...)           __imp_fff(__NAME, __VA_ARGS__)

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


struct __FILE {
    implement(arm_file_node_t)
};

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
        arm_fff_err_t       (*Set)          (const arm_file_node_t *ptNode, 
                                             int_fast32_t nPosition, 
                                             int_fast32_t nOffset);
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

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/

extern 
arm_fff_err_t arm_fff_init( const arm_fff_cfg_t *ptCFG);

extern
arm_fff_err_t arm_fff_set_working_path(const char *pchWorkingPath);

extern 
const arm_file_node_t * arm_fff_get_working_path(void);

extern
const arm_file_node_t * arm_fff_find_path(const char *pchPath);

extern
char * arm_fff_get_path_string( const arm_file_node_t *ptPathNode,
                                char *pchBuffer, size_t wBufferSize);

extern
const arm_file_node_t *arm_fff_open(const char *pchPath, uint16_t wFeature);

extern
bool arm_fff_close(arm_file_node_t * ptNode);

extern
bool arm_fff_eof( arm_file_node_t * ptNode);

extern
bool arm_fff_flush( arm_file_node_t * ptNode);

extern
int_fast32_t arm_fff_read(  arm_file_node_t *ptNode,
                            uint8_t *pchBuffer,
                            int_fast32_t nSize);
extern
int_fast32_t arm_fff_write( arm_file_node_t *ptNode,
                            uint8_t *pchBuffer,
                            int_fast32_t nSize);
                            
extern
int_fast32_t arm_fff_read_byte(arm_file_node_t *ptNode);

extern
bool arm_fff_write_byte(arm_file_node_t *ptNode, uint_fast8_t chByte);


/*============================ INCLUDES ======================================*/
#include "./__memory_file.h"

#endif