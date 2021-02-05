/* ----------------------------------------------------------------------
*
* Copyright (C) 2010-2021 ARM Limited or its affiliates. All rights reserved.
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

/*

Vector datatypes and templates for Helium

*/

#ifndef _VECTOR_HELIUM_H_
#define _VECTOR_HELIUM_H_

/**************

VECTOR DESCRIPTIONS

***************/

#if defined(VECTOR_F32)

template<>
struct vector<float32_t>
{
    typedef f32x4_t type;
    typedef uint32x4_t offset_type;


     /* Arguments are offsets in arrays of complex values hence the factor 2.
       Access to 2 complexes at index with stride.
       So {index,index+1} offset in array of complexes with stride and
       where the real and imaginary parts are contiguous.

      */
    __STATIC_FORCEINLINE offset_type complex_offset(const int stride,const int index){
         const uint32_t off[4]={2*sizeof(float32_t)*stride*index
                               ,sizeof(float32_t)*(2*stride*index+1)
                               ,2*sizeof(float32_t)*stride*(index+1)
                               ,sizeof(float32_t)*(2*stride*(index+1)+1)};

         return(vld1q(off));
    };

    __STATIC_FORCEINLINE type gather_offset(float32_t* p,const int stride,const int index)
    {
            offset_type voffset=complex_offset(stride,index);
            return(vldrwq_gather_offset(p,voffset));
    };

    __STATIC_FORCEINLINE void scatter_offset(float32_t* p,const int stride,const int index, type value)
    {
            offset_type voffset=complex_offset(stride,index);
            vstrwq_scatter_offset(p,voffset,value);
    };

    enum {
           vector_enabled = 1
    };

    /* Number of complex numbers in the vector and related
    shift and masks for the loop counters. */
    enum {
           nb_complexes = 2,
           loop_shift = 1,
           tail_mask = 1
    };
};
#endif /* defined(VECTOR_F32) */

#if defined(VECTOR_F16)

template<>
struct vector<float16_t>
{
    typedef f16x8_t type;
    typedef uint16x8_t offset_type;


     /* Arguments are offsets in arrays of complex values hence the factor 2.
       Access to 2 complexes at index with stride.

       So {index,index+1, index+2, index+3} offset in array of complexes with stride and
       where the real and imaginary parts are contiguous.

      */
    __STATIC_FORCEINLINE offset_type complex_offset(const int stride,const int index){
         const uint16_t off[8]={ static_cast<uint16_t>(2*(2*stride*(index+0)))
                               , static_cast<uint16_t>(2*(2*stride*(index+0)+1))
                               , static_cast<uint16_t>(2*(2*stride*(index+1)))
                               , static_cast<uint16_t>(2*(2*stride*(index+1)+1))
                               , static_cast<uint16_t>(2*(2*stride*(index+2)))
                               , static_cast<uint16_t>(2*(2*stride*(index+2)+1))
                               , static_cast<uint16_t>(2*(2*stride*(index+3)))
                               , static_cast<uint16_t>(2*(2*stride*(index+3)+1))
                               };

         return(vld1q(off));
    };

    __STATIC_FORCEINLINE type gather_offset(float16_t* p,const int stride,const int index)
    {
            offset_type voffset=complex_offset(stride,index);
            return(vldrhq_gather_offset(p,voffset));
    };

    __STATIC_FORCEINLINE void scatter_offset(float16_t* p,const int stride,const int index, type value)
    {
            offset_type voffset=complex_offset(stride,index);
            vstrhq_scatter_offset(p,voffset,value);
    };

    enum {
           vector_enabled = 1
    };

    /* Number of complex numbers in the vector and related
    shift and masks for the loop counters. */
    enum {
           nb_complexes = 4,
           loop_shift = 2,
           tail_mask = 3
    };
};
#endif /* defined(VECTOR_F16) */

/*

load/store with no stride.

*/
template<typename T> 
struct vmemory<T,0>{
    __STATIC_FORCEINLINE VECTORTYPE<T> load(const complex<T> *buf, const int index, const int stride)
    {
       (void)stride;
       return(vld1q((T*)(buf + index)));
    };

    __STATIC_FORCEINLINE void store(complex<T> *buf,const int index,VECTORTYPE<T> v, const int stride)
    {
       (void)stride;
        vst1q((T*)(buf + index), v);
    };
};

/*

Load/store with stride

*/
template<typename T> 
struct vmemory<T,1>{
    __STATIC_FORCEINLINE VECTORTYPE<T> load(const complex<T> *buf, const int index, const int stride)
    {

       return(vector<T>::gather_offset((T *)buf,stride,index));
    };

    __STATIC_FORCEINLINE void store(complex<T> *buf,const int index,VECTORTYPE<T> v, const int stride)
    {
       vector<T>::scatter_offset((T*)buf, stride,index,v);
    };
};


