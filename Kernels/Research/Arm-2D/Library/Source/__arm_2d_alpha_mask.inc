
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
 * Title:        __arm_2d_alpha_mask.inc
 * Description:  c code template for copy and fill like operations
 *
 * $Date:        30. Sept 2021
 * $Revision:    V.1.0.0
 *
 * -------------------------------------------------------------------- */
 
#ifndef __API_CAFWM_COLOUR
#   error You have to define __API_CAFWM_COLOUR before using this c template
#endif
#ifndef __API_CAFWM_INT_TYPE
#   error You have to define the __API_CAFWM_INT_TYPE before using this c template
#endif
#ifndef __API_CAFWM_INT_TYPE_BIT_NUM
#   error You have to define the __API_CAFWM_INT_TYPE_BIT_NUM before using this c template
#endif
#ifndef __API_CAFWM_PIXEL_BLENDING
#   error You have to define __API_PIXEL_BLENDING before using this c template
#endif

#undef ____CAFWM_FUNC
#undef ___CAFWM_FUNC
#undef __CAFWM_FUNC


#ifndef __API_CAFWM_OP_NAME
#   define ____CAFWM_FUNC(__NAME, __COLOUR)                                      \
        __arm_2d_impl_##__COLOUR##_##__NAME
#   define ___CAFWM_FUNC(__NAME, __COLOUR)   ____CAFWM_FUNC(__NAME, __COLOUR) 
#else
#   define _____CAFWM_FUNC(__OP_NAME, __NAME, __COLOUR)                          \
        __arm_2d_impl_##__COLOUR##_##__OP_NAME##_##__NAME
#   define ____CAFWM_FUNC(__OP_NAME, __NAME, __COLOUR)                           \
        _____CAFWM_FUNC(__OP_NAME, __NAME, __COLOUR)
#   define ___CAFWM_FUNC(__NAME, __COLOUR)                                       \
        ____CAFWM_FUNC(__API_CAFWM_OP_NAME, __NAME, __COLOUR)
#endif

#define __CAFWM_FUNC(__NAME)   ___CAFWM_FUNC(__NAME, __API_CAFWM_COLOUR)


#undef ____CAFWM_TYPE
#undef ___CAFWM_TYPE
#undef __CAFWM_TYPE

#ifndef __API_CAFWM_OP_NAME
#   define ____CAFWM_TYPE(__NAME, __COLOUR)  arm_2d_##__COLOUR##_##__NAME
#   define ___CAFWM_TYPE(__NAME, __COLOUR)   ____CAFWM_TYPE(__NAME, __COLOUR) 
#else
#   define _____CAFWM_TYPE(__OP_NAME, __NAME, __COLOUR)                        \
        arm_2d_##__COLOUR##_##__OP_NAME##_##__NAME
#   define ____CAFWM_TYPE(__OP_NAME, __NAME, __COLOUR)                         \
        _____CAFWM_TYPE(__OP_NAME, __NAME, __COLOUR)
#   define ___CAFWM_TYPE(__NAME, __COLOUR)                                     \
        ____CAFWM_TYPE(__API_CAFWM_OP_NAME, __NAME, __COLOUR) 
#endif


#define __CAFWM_TYPE(__NAME)   ___CAFWM_TYPE(__NAME, __API_CAFWM_COLOUR)

/*----------------------------------------------------------------------------*
 * Fill with Mirroring (both masks)                                           *
 *----------------------------------------------------------------------------*/

__WEAK
void __CAFWM_FUNC(masks_fill)(
                        __API_CAFWM_INT_TYPE * __RESTRICT ptSourceBase,
                        int16_t iSourceStride,
                        arm_2d_size_t *__RESTRICT ptSourceSize,
                        
                        uint8_t * __RESTRICT pchSourceMaskBase,
                        int16_t iSourceMaskStride,
                        arm_2d_size_t *__RESTRICT ptSourceMaskSize,
                        
                        __API_CAFWM_INT_TYPE *__RESTRICT ptTargetBase,
                        int16_t iTargetStride,
                        arm_2d_size_t *__RESTRICT ptTargetSize,
                        
                        uint8_t *__RESTRICT pchTargetMaskBase,
                        int16_t iTargetMaskStride,
                        arm_2d_size_t *__RESTRICT ptTargetMaskSize)
{
    uint8_t *__RESTRICT pchTargetMaskLineBase = pchTargetMaskBase;  
    
    for (int_fast16_t iTargetY = 0, iTargetMaskY = 0; iTargetY < ptTargetSize->iHeight;) {
    
        //! reset source
        __API_CAFWM_INT_TYPE *__RESTRICT ptSource = ptSourceBase;      
        uint8_t *pchSourceMask = pchSourceMaskBase;    
        int_fast16_t iSourceMaskY = 0;
        
        for (int_fast16_t iSourceY = 0; iSourceY < ptSourceSize->iHeight; iSourceY++) {
            __API_CAFWM_INT_TYPE *__RESTRICT ptTarget = ptTargetBase;
            uint8_t *__RESTRICT pchTargetMask = pchTargetMaskLineBase;      
            
            /*---------------- Height Loop Begin----------------*/
            uint_fast32_t   wLengthLeft = ptTargetSize->iWidth;

            do {
                uint_fast32_t   wLength = MIN(wLengthLeft, ptSourceSize->iWidth);
                /*---------------- Width Loop Begin----------------*/

                __API_CAFWM_INT_TYPE *__RESTRICT ptSrc = ptSource;
                uint8_t *__RESTRICT pchSrcMsk = pchSourceMask;
                for (int_fast16_t x = 0; x < wLength; x++) {
                    uint16_t hwOpacity = 256 - ((*pchSrcMsk++) * (*pchTargetMask++) >> 8);
                    __API_CAFWM_PIXEL_BLENDING(ptSrc++, ptTarget++, hwOpacity);
                }

                /*---------------- Width Loop End----------------*/
                wLengthLeft -= wLength;
            } while (wLengthLeft);
            
            /*---------------- Height Loop End----------------*/
            ptSource += iSourceStride;
            ptTargetBase += iTargetStride;
            
            iSourceMaskY++;
            //! handle source mask 
            if (    (iSourceMaskY >= ptSourceMaskSize->iHeight)
               ||   (iSourceMaskY >= ptSourceSize->iHeight)) {
                pchSourceMask = pchSourceMaskBase;
                iSourceMaskY = 0;
            } else {
                pchSourceMask += iSourceMaskStride;
            }
            
            iTargetMaskY++;
            //! handle target mask 
            if (    (iTargetMaskY >= ptTargetMaskSize->iHeight)
               ||   (iTargetMaskY >= ptTargetSize->iHeight)) {
                pchTargetMaskLineBase = pchTargetMaskBase;
                iTargetMaskY = 0;
            } else {
                pchTargetMaskLineBase += iTargetMaskStride;
            }
            
            iTargetY++;
            if (iTargetY >= ptTargetSize->iHeight) {
                break;
            }
        }
    }
}

__WEAK
void __CAFWM_FUNC(masks_fill_x_mirror)(
                        __API_CAFWM_INT_TYPE * __RESTRICT ptSourceBase,
                        int16_t iSourceStride,
                        arm_2d_size_t *__RESTRICT ptSourceSize,
                        
                        uint8_t * __RESTRICT pchSourceMaskBase,
                        int16_t iSourceMaskStride,
                        arm_2d_size_t *__RESTRICT ptSourceMaskSize,
                        
                        __API_CAFWM_INT_TYPE *__RESTRICT ptTargetBase,
                        int16_t iTargetStride,
                        arm_2d_size_t *__RESTRICT ptTargetSize,
                        
                        uint8_t *__RESTRICT pchTargetMaskBase,
                        int16_t iTargetMaskStride,
                        arm_2d_size_t *__RESTRICT ptTargetMaskSize)
{
    uint8_t *__RESTRICT pchTargetMaskLineBase = pchTargetMaskBase;  
    
    for (int_fast16_t iTargetY = 0, iTargetMaskY = 0; iTargetY < ptTargetSize->iHeight;) {
    
        //! reset source
        __API_CAFWM_INT_TYPE *__RESTRICT ptSource = ptSourceBase;      
        uint8_t *pchSourceMask = pchSourceMaskBase;    
        int_fast16_t iSourceMaskY = 0;
        
        for (int_fast16_t iSourceY = 0; iSourceY < ptSourceSize->iHeight; iSourceY++) {
            __API_CAFWM_INT_TYPE *__RESTRICT ptTarget = ptTargetBase;
            uint8_t *__RESTRICT pchTargetMask = pchTargetMaskLineBase;      
            
            /*---------------- Height Loop Begin----------------*/
            uint_fast32_t   wLengthLeft = ptTargetSize->iWidth;

            do {
                uint_fast32_t   wLength = MIN(wLengthLeft, ptSourceSize->iWidth);
                /*---------------- Width Loop Begin----------------*/

                __API_CAFWM_INT_TYPE *__RESTRICT ptSrc = ptSource;
                uint8_t *__RESTRICT pchSrcMsk = pchSourceMask;
                
                ptSrc += ptSourceSize->iWidth - 1;
                pchSrcMsk += ptSourceSize->iWidth - 1;
                
                for (int_fast16_t x = 0; x < wLength; x++) {
                    uint16_t hwOpacity = 256 - ((*pchSrcMsk--) * (*pchTargetMask++) >> 8);
                    __API_CAFWM_PIXEL_BLENDING(ptSrc--, ptTarget++, hwOpacity);
                }

                /*---------------- Width Loop End----------------*/
                wLengthLeft -= wLength;
            } while (wLengthLeft);
            
            /*---------------- Height Loop End----------------*/
            ptSource += iSourceStride;
            ptTargetBase += iTargetStride;
            
            iSourceMaskY++;
            //! handle source mask 
            if (    (iSourceMaskY >= ptSourceMaskSize->iHeight)
               ||   (iSourceMaskY >= ptSourceSize->iHeight)) {
                pchSourceMask = pchSourceMaskBase;
                iSourceMaskY = 0;
            } else {
                pchSourceMask += iSourceMaskStride;
            }
            
            iTargetMaskY++;
            //! handle target mask 
            if (    (iTargetMaskY >= ptTargetMaskSize->iHeight)
               ||   (iTargetMaskY >= ptTargetSize->iHeight)) {
                pchTargetMaskLineBase = pchTargetMaskBase;
                iTargetMaskY = 0;
            } else {
                pchTargetMaskLineBase += iTargetMaskStride;
            }
            
            iTargetY++;
            if (iTargetY >= ptTargetSize->iHeight) {
                break;
            }
        }
    }


}

