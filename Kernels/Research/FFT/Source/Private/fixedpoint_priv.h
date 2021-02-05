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

Fixed point operators and specialization for Q15.

*/

#ifndef FIXEDPOINT_PRIV_H
#define FIXEDPOINT_PRIV_H

/***************

OPERATORS

****************/
#include "mode.h"

namespace FFTSCI {

template<int F>
__FORCE_INLINE Q<F> operator+(const Q<F> a, const Q<F> b) {
  return a.value + b.value;
}

template<int F>
__FORCE_INLINE Q<F> operator-(const Q<F> a, const Q<F> b) {
  return a.value - b.value;
}

template<int F>
__FORCE_INLINE PROD<Q<F>> operator*(const Q<F> a, const Q<F> b) {
  return ((PRODSTORAGE<Q<F>>)(a.value) * (PRODSTORAGE<Q<F>>)(b.value));
}






/***************

BUTTERFLY FUNCTIONS

****************/


template<typename T> PROD<T> upcast(T v);

template<>
__FORCE_INLINE Q15 upcast(Q7 t)
{
  return (PRODSTORAGE<Q7>)t.value;
};

template<>
__FORCE_INLINE Q31 upcast(Q15 t)
{
  return (PRODSTORAGE<Q15>)t.value;
};

template<>
__FORCE_INLINE Q63 upcast(Q31 t)
{
  return (PRODSTORAGE<Q31>)t.value;
};



template<typename T> T sat(PROD<T> v);

template<>
__FORCE_INLINE Q31 sat(Q63 v)
{
  return((Q31)clip_q63_to_q31(v.value));
};

template<>
__FORCE_INLINE Q15 sat(Q31 v)
{
  return((Q15)__SSAT(v.value,16));
};

template<>
__FORCE_INLINE Q7 sat(Q15 v)
{
  return((Q7)__SSAT(v.value,8));
};

template<typename T>
__FORCE_INLINE T decq(T v,int shift)
{
   return(v.value >>shift);
};

template<typename T>
__FORCE_INLINE T incq(T v,int shift)
{
   return(sat<T>(((PRODSTORAGE<T>)v.value)<<shift));
};


template<int F>
struct widen<Q<F>>{
__STATIC_FORCEINLINE PROD<Q<F>> run(Q<F> v)
{
   return(((PRODSTORAGE<Q<F>>)v.value) << QPRODUCT<Q<F>>::shift);
}
};


template<int mode,int F>
struct narrow<mode,Q<F>,typename std::enable_if<(mode < 0),int>::type>{
  __STATIC_FORCEINLINE  Q<F> run(PROD<Q<F>> v)
  {
    #ifdef NOSAT
      return(v.value >> QPRODUCT<Q<F>>::shift);
    #else
      return(sat<Q<F>>(v.value >> QPRODUCT<Q<F>>::shift));
    #endif
  }
};

template<int mode,int F>
struct narrow<mode,Q<F>,typename std::enable_if<(mode >= 0),int>::type>
{
  __STATIC_FORCEINLINE Q<F> run(PROD<Q<F>> v)
  {
     #ifdef NOSAT
       return(v.value >> (QPRODUCT<Q<F>>::shift + mode));
     #else
       return(sat<Q<F>>(v.value >> (QPRODUCT<Q<F>>::shift + mode)));
     #endif
  };
};

}

/***************

COMPLEX OPERATORS

****************/

template<int F>
__FORCE_INLINE complex<FFTSCI::Q<F>> decq(complex<FFTSCI::Q<F>> v,int shift)
{
   return complex<FFTSCI::Q<F>>(decq(v.real(),shift),decq(v.imag(), shift));
}

/* Complex * Conjugate complex

Useful for fixed point where -0x8000 is 0x8000.
So instead of computing the conjugate of I 0x8000, the multiplication with the conjugate
is done directly.

*/
template<int F>
__FORCE_INLINE complex<FFTSCI::PROD<FFTSCI::Q<F>>> const multconj(const complex<FFTSCI::Q<F>> a, const complex<FFTSCI::Q<F>> b) {
  return complex<FFTSCI::PROD<FFTSCI::Q<F>>>(a.im * b.im + a.re * b.re, a.im * b.re - a.re * b.im);
}

/* Complex * Complex */
template<int F>
__FORCE_INLINE complex<FFTSCI::PROD<FFTSCI::Q<F>>> const operator*(const complex<FFTSCI::Q<F>> a, const complex<FFTSCI::Q<F>> b) {
  return complex<FFTSCI::PROD<FFTSCI::Q<F>>>(a.re * b.re - a.im * b.im, a.re * b.im + a.im * b.re);
}

/* Complex * scalar */
template<int F>
__FORCE_INLINE complex<FFTSCI::Q<F>> operator*(const complex<FFTSCI::Q<F>> a, const FFTSCI::Q<F> b) {
  return complex<FFTSCI::Q<F>>(FFTSCI::narrow<NORMALNARROW,FFTSCI::Q<F>>::run(a.re * b), FFTSCI::narrow<NORMALNARROW,FFTSCI::Q<F>>::run(a.im * b));
}

template<int F>
__FORCE_INLINE complex<FFTSCI::Q<F>> const shadd(const complex<FFTSCI::Q<F>> a,const complex<FFTSCI::Q<F>> b)
{
  complex<FFTSCI::Q<F>> r= decq(a,1) + decq(b,1);

  return r;
}

template<int F>
__FORCE_INLINE complex<FFTSCI::Q<F>> const shsub(const complex<FFTSCI::Q<F>> a,const complex<FFTSCI::Q<F>> b)
{
  complex<FFTSCI::Q<F>> r= decq(a,1) - decq(b,1);

  return r;
}


