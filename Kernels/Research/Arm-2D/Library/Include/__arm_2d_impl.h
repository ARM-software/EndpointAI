/*
 * Copyright (C) 2010-2021 Arm Limited or its affiliates. All rights reserved.
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

/* ----------------------------------------------------------------------
 * Project:      Arm-2D Library
 * Title:        arm-2d_acc.h
 * Description:  Basic Tile operations
 *
 * $Date:        13. Jan 2021
 * $Revision:    V.0.5.0
 *
 * Target Processor:  Cortex-M cores
 *
 * -------------------------------------------------------------------- */

#ifndef __ARM_2D_ACCELERATION_H__
#   define __ARM_2D_ACCELERATION_H__      1

/*============================ INCLUDES ======================================*/

#include "arm_2d.h"

#ifdef   __cplusplus
extern "C" {
#endif

#if defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#   pragma clang diagnostic ignored "-Wmissing-declarations"
//#   pragma clang diagnostic ignored "-Wpadded"
#elif __IS_COMPILER_ARM_COMPILER_5__
#   pragma diag_suppress 174,177,188,68,513,144,64
#elif __IS_COMPILER_GCC__
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wunused-value"
#endif

/*============================ MACROS ========================================*/
#undef this
#define this            (*ptThis)

#undef OP_CORE
#define OP_CORE         this.use_as__arm_2d_op_core_t

#define ARM_2D_IMPL(__TYPE, ...)                                                \
            __TYPE *ptThis = (__TYPE *)(NULL,##__VA_ARGS__);                    \
            if (NULL == ptThis) {                                               \
                ptThis = (__TYPE *)&ARM_2D_CTRL.DefaultOP;                      \
            }

/*============================ MACROFIED FUNCTIONS ===========================*/

#define ARM_2D_TRY_ACCELERATION(__ID, __FUNC_PROTOTYPE, ...)                    \
        if (    (NULL != __ARM_2D_IO_TABLE.OP[__ID].HW)                         \
            &&  ((__ID) != (uint8_t)__ARM_2D_IO_NONE)) {                        \
            tResult =                                                           \
                (*(__FUNC_PROTOTYPE *)__ARM_2D_IO_TABLE.OP[__ID].HW)(           \
                                        ptTask,                                 \
                                        ##__VA_ARGS__);                         \
        }

#define ARM_2D_RUN_DEFAULT(__ID, __FUNC_PROTOTYPE, ...)                         \
        if (    (NULL != __ARM_2D_IO_TABLE.OP[__ID].SW)                         \
            &&  ((__ID) != (uint8_t)__ARM_2D_IO_NONE)) {                        \
            tResult =                                                           \
                (*(__FUNC_PROTOTYPE *)__ARM_2D_IO_TABLE.OP[__ID].SW)(           \
                                        ptTask,                                 \
                                        ##__VA_ARGS__);                         \
        } else {                                                                \
            tResult = (arm_fsm_rt_t)ARM_2D_ERR_NOT_SUPPORT;                     \
        }

/*============================ TYPES =========================================*/



//! \name Operation Index: used for logging and debugging purpose
//! @{
enum {
    /*------------ arm-2d operation idx begin --------------*/
    __ARM_2D_OP_IDX_BARRIER,
    __ARM_2D_OP_IDX_SYNC = __ARM_2D_OP_IDX_BARRIER,
    
    __ARM_2D_OP_IDX_COPY,
    __ARM_2D_OP_IDX_COPY_WITH_COLOUR_MASKING,
    __ARM_2D_OP_IDX_FILL_COLOUR,
    __ARM_2D_OP_IDX_FILL_COLOUR_WITH_COLOUR_MASKING,
    
    __ARM_2D_OP_IDX_ALPHA_BLENDING,
    __ARM_2D_OP_IDX_ALPHA_BLENDING_WITH_COLOUR_MASKING,
    __ARM_2D_OP_IDX_ALPHA_FILL_COLOUR,
    __ARM_2D_OP_IDX_ALPHA_FILL_COLOUR_WITH_COLOUR_MASKING,
    
    __ARM_2D_OP_IDX_DRAW_POINT,
    __ARM_2D_OP_IDX_DRAW_PATTERN,
    
    __ARM_2D_OP_IDX_COLOUR_FORMAT_CONVERSION,
    
    __ARM_2D_OP_IDX_ROTATE,
    /*------------ cmsisi-2d operation idx end --------------*/
};
//! @}


//! \name Operation Low Level IO Index
//! @{
enum {
    /*------------ arm-2d operation idx begin --------------*/
    __ARM_2D_IO_NONE = -1,
    
    __ARM_2D_IO_COPY,
    __ARM_2D_IO_FILL,
    
    __ARM_2D_IO_COPY_WITH_COLOUR_MASKING,
    __ARM_2D_IO_FILL_WITH_COLOUR_MASKING,
    
    __ARM_2D_IO_FILL_COLOUR,
    __ARM_2D_IO_FILL_COLOUR_WITH_COLOUR_MASKING,
    
    __ARM_2D_IO_ALPHA_BLENDING,
    __ARM_2D_IO_ALPHA_BLENDING_WITH_FILL,
    
    __ARM_2D_IO_ALPHA_BLENDING_WITH_COLOUR_MASKING,
    __ARM_2D_IO_ALPHA_BLENDING_WITH_FILL_AND_COLOUR_MASKING,
    
    __ARM_2D_IO_ALPHA_FILL_COLOUR,
    __ARM_2D_IO_ALPHA_FILL_COLOUR_WITH_COLOUR_MASKING,
    
    __ARM_2D_IO_DRAW_POINT,
    __ARM_2D_IO_DRAW_PATTERN,
    __ARM_2D_IO_COLOUR_CONVERT_TO_RGB565,
    __ARM_2D_IO_COLOUR_CONVERT_TO_RGB888,
    
    __ARM_2D_IO_ROTATE,
    
    /*------------ arm-2d operation idx end ----------------*/
    __ARM_2D_IO_NUMBER,
    __ARM_2D_IO_DEFAULT_COPY = __ARM_2D_IO_COPY,
    __ARM_2D_IO_DEFAULT_FILL = __ARM_2D_IO_FILL,
};
//! @}


enum {
    ARM_2D_OP_SYNC,
    ARM_2D_OP_BARRIER = ARM_2D_OP_SYNC,
    

    ARM_2D_OP_TILE_COPY_RGB16,
    ARM_2D_OP_TILE_COPY_RGB32,
    ARM_2D_OP_TILE_COPY_WITH_COLOUR_MASKING_RGB16,
    ARM_2D_OP_TILE_COPY_WITH_COLOUR_MASKING_RGB32,
    ARM_2D_OP_COLOUR_FILL_RGB16,
    ARM_2D_OP_COLOUR_FILL_RGB32,
    ARM_2D_OP_ALPHA_BLENDING_RGB565,
    ARM_2D_OP_ALPHA_BLENDING_RGB888,
    ARM_2D_OP_ALPHA_BLENDING_WITH_COLOUR_MASKING_RGB565,
    ARM_2D_OP_ALPHA_BLENDING_WITH_COLOUR_MASKING_RGB888,
    ARM_2D_OP_ALPHA_COLOUR_FILL_RGB565,
    ARM_2D_OP_ALPHA_COLOUR_FILL_RGB888,

    ARM_2D_OP_DRAW_POINT_RGB16,
    ARM_2D_OP_DRAW_POINT_RGB32,
    ARM_2D_OP_DRAW_PATTERN_RGB16,
    ARM_2D_OP_DRAW_PATTERN_RGB32,
    
    ARM_2D_OP_CONVERT_TO_RGB565,
    ARM_2D_OP_CONVERT_TO_RGB888,
    
    ARM_2D_OP_ROTATE_RGB565,
    ARM_2D_OP_ROTATE_RGB888,

    __ARM_2D_OP_NUMBER,
};


typedef struct __arm_2d_sub_task_t __arm_2d_sub_task_t;

typedef
arm_fsm_rt_t __arm_2d_io_tile_default_tile_process_t(   
                                        __arm_2d_sub_task_t *ptTask,
                                        void *__RESTRICT pTarget,
                                        int16_t iStride,
                                        arm_2d_size_t *__RESTRICT ptSize);


typedef arm_fsm_rt_t __arm_2d_io_func_t(__arm_2d_sub_task_t *ptTask);

struct __arm_2d_io_table {
ARM_PRIVATE(
    struct {
        arm_fsm_rt_t    (*SW)(__arm_2d_sub_task_t *ptTask);
        arm_fsm_rt_t    (*HW)(__arm_2d_sub_task_t *ptTask);
    }OP[__ARM_2D_IO_NUMBER ];
)};

typedef struct __arm_2d_tile_param_t {
    void *              pBuffer;
    int32_t             nOffset;
    int16_t             iStride;
    uint16_t                        : 16;
    arm_2d_region_t     tValidRegion;
} __arm_2d_tile_param_t;

typedef struct __arm_2d_param_copy_t {
    __arm_2d_tile_param_t tSource;
    __arm_2d_tile_param_t tTarget;
    arm_2d_size_t       tCopySize;
} __arm_2d_param_copy_t;

typedef struct __arm_2d_param_copy_orig_t {
    implement(__arm_2d_param_copy_t);
    
    __arm_2d_tile_param_t tOrigin;
    
} __arm_2d_param_copy_orig_t;

typedef struct __arm_2d_param_fill_t {
    __arm_2d_tile_param_t tSource;
    __arm_2d_tile_param_t tTarget;
} __arm_2d_param_fill_t;

typedef struct __arm_2d_param_fill_orig_t {
    implement(__arm_2d_param_fill_t);
    
    __arm_2d_tile_param_t tOrigin;
    
} __arm_2d_param_fill_orig_t;

struct __arm_2d_sub_task_t{
ARM_PRIVATE(
    __arm_2d_sub_task_t         *ptNext;
    
    arm_2d_op_core_t            *ptOP;
    
    uint8_t         chLowLeveIOIndex;                                              //!< the type of IO interface
    uint8_t         bIsCPL              : 1;
    uint8_t                             : 7;
    uint16_t                            : 16;
    
    union {
        __arm_2d_tile_param_t           tTileProcess;
        __arm_2d_param_copy_t           tCopy;
        __arm_2d_param_fill_t           tFill;
        __arm_2d_param_copy_orig_t      tCopyOrig;
        __arm_2d_param_fill_orig_t      tFillOrig;
    }Param;
)};


struct __arm_2d_op_control {
ARM_PRIVATE(
    __arm_2d_sub_task_t   *ptFreeList;
    struct {
        __arm_2d_sub_task_t   *ptHead;
        __arm_2d_sub_task_t   *ptTail;
    } TaskFIFO;
    uint16_t                hwFreeCount;
    uint16_t                hwTaskCount;
    uint16_t                hwBookCount;
    uint16_t                            : 16;
    
    arm_2d_tile_t           *ptDefaultFrameBuffer;
    
    union {
        arm_2d_op_t                tBasic;
        arm_2d_op_fill_cl_t        tFillColour;
        arm_2d_op_src_t            tWithSource;
        arm_2d_op_src_alpha_msk_t  tWithAlphaMask;
        arm_2d_op_alpha_t          tAlpha;
        arm_2d_op_alpha_cl_msk_t   tAlphaColourMask;
        arm_2d_op_alpha_fill_cl_t  tAlphaColourFill;
        arm_2d_op_drw_patn_t       tDrawPattern;
    } DefaultOP;
)};



/*============================ GLOBAL VARIABLES ==============================*/
extern 
const struct __arm_2d_io_table __ARM_2D_IO_TABLE;

extern
const __arm_2d_op_info_t ARM_2D_OP_TABLE[__ARM_2D_OP_NUMBER];

extern struct __arm_2d_op_control ARM_2D_CTRL;

/*============================ PROTOTYPES ====================================*/

extern
arm_fsm_rt_t __arm_2d_op_invoke(arm_2d_op_core_t *ptOP);


/*----------------------------------------------------------------------------*
 * Default Software Implementations                                           *
 *----------------------------------------------------------------------------*/
 
extern 
arm_fsm_rt_t __arm_2d_sw_colour_filling(__arm_2d_sub_task_t *ptTask);
        
extern 
arm_fsm_rt_t __arm_2d_sw_draw_point(__arm_2d_sub_task_t *ptTask);        

extern 
arm_fsm_rt_t __arm_2d_sw_draw_pattern( __arm_2d_sub_task_t *ptTask);

extern
arm_fsm_rt_t __arm_2d_sw_tile_fill( __arm_2d_sub_task_t *ptTask);

extern
arm_fsm_rt_t __arm_2d_sw_tile_copy(  __arm_2d_sub_task_t *ptTask);

extern 
arm_fsm_rt_t __arm_2d_sw_tile_copy_with_colour_masking(
                                        __arm_2d_sub_task_t *ptTask);
                                        
extern
arm_fsm_rt_t __arm_2d_sw_tile_fill_with_colour_masking( 
                                        __arm_2d_sub_task_t *ptTask);

extern
arm_fsm_rt_t __arm_2d_sw_alpha_blending(__arm_2d_sub_task_t *ptTask);
                                        
extern
arm_fsm_rt_t __arm_2d_sw_alpha_blending_with_colour_masking(
                                        __arm_2d_sub_task_t *ptTask);

extern 
arm_fsm_rt_t __arm_2d_sw_colour_filling_with_alpha(
                                        __arm_2d_sub_task_t *ptTask);

extern
arm_fsm_rt_t __arm_2d_sw_convert_colour_to_rgb565(  
                                    __arm_2d_sub_task_t *ptTask);

extern
arm_fsm_rt_t __arm_2d_sw_convert_colour_to_rgb888(  
                                    __arm_2d_sub_task_t *ptTask);

#if defined(__clang__)
#   pragma clang diagnostic pop
#elif __IS_COMPILER_ARM_COMPILER_5__
#   pragma diag_warning 174,177,188,68,513,144,64
#endif

#ifdef   __cplusplus
}
#endif

#endif


