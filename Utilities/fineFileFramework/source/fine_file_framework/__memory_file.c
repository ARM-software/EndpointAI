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

#include "./fine_file_framework.h"
#include "./__memory_file.h"

#include <rt_sys.h>
#include <stdlib.h>
#include <string.h>

#include "file_io.h"



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
#   pragma clang diagnostic ignored "-Wimplicit-int-conversion"
#   pragma clang diagnostic ignored "-Wcast-qual"
#   pragma clang diagnostic ignored "-Wsign-conversion"
#elif defined(__IS_COMPILER_ARM_COMPILER_5__)
#   pragma diag_suppress 1296,174
#endif

/*============================ MACROS ========================================*/
#undef this
#define this        (*ptThis)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/

static __arm_fff_mem_block_t *__arm_fff_mem_file_request_new_block(
                                                arm_fff_mem_file_cb_t *ptThis)
{
    FFF_ASSERT(NULL != ptThis);
    //! allocate a new memory block for write
    __arm_fff_mem_block_t *ptBlock 
        = arm_fff_malloc(sizeof(__arm_fff_mem_block_t) + this.hwPageSize);
    do {
        if (NULL == ptBlock) {
            break;
        }
        memset( ptBlock, 
                0, 
                (sizeof(__arm_fff_mem_block_t) + this.hwPageSize));
        ptBlock->pchBuffer = (uint8_t *)(ptBlock + 1);
        ptBlock->u31BufferSize = this.hwPageSize;
    } while(0);
    return ptBlock;
}

