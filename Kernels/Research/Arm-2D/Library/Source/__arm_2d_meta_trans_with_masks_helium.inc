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
 * Description:  c code template for :
 *                  - transform_with_src_chn_mask_and_opacity
 *                  - transform_with_src_mask_and_opacity
 *                  - transform_with_src_chn_mask
 *                  - transform_with_src_mask
 *
 * $Date:        31. May 2022
 * $Revision:    V.1.0.1
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


#if __API_MTWM_CFG_CHANNEL_8in32_SUPPORT_ON_SOURCE_SIDE
    #define MASK_STRIDE_SCALE 4
#else
    #define MASK_STRIDE_SCALE 1
#endif

#if __API_MTWM_CFG_SUPPORT_OPATICY
    #define SCALE_BY_OPACITY(pixelAlpha, opa)   vPixelAlpha = (vPixelAlpha * opa) >> 8;
#else
    #define SCALE_BY_OPACITY(pixelAlpha, opa)
#endif


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

#if __API_MTWM_INT_TYPE_BIT_NUM == 8

#define __ARM_2D_GRAY8_GET_PIXVEC_FROM_POINT_MASK_ARR_FAR(vecX, vecY, ptVal8, vPixelAlpha,          \
                                        iOrigStride, pMaskArr, maskStrd, maskScal, opacity, pGlb) \
        arm_2d_point_s16x8_t vPoint = {.X = vecX,.Y = vecY };                                       \
        /* set vector predicate if point is inside the region */                                    \
        mve_pred16_t    p =                                                                         \
            arm_2d_is_point_vec_inside_region_s16(ptOrigValidRegion, &vPoint);                      \
        pGlb |= p;                                                                                  \
        /* prepare vector of point offsets */                                                       \
        int16_t         correctionOffset = vminvq_s16(INT16_MAX, vPoint.Y) - 1;                     \
        uint16x8_t      ptOffs = vPoint.X + (vPoint.Y - correctionOffset) * iOrigStride;            \
                                                                                                    \
        /* base pointer update to compensate offset */                                              \
        uint8_t       *pOriginCorrected = pOrigin + (correctionOffset * iOrigStride);               \
        /* retrieve all point values */                                                             \
        ptVal8 =                                                                                    \
            vldrbq_gather_offset_z_u16(pOriginCorrected, ptOffs, predTail & p);                     \
                                                                                                    \
        uint16x8_t     maskOffs = maskScal * vPoint.X + (vPoint.Y - correctionOffset) * maskStrd;   \
        uint8_t       *pMaskCorrected = pMaskArr + (correctionOffset * maskStrd);                   \
        /* retrieve all mask values */                                                              \
        vPixelAlpha =                                                                               \
            vldrbq_gather_offset_z_u16(pMaskCorrected, maskOffs, predTail & p);                     \
                                                                                                    \
        SCALE_BY_OPACITY(vPixelAlpha, opacity);                                                     \
                                                                                                    \
        ALPHA_255_COMP_VEC16(vPixelAlpha, 255);

#elif __API_MTWM_INT_TYPE_BIT_NUM == 16

#define __ARM_2D_RGB565_GET_RGBVEC_FROM_POINT_MASK_ARRR_FAR(vecX, vecY, R, G, B, vPixelAlpha,              \
                                        iOrigStride, opacity, pMaskArr, maskStrd, maskScal, pGlb)          \
        arm_2d_point_s16x8_t vPoint = {.X = vecX,.Y = vecY };                                              \
        /* set vector predicate if point is inside the region */                                           \
        mve_pred16_t    p =                                                                                \
            arm_2d_is_point_vec_inside_region_s16(ptOrigValidRegion, &vPoint);                             \
        pGlb |= p;                                                                                         \
        /* prepare vector of point offsets */                                                              \
        int16_t         correctionOffset = vminvq_s16(INT16_MAX, vPoint.Y) - 1;                            \
        uint16x8_t      ptOffs = vPoint.X + (vPoint.Y - correctionOffset) * iOrigStride;                   \
                                                                                                           \
        /* base pointer update to compensate offset */                                                     \
        uint16_t       *pOriginCorrected = pOrigin + (correctionOffset * iOrigStride);                     \
        /* retrieve all point values */                                                                    \
        uint16x8_t      ptVal =                                                                            \
            vldrhq_gather_shifted_offset_z_u16(pOriginCorrected, ptOffs, predTail & p);                    \
                                                                                                           \
        /* expand channels */                                                                              \
        __arm_2d_rgb565_unpack_single_vec(ptVal, &R, &G, &B);                                              \
        uint16x8_t     maskOffs = maskScal * vPoint.X + (vPoint.Y - correctionOffset) * maskStrd;          \
        uint8_t       *pMaskCorrected = pMaskArr + (correctionOffset * maskStrd);                          \
        /* retrieve all mask values */                                                                     \
        vPixelAlpha =                                                                                      \
            vldrbq_gather_offset_z_u16(pMaskCorrected, maskOffs, predTail & p);                            \
                                                                                                           \
        SCALE_BY_OPACITY(vPixelAlpha, opacity);                                                            \
                                                                                                           \
        ALPHA_255_COMP_VEC16(vPixelAlpha, 255);

#elif __API_MTWM_INT_TYPE_BIT_NUM == 32

#define __ARM_2D_RGB888_GET_RGBVEC_FROM_POINT_MASK_ARR(vecX, vecY, R, G, B, vPixelAlpha, iOrigStride,      \
                                                 pMaskArr, maskStrd, maskScal, opacity, pGlbLo, pGlbHi)    \
        arm_2d_point_s16x8_t vPoint = {.X = vecX,.Y = vecY };                                              \
        arm_2d_point_s32x4_t tPointLo, tPointHi;                                                           \
                                                                                                           \
        /* split 16-bit point vector into 2 x 32-bit vectors */                                            \
        vst1q(pscratch16, vPoint.X);                                                                       \
        tPointLo.X = vldrhq_s32(pscratch16);                                                               \
        tPointHi.X = vldrhq_s32(pscratch16 + 4);                                                           \
                                                                                                           \
        vst1q(pscratch16, vPoint.Y);                                                                       \
        tPointLo.Y = vldrhq_s32(pscratch16);                                                               \
        tPointHi.Y = vldrhq_s32(pscratch16 + 4);                                                           \
                                                                                                           \
        /* 1st half */                                                                                     \
                                                                                                           \
        /* set vector predicate if point is inside the region */                                           \
        mve_pred16_t    p = arm_2d_is_point_vec_inside_region_s32(ptOrigValidRegion, &tPointLo);           \
        pGlbLo |= p;  \
        /* prepare vector of point offsets */                                                              \
        uint32x4_t      ptOffs = tPointLo.X + tPointLo.Y * iOrigStride;                                    \
                                                                                                           \
        /* retrieve all point values */                                                                    \
        uint32x4_t      ptVal = vldrwq_gather_shifted_offset_z_u32(pOrigin, ptOffs, predTailLow & p);      \
                                                                                                           \
        vst1q(scratch32, ptVal);                                                                           \
                                                                                                           \
        uint32x4_t     maskOffs = maskScal * tPointLo.X + tPointLo.Y * maskStrd;                           \
        uint32x4_t      maskVal =                                                                          \
            vldrbq_gather_offset_z_u32(pMaskArr, maskOffs, predTailLow & p);                               \
                                                                                                           \
        vst1q(scratch32+8, maskVal);                                                                       \
                                                                                                           \
        /* 2nd half */                                                                                     \
                                                                                                           \
        /* set vector predicate if point is inside the region */                                           \
        p = arm_2d_is_point_vec_inside_region_s32(ptOrigValidRegion, &tPointHi);                           \
        pGlbHi |= p;                                                                                       \
        /* prepare vector of point offsets */                                                              \
        ptOffs = tPointHi.X + tPointHi.Y * iOrigStride;                                                    \
                                                                                                           \
        /* retrieve all point values */                                                                    \
        ptVal = vldrwq_gather_shifted_offset_z_u32(pOrigin, ptOffs, predTailHigh & p);                     \
                                                                                                           \
        vst1q(scratch32 + 4, ptVal);                                                                       \
                                                                                                           \
        maskOffs = maskScal * tPointHi.X + tPointHi.Y * maskStrd;                                          \
         maskVal =                                                                                         \
        vldrbq_gather_offset_z_u32(pMaskArr, maskOffs, predTailHigh & p);                                  \
                                                                                                           \
        vst1q(scratch32+12, maskVal);                                                                      \
                                                                                                           \
        /* expand channels */                                                                              \
        __arm_2d_unpack_rgb888_from_mem((uint8_t *) scratch32, &R, &G, &B);                                \
                                                                                                           \
        vPixelAlpha = vldrbq_gather_offset_u16((uint8_t *) &scratch32[8], vidupq_n_u16(0, 4));             \
                                                                                                           \
        SCALE_BY_OPACITY(vPixelAlpha, opacity);                                                            \
                                                                                                           \
        ALPHA_255_COMP_VEC16(vPixelAlpha, 255);

