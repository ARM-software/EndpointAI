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

#ifndef _RADIX_VECTOR_F16_4_H
#define _RADIX_VECTOR_F16_4_H

#include "complex.h"

/**************

DIRECT FFT 

***************/
template<int inputstrideenabled> 
struct radixVector<DIRECT,float16_t, 4,inputstrideenabled>{

static constexpr complex<float16_t> C0_4_tw{0.0f16, 1.0f16};
  
static constexpr complex<float16_t> C0_4_notw{0.0f16, 1.0f16};
  

/* Nb Multiplications = 1, 
Nb Additions = 8,
Nb Negates = 4,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_notw(int stageLength,complex<float16_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f16x8_t a[4];
f16x8_t res[4];


 f16x8_t tmp_0;
 f16x8_t tmp_1;
 f16x8_t tmp_2;
 f16x8_t tmp_3;
 f16x8_t tmp_4;


a[0] = vmemory<float16_t,inputstrideenabled>::load(in , 0 * stageLength, strideFactor);
a[1] = vmemory<float16_t,inputstrideenabled>::load(in , 1 * stageLength, strideFactor);
a[2] = vmemory<float16_t,inputstrideenabled>::load(in , 2 * stageLength, strideFactor);
a[3] = vmemory<float16_t,inputstrideenabled>::load(in , 3 * stageLength, strideFactor);


tmp_0 = vaddq(a[0] , a[2]) ;
tmp_1 = vaddq(a[1] , a[3]) ;
res[0] = vaddq(tmp_0 , tmp_1) ;
tmp_2 = vsubq(a[0] , a[2]) ;
tmp_3 = vsubq(a[1] , a[3]) ;
tmp_4 =  vrot(tmp_3)  ;
res[1] = vsubq(tmp_2 , tmp_4) ;
res[2] = vsubq(tmp_0 , tmp_1) ;
res[3] = vaddq(tmp_2 , tmp_4) ;


vmemory<float16_t,inputstrideenabled>::store(in, 0 * stageLength, res[0], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(in, 1 * stageLength, res[1], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(in, 2 * stageLength, res[2], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(in, 3 * stageLength, res[3], strideFactor);


};

/* Nb Multiplications = 4, 
Nb Additions = 8,
Nb Negates = 4,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_tw(int stageLength,const f16x8_t *w,complex<float16_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f16x8_t a[4];
f16x8_t res[4];


 f16x8_t tmp_0;
 f16x8_t tmp_1;
 f16x8_t tmp_2;
 f16x8_t tmp_3;
 f16x8_t tmp_4;


a[0] = vmemory<float16_t,inputstrideenabled>::load(in , 0 * stageLength, strideFactor);
a[1] = vmemory<float16_t,inputstrideenabled>::load(in , 1 * stageLength, strideFactor);
a[2] = vmemory<float16_t,inputstrideenabled>::load(in , 2 * stageLength, strideFactor);
a[3] = vmemory<float16_t,inputstrideenabled>::load(in , 3 * stageLength, strideFactor);


tmp_0 = vaddq(a[0] , a[2]) ;
tmp_1 = vaddq(a[1] , a[3]) ;
res[0] = vaddq(tmp_0 , tmp_1) ;
tmp_2 = vsubq(a[0] , a[2]) ;
tmp_3 = vsubq(a[1] , a[3]) ;
tmp_4 =  vrot(tmp_3)  ;
res[1] = vcomplex_mulq(w[0] , vsubq(tmp_2 , tmp_4) ) ;
res[2] = vcomplex_mulq(w[1] , vsubq(tmp_0 , tmp_1) ) ;
res[3] = vcomplex_mulq(w[2] , vaddq(tmp_2 , tmp_4) ) ;


vmemory<float16_t,inputstrideenabled>::store(in, 0 * stageLength, res[0], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(in, 1 * stageLength, res[1], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(in, 2 * stageLength, res[2], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(in, 3 * stageLength, res[3], strideFactor);


};

/* Nb Multiplications = 1, 
Nb Additions = 8,
Nb Negates = 4,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_notw(int stageLength,const complex<float16_t> *in,complex<float16_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f16x8_t a[4];
f16x8_t res[4];


 f16x8_t tmp_0;
 f16x8_t tmp_1;
 f16x8_t tmp_2;
 f16x8_t tmp_3;
 f16x8_t tmp_4;


a[0] = vmemory<float16_t,inputstrideenabled>::load(in , 0 * stageLength, strideFactor);
a[1] = vmemory<float16_t,inputstrideenabled>::load(in , 1 * stageLength, strideFactor);
a[2] = vmemory<float16_t,inputstrideenabled>::load(in , 2 * stageLength, strideFactor);
a[3] = vmemory<float16_t,inputstrideenabled>::load(in , 3 * stageLength, strideFactor);


tmp_0 = vaddq(a[0] , a[2]) ;
tmp_1 = vaddq(a[1] , a[3]) ;
res[0] = vaddq(tmp_0 , tmp_1) ;
tmp_2 = vsubq(a[0] , a[2]) ;
tmp_3 = vsubq(a[1] , a[3]) ;
tmp_4 =  vrot(tmp_3)  ;
res[1] = vsubq(tmp_2 , tmp_4) ;
res[2] = vsubq(tmp_0 , tmp_1) ;
res[3] = vaddq(tmp_2 , tmp_4) ;


vmemory<float16_t,inputstrideenabled>::store(out, 0 * stageLength , res[0], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(out, 1 * stageLength , res[1], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(out, 2 * stageLength , res[2], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(out, 3 * stageLength , res[3], strideFactor);


};

/* Nb Multiplications = 4, 
Nb Additions = 8,
Nb Negates = 4,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_tw(int stageLength,const f16x8_t *w,const complex<float16_t> *in,complex<float16_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f16x8_t a[4];
f16x8_t res[4];


 f16x8_t tmp_0;
 f16x8_t tmp_1;
 f16x8_t tmp_2;
 f16x8_t tmp_3;
 f16x8_t tmp_4;


a[0] = vmemory<float16_t,inputstrideenabled>::load(in , 0 * stageLength, strideFactor);
a[1] = vmemory<float16_t,inputstrideenabled>::load(in , 1 * stageLength, strideFactor);
a[2] = vmemory<float16_t,inputstrideenabled>::load(in , 2 * stageLength, strideFactor);
a[3] = vmemory<float16_t,inputstrideenabled>::load(in , 3 * stageLength, strideFactor);


tmp_0 = vaddq(a[0] , a[2]) ;
tmp_1 = vaddq(a[1] , a[3]) ;
res[0] = vaddq(tmp_0 , tmp_1) ;
tmp_2 = vsubq(a[0] , a[2]) ;
tmp_3 = vsubq(a[1] , a[3]) ;
tmp_4 =  vrot(tmp_3)  ;
res[1] = vcomplex_mulq(w[0] , vsubq(tmp_2 , tmp_4) ) ;
res[2] = vcomplex_mulq(w[1] , vsubq(tmp_0 , tmp_1) ) ;
res[3] = vcomplex_mulq(w[2] , vaddq(tmp_2 , tmp_4) ) ;


vmemory<float16_t,inputstrideenabled>::store(out, 0 * stageLength , res[0], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(out, 1 * stageLength , res[1], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(out, 2 * stageLength , res[2], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(out, 3 * stageLength , res[3], strideFactor);


};

/* Nb Multiplications = 1, 
Nb Additions = 8,
Nb Negates = 4,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_last(complex<float16_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f16x8_t a[4];
f16x8_t res[4];


 f16x8_t tmp_0;
 f16x8_t tmp_1;
 f16x8_t tmp_2;
 f16x8_t tmp_3;
 f16x8_t tmp_4;


a[0] = vmemory<float16_t,inputstrideenabled>::load(in , 0 , strideFactor);
a[1] = vmemory<float16_t,inputstrideenabled>::load(in , 1 , strideFactor);
a[2] = vmemory<float16_t,inputstrideenabled>::load(in , 2 , strideFactor);
a[3] = vmemory<float16_t,inputstrideenabled>::load(in , 3 , strideFactor);


tmp_0 = vaddq(a[0] , a[2]) ;
tmp_1 = vaddq(a[1] , a[3]) ;
res[0] = vaddq(tmp_0 , tmp_1) ;
tmp_2 = vsubq(a[0] , a[2]) ;
tmp_3 = vsubq(a[1] , a[3]) ;
tmp_4 =  vrot(tmp_3)  ;
res[1] = vsubq(tmp_2 , tmp_4) ;
res[2] = vsubq(tmp_0 , tmp_1) ;
res[3] = vaddq(tmp_2 , tmp_4) ;


vmemory<float16_t,inputstrideenabled>::store(in, 0 , res[0], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(in, 1 , res[1], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(in, 2 , res[2], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(in, 3 , res[3], strideFactor);


};

/* Nb Multiplications = 1, 
Nb Additions = 8,
Nb Negates = 4,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_last(const complex<float16_t> *in,complex<float16_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f16x8_t a[4];
f16x8_t res[4];


 f16x8_t tmp_0;
 f16x8_t tmp_1;
 f16x8_t tmp_2;
 f16x8_t tmp_3;
 f16x8_t tmp_4;


a[0] = vmemory<float16_t,inputstrideenabled>::load(in , 0 , strideFactor);
a[1] = vmemory<float16_t,inputstrideenabled>::load(in , 1 , strideFactor);
a[2] = vmemory<float16_t,inputstrideenabled>::load(in , 2 , strideFactor);
a[3] = vmemory<float16_t,inputstrideenabled>::load(in , 3 , strideFactor);


tmp_0 = vaddq(a[0] , a[2]) ;
tmp_1 = vaddq(a[1] , a[3]) ;
res[0] = vaddq(tmp_0 , tmp_1) ;
tmp_2 = vsubq(a[0] , a[2]) ;
tmp_3 = vsubq(a[1] , a[3]) ;
tmp_4 =  vrot(tmp_3)  ;
res[1] = vsubq(tmp_2 , tmp_4) ;
res[2] = vsubq(tmp_0 , tmp_1) ;
res[3] = vaddq(tmp_2 , tmp_4) ;


vmemory<float16_t,inputstrideenabled>::store(out, 0, res[0], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(out, 1, res[1], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(out, 2, res[2], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(out, 3, res[3], strideFactor);


};

};

/**************

INVERSE FFT 

***************/
template<int inputstrideenabled> 
struct radixVector<INVERSE,float16_t, 4,inputstrideenabled>{

static constexpr complex<float16_t> C0_4_tw{0.0f16, 1.0f16};
  
static constexpr complex<float16_t> C0_4_notw{0.0f16, 1.0f16};
  

/* Nb Multiplications = 1, 
Nb Additions = 8,
Nb Negates = 4,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_notw(int stageLength,complex<float16_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f16x8_t a[4];
f16x8_t res[4];


 f16x8_t tmp_0;
 f16x8_t tmp_1;
 f16x8_t tmp_2;
 f16x8_t tmp_3;
 f16x8_t tmp_4;


a[0] = vmemory<float16_t,inputstrideenabled>::load(in , 0 * stageLength, strideFactor);
a[1] = vmemory<float16_t,inputstrideenabled>::load(in , 1 * stageLength, strideFactor);
a[2] = vmemory<float16_t,inputstrideenabled>::load(in , 2 * stageLength, strideFactor);
a[3] = vmemory<float16_t,inputstrideenabled>::load(in , 3 * stageLength, strideFactor);


tmp_0 = vaddq(a[0] , a[2]) ;
tmp_1 = vaddq(a[1] , a[3]) ;
res[0] = vaddq(tmp_0 , tmp_1) ;
tmp_2 = vsubq(a[0] , a[2]) ;
tmp_3 = vsubq(a[1] , a[3]) ;
tmp_4 =  vrot(tmp_3)  ;
res[1] = vaddq(tmp_2 , tmp_4) ;
res[2] = vsubq(tmp_0 , tmp_1) ;
res[3] = vsubq(tmp_2 , tmp_4) ;


vmemory<float16_t,inputstrideenabled>::store(in, 0 * stageLength, res[0], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(in, 1 * stageLength, res[1], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(in, 2 * stageLength, res[2], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(in, 3 * stageLength, res[3], strideFactor);


};

/* Nb Multiplications = 4, 
Nb Additions = 8,
Nb Negates = 4,
Nb Conjugates = 3,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_tw(int stageLength,const f16x8_t *w,complex<float16_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f16x8_t a[4];
f16x8_t res[4];


 f16x8_t tmp_0;
 f16x8_t tmp_1;
 f16x8_t tmp_2;
 f16x8_t tmp_3;
 f16x8_t tmp_4;


a[0] = vmemory<float16_t,inputstrideenabled>::load(in , 0 * stageLength, strideFactor);
a[1] = vmemory<float16_t,inputstrideenabled>::load(in , 1 * stageLength, strideFactor);
a[2] = vmemory<float16_t,inputstrideenabled>::load(in , 2 * stageLength, strideFactor);
a[3] = vmemory<float16_t,inputstrideenabled>::load(in , 3 * stageLength, strideFactor);


tmp_0 = vaddq(a[0] , a[2]) ;
tmp_1 = vaddq(a[1] , a[3]) ;
res[0] = vaddq(tmp_0 , tmp_1) ;
tmp_2 = vsubq(a[0] , a[2]) ;
tmp_3 = vsubq(a[1] , a[3]) ;
tmp_4 =  vrot(tmp_3)  ;
res[1] = vcomplex_mulq( vconjugate(w[0])   , vaddq(tmp_2 , tmp_4) ) ;
res[2] = vcomplex_mulq( vconjugate(w[1])   , vsubq(tmp_0 , tmp_1) ) ;
res[3] = vcomplex_mulq( vconjugate(w[2])   , vsubq(tmp_2 , tmp_4) ) ;


vmemory<float16_t,inputstrideenabled>::store(in, 0 * stageLength, res[0], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(in, 1 * stageLength, res[1], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(in, 2 * stageLength, res[2], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(in, 3 * stageLength, res[3], strideFactor);


};

/* Nb Multiplications = 1, 
Nb Additions = 8,
Nb Negates = 4,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_notw(int stageLength,const complex<float16_t> *in,complex<float16_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f16x8_t a[4];
f16x8_t res[4];


 f16x8_t tmp_0;
 f16x8_t tmp_1;
 f16x8_t tmp_2;
 f16x8_t tmp_3;
 f16x8_t tmp_4;


a[0] = vmemory<float16_t,inputstrideenabled>::load(in , 0 * stageLength, strideFactor);
a[1] = vmemory<float16_t,inputstrideenabled>::load(in , 1 * stageLength, strideFactor);
a[2] = vmemory<float16_t,inputstrideenabled>::load(in , 2 * stageLength, strideFactor);
a[3] = vmemory<float16_t,inputstrideenabled>::load(in , 3 * stageLength, strideFactor);


tmp_0 = vaddq(a[0] , a[2]) ;
tmp_1 = vaddq(a[1] , a[3]) ;
res[0] = vaddq(tmp_0 , tmp_1) ;
tmp_2 = vsubq(a[0] , a[2]) ;
tmp_3 = vsubq(a[1] , a[3]) ;
tmp_4 =  vrot(tmp_3)  ;
res[1] = vaddq(tmp_2 , tmp_4) ;
res[2] = vsubq(tmp_0 , tmp_1) ;
res[3] = vsubq(tmp_2 , tmp_4) ;


vmemory<float16_t,inputstrideenabled>::store(out, 0 * stageLength , res[0], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(out, 1 * stageLength , res[1], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(out, 2 * stageLength , res[2], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(out, 3 * stageLength , res[3], strideFactor);


};

/* Nb Multiplications = 4, 
Nb Additions = 8,
Nb Negates = 4,
Nb Conjugates = 3,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_tw(int stageLength,const f16x8_t *w,const complex<float16_t> *in,complex<float16_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f16x8_t a[4];
f16x8_t res[4];


 f16x8_t tmp_0;
 f16x8_t tmp_1;
 f16x8_t tmp_2;
 f16x8_t tmp_3;
 f16x8_t tmp_4;


a[0] = vmemory<float16_t,inputstrideenabled>::load(in , 0 * stageLength, strideFactor);
a[1] = vmemory<float16_t,inputstrideenabled>::load(in , 1 * stageLength, strideFactor);
a[2] = vmemory<float16_t,inputstrideenabled>::load(in , 2 * stageLength, strideFactor);
a[3] = vmemory<float16_t,inputstrideenabled>::load(in , 3 * stageLength, strideFactor);


tmp_0 = vaddq(a[0] , a[2]) ;
tmp_1 = vaddq(a[1] , a[3]) ;
res[0] = vaddq(tmp_0 , tmp_1) ;
tmp_2 = vsubq(a[0] , a[2]) ;
tmp_3 = vsubq(a[1] , a[3]) ;
tmp_4 =  vrot(tmp_3)  ;
res[1] = vcomplex_mulq( vconjugate(w[0])   , vaddq(tmp_2 , tmp_4) ) ;
res[2] = vcomplex_mulq( vconjugate(w[1])   , vsubq(tmp_0 , tmp_1) ) ;
res[3] = vcomplex_mulq( vconjugate(w[2])   , vsubq(tmp_2 , tmp_4) ) ;


vmemory<float16_t,inputstrideenabled>::store(out, 0 * stageLength , res[0], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(out, 1 * stageLength , res[1], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(out, 2 * stageLength , res[2], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(out, 3 * stageLength , res[3], strideFactor);


};

/* Nb Multiplications = 1, 
Nb Additions = 8,
Nb Negates = 4,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_last(complex<float16_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f16x8_t a[4];
f16x8_t res[4];


 f16x8_t tmp_0;
 f16x8_t tmp_1;
 f16x8_t tmp_2;
 f16x8_t tmp_3;
 f16x8_t tmp_4;


a[0] = vmemory<float16_t,inputstrideenabled>::load(in , 0 , strideFactor);
a[1] = vmemory<float16_t,inputstrideenabled>::load(in , 1 , strideFactor);
a[2] = vmemory<float16_t,inputstrideenabled>::load(in , 2 , strideFactor);
a[3] = vmemory<float16_t,inputstrideenabled>::load(in , 3 , strideFactor);


tmp_0 = vaddq(a[0] , a[2]) ;
tmp_1 = vaddq(a[1] , a[3]) ;
res[0] = vaddq(tmp_0 , tmp_1) ;
tmp_2 = vsubq(a[0] , a[2]) ;
tmp_3 = vsubq(a[1] , a[3]) ;
tmp_4 =  vrot(tmp_3)  ;
res[1] = vaddq(tmp_2 , tmp_4) ;
res[2] = vsubq(tmp_0 , tmp_1) ;
res[3] = vsubq(tmp_2 , tmp_4) ;


vmemory<float16_t,inputstrideenabled>::store(in, 0 , res[0], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(in, 1 , res[1], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(in, 2 , res[2], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(in, 3 , res[3], strideFactor);


};

/* Nb Multiplications = 1, 
Nb Additions = 8,
Nb Negates = 4,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_last(const complex<float16_t> *in,complex<float16_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f16x8_t a[4];
f16x8_t res[4];


 f16x8_t tmp_0;
 f16x8_t tmp_1;
 f16x8_t tmp_2;
 f16x8_t tmp_3;
 f16x8_t tmp_4;


a[0] = vmemory<float16_t,inputstrideenabled>::load(in , 0 , strideFactor);
a[1] = vmemory<float16_t,inputstrideenabled>::load(in , 1 , strideFactor);
a[2] = vmemory<float16_t,inputstrideenabled>::load(in , 2 , strideFactor);
a[3] = vmemory<float16_t,inputstrideenabled>::load(in , 3 , strideFactor);


tmp_0 = vaddq(a[0] , a[2]) ;
tmp_1 = vaddq(a[1] , a[3]) ;
res[0] = vaddq(tmp_0 , tmp_1) ;
tmp_2 = vsubq(a[0] , a[2]) ;
tmp_3 = vsubq(a[1] , a[3]) ;
tmp_4 =  vrot(tmp_3)  ;
res[1] = vaddq(tmp_2 , tmp_4) ;
res[2] = vsubq(tmp_0 , tmp_1) ;
res[3] = vsubq(tmp_2 , tmp_4) ;


vmemory<float16_t,inputstrideenabled>::store(out, 0, res[0], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(out, 1, res[1], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(out, 2, res[2], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(out, 3, res[3], strideFactor);


};

};

#endif /* #ifndef _RADIX_VECTOR_F16_4_H */