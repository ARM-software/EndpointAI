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

/*************************

Implementation of the FFTs.
Include all headers with template definitions.

**************************/

//#define DEBUG 1

#if defined(DEBUG)
#include <cstdio>
#endif

#include "fft_types.h"


#include "butterfly.h"
#include "Private/complex_priv.h"
#include "Private/fixedpoint_priv.h"


using namespace FFTSCI;


// FFT MODE
constexpr int DIRECT=0;
constexpr int INVERSE=1;


/*

APIs for the radix

*/

template <int mode,typename T,int factor,int inputstrideenabled>
struct radix{

// In Place with Twiddle multiplications
void run_ip_tw(int stageLength,
  const complex<T> *w,
  complex<T> *in,
  int inputstride
  );

// Out of Place, with twiddle
void run_op_tw(int stageLength,
  const complex<T> *w,
  const complex<T> *in,
  complex<T> *out,
  int inputstride);

// In Place, no twiddle
void run_ip_notw(int stageLength,
  complex<T> *in,
  int inputstride);

// Out of Place, no twiddle
void run_op_notw(int stageLength,
  const complex<T> *in,
  complex<T> *out,
  int inputstride);

// In place, last stage
void run_ip_last(complex<T> *in,int inputstride);

// Out of Place, last stage
void run_op_last(const complex<T> *in,
  complex<T> *out,int inputstride);

};

/*

By default scalar version is used unless this is
redefined for specific datatypes

*/

template<typename T>
struct vector
{
    enum {
           vector_enabled = 0
    };
};

template<typename T>
using VECTORTYPE = typename vector<T>::type;

template<typename T>
using OFFSETTYPE = typename vector<T>::offset_type;

// Template specializations for the different radixes and datatypes
#include "radix.h"

// Bitreversal : permutation of the FFT output
#include "reversal.h"

// VAPIs for vectorized radix if vector are supported by the selected core.
#if defined(HASVECTOR)
#include "vector.h"
#endif

// Loop for a radix (looping on group and samples)
#include "grouploop.h"
#include "specialgrouploop.h"

// Vectorized loop is vector supported by selected core 
#if defined(HASVECTOR)
#include "vector_grouploop.h"
#include "vector_specialgrouploop.h"
#endif

// Looping on radix and stage
#include "stageloop.h"


// FFT Implementation
template <int mode,typename T,int inputstrideenabled>
struct fft{
  static void run(const cfftconfig<T> *config,
    int n,
    complex<T> *in, 
    complex<T> *out,
    int inputstride)
 {
    int stride;
    uint32_t factor;
    uint32_t nbFactors;
    uint32_t stage;

    bool hasFirst;
    bool hasLast;
    
    const complex<T> *twid = (complex<T>*)config->twiddle;

    stride=1;

    // Iterate on the stages.
    // Stages are grouped per radix.
    // We run all the stages for a given radix.
    // This loop of stages may contain the first, last of both stages
    // and the corresponding special radix kernel must be selected.
    stage=0;
    for(uint32_t stageGroup = 0; stageGroup < config->nbGroupedFactors; stageGroup++)
    {
       factor = config->factors[2*stageGroup];
       nbFactors = config->factors[2*stageGroup+1];
       hasFirst=(stage == 0); 
       hasLast = (stage + nbFactors == config->nbFactors );

       if (hasFirst && !hasLast)
       {
          stride=factorSwitch<mode,T,STARTONLY,inputstrideenabled>::run(config,n,factor,nbFactors,stage,stride,in,out,twid,inputstride);
       }
       else if (!hasFirst && hasLast)
       {
          stride=factorSwitch<mode,T,ENDONLY,inputstrideenabled>::run(config,n,factor,nbFactors,stage,stride,in,out,twid,inputstride);
       }
       else if (hasFirst && hasLast)
       {
          if (nbFactors==1)
          {
             stride=factorSwitch<mode,T,ONESTAGE,inputstrideenabled>::run(config,n,factor,nbFactors,stage,stride,in,out,twid,inputstride);
          }
          else
          {
             stride=factorSwitch<mode,T,BOTH,inputstrideenabled>::run(config,n,factor,nbFactors,stage,stride,in,out,twid,inputstride);
          }
       }
       else
       {
          stride=factorSwitch<mode,T,NORMALLOOP,inputstrideenabled>::run(config,n,factor,nbFactors,stage,stride,in,out,twid,inputstride);
       }


       stage += nbFactors;
    }

   
  }
};

// API for FFT, RFFT init and #define for supported
// functions
#include "config_func.h"