#endif

#if !__ARM_2D_CFG_FORCED_FIXED_POINT_TRANSFORM__

#define __ARM_2D_SCALE_MASK_GRAY8VEC(vAvgPix, vTransp, vPtVal, vAreaScal, vAlphaSc)   \
        float16x8_t vAlpha = vAreaScal * vcvtq_f16_u16(vAlphaSc) ;                    \
        vTransp = 256.0f16 * vAreaScal - vAlpha;                                      \
        vAvgPix = vAlpha * vcvtq_f16_u16(vPtVal);


#define __ARM_2D_SCALE_MASK_GRAY8VEC_ACC(vAvgPix, vTransp, vPtVal, vAreaScal, vAlphaSc) \
        float16x8_t vAlpha = vAreaScal * vcvtq_f16_u16(vAlphaSc);                       \
        vTransp += 256.0f16 * vAreaScal - vAlpha;                                       \
        vAvgPix += vAlpha * vcvtq_f16_u16(vPtVal);


#define __ARM_2D_SCALE_MASK_RGBVEC(vAvgPixelR, vAvgPixelG, vAvgPixelB, vhwTransparency,    \
                                                                R, G, B, vScal, vAlphaSc)  \
        float16x8_t vAlpha = vScal * vcvtq_f16_u16(vAlphaSc) ;                             \
        vhwTransparency = 256.0f16 * vScal - vAlpha;                                       \
        vAvgPixelR = vAlpha * vcvtq_f16_u16(R);                                            \
        vAvgPixelG = vAlpha * vcvtq_f16_u16(G);                                            \
        vAvgPixelB = vAlpha * vcvtq_f16_u16(B);


#define __ARM_2D_SCALE_MASK_RGBVEC_ACC(vAvgPixelR, vAvgPixelG, vAvgPixelB, vhwTransparency,\
                                                                 R, G, B,  vScal, vAlphaSc)\
        float16x8_t vAlpha = vScal * vcvtq_f16_u16(vAlphaSc) ;                             \
        vhwTransparency += 256.0f16 * vScal - vAlpha;                                      \
        vAvgPixelR += vAlpha * vcvtq_f16_u16(R);                                           \
        vAvgPixelG += vAlpha * vcvtq_f16_u16(G);                                           \
        vAvgPixelB += vAlpha * vcvtq_f16_u16(B);


#if __API_MTWM_INT_TYPE_BIT_NUM == 8


static
void ARM_CONNECT2(__arm_2d_impl_gray8_, get_pixel_colour_mask)(arm_2d_point_f16x8_t * ptPoint,
                                                           arm_2d_region_t * ptOrigValidRegion,
                                                           uint8_t * pOrigin,
                                                           int16_t iOrigStride,
                                                           uint8_t * pTarget,
                                                    #if __API_MTWM_CFG_SUPPORT_SOURCE_MASK
                                                           uint8_t * pchOrigMask,
                                                           int16_t iOrigmaskStride,
                                                    #else
                                                        __API_INT_TYPE MaskColour,
                                                    #endif
                                                    #if __API_MTWM_CFG_SUPPORT_OPATICY
                                                           uint16_t hwOpacity,
                                                    #endif
                                                           uint32_t elts)
{

    iOrigmaskStride *= MASK_STRIDE_SCALE;

    mve_pred16_t    predTail = vctp16q(elts);
    uint16x8_t      vTarget = vldrbq_u16(pTarget);
    int16x8_t       vXi = vcvtq_s16_f16(ptPoint->X);
    int16x8_t       vYi = vcvtq_s16_f16(ptPoint->Y);

    /* accumulated pixel vectors */
    float16x8_t     vAvgPixel;

    /* predicate accumulator */
    /* tracks all predications conditions for selecting final */
    /* averaged pixed / target pixel */
    /* can probably optimized away since averaged target pixel is */
    /* equivalent to original pixel, but precision limitations would introduce small errors */
    mve_pred16_t    predGlb = 0;
    uint16x8_t      ptVal8;
    uint16x8_t      vPixelAlpha;
    float16x8_t     vhwTransparency;

#if defined(__ARM_2D_HAS_ANTI_ALIAS_TRANSFORM__) &&  __ARM_2D_HAS_ANTI_ALIAS_TRANSFORM__
    /*
     * accumulate / average over the 4 neigbouring pixels
     */

    float16x8_t     vOne = vdupq_n_f16(1.0f);

    vXi = vsubq_m_n_s16(vXi, vXi, 1, vcmpltq_n_f16(ptPoint->X, 0));
    vYi = vsubq_m_n_s16(vYi, vYi, 1, vcmpltq_n_f16(ptPoint->Y, 0));

    float16x8_t     vWX = ptPoint->X - vcvtq_f16_s16(vXi);
    float16x8_t     vWY = ptPoint->Y - vcvtq_f16_s16(vYi);

    /* combination of Bottom / Top & Left / Right areas contributions */
    float16x8_t     vAreaTR = vWX * vWY;
    float16x8_t     vAreaTL = (vOne - vWX) * vWY;
    float16x8_t     vAreaBR = vWX * (vOne - vWY);
    float16x8_t     vAreaBL = (vOne - vWX) * (vOne - vWY);

    /* Bottom Left averaging */
    {

        __ARM_2D_GRAY8_GET_PIXVEC_FROM_POINT_MASK_ARR_FAR(vXi, vYi, ptVal8, vPixelAlpha, iOrigStride,
                                                          pchOrigMask, iOrigmaskStride, MASK_STRIDE_SCALE,
                                                          hwOpacity, predGlb);

        __ARM_2D_SCALE_MASK_GRAY8VEC(vAvgPixel, vhwTransparency, ptVal8, vAreaBL, vPixelAlpha);

    }

    /* Bottom Right averaging */
    {
        __ARM_2D_GRAY8_GET_PIXVEC_FROM_POINT_MASK_ARR_FAR(vaddq_n_s16(vXi, 1), vYi,
                                                          ptVal8, vPixelAlpha, iOrigStride,
                                                          pchOrigMask, iOrigmaskStride, MASK_STRIDE_SCALE,
                                                          hwOpacity, predGlb);

        __ARM_2D_SCALE_MASK_GRAY8VEC_ACC(vAvgPixel, vhwTransparency, ptVal8, vAreaBR, vPixelAlpha);
    }

    /* Top Left averaging */
    {
        __ARM_2D_GRAY8_GET_PIXVEC_FROM_POINT_MASK_ARR_FAR(vXi, vaddq_n_s16(vYi, 1),
                                                          ptVal8, vPixelAlpha, iOrigStride,
                                                          pchOrigMask, iOrigmaskStride, MASK_STRIDE_SCALE,
                                                          hwOpacity, predGlb);

        __ARM_2D_SCALE_MASK_GRAY8VEC_ACC(vAvgPixel, vhwTransparency, ptVal8, vAreaTL, vPixelAlpha);
    }

    /* Top Right averaging */
    {
        __ARM_2D_GRAY8_GET_PIXVEC_FROM_POINT_MASK_ARR_FAR(vaddq_n_s16(vXi, 1),
                                                          vaddq_n_s16(vYi, 1), ptVal8, vPixelAlpha,
                                                          iOrigStride, pchOrigMask, iOrigmaskStride, MASK_STRIDE_SCALE,
                                                          hwOpacity, predGlb);

        __ARM_2D_SCALE_MASK_GRAY8VEC_ACC(vAvgPixel, vhwTransparency, ptVal8, vAreaTR, vPixelAlpha);
    }

#else
    {
        __ARM_2D_GRAY8_GET_PIXVEC_FROM_POINT_MASK_ARR_FAR(vXi, vYi, ptVal8, vPixelAlpha, iOrigStride,
                                                          pchOrigMask, iOrigmaskStride, MASK_STRIDE_SCALE,
                                                          hwOpacity, predGlb);

        __ARM_2D_SCALE_MASK_GRAY8VEC(vAvgPixel, vhwTransparency, ptVal8, 1.0f16, vPixelAlpha);

    }
#endif

    /* blending */
    uint16x8_t      vAvg = vcvtq_u16_f16(vAvgPixel);
    uint16x8_t      vTrans = vcvtq_u16_f16(vhwTransparency);
    uint16x8_t      vBlended = vqaddq(vAvg, vTarget * vTrans) >> 8;

    /* select between target pixel, averaged pixed */
    vTarget = vpselq_u16(vBlended, vTarget, predGlb);

    vstrbq_p_u16(pTarget, vTarget, predTail);
}

