/*
 * Copyright (C) 2010-2022 Arm Limited or its affiliates. All rights reserved.
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
 * Title:        __arm_2d_meta_trans_with_masks_helium.inc
 * Description:  c code template for copy and fill like operations
 *
 * $Date:        20. May 2022
 * $Revision:    V.1.0.0
 *
 * -------------------------------------------------------------------- */

#ifndef __API_MTWM_COLOUR
#   error You have to define __API_MTWM_COLOUR before using this c template
#endif
#ifndef __API_MTWM_INT_TYPE
#   error You have to define the __API_MTWM_INT_TYPE before using this c template
#endif
#ifndef __API_MTWM_INT_TYPE_BIT_NUM
#   error You have to define the __API_MTWM_INT_TYPE_BIT_NUM before using this c template
#endif
#ifndef __API_MTWM_PIXEL_BLENDING
#   error You have to define __API_MTWM_PIXEL_BLENDING before using this c template
#endif
#ifndef __API_MTWM_PIXEL_AVERAGE
#   error You have to define __API_MTWM_PIXEL_AVERAGE before using this c template
#endif
#ifndef __API_MTWM_PIXEL_AVERAGE_RESULT
#   error You have to define __API_MTWM_PIXEL_AVERAGE_RESULT before using this c template
#endif
#ifndef __API_MTWM_PIXEL_AVERAGE_INIT
#   define __API_MTWM_PIXEL_AVERAGE_INIT()   __arm_2d_color_fast_rgb_t tPixel = {0};
#endif


/*! disable this feature by default */
#ifndef __API_MTWM_CFG_SUPPORT_SRC_MSK_WRAPING
#   define __API_MTWM_CFG_SUPPORT_SRC_MSK_WRAPING               0
#endif

//#ifndef __API_MTWM_CFG_1_HORIZONTAL_LINE
//#   define __API_MTWM_CFG_1_HORIZONTAL_LINE                     0
//#endif

//#ifndef __API_MTWM_CFG_CHANNEL_8in32_SUPPORT
//#   define __API_MTWM_CFG_CHANNEL_8in32_SUPPORT                 0
//#endif

#ifndef __API_MTWM_CFG_CHANNEL_8in32_SUPPORT_ON_SOURCE_SIDE
#   define __API_MTWM_CFG_CHANNEL_8in32_SUPPORT_ON_SOURCE_SIDE  0
#endif

//#ifndef __API_MTWM_CFG_CHANNEL_8in32_SUPPORT_ON_TARGET_SIDE
//#   define __API_MTWM_CFG_CHANNEL_8in32_SUPPORT_ON_TARGET_SIDE  0
//#endif

#ifndef __API_MTWM_CFG_SUPPORT_SOURCE_MASK
#   define __API_MTWM_CFG_SUPPORT_SOURCE_MASK                   0
#endif

//#ifndef __API_MTWM_CFG_SUPPORT_TARGET_MASK
//#   define __API_MTWM_CFG_SUPPORT_TARGET_MASK                   0
//#endif

#ifndef __API_MTWM_CFG_SUPPORT_OPATICY
#   define __API_MTWM_CFG_SUPPORT_OPATICY                       0
#endif

#undef ____MTWM_FUNC
#undef ___MTWM_FUNC
#undef __MTWM_FUNC



#ifndef __API_MTWM_OP_NAME
#   define ____MTWM_FUNC(__NAME, __COLOUR)                                      \
        __arm_2d_impl_##__COLOUR##_##__NAME
#   define ___MTWM_FUNC(__NAME, __COLOUR)   ____MTWM_FUNC(__NAME, __COLOUR)
#else
#   define _____MTWM_FUNC(__OP_NAME, __NAME, __COLOUR)                          \
        __arm_2d_impl_##__COLOUR##_##__OP_NAME##_##__NAME
#   define ____MTWM_FUNC(__OP_NAME, __NAME, __COLOUR)                           \
        _____MTWM_FUNC(__OP_NAME, __NAME, __COLOUR)
#   define ___MTWM_FUNC(__NAME, __COLOUR)                                       \
        ____MTWM_FUNC(__API_MTWM_OP_NAME, __NAME, __COLOUR)
#endif

#define __MTWM_FUNC(__NAME)   ___MTWM_FUNC(__NAME, __API_MTWM_COLOUR)


#undef ____MTWM_TYPE
#undef ___MTWM_TYPE
#undef __MTWM_TYPE

#ifndef __API_MTWM_OP_NAME
#   define ____MTWM_TYPE(__NAME, __COLOUR)  arm_2d_##__COLOUR##_##__NAME
#   define ___MTWM_TYPE(__NAME, __COLOUR)   ____MTWM_TYPE(__NAME, __COLOUR)
#else
#   define _____MTWM_TYPE(__OP_NAME, __NAME, __COLOUR)                          \
        arm_2d_##__COLOUR##_##__OP_NAME##_##__NAME