/*************

HELIUM IMPLEMENTATIONS

**************/


template<typename T>
__FORCE_INLINE T vnegate(T v)
{
    return(vnegq(v));
};

template<typename T> 
__FORCE_INLINE VECTORTYPE<T> vload(T *v)
{
    return(vld1q(v));
};


#if defined(VECTOR_F16)

static constexpr f16x8_t cmplx_conj_sign_f16 = { 1.0f, -1.0f, 1.0f, -1.0f,1.0f, -1.0f, 1.0f, -1.0f };
static constexpr f16x8_t cmplx_rot_f16 = { 0.0f16, 1.0f16, 0.0f16, 1.0f16,0.0f16, 1.0f16, 0.0f16, 1.0f16 };



template<>
__FORCE_INLINE float16x8_t vrot<float16x8_t>(float16x8_t v)
{
    return(vcmulq_rot90(cmplx_rot_f16,v));
};

template<>
__FORCE_INLINE float16x8_t vconjugate<float16x8_t>(float16x8_t v)
{
    return(vmulq(cmplx_conj_sign_f16,v));
};

template<>
__FORCE_INLINE float16x8_t vcomplex_mulq<float16x8_t>(float16x8_t a,float16x8_t b)
{
    float16x8_t vec_acc = vcmulq(a, b);
    vec_acc = vcmlaq_rot90(vec_acc, a, b);
    return(vec_acc);
};

template<>
__FORCE_INLINE float16x8_t vcomplex_mulq_n<float16x8_t,complex<float16_t>>(float16x8_t a,complex<float16_t> b)
{
    float16x8_t tmp;
    tmp[0]=b.real();
    tmp[1]=b.imag();
    tmp[2]=b.real();
    tmp[3]=b.imag();
    tmp[4]=b.real();
    tmp[5]=b.imag();
    tmp[6]=b.real();
    tmp[7]=b.imag();

    return(vcomplex_mulq(a,tmp));
};
#endif /* defined(VECTOR_F16) */

#if defined(VECTOR_F32)

static constexpr f32x4_t cmplx_conj_sign_f32 = { 1.0f, -1.0f, 1.0f, -1.0f };
static constexpr f32x4_t cmplx_rot_f32 = { 0.0f, 1.0f, 0.0f, 1.0f };

template<>
__FORCE_INLINE float32x4_t vrot<float32x4_t>(float32x4_t v)
{
    return(vcmulq_rot90(cmplx_rot_f32,v));
};

template<>
__FORCE_INLINE float32x4_t vconjugate<float32x4_t>(float32x4_t v)
{
    return(vmulq(cmplx_conj_sign_f32,v));
};

template<>
__FORCE_INLINE float32x4_t vcomplex_mulq<float32x4_t>(float32x4_t a,float32x4_t b)
{
    float32x4_t vec_acc = vcmulq(a, b);
    vec_acc = vcmlaq_rot90(vec_acc, a, b);
    return(vec_acc);
};

template<>
__FORCE_INLINE float32x4_t vcomplex_mulq_n<float32x4_t,complex<float32_t>>(float32x4_t a,complex<float32_t> b)
{
    float32x4_t tmp;
    tmp[0]=b.real();
    tmp[1]=b.imag();
    tmp[2]=b.real();
    tmp[3]=b.imag();

    return(vcomplex_mulq(a,tmp));
};
#endif /* #if defined(VECTOR_F32) */


