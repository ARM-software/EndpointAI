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

#ifndef _RADIX_Q15_6_H
#define _RADIX_Q15_6_H

#include "complex.h"

/**************

DIRECT FFT 

***************/
template<int inputstrideenabled> 
struct radix<DIRECT,Q15, 6,inputstrideenabled>{

static constexpr complex<Q15> C0_6_tw{Q15{(q15_t)0xC000}, Q15{(q15_t)0x9126}};
  static constexpr complex<Q15> C1_6_tw{Q15{(q15_t)0xC000}, Q15{(q15_t)0x6EDA}};
  static constexpr complex<Q15> C2_6_tw{Q15{(q15_t)0x4000}, Q15{(q15_t)0x9126}};
  
static constexpr complex<Q15> C0_6_notw{Q15{(q15_t)0xC000}, Q15{(q15_t)0x9126}};
  static constexpr complex<Q15> C1_6_notw{Q15{(q15_t)0xC000}, Q15{(q15_t)0x6EDA}};
  static constexpr complex<Q15> C2_6_notw{Q15{(q15_t)0x4000}, Q15{(q15_t)0x9126}};
  

/* Nb Multiplications = 10, 
Nb Additions = 18,
Nb Negates = 3,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 3

*/
 __STATIC_FORCEINLINE void run_ip_notw(int stageLength,complex<Q15> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[6];
complex<Q15> res[6];


 complex<Q15> tmp_0;
 complex<Q15> tmp_1;
 complex<Q15> tmp_2;
 complex<Q15> tmp_3;
 complex<Q15> tmp_4;
 complex<Q31> tmp_5;
 complex<Q15> tmp_6;
 complex<Q31> tmp_7;
 complex<Q15> tmp_8;
 complex<Q31> tmp_9;
 complex<Q31> tmp_10;
 complex<Q31> tmp_11;
 complex<Q31> tmp_12;
 complex<Q31> tmp_13;


a[0] = memory<complex<Q15>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<Q15>>::load(in,4 * stageLength * strideFactor);
a[5] = memory<complex<Q15>>::load(in,5 * stageLength * strideFactor);


tmp_0 = shadd(a[0] , a[3]) ;
tmp_1 = shadd(a[1] , a[4]) ;
tmp_2 = shadd(a[2] , a[5]) ;
tmp_8 = shadd( decq(tmp_0,1)   , shadd(tmp_1 , tmp_2) ) ;
res[0] = tmp_8;
tmp_3 = shsub(a[0] , a[3]) ;
tmp_4 = shsub(a[2] , a[5]) ;
tmp_5 = (C0_6_notw) * tmp_4 ;
tmp_6 = shsub(a[1] , a[4]) ;
tmp_7 = (C2_6_notw) * tmp_6 ;
tmp_9 = ( widen<complex<Q15>>::run(tmp_3)   + (tmp_5 + tmp_7) ) ;
res[1] =  narrow<2,complex<Q15>>::run(tmp_9)  ;
tmp_10 = ( widen<complex<Q15>>::run(tmp_0)   + MAC((C0_6_notw) * tmp_1  , (C1_6_notw),tmp_2) ) ;
res[2] =  narrow<2,complex<Q15>>::run(tmp_10)  ;
tmp_11 = ( widen<complex<Q15>>::run(tmp_3)   + MAC((C0_6_notw) *  narrow<NORMALNARROW,complex<Q15>>::run(tmp_7)    , (C1_6_notw), narrow<NORMALNARROW,complex<Q15>>::run(tmp_5)  ) ) ;
res[3] =  narrow<2,complex<Q15>>::run(tmp_11)  ;
tmp_12 = ( widen<complex<Q15>>::run(tmp_0)   + MAC((C0_6_notw) * tmp_2  , (C1_6_notw),tmp_1) ) ;
res[4] =  narrow<2,complex<Q15>>::run(tmp_12)  ;
tmp_13 = ( widen<complex<Q15>>::run(tmp_3)   + MAC((C0_6_notw) *  narrow<NORMALNARROW,complex<Q15>>::run(tmp_5)    , (C1_6_notw), narrow<NORMALNARROW,complex<Q15>>::run(tmp_7)  ) ) ;
res[5] =  narrow<2,complex<Q15>>::run(tmp_13)  ;


memory<complex<Q15>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<Q15>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<Q15>>::store(in,2 * stageLength * strideFactor , res[2]);
memory<complex<Q15>>::store(in,3 * stageLength * strideFactor , res[3]);
memory<complex<Q15>>::store(in,4 * stageLength * strideFactor , res[4]);
memory<complex<Q15>>::store(in,5 * stageLength * strideFactor , res[5]);


};

/* Nb Multiplications = 15, 
Nb Additions = 18,
Nb Negates = 3,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 3

*/
 __STATIC_FORCEINLINE void run_ip_tw(int stageLength,const complex<Q15> *w,complex<Q15> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[6];
complex<Q15> res[6];


 complex<Q15> tmp_0;
 complex<Q15> tmp_1;
 complex<Q15> tmp_2;
 complex<Q15> tmp_3;
 complex<Q15> tmp_4;
 complex<Q31> tmp_5;
 complex<Q15> tmp_6;
 complex<Q31> tmp_7;
 complex<Q15> tmp_8;
 complex<Q31> tmp_9;
 complex<Q31> tmp_10;
 complex<Q31> tmp_11;
 complex<Q31> tmp_12;
 complex<Q31> tmp_13;


a[0] = memory<complex<Q15>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<Q15>>::load(in,4 * stageLength * strideFactor);
a[5] = memory<complex<Q15>>::load(in,5 * stageLength * strideFactor);


tmp_0 = shadd(a[0] , a[3]) ;
tmp_1 = shadd(a[1] , a[4]) ;
tmp_2 = shadd(a[2] , a[5]) ;
tmp_8 = shadd( decq(tmp_0,1)   , shadd(tmp_1 , tmp_2) ) ;
res[0] = tmp_8;
tmp_3 = shsub(a[0] , a[3]) ;
tmp_4 = shsub(a[2] , a[5]) ;
tmp_5 = (C0_6_tw) * tmp_4 ;
tmp_6 = shsub(a[1] , a[4]) ;
tmp_7 = (C2_6_tw) * tmp_6 ;
tmp_9 = ( widen<complex<Q15>>::run(tmp_3)   + (tmp_5 + tmp_7) ) ;
res[1] =  narrow<2,complex<Q15>>::run(w[0] *  narrow<NORMALNARROW,complex<Q15>>::run(tmp_9)   )  ;
tmp_10 = ( widen<complex<Q15>>::run(tmp_0)   + MAC((C0_6_tw) * tmp_1  , (C1_6_tw),tmp_2) ) ;
res[2] =  narrow<2,complex<Q15>>::run(w[1] *  narrow<NORMALNARROW,complex<Q15>>::run(tmp_10)   )  ;
tmp_11 = ( widen<complex<Q15>>::run(tmp_3)   + MAC((C0_6_tw) *  narrow<NORMALNARROW,complex<Q15>>::run(tmp_7)    , (C1_6_tw), narrow<NORMALNARROW,complex<Q15>>::run(tmp_5)  ) ) ;
res[3] =  narrow<2,complex<Q15>>::run(w[2] *  narrow<NORMALNARROW,complex<Q15>>::run(tmp_11)   )  ;
tmp_12 = ( widen<complex<Q15>>::run(tmp_0)   + MAC((C0_6_tw) * tmp_2  , (C1_6_tw),tmp_1) ) ;
res[4] =  narrow<2,complex<Q15>>::run(w[3] *  narrow<NORMALNARROW,complex<Q15>>::run(tmp_12)   )  ;
tmp_13 = ( widen<complex<Q15>>::run(tmp_3)   + MAC((C0_6_tw) *  narrow<NORMALNARROW,complex<Q15>>::run(tmp_5)    , (C1_6_tw), narrow<NORMALNARROW,complex<Q15>>::run(tmp_7)  ) ) ;
res[5] =  narrow<2,complex<Q15>>::run(w[4] *  narrow<NORMALNARROW,complex<Q15>>::run(tmp_13)   )  ;


memory<complex<Q15>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<Q15>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<Q15>>::store(in,2 * stageLength * strideFactor , res[2]);
memory<complex<Q15>>::store(in,3 * stageLength * strideFactor , res[3]);
memory<complex<Q15>>::store(in,4 * stageLength * strideFactor , res[4]);
memory<complex<Q15>>::store(in,5 * stageLength * strideFactor , res[5]);


};

/* Nb Multiplications = 10, 
Nb Additions = 18,
Nb Negates = 3,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 3

*/
 __STATIC_FORCEINLINE void run_op_notw(int stageLength,const complex<Q15> *in,complex<Q15> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[6];
complex<Q15> res[6];


 complex<Q15> tmp_0;
 complex<Q15> tmp_1;
 complex<Q15> tmp_2;
 complex<Q15> tmp_3;
 complex<Q15> tmp_4;
 complex<Q31> tmp_5;
 complex<Q15> tmp_6;
 complex<Q31> tmp_7;
 complex<Q15> tmp_8;
 complex<Q31> tmp_9;
 complex<Q31> tmp_10;
 complex<Q31> tmp_11;
 complex<Q31> tmp_12;
 complex<Q31> tmp_13;


a[0] = memory<complex<Q15>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<Q15>>::load(in,4 * stageLength * strideFactor);
a[5] = memory<complex<Q15>>::load(in,5 * stageLength * strideFactor);


tmp_0 = shadd(a[0] , a[3]) ;
tmp_1 = shadd(a[1] , a[4]) ;
tmp_2 = shadd(a[2] , a[5]) ;
tmp_8 = shadd( decq(tmp_0,1)   , shadd(tmp_1 , tmp_2) ) ;
res[0] = tmp_8;
tmp_3 = shsub(a[0] , a[3]) ;
tmp_4 = shsub(a[2] , a[5]) ;
tmp_5 = (C0_6_notw) * tmp_4 ;
tmp_6 = shsub(a[1] , a[4]) ;
tmp_7 = (C2_6_notw) * tmp_6 ;
tmp_9 = ( widen<complex<Q15>>::run(tmp_3)   + (tmp_5 + tmp_7) ) ;
res[1] =  narrow<2,complex<Q15>>::run(tmp_9)  ;
tmp_10 = ( widen<complex<Q15>>::run(tmp_0)   + MAC((C0_6_notw) * tmp_1  , (C1_6_notw),tmp_2) ) ;
res[2] =  narrow<2,complex<Q15>>::run(tmp_10)  ;
tmp_11 = ( widen<complex<Q15>>::run(tmp_3)   + MAC((C0_6_notw) *  narrow<NORMALNARROW,complex<Q15>>::run(tmp_7)    , (C1_6_notw), narrow<NORMALNARROW,complex<Q15>>::run(tmp_5)  ) ) ;
res[3] =  narrow<2,complex<Q15>>::run(tmp_11)  ;
tmp_12 = ( widen<complex<Q15>>::run(tmp_0)   + MAC((C0_6_notw) * tmp_2  , (C1_6_notw),tmp_1) ) ;
res[4] =  narrow<2,complex<Q15>>::run(tmp_12)  ;
tmp_13 = ( widen<complex<Q15>>::run(tmp_3)   + MAC((C0_6_notw) *  narrow<NORMALNARROW,complex<Q15>>::run(tmp_5)    , (C1_6_notw), narrow<NORMALNARROW,complex<Q15>>::run(tmp_7)  ) ) ;
res[5] =  narrow<2,complex<Q15>>::run(tmp_13)  ;


memory<complex<Q15>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<Q15>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<Q15>>::store(out,2 * stageLength * strideFactor , res[2]);
memory<complex<Q15>>::store(out,3 * stageLength * strideFactor , res[3]);
memory<complex<Q15>>::store(out,4 * stageLength * strideFactor , res[4]);
memory<complex<Q15>>::store(out,5 * stageLength * strideFactor , res[5]);


};

/* Nb Multiplications = 15, 
Nb Additions = 18,
Nb Negates = 3,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 3

*/
 __STATIC_FORCEINLINE void run_op_tw(int stageLength,const complex<Q15> *w,const complex<Q15> *in,complex<Q15> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[6];
complex<Q15> res[6];


 complex<Q15> tmp_0;
 complex<Q15> tmp_1;
 complex<Q15> tmp_2;
 complex<Q15> tmp_3;
 complex<Q15> tmp_4;
 complex<Q31> tmp_5;
 complex<Q15> tmp_6;
 complex<Q31> tmp_7;
 complex<Q15> tmp_8;
 complex<Q31> tmp_9;
 complex<Q31> tmp_10;
 complex<Q31> tmp_11;
 complex<Q31> tmp_12;
 complex<Q31> tmp_13;


a[0] = memory<complex<Q15>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<Q15>>::load(in,4 * stageLength * strideFactor);
a[5] = memory<complex<Q15>>::load(in,5 * stageLength * strideFactor);


tmp_0 = shadd(a[0] , a[3]) ;
tmp_1 = shadd(a[1] , a[4]) ;
tmp_2 = shadd(a[2] , a[5]) ;
tmp_8 = shadd( decq(tmp_0,1)   , shadd(tmp_1 , tmp_2) ) ;
res[0] = tmp_8;
tmp_3 = shsub(a[0] , a[3]) ;
tmp_4 = shsub(a[2] , a[5]) ;
tmp_5 = (C0_6_tw) * tmp_4 ;
tmp_6 = shsub(a[1] , a[4]) ;
tmp_7 = (C2_6_tw) * tmp_6 ;
tmp_9 = ( widen<complex<Q15>>::run(tmp_3)   + (tmp_5 + tmp_7) ) ;
res[1] =  narrow<2,complex<Q15>>::run(w[0] *  narrow<NORMALNARROW,complex<Q15>>::run(tmp_9)   )  ;
tmp_10 = ( widen<complex<Q15>>::run(tmp_0)   + MAC((C0_6_tw) * tmp_1  , (C1_6_tw),tmp_2) ) ;
res[2] =  narrow<2,complex<Q15>>::run(w[1] *  narrow<NORMALNARROW,complex<Q15>>::run(tmp_10)   )  ;
tmp_11 = ( widen<complex<Q15>>::run(tmp_3)   + MAC((C0_6_tw) *  narrow<NORMALNARROW,complex<Q15>>::run(tmp_7)    , (C1_6_tw), narrow<NORMALNARROW,complex<Q15>>::run(tmp_5)  ) ) ;
res[3] =  narrow<2,complex<Q15>>::run(w[2] *  narrow<NORMALNARROW,complex<Q15>>::run(tmp_11)   )  ;
tmp_12 = ( widen<complex<Q15>>::run(tmp_0)   + MAC((C0_6_tw) * tmp_2  , (C1_6_tw),tmp_1) ) ;
res[4] =  narrow<2,complex<Q15>>::run(w[3] *  narrow<NORMALNARROW,complex<Q15>>::run(tmp_12)   )  ;
tmp_13 = ( widen<complex<Q15>>::run(tmp_3)   + MAC((C0_6_tw) *  narrow<NORMALNARROW,complex<Q15>>::run(tmp_5)    , (C1_6_tw), narrow<NORMALNARROW,complex<Q15>>::run(tmp_7)  ) ) ;
res[5] =  narrow<2,complex<Q15>>::run(w[4] *  narrow<NORMALNARROW,complex<Q15>>::run(tmp_13)   )  ;


memory<complex<Q15>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<Q15>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<Q15>>::store(out,2 * stageLength * strideFactor , res[2]);
memory<complex<Q15>>::store(out,3 * stageLength * strideFactor , res[3]);
memory<complex<Q15>>::store(out,4 * stageLength * strideFactor , res[4]);
memory<complex<Q15>>::store(out,5 * stageLength * strideFactor , res[5]);


};

/* Nb Multiplications = 10, 
Nb Additions = 18,
Nb Negates = 3,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 3

*/
 __STATIC_FORCEINLINE void run_ip_last(complex<Q15> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[6];
complex<Q15> res[6];


 complex<Q15> tmp_0;
 complex<Q15> tmp_1;
 complex<Q15> tmp_2;
 complex<Q15> tmp_3;
 complex<Q15> tmp_4;
 complex<Q31> tmp_5;
 complex<Q15> tmp_6;
 complex<Q31> tmp_7;
 complex<Q15> tmp_8;
 complex<Q31> tmp_9;
 complex<Q31> tmp_10;
 complex<Q31> tmp_11;
 complex<Q31> tmp_12;
 complex<Q31> tmp_13;


a[0] = memory<complex<Q15>>::load(in,0 * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * strideFactor);
a[4] = memory<complex<Q15>>::load(in,4 * strideFactor);
a[5] = memory<complex<Q15>>::load(in,5 * strideFactor);


tmp_0 = shadd(a[0] , a[3]) ;
tmp_1 = shadd(a[1] , a[4]) ;
tmp_2 = shadd(a[2] , a[5]) ;
tmp_8 = shadd( decq(tmp_0,1)   , shadd(tmp_1 , tmp_2) ) ;
res[0] = tmp_8;
tmp_3 = shsub(a[0] , a[3]) ;
tmp_4 = shsub(a[2] , a[5]) ;
tmp_5 = (C0_6_notw) * tmp_4 ;
tmp_6 = shsub(a[1] , a[4]) ;
tmp_7 = (C2_6_notw) * tmp_6 ;
tmp_9 = ( widen<complex<Q15>>::run(tmp_3)   + (tmp_5 + tmp_7) ) ;
res[1] =  narrow<2,complex<Q15>>::run(tmp_9)  ;
tmp_10 = ( widen<complex<Q15>>::run(tmp_0)   + MAC((C0_6_notw) * tmp_1  , (C1_6_notw),tmp_2) ) ;
res[2] =  narrow<2,complex<Q15>>::run(tmp_10)  ;
tmp_11 = ( widen<complex<Q15>>::run(tmp_3)   + MAC((C0_6_notw) *  narrow<NORMALNARROW,complex<Q15>>::run(tmp_7)    , (C1_6_notw), narrow<NORMALNARROW,complex<Q15>>::run(tmp_5)  ) ) ;
res[3] =  narrow<2,complex<Q15>>::run(tmp_11)  ;
tmp_12 = ( widen<complex<Q15>>::run(tmp_0)   + MAC((C0_6_notw) * tmp_2  , (C1_6_notw),tmp_1) ) ;
res[4] =  narrow<2,complex<Q15>>::run(tmp_12)  ;
tmp_13 = ( widen<complex<Q15>>::run(tmp_3)   + MAC((C0_6_notw) *  narrow<NORMALNARROW,complex<Q15>>::run(tmp_5)    , (C1_6_notw), narrow<NORMALNARROW,complex<Q15>>::run(tmp_7)  ) ) ;
res[5] =  narrow<2,complex<Q15>>::run(tmp_13)  ;


memory<complex<Q15>>::store(in,0 * strideFactor , res[0]);
memory<complex<Q15>>::store(in,1 * strideFactor , res[1]);
memory<complex<Q15>>::store(in,2 * strideFactor , res[2]);
memory<complex<Q15>>::store(in,3 * strideFactor , res[3]);
memory<complex<Q15>>::store(in,4 * strideFactor , res[4]);
memory<complex<Q15>>::store(in,5 * strideFactor , res[5]);


};

/* Nb Multiplications = 10, 
Nb Additions = 18,
Nb Negates = 3,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 3

*/
 __STATIC_FORCEINLINE void run_op_last(const complex<Q15> *in,complex<Q15> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[6];
complex<Q15> res[6];


 complex<Q15> tmp_0;
 complex<Q15> tmp_1;
 complex<Q15> tmp_2;
 complex<Q15> tmp_3;
 complex<Q15> tmp_4;
 complex<Q31> tmp_5;
 complex<Q15> tmp_6;
 complex<Q31> tmp_7;
 complex<Q15> tmp_8;
 complex<Q31> tmp_9;
 complex<Q31> tmp_10;
 complex<Q31> tmp_11;
 complex<Q31> tmp_12;
 complex<Q31> tmp_13;


a[0] = memory<complex<Q15>>::load(in,0 * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * strideFactor);
a[4] = memory<complex<Q15>>::load(in,4 * strideFactor);
a[5] = memory<complex<Q15>>::load(in,5 * strideFactor);


tmp_0 = shadd(a[0] , a[3]) ;
tmp_1 = shadd(a[1] , a[4]) ;
tmp_2 = shadd(a[2] , a[5]) ;
tmp_8 = shadd( decq(tmp_0,1)   , shadd(tmp_1 , tmp_2) ) ;
res[0] = tmp_8;
tmp_3 = shsub(a[0] , a[3]) ;
tmp_4 = shsub(a[2] , a[5]) ;
tmp_5 = (C0_6_notw) * tmp_4 ;
tmp_6 = shsub(a[1] , a[4]) ;
tmp_7 = (C2_6_notw) * tmp_6 ;
tmp_9 = ( widen<complex<Q15>>::run(tmp_3)   + (tmp_5 + tmp_7) ) ;
res[1] =  narrow<2,complex<Q15>>::run(tmp_9)  ;
tmp_10 = ( widen<complex<Q15>>::run(tmp_0)   + MAC((C0_6_notw) * tmp_1  , (C1_6_notw),tmp_2) ) ;
res[2] =  narrow<2,complex<Q15>>::run(tmp_10)  ;
tmp_11 = ( widen<complex<Q15>>::run(tmp_3)   + MAC((C0_6_notw) *  narrow<NORMALNARROW,complex<Q15>>::run(tmp_7)    , (C1_6_notw), narrow<NORMALNARROW,complex<Q15>>::run(tmp_5)  ) ) ;
res[3] =  narrow<2,complex<Q15>>::run(tmp_11)  ;
tmp_12 = ( widen<complex<Q15>>::run(tmp_0)   + MAC((C0_6_notw) * tmp_2  , (C1_6_notw),tmp_1) ) ;
res[4] =  narrow<2,complex<Q15>>::run(tmp_12)  ;
tmp_13 = ( widen<complex<Q15>>::run(tmp_3)   + MAC((C0_6_notw) *  narrow<NORMALNARROW,complex<Q15>>::run(tmp_5)    , (C1_6_notw), narrow<NORMALNARROW,complex<Q15>>::run(tmp_7)  ) ) ;
res[5] =  narrow<2,complex<Q15>>::run(tmp_13)  ;


memory<complex<Q15>>::store(out,0 * strideFactor , res[0]);
memory<complex<Q15>>::store(out,1 * strideFactor , res[1]);
memory<complex<Q15>>::store(out,2 * strideFactor , res[2]);
memory<complex<Q15>>::store(out,3 * strideFactor , res[3]);
memory<complex<Q15>>::store(out,4 * strideFactor , res[4]);
memory<complex<Q15>>::store(out,5 * strideFactor , res[5]);


};

};

