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
#ifndef __FFF_MEMORY_FILE_H__
#define __FFF_MEMORY_FILE_H__

#include <stdint.h>

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/

/*----------------------------------------------------------------------------*
 * Memory File Wrapper                                                        *
 *----------------------------------------------------------------------------*/

#define fff_mem_buffer(__BUFFER, __SIZE, ...)                                   \
        .ptRuntime = &(arm_fff_mem_file_cb_t){                                  \
                        .ptBlockList = &(__arm_fff_mem_block_t){                \
                                .pchBuffer = (uint8_t *)(__BUFFER),             \
                                .nBufferSize = (__SIZE),                        \
                            },                                                  \
                        .hwPageSize = 256,                                      \
                        __VA_ARGS__                                             \
                    }
#define fff_mem_content(__BUFFER, __SIZE_VAR, ...)                              \
        .ptRuntime = &(arm_fff_mem_file_cb_t){                                  \
                        .ptBlockList = &(__arm_fff_mem_block_t){                \
                                .pchBuffer = (uint8_t *)(__BUFFER),             \
                                .pwExternalSize = (&(__SIZE_VAR)),              \
                            },                                                  \
                        .hwPageSize = 256,                                      \
                        __VA_ARGS__                                             \
                    }
                    

#define __fff_mem_file(__NAME, __PARENT, __NEXT, ...)                           \
    .__NAME = {                                                                 \
        /*! default configuration */                                            \
        fff_attribute(use_as__arm_file_node_t.chID,FFF_FILE_TYPE_ID_MEM_FILE),  \
        fff_attribute(use_as__arm_file_node_t.bIsVisible,true),                 \
        fff_access(FFF_READ_WRITE),                                             \
        .ptRuntime = &(arm_fff_mem_file_cb_t){.hwPageSize = 256},               \
                                                                                \
        /*! user configuration */                                               \
        __VA_ARGS__                                                             \
                                                                                \
        /*! override user definition */                                         \
        fff_attribute(use_as__arm_file_node_t.bIsValid,true),                   \
        .use_as__arm_file_node_t                                                \
            .ptParent = (const arm_file_node_t *)(__PARENT),                    \
        .use_as__arm_file_node_t                                                \
            .ptNext = (const arm_file_node_t *)&((__PARENT)->__NEXT),           \
        .use_as__arm_file_node_t.ptList = NULL,                                 \
    }
#define fff_mem_file(__NAME, __PARENT, __NEXT, ...)                             \
            __fff_mem_file(__NAME, __PARENT, __NEXT, __VA_ARGS__)

/*============================ TYPES =========================================*/

typedef struct __arm_fff_mem_block_t __arm_fff_mem_block_t;

struct __arm_fff_mem_block_t {
    uint8_t         *pchBuffer;
    const uint32_t  *pwExternalSize;
    int_fast32_t    nBufferSize;
    
    int_fast32_t    nContentSize;
    
    __arm_fff_mem_block_t *ptNext;
    __arm_fff_mem_block_t *ptPrevious;
};

typedef struct arm_fff_mem_file_cb_t {
    __arm_fff_mem_block_t *ptBlockList;
    
    struct {
        __arm_fff_mem_block_t *ptCurrent;
        int_fast32_t nInblockOffset;
        int_fast32_t nPosition;
    } tAccess;
    
    const uint16_t  hwPageSize;
    uint8_t         chAttribute;
    int_fast32_t nTotalSize;
    
} arm_fff_mem_file_cb_t;

typedef struct arm_mem_file_node_t {
    implement(arm_file_node_t)
    arm_fff_mem_file_cb_t *ptRuntime;
} arm_mem_file_node_t;

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/

extern
arm_fff_err_t arm_fff_mem_file_open(const arm_file_node_t *ptNode,  
                                    uint_fast8_t chAttribute);

extern
int_fast32_t arm_fff_mem_file_read(const arm_file_node_t *ptNode,
                                    void *pBuffer,
                                    int_fast32_t nSize);

extern
int_fast32_t arm_fff_mem_file_write(const arm_file_node_t *ptNode,
                                    void *pBuffer,
                                    int_fast32_t nSize);
                                    
extern
bool arm_fff_mem_file_eof(const arm_file_node_t *ptNode);

extern
int arm_fff_mem_file_seek(  const arm_file_node_t *ptNode, 
                            int_fast32_t nOffset,
                            int_fast32_t nWhence);
#endif
