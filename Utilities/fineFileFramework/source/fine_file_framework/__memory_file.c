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
#include "./app_cfg.h"

#include "./fine_file_framework.h"
#include "./__memory_file.h"

#include <stdint.h>
#include <stdbool.h>
#include <rt_sys.h>
#include "file_io.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/

static __arm_fff_mem_block_t *__arm_fff_mem_file_request_new_block(
                                                arm_fff_mem_file_cb_t *ptThis)
{
    //! allocate a new memory block for write
    __arm_fff_mem_block_t *ptBlock = malloc(    sizeof(__arm_fff_mem_block_t) 
                    +   this.hwPageSize);
    do {
        if (NULL == ptBlock) {
            break;
        }
        memset( ptBlock, 
                0, 
                (sizeof(__arm_fff_mem_block_t) + this.hwPageSize));
        ptBlock->pchBuffer = (uint8_t *)(ptBlock + 1);
        ptBlock->nBufferSize = this.hwPageSize;
    } while(0);
    return ptBlock;
}

arm_fff_err_t arm_fff_mem_file_open(const arm_file_node_t *ptNode,  
                                    uint_fast8_t chAttribute)
{
    arm_fff_err_t tResult = ARM_FFF_ERR_NONE;
    arm_fff_mem_file_cb_t *ptThis = ((arm_mem_file_node_t *)ptNode)->ptRuntime;

    do {
        if (NULL == ptThis) {
            tResult = ARM_FFF_ERR_UNAVAILABLE;
            break;
        }
        
        this.chAttribute = chAttribute;
        do {
            //! read
            if (OPEN_R == (chAttribute & OPEN_R)) {
                if (NULL == this.ptBlockList) {
                    tResult = ARM_FFF_ERR_NO_BUFFER;
                    break;
                }
                if (    (NULL == this.ptBlockList->pchBuffer) 
                    ||  (   (NULL == this.ptBlockList->pwExternalSize)
                        &&  (0 == this.ptBlockList->nBufferSize))){
                    tResult = ARM_FFF_ERR_NO_BUFFER;
                    break;
                }
                
                //! update nSize
                if (NULL != this.ptBlockList->pwExternalSize) {
                    this.ptBlockList->nBufferSize = *this.ptBlockList->pwExternalSize;
                    this.ptBlockList->nContentSize = this.ptBlockList->nBufferSize;
                }
                
                this.tAccess.ptCurrent = this.ptBlockList;
                this.tAccess.nInblockOffset = 0;
                this.tAccess.nPosition = 0;
                
                //! calculate the total size
                do {
                    int_fast32_t nTotalSize = 0;
                    __arm_fff_mem_block_t *ptBlock = this.ptBlockList;
                    do {
                        nTotalSize += ptBlock->nContentSize;
                        ptBlock = ptBlock->ptNext;
                    } while(ptBlock != NULL);
                    
                    this.nTotalSize = nTotalSize;
                } while(0);
            }
        } while(0);
        
        //! write
        if (OPEN_W == (chAttribute & OPEN_W)) {
            if (NULL == this.ptBlockList) {
                this.ptBlockList = __arm_fff_mem_file_request_new_block(ptThis);
                //! allocate a new memory block for write
                if (NULL == this.ptBlockList) {
                    tResult = ARM_FFF_ERR_NOT_ENOUGH_RESOURCE;
                    break;
                }
                this.tAccess.ptCurrent = this.ptBlockList;
                this.tAccess.nInblockOffset = 0;
                this.tAccess.nPosition = 0;
                this.nTotalSize = 0;
            }
        }
        
        if (OPEN_A == (chAttribute & OPEN_A)) {
            //! move to the end of the file
            
            this.tAccess.ptCurrent = this.ptBlockList;
            int_fast32_t nTotalSize = 0;
            do {
                nTotalSize += this.tAccess.ptCurrent->nContentSize;
                this.tAccess.nInblockOffset = this.tAccess.ptCurrent->nContentSize;
                if (NULL == this.tAccess.ptCurrent->ptNext) {
                    break;
                }
                this.tAccess.ptCurrent = this.tAccess.ptCurrent->ptNext;
            } while(true);

            this.tAccess.nPosition = nTotalSize;
            this.nTotalSize = nTotalSize;
        }
        tResult = ARM_FFF_ERR_NONE;
    } while(0);

    return tResult;
}