/**************

INVERSE FFT 

***************/
template<int inputstrideenabled> 
struct radix<INVERSE,Q15, 6,inputstrideenabled>{

static constexpr complex<Q15> C0_6_tw{Q15{(q15_t)0xC000}, Q15{(q15_t)0x9126}};
  static constexpr complex<Q15> C1_6_tw{Q15{(q15_t)0x4000}, Q15{(q15_t)0x6EDA}};
  static constexpr complex<Q15> C2_6_tw{Q15{(q15_t)0xC000}, Q15{(q15_t)0x6EDA}};
  
static constexpr complex<Q15> C0_6_notw{Q15{(q15_t)0xC000}, Q15{(q15_t)0x9126}};
  static constexpr complex<Q15> C1_6_notw{Q15{(q15_t)0x4000}, Q15{(q15_t)0x6EDA}};
  static constexpr complex<Q15> C2_6_notw{Q15{(q15_t)0xC000}, Q15{(q15_t)0x6EDA}};
  

/* Nb Multiplications = 10, 
Nb Additions = 18,
Nb Negates = 3,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 3

*/
 __STATIC_FORCEINLINE void run_ip_notw(int stageLength,complex<Q15> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[6];
complex<Q15> res[6];


 complex<Q15> tmp_0;
 complex<Q15> tmp_1;
 complex<Q15> tmp_2;
 complex<Q15> tmp_3;
 complex<Q15> tmp_4;
 complex<Q31> tmp_5;
 complex<Q15> tmp_6;
 complex<Q31> tmp_7;
 complex<Q15> tmp_8;
 complex<Q31> tmp_9;
 complex<Q31> tmp_10;
 complex<Q31> tmp_11;
 complex<Q31> tmp_12;
 complex<Q31> tmp_13;


a[0] = memory<complex<Q15>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<Q15>>::load(in,4 * stageLength * strideFactor);
a[5] = memory<complex<Q15>>::load(in,5 * stageLength * strideFactor);


tmp_0 = shadd(a[0] , a[3]) ;
tmp_1 = shadd(a[1] , a[4]) ;
tmp_2 = shadd(a[2] , a[5]) ;
tmp_8 = shadd( decq(tmp_0,1)   , shadd(tmp_1 , tmp_2) ) ;
res[0] = tmp_8;
tmp_3 = shsub(a[0] , a[3]) ;
tmp_4 = shsub(a[2] , a[5]) ;
tmp_5 = (C2_6_notw) * tmp_4 ;
tmp_6 = shsub(a[1] , a[4]) ;
tmp_7 = (C1_6_notw) * tmp_6 ;
tmp_9 = ( widen<complex<Q15>>::run(tmp_3)   + (tmp_5 + tmp_7) ) ;
res[1] =  narrow<2,complex<Q15>>::run(tmp_9)  ;
tmp_10 = ( widen<complex<Q15>>::run(tmp_0)   + MAC((C0_6_notw) * tmp_2  , (C2_6_notw),tmp_1) ) ;
res[2] =  narrow<2,complex<Q15>>::run(tmp_10)  ;
tmp_11 = ( widen<complex<Q15>>::run(tmp_3)   + MAC((C0_6_notw) *  narrow<NORMALNARROW,complex<Q15>>::run(tmp_5)    , (C2_6_notw), narrow<NORMALNARROW,complex<Q15>>::run(tmp_7)  ) ) ;
res[3] =  narrow<2,complex<Q15>>::run(tmp_11)  ;
tmp_12 = ( widen<complex<Q15>>::run(tmp_0)   + MAC((C0_6_notw) * tmp_1  , (C2_6_notw),tmp_2) ) ;
res[4] =  narrow<2,complex<Q15>>::run(tmp_12)  ;
tmp_13 = ( widen<complex<Q15>>::run(tmp_3)   + MAC((C0_6_notw) *  narrow<NORMALNARROW,complex<Q15>>::run(tmp_7)    , (C2_6_notw), narrow<NORMALNARROW,complex<Q15>>::run(tmp_5)  ) ) ;
res[5] =  narrow<2,complex<Q15>>::run(tmp_13)  ;


memory<complex<Q15>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<Q15>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<Q15>>::store(in,2 * stageLength * strideFactor , res[2]);
memory<complex<Q15>>::store(in,3 * stageLength * strideFactor , res[3]);
memory<complex<Q15>>::store(in,4 * stageLength * strideFactor , res[4]);
memory<complex<Q15>>::store(in,5 * stageLength * strideFactor , res[5]);


};

/* Nb Multiplications = 15, 
Nb Additions = 18,
Nb Negates = 3,
Nb Conjugates = 5,
*/
/*

Output format : Shifted right by 3

*/
 __STATIC_FORCEINLINE void run_ip_tw(int stageLength,const complex<Q15> *w,complex<Q15> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[6];
complex<Q15> res[6];


 complex<Q15> tmp_0;
 complex<Q15> tmp_1;
 complex<Q15> tmp_2;
 complex<Q15> tmp_3;
 complex<Q15> tmp_4;
 complex<Q31> tmp_5;
 complex<Q15> tmp_6;
 complex<Q31> tmp_7;
 complex<Q15> tmp_8;
 complex<Q31> tmp_9;
 complex<Q31> tmp_10;
 complex<Q31> tmp_11;
 complex<Q31> tmp_12;
 complex<Q31> tmp_13;


a[0] = memory<complex<Q15>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<Q15>>::load(in,4 * stageLength * strideFactor);
a[5] = memory<complex<Q15>>::load(in,5 * stageLength * strideFactor);


tmp_0 = shadd(a[0] , a[3]) ;
tmp_1 = shadd(a[1] , a[4]) ;
tmp_2 = shadd(a[2] , a[5]) ;
tmp_8 = shadd( decq(tmp_0,1)   , shadd(tmp_1 , tmp_2) ) ;
res[0] = tmp_8;
tmp_3 = shsub(a[0] , a[3]) ;
tmp_4 = shsub(a[2] , a[5]) ;
tmp_5 = (C2_6_tw) * tmp_4 ;
tmp_6 = shsub(a[1] , a[4]) ;
tmp_7 = (C1_6_tw) * tmp_6 ;
tmp_9 = ( widen<complex<Q15>>::run(tmp_3)   + (tmp_5 + tmp_7) ) ;
res[1] =  narrow<2,complex<Q15>>::run(multconj( narrow<NORMALNARROW,complex<Q15>>::run(tmp_9)   , w[0]) )  ;
tmp_10 = ( widen<complex<Q15>>::run(tmp_0)   + MAC((C0_6_tw) * tmp_2  , (C2_6_tw),tmp_1) ) ;
res[2] =  narrow<2,complex<Q15>>::run(multconj( narrow<NORMALNARROW,complex<Q15>>::run(tmp_10)   , w[1]) )  ;
tmp_11 = ( widen<complex<Q15>>::run(tmp_3)   + MAC((C0_6_tw) *  narrow<NORMALNARROW,complex<Q15>>::run(tmp_5)    , (C2_6_tw), narrow<NORMALNARROW,complex<Q15>>::run(tmp_7)  ) ) ;
res[3] =  narrow<2,complex<Q15>>::run(multconj( narrow<NORMALNARROW,complex<Q15>>::run(tmp_11)   , w[2]) )  ;
tmp_12 = ( widen<complex<Q15>>::run(tmp_0)   + MAC((C0_6_tw) * tmp_1  , (C2_6_tw),tmp_2) ) ;
res[4] =  narrow<2,complex<Q15>>::run(multconj( narrow<NORMALNARROW,complex<Q15>>::run(tmp_12)   , w[3]) )  ;
tmp_13 = ( widen<complex<Q15>>::run(tmp_3)   + MAC((C0_6_tw) *  narrow<NORMALNARROW,complex<Q15>>::run(tmp_7)    , (C2_6_tw), narrow<NORMALNARROW,complex<Q15>>::run(tmp_5)  ) ) ;
res[5] =  narrow<2,complex<Q15>>::run(multconj( narrow<NORMALNARROW,complex<Q15>>::run(tmp_13)   , w[4]) )  ;


memory<complex<Q15>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<Q15>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<Q15>>::store(in,2 * stageLength * strideFactor , res[2]);
memory<complex<Q15>>::store(in,3 * stageLength * strideFactor , res[3]);
memory<complex<Q15>>::store(in,4 * stageLength * strideFactor , res[4]);
memory<complex<Q15>>::store(in,5 * stageLength * strideFactor , res[5]);


};

/* Nb Multiplications = 10, 
Nb Additions = 18,
Nb Negates = 3,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 3

*/
 __STATIC_FORCEINLINE void run_op_notw(int stageLength,const complex<Q15> *in,complex<Q15> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[6];
complex<Q15> res[6];


 complex<Q15> tmp_0;
 complex<Q15> tmp_1;
 complex<Q15> tmp_2;
 complex<Q15> tmp_3;
 complex<Q15> tmp_4;
 complex<Q31> tmp_5;
 complex<Q15> tmp_6;
 complex<Q31> tmp_7;
 complex<Q15> tmp_8;
 complex<Q31> tmp_9;
 complex<Q31> tmp_10;
 complex<Q31> tmp_11;
 complex<Q31> tmp_12;
 complex<Q31> tmp_13;


a[0] = memory<complex<Q15>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<Q15>>::load(in,4 * stageLength * strideFactor);
a[5] = memory<complex<Q15>>::load(in,5 * stageLength * strideFactor);


tmp_0 = shadd(a[0] , a[3]) ;
tmp_1 = shadd(a[1] , a[4]) ;
tmp_2 = shadd(a[2] , a[5]) ;
tmp_8 = shadd( decq(tmp_0,1)   , shadd(tmp_1 , tmp_2) ) ;
res[0] = tmp_8;
tmp_3 = shsub(a[0] , a[3]) ;
tmp_4 = shsub(a[2] , a[5]) ;
tmp_5 = (C2_6_notw) * tmp_4 ;
tmp_6 = shsub(a[1] , a[4]) ;
tmp_7 = (C1_6_notw) * tmp_6 ;
tmp_9 = ( widen<complex<Q15>>::run(tmp_3)   + (tmp_5 + tmp_7) ) ;
res[1] =  narrow<2,complex<Q15>>::run(tmp_9)  ;
tmp_10 = ( widen<complex<Q15>>::run(tmp_0)   + MAC((C0_6_notw) * tmp_2  , (C2_6_notw),tmp_1) ) ;
res[2] =  narrow<2,complex<Q15>>::run(tmp_10)  ;
tmp_11 = ( widen<complex<Q15>>::run(tmp_3)   + MAC((C0_6_notw) *  narrow<NORMALNARROW,complex<Q15>>::run(tmp_5)    , (C2_6_notw), narrow<NORMALNARROW,complex<Q15>>::run(tmp_7)  ) ) ;
res[3] =  narrow<2,complex<Q15>>::run(tmp_11)  ;
tmp_12 = ( widen<complex<Q15>>::run(tmp_0)   + MAC((C0_6_notw) * tmp_1  , (C2_6_notw),tmp_2) ) ;
res[4] =  narrow<2,complex<Q15>>::run(tmp_12)  ;
tmp_13 = ( widen<complex<Q15>>::run(tmp_3)   + MAC((C0_6_notw) *  narrow<NORMALNARROW,complex<Q15>>::run(tmp_7)    , (C2_6_notw), narrow<NORMALNARROW,complex<Q15>>::run(tmp_5)  ) ) ;
res[5] =  narrow<2,complex<Q15>>::run(tmp_13)  ;


memory<complex<Q15>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<Q15>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<Q15>>::store(out,2 * stageLength * strideFactor , res[2]);
memory<complex<Q15>>::store(out,3 * stageLength * strideFactor , res[3]);
memory<complex<Q15>>::store(out,4 * stageLength * strideFactor , res[4]);
memory<complex<Q15>>::store(out,5 * stageLength * strideFactor , res[5]);


};

/* Nb Multiplications = 15, 
Nb Additions = 18,
Nb Negates = 3,
Nb Conjugates = 5,
*/
/*

Output format : Shifted right by 3

*/
 __STATIC_FORCEINLINE void run_op_tw(int stageLength,const complex<Q15> *w,const complex<Q15> *in,complex<Q15> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[6];
complex<Q15> res[6];


 complex<Q15> tmp_0;
 complex<Q15> tmp_1;
 complex<Q15> tmp_2;
 complex<Q15> tmp_3;
 complex<Q15> tmp_4;
 complex<Q31> tmp_5;
 complex<Q15> tmp_6;
 complex<Q31> tmp_7;
 complex<Q15> tmp_8;
 complex<Q31> tmp_9;
 complex<Q31> tmp_10;
 complex<Q31> tmp_11;
 complex<Q31> tmp_12;
 complex<Q31> tmp_13;


a[0] = memory<complex<Q15>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<Q15>>::load(in,4 * stageLength * strideFactor);
a[5] = memory<complex<Q15>>::load(in,5 * stageLength * strideFactor);


tmp_0 = shadd(a[0] , a[3]) ;
tmp_1 = shadd(a[1] , a[4]) ;
tmp_2 = shadd(a[2] , a[5]) ;
tmp_8 = shadd( decq(tmp_0,1)   , shadd(tmp_1 , tmp_2) ) ;
res[0] = tmp_8;
tmp_3 = shsub(a[0] , a[3]) ;
tmp_4 = shsub(a[2] , a[5]) ;
tmp_5 = (C2_6_tw) * tmp_4 ;
tmp_6 = shsub(a[1] , a[4]) ;
tmp_7 = (C1_6_tw) * tmp_6 ;
tmp_9 = ( widen<complex<Q15>>::run(tmp_3)   + (tmp_5 + tmp_7) ) ;
res[1] =  narrow<2,complex<Q15>>::run(multconj( narrow<NORMALNARROW,complex<Q15>>::run(tmp_9)   , w[0]) )  ;
tmp_10 = ( widen<complex<Q15>>::run(tmp_0)   + MAC((C0_6_tw) * tmp_2  , (C2_6_tw),tmp_1) ) ;
res[2] =  narrow<2,complex<Q15>>::run(multconj( narrow<NORMALNARROW,complex<Q15>>::run(tmp_10)   , w[1]) )  ;
tmp_11 = ( widen<complex<Q15>>::run(tmp_3)   + MAC((C0_6_tw) *  narrow<NORMALNARROW,complex<Q15>>::run(tmp_5)    , (C2_6_tw), narrow<NORMALNARROW,complex<Q15>>::run(tmp_7)  ) ) ;
res[3] =  narrow<2,complex<Q15>>::run(multconj( narrow<NORMALNARROW,complex<Q15>>::run(tmp_11)   , w[2]) )  ;
tmp_12 = ( widen<complex<Q15>>::run(tmp_0)   + MAC((C0_6_tw) * tmp_1  , (C2_6_tw),tmp_2) ) ;
res[4] =  narrow<2,complex<Q15>>::run(multconj( narrow<NORMALNARROW,complex<Q15>>::run(tmp_12)   , w[3]) )  ;
tmp_13 = ( widen<complex<Q15>>::run(tmp_3)   + MAC((C0_6_tw) *  narrow<NORMALNARROW,complex<Q15>>::run(tmp_7)    , (C2_6_tw), narrow<NORMALNARROW,complex<Q15>>::run(tmp_5)  ) ) ;
res[5] =  narrow<2,complex<Q15>>::run(multconj( narrow<NORMALNARROW,complex<Q15>>::run(tmp_13)   , w[4]) )  ;


memory<complex<Q15>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<Q15>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<Q15>>::store(out,2 * stageLength * strideFactor , res[2]);
memory<complex<Q15>>::store(out,3 * stageLength * strideFactor , res[3]);
memory<complex<Q15>>::store(out,4 * stageLength * strideFactor , res[4]);
memory<complex<Q15>>::store(out,5 * stageLength * strideFactor , res[5]);


};

/* Nb Multiplications = 10, 
Nb Additions = 18,
Nb Negates = 3,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 3

*/
 __STATIC_FORCEINLINE void run_ip_last(complex<Q15> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[6];
complex<Q15> res[6];


 complex<Q15> tmp_0;
 complex<Q15> tmp_1;
 complex<Q15> tmp_2;
 complex<Q15> tmp_3;
 complex<Q15> tmp_4;
 complex<Q31> tmp_5;
 complex<Q15> tmp_6;
 complex<Q31> tmp_7;
 complex<Q15> tmp_8;
 complex<Q31> tmp_9;
 complex<Q31> tmp_10;
 complex<Q31> tmp_11;
 complex<Q31> tmp_12;
 complex<Q31> tmp_13;


a[0] = memory<complex<Q15>>::load(in,0 * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * strideFactor);
a[4] = memory<complex<Q15>>::load(in,4 * strideFactor);
a[5] = memory<complex<Q15>>::load(in,5 * strideFactor);


tmp_0 = shadd(a[0] , a[3]) ;
tmp_1 = shadd(a[1] , a[4]) ;
tmp_2 = shadd(a[2] , a[5]) ;
tmp_8 = shadd( decq(tmp_0,1)   , shadd(tmp_1 , tmp_2) ) ;
res[0] = tmp_8;
tmp_3 = shsub(a[0] , a[3]) ;
tmp_4 = shsub(a[2] , a[5]) ;
tmp_5 = (C2_6_notw) * tmp_4 ;
tmp_6 = shsub(a[1] , a[4]) ;
tmp_7 = (C1_6_notw) * tmp_6 ;
tmp_9 = ( widen<complex<Q15>>::run(tmp_3)   + (tmp_5 + tmp_7) ) ;
res[1] =  narrow<2,complex<Q15>>::run(tmp_9)  ;
tmp_10 = ( widen<complex<Q15>>::run(tmp_0)   + MAC((C0_6_notw) * tmp_2  , (C2_6_notw),tmp_1) ) ;
res[2] =  narrow<2,complex<Q15>>::run(tmp_10)  ;
tmp_11 = ( widen<complex<Q15>>::run(tmp_3)   + MAC((C0_6_notw) *  narrow<NORMALNARROW,complex<Q15>>::run(tmp_5)    , (C2_6_notw), narrow<NORMALNARROW,complex<Q15>>::run(tmp_7)  ) ) ;
res[3] =  narrow<2,complex<Q15>>::run(tmp_11)  ;
tmp_12 = ( widen<complex<Q15>>::run(tmp_0)   + MAC((C0_6_notw) * tmp_1  , (C2_6_notw),tmp_2) ) ;
res[4] =  narrow<2,complex<Q15>>::run(tmp_12)  ;
tmp_13 = ( widen<complex<Q15>>::run(tmp_3)   + MAC((C0_6_notw) *  narrow<NORMALNARROW,complex<Q15>>::run(tmp_7)    , (C2_6_notw), narrow<NORMALNARROW,complex<Q15>>::run(tmp_5)  ) ) ;
res[5] =  narrow<2,complex<Q15>>::run(tmp_13)  ;


memory<complex<Q15>>::store(in,0 * strideFactor , res[0]);
memory<complex<Q15>>::store(in,1 * strideFactor , res[1]);
memory<complex<Q15>>::store(in,2 * strideFactor , res[2]);
memory<complex<Q15>>::store(in,3 * strideFactor , res[3]);
memory<complex<Q15>>::store(in,4 * strideFactor , res[4]);
memory<complex<Q15>>::store(in,5 * strideFactor , res[5]);


};

/* Nb Multiplications = 10, 
Nb Additions = 18,
Nb Negates = 3,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 3

*/
 __STATIC_FORCEINLINE void run_op_last(const complex<Q15> *in,complex<Q15> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[6];
complex<Q15> res[6];


 complex<Q15> tmp_0;
 complex<Q15> tmp_1;
 complex<Q15> tmp_2;
 complex<Q15> tmp_3;
 complex<Q15> tmp_4;
 complex<Q31> tmp_5;
 complex<Q15> tmp_6;
 complex<Q31> tmp_7;
 complex<Q15> tmp_8;
 complex<Q31> tmp_9;
 complex<Q31> tmp_10;
 complex<Q31> tmp_11;
 complex<Q31> tmp_12;
 complex<Q31> tmp_13;


a[0] = memory<complex<Q15>>::load(in,0 * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * strideFactor);
a[4] = memory<complex<Q15>>::load(in,4 * strideFactor);
a[5] = memory<complex<Q15>>::load(in,5 * strideFactor);


tmp_0 = shadd(a[0] , a[3]) ;
tmp_1 = shadd(a[1] , a[4]) ;
tmp_2 = shadd(a[2] , a[5]) ;
tmp_8 = shadd( decq(tmp_0,1)   , shadd(tmp_1 , tmp_2) ) ;
res[0] = tmp_8;
tmp_3 = shsub(a[0] , a[3]) ;
tmp_4 = shsub(a[2] , a[5]) ;
tmp_5 = (C2_6_notw) * tmp_4 ;
tmp_6 = shsub(a[1] , a[4]) ;
tmp_7 = (C1_6_notw) * tmp_6 ;
tmp_9 = ( widen<complex<Q15>>::run(tmp_3)   + (tmp_5 + tmp_7) ) ;
res[1] =  narrow<2,complex<Q15>>::run(tmp_9)  ;
tmp_10 = ( widen<complex<Q15>>::run(tmp_0)   + MAC((C0_6_notw) * tmp_2  , (C2_6_notw),tmp_1) ) ;
res[2] =  narrow<2,complex<Q15>>::run(tmp_10)  ;
tmp_11 = ( widen<complex<Q15>>::run(tmp_3)   + MAC((C0_6_notw) *  narrow<NORMALNARROW,complex<Q15>>::run(tmp_5)    , (C2_6_notw), narrow<NORMALNARROW,complex<Q15>>::run(tmp_7)  ) ) ;
res[3] =  narrow<2,complex<Q15>>::run(tmp_11)  ;
tmp_12 = ( widen<complex<Q15>>::run(tmp_0)   + MAC((C0_6_notw) * tmp_1  , (C2_6_notw),tmp_2) ) ;
res[4] =  narrow<2,complex<Q15>>::run(tmp_12)  ;
tmp_13 = ( widen<complex<Q15>>::run(tmp_3)   + MAC((C0_6_notw) *  narrow<NORMALNARROW,complex<Q15>>::run(tmp_7)    , (C2_6_notw), narrow<NORMALNARROW,complex<Q15>>::run(tmp_5)  ) ) ;
res[5] =  narrow<2,complex<Q15>>::run(tmp_13)  ;


memory<complex<Q15>>::store(out,0 * strideFactor , res[0]);
memory<complex<Q15>>::store(out,1 * strideFactor , res[1]);
memory<complex<Q15>>::store(out,2 * strideFactor , res[2]);
memory<complex<Q15>>::store(out,3 * strideFactor , res[3]);
memory<complex<Q15>>::store(out,4 * strideFactor , res[4]);
memory<complex<Q15>>::store(out,5 * strideFactor , res[5]);


};

};

#endif /* #ifndef _RADIX_Q15_6_H */