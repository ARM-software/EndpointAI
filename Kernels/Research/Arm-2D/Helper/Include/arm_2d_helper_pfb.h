/*
 * Copyright (c) 2009-2020 Arm Limited. All rights reserved.
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

#ifndef __ARM_2D_HELPER_PFB_H__
#define __ARM_2D_HELPER_PFB_H__

/*============================ INCLUDES ======================================*/
#include "arm_2d.h"

#ifdef   __cplusplus
extern "C" {
#endif


#if defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#endif

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/

#define ARM_2D_HELPER_PFB_INIT( __CB_ADDR,                                      \
                                __SCREEN_WIDTH,                                 \
                                __SCREEN_HEIGHT,                                \
                                __PIXEL_TYPE,                                   \
                                __WIDTH,                                        \
                                __HEIGHT,                                       \
                                __PFB_NUM,                                      \
                                ...                                             \
                                )                                               \
    ({                                                                          \
        ARM_NOINIT static struct {                                              \
            arm_2d_pfb_t tFPB;                                                  \
            __PIXEL_TYPE tBuffer[(__WIDTH) * (__HEIGHT)];                       \
        } s_tPFBs[__PFB_NUM];                                                   \
                                                                                \
        arm_2d_helper_pfb_cfg_t tCFG = {                                        \
            .tDisplayArea.tSize = {                                             \
                .iWidth = (__SCREEN_WIDTH),                                     \
                .iHeight = (__SCREEN_HEIGHT),                                   \
            },                                                                  \
                                                                                \
            .FrameBuffer.ptPFBs = (arm_2d_pfb_t *)s_tPFBs,                      \
            .FrameBuffer.tFrameSize = {                                         \
                .iWidth = (__WIDTH),                                            \
                .iHeight = (__HEIGHT),                                          \
            },                                                                  \
            .FrameBuffer.wBufferSize = sizeof(s_tPFBs[0].tBuffer),              \
            .FrameBuffer.hwPFBNum = dimof(s_tPFBs),                             \
            .Dependency =                                                       \
            __VA_ARGS__,                                                        \
        };                                                                      \
                                                                                \
        arm_2d_helper_pfb_init((__CB_ADDR), &tCFG);                             \
    })

