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

#ifndef _RADIX_VECTOR_F32_8_H
#define _RADIX_VECTOR_F32_8_H

#include "complex.h"

/**************

DIRECT FFT 

***************/
template<int inputstrideenabled> 
struct radixVector<DIRECT,float32_t, 8,inputstrideenabled>{

static constexpr complex<float32_t> C0_8_tw{0.0f, 1.0f};
  static constexpr complex<float32_t> C1_8_tw{0.7071067811865476f, -0.7071067811865476f};
  static constexpr complex<float32_t> C2_8_tw{-0.7071067811865476f, -0.7071067811865476f};
  
static constexpr complex<float32_t> C0_8_notw{0.0f, 1.0f};
  static constexpr complex<float32_t> C1_8_notw{0.7071067811865476f, -0.7071067811865476f};
  static constexpr complex<float32_t> C2_8_notw{-0.7071067811865476f, -0.7071067811865476f};
  

/* Nb Multiplications = 5, 
Nb Additions = 24,
Nb Negates = 12,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_notw(int stageLength,complex<float32_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4_t a[8];
f32x4_t res[8];


 f32x4_t tmp_0;
 f32x4_t tmp_1;
 f32x4_t tmp_2;
 f32x4_t tmp_3;
 f32x4_t tmp_4;
 f32x4_t tmp_5;
 f32x4_t tmp_6;
 f32x4_t tmp_7;
 f32x4_t tmp_8;
 f32x4_t tmp_9;
 f32x4_t tmp_10;
 f32x4_t tmp_11;
 f32x4_t tmp_12;
 f32x4_t tmp_13;
 f32x4_t tmp_14;
 f32x4_t tmp_15;
 f32x4_t tmp_16;
 f32x4_t tmp_17;
 f32x4_t tmp_18;
 f32x4_t tmp_19;
 f32x4_t tmp_20;


a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 * stageLength, strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 * stageLength, strideFactor);
a[2] = vmemory<float32_t,inputstrideenabled>::load(in , 2 * stageLength, strideFactor);
a[3] = vmemory<float32_t,inputstrideenabled>::load(in , 3 * stageLength, strideFactor);
a[4] = vmemory<float32_t,inputstrideenabled>::load(in , 4 * stageLength, strideFactor);
a[5] = vmemory<float32_t,inputstrideenabled>::load(in , 5 * stageLength, strideFactor);
a[6] = vmemory<float32_t,inputstrideenabled>::load(in , 6 * stageLength, strideFactor);
a[7] = vmemory<float32_t,inputstrideenabled>::load(in , 7 * stageLength, strideFactor);


tmp_0 = vaddq(a[0] , a[4]) ;
tmp_1 = vaddq(a[2] , a[6]) ;
tmp_2 = vaddq(tmp_0 , tmp_1) ;
tmp_3 = vaddq(a[1] , a[5]) ;
tmp_4 = vaddq(a[3] , a[7]) ;
tmp_5 = vaddq(tmp_3 , tmp_4) ;
res[0] = vaddq(tmp_2 , tmp_5) ;
tmp_6 = vsubq(a[0] , a[4]) ;
tmp_7 = vsubq(a[2] , a[6]) ;
tmp_8 =  vrot(tmp_7)  ;
tmp_9 = vsubq(tmp_6 , tmp_8) ;
tmp_10 = vsubq(a[3] , a[7]) ;
tmp_11 = vcomplex_mulq_n(tmp_10,(C2_8_notw) ) ;
tmp_12 = vsubq(a[1] , a[5]) ;
tmp_13 = vcomplex_mulq_n(tmp_12,(C1_8_notw) ) ;
tmp_14 = vaddq(tmp_11 , tmp_13) ;
res[1] = vaddq(tmp_9 , tmp_14) ;
tmp_15 = vsubq(tmp_0 , tmp_1) ;
tmp_16 = vsubq(tmp_3 , tmp_4) ;
tmp_17 =  vrot(tmp_16)  ;
res[2] = vsubq(tmp_15 , tmp_17) ;
tmp_18 = vaddq(tmp_6 , tmp_8) ;
tmp_19 = vsubq(tmp_13 , tmp_11) ;
tmp_20 =  vrot(tmp_19)  ;
res[3] = vsubq(tmp_18 , tmp_20) ;
res[4] = vsubq(tmp_2 , tmp_5) ;
res[5] = vsubq(tmp_9 , tmp_14) ;
res[6] = vaddq(tmp_15 , tmp_17) ;
res[7] = vaddq(tmp_18 , tmp_20) ;


vmemory<float32_t,inputstrideenabled>::store(in, 0 * stageLength, res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 1 * stageLength, res[1], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 2 * stageLength, res[2], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 3 * stageLength, res[3], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 4 * stageLength, res[4], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 5 * stageLength, res[5], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 6 * stageLength, res[6], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 7 * stageLength, res[7], strideFactor);


};

/* Nb Multiplications = 12, 
Nb Additions = 24,
Nb Negates = 12,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_tw(int stageLength,const f32x4_t *w,complex<float32_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4_t a[8];
f32x4_t res[8];


 f32x4_t tmp_0;
 f32x4_t tmp_1;
 f32x4_t tmp_2;
 f32x4_t tmp_3;
 f32x4_t tmp_4;
 f32x4_t tmp_5;
 f32x4_t tmp_6;
 f32x4_t tmp_7;
 f32x4_t tmp_8;
 f32x4_t tmp_9;
 f32x4_t tmp_10;
 f32x4_t tmp_11;
 f32x4_t tmp_12;
 f32x4_t tmp_13;
 f32x4_t tmp_14;
 f32x4_t tmp_15;
 f32x4_t tmp_16;
 f32x4_t tmp_17;
 f32x4_t tmp_18;
 f32x4_t tmp_19;
 f32x4_t tmp_20;


a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 * stageLength, strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 * stageLength, strideFactor);
a[2] = vmemory<float32_t,inputstrideenabled>::load(in , 2 * stageLength, strideFactor);
a[3] = vmemory<float32_t,inputstrideenabled>::load(in , 3 * stageLength, strideFactor);
a[4] = vmemory<float32_t,inputstrideenabled>::load(in , 4 * stageLength, strideFactor);
a[5] = vmemory<float32_t,inputstrideenabled>::load(in , 5 * stageLength, strideFactor);
a[6] = vmemory<float32_t,inputstrideenabled>::load(in , 6 * stageLength, strideFactor);
a[7] = vmemory<float32_t,inputstrideenabled>::load(in , 7 * stageLength, strideFactor);


tmp_0 = vaddq(a[0] , a[4]) ;
tmp_1 = vaddq(a[2] , a[6]) ;
tmp_2 = vaddq(tmp_0 , tmp_1) ;
tmp_3 = vaddq(a[1] , a[5]) ;
tmp_4 = vaddq(a[3] , a[7]) ;
tmp_5 = vaddq(tmp_3 , tmp_4) ;
res[0] = vaddq(tmp_2 , tmp_5) ;
tmp_6 = vsubq(a[0] , a[4]) ;
tmp_7 = vsubq(a[2] , a[6]) ;
tmp_8 =  vrot(tmp_7)  ;
tmp_9 = vsubq(tmp_6 , tmp_8) ;
tmp_10 = vsubq(a[3] , a[7]) ;
tmp_11 = vcomplex_mulq_n(tmp_10,(C2_8_tw) ) ;
tmp_12 = vsubq(a[1] , a[5]) ;
tmp_13 = vcomplex_mulq_n(tmp_12,(C1_8_tw) ) ;
tmp_14 = vaddq(tmp_11 , tmp_13) ;
res[1] = vcomplex_mulq(w[0] , vaddq(tmp_9 , tmp_14) ) ;
tmp_15 = vsubq(tmp_0 , tmp_1) ;
tmp_16 = vsubq(tmp_3 , tmp_4) ;
tmp_17 =  vrot(tmp_16)  ;
res[2] = vcomplex_mulq(w[1] , vsubq(tmp_15 , tmp_17) ) ;
tmp_18 = vaddq(tmp_6 , tmp_8) ;
tmp_19 = vsubq(tmp_13 , tmp_11) ;
tmp_20 =  vrot(tmp_19)  ;
res[3] = vcomplex_mulq(w[2] , vsubq(tmp_18 , tmp_20) ) ;
res[4] = vcomplex_mulq(w[3] , vsubq(tmp_2 , tmp_5) ) ;
res[5] = vcomplex_mulq(w[4] , vsubq(tmp_9 , tmp_14) ) ;
res[6] = vcomplex_mulq(w[5] , vaddq(tmp_15 , tmp_17) ) ;
res[7] = vcomplex_mulq(w[6] , vaddq(tmp_18 , tmp_20) ) ;


vmemory<float32_t,inputstrideenabled>::store(in, 0 * stageLength, res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 1 * stageLength, res[1], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 2 * stageLength, res[2], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 3 * stageLength, res[3], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 4 * stageLength, res[4], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 5 * stageLength, res[5], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 6 * stageLength, res[6], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 7 * stageLength, res[7], strideFactor);


};

/* Nb Multiplications = 5, 
Nb Additions = 24,
Nb Negates = 12,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_notw(int stageLength,const complex<float32_t> *in,complex<float32_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4_t a[8];
f32x4_t res[8];


 f32x4_t tmp_0;
 f32x4_t tmp_1;
 f32x4_t tmp_2;
 f32x4_t tmp_3;
 f32x4_t tmp_4;
 f32x4_t tmp_5;
 f32x4_t tmp_6;
 f32x4_t tmp_7;
 f32x4_t tmp_8;
 f32x4_t tmp_9;
 f32x4_t tmp_10;
 f32x4_t tmp_11;
 f32x4_t tmp_12;
 f32x4_t tmp_13;
 f32x4_t tmp_14;
 f32x4_t tmp_15;
 f32x4_t tmp_16;
 f32x4_t tmp_17;
 f32x4_t tmp_18;
 f32x4_t tmp_19;
 f32x4_t tmp_20;


a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 * stageLength, strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 * stageLength, strideFactor);
a[2] = vmemory<float32_t,inputstrideenabled>::load(in , 2 * stageLength, strideFactor);
a[3] = vmemory<float32_t,inputstrideenabled>::load(in , 3 * stageLength, strideFactor);
a[4] = vmemory<float32_t,inputstrideenabled>::load(in , 4 * stageLength, strideFactor);
a[5] = vmemory<float32_t,inputstrideenabled>::load(in , 5 * stageLength, strideFactor);
a[6] = vmemory<float32_t,inputstrideenabled>::load(in , 6 * stageLength, strideFactor);
a[7] = vmemory<float32_t,inputstrideenabled>::load(in , 7 * stageLength, strideFactor);


tmp_0 = vaddq(a[0] , a[4]) ;
tmp_1 = vaddq(a[2] , a[6]) ;
tmp_2 = vaddq(tmp_0 , tmp_1) ;
tmp_3 = vaddq(a[1] , a[5]) ;
tmp_4 = vaddq(a[3] , a[7]) ;
tmp_5 = vaddq(tmp_3 , tmp_4) ;
res[0] = vaddq(tmp_2 , tmp_5) ;
tmp_6 = vsubq(a[0] , a[4]) ;
tmp_7 = vsubq(a[2] , a[6]) ;
tmp_8 =  vrot(tmp_7)  ;
tmp_9 = vsubq(tmp_6 , tmp_8) ;
tmp_10 = vsubq(a[3] , a[7]) ;
tmp_11 = vcomplex_mulq_n(tmp_10,(C2_8_notw) ) ;
tmp_12 = vsubq(a[1] , a[5]) ;
tmp_13 = vcomplex_mulq_n(tmp_12,(C1_8_notw) ) ;
tmp_14 = vaddq(tmp_11 , tmp_13) ;
res[1] = vaddq(tmp_9 , tmp_14) ;
tmp_15 = vsubq(tmp_0 , tmp_1) ;
tmp_16 = vsubq(tmp_3 , tmp_4) ;
tmp_17 =  vrot(tmp_16)  ;
res[2] = vsubq(tmp_15 , tmp_17) ;
tmp_18 = vaddq(tmp_6 , tmp_8) ;
tmp_19 = vsubq(tmp_13 , tmp_11) ;
tmp_20 =  vrot(tmp_19)  ;
res[3] = vsubq(tmp_18 , tmp_20) ;
res[4] = vsubq(tmp_2 , tmp_5) ;
res[5] = vsubq(tmp_9 , tmp_14) ;
res[6] = vaddq(tmp_15 , tmp_17) ;
res[7] = vaddq(tmp_18 , tmp_20) ;


vmemory<float32_t,inputstrideenabled>::store(out, 0 * stageLength , res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 1 * stageLength , res[1], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 2 * stageLength , res[2], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 3 * stageLength , res[3], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 4 * stageLength , res[4], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 5 * stageLength , res[5], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 6 * stageLength , res[6], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 7 * stageLength , res[7], strideFactor);


};

/* Nb Multiplications = 12, 
Nb Additions = 24,
Nb Negates = 12,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_tw(int stageLength,const f32x4_t *w,const complex<float32_t> *in,complex<float32_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4_t a[8];
f32x4_t res[8];


 f32x4_t tmp_0;
 f32x4_t tmp_1;
 f32x4_t tmp_2;
 f32x4_t tmp_3;
 f32x4_t tmp_4;
 f32x4_t tmp_5;
 f32x4_t tmp_6;
 f32x4_t tmp_7;
 f32x4_t tmp_8;
 f32x4_t tmp_9;
 f32x4_t tmp_10;
 f32x4_t tmp_11;
 f32x4_t tmp_12;
 f32x4_t tmp_13;
 f32x4_t tmp_14;
 f32x4_t tmp_15;
 f32x4_t tmp_16;
 f32x4_t tmp_17;
 f32x4_t tmp_18;
 f32x4_t tmp_19;
 f32x4_t tmp_20;


a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 * stageLength, strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 * stageLength, strideFactor);
a[2] = vmemory<float32_t,inputstrideenabled>::load(in , 2 * stageLength, strideFactor);
a[3] = vmemory<float32_t,inputstrideenabled>::load(in , 3 * stageLength, strideFactor);
a[4] = vmemory<float32_t,inputstrideenabled>::load(in , 4 * stageLength, strideFactor);
a[5] = vmemory<float32_t,inputstrideenabled>::load(in , 5 * stageLength, strideFactor);
a[6] = vmemory<float32_t,inputstrideenabled>::load(in , 6 * stageLength, strideFactor);
a[7] = vmemory<float32_t,inputstrideenabled>::load(in , 7 * stageLength, strideFactor);


tmp_0 = vaddq(a[0] , a[4]) ;
tmp_1 = vaddq(a[2] , a[6]) ;
tmp_2 = vaddq(tmp_0 , tmp_1) ;
tmp_3 = vaddq(a[1] , a[5]) ;
tmp_4 = vaddq(a[3] , a[7]) ;
tmp_5 = vaddq(tmp_3 , tmp_4) ;
res[0] = vaddq(tmp_2 , tmp_5) ;
tmp_6 = vsubq(a[0] , a[4]) ;
tmp_7 = vsubq(a[2] , a[6]) ;
tmp_8 =  vrot(tmp_7)  ;
tmp_9 = vsubq(tmp_6 , tmp_8) ;
tmp_10 = vsubq(a[3] , a[7]) ;
tmp_11 = vcomplex_mulq_n(tmp_10,(C2_8_tw) ) ;
tmp_12 = vsubq(a[1] , a[5]) ;
tmp_13 = vcomplex_mulq_n(tmp_12,(C1_8_tw) ) ;
tmp_14 = vaddq(tmp_11 , tmp_13) ;
res[1] = vcomplex_mulq(w[0] , vaddq(tmp_9 , tmp_14) ) ;
tmp_15 = vsubq(tmp_0 , tmp_1) ;
tmp_16 = vsubq(tmp_3 , tmp_4) ;
tmp_17 =  vrot(tmp_16)  ;
res[2] = vcomplex_mulq(w[1] , vsubq(tmp_15 , tmp_17) ) ;
tmp_18 = vaddq(tmp_6 , tmp_8) ;
tmp_19 = vsubq(tmp_13 , tmp_11) ;
tmp_20 =  vrot(tmp_19)  ;
res[3] = vcomplex_mulq(w[2] , vsubq(tmp_18 , tmp_20) ) ;
res[4] = vcomplex_mulq(w[3] , vsubq(tmp_2 , tmp_5) ) ;
res[5] = vcomplex_mulq(w[4] , vsubq(tmp_9 , tmp_14) ) ;
res[6] = vcomplex_mulq(w[5] , vaddq(tmp_15 , tmp_17) ) ;
res[7] = vcomplex_mulq(w[6] , vaddq(tmp_18 , tmp_20) ) ;


vmemory<float32_t,inputstrideenabled>::store(out, 0 * stageLength , res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 1 * stageLength , res[1], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 2 * stageLength , res[2], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 3 * stageLength , res[3], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 4 * stageLength , res[4], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 5 * stageLength , res[5], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 6 * stageLength , res[6], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 7 * stageLength , res[7], strideFactor);


};

/* Nb Multiplications = 5, 
Nb Additions = 24,
Nb Negates = 12,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_last(complex<float32_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4_t a[8];
f32x4_t res[8];


 f32x4_t tmp_0;
 f32x4_t tmp_1;
 f32x4_t tmp_2;
 f32x4_t tmp_3;
 f32x4_t tmp_4;
 f32x4_t tmp_5;
 f32x4_t tmp_6;
 f32x4_t tmp_7;
 f32x4_t tmp_8;
 f32x4_t tmp_9;
 f32x4_t tmp_10;
 f32x4_t tmp_11;
 f32x4_t tmp_12;
 f32x4_t tmp_13;
 f32x4_t tmp_14;
 f32x4_t tmp_15;
 f32x4_t tmp_16;
 f32x4_t tmp_17;
 f32x4_t tmp_18;
 f32x4_t tmp_19;
 f32x4_t tmp_20;


a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 , strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 , strideFactor);
a[2] = vmemory<float32_t,inputstrideenabled>::load(in , 2 , strideFactor);
a[3] = vmemory<float32_t,inputstrideenabled>::load(in , 3 , strideFactor);
a[4] = vmemory<float32_t,inputstrideenabled>::load(in , 4 , strideFactor);
a[5] = vmemory<float32_t,inputstrideenabled>::load(in , 5 , strideFactor);
a[6] = vmemory<float32_t,inputstrideenabled>::load(in , 6 , strideFactor);
a[7] = vmemory<float32_t,inputstrideenabled>::load(in , 7 , strideFactor);


tmp_0 = vaddq(a[0] , a[4]) ;
tmp_1 = vaddq(a[2] , a[6]) ;
tmp_2 = vaddq(tmp_0 , tmp_1) ;
tmp_3 = vaddq(a[1] , a[5]) ;
tmp_4 = vaddq(a[3] , a[7]) ;
tmp_5 = vaddq(tmp_3 , tmp_4) ;
res[0] = vaddq(tmp_2 , tmp_5) ;
tmp_6 = vsubq(a[0] , a[4]) ;
tmp_7 = vsubq(a[2] , a[6]) ;
tmp_8 =  vrot(tmp_7)  ;
tmp_9 = vsubq(tmp_6 , tmp_8) ;
tmp_10 = vsubq(a[3] , a[7]) ;
tmp_11 = vcomplex_mulq_n(tmp_10,(C2_8_notw) ) ;
tmp_12 = vsubq(a[1] , a[5]) ;
tmp_13 = vcomplex_mulq_n(tmp_12,(C1_8_notw) ) ;
tmp_14 = vaddq(tmp_11 , tmp_13) ;
res[1] = vaddq(tmp_9 , tmp_14) ;
tmp_15 = vsubq(tmp_0 , tmp_1) ;
tmp_16 = vsubq(tmp_3 , tmp_4) ;
tmp_17 =  vrot(tmp_16)  ;
res[2] = vsubq(tmp_15 , tmp_17) ;
tmp_18 = vaddq(tmp_6 , tmp_8) ;
tmp_19 = vsubq(tmp_13 , tmp_11) ;
tmp_20 =  vrot(tmp_19)  ;
res[3] = vsubq(tmp_18 , tmp_20) ;
res[4] = vsubq(tmp_2 , tmp_5) ;
res[5] = vsubq(tmp_9 , tmp_14) ;
res[6] = vaddq(tmp_15 , tmp_17) ;
res[7] = vaddq(tmp_18 , tmp_20) ;


vmemory<float32_t,inputstrideenabled>::store(in, 0 , res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 1 , res[1], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 2 , res[2], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 3 , res[3], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 4 , res[4], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 5 , res[5], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 6 , res[6], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 7 , res[7], strideFactor);


};

/* Nb Multiplications = 5, 
Nb Additions = 24,
Nb Negates = 12,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_last(const complex<float32_t> *in,complex<float32_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4_t a[8];
f32x4_t res[8];


 f32x4_t tmp_0;
 f32x4_t tmp_1;
 f32x4_t tmp_2;
 f32x4_t tmp_3;
 f32x4_t tmp_4;
 f32x4_t tmp_5;
 f32x4_t tmp_6;
 f32x4_t tmp_7;
 f32x4_t tmp_8;
 f32x4_t tmp_9;
 f32x4_t tmp_10;
 f32x4_t tmp_11;
 f32x4_t tmp_12;
 f32x4_t tmp_13;
 f32x4_t tmp_14;
 f32x4_t tmp_15;
 f32x4_t tmp_16;
 f32x4_t tmp_17;
 f32x4_t tmp_18;
 f32x4_t tmp_19;
 f32x4_t tmp_20;


a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 , strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 , strideFactor);
a[2] = vmemory<float32_t,inputstrideenabled>::load(in , 2 , strideFactor);
a[3] = vmemory<float32_t,inputstrideenabled>::load(in , 3 , strideFactor);
a[4] = vmemory<float32_t,inputstrideenabled>::load(in , 4 , strideFactor);
a[5] = vmemory<float32_t,inputstrideenabled>::load(in , 5 , strideFactor);
a[6] = vmemory<float32_t,inputstrideenabled>::load(in , 6 , strideFactor);
a[7] = vmemory<float32_t,inputstrideenabled>::load(in , 7 , strideFactor);


tmp_0 = vaddq(a[0] , a[4]) ;
tmp_1 = vaddq(a[2] , a[6]) ;
tmp_2 = vaddq(tmp_0 , tmp_1) ;
tmp_3 = vaddq(a[1] , a[5]) ;
tmp_4 = vaddq(a[3] , a[7]) ;
tmp_5 = vaddq(tmp_3 , tmp_4) ;
res[0] = vaddq(tmp_2 , tmp_5) ;
tmp_6 = vsubq(a[0] , a[4]) ;
tmp_7 = vsubq(a[2] , a[6]) ;
tmp_8 =  vrot(tmp_7)  ;
tmp_9 = vsubq(tmp_6 , tmp_8) ;
tmp_10 = vsubq(a[3] , a[7]) ;
tmp_11 = vcomplex_mulq_n(tmp_10,(C2_8_notw) ) ;
tmp_12 = vsubq(a[1] , a[5]) ;
tmp_13 = vcomplex_mulq_n(tmp_12,(C1_8_notw) ) ;
tmp_14 = vaddq(tmp_11 , tmp_13) ;
res[1] = vaddq(tmp_9 , tmp_14) ;
tmp_15 = vsubq(tmp_0 , tmp_1) ;
tmp_16 = vsubq(tmp_3 , tmp_4) ;
tmp_17 =  vrot(tmp_16)  ;
res[2] = vsubq(tmp_15 , tmp_17) ;
tmp_18 = vaddq(tmp_6 , tmp_8) ;
tmp_19 = vsubq(tmp_13 , tmp_11) ;
tmp_20 =  vrot(tmp_19)  ;
res[3] = vsubq(tmp_18 , tmp_20) ;
res[4] = vsubq(tmp_2 , tmp_5) ;
res[5] = vsubq(tmp_9 , tmp_14) ;
res[6] = vaddq(tmp_15 , tmp_17) ;
res[7] = vaddq(tmp_18 , tmp_20) ;


vmemory<float32_t,inputstrideenabled>::store(out, 0, res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 1, res[1], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 2, res[2], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 3, res[3], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 4, res[4], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 5, res[5], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 6, res[6], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 7, res[7], strideFactor);


};

};

/**************

INVERSE FFT 

***************/
template<int inputstrideenabled> 
struct radixVector<INVERSE,float32_t, 8,inputstrideenabled>{

static constexpr complex<float32_t> C0_8_tw{0.0f, 1.0f};
  static constexpr complex<float32_t> C1_8_tw{0.7071067811865476f, 0.7071067811865476f};
  static constexpr complex<float32_t> C2_8_tw{-0.7071067811865476f, 0.7071067811865476f};
  
static constexpr complex<float32_t> C0_8_notw{0.0f, 1.0f};
  static constexpr complex<float32_t> C1_8_notw{0.7071067811865476f, 0.7071067811865476f};
  static constexpr complex<float32_t> C2_8_notw{-0.7071067811865476f, 0.7071067811865476f};
  

/* Nb Multiplications = 5, 
Nb Additions = 24,
Nb Negates = 12,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_notw(int stageLength,complex<float32_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4_t a[8];
f32x4_t res[8];


 f32x4_t tmp_0;
 f32x4_t tmp_1;
 f32x4_t tmp_2;
 f32x4_t tmp_3;
 f32x4_t tmp_4;
 f32x4_t tmp_5;
 f32x4_t tmp_6;
 f32x4_t tmp_7;
 f32x4_t tmp_8;
 f32x4_t tmp_9;
 f32x4_t tmp_10;
 f32x4_t tmp_11;
 f32x4_t tmp_12;
 f32x4_t tmp_13;
 f32x4_t tmp_14;
 f32x4_t tmp_15;
 f32x4_t tmp_16;
 f32x4_t tmp_17;
 f32x4_t tmp_18;
 f32x4_t tmp_19;
 f32x4_t tmp_20;


a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 * stageLength, strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 * stageLength, strideFactor);
a[2] = vmemory<float32_t,inputstrideenabled>::load(in , 2 * stageLength, strideFactor);
a[3] = vmemory<float32_t,inputstrideenabled>::load(in , 3 * stageLength, strideFactor);
a[4] = vmemory<float32_t,inputstrideenabled>::load(in , 4 * stageLength, strideFactor);
a[5] = vmemory<float32_t,inputstrideenabled>::load(in , 5 * stageLength, strideFactor);
a[6] = vmemory<float32_t,inputstrideenabled>::load(in , 6 * stageLength, strideFactor);
a[7] = vmemory<float32_t,inputstrideenabled>::load(in , 7 * stageLength, strideFactor);


tmp_0 = vaddq(a[0] , a[4]) ;
tmp_1 = vaddq(a[2] , a[6]) ;
tmp_2 = vaddq(tmp_0 , tmp_1) ;
tmp_3 = vaddq(a[1] , a[5]) ;
tmp_4 = vaddq(a[3] , a[7]) ;
tmp_5 = vaddq(tmp_3 , tmp_4) ;
res[0] = vaddq(tmp_2 , tmp_5) ;
tmp_6 = vsubq(a[0] , a[4]) ;
tmp_7 = vsubq(a[2] , a[6]) ;
tmp_8 =  vrot(tmp_7)  ;
tmp_9 = vaddq(tmp_6 , tmp_8) ;
tmp_10 = vsubq(a[3] , a[7]) ;
tmp_11 = vcomplex_mulq_n(tmp_10,(C2_8_notw) ) ;
tmp_12 = vsubq(a[1] , a[5]) ;
tmp_13 = vcomplex_mulq_n(tmp_12,(C1_8_notw) ) ;
tmp_14 = vaddq(tmp_11 , tmp_13) ;
res[1] = vaddq(tmp_9 , tmp_14) ;
tmp_15 = vsubq(tmp_0 , tmp_1) ;
tmp_16 = vsubq(tmp_3 , tmp_4) ;
tmp_17 =  vrot(tmp_16)  ;
res[2] = vaddq(tmp_15 , tmp_17) ;
tmp_18 = vsubq(tmp_6 , tmp_8) ;
tmp_19 = vsubq(tmp_13 , tmp_11) ;
tmp_20 =  vrot(tmp_19)  ;
res[3] = vaddq(tmp_18 , tmp_20) ;
res[4] = vsubq(tmp_2 , tmp_5) ;
res[5] = vsubq(tmp_9 , tmp_14) ;
res[6] = vsubq(tmp_15 , tmp_17) ;
res[7] = vsubq(tmp_18 , tmp_20) ;


vmemory<float32_t,inputstrideenabled>::store(in, 0 * stageLength, res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 1 * stageLength, res[1], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 2 * stageLength, res[2], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 3 * stageLength, res[3], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 4 * stageLength, res[4], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 5 * stageLength, res[5], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 6 * stageLength, res[6], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 7 * stageLength, res[7], strideFactor);


};

/* Nb Multiplications = 12, 
Nb Additions = 24,
Nb Negates = 12,
Nb Conjugates = 7,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_tw(int stageLength,const f32x4_t *w,complex<float32_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4_t a[8];
f32x4_t res[8];


 f32x4_t tmp_0;
 f32x4_t tmp_1;
 f32x4_t tmp_2;
 f32x4_t tmp_3;
 f32x4_t tmp_4;
 f32x4_t tmp_5;
 f32x4_t tmp_6;
 f32x4_t tmp_7;
 f32x4_t tmp_8;
 f32x4_t tmp_9;
 f32x4_t tmp_10;
 f32x4_t tmp_11;
 f32x4_t tmp_12;
 f32x4_t tmp_13;
 f32x4_t tmp_14;
 f32x4_t tmp_15;
 f32x4_t tmp_16;
 f32x4_t tmp_17;
 f32x4_t tmp_18;
 f32x4_t tmp_19;
 f32x4_t tmp_20;


a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 * stageLength, strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 * stageLength, strideFactor);
a[2] = vmemory<float32_t,inputstrideenabled>::load(in , 2 * stageLength, strideFactor);
a[3] = vmemory<float32_t,inputstrideenabled>::load(in , 3 * stageLength, strideFactor);
a[4] = vmemory<float32_t,inputstrideenabled>::load(in , 4 * stageLength, strideFactor);
a[5] = vmemory<float32_t,inputstrideenabled>::load(in , 5 * stageLength, strideFactor);
a[6] = vmemory<float32_t,inputstrideenabled>::load(in , 6 * stageLength, strideFactor);
a[7] = vmemory<float32_t,inputstrideenabled>::load(in , 7 * stageLength, strideFactor);


tmp_0 = vaddq(a[0] , a[4]) ;
tmp_1 = vaddq(a[2] , a[6]) ;
tmp_2 = vaddq(tmp_0 , tmp_1) ;
tmp_3 = vaddq(a[1] , a[5]) ;
tmp_4 = vaddq(a[3] , a[7]) ;
tmp_5 = vaddq(tmp_3 , tmp_4) ;
res[0] = vaddq(tmp_2 , tmp_5) ;
tmp_6 = vsubq(a[0] , a[4]) ;
tmp_7 = vsubq(a[2] , a[6]) ;
tmp_8 =  vrot(tmp_7)  ;
tmp_9 = vaddq(tmp_6 , tmp_8) ;
tmp_10 = vsubq(a[3] , a[7]) ;
tmp_11 = vcomplex_mulq_n(tmp_10,(C2_8_tw) ) ;
tmp_12 = vsubq(a[1] , a[5]) ;
tmp_13 = vcomplex_mulq_n(tmp_12,(C1_8_tw) ) ;
tmp_14 = vaddq(tmp_11 , tmp_13) ;
res[1] = vcomplex_mulq( vconjugate(w[0])   , vaddq(tmp_9 , tmp_14) ) ;
tmp_15 = vsubq(tmp_0 , tmp_1) ;
tmp_16 = vsubq(tmp_3 , tmp_4) ;
tmp_17 =  vrot(tmp_16)  ;
res[2] = vcomplex_mulq( vconjugate(w[1])   , vaddq(tmp_15 , tmp_17) ) ;
tmp_18 = vsubq(tmp_6 , tmp_8) ;
tmp_19 = vsubq(tmp_13 , tmp_11) ;
tmp_20 =  vrot(tmp_19)  ;
res[3] = vcomplex_mulq( vconjugate(w[2])   , vaddq(tmp_18 , tmp_20) ) ;
res[4] = vcomplex_mulq( vconjugate(w[3])   , vsubq(tmp_2 , tmp_5) ) ;
res[5] = vcomplex_mulq( vconjugate(w[4])   , vsubq(tmp_9 , tmp_14) ) ;
res[6] = vcomplex_mulq( vconjugate(w[5])   , vsubq(tmp_15 , tmp_17) ) ;
res[7] = vcomplex_mulq( vconjugate(w[6])   , vsubq(tmp_18 , tmp_20) ) ;


vmemory<float32_t,inputstrideenabled>::store(in, 0 * stageLength, res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 1 * stageLength, res[1], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 2 * stageLength, res[2], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 3 * stageLength, res[3], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 4 * stageLength, res[4], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 5 * stageLength, res[5], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 6 * stageLength, res[6], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 7 * stageLength, res[7], strideFactor);


};

/* Nb Multiplications = 5, 
Nb Additions = 24,
Nb Negates = 12,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_notw(int stageLength,const complex<float32_t> *in,complex<float32_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4_t a[8];
f32x4_t res[8];


 f32x4_t tmp_0;
 f32x4_t tmp_1;
 f32x4_t tmp_2;
 f32x4_t tmp_3;
 f32x4_t tmp_4;
 f32x4_t tmp_5;
 f32x4_t tmp_6;
 f32x4_t tmp_7;
 f32x4_t tmp_8;
 f32x4_t tmp_9;
 f32x4_t tmp_10;
 f32x4_t tmp_11;
 f32x4_t tmp_12;
 f32x4_t tmp_13;
 f32x4_t tmp_14;
 f32x4_t tmp_15;
 f32x4_t tmp_16;
 f32x4_t tmp_17;
 f32x4_t tmp_18;
 f32x4_t tmp_19;
 f32x4_t tmp_20;


a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 * stageLength, strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 * stageLength, strideFactor);
a[2] = vmemory<float32_t,inputstrideenabled>::load(in , 2 * stageLength, strideFactor);
a[3] = vmemory<float32_t,inputstrideenabled>::load(in , 3 * stageLength, strideFactor);
a[4] = vmemory<float32_t,inputstrideenabled>::load(in , 4 * stageLength, strideFactor);
a[5] = vmemory<float32_t,inputstrideenabled>::load(in , 5 * stageLength, strideFactor);
a[6] = vmemory<float32_t,inputstrideenabled>::load(in , 6 * stageLength, strideFactor);
a[7] = vmemory<float32_t,inputstrideenabled>::load(in , 7 * stageLength, strideFactor);


tmp_0 = vaddq(a[0] , a[4]) ;
tmp_1 = vaddq(a[2] , a[6]) ;
tmp_2 = vaddq(tmp_0 , tmp_1) ;
tmp_3 = vaddq(a[1] , a[5]) ;
tmp_4 = vaddq(a[3] , a[7]) ;
tmp_5 = vaddq(tmp_3 , tmp_4) ;
res[0] = vaddq(tmp_2 , tmp_5) ;
tmp_6 = vsubq(a[0] , a[4]) ;
tmp_7 = vsubq(a[2] , a[6]) ;
tmp_8 =  vrot(tmp_7)  ;
tmp_9 = vaddq(tmp_6 , tmp_8) ;
tmp_10 = vsubq(a[3] , a[7]) ;
tmp_11 = vcomplex_mulq_n(tmp_10,(C2_8_notw) ) ;
tmp_12 = vsubq(a[1] , a[5]) ;
tmp_13 = vcomplex_mulq_n(tmp_12,(C1_8_notw) ) ;
tmp_14 = vaddq(tmp_11 , tmp_13) ;
res[1] = vaddq(tmp_9 , tmp_14) ;
tmp_15 = vsubq(tmp_0 , tmp_1) ;
tmp_16 = vsubq(tmp_3 , tmp_4) ;
tmp_17 =  vrot(tmp_16)  ;
res[2] = vaddq(tmp_15 , tmp_17) ;
tmp_18 = vsubq(tmp_6 , tmp_8) ;
tmp_19 = vsubq(tmp_13 , tmp_11) ;
tmp_20 =  vrot(tmp_19)  ;
res[3] = vaddq(tmp_18 , tmp_20) ;
res[4] = vsubq(tmp_2 , tmp_5) ;
res[5] = vsubq(tmp_9 , tmp_14) ;
res[6] = vsubq(tmp_15 , tmp_17) ;
res[7] = vsubq(tmp_18 , tmp_20) ;


vmemory<float32_t,inputstrideenabled>::store(out, 0 * stageLength , res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 1 * stageLength , res[1], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 2 * stageLength , res[2], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 3 * stageLength , res[3], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 4 * stageLength , res[4], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 5 * stageLength , res[5], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 6 * stageLength , res[6], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 7 * stageLength , res[7], strideFactor);


};

/* Nb Multiplications = 12, 
Nb Additions = 24,
Nb Negates = 12,
Nb Conjugates = 7,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_tw(int stageLength,const f32x4_t *w,const complex<float32_t> *in,complex<float32_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4_t a[8];
f32x4_t res[8];


 f32x4_t tmp_0;
 f32x4_t tmp_1;
 f32x4_t tmp_2;
 f32x4_t tmp_3;
 f32x4_t tmp_4;
 f32x4_t tmp_5;
 f32x4_t tmp_6;
 f32x4_t tmp_7;
 f32x4_t tmp_8;
 f32x4_t tmp_9;
 f32x4_t tmp_10;
 f32x4_t tmp_11;
 f32x4_t tmp_12;
 f32x4_t tmp_13;
 f32x4_t tmp_14;
 f32x4_t tmp_15;
 f32x4_t tmp_16;
 f32x4_t tmp_17;
 f32x4_t tmp_18;
 f32x4_t tmp_19;
 f32x4_t tmp_20;


a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 * stageLength, strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 * stageLength, strideFactor);
a[2] = vmemory<float32_t,inputstrideenabled>::load(in , 2 * stageLength, strideFactor);
a[3] = vmemory<float32_t,inputstrideenabled>::load(in , 3 * stageLength, strideFactor);
a[4] = vmemory<float32_t,inputstrideenabled>::load(in , 4 * stageLength, strideFactor);
a[5] = vmemory<float32_t,inputstrideenabled>::load(in , 5 * stageLength, strideFactor);
a[6] = vmemory<float32_t,inputstrideenabled>::load(in , 6 * stageLength, strideFactor);
a[7] = vmemory<float32_t,inputstrideenabled>::load(in , 7 * stageLength, strideFactor);


tmp_0 = vaddq(a[0] , a[4]) ;
tmp_1 = vaddq(a[2] , a[6]) ;
tmp_2 = vaddq(tmp_0 , tmp_1) ;
tmp_3 = vaddq(a[1] , a[5]) ;
tmp_4 = vaddq(a[3] , a[7]) ;
tmp_5 = vaddq(tmp_3 , tmp_4) ;
res[0] = vaddq(tmp_2 , tmp_5) ;
tmp_6 = vsubq(a[0] , a[4]) ;
tmp_7 = vsubq(a[2] , a[6]) ;
tmp_8 =  vrot(tmp_7)  ;
tmp_9 = vaddq(tmp_6 , tmp_8) ;
tmp_10 = vsubq(a[3] , a[7]) ;
tmp_11 = vcomplex_mulq_n(tmp_10,(C2_8_tw) ) ;
tmp_12 = vsubq(a[1] , a[5]) ;
tmp_13 = vcomplex_mulq_n(tmp_12,(C1_8_tw) ) ;
tmp_14 = vaddq(tmp_11 , tmp_13) ;
res[1] = vcomplex_mulq( vconjugate(w[0])   , vaddq(tmp_9 , tmp_14) ) ;
tmp_15 = vsubq(tmp_0 , tmp_1) ;
tmp_16 = vsubq(tmp_3 , tmp_4) ;
tmp_17 =  vrot(tmp_16)  ;
res[2] = vcomplex_mulq( vconjugate(w[1])   , vaddq(tmp_15 , tmp_17) ) ;
tmp_18 = vsubq(tmp_6 , tmp_8) ;
tmp_19 = vsubq(tmp_13 , tmp_11) ;
tmp_20 =  vrot(tmp_19)  ;
res[3] = vcomplex_mulq( vconjugate(w[2])   , vaddq(tmp_18 , tmp_20) ) ;
res[4] = vcomplex_mulq( vconjugate(w[3])   , vsubq(tmp_2 , tmp_5) ) ;
res[5] = vcomplex_mulq( vconjugate(w[4])   , vsubq(tmp_9 , tmp_14) ) ;
res[6] = vcomplex_mulq( vconjugate(w[5])   , vsubq(tmp_15 , tmp_17) ) ;
res[7] = vcomplex_mulq( vconjugate(w[6])   , vsubq(tmp_18 , tmp_20) ) ;


vmemory<float32_t,inputstrideenabled>::store(out, 0 * stageLength , res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 1 * stageLength , res[1], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 2 * stageLength , res[2], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 3 * stageLength , res[3], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 4 * stageLength , res[4], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 5 * stageLength , res[5], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 6 * stageLength , res[6], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 7 * stageLength , res[7], strideFactor);


};

/* Nb Multiplications = 5, 
Nb Additions = 24,
Nb Negates = 12,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_last(complex<float32_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4_t a[8];
f32x4_t res[8];


 f32x4_t tmp_0;
 f32x4_t tmp_1;
 f32x4_t tmp_2;
 f32x4_t tmp_3;
 f32x4_t tmp_4;
 f32x4_t tmp_5;
 f32x4_t tmp_6;
 f32x4_t tmp_7;
 f32x4_t tmp_8;
 f32x4_t tmp_9;
 f32x4_t tmp_10;
 f32x4_t tmp_11;
 f32x4_t tmp_12;
 f32x4_t tmp_13;
 f32x4_t tmp_14;
 f32x4_t tmp_15;
 f32x4_t tmp_16;
 f32x4_t tmp_17;
 f32x4_t tmp_18;
 f32x4_t tmp_19;
 f32x4_t tmp_20;


a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 , strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 , strideFactor);
a[2] = vmemory<float32_t,inputstrideenabled>::load(in , 2 , strideFactor);
a[3] = vmemory<float32_t,inputstrideenabled>::load(in , 3 , strideFactor);
a[4] = vmemory<float32_t,inputstrideenabled>::load(in , 4 , strideFactor);
a[5] = vmemory<float32_t,inputstrideenabled>::load(in , 5 , strideFactor);
a[6] = vmemory<float32_t,inputstrideenabled>::load(in , 6 , strideFactor);
a[7] = vmemory<float32_t,inputstrideenabled>::load(in , 7 , strideFactor);


tmp_0 = vaddq(a[0] , a[4]) ;
tmp_1 = vaddq(a[2] , a[6]) ;
tmp_2 = vaddq(tmp_0 , tmp_1) ;
tmp_3 = vaddq(a[1] , a[5]) ;
tmp_4 = vaddq(a[3] , a[7]) ;
tmp_5 = vaddq(tmp_3 , tmp_4) ;
res[0] = vaddq(tmp_2 , tmp_5) ;
tmp_6 = vsubq(a[0] , a[4]) ;
tmp_7 = vsubq(a[2] , a[6]) ;
tmp_8 =  vrot(tmp_7)  ;
tmp_9 = vaddq(tmp_6 , tmp_8) ;
tmp_10 = vsubq(a[3] , a[7]) ;
tmp_11 = vcomplex_mulq_n(tmp_10,(C2_8_notw) ) ;
tmp_12 = vsubq(a[1] , a[5]) ;
tmp_13 = vcomplex_mulq_n(tmp_12,(C1_8_notw) ) ;
tmp_14 = vaddq(tmp_11 , tmp_13) ;
res[1] = vaddq(tmp_9 , tmp_14) ;
tmp_15 = vsubq(tmp_0 , tmp_1) ;
tmp_16 = vsubq(tmp_3 , tmp_4) ;
tmp_17 =  vrot(tmp_16)  ;
res[2] = vaddq(tmp_15 , tmp_17) ;
tmp_18 = vsubq(tmp_6 , tmp_8) ;
tmp_19 = vsubq(tmp_13 , tmp_11) ;
tmp_20 =  vrot(tmp_19)  ;
res[3] = vaddq(tmp_18 , tmp_20) ;
res[4] = vsubq(tmp_2 , tmp_5) ;
res[5] = vsubq(tmp_9 , tmp_14) ;
res[6] = vsubq(tmp_15 , tmp_17) ;
res[7] = vsubq(tmp_18 , tmp_20) ;


vmemory<float32_t,inputstrideenabled>::store(in, 0 , res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 1 , res[1], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 2 , res[2], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 3 , res[3], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 4 , res[4], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 5 , res[5], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 6 , res[6], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 7 , res[7], strideFactor);


};

/* Nb Multiplications = 5, 
Nb Additions = 24,
Nb Negates = 12,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_last(const complex<float32_t> *in,complex<float32_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4_t a[8];
f32x4_t res[8];


 f32x4_t tmp_0;
 f32x4_t tmp_1;
 f32x4_t tmp_2;
 f32x4_t tmp_3;
 f32x4_t tmp_4;
 f32x4_t tmp_5;
 f32x4_t tmp_6;
 f32x4_t tmp_7;
 f32x4_t tmp_8;
 f32x4_t tmp_9;
 f32x4_t tmp_10;
 f32x4_t tmp_11;
 f32x4_t tmp_12;
 f32x4_t tmp_13;
 f32x4_t tmp_14;
 f32x4_t tmp_15;
 f32x4_t tmp_16;
 f32x4_t tmp_17;
 f32x4_t tmp_18;
 f32x4_t tmp_19;
 f32x4_t tmp_20;


a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 , strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 , strideFactor);
a[2] = vmemory<float32_t,inputstrideenabled>::load(in , 2 , strideFactor);
a[3] = vmemory<float32_t,inputstrideenabled>::load(in , 3 , strideFactor);
a[4] = vmemory<float32_t,inputstrideenabled>::load(in , 4 , strideFactor);
a[5] = vmemory<float32_t,inputstrideenabled>::load(in , 5 , strideFactor);
a[6] = vmemory<float32_t,inputstrideenabled>::load(in , 6 , strideFactor);
a[7] = vmemory<float32_t,inputstrideenabled>::load(in , 7 , strideFactor);


tmp_0 = vaddq(a[0] , a[4]) ;
tmp_1 = vaddq(a[2] , a[6]) ;
tmp_2 = vaddq(tmp_0 , tmp_1) ;
tmp_3 = vaddq(a[1] , a[5]) ;
tmp_4 = vaddq(a[3] , a[7]) ;
tmp_5 = vaddq(tmp_3 , tmp_4) ;
res[0] = vaddq(tmp_2 , tmp_5) ;
tmp_6 = vsubq(a[0] , a[4]) ;
tmp_7 = vsubq(a[2] , a[6]) ;
tmp_8 =  vrot(tmp_7)  ;
tmp_9 = vaddq(tmp_6 , tmp_8) ;
tmp_10 = vsubq(a[3] , a[7]) ;
tmp_11 = vcomplex_mulq_n(tmp_10,(C2_8_notw) ) ;
tmp_12 = vsubq(a[1] , a[5]) ;
tmp_13 = vcomplex_mulq_n(tmp_12,(C1_8_notw) ) ;
tmp_14 = vaddq(tmp_11 , tmp_13) ;
res[1] = vaddq(tmp_9 , tmp_14) ;
tmp_15 = vsubq(tmp_0 , tmp_1) ;
tmp_16 = vsubq(tmp_3 , tmp_4) ;
tmp_17 =  vrot(tmp_16)  ;
res[2] = vaddq(tmp_15 , tmp_17) ;
tmp_18 = vsubq(tmp_6 , tmp_8) ;
tmp_19 = vsubq(tmp_13 , tmp_11) ;
tmp_20 =  vrot(tmp_19)  ;
res[3] = vaddq(tmp_18 , tmp_20) ;
res[4] = vsubq(tmp_2 , tmp_5) ;
res[5] = vsubq(tmp_9 , tmp_14) ;
res[6] = vsubq(tmp_15 , tmp_17) ;
res[7] = vsubq(tmp_18 , tmp_20) ;


vmemory<float32_t,inputstrideenabled>::store(out, 0, res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 1, res[1], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 2, res[2], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 3, res[3], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 4, res[4], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 5, res[5], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 6, res[6], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 7, res[7], strideFactor);


};

};

#endif /* #ifndef _RADIX_VECTOR_F32_8_H */