int_fast32_t arm_fff_mem_file_read( const arm_file_node_t *ptNode,
                                    void *pBuffer,
                                    int_fast32_t nSize)
{
    int_fast32_t nReadSize = -1;
    arm_fff_mem_file_cb_t *ptThis = ((arm_mem_file_node_t *)ptNode)->ptRuntime;
    
    do {
        if (NULL == ptThis) {
            break;
        }
        
        //! calculate the readable length
        int_fast32_t nReadableLenght = this.nTotalSize - this.tAccess.nPosition;
        int_fast32_t nReadableWithinBlock = 
            this.tAccess.ptCurrent->nContentSize - this.tAccess.nInblockOffset;
            
        while (0 == nReadableWithinBlock) {
            if (NULL == this.tAccess.ptCurrent->ptNext) {
                break;
            }
            this.tAccess.ptCurrent = this.tAccess.ptCurrent->ptNext;
            this.tAccess.nInblockOffset = 0;
            nReadableWithinBlock = 
                this.tAccess.ptCurrent->nContentSize - this.tAccess.nInblockOffset;
        }
            
        nReadSize = MIN(nReadableWithinBlock, nSize);
        nReadSize = MIN(nReadSize, nReadableLenght);
        //! copy content
        memcpy( pBuffer, 
                this.tAccess.ptCurrent->pchBuffer + this.tAccess.nInblockOffset, 
                nReadSize);
        
        this.tAccess.nInblockOffset += nReadSize;
        this.tAccess.nPosition += nReadSize;
        
    } while(0);

    return nReadSize;
}

int_fast32_t arm_fff_mem_file_write(const arm_file_node_t *ptNode,
                                    void *pBuffer,
                                    int_fast32_t nSize)
{
    int_fast32_t nWrittenSize = -1;
    arm_fff_mem_file_cb_t *ptThis = ((arm_mem_file_node_t *)ptNode)->ptRuntime;
    
    do {
        if (NULL == ptThis) {
            break;
        }
        
        int_fast32_t nWritableWithinBlock = 
                this.tAccess.ptCurrent->nBufferSize 
            -   this.tAccess.ptCurrent->nContentSize;
            
        while (0 == nWritableWithinBlock) {
            if (NULL == this.tAccess.ptCurrent->ptNext) {
                //! try to get a new block
                __arm_fff_mem_block_t * ptBlock 
                    = __arm_fff_mem_file_request_new_block(ptThis);
                if (NULL == ptBlock) {
                    //! not enough resource
                    return -1;
                }
                
                //! connect the node to the list
                ptBlock->ptPrevious = this.tAccess.ptCurrent;
                this.tAccess.ptCurrent->ptNext = ptBlock;
            }
            this.tAccess.ptCurrent = this.tAccess.ptCurrent->ptNext;
            this.tAccess.nInblockOffset = 0;
            this.tAccess.ptCurrent->nContentSize = 0;
            nWritableWithinBlock = this.tAccess.ptCurrent->nBufferSize;
        }
        
        nWrittenSize = MIN(nWritableWithinBlock, nSize);
        
        if (nWrittenSize > 0) {
            //! copy content
            memcpy( this.tAccess.ptCurrent->pchBuffer + this.tAccess.nInblockOffset, 
                    pBuffer, 
                    nWrittenSize);
            this.tAccess.nInblockOffset += nWrittenSize;
            this.tAccess.ptCurrent->nContentSize += nWrittenSize;
            this.nTotalSize += nWrittenSize;
        }
    } while(0);

    return nWrittenSize;
}


bool arm_fff_mem_file_eof(const arm_file_node_t *ptNode)
{
    arm_fff_mem_file_cb_t *ptThis = ((arm_mem_file_node_t *)ptNode)->ptRuntime;
    
    do {
        if (NULL == ptThis) {
            break;
        }
        return (this.tAccess.nPosition >= this.nTotalSize);
    } while(0);
    
    return true;
}
