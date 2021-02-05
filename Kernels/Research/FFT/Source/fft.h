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

/**********************

FFT APIs.

To include in client code which is using those FFTs

***********************/

#ifndef FFT_H
#define FFT_H

#include "fft_types.h"


/*

CFFT

*/

#if defined(FUNC_CFFT_F64)
extern template arm_status cfft(const float64_t *in,float64_t* out, uint16_t N);
extern template arm_status cifft(const float64_t *in,float64_t* out, uint16_t N);

extern template arm_status cfft2D(const float64_t *in,float64_t* out, uint16_t rows,uint16_t cols);
extern template arm_status cifft2D(const float64_t *in,float64_t* out, uint16_t rows,uint16_t cols);
#endif

#if defined(FUNC_CFFT_F32)
extern template arm_status cfft<float32_t>(const float32_t *in,float32_t* out, uint16_t N);
extern template arm_status cifft<float32_t>(const float32_t *in,float32_t* out, uint16_t N);

extern template arm_status cfft2D<float32_t>(const float32_t *in,float32_t* out, uint16_t rows,uint16_t cols);
extern template arm_status cifft2D<float32_t>(const float32_t *in,float32_t* out, uint16_t rows,uint16_t cols);
#endif

#if defined(FUNC_CFFT_F16)
extern template arm_status cfft(const float16_t *in,float16_t* out, uint16_t N);
extern template arm_status cifft(const float16_t *in,float16_t* out, uint16_t N);

extern template arm_status cfft2D(const float16_t *in,float16_t* out, uint16_t rows,uint16_t cols);
extern template arm_status cifft2D(const float16_t *in,float16_t* out, uint16_t rows,uint16_t cols);
#endif

#if defined(FUNC_CFFT_Q31)
extern template arm_status cfft(const Q31 *in,Q31* out, uint16_t N);
extern template arm_status cifft(const Q31 *in,Q31* out, uint16_t N);

extern template arm_status cfft2D(const Q31 *in,Q31* out, uint16_t rows,uint16_t cols);
extern template arm_status cifft2D(const Q31 *in,Q31* out, uint16_t rows,uint16_t cols);
#endif

#if defined(FUNC_CFFT_Q15)
extern template arm_status cfft(const Q15 *in,Q15* out, uint16_t N);
extern template arm_status cifft(const Q15 *in,Q15* out, uint16_t N);

extern template arm_status cfft2D(const Q15 *in,Q15* out, uint16_t rows,uint16_t cols);
extern template arm_status cifft2D(const Q15 *in,Q15* out, uint16_t rows,uint16_t cols);
#endif

#if defined(FUNC_CFFT_Q7)
extern template arm_status cfft(const Q7 *in,Q7* out, uint16_t N);
extern template arm_status cifft(const Q7 *in,Q7* out, uint16_t N);

extern template arm_status cfft2D(const Q7 *in,Q7* out, uint16_t rows,uint16_t cols);
extern template arm_status cifft2D(const Q7 *in,Q7* out, uint16_t rows,uint16_t cols);
#endif

/*

RFFT

*/

/*

For RFFT, tmp must be different from out.
But in and tmp can be the same.
out buffer has length n/2+1 complex samples for a real fft of length n
So 2*(n/2+1) float since samples are complex

For RIFFT, tmp must be different from in 
But out and tmp can be the same.

in buffer has length n/2+1 complex samples
out buffer has length n real samples.


*/

#if defined(FUNC_RFFT_F64)
extern template arm_status rfft(const float64_t *in,float64_t* tmp,float64_t* out, uint16_t N);

extern template arm_status rifft(const float64_t *in,float64_t* tmp,float64_t* out, uint16_t N);
#endif

#if defined(FUNC_RFFT_F32)
extern template arm_status rfft(const float32_t *in,float32_t* tmp,float32_t* out, uint16_t N);

extern template arm_status rifft(const float32_t *in,float32_t* tmp,float32_t* out, uint16_t N);
#endif

#if defined(FUNC_RFFT_F16)
extern template arm_status rfft(const float16_t *in,float16_t* tmp,float16_t* out, uint16_t N);

extern template arm_status rifft(const float16_t *in,float16_t* tmp,float16_t* out, uint16_t N);
#endif

#if defined(FUNC_RFFT_Q31)
extern template arm_status rfft(const Q31 *in,Q31* tmp,Q31* out, uint16_t N);

extern template arm_status rifft(const Q31 *in,Q31* tmp,Q31* out, uint16_t N);

#endif

#if defined(FUNC_RFFT_Q15)
extern template arm_status rfft(const Q15 *in,Q15* tmp,Q15* out, uint16_t N);

extern template arm_status rifft(const Q15 *in,Q15* tmp,Q15* out, uint16_t N);

#endif

#if defined(FUNC_RFFT_Q7)
extern template arm_status rfft(const Q7 *in,Q7* tmp,Q7* out, uint16_t N);

extern template arm_status rifft(const Q7 *in,Q7* tmp,Q7* out, uint16_t N);

#endif



#endif