__WEAK
void __CAFWM_FUNC(masks_fill_y_mirror)(
                        __API_CAFWM_INT_TYPE * __RESTRICT ptSourceBase,
                        int16_t iSourceStride,
                        arm_2d_size_t *__RESTRICT ptSourceSize,
                        
                        uint8_t * __RESTRICT pchSourceMaskBase,
                        int16_t iSourceMaskStride,
                        arm_2d_size_t *__RESTRICT ptSourceMaskSize,
                        
                        __API_CAFWM_INT_TYPE *__RESTRICT ptTargetBase,
                        int16_t iTargetStride,
                        arm_2d_size_t *__RESTRICT ptTargetSize,
                        
                        uint8_t *__RESTRICT pchTargetMaskBase,
                        int16_t iTargetMaskStride,
                        arm_2d_size_t *__RESTRICT ptTargetMaskSize)
{
    assert(ptSourceSize->iHeight <= ptSourceMaskSize->iHeight);
    pchSourceMaskBase += iSourceMaskStride * (ptSourceSize->iHeight - 1);
    

    uint8_t *__RESTRICT pchTargetMaskLineBase = pchTargetMaskBase;  
    
    for (int_fast16_t iTargetY = 0, iTargetMaskY = 0; iTargetY < ptTargetSize->iHeight;) {
    
        //! reset source
        __API_CAFWM_INT_TYPE *__RESTRICT ptSource 
            = ptSourceBase + iSourceStride * (ptSourceSize->iHeight - 1);     
        uint8_t *pchSourceMask = pchSourceMaskBase;    
        int_fast16_t iSourceMaskY = 0;
        
        for (int_fast16_t iSourceY = 0; iSourceY < ptSourceSize->iHeight; iSourceY++) {
            __API_CAFWM_INT_TYPE *__RESTRICT ptTarget = ptTargetBase;
            uint8_t *__RESTRICT pchTargetMask = pchTargetMaskLineBase;      
            
            /*---------------- Height Loop Begin----------------*/
            uint_fast32_t   wLengthLeft = ptTargetSize->iWidth;

            do {
                uint_fast32_t   wLength = MIN(wLengthLeft, ptSourceSize->iWidth);
                /*---------------- Width Loop Begin----------------*/

                __API_CAFWM_INT_TYPE *__RESTRICT ptSrc = ptSource;
                uint8_t *__RESTRICT pchSrcMsk = pchSourceMask;
                for (int_fast16_t x = 0; x < wLength; x++) {
                    uint16_t hwOpacity = 256 - ((*pchSrcMsk++) * (*pchTargetMask++) >> 8);
                    __API_CAFWM_PIXEL_BLENDING(ptSrc++, ptTarget++, hwOpacity);
                }

                /*---------------- Width Loop End----------------*/
                wLengthLeft -= wLength;
            } while (wLengthLeft);
            
            /*---------------- Height Loop End----------------*/
            ptSource -= iSourceStride;
            ptTargetBase += iTargetStride;
            
            iSourceMaskY++;
            //! handle source mask 
            if (    (iSourceMaskY >= ptSourceMaskSize->iHeight)
               ||   (iSourceMaskY >= ptSourceSize->iHeight)) {
                pchSourceMask = pchSourceMaskBase;
                iSourceMaskY = 0;
            } else {
                pchSourceMask -= iSourceMaskStride;
            }
            
            iTargetMaskY++;
            //! handle target mask 
            if (    (iTargetMaskY >= ptTargetMaskSize->iHeight)
               ||   (iTargetMaskY >= ptTargetSize->iHeight)) {
                pchTargetMaskLineBase = pchTargetMaskBase;
                iTargetMaskY = 0;
            } else {
                pchTargetMaskLineBase += iTargetMaskStride;
            }
            
            iTargetY++;
            if (iTargetY >= ptTargetSize->iHeight) {
                break;
            }
        }
    }

}

__WEAK
void __CAFWM_FUNC(masks_fill_xy_mirror)(
                        __API_CAFWM_INT_TYPE * __RESTRICT ptSourceBase,
                        int16_t iSourceStride,
                        arm_2d_size_t *__RESTRICT ptSourceSize,
                        
                        uint8_t * __RESTRICT pchSourceMaskBase,
                        int16_t iSourceMaskStride,
                        arm_2d_size_t *__RESTRICT ptSourceMaskSize,
                        
                        __API_CAFWM_INT_TYPE *__RESTRICT ptTargetBase,
                        int16_t iTargetStride,
                        arm_2d_size_t *__RESTRICT ptTargetSize,
                        
                        uint8_t *__RESTRICT pchTargetMaskBase,
                        int16_t iTargetMaskStride,
                        arm_2d_size_t *__RESTRICT ptTargetMaskSize)
{
    assert(ptSourceSize->iHeight <= ptSourceMaskSize->iHeight);
    pchSourceMaskBase += iSourceMaskStride * (ptSourceSize->iHeight - 1);
    

    uint8_t *__RESTRICT pchTargetMaskLineBase = pchTargetMaskBase;  
    
    for (int_fast16_t iTargetY = 0, iTargetMaskY = 0; iTargetY < ptTargetSize->iHeight;) {
    
        //! reset source
        __API_CAFWM_INT_TYPE *__RESTRICT ptSource 
            = ptSourceBase + iSourceStride * (ptSourceSize->iHeight - 1);     
        uint8_t *pchSourceMask = pchSourceMaskBase;    
        int_fast16_t iSourceMaskY = 0;
        
        for (int_fast16_t iSourceY = 0; iSourceY < ptSourceSize->iHeight; iSourceY++) {
            __API_CAFWM_INT_TYPE *__RESTRICT ptTarget = ptTargetBase;
            uint8_t *__RESTRICT pchTargetMask = pchTargetMaskLineBase;      
            
            /*---------------- Height Loop Begin----------------*/
            uint_fast32_t   wLengthLeft = ptTargetSize->iWidth;

            do {
                uint_fast32_t   wLength = MIN(wLengthLeft, ptSourceSize->iWidth);
                /*---------------- Width Loop Begin----------------*/

                __API_CAFWM_INT_TYPE *__RESTRICT ptSrc = ptSource;
                uint8_t *__RESTRICT pchSrcMsk = pchSourceMask;
                
                ptSrc += ptSourceSize->iWidth - 1;
                pchSrcMsk += ptSourceSize->iWidth - 1;
                
                
                for (int_fast16_t x = 0; x < wLength; x++) {
                    uint16_t hwOpacity = 256 - ((*pchSrcMsk--) * (*pchTargetMask++) >> 8);
                    __API_CAFWM_PIXEL_BLENDING(ptSrc--, ptTarget++, hwOpacity);
                }

                /*---------------- Width Loop End----------------*/
                wLengthLeft -= wLength;
            } while (wLengthLeft);
            
            /*---------------- Height Loop End----------------*/
            ptSource -= iSourceStride;
            ptTargetBase += iTargetStride;
            
            iSourceMaskY++;
            //! handle source mask 
            if (    (iSourceMaskY >= ptSourceMaskSize->iHeight)
               ||   (iSourceMaskY >= ptSourceSize->iHeight)) {
                pchSourceMask = pchSourceMaskBase;
                iSourceMaskY = 0;
            } else {
                pchSourceMask -= iSourceMaskStride;
            }
            
            iTargetMaskY++;
            //! handle target mask 
            if (    (iTargetMaskY >= ptTargetMaskSize->iHeight)
               ||   (iTargetMaskY >= ptTargetSize->iHeight)) {
                pchTargetMaskLineBase = pchTargetMaskBase;
                iTargetMaskY = 0;
            } else {
                pchTargetMaskLineBase += iTargetMaskStride;
            }
            
            iTargetY++;
            if (iTargetY >= ptTargetSize->iHeight) {
                break;
            }
        }
    }

}


__WEAK
void __CAFWM_FUNC(masks_fill_mirror)(
                        __API_CAFWM_INT_TYPE * __RESTRICT ptSourceBase,
                        int16_t iSourceStride,
                        arm_2d_size_t *__RESTRICT ptSourceSize,
                        
                        uint8_t * __RESTRICT pchSourceMaskBase,
                        int16_t iSourceMaskStride,
                        arm_2d_size_t *__RESTRICT ptSourceMaskSize,
                        
                        __API_CAFWM_INT_TYPE *__RESTRICT ptTargetBase,
                        int16_t iTargetStride,
                        arm_2d_size_t *__RESTRICT ptTargetSize,
                        
                        uint8_t *__RESTRICT pchTargetMaskBase,
                        int16_t iTargetMaskStride,
                        arm_2d_size_t *__RESTRICT ptTargetMaskSize,
                        uint32_t wMode)
{
    switch (wMode & (ARM_2D_CP_MODE_Y_MIRROR | ARM_2D_CP_MODE_X_MIRROR)) {
        case ARM_2D_CP_MODE_X_MIRROR:
            __CAFWM_FUNC(masks_fill_x_mirror)(
                    ptSourceBase, iSourceStride, ptSourceSize,
                    pchSourceMaskBase, iSourceMaskStride, ptSourceMaskSize,
                    ptTargetBase, iTargetStride, ptTargetSize,
                    pchTargetMaskBase, iTargetMaskStride, ptTargetMaskSize);
            break;
        case ARM_2D_CP_MODE_Y_MIRROR:
            __CAFWM_FUNC(masks_fill_y_mirror)(
                    ptSourceBase, iSourceStride, ptSourceSize,
                    pchSourceMaskBase, iSourceMaskStride, ptSourceMaskSize,
                    ptTargetBase, iTargetStride, ptTargetSize,
                    pchTargetMaskBase, iTargetMaskStride, ptTargetMaskSize);
            break;
        case ARM_2D_CP_MODE_Y_MIRROR | ARM_2D_CP_MODE_X_MIRROR :
            __CAFWM_FUNC(masks_fill_xy_mirror)(
                    ptSourceBase, iSourceStride, ptSourceSize,
                    pchSourceMaskBase, iSourceMaskStride, ptSourceMaskSize,
                    ptTargetBase, iTargetStride, ptTargetSize,
                    pchTargetMaskBase, iTargetMaskStride, ptTargetMaskSize);
            break;
        default:
            assert(false);  /*! this should not happen */
            //break;
    }
}

