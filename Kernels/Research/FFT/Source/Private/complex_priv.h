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
#ifndef COMPLEX_PRIV_H
#define  COMPLEX_PRIV_H

/*


Complex operators and specializations for f15 and f32

*/

#include "complex.h"

#ifdef DEBUG
template<typename T>
void debug(const char*s, complex<T> v)
{
   printf("%s : %f + i %f\n",s,v.real().f(),v.imag().f());
};
#endif

template<typename T>
__FORCE_INLINE complex<T> const operator+(const complex<T> a, const complex<T> b) {
  return complex<T>(a.re + b.re, a.im + b.im);
}

template<typename T>
__FORCE_INLINE complex<T> const operator-(const complex<T> a, const complex<T> b) {
  return complex<T>(a.re - b.re, a.im - b.im);
}


/* Complex * Conjugate Complex */
template<typename T>
__FORCE_INLINE complex<FFTSCI::PROD<T>> const multconj(const complex<T> a, const complex<T> b) {
  return complex<T>(a.im * b.im + a.re * b.re, a.im * b.re - a.re * b.im);
}



/* Complex * Complex */
template<typename T>
__FORCE_INLINE complex<FFTSCI::PROD<T>> const operator*(const complex<T> a, const complex<T> b) {
  return complex<T>(a.re * b.re - a.im * b.im, a.re * b.im + a.im * b.re);
}

/* Complex scalar */
template<typename T>
__FORCE_INLINE complex<T> const operator*(const complex<T> a, const T b) {
  return complex<T>(a.re * b, a.im * b);
}

/* MAC */
template<typename T>
__FORCE_INLINE complex<FFTSCI::PROD<T>> const MAC(const complex<FFTSCI::PROD<T>> a, const complex<T> b,const  complex<T> c) {
  return (a + b * c);
}

/* MAS */
template<typename T>
__FORCE_INLINE complex<FFTSCI::PROD<T>> const MAS(const complex<FFTSCI::PROD<T>> a, const complex<T> b,const  complex<T> c) {
  return (a - b * c);
}

template<typename T>
__FORCE_INLINE complex<T> const rot(const complex<T> a)
{
  return complex<T>( -a.im ,  a.re);
}

template<typename T>
__FORCE_INLINE complex<T> const conjugate(const complex<T> a)
{
  return complex<T>( a.re ,  -a.im);
}



/*

float16_t


*/
#if defined(ARM_FLOAT16_SUPPORTED)
template<>
__FORCE_INLINE complex<float16_t> const operator+(const complex<float16_t> a, const complex<float16_t> b) {
  return complex<float16_t>((_Float16)a.re + (_Float16)b.re, (_Float16)a.im + (_Float16)b.im);
}

template<>
__FORCE_INLINE complex<float16_t> const operator-(const complex<float16_t> a, const complex<float16_t> b) {
  return complex<float16_t>((_Float16)a.re - (_Float16)b.re, (_Float16)a.im - (_Float16)b.im);
}

template<>
__FORCE_INLINE complex<float16_t> const complex<float16_t>::operator-() const {
  return complex<float16_t>(-(_Float16)re , -(_Float16)im);
}

template<>
__FORCE_INLINE complex<float16_t> const operator*(const complex<float16_t> a, const complex<float16_t> b) {
  return complex<float16_t>((_Float16)a.re * (_Float16)b.re - (_Float16)a.im * (_Float16)b.im, (_Float16)a.re * (_Float16)b.im + (_Float16)a.im * (_Float16)b.re);
}

__FORCE_INLINE complex<float16_t> const operator*(const complex<float16_t> a, const float16_t b) {
  return complex<float16_t>((_Float16)a.re * (_Float16)b, (_Float16)a.im * (_Float16)b);
}

template<>
__FORCE_INLINE complex<float16_t> const rot(const complex<float16_t> a)
{
  return complex<float16_t>(-(_Float16)a.im , (_Float16)a.re);
}

template<>
__FORCE_INLINE complex<float16_t> const conjugate(const complex<float16_t> a)
{
  return complex<float16_t>((_Float16)a.re , -(_Float16)a.im);
}


namespace FFTSCI {

template<int mode>
struct narrow<mode,float16_t,typename std::enable_if<(mode < 0),int>::type>{
  __STATIC_FORCEINLINE float16_t run(float16_t v)
  {
     return(v);
  }
};

template<int mode>
struct narrow<mode,float16_t,typename std::enable_if<(mode >= 0),int>::type>
{
  __STATIC_FORCEINLINE float16_t run(float16_t v)
  {
     return(v / (1 << mode));
  };
};

}
#endif 

namespace FFTSCI {
template<typename T>
struct widen<complex<T>>{
__STATIC_FORCEINLINE complex<PROD<T>> run(complex<T> v)
{
     PROD<T> re = widen<T>::run(v.real());
     PROD<T> im = widen<T>::run(v.imag());
     return(complex<PROD<T>>(re,im));
}
};

template<int mode>
struct narrow<mode,float32_t,typename std::enable_if<(mode < 0),int>::type>{
  __STATIC_FORCEINLINE float32_t run(float32_t v)
  {
     return(v);
  }
};

template<int mode>
struct narrow<mode,float32_t,typename std::enable_if<(mode >= 0),int>::type>
{
  __STATIC_FORCEINLINE float32_t run(float32_t v)
  {
     return(v / (1 << mode));
  };
};



template<int mode,typename T>
struct narrow<mode,complex<T>,typename std::enable_if<(mode < 0),int>::type>{
  __STATIC_FORCEINLINE  complex<T> run(complex<PROD<T>> v)
  {
     T re = narrow<mode,T>::run(v.re);
     T im = narrow<mode,T>::run(v.im);
     return(complex<T>(re,im));
  }
};

template<int mode,typename T>
struct narrow<mode,complex<T>,typename std::enable_if<(mode > 0),int>::type>{
  __STATIC_FORCEINLINE  complex<T> run(complex<PROD<T>> v)
  {
     T re = narrow<mode,T>::run(v.re);
     T im = narrow<mode,T>::run(v.im);
     return(complex<T>(re,im));
  }
};


}


#endif