#elif __API_MTWM_INT_TYPE_BIT_NUM == 16


static
void ARM_CONNECT2(__arm_2d_impl_rgb565_, get_pixel_colour_mask)(arm_2d_point_f16x8_t * ptPoint,
                                                            arm_2d_region_t * ptOrigValidRegion,
                                                            uint16_t * pOrigin,
                                                            int16_t iOrigStride,
                                                            uint16_t * pTarget,
                                                    #if __API_MTWM_CFG_SUPPORT_SOURCE_MASK
                                                           uint8_t * pchOrigMask,
                                                           int16_t iOrigmaskStride,
                                                    #else
                                                        __API_INT_TYPE MaskColour,
                                                    #endif
                                                    #if __API_MTWM_CFG_SUPPORT_OPATICY
                                                           uint16_t hwOpacity,
                                                    #endif
                                                            uint32_t elts)
{
    iOrigmaskStride *= MASK_STRIDE_SCALE;

    mve_pred16_t    predTail = vctp16q(elts);
    uint16x8_t      vTarget = vld1q(pTarget);
    int16x8_t       vXi = vcvtq_s16_f16(ptPoint->X);
    int16x8_t       vYi = vcvtq_s16_f16(ptPoint->Y);

    /* accumulated pixel vectors */
    float16x8_t     vAvgPixelR, vAvgPixelG, vAvgPixelB;

    /* predicate accumulator */
    /* tracks all predications conditions for selecting final */
    /* averaged pixed / target pixel */
    mve_pred16_t    predGlb = 0;
    uint16x8_t      vPixelAlpha;
    float16x8_t     vhwTransparency;
    uint16x8_t      R, G, B;

#if defined(__ARM_2D_HAS_ANTI_ALIAS_TRANSFORM__) &&  __ARM_2D_HAS_ANTI_ALIAS_TRANSFORM__
    /*
     * accumulate / average over the 4 neigbouring pixels
     */
    float16x8_t     vOne = vdupq_n_f16(1.0f);

    vXi = vsubq_m_n_s16(vXi, vXi, 1, vcmpltq_n_f16(ptPoint->X, 0));
    vYi = vsubq_m_n_s16(vYi, vYi, 1, vcmpltq_n_f16(ptPoint->Y, 0));

    float16x8_t     vWX = ptPoint->X - vcvtq_f16_s16(vXi);
    float16x8_t     vWY = ptPoint->Y - vcvtq_f16_s16(vYi);

    /* combination of Bottom / Top & Left / Right areas contributions */
    float16x8_t     vAreaTR = vWX * vWY;
    float16x8_t     vAreaTL = (vOne - vWX) * vWY;
    float16x8_t     vAreaBR = vWX * (vOne - vWY);
    float16x8_t     vAreaBL = (vOne - vWX) * (vOne - vWY);


    /* Bottom Left averaging */
    {
        __ARM_2D_RGB565_GET_RGBVEC_FROM_POINT_MASK_ARRR_FAR(vXi, vYi, R, G, B,
                                                            vPixelAlpha, iOrigStride,
                                                            hwOpacity, pchOrigMask,
                                                            iOrigmaskStride, MASK_STRIDE_SCALE, predGlb);

        __ARM_2D_SCALE_MASK_RGBVEC(vAvgPixelR, vAvgPixelG, vAvgPixelB, vhwTransparency, R,
                                   G, B, vAreaBL, vPixelAlpha);
    }


    /* Bottom Right averaging */
    {
        __ARM_2D_RGB565_GET_RGBVEC_FROM_POINT_MASK_ARRR_FAR(vaddq_n_s16(vXi, 1), vYi, R,
                                                            G, B, vPixelAlpha,
                                                            iOrigStride, hwOpacity,
                                                            pchOrigMask,iOrigmaskStride, MASK_STRIDE_SCALE,
                                                            predGlb);

        __ARM_2D_SCALE_MASK_RGBVEC_ACC(vAvgPixelR, vAvgPixelG, vAvgPixelB,
                                       vhwTransparency, R, G, B, vAreaBR, vPixelAlpha);
    }

    /* Top Left averaging */
    {
        __ARM_2D_RGB565_GET_RGBVEC_FROM_POINT_MASK_ARRR_FAR(vXi, vaddq_n_s16(vYi, 1), R,
                                                            G, B, vPixelAlpha,
                                                            iOrigStride, hwOpacity,
                                                            pchOrigMask, iOrigmaskStride, MASK_STRIDE_SCALE,
                                                            predGlb);

        __ARM_2D_SCALE_MASK_RGBVEC_ACC(vAvgPixelR, vAvgPixelG, vAvgPixelB,
                                       vhwTransparency, R, G, B, vAreaTL, vPixelAlpha);
    }

    /* Top Right averaging */
    {
        __ARM_2D_RGB565_GET_RGBVEC_FROM_POINT_MASK_ARRR_FAR(vaddq_n_s16(vXi, 1),
                                                            vaddq_n_s16(vYi, 1), R, G, B,
                                                            vPixelAlpha, iOrigStride,
                                                            hwOpacity, pchOrigMask,
                                                            iOrigmaskStride, MASK_STRIDE_SCALE,predGlb);

        __ARM_2D_SCALE_MASK_RGBVEC_ACC(vAvgPixelR, vAvgPixelG, vAvgPixelB,
                                       vhwTransparency, R, G, B, vAreaTR, vPixelAlpha);
    }
#else

    {
        __ARM_2D_RGB565_GET_RGBVEC_FROM_POINT_MASK_ARRR_FAR(vXi, vYi, R, G, B,
                                                            vPixelAlpha, iOrigStride,
                                                            hwOpacity, pchOrigMask,
                                                            iOrigmaskStride, MASK_STRIDE_SCALE,predGlb);

        __ARM_2D_SCALE_MASK_RGBVEC(vAvgPixelR, vAvgPixelG, vAvgPixelB, vhwTransparency, R,
                                   G, B, 1.0f16, vPixelAlpha);
    }

#endif

    /* blending */
    uint16x8_t      vAvgR, vAvgG, vAvgB;
    uint16x8_t      vTrans = vcvtq_u16_f16(vhwTransparency);
    uint16x8_t      vBlended;

    vAvgR = vcvtq_u16_f16(vAvgPixelR);
    vAvgG = vcvtq_u16_f16(vAvgPixelG);
    vAvgB = vcvtq_u16_f16(vAvgPixelB);


    uint16x8_t      vTargetR, vTargetG, vTargetB;

    __arm_2d_rgb565_unpack_single_vec(vTarget, &vTargetR, &vTargetG, &vTargetB);

    /* merge */
    vAvgR = vqaddq(vAvgR,  vTargetR * vTrans);
    vAvgR = vAvgR >> 8;

    vAvgG = vqaddq(vAvgG, vTargetG * vTrans);
    vAvgG = vAvgG >> 8;

    vAvgB = vqaddq(vAvgB, vTargetB * vTrans);
    vAvgB = vAvgB >> 8;

    vBlended = __arm_2d_rgb565_pack_single_vec(vAvgR, vAvgG, vAvgB);

    /* select between target pixel, averaged pixed */
    vTarget = vpselq_u16(vBlended, vTarget, predGlb);

    vst1q_p(pTarget, vTarget, predTail);
}

