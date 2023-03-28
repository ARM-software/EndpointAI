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

#include <stdint.h>
#include <stdbool.h>
#include <rt_sys.h>
#include "file_io.h"
#include <string.h>
#include <stdlib.h>

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
#   pragma clang diagnostic ignored "-Wundef"
#   pragma clang diagnostic ignored "-Wextra-semi"
#   pragma clang diagnostic ignored "-Wmissing-variable-declarations"
#   pragma clang diagnostic ignored "-Wmissing-declarations"
#   pragma clang diagnostic ignored "-Wmicrosoft-anon-tag"
#elif defined(__IS_COMPILER_ARM_COMPILER_5__)
#   pragma diag_suppress 1296,174,381
#endif

/*============================ MACROS ========================================*/
#undef this
#define this        (*ptThis)


#if !defined(__FFF_CFG_SEMIHOSTING_CMD_LINE_BUFFER_SIZE__)
#   define __FFF_CFG_SEMIHOSTING_CMD_LINE_BUFFER_SIZE__         80
#endif

#if !__FFF_CFG_IGNORE_NO_SEMIHOSTING__
#undef __FFF_CFG_GET_COMMAND_LINE_VIA_SEMIHOSTING__
#define __FFF_CFG_GET_COMMAND_LINE_VIA_SEMIHOSTING__            0
#endif
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/

//declare_class(__arm_fff_t)

//def_class(__arm_fff_t,

//    private_member(
//        implement_ex(arm_fff_cfg_t, tConfig);
//        const arm_file_node_t *ptCurrent;
//    )
//)
//end_def_class(__arm_fff_t)

typedef struct __arm_fff_t  {
    implement_ex(arm_fff_cfg_t, tConfig);
    const arm_file_node_t *ptCurrent;
}__arm_fff_t;

/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
extern char *_sys_command_string(char *cmd, int len);
extern void _sys_exit(int ch);


/*============================ GLOBAL VARIABLES ==============================*/
static __arm_fff_t s_tLocalFFF = {0};
FILE __stdout, __stdin, __stderr;

const i_file_io_t FFF_IO = {
    .Open           = &arm_fff_open,
    .Close          = &arm_fff_close,
    .ReadByte       = &arm_fff_read_byte,
    .WriteByte      = &arm_fff_write_byte,
    .Read           = &arm_fff_helper_read_file,
    .Write          = &arm_fff_helper_write_file,
    .Flush          = &arm_fff_flush,
    .EndOfStream    = &arm_fff_eof,
    .Seek           = &arm_fff_seek,
    .Tell           = &arm_fff_tell,
};
/*============================ IMPLEMENTATION ================================*/

struct {
    arm_file_node_t *ptOUT;
    arm_file_node_t *ptIN;
    arm_file_node_t *ptERR;
}s_tSTDIO = {0};


/*----------------------------------------------------------------------------*
 * Compiler Specific Code to run __file_io_stdio_library_init() before main() *
 *----------------------------------------------------------------------------*/
typedef union {
    uint64_t dwValue;
    struct {
        int32_t argc;
        char** argv;
    };
}str_arg_t;


static 
size_t __arm_get_token(const char **ppchPath, const char *pchSeparator)
{   
    size_t wLength = 0;
    do {
        if (NULL == ppchPath || NULL == pchSeparator) {
            break;
        } else if (NULL == *ppchPath) {
            break;
        } 
        
        wLength = strlen(*ppchPath);
        if (    0 == strlen(pchSeparator) 
            ||  0 == wLength) {
            break;
        }
        /*! \note find the number of tokens */
        do {
            const char *pchLocate = strpbrk(*ppchPath, pchSeparator);
            if (NULL == pchLocate) {
                //! cannot find any separator
                break;
            }
            size_t wSize = (uint32_t)(pchLocate - *ppchPath);
            if (wSize > 0) {
                //! get one token
                wLength = wSize;
                break;
            } else {
                (*ppchPath)++;
                wLength --;
            }
        
        } while(wLength);
    } while(0);
    
    return wLength;
}

