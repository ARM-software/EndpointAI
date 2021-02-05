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

#ifndef _RADIX_Q15_5_H
#define _RADIX_Q15_5_H

#include "complex.h"

/**************

DIRECT FFT 

***************/
template<int inputstrideenabled> 
struct radix<DIRECT,Q15, 5,inputstrideenabled>{

static constexpr complex<Q15> C0_5_tw{Q15{(q15_t)0x9872}, Q15{(q15_t)0xB4C3}};
  static constexpr complex<Q15> C1_5_tw{Q15{(q15_t)0x9872}, Q15{(q15_t)0x4B3D}};
  static constexpr complex<Q15> C2_5_tw{Q15{(q15_t)0x278E}, Q15{(q15_t)0x8644}};
  static constexpr complex<Q15> C3_5_tw{Q15{(q15_t)0x278E}, Q15{(q15_t)0x79BC}};
  
static constexpr complex<Q15> C0_5_notw{Q15{(q15_t)0x9872}, Q15{(q15_t)0xB4C3}};
  static constexpr complex<Q15> C1_5_notw{Q15{(q15_t)0x9872}, Q15{(q15_t)0x4B3D}};
  static constexpr complex<Q15> C2_5_notw{Q15{(q15_t)0x278E}, Q15{(q15_t)0x8644}};
  static constexpr complex<Q15> C3_5_notw{Q15{(q15_t)0x278E}, Q15{(q15_t)0x79BC}};
  

/* Nb Multiplications = 16, 
Nb Additions = 20,
Nb Negates = 0,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 4

*/
 __STATIC_FORCEINLINE void run_ip_notw(int stageLength,complex<Q15> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[5];
complex<Q15> res[5];


 complex<Q15> tmp_0;
 complex<Q31> tmp_1;
 complex<Q31> tmp_2;
 complex<Q31> tmp_3;
 complex<Q31> tmp_4;


a[0] = memory<complex<Q15>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<Q15>>::load(in,4 * stageLength * strideFactor);


tmp_0 = shadd( decq(a[0],3)   , shadd( decq(a[1],2)   , shadd( decq(a[2],1)   , shadd(a[3] , a[4]) ) ) ) ;
res[0] = tmp_0;
tmp_1 = MAC(MAC(MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C3_5_notw),a[4])  , (C2_5_notw),a[1])  , (C1_5_notw),a[3])  , (C0_5_notw),a[2]) ;
res[1] =  narrow<4,complex<Q15>>::run(tmp_1)  ;
tmp_2 = MAC(MAC(MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C3_5_notw),a[2])  , (C2_5_notw),a[3])  , (C1_5_notw),a[4])  , (C0_5_notw),a[1]) ;
res[2] =  narrow<4,complex<Q15>>::run(tmp_2)  ;
tmp_3 = MAC(MAC(MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C3_5_notw),a[3])  , (C2_5_notw),a[2])  , (C1_5_notw),a[1])  , (C0_5_notw),a[4]) ;
res[3] =  narrow<4,complex<Q15>>::run(tmp_3)  ;
tmp_4 = MAC(MAC(MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C3_5_notw),a[1])  , (C2_5_notw),a[4])  , (C1_5_notw),a[2])  , (C0_5_notw),a[3]) ;
res[4] =  narrow<4,complex<Q15>>::run(tmp_4)  ;


memory<complex<Q15>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<Q15>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<Q15>>::store(in,2 * stageLength * strideFactor , res[2]);
memory<complex<Q15>>::store(in,3 * stageLength * strideFactor , res[3]);
memory<complex<Q15>>::store(in,4 * stageLength * strideFactor , res[4]);


};