static void __arm_fff_mem_file_free_block(  arm_fff_mem_file_cb_t *ptThis, 
                                            __arm_fff_mem_block_t *ptBlock)
{
    //! allocate a new memory block for write
    FFF_ASSERT(NULL != ptThis);

    do {
        if (NULL == ptBlock) {
            break;
        }
        if (NULL != ptBlock->pwExternalSize || ptBlock->bIsExternal) {
            /* external block */
            break;
        }

        arm_fff_free(ptBlock);
    } while(0);
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

        //! read
        if (    (OPEN_R == (chAttribute & OPEN_R))
            ||  (OPEN_PLUS == (chAttribute & OPEN_PLUS))) {

            if (    (NULL == this.ptBlockList)
                &&  (0 == (chAttribute & (OPEN_PLUS | OPEN_W | OPEN_A)))) {
                /* only read */
                tResult = ARM_FFF_ERR_NO_BUFFER;
                break;
            }

            if (NULL != this.ptBlockList) {
                if (    (NULL == this.ptBlockList->pchBuffer)
                    ||  (   (!this.ptBlockList->bIsExternal)
                        &&  (0 == this.ptBlockList->u31BufferSize))){
                    tResult = ARM_FFF_ERR_NO_BUFFER;
                    break;
                }
                
                //! update nSize
                if (NULL != this.ptBlockList->pwExternalSize) {
                    this.ptBlockList->u31BufferSize = *this.ptBlockList->pwExternalSize;
                    this.ptBlockList->nContentSize = this.ptBlockList->u31BufferSize;
                }

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
            } else {
                this.nTotalSize = 0;
            }
            
            /* move to the start of the file */
            this.tAccess.ptCurrent = this.ptBlockList;
            this.tAccess.nInblockOffset = 0;
            this.tAccess.nPosition = 0;
        }


        //! truncate to zero length
        if (OPEN_W == (chAttribute & OPEN_W)) {
            if (NULL != this.ptBlockList) {
                /* free existing mem blocks */
                __arm_fff_mem_block_t *ptBlock = this.ptBlockList;
                this.ptBlockList = NULL;
                do {
                    __arm_fff_mem_block_t *ptFreeBlock = ptBlock;
                    ptBlock = ptBlock->ptNext;

                    /* clean pointers before release */
                    ptFreeBlock->ptNext = NULL;
                    ptFreeBlock->ptPrevious = NULL;

                    __arm_fff_mem_file_free_block(ptThis, ptFreeBlock);
                } while(ptBlock != NULL);
            }
            this.tAccess.ptCurrent = NULL;
            this.tAccess.nInblockOffset = 0;
            this.tAccess.nPosition = 0;
            this.nTotalSize = 0;
        }

        /* prepare for write */
        if (    (OPEN_W == (chAttribute & OPEN_W))
            ||  (OPEN_A == (chAttribute & OPEN_A))
            ||  (OPEN_PLUS == (chAttribute & OPEN_PLUS))) {

            if (NULL == this.ptBlockList) {
                this.ptBlockList = __arm_fff_mem_file_request_new_block(ptThis);
                //! allocate a new memory block for write
                if (NULL == this.ptBlockList) {
                    tResult = ARM_FFF_ERR_NOT_ENOUGH_RESOURCE;
                    break;
                }
            }
            this.tAccess.ptCurrent = this.ptBlockList;
        }

        /* append */
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

        if (NULL == this.tAccess.ptCurrent) {
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

        /* for file opened with write mode, there should be at least one memory 
         * block 
         */
        FFF_ASSERT(NULL != this.tAccess.ptCurrent);


        int_fast32_t nWritableWithinBlock = 
                this.tAccess.ptCurrent->u31BufferSize 
            -   this.tAccess.nInblockOffset;
        
        while (nWritableWithinBlock <= 0) {
            FFF_ASSERT(0 == nWritableWithinBlock);

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
            nWritableWithinBlock = this.tAccess.ptCurrent->u31BufferSize;
        }

        nWrittenSize = MIN(nWritableWithinBlock, nSize);
        
        if (nWrittenSize > 0) {
            //! copy content
            memcpy( this.tAccess.ptCurrent->pchBuffer + this.tAccess.nInblockOffset, 
                    pBuffer, 
                    nWrittenSize);
            this.tAccess.nInblockOffset += nWrittenSize;
            this.tAccess.nPosition += nWrittenSize;

            /* update size info*/
            this.tAccess.ptCurrent->nContentSize = MAX( this.tAccess.nInblockOffset, this.tAccess.ptCurrent->nContentSize);
            this.nTotalSize = MAX(this.nTotalSize, this.tAccess.nPosition);
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

#undef arm_fff_abs
#define arm_fff_abs(__N)            ((__N) < 0 ? -(__N) : (__N))

int_fast32_t arm_fff_mem_file_seek( const arm_file_node_t *ptNode, 
                                    int_fast32_t nOffset,
                                    int_fast32_t nWhence)
{
    int nResult = -1;
    arm_fff_mem_file_cb_t *ptThis = ((arm_mem_file_node_t *)ptNode)->ptRuntime;
    
    /* read original postion */
    __arm_fff_mem_file_pos_t tAccess = this.tAccess;
    
    do {
        if (NULL == ptThis) {
            break;
        }
        
        switch (nWhence) {
            case SEEK_CUR:
                if (arm_fff_abs(nOffset) > this.nTotalSize) {
                    goto __seek_exit;
                }

                /* calculate new offset and fall-through to SEEK_END: */
                nOffset = (tAccess.nPosition + nOffset) - this.nTotalSize;

                /* fall-through */
            case SEEK_END:
                if (nOffset > 0) {
                    goto __seek_exit;
                }

                /* empty file, for example "rw", "r+", "w+" etc. */
                if (NULL == tAccess.ptCurrent) {
                    tAccess.nPosition = 0;
                    tAccess.nInblockOffset = 0;
                    break;
                }
                
                /* calculate new offset and fall-through to SEEK_SET */
                nOffset = this.nTotalSize + nOffset;
                /* fall-through */
            case SEEK_SET:
                if (nOffset < 0) {
                    goto __seek_exit;
                }
                /* move to the begining */
                tAccess.ptCurrent = this.ptBlockList;
                tAccess.nPosition = 0;
                tAccess.nInblockOffset = 0;

                while (nOffset > 0) {
                    if (NULL == tAccess.ptCurrent) {
                        goto __seek_exit;
                    }
                    
                    int32_t nBlockSize = tAccess.ptCurrent->nContentSize;
                    if (nBlockSize >= nOffset) {
                        /* finish moving */
                        tAccess.nPosition += nOffset;
                        tAccess.nInblockOffset = nOffset;
                        break;
                    }

                    /* move to next block */
                    nOffset -= nBlockSize;
                    tAccess.nPosition += nBlockSize;
                    tAccess.nInblockOffset = 0;
                    tAccess.ptCurrent = tAccess.ptCurrent->ptNext;
                }
                break;
            default:
                goto __seek_exit;
        }
        
        /* save new position */
        this.tAccess = tAccess;
        
        return tAccess.nPosition;
    } while(0);

__seek_exit:

    return nResult;
}

int_fast32_t arm_fff_mem_file_tell(const arm_file_node_t *ptNode)
{
    int_fast32_t nResult = -1;
    arm_fff_mem_file_cb_t *ptThis = ((arm_mem_file_node_t *)ptNode)->ptRuntime;
    
    do {
        if (NULL == ptThis) {
            break;
        }
        
        nResult = this.tAccess.nPosition;
    } while(0);
    
    return nResult;
}

#ifdef   __cplusplus
}
#endif