#if __FFF_CFG_GET_COMMAND_LINE_VIA_SEMIHOSTING__
static char *__semihost_command_string(void)
{
    static char s_cBuffer[__FFF_CFG_SEMIHOSTING_CMD_LINE_BUFFER_SIZE__ + 1];

    struct {
        char *pchCMDLine;
        size_t tSize;
    } tCMDLine = {
        .pchCMDLine = s_cBuffer,
        .tSize = sizeof(s_cBuffer)
    };
    
    if (0 == __semihost(0x15, &tCMDLine) && (tCMDLine.tSize > 0)) {
        return tCMDLine.pchCMDLine;
    }
    return NULL;
}
#endif

static str_arg_t get_arg(void)
{
#if __FFF_CFG_GET_COMMAND_LINE_VIA_SEMIHOSTING__
    const char *pchCommandLine = __semihost_command_string();
    if (NULL == pchCommandLine) {
        pchCommandLine = _sys_command_string(NULL, 0);
    }
#else
    const char *pchCommandLine = _sys_command_string(NULL, 0);
#endif


    const char *pchSrc = pchCommandLine;
    str_arg_t tResult = {0};
    do {
        int32_t nTokenCount = 0;
        
        if (NULL == pchSrc) {
            break;
        }
        
        //! calculate the total number of tokens
        do {
            size_t wTokenSize = __arm_get_token(&pchSrc, " \t\r\n");
            if (0 == wTokenSize) {
                break;
            }
            
            pchSrc += wTokenSize;
            nTokenCount++;
        } while(true);
        tResult.argc = nTokenCount;
        
        char **argv = arm_fff_malloc(sizeof(char *) * (size_t)nTokenCount);
        char **arvTemp = argv;
        pchSrc = pchCommandLine;
        
        //! calculate the total number of tokens
        do {
            size_t wTokenSize = __arm_get_token(&pchSrc, " \t\r\n");
            if (0 == wTokenSize) {
                break;
            }
            
            char *pchOption = arm_fff_malloc(wTokenSize+1);
            FFF_ASSERT(NULL != pchOption);
            memcpy(pchOption, pchSrc, wTokenSize);
            pchOption[wTokenSize] = 0;
            
            *arvTemp++ =  pchOption;
            pchSrc += wTokenSize;

        } while(true);
        tResult.argv = argv;


        return tResult;
    } while(0);
    
    return (str_arg_t){0};
}

static bool __arm_fff_alias_check(  const char *const* ppchPathString, 
                                    uint_fast8_t chAliasCount,
                                    const char *pchToken,
                                    size_t *pnTokenLength)
{
    size_t nTokenLength = *pnTokenLength;
    if (NULL == ppchPathString || 0 == chAliasCount) {
        return false;
    }
    
    do {
        if (NULL == *ppchPathString) {
            ppchPathString++;
            continue;
        }
        size_t wLength = strlen(*ppchPathString);
        wLength = MAX(wLength, nTokenLength);
        if (0 == strncasecmp(*ppchPathString++, pchToken, wLength)) {
            *pnTokenLength = wLength;
            return true;
        }
    } while(--chAliasCount);

    return false;
}

