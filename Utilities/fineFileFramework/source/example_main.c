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

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "fake_file_framework/fake_file_framework.h"
/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
/*============================ IMPLEMENTATION ================================*/

static void print_path(const arm_file_node_t *ptPathNode)
{
    static char s_chLineBuffer[256];
    
    printf("%s\r\n",
        arm_fff_get_path_string(ptPathNode, 
                                s_chLineBuffer, 
                                sizeof(s_chLineBuffer))
    );
}

#if 0
static 
void __arm_fff_print_folder_structure(const arm_file_node_t *ptPathNode,
                                      uint_fast16_t hwLevel)
{
    
}

void arm_fff_print_folder_structure(const arm_file_node_t *ptPathNode)
{
    
}
#endif

static void file_path_demo(void)
{
    printf("\r\n");
    printf("--------------------------------------------------\r\n");
    printf("   Demo for Absolute Path and Relative Path       \r\n");
    printf("--------------------------------------------------\r\n\r\n");
    
    printf("Try to open file with absolute path: [d:\\symphony\\music\\beethoven\\no_2.mp3] ...");
    FILE *ptInput = fopen("d:\\symphony\\music\\beethoven\\no_2.mp3", "rb");
    if (NULL == ptInput) {
        printf("Failed\r\n");
    } else {
        printf("OK\r\n");
    }
    fclose(ptInput);

    printf( "Current working path: ");
    print_path(arm_fff_get_working_path());
    
    printf("Try to open file with relative path : [no_2.mp3] ...");
    ptInput = fopen("no_2.mp3", "rb");
    if (NULL == ptInput) {
        printf("Failed\r\n");
    } else {
        printf("OK\r\n");
    }
    fclose(ptInput);
    
    
    printf( "\r\nset working path to d:\\symphony\\music\\beethoven...");
    if (ARM_FFF_ERR_NONE == arm_fff_set_working_path("d:\\symphony\\music\\beethoven")) {
        printf("OK\r\n");
    } else {
        printf("Failed\r\n");
    }
    
    printf( "Current working path: ");
    print_path(arm_fff_get_working_path());
    
    printf("Try to open file with relative path : [no_2.mp3] ...");
    ptInput = fopen("no_2.mp3", "r");
    if (NULL == ptInput) {
        printf("Failed\r\n");
    } else {
        printf("OK\r\n");
    }
    fclose(ptInput);
}


static uint_fast16_t __read_file_to_buffer( FILE *ptInputFile, 
                                            void *pBuffer, 
                                            uint_fast16_t hwSize)
{
    assert(NULL != ptInputFile);
    
    uint8_t *pchBuffer = (uint8_t *)pBuffer;
    uint_fast16_t hwSizeLeft = hwSize;
    while(!feof(ptInputFile)) {
        int_fast32_t nSize = fread(pchBuffer, 1, hwSizeLeft, ptInputFile);
        if (nSize < 0) {
            break;
        } 
        pchBuffer += nSize;
        hwSizeLeft -= nSize;
        if (0 == hwSizeLeft) {
            break;
        }
    } 
    
    return hwSize - hwSizeLeft;
}

static uint_fast16_t __write_buffer_to_file(FILE *ptOutputFile, 
                                            void *pBuffer, 
                                            uint_fast16_t hwSize)
{
    assert(NULL != ptOutputFile);
    
    uint8_t *pchBuffer = (uint8_t *)pBuffer;
    uint_fast16_t hwSizeLeft = hwSize;
    while(true) {
        int_fast32_t nSize = fwrite(pchBuffer, 1, hwSizeLeft, ptOutputFile);
        if (nSize < 0) {
            break;
        } 
        pchBuffer += nSize;
        hwSizeLeft -= nSize;
        if (0 == hwSizeLeft) {
            break;
        }
    } 
    
    return hwSize - hwSizeLeft;
}

static void file_copy_demo(void)
{
    printf("\r\n");
    printf("--------------------------------------------------\r\n");
    printf("   Demo for File Copy                             \r\n");
    printf("--------------------------------------------------\r\n\r\n");
    
    printf("Try to open file with absolute path: [c:\\hotel.mp3] ...");
    FILE *ptInput = fopen("c:\\hotel.mp3", "rb");
    if (NULL == ptInput) {
        printf("Failed\r\n");
    } else {
        printf("OK\r\n");
    }
    
    printf("Try to open file with relative path: [no_1.mp3] ...");
    FILE *ptOutput = fopen("no_1.mp3", "wb");
    if (NULL == ptOutput) {
        printf("Failed\r\n");
    } else {
        printf("OK\r\n");
    }
    
    printf("Copying file...");
    int_fast32_t nTotalSize = 0;
    bool bError = false;
    while(!feof(ptInput)) {
        static uint8_t s_chBuffer[1024];
        
        int_fast32_t nSize = 
            __read_file_to_buffer(ptInput, s_chBuffer, sizeof(s_chBuffer));
        if (nSize < 0) {
            bError = true;
            break;
        }
        
        nSize = __write_buffer_to_file(ptOutput, s_chBuffer, nSize);
        if (nSize < 0) {
            bError = true;
            break;
        }
        
        nTotalSize += nSize;
    }
    
    if (bError) {
        printf("Failed");
    } else {
        printf("Complete");
    }
    printf("\r\nTotal Size: %d Bytes \r\n"
            "Close files.\r\n", nTotalSize );

    fclose(ptOutput);
    fclose(ptInput);
}

int main(int argc, char **argv)
{
    //! parsing input command line
    do {
        uint_fast8_t chArgc = argc;
        printf("\r\nEntering main...\r\n"
               "--------------------------------------------------\r\n");
        if (0 == argc) {
            printf("No argument is passed to main...\r\n");
        } else {
            printf("%d arguments are passed to main:\r\n"
                   "--------------------------------------------------\r\n", chArgc);
            do {
                printf("%d: %s \r\n", chArgc, (char *)argv[chArgc-1]);
            } while(--chArgc);
        }
    } while(0);
    
    file_path_demo();
    file_copy_demo();
    
    return 0;
}