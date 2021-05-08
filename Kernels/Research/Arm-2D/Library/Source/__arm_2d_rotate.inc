
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
 * Title:        __arm_2d_fill_colour.inc
 * Description:  c code template for drawing pattern
 *
 * $Date:        14. April 2020
 * $Revision:    V.1.0.0
 *
 * -------------------------------------------------------------------- */


#ifndef __API_COLOUR
#   error You have to define __API_COLOUR before using this c template
#endif
#ifndef __API_INT_TYPE
#   error You have to define the __API_INT_TYPE before using this c template
#endif

#undef ____ARM_2D_FUNC
#undef ___ARM_2D_FUNC
#undef __ARM_2D_FUNC
#define ____ARM_2D_FUNC(__NAME, __COLOUR)  __arm_2d_impl_##__COLOUR##_##__NAME
#define ___ARM_2D_FUNC(__NAME, __COLOUR)   ____ARM_2D_FUNC(__NAME, __COLOUR) 
#define __ARM_2D_FUNC(__NAME)   ___ARM_2D_FUNC(__NAME, __API_COLOUR)


static
arm_2d_point_float_t *__arm_2d_rotate_point(const arm_2d_location_t *ptLocation, 
                                            const arm_2d_location_t *ptCenter,
                                            float fAngle,
                                            arm_2d_point_float_t *ptOutBuffer);

extern 
void __ARM_2D_FUNC(rotate)(   __arm_2d_param_copy_orig_t *ptParam,
                                    __arm_2d_rotate_info_t *ptInfo);

__WEAK 
__API_INT_TYPE __ARM_2D_FUNC(get_pixel_colour)(   arm_2d_point_float_t *ptPoint,
                                            arm_2d_region_t *ptOrigValidRegion,
                                            __API_INT_TYPE *pOrigin,
                                            int16_t iOrigStride,
                                            __API_INT_TYPE *pTarget,
                                            int16_t iTargetSride,
                                            __API_INT_TYPE MaskColour)
{
#if     defined(__ARM_2D_HAS_INTERPOLATION_ROTATION__)                          \
    &&  __ARM_2D_HAS_INTERPOLATION_ROTATION__
#error "The current version hasn\'t support interpolation in rotation yet."
#else
    arm_2d_location_t tOriginLocation;

    tOriginLocation.iX = ptPoint->fX;
    tOriginLocation.iY = ptPoint->fY;
    __API_INT_TYPE Pixel = *pTarget;
    
    if (arm_2d_is_point_inside_region(ptOrigValidRegion, &tOriginLocation)) {
        __API_INT_TYPE Temp = pOrigin[   tOriginLocation.iY * iOrigStride 
                                    +   tOriginLocation.iX];
        if (Temp != MaskColour) {
            Pixel = Temp;
        } 
    } 
    
    return Pixel;
#endif
}

__WEAK
void __ARM_2D_FUNC(rotate)(   __arm_2d_param_copy_orig_t *ptParam,
                                    __arm_2d_rotate_info_t *ptInfo)
{
    int_fast16_t iHeight = ptParam->use_as____arm_2d_param_copy_t.tCopySize.iHeight;
    int_fast16_t iWidth = ptParam->use_as____arm_2d_param_copy_t.tCopySize.iWidth;

    int_fast16_t iTargetStride = ptParam->use_as____arm_2d_param_copy_t.tTarget.iStride;
    __API_INT_TYPE *pTargetBase = ptParam->use_as____arm_2d_param_copy_t.tTarget.pBuffer;
    __API_INT_TYPE *pOrigin = ptParam->tOrigin.pBuffer;
    int_fast16_t iOrigStride = ptParam->tOrigin.iStride;
    __API_INT_TYPE MaskColour = ptInfo->Mask.hwColour;
    float fAngle = -ptInfo->fAngle;
    arm_2d_location_t tOffset = ptParam->use_as____arm_2d_param_copy_t.tSource.tValidRegion.tLocation;
    
    for (int_fast16_t y = 0; y < iHeight; y++) {

        for (int_fast16_t x = 0; x < iWidth; x++) {
            
            arm_2d_point_float_t tPoint;
            arm_2d_location_t tSrcPoint = ptInfo->tDummySourceOffset;
            tSrcPoint.iX += x + tOffset.iX;
            tSrcPoint.iY += y + tOffset.iY;
            __arm_2d_rotate_point(  &tSrcPoint,
                                    &(ptInfo->tCenter),
                                    fAngle,
                                    &tPoint);
            
            *pTargetBase = __ARM_2D_FUNC(get_pixel_colour)(
                                        &tPoint,
                                        &ptParam->tOrigin.tValidRegion,
                                        pOrigin,
                                        iOrigStride,
                                        pTargetBase,
                                        iTargetStride,
                                        MaskColour
                                    );
            pTargetBase++;
        }
        //phwSourceBase += (iSourceStride - iWidth);
        pTargetBase += (iTargetStride - iWidth);
    }
}



#undef ____ARM_2D_FUNC
#undef ___ARM_2D_FUNC
#undef __ARM_2D_FUNC
#undef __API_COLOUR
#undef __API_INT_TYPE