static const arm_file_node_t *__arm_fff_find_node(  
                                            __arm_fff_t *ptThis, 
                                            const char *pchPath,
                                            const arm_file_node_t *ptStartPoint)
{
    const arm_file_node_t * ptTargetNode = NULL, *ptNode = ptStartPoint;

    do {
        if (NULL == pchPath || NULL == ptThis) {
            break;
        } else if (NULL == ptStartPoint) {
            break;
        }
        
        //! left trim
        do {
            size_t wSize = strspn(pchPath, " ");
            pchPath += wSize;
        } while(0);
        
        //! if the first token is "\"
        do {
            size_t wSize = strspn(pchPath, "\\/");
            pchPath += wSize;
            if (1 == wSize) {
                //! the first token is '\' or "/", go to root
                while(NULL != ptNode->ptParent) {
                    if (ptNode == ptNode->ptParent) {
                        //! reached the top
                        break;
                    }
                    ptNode = ptNode->ptParent;
                }
                ptNode = ptNode->ptList;
            }
        } while(0);
        
        bool bFindAFile = false;
        //! calculate the total number of tokens
        do {
            size_t wTokenSize = __arm_get_token(&pchPath, "\\/");
            if (0 == wTokenSize) {
                //! no token any more
                break;
            }
            if (bFindAFile) {
                //! the folder doesn't exist
                ptTargetNode = NULL;
                break;
            }
            
            //! get one token
            do {
                if (0 == strncmp(pchPath, ".", wTokenSize)) {
                    //! current folder, no change
                    break;
                } else if (0 == strncmp(pchPath, "..", MAX(2,wTokenSize))) {
                    if (NULL != ptNode->ptParent && ptNode != ptNode->ptParent) {
                        ptNode = ptNode->ptParent;
                        if (NULL != ptNode->ptParent) {
                            ptNode = ptNode->ptParent->ptList;
                        } else {
                            ptNode = ptNode->ptList;
                        }
                    }
                    break;
                } 
                
                //! search for the node
                while( NULL != ptNode) {
                    if (__arm_fff_alias_check(  ptNode->ppchPathString, 
                                                ptNode->chAliasCount,
                                                pchPath, 
                                                &wTokenSize)) {
                        ptTargetNode = ptNode;
                        //! find the node
                        bFindAFile = false;
                        if (NULL == ptNode->ptList) {
                            bFindAFile = true;
                        } else {
                            ptNode = ptNode->ptList;
                        }
                        break;
                    }
                    
                    //! the final node
                    if (NULL == ptNode->ptNext || ptNode == ptNode->ptNext) {
                        //! could not find the file
                        ptNode = NULL;
                        ptTargetNode = NULL;
                    } else {
                        //! check next file
                        ptNode = ptNode->ptNext;
                    }
                }
            
            } while(0);
            
            //! prepare for next token
            pchPath += wTokenSize;
        } while(NULL != ptNode);
    
    } while(0);

    return ptTargetNode;
}

static const arm_file_node_t * __arm_fff_find_path( __arm_fff_t *ptThis, 
                                                    const char *pchPath)
{
    const arm_file_node_t * ptNode = NULL;
    
    do {
        if (NULL == ptThis || NULL == pchPath) {
            break;
        } 
    
        if (NULL != this.ptCurrent) {
            //! search current position
            ptNode = __arm_fff_find_node(ptThis, pchPath, this.ptCurrent->ptList);
            if (NULL != ptNode) {
                //! find the target node
                break;
            }
        }
        
        if (NULL != this.tConfig.ptRoot) {
            //! search root 
            ptNode = __arm_fff_find_node(ptThis, pchPath, this.tConfig.ptRoot->ptList);
            if (NULL != ptNode) {
                //! find the target node
                break;
            }
        }
        
        return NULL;
    } while(0);
    
    return ptNode;
}


extern int main (int argc, const char* argv[]);

static void run_main(void)
{
    str_arg_t tArgument = get_arg();
    _sys_exit(main(tArgument.argc, (const char **)tArgument.argv));
}


static 
arm_fff_err_t __arm_fff_init(   __arm_fff_t *ptThis, 
                                const arm_fff_cfg_t *ptCFG)
{
    
    if (NULL == ptThis || NULL == ptCFG) {
        return ARM_FFF_ERR_INVALID_PTR;
    } 
    memset(ptThis, 0, sizeof(__arm_fff_t));
    
    this.tConfig = (*ptCFG);
    
    this.ptCurrent = __arm_fff_find_path(ptThis, this.tConfig.pchWorkingPath);
    
    if (NULL == this.ptCurrent) {
        this.ptCurrent = this.tConfig.ptRoot->ptList;
    }
    
    run_main();

    return ARM_FFF_ERR_NONE;
}

