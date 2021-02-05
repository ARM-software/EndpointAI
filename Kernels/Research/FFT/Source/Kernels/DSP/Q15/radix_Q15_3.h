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

#ifndef _RADIX_Q15_3_H
#define _RADIX_Q15_3_H

#include "complex.h"

/**************

DIRECT FFT 

***************/
template<int inputstrideenabled> 
struct radix<DIRECT,Q15, 3,inputstrideenabled>{

static constexpr complex<Q15> C0_3_tw{Q15{(q15_t)0xC000}, Q15{(q15_t)0x9126}};
  static constexpr complex<Q15> C1_3_tw{Q15{(q15_t)0xC000}, Q15{(q15_t)0x6EDA}};
  
static constexpr complex<Q15> C0_3_notw{Q15{(q15_t)0xC000}, Q15{(q15_t)0x9126}};
  static constexpr complex<Q15> C1_3_notw{Q15{(q15_t)0xC000}, Q15{(q15_t)0x6EDA}};
  

/* Nb Multiplications = 4, 
Nb Additions = 6,
Nb Negates = 0,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 2

*/
 __STATIC_FORCEINLINE void run_ip_notw(int stageLength,complex<Q15> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[3];
complex<Q15> res[3];


 complex<Q15> tmp_0;
 complex<Q31> tmp_1;
 complex<Q31> tmp_2;


a[0] = memory<complex<Q15>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * stageLength * strideFactor);


tmp_0 = shadd( decq(a[0],1)   , shadd(a[1] , a[2]) ) ;
res[0] = tmp_0;
tmp_1 = MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C1_3_notw),a[2])  , (C0_3_notw),a[1]) ;
res[1] =  narrow<2,complex<Q15>>::run(tmp_1)  ;
tmp_2 = MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C1_3_notw),a[1])  , (C0_3_notw),a[2]) ;
res[2] =  narrow<2,complex<Q15>>::run(tmp_2)  ;


memory<complex<Q15>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<Q15>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<Q15>>::store(in,2 * stageLength * strideFactor , res[2]);


};

/* Nb Multiplications = 6, 
Nb Additions = 6,
Nb Negates = 0,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 2

*/
 __STATIC_FORCEINLINE void run_ip_tw(int stageLength,const complex<Q15> *w,complex<Q15> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[3];
complex<Q15> res[3];


 complex<Q15> tmp_0;
 complex<Q31> tmp_1;
 complex<Q31> tmp_2;


a[0] = memory<complex<Q15>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * stageLength * strideFactor);


tmp_0 = shadd( decq(a[0],1)   , shadd(a[1] , a[2]) ) ;
res[0] = tmp_0;
tmp_1 = MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C1_3_tw),a[2])  , (C0_3_tw),a[1]) ;
res[1] =  narrow<2,complex<Q15>>::run(w[0] *  narrow<NORMALNARROW,complex<Q15>>::run(tmp_1)   )  ;
tmp_2 = MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C1_3_tw),a[1])  , (C0_3_tw),a[2]) ;
res[2] =  narrow<2,complex<Q15>>::run(w[1] *  narrow<NORMALNARROW,complex<Q15>>::run(tmp_2)   )  ;


memory<complex<Q15>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<Q15>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<Q15>>::store(in,2 * stageLength * strideFactor , res[2]);


};

/* Nb Multiplications = 4, 
Nb Additions = 6,
Nb Negates = 0,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 2

*/
 __STATIC_FORCEINLINE void run_op_notw(int stageLength,const complex<Q15> *in,complex<Q15> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[3];
complex<Q15> res[3];


 complex<Q15> tmp_0;
 complex<Q31> tmp_1;
 complex<Q31> tmp_2;


a[0] = memory<complex<Q15>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * stageLength * strideFactor);


tmp_0 = shadd( decq(a[0],1)   , shadd(a[1] , a[2]) ) ;
res[0] = tmp_0;
tmp_1 = MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C1_3_notw),a[2])  , (C0_3_notw),a[1]) ;
res[1] =  narrow<2,complex<Q15>>::run(tmp_1)  ;
tmp_2 = MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C1_3_notw),a[1])  , (C0_3_notw),a[2]) ;
res[2] =  narrow<2,complex<Q15>>::run(tmp_2)  ;