/* Nb Multiplications = 20, 
Nb Additions = 20,
Nb Negates = 0,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 4

*/
 __STATIC_FORCEINLINE void run_ip_tw(int stageLength,const complex<Q15> *w,complex<Q15> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[5];
complex<Q15> res[5];


 complex<Q15> tmp_0;
 complex<Q31> tmp_1;
 complex<Q31> tmp_2;
 complex<Q31> tmp_3;
 complex<Q31> tmp_4;


a[0] = memory<complex<Q15>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<Q15>>::load(in,4 * stageLength * strideFactor);


tmp_0 = shadd( decq(a[0],3)   , shadd( decq(a[1],2)   , shadd( decq(a[2],1)   , shadd(a[3] , a[4]) ) ) ) ;
res[0] = tmp_0;
tmp_1 = MAC(MAC(MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C3_5_tw),a[4])  , (C2_5_tw),a[1])  , (C1_5_tw),a[3])  , (C0_5_tw),a[2]) ;
res[1] =  narrow<4,complex<Q15>>::run(w[0] *  narrow<NORMALNARROW,complex<Q15>>::run(tmp_1)   )  ;
tmp_2 = MAC(MAC(MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C3_5_tw),a[2])  , (C2_5_tw),a[3])  , (C1_5_tw),a[4])  , (C0_5_tw),a[1]) ;
res[2] =  narrow<4,complex<Q15>>::run(w[1] *  narrow<NORMALNARROW,complex<Q15>>::run(tmp_2)   )  ;
tmp_3 = MAC(MAC(MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C3_5_tw),a[3])  , (C2_5_tw),a[2])  , (C1_5_tw),a[1])  , (C0_5_tw),a[4]) ;
res[3] =  narrow<4,complex<Q15>>::run(w[2] *  narrow<NORMALNARROW,complex<Q15>>::run(tmp_3)   )  ;
tmp_4 = MAC(MAC(MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C3_5_tw),a[1])  , (C2_5_tw),a[4])  , (C1_5_tw),a[2])  , (C0_5_tw),a[3]) ;
res[4] =  narrow<4,complex<Q15>>::run(w[3] *  narrow<NORMALNARROW,complex<Q15>>::run(tmp_4)   )  ;


memory<complex<Q15>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<Q15>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<Q15>>::store(in,2 * stageLength * strideFactor , res[2]);
memory<complex<Q15>>::store(in,3 * stageLength * strideFactor , res[3]);
memory<complex<Q15>>::store(in,4 * stageLength * strideFactor , res[4]);


};

/* Nb Multiplications = 16, 
Nb Additions = 20,
Nb Negates = 0,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 4

*/
 __STATIC_FORCEINLINE void run_op_notw(int stageLength,const complex<Q15> *in,complex<Q15> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[5];
complex<Q15> res[5];


 complex<Q15> tmp_0;
 complex<Q31> tmp_1;
 complex<Q31> tmp_2;
 complex<Q31> tmp_3;
 complex<Q31> tmp_4;


a[0] = memory<complex<Q15>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<Q15>>::load(in,4 * stageLength * strideFactor);


tmp_0 = shadd( decq(a[0],3)   , shadd( decq(a[1],2)   , shadd( decq(a[2],1)   , shadd(a[3] , a[4]) ) ) ) ;
res[0] = tmp_0;
tmp_1 = MAC(MAC(MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C3_5_notw),a[4])  , (C2_5_notw),a[1])  , (C1_5_notw),a[3])  , (C0_5_notw),a[2]) ;
res[1] =  narrow<4,complex<Q15>>::run(tmp_1)  ;
tmp_2 = MAC(MAC(MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C3_5_notw),a[2])  , (C2_5_notw),a[3])  , (C1_5_notw),a[4])  , (C0_5_notw),a[1]) ;
res[2] =  narrow<4,complex<Q15>>::run(tmp_2)  ;
tmp_3 = MAC(MAC(MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C3_5_notw),a[3])  , (C2_5_notw),a[2])  , (C1_5_notw),a[1])  , (C0_5_notw),a[4]) ;
res[3] =  narrow<4,complex<Q15>>::run(tmp_3)  ;
tmp_4 = MAC(MAC(MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C3_5_notw),a[1])  , (C2_5_notw),a[4])  , (C1_5_notw),a[2])  , (C0_5_notw),a[3]) ;
res[4] =  narrow<4,complex<Q15>>::run(tmp_4)  ;