static 
arm_fff_err_t __arm_fff_set_working_path(   __arm_fff_t *ptThis,
                                            const char *pchWorkingPath)
{
    
    if (NULL == ptThis || NULL == pchWorkingPath) {
        return ARM_FFF_ERR_INVALID_PTR;
    } 
    
    const arm_file_node_t *ptNode = __arm_fff_find_path(ptThis,pchWorkingPath);
    if (NULL == ptNode) {
        return ARM_FFF_ERR_INVALID_PATH;
    }
    this.ptCurrent = ptNode;
    
    return ARM_FFF_ERR_NONE;
}

static 
const arm_file_node_t * __arm_fff_get_working_path(__arm_fff_t *ptThis)
{
    const arm_file_node_t *ptNode = NULL;
    
    do {
        if (NULL == ptThis) {
            break;
        } 
        ptNode = this.ptCurrent;
        if (NULL == ptNode) {
            break;
        }
        if (NULL == ptNode->ptList) {
            if (NULL != ptNode->ptParent) {
                ptNode = ptNode->ptParent;
            }
        }
        
    } while(0);
    return ptNode;
}

static 
const arm_file_node_t *__arm_fff_open(  __arm_fff_t *ptThis, 
                                        const char *pchPath, 
                                        uint16_t wFeature)
{
    const arm_file_node_t * ptNode = __arm_fff_find_path(ptThis,pchPath);
    
    do {
        if (NULL == ptNode) {
            //! cannot find the target node
            break;
        } else if (0 == ptNode->chID) {         
            //! you are not allowed to open a default folder
            break;
        } else if (ptNode->chID > this.tConfig.tTypes.chCount + 1) {
            //! illegal ID
            break;
        }
        
        if (    (   (OPEN_R == (wFeature & OPEN_R))
                ||  (OPEN_PLUS == (wFeature & OPEN_PLUS)))
            &&  !(ptNode->bCanRead)) {
            //! doesn't support read
            break;
        }
        
        if (    (   (OPEN_W == (wFeature & OPEN_W))
                ||  (OPEN_A == (wFeature & OPEN_A))
                ||  (OPEN_PLUS == (wFeature & OPEN_PLUS)))
            &&  !(ptNode->bCanWrite)) {
            //! doesn't support write
            break;
        }
        
        if (ptNode->chID >= 2) {
            //! user defined type
            const i_arm_file_node_t *ptType = 
                &this.tConfig.tTypes.ptList[ptNode->chID - 2];
            
            //! call user defined open method
            if (NULL != ptType->Control.Open){
                if (ARM_FFF_ERR_NONE != 
                        (*(ptType->Control.Open))(ptNode, wFeature)) {
                    break;
                }
            }
        } else  if (ARM_FFF_ERR_NONE != 
                        arm_fff_mem_file_open(ptNode, wFeature)) {
            break;
        }
        
        return ptNode;
    } while(0);
    
    return NULL;
}

static 
arm_fff_err_t __arm_fff_close(  __arm_fff_t *ptThis, const arm_file_node_t * ptNode)
{
    arm_fff_err_t tErr = ARM_FFF_ERR_NONE;
    
    do {
        if (NULL == ptNode) {
            //! cannot find the target node
            tErr = ARM_FFF_ERR_INVALID_PTR;
            break;
        } else if (0 == ptNode->chID) {         
            //! you are not allowed to open a default folder
            tErr = ARM_FFF_ERR_UNSUPPORTED;
            break;
        } else if (ptNode->chID > this.tConfig.tTypes.chCount + 1) {
            //! illegal ID
            tErr = ARM_FFF_ERR_UNSUPPORTED;
            break;
        }
        
        if (ptNode->chID >= 2) {
            //! user defined type
            const i_arm_file_node_t *ptType = 
                &this.tConfig.tTypes.ptList[ptNode->chID - 2];
            
            //! call user defined close method
            if (NULL != ptType->Control.Close){
                (*(ptType->Control.Close))(ptNode);
            }
        }

    } while(0);
    
    return tErr;
}


