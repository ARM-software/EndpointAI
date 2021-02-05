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

#ifndef _RADIX_VECTOR_F16_3_H
#define _RADIX_VECTOR_F16_3_H

#include "complex.h"

/**************

DIRECT FFT 

***************/
template<int inputstrideenabled> 
struct radixVector<DIRECT,float16_t, 3,inputstrideenabled>{

static constexpr complex<float16_t> C0_3_tw{-0.5f16, -0.8660254037844386f16};
  static constexpr complex<float16_t> C1_3_tw{-0.5f16, 0.8660254037844386f16};
  
static constexpr complex<float16_t> C0_3_notw{-0.5f16, -0.8660254037844386f16};
  static constexpr complex<float16_t> C1_3_notw{-0.5f16, 0.8660254037844386f16};
  

/* Nb Multiplications = 4, 
Nb Additions = 6,
Nb Negates = 0,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_notw(int stageLength,complex<float16_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f16x8_t a[3];
f16x8_t res[3];


 f16x8_t tmp_0;
 f16x8_t tmp_1;
 f16x8_t tmp_2;


a[0] = vmemory<float16_t,inputstrideenabled>::load(in , 0 * stageLength, strideFactor);
a[1] = vmemory<float16_t,inputstrideenabled>::load(in , 1 * stageLength, strideFactor);
a[2] = vmemory<float16_t,inputstrideenabled>::load(in , 2 * stageLength, strideFactor);


tmp_0 = vaddq(a[0] , vaddq(a[1] , a[2]) ) ;
res[0] = tmp_0;
tmp_1 = vaddq(vcomplex_mulq_n(a[1],(C0_3_notw) )  , vaddq(vcomplex_mulq_n(a[2],(C1_3_notw) )  , a[0]) ) ;
res[1] = tmp_1;
tmp_2 = vaddq(vcomplex_mulq_n(a[2],(C0_3_notw) )  , vaddq(vcomplex_mulq_n(a[1],(C1_3_notw) )  , a[0]) ) ;
res[2] = tmp_2;


vmemory<float16_t,inputstrideenabled>::store(in, 0 * stageLength, res[0], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(in, 1 * stageLength, res[1], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(in, 2 * stageLength, res[2], strideFactor);


};

/* Nb Multiplications = 6, 
Nb Additions = 6,
Nb Negates = 0,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_tw(int stageLength,const f16x8_t *w,complex<float16_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f16x8_t a[3];
f16x8_t res[3];


 f16x8_t tmp_0;
 f16x8_t tmp_1;
 f16x8_t tmp_2;


a[0] = vmemory<float16_t,inputstrideenabled>::load(in , 0 * stageLength, strideFactor);
a[1] = vmemory<float16_t,inputstrideenabled>::load(in , 1 * stageLength, strideFactor);
a[2] = vmemory<float16_t,inputstrideenabled>::load(in , 2 * stageLength, strideFactor);


tmp_0 = vaddq(a[0] , vaddq(a[1] , a[2]) ) ;
res[0] = tmp_0;
tmp_1 = vaddq(vcomplex_mulq_n(a[1],(C0_3_tw) )  , vaddq(vcomplex_mulq_n(a[2],(C1_3_tw) )  , a[0]) ) ;
res[1] = vcomplex_mulq(w[0] , tmp_1) ;
tmp_2 = vaddq(vcomplex_mulq_n(a[2],(C0_3_tw) )  , vaddq(vcomplex_mulq_n(a[1],(C1_3_tw) )  , a[0]) ) ;
res[2] = vcomplex_mulq(w[1] , tmp_2) ;


vmemory<float16_t,inputstrideenabled>::store(in, 0 * stageLength, res[0], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(in, 1 * stageLength, res[1], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(in, 2 * stageLength, res[2], strideFactor);


};

/* Nb Multiplications = 4, 
Nb Additions = 6,
Nb Negates = 0,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_notw(int stageLength,const complex<float16_t> *in,complex<float16_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f16x8_t a[3];
f16x8_t res[3];


 f16x8_t tmp_0;
 f16x8_t tmp_1;
 f16x8_t tmp_2;


a[0] = vmemory<float16_t,inputstrideenabled>::load(in , 0 * stageLength, strideFactor);
a[1] = vmemory<float16_t,inputstrideenabled>::load(in , 1 * stageLength, strideFactor);
a[2] = vmemory<float16_t,inputstrideenabled>::load(in , 2 * stageLength, strideFactor);


tmp_0 = vaddq(a[0] , vaddq(a[1] , a[2]) ) ;
res[0] = tmp_0;
tmp_1 = vaddq(vcomplex_mulq_n(a[1],(C0_3_notw) )  , vaddq(vcomplex_mulq_n(a[2],(C1_3_notw) )  , a[0]) ) ;
res[1] = tmp_1;
tmp_2 = vaddq(vcomplex_mulq_n(a[2],(C0_3_notw) )  , vaddq(vcomplex_mulq_n(a[1],(C1_3_notw) )  , a[0]) ) ;
res[2] = tmp_2;


vmemory<float16_t,inputstrideenabled>::store(out, 0 * stageLength , res[0], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(out, 1 * stageLength , res[1], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(out, 2 * stageLength , res[2], strideFactor);


};

/* Nb Multiplications = 6, 
Nb Additions = 6,
Nb Negates = 0,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_tw(int stageLength,const f16x8_t *w,const complex<float16_t> *in,complex<float16_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f16x8_t a[3];
f16x8_t res[3];


 f16x8_t tmp_0;
 f16x8_t tmp_1;
 f16x8_t tmp_2;


a[0] = vmemory<float16_t,inputstrideenabled>::load(in , 0 * stageLength, strideFactor);
a[1] = vmemory<float16_t,inputstrideenabled>::load(in , 1 * stageLength, strideFactor);
a[2] = vmemory<float16_t,inputstrideenabled>::load(in , 2 * stageLength, strideFactor);


tmp_0 = vaddq(a[0] , vaddq(a[1] , a[2]) ) ;
res[0] = tmp_0;
tmp_1 = vaddq(vcomplex_mulq_n(a[1],(C0_3_tw) )  , vaddq(vcomplex_mulq_n(a[2],(C1_3_tw) )  , a[0]) ) ;
res[1] = vcomplex_mulq(w[0] , tmp_1) ;
tmp_2 = vaddq(vcomplex_mulq_n(a[2],(C0_3_tw) )  , vaddq(vcomplex_mulq_n(a[1],(C1_3_tw) )  , a[0]) ) ;
res[2] = vcomplex_mulq(w[1] , tmp_2) ;


vmemory<float16_t,inputstrideenabled>::store(out, 0 * stageLength , res[0], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(out, 1 * stageLength , res[1], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(out, 2 * stageLength , res[2], strideFactor);


};

/* Nb Multiplications = 4, 
Nb Additions = 6,
Nb Negates = 0,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_last(complex<float16_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f16x8_t a[3];
f16x8_t res[3];


 f16x8_t tmp_0;
 f16x8_t tmp_1;
 f16x8_t tmp_2;


a[0] = vmemory<float16_t,inputstrideenabled>::load(in , 0 , strideFactor);
a[1] = vmemory<float16_t,inputstrideenabled>::load(in , 1 , strideFactor);
a[2] = vmemory<float16_t,inputstrideenabled>::load(in , 2 , strideFactor);


tmp_0 = vaddq(a[0] , vaddq(a[1] , a[2]) ) ;
res[0] = tmp_0;
tmp_1 = vaddq(vcomplex_mulq_n(a[1],(C0_3_notw) )  , vaddq(vcomplex_mulq_n(a[2],(C1_3_notw) )  , a[0]) ) ;
res[1] = tmp_1;
tmp_2 = vaddq(vcomplex_mulq_n(a[2],(C0_3_notw) )  , vaddq(vcomplex_mulq_n(a[1],(C1_3_notw) )  , a[0]) ) ;
res[2] = tmp_2;


vmemory<float16_t,inputstrideenabled>::store(in, 0 , res[0], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(in, 1 , res[1], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(in, 2 , res[2], strideFactor);


};

/* Nb Multiplications = 4, 
Nb Additions = 6,
Nb Negates = 0,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_last(const complex<float16_t> *in,complex<float16_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f16x8_t a[3];
f16x8_t res[3];


 f16x8_t tmp_0;
 f16x8_t tmp_1;
 f16x8_t tmp_2;


a[0] = vmemory<float16_t,inputstrideenabled>::load(in , 0 , strideFactor);
a[1] = vmemory<float16_t,inputstrideenabled>::load(in , 1 , strideFactor);
a[2] = vmemory<float16_t,inputstrideenabled>::load(in , 2 , strideFactor);


tmp_0 = vaddq(a[0] , vaddq(a[1] , a[2]) ) ;
res[0] = tmp_0;
tmp_1 = vaddq(vcomplex_mulq_n(a[1],(C0_3_notw) )  , vaddq(vcomplex_mulq_n(a[2],(C1_3_notw) )  , a[0]) ) ;
res[1] = tmp_1;
tmp_2 = vaddq(vcomplex_mulq_n(a[2],(C0_3_notw) )  , vaddq(vcomplex_mulq_n(a[1],(C1_3_notw) )  , a[0]) ) ;
res[2] = tmp_2;


vmemory<float16_t,inputstrideenabled>::store(out, 0, res[0], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(out, 1, res[1], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(out, 2, res[2], strideFactor);


};

};

/**************

INVERSE FFT 

***************/
template<int inputstrideenabled> 
struct radixVector<INVERSE,float16_t, 3,inputstrideenabled>{

static constexpr complex<float16_t> C0_3_tw{-0.5f16, -0.8660254037844386f16};
  static constexpr complex<float16_t> C1_3_tw{-0.5f16, 0.8660254037844386f16};
  
static constexpr complex<float16_t> C0_3_notw{-0.5f16, -0.8660254037844386f16};
  static constexpr complex<float16_t> C1_3_notw{-0.5f16, 0.8660254037844386f16};
  

/* Nb Multiplications = 4, 
Nb Additions = 6,
Nb Negates = 0,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_notw(int stageLength,complex<float16_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f16x8_t a[3];
f16x8_t res[3];


 f16x8_t tmp_0;
 f16x8_t tmp_1;
 f16x8_t tmp_2;


a[0] = vmemory<float16_t,inputstrideenabled>::load(in , 0 * stageLength, strideFactor);
a[1] = vmemory<float16_t,inputstrideenabled>::load(in , 1 * stageLength, strideFactor);
a[2] = vmemory<float16_t,inputstrideenabled>::load(in , 2 * stageLength, strideFactor);


tmp_0 = vaddq(a[0] , vaddq(a[1] , a[2]) ) ;
res[0] = tmp_0;
tmp_1 = vaddq(vcomplex_mulq_n(a[2],(C0_3_notw) )  , vaddq(vcomplex_mulq_n(a[1],(C1_3_notw) )  , a[0]) ) ;
res[1] = tmp_1;
tmp_2 = vaddq(vcomplex_mulq_n(a[1],(C0_3_notw) )  , vaddq(vcomplex_mulq_n(a[2],(C1_3_notw) )  , a[0]) ) ;
res[2] = tmp_2;


vmemory<float16_t,inputstrideenabled>::store(in, 0 * stageLength, res[0], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(in, 1 * stageLength, res[1], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(in, 2 * stageLength, res[2], strideFactor);


};

/* Nb Multiplications = 6, 
Nb Additions = 6,
Nb Negates = 0,
Nb Conjugates = 2,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_tw(int stageLength,const f16x8_t *w,complex<float16_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f16x8_t a[3];
f16x8_t res[3];


 f16x8_t tmp_0;
 f16x8_t tmp_1;
 f16x8_t tmp_2;


a[0] = vmemory<float16_t,inputstrideenabled>::load(in , 0 * stageLength, strideFactor);
a[1] = vmemory<float16_t,inputstrideenabled>::load(in , 1 * stageLength, strideFactor);
a[2] = vmemory<float16_t,inputstrideenabled>::load(in , 2 * stageLength, strideFactor);


tmp_0 = vaddq(a[0] , vaddq(a[1] , a[2]) ) ;
res[0] = tmp_0;
tmp_1 = vaddq(vcomplex_mulq_n(a[2],(C0_3_tw) )  , vaddq(vcomplex_mulq_n(a[1],(C1_3_tw) )  , a[0]) ) ;
res[1] = vcomplex_mulq( vconjugate(w[0])   , tmp_1) ;
tmp_2 = vaddq(vcomplex_mulq_n(a[1],(C0_3_tw) )  , vaddq(vcomplex_mulq_n(a[2],(C1_3_tw) )  , a[0]) ) ;
res[2] = vcomplex_mulq( vconjugate(w[1])   , tmp_2) ;


vmemory<float16_t,inputstrideenabled>::store(in, 0 * stageLength, res[0], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(in, 1 * stageLength, res[1], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(in, 2 * stageLength, res[2], strideFactor);


};

/* Nb Multiplications = 4, 
Nb Additions = 6,
Nb Negates = 0,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_notw(int stageLength,const complex<float16_t> *in,complex<float16_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f16x8_t a[3];
f16x8_t res[3];


 f16x8_t tmp_0;
 f16x8_t tmp_1;
 f16x8_t tmp_2;


a[0] = vmemory<float16_t,inputstrideenabled>::load(in , 0 * stageLength, strideFactor);
a[1] = vmemory<float16_t,inputstrideenabled>::load(in , 1 * stageLength, strideFactor);
a[2] = vmemory<float16_t,inputstrideenabled>::load(in , 2 * stageLength, strideFactor);


tmp_0 = vaddq(a[0] , vaddq(a[1] , a[2]) ) ;
res[0] = tmp_0;
tmp_1 = vaddq(vcomplex_mulq_n(a[2],(C0_3_notw) )  , vaddq(vcomplex_mulq_n(a[1],(C1_3_notw) )  , a[0]) ) ;
res[1] = tmp_1;
tmp_2 = vaddq(vcomplex_mulq_n(a[1],(C0_3_notw) )  , vaddq(vcomplex_mulq_n(a[2],(C1_3_notw) )  , a[0]) ) ;
res[2] = tmp_2;


vmemory<float16_t,inputstrideenabled>::store(out, 0 * stageLength , res[0], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(out, 1 * stageLength , res[1], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(out, 2 * stageLength , res[2], strideFactor);


};

/* Nb Multiplications = 6, 
Nb Additions = 6,
Nb Negates = 0,
Nb Conjugates = 2,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_tw(int stageLength,const f16x8_t *w,const complex<float16_t> *in,complex<float16_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f16x8_t a[3];
f16x8_t res[3];


 f16x8_t tmp_0;
 f16x8_t tmp_1;
 f16x8_t tmp_2;


a[0] = vmemory<float16_t,inputstrideenabled>::load(in , 0 * stageLength, strideFactor);
a[1] = vmemory<float16_t,inputstrideenabled>::load(in , 1 * stageLength, strideFactor);
a[2] = vmemory<float16_t,inputstrideenabled>::load(in , 2 * stageLength, strideFactor);


tmp_0 = vaddq(a[0] , vaddq(a[1] , a[2]) ) ;
res[0] = tmp_0;
tmp_1 = vaddq(vcomplex_mulq_n(a[2],(C0_3_tw) )  , vaddq(vcomplex_mulq_n(a[1],(C1_3_tw) )  , a[0]) ) ;
res[1] = vcomplex_mulq( vconjugate(w[0])   , tmp_1) ;
tmp_2 = vaddq(vcomplex_mulq_n(a[1],(C0_3_tw) )  , vaddq(vcomplex_mulq_n(a[2],(C1_3_tw) )  , a[0]) ) ;
res[2] = vcomplex_mulq( vconjugate(w[1])   , tmp_2) ;


vmemory<float16_t,inputstrideenabled>::store(out, 0 * stageLength , res[0], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(out, 1 * stageLength , res[1], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(out, 2 * stageLength , res[2], strideFactor);


};

/* Nb Multiplications = 4, 
Nb Additions = 6,
Nb Negates = 0,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_last(complex<float16_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f16x8_t a[3];
f16x8_t res[3];


 f16x8_t tmp_0;
 f16x8_t tmp_1;
 f16x8_t tmp_2;


a[0] = vmemory<float16_t,inputstrideenabled>::load(in , 0 , strideFactor);
a[1] = vmemory<float16_t,inputstrideenabled>::load(in , 1 , strideFactor);
a[2] = vmemory<float16_t,inputstrideenabled>::load(in , 2 , strideFactor);


tmp_0 = vaddq(a[0] , vaddq(a[1] , a[2]) ) ;
res[0] = tmp_0;
tmp_1 = vaddq(vcomplex_mulq_n(a[2],(C0_3_notw) )  , vaddq(vcomplex_mulq_n(a[1],(C1_3_notw) )  , a[0]) ) ;
res[1] = tmp_1;
tmp_2 = vaddq(vcomplex_mulq_n(a[1],(C0_3_notw) )  , vaddq(vcomplex_mulq_n(a[2],(C1_3_notw) )  , a[0]) ) ;
res[2] = tmp_2;


vmemory<float16_t,inputstrideenabled>::store(in, 0 , res[0], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(in, 1 , res[1], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(in, 2 , res[2], strideFactor);


};

/* Nb Multiplications = 4, 
Nb Additions = 6,
Nb Negates = 0,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_last(const complex<float16_t> *in,complex<float16_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


f16x8_t a[3];
f16x8_t res[3];


 f16x8_t tmp_0;
 f16x8_t tmp_1;
 f16x8_t tmp_2;


a[0] = vmemory<float16_t,inputstrideenabled>::load(in , 0 , strideFactor);
a[1] = vmemory<float16_t,inputstrideenabled>::load(in , 1 , strideFactor);
a[2] = vmemory<float16_t,inputstrideenabled>::load(in , 2 , strideFactor);


tmp_0 = vaddq(a[0] , vaddq(a[1] , a[2]) ) ;
res[0] = tmp_0;
tmp_1 = vaddq(vcomplex_mulq_n(a[2],(C0_3_notw) )  , vaddq(vcomplex_mulq_n(a[1],(C1_3_notw) )  , a[0]) ) ;
res[1] = tmp_1;
tmp_2 = vaddq(vcomplex_mulq_n(a[1],(C0_3_notw) )  , vaddq(vcomplex_mulq_n(a[2],(C1_3_notw) )  , a[0]) ) ;
res[2] = tmp_2;


vmemory<float16_t,inputstrideenabled>::store(out, 0, res[0], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(out, 1, res[1], strideFactor);
vmemory<float16_t,inputstrideenabled>::store(out, 2, res[2], strideFactor);


};

};

#endif /* #ifndef _RADIX_VECTOR_F16_3_H */