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
#ifndef BUTTERFLY_H
#define BUTTERFLY_H

/*

Functions used for the implementation of butterflies and
some other functions like the rfft split/merge.

*/

template<typename> struct constant;

template<>
struct constant<float64_t>{
    static constexpr float64_t zero=0.0;
};

template<>
struct constant<float32_t>{
    static constexpr float32_t zero=0.0f;
};

#if defined(ARM_FLOAT16_SUPPORTED)
template<>
struct constant<float16_t>{
    static constexpr float16_t zero=0.0f16;
};
#endif


namespace FFTSCI {

template<typename T> 
struct memory {
    __STATIC_FORCEINLINE T load(const T *buf, const int index)
    {
      return(buf[index]);
    };
    __STATIC_FORCEINLINE void store(T *buf,const int index,T value)
    {
       buf[index]=value;
    };
};



template<>
struct QPRODUCT<float32_t> {
   typedef float32_t type;
   enum {
      shift = 0
   };
};

#if defined(ARM_FLOAT16_SUPPORTED)
template<>
struct QPRODUCT<float16_t> {
   typedef float16_t type;
   enum {
      shift = 0
   };
};
#endif 

#define NORMALNARROW (-1)

template<typename T>
struct widen;


template <>
struct widen<float32_t>{
__STATIC_FORCEINLINE PROD<float32_t> run(float32_t v)
{
   return(v);
}
};

#if defined(ARM_FLOAT16_SUPPORTED)
template <>
struct widen<float16_t>{
__STATIC_FORCEINLINE PROD<float16_t> run(float16_t v)
{
   return(v);
}
};
#endif

template<int mode,typename T,typename = int>
struct narrow;





}


#endif