memory<complex<Q15>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<Q15>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<Q15>>::store(out,2 * stageLength * strideFactor , res[2]);


};

/* Nb Multiplications = 6, 
Nb Additions = 6,
Nb Negates = 0,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 2

*/
 __STATIC_FORCEINLINE void run_op_tw(int stageLength,const complex<Q15> *w,const complex<Q15> *in,complex<Q15> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[3];
complex<Q15> res[3];


 complex<Q15> tmp_0;
 complex<Q31> tmp_1;
 complex<Q31> tmp_2;


a[0] = memory<complex<Q15>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * stageLength * strideFactor);


tmp_0 = shadd( decq(a[0],1)   , shadd(a[1] , a[2]) ) ;
res[0] = tmp_0;
tmp_1 = MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C1_3_tw),a[2])  , (C0_3_tw),a[1]) ;
res[1] =  narrow<2,complex<Q15>>::run(w[0] *  narrow<NORMALNARROW,complex<Q15>>::run(tmp_1)   )  ;
tmp_2 = MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C1_3_tw),a[1])  , (C0_3_tw),a[2]) ;
res[2] =  narrow<2,complex<Q15>>::run(w[1] *  narrow<NORMALNARROW,complex<Q15>>::run(tmp_2)   )  ;


memory<complex<Q15>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<Q15>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<Q15>>::store(out,2 * stageLength * strideFactor , res[2]);


};

/* Nb Multiplications = 4, 
Nb Additions = 6,
Nb Negates = 0,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 2

*/
 __STATIC_FORCEINLINE void run_ip_last(complex<Q15> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[3];
complex<Q15> res[3];


 complex<Q15> tmp_0;
 complex<Q31> tmp_1;
 complex<Q31> tmp_2;


a[0] = memory<complex<Q15>>::load(in,0 * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * strideFactor);


tmp_0 = shadd( decq(a[0],1)   , shadd(a[1] , a[2]) ) ;
res[0] = tmp_0;
tmp_1 = MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C1_3_notw),a[2])  , (C0_3_notw),a[1]) ;
res[1] =  narrow<2,complex<Q15>>::run(tmp_1)  ;
tmp_2 = MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C1_3_notw),a[1])  , (C0_3_notw),a[2]) ;
res[2] =  narrow<2,complex<Q15>>::run(tmp_2)  ;


memory<complex<Q15>>::store(in,0 * strideFactor , res[0]);
memory<complex<Q15>>::store(in,1 * strideFactor , res[1]);
memory<complex<Q15>>::store(in,2 * strideFactor , res[2]);


};

/* Nb Multiplications = 4, 
Nb Additions = 6,
Nb Negates = 0,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 2

*/
 __STATIC_FORCEINLINE void run_op_last(const complex<Q15> *in,complex<Q15> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[3];
complex<Q15> res[3];


 complex<Q15> tmp_0;
 complex<Q31> tmp_1;
 complex<Q31> tmp_2;


a[0] = memory<complex<Q15>>::load(in,0 * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * strideFactor);


tmp_0 = shadd( decq(a[0],1)   , shadd(a[1] , a[2]) ) ;
res[0] = tmp_0;
tmp_1 = MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C1_3_notw),a[2])  , (C0_3_notw),a[1]) ;
res[1] =  narrow<2,complex<Q15>>::run(tmp_1)  ;
tmp_2 = MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C1_3_notw),a[1])  , (C0_3_notw),a[2]) ;
res[2] =  narrow<2,complex<Q15>>::run(tmp_2)  ;


memory<complex<Q15>>::store(out,0 * strideFactor , res[0]);
memory<complex<Q15>>::store(out,1 * strideFactor , res[1]);
memory<complex<Q15>>::store(out,2 * strideFactor , res[2]);


};

};

