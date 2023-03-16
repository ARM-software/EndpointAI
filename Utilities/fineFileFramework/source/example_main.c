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
        size_t nSize = fread(pchBuffer, 1, hwSizeLeft, ptInputFile);
        if (nSize == 0) {
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
        size_t nSize = fwrite(pchBuffer, 1, hwSizeLeft, ptOutputFile);
        if (nSize == 0) {
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
        
        size_t nSize = 
            __read_file_to_buffer(ptInput, s_chBuffer, sizeof(s_chBuffer));
        if (nSize == 0) {
            bError = true;
            break;
        }
        
        nSize = __write_buffer_to_file(ptOutput, s_chBuffer, nSize);
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
    printf("\r\nTotal Size: %d Bytes \r\n"
            "Close files.\r\n", nTotalSize );

    fclose(ptOutput);
    fclose(ptInput);
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
    
    file_path_demo();
    file_copy_demo();
    
    return 0;
}


#ifdef   __cplusplus
{
#endif
