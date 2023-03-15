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

#include <stdint.h>
#include <stdbool.h>
#include <rt_sys.h>
#include "file_io.h"

#define __PLOOC_CLASS_USE_STRICT_TEMPLATE__
#define __PLOOC_CLASS_IMPLEMENT
#include "./utilities/ooc.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/

declare_class(__arm_fff_t)

def_class(__arm_fff_t,

    private_member(
        implement_ex(arm_fff_cfg_t, tConfig);
        const arm_file_node_t *ptCurrent;
    )
)
end_def_class(__arm_fff_t)

/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
extern char *_sys_command_string(char *cmd, int len);
extern void _sys_exit(int ch);


/*============================ GLOBAL VARIABLES ==============================*/
NO_INIT static __arm_fff_t s_tLocalFFF;
FILE __stdout, __stdin, __stderr;

const i_file_io_t FFF_IO = {
    .Open           = &arm_fff_open,
    .Close          = &arm_fff_close,
    .ReadByte       = &arm_fff_read_byte,
    .WriteByte      = &arm_fff_write_byte,
    .Read           = &arm_fff_read,
    .Write          = &arm_fff_write,
    .Flush          = &arm_fff_flush,
    .EndOfStream    = &arm_fff_eof,
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
        uint32_t argc;
        char** argv;
    };
}str_arg_t;


static 
size_t __arm_get_token(const char **ppchPath, const char *pchSeparator)
{   
    size_t wLength;
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


static str_arg_t get_arg(void)
{
    const char *pchCommandLine = _sys_command_string(NULL, 0);
    const char *pchSrc = pchCommandLine;
    str_arg_t tResult = {0};
    do {
        uint32_t nTokenCount = 0;
        
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
        
        char **argv = malloc(sizeof(char *) * nTokenCount);
        char **arvTemp = argv;
        pchSrc = pchCommandLine;
        
        //! calculate the total number of tokens
        do {
            size_t wTokenSize = __arm_get_token(&pchSrc, " \t\r\n");
            if (0 == wTokenSize) {
                break;
            }
            
            char *pchOption = malloc(wTokenSize+1);
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
                                            __arm_fff_t *ptObj, 
                                            const char *pchPath,
                                            const arm_file_node_t *ptStartPoint)
{
    const arm_file_node_t * ptTargetNode = NULL, *ptNode = ptStartPoint;
    class_internal(ptObj, ptThis, __arm_fff_t);

    do {
        if (NULL == pchPath || NULL == ptObj) {
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

static const arm_file_node_t * __arm_fff_find_path( __arm_fff_t *ptObj, 
                                                    const char *pchPath)
{
    class_internal(ptObj, ptThis, __arm_fff_t);
    const arm_file_node_t * ptNode = NULL;
    
    do {
        if (NULL == ptObj || NULL == pchPath) {
            break;
        } 
    
        if (NULL != this.ptCurrent) {
            //! search current position
            ptNode = __arm_fff_find_node(ptObj, pchPath, this.ptCurrent->ptList);
            if (NULL != ptNode) {
                //! find the target node
                break;
            }
        }
        
        if (NULL != this.ptRoot) {
            //! search root 
            ptNode = __arm_fff_find_node(ptObj, pchPath, this.ptRoot->ptList);
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
arm_fff_err_t __arm_fff_init(__arm_fff_t *ptObj, 
                                    const arm_fff_cfg_t *ptCFG)
{
    class_internal(ptObj, ptThis, __arm_fff_t);
    
    if (NULL == ptObj || NULL == ptCFG) {
        return ARM_FFF_ERR_INVALID_PTR;
    } 
    memset(ptObj, 0, sizeof(__arm_fff_t));
    
    this.tConfig = (*ptCFG);
    
    this.ptCurrent = __arm_fff_find_path(ptObj, this.pchWorkingPath);
    
    if (NULL == this.ptCurrent) {
        this.ptCurrent = this.ptRoot->ptList;
    }
    
    run_main();

    return ARM_FFF_ERR_NONE;
}

static 
arm_fff_err_t __arm_fff_set_working_path(   __arm_fff_t *ptObj,
                                            const char *pchWorkingPath)
{
    class_internal(ptObj, ptThis, __arm_fff_t);
    
    if (NULL == ptObj || NULL == pchWorkingPath) {
        return ARM_FFF_ERR_INVALID_PTR;
    } 
    
    const arm_file_node_t *ptNode = __arm_fff_find_path(ptObj,pchWorkingPath);
    if (NULL == ptNode) {
        return ARM_FFF_ERR_INVALID_PATH;
    }
    this.ptCurrent = ptNode;
    
    return ARM_FFF_ERR_NONE;
}

static 
const arm_file_node_t * __arm_fff_get_working_path(__arm_fff_t *ptObj)
{
    const arm_file_node_t *ptNode = NULL;
    class_internal(ptObj, ptThis, __arm_fff_t);
    
    do {
        if (NULL == ptObj) {
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
const arm_file_node_t *__arm_fff_open(  __arm_fff_t *ptObj, 
                                        const char *pchPath, 
                                        uint16_t wFeature)
{
    const arm_file_node_t * ptNode = __arm_fff_find_path(ptObj,pchPath);
    class_internal(ptObj, ptThis, __arm_fff_t);
    
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
        
        if ((OPEN_R == (wFeature & OPEN_R)) && !(ptNode->bCanRead)) {
            //! doesn't support read
            break;
        }
        
        if ((OPEN_W == (wFeature & OPEN_W)) && !(ptNode->bCanWrite)) {
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
arm_fff_err_t __arm_fff_close(  __arm_fff_t *ptObj, const arm_file_node_t * ptNode)
{
    arm_fff_err_t tErr = ARM_FFF_ERR_NONE;
    class_internal(ptObj, ptThis, __arm_fff_t);
    
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
bool __arm_fff_eof(  __arm_fff_t *ptObj, const arm_file_node_t * ptNode)
{
    bool bResult = true;
    class_internal(ptObj, ptThis, __arm_fff_t);
    
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
bool __arm_fff_flush(  __arm_fff_t *ptObj, const arm_file_node_t * ptNode)
{
    bool bResult = true;
    class_internal(ptObj, ptThis, __arm_fff_t);
    
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
int_fast32_t __arm_fff_read(__arm_fff_t *ptObj, 
                            const arm_file_node_t *ptNode,
                            void *pBuffer,
                            int_fast32_t nSize)
{
    int_fast32_t nResult = -1;
    class_internal(ptObj, ptThis, __arm_fff_t);
    
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
int_fast32_t __arm_fff_write(__arm_fff_t *ptObj, 
                            const arm_file_node_t *ptNode,
                            void *pBuffer,
                            int_fast32_t nSize)
{
    int_fast32_t nResult = -1;
    class_internal(ptObj, ptThis, __arm_fff_t);
    
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