#elif __API_MTWM_INT_TYPE_BIT_NUM == 32


static
void ARM_CONNECT2(__arm_2d_impl_cccn888_, get_pixel_colour_mask)(arm_2d_point_f16x8_t *ptPoint,
                                        arm_2d_region_t *ptOrigValidRegion,
                                        uint32_t *pOrigin,
                                        int16_t iOrigStride,
                                        uint32_t *pTarget,
                                        #if __API_MTWM_CFG_SUPPORT_SOURCE_MASK
                                               uint8_t * pchOrigMask,
                                               int16_t iOrigmaskStride,
                                        #else
                                            __API_INT_TYPE MaskColour,
                                        #endif
                                        #if __API_MTWM_CFG_SUPPORT_OPATICY
                                               uint16_t hwOpacity,
                                        #endif
                                        uint32_t elts
                                       )
{
    iOrigmaskStride *= MASK_STRIDE_SCALE;

    ARM_ALIGN(8) uint32_t scratch32[32];
    int16_t        *pscratch16 = (int16_t *) scratch32;
    uint32x4_t      vTargetLo = vld1q(pTarget);
    uint32x4_t      vTargetHi = vld1q(pTarget + 4);
    mve_pred16_t    predTailLow = vctp32q(elts);
    mve_pred16_t    predTailHigh = elts - 4 > 0 ? vctp32q(elts - 4) : 0;
    int16x8_t       vXi = vcvtq_s16_f16(ptPoint->X);
    int16x8_t       vYi = vcvtq_s16_f16(ptPoint->Y);

    /* accumulated pixel vectors */
    float16x8_t     vAvgPixelR, vAvgPixelG, vAvgPixelB;

    /* predicate accumulators */
    /* tracks all predications conditions for selecting final */
    /* averaged pixed / target pixel */
    /* can probably optimized away since averaged target pixel is */
    /* equivalent to original pixel, but precision limitations would introduce small errors */
    mve_pred16_t    predGlbLo = 0, predGlbHi = 0;

    /*
     * accumulate / average over the 4 neigbouring pixels
     */
    uint16x8_t      R, G, B;
    uint16x8_t      vPixelAlpha;
    float16x8_t     vhwTransparency;

#if defined(__ARM_2D_HAS_ANTI_ALIAS_TRANSFORM__)  &&  __ARM_2D_HAS_ANTI_ALIAS_TRANSFORM__
    float16x8_t     vOne = vdupq_n_f16(1.0f);

    vXi = vsubq_m_n_s16(vXi, vXi, 1, vcmpltq_n_f16(ptPoint->X, 0));
    vYi = vsubq_m_n_s16(vYi, vYi, 1, vcmpltq_n_f16(ptPoint->Y, 0));

    float16x8_t     vWX = ptPoint->X - vcvtq_f16_s16(vXi);
    float16x8_t     vWY = ptPoint->Y - vcvtq_f16_s16(vYi);

    /* combination of Bottom / Top & Left / Right areas contributions */
    float16x8_t     vAreaTR = vWX * vWY;
    float16x8_t     vAreaTL = (vOne - vWX) * vWY;
    float16x8_t     vAreaBR = vWX * (vOne - vWY);
    float16x8_t     vAreaBL = (vOne - vWX) * (vOne - vWY);


    /* Bottom Left averaging */
    {
        __ARM_2D_RGB888_GET_RGBVEC_FROM_POINT_MASK_ARR(vXi, vYi, R, G, B, vPixelAlpha,
                                                       iOrigStride, pchOrigMask,
                                                       iOrigmaskStride, MASK_STRIDE_SCALE, hwOpacity,
                                                       predGlbLo, predGlbHi);

        __ARM_2D_SCALE_MASK_RGBVEC(vAvgPixelR, vAvgPixelG, vAvgPixelB, vhwTransparency, R,
                                   G, B, vAreaBL, vPixelAlpha);
    }

    /* Bottom Right averaging */
    {
        __ARM_2D_RGB888_GET_RGBVEC_FROM_POINT_MASK_ARR(vaddq_n_s16(vXi, 1), vYi, R, G, B,
                                                       vPixelAlpha, iOrigStride,
                                                       pchOrigMask, iOrigmaskStride, MASK_STRIDE_SCALE,
                                                       hwOpacity, predGlbLo, predGlbHi);

        __ARM_2D_SCALE_MASK_RGBVEC_ACC(vAvgPixelR, vAvgPixelG, vAvgPixelB,
                                       vhwTransparency, R, G, B, vAreaBR, vPixelAlpha);
    }

    /* Top Left averaging */
    {
        __ARM_2D_RGB888_GET_RGBVEC_FROM_POINT_MASK_ARR(vXi, vaddq_n_s16(vYi, 1),
                                                       R, G, B, vPixelAlpha, iOrigStride,
                                                       pchOrigMask, iOrigmaskStride, MASK_STRIDE_SCALE,
                                                       hwOpacity, predGlbLo, predGlbHi);

        __ARM_2D_SCALE_MASK_RGBVEC_ACC(vAvgPixelR, vAvgPixelG, vAvgPixelB,
                                       vhwTransparency, R, G, B, vAreaTL, vPixelAlpha)
    }

    /* Top Right averaging */
    {
        __ARM_2D_RGB888_GET_RGBVEC_FROM_POINT_MASK_ARR(vaddq_n_s16(vXi, 1),
                                                       vaddq_n_s16(vYi, 1), R, G, B,
                                                       vPixelAlpha, iOrigStride,
                                                       pchOrigMask, iOrigmaskStride, MASK_STRIDE_SCALE,
                                                       hwOpacity, predGlbLo, predGlbHi);

        __ARM_2D_SCALE_MASK_RGBVEC_ACC(vAvgPixelR, vAvgPixelG, vAvgPixelB,
                                       vhwTransparency, R, G, B, vAreaTR, vPixelAlpha)
    }

#else
    {
        __ARM_2D_RGB888_GET_RGBVEC_FROM_POINT_MASK_ARR(vXi, vYi, R, G, B, vPixelAlpha,
                                                       iOrigStride, pchOrigMask,
                                                       iOrigmaskStride, MASK_STRIDE_SCALE, hwOpacity,
                                                       predGlbLo, predGlbHi);

        __ARM_2D_SCALE_MASK_RGBVEC(vAvgPixelR, vAvgPixelG, vAvgPixelB, vhwTransparency, R,
                                   G, B, 1.0f16, vPixelAlpha);
    }
#endif

    /* alpha blending */
    uint16x8_t      vTargetR, vTargetG, vTargetB;
    uint16x8_t      vTrans = vcvtq_u16_f16(vhwTransparency);;
    uint16x8_t      vAvgR, vAvgG, vAvgB;

    __arm_2d_unpack_rgb888_from_mem((const uint8_t *) pTarget, &vTargetR, &vTargetG,
                                    &vTargetB);

    vAvgR = vcvtq_u16_f16(vAvgPixelR);
    vAvgG = vcvtq_u16_f16(vAvgPixelG);
    vAvgB = vcvtq_u16_f16(vAvgPixelB);


    /* merge */
    vAvgR = vqaddq(vAvgR, vTargetR * vTrans);
    vAvgR = vAvgR >> 8;

    vAvgG = vqaddq(vAvgG, vTargetG * vTrans);
    vAvgG = vAvgG >> 8;

    vAvgB = vqaddq(vAvgB, vTargetB * vTrans);
    vAvgB = vAvgB >> 8;

    /* pack */
    __arm_2d_pack_rgb888_to_mem((uint8_t *) scratch32, vAvgR, vAvgG, vAvgB);

    uint32x4_t      TempPixel = vld1q(scratch32);

    /* select between target pixel, averaged pixed */
    TempPixel = vpselq_u32(TempPixel, vTargetLo, predGlbLo);

    vst1q_p(pTarget, TempPixel, predTailLow);

    TempPixel = vld1q(scratch32 + 4);

    /* select between target pixel, averaged pixed */
    TempPixel = vpselq_u32(TempPixel, vTargetHi, predGlbHi);

    vst1q_p(pTarget + 4, TempPixel, predTailHigh);
}