/*----------------------------------------------------------------------------*
 * Fill with Mirroring (target mask only)                                     *
 *----------------------------------------------------------------------------*/

__WEAK
void __CAFWM_FUNC(des_msk_fill)(
                        __API_CAFWM_INT_TYPE * __RESTRICT ptSourceBase,
                        int16_t iSourceStride,
                        arm_2d_size_t *__RESTRICT ptSourceSize,

                        __API_CAFWM_INT_TYPE *__RESTRICT ptTargetBase,
                        int16_t iTargetStride,
                        arm_2d_size_t *__RESTRICT ptTargetSize,
                        
                        uint8_t *__RESTRICT pchTargetMaskBase,
                        int16_t iTargetMaskStride,
                        arm_2d_size_t *__RESTRICT ptTargetMaskSize)
{
    uint8_t *__RESTRICT pchTargetMaskLineBase = pchTargetMaskBase;  
    
    for (int_fast16_t iTargetY = 0, iTargetMaskY = 0; iTargetY < ptTargetSize->iHeight;) {
    
        //! reset source
        __API_CAFWM_INT_TYPE *__RESTRICT ptSource = ptSourceBase;      
        
        for (int_fast16_t iSourceY = 0; iSourceY < ptSourceSize->iHeight; iSourceY++) {
            __API_CAFWM_INT_TYPE *__RESTRICT ptTarget = ptTargetBase;
            uint8_t *__RESTRICT pchTargetMask = pchTargetMaskLineBase;      
            
            /*---------------- Height Loop Begin----------------*/
            uint_fast32_t   wLengthLeft = ptTargetSize->iWidth;

            do {
                uint_fast32_t   wLength = MIN(wLengthLeft, ptSourceSize->iWidth);
                /*---------------- Width Loop Begin----------------*/

                __API_CAFWM_INT_TYPE *__RESTRICT ptSrc = ptSource;

                for (int_fast16_t x = 0; x < wLength; x++) {
                    uint16_t hwOpacity = 256 - (*pchTargetMask++);
                    __API_CAFWM_PIXEL_BLENDING(ptSrc++, ptTarget++, hwOpacity);
                }

                /*---------------- Width Loop End----------------*/
                wLengthLeft -= wLength;
            } while (wLengthLeft);
            
            /*---------------- Height Loop End----------------*/
            ptSource += iSourceStride;
            ptTargetBase += iTargetStride;
            
            iTargetMaskY++;
            //! handle target mask 
            if (    (iTargetMaskY >= ptTargetMaskSize->iHeight)
               ||   (iTargetMaskY >= ptTargetSize->iHeight)) {
                pchTargetMaskLineBase = pchTargetMaskBase;
                iTargetMaskY = 0;
            } else {
                pchTargetMaskLineBase += iTargetMaskStride;
            }
            
            iTargetY++;
            if (iTargetY >= ptTargetSize->iHeight) {
                break;
            }
        }
    }
}

__WEAK
void __CAFWM_FUNC(des_msk_fill_x_mirror)(
                        __API_CAFWM_INT_TYPE * __RESTRICT ptSourceBase,
                        int16_t iSourceStride,
                        arm_2d_size_t *__RESTRICT ptSourceSize,
                        
                        __API_CAFWM_INT_TYPE *__RESTRICT ptTargetBase,
                        int16_t iTargetStride,
                        arm_2d_size_t *__RESTRICT ptTargetSize,
                        
                        uint8_t *__RESTRICT pchTargetMaskBase,
                        int16_t iTargetMaskStride,
                        arm_2d_size_t *__RESTRICT ptTargetMaskSize)
{
    uint8_t *__RESTRICT pchTargetMaskLineBase = pchTargetMaskBase;  
    
    for (int_fast16_t iTargetY = 0, iTargetMaskY = 0; iTargetY < ptTargetSize->iHeight;) {
    
        //! reset source
        __API_CAFWM_INT_TYPE *__RESTRICT ptSource = ptSourceBase;      
        
        for (int_fast16_t iSourceY = 0; iSourceY < ptSourceSize->iHeight; iSourceY++) {
            __API_CAFWM_INT_TYPE *__RESTRICT ptTarget = ptTargetBase;
            uint8_t *__RESTRICT pchTargetMask = pchTargetMaskLineBase;      
            
            /*---------------- Height Loop Begin----------------*/
            uint_fast32_t   wLengthLeft = ptTargetSize->iWidth;

            do {
                uint_fast32_t   wLength = MIN(wLengthLeft, ptSourceSize->iWidth);
                /*---------------- Width Loop Begin----------------*/

                __API_CAFWM_INT_TYPE *__RESTRICT ptSrc = ptSource;
                
                ptSrc += ptSourceSize->iWidth - 1;
                
                for (int_fast16_t x = 0; x < wLength; x++) {
                    uint16_t hwOpacity = 256 - (*pchTargetMask++);
                    __API_CAFWM_PIXEL_BLENDING(ptSrc--, ptTarget++, hwOpacity);
                }

                /*---------------- Width Loop End----------------*/
                wLengthLeft -= wLength;
            } while (wLengthLeft);
            
            /*---------------- Height Loop End----------------*/
            ptSource += iSourceStride;
            ptTargetBase += iTargetStride;
            
            iTargetMaskY++;
            //! handle target mask 
            if (    (iTargetMaskY >= ptTargetMaskSize->iHeight)
               ||   (iTargetMaskY >= ptTargetSize->iHeight)) {
                pchTargetMaskLineBase = pchTargetMaskBase;
                iTargetMaskY = 0;
            } else {
                pchTargetMaskLineBase += iTargetMaskStride;
            }
            
            iTargetY++;
            if (iTargetY >= ptTargetSize->iHeight) {
                break;
            }
        }
    }


}

__WEAK
void __CAFWM_FUNC(des_msk_fill_y_mirror)(
                        __API_CAFWM_INT_TYPE * __RESTRICT ptSourceBase,
                        int16_t iSourceStride,
                        arm_2d_size_t *__RESTRICT ptSourceSize,
                        
                        __API_CAFWM_INT_TYPE *__RESTRICT ptTargetBase,
                        int16_t iTargetStride,
                        arm_2d_size_t *__RESTRICT ptTargetSize,
                        
                        uint8_t *__RESTRICT pchTargetMaskBase,
                        int16_t iTargetMaskStride,
                        arm_2d_size_t *__RESTRICT ptTargetMaskSize)
{
    uint8_t *__RESTRICT pchTargetMaskLineBase = pchTargetMaskBase;  
    
    for (int_fast16_t iTargetY = 0, iTargetMaskY = 0; iTargetY < ptTargetSize->iHeight;) {
    
        //! reset source
        __API_CAFWM_INT_TYPE *__RESTRICT ptSource 
            = ptSourceBase + iSourceStride * (ptSourceSize->iHeight - 1);     
        
        for (int_fast16_t iSourceY = 0; iSourceY < ptSourceSize->iHeight; iSourceY++) {
            __API_CAFWM_INT_TYPE *__RESTRICT ptTarget = ptTargetBase;
            uint8_t *__RESTRICT pchTargetMask = pchTargetMaskLineBase;      
            
            /*---------------- Height Loop Begin----------------*/
            uint_fast32_t   wLengthLeft = ptTargetSize->iWidth;

            do {
                uint_fast32_t   wLength = MIN(wLengthLeft, ptSourceSize->iWidth);
                /*---------------- Width Loop Begin----------------*/

                __API_CAFWM_INT_TYPE *__RESTRICT ptSrc = ptSource;
                
                for (int_fast16_t x = 0; x < wLength; x++) {
                    uint16_t hwOpacity = 256 - (*pchTargetMask++);
                    __API_CAFWM_PIXEL_BLENDING(ptSrc++, ptTarget++, hwOpacity);
                }

                /*---------------- Width Loop End----------------*/
                wLengthLeft -= wLength;
            } while (wLengthLeft);
            
            /*---------------- Height Loop End----------------*/
            ptSource -= iSourceStride;
            ptTargetBase += iTargetStride;
            
            iTargetMaskY++;
            //! handle target mask 
            if (    (iTargetMaskY >= ptTargetMaskSize->iHeight)
               ||   (iTargetMaskY >= ptTargetSize->iHeight)) {
                pchTargetMaskLineBase = pchTargetMaskBase;
                iTargetMaskY = 0;
            } else {
                pchTargetMaskLineBase += iTargetMaskStride;
            }
            
            iTargetY++;
            if (iTargetY >= ptTargetSize->iHeight) {
                break;
            }
        }
    }

}

