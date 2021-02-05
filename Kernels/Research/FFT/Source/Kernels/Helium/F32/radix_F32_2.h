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

#ifndef _RADIX_VECTOR_F32_2_H
#define _RADIX_VECTOR_F32_2_H

#include "complex.h"

/**************

DIRECT FFT 

***************/
template<int inputstrideenabled> 
struct radixVector<DIRECT,float32_t, 2,inputstrideenabled>{




/* Nb Multiplications = 0, 
Nb Additions = 2,
Nb Negates = 1,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_notw(int stageLength,complex<float32_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4_t a[2];
f32x4_t res[2];




a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 * stageLength, strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 * stageLength, strideFactor);


res[0] = vaddq(a[0] , a[1]) ;
res[1] = vsubq(a[0] , a[1]) ;


vmemory<float32_t,inputstrideenabled>::store(in, 0 * stageLength, res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 1 * stageLength, res[1], strideFactor);


};

/* Nb Multiplications = 1, 
Nb Additions = 2,
Nb Negates = 1,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_tw(int stageLength,const f32x4_t *w,complex<float32_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4_t a[2];
f32x4_t res[2];




a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 * stageLength, strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 * stageLength, strideFactor);


res[0] = vaddq(a[0] , a[1]) ;
res[1] = vcomplex_mulq(w[0] , vsubq(a[0] , a[1]) ) ;


vmemory<float32_t,inputstrideenabled>::store(in, 0 * stageLength, res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 1 * stageLength, res[1], strideFactor);


};

/* Nb Multiplications = 0, 
Nb Additions = 2,
Nb Negates = 1,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_notw(int stageLength,const complex<float32_t> *in,complex<float32_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4_t a[2];
f32x4_t res[2];




a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 * stageLength, strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 * stageLength, strideFactor);


res[0] = vaddq(a[0] , a[1]) ;
res[1] = vsubq(a[0] , a[1]) ;


vmemory<float32_t,inputstrideenabled>::store(out, 0 * stageLength , res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 1 * stageLength , res[1], strideFactor);


};

/* Nb Multiplications = 1, 
Nb Additions = 2,
Nb Negates = 1,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_tw(int stageLength,const f32x4_t *w,const complex<float32_t> *in,complex<float32_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4_t a[2];
f32x4_t res[2];




a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 * stageLength, strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 * stageLength, strideFactor);


res[0] = vaddq(a[0] , a[1]) ;
res[1] = vcomplex_mulq(w[0] , vsubq(a[0] , a[1]) ) ;


vmemory<float32_t,inputstrideenabled>::store(out, 0 * stageLength , res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 1 * stageLength , res[1], strideFactor);


};

/* Nb Multiplications = 0, 
Nb Additions = 2,
Nb Negates = 1,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_last(complex<float32_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4_t a[2];
f32x4_t res[2];




a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 , strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 , strideFactor);


res[0] = vaddq(a[0] , a[1]) ;
res[1] = vsubq(a[0] , a[1]) ;


vmemory<float32_t,inputstrideenabled>::store(in, 0 , res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 1 , res[1], strideFactor);


};

/* Nb Multiplications = 0, 
Nb Additions = 2,
Nb Negates = 1,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_last(const complex<float32_t> *in,complex<float32_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4_t a[2];
f32x4_t res[2];




a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 , strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 , strideFactor);


res[0] = vaddq(a[0] , a[1]) ;
res[1] = vsubq(a[0] , a[1]) ;


vmemory<float32_t,inputstrideenabled>::store(out, 0, res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 1, res[1], strideFactor);


};

};

/**************

INVERSE FFT 

***************/
template<int inputstrideenabled> 
struct radixVector<INVERSE,float32_t, 2,inputstrideenabled>{




/* Nb Multiplications = 0, 
Nb Additions = 2,
Nb Negates = 1,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_notw(int stageLength,complex<float32_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4_t a[2];
f32x4_t res[2];




a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 * stageLength, strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 * stageLength, strideFactor);


res[0] = vaddq(a[0] , a[1]) ;
res[1] = vsubq(a[0] , a[1]) ;


vmemory<float32_t,inputstrideenabled>::store(in, 0 * stageLength, res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 1 * stageLength, res[1], strideFactor);


};

/* Nb Multiplications = 1, 
Nb Additions = 2,
Nb Negates = 1,
Nb Conjugates = 1,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_tw(int stageLength,const f32x4_t *w,complex<float32_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4_t a[2];
f32x4_t res[2];




a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 * stageLength, strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 * stageLength, strideFactor);


res[0] = vaddq(a[0] , a[1]) ;
res[1] = vcomplex_mulq( vconjugate(w[0])   , vsubq(a[0] , a[1]) ) ;


vmemory<float32_t,inputstrideenabled>::store(in, 0 * stageLength, res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 1 * stageLength, res[1], strideFactor);


};

/* Nb Multiplications = 0, 
Nb Additions = 2,
Nb Negates = 1,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_notw(int stageLength,const complex<float32_t> *in,complex<float32_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4_t a[2];
f32x4_t res[2];




a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 * stageLength, strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 * stageLength, strideFactor);


res[0] = vaddq(a[0] , a[1]) ;
res[1] = vsubq(a[0] , a[1]) ;


vmemory<float32_t,inputstrideenabled>::store(out, 0 * stageLength , res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 1 * stageLength , res[1], strideFactor);


};

/* Nb Multiplications = 1, 
Nb Additions = 2,
Nb Negates = 1,
Nb Conjugates = 1,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_tw(int stageLength,const f32x4_t *w,const complex<float32_t> *in,complex<float32_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4_t a[2];
f32x4_t res[2];




a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 * stageLength, strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 * stageLength, strideFactor);


res[0] = vaddq(a[0] , a[1]) ;
res[1] = vcomplex_mulq( vconjugate(w[0])   , vsubq(a[0] , a[1]) ) ;


vmemory<float32_t,inputstrideenabled>::store(out, 0 * stageLength , res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 1 * stageLength , res[1], strideFactor);


};

/* Nb Multiplications = 0, 
Nb Additions = 2,
Nb Negates = 1,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_last(complex<float32_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4_t a[2];
f32x4_t res[2];




a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 , strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 , strideFactor);


res[0] = vaddq(a[0] , a[1]) ;
res[1] = vsubq(a[0] , a[1]) ;


vmemory<float32_t,inputstrideenabled>::store(in, 0 , res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 1 , res[1], strideFactor);


};

/* Nb Multiplications = 0, 
Nb Additions = 2,
Nb Negates = 1,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_last(const complex<float32_t> *in,complex<float32_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4_t a[2];
f32x4_t res[2];




a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 , strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 , strideFactor);


res[0] = vaddq(a[0] , a[1]) ;
res[1] = vsubq(a[0] , a[1]) ;


vmemory<float32_t,inputstrideenabled>::store(out, 0, res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 1, res[1], strideFactor);


};

};

#endif /* #ifndef _RADIX_VECTOR_F32_2_H */