static 
bool __arm_fff_eof(  __arm_fff_t *ptThis, const arm_file_node_t * ptNode)
{
    bool bResult = true;
    
    do {
        if (NULL == ptNode) {
            //! cannot find the target node
            break;
        } else if (0 == ptNode->chID) {         
            //! you are not allowed to open a default folder
            break;
        } else if (ptNode->chID > this.tConfig.tTypes.chCount + 1) {
            //! illegal ID
            break;
        }
        
        if (ptNode->chID >= 2) {
            //! user defined type
            const i_arm_file_node_t *ptType = 
                &this.tConfig.tTypes.ptList[ptNode->chID - 2];
            
            //! call user defined eof method
            if (NULL != ptType->Control.EndOfStream){
                bResult = (*(ptType->Control.EndOfStream))(ptNode);
            }
        } else {
            /*! memory file */
            bResult = arm_fff_mem_file_eof(ptNode);
        }

    } while(0);
    
    return bResult;
}

static 
bool __arm_fff_flush(  __arm_fff_t *ptThis, const arm_file_node_t * ptNode)
{
    bool bResult = true;
    
    do {
        if (NULL == ptNode) {
            //! cannot find the target node
            break;
        } else if (0 == ptNode->chID) {         
            //! you are not allowed to open a default folder
            break;
        } else if (ptNode->chID > this.tConfig.tTypes.chCount + 1) {
            //! illegal ID
            break;
        }
        
        if (ptNode->chID >= 2) {
            //! user defined type
            const i_arm_file_node_t *ptType = 
                &this.tConfig.tTypes.ptList[ptNode->chID - 2];
            
            //! call user defined flush method
            if (NULL != ptType->Control.Flush){
                bResult = (*(ptType->Control.Flush))(ptNode);
            }
        } 

    } while(0);
    
    return bResult;
}


static 
int __arm_fff_seek( __arm_fff_t *ptThis, 
                    const arm_file_node_t * ptNode,
                    int32_t nOffset,
                    int32_t nWhence)
{
    int nResult = -1;
    
    do {
        if (NULL == ptNode) {
            //! cannot find the target node
            break;
        } else if (0 == ptNode->chID) {         
            //! you are not allowed to open a default folder
            break;
        } else if (ptNode->chID > this.tConfig.tTypes.chCount + 1) {
            //! illegal ID
            break;
        }
        
        if (ptNode->chID >= 2) {
            //! user defined type
            const i_arm_file_node_t *ptType = 
                &this.tConfig.tTypes.ptList[ptNode->chID - 2];
            
            //! call user defined flush method
            if (NULL != ptType->Position.Set){
                nResult = (*(ptType->Position.Set))(ptNode, nOffset, nWhence);
            }
        } else {
            nResult = arm_fff_mem_file_seek(ptNode, nOffset, nWhence);
        }

    } while(0);

    return nResult;
}

static
long int __arm_fff_tell(__arm_fff_t *ptThis, arm_file_node_t * ptNode)
{
    int nResult = -1;
    
    do {
        if (NULL == ptNode) {
            //! cannot find the target node
            break;
        } else if (0 == ptNode->chID) {         
            //! you are not allowed to open a default folder
            break;
        } else if (ptNode->chID > this.tConfig.tTypes.chCount + 1) {
            //! illegal ID
            break;
        }
        
        if (ptNode->chID >= 2) {
            //! user defined type
            const i_arm_file_node_t *ptType = 
                &this.tConfig.tTypes.ptList[ptNode->chID - 2];
            
            //! call user defined flush method
            if (NULL != ptType->Position.Get){
                nResult = (*(ptType->Position.Get))(ptNode);
            }
        } else {
            nResult = arm_fff_mem_file_tell(ptNode);
        }

    } while(0);

    return nResult;
}



static 
int_fast32_t __arm_fff_read(__arm_fff_t *ptThis, 
                            const arm_file_node_t *ptNode,
                            void *pBuffer,
                            int_fast32_t nSize)
{
    int_fast32_t nResult = -1;
    
    do {
        if (NULL == ptNode) {
            //! cannot find the target node
            break;
        } else if (!ptNode->bCanRead) {
            //! cannot read
            break;
        } else if (0 == ptNode->chID) {         
            //! you are not allowed to open a default folder
            break;
        } else if (ptNode->chID > this.tConfig.tTypes.chCount + 1) {
            //! illegal ID
            break;
        } 
        
        if (ptNode->chID >= 2) {
            //! user defined type
            const i_arm_file_node_t *ptType = 
                &this.tConfig.tTypes.ptList[ptNode->chID - 2];
            
            //! call user defined read method
            if (NULL != ptType->Read){
                nResult = (*(ptType->Read))(ptNode, pBuffer, nSize);
            }
        } else {
            /*! memory file */
            nResult = arm_fff_mem_file_read(ptNode, pBuffer, nSize);
        }

    } while(0);
    
    return nResult;
}