#   define ____MTWM_TYPE(__OP_NAME, __NAME, __COLOUR)                           \
        _____MTWM_TYPE(__OP_NAME, __NAME, __COLOUR)
#   define ___MTWM_TYPE(__NAME, __COLOUR)                                       \
        ____MTWM_TYPE(__API_MTWM_OP_NAME, __NAME, __COLOUR)
#endif


#define __MTWM_TYPE(__NAME)   ___MTWM_TYPE(__NAME, __API_MTWM_COLOUR)

/*============================ PROTOTYPES ====================================*/
extern
void __MTWM_FUNC(transform_with_mask)(
                                    #if __API_MTWM_CFG_SUPPORT_SOURCE_MASK      \
                                     || __API_MTWM_CFG_SUPPORT_TARGET_MASK
                                        __arm_2d_param_copy_orig_msk_t *ptThis,
                                    #else
                                        __arm_2d_param_copy_orig_t *ptParam,
                                    #endif
                                        __arm_2d_transform_info_t *ptInfo
                                    #if __API_MTWM_CFG_SUPPORT_OPATICY
                                       ,uint_fast16_t hwOpacity
                                    #endif
                                        );

/*============================ IMPLEMENTATION ================================*/

#if !__ARM_2D_CFG_FORCED_FIXED_POINT_TRANSFORM__




__OVERRIDE_WEAK
void __MTWM_FUNC(transform_with_mask)(
                                    #if __API_MTWM_CFG_SUPPORT_SOURCE_MASK      \
                                     || __API_MTWM_CFG_SUPPORT_TARGET_MASK
                                        __arm_2d_param_copy_orig_msk_t *ptThis,
                                    #else
                                        __arm_2d_param_copy_orig_t *ptParam,
                                    #endif
                                        __arm_2d_transform_info_t *ptInfo
                                    #if __API_MTWM_CFG_SUPPORT_OPATICY
                                       ,uint_fast16_t hwOpacity
                                    #endif
                                        )
{

#if __API_MTWM_CFG_SUPPORT_SOURCE_MASK                                          \
 || __API_MTWM_CFG_SUPPORT_TARGET_MASK
    __arm_2d_param_copy_orig_t *ptParam =
        &(ptThis->use_as____arm_2d_param_copy_orig_t);
#endif

    int_fast16_t iHeight = ptParam->use_as____arm_2d_param_copy_t.tCopySize.iHeight;
    int_fast16_t iWidth = ptParam->use_as____arm_2d_param_copy_t.tCopySize.iWidth;

    int_fast16_t iTargetStride = ptParam->use_as____arm_2d_param_copy_t.tTarget.iStride;
    __API_INT_TYPE *pTargetBase = ptParam->use_as____arm_2d_param_copy_t.tTarget.pBuffer;

    int_fast16_t iOrigStride = ptParam->tOrigin.iStride;

#if __API_MTWM_CFG_SUPPORT_SOURCE_MASK
    uint8_t *pOriginMask = this.tOrigMask.pBuffer;
    int_fast16_t iOrigMaskStride = this.tOrigMask.iStride;
#else
    __API_INT_TYPE MaskColour = ptInfo->Mask.hwColour;
#endif

#if __API_MTWM_CFG_SUPPORT_OPATICY
    hwOpacity += (hwOpacity == 255);
#endif

    float fAngle = -ptInfo->fAngle;
    arm_2d_location_t tOffset = ptParam->use_as____arm_2d_param_copy_t.tSource.tValidRegion.tLocation;
    float           invIWidth = iWidth > 1 ? 1.0f / (float) (iWidth - 1) : __LARGEINVF32;
    arm_2d_rot_linear_regr_t regrCoefs[2];
    arm_2d_location_t   SrcPt = ptInfo->tDummySourceOffset;

    /* get regression parameters over 1st and last column */
    __arm_2d_transform_regression(
        &ptParam->use_as____arm_2d_param_copy_t.tCopySize,
        &SrcPt,
        fAngle,
        ptInfo->fScale,
        &tOffset,
        &(ptInfo->tCenter),
        iOrigStride,
        regrCoefs);

    /* slopes between 1st and last cols */
    float           slopeY, slopeX;

    slopeY = (regrCoefs[1].interceptY - regrCoefs[0].interceptY) * invIWidth;
    slopeX = (regrCoefs[1].interceptX - regrCoefs[0].interceptX) * invIWidth;

    for (int_fast16_t y = 0; y < iHeight; y++) {
        /* 1st column estimates (intercepts for regression in X direction */
        float           colFirstY = regrCoefs[0].slopeY * y + regrCoefs[0].interceptY;
        float           colFirstX = regrCoefs[0].slopeX * y + regrCoefs[0].interceptX;

        int32_t         nbVecElts = iWidth;
        float16x8_t     vX = vcvtq_f16_s16((int16x8_t) vidupq_n_u16(0, 1));
        __API_INT_TYPE       *pTargetBaseCur = pTargetBase;

        while (nbVecElts > 0) {
            arm_2d_point_f16x8_t tPointV;

            /* linear interpolation thru first & last columns */
            tPointV.X =
                vfmaq_n_f16(vdupq_n_f16(colFirstX), vX, slopeX);
            tPointV.Y =
                vfmaq_n_f16(vdupq_n_f16(colFirstY), vX, slopeY);

#if !defined(__ARM_2D_CFG_UNSAFE_IGNORE_CALIB_IN_ROTATION_FOR_PERFORMANCE__)
            tPointV.X = vaddq_m_n_f16(tPointV.X, tPointV.X, __CALIB, vcmpgtq(tPointV.X, 0));
            tPointV.X = vsubq_m_n_f16(tPointV.X, tPointV.X, __CALIB, vcmpleq(tPointV.X, 0));

            tPointV.Y = vaddq_m_n_f16(tPointV.Y, tPointV.Y, __CALIB, vcmpgtq(tPointV.Y, 0));
            tPointV.Y = vsubq_m_n_f16(tPointV.Y, tPointV.Y, __CALIB, vcmpleq(tPointV.Y, 0));
#endif

            __ARM_2D_FUNC(get_pixel_colour_mask)(
                                &tPointV,
                                &ptParam->tOrigin.tValidRegion,
                                ptParam->tOrigin.pBuffer,
                                iOrigStride,
                                pTargetBaseCur,
                            #if __API_MTWM_CFG_SUPPORT_SOURCE_MASK
                                pOriginMask,
                                iOrigMaskStride,
                            #else
                                MaskColour,
                            #endif
                            #if __API_MTWM_CFG_SUPPORT_OPATICY
                                hwOpacity,
                            #endif
                                nbVecElts
                            );

                pTargetBaseCur += 8;
                vX += 8.0f16;
                nbVecElts -= 8;
        }
        pTargetBase += (iTargetStride);
    }

}

