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

/**************

FFT Types and configuration of the FFTs.
Indirectly include by clients when fft.h is included

**************/

#ifndef FFT_TYPES_H
#define FFT_TYPES_H


// Define supported radix and datatypes
#include "config.h"

// When defined, less accurate but a bit faster
// (Mainly for fixed point when DSP extensions are available)
//#define FASTER 1

// No saturation when narrowing
#define NOSAT 1

#include <arm_math_types.h>

#define  __FORCE_INLINE __attribute__((always_inline)) __INLINE


#include <arm_math_types_f16.h>
#include "arm_math_memory.h"

#include "complex.h"
#include "fixedpoint.h"

/*

CFFT Configuration structure

*/
template<typename T>
struct cfftconfig {
   // Normalization factor for ifft (scaling factor)
   const T normalization;
   // Pointer to permutation array with number of permutations
   uint16_t nbPerms;
   const uint16_t *perms;
   // Pointer to Twiddle with number of twiddles
   uint16_t nbTwiddle;
   const T *twiddle;
   // Pointer to factors with number of different factors
   // and group of factors.
   uint16_t nbGroupedFactors;
   uint16_t nbFactors;
   const uint16_t *factors;
   const uint16_t *lengths;
   // Output format for fixed point arithmetic (shift)
   const uint16_t format;
   // Is permutation (named bitreversal) vectorizable
   const uint16_t reversalVectorizable;
};

/*

RFFT Configuration structure

*/
template<typename T>
struct rfftconfig {
   /* Array of twiddle and number of twiddles for the split/merge
      steps of the RFFT 
   */
   uint16_t nbTwiddle;
   const T *twiddle;
};


/*

Interfaces

*/
template<typename T>
const cfftconfig<T> *cfft_config(uint32_t nbSamples);

template<typename T>
const rfftconfig<T> *rfft_config(uint32_t nbSamples);

template<typename T>
arm_status cfft(const T *in,T* out, uint16_t N);

template<typename T>
arm_status cifft(const T *in,T* out, uint16_t N);

template<typename T>
arm_status cfft2D(const T *in,T* out, uint16_t rows,uint16_t cols);

template<typename T>
arm_status cifft2D(const T *in,T* out, uint16_t rows,uint16_t cols);

template<typename T>
arm_status rfft(const T *in,T* tmp,T* out, uint16_t N);

template<typename T>
arm_status rifft(const T *in,T* tmp,T* out, uint16_t N);



#endif