__WEAK
void __CAFWM_FUNC(des_msk_fill_xy_mirror)(
                        __API_CAFWM_INT_TYPE * __RESTRICT ptSourceBase,
                        int16_t iSourceStride,
                        arm_2d_size_t *__RESTRICT ptSourceSize,
                        
                        __API_CAFWM_INT_TYPE *__RESTRICT ptTargetBase,
                        int16_t iTargetStride,
                        arm_2d_size_t *__RESTRICT ptTargetSize,
                        
                        uint8_t *__RESTRICT pchTargetMaskBase,
                        int16_t iTargetMaskStride,
                        arm_2d_size_t *__RESTRICT ptTargetMaskSize)
{
    uint8_t *__RESTRICT pchTargetMaskLineBase = pchTargetMaskBase;  
    
    for (int_fast16_t iTargetY = 0, iTargetMaskY = 0; iTargetY < ptTargetSize->iHeight;) {
    
        //! reset source
        __API_CAFWM_INT_TYPE *__RESTRICT ptSource 
            = ptSourceBase + iSourceStride * (ptSourceSize->iHeight - 1);     
        
        for (int_fast16_t iSourceY = 0; iSourceY < ptSourceSize->iHeight; iSourceY++) {
            __API_CAFWM_INT_TYPE *__RESTRICT ptTarget = ptTargetBase;
            uint8_t *__RESTRICT pchTargetMask = pchTargetMaskLineBase;      
            
            /*---------------- Height Loop Begin----------------*/
            uint_fast32_t   wLengthLeft = ptTargetSize->iWidth;

            do {
                uint_fast32_t   wLength = MIN(wLengthLeft, ptSourceSize->iWidth);
                /*---------------- Width Loop Begin----------------*/

                __API_CAFWM_INT_TYPE *__RESTRICT ptSrc = ptSource;                
                ptSrc += ptSourceSize->iWidth - 1;

                
                for (int_fast16_t x = 0; x < wLength; x++) {
                    uint16_t hwOpacity = 256 - (*pchTargetMask++);
                    __API_CAFWM_PIXEL_BLENDING(ptSrc--, ptTarget++, hwOpacity);
                }

                /*---------------- Width Loop End----------------*/
                wLengthLeft -= wLength;
            } while (wLengthLeft);
            
            /*---------------- Height Loop End----------------*/
            ptSource -= iSourceStride;
            ptTargetBase += iTargetStride;
            
            iTargetMaskY++;
            //! handle target mask 
            if (    (iTargetMaskY >= ptTargetMaskSize->iHeight)
               ||   (iTargetMaskY >= ptTargetSize->iHeight)) {
                pchTargetMaskLineBase = pchTargetMaskBase;
                iTargetMaskY = 0;
            } else {
                pchTargetMaskLineBase += iTargetMaskStride;
            }
            
            iTargetY++;
            if (iTargetY >= ptTargetSize->iHeight) {
                break;
            }
        }
    }

}


__WEAK
void __CAFWM_FUNC(des_msk_fill_mirror)(
                        __API_CAFWM_INT_TYPE * __RESTRICT ptSourceBase,
                        int16_t iSourceStride,
                        arm_2d_size_t *__RESTRICT ptSourceSize,
                        
                        __API_CAFWM_INT_TYPE *__RESTRICT ptTargetBase,
                        int16_t iTargetStride,
                        arm_2d_size_t *__RESTRICT ptTargetSize,
                        
                        uint8_t *__RESTRICT pchTargetMaskBase,
                        int16_t iTargetMaskStride,
                        arm_2d_size_t *__RESTRICT ptTargetMaskSize,
                        uint32_t wMode)
{
    switch (wMode & (ARM_2D_CP_MODE_Y_MIRROR | ARM_2D_CP_MODE_X_MIRROR)) {
        case ARM_2D_CP_MODE_X_MIRROR:
            __CAFWM_FUNC(des_msk_fill_x_mirror)(
                    ptSourceBase, iSourceStride, ptSourceSize,
                    ptTargetBase, iTargetStride, ptTargetSize,
                    pchTargetMaskBase, iTargetMaskStride, ptTargetMaskSize);
            break;
        case ARM_2D_CP_MODE_Y_MIRROR:
            __CAFWM_FUNC(des_msk_fill_y_mirror)(
                    ptSourceBase, iSourceStride, ptSourceSize,
                    ptTargetBase, iTargetStride, ptTargetSize,
                    pchTargetMaskBase, iTargetMaskStride, ptTargetMaskSize);
            break;
        case ARM_2D_CP_MODE_Y_MIRROR | ARM_2D_CP_MODE_X_MIRROR :
            __CAFWM_FUNC(des_msk_fill_xy_mirror)(
                    ptSourceBase, iSourceStride, ptSourceSize,
                    ptTargetBase, iTargetStride, ptTargetSize,
                    pchTargetMaskBase, iTargetMaskStride, ptTargetMaskSize);
            break;
        default:
            assert(false);  /*! this should not happen */
            //break;
    }
}



/*----------------------------------------------------------------------------*
 * Fill with Mirroring (src mask only)                                        *
 *----------------------------------------------------------------------------*/

__WEAK
void __CAFWM_FUNC(src_msk_fill)(
                        __API_CAFWM_INT_TYPE * __RESTRICT ptSourceBase,
                        int16_t iSourceStride,
                        arm_2d_size_t *__RESTRICT ptSourceSize,
                        
                        uint8_t * __RESTRICT pchSourceMaskBase,
                        int16_t iSourceMaskStride,
                        arm_2d_size_t *__RESTRICT ptSourceMaskSize,
                        
                        __API_CAFWM_INT_TYPE *__RESTRICT ptTargetBase,
                        int16_t iTargetStride,
                        arm_2d_size_t *__RESTRICT ptTargetSize)
{
    
    for (int_fast16_t iTargetY = 0; iTargetY < ptTargetSize->iHeight;) {
    
        //! reset source
        __API_CAFWM_INT_TYPE *__RESTRICT ptSource = ptSourceBase;      
        uint8_t *pchSourceMask = pchSourceMaskBase;    
        int_fast16_t iSourceMaskY = 0;
        
        for (int_fast16_t iSourceY = 0; iSourceY < ptSourceSize->iHeight; iSourceY++) {
            __API_CAFWM_INT_TYPE *__RESTRICT ptTarget = ptTargetBase;     
            
            /*---------------- Height Loop Begin----------------*/
            uint_fast32_t   wLengthLeft = ptTargetSize->iWidth;

            do {
                uint_fast32_t   wLength = MIN(wLengthLeft, ptSourceSize->iWidth);
                /*---------------- Width Loop Begin----------------*/

                __API_CAFWM_INT_TYPE *__RESTRICT ptSrc = ptSource;
                uint8_t *__RESTRICT pchSrcMsk = pchSourceMask;
                for (int_fast16_t x = 0; x < wLength; x++) {
                    uint16_t hwOpacity = 256 - (*pchSrcMsk++);
                    __API_CAFWM_PIXEL_BLENDING(ptSrc++, ptTarget++, hwOpacity);
                }

                /*---------------- Width Loop End----------------*/
                wLengthLeft -= wLength;
            } while (wLengthLeft);
            
            /*---------------- Height Loop End----------------*/
            ptSource += iSourceStride;
            ptTargetBase += iTargetStride;
            
            iSourceMaskY++;
            //! handle source mask 
            if (    (iSourceMaskY >= ptSourceMaskSize->iHeight)
               ||   (iSourceMaskY >= ptSourceSize->iHeight)) {
                pchSourceMask = pchSourceMaskBase;
                iSourceMaskY = 0;
            } else {
                pchSourceMask += iSourceMaskStride;
            }
            
            iTargetY++;
            if (iTargetY >= ptTargetSize->iHeight) {
                break;
            }
        }
    }
}

__WEAK
void __CAFWM_FUNC(src_msk_fill_x_mirror)(
                        __API_CAFWM_INT_TYPE * __RESTRICT ptSourceBase,
                        int16_t iSourceStride,
                        arm_2d_size_t *__RESTRICT ptSourceSize,
                        
                        uint8_t * __RESTRICT pchSourceMaskBase,
                        int16_t iSourceMaskStride,
                        arm_2d_size_t *__RESTRICT ptSourceMaskSize,
                        
                        __API_CAFWM_INT_TYPE *__RESTRICT ptTargetBase,
                        int16_t iTargetStride,
                        arm_2d_size_t *__RESTRICT ptTargetSize)
{
    
    for (int_fast16_t iTargetY = 0; iTargetY < ptTargetSize->iHeight;) {
    
        //! reset source
        __API_CAFWM_INT_TYPE *__RESTRICT ptSource = ptSourceBase;      
        uint8_t *pchSourceMask = pchSourceMaskBase;    
        int_fast16_t iSourceMaskY = 0;
        
        for (int_fast16_t iSourceY = 0; iSourceY < ptSourceSize->iHeight; iSourceY++) {
            __API_CAFWM_INT_TYPE *__RESTRICT ptTarget = ptTargetBase;   
            
            /*---------------- Height Loop Begin----------------*/
            uint_fast32_t   wLengthLeft = ptTargetSize->iWidth;

            do {
                uint_fast32_t   wLength = MIN(wLengthLeft, ptSourceSize->iWidth);
                /*---------------- Width Loop Begin----------------*/

                __API_CAFWM_INT_TYPE *__RESTRICT ptSrc = ptSource;
                uint8_t *__RESTRICT pchSrcMsk = pchSourceMask;
                
                ptSrc += ptSourceSize->iWidth - 1;
                pchSrcMsk += ptSourceSize->iWidth - 1;
                
                for (int_fast16_t x = 0; x < wLength; x++) {
                    uint16_t hwOpacity = 256 - (*pchSrcMsk--);
                    __API_CAFWM_PIXEL_BLENDING(ptSrc--, ptTarget++, hwOpacity);
                }

                /*---------------- Width Loop End----------------*/
                wLengthLeft -= wLength;
            } while (wLengthLeft);
            
            /*---------------- Height Loop End----------------*/
            ptSource += iSourceStride;
            ptTargetBase += iTargetStride;
            
            iSourceMaskY++;
            //! handle source mask 
            if (    (iSourceMaskY >= ptSourceMaskSize->iHeight)
               ||   (iSourceMaskY >= ptSourceSize->iHeight)) {
                pchSourceMask = pchSourceMaskBase;
                iSourceMaskY = 0;
            } else {
                pchSourceMask += iSourceMaskStride;
            }
            
            iTargetY++;
            if (iTargetY >= ptTargetSize->iHeight) {
                break;
            }
        }
    }


}