memory<complex<Q15>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<Q15>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<Q15>>::store(out,2 * stageLength * strideFactor , res[2]);
memory<complex<Q15>>::store(out,3 * stageLength * strideFactor , res[3]);
memory<complex<Q15>>::store(out,4 * stageLength * strideFactor , res[4]);


};

/* Nb Multiplications = 20, 
Nb Additions = 20,
Nb Negates = 0,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 4

*/
 __STATIC_FORCEINLINE void run_op_tw(int stageLength,const complex<Q15> *w,const complex<Q15> *in,complex<Q15> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[5];
complex<Q15> res[5];


 complex<Q15> tmp_0;
 complex<Q31> tmp_1;
 complex<Q31> tmp_2;
 complex<Q31> tmp_3;
 complex<Q31> tmp_4;


a[0] = memory<complex<Q15>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<Q15>>::load(in,4 * stageLength * strideFactor);


tmp_0 = shadd( decq(a[0],3)   , shadd( decq(a[1],2)   , shadd( decq(a[2],1)   , shadd(a[3] , a[4]) ) ) ) ;
res[0] = tmp_0;
tmp_1 = MAC(MAC(MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C3_5_tw),a[4])  , (C2_5_tw),a[1])  , (C1_5_tw),a[3])  , (C0_5_tw),a[2]) ;
res[1] =  narrow<4,complex<Q15>>::run(w[0] *  narrow<NORMALNARROW,complex<Q15>>::run(tmp_1)   )  ;
tmp_2 = MAC(MAC(MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C3_5_tw),a[2])  , (C2_5_tw),a[3])  , (C1_5_tw),a[4])  , (C0_5_tw),a[1]) ;
res[2] =  narrow<4,complex<Q15>>::run(w[1] *  narrow<NORMALNARROW,complex<Q15>>::run(tmp_2)   )  ;
tmp_3 = MAC(MAC(MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C3_5_tw),a[3])  , (C2_5_tw),a[2])  , (C1_5_tw),a[1])  , (C0_5_tw),a[4]) ;
res[3] =  narrow<4,complex<Q15>>::run(w[2] *  narrow<NORMALNARROW,complex<Q15>>::run(tmp_3)   )  ;
tmp_4 = MAC(MAC(MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C3_5_tw),a[1])  , (C2_5_tw),a[4])  , (C1_5_tw),a[2])  , (C0_5_tw),a[3]) ;
res[4] =  narrow<4,complex<Q15>>::run(w[3] *  narrow<NORMALNARROW,complex<Q15>>::run(tmp_4)   )  ;


memory<complex<Q15>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<Q15>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<Q15>>::store(out,2 * stageLength * strideFactor , res[2]);
memory<complex<Q15>>::store(out,3 * stageLength * strideFactor , res[3]);
memory<complex<Q15>>::store(out,4 * stageLength * strideFactor , res[4]);


};