#endif




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


#define __ARM_2D_SCALE_MASK_GRAY8VEC(vAvgPix, vTransp, vPtVal, vAreaScal, vAlphaSc)     \
        uint16x8_t vAlpha = vmulq_u16((vAreaScal >> 8),  vAlphaSc);                     \
        vTransp = vAreaScal - vAlpha;                                                   \
        vAvgPix = vmulhq(vAlpha, vPtVal << 8);


#define __ARM_2D_SCALE_MASK_GRAY8VEC_ACC(vAvgPix, vTransp, vPtVal, vAreaScal, vAlphaSc) \
        uint16x8_t vAlpha = vmulq_u16((vAreaScal >> 8), vAlphaSc);                      \
        vTransp += vAreaScal - vAlpha;                                                  \
        vAvgPix += vmulhq(vAlpha,  vPtVal << 8);


#define __ARM_2D_SCALE_MASK_RGBVEC(vAvgPixR, vAvgPixG, vAvgPixB, vTransp, R, G, B,      \
                                                            vAreaScal, vAlphaSc)        \
        uint16x8_t vAlpha = vmulq_u16((vAreaScal >> 8), vAlphaSc);                      \
        vTransp = vAreaScal - vAlpha;                                                   \
        vAvgPixelR = vmulhq_u16(vAlpha, R << 8);                                        \
        vAvgPixelG = vmulhq_u16(vAlpha, G << 8);                                        \
        vAvgPixelB = vmulhq_u16(vAlpha, B << 8);


#define __ARM_2D_SCALE_MASK_RGBVEC_ACC(vAvgPixR, vAvgPixG, vAvgPixB, vTransp, R, G, B,  \
                                                             vAreaScal, vAlphaSc)       \
        uint16x8_t vAlpha = vmulq_u16((vAreaScal >> 8), vAlphaSc);                      \
        vTransp += vAreaScal - vAlpha;                                                  \
        vAvgPixelR += vmulhq_u16(vAlpha, R << 8);                                       \
        vAvgPixelG += vmulhq_u16(vAlpha, G << 8);                                       \
        vAvgPixelB += vmulhq_u16(vAlpha, B << 8);


#if __API_MTWM_INT_TYPE_BIT_NUM == 8