__WEAK
void __CAFWM_FUNC(src_msk_fill_y_mirror)(
                        __API_CAFWM_INT_TYPE * __RESTRICT ptSourceBase,
                        int16_t iSourceStride,
                        arm_2d_size_t *__RESTRICT ptSourceSize,
                        
                        uint8_t * __RESTRICT pchSourceMaskBase,
                        int16_t iSourceMaskStride,
                        arm_2d_size_t *__RESTRICT ptSourceMaskSize,
                        
                        __API_CAFWM_INT_TYPE *__RESTRICT ptTargetBase,
                        int16_t iTargetStride,
                        arm_2d_size_t *__RESTRICT ptTargetSize)
{

    assert(ptSourceSize->iHeight <= ptSourceMaskSize->iHeight);
    pchSourceMaskBase += iSourceMaskStride * (ptSourceSize->iHeight - 1);
    
    for (int_fast16_t iTargetY = 0; iTargetY < ptTargetSize->iHeight;) {
    
        //! reset source
        __API_CAFWM_INT_TYPE *__RESTRICT ptSource 
            = ptSourceBase + iSourceStride * (ptSourceSize->iHeight - 1);     
        uint8_t *pchSourceMask = pchSourceMaskBase;    
        int_fast16_t iSourceMaskY = 0;
        
        for (int_fast16_t iSourceY = 0; iSourceY < ptSourceSize->iHeight; iSourceY++) {
            __API_CAFWM_INT_TYPE *__RESTRICT ptTarget = ptTargetBase;    
            
            /*---------------- Height Loop Begin----------------*/
            uint_fast32_t   wLengthLeft = ptTargetSize->iWidth;

            do {
                uint_fast32_t   wLength = MIN(wLengthLeft, ptSourceSize->iWidth);
                /*---------------- Width Loop Begin----------------*/

                __API_CAFWM_INT_TYPE *__RESTRICT ptSrc = ptSource;
                uint8_t *__RESTRICT pchSrcMsk = pchSourceMask;
                for (int_fast16_t x = 0; x < wLength; x++) {
                    uint16_t hwOpacity = 256 - (*pchSrcMsk++);
                    __API_CAFWM_PIXEL_BLENDING(ptSrc++, ptTarget++, hwOpacity);
                }

                /*---------------- Width Loop End----------------*/
                wLengthLeft -= wLength;
            } while (wLengthLeft);
            
            /*---------------- Height Loop End----------------*/
            ptSource -= iSourceStride;
            ptTargetBase += iTargetStride;
            
            iSourceMaskY++;
            //! handle source mask 
            if (    (iSourceMaskY >= ptSourceMaskSize->iHeight)
               ||   (iSourceMaskY >= ptSourceSize->iHeight)) {
                pchSourceMask = pchSourceMaskBase;
                iSourceMaskY = 0;
            } else {
                pchSourceMask -= iSourceMaskStride;
            }
            
            iTargetY++;
            if (iTargetY >= ptTargetSize->iHeight) {
                break;
            }
        }
    }

}

__WEAK
void __CAFWM_FUNC(src_msk_fill_xy_mirror)(
                        __API_CAFWM_INT_TYPE * __RESTRICT ptSourceBase,
                        int16_t iSourceStride,
                        arm_2d_size_t *__RESTRICT ptSourceSize,
                        
                        uint8_t * __RESTRICT pchSourceMaskBase,
                        int16_t iSourceMaskStride,
                        arm_2d_size_t *__RESTRICT ptSourceMaskSize,
                        
                        __API_CAFWM_INT_TYPE *__RESTRICT ptTargetBase,
                        int16_t iTargetStride,
                        arm_2d_size_t *__RESTRICT ptTargetSize)
{
    assert(ptSourceSize->iHeight <= ptSourceMaskSize->iHeight);
    pchSourceMaskBase += iSourceMaskStride * (ptSourceSize->iHeight - 1);
    
    for (int_fast16_t iTargetY = 0; iTargetY < ptTargetSize->iHeight;) {
    
        //! reset source
        __API_CAFWM_INT_TYPE *__RESTRICT ptSource 
            = ptSourceBase + iSourceStride * (ptSourceSize->iHeight - 1);     
        uint8_t *pchSourceMask = pchSourceMaskBase;    
        int_fast16_t iSourceMaskY = 0;
        
        for (int_fast16_t iSourceY = 0; iSourceY < ptSourceSize->iHeight; iSourceY++) {
            __API_CAFWM_INT_TYPE *__RESTRICT ptTarget = ptTargetBase;    
            
            /*---------------- Height Loop Begin----------------*/
            uint_fast32_t   wLengthLeft = ptTargetSize->iWidth;

            do {
                uint_fast32_t   wLength = MIN(wLengthLeft, ptSourceSize->iWidth);
                /*---------------- Width Loop Begin----------------*/

                __API_CAFWM_INT_TYPE *__RESTRICT ptSrc = ptSource;
                uint8_t *__RESTRICT pchSrcMsk = pchSourceMask;
                
                ptSrc += ptSourceSize->iWidth - 1;
                pchSrcMsk += ptSourceSize->iWidth - 1;
                
                
                for (int_fast16_t x = 0; x < wLength; x++) {
                    uint16_t hwOpacity = 256 - (*pchSrcMsk--);
                    __API_CAFWM_PIXEL_BLENDING(ptSrc--, ptTarget++, hwOpacity);
                }

                /*---------------- Width Loop End----------------*/
                wLengthLeft -= wLength;
            } while (wLengthLeft);
            
            /*---------------- Height Loop End----------------*/
            ptSource -= iSourceStride;
            ptTargetBase += iTargetStride;
            
            iSourceMaskY++;
            //! handle source mask 
            if (    (iSourceMaskY >= ptSourceMaskSize->iHeight)
               ||   (iSourceMaskY >= ptSourceSize->iHeight)) {
                pchSourceMask = pchSourceMaskBase;
                iSourceMaskY = 0;
            } else {
                pchSourceMask -= iSourceMaskStride;
            }
            
            iTargetY++;
            if (iTargetY >= ptTargetSize->iHeight) {
                break;
            }
        }
    }

}


__WEAK
void __CAFWM_FUNC(src_msk_fill_mirror)(
                        __API_CAFWM_INT_TYPE * __RESTRICT ptSourceBase,
                        int16_t iSourceStride,
                        arm_2d_size_t *__RESTRICT ptSourceSize,
                        
                        uint8_t * __RESTRICT pchSourceMaskBase,
                        int16_t iSourceMaskStride,
                        arm_2d_size_t *__RESTRICT ptSourceMaskSize,
                        
                        __API_CAFWM_INT_TYPE *__RESTRICT ptTargetBase,
                        int16_t iTargetStride,
                        arm_2d_size_t *__RESTRICT ptTargetSize,
                        uint32_t wMode)
{
    switch (wMode & (ARM_2D_CP_MODE_Y_MIRROR | ARM_2D_CP_MODE_X_MIRROR)) {
        case ARM_2D_CP_MODE_X_MIRROR:
            __CAFWM_FUNC(src_msk_fill_x_mirror)(
                    ptSourceBase, iSourceStride, ptSourceSize,
                    pchSourceMaskBase, iSourceMaskStride, ptSourceMaskSize,
                    ptTargetBase, iTargetStride, ptTargetSize);
            break;
        case ARM_2D_CP_MODE_Y_MIRROR:
            __CAFWM_FUNC(src_msk_fill_y_mirror)(
                    ptSourceBase, iSourceStride, ptSourceSize,
                    pchSourceMaskBase, iSourceMaskStride, ptSourceMaskSize,
                    ptTargetBase, iTargetStride, ptTargetSize);
            break;
        case ARM_2D_CP_MODE_Y_MIRROR | ARM_2D_CP_MODE_X_MIRROR :
            __CAFWM_FUNC(src_msk_fill_xy_mirror)(
                    ptSourceBase, iSourceStride, ptSourceSize,
                    pchSourceMaskBase, iSourceMaskStride, ptSourceMaskSize,
                    ptTargetBase, iTargetStride, ptTargetSize);
            break;
        default:
            assert(false);  /*! this should not happen */
            //break;
    }
}



/*----------------------------------------------------------------------------*
 * Copy with Mirroring  (both masks)                                          *
 *----------------------------------------------------------------------------*/

__WEAK
void __CAFWM_FUNC(masks_copy)(   
                            __API_CAFWM_INT_TYPE * __RESTRICT pSourceBase,
                            int16_t iSourceStride,
                            
                            uint8_t * __RESTRICT pchSourceMaskBase,
                            int16_t iSourceMaskStride,
                            arm_2d_size_t *__RESTRICT ptSourceMaskSize,
                            
                            __API_CAFWM_INT_TYPE * __RESTRICT pTargetBase,
                            int16_t iTargetStride,
                            
                            uint8_t * __RESTRICT pchTargetMaskBase,
                            int16_t iTargetMaskStride,
                            arm_2d_size_t *__RESTRICT ptTargetMaskSize,

                            arm_2d_size_t * __RESTRICT ptCopySize)
{
    int_fast16_t    iHeight = ptCopySize->iHeight;
    int_fast16_t    iWidth = ptCopySize->iWidth;
    //uint16_t        hwRatioCompl = 256 - chRatio;

    uint8_t *pchSourceMask = pchSourceMaskBase;
    uint8_t *pchTargetMask = pchTargetMaskBase;
    
    for (   int_fast16_t y = 0, iSourceMaskY = 0, iTargetMaskY = 0; 
            y < iHeight; 
            y++) {
    
        for (int_fast16_t x = 0; x < iWidth; x++) {
            uint16_t hwOpacity = 256 - ((*pchSourceMask++) * (*pchTargetMask++) >> 8);

            __API_CAFWM_PIXEL_BLENDING( pSourceBase++, pTargetBase++, hwOpacity);
            
        }
        pSourceBase += (iSourceStride - iWidth);
        pTargetBase += (iTargetStride - iWidth);
        
        //! handle source mask 
        iSourceMaskY++;
        if (    (iSourceMaskY >= ptSourceMaskSize->iHeight)
           ||   (iSourceMaskY >= iHeight)) {
            pchSourceMask = pchSourceMaskBase;
            iSourceMaskY = 0;
        } else {
            pchSourceMask += (iSourceMaskStride - iWidth);
        }
        
        //! handle target mask 
        iTargetMaskY++;
        if (    (iTargetMaskY >= ptTargetMaskSize->iHeight)
           ||   (iTargetMaskY >= iHeight)) {
            pchTargetMask = pchTargetMaskBase;
            iTargetMaskY = 0;
        } else {
            pchTargetMask += (iTargetMaskStride - iWidth);
        }
    }
}