/* Nb Multiplications = 16, 
Nb Additions = 20,
Nb Negates = 0,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 4

*/
 __STATIC_FORCEINLINE void run_ip_last(complex<Q15> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[5];
complex<Q15> res[5];


 complex<Q15> tmp_0;
 complex<Q31> tmp_1;
 complex<Q31> tmp_2;
 complex<Q31> tmp_3;
 complex<Q31> tmp_4;


a[0] = memory<complex<Q15>>::load(in,0 * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * strideFactor);
a[4] = memory<complex<Q15>>::load(in,4 * strideFactor);


tmp_0 = shadd( decq(a[0],3)   , shadd( decq(a[1],2)   , shadd( decq(a[2],1)   , shadd(a[3] , a[4]) ) ) ) ;
res[0] = tmp_0;
tmp_1 = MAC(MAC(MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C3_5_notw),a[4])  , (C2_5_notw),a[1])  , (C1_5_notw),a[3])  , (C0_5_notw),a[2]) ;
res[1] =  narrow<4,complex<Q15>>::run(tmp_1)  ;
tmp_2 = MAC(MAC(MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C3_5_notw),a[2])  , (C2_5_notw),a[3])  , (C1_5_notw),a[4])  , (C0_5_notw),a[1]) ;
res[2] =  narrow<4,complex<Q15>>::run(tmp_2)  ;
tmp_3 = MAC(MAC(MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C3_5_notw),a[3])  , (C2_5_notw),a[2])  , (C1_5_notw),a[1])  , (C0_5_notw),a[4]) ;
res[3] =  narrow<4,complex<Q15>>::run(tmp_3)  ;
tmp_4 = MAC(MAC(MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C3_5_notw),a[1])  , (C2_5_notw),a[4])  , (C1_5_notw),a[2])  , (C0_5_notw),a[3]) ;
res[4] =  narrow<4,complex<Q15>>::run(tmp_4)  ;


memory<complex<Q15>>::store(in,0 * strideFactor , res[0]);
memory<complex<Q15>>::store(in,1 * strideFactor , res[1]);
memory<complex<Q15>>::store(in,2 * strideFactor , res[2]);
memory<complex<Q15>>::store(in,3 * strideFactor , res[3]);
memory<complex<Q15>>::store(in,4 * strideFactor , res[4]);


};

/* Nb Multiplications = 16, 
Nb Additions = 20,
Nb Negates = 0,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 4

*/
 __STATIC_FORCEINLINE void run_op_last(const complex<Q15> *in,complex<Q15> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[5];
complex<Q15> res[5];


 complex<Q15> tmp_0;
 complex<Q31> tmp_1;
 complex<Q31> tmp_2;
 complex<Q31> tmp_3;
 complex<Q31> tmp_4;


a[0] = memory<complex<Q15>>::load(in,0 * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * strideFactor);
a[4] = memory<complex<Q15>>::load(in,4 * strideFactor);


tmp_0 = shadd( decq(a[0],3)   , shadd( decq(a[1],2)   , shadd( decq(a[2],1)   , shadd(a[3] , a[4]) ) ) ) ;
res[0] = tmp_0;
tmp_1 = MAC(MAC(MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C3_5_notw),a[4])  , (C2_5_notw),a[1])  , (C1_5_notw),a[3])  , (C0_5_notw),a[2]) ;
res[1] =  narrow<4,complex<Q15>>::run(tmp_1)  ;
tmp_2 = MAC(MAC(MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C3_5_notw),a[2])  , (C2_5_notw),a[3])  , (C1_5_notw),a[4])  , (C0_5_notw),a[1]) ;
res[2] =  narrow<4,complex<Q15>>::run(tmp_2)  ;
tmp_3 = MAC(MAC(MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C3_5_notw),a[3])  , (C2_5_notw),a[2])  , (C1_5_notw),a[1])  , (C0_5_notw),a[4]) ;
res[3] =  narrow<4,complex<Q15>>::run(tmp_3)  ;
tmp_4 = MAC(MAC(MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C3_5_notw),a[1])  , (C2_5_notw),a[4])  , (C1_5_notw),a[2])  , (C0_5_notw),a[3]) ;
res[4] =  narrow<4,complex<Q15>>::run(tmp_4)  ;


memory<complex<Q15>>::store(out,0 * strideFactor , res[0]);
memory<complex<Q15>>::store(out,1 * strideFactor , res[1]);
memory<complex<Q15>>::store(out,2 * strideFactor , res[2]);
memory<complex<Q15>>::store(out,3 * strideFactor , res[3]);
memory<complex<Q15>>::store(out,4 * strideFactor , res[4]);


};

};