static
void ARM_CONNECT2(__arm_2d_impl_gray8_, get_pixel_colour_mask)(arm_2d_point_s16x8_t * ptPoint,
                                                           arm_2d_region_t * ptOrigValidRegion,
                                                           uint8_t * pOrigin,
                                                           int16_t iOrigStride,
                                                           uint8_t * pTarget,
                                                    #if __API_MTWM_CFG_SUPPORT_SOURCE_MASK
                                                           uint8_t * pchOrigMask,
                                                           int16_t iOrigmaskStride,
                                                    #else
                                                        __API_INT_TYPE MaskColour,
                                                    #endif
                                                    #if __API_MTWM_CFG_SUPPORT_OPATICY
                                                           uint16_t hwOpacity,
                                                    #endif
                                                           uint32_t elts)
{
    iOrigmaskStride *= MASK_STRIDE_SCALE;

    mve_pred16_t    predTail = vctp16q(elts);
    uint16x8_t      vTarget = vldrbq_u16(pTarget);

    int16x8_t       vXi = GET_Q6INT(ptPoint->X);
    int16x8_t       vYi = GET_Q6INT(ptPoint->Y);

    /* accumulated pixel vectors */
    uint16x8_t      vAvgPixel;

    /* predicate accumulator */
    /* tracks all predications conditions for selecting final */
    /* averaged pixed / target pixel */
    /* can probably optimized away since averaged target pixel is */
    /* equivalent to original pixel, but precision limitations would introduce small errors */
    mve_pred16_t    predGlb = 0;
    uint16x8_t      ptVal8;
    uint16x8_t      vPixelAlpha;
    uint16x8_t      vhwTransparency;


#if defined(__ARM_2D_HAS_ANTI_ALIAS_TRANSFORM__) &&  __ARM_2D_HAS_ANTI_ALIAS_TRANSFORM__
    int16x8_t       vOne = vdupq_n_s16(SET_Q6INT(1));

    vXi = vsubq_m_n_s16(vXi, vXi, 1, vcmpltq_n_s16(ptPoint->X, 0));
    vYi = vsubq_m_n_s16(vYi, vYi, 1, vcmpltq_n_s16(ptPoint->Y, 0));

    int16x8_t       vWX = ptPoint->X - SET_Q6INT(vXi);
    int16x8_t       vWY = ptPoint->Y - SET_Q6INT(vYi);

    /* combination of Bottom / Top & Left / Right areas contributions */
    uint16x8_t      vAreaTR = vmulq_u16(vWX, vWY);
    uint16x8_t      vAreaTL = vmulq_u16((vOne - vWX), vWY);
    uint16x8_t      vAreaBR = vmulq_u16(vWX, (vOne - vWY));
    uint16x8_t      vAreaBL = vmulq_u16((vOne - vWX), (vOne - vWY));

    /* Q16 conversion */
    vAreaTR = vqshlq_n_u16(vAreaTR, 4);
    vAreaTL = vqshlq_n_u16(vAreaTL, 4);
    vAreaBR = vqshlq_n_u16(vAreaBR, 4);
    vAreaBL = vqshlq_n_u16(vAreaBL, 4);


    /*
     * accumulate / average over the 4 neigbouring pixels
     */


    /* Bottom Left averaging */
    {

        __ARM_2D_GRAY8_GET_PIXVEC_FROM_POINT_MASK_ARR_FAR(vXi, vYi, ptVal8, vPixelAlpha,
                                                          iOrigStride, pchOrigMask,
                                                          iOrigmaskStride, MASK_STRIDE_SCALE,
                                                          hwOpacity, predGlb);

        __ARM_2D_SCALE_MASK_GRAY8VEC(vAvgPixel, vhwTransparency, ptVal8, vAreaBL,
                                     vPixelAlpha);
    }


    /* Bottom Right averaging */
    {
        __ARM_2D_GRAY8_GET_PIXVEC_FROM_POINT_MASK_ARR_FAR(vaddq_n_s16(vXi, 1), vYi,
                                                          ptVal8, vPixelAlpha,
                                                          iOrigStride, pchOrigMask,
                                                          iOrigmaskStride, MASK_STRIDE_SCALE,
                                                          hwOpacity, predGlb);

        __ARM_2D_SCALE_MASK_GRAY8VEC_ACC(vAvgPixel, vhwTransparency, ptVal8, vAreaBR,
                                         vPixelAlpha);
    }

    /* Top Left averaging */
    {
        __ARM_2D_GRAY8_GET_PIXVEC_FROM_POINT_MASK_ARR_FAR(vXi, vaddq_n_s16(vYi, 1),
                                                          ptVal8, vPixelAlpha,
                                                          iOrigStride, pchOrigMask,
                                                          iOrigmaskStride, MASK_STRIDE_SCALE,
                                                          hwOpacity, predGlb);

        __ARM_2D_SCALE_MASK_GRAY8VEC_ACC(vAvgPixel, vhwTransparency, ptVal8, vAreaTL,
                                         vPixelAlpha);
    }

    /* Top Right averaging */
    {
        __ARM_2D_GRAY8_GET_PIXVEC_FROM_POINT_MASK_ARR_FAR(vaddq_n_s16(vXi, 1),
                                                          vaddq_n_s16(vYi, 1), ptVal8,
                                                          vPixelAlpha, iOrigStride,
                                                          pchOrigMask, iOrigmaskStride, MASK_STRIDE_SCALE,
                                                          hwOpacity, predGlb);

        __ARM_2D_SCALE_MASK_GRAY8VEC_ACC(vAvgPixel, vhwTransparency, ptVal8, vAreaTR,
                                         vPixelAlpha);
    }
#else
    {
        __ARM_2D_GRAY8_GET_PIXVEC_FROM_POINT_MASK_ARR_FAR(vXi, vYi, ptVal8, vPixelAlpha,
                                                          iOrigStride, pchOrigMask,
                                                          iOrigmaskStride, MASK_STRIDE_SCALE,
                                                          hwOpacity, predGlb);


        __ARM_2D_SCALE_MASK_GRAY8VEC(vAvgPixel, vhwTransparency, ptVal8,
                                     vdupq_n_u16(ONE_Q16), vPixelAlpha);

    }
#endif

    /* blending */

    uint16x8_t      vBlended = vqaddq(vAvgPixel, vmulhq(vTarget << 8, vhwTransparency)) >> 8;

    /* select between target pixel, averaged pixed */
    vTarget = vpselq_u16(vBlended, vTarget, predGlb);

    vstrbq_p_u16(pTarget, vTarget, predTail);
}

#elif __API_MTWM_INT_TYPE_BIT_NUM == 16