#else /* __ARM_2D_CFG_FORCED_FIXED_POINT_TRANSFORM__ */



__OVERRIDE_WEAK
void __MTWM_FUNC(transform_with_mask)(
                                    #if __API_MTWM_CFG_SUPPORT_SOURCE_MASK      \
                                     || __API_MTWM_CFG_SUPPORT_TARGET_MASK
                                        __arm_2d_param_copy_orig_msk_t *ptThis,
                                    #else
                                        __arm_2d_param_copy_orig_t *ptParam,
                                    #endif
                                        __arm_2d_transform_info_t *ptInfo
                                    #if __API_MTWM_CFG_SUPPORT_OPATICY
                                       ,uint_fast16_t hwOpacity
                                    #endif
                                        )
{
#if __API_MTWM_CFG_SUPPORT_SOURCE_MASK                                          \
 || __API_MTWM_CFG_SUPPORT_TARGET_MASK
    __arm_2d_param_copy_orig_t *ptParam =
        &(ptThis->use_as____arm_2d_param_copy_orig_t);
#endif

    int_fast16_t iHeight = ptParam->use_as____arm_2d_param_copy_t.tCopySize.iHeight;
    int_fast16_t iWidth = ptParam->use_as____arm_2d_param_copy_t.tCopySize.iWidth;

    int_fast16_t iTargetStride = ptParam->use_as____arm_2d_param_copy_t.tTarget.iStride;
    __API_INT_TYPE *pTargetBase = ptParam->use_as____arm_2d_param_copy_t.tTarget.pBuffer;
    int_fast16_t iOrigStride = ptParam->tOrigin.iStride;

#if __API_MTWM_CFG_SUPPORT_SOURCE_MASK
    uint8_t *pOriginMask = this.tOrigMask.pBuffer;
    int_fast16_t iOrigMaskStride = this.tOrigMask.iStride;
#else
    __API_INT_TYPE MaskColour = ptInfo->Mask.hwColour;
#endif

#if __API_MTWM_CFG_SUPPORT_OPATICY
    hwOpacity += (hwOpacity == 255);
#endif

    float fAngle = -ptInfo->fAngle;
    arm_2d_location_t tOffset = ptParam->use_as____arm_2d_param_copy_t.tSource.tValidRegion.tLocation;
    q31_t             invIWidth = iWidth > 1 ? 0x7fffffff / (iWidth - 1) : 0x7fffffff;
    arm_2d_rot_linear_regr_t regrCoefs[2];
    arm_2d_location_t   SrcPt = ptInfo->tDummySourceOffset;

    /* get regression parameters over 1st and last column */
    __arm_2d_transform_regression(
        &ptParam->use_as____arm_2d_param_copy_t.tCopySize,
        &SrcPt,
        fAngle,
        ptInfo->fScale,
        &tOffset,
        &(ptInfo->tCenter),
        iOrigStride,
        regrCoefs);



    /* slopes between 1st and last cols */
    int32_t         slopeY, slopeX;

    slopeY =
        MULTFX((regrCoefs[1].interceptY - regrCoefs[0].interceptY), invIWidth);
    slopeX =
        MULTFX((regrCoefs[1].interceptX - regrCoefs[0].interceptX), invIWidth);

    int32_t         nrmSlopeX = 17 - __CLZ(ABS(slopeX));
    int32_t         nrmSlopeY = 17 - __CLZ(ABS(slopeY));

    slopeX = ARSHIFT(slopeX, nrmSlopeX);
    slopeY = ARSHIFT(slopeY, nrmSlopeY);

    for (int_fast16_t y = 0; y < iHeight; y++) {
        /* 1st column estimates */
        int32_t         colFirstY =
            __QADD((regrCoefs[0].slopeY * y), regrCoefs[0].interceptY);
        int32_t         colFirstX =
            __QADD((regrCoefs[0].slopeX * y), regrCoefs[0].interceptX);

        /* Q6 conversion */
        colFirstX = colFirstX >> 10;
        colFirstY = colFirstY >> 10;

        int32_t         nbVecElts = iWidth;
        int16x8_t       vX = (int16x8_t) vidupq_n_u16(0, 1);
        __API_INT_TYPE       *pTargetBaseCur = pTargetBase;

        /* Q9.6 coversion */
        vX = SET_Q6INT(vX);

        while (nbVecElts > 0) {
            arm_2d_point_s16x8_t tPointV;

            tPointV.X = vqdmulhq_n_s16(vX, slopeX);
            tPointV.X = vaddq_n_s16(vqrshlq_n_s16(tPointV.X, nrmSlopeX), colFirstX);

            tPointV.Y = vqdmulhq_n_s16(vX, slopeY);
            tPointV.Y = vaddq_n_s16(vqrshlq_n_s16(tPointV.Y, nrmSlopeY), colFirstY);



            __ARM_2D_FUNC(get_pixel_colour_mask)(
                                &tPointV,
                                &ptParam->tOrigin.tValidRegion,
                                ptParam->tOrigin.pBuffer,
                                iOrigStride,
                                pTargetBaseCur,
                            #if __API_MTWM_CFG_SUPPORT_SOURCE_MASK
                                pOriginMask,
                                iOrigMaskStride,
                            #else
                                MaskColour,
                            #endif
                            #if __API_MTWM_CFG_SUPPORT_OPATICY
                                hwOpacity,
                            #endif
                                nbVecElts
                            );

            pTargetBaseCur += 8;
            vX += SET_Q6INT(8);
            nbVecElts -= 8;
        }
        pTargetBase += iTargetStride;
    }
}