/**************

INVERSE FFT 

***************/
template<int inputstrideenabled> 
struct radix<INVERSE,Q15, 5,inputstrideenabled>{

static constexpr complex<Q15> C0_5_tw{Q15{(q15_t)0x9872}, Q15{(q15_t)0xB4C3}};
  static constexpr complex<Q15> C1_5_tw{Q15{(q15_t)0x9872}, Q15{(q15_t)0x4B3D}};
  static constexpr complex<Q15> C2_5_tw{Q15{(q15_t)0x278E}, Q15{(q15_t)0x8644}};
  static constexpr complex<Q15> C3_5_tw{Q15{(q15_t)0x278E}, Q15{(q15_t)0x79BC}};
  
static constexpr complex<Q15> C0_5_notw{Q15{(q15_t)0x9872}, Q15{(q15_t)0xB4C3}};
  static constexpr complex<Q15> C1_5_notw{Q15{(q15_t)0x9872}, Q15{(q15_t)0x4B3D}};
  static constexpr complex<Q15> C2_5_notw{Q15{(q15_t)0x278E}, Q15{(q15_t)0x8644}};
  static constexpr complex<Q15> C3_5_notw{Q15{(q15_t)0x278E}, Q15{(q15_t)0x79BC}};
  

/* Nb Multiplications = 16, 
Nb Additions = 20,
Nb Negates = 0,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 4

*/
 __STATIC_FORCEINLINE void run_ip_notw(int stageLength,complex<Q15> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[5];
complex<Q15> res[5];


 complex<Q15> tmp_0;
 complex<Q31> tmp_1;
 complex<Q31> tmp_2;
 complex<Q31> tmp_3;
 complex<Q31> tmp_4;


a[0] = memory<complex<Q15>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<Q15>>::load(in,4 * stageLength * strideFactor);


tmp_0 = shadd( decq(a[0],3)   , shadd( decq(a[1],2)   , shadd( decq(a[2],1)   , shadd(a[3] , a[4]) ) ) ) ;
res[0] = tmp_0;
tmp_1 = MAC(MAC(MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C3_5_notw),a[1])  , (C2_5_notw),a[4])  , (C1_5_notw),a[2])  , (C0_5_notw),a[3]) ;
res[1] =  narrow<4,complex<Q15>>::run(tmp_1)  ;
tmp_2 = MAC(MAC(MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C3_5_notw),a[3])  , (C2_5_notw),a[2])  , (C1_5_notw),a[1])  , (C0_5_notw),a[4]) ;
res[2] =  narrow<4,complex<Q15>>::run(tmp_2)  ;
tmp_3 = MAC(MAC(MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C3_5_notw),a[2])  , (C2_5_notw),a[3])  , (C1_5_notw),a[4])  , (C0_5_notw),a[1]) ;
res[3] =  narrow<4,complex<Q15>>::run(tmp_3)  ;
tmp_4 = MAC(MAC(MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C3_5_notw),a[4])  , (C2_5_notw),a[1])  , (C1_5_notw),a[3])  , (C0_5_notw),a[2]) ;
res[4] =  narrow<4,complex<Q15>>::run(tmp_4)  ;


memory<complex<Q15>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<Q15>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<Q15>>::store(in,2 * stageLength * strideFactor , res[2]);
memory<complex<Q15>>::store(in,3 * stageLength * strideFactor , res[3]);
memory<complex<Q15>>::store(in,4 * stageLength * strideFactor , res[4]);


};

