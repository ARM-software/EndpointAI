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
#include <string.h>

#include "fine_file_framework.h"
#include <RTE_Components.h>
#if defined(RTE_Compiler_EventRecorder) && defined(RTE_Compiler_IO_STDOUT_EVR)
#   include <EventRecorder.h>
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

#elif defined(__IS_COMPILER_ARM_COMPILER_5__)
#   pragma diag_suppress 1296,174
#endif

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
/*============================ IMPLEMENTATION ================================*/

void SysTick_Handler(void);

void SysTick_Handler(void)
{
}

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
    FILE *ptInput = fopen("d:\\symphony\\music\\beethoven\\no_2.mp3", "r+b");
    if (NULL == ptInput) {
        printf("Failed\r\n");
    } else {
        printf("OK\r\n");
    }
    fclose(ptInput);

    printf( "Current working path: ");
    print_path(arm_fff_get_working_path());
    
    printf("Try to open file with relative path : [no_2.mp3] ...");
    ptInput = fopen("no_2.mp3", "r+b");
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
    ptInput = fopen("no_2.mp3", "r+b");
    if (NULL == ptInput) {
        printf("Failed\r\n");
    } else {
        printf("OK\r\n");
    }
    fclose(ptInput);
}


static void file_copy_demo(void)
{
    printf("\r\n");
    printf("--------------------------------------------------\r\n");
    printf("   Demo for File Copy                             \r\n");
    printf("--------------------------------------------------\r\n\r\n");
    
    printf("Try to open file with absolute path: [c:\\fffyml2c.py] ...");
    FILE *ptInput = fopen("c:\\fffyml2c.py", "rb");
    if (NULL == ptInput) {
        printf("Failed\r\n");
    } else {
        printf("OK\r\n");
    }
    
    printf("Try to open file with relative path: [no_1.mp3] ...");
    FILE *ptOutput = fopen("no_1.mp3", "r+b");
    if (NULL == ptOutput) {
        printf("Failed\r\n");
    } else {
        printf("OK\r\n");
    }
    
    printf("Copying file...");
    int_fast32_t nTotalSize = 0;
    bool bError = false;
    static union {
        uint8_t chBuffer[1024];
        struct {
            uint8_t chBuffer1[512];
            uint8_t chBuffer2[512];
        };
    }s_tBuffer;

    while(!feof(ptInput)) {
        size_t nSize = fread(s_tBuffer.chBuffer, 1, sizeof(s_tBuffer.chBuffer), ptInput);
        if (nSize == 0) {
            bError = true;
            break;
        }
        
        nSize = fwrite(s_tBuffer.chBuffer, 1, nSize, ptOutput);
        if (nSize == 0) {
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
    printf("\r\nTotal Size: %d Bytes \r\n" , nTotalSize );


    fseek(ptInput, -ftell(ptInput), SEEK_END);
    //fseek(ptOutput, 0, SEEK_SET);
    //rewind(ptInput);
    rewind(ptOutput);
    


    bError = false;

    printf("Verifying...");
    /* verify */
    while(!feof(ptInput)) {
        size_t nSize1 = fread(s_tBuffer.chBuffer1, 1, sizeof(s_tBuffer.chBuffer1), ptInput);
        if (nSize1 == 0) {
            bError = true;
            break;
        }
        
        size_t nSize2 = fread(s_tBuffer.chBuffer2, 1, nSize1, ptOutput);
        if (nSize2 == 0 || nSize1 != nSize2) {
            bError = true;
            break;
        }
        
        if (0 != memcmp(s_tBuffer.chBuffer1, s_tBuffer.chBuffer2, nSize1)) {
            bError = true;
        }
    }

    /* get file size */
    do {
        fpos_t tPos; 
        fgetpos(ptOutput, &tPos);
        
        fseek(ptOutput, 0, SEEK_END);
        nTotalSize = ftell(ptOutput);
        
        fsetpos(ptOutput, &tPos);
    } while(0);

    if (bError) {
        printf("Failed");
    } else {
        printf("Complete...%d bytes were verified", nTotalSize);
    }

    fclose(ptOutput);
    fclose(ptInput);
    
    printf("\r\nClose files.\r\n");
}

int main(int argc, char **argv)
{
#if defined(RTE_Compiler_EventRecorder) && defined(RTE_Compiler_IO_STDOUT_EVR)
    EventRecorderInitialize(0, 1);
#endif

    //! parsing input command line
    do {
        uint_fast8_t chArgc = (uint_fast8_t)argc;
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
    
    printf("sizeof(long): %d\r\n", sizeof(long));
    
    file_path_demo();
    file_copy_demo();
    file_copy_demo();
    return 0;
}


#ifdef   __cplusplus
{
#endif