#define __IMPL_ARM_2D_REGION_LIST(__NAME, ...)                                  \
            enum {                                                              \
                __NAME##_offset = __COUNTER__,                                  \
            };                                                                  \
            __VA_ARGS__                                                         \
            arm_2d_region_list_item_t __NAME[] = {
            
            
#define IMPL_ARM_2D_REGION_LIST(__NAME, ...)                                    \
            __IMPL_ARM_2D_REGION_LIST(__NAME,##__VA_ARGS__)
            

#define END_IMPL_ARM_2D_REGION_LIST(...)                                        \
            };
            
#define __ADD_REGION_TO_LIST(__NAME, ...)                                       \
            {                                                                   \
                .ptNext = (arm_2d_region_list_item_t *)                         \
                            &(__NAME[__COUNTER__ - __NAME##_offset]),           \
                .tRegion = {                                                    \
                __VA_ARGS__                                                     \
                },                                                              \
            }
            
#define ADD_REGION_TO_LIST(__NAME, ...)                                         \
            __ADD_REGION_TO_LIST(__NAME, ##__VA_ARGS__) 
            
           
#define __ADD_LAST_REGION_TO_LIST(__NAME, ...)                                  \
            {                                                                   \
                .ptNext = NULL,                                                 \
                .tRegion = {                                                    \
                __VA_ARGS__                                                     \
                },                                                              \
            }
            
#define ADD_LAST_REGION_TO_LIST(__NAME, ...)                                    \
            __ADD_LAST_REGION_TO_LIST(__NAME, ##__VA_ARGS__) 

/*============================ TYPES =========================================*/

typedef struct arm_2d_pfb_t {
    struct arm_2d_pfb_t *ptNext;
    arm_2d_tile_t tTile;
}arm_2d_pfb_t;

typedef struct arm_2d_region_list_item_t {
    struct arm_2d_region_list_item_t *ptNext;
    arm_2d_region_t tRegion;
}arm_2d_region_list_item_t;


typedef arm_fsm_rt_t arm_2d_helper_draw_handler_t( 
                                          void *pTarget,
                                          const arm_2d_tile_t *ptTile);

typedef void arm_2d_helper_render_handler_t( 
                                          void *pTarget,
                                          const arm_2d_pfb_t *ptPFB);

typedef struct arm_2d_helper_render_evt_t {
    arm_2d_helper_render_handler_t *fnHandler;                                     //!< event handler function
    void *pTarget;                                                              //!< user attached target
} arm_2d_helper_render_evt_t;

typedef struct arm_2d_helper_draw_evt_t {
    arm_2d_helper_draw_handler_t *fnHandler;                                     //!< event handler function
    void *pTarget;                                                              //!< user attached target
} arm_2d_helper_draw_evt_t;


enum {
    ARM_2D_PFB_DEPEND_ON_LOW_LEVEL_RENDERING    = _BV(0),
    ARM_2D_PFB_DEPEND_ON_DRAWING                = _BV(1),
    ARM_2D_PFB_DEPEND_ON_LOW_LEVEL_SYNC_UP      = _BV(2),
};

typedef struct arm_2d_helper_pfb_dependency_t {
    //! event handler for low level rendering
    arm_2d_helper_render_evt_t  evtOnLowLevelRendering;                           

    //!< event handler for drawing GUI
    arm_2d_helper_draw_evt_t    evtOnDrawing;  

    //!< low level rendering handler wants to sync-up (return arm_fsm_rt_wait_for_obj)
    arm_2d_evt_t                evtOnLowLevelSyncUp;     
} arm_2d_helper_pfb_dependency_t;

typedef struct arm_2d_helper_pfb_cfg_t {

    arm_2d_region_t tDisplayArea;
    
    struct {
        arm_2d_pfb_t  *ptPFBs;
        arm_2d_size_t  tFrameSize;
        uint32_t       wBufferSize;
        uint16_t       hwPFBNum;
        uint16_t       bDoNOTUpdateDefaultFrameBuffer   : 1;
        uint16_t       bDisableDynamicFPBSize           : 1;
        uint16_t                                        : 14;
    } FrameBuffer;
    
    arm_2d_helper_pfb_dependency_t Dependency;

} arm_2d_helper_pfb_cfg_t;


typedef struct arm_2d_helper_pfb_t {

ARM_PRIVATE(
    arm_2d_helper_pfb_cfg_t tCFG;
    
    struct {
        arm_2d_region_t             tDrawRegion;
        arm_2d_region_t             tTargetRegion;
        arm_2d_region_list_item_t  *ptDirtyRegion;
        arm_2d_tile_t               tPFBTile;
        arm_2d_size_t               tFrameSize;
        bool                        bFirstIteration;
        bool                        bIsRegionChanged;
        uint8_t                     chPT;
        uint8_t                                    : 8;
        
        arm_2d_pfb_t               *ptCurrent;
        arm_2d_pfb_t               *ptFreeList;
        arm_2d_tile_t               *ptFrameBuffer;
    } Adapter;
)   

    struct {
        int32_t nTotalCycle;
        int32_t nRenderingCycle;
    } Statistics; 

} arm_2d_helper_pfb_t;

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/

ARM_NONNULL(1,2)
extern
arm_2d_err_t arm_2d_helper_pfb_init(arm_2d_helper_pfb_t *ptThis, 
                                    arm_2d_helper_pfb_cfg_t *ptCFG);

ARM_NONNULL(1)
extern
arm_fsm_rt_t arm_2d_helper_pfb_task(arm_2d_helper_pfb_t *ptThis, 
                                    arm_2d_region_list_item_t *ptDirtyRegions);
                                    

ARM_NONNULL(1,3)
extern
arm_2d_err_t arm_2d_helper_pfb_update_dependency(
                            arm_2d_helper_pfb_t *ptThis, 
                            uint_fast8_t chMask,
                            const arm_2d_helper_pfb_dependency_t *ptDependency);


ARM_NONNULL(1,2)
extern
void arm_2d_helper_pfb_report_rendering_complete(   arm_2d_helper_pfb_t *ptThis,
                                                    arm_2d_pfb_t *ptPFB);

#if defined(__clang__)
#   pragma clang diagnostic pop
#endif

#ifdef   __cplusplus
}
#endif



#endif