static
void ARM_CONNECT2(__arm_2d_impl_rgb565_, get_pixel_colour_mask)(arm_2d_point_s16x8_t * ptPoint,
                                                            arm_2d_region_t * ptOrigValidRegion,
                                                            uint16_t * pOrigin,
                                                            int16_t iOrigStride,
                                                            uint16_t * pTarget,
                                                    #if __API_MTWM_CFG_SUPPORT_SOURCE_MASK
                                                           uint8_t * pchOrigMask,
                                                           int16_t iOrigmaskStride,
                                                    #else
                                                        __API_INT_TYPE MaskColour,
                                                    #endif
                                                    #if __API_MTWM_CFG_SUPPORT_OPATICY
                                                           uint16_t hwOpacity,
                                                    #endif
                                                            uint32_t elts)
{
    iOrigmaskStride *= MASK_STRIDE_SCALE;

    mve_pred16_t    predTail = vctp16q(elts);
    uint16x8_t      vTarget = vld1q(pTarget);
    uint16x8_t      vPixelAlpha;
    uint16x8_t      vhwTransparency;

    /* accumulated pixel vectors */
    uint16x8_t      vAvgPixelR, vAvgPixelG, vAvgPixelB;
    uint16x8_t      R, G, B;

    /* predicate accumulator */
    /* tracks all predications conditions for selecting final */
    /* averaged pixed / target pixel */
    /* can probably optimized away since averaged target pixel is */
    /* equivalent to original pixel, but precision limitations would introduce small errors */
    mve_pred16_t    predGlb = 0;

    int16x8_t       vXi = GET_Q6INT(ptPoint->X);
    int16x8_t       vYi = GET_Q6INT(ptPoint->Y);


#if defined(__ARM_2D_HAS_ANTI_ALIAS_TRANSFORM__) &&  __ARM_2D_HAS_ANTI_ALIAS_TRANSFORM__
    int16x8_t       vOne = vdupq_n_s16(SET_Q6INT(1));

    vXi = vsubq_m_n_s16(vXi, vXi, 1, vcmpltq_n_s16(ptPoint->X, 0));
    vYi = vsubq_m_n_s16(vYi, vYi, 1, vcmpltq_n_s16(ptPoint->Y, 0));

    int16x8_t       vWX = ptPoint->X - SET_Q6INT(vXi);
    int16x8_t       vWY = ptPoint->Y - SET_Q6INT(vYi);

    /* combination of Bottom / Top & Left / Right areas contributions */
    uint16x8_t      vAreaTR = vmulq_u16(vWX, vWY);
    uint16x8_t      vAreaTL = vmulq_u16((vOne - vWX), vWY);
    uint16x8_t      vAreaBR = vmulq_u16(vWX, (vOne - vWY));
    uint16x8_t      vAreaBL = vmulq_u16((vOne - vWX), (vOne - vWY));

    /* Q16 conversion */
    vAreaTR = vqshlq_n_u16(vAreaTR, 4);
    vAreaTL = vqshlq_n_u16(vAreaTL, 4);
    vAreaBR = vqshlq_n_u16(vAreaBR, 4);
    vAreaBL = vqshlq_n_u16(vAreaBL, 4);

    /*
     * accumulate / average over the 4 neigbouring pixels
     */


    /* Bottom Left averaging */
    {
        __ARM_2D_RGB565_GET_RGBVEC_FROM_POINT_MASK_ARRR_FAR(vXi, vYi, R, G, B,
                                                            vPixelAlpha, iOrigStride,
                                                            hwOpacity, pchOrigMask,
                                                            iOrigmaskStride, MASK_STRIDE_SCALE, predGlb);

        __ARM_2D_SCALE_MASK_RGBVEC(vAvgPixelR, vAvgPixelG, vAvgPixelB, vhwTransparency, R,
                                   G, B, vAreaBL, vPixelAlpha);
    }


    /* Bottom Right averaging */
    {
        __ARM_2D_RGB565_GET_RGBVEC_FROM_POINT_MASK_ARRR_FAR(vaddq_n_s16(vXi, 1), vYi, R,
                                                            G, B, vPixelAlpha,
                                                            iOrigStride, hwOpacity,
                                                            pchOrigMask, iOrigmaskStride, MASK_STRIDE_SCALE,
                                                            predGlb);

        __ARM_2D_SCALE_MASK_RGBVEC_ACC(vAvgPixelR, vAvgPixelG, vAvgPixelB,
                                       vhwTransparency, R, G, B, vAreaBR, vPixelAlpha);
    }

    /* Top Left averaging */
    {
        __ARM_2D_RGB565_GET_RGBVEC_FROM_POINT_MASK_ARRR_FAR(vXi, vaddq_n_s16(vYi, 1), R,
                                                            G, B, vPixelAlpha,
                                                            iOrigStride, hwOpacity,
                                                            pchOrigMask, iOrigmaskStride, MASK_STRIDE_SCALE,
                                                            predGlb);

        __ARM_2D_SCALE_MASK_RGBVEC_ACC(vAvgPixelR, vAvgPixelG, vAvgPixelB,
                                       vhwTransparency, R, G, B, vAreaTL, vPixelAlpha)
    }

    /* Top Right averaging */
    {
        __ARM_2D_RGB565_GET_RGBVEC_FROM_POINT_MASK_ARRR_FAR(vaddq_n_s16(vXi, 1),
                                                            vaddq_n_s16(vYi, 1), R, G, B,
                                                            vPixelAlpha, iOrigStride,
                                                            hwOpacity, pchOrigMask,
                                                            iOrigmaskStride, MASK_STRIDE_SCALE, predGlb);

        __ARM_2D_SCALE_MASK_RGBVEC_ACC(vAvgPixelR, vAvgPixelG, vAvgPixelB,
                                       vhwTransparency, R, G, B, vAreaTR, vPixelAlpha)
    }
#else
    {
        __ARM_2D_RGB565_GET_RGBVEC_FROM_POINT_MASK_ARRR_FAR(vXi, vYi, R, G, B,
                                                            vPixelAlpha, iOrigStride,
                                                            hwOpacity, pchOrigMask,
                                                            iOrigmaskStride, MASK_STRIDE_SCALE, predGlb);

        __ARM_2D_SCALE_MASK_RGBVEC(vAvgPixelR, vAvgPixelG, vAvgPixelB, vhwTransparency, R,
                                   G, B, vdupq_n_u16(ONE_Q16), vPixelAlpha);
    }
#endif

    /* blending */
    uint16x8_t      vBlended;
    uint16x8_t      vTargetR, vTargetG, vTargetB;

    __arm_2d_rgb565_unpack_single_vec(vTarget, &vTargetR, &vTargetG, &vTargetB);

    /* merge */
    vAvgPixelR = vqaddq(vAvgPixelR, vmulhq(vTargetR << 8, vhwTransparency));
    vAvgPixelR = vAvgPixelR >> 8;

    vAvgPixelG = vqaddq(vAvgPixelG, vmulhq(vTargetG << 8, vhwTransparency));
    vAvgPixelG = vAvgPixelG >> 8;

    vAvgPixelB = vqaddq(vAvgPixelB, vmulhq(vTargetB << 8, vhwTransparency));
    vAvgPixelB = vAvgPixelB >> 8;

    vBlended = __arm_2d_rgb565_pack_single_vec(vAvgPixelR, vAvgPixelG, vAvgPixelB);

    /* select between target pixel, averaged pixed */
    vTarget = vpselq_u16(vBlended, vTarget, predGlb);

    vst1q_p(pTarget, vTarget, predTail);

}


#elif __API_MTWM_INT_TYPE_BIT_NUM == 32

