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

#ifndef _RADIX_VECTOR_F32_5_H
#define _RADIX_VECTOR_F32_5_H

#include "complex.h"

/**************

DIRECT FFT 

***************/
template<int inputstrideenabled> 
struct radixVector<DIRECT,float32_t, 5,inputstrideenabled>{

static constexpr complex<float32_t> C0_5_tw{-0.8090169943749475f, -0.5877852522924731f};
  static constexpr complex<float32_t> C1_5_tw{-0.8090169943749475f, 0.5877852522924731f};
  static constexpr complex<float32_t> C2_5_tw{0.30901699437494745f, -0.9510565162951535f};
  static constexpr complex<float32_t> C3_5_tw{0.30901699437494745f, 0.9510565162951535f};
  
static constexpr complex<float32_t> C0_5_notw{-0.8090169943749475f, -0.5877852522924731f};
  static constexpr complex<float32_t> C1_5_notw{-0.8090169943749475f, 0.5877852522924731f};
  static constexpr complex<float32_t> C2_5_notw{0.30901699437494745f, -0.9510565162951535f};
  static constexpr complex<float32_t> C3_5_notw{0.30901699437494745f, 0.9510565162951535f};
  

/* Nb Multiplications = 16, 
Nb Additions = 20,
Nb Negates = 0,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_notw(int stageLength,complex<float32_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4x2_t a[5];
f32x4x2_t res[5];


 f32x4x2_t tmp_0;
 f32x4x2_t tmp_1;
 f32x4x2_t tmp_2;
 f32x4x2_t tmp_3;
 f32x4x2_t tmp_4;


a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 * stageLength, strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 * stageLength, strideFactor);
a[2] = vmemory<float32_t,inputstrideenabled>::load(in , 2 * stageLength, strideFactor);
a[3] = vmemory<float32_t,inputstrideenabled>::load(in , 3 * stageLength, strideFactor);
a[4] = vmemory<float32_t,inputstrideenabled>::load(in , 4 * stageLength, strideFactor);


tmp_0 = vaddition(a[0] , vaddition(a[1] , vaddition(a[2] , vaddition(a[3] , a[4]) ) ) ) ;
res[0] = tmp_0;
tmp_1 = vaddition(vcomplex_mulq_n(a[2],(C0_5_notw) )  , vaddition(vcomplex_mulq_n(a[3],(C1_5_notw) )  , vaddition(vcomplex_mulq_n(a[1],(C2_5_notw) )  , vaddition(vcomplex_mulq_n(a[4],(C3_5_notw) )  , a[0]) ) ) ) ;
res[1] = tmp_1;
tmp_2 = vaddition(vcomplex_mulq_n(a[1],(C0_5_notw) )  , vaddition(vcomplex_mulq_n(a[4],(C1_5_notw) )  , vaddition(vcomplex_mulq_n(a[3],(C2_5_notw) )  , vaddition(vcomplex_mulq_n(a[2],(C3_5_notw) )  , a[0]) ) ) ) ;
res[2] = tmp_2;
tmp_3 = vaddition(vcomplex_mulq_n(a[4],(C0_5_notw) )  , vaddition(vcomplex_mulq_n(a[1],(C1_5_notw) )  , vaddition(vcomplex_mulq_n(a[2],(C2_5_notw) )  , vaddition(vcomplex_mulq_n(a[3],(C3_5_notw) )  , a[0]) ) ) ) ;
res[3] = tmp_3;
tmp_4 = vaddition(vcomplex_mulq_n(a[3],(C0_5_notw) )  , vaddition(vcomplex_mulq_n(a[2],(C1_5_notw) )  , vaddition(vcomplex_mulq_n(a[4],(C2_5_notw) )  , vaddition(vcomplex_mulq_n(a[1],(C3_5_notw) )  , a[0]) ) ) ) ;
res[4] = tmp_4;


vmemory<float32_t,inputstrideenabled>::store(in, 0 * stageLength, res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 1 * stageLength, res[1], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 2 * stageLength, res[2], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 3 * stageLength, res[3], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 4 * stageLength, res[4], strideFactor);


};

/* Nb Multiplications = 20, 
Nb Additions = 20,
Nb Negates = 0,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_tw(int stageLength,const f32x4x2_t *w,complex<float32_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4x2_t a[5];
f32x4x2_t res[5];


 f32x4x2_t tmp_0;
 f32x4x2_t tmp_1;
 f32x4x2_t tmp_2;
 f32x4x2_t tmp_3;
 f32x4x2_t tmp_4;


a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 * stageLength, strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 * stageLength, strideFactor);
a[2] = vmemory<float32_t,inputstrideenabled>::load(in , 2 * stageLength, strideFactor);
a[3] = vmemory<float32_t,inputstrideenabled>::load(in , 3 * stageLength, strideFactor);
a[4] = vmemory<float32_t,inputstrideenabled>::load(in , 4 * stageLength, strideFactor);


tmp_0 = vaddition(a[0] , vaddition(a[1] , vaddition(a[2] , vaddition(a[3] , a[4]) ) ) ) ;
res[0] = tmp_0;
tmp_1 = vaddition(vcomplex_mulq_n(a[2],(C0_5_tw) )  , vaddition(vcomplex_mulq_n(a[3],(C1_5_tw) )  , vaddition(vcomplex_mulq_n(a[1],(C2_5_tw) )  , vaddition(vcomplex_mulq_n(a[4],(C3_5_tw) )  , a[0]) ) ) ) ;
res[1] = vcomplex_mulq(w[0] , tmp_1) ;
tmp_2 = vaddition(vcomplex_mulq_n(a[1],(C0_5_tw) )  , vaddition(vcomplex_mulq_n(a[4],(C1_5_tw) )  , vaddition(vcomplex_mulq_n(a[3],(C2_5_tw) )  , vaddition(vcomplex_mulq_n(a[2],(C3_5_tw) )  , a[0]) ) ) ) ;
res[2] = vcomplex_mulq(w[1] , tmp_2) ;
tmp_3 = vaddition(vcomplex_mulq_n(a[4],(C0_5_tw) )  , vaddition(vcomplex_mulq_n(a[1],(C1_5_tw) )  , vaddition(vcomplex_mulq_n(a[2],(C2_5_tw) )  , vaddition(vcomplex_mulq_n(a[3],(C3_5_tw) )  , a[0]) ) ) ) ;
res[3] = vcomplex_mulq(w[2] , tmp_3) ;
tmp_4 = vaddition(vcomplex_mulq_n(a[3],(C0_5_tw) )  , vaddition(vcomplex_mulq_n(a[2],(C1_5_tw) )  , vaddition(vcomplex_mulq_n(a[4],(C2_5_tw) )  , vaddition(vcomplex_mulq_n(a[1],(C3_5_tw) )  , a[0]) ) ) ) ;
res[4] = vcomplex_mulq(w[3] , tmp_4) ;


vmemory<float32_t,inputstrideenabled>::store(in, 0 * stageLength, res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 1 * stageLength, res[1], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 2 * stageLength, res[2], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 3 * stageLength, res[3], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 4 * stageLength, res[4], strideFactor);


};

/* Nb Multiplications = 16, 
Nb Additions = 20,
Nb Negates = 0,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_notw(int stageLength,const complex<float32_t> *in,complex<float32_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4x2_t a[5];
f32x4x2_t res[5];


 f32x4x2_t tmp_0;
 f32x4x2_t tmp_1;
 f32x4x2_t tmp_2;
 f32x4x2_t tmp_3;
 f32x4x2_t tmp_4;


a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 * stageLength, strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 * stageLength, strideFactor);
a[2] = vmemory<float32_t,inputstrideenabled>::load(in , 2 * stageLength, strideFactor);
a[3] = vmemory<float32_t,inputstrideenabled>::load(in , 3 * stageLength, strideFactor);
a[4] = vmemory<float32_t,inputstrideenabled>::load(in , 4 * stageLength, strideFactor);


tmp_0 = vaddition(a[0] , vaddition(a[1] , vaddition(a[2] , vaddition(a[3] , a[4]) ) ) ) ;
res[0] = tmp_0;
tmp_1 = vaddition(vcomplex_mulq_n(a[2],(C0_5_notw) )  , vaddition(vcomplex_mulq_n(a[3],(C1_5_notw) )  , vaddition(vcomplex_mulq_n(a[1],(C2_5_notw) )  , vaddition(vcomplex_mulq_n(a[4],(C3_5_notw) )  , a[0]) ) ) ) ;
res[1] = tmp_1;
tmp_2 = vaddition(vcomplex_mulq_n(a[1],(C0_5_notw) )  , vaddition(vcomplex_mulq_n(a[4],(C1_5_notw) )  , vaddition(vcomplex_mulq_n(a[3],(C2_5_notw) )  , vaddition(vcomplex_mulq_n(a[2],(C3_5_notw) )  , a[0]) ) ) ) ;
res[2] = tmp_2;
tmp_3 = vaddition(vcomplex_mulq_n(a[4],(C0_5_notw) )  , vaddition(vcomplex_mulq_n(a[1],(C1_5_notw) )  , vaddition(vcomplex_mulq_n(a[2],(C2_5_notw) )  , vaddition(vcomplex_mulq_n(a[3],(C3_5_notw) )  , a[0]) ) ) ) ;
res[3] = tmp_3;
tmp_4 = vaddition(vcomplex_mulq_n(a[3],(C0_5_notw) )  , vaddition(vcomplex_mulq_n(a[2],(C1_5_notw) )  , vaddition(vcomplex_mulq_n(a[4],(C2_5_notw) )  , vaddition(vcomplex_mulq_n(a[1],(C3_5_notw) )  , a[0]) ) ) ) ;
res[4] = tmp_4;


vmemory<float32_t,inputstrideenabled>::store(out, 0 * stageLength , res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 1 * stageLength , res[1], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 2 * stageLength , res[2], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 3 * stageLength , res[3], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 4 * stageLength , res[4], strideFactor);


};

/* Nb Multiplications = 20, 
Nb Additions = 20,
Nb Negates = 0,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_tw(int stageLength,const f32x4x2_t *w,const complex<float32_t> *in,complex<float32_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4x2_t a[5];
f32x4x2_t res[5];


 f32x4x2_t tmp_0;
 f32x4x2_t tmp_1;
 f32x4x2_t tmp_2;
 f32x4x2_t tmp_3;
 f32x4x2_t tmp_4;


a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 * stageLength, strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 * stageLength, strideFactor);
a[2] = vmemory<float32_t,inputstrideenabled>::load(in , 2 * stageLength, strideFactor);
a[3] = vmemory<float32_t,inputstrideenabled>::load(in , 3 * stageLength, strideFactor);
a[4] = vmemory<float32_t,inputstrideenabled>::load(in , 4 * stageLength, strideFactor);


tmp_0 = vaddition(a[0] , vaddition(a[1] , vaddition(a[2] , vaddition(a[3] , a[4]) ) ) ) ;
res[0] = tmp_0;
tmp_1 = vaddition(vcomplex_mulq_n(a[2],(C0_5_tw) )  , vaddition(vcomplex_mulq_n(a[3],(C1_5_tw) )  , vaddition(vcomplex_mulq_n(a[1],(C2_5_tw) )  , vaddition(vcomplex_mulq_n(a[4],(C3_5_tw) )  , a[0]) ) ) ) ;
res[1] = vcomplex_mulq(w[0] , tmp_1) ;
tmp_2 = vaddition(vcomplex_mulq_n(a[1],(C0_5_tw) )  , vaddition(vcomplex_mulq_n(a[4],(C1_5_tw) )  , vaddition(vcomplex_mulq_n(a[3],(C2_5_tw) )  , vaddition(vcomplex_mulq_n(a[2],(C3_5_tw) )  , a[0]) ) ) ) ;
res[2] = vcomplex_mulq(w[1] , tmp_2) ;
tmp_3 = vaddition(vcomplex_mulq_n(a[4],(C0_5_tw) )  , vaddition(vcomplex_mulq_n(a[1],(C1_5_tw) )  , vaddition(vcomplex_mulq_n(a[2],(C2_5_tw) )  , vaddition(vcomplex_mulq_n(a[3],(C3_5_tw) )  , a[0]) ) ) ) ;
res[3] = vcomplex_mulq(w[2] , tmp_3) ;
tmp_4 = vaddition(vcomplex_mulq_n(a[3],(C0_5_tw) )  , vaddition(vcomplex_mulq_n(a[2],(C1_5_tw) )  , vaddition(vcomplex_mulq_n(a[4],(C2_5_tw) )  , vaddition(vcomplex_mulq_n(a[1],(C3_5_tw) )  , a[0]) ) ) ) ;
res[4] = vcomplex_mulq(w[3] , tmp_4) ;


vmemory<float32_t,inputstrideenabled>::store(out, 0 * stageLength , res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 1 * stageLength , res[1], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 2 * stageLength , res[2], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 3 * stageLength , res[3], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 4 * stageLength , res[4], strideFactor);


};

/* Nb Multiplications = 16, 
Nb Additions = 20,
Nb Negates = 0,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_last(complex<float32_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4x2_t a[5];
f32x4x2_t res[5];


 f32x4x2_t tmp_0;
 f32x4x2_t tmp_1;
 f32x4x2_t tmp_2;
 f32x4x2_t tmp_3;
 f32x4x2_t tmp_4;


a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 , strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 , strideFactor);
a[2] = vmemory<float32_t,inputstrideenabled>::load(in , 2 , strideFactor);
a[3] = vmemory<float32_t,inputstrideenabled>::load(in , 3 , strideFactor);
a[4] = vmemory<float32_t,inputstrideenabled>::load(in , 4 , strideFactor);


tmp_0 = vaddition(a[0] , vaddition(a[1] , vaddition(a[2] , vaddition(a[3] , a[4]) ) ) ) ;
res[0] = tmp_0;
tmp_1 = vaddition(vcomplex_mulq_n(a[2],(C0_5_notw) )  , vaddition(vcomplex_mulq_n(a[3],(C1_5_notw) )  , vaddition(vcomplex_mulq_n(a[1],(C2_5_notw) )  , vaddition(vcomplex_mulq_n(a[4],(C3_5_notw) )  , a[0]) ) ) ) ;
res[1] = tmp_1;
tmp_2 = vaddition(vcomplex_mulq_n(a[1],(C0_5_notw) )  , vaddition(vcomplex_mulq_n(a[4],(C1_5_notw) )  , vaddition(vcomplex_mulq_n(a[3],(C2_5_notw) )  , vaddition(vcomplex_mulq_n(a[2],(C3_5_notw) )  , a[0]) ) ) ) ;
res[2] = tmp_2;
tmp_3 = vaddition(vcomplex_mulq_n(a[4],(C0_5_notw) )  , vaddition(vcomplex_mulq_n(a[1],(C1_5_notw) )  , vaddition(vcomplex_mulq_n(a[2],(C2_5_notw) )  , vaddition(vcomplex_mulq_n(a[3],(C3_5_notw) )  , a[0]) ) ) ) ;
res[3] = tmp_3;
tmp_4 = vaddition(vcomplex_mulq_n(a[3],(C0_5_notw) )  , vaddition(vcomplex_mulq_n(a[2],(C1_5_notw) )  , vaddition(vcomplex_mulq_n(a[4],(C2_5_notw) )  , vaddition(vcomplex_mulq_n(a[1],(C3_5_notw) )  , a[0]) ) ) ) ;
res[4] = tmp_4;


vmemory<float32_t,inputstrideenabled>::store(in, 0 , res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 1 , res[1], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 2 , res[2], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 3 , res[3], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 4 , res[4], strideFactor);


};

/* Nb Multiplications = 16, 
Nb Additions = 20,
Nb Negates = 0,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_last(const complex<float32_t> *in,complex<float32_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4x2_t a[5];
f32x4x2_t res[5];


 f32x4x2_t tmp_0;
 f32x4x2_t tmp_1;
 f32x4x2_t tmp_2;
 f32x4x2_t tmp_3;
 f32x4x2_t tmp_4;


a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 , strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 , strideFactor);
a[2] = vmemory<float32_t,inputstrideenabled>::load(in , 2 , strideFactor);
a[3] = vmemory<float32_t,inputstrideenabled>::load(in , 3 , strideFactor);
a[4] = vmemory<float32_t,inputstrideenabled>::load(in , 4 , strideFactor);


tmp_0 = vaddition(a[0] , vaddition(a[1] , vaddition(a[2] , vaddition(a[3] , a[4]) ) ) ) ;
res[0] = tmp_0;
tmp_1 = vaddition(vcomplex_mulq_n(a[2],(C0_5_notw) )  , vaddition(vcomplex_mulq_n(a[3],(C1_5_notw) )  , vaddition(vcomplex_mulq_n(a[1],(C2_5_notw) )  , vaddition(vcomplex_mulq_n(a[4],(C3_5_notw) )  , a[0]) ) ) ) ;
res[1] = tmp_1;
tmp_2 = vaddition(vcomplex_mulq_n(a[1],(C0_5_notw) )  , vaddition(vcomplex_mulq_n(a[4],(C1_5_notw) )  , vaddition(vcomplex_mulq_n(a[3],(C2_5_notw) )  , vaddition(vcomplex_mulq_n(a[2],(C3_5_notw) )  , a[0]) ) ) ) ;
res[2] = tmp_2;
tmp_3 = vaddition(vcomplex_mulq_n(a[4],(C0_5_notw) )  , vaddition(vcomplex_mulq_n(a[1],(C1_5_notw) )  , vaddition(vcomplex_mulq_n(a[2],(C2_5_notw) )  , vaddition(vcomplex_mulq_n(a[3],(C3_5_notw) )  , a[0]) ) ) ) ;
res[3] = tmp_3;
tmp_4 = vaddition(vcomplex_mulq_n(a[3],(C0_5_notw) )  , vaddition(vcomplex_mulq_n(a[2],(C1_5_notw) )  , vaddition(vcomplex_mulq_n(a[4],(C2_5_notw) )  , vaddition(vcomplex_mulq_n(a[1],(C3_5_notw) )  , a[0]) ) ) ) ;
res[4] = tmp_4;


vmemory<float32_t,inputstrideenabled>::store(out, 0, res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 1, res[1], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 2, res[2], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 3, res[3], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 4, res[4], strideFactor);


};

};

/**************

INVERSE FFT 

***************/
template<int inputstrideenabled> 
struct radixVector<INVERSE,float32_t, 5,inputstrideenabled>{

static constexpr complex<float32_t> C0_5_tw{-0.8090169943749475f, -0.5877852522924731f};
  static constexpr complex<float32_t> C1_5_tw{-0.8090169943749475f, 0.5877852522924731f};
  static constexpr complex<float32_t> C2_5_tw{0.30901699437494745f, -0.9510565162951535f};
  static constexpr complex<float32_t> C3_5_tw{0.30901699437494745f, 0.9510565162951535f};
  
static constexpr complex<float32_t> C0_5_notw{-0.8090169943749475f, -0.5877852522924731f};
  static constexpr complex<float32_t> C1_5_notw{-0.8090169943749475f, 0.5877852522924731f};
  static constexpr complex<float32_t> C2_5_notw{0.30901699437494745f, -0.9510565162951535f};
  static constexpr complex<float32_t> C3_5_notw{0.30901699437494745f, 0.9510565162951535f};
  

/* Nb Multiplications = 16, 
Nb Additions = 20,
Nb Negates = 0,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_notw(int stageLength,complex<float32_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4x2_t a[5];
f32x4x2_t res[5];


 f32x4x2_t tmp_0;
 f32x4x2_t tmp_1;
 f32x4x2_t tmp_2;
 f32x4x2_t tmp_3;
 f32x4x2_t tmp_4;


a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 * stageLength, strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 * stageLength, strideFactor);
a[2] = vmemory<float32_t,inputstrideenabled>::load(in , 2 * stageLength, strideFactor);
a[3] = vmemory<float32_t,inputstrideenabled>::load(in , 3 * stageLength, strideFactor);
a[4] = vmemory<float32_t,inputstrideenabled>::load(in , 4 * stageLength, strideFactor);


tmp_0 = vaddition(a[0] , vaddition(a[1] , vaddition(a[2] , vaddition(a[3] , a[4]) ) ) ) ;
res[0] = tmp_0;
tmp_1 = vaddition(vcomplex_mulq_n(a[3],(C0_5_notw) )  , vaddition(vcomplex_mulq_n(a[2],(C1_5_notw) )  , vaddition(vcomplex_mulq_n(a[4],(C2_5_notw) )  , vaddition(vcomplex_mulq_n(a[1],(C3_5_notw) )  , a[0]) ) ) ) ;
res[1] = tmp_1;
tmp_2 = vaddition(vcomplex_mulq_n(a[4],(C0_5_notw) )  , vaddition(vcomplex_mulq_n(a[1],(C1_5_notw) )  , vaddition(vcomplex_mulq_n(a[2],(C2_5_notw) )  , vaddition(vcomplex_mulq_n(a[3],(C3_5_notw) )  , a[0]) ) ) ) ;
res[2] = tmp_2;
tmp_3 = vaddition(vcomplex_mulq_n(a[1],(C0_5_notw) )  , vaddition(vcomplex_mulq_n(a[4],(C1_5_notw) )  , vaddition(vcomplex_mulq_n(a[3],(C2_5_notw) )  , vaddition(vcomplex_mulq_n(a[2],(C3_5_notw) )  , a[0]) ) ) ) ;
res[3] = tmp_3;
tmp_4 = vaddition(vcomplex_mulq_n(a[2],(C0_5_notw) )  , vaddition(vcomplex_mulq_n(a[3],(C1_5_notw) )  , vaddition(vcomplex_mulq_n(a[1],(C2_5_notw) )  , vaddition(vcomplex_mulq_n(a[4],(C3_5_notw) )  , a[0]) ) ) ) ;
res[4] = tmp_4;


vmemory<float32_t,inputstrideenabled>::store(in, 0 * stageLength, res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 1 * stageLength, res[1], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 2 * stageLength, res[2], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 3 * stageLength, res[3], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 4 * stageLength, res[4], strideFactor);


};

/* Nb Multiplications = 20, 
Nb Additions = 20,
Nb Negates = 0,
Nb Conjugates = 4,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_tw(int stageLength,const f32x4x2_t *w,complex<float32_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4x2_t a[5];
f32x4x2_t res[5];


 f32x4x2_t tmp_0;
 f32x4x2_t tmp_1;
 f32x4x2_t tmp_2;
 f32x4x2_t tmp_3;
 f32x4x2_t tmp_4;


a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 * stageLength, strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 * stageLength, strideFactor);
a[2] = vmemory<float32_t,inputstrideenabled>::load(in , 2 * stageLength, strideFactor);
a[3] = vmemory<float32_t,inputstrideenabled>::load(in , 3 * stageLength, strideFactor);
a[4] = vmemory<float32_t,inputstrideenabled>::load(in , 4 * stageLength, strideFactor);


tmp_0 = vaddition(a[0] , vaddition(a[1] , vaddition(a[2] , vaddition(a[3] , a[4]) ) ) ) ;
res[0] = tmp_0;
tmp_1 = vaddition(vcomplex_mulq_n(a[3],(C0_5_tw) )  , vaddition(vcomplex_mulq_n(a[2],(C1_5_tw) )  , vaddition(vcomplex_mulq_n(a[4],(C2_5_tw) )  , vaddition(vcomplex_mulq_n(a[1],(C3_5_tw) )  , a[0]) ) ) ) ;
res[1] = vcomplex_mulq( vconjugate(w[0])   , tmp_1) ;
tmp_2 = vaddition(vcomplex_mulq_n(a[4],(C0_5_tw) )  , vaddition(vcomplex_mulq_n(a[1],(C1_5_tw) )  , vaddition(vcomplex_mulq_n(a[2],(C2_5_tw) )  , vaddition(vcomplex_mulq_n(a[3],(C3_5_tw) )  , a[0]) ) ) ) ;
res[2] = vcomplex_mulq( vconjugate(w[1])   , tmp_2) ;
tmp_3 = vaddition(vcomplex_mulq_n(a[1],(C0_5_tw) )  , vaddition(vcomplex_mulq_n(a[4],(C1_5_tw) )  , vaddition(vcomplex_mulq_n(a[3],(C2_5_tw) )  , vaddition(vcomplex_mulq_n(a[2],(C3_5_tw) )  , a[0]) ) ) ) ;
res[3] = vcomplex_mulq( vconjugate(w[2])   , tmp_3) ;
tmp_4 = vaddition(vcomplex_mulq_n(a[2],(C0_5_tw) )  , vaddition(vcomplex_mulq_n(a[3],(C1_5_tw) )  , vaddition(vcomplex_mulq_n(a[1],(C2_5_tw) )  , vaddition(vcomplex_mulq_n(a[4],(C3_5_tw) )  , a[0]) ) ) ) ;
res[4] = vcomplex_mulq( vconjugate(w[3])   , tmp_4) ;


vmemory<float32_t,inputstrideenabled>::store(in, 0 * stageLength, res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 1 * stageLength, res[1], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 2 * stageLength, res[2], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 3 * stageLength, res[3], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 4 * stageLength, res[4], strideFactor);


};

/* Nb Multiplications = 16, 
Nb Additions = 20,
Nb Negates = 0,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_notw(int stageLength,const complex<float32_t> *in,complex<float32_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4x2_t a[5];
f32x4x2_t res[5];


 f32x4x2_t tmp_0;
 f32x4x2_t tmp_1;
 f32x4x2_t tmp_2;
 f32x4x2_t tmp_3;
 f32x4x2_t tmp_4;


a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 * stageLength, strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 * stageLength, strideFactor);
a[2] = vmemory<float32_t,inputstrideenabled>::load(in , 2 * stageLength, strideFactor);
a[3] = vmemory<float32_t,inputstrideenabled>::load(in , 3 * stageLength, strideFactor);
a[4] = vmemory<float32_t,inputstrideenabled>::load(in , 4 * stageLength, strideFactor);


tmp_0 = vaddition(a[0] , vaddition(a[1] , vaddition(a[2] , vaddition(a[3] , a[4]) ) ) ) ;
res[0] = tmp_0;
tmp_1 = vaddition(vcomplex_mulq_n(a[3],(C0_5_notw) )  , vaddition(vcomplex_mulq_n(a[2],(C1_5_notw) )  , vaddition(vcomplex_mulq_n(a[4],(C2_5_notw) )  , vaddition(vcomplex_mulq_n(a[1],(C3_5_notw) )  , a[0]) ) ) ) ;
res[1] = tmp_1;
tmp_2 = vaddition(vcomplex_mulq_n(a[4],(C0_5_notw) )  , vaddition(vcomplex_mulq_n(a[1],(C1_5_notw) )  , vaddition(vcomplex_mulq_n(a[2],(C2_5_notw) )  , vaddition(vcomplex_mulq_n(a[3],(C3_5_notw) )  , a[0]) ) ) ) ;
res[2] = tmp_2;
tmp_3 = vaddition(vcomplex_mulq_n(a[1],(C0_5_notw) )  , vaddition(vcomplex_mulq_n(a[4],(C1_5_notw) )  , vaddition(vcomplex_mulq_n(a[3],(C2_5_notw) )  , vaddition(vcomplex_mulq_n(a[2],(C3_5_notw) )  , a[0]) ) ) ) ;
res[3] = tmp_3;
tmp_4 = vaddition(vcomplex_mulq_n(a[2],(C0_5_notw) )  , vaddition(vcomplex_mulq_n(a[3],(C1_5_notw) )  , vaddition(vcomplex_mulq_n(a[1],(C2_5_notw) )  , vaddition(vcomplex_mulq_n(a[4],(C3_5_notw) )  , a[0]) ) ) ) ;
res[4] = tmp_4;


vmemory<float32_t,inputstrideenabled>::store(out, 0 * stageLength , res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 1 * stageLength , res[1], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 2 * stageLength , res[2], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 3 * stageLength , res[3], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 4 * stageLength , res[4], strideFactor);


};

/* Nb Multiplications = 20, 
Nb Additions = 20,
Nb Negates = 0,
Nb Conjugates = 4,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_tw(int stageLength,const f32x4x2_t *w,const complex<float32_t> *in,complex<float32_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4x2_t a[5];
f32x4x2_t res[5];


 f32x4x2_t tmp_0;
 f32x4x2_t tmp_1;
 f32x4x2_t tmp_2;
 f32x4x2_t tmp_3;
 f32x4x2_t tmp_4;


a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 * stageLength, strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 * stageLength, strideFactor);
a[2] = vmemory<float32_t,inputstrideenabled>::load(in , 2 * stageLength, strideFactor);
a[3] = vmemory<float32_t,inputstrideenabled>::load(in , 3 * stageLength, strideFactor);
a[4] = vmemory<float32_t,inputstrideenabled>::load(in , 4 * stageLength, strideFactor);


tmp_0 = vaddition(a[0] , vaddition(a[1] , vaddition(a[2] , vaddition(a[3] , a[4]) ) ) ) ;
res[0] = tmp_0;
tmp_1 = vaddition(vcomplex_mulq_n(a[3],(C0_5_tw) )  , vaddition(vcomplex_mulq_n(a[2],(C1_5_tw) )  , vaddition(vcomplex_mulq_n(a[4],(C2_5_tw) )  , vaddition(vcomplex_mulq_n(a[1],(C3_5_tw) )  , a[0]) ) ) ) ;
res[1] = vcomplex_mulq( vconjugate(w[0])   , tmp_1) ;
tmp_2 = vaddition(vcomplex_mulq_n(a[4],(C0_5_tw) )  , vaddition(vcomplex_mulq_n(a[1],(C1_5_tw) )  , vaddition(vcomplex_mulq_n(a[2],(C2_5_tw) )  , vaddition(vcomplex_mulq_n(a[3],(C3_5_tw) )  , a[0]) ) ) ) ;
res[2] = vcomplex_mulq( vconjugate(w[1])   , tmp_2) ;
tmp_3 = vaddition(vcomplex_mulq_n(a[1],(C0_5_tw) )  , vaddition(vcomplex_mulq_n(a[4],(C1_5_tw) )  , vaddition(vcomplex_mulq_n(a[3],(C2_5_tw) )  , vaddition(vcomplex_mulq_n(a[2],(C3_5_tw) )  , a[0]) ) ) ) ;
res[3] = vcomplex_mulq( vconjugate(w[2])   , tmp_3) ;
tmp_4 = vaddition(vcomplex_mulq_n(a[2],(C0_5_tw) )  , vaddition(vcomplex_mulq_n(a[3],(C1_5_tw) )  , vaddition(vcomplex_mulq_n(a[1],(C2_5_tw) )  , vaddition(vcomplex_mulq_n(a[4],(C3_5_tw) )  , a[0]) ) ) ) ;
res[4] = vcomplex_mulq( vconjugate(w[3])   , tmp_4) ;


vmemory<float32_t,inputstrideenabled>::store(out, 0 * stageLength , res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 1 * stageLength , res[1], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 2 * stageLength , res[2], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 3 * stageLength , res[3], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 4 * stageLength , res[4], strideFactor);


};

/* Nb Multiplications = 16, 
Nb Additions = 20,
Nb Negates = 0,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_last(complex<float32_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4x2_t a[5];
f32x4x2_t res[5];


 f32x4x2_t tmp_0;
 f32x4x2_t tmp_1;
 f32x4x2_t tmp_2;
 f32x4x2_t tmp_3;
 f32x4x2_t tmp_4;


a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 , strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 , strideFactor);
a[2] = vmemory<float32_t,inputstrideenabled>::load(in , 2 , strideFactor);
a[3] = vmemory<float32_t,inputstrideenabled>::load(in , 3 , strideFactor);
a[4] = vmemory<float32_t,inputstrideenabled>::load(in , 4 , strideFactor);


tmp_0 = vaddition(a[0] , vaddition(a[1] , vaddition(a[2] , vaddition(a[3] , a[4]) ) ) ) ;
res[0] = tmp_0;
tmp_1 = vaddition(vcomplex_mulq_n(a[3],(C0_5_notw) )  , vaddition(vcomplex_mulq_n(a[2],(C1_5_notw) )  , vaddition(vcomplex_mulq_n(a[4],(C2_5_notw) )  , vaddition(vcomplex_mulq_n(a[1],(C3_5_notw) )  , a[0]) ) ) ) ;
res[1] = tmp_1;
tmp_2 = vaddition(vcomplex_mulq_n(a[4],(C0_5_notw) )  , vaddition(vcomplex_mulq_n(a[1],(C1_5_notw) )  , vaddition(vcomplex_mulq_n(a[2],(C2_5_notw) )  , vaddition(vcomplex_mulq_n(a[3],(C3_5_notw) )  , a[0]) ) ) ) ;
res[2] = tmp_2;
tmp_3 = vaddition(vcomplex_mulq_n(a[1],(C0_5_notw) )  , vaddition(vcomplex_mulq_n(a[4],(C1_5_notw) )  , vaddition(vcomplex_mulq_n(a[3],(C2_5_notw) )  , vaddition(vcomplex_mulq_n(a[2],(C3_5_notw) )  , a[0]) ) ) ) ;
res[3] = tmp_3;
tmp_4 = vaddition(vcomplex_mulq_n(a[2],(C0_5_notw) )  , vaddition(vcomplex_mulq_n(a[3],(C1_5_notw) )  , vaddition(vcomplex_mulq_n(a[1],(C2_5_notw) )  , vaddition(vcomplex_mulq_n(a[4],(C3_5_notw) )  , a[0]) ) ) ) ;
res[4] = tmp_4;


vmemory<float32_t,inputstrideenabled>::store(in, 0 , res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 1 , res[1], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 2 , res[2], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 3 , res[3], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(in, 4 , res[4], strideFactor);


};

/* Nb Multiplications = 16, 
Nb Additions = 20,
Nb Negates = 0,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_last(const complex<float32_t> *in,complex<float32_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f32x4x2_t a[5];
f32x4x2_t res[5];


 f32x4x2_t tmp_0;
 f32x4x2_t tmp_1;
 f32x4x2_t tmp_2;
 f32x4x2_t tmp_3;
 f32x4x2_t tmp_4;


a[0] = vmemory<float32_t,inputstrideenabled>::load(in , 0 , strideFactor);
a[1] = vmemory<float32_t,inputstrideenabled>::load(in , 1 , strideFactor);
a[2] = vmemory<float32_t,inputstrideenabled>::load(in , 2 , strideFactor);
a[3] = vmemory<float32_t,inputstrideenabled>::load(in , 3 , strideFactor);
a[4] = vmemory<float32_t,inputstrideenabled>::load(in , 4 , strideFactor);


tmp_0 = vaddition(a[0] , vaddition(a[1] , vaddition(a[2] , vaddition(a[3] , a[4]) ) ) ) ;
res[0] = tmp_0;
tmp_1 = vaddition(vcomplex_mulq_n(a[3],(C0_5_notw) )  , vaddition(vcomplex_mulq_n(a[2],(C1_5_notw) )  , vaddition(vcomplex_mulq_n(a[4],(C2_5_notw) )  , vaddition(vcomplex_mulq_n(a[1],(C3_5_notw) )  , a[0]) ) ) ) ;
res[1] = tmp_1;
tmp_2 = vaddition(vcomplex_mulq_n(a[4],(C0_5_notw) )  , vaddition(vcomplex_mulq_n(a[1],(C1_5_notw) )  , vaddition(vcomplex_mulq_n(a[2],(C2_5_notw) )  , vaddition(vcomplex_mulq_n(a[3],(C3_5_notw) )  , a[0]) ) ) ) ;
res[2] = tmp_2;
tmp_3 = vaddition(vcomplex_mulq_n(a[1],(C0_5_notw) )  , vaddition(vcomplex_mulq_n(a[4],(C1_5_notw) )  , vaddition(vcomplex_mulq_n(a[3],(C2_5_notw) )  , vaddition(vcomplex_mulq_n(a[2],(C3_5_notw) )  , a[0]) ) ) ) ;
res[3] = tmp_3;
tmp_4 = vaddition(vcomplex_mulq_n(a[2],(C0_5_notw) )  , vaddition(vcomplex_mulq_n(a[3],(C1_5_notw) )  , vaddition(vcomplex_mulq_n(a[1],(C2_5_notw) )  , vaddition(vcomplex_mulq_n(a[4],(C3_5_notw) )  , a[0]) ) ) ) ;
res[4] = tmp_4;


vmemory<float32_t,inputstrideenabled>::store(out, 0, res[0], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 1, res[1], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 2, res[2], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 3, res[3], strideFactor);
vmemory<float32_t,inputstrideenabled>::store(out, 4, res[4], strideFactor);


};

};

#endif /* #ifndef _RADIX_VECTOR_F32_5_H */