/* Nb Multiplications = 20, 
Nb Additions = 20,
Nb Negates = 0,
Nb Conjugates = 4,
*/
/*

Output format : Shifted right by 4

*/
 __STATIC_FORCEINLINE void run_ip_tw(int stageLength,const complex<Q15> *w,complex<Q15> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[5];
complex<Q15> res[5];


 complex<Q15> tmp_0;
 complex<Q31> tmp_1;
 complex<Q31> tmp_2;
 complex<Q31> tmp_3;
 complex<Q31> tmp_4;


a[0] = memory<complex<Q15>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<Q15>>::load(in,4 * stageLength * strideFactor);


tmp_0 = shadd( decq(a[0],3)   , shadd( decq(a[1],2)   , shadd( decq(a[2],1)   , shadd(a[3] , a[4]) ) ) ) ;
res[0] = tmp_0;
tmp_1 = MAC(MAC(MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C3_5_tw),a[1])  , (C2_5_tw),a[4])  , (C1_5_tw),a[2])  , (C0_5_tw),a[3]) ;
res[1] =  narrow<4,complex<Q15>>::run(multconj( narrow<NORMALNARROW,complex<Q15>>::run(tmp_1)   , w[0]) )  ;
tmp_2 = MAC(MAC(MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C3_5_tw),a[3])  , (C2_5_tw),a[2])  , (C1_5_tw),a[1])  , (C0_5_tw),a[4]) ;
res[2] =  narrow<4,complex<Q15>>::run(multconj( narrow<NORMALNARROW,complex<Q15>>::run(tmp_2)   , w[1]) )  ;
tmp_3 = MAC(MAC(MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C3_5_tw),a[2])  , (C2_5_tw),a[3])  , (C1_5_tw),a[4])  , (C0_5_tw),a[1]) ;
res[3] =  narrow<4,complex<Q15>>::run(multconj( narrow<NORMALNARROW,complex<Q15>>::run(tmp_3)   , w[2]) )  ;
tmp_4 = MAC(MAC(MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C3_5_tw),a[4])  , (C2_5_tw),a[1])  , (C1_5_tw),a[3])  , (C0_5_tw),a[2]) ;
res[4] =  narrow<4,complex<Q15>>::run(multconj( narrow<NORMALNARROW,complex<Q15>>::run(tmp_4)   , w[3]) )  ;


memory<complex<Q15>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<Q15>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<Q15>>::store(in,2 * stageLength * strideFactor , res[2]);
memory<complex<Q15>>::store(in,3 * stageLength * strideFactor , res[3]);
memory<complex<Q15>>::store(in,4 * stageLength * strideFactor , res[4]);


};

/* Nb Multiplications = 16, 
Nb Additions = 20,
Nb Negates = 0,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 4

*/
 __STATIC_FORCEINLINE void run_op_notw(int stageLength,const complex<Q15> *in,complex<Q15> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[5];
complex<Q15> res[5];


 complex<Q15> tmp_0;
 complex<Q31> tmp_1;
 complex<Q31> tmp_2;
 complex<Q31> tmp_3;
 complex<Q31> tmp_4;


a[0] = memory<complex<Q15>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<Q15>>::load(in,4 * stageLength * strideFactor);


tmp_0 = shadd( decq(a[0],3)   , shadd( decq(a[1],2)   , shadd( decq(a[2],1)   , shadd(a[3] , a[4]) ) ) ) ;
res[0] = tmp_0;
tmp_1 = MAC(MAC(MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C3_5_notw),a[1])  , (C2_5_notw),a[4])  , (C1_5_notw),a[2])  , (C0_5_notw),a[3]) ;
res[1] =  narrow<4,complex<Q15>>::run(tmp_1)  ;
tmp_2 = MAC(MAC(MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C3_5_notw),a[3])  , (C2_5_notw),a[2])  , (C1_5_notw),a[1])  , (C0_5_notw),a[4]) ;
res[2] =  narrow<4,complex<Q15>>::run(tmp_2)  ;
tmp_3 = MAC(MAC(MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C3_5_notw),a[2])  , (C2_5_notw),a[3])  , (C1_5_notw),a[4])  , (C0_5_notw),a[1]) ;
res[3] =  narrow<4,complex<Q15>>::run(tmp_3)  ;
tmp_4 = MAC(MAC(MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C3_5_notw),a[4])  , (C2_5_notw),a[1])  , (C1_5_notw),a[3])  , (C0_5_notw),a[2]) ;
res[4] =  narrow<4,complex<Q15>>::run(tmp_4)  ;