static 
int_fast32_t __arm_fff_write(__arm_fff_t *ptThis, 
                            const arm_file_node_t *ptNode,
                            void *pBuffer,
                            int_fast32_t nSize)
{
    int_fast32_t nResult = -1;
    
    do {
        if (NULL == ptNode) {
            //! cannot find the target node
            break;
        } else if (!ptNode->bCanWrite) {
            //! cannot write
            break;
        } else if (0 == ptNode->chID) {         
            //! you are not allowed to open a default folder
            break;
        } else if (ptNode->chID > this.tConfig.tTypes.chCount + 1) {
            //! illegal ID
            break;
        } 
        
        if (ptNode->chID >= 2) {
            //! user defined type
            const i_arm_file_node_t *ptType = 
                &this.tConfig.tTypes.ptList[ptNode->chID - 2];
            
            //! call user defined write method
            if (NULL != ptType->Write){
                nResult = (*(ptType->Write))(ptNode, pBuffer, nSize);
            }
        } else {
            /*! memory file */
            nResult = arm_fff_mem_file_write(ptNode, pBuffer, nSize);
        }

    } while(0);
    
    return nResult;
}

/*----------------------------------------------------------------------------*
 * Utility Functions                                                          *
 *----------------------------------------------------------------------------*/
char * arm_fff_get_path_string( const arm_file_node_t *ptPathNode,
                                char *pchBuffer, size_t wBufferSize)
{
    const arm_file_node_t *ptNode = ptPathNode;
    char *pchReturn = pchBuffer;
    if (NULL == ptPathNode || NULL == pchBuffer || wBufferSize < 4) {
        return NULL;
    }
    
    //! calculate the total path string length
    size_t wPathSize = 0, wStringSize = 0;
    while(NULL != ptNode) {
        wPathSize += strlen(*(ptNode->ppchPathString))+1;
        if (NULL == ptNode->ptParent || ptNode->ptParent == ptNode){
            break;
        }
        ptNode = ptNode->ptParent;
    }
    
    wStringSize = MIN(wPathSize, wBufferSize - 1);
    memset(pchBuffer, '\\', wStringSize);
    pchBuffer[wStringSize] = '\0';            //! end of the string
    
    ptNode = ptPathNode;
    
    if ( (wBufferSize - 1) < wPathSize) {
        memcpy(pchBuffer, "...", 3);
        pchBuffer += 3;
        wStringSize = (wBufferSize - 1) - 3;
    }
    
    while(NULL != ptNode && wStringSize) {
        
        size_t wLength = strlen(*(ptNode->ppchPathString));
        if (wStringSize < (wLength + 1)) {
            memcpy(pchBuffer, *(ptNode->ppchPathString) + wLength - wStringSize, wStringSize);
            break;
        } else {
            wStringSize -= wLength + 1;
            memcpy(&pchBuffer[wStringSize], *(ptNode->ppchPathString), wLength);
            
            if (NULL == ptNode->ptParent || ptNode->ptParent == ptNode){
                break;
            }
            ptNode = ptNode->ptParent;
        }
    }

    return pchReturn;
}

__attribute__((weak))
void *arm_fff_malloc(size_t tSize)
{
    return malloc(tSize);
}

__attribute__((weak))
void arm_fff_free(void *pAddress)
{
    if (NULL != pAddress) {
        free(pAddress);
    }
}

