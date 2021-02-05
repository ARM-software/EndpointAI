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

#ifndef _RADIX_VECTOR_F32_6_H
#define _RADIX_VECTOR_F32_6_H

#include "complex.h"

/**************

DIRECT FFT 

***************/
template<int inputstrideenabled> 
struct radixVector<DIRECT,float32_t, 6,inputstrideenabled>{

static constexpr complex<float32_t> C0_6_tw{-0.5f, -0.8660254037844386f};
  static constexpr complex<float32_t> C1_6_tw{-0.5f, 0.8660254037844386f};
  static constexpr complex<float32_t> C2_6_tw{0.5f, -0.8660254037844386f};
  
static constexpr complex<float32_t> C0_6_notw{-0.5f, -0.8660254037844386f};
  static constexpr complex<float32_t> C1_6_notw{-0.5f, 0.8660254037844386f};
  static constexpr complex<float32_t> C2_6_notw{0.5f, -0.8660254037844386f};
  

/* Nb Multiplications = 10, 
Nb Additions = 18,
Nb Negates = 3,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_notw(int stageLength,complex<float32_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4_t a[6];
f32x4_t res[6];


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


a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 * stageLength, strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 * stageLength, strideFactor);
a[2] = vmemory<float32_t,inputstrideenabled>::load(in , 2 * stageLength, strideFactor);
a[3] = vmemory<float32_t,inputstrideenabled>::load(in , 3 * stageLength, strideFactor);
a[4] = vmemory<float32_t,inputstrideenabled>::load(in , 4 * stageLength, strideFactor);
a[5] = vmemory<float32_t,inputstrideenabled>::load(in , 5 * stageLength, strideFactor);


tmp_0 = vaddq(a[0] , a[3]) ;
tmp_1 = vaddq(a[1] , a[4]) ;
tmp_2 = vaddq(a[2] , a[5]) ;
tmp_8 = vaddq(tmp_0 , vaddq(tmp_1 , tmp_2) ) ;
res[0] = tmp_8;
tmp_3 = vsubq(a[0] , a[3]) ;
tmp_4 = vsubq(a[2] , a[5]) ;
tmp_5 = vcomplex_mulq_n(tmp_4,(C0_6_notw) ) ;
tmp_6 = vsubq(a[1] , a[4]) ;
tmp_7 = vcomplex_mulq_n(tmp_6,(C2_6_notw) ) ;
tmp_9 = vaddq(tmp_3 , vaddq(tmp_5 , tmp_7) ) ;
res[1] = tmp_9;
tmp_10 = vaddq(tmp_0 , vaddq(vcomplex_mulq_n(tmp_1,(C0_6_notw) )  , vcomplex_mulq_n(tmp_2,(C1_6_notw) ) ) ) ;
res[2] = tmp_10;
tmp_11 = vaddq(tmp_3 , vaddq(vcomplex_mulq_n(tmp_7,(C0_6_notw) )  , vcomplex_mulq_n(tmp_5,(C1_6_notw) ) ) ) ;
res[3] = tmp_11;
tmp_12 = vaddq(tmp_0 , vaddq(vcomplex_mulq_n(tmp_2,(C0_6_notw) )  , vcomplex_mulq_n(tmp_1,(C1_6_notw) ) ) ) ;
res[4] = tmp_12;
tmp_13 = vaddq(tmp_3 , vaddq(vcomplex_mulq_n(tmp_5,(C0_6_notw) )  , vcomplex_mulq_n(tmp_7,(C1_6_notw) ) ) ) ;
res[5] = tmp_13;


vmemory<float32_t,inputstrideenabled>::store(in, 0 * stageLength, res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 1 * stageLength, res[1], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 2 * stageLength, res[2], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 3 * stageLength, res[3], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 4 * stageLength, res[4], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 5 * stageLength, res[5], strideFactor);


};

/* Nb Multiplications = 15, 
Nb Additions = 18,
Nb Negates = 3,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_tw(int stageLength,const f32x4_t *w,complex<float32_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4_t a[6];
f32x4_t res[6];


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


a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 * stageLength, strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 * stageLength, strideFactor);
a[2] = vmemory<float32_t,inputstrideenabled>::load(in , 2 * stageLength, strideFactor);
a[3] = vmemory<float32_t,inputstrideenabled>::load(in , 3 * stageLength, strideFactor);
a[4] = vmemory<float32_t,inputstrideenabled>::load(in , 4 * stageLength, strideFactor);
a[5] = vmemory<float32_t,inputstrideenabled>::load(in , 5 * stageLength, strideFactor);


tmp_0 = vaddq(a[0] , a[3]) ;
tmp_1 = vaddq(a[1] , a[4]) ;
tmp_2 = vaddq(a[2] , a[5]) ;
tmp_8 = vaddq(tmp_0 , vaddq(tmp_1 , tmp_2) ) ;
res[0] = tmp_8;
tmp_3 = vsubq(a[0] , a[3]) ;
tmp_4 = vsubq(a[2] , a[5]) ;
tmp_5 = vcomplex_mulq_n(tmp_4,(C0_6_tw) ) ;
tmp_6 = vsubq(a[1] , a[4]) ;
tmp_7 = vcomplex_mulq_n(tmp_6,(C2_6_tw) ) ;
tmp_9 = vaddq(tmp_3 , vaddq(tmp_5 , tmp_7) ) ;
res[1] = vcomplex_mulq(w[0] , tmp_9) ;
tmp_10 = vaddq(tmp_0 , vaddq(vcomplex_mulq_n(tmp_1,(C0_6_tw) )  , vcomplex_mulq_n(tmp_2,(C1_6_tw) ) ) ) ;
res[2] = vcomplex_mulq(w[1] , tmp_10) ;
tmp_11 = vaddq(tmp_3 , vaddq(vcomplex_mulq_n(tmp_7,(C0_6_tw) )  , vcomplex_mulq_n(tmp_5,(C1_6_tw) ) ) ) ;
res[3] = vcomplex_mulq(w[2] , tmp_11) ;
tmp_12 = vaddq(tmp_0 , vaddq(vcomplex_mulq_n(tmp_2,(C0_6_tw) )  , vcomplex_mulq_n(tmp_1,(C1_6_tw) ) ) ) ;
res[4] = vcomplex_mulq(w[3] , tmp_12) ;
tmp_13 = vaddq(tmp_3 , vaddq(vcomplex_mulq_n(tmp_5,(C0_6_tw) )  , vcomplex_mulq_n(tmp_7,(C1_6_tw) ) ) ) ;
res[5] = vcomplex_mulq(w[4] , tmp_13) ;


vmemory<float32_t,inputstrideenabled>::store(in, 0 * stageLength, res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 1 * stageLength, res[1], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 2 * stageLength, res[2], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 3 * stageLength, res[3], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 4 * stageLength, res[4], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 5 * stageLength, res[5], strideFactor);


};

/* Nb Multiplications = 10, 
Nb Additions = 18,
Nb Negates = 3,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_notw(int stageLength,const complex<float32_t> *in,complex<float32_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4_t a[6];
f32x4_t res[6];


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


a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 * stageLength, strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 * stageLength, strideFactor);
a[2] = vmemory<float32_t,inputstrideenabled>::load(in , 2 * stageLength, strideFactor);
a[3] = vmemory<float32_t,inputstrideenabled>::load(in , 3 * stageLength, strideFactor);
a[4] = vmemory<float32_t,inputstrideenabled>::load(in , 4 * stageLength, strideFactor);
a[5] = vmemory<float32_t,inputstrideenabled>::load(in , 5 * stageLength, strideFactor);


tmp_0 = vaddq(a[0] , a[3]) ;
tmp_1 = vaddq(a[1] , a[4]) ;
tmp_2 = vaddq(a[2] , a[5]) ;
tmp_8 = vaddq(tmp_0 , vaddq(tmp_1 , tmp_2) ) ;
res[0] = tmp_8;
tmp_3 = vsubq(a[0] , a[3]) ;
tmp_4 = vsubq(a[2] , a[5]) ;
tmp_5 = vcomplex_mulq_n(tmp_4,(C0_6_notw) ) ;
tmp_6 = vsubq(a[1] , a[4]) ;
tmp_7 = vcomplex_mulq_n(tmp_6,(C2_6_notw) ) ;
tmp_9 = vaddq(tmp_3 , vaddq(tmp_5 , tmp_7) ) ;
res[1] = tmp_9;
tmp_10 = vaddq(tmp_0 , vaddq(vcomplex_mulq_n(tmp_1,(C0_6_notw) )  , vcomplex_mulq_n(tmp_2,(C1_6_notw) ) ) ) ;
res[2] = tmp_10;
tmp_11 = vaddq(tmp_3 , vaddq(vcomplex_mulq_n(tmp_7,(C0_6_notw) )  , vcomplex_mulq_n(tmp_5,(C1_6_notw) ) ) ) ;
res[3] = tmp_11;
tmp_12 = vaddq(tmp_0 , vaddq(vcomplex_mulq_n(tmp_2,(C0_6_notw) )  , vcomplex_mulq_n(tmp_1,(C1_6_notw) ) ) ) ;
res[4] = tmp_12;
tmp_13 = vaddq(tmp_3 , vaddq(vcomplex_mulq_n(tmp_5,(C0_6_notw) )  , vcomplex_mulq_n(tmp_7,(C1_6_notw) ) ) ) ;
res[5] = tmp_13;


vmemory<float32_t,inputstrideenabled>::store(out, 0 * stageLength , res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 1 * stageLength , res[1], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 2 * stageLength , res[2], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 3 * stageLength , res[3], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 4 * stageLength , res[4], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 5 * stageLength , res[5], strideFactor);


};

/* Nb Multiplications = 15, 
Nb Additions = 18,
Nb Negates = 3,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_tw(int stageLength,const f32x4_t *w,const complex<float32_t> *in,complex<float32_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4_t a[6];
f32x4_t res[6];


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


a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 * stageLength, strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 * stageLength, strideFactor);
a[2] = vmemory<float32_t,inputstrideenabled>::load(in , 2 * stageLength, strideFactor);
a[3] = vmemory<float32_t,inputstrideenabled>::load(in , 3 * stageLength, strideFactor);
a[4] = vmemory<float32_t,inputstrideenabled>::load(in , 4 * stageLength, strideFactor);
a[5] = vmemory<float32_t,inputstrideenabled>::load(in , 5 * stageLength, strideFactor);


tmp_0 = vaddq(a[0] , a[3]) ;
tmp_1 = vaddq(a[1] , a[4]) ;
tmp_2 = vaddq(a[2] , a[5]) ;
tmp_8 = vaddq(tmp_0 , vaddq(tmp_1 , tmp_2) ) ;
res[0] = tmp_8;
tmp_3 = vsubq(a[0] , a[3]) ;
tmp_4 = vsubq(a[2] , a[5]) ;
tmp_5 = vcomplex_mulq_n(tmp_4,(C0_6_tw) ) ;
tmp_6 = vsubq(a[1] , a[4]) ;
tmp_7 = vcomplex_mulq_n(tmp_6,(C2_6_tw) ) ;
tmp_9 = vaddq(tmp_3 , vaddq(tmp_5 , tmp_7) ) ;
res[1] = vcomplex_mulq(w[0] , tmp_9) ;
tmp_10 = vaddq(tmp_0 , vaddq(vcomplex_mulq_n(tmp_1,(C0_6_tw) )  , vcomplex_mulq_n(tmp_2,(C1_6_tw) ) ) ) ;
res[2] = vcomplex_mulq(w[1] , tmp_10) ;
tmp_11 = vaddq(tmp_3 , vaddq(vcomplex_mulq_n(tmp_7,(C0_6_tw) )  , vcomplex_mulq_n(tmp_5,(C1_6_tw) ) ) ) ;
res[3] = vcomplex_mulq(w[2] , tmp_11) ;
tmp_12 = vaddq(tmp_0 , vaddq(vcomplex_mulq_n(tmp_2,(C0_6_tw) )  , vcomplex_mulq_n(tmp_1,(C1_6_tw) ) ) ) ;
res[4] = vcomplex_mulq(w[3] , tmp_12) ;
tmp_13 = vaddq(tmp_3 , vaddq(vcomplex_mulq_n(tmp_5,(C0_6_tw) )  , vcomplex_mulq_n(tmp_7,(C1_6_tw) ) ) ) ;
res[5] = vcomplex_mulq(w[4] , tmp_13) ;


vmemory<float32_t,inputstrideenabled>::store(out, 0 * stageLength , res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 1 * stageLength , res[1], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 2 * stageLength , res[2], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 3 * stageLength , res[3], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 4 * stageLength , res[4], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 5 * stageLength , res[5], strideFactor);


};

/* Nb Multiplications = 10, 
Nb Additions = 18,
Nb Negates = 3,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_last(complex<float32_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4_t a[6];
f32x4_t res[6];


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


a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 , strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 , strideFactor);
a[2] = vmemory<float32_t,inputstrideenabled>::load(in , 2 , strideFactor);
a[3] = vmemory<float32_t,inputstrideenabled>::load(in , 3 , strideFactor);
a[4] = vmemory<float32_t,inputstrideenabled>::load(in , 4 , strideFactor);
a[5] = vmemory<float32_t,inputstrideenabled>::load(in , 5 , strideFactor);


tmp_0 = vaddq(a[0] , a[3]) ;
tmp_1 = vaddq(a[1] , a[4]) ;
tmp_2 = vaddq(a[2] , a[5]) ;
tmp_8 = vaddq(tmp_0 , vaddq(tmp_1 , tmp_2) ) ;
res[0] = tmp_8;
tmp_3 = vsubq(a[0] , a[3]) ;
tmp_4 = vsubq(a[2] , a[5]) ;
tmp_5 = vcomplex_mulq_n(tmp_4,(C0_6_notw) ) ;
tmp_6 = vsubq(a[1] , a[4]) ;
tmp_7 = vcomplex_mulq_n(tmp_6,(C2_6_notw) ) ;
tmp_9 = vaddq(tmp_3 , vaddq(tmp_5 , tmp_7) ) ;
res[1] = tmp_9;
tmp_10 = vaddq(tmp_0 , vaddq(vcomplex_mulq_n(tmp_1,(C0_6_notw) )  , vcomplex_mulq_n(tmp_2,(C1_6_notw) ) ) ) ;
res[2] = tmp_10;
tmp_11 = vaddq(tmp_3 , vaddq(vcomplex_mulq_n(tmp_7,(C0_6_notw) )  , vcomplex_mulq_n(tmp_5,(C1_6_notw) ) ) ) ;
res[3] = tmp_11;
tmp_12 = vaddq(tmp_0 , vaddq(vcomplex_mulq_n(tmp_2,(C0_6_notw) )  , vcomplex_mulq_n(tmp_1,(C1_6_notw) ) ) ) ;
res[4] = tmp_12;
tmp_13 = vaddq(tmp_3 , vaddq(vcomplex_mulq_n(tmp_5,(C0_6_notw) )  , vcomplex_mulq_n(tmp_7,(C1_6_notw) ) ) ) ;
res[5] = tmp_13;


vmemory<float32_t,inputstrideenabled>::store(in, 0 , res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 1 , res[1], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 2 , res[2], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 3 , res[3], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 4 , res[4], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 5 , res[5], strideFactor);


};

/* Nb Multiplications = 10, 
Nb Additions = 18,
Nb Negates = 3,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_last(const complex<float32_t> *in,complex<float32_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4_t a[6];
f32x4_t res[6];


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


a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 , strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 , strideFactor);
a[2] = vmemory<float32_t,inputstrideenabled>::load(in , 2 , strideFactor);
a[3] = vmemory<float32_t,inputstrideenabled>::load(in , 3 , strideFactor);
a[4] = vmemory<float32_t,inputstrideenabled>::load(in , 4 , strideFactor);
a[5] = vmemory<float32_t,inputstrideenabled>::load(in , 5 , strideFactor);


tmp_0 = vaddq(a[0] , a[3]) ;
tmp_1 = vaddq(a[1] , a[4]) ;
tmp_2 = vaddq(a[2] , a[5]) ;
tmp_8 = vaddq(tmp_0 , vaddq(tmp_1 , tmp_2) ) ;
res[0] = tmp_8;
tmp_3 = vsubq(a[0] , a[3]) ;
tmp_4 = vsubq(a[2] , a[5]) ;
tmp_5 = vcomplex_mulq_n(tmp_4,(C0_6_notw) ) ;
tmp_6 = vsubq(a[1] , a[4]) ;
tmp_7 = vcomplex_mulq_n(tmp_6,(C2_6_notw) ) ;
tmp_9 = vaddq(tmp_3 , vaddq(tmp_5 , tmp_7) ) ;
res[1] = tmp_9;
tmp_10 = vaddq(tmp_0 , vaddq(vcomplex_mulq_n(tmp_1,(C0_6_notw) )  , vcomplex_mulq_n(tmp_2,(C1_6_notw) ) ) ) ;
res[2] = tmp_10;
tmp_11 = vaddq(tmp_3 , vaddq(vcomplex_mulq_n(tmp_7,(C0_6_notw) )  , vcomplex_mulq_n(tmp_5,(C1_6_notw) ) ) ) ;
res[3] = tmp_11;
tmp_12 = vaddq(tmp_0 , vaddq(vcomplex_mulq_n(tmp_2,(C0_6_notw) )  , vcomplex_mulq_n(tmp_1,(C1_6_notw) ) ) ) ;
res[4] = tmp_12;
tmp_13 = vaddq(tmp_3 , vaddq(vcomplex_mulq_n(tmp_5,(C0_6_notw) )  , vcomplex_mulq_n(tmp_7,(C1_6_notw) ) ) ) ;
res[5] = tmp_13;


vmemory<float32_t,inputstrideenabled>::store(out, 0, res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 1, res[1], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 2, res[2], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 3, res[3], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 4, res[4], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 5, res[5], strideFactor);


};

};

/**************

INVERSE FFT 

***************/
template<int inputstrideenabled> 
struct radixVector<INVERSE,float32_t, 6,inputstrideenabled>{

static constexpr complex<float32_t> C0_6_tw{-0.5f, -0.8660254037844386f};
  static constexpr complex<float32_t> C1_6_tw{0.5f, 0.8660254037844386f};
  static constexpr complex<float32_t> C2_6_tw{-0.5f, 0.8660254037844386f};
  
static constexpr complex<float32_t> C0_6_notw{-0.5f, -0.8660254037844386f};
  static constexpr complex<float32_t> C1_6_notw{0.5f, 0.8660254037844386f};
  static constexpr complex<float32_t> C2_6_notw{-0.5f, 0.8660254037844386f};
  

/* Nb Multiplications = 10, 
Nb Additions = 18,
Nb Negates = 3,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_notw(int stageLength,complex<float32_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4_t a[6];
f32x4_t res[6];


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


a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 * stageLength, strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 * stageLength, strideFactor);
a[2] = vmemory<float32_t,inputstrideenabled>::load(in , 2 * stageLength, strideFactor);
a[3] = vmemory<float32_t,inputstrideenabled>::load(in , 3 * stageLength, strideFactor);
a[4] = vmemory<float32_t,inputstrideenabled>::load(in , 4 * stageLength, strideFactor);
a[5] = vmemory<float32_t,inputstrideenabled>::load(in , 5 * stageLength, strideFactor);


tmp_0 = vaddq(a[0] , a[3]) ;
tmp_1 = vaddq(a[1] , a[4]) ;
tmp_2 = vaddq(a[2] , a[5]) ;
tmp_8 = vaddq(tmp_0 , vaddq(tmp_1 , tmp_2) ) ;
res[0] = tmp_8;
tmp_3 = vsubq(a[0] , a[3]) ;
tmp_4 = vsubq(a[2] , a[5]) ;
tmp_5 = vcomplex_mulq_n(tmp_4,(C2_6_notw) ) ;
tmp_6 = vsubq(a[1] , a[4]) ;
tmp_7 = vcomplex_mulq_n(tmp_6,(C1_6_notw) ) ;
tmp_9 = vaddq(tmp_3 , vaddq(tmp_5 , tmp_7) ) ;
res[1] = tmp_9;
tmp_10 = vaddq(tmp_0 , vaddq(vcomplex_mulq_n(tmp_2,(C0_6_notw) )  , vcomplex_mulq_n(tmp_1,(C2_6_notw) ) ) ) ;
res[2] = tmp_10;
tmp_11 = vaddq(tmp_3 , vaddq(vcomplex_mulq_n(tmp_5,(C0_6_notw) )  , vcomplex_mulq_n(tmp_7,(C2_6_notw) ) ) ) ;
res[3] = tmp_11;
tmp_12 = vaddq(tmp_0 , vaddq(vcomplex_mulq_n(tmp_1,(C0_6_notw) )  , vcomplex_mulq_n(tmp_2,(C2_6_notw) ) ) ) ;
res[4] = tmp_12;
tmp_13 = vaddq(tmp_3 , vaddq(vcomplex_mulq_n(tmp_7,(C0_6_notw) )  , vcomplex_mulq_n(tmp_5,(C2_6_notw) ) ) ) ;
res[5] = tmp_13;


vmemory<float32_t,inputstrideenabled>::store(in, 0 * stageLength, res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 1 * stageLength, res[1], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 2 * stageLength, res[2], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 3 * stageLength, res[3], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 4 * stageLength, res[4], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 5 * stageLength, res[5], strideFactor);


};

/* Nb Multiplications = 15, 
Nb Additions = 18,
Nb Negates = 3,
Nb Conjugates = 5,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_tw(int stageLength,const f32x4_t *w,complex<float32_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4_t a[6];
f32x4_t res[6];


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


a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 * stageLength, strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 * stageLength, strideFactor);
a[2] = vmemory<float32_t,inputstrideenabled>::load(in , 2 * stageLength, strideFactor);
a[3] = vmemory<float32_t,inputstrideenabled>::load(in , 3 * stageLength, strideFactor);
a[4] = vmemory<float32_t,inputstrideenabled>::load(in , 4 * stageLength, strideFactor);
a[5] = vmemory<float32_t,inputstrideenabled>::load(in , 5 * stageLength, strideFactor);


tmp_0 = vaddq(a[0] , a[3]) ;
tmp_1 = vaddq(a[1] , a[4]) ;
tmp_2 = vaddq(a[2] , a[5]) ;
tmp_8 = vaddq(tmp_0 , vaddq(tmp_1 , tmp_2) ) ;
res[0] = tmp_8;
tmp_3 = vsubq(a[0] , a[3]) ;
tmp_4 = vsubq(a[2] , a[5]) ;
tmp_5 = vcomplex_mulq_n(tmp_4,(C2_6_tw) ) ;
tmp_6 = vsubq(a[1] , a[4]) ;
tmp_7 = vcomplex_mulq_n(tmp_6,(C1_6_tw) ) ;
tmp_9 = vaddq(tmp_3 , vaddq(tmp_5 , tmp_7) ) ;
res[1] = vcomplex_mulq( vconjugate(w[0])   , tmp_9) ;
tmp_10 = vaddq(tmp_0 , vaddq(vcomplex_mulq_n(tmp_2,(C0_6_tw) )  , vcomplex_mulq_n(tmp_1,(C2_6_tw) ) ) ) ;
res[2] = vcomplex_mulq( vconjugate(w[1])   , tmp_10) ;
tmp_11 = vaddq(tmp_3 , vaddq(vcomplex_mulq_n(tmp_5,(C0_6_tw) )  , vcomplex_mulq_n(tmp_7,(C2_6_tw) ) ) ) ;
res[3] = vcomplex_mulq( vconjugate(w[2])   , tmp_11) ;
tmp_12 = vaddq(tmp_0 , vaddq(vcomplex_mulq_n(tmp_1,(C0_6_tw) )  , vcomplex_mulq_n(tmp_2,(C2_6_tw) ) ) ) ;
res[4] = vcomplex_mulq( vconjugate(w[3])   , tmp_12) ;
tmp_13 = vaddq(tmp_3 , vaddq(vcomplex_mulq_n(tmp_7,(C0_6_tw) )  , vcomplex_mulq_n(tmp_5,(C2_6_tw) ) ) ) ;
res[5] = vcomplex_mulq( vconjugate(w[4])   , tmp_13) ;


vmemory<float32_t,inputstrideenabled>::store(in, 0 * stageLength, res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 1 * stageLength, res[1], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 2 * stageLength, res[2], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 3 * stageLength, res[3], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 4 * stageLength, res[4], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 5 * stageLength, res[5], strideFactor);


};

/* Nb Multiplications = 10, 
Nb Additions = 18,
Nb Negates = 3,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_notw(int stageLength,const complex<float32_t> *in,complex<float32_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4_t a[6];
f32x4_t res[6];


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


a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 * stageLength, strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 * stageLength, strideFactor);
a[2] = vmemory<float32_t,inputstrideenabled>::load(in , 2 * stageLength, strideFactor);
a[3] = vmemory<float32_t,inputstrideenabled>::load(in , 3 * stageLength, strideFactor);
a[4] = vmemory<float32_t,inputstrideenabled>::load(in , 4 * stageLength, strideFactor);
a[5] = vmemory<float32_t,inputstrideenabled>::load(in , 5 * stageLength, strideFactor);


tmp_0 = vaddq(a[0] , a[3]) ;
tmp_1 = vaddq(a[1] , a[4]) ;
tmp_2 = vaddq(a[2] , a[5]) ;
tmp_8 = vaddq(tmp_0 , vaddq(tmp_1 , tmp_2) ) ;
res[0] = tmp_8;
tmp_3 = vsubq(a[0] , a[3]) ;
tmp_4 = vsubq(a[2] , a[5]) ;
tmp_5 = vcomplex_mulq_n(tmp_4,(C2_6_notw) ) ;
tmp_6 = vsubq(a[1] , a[4]) ;
tmp_7 = vcomplex_mulq_n(tmp_6,(C1_6_notw) ) ;
tmp_9 = vaddq(tmp_3 , vaddq(tmp_5 , tmp_7) ) ;
res[1] = tmp_9;
tmp_10 = vaddq(tmp_0 , vaddq(vcomplex_mulq_n(tmp_2,(C0_6_notw) )  , vcomplex_mulq_n(tmp_1,(C2_6_notw) ) ) ) ;
res[2] = tmp_10;
tmp_11 = vaddq(tmp_3 , vaddq(vcomplex_mulq_n(tmp_5,(C0_6_notw) )  , vcomplex_mulq_n(tmp_7,(C2_6_notw) ) ) ) ;
res[3] = tmp_11;
tmp_12 = vaddq(tmp_0 , vaddq(vcomplex_mulq_n(tmp_1,(C0_6_notw) )  , vcomplex_mulq_n(tmp_2,(C2_6_notw) ) ) ) ;
res[4] = tmp_12;
tmp_13 = vaddq(tmp_3 , vaddq(vcomplex_mulq_n(tmp_7,(C0_6_notw) )  , vcomplex_mulq_n(tmp_5,(C2_6_notw) ) ) ) ;
res[5] = tmp_13;


vmemory<float32_t,inputstrideenabled>::store(out, 0 * stageLength , res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 1 * stageLength , res[1], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 2 * stageLength , res[2], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 3 * stageLength , res[3], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 4 * stageLength , res[4], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 5 * stageLength , res[5], strideFactor);


};

/* Nb Multiplications = 15, 
Nb Additions = 18,
Nb Negates = 3,
Nb Conjugates = 5,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_tw(int stageLength,const f32x4_t *w,const complex<float32_t> *in,complex<float32_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4_t a[6];
f32x4_t res[6];


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


a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 * stageLength, strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 * stageLength, strideFactor);
a[2] = vmemory<float32_t,inputstrideenabled>::load(in , 2 * stageLength, strideFactor);
a[3] = vmemory<float32_t,inputstrideenabled>::load(in , 3 * stageLength, strideFactor);
a[4] = vmemory<float32_t,inputstrideenabled>::load(in , 4 * stageLength, strideFactor);
a[5] = vmemory<float32_t,inputstrideenabled>::load(in , 5 * stageLength, strideFactor);


tmp_0 = vaddq(a[0] , a[3]) ;
tmp_1 = vaddq(a[1] , a[4]) ;
tmp_2 = vaddq(a[2] , a[5]) ;
tmp_8 = vaddq(tmp_0 , vaddq(tmp_1 , tmp_2) ) ;
res[0] = tmp_8;
tmp_3 = vsubq(a[0] , a[3]) ;
tmp_4 = vsubq(a[2] , a[5]) ;
tmp_5 = vcomplex_mulq_n(tmp_4,(C2_6_tw) ) ;
tmp_6 = vsubq(a[1] , a[4]) ;
tmp_7 = vcomplex_mulq_n(tmp_6,(C1_6_tw) ) ;
tmp_9 = vaddq(tmp_3 , vaddq(tmp_5 , tmp_7) ) ;
res[1] = vcomplex_mulq( vconjugate(w[0])   , tmp_9) ;
tmp_10 = vaddq(tmp_0 , vaddq(vcomplex_mulq_n(tmp_2,(C0_6_tw) )  , vcomplex_mulq_n(tmp_1,(C2_6_tw) ) ) ) ;
res[2] = vcomplex_mulq( vconjugate(w[1])   , tmp_10) ;
tmp_11 = vaddq(tmp_3 , vaddq(vcomplex_mulq_n(tmp_5,(C0_6_tw) )  , vcomplex_mulq_n(tmp_7,(C2_6_tw) ) ) ) ;
res[3] = vcomplex_mulq( vconjugate(w[2])   , tmp_11) ;
tmp_12 = vaddq(tmp_0 , vaddq(vcomplex_mulq_n(tmp_1,(C0_6_tw) )  , vcomplex_mulq_n(tmp_2,(C2_6_tw) ) ) ) ;
res[4] = vcomplex_mulq( vconjugate(w[3])   , tmp_12) ;
tmp_13 = vaddq(tmp_3 , vaddq(vcomplex_mulq_n(tmp_7,(C0_6_tw) )  , vcomplex_mulq_n(tmp_5,(C2_6_tw) ) ) ) ;
res[5] = vcomplex_mulq( vconjugate(w[4])   , tmp_13) ;


vmemory<float32_t,inputstrideenabled>::store(out, 0 * stageLength , res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 1 * stageLength , res[1], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 2 * stageLength , res[2], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 3 * stageLength , res[3], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 4 * stageLength , res[4], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 5 * stageLength , res[5], strideFactor);


};

/* Nb Multiplications = 10, 
Nb Additions = 18,
Nb Negates = 3,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_last(complex<float32_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4_t a[6];
f32x4_t res[6];


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


a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 , strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 , strideFactor);
a[2] = vmemory<float32_t,inputstrideenabled>::load(in , 2 , strideFactor);
a[3] = vmemory<float32_t,inputstrideenabled>::load(in , 3 , strideFactor);
a[4] = vmemory<float32_t,inputstrideenabled>::load(in , 4 , strideFactor);
a[5] = vmemory<float32_t,inputstrideenabled>::load(in , 5 , strideFactor);


tmp_0 = vaddq(a[0] , a[3]) ;
tmp_1 = vaddq(a[1] , a[4]) ;
tmp_2 = vaddq(a[2] , a[5]) ;
tmp_8 = vaddq(tmp_0 , vaddq(tmp_1 , tmp_2) ) ;
res[0] = tmp_8;
tmp_3 = vsubq(a[0] , a[3]) ;
tmp_4 = vsubq(a[2] , a[5]) ;
tmp_5 = vcomplex_mulq_n(tmp_4,(C2_6_notw) ) ;
tmp_6 = vsubq(a[1] , a[4]) ;
tmp_7 = vcomplex_mulq_n(tmp_6,(C1_6_notw) ) ;
tmp_9 = vaddq(tmp_3 , vaddq(tmp_5 , tmp_7) ) ;
res[1] = tmp_9;
tmp_10 = vaddq(tmp_0 , vaddq(vcomplex_mulq_n(tmp_2,(C0_6_notw) )  , vcomplex_mulq_n(tmp_1,(C2_6_notw) ) ) ) ;
res[2] = tmp_10;
tmp_11 = vaddq(tmp_3 , vaddq(vcomplex_mulq_n(tmp_5,(C0_6_notw) )  , vcomplex_mulq_n(tmp_7,(C2_6_notw) ) ) ) ;
res[3] = tmp_11;
tmp_12 = vaddq(tmp_0 , vaddq(vcomplex_mulq_n(tmp_1,(C0_6_notw) )  , vcomplex_mulq_n(tmp_2,(C2_6_notw) ) ) ) ;
res[4] = tmp_12;
tmp_13 = vaddq(tmp_3 , vaddq(vcomplex_mulq_n(tmp_7,(C0_6_notw) )  , vcomplex_mulq_n(tmp_5,(C2_6_notw) ) ) ) ;
res[5] = tmp_13;


vmemory<float32_t,inputstrideenabled>::store(in, 0 , res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 1 , res[1], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 2 , res[2], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 3 , res[3], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 4 , res[4], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 5 , res[5], strideFactor);


};

/* Nb Multiplications = 10, 
Nb Additions = 18,
Nb Negates = 3,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_last(const complex<float32_t> *in,complex<float32_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4_t a[6];
f32x4_t res[6];


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


a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 , strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 , strideFactor);
a[2] = vmemory<float32_t,inputstrideenabled>::load(in , 2 , strideFactor);
a[3] = vmemory<float32_t,inputstrideenabled>::load(in , 3 , strideFactor);
a[4] = vmemory<float32_t,inputstrideenabled>::load(in , 4 , strideFactor);
a[5] = vmemory<float32_t,inputstrideenabled>::load(in , 5 , strideFactor);


tmp_0 = vaddq(a[0] , a[3]) ;
tmp_1 = vaddq(a[1] , a[4]) ;
tmp_2 = vaddq(a[2] , a[5]) ;
tmp_8 = vaddq(tmp_0 , vaddq(tmp_1 , tmp_2) ) ;
res[0] = tmp_8;
tmp_3 = vsubq(a[0] , a[3]) ;
tmp_4 = vsubq(a[2] , a[5]) ;
tmp_5 = vcomplex_mulq_n(tmp_4,(C2_6_notw) ) ;
tmp_6 = vsubq(a[1] , a[4]) ;
tmp_7 = vcomplex_mulq_n(tmp_6,(C1_6_notw) ) ;
tmp_9 = vaddq(tmp_3 , vaddq(tmp_5 , tmp_7) ) ;
res[1] = tmp_9;
tmp_10 = vaddq(tmp_0 , vaddq(vcomplex_mulq_n(tmp_2,(C0_6_notw) )  , vcomplex_mulq_n(tmp_1,(C2_6_notw) ) ) ) ;
res[2] = tmp_10;
tmp_11 = vaddq(tmp_3 , vaddq(vcomplex_mulq_n(tmp_5,(C0_6_notw) )  , vcomplex_mulq_n(tmp_7,(C2_6_notw) ) ) ) ;
res[3] = tmp_11;
tmp_12 = vaddq(tmp_0 , vaddq(vcomplex_mulq_n(tmp_1,(C0_6_notw) )  , vcomplex_mulq_n(tmp_2,(C2_6_notw) ) ) ) ;
res[4] = tmp_12;
tmp_13 = vaddq(tmp_3 , vaddq(vcomplex_mulq_n(tmp_7,(C0_6_notw) )  , vcomplex_mulq_n(tmp_5,(C2_6_notw) ) ) ) ;
res[5] = tmp_13;


vmemory<float32_t,inputstrideenabled>::store(out, 0, res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 1, res[1], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 2, res[2], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 3, res[3], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 4, res[4], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 5, res[5], strideFactor);


};

};

#endif /* #ifndef _RADIX_VECTOR_F32_6_H */