memory<complex<Q15>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<Q15>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<Q15>>::store(out,2 * stageLength * strideFactor , res[2]);
memory<complex<Q15>>::store(out,3 * stageLength * strideFactor , res[3]);
memory<complex<Q15>>::store(out,4 * stageLength * strideFactor , res[4]);


};

/* Nb Multiplications = 20, 
Nb Additions = 20,
Nb Negates = 0,
Nb Conjugates = 4,
*/
/*

Output format : Shifted right by 4

*/
 __STATIC_FORCEINLINE void run_op_tw(int stageLength,const complex<Q15> *w,const complex<Q15> *in,complex<Q15> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[5];
complex<Q15> res[5];


 complex<Q15> tmp_0;
 complex<Q31> tmp_1;
 complex<Q31> tmp_2;
 complex<Q31> tmp_3;
 complex<Q31> tmp_4;


a[0] = memory<complex<Q15>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<Q15>>::load(in,4 * stageLength * strideFactor);


tmp_0 = shadd( decq(a[0],3)   , shadd( decq(a[1],2)   , shadd( decq(a[2],1)   , shadd(a[3] , a[4]) ) ) ) ;
res[0] = tmp_0;
tmp_1 = MAC(MAC(MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C3_5_tw),a[1])  , (C2_5_tw),a[4])  , (C1_5_tw),a[2])  , (C0_5_tw),a[3]) ;
res[1] =  narrow<4,complex<Q15>>::run(multconj( narrow<NORMALNARROW,complex<Q15>>::run(tmp_1)   , w[0]) )  ;
tmp_2 = MAC(MAC(MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C3_5_tw),a[3])  , (C2_5_tw),a[2])  , (C1_5_tw),a[1])  , (C0_5_tw),a[4]) ;
res[2] =  narrow<4,complex<Q15>>::run(multconj( narrow<NORMALNARROW,complex<Q15>>::run(tmp_2)   , w[1]) )  ;
tmp_3 = MAC(MAC(MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C3_5_tw),a[2])  , (C2_5_tw),a[3])  , (C1_5_tw),a[4])  , (C0_5_tw),a[1]) ;
res[3] =  narrow<4,complex<Q15>>::run(multconj( narrow<NORMALNARROW,complex<Q15>>::run(tmp_3)   , w[2]) )  ;
tmp_4 = MAC(MAC(MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C3_5_tw),a[4])  , (C2_5_tw),a[1])  , (C1_5_tw),a[3])  , (C0_5_tw),a[2]) ;
res[4] =  narrow<4,complex<Q15>>::run(multconj( narrow<NORMALNARROW,complex<Q15>>::run(tmp_4)   , w[3]) )  ;


memory<complex<Q15>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<Q15>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<Q15>>::store(out,2 * stageLength * strideFactor , res[2]);
memory<complex<Q15>>::store(out,3 * stageLength * strideFactor , res[3]);
memory<complex<Q15>>::store(out,4 * stageLength * strideFactor , res[4]);


};

