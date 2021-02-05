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

#ifndef _VECTOR_NEON_H_
#define _VECTOR_NEON_H_

/**************

VECTOR DESCRIPTIONS

***************/

/*

vaddq is not polymorphic in Neon.
We do not want to have to write vaddq_f32, vaddq_f16 ...

So a template is used.

*/
template<typename T> __FORCE_INLINE T vaddition(T a, T b);
template<typename T> __FORCE_INLINE T vsubstraction(T a, T b);


#if defined(VECTOR_F32)

template<>
struct vector<float32_t>
{
    typedef f32x4x2_t type;
    typedef uint32x4_t offset_type;


    /* Scatter / gather To rework to see if a more efficient implementation
       can be done 
    */
    __STATIC_FORCEINLINE type gather_offset(float32_t* p,const int stride,const int index)
    {
            f32x4x2_t res;

            res.val[0] = vsetq_lane_f32(p[2*stride*(index + 0)],res.val[0],0);
            res.val[0] = vsetq_lane_f32(p[2*stride*(index + 1)],res.val[0],1);
            res.val[0] = vsetq_lane_f32(p[2*stride*(index + 2)],res.val[0],2);
            res.val[0] = vsetq_lane_f32(p[2*stride*(index + 3)],res.val[0],3);

            res.val[1] = vsetq_lane_f32(p[2*stride*(index + 0) + 1],res.val[1],0);
            res.val[1] = vsetq_lane_f32(p[2*stride*(index + 1) + 1],res.val[1],1);
            res.val[1] = vsetq_lane_f32(p[2*stride*(index + 2) + 1],res.val[1],2);
            res.val[1] = vsetq_lane_f32(p[2*stride*(index + 3) + 1],res.val[1],3);

            return(res);
     };

    __STATIC_FORCEINLINE void scatter_offset(float32_t* p,const int stride,const int index, type value)
    {
            p[2*stride*(index + 0)] = vgetq_lane_f32(value.val[0],0);
            p[2*stride*(index + 1)] = vgetq_lane_f32(value.val[0],1);
            p[2*stride*(index + 2)] = vgetq_lane_f32(value.val[0],2);
            p[2*stride*(index + 3)] = vgetq_lane_f32(value.val[0],3);

            p[2*stride*(index + 0) + 1] = vgetq_lane_f32(value.val[1],0);
            p[2*stride*(index + 1) + 1] = vgetq_lane_f32(value.val[1],1);
            p[2*stride*(index + 2) + 1] = vgetq_lane_f32(value.val[1],2);
            p[2*stride*(index + 3) + 1] = vgetq_lane_f32(value.val[1],3);
    };

    enum {
           vector_enabled = 1
    };

    /* Number of complex numbers in the vector and related
    shift and masks for the loop counters. 

    We load 4 complexes into two registers
    */
    enum {
           nb_complexes = 4,
           loop_shift = 2,
           tail_mask = 3
    };
};
#endif





template<typename T> 
struct vmemory<T,0>{
    __STATIC_FORCEINLINE VECTORTYPE<T> load(const complex<T> *buf, const int index, const int stride)
    {
       (void)stride;
       return(vld2q_f32((T*)(buf + index)));
    };

    __STATIC_FORCEINLINE void store(complex<T> *buf,const int index,VECTORTYPE<T> v, const int stride)
    {
       (void)stride;
        vst2q_f32((T*)(buf + index), v);
    };
};


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


template<>
__FORCE_INLINE f32x4x2_t vaddition(f32x4x2_t a,f32x4x2_t b)
{
    f32x4x2_t res;

    res.val[0] = vaddq_f32(a.val[0],b.val[0]);
    res.val[1] = vaddq_f32(a.val[1],b.val[1]);
    return(res);
};

template<>
__FORCE_INLINE f32x4x2_t vsubstraction(f32x4x2_t a,f32x4x2_t b)
{
    f32x4x2_t res;

    res.val[0] = vsubq_f32(a.val[0],b.val[0]);
    res.val[1] = vsubq_f32(a.val[1],b.val[1]);
    return(res);
};


template<>
__FORCE_INLINE f32x4x2_t vnegate(f32x4x2_t v)
{
    f32x4x2_t res;

    res.val[0] = vnegq_f32(v.val[0]);
    res.val[1] = vnegq_f32(v.val[1]);
    return(res);
};


template<> 
__FORCE_INLINE f32x4x2_t vload(float32_t *v)
{
    return(vld2q_f32(v));
};

template<>
__FORCE_INLINE f32x4x2_t vrot<f32x4x2_t>(f32x4x2_t v)
{   
    f32x4x2_t res;

    res.val[0] = vnegq_f32(v.val[1]);
    res.val[1] = v.val[0];
    return(res);
};

template<>
__FORCE_INLINE f32x4x2_t vconjugate<f32x4x2_t>(f32x4x2_t v)
{
    f32x4x2_t res=v;
    res.val[1] = vsubq_f32(vdupq_n_f32(0.0f),res.val[1]);
    return(res);
};

template<>
__FORCE_INLINE f32x4x2_t vcomplex_mulq<f32x4x2_t>(f32x4x2_t a,f32x4x2_t b)
{
    f32x4x2_t res;

    res.val[0] = vmulq_f32(a.val[0], b.val[0]);
    res.val[0] = vmlsq_f32(res.val[0], a.val[1], b.val[1]);

    res.val[1] = vmulq_f32(a.val[0], b.val[1]);
    res.val[1] = vmlaq_f32(res.val[1], a.val[1], b.val[0]);
    return(res);
};

template<>
__FORCE_INLINE f32x4x2_t vcomplex_mulq_n<f32x4x2_t,complex<float32_t>>(f32x4x2_t a,complex<float32_t> b)
{
    f32x4x2_t tmp;

    tmp.val[0] = vdupq_n_f32(b.real());
    tmp.val[1] = vdupq_n_f32(b.imag());

    return(vcomplex_mulq(a,tmp));
};

/*


This vector version (1 is last template argument) has not yet
been vectorized.

*/
template<typename T,int inputstrideenabled>
struct bitreversal<T,inputstrideenabled,1>{
__STATIC_FORCEINLINE void run(
        T *pSrc,
  const int bitRevLen,
  const uint16_t *pBitRevTab,
  const uint16_t isVectorizable,
  const int inputstride)
{
  T tmp;
  int a,b;
  int i;
  (void)isVectorizable;
  
  const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));
    

  for (i = 0; i < bitRevLen; )
  {
     /* Index in a complex array. So they have been pre-multiplied by 2
     in the Python script */
     a = pBitRevTab[i    ] ;
     b = pBitRevTab[i + 1] ;

     //real
     tmp = pSrc[a * strideFactor];
     pSrc[a * strideFactor] = pSrc[b * strideFactor];
     pSrc[b * strideFactor] = tmp;

     //complex
     tmp = pSrc[a * strideFactor + 1];
     pSrc[a * strideFactor + 1] = pSrc[b * strideFactor + 1];
     pSrc[b * strideFactor + 1] = tmp;

     i += 2;
  }
}
};

#ifdef DATATYPE_F32
    #if defined(VECTOR_F32)
        #include "Neon/F32/radix_F32_2.h"
        #include "Neon/F32/radix_F32_3.h"
        #include "Neon/F32/radix_F32_4.h"
        #include "Neon/F32/radix_F32_5.h"
        #include "Neon/F32/radix_F32_6.h"
        #include "Neon/F32/radix_F32_8.h"
    #endif
#endif

#endif /* _VECTOR_NEON_H_ */