/*
 * Copyright (C) 2023 Arm Limited or its affiliates. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifdef __FFF_USER_DEFINE_FILE_FRAMEWORK__
#undef __FFF_USER_DEFINE_FILE_FRAMEWORK__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/

#define __FFF_CFG_ROOT__            MPSx_Local_Disk
#define __FFF_CFG_WORKING_PATH__    "."

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
/*============================ IMPLEMENTATION ================================*/

#if __IS_COMPILER_ARM_COMPILER_5__
extern const uint8_t FFFYML2C_ROM_data[];
extern const uint32_t FFFYML2C_ROM_size;

#else
#define INCBIN_PREFIX
#define INCBIN_STYLE INCBIN_STYLE_SNAKE
#include "incbin.h"

INCBIN(FFFYML2C_ROM, "RTE/File_System/fffyml2c.py");
#endif

def_fff(MPSx_Local_Disk,
    use_fff_disk(disk_c,
        arm_mem_file_node_t         fffyml2c_py;
    );
    
    use_fff_disk(disk_d,
        use_fff_folder(symphony,
            arm_mem_file_node_t     no_1_mp3;
            arm_mem_file_node_t     no_2_mp3;
            arm_mem_file_node_t     no_3_mp3;
            arm_mem_file_node_t     no_4_mp3;
        );
    );
)

imp_fff(MPSx_Local_Disk,
    fff_list(
        fff_disk(disk_c, &MPSx_Local_Disk, disk_d,
            fff_path("C:"),
            fff_list(
                //! memory file: cannon.mp3, read-only
                fff_mem_file(fffyml2c_py, &MPSx_Local_Disk.disk_c, fffyml2c_py, 
                    fff_path("fffyml2c.py"),
                    fff_access(FFF_READ_ONLY),
                    
                    fff_mem_content(FFFYML2C_ROM_data, FFFYML2C_ROM_size),
                ),
            ),
        ),

        fff_disk(disk_d, &MPSx_Local_Disk, disk_d,
            fff_path("D:"),
            fff_list(
                fff_folder(symphony, &MPSx_Local_Disk.disk_d, symphony,
                    fff_path(
                        "symphony\\music\\beethoven", 
                        "symphony/music/beethoven"
                    ),

                    fff_list(
                        fff_mem_file(no_1_mp3, &MPSx_Local_Disk.disk_d.symphony, no_2_mp3, 
                            fff_path("no_1.mp3"),
                            fff_access(FFF_READ_WRITE),
                        ),
                        fff_mem_file(no_2_mp3, &MPSx_Local_Disk.disk_d.symphony, no_3_mp3, 
                            fff_path("no_2.mp3"),
                            fff_access(FFF_READ_WRITE),
                        ),
                        fff_mem_file(no_3_mp3, &MPSx_Local_Disk.disk_d.symphony, no_4_mp3, 
                            fff_path("no_3.mp3"),
                            fff_access(FFF_READ_WRITE),
                        ),
                        fff_mem_file(no_4_mp3, &MPSx_Local_Disk.disk_d.symphony, no_4_mp3, 
                            fff_path("no_4.mp3"),
                            fff_access(FFF_READ_WRITE),
                        ),
                    ),
                ),
            ),
        ),
    ),
);


__attribute__((used))
char *_sys_command_string(char *cmd, int len)
{
    (void)cmd;
    (void)len;
    
    /* write a command line here, which will be passed to main */
    return "--input_file hotel.mp3";
}

#endif