/**************

INVERSE FFT 

***************/
template<int inputstrideenabled> 
struct radix<INVERSE,Q15, 3,inputstrideenabled>{

static constexpr complex<Q15> C0_3_tw{Q15{(q15_t)0xC000}, Q15{(q15_t)0x9126}};
  static constexpr complex<Q15> C1_3_tw{Q15{(q15_t)0xC000}, Q15{(q15_t)0x6EDA}};
  
static constexpr complex<Q15> C0_3_notw{Q15{(q15_t)0xC000}, Q15{(q15_t)0x9126}};
  static constexpr complex<Q15> C1_3_notw{Q15{(q15_t)0xC000}, Q15{(q15_t)0x6EDA}};
  

/* Nb Multiplications = 4, 
Nb Additions = 6,
Nb Negates = 0,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 2

*/
 __STATIC_FORCEINLINE void run_ip_notw(int stageLength,complex<Q15> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[3];
complex<Q15> res[3];


 complex<Q15> tmp_0;
 complex<Q31> tmp_1;
 complex<Q31> tmp_2;


a[0] = memory<complex<Q15>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * stageLength * strideFactor);


tmp_0 = shadd( decq(a[0],1)   , shadd(a[1] , a[2]) ) ;
res[0] = tmp_0;
tmp_1 = MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C1_3_notw),a[1])  , (C0_3_notw),a[2]) ;
res[1] =  narrow<2,complex<Q15>>::run(tmp_1)  ;
tmp_2 = MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C1_3_notw),a[2])  , (C0_3_notw),a[1]) ;
res[2] =  narrow<2,complex<Q15>>::run(tmp_2)  ;


memory<complex<Q15>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<Q15>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<Q15>>::store(in,2 * stageLength * strideFactor , res[2]);


};

/* Nb Multiplications = 6, 
Nb Additions = 6,
Nb Negates = 0,
Nb Conjugates = 2,
*/
/*

Output format : Shifted right by 2

*/
 __STATIC_FORCEINLINE void run_ip_tw(int stageLength,const complex<Q15> *w,complex<Q15> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[3];
complex<Q15> res[3];


 complex<Q15> tmp_0;
 complex<Q31> tmp_1;
 complex<Q31> tmp_2;


a[0] = memory<complex<Q15>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * stageLength * strideFactor);


tmp_0 = shadd( decq(a[0],1)   , shadd(a[1] , a[2]) ) ;
res[0] = tmp_0;
tmp_1 = MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C1_3_tw),a[1])  , (C0_3_tw),a[2]) ;
res[1] =  narrow<2,complex<Q15>>::run(multconj( narrow<NORMALNARROW,complex<Q15>>::run(tmp_1)   , w[0]) )  ;
tmp_2 = MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C1_3_tw),a[2])  , (C0_3_tw),a[1]) ;
res[2] =  narrow<2,complex<Q15>>::run(multconj( narrow<NORMALNARROW,complex<Q15>>::run(tmp_2)   , w[1]) )  ;


memory<complex<Q15>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<Q15>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<Q15>>::store(in,2 * stageLength * strideFactor , res[2]);


};

/* Nb Multiplications = 4, 
Nb Additions = 6,
Nb Negates = 0,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 2

*/
 __STATIC_FORCEINLINE void run_op_notw(int stageLength,const complex<Q15> *in,complex<Q15> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[3];
complex<Q15> res[3];


 complex<Q15> tmp_0;
 complex<Q31> tmp_1;
 complex<Q31> tmp_2;


a[0] = memory<complex<Q15>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * stageLength * strideFactor);


tmp_0 = shadd( decq(a[0],1)   , shadd(a[1] , a[2]) ) ;
res[0] = tmp_0;
tmp_1 = MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C1_3_notw),a[1])  , (C0_3_notw),a[2]) ;
res[1] =  narrow<2,complex<Q15>>::run(tmp_1)  ;
tmp_2 = MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C1_3_notw),a[2])  , (C0_3_notw),a[1]) ;
res[2] =  narrow<2,complex<Q15>>::run(tmp_2)  ;