size_t arm_fff_helper_read_file(arm_file_node_t *ptInputFile, 
                                void *pBuffer, 
                                size_t tSize)
{
    assert(NULL != ptInputFile);
    
    uint8_t *pchBuffer = (uint8_t *)pBuffer;
    size_t tSizeLeft = tSize;
    while(!arm_fff_eof(ptInputFile)) {
        size_t nSize = arm_fff_read(ptInputFile, pchBuffer, tSizeLeft);
        if (nSize == 0) {
            break;
        }
        pchBuffer += nSize;
        tSizeLeft -= nSize;
        if (0 == tSizeLeft) {
            break;
        }
    } 
    
    return tSize - tSizeLeft;
}

size_t arm_fff_helper_write_file(   arm_file_node_t *ptOutputFile, 
                                    void *pBuffer, 
                                    size_t tSize)
{
    assert(NULL != ptOutputFile);
    
    uint8_t *pchBuffer = (uint8_t *)pBuffer;
    size_t tSizeLeft = tSize;
    while(true) {
        size_t nSize = arm_fff_write(ptOutputFile, pchBuffer, tSizeLeft);
        if (nSize == 0) {
            break;
        } 
        pchBuffer += nSize;
        tSizeLeft -= nSize;
        if (0 == tSizeLeft) {
            break;
        }
    } 
    
    return tSize - tSizeLeft;
}

/*----------------------------------------------------------------------------*
 * Wrapper Functions                                                          *
 *----------------------------------------------------------------------------*/


arm_fff_err_t arm_fff_init( const arm_fff_cfg_t *ptCFG)
{
    return __arm_fff_init(&s_tLocalFFF, ptCFG);
}

arm_fff_err_t arm_fff_set_working_path(const char *pchWorkingPath)
{
    return __arm_fff_set_working_path(&s_tLocalFFF, pchWorkingPath);
}

const arm_file_node_t * arm_fff_get_working_path(void)
{
    return __arm_fff_get_working_path(&s_tLocalFFF);
}

const arm_file_node_t * arm_fff_find_path(const char *pchPath)
{
    return __arm_fff_find_path(&s_tLocalFFF, pchPath);
}

const arm_file_node_t *arm_fff_open( const char *pchPath, uint16_t wFeature)
{
    return __arm_fff_open(&s_tLocalFFF, pchPath, wFeature);
}

bool arm_fff_close(arm_file_node_t * ptNode)
{
    return ARM_FFF_ERR_NONE == __arm_fff_close(&s_tLocalFFF, ptNode);
}
 
bool arm_fff_eof( arm_file_node_t * ptNode)
{
    return __arm_fff_eof(&s_tLocalFFF, ptNode);
}

bool arm_fff_flush( arm_file_node_t * ptNode)
{
    return __arm_fff_flush(&s_tLocalFFF, ptNode);
}

int arm_fff_seek(   arm_file_node_t * ptNode,
                    int32_t nOffset,
                    int32_t nWhence)
{
    return __arm_fff_seek(&s_tLocalFFF, ptNode, nOffset, nWhence);
}

long int arm_fff_tell(arm_file_node_t * ptNode)
{
    return __arm_fff_tell(&s_tLocalFFF, ptNode);
}

int_fast32_t arm_fff_read(  arm_file_node_t *ptNode,
                            uint8_t *pchBuffer,
                            int_fast32_t nSize)
{
    return __arm_fff_read(&s_tLocalFFF, ptNode, pchBuffer, nSize);
}

int_fast32_t arm_fff_write(  arm_file_node_t *ptNode,
                            uint8_t *pchBuffer,
                            int_fast32_t nSize)
{
    return __arm_fff_write(&s_tLocalFFF, ptNode, pchBuffer, nSize);
}

int_fast32_t arm_fff_read_byte(arm_file_node_t *ptNode)
{
    uint8_t chByte;
    if (1 != __arm_fff_read(&s_tLocalFFF, ptNode, &chByte, 1) ) {
        return -1;
    }
    return chByte;
}

bool arm_fff_write_byte(arm_file_node_t *ptNode, uint_fast8_t chByte)
{
    if (1 != __arm_fff_write(&s_tLocalFFF, ptNode, &chByte, 1) ) {
        return false;
    }
    return true;
}

#ifdef   __cplusplus
}
#endif