/* Nb Multiplications = 16, 
Nb Additions = 20,
Nb Negates = 0,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 4

*/
 __STATIC_FORCEINLINE void run_ip_last(complex<Q15> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[5];
complex<Q15> res[5];


 complex<Q15> tmp_0;
 complex<Q31> tmp_1;
 complex<Q31> tmp_2;
 complex<Q31> tmp_3;
 complex<Q31> tmp_4;


a[0] = memory<complex<Q15>>::load(in,0 * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * strideFactor);
a[4] = memory<complex<Q15>>::load(in,4 * strideFactor);


tmp_0 = shadd( decq(a[0],3)   , shadd( decq(a[1],2)   , shadd( decq(a[2],1)   , shadd(a[3] , a[4]) ) ) ) ;
res[0] = tmp_0;
tmp_1 = MAC(MAC(MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C3_5_notw),a[1])  , (C2_5_notw),a[4])  , (C1_5_notw),a[2])  , (C0_5_notw),a[3]) ;
res[1] =  narrow<4,complex<Q15>>::run(tmp_1)  ;
tmp_2 = MAC(MAC(MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C3_5_notw),a[3])  , (C2_5_notw),a[2])  , (C1_5_notw),a[1])  , (C0_5_notw),a[4]) ;
res[2] =  narrow<4,complex<Q15>>::run(tmp_2)  ;
tmp_3 = MAC(MAC(MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C3_5_notw),a[2])  , (C2_5_notw),a[3])  , (C1_5_notw),a[4])  , (C0_5_notw),a[1]) ;
res[3] =  narrow<4,complex<Q15>>::run(tmp_3)  ;
tmp_4 = MAC(MAC(MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C3_5_notw),a[4])  , (C2_5_notw),a[1])  , (C1_5_notw),a[3])  , (C0_5_notw),a[2]) ;
res[4] =  narrow<4,complex<Q15>>::run(tmp_4)  ;


memory<complex<Q15>>::store(in,0 * strideFactor , res[0]);
memory<complex<Q15>>::store(in,1 * strideFactor , res[1]);
memory<complex<Q15>>::store(in,2 * strideFactor , res[2]);
memory<complex<Q15>>::store(in,3 * strideFactor , res[3]);
memory<complex<Q15>>::store(in,4 * strideFactor , res[4]);


};

/* Nb Multiplications = 16, 
Nb Additions = 20,
Nb Negates = 0,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 4

*/
 __STATIC_FORCEINLINE void run_op_last(const complex<Q15> *in,complex<Q15> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[5];
complex<Q15> res[5];


 complex<Q15> tmp_0;
 complex<Q31> tmp_1;
 complex<Q31> tmp_2;
 complex<Q31> tmp_3;
 complex<Q31> tmp_4;


a[0] = memory<complex<Q15>>::load(in,0 * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * strideFactor);
a[4] = memory<complex<Q15>>::load(in,4 * strideFactor);


tmp_0 = shadd( decq(a[0],3)   , shadd( decq(a[1],2)   , shadd( decq(a[2],1)   , shadd(a[3] , a[4]) ) ) ) ;
res[0] = tmp_0;
tmp_1 = MAC(MAC(MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C3_5_notw),a[1])  , (C2_5_notw),a[4])  , (C1_5_notw),a[2])  , (C0_5_notw),a[3]) ;
res[1] =  narrow<4,complex<Q15>>::run(tmp_1)  ;
tmp_2 = MAC(MAC(MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C3_5_notw),a[3])  , (C2_5_notw),a[2])  , (C1_5_notw),a[1])  , (C0_5_notw),a[4]) ;
res[2] =  narrow<4,complex<Q15>>::run(tmp_2)  ;
tmp_3 = MAC(MAC(MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C3_5_notw),a[2])  , (C2_5_notw),a[3])  , (C1_5_notw),a[4])  , (C0_5_notw),a[1]) ;
res[3] =  narrow<4,complex<Q15>>::run(tmp_3)  ;
tmp_4 = MAC(MAC(MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C3_5_notw),a[4])  , (C2_5_notw),a[1])  , (C1_5_notw),a[3])  , (C0_5_notw),a[2]) ;
res[4] =  narrow<4,complex<Q15>>::run(tmp_4)  ;


memory<complex<Q15>>::store(out,0 * strideFactor , res[0]);
memory<complex<Q15>>::store(out,1 * strideFactor , res[1]);
memory<complex<Q15>>::store(out,2 * strideFactor , res[2]);
memory<complex<Q15>>::store(out,3 * strideFactor , res[3]);
memory<complex<Q15>>::store(out,4 * strideFactor , res[4]);


};

};

#endif /* #ifndef _RADIX_Q15_5_H */