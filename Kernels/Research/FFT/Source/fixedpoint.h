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

/*******************

Fixed point arithmetic

*******************/

#ifndef FIXEDPOINT_H
#define FIXEDPOINT_H

#include "dsp/none.h"



namespace FFTSCI {

  namespace internal {
  
    // Define the storage of different Q formats
    template <int bits,typename = int> struct storage;
    
      template <int bits>
      struct storage<bits,typename std::enable_if<((0 <= bits) && (bits <= 8)),int>::type> {
          enum {
           storageSize = 8,
          };
          typedef char type;
      };
      
      template <int bits>
      struct storage<bits,typename std::enable_if< ((9 <= bits) && (bits <= 16)),int>::type> {
          enum {
           storageSize = 16,
          };
          typedef int16_t type;
      };
      
      template <int bits>
      struct storage<bits,typename std::enable_if< ((17 <= bits) && (bits <= 32)),int>::type> {
          enum {
           storageSize = 32,
          };
          typedef int32_t type;
      };
      
      template <int bits>
      struct storage<bits,typename std::enable_if< ((33 <= bits) && (bits <= 64)),int>::type> {
          enum {
           storageSize = 64,
          };
          typedef int64_t type;
      };
  
   
    
  };


/***************

FIXED POINT

****************/

// The main fixed point type
template <int frac> struct Q ;


template<int frac>
struct Q {
    typedef typename internal::storage<frac>::type storage_type_t;

    storage_type_t value;

    Q() {}
    constexpr Q(const storage_type_t v) : value(v) {}


    __FORCE_INLINE Q<frac> operator-() const {
        return Q(-value);
    }

    __FORCE_INLINE Q<frac>& operator+=(const Q<frac> b) {
      value += b.value;
      return (*this);
    }
    
    __FORCE_INLINE Q<frac>& operator-=(const Q<frac> b) {
      value -= b.value;
      return (*this);
    }

#ifdef DEBUG
    __FORCE_INLINE float32_t f()
    {
      return(1.0 * value / (1 << frac));
    }
#endif

};

// Dadatypes used in CMSIS-DSP
using Q63 = Q<63>;
using Q31 = Q<31>;
using Q15 = Q<15>;
using Q7 = Q<7>;

// Template to compute the type of a fixed poitn product
// and its storage type.
template <typename T> struct QPRODUCT
{
};

template<typename T>
using PROD = typename QPRODUCT<T>::type;

template<typename T>
using PRODSTORAGE = typename QPRODUCT<T>::type::storage_type_t;


// Q7 * Q7 is Q15 and a shift right of 7 is required to convert
// back to Q7
template<>
struct QPRODUCT<Q7> {
   typedef Q15 type;
   enum {
      shift = 7
   };
};

template<>
struct QPRODUCT<Q15> {
   typedef Q31 type;
   enum {
      shift = 15
   };
};

template<>
struct QPRODUCT<Q31> {
   typedef Q63 type;
   enum {
      shift = 31
   };
};


}

#endif