/*

A FFT permutation may not always be vectorizable.
So there is an argument in the function to know if the
vector or scalar version must be used.

A FFT permutation is vectorizable only if the sequence of
transpositions are (when taken by pair) independent.

It is detected in the Python configuration code.

*/
template<int inputstrideenabled>
struct bitreversal<float32_t,inputstrideenabled,1>{
  __STATIC_FORCEINLINE void run(
        float32_t *pSrc,
  const int bitRevLen,
  const uint16_t *pBitRevTab,
  const uint16_t isVectorizable,
  const int inputstride)
{
  float32_t tmp;
  uint16_t *pBitRevTabTmp = (uint16_t *)pBitRevTab;
  int a,b;
  int index=0;
  
  uint64_t       *src = (uint64_t *) pSrc;
  uint32_t        blkCnt;     /* loop counters */
  uint32x4_t      bitRevTabOff;

  const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));
  uint32x4_t     factor = vdupq_n_u32(4*strideFactor);

  if (isVectorizable)
  {
     blkCnt = bitRevLen >> 2; 
  }
  else
  {
    blkCnt = 0;
  }
  while(blkCnt >0)
  {
    bitRevTabOff = vldrhq_u32(pBitRevTabTmp);
    pBitRevTabTmp += 4;


    uint64x2_t      bitRevOff1 = vmullbq_int_u32(bitRevTabOff, factor);
    uint64x2_t      bitRevOff2 = vmulltq_int_u32(bitRevTabOff, factor);

    uint64x2_t      in1 = vldrdq_gather_offset_u64(src, bitRevOff1);
    uint64x2_t      in2 = vldrdq_gather_offset_u64(src, bitRevOff2);

    vstrdq_scatter_offset_u64(src, bitRevOff1, in2);
    vstrdq_scatter_offset_u64(src, bitRevOff2, in1);

    blkCnt --;
    index += 4;
  }

  if (isVectorizable)
  {
     blkCnt = (bitRevLen >> 1) & 1;
  }
  else
  {
    blkCnt = bitRevLen >> 1;
  }
  while(blkCnt>0)
  {
     /* Index in a complex array. So they have been pre-multiplied by 2
     in the Python script */
     a = pBitRevTab[index    ] ;
     b = pBitRevTab[index + 1] ;

     //real
     tmp = pSrc[a * strideFactor];
     pSrc[a * strideFactor] = pSrc[b * strideFactor];
     pSrc[b * strideFactor] = tmp;

     //complex
     tmp = pSrc[a * strideFactor + 1];
     pSrc[a * strideFactor + 1] = pSrc[b * strideFactor + 1];
     pSrc[b * strideFactor + 1] = tmp;

     index +=2;
     blkCnt--;
  }
}
};



template<int inputstrideenabled>
struct bitreversal<float16_t,inputstrideenabled,1>{
  __STATIC_FORCEINLINE void run(
        float16_t *pSrc,
  const int bitRevLen,
  const uint16_t *pBitRevTab,
  const uint16_t isVectorizable,
  const int inputstride)
{
  float16_t tmp;
  int a,b;
  int index=0;
  uint16_t *pBitRevTabTmp = (uint16_t *)pBitRevTab;

  uint32_t       *src = (uint32_t *) pSrc;
  uint32_t        blkCnt;     /* loop counters */
  uint16x8_t      bitRevTabOff;

  const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));
  uint16x8_t     factor = vdupq_n_u16(strideFactor*2);

  if (isVectorizable)
  {
     blkCnt = bitRevLen >> 3; 
  }
  else
  {
    blkCnt = 0;
  }

  while(blkCnt >0)
  {
    bitRevTabOff = vldrhq_u16(pBitRevTabTmp);
    pBitRevTabTmp += 8;

    uint32x4_t      bitRevOff1 = vmullbq_int_u16(bitRevTabOff, factor);
    uint32x4_t      bitRevOff2 = vmulltq_int_u16(bitRevTabOff, factor);

    uint32x4_t      in1 = vldrwq_gather_offset_u32(src, bitRevOff1);
    uint32x4_t      in2 = vldrwq_gather_offset_u32(src, bitRevOff2);

    vstrwq_scatter_offset_u32(src, bitRevOff1, in2);
    vstrwq_scatter_offset_u32(src, bitRevOff2, in1);

    blkCnt --;
    index += 8;
  }

  if (isVectorizable)
  {
     blkCnt = (bitRevLen >> 1) & 3;
  }
  else
  {
    blkCnt = bitRevLen >> 1;
  }
  while(blkCnt>0)
  {
     /* Index in a complex array. So they have been pre-multiplied by 2
     in the Python script */
     a = pBitRevTab[index    ] ;
     b = pBitRevTab[index + 1] ;

     //real
     tmp = pSrc[a * strideFactor];
     pSrc[a * strideFactor] = pSrc[b * strideFactor];
     pSrc[b * strideFactor] = tmp;

     //complex
     tmp = pSrc[a * strideFactor + 1];
     pSrc[a * strideFactor + 1] = pSrc[b * strideFactor + 1];
     pSrc[b * strideFactor + 1] = tmp;

     index +=2;
     blkCnt--;
  }
}
};

#ifdef DATATYPE_F32
    #if defined(VECTOR_F32)
        #include "Helium/F32/radix_F32_2.h"
        #include "Helium/F32/radix_F32_3.h"
        #include "Helium/F32/radix_F32_4.h"
        #include "Helium/F32/radix_F32_5.h"
        #include "Helium/F32/radix_F32_6.h"
        #include "Helium/F32/radix_F32_8.h"
    #endif
#endif

#ifdef DATATYPE_F16
    #if defined(VECTOR_F16)
        #include "Helium/F16/radix_F16_2.h"
        #include "Helium/F16/radix_F16_3.h"
        #include "Helium/F16/radix_F16_4.h"
        #include "Helium/F16/radix_F16_5.h"
        #include "Helium/F16/radix_F16_6.h"
        #include "Helium/F16/radix_F16_8.h"
    #endif
#endif

#endif /* _VECTOR_HELIUM_H_ */