memory<complex<Q15>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<Q15>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<Q15>>::store(out,2 * stageLength * strideFactor , res[2]);


};

/* Nb Multiplications = 6, 
Nb Additions = 6,
Nb Negates = 0,
Nb Conjugates = 2,
*/
/*

Output format : Shifted right by 2

*/
 __STATIC_FORCEINLINE void run_op_tw(int stageLength,const complex<Q15> *w,const complex<Q15> *in,complex<Q15> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[3];
complex<Q15> res[3];


 complex<Q15> tmp_0;
 complex<Q31> tmp_1;
 complex<Q31> tmp_2;


a[0] = memory<complex<Q15>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * stageLength * strideFactor);


tmp_0 = shadd( decq(a[0],1)   , shadd(a[1] , a[2]) ) ;
res[0] = tmp_0;
tmp_1 = MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C1_3_tw),a[1])  , (C0_3_tw),a[2]) ;
res[1] =  narrow<2,complex<Q15>>::run(multconj( narrow<NORMALNARROW,complex<Q15>>::run(tmp_1)   , w[0]) )  ;
tmp_2 = MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C1_3_tw),a[2])  , (C0_3_tw),a[1]) ;
res[2] =  narrow<2,complex<Q15>>::run(multconj( narrow<NORMALNARROW,complex<Q15>>::run(tmp_2)   , w[1]) )  ;


memory<complex<Q15>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<Q15>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<Q15>>::store(out,2 * stageLength * strideFactor , res[2]);


};

/* Nb Multiplications = 4, 
Nb Additions = 6,
Nb Negates = 0,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 2

*/
 __STATIC_FORCEINLINE void run_ip_last(complex<Q15> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[3];
complex<Q15> res[3];


 complex<Q15> tmp_0;
 complex<Q31> tmp_1;
 complex<Q31> tmp_2;


a[0] = memory<complex<Q15>>::load(in,0 * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * strideFactor);


tmp_0 = shadd( decq(a[0],1)   , shadd(a[1] , a[2]) ) ;
res[0] = tmp_0;
tmp_1 = MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C1_3_notw),a[1])  , (C0_3_notw),a[2]) ;
res[1] =  narrow<2,complex<Q15>>::run(tmp_1)  ;
tmp_2 = MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C1_3_notw),a[2])  , (C0_3_notw),a[1]) ;
res[2] =  narrow<2,complex<Q15>>::run(tmp_2)  ;


memory<complex<Q15>>::store(in,0 * strideFactor , res[0]);
memory<complex<Q15>>::store(in,1 * strideFactor , res[1]);
memory<complex<Q15>>::store(in,2 * strideFactor , res[2]);


};

/* Nb Multiplications = 4, 
Nb Additions = 6,
Nb Negates = 0,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 2

*/
 __STATIC_FORCEINLINE void run_op_last(const complex<Q15> *in,complex<Q15> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[3];
complex<Q15> res[3];


 complex<Q15> tmp_0;
 complex<Q31> tmp_1;
 complex<Q31> tmp_2;


a[0] = memory<complex<Q15>>::load(in,0 * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * strideFactor);


tmp_0 = shadd( decq(a[0],1)   , shadd(a[1] , a[2]) ) ;
res[0] = tmp_0;
tmp_1 = MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C1_3_notw),a[1])  , (C0_3_notw),a[2]) ;
res[1] =  narrow<2,complex<Q15>>::run(tmp_1)  ;
tmp_2 = MAC(MAC( widen<complex<Q15>>::run(a[0])   , (C1_3_notw),a[2])  , (C0_3_notw),a[1]) ;
res[2] =  narrow<2,complex<Q15>>::run(tmp_2)  ;


memory<complex<Q15>>::store(out,0 * strideFactor , res[0]);
memory<complex<Q15>>::store(out,1 * strideFactor , res[1]);
memory<complex<Q15>>::store(out,2 * strideFactor , res[2]);


};

};

#endif /* #ifndef _RADIX_Q15_3_H */