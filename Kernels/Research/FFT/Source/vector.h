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

Vectorized API for the radix

*/

#ifndef _VECTOR_H_
#define _VECTOR_H_

/**************

VECTOR DESCRIPTIONS

***************/

/*******************

VECTORIZED RADIX


********************/
/*

The only difference from scalar APIs is that
the twiddle is using the vector type.

*/
template <int mode,typename T,int factor,int inputstrideenabled>
struct radixVector{

void run_ip_tw(int stageLength,
  const VECTORTYPE<T> *w,
  complex<T> *in,
  int inputstride
  );

void run_op_tw(int stageLength,
  const VECTORTYPE<T> *w,
  const complex<T> *in,
  complex<T> *out,
  int inputstride);

void run_ip_notw(int stageLength,
  complex<T> *in,
  int inputstride);

void run_op_notw(int stageLength,
  const complex<T> *in,
  complex<T> *out,
  int inputstride);

void run_ip_last(complex<T> *in,int inputstride);

void run_op_last(const complex<T> *in,
  complex<T> *out,int inputstride);

};

/********

VECTOR INTRINSICS

Used to have mostly the same C++ code for Helium and Neon

**********/

template<typename T> __FORCE_INLINE VECTORTYPE<T> vload(T *t);
template<typename T> __FORCE_INLINE T vnegate(T t);
template<typename T> __FORCE_INLINE T vrot(T t);
template<typename T> __FORCE_INLINE T vconjugate(T t);
template<typename T> __FORCE_INLINE T vcomplex_mulq(T, T);
template<typename T,typename S> __FORCE_INLINE T vcomplex_mulq_n(T,S);

template<typename T,int hasstride> 
struct vmemory{
    __STATIC_FORCEINLINE VECTORTYPE<T> load(const complex<T> *buf, const int index, const int stride);
    __STATIC_FORCEINLINE void store(complex<T> *buf,const int index,VECTORTYPE<T> v, const int stride);
};

#if defined(HELIUM_VECTOR)
#include "vector_helium.h"
#endif 

#if defined(NEON_VECTOR)
#include "vector_neon.h"
#endif



#endif