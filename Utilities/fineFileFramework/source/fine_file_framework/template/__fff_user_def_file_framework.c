

#ifdef __FFF_USER_DEFINE_FILE_FRAMEWORK__
#undef __FFF_USER_DEFINE_FILE_FRAMEWORK__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
/*============================ IMPLEMENTATION ================================*/

#if __IS_COMPILER_ARM_COMPILER_5__
extern const uint8_t HOTEL_MP3_ROM_data[];
extern const uint32_t HOTEL_MP3_ROM_size;

#else
#define INCBIN_PREFIX
#define INCBIN_STYLE INCBIN_STYLE_SNAKE
#include "incbin.h"

INCBIN(HOTEL_MP3_ROM, "RTE/File_System/hotel.mp3");
#endif

def_fff(MPSx_Local_Disk,
    use_fff_disk(disk_c,
        arm_mem_file_node_t         hotel_mp3;
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

    fff_disk(disk_c, &MPSx_Local_Disk, disk_d,
        fff_disk_path("C:"),
        fff_list(
            //! memory file: cannon.mp3, read-only
            fff_mem_file(hotel_mp3, &MPSx_Local_Disk.disk_c, hotel_mp3, 
                fff_path("hotel.mp3"),
                fff_access(FFF_READ_ONLY),
                
                fff_mem_content(HOTEL_MP3_ROM_data, HOTEL_MP3_ROM_size),
            ),
        ),
    ),
    
    fff_disk(disk_d, &MPSx_Local_Disk, disk_d,
        fff_disk_path("D:"),
        fff_list(
            fff_folder(symphony, &MPSx_Local_Disk.disk_d, symphony,
                fff_folder_path(
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
);


__attribute__((used))
char *_sys_command_string(char *cmd, int len)
{
    (void)cmd;
    (void)len;

#if __FFF_CFG_IGNORE_NO_SEMIHOSTING__
    static char s_cBuffer[__FFF_CFG_SEMIHOSTING_CMD_LINE_BUFFER_SIZE__ + 1];

struct {
    char *pchCMDLine;
    size_t tSize;
} tCMDLine = {
    .pchCMDLine = s_cBuffer,
    .tSize = sizeof(s_cBuffer)
};
    
    if (0 == __semihost(15, &tCMDLine)) {
        return tCMDLine.pchCMDLine;
    }
#endif
    
    /* write a command line here, which will be passed to main */
    return "--input_file hotel.mp3";
}

#endif