__WEAK
void __CAFWM_FUNC(masks_copy_x_mirror)(  
                                    __API_CAFWM_INT_TYPE * __RESTRICT pSourceBase,
                                    int16_t iSourceStride,

                                    uint8_t * __RESTRICT pchSourceMaskBase,
                                    int16_t iSourceMaskStride,
                                    arm_2d_size_t *__RESTRICT ptSourceMaskSize,

                                    __API_CAFWM_INT_TYPE * __RESTRICT pTargetBase,
                                    int16_t iTargetStride,

                                    uint8_t * __RESTRICT pchTargetMaskBase,
                                    int16_t iTargetMaskStride,
                                    arm_2d_size_t *__RESTRICT ptTargetMaskSize,

                                    arm_2d_size_t * __RESTRICT ptCopySize)
{
    int_fast16_t    iHeight = ptCopySize->iHeight;
    int_fast16_t    iWidth = ptCopySize->iWidth;

    uint8_t *pchSourceMask = pchSourceMaskBase;
    uint8_t *pchTargetMask = pchTargetMaskBase;
    
    for (   int_fast16_t y = 0, iSourceMaskY = 0, iTargetMaskY = 0; 
            y < iHeight; 
            y++) {
    
        __API_CAFWM_INT_TYPE  *ptTargetCur = pTargetBase;
        __API_CAFWM_INT_TYPE  *ptSourceCur = pSourceBase;
        uint8_t *pchSourceMaskCur = pchSourceMask;

        ptSourceCur += ptCopySize->iWidth - 1;
        //! \note do not use ptSourceMaskSize->iWidth
        pchSourceMaskCur += ptCopySize->iWidth - 1; 
        
        for (int_fast16_t x = 0; x < iWidth; x++) {
            uint16_t hwOpacity = 256 - ((*pchSourceMaskCur--) * (*pchTargetMask++) >> 8);
            __API_CAFWM_PIXEL_BLENDING( ptSourceCur--, pTargetBase++, hwOpacity);
            
        }
        pSourceBase += iSourceStride;
        pTargetBase += (iTargetStride - iWidth);
        
        //! handle source mask 
        iSourceMaskY++;
        if (    (iSourceMaskY >= ptSourceMaskSize->iHeight)
           ||   (iSourceMaskY >= iHeight)) {
            pchSourceMask = pchSourceMaskBase;
            iSourceMaskY = 0;
        } else {
            pchSourceMask += iSourceMaskStride;
        }
        
        //! handle target mask 
        iTargetMaskY++;
        if (    (iTargetMaskY >= ptTargetMaskSize->iHeight)
           ||   (iTargetMaskY >= iHeight)) {
            pchTargetMask = pchTargetMaskBase;
            iTargetMaskY = 0;
        } else {
            pchTargetMask += (iTargetMaskStride - iWidth);
        }
    }
}


__WEAK
void __CAFWM_FUNC(masks_copy_y_mirror)(  
                                    __API_CAFWM_INT_TYPE * __RESTRICT pSourceBase,
                                    int16_t iSourceStride,

                                    uint8_t * __RESTRICT pchSourceMaskBase,
                                    int16_t iSourceMaskStride,
                                    arm_2d_size_t *__RESTRICT ptSourceMaskSize,

                                    __API_CAFWM_INT_TYPE * __RESTRICT pTargetBase,
                                    int16_t iTargetStride,

                                    uint8_t * __RESTRICT pchTargetMaskBase,
                                    int16_t iTargetMaskStride,
                                    arm_2d_size_t *__RESTRICT ptTargetMaskSize,

                                    arm_2d_size_t * __RESTRICT ptCopySize)
{
    int_fast16_t    iHeight = ptCopySize->iHeight;
    int_fast16_t    iWidth = ptCopySize->iWidth;
    //uint16_t        hwRatioCompl = 256 - chRatio;

    pSourceBase += iSourceStride * (ptCopySize->iHeight - 1);
    
    assert (ptCopySize->iHeight <= ptSourceMaskSize->iHeight);
    pchSourceMaskBase += iSourceMaskStride * (ptCopySize->iHeight - 1);
    

    uint8_t *pchSourceMask = pchSourceMaskBase;
    uint8_t *pchTargetMask = pchTargetMaskBase;
    
    for (   int_fast16_t y = 0, iSourceMaskY = 0, iTargetMaskY = 0; 
            y < iHeight; 
            y++) {
    
        __API_CAFWM_INT_TYPE  *ptSourceCur = pSourceBase;
        uint8_t *pchSourceMaskCur = pchSourceMask;
        
        for (int_fast16_t x = 0; x < iWidth; x++) {
            uint16_t hwOpacity = 256 - ((*pchSourceMaskCur++) * (*pchTargetMask++) >> 8);
            

            __API_CAFWM_PIXEL_BLENDING( ptSourceCur++, pTargetBase++, hwOpacity);
            
        }
        pSourceBase -= iSourceStride;
        pTargetBase += (iTargetStride - iWidth);
        
        //! handle source mask 
        iSourceMaskY++;
        if (    (iSourceMaskY >= ptSourceMaskSize->iHeight)
           ||   (iSourceMaskY >= iHeight)) {
            pchSourceMask = pchSourceMaskBase;
            iSourceMaskY = 0;
        } else {
            pchSourceMask -= iSourceMaskStride;
        }
        
        //! handle target mask 
        iTargetMaskY++;
        if (    (iTargetMaskY >= ptTargetMaskSize->iHeight)
           ||   (iTargetMaskY >= iHeight)) {
            pchTargetMask = pchTargetMaskBase;
            iTargetMaskY = 0;
        } else {
            pchTargetMask += (iTargetMaskStride - iWidth);
        }
    }

}


__WEAK
void __CAFWM_FUNC(masks_copy_xy_mirror)(  
                                    __API_CAFWM_INT_TYPE * __RESTRICT pSourceBase,
                                    int16_t iSourceStride,

                                    uint8_t * __RESTRICT pchSourceMaskBase,
                                    int16_t iSourceMaskStride,
                                    arm_2d_size_t *__RESTRICT ptSourceMaskSize,

                                    __API_CAFWM_INT_TYPE * __RESTRICT pTargetBase,
                                    int16_t iTargetStride,

                                    uint8_t * __RESTRICT pchTargetMaskBase,
                                    int16_t iTargetMaskStride,
                                    arm_2d_size_t *__RESTRICT ptTargetMaskSize,

                                    arm_2d_size_t * __RESTRICT ptCopySize)
{
    int_fast16_t    iHeight = ptCopySize->iHeight;
    int_fast16_t    iWidth = ptCopySize->iWidth;
    //uint16_t        hwRatioCompl = 256 - chRatio;

    pSourceBase += iSourceStride * (ptCopySize->iHeight - 1);
    
    assert (ptCopySize->iHeight <= ptSourceMaskSize->iHeight);
    pchSourceMaskBase += iSourceMaskStride * (ptCopySize->iHeight - 1);
    
    

    uint8_t *pchSourceMask = pchSourceMaskBase;
    uint8_t *pchTargetMask = pchTargetMaskBase;
    
    for (   int_fast16_t y = 0, iSourceMaskY = 0, iTargetMaskY = 0; 
            y < iHeight; 
            y++) {
    
        __API_CAFWM_INT_TYPE  *ptSourceCur = pSourceBase;
        uint8_t *pchSourceMaskCur = pchSourceMask;
        
        ptSourceCur += ptCopySize->iWidth - 1;
        //! \note do not use ptSourceMaskSize->iWidth
        pchSourceMaskCur += ptCopySize->iWidth - 1; 
        
        for (int_fast16_t x = 0; x < iWidth; x++) {
            uint16_t hwOpacity = 256 
                               - (  (*pchSourceMaskCur--) 
                                 *  (*pchTargetMask++) 
                                 >> 8);

            __API_CAFWM_PIXEL_BLENDING( ptSourceCur--, pTargetBase++, hwOpacity);
            
        }
        pSourceBase -= iSourceStride;
        pTargetBase += (iTargetStride - iWidth);
        
        //! handle source mask 
        iSourceMaskY++;
        if (    (iSourceMaskY >= ptSourceMaskSize->iHeight)
           ||   (iSourceMaskY >= iHeight)) {
            pchSourceMask = pchSourceMaskBase;
            iSourceMaskY = 0;
        } else {
            pchSourceMask -= iSourceMaskStride;
        }
        
        //! handle target mask 
        iTargetMaskY++;
        if (    (iTargetMaskY >= ptTargetMaskSize->iHeight)
           ||   (iTargetMaskY >= iHeight)) {
            pchTargetMask = pchTargetMaskBase;
            iTargetMaskY = 0;
        } else {
            pchTargetMask += (iTargetMaskStride - iWidth);
        }
    }

}

__WEAK
void __CAFWM_FUNC(masks_copy_mirror)(  
                                    __API_CAFWM_INT_TYPE * __RESTRICT pSourceBase,
                                    int16_t iSourceStride,

                                    uint8_t * __RESTRICT pchSourceMaskBase,
                                    int16_t iSourceMaskStride,
                                    arm_2d_size_t *__RESTRICT ptSourceMaskSize,

                                    __API_CAFWM_INT_TYPE * __RESTRICT pTargetBase,
                                    int16_t iTargetStride,

                                    uint8_t * __RESTRICT pchTargetMaskBase,
                                    int16_t iTargetMaskStride,
                                    arm_2d_size_t *__RESTRICT ptTargetMaskSize,

                                    arm_2d_size_t * __RESTRICT ptCopySize,
                                    uint32_t wMode)
{
    switch (wMode & (ARM_2D_CP_MODE_Y_MIRROR | ARM_2D_CP_MODE_X_MIRROR)) {
      case ARM_2D_CP_MODE_X_MIRROR:
          __CAFWM_FUNC(masks_copy_x_mirror)(pSourceBase, iSourceStride,
                                            pchSourceMaskBase, iSourceMaskStride,
                                            ptSourceMaskSize,
                                            pTargetBase, iTargetStride, 
                                            pchTargetMaskBase, iTargetMaskStride,
                                            ptTargetMaskSize,
                                            ptCopySize);
          break;
      case ARM_2D_CP_MODE_Y_MIRROR:
          __CAFWM_FUNC(masks_copy_y_mirror)(pSourceBase, iSourceStride,
                                            pchSourceMaskBase, iSourceMaskStride,
                                            ptSourceMaskSize,
                                            pTargetBase, iTargetStride, 
                                            pchTargetMaskBase, iTargetMaskStride,
                                            ptTargetMaskSize,
                                            ptCopySize);
          break;
      case ARM_2D_CP_MODE_Y_MIRROR | ARM_2D_CP_MODE_X_MIRROR:
          __CAFWM_FUNC(masks_copy_xy_mirror)(pSourceBase, iSourceStride,
                                            pchSourceMaskBase, iSourceMaskStride,
                                            ptSourceMaskSize,
                                            pTargetBase, iTargetStride, 
                                            pchTargetMaskBase, iTargetMaskStride,
                                            ptTargetMaskSize,
                                            ptCopySize);
          break;
      default:
          assert(false);        /*! this should not happen */
          //break;
    }
}