static
void ARM_CONNECT2(__arm_2d_impl_cccn888_, get_pixel_colour_mask)(arm_2d_point_s16x8_t *ptPoint,
                                        arm_2d_region_t *ptOrigValidRegion,
                                        uint32_t *pOrigin,
                                        int16_t iOrigStride,
                                        uint32_t *pTarget,
                                        #if __API_MTWM_CFG_SUPPORT_SOURCE_MASK
                                           uint8_t * pchOrigMask,
                                           int16_t iOrigmaskStride,
                                        #else
                                            __API_INT_TYPE MaskColour,
                                        #endif
                                        #if __API_MTWM_CFG_SUPPORT_OPATICY
                                               uint16_t hwOpacity,
                                        #endif
                                        uint32_t elts
                                       )
{
    iOrigmaskStride *= MASK_STRIDE_SCALE;

    uint16x8_t      vPixelAlpha;
    uint16x8_t      vhwTransparency;
    ARM_ALIGN(8) uint32_t scratch32[32];
    int16_t        *pscratch16 = (int16_t *) scratch32;
    uint32x4_t      vTargetLo = vld1q(pTarget);
    uint32x4_t      vTargetHi = vld1q(pTarget + 4);
    mve_pred16_t    predTailLow = vctp32q(elts);
    mve_pred16_t    predTailHigh = elts - 4 > 0 ? vctp32q(elts - 4) : 0;

    int16x8_t       vXi = GET_Q6INT(ptPoint->X);
    int16x8_t       vYi = GET_Q6INT(ptPoint->Y);

    /* accumulated pixel vectors */
    uint16x8_t      vAvgPixelR, vAvgPixelG, vAvgPixelB;

    /* predicate accumulators */
    /* tracks all predications conditions for selecting final */
    /* averaged pixed / target pixel */
    /* can probably optimized away since averaged target pixel is */
    /* equivalent to original pixel, but precision limitations would introduce small errors */
    mve_pred16_t    predGlbLo = 0, predGlbHi = 0;

    uint16x8_t      R, G, B;


#if     defined(__ARM_2D_HAS_ANTI_ALIAS_TRANSFORM__)  &&  __ARM_2D_HAS_ANTI_ALIAS_TRANSFORM__
    vXi = vsubq_m_n_s16(vXi, vXi, 1, vcmpltq_n_s16(ptPoint->X, 0));
    vYi = vsubq_m_n_s16(vYi, vYi, 1, vcmpltq_n_s16(ptPoint->Y, 0));

    int16x8_t       vOne = vdupq_n_s16(SET_Q6INT(1));
    int16x8_t       vWX = ptPoint->X - (SET_Q6INT(vXi));
    int16x8_t       vWY = ptPoint->Y - (SET_Q6INT(vYi));

    /* combination of Bottom / Top & Left / Right areas contributions */
    uint16x8_t      vAreaTR = vmulq_u16(vWX, vWY);
    uint16x8_t      vAreaTL = vmulq_u16((vOne - vWX), vWY);
    uint16x8_t      vAreaBR = vmulq_u16(vWX, (vOne - vWY));
    uint16x8_t      vAreaBL = vmulq_u16((vOne - vWX), (vOne - vWY));

    /* Q16 conversion */
    vAreaTR = vqshlq_n_u16(vAreaTR, 4);
    vAreaTL = vqshlq_n_u16(vAreaTL, 4);
    vAreaBR = vqshlq_n_u16(vAreaBR, 4);
    vAreaBL = vqshlq_n_u16(vAreaBL, 4);


    /*
     * accumulate / average over the 4 neigbouring pixels
     */


    /* Bottom Left averaging */
    {
        __ARM_2D_RGB888_GET_RGBVEC_FROM_POINT_MASK_ARR(vXi, vYi, R, G, B, vPixelAlpha,
                                                       iOrigStride, pchOrigMask,
                                                       iOrigmaskStride, MASK_STRIDE_SCALE, hwOpacity,
                                                       predGlbLo, predGlbHi);

        __ARM_2D_SCALE_MASK_RGBVEC(vAvgPixelR, vAvgPixelG, vAvgPixelB, vhwTransparency, R,
                                   G, B, vAreaBL, vPixelAlpha);
    }

    /* Bottom Right averaging */
    {
        __ARM_2D_RGB888_GET_RGBVEC_FROM_POINT_MASK_ARR(vaddq_n_s16(vXi, 1), vYi, R, G, B,
                                                       vPixelAlpha, iOrigStride,
                                                       pchOrigMask, iOrigmaskStride, MASK_STRIDE_SCALE,
                                                       hwOpacity, predGlbLo, predGlbHi);

        __ARM_2D_SCALE_MASK_RGBVEC_ACC(vAvgPixelR, vAvgPixelG, vAvgPixelB,
                                       vhwTransparency, R, G, B, vAreaBR, vPixelAlpha);
    }

    /* Top Left averaging */
    {
        __ARM_2D_RGB888_GET_RGBVEC_FROM_POINT_MASK_ARR(vXi, vaddq_n_s16(vYi, 1),
                                                       R, G, B, vPixelAlpha, iOrigStride,
                                                       pchOrigMask, iOrigmaskStride, MASK_STRIDE_SCALE,
                                                       hwOpacity, predGlbLo, predGlbHi);

        __ARM_2D_SCALE_MASK_RGBVEC_ACC(vAvgPixelR, vAvgPixelG, vAvgPixelB,
                                       vhwTransparency, R, G, B, vAreaTL, vPixelAlpha)
    }

    /* Top Right averaging */
    {
        __ARM_2D_RGB888_GET_RGBVEC_FROM_POINT_MASK_ARR(vaddq_n_s16(vXi, 1),
                                                       vaddq_n_s16(vYi, 1), R, G, B,
                                                       vPixelAlpha, iOrigStride,
                                                       pchOrigMask, iOrigmaskStride, MASK_STRIDE_SCALE,
                                                       hwOpacity, predGlbLo, predGlbHi);

        __ARM_2D_SCALE_MASK_RGBVEC_ACC(vAvgPixelR, vAvgPixelG, vAvgPixelB,
                                       vhwTransparency, R, G, B, vAreaTR, vPixelAlpha)
    }

#else

    {
        __ARM_2D_RGB888_GET_RGBVEC_FROM_POINT_MASK_ARR(vXi, vYi, R, G, B, vPixelAlpha,
                                                       iOrigStride, pchOrigMask,
                                                       iOrigmaskStride, MASK_STRIDE_SCALE, hwOpacity,
                                                       predGlbLo, predGlbHi);

        __ARM_2D_SCALE_MASK_RGBVEC(vAvgPixelR, vAvgPixelG, vAvgPixelB, vhwTransparency, R,
                                   G, B, vdupq_n_u16(ONE_Q16), vPixelAlpha);
    }

#endif
    /* alpha blending */
    uint16x8_t      vTargetR, vTargetG, vTargetB;

    __arm_2d_unpack_rgb888_from_mem((const uint8_t *) pTarget, &vTargetR, &vTargetG,
                                    &vTargetB);


    /* merge */
    vAvgPixelR = vqaddq(vAvgPixelR, vmulhq(vTargetR << 8, vhwTransparency));
    vAvgPixelR = vAvgPixelR >> 8;

    vAvgPixelG = vqaddq(vAvgPixelG, vmulhq(vTargetG << 8, vhwTransparency));
    vAvgPixelG = vAvgPixelG >> 8;

    vAvgPixelB = vqaddq(vAvgPixelB, vmulhq(vTargetB << 8, vhwTransparency));
    vAvgPixelB = vAvgPixelB >> 8;

    /* pack */
    __arm_2d_pack_rgb888_to_mem((uint8_t *) scratch32, vAvgPixelR, vAvgPixelG,
                                vAvgPixelB);

    uint32x4_t      TempPixel = vld1q(scratch32);

    /* select between target pixel, averaged pixed */
    TempPixel = vpselq_u32(TempPixel, vTargetLo, predGlbLo);

    vst1q_p(pTarget, TempPixel, predTailLow);

    TempPixel = vld1q(scratch32 + 4);

    /* select between target pixel, averaged pixed */
    TempPixel = vpselq_u32(TempPixel, vTargetHi, predGlbHi);

    vst1q_p(pTarget + 4, TempPixel, predTailHigh);
}

#endif



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
#undef MASK_STRIDE_SCALE
#undef SCALE_BY_OPACITY

#undef __ARM_2D_GRAY8_GET_PIXVEC_FROM_POINT_MASK_ARR_FAR
#undef __ARM_2D_RGB565_GET_RGBVEC_FROM_POINT_MASK_ARRR_FAR
#undef __ARM_2D_RGB888_GET_RGBVEC_FROM_POINT_MASK_ARR

#undef __ARM_2D_SCALE_MASK_GRAY8VEC
#undef __ARM_2D_SCALE_MASK_GRAY8VEC_ACC
#undef __ARM_2D_SCALE_MASK_RGBVEC
#undef __ARM_2D_SCALE_MASK_RGBVEC_ACC

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
#undef __API_MTWM_CFG_SUPPORT_SOURCE_MASK
#undef __API_MTWM_CFG_SUPPORT_TARGET_MASK
#undef __API_MTWM_CFG_SUPPORT_OPATICY