// The core of the implementation
#include "fft_impl.h"

/********************

Instantiate templates for supported functions
according to the flags defined in config_func.h

********************/

/****

CFFT

*****/

#if defined(FUNC_CFFT_F64)
template
arm_status cfft(const float64_t *in,float64_t* out, uint16_t N);

template
arm_status cifft(const float64_t *in,float64_t* out, uint16_t N);

template
arm_status cfft2D(const float64_t *in,float64_t* out, uint16_t rows,uint16_t cols);

template
arm_status cifft2D(const float64_t *in,float64_t* out, uint16_t rows,uint16_t cols);
#endif

#if defined(FUNC_CFFT_F32)

template
arm_status cfft(const float32_t *in,float32_t* out, uint16_t N);

template
arm_status cifft(const float32_t *in,float32_t* out, uint16_t N);

template
arm_status cfft2D(const float32_t *in,float32_t* out, uint16_t rows,uint16_t cols);

template
arm_status cifft2D(const float32_t *in,float32_t* out, uint16_t rows,uint16_t cols);
#endif

#if defined(FUNC_CFFT_F16) && defined(ARM_FLOAT16_SUPPORTED)
template
arm_status cfft(const float16_t *in,float16_t* out, uint16_t N);

template
arm_status cifft(const float16_t *in,float16_t* out, uint16_t N);

template
arm_status cfft2D(const float16_t *in,float16_t* out, uint16_t rows,uint16_t cols);

template
arm_status cifft2D(const float16_t *in,float16_t* out, uint16_t rows,uint16_t cols);

#endif

#if defined(FUNC_CFFT_Q31)
template
arm_status cfft(const Q31 *in,Q31* out, uint16_t N);

template
arm_status cifft(const Q31 *in,Q31* out, uint16_t N);

template
arm_status cfft2D(const Q31 *in,Q31* out, uint16_t rows,uint16_t cols);

template
arm_status cifft2D(const Q31 *in,Q31* out, uint16_t rows,uint16_t cols);

#endif

#if defined(FUNC_CFFT_Q15)
template
arm_status cfft(const Q15 *in,Q15* out, uint16_t N);

template
arm_status cifft(const Q15 *in,Q15* out, uint16_t N);

template
arm_status cfft2D(const Q15 *in,Q15* out, uint16_t rows,uint16_t cols);

template
arm_status cifft2D(const Q15 *in,Q15* out, uint16_t rows,uint16_t cols);

#endif

#if defined(FUNC_CFFT_Q7)
template
arm_status cfft(const Q7 *in,Q7* out, uint16_t N);

template
arm_status cifft(const Q7 *in,Q7* out, uint16_t N);

template
arm_status cfft2D(const Q7 *in,Q7* out, uint16_t rows,uint16_t cols);

template
arm_status cifft2D(const Q7 *in,Q7* out, uint16_t rows,uint16_t cols);

#endif


/****

RFFT

*****/

#if defined(FUNC_RFFT_F64)
template
arm_status rfft(const float64_t *in,float64_t* tmp,float64_t* out, uint16_t N);

template
arm_status rifft(const float64_t *in,float64_t* tmp,float64_t* out, uint16_t N);
#endif

#if defined(FUNC_RFFT_F32)
template
arm_status rfft(const float32_t *in,float32_t* tmp,float32_t* out, uint16_t N);

template
arm_status rifft(const float32_t *in,float32_t* tmp,float32_t* out, uint16_t N);
#endif

#if defined(FUNC_RFFT_F16) && defined(ARM_FLOAT16_SUPPORTED)
template
arm_status rfft(const float16_t *in,float16_t* tmp,float16_t* out, uint16_t N);

template
arm_status rifft(const float16_t *in,float16_t* tmp,float16_t* out, uint16_t N);

#endif

#if defined(FUNC_RFFT_Q31)
template
arm_status rfft(const Q31 *in,Q31* tmp,Q31* out, uint16_t N);

template
arm_status rifft(const Q31 *in,Q31* tmp,Q31* out, uint16_t N);

#endif

#if defined(FUNC_RFFT_Q15)
template
arm_status rfft(const Q15 *in,Q15* tmp,Q15* out, uint16_t N);

template
arm_status rifft(const Q15 *in,Q15* tmp,Q15* out, uint16_t N);

#endif

#if defined(FUNC_RFFT_Q7)
template
arm_status rfft(const Q7 *in,Q7* out,Q7* out, uint16_t N);

template
arm_status rifft(const Q7 *in,Q7* out,Q7* out, uint16_t N);

#endif