/*----------------------------------------------------------------------------*
 * Copy with Mirroring  (target mask only)                                    *
 *----------------------------------------------------------------------------*/

__WEAK
void __CAFWM_FUNC(des_msk_copy)(   
                            __API_CAFWM_INT_TYPE * __RESTRICT pSourceBase,
                            int16_t iSourceStride,
                            
                            __API_CAFWM_INT_TYPE * __RESTRICT pTargetBase,
                            int16_t iTargetStride,
                            
                            uint8_t * __RESTRICT pchTargetMaskBase,
                            int16_t iTargetMaskStride,
                            arm_2d_size_t *__RESTRICT ptTargetMaskSize,

                            arm_2d_size_t * __RESTRICT ptCopySize)
{
    int_fast16_t    iHeight = ptCopySize->iHeight;
    int_fast16_t    iWidth = ptCopySize->iWidth;
    //uint16_t        hwRatioCompl = 256 - chRatio;

    uint8_t *pchTargetMask = pchTargetMaskBase;
    
    for (   int_fast16_t y = 0,iTargetMaskY = 0; 
            y < iHeight; 
            y++) {
    
        for (int_fast16_t x = 0; x < iWidth; x++) {
            uint16_t hwOpacity = 256 - (*pchTargetMask++);

            __API_CAFWM_PIXEL_BLENDING( pSourceBase++, pTargetBase++, hwOpacity);
            
        }
        pSourceBase += (iSourceStride - iWidth);
        pTargetBase += (iTargetStride - iWidth);
        
        //! handle target mask 
        iTargetMaskY++;
        if (    (iTargetMaskY >= ptTargetMaskSize->iHeight)
           ||   (iTargetMaskY >= iHeight)) {
            pchTargetMask = pchTargetMaskBase;
            iTargetMaskY = 0;
        } else {
            pchTargetMask += (iTargetMaskStride - iWidth);
        }
    }
}


__WEAK
void __CAFWM_FUNC(des_msk_copy_x_mirror)(  
                                    __API_CAFWM_INT_TYPE * __RESTRICT pSourceBase,
                                    int16_t iSourceStride,

                                    __API_CAFWM_INT_TYPE * __RESTRICT pTargetBase,
                                    int16_t iTargetStride,

                                    uint8_t * __RESTRICT pchTargetMaskBase,
                                    int16_t iTargetMaskStride,
                                    arm_2d_size_t *__RESTRICT ptTargetMaskSize,

                                    arm_2d_size_t * __RESTRICT ptCopySize)
{
    int_fast16_t    iHeight = ptCopySize->iHeight;
    int_fast16_t    iWidth = ptCopySize->iWidth;

    uint8_t *pchTargetMask = pchTargetMaskBase;
    
    for (   int_fast16_t y = 0, iTargetMaskY = 0; 
            y < iHeight; 
            y++) {
    
        __API_CAFWM_INT_TYPE  *ptTargetCur = pTargetBase;
        __API_CAFWM_INT_TYPE  *ptSourceCur = pSourceBase;

        ptSourceCur += ptCopySize->iWidth - 1;
        
        for (int_fast16_t x = 0; x < iWidth; x++) {
            uint16_t hwOpacity = 256 - (*pchTargetMask++);
            __API_CAFWM_PIXEL_BLENDING( ptSourceCur--, pTargetBase++, hwOpacity);
            
        }
        pSourceBase += iSourceStride;
        pTargetBase += (iTargetStride - iWidth);
        
        //! handle target mask 
        iTargetMaskY++;
        if (    (iTargetMaskY >= ptTargetMaskSize->iHeight)
           ||   (iTargetMaskY >= iHeight)) {
            pchTargetMask = pchTargetMaskBase;
            iTargetMaskY = 0;
        } else {
            pchTargetMask += (iTargetMaskStride - iWidth);
        }
    }
}


__WEAK
void __CAFWM_FUNC(des_msk_copy_y_mirror)(  
                                    __API_CAFWM_INT_TYPE * __RESTRICT pSourceBase,
                                    int16_t iSourceStride,

                                    __API_CAFWM_INT_TYPE * __RESTRICT pTargetBase,
                                    int16_t iTargetStride,

                                    uint8_t * __RESTRICT pchTargetMaskBase,
                                    int16_t iTargetMaskStride,
                                    arm_2d_size_t *__RESTRICT ptTargetMaskSize,

                                    arm_2d_size_t * __RESTRICT ptCopySize)
{
    int_fast16_t    iHeight = ptCopySize->iHeight;
    int_fast16_t    iWidth = ptCopySize->iWidth;

    pSourceBase += iSourceStride * (ptCopySize->iHeight - 1);
    uint8_t *pchTargetMask = pchTargetMaskBase;
    
    for (   int_fast16_t y = 0, iTargetMaskY = 0; y < iHeight; y++) {
    
        __API_CAFWM_INT_TYPE  *ptSourceCur = pSourceBase;

        for (int_fast16_t x = 0; x < iWidth; x++) {
            uint16_t hwOpacity = 256 - (*pchTargetMask++);
            __API_CAFWM_PIXEL_BLENDING( ptSourceCur++, pTargetBase++, hwOpacity);
            
        }
        pSourceBase -= iSourceStride;
        pTargetBase += (iTargetStride - iWidth);
        
        //! handle target mask 
        iTargetMaskY++;
        if (    (iTargetMaskY >= ptTargetMaskSize->iHeight)
           ||   (iTargetMaskY >= iHeight)) {
            pchTargetMask = pchTargetMaskBase;
            iTargetMaskY = 0;
        } else {
            pchTargetMask += (iTargetMaskStride - iWidth);
        }
    }

}


__WEAK
void __CAFWM_FUNC(des_msk_copy_xy_mirror)(  
                                    __API_CAFWM_INT_TYPE * __RESTRICT pSourceBase,
                                    int16_t iSourceStride,

                                    __API_CAFWM_INT_TYPE * __RESTRICT pTargetBase,
                                    int16_t iTargetStride,

                                    uint8_t * __RESTRICT pchTargetMaskBase,
                                    int16_t iTargetMaskStride,
                                    arm_2d_size_t *__RESTRICT ptTargetMaskSize,

                                    arm_2d_size_t * __RESTRICT ptCopySize)
{
    int_fast16_t    iHeight = ptCopySize->iHeight;
    int_fast16_t    iWidth = ptCopySize->iWidth;

    pSourceBase += iSourceStride * (ptCopySize->iHeight - 1);
    uint8_t *pchTargetMask = pchTargetMaskBase;
    
    for (   int_fast16_t y = 0, iTargetMaskY = 0; y < iHeight; y++) {
    
        __API_CAFWM_INT_TYPE  *ptSourceCur = pSourceBase;
        
        ptSourceCur += ptCopySize->iWidth - 1;
        
        for (int_fast16_t x = 0; x < iWidth; x++) {
            uint16_t hwOpacity = 256 - (*pchTargetMask++);

            __API_CAFWM_PIXEL_BLENDING( ptSourceCur--, pTargetBase++, hwOpacity);
            
        }
        pSourceBase -= iSourceStride;
        pTargetBase += (iTargetStride - iWidth);
        
        //! handle target mask 
        iTargetMaskY++;
        if (    (iTargetMaskY >= ptTargetMaskSize->iHeight)
           ||   (iTargetMaskY >= iHeight)) {
            pchTargetMask = pchTargetMaskBase;
            iTargetMaskY = 0;
        } else {
            pchTargetMask += (iTargetMaskStride - iWidth);
        }
    }

}

__WEAK
void __CAFWM_FUNC(des_msk_copy_mirror)(  
                                    __API_CAFWM_INT_TYPE * __RESTRICT pSourceBase,
                                    int16_t iSourceStride,

                                    __API_CAFWM_INT_TYPE * __RESTRICT pTargetBase,
                                    int16_t iTargetStride,

                                    uint8_t * __RESTRICT pchTargetMaskBase,
                                    int16_t iTargetMaskStride,
                                    arm_2d_size_t *__RESTRICT ptTargetMaskSize,

                                    arm_2d_size_t * __RESTRICT ptCopySize,
                                    uint32_t wMode)
{
    switch (wMode & (ARM_2D_CP_MODE_Y_MIRROR | ARM_2D_CP_MODE_X_MIRROR)) {
      case ARM_2D_CP_MODE_X_MIRROR:
          __CAFWM_FUNC(des_msk_copy_x_mirror)(pSourceBase, iSourceStride,
                                            pTargetBase, iTargetStride, 
                                            pchTargetMaskBase, iTargetMaskStride,
                                            ptTargetMaskSize,
                                            ptCopySize);
          break;
      case ARM_2D_CP_MODE_Y_MIRROR:
          __CAFWM_FUNC(des_msk_copy_y_mirror)(pSourceBase, iSourceStride,
                                            pTargetBase, iTargetStride, 
                                            pchTargetMaskBase, iTargetMaskStride,
                                            ptTargetMaskSize,
                                            ptCopySize);
          break;
      case ARM_2D_CP_MODE_Y_MIRROR | ARM_2D_CP_MODE_X_MIRROR:
          __CAFWM_FUNC(des_msk_copy_xy_mirror)(pSourceBase, iSourceStride,
                                            pTargetBase, iTargetStride, 
                                            pchTargetMaskBase, iTargetMaskStride,
                                            ptTargetMaskSize,
                                            ptCopySize);
          break;
      default:
          assert(false);        /*! this should not happen */
          //break;
    }
}



/*----------------------------------------------------------------------------*
 * Copy with Mirroring  (src mask only)                                       *
 *----------------------------------------------------------------------------*/

