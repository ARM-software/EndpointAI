/*
 * Copyright (C) 2010-2020 Arm Limited or its affiliates. All rights reserved.
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
 * Title:        arm-2d_op_table.c
 * Description:  Basic Tile operations
 *
 * $Date:        01. December 2020
 * $Revision:    V.0.5.0
 *
 * Target Processor:  Cortex-M cores
 *
 * -------------------------------------------------------------------- */

#define __ARM_2D_IMPL__

#include "arm_2d.h"
#include "__arm_2d_impl.h"

#ifdef   __cplusplus
extern "C" {
#endif

#if defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wincompatible-pointer-types-discards-qualifiers"
#   pragma clang diagnostic ignored "-Wcast-qual"
#   pragma clang diagnostic ignored "-Wcast-align"
#   pragma clang diagnostic ignored "-Wextra-semi-stmt"
#   pragma clang diagnostic ignored "-Wsign-conversion"
#   pragma clang diagnostic ignored "-Wunused-function"
#   pragma clang diagnostic ignored "-Wimplicit-int-float-conversion"
#   pragma clang diagnostic ignored "-Wdouble-promotion"
#   pragma clang diagnostic ignored "-Wunused-parameter"
#   pragma clang diagnostic ignored "-Wimplicit-float-conversion"
#   pragma clang diagnostic ignored "-Wimplicit-int-conversion"
#   pragma clang diagnostic ignored "-Wtautological-pointer-compare"
#   pragma clang diagnostic ignored "-Wsign-compare"
#endif

const __arm_2d_op_info_t ARM_2D_OP_TABLE[__ARM_2D_OP_NUMBER] = {

    [ARM_2D_OP_BARRIER] = {
        .Info = {
            //! control operation
            .Param = {
                .bHasSource     = false,
                .bHasTarget     = false,
            },
            .chOpIndex      = __ARM_2D_OP_IDX_BARRIER,
            .LowLevelInterfaceIndex = {
                .CopyLike = __ARM_2D_IO_NONE,
                .FillLike = __ARM_2D_IO_NONE,
            },
        },
    },
    
    [ARM_2D_OP_TILE_COPY_RGB16] = {
        .Info = {
            .Colour = {
                .chScheme   = ARM_2D_COLOUR_RGB565,
            },
            .Param = {
                .bHasSource     = true,
                .bHasTarget     = true,
            },
            .chOpIndex      = __ARM_2D_OP_IDX_COPY,
            
            .LowLevelInterfaceIndex = {
                .CopyLike = __ARM_2D_IO_COPY,
                .FillLike = __ARM_2D_IO_FILL,
            },
        },
    },
    
    [ARM_2D_OP_TILE_COPY_RGB32] = {
        .Info = {
            .Colour = {
                .chScheme   = ARM_2D_COLOUR_RGB888,
            },
            .Param = {
                .bHasSource     = true,
                .bHasTarget     = true,
            },
            .chOpIndex      = __ARM_2D_OP_IDX_COPY,
            
            .LowLevelInterfaceIndex = {
                .CopyLike = __ARM_2D_IO_COPY,
                .FillLike = __ARM_2D_IO_FILL,
            },
        },
    },
    
    [ARM_2D_OP_TILE_COPY_WITH_COLOUR_MASKING_RGB16] = {
        .Info = {
            .Colour = {
                .chScheme   = ARM_2D_COLOUR_RGB16,
            },
            .Param = {
                .bHasSource     = true,
                .bHasTarget     = true,
            },
            .chOpIndex      = __ARM_2D_OP_IDX_COPY_WITH_COLOUR_MASKING,
            
            .LowLevelInterfaceIndex = {
                .CopyLike = __ARM_2D_IO_COPY_WITH_COLOUR_MASKING,
                .FillLike = __ARM_2D_IO_FILL_WITH_COLOUR_MASKING,
            },
        },
    },
    
    [ARM_2D_OP_TILE_COPY_WITH_COLOUR_MASKING_RGB32] = {
        .Info = {
            .Colour = {
                .chScheme   = ARM_2D_COLOUR_RGB32,
            },
            .Param = {
                .bHasSource     = true,
                .bHasTarget     = true,
            },
            .chOpIndex      = __ARM_2D_OP_IDX_COPY_WITH_COLOUR_MASKING,
            
            .LowLevelInterfaceIndex = {
                .CopyLike = __ARM_2D_IO_COPY_WITH_COLOUR_MASKING,
                .FillLike = __ARM_2D_IO_FILL_WITH_COLOUR_MASKING,
            },
        },
    },
    
    [ARM_2D_OP_COLOUR_FILL_RGB16] = {
        .Info = {
            .Colour = {
                .chScheme   = ARM_2D_COLOUR_RGB16,
            },
            .Param = {
                .bHasSource     = false,
                .bHasTarget     = true,
            },
            .chOpIndex      = __ARM_2D_OP_IDX_FILL_COLOUR,
            
            .LowLevelInterfaceIndex = {
                .TileProcessLike = __ARM_2D_IO_FILL_COLOUR,
            },
        },
    },
    
    [ARM_2D_OP_COLOUR_FILL_RGB32] = {
        .Info = {
            .Colour = {
                .chScheme   = ARM_2D_COLOUR_RGB32,
            },
            .Param = {
                .bHasSource     = false,
                .bHasTarget     = true,
            },
            .chOpIndex      = __ARM_2D_OP_IDX_FILL_COLOUR,
            
            .LowLevelInterfaceIndex = {
                .TileProcessLike = __ARM_2D_IO_FILL_COLOUR,
            },
        },
    },
    
    
    [ARM_2D_OP_ALPHA_BLENDING_RGB565] = {
        .Info = {
            .Colour = {
                .chScheme   = ARM_2D_COLOUR_RGB565,
            },
            .Param = {
                .bHasSource     = true,
                .bHasTarget     = true,
            },
            .chOpIndex      = __ARM_2D_OP_IDX_ALPHA_BLENDING,
            
            .LowLevelInterfaceIndex = {
                .CopyLike = __ARM_2D_IO_ALPHA_BLENDING,
                .FillLike = __ARM_2D_IO_NONE,
            },
        },
    },
    
    [ARM_2D_OP_ALPHA_BLENDING_RGB888] = {
        .Info = {
            .Colour = {
                .chScheme   = ARM_2D_COLOUR_RGB888,
            },
            .Param = {
                .bHasSource     = true,
                .bHasTarget     = true,
            },
            .chOpIndex      = __ARM_2D_OP_IDX_ALPHA_BLENDING,
            
            .LowLevelInterfaceIndex = {
                .CopyLike = __ARM_2D_IO_ALPHA_BLENDING,
                .FillLike = __ARM_2D_IO_NONE,
            },
        },
    },
    
    [ARM_2D_OP_ALPHA_BLENDING_WITH_COLOUR_MASKING_RGB565] = {
        .Info = {
            .Colour = {
                .chScheme   = ARM_2D_COLOUR_RGB565,
            },
            .Param = {
                .bHasSource     = true,
                .bHasTarget     = true,
            },
            .chOpIndex      = __ARM_2D_OP_IDX_ALPHA_BLENDING_WITH_COLOUR_MASKING,
            
            .LowLevelInterfaceIndex = {
                .CopyLike = __ARM_2D_IO_ALPHA_BLENDING_WITH_COLOUR_MASKING,
                .FillLike = __ARM_2D_IO_NONE,
            },
        },
    },
    
    [ARM_2D_OP_ALPHA_BLENDING_WITH_COLOUR_MASKING_RGB888] = {
        .Info = {
            .Colour = {
                .chScheme   = ARM_2D_COLOUR_RGB888,
            },
            .Param = {
                .bHasSource     = true,
                .bHasTarget     = true,
            },
            .chOpIndex      = __ARM_2D_OP_IDX_ALPHA_BLENDING_WITH_COLOUR_MASKING,
            
            .LowLevelInterfaceIndex = {
                .CopyLike = __ARM_2D_IO_ALPHA_BLENDING_WITH_COLOUR_MASKING,
                .FillLike = __ARM_2D_IO_NONE,
            },
        },
    },
    
    [ARM_2D_OP_ALPHA_COLOUR_FILL_RGB565] = {
        .Info = {
            .Colour = {
                .chScheme   = ARM_2D_COLOUR_RGB565,
            },
            .Param = {
                .bHasSource     = false,
                .bHasTarget     = true,
            },
            .chOpIndex      = __ARM_2D_OP_IDX_ALPHA_FILL_COLOUR,
            
            .LowLevelInterfaceIndex = {
                .TileProcessLike = __ARM_2D_IO_ALPHA_FILL_COLOUR,
            },
        },
    },
    
    [ARM_2D_OP_ALPHA_COLOUR_FILL_RGB888] = {
        .Info = {
            .Colour = {
                .chScheme   = ARM_2D_COLOUR_RGB888,
            },
            .Param = {
                .bHasSource     = false,
                .bHasTarget     = true,
            },
            .chOpIndex      = __ARM_2D_OP_IDX_ALPHA_FILL_COLOUR,
            
            .LowLevelInterfaceIndex = {
                .TileProcessLike = __ARM_2D_IO_ALPHA_FILL_COLOUR,
            },
        },
    },
    
    [ARM_2D_OP_DRAW_POINT_RGB16] = {
        .Info = {
            .Colour = {
                .chScheme   = ARM_2D_COLOUR_RGB16,
            },
            .Param = {
                .bHasSource     = false,
                .bHasTarget     = true,
            },
            .chOpIndex      = __ARM_2D_OP_IDX_DRAW_POINT,
            
            .LowLevelInterfaceIndex = {
                .TileProcessLike = __ARM_2D_IO_DRAW_POINT,
            },
        },
    },
    
    [ARM_2D_OP_DRAW_POINT_RGB32] = {
        .Info = {
            .Colour = {
                .chScheme   = ARM_2D_COLOUR_RGB32,
            },
            .Param = {
                .bHasSource     = false,
                .bHasTarget     = true,
            },
            .chOpIndex      = __ARM_2D_OP_IDX_DRAW_POINT,
            
            .LowLevelInterfaceIndex = {
                .TileProcessLike = __ARM_2D_IO_DRAW_POINT,
            },
        },
    },
    
    [ARM_2D_OP_DRAW_PATTERN_RGB16] = {
        .Info = {
            .Colour = {
                .chScheme   = ARM_2D_COLOUR_RGB16,
            },
            .Param = {
                .bHasSource             = true,
                .bHasTarget             = true,
                .bAllowEnforcedColour   = true,
            },
            .chOpIndex      = __ARM_2D_OP_IDX_DRAW_PATTERN,
            
            .LowLevelInterfaceIndex = {
                .CopyLike = __ARM_2D_IO_DRAW_PATTERN,
                .FillLike = __ARM_2D_IO_NONE,
            },
        },
    },
    
    [ARM_2D_OP_DRAW_PATTERN_RGB32] = {
        .Info = {
            .Colour = {
                .chScheme   = ARM_2D_COLOUR_RGB32,
            },
            .Param = {
                .bHasSource             = true,
                .bHasTarget             = true,
                .bAllowEnforcedColour   = true,
            },
            .chOpIndex      = __ARM_2D_OP_IDX_DRAW_PATTERN,
            
            .LowLevelInterfaceIndex = {
                .CopyLike = __ARM_2D_IO_DRAW_PATTERN,
                .FillLike = __ARM_2D_IO_NONE,
            },
        },
    },
    
    [ARM_2D_OP_CONVERT_TO_RGB565] = {
        .Info = {
            .Colour = {
                .chScheme   = ARM_2D_COLOUR_RGB565,
            },
            .Param = {
                .bHasSource             = true,
                .bHasTarget             = true,
                .bAllowEnforcedColour   = true,
            },
            .chOpIndex      = __ARM_2D_OP_IDX_COLOUR_FORMAT_CONVERSION,
            
            .LowLevelInterfaceIndex = {
                .CopyLike = __ARM_2D_IO_COLOUR_CONVERT_TO_RGB565,
                .FillLike = __ARM_2D_IO_NONE,
            },
        },
    },
    
    
    [ARM_2D_OP_CONVERT_TO_RGB888] = {
        .Info = {
            .Colour = {
                .chScheme   = ARM_2D_COLOUR_RGB888,
            },
            .Param = {
                .bHasSource             = true,
                .bHasTarget             = true,
                .bAllowEnforcedColour   = true,
            },
            .chOpIndex      = __ARM_2D_OP_IDX_COLOUR_FORMAT_CONVERSION,
            
            .LowLevelInterfaceIndex = {
                .CopyLike = __ARM_2D_IO_COLOUR_CONVERT_TO_RGB888,
                .FillLike = __ARM_2D_IO_NONE,
            },
        },
    },
};


__WEAK
const struct __arm_2d_io_table __ARM_2D_IO_TABLE = {
    .OP = {
        [__ARM_2D_IO_COPY] = {
            .SW = (__arm_2d_io_func_t *)&__arm_2d_sw_tile_copy,
        },
        [__ARM_2D_IO_FILL] = {
            .SW = (__arm_2d_io_func_t *)&__arm_2d_sw_tile_fill,
        },
        [__ARM_2D_IO_COPY_WITH_COLOUR_MASKING] = {
            .SW = (__arm_2d_io_func_t *)&__arm_2d_sw_tile_copy_with_colour_masking,
        },
        [__ARM_2D_IO_FILL_WITH_COLOUR_MASKING] = {
            .SW = (__arm_2d_io_func_t *)&__arm_2d_sw_tile_fill_with_colour_masking,
        },
        [__ARM_2D_IO_FILL_COLOUR] = {
            .SW = (__arm_2d_io_func_t *)&__arm_2d_sw_colour_filling,
        },
        [__ARM_2D_IO_ALPHA_BLENDING] = {
            .SW = (__arm_2d_io_func_t *)&__arm_2d_sw_alpha_blending,
        },
        [__ARM_2D_IO_ALPHA_BLENDING_WITH_COLOUR_MASKING] = {
            .SW = (__arm_2d_io_func_t *)&__arm_2d_sw_alpha_blending_with_colour_masking,
        },
        [__ARM_2D_IO_ALPHA_FILL_COLOUR] = {
            .SW = (__arm_2d_io_func_t *)&__arm_2d_sw_colour_filling_with_alpha,
        },
        [__ARM_2D_IO_DRAW_POINT] = {
            .SW = (__arm_2d_io_func_t *)&__arm_2d_sw_draw_point,
        },
        [__ARM_2D_IO_DRAW_PATTERN] = {
            .SW = (__arm_2d_io_func_t *)&__arm_2d_sw_draw_pattern,
        },
        [__ARM_2D_IO_COLOUR_CONVERT_TO_RGB565] = {
            .SW = (__arm_2d_io_func_t *)__arm_2d_sw_convert_colour_to_rgb565,
        },
        [__ARM_2D_IO_COLOUR_CONVERT_TO_RGB888] = {
            .SW = (__arm_2d_io_func_t *)__arm_2d_sw_convert_colour_to_rgb888,
        },
        
    },
};

#if defined(__clang__)
#   pragma clang diagnostic pop
#endif

#ifdef   __cplusplus
}
#endif
