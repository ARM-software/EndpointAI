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

#ifndef _RADIX_VECTOR_F32_4_H
#define _RADIX_VECTOR_F32_4_H

#include "complex.h"

/**************

DIRECT FFT 

***************/
template<int inputstrideenabled> 
struct radixVector<DIRECT,float32_t, 4,inputstrideenabled>{

static constexpr complex<float32_t> C0_4_tw{0.0f, 1.0f};
  
static constexpr complex<float32_t> C0_4_notw{0.0f, 1.0f};
  

/* Nb Multiplications = 1, 
Nb Additions = 8,
Nb Negates = 4,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_notw(int stageLength,complex<float32_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4x2_t a[4];
f32x4x2_t res[4];


 f32x4x2_t tmp_0;
 f32x4x2_t tmp_1;
 f32x4x2_t tmp_2;
 f32x4x2_t tmp_3;
 f32x4x2_t tmp_4;


a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 * stageLength, strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 * stageLength, strideFactor);
a[2] = vmemory<float32_t,inputstrideenabled>::load(in , 2 * stageLength, strideFactor);
a[3] = vmemory<float32_t,inputstrideenabled>::load(in , 3 * stageLength, strideFactor);


tmp_0 = vaddition(a[0] , a[2]) ;
tmp_1 = vaddition(a[1] , a[3]) ;
res[0] = vaddition(tmp_0 , tmp_1) ;
tmp_2 = vsubstraction(a[0] , a[2]) ;
tmp_3 = vsubstraction(a[1] , a[3]) ;
tmp_4 =  vrot(tmp_3)  ;
res[1] = vsubstraction(tmp_2 , tmp_4) ;
res[2] = vsubstraction(tmp_0 , tmp_1) ;
res[3] = vaddition(tmp_2 , tmp_4) ;


vmemory<float32_t,inputstrideenabled>::store(in, 0 * stageLength, res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 1 * stageLength, res[1], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 2 * stageLength, res[2], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 3 * stageLength, res[3], strideFactor);


};

/* Nb Multiplications = 4, 
Nb Additions = 8,
Nb Negates = 4,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_tw(int stageLength,const f32x4x2_t *w,complex<float32_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4x2_t a[4];
f32x4x2_t res[4];


 f32x4x2_t tmp_0;
 f32x4x2_t tmp_1;
 f32x4x2_t tmp_2;
 f32x4x2_t tmp_3;
 f32x4x2_t tmp_4;


a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 * stageLength, strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 * stageLength, strideFactor);
a[2] = vmemory<float32_t,inputstrideenabled>::load(in , 2 * stageLength, strideFactor);
a[3] = vmemory<float32_t,inputstrideenabled>::load(in , 3 * stageLength, strideFactor);


tmp_0 = vaddition(a[0] , a[2]) ;
tmp_1 = vaddition(a[1] , a[3]) ;
res[0] = vaddition(tmp_0 , tmp_1) ;
tmp_2 = vsubstraction(a[0] , a[2]) ;
tmp_3 = vsubstraction(a[1] , a[3]) ;
tmp_4 =  vrot(tmp_3)  ;
res[1] = vcomplex_mulq(w[0] , vsubstraction(tmp_2 , tmp_4) ) ;
res[2] = vcomplex_mulq(w[1] , vsubstraction(tmp_0 , tmp_1) ) ;
res[3] = vcomplex_mulq(w[2] , vaddition(tmp_2 , tmp_4) ) ;


vmemory<float32_t,inputstrideenabled>::store(in, 0 * stageLength, res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 1 * stageLength, res[1], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 2 * stageLength, res[2], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 3 * stageLength, res[3], strideFactor);


};

/* Nb Multiplications = 1, 
Nb Additions = 8,
Nb Negates = 4,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_notw(int stageLength,const complex<float32_t> *in,complex<float32_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4x2_t a[4];
f32x4x2_t res[4];


 f32x4x2_t tmp_0;
 f32x4x2_t tmp_1;
 f32x4x2_t tmp_2;
 f32x4x2_t tmp_3;
 f32x4x2_t tmp_4;


a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 * stageLength, strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 * stageLength, strideFactor);
a[2] = vmemory<float32_t,inputstrideenabled>::load(in , 2 * stageLength, strideFactor);
a[3] = vmemory<float32_t,inputstrideenabled>::load(in , 3 * stageLength, strideFactor);


tmp_0 = vaddition(a[0] , a[2]) ;
tmp_1 = vaddition(a[1] , a[3]) ;
res[0] = vaddition(tmp_0 , tmp_1) ;
tmp_2 = vsubstraction(a[0] , a[2]) ;
tmp_3 = vsubstraction(a[1] , a[3]) ;
tmp_4 =  vrot(tmp_3)  ;
res[1] = vsubstraction(tmp_2 , tmp_4) ;
res[2] = vsubstraction(tmp_0 , tmp_1) ;
res[3] = vaddition(tmp_2 , tmp_4) ;


vmemory<float32_t,inputstrideenabled>::store(out, 0 * stageLength , res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 1 * stageLength , res[1], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 2 * stageLength , res[2], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 3 * stageLength , res[3], strideFactor);


};

/* Nb Multiplications = 4, 
Nb Additions = 8,
Nb Negates = 4,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_tw(int stageLength,const f32x4x2_t *w,const complex<float32_t> *in,complex<float32_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4x2_t a[4];
f32x4x2_t res[4];


 f32x4x2_t tmp_0;
 f32x4x2_t tmp_1;
 f32x4x2_t tmp_2;
 f32x4x2_t tmp_3;
 f32x4x2_t tmp_4;


a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 * stageLength, strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 * stageLength, strideFactor);
a[2] = vmemory<float32_t,inputstrideenabled>::load(in , 2 * stageLength, strideFactor);
a[3] = vmemory<float32_t,inputstrideenabled>::load(in , 3 * stageLength, strideFactor);


tmp_0 = vaddition(a[0] , a[2]) ;
tmp_1 = vaddition(a[1] , a[3]) ;
res[0] = vaddition(tmp_0 , tmp_1) ;
tmp_2 = vsubstraction(a[0] , a[2]) ;
tmp_3 = vsubstraction(a[1] , a[3]) ;
tmp_4 =  vrot(tmp_3)  ;
res[1] = vcomplex_mulq(w[0] , vsubstraction(tmp_2 , tmp_4) ) ;
res[2] = vcomplex_mulq(w[1] , vsubstraction(tmp_0 , tmp_1) ) ;
res[3] = vcomplex_mulq(w[2] , vaddition(tmp_2 , tmp_4) ) ;


vmemory<float32_t,inputstrideenabled>::store(out, 0 * stageLength , res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 1 * stageLength , res[1], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 2 * stageLength , res[2], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 3 * stageLength , res[3], strideFactor);


};

/* Nb Multiplications = 1, 
Nb Additions = 8,
Nb Negates = 4,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_last(complex<float32_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4x2_t a[4];
f32x4x2_t res[4];


 f32x4x2_t tmp_0;
 f32x4x2_t tmp_1;
 f32x4x2_t tmp_2;
 f32x4x2_t tmp_3;
 f32x4x2_t tmp_4;


a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 , strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 , strideFactor);
a[2] = vmemory<float32_t,inputstrideenabled>::load(in , 2 , strideFactor);
a[3] = vmemory<float32_t,inputstrideenabled>::load(in , 3 , strideFactor);


tmp_0 = vaddition(a[0] , a[2]) ;
tmp_1 = vaddition(a[1] , a[3]) ;
res[0] = vaddition(tmp_0 , tmp_1) ;
tmp_2 = vsubstraction(a[0] , a[2]) ;
tmp_3 = vsubstraction(a[1] , a[3]) ;
tmp_4 =  vrot(tmp_3)  ;
res[1] = vsubstraction(tmp_2 , tmp_4) ;
res[2] = vsubstraction(tmp_0 , tmp_1) ;
res[3] = vaddition(tmp_2 , tmp_4) ;


vmemory<float32_t,inputstrideenabled>::store(in, 0 , res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 1 , res[1], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 2 , res[2], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 3 , res[3], strideFactor);


};

/* Nb Multiplications = 1, 
Nb Additions = 8,
Nb Negates = 4,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_last(const complex<float32_t> *in,complex<float32_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4x2_t a[4];
f32x4x2_t res[4];


 f32x4x2_t tmp_0;
 f32x4x2_t tmp_1;
 f32x4x2_t tmp_2;
 f32x4x2_t tmp_3;
 f32x4x2_t tmp_4;


a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 , strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 , strideFactor);
a[2] = vmemory<float32_t,inputstrideenabled>::load(in , 2 , strideFactor);
a[3] = vmemory<float32_t,inputstrideenabled>::load(in , 3 , strideFactor);


tmp_0 = vaddition(a[0] , a[2]) ;
tmp_1 = vaddition(a[1] , a[3]) ;
res[0] = vaddition(tmp_0 , tmp_1) ;
tmp_2 = vsubstraction(a[0] , a[2]) ;
tmp_3 = vsubstraction(a[1] , a[3]) ;
tmp_4 =  vrot(tmp_3)  ;
res[1] = vsubstraction(tmp_2 , tmp_4) ;
res[2] = vsubstraction(tmp_0 , tmp_1) ;
res[3] = vaddition(tmp_2 , tmp_4) ;


vmemory<float32_t,inputstrideenabled>::store(out, 0, res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 1, res[1], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 2, res[2], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 3, res[3], strideFactor);


};

};

/**************

INVERSE FFT 

***************/
template<int inputstrideenabled> 
struct radixVector<INVERSE,float32_t, 4,inputstrideenabled>{

static constexpr complex<float32_t> C0_4_tw{0.0f, 1.0f};
  
static constexpr complex<float32_t> C0_4_notw{0.0f, 1.0f};
  

/* Nb Multiplications = 1, 
Nb Additions = 8,
Nb Negates = 4,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_notw(int stageLength,complex<float32_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4x2_t a[4];
f32x4x2_t res[4];


 f32x4x2_t tmp_0;
 f32x4x2_t tmp_1;
 f32x4x2_t tmp_2;
 f32x4x2_t tmp_3;
 f32x4x2_t tmp_4;


a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 * stageLength, strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 * stageLength, strideFactor);
a[2] = vmemory<float32_t,inputstrideenabled>::load(in , 2 * stageLength, strideFactor);
a[3] = vmemory<float32_t,inputstrideenabled>::load(in , 3 * stageLength, strideFactor);


tmp_0 = vaddition(a[0] , a[2]) ;
tmp_1 = vaddition(a[1] , a[3]) ;
res[0] = vaddition(tmp_0 , tmp_1) ;
tmp_2 = vsubstraction(a[0] , a[2]) ;
tmp_3 = vsubstraction(a[1] , a[3]) ;
tmp_4 =  vrot(tmp_3)  ;
res[1] = vaddition(tmp_2 , tmp_4) ;
res[2] = vsubstraction(tmp_0 , tmp_1) ;
res[3] = vsubstraction(tmp_2 , tmp_4) ;


vmemory<float32_t,inputstrideenabled>::store(in, 0 * stageLength, res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 1 * stageLength, res[1], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 2 * stageLength, res[2], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 3 * stageLength, res[3], strideFactor);


};

/* Nb Multiplications = 4, 
Nb Additions = 8,
Nb Negates = 4,
Nb Conjugates = 3,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_tw(int stageLength,const f32x4x2_t *w,complex<float32_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4x2_t a[4];
f32x4x2_t res[4];


 f32x4x2_t tmp_0;
 f32x4x2_t tmp_1;
 f32x4x2_t tmp_2;
 f32x4x2_t tmp_3;
 f32x4x2_t tmp_4;


a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 * stageLength, strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 * stageLength, strideFactor);
a[2] = vmemory<float32_t,inputstrideenabled>::load(in , 2 * stageLength, strideFactor);
a[3] = vmemory<float32_t,inputstrideenabled>::load(in , 3 * stageLength, strideFactor);


tmp_0 = vaddition(a[0] , a[2]) ;
tmp_1 = vaddition(a[1] , a[3]) ;
res[0] = vaddition(tmp_0 , tmp_1) ;
tmp_2 = vsubstraction(a[0] , a[2]) ;
tmp_3 = vsubstraction(a[1] , a[3]) ;
tmp_4 =  vrot(tmp_3)  ;
res[1] = vcomplex_mulq( vconjugate(w[0])   , vaddition(tmp_2 , tmp_4) ) ;
res[2] = vcomplex_mulq( vconjugate(w[1])   , vsubstraction(tmp_0 , tmp_1) ) ;
res[3] = vcomplex_mulq( vconjugate(w[2])   , vsubstraction(tmp_2 , tmp_4) ) ;


vmemory<float32_t,inputstrideenabled>::store(in, 0 * stageLength, res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 1 * stageLength, res[1], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 2 * stageLength, res[2], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 3 * stageLength, res[3], strideFactor);


};

/* Nb Multiplications = 1, 
Nb Additions = 8,
Nb Negates = 4,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_notw(int stageLength,const complex<float32_t> *in,complex<float32_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4x2_t a[4];
f32x4x2_t res[4];


 f32x4x2_t tmp_0;
 f32x4x2_t tmp_1;
 f32x4x2_t tmp_2;
 f32x4x2_t tmp_3;
 f32x4x2_t tmp_4;


a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 * stageLength, strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 * stageLength, strideFactor);
a[2] = vmemory<float32_t,inputstrideenabled>::load(in , 2 * stageLength, strideFactor);
a[3] = vmemory<float32_t,inputstrideenabled>::load(in , 3 * stageLength, strideFactor);


tmp_0 = vaddition(a[0] , a[2]) ;
tmp_1 = vaddition(a[1] , a[3]) ;
res[0] = vaddition(tmp_0 , tmp_1) ;
tmp_2 = vsubstraction(a[0] , a[2]) ;
tmp_3 = vsubstraction(a[1] , a[3]) ;
tmp_4 =  vrot(tmp_3)  ;
res[1] = vaddition(tmp_2 , tmp_4) ;
res[2] = vsubstraction(tmp_0 , tmp_1) ;
res[3] = vsubstraction(tmp_2 , tmp_4) ;


vmemory<float32_t,inputstrideenabled>::store(out, 0 * stageLength , res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 1 * stageLength , res[1], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 2 * stageLength , res[2], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 3 * stageLength , res[3], strideFactor);


};

/* Nb Multiplications = 4, 
Nb Additions = 8,
Nb Negates = 4,
Nb Conjugates = 3,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_tw(int stageLength,const f32x4x2_t *w,const complex<float32_t> *in,complex<float32_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4x2_t a[4];
f32x4x2_t res[4];


 f32x4x2_t tmp_0;
 f32x4x2_t tmp_1;
 f32x4x2_t tmp_2;
 f32x4x2_t tmp_3;
 f32x4x2_t tmp_4;


a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 * stageLength, strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 * stageLength, strideFactor);
a[2] = vmemory<float32_t,inputstrideenabled>::load(in , 2 * stageLength, strideFactor);
a[3] = vmemory<float32_t,inputstrideenabled>::load(in , 3 * stageLength, strideFactor);


tmp_0 = vaddition(a[0] , a[2]) ;
tmp_1 = vaddition(a[1] , a[3]) ;
res[0] = vaddition(tmp_0 , tmp_1) ;
tmp_2 = vsubstraction(a[0] , a[2]) ;
tmp_3 = vsubstraction(a[1] , a[3]) ;
tmp_4 =  vrot(tmp_3)  ;
res[1] = vcomplex_mulq( vconjugate(w[0])   , vaddition(tmp_2 , tmp_4) ) ;
res[2] = vcomplex_mulq( vconjugate(w[1])   , vsubstraction(tmp_0 , tmp_1) ) ;
res[3] = vcomplex_mulq( vconjugate(w[2])   , vsubstraction(tmp_2 , tmp_4) ) ;


vmemory<float32_t,inputstrideenabled>::store(out, 0 * stageLength , res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 1 * stageLength , res[1], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 2 * stageLength , res[2], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 3 * stageLength , res[3], strideFactor);


};

/* Nb Multiplications = 1, 
Nb Additions = 8,
Nb Negates = 4,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_last(complex<float32_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4x2_t a[4];
f32x4x2_t res[4];


 f32x4x2_t tmp_0;
 f32x4x2_t tmp_1;
 f32x4x2_t tmp_2;
 f32x4x2_t tmp_3;
 f32x4x2_t tmp_4;


a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 , strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 , strideFactor);
a[2] = vmemory<float32_t,inputstrideenabled>::load(in , 2 , strideFactor);
a[3] = vmemory<float32_t,inputstrideenabled>::load(in , 3 , strideFactor);


tmp_0 = vaddition(a[0] , a[2]) ;
tmp_1 = vaddition(a[1] , a[3]) ;
res[0] = vaddition(tmp_0 , tmp_1) ;
tmp_2 = vsubstraction(a[0] , a[2]) ;
tmp_3 = vsubstraction(a[1] , a[3]) ;
tmp_4 =  vrot(tmp_3)  ;
res[1] = vaddition(tmp_2 , tmp_4) ;
res[2] = vsubstraction(tmp_0 , tmp_1) ;
res[3] = vsubstraction(tmp_2 , tmp_4) ;


vmemory<float32_t,inputstrideenabled>::store(in, 0 , res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 1 , res[1], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 2 , res[2], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 3 , res[3], strideFactor);


};

/* Nb Multiplications = 1, 
Nb Additions = 8,
Nb Negates = 4,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_last(const complex<float32_t> *in,complex<float32_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4x2_t a[4];
f32x4x2_t res[4];


 f32x4x2_t tmp_0;
 f32x4x2_t tmp_1;
 f32x4x2_t tmp_2;
 f32x4x2_t tmp_3;
 f32x4x2_t tmp_4;


a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 , strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 , strideFactor);
a[2] = vmemory<float32_t,inputstrideenabled>::load(in , 2 , strideFactor);
a[3] = vmemory<float32_t,inputstrideenabled>::load(in , 3 , strideFactor);


tmp_0 = vaddition(a[0] , a[2]) ;
tmp_1 = vaddition(a[1] , a[3]) ;
res[0] = vaddition(tmp_0 , tmp_1) ;
tmp_2 = vsubstraction(a[0] , a[2]) ;
tmp_3 = vsubstraction(a[1] , a[3]) ;
tmp_4 =  vrot(tmp_3)  ;
res[1] = vaddition(tmp_2 , tmp_4) ;
res[2] = vsubstraction(tmp_0 , tmp_1) ;
res[3] = vsubstraction(tmp_2 , tmp_4) ;


vmemory<float32_t,inputstrideenabled>::store(out, 0, res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 1, res[1], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 2, res[2], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 3, res[3], strideFactor);


};

};

#endif /* #ifndef _RADIX_VECTOR_F32_4_H */