__WEAK
void __CAFWM_FUNC(src_msk_copy)(   
                            __API_CAFWM_INT_TYPE * __RESTRICT pSourceBase,
                            int16_t iSourceStride,
                            
                            uint8_t * __RESTRICT pchSourceMaskBase,
                            int16_t iSourceMaskStride,
                            arm_2d_size_t *__RESTRICT ptSourceMaskSize,
                            
                            __API_CAFWM_INT_TYPE * __RESTRICT pTargetBase,
                            int16_t iTargetStride,

                            arm_2d_size_t * __RESTRICT ptCopySize)
{
    int_fast16_t    iHeight = ptCopySize->iHeight;
    int_fast16_t    iWidth = ptCopySize->iWidth;

    uint8_t *pchSourceMask = pchSourceMaskBase;
    
    for (   int_fast16_t y = 0, iSourceMaskY = 0;
            y < iHeight; 
            y++) {
    
        for (int_fast16_t x = 0; x < iWidth; x++) {
            uint16_t hwOpacity = 256 - (*pchSourceMask++);

            __API_CAFWM_PIXEL_BLENDING( pSourceBase++, pTargetBase++, hwOpacity);
            
        }
        pSourceBase += (iSourceStride - iWidth);
        pTargetBase += (iTargetStride - iWidth);
        
        //! handle source mask 
        iSourceMaskY++;
        if (    (iSourceMaskY >= ptSourceMaskSize->iHeight)
           ||   (iSourceMaskY >= iHeight)) {
            pchSourceMask = pchSourceMaskBase;
            iSourceMaskY = 0;
        } else {
            pchSourceMask += (iSourceMaskStride - iWidth);
        }
    }
}


__WEAK
void __CAFWM_FUNC(src_msk_copy_x_mirror)(  
                                    __API_CAFWM_INT_TYPE * __RESTRICT pSourceBase,
                                    int16_t iSourceStride,

                                    uint8_t * __RESTRICT pchSourceMaskBase,
                                    int16_t iSourceMaskStride,
                                    arm_2d_size_t *__RESTRICT ptSourceMaskSize,

                                    __API_CAFWM_INT_TYPE * __RESTRICT pTargetBase,
                                    int16_t iTargetStride,

                                    arm_2d_size_t * __RESTRICT ptCopySize)
{
    int_fast16_t    iHeight = ptCopySize->iHeight;
    int_fast16_t    iWidth = ptCopySize->iWidth;

    uint8_t *pchSourceMask = pchSourceMaskBase;
    
    for (   int_fast16_t y = 0, iSourceMaskY = 0; 
            y < iHeight; 
            y++) {
    
        __API_CAFWM_INT_TYPE  *ptTargetCur = pTargetBase;
        __API_CAFWM_INT_TYPE  *ptSourceCur = pSourceBase;
        uint8_t *pchSourceMaskCur = pchSourceMask;

        ptSourceCur += ptCopySize->iWidth - 1;
        //! \note do not use ptSourceMaskSize->iWidth
        pchSourceMaskCur += ptCopySize->iWidth - 1; 
        
        for (int_fast16_t x = 0; x < iWidth; x++) {
            uint16_t hwOpacity = 256 - (*pchSourceMaskCur--);
            __API_CAFWM_PIXEL_BLENDING( ptSourceCur--, pTargetBase++, hwOpacity);
            
        }
        pSourceBase += iSourceStride;
        pTargetBase += (iTargetStride - iWidth);
        
        //! handle source mask 
        iSourceMaskY++;
        if (    (iSourceMaskY >= ptSourceMaskSize->iHeight)
           ||   (iSourceMaskY >= iHeight)) {
            pchSourceMask = pchSourceMaskBase;
            iSourceMaskY = 0;
        } else {
            pchSourceMask += iSourceMaskStride;
        }
    }
}


__WEAK
void __CAFWM_FUNC(src_msk_copy_y_mirror)(  
                                    __API_CAFWM_INT_TYPE * __RESTRICT pSourceBase,
                                    int16_t iSourceStride,

                                    uint8_t * __RESTRICT pchSourceMaskBase,
                                    int16_t iSourceMaskStride,
                                    arm_2d_size_t *__RESTRICT ptSourceMaskSize,

                                    __API_CAFWM_INT_TYPE * __RESTRICT pTargetBase,
                                    int16_t iTargetStride,

                                    arm_2d_size_t * __RESTRICT ptCopySize)
{
    int_fast16_t    iHeight = ptCopySize->iHeight;
    int_fast16_t    iWidth = ptCopySize->iWidth;

    pSourceBase += iSourceStride * (ptCopySize->iHeight - 1);
    
    assert (ptCopySize->iHeight <= ptSourceMaskSize->iHeight);
    pchSourceMaskBase += iSourceMaskStride * (ptCopySize->iHeight - 1);
    

    uint8_t *pchSourceMask = pchSourceMaskBase;
    
    for (   int_fast16_t y = 0, iSourceMaskY = 0; 
            y < iHeight; 
            y++) {
    
        __API_CAFWM_INT_TYPE  *ptSourceCur = pSourceBase;
        uint8_t *pchSourceMaskCur = pchSourceMask;
        
        for (int_fast16_t x = 0; x < iWidth; x++) {
            uint16_t hwOpacity = 256 - (*pchSourceMaskCur++);
            __API_CAFWM_PIXEL_BLENDING( ptSourceCur++, pTargetBase++, hwOpacity);
        }
        
        pSourceBase -= iSourceStride;
        pTargetBase += (iTargetStride - iWidth);
        
        //! handle source mask 
        iSourceMaskY++;
        if (    (iSourceMaskY >= ptSourceMaskSize->iHeight)
           ||   (iSourceMaskY >= iHeight)) {
            pchSourceMask = pchSourceMaskBase;
            iSourceMaskY = 0;
        } else {
            pchSourceMask -= iSourceMaskStride;
        }
    }

}


__WEAK
void __CAFWM_FUNC(src_msk_copy_xy_mirror)(  
                                    __API_CAFWM_INT_TYPE * __RESTRICT pSourceBase,
                                    int16_t iSourceStride,

                                    uint8_t * __RESTRICT pchSourceMaskBase,
                                    int16_t iSourceMaskStride,
                                    arm_2d_size_t *__RESTRICT ptSourceMaskSize,

                                    __API_CAFWM_INT_TYPE * __RESTRICT pTargetBase,
                                    int16_t iTargetStride,

                                    arm_2d_size_t * __RESTRICT ptCopySize)
{
    int_fast16_t    iHeight = ptCopySize->iHeight;
    int_fast16_t    iWidth = ptCopySize->iWidth;

    pSourceBase += iSourceStride * (ptCopySize->iHeight - 1);
    
    assert (ptCopySize->iHeight <= ptSourceMaskSize->iHeight);
    pchSourceMaskBase += iSourceMaskStride * (ptCopySize->iHeight - 1);
    

    uint8_t *pchSourceMask = pchSourceMaskBase;
    
    for (   int_fast16_t y = 0, iSourceMaskY = 0; 
            y < iHeight; 
            y++) {
    
        __API_CAFWM_INT_TYPE  *ptSourceCur = pSourceBase;
        uint8_t *pchSourceMaskCur = pchSourceMask;
        
        ptSourceCur += ptCopySize->iWidth - 1;
        //! \note do not use ptSourceMaskSize->iWidth
        pchSourceMaskCur += ptCopySize->iWidth - 1; 
        
        for (int_fast16_t x = 0; x < iWidth; x++) {
            uint16_t hwOpacity = 256 - (*pchSourceMaskCur--);

            __API_CAFWM_PIXEL_BLENDING( ptSourceCur--, pTargetBase++, hwOpacity);
            
        }
        pSourceBase -= iSourceStride;
        pTargetBase += (iTargetStride - iWidth);
        
        //! handle source mask 
        iSourceMaskY++;
        if (    (iSourceMaskY >= ptSourceMaskSize->iHeight)
           ||   (iSourceMaskY >= iHeight)) {
            pchSourceMask = pchSourceMaskBase;
            iSourceMaskY = 0;
        } else {
            pchSourceMask -= iSourceMaskStride;
        }
    }

}

__WEAK
void __CAFWM_FUNC(src_msk_copy_mirror)(  
                                    __API_CAFWM_INT_TYPE * __RESTRICT pSourceBase,
                                    int16_t iSourceStride,

                                    uint8_t * __RESTRICT pchSourceMaskBase,
                                    int16_t iSourceMaskStride,
                                    arm_2d_size_t *__RESTRICT ptSourceMaskSize,

                                    __API_CAFWM_INT_TYPE * __RESTRICT pTargetBase,
                                    int16_t iTargetStride,

                                    arm_2d_size_t * __RESTRICT ptCopySize,
                                    uint32_t wMode)
{
    switch (wMode & (ARM_2D_CP_MODE_Y_MIRROR | ARM_2D_CP_MODE_X_MIRROR)) {
      case ARM_2D_CP_MODE_X_MIRROR:
          __CAFWM_FUNC(src_msk_copy_x_mirror)(pSourceBase, iSourceStride,
                                            pchSourceMaskBase, iSourceMaskStride,
                                            ptSourceMaskSize,
                                            pTargetBase, iTargetStride, 
                                            ptCopySize);
          break;
      case ARM_2D_CP_MODE_Y_MIRROR:
          __CAFWM_FUNC(src_msk_copy_y_mirror)(pSourceBase, iSourceStride,
                                            pchSourceMaskBase, iSourceMaskStride,
                                            ptSourceMaskSize,
                                            pTargetBase, iTargetStride, 
                                            ptCopySize);
          break;
      case ARM_2D_CP_MODE_Y_MIRROR | ARM_2D_CP_MODE_X_MIRROR:
          __CAFWM_FUNC(src_msk_copy_xy_mirror)(pSourceBase, iSourceStride,
                                            pchSourceMaskBase, iSourceMaskStride,
                                            ptSourceMaskSize,
                                            pTargetBase, iTargetStride, 
                                            ptCopySize);
          break;
      default:
          assert(false);        /*! this should not happen */
          //break;
    }
}


#undef __API_CAFWM_COPY_LIKE_OP_NAME
#undef __API_CAFWM_OP_NAME
#undef __API_CAFWM_PIXEL_BLENDING
#undef ____CAFWM_FUNC
#undef ___CAFWM_FUNC
#undef __CAFWM_FUNC
#undef __API_CAFWM_COLOUR
#undef __API_CAFWM_INT_TYPE
#undef __API_CAFWM_INT_TYPE_BIT_NUM
#undef ____CAFWM_TYPE
#undef ___CAFWM_TYPE
#undef __CAFWM_TYPE