template<>
struct constant<FFTSCI::Q31>{
    static constexpr FFTSCI::Q31 zero{0};
};

template<>
struct constant<FFTSCI::Q15>{
    static constexpr FFTSCI::Q15 zero{0};
};

template<>
struct constant<FFTSCI::Q7>{
    static constexpr FFTSCI::Q7 zero{0};
};

/***************

Specialized implementations

****************/

#ifdef DSPE_Q15
  

  /* DSP EXTENSION IMPLEMENTATION FOR Q15 */
template <>
  struct complex<FFTSCI::Q15> {

    q31_t value;  

    constexpr complex(const FFTSCI::Q15& re = FFTSCI::Q15(), const FFTSCI::Q15& im = FFTSCI::Q15()):value(__PKHBT(re.value, im.value, 16)) {};
    constexpr complex(const q31_t& v):value(v) {};

    constexpr complex(const complex& c):value(c.value){}; 

    FFTSCI::Q15 real() const {return FFTSCI::Q15((q15_t)value);};
    FFTSCI::Q15 imag() const {return FFTSCI::Q15((q15_t)(value >> 16));};
    
    __FORCE_INLINE complex& operator=(const complex& c){
         value = c.value ;

         return(*this);
    };

#ifdef DEBUG
    void p(const char*s)
    {
      printf("%s : %f + i %f\n",s,this->real().f(),this->imag().f());
    }
#endif

    __FORCE_INLINE complex const operator-() const {
        return complex(__QSUB16(0,value));
    }



};


template<> 
struct FFTSCI::memory<complex<FFTSCI::Q15>> {
    __STATIC_FORCEINLINE complex<FFTSCI::Q15> load(const complex<FFTSCI::Q15> *buf, const int index)
    {
      return(complex<FFTSCI::Q15>(read_q15x2((q15_t*)buf+(index<<1))));
    };
    __STATIC_FORCEINLINE void store(complex<FFTSCI::Q15> *buf,const int index,complex<FFTSCI::Q15> v)
    {
       write_q15x2 ((q15_t*)buf + (index<<1), v.value);
    };
};


template<>
__FORCE_INLINE complex<FFTSCI::Q15> const operator+(const complex<FFTSCI::Q15> a, const complex<FFTSCI::Q15> b) {
  return complex<FFTSCI::Q15>(__QADD16(a.value,b.value));
}

template<>
__FORCE_INLINE complex<FFTSCI::Q15> const operator-(const complex<FFTSCI::Q15> a, const complex<FFTSCI::Q15> b) {
  return complex<FFTSCI::Q15>(__QSUB16(a.value,b.value));
}

/* Complex * Complex */
template<>
__FORCE_INLINE complex<FFTSCI::Q31> const multconj(const complex<FFTSCI::Q15> a, const complex<FFTSCI::Q15> b) {
     q31_t re,im;
     re = __SMUAD(a.value, b.value);
     im = __SMUSDX(b.value, a.value);
     
     return complex<FFTSCI::Q31>(re,im);
}

template<>
__FORCE_INLINE complex<FFTSCI::Q31> const operator*(const complex<FFTSCI::Q15> a, const complex<FFTSCI::Q15> b) {
     q31_t re,im;
     re = __SMUSD(a.value, b.value) ;
     im = __SMUADX(b.value, a.value);
     
     return complex<FFTSCI::Q31>(re,im);
}

template<>
__FORCE_INLINE complex<FFTSCI::Q15> operator*(const complex<FFTSCI::Q15> a, 
  const FFTSCI::Q15 b) {
     q31_t re,im;
     re = a.real().value * b.value;
     im = a.imag().value * b.value;


     #ifdef NOSAT
       return complex<FFTSCI::Q15>(re>>15,im>>15);
     #else
      return complex<FFTSCI::Q15>(FFTSCI::sat<FFTSCI::Q15>(re>>15),FFTSCI::sat<FFTSCI::Q15>(im>>15));
     #endif
}


template<>
__FORCE_INLINE complex<FFTSCI::Q15> const rot(const complex<FFTSCI::Q15> a)
{
  q31_t in1=__QASX(0,a.value);
  return complex<FFTSCI::Q15>(in1);
}

template<>
__FORCE_INLINE complex<FFTSCI::Q15> const conjugate(const complex<FFTSCI::Q15> a)
{
  q31_t in1=__QASX(0,a.value);
  in1 = ((uint32_t) in1 >> 16) | ((uint32_t) in1 << 16);

  return complex<FFTSCI::Q15>(in1);
}

#ifdef FASTER
template<>
__FORCE_INLINE complex<FFTSCI::Q15> const shadd(const complex<FFTSCI::Q15> a,const complex<FFTSCI::Q15> b)
{
  return complex<FFTSCI::Q15>( __SHADD16(__QADD16(a.value,b.value),0));
}

template<>
__FORCE_INLINE complex<FFTSCI::Q15> const shsub(const complex<FFTSCI::Q15> a,const complex<FFTSCI::Q15> b)
{
  return complex<FFTSCI::Q15>( __SHADD16(__QSUB16(a.value,b.value),0));
}
#else
template<>
__FORCE_INLINE complex<FFTSCI::Q15> const shadd(const complex<FFTSCI::Q15> a,const complex<FFTSCI::Q15> b)
{
  return complex<FFTSCI::Q15>( __QADD16(__SHADD16(a.value,0),__SHADD16(b.value,0)));
}

template<>
__FORCE_INLINE complex<FFTSCI::Q15> const shsub(const complex<FFTSCI::Q15> a,const complex<FFTSCI::Q15> b)
{
  return complex<FFTSCI::Q15>( __QSUB16(__SHADD16(a.value,0),__SHADD16(b.value,0)));
}
#endif


#endif 

#endif 

