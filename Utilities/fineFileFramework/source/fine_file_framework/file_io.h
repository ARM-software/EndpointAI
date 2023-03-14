/*********s*******************************************************************
*  Copyright 2017 Gorgon Meducer (Email:embedded_zhuoran@hotmail.com)       *
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

#ifndef __USE_FILE_IO_H__
#define __USE_FILE_IO_H__

/*============================ INCLUDES ======================================*/
#include "./fine_file_framework.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/ 
//! \brief io stream serice interface
//! @{
typedef struct  
{   
    const 
    arm_file_node_t*(*Open)             (   const char *pchName, uint16_t wFeature);
    bool            (*Close)            (   arm_file_node_t *ptFile);
    
    int_fast32_t    (*ReadByte)         (   arm_file_node_t *ptObj);
    bool            (*WriteByte)        (   arm_file_node_t *ptObj, 
                                            uint_fast8_t chByte);   
    int_fast32_t    (*Read)             (   arm_file_node_t *ptObj, 
                                            uint8_t *pchSrc, 
                                            int_fast32_t nSize);
    int_fast32_t    (*Write)            (   arm_file_node_t *ptObj, 
                                            uint8_t *pchSrc, 
                                            int_fast32_t nSize);
    bool            (*Flush)            (   arm_file_node_t *ptObj);
    bool            (*EndOfStream)      (   arm_file_node_t *ptObj);
} i_file_io_t;
//! @}
    

/*============================ GLOBAL VARIABLES ==============================*/
extern const i_file_io_t   FFF_IO;


/*============================ PROTOTYPES ====================================*/

#endif