#endif  /* __ARM_2D_CFG_FORCED_FIXED_POINT_TRANSFORM__ */




#undef get_pixel_colour_mask
#undef transform_with_mask


#undef __API_MTWM_COPY_LIKE_OP_NAME
#undef __API_MTWM_OP_NAME
#undef __API_MTWM_PIXEL_BLENDING
#undef ____MTWM_FUNC
#undef ___MTWM_FUNC
#undef __MTWM_FUNC
#undef __API_MTWM_COLOUR
#undef __API_MTWM_INT_TYPE
#undef __API_MTWM_INT_TYPE_BIT_NUM
#undef ____MTWM_TYPE
#undef ___MTWM_TYPE
#undef __MTWM_TYPE
#undef __API_MTWM_CFG_SUPPORT_SRC_MSK_WRAPING
#undef __API_MTWM_CFG_1_HORIZONTAL_LINE
#undef __API_MTWM_CFG_CHANNEL_8in32_SUPPORT
#undef __API_MTWM_CFG_CHANNEL_8in32_SUPPORT_ON_SOURCE_SIDE
#undef __API_MTWM_CFG_CHANNEL_8in32_SUPPORT_ON_TARGET_SIDE
#undef __API_MTWM_PIXEL_AVERAGE
#undef __API_MTWM_PIXEL_AVERAGE_RESULT
#undef __API_MTWM_PIXEL_AVERAGE_INIT
#undef __API_MTWM_CFG_SUPPORT_SOURCE_MASK
#undef __API_MTWM_CFG_SUPPORT_TARGET_MASK
#undef __API_MTWM_CFG_SUPPORT_OPATICY
