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

#ifndef _RADIX_Q15_8_H
#define _RADIX_Q15_8_H

#include "complex.h"

/**************

DIRECT FFT 

***************/
template<int inputstrideenabled> 
struct radix<DIRECT,Q15, 8,inputstrideenabled>{

static constexpr complex<Q15> C0_8_tw{Q15{(q15_t)0x0000}, Q15{(q15_t)0x7FFF}};
  static constexpr complex<Q15> C1_8_tw{Q15{(q15_t)0x5A82}, Q15{(q15_t)0xA57E}};
  static constexpr complex<Q15> C2_8_tw{Q15{(q15_t)0xA57E}, Q15{(q15_t)0xA57E}};
  
static constexpr complex<Q15> C0_8_notw{Q15{(q15_t)0x0000}, Q15{(q15_t)0x7FFF}};
  static constexpr complex<Q15> C1_8_notw{Q15{(q15_t)0x5A82}, Q15{(q15_t)0xA57E}};
  static constexpr complex<Q15> C2_8_notw{Q15{(q15_t)0xA57E}, Q15{(q15_t)0xA57E}};
  

/* Nb Multiplications = 5, 
Nb Additions = 24,
Nb Negates = 12,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 3

*/
 __STATIC_FORCEINLINE void run_ip_notw(int stageLength,complex<Q15> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[8];
complex<Q15> res[8];


 complex<Q15> tmp_0;
 complex<Q15> tmp_1;
 complex<Q15> tmp_2;
 complex<Q15> tmp_3;
 complex<Q15> tmp_4;
 complex<Q15> tmp_5;
 complex<Q15> tmp_6;
 complex<Q15> tmp_7;
 complex<Q15> tmp_8;
 complex<Q15> tmp_9;
 complex<Q15> tmp_10;
 complex<Q31> tmp_11;
 complex<Q15> tmp_12;
 complex<Q31> tmp_13;
 complex<Q31> tmp_14;
 complex<Q15> tmp_15;
 complex<Q15> tmp_16;
 complex<Q15> tmp_17;
 complex<Q15> tmp_18;
 complex<Q31> tmp_19;
 complex<Q31> tmp_20;


a[0] = memory<complex<Q15>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<Q15>>::load(in,4 * stageLength * strideFactor);
a[5] = memory<complex<Q15>>::load(in,5 * stageLength * strideFactor);
a[6] = memory<complex<Q15>>::load(in,6 * stageLength * strideFactor);
a[7] = memory<complex<Q15>>::load(in,7 * stageLength * strideFactor);


tmp_0 = shadd(a[0] , a[4]) ;
tmp_1 = shadd(a[2] , a[6]) ;
tmp_2 = shadd(tmp_0 , tmp_1) ;
tmp_3 = shadd(a[1] , a[5]) ;
tmp_4 = shadd(a[3] , a[7]) ;
tmp_5 = shadd(tmp_3 , tmp_4) ;
res[0] = shadd(tmp_2 , tmp_5) ;
tmp_6 = shsub(a[0] , a[4]) ;
tmp_7 = shsub(a[2] , a[6]) ;
tmp_8 =  rot(tmp_7)  ;
tmp_9 = shsub(tmp_6 , tmp_8) ;
tmp_10 = shsub(a[3] , a[7]) ;
tmp_11 = (C2_8_notw) * tmp_10 ;
tmp_12 = shsub(a[1] , a[5]) ;
tmp_13 = (C1_8_notw) * tmp_12 ;
tmp_14 = (tmp_11 + tmp_13) ;
res[1] =  narrow<1,complex<Q15>>::run(( widen<complex<Q15>>::run(tmp_9)   +  decq(tmp_14,1)  ) )  ;
tmp_15 = shsub(tmp_0 , tmp_1) ;
tmp_16 = shsub(tmp_3 , tmp_4) ;
tmp_17 =  rot(tmp_16)  ;
res[2] = shsub(tmp_15 , tmp_17) ;
tmp_18 = shadd(tmp_6 , tmp_8) ;
tmp_19 = (tmp_13 - tmp_11) ;
tmp_20 =  rot(tmp_19)  ;
res[3] =  narrow<1,complex<Q15>>::run(( widen<complex<Q15>>::run(tmp_18)   -  decq(tmp_20,1)  ) )  ;
res[4] = shsub(tmp_2 , tmp_5) ;
res[5] =  narrow<1,complex<Q15>>::run(( widen<complex<Q15>>::run(tmp_9)   -  decq(tmp_14,1)  ) )  ;
res[6] = shadd(tmp_15 , tmp_17) ;
res[7] =  narrow<1,complex<Q15>>::run(( widen<complex<Q15>>::run(tmp_18)   +  decq(tmp_20,1)  ) )  ;


memory<complex<Q15>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<Q15>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<Q15>>::store(in,2 * stageLength * strideFactor , res[2]);
memory<complex<Q15>>::store(in,3 * stageLength * strideFactor , res[3]);
memory<complex<Q15>>::store(in,4 * stageLength * strideFactor , res[4]);
memory<complex<Q15>>::store(in,5 * stageLength * strideFactor , res[5]);
memory<complex<Q15>>::store(in,6 * stageLength * strideFactor , res[6]);
memory<complex<Q15>>::store(in,7 * stageLength * strideFactor , res[7]);


};

/* Nb Multiplications = 12, 
Nb Additions = 24,
Nb Negates = 12,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 3

*/
 __STATIC_FORCEINLINE void run_ip_tw(int stageLength,const complex<Q15> *w,complex<Q15> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[8];
complex<Q15> res[8];


 complex<Q15> tmp_0;
 complex<Q15> tmp_1;
 complex<Q15> tmp_2;
 complex<Q15> tmp_3;
 complex<Q15> tmp_4;
 complex<Q15> tmp_5;
 complex<Q15> tmp_6;
 complex<Q15> tmp_7;
 complex<Q15> tmp_8;
 complex<Q15> tmp_9;
 complex<Q15> tmp_10;
 complex<Q31> tmp_11;
 complex<Q15> tmp_12;
 complex<Q31> tmp_13;
 complex<Q31> tmp_14;
 complex<Q15> tmp_15;
 complex<Q15> tmp_16;
 complex<Q15> tmp_17;
 complex<Q15> tmp_18;
 complex<Q31> tmp_19;
 complex<Q31> tmp_20;


a[0] = memory<complex<Q15>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<Q15>>::load(in,4 * stageLength * strideFactor);
a[5] = memory<complex<Q15>>::load(in,5 * stageLength * strideFactor);
a[6] = memory<complex<Q15>>::load(in,6 * stageLength * strideFactor);
a[7] = memory<complex<Q15>>::load(in,7 * stageLength * strideFactor);


tmp_0 = shadd(a[0] , a[4]) ;
tmp_1 = shadd(a[2] , a[6]) ;
tmp_2 = shadd(tmp_0 , tmp_1) ;
tmp_3 = shadd(a[1] , a[5]) ;
tmp_4 = shadd(a[3] , a[7]) ;
tmp_5 = shadd(tmp_3 , tmp_4) ;
res[0] = shadd(tmp_2 , tmp_5) ;
tmp_6 = shsub(a[0] , a[4]) ;
tmp_7 = shsub(a[2] , a[6]) ;
tmp_8 =  rot(tmp_7)  ;
tmp_9 = shsub(tmp_6 , tmp_8) ;
tmp_10 = shsub(a[3] , a[7]) ;
tmp_11 = (C2_8_tw) * tmp_10 ;
tmp_12 = shsub(a[1] , a[5]) ;
tmp_13 = (C1_8_tw) * tmp_12 ;
tmp_14 = (tmp_11 + tmp_13) ;
res[1] =  narrow<1,complex<Q15>>::run(w[0] *  narrow<NORMALNARROW,complex<Q15>>::run(( widen<complex<Q15>>::run(tmp_9)   +  decq(tmp_14,1)  ) )   )  ;
tmp_15 = shsub(tmp_0 , tmp_1) ;
tmp_16 = shsub(tmp_3 , tmp_4) ;
tmp_17 =  rot(tmp_16)  ;
res[2] =  narrow<NORMALNARROW,complex<Q15>>::run(w[1] * shsub(tmp_15 , tmp_17)  )  ;
tmp_18 = shadd(tmp_6 , tmp_8) ;
tmp_19 = (tmp_13 - tmp_11) ;
tmp_20 =  rot(tmp_19)  ;
res[3] =  narrow<1,complex<Q15>>::run(w[2] *  narrow<NORMALNARROW,complex<Q15>>::run(( widen<complex<Q15>>::run(tmp_18)   -  decq(tmp_20,1)  ) )   )  ;
res[4] =  narrow<NORMALNARROW,complex<Q15>>::run(w[3] * shsub(tmp_2 , tmp_5)  )  ;
res[5] =  narrow<1,complex<Q15>>::run(w[4] *  narrow<NORMALNARROW,complex<Q15>>::run(( widen<complex<Q15>>::run(tmp_9)   -  decq(tmp_14,1)  ) )   )  ;
res[6] =  narrow<NORMALNARROW,complex<Q15>>::run(w[5] * shadd(tmp_15 , tmp_17)  )  ;
res[7] =  narrow<1,complex<Q15>>::run(w[6] *  narrow<NORMALNARROW,complex<Q15>>::run(( widen<complex<Q15>>::run(tmp_18)   +  decq(tmp_20,1)  ) )   )  ;


memory<complex<Q15>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<Q15>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<Q15>>::store(in,2 * stageLength * strideFactor , res[2]);
memory<complex<Q15>>::store(in,3 * stageLength * strideFactor , res[3]);
memory<complex<Q15>>::store(in,4 * stageLength * strideFactor , res[4]);
memory<complex<Q15>>::store(in,5 * stageLength * strideFactor , res[5]);
memory<complex<Q15>>::store(in,6 * stageLength * strideFactor , res[6]);
memory<complex<Q15>>::store(in,7 * stageLength * strideFactor , res[7]);


};

/* Nb Multiplications = 5, 
Nb Additions = 24,
Nb Negates = 12,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 3

*/
 __STATIC_FORCEINLINE void run_op_notw(int stageLength,const complex<Q15> *in,complex<Q15> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[8];
complex<Q15> res[8];


 complex<Q15> tmp_0;
 complex<Q15> tmp_1;
 complex<Q15> tmp_2;
 complex<Q15> tmp_3;
 complex<Q15> tmp_4;
 complex<Q15> tmp_5;
 complex<Q15> tmp_6;
 complex<Q15> tmp_7;
 complex<Q15> tmp_8;
 complex<Q15> tmp_9;
 complex<Q15> tmp_10;
 complex<Q31> tmp_11;
 complex<Q15> tmp_12;
 complex<Q31> tmp_13;
 complex<Q31> tmp_14;
 complex<Q15> tmp_15;
 complex<Q15> tmp_16;
 complex<Q15> tmp_17;
 complex<Q15> tmp_18;
 complex<Q31> tmp_19;
 complex<Q31> tmp_20;


a[0] = memory<complex<Q15>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<Q15>>::load(in,4 * stageLength * strideFactor);
a[5] = memory<complex<Q15>>::load(in,5 * stageLength * strideFactor);
a[6] = memory<complex<Q15>>::load(in,6 * stageLength * strideFactor);
a[7] = memory<complex<Q15>>::load(in,7 * stageLength * strideFactor);


tmp_0 = shadd(a[0] , a[4]) ;
tmp_1 = shadd(a[2] , a[6]) ;
tmp_2 = shadd(tmp_0 , tmp_1) ;
tmp_3 = shadd(a[1] , a[5]) ;
tmp_4 = shadd(a[3] , a[7]) ;
tmp_5 = shadd(tmp_3 , tmp_4) ;
res[0] = shadd(tmp_2 , tmp_5) ;
tmp_6 = shsub(a[0] , a[4]) ;
tmp_7 = shsub(a[2] , a[6]) ;
tmp_8 =  rot(tmp_7)  ;
tmp_9 = shsub(tmp_6 , tmp_8) ;
tmp_10 = shsub(a[3] , a[7]) ;
tmp_11 = (C2_8_notw) * tmp_10 ;
tmp_12 = shsub(a[1] , a[5]) ;
tmp_13 = (C1_8_notw) * tmp_12 ;
tmp_14 = (tmp_11 + tmp_13) ;
res[1] =  narrow<1,complex<Q15>>::run(( widen<complex<Q15>>::run(tmp_9)   +  decq(tmp_14,1)  ) )  ;
tmp_15 = shsub(tmp_0 , tmp_1) ;
tmp_16 = shsub(tmp_3 , tmp_4) ;
tmp_17 =  rot(tmp_16)  ;
res[2] = shsub(tmp_15 , tmp_17) ;
tmp_18 = shadd(tmp_6 , tmp_8) ;
tmp_19 = (tmp_13 - tmp_11) ;
tmp_20 =  rot(tmp_19)  ;
res[3] =  narrow<1,complex<Q15>>::run(( widen<complex<Q15>>::run(tmp_18)   -  decq(tmp_20,1)  ) )  ;
res[4] = shsub(tmp_2 , tmp_5) ;
res[5] =  narrow<1,complex<Q15>>::run(( widen<complex<Q15>>::run(tmp_9)   -  decq(tmp_14,1)  ) )  ;
res[6] = shadd(tmp_15 , tmp_17) ;
res[7] =  narrow<1,complex<Q15>>::run(( widen<complex<Q15>>::run(tmp_18)   +  decq(tmp_20,1)  ) )  ;


memory<complex<Q15>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<Q15>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<Q15>>::store(out,2 * stageLength * strideFactor , res[2]);
memory<complex<Q15>>::store(out,3 * stageLength * strideFactor , res[3]);
memory<complex<Q15>>::store(out,4 * stageLength * strideFactor , res[4]);
memory<complex<Q15>>::store(out,5 * stageLength * strideFactor , res[5]);
memory<complex<Q15>>::store(out,6 * stageLength * strideFactor , res[6]);
memory<complex<Q15>>::store(out,7 * stageLength * strideFactor , res[7]);


};

/* Nb Multiplications = 12, 
Nb Additions = 24,
Nb Negates = 12,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 3

*/
 __STATIC_FORCEINLINE void run_op_tw(int stageLength,const complex<Q15> *w,const complex<Q15> *in,complex<Q15> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[8];
complex<Q15> res[8];


 complex<Q15> tmp_0;
 complex<Q15> tmp_1;
 complex<Q15> tmp_2;
 complex<Q15> tmp_3;
 complex<Q15> tmp_4;
 complex<Q15> tmp_5;
 complex<Q15> tmp_6;
 complex<Q15> tmp_7;
 complex<Q15> tmp_8;
 complex<Q15> tmp_9;
 complex<Q15> tmp_10;
 complex<Q31> tmp_11;
 complex<Q15> tmp_12;
 complex<Q31> tmp_13;
 complex<Q31> tmp_14;
 complex<Q15> tmp_15;
 complex<Q15> tmp_16;
 complex<Q15> tmp_17;
 complex<Q15> tmp_18;
 complex<Q31> tmp_19;
 complex<Q31> tmp_20;


a[0] = memory<complex<Q15>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<Q15>>::load(in,4 * stageLength * strideFactor);
a[5] = memory<complex<Q15>>::load(in,5 * stageLength * strideFactor);
a[6] = memory<complex<Q15>>::load(in,6 * stageLength * strideFactor);
a[7] = memory<complex<Q15>>::load(in,7 * stageLength * strideFactor);


tmp_0 = shadd(a[0] , a[4]) ;
tmp_1 = shadd(a[2] , a[6]) ;
tmp_2 = shadd(tmp_0 , tmp_1) ;
tmp_3 = shadd(a[1] , a[5]) ;
tmp_4 = shadd(a[3] , a[7]) ;
tmp_5 = shadd(tmp_3 , tmp_4) ;
res[0] = shadd(tmp_2 , tmp_5) ;
tmp_6 = shsub(a[0] , a[4]) ;
tmp_7 = shsub(a[2] , a[6]) ;
tmp_8 =  rot(tmp_7)  ;
tmp_9 = shsub(tmp_6 , tmp_8) ;
tmp_10 = shsub(a[3] , a[7]) ;
tmp_11 = (C2_8_tw) * tmp_10 ;
tmp_12 = shsub(a[1] , a[5]) ;
tmp_13 = (C1_8_tw) * tmp_12 ;
tmp_14 = (tmp_11 + tmp_13) ;
res[1] =  narrow<1,complex<Q15>>::run(w[0] *  narrow<NORMALNARROW,complex<Q15>>::run(( widen<complex<Q15>>::run(tmp_9)   +  decq(tmp_14,1)  ) )   )  ;
tmp_15 = shsub(tmp_0 , tmp_1) ;
tmp_16 = shsub(tmp_3 , tmp_4) ;
tmp_17 =  rot(tmp_16)  ;
res[2] =  narrow<NORMALNARROW,complex<Q15>>::run(w[1] * shsub(tmp_15 , tmp_17)  )  ;
tmp_18 = shadd(tmp_6 , tmp_8) ;
tmp_19 = (tmp_13 - tmp_11) ;
tmp_20 =  rot(tmp_19)  ;
res[3] =  narrow<1,complex<Q15>>::run(w[2] *  narrow<NORMALNARROW,complex<Q15>>::run(( widen<complex<Q15>>::run(tmp_18)   -  decq(tmp_20,1)  ) )   )  ;
res[4] =  narrow<NORMALNARROW,complex<Q15>>::run(w[3] * shsub(tmp_2 , tmp_5)  )  ;
res[5] =  narrow<1,complex<Q15>>::run(w[4] *  narrow<NORMALNARROW,complex<Q15>>::run(( widen<complex<Q15>>::run(tmp_9)   -  decq(tmp_14,1)  ) )   )  ;
res[6] =  narrow<NORMALNARROW,complex<Q15>>::run(w[5] * shadd(tmp_15 , tmp_17)  )  ;
res[7] =  narrow<1,complex<Q15>>::run(w[6] *  narrow<NORMALNARROW,complex<Q15>>::run(( widen<complex<Q15>>::run(tmp_18)   +  decq(tmp_20,1)  ) )   )  ;


memory<complex<Q15>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<Q15>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<Q15>>::store(out,2 * stageLength * strideFactor , res[2]);
memory<complex<Q15>>::store(out,3 * stageLength * strideFactor , res[3]);
memory<complex<Q15>>::store(out,4 * stageLength * strideFactor , res[4]);
memory<complex<Q15>>::store(out,5 * stageLength * strideFactor , res[5]);
memory<complex<Q15>>::store(out,6 * stageLength * strideFactor , res[6]);
memory<complex<Q15>>::store(out,7 * stageLength * strideFactor , res[7]);


};

/* Nb Multiplications = 5, 
Nb Additions = 24,
Nb Negates = 12,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 3

*/
 __STATIC_FORCEINLINE void run_ip_last(complex<Q15> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[8];
complex<Q15> res[8];


 complex<Q15> tmp_0;
 complex<Q15> tmp_1;
 complex<Q15> tmp_2;
 complex<Q15> tmp_3;
 complex<Q15> tmp_4;
 complex<Q15> tmp_5;
 complex<Q15> tmp_6;
 complex<Q15> tmp_7;
 complex<Q15> tmp_8;
 complex<Q15> tmp_9;
 complex<Q15> tmp_10;
 complex<Q31> tmp_11;
 complex<Q15> tmp_12;
 complex<Q31> tmp_13;
 complex<Q31> tmp_14;
 complex<Q15> tmp_15;
 complex<Q15> tmp_16;
 complex<Q15> tmp_17;
 complex<Q15> tmp_18;
 complex<Q31> tmp_19;
 complex<Q31> tmp_20;


a[0] = memory<complex<Q15>>::load(in,0 * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * strideFactor);
a[4] = memory<complex<Q15>>::load(in,4 * strideFactor);
a[5] = memory<complex<Q15>>::load(in,5 * strideFactor);
a[6] = memory<complex<Q15>>::load(in,6 * strideFactor);
a[7] = memory<complex<Q15>>::load(in,7 * strideFactor);


tmp_0 = shadd(a[0] , a[4]) ;
tmp_1 = shadd(a[2] , a[6]) ;
tmp_2 = shadd(tmp_0 , tmp_1) ;
tmp_3 = shadd(a[1] , a[5]) ;
tmp_4 = shadd(a[3] , a[7]) ;
tmp_5 = shadd(tmp_3 , tmp_4) ;
res[0] = shadd(tmp_2 , tmp_5) ;
tmp_6 = shsub(a[0] , a[4]) ;
tmp_7 = shsub(a[2] , a[6]) ;
tmp_8 =  rot(tmp_7)  ;
tmp_9 = shsub(tmp_6 , tmp_8) ;
tmp_10 = shsub(a[3] , a[7]) ;
tmp_11 = (C2_8_notw) * tmp_10 ;
tmp_12 = shsub(a[1] , a[5]) ;
tmp_13 = (C1_8_notw) * tmp_12 ;
tmp_14 = (tmp_11 + tmp_13) ;
res[1] =  narrow<1,complex<Q15>>::run(( widen<complex<Q15>>::run(tmp_9)   +  decq(tmp_14,1)  ) )  ;
tmp_15 = shsub(tmp_0 , tmp_1) ;
tmp_16 = shsub(tmp_3 , tmp_4) ;
tmp_17 =  rot(tmp_16)  ;
res[2] = shsub(tmp_15 , tmp_17) ;
tmp_18 = shadd(tmp_6 , tmp_8) ;
tmp_19 = (tmp_13 - tmp_11) ;
tmp_20 =  rot(tmp_19)  ;
res[3] =  narrow<1,complex<Q15>>::run(( widen<complex<Q15>>::run(tmp_18)   -  decq(tmp_20,1)  ) )  ;
res[4] = shsub(tmp_2 , tmp_5) ;
res[5] =  narrow<1,complex<Q15>>::run(( widen<complex<Q15>>::run(tmp_9)   -  decq(tmp_14,1)  ) )  ;
res[6] = shadd(tmp_15 , tmp_17) ;
res[7] =  narrow<1,complex<Q15>>::run(( widen<complex<Q15>>::run(tmp_18)   +  decq(tmp_20,1)  ) )  ;


memory<complex<Q15>>::store(in,0 * strideFactor , res[0]);
memory<complex<Q15>>::store(in,1 * strideFactor , res[1]);
memory<complex<Q15>>::store(in,2 * strideFactor , res[2]);
memory<complex<Q15>>::store(in,3 * strideFactor , res[3]);
memory<complex<Q15>>::store(in,4 * strideFactor , res[4]);
memory<complex<Q15>>::store(in,5 * strideFactor , res[5]);
memory<complex<Q15>>::store(in,6 * strideFactor , res[6]);
memory<complex<Q15>>::store(in,7 * strideFactor , res[7]);


};

/* Nb Multiplications = 5, 
Nb Additions = 24,
Nb Negates = 12,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 3

*/
 __STATIC_FORCEINLINE void run_op_last(const complex<Q15> *in,complex<Q15> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[8];
complex<Q15> res[8];


 complex<Q15> tmp_0;
 complex<Q15> tmp_1;
 complex<Q15> tmp_2;
 complex<Q15> tmp_3;
 complex<Q15> tmp_4;
 complex<Q15> tmp_5;
 complex<Q15> tmp_6;
 complex<Q15> tmp_7;
 complex<Q15> tmp_8;
 complex<Q15> tmp_9;
 complex<Q15> tmp_10;
 complex<Q31> tmp_11;
 complex<Q15> tmp_12;
 complex<Q31> tmp_13;
 complex<Q31> tmp_14;
 complex<Q15> tmp_15;
 complex<Q15> tmp_16;
 complex<Q15> tmp_17;
 complex<Q15> tmp_18;
 complex<Q31> tmp_19;
 complex<Q31> tmp_20;


a[0] = memory<complex<Q15>>::load(in,0 * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * strideFactor);
a[4] = memory<complex<Q15>>::load(in,4 * strideFactor);
a[5] = memory<complex<Q15>>::load(in,5 * strideFactor);
a[6] = memory<complex<Q15>>::load(in,6 * strideFactor);
a[7] = memory<complex<Q15>>::load(in,7 * strideFactor);


tmp_0 = shadd(a[0] , a[4]) ;
tmp_1 = shadd(a[2] , a[6]) ;
tmp_2 = shadd(tmp_0 , tmp_1) ;
tmp_3 = shadd(a[1] , a[5]) ;
tmp_4 = shadd(a[3] , a[7]) ;
tmp_5 = shadd(tmp_3 , tmp_4) ;
res[0] = shadd(tmp_2 , tmp_5) ;
tmp_6 = shsub(a[0] , a[4]) ;
tmp_7 = shsub(a[2] , a[6]) ;
tmp_8 =  rot(tmp_7)  ;
tmp_9 = shsub(tmp_6 , tmp_8) ;
tmp_10 = shsub(a[3] , a[7]) ;
tmp_11 = (C2_8_notw) * tmp_10 ;
tmp_12 = shsub(a[1] , a[5]) ;
tmp_13 = (C1_8_notw) * tmp_12 ;
tmp_14 = (tmp_11 + tmp_13) ;
res[1] =  narrow<1,complex<Q15>>::run(( widen<complex<Q15>>::run(tmp_9)   +  decq(tmp_14,1)  ) )  ;
tmp_15 = shsub(tmp_0 , tmp_1) ;
tmp_16 = shsub(tmp_3 , tmp_4) ;
tmp_17 =  rot(tmp_16)  ;
res[2] = shsub(tmp_15 , tmp_17) ;
tmp_18 = shadd(tmp_6 , tmp_8) ;
tmp_19 = (tmp_13 - tmp_11) ;
tmp_20 =  rot(tmp_19)  ;
res[3] =  narrow<1,complex<Q15>>::run(( widen<complex<Q15>>::run(tmp_18)   -  decq(tmp_20,1)  ) )  ;
res[4] = shsub(tmp_2 , tmp_5) ;
res[5] =  narrow<1,complex<Q15>>::run(( widen<complex<Q15>>::run(tmp_9)   -  decq(tmp_14,1)  ) )  ;
res[6] = shadd(tmp_15 , tmp_17) ;
res[7] =  narrow<1,complex<Q15>>::run(( widen<complex<Q15>>::run(tmp_18)   +  decq(tmp_20,1)  ) )  ;


memory<complex<Q15>>::store(out,0 * strideFactor , res[0]);
memory<complex<Q15>>::store(out,1 * strideFactor , res[1]);
memory<complex<Q15>>::store(out,2 * strideFactor , res[2]);
memory<complex<Q15>>::store(out,3 * strideFactor , res[3]);
memory<complex<Q15>>::store(out,4 * strideFactor , res[4]);
memory<complex<Q15>>::store(out,5 * strideFactor , res[5]);
memory<complex<Q15>>::store(out,6 * strideFactor , res[6]);
memory<complex<Q15>>::store(out,7 * strideFactor , res[7]);


};

};

/**************

INVERSE FFT 

***************/
template<int inputstrideenabled> 
struct radix<INVERSE,Q15, 8,inputstrideenabled>{

static constexpr complex<Q15> C0_8_tw{Q15{(q15_t)0x0000}, Q15{(q15_t)0x7FFF}};
  static constexpr complex<Q15> C1_8_tw{Q15{(q15_t)0x5A82}, Q15{(q15_t)0x5A82}};
  static constexpr complex<Q15> C2_8_tw{Q15{(q15_t)0xA57E}, Q15{(q15_t)0x5A82}};
  
static constexpr complex<Q15> C0_8_notw{Q15{(q15_t)0x0000}, Q15{(q15_t)0x7FFF}};
  static constexpr complex<Q15> C1_8_notw{Q15{(q15_t)0x5A82}, Q15{(q15_t)0x5A82}};
  static constexpr complex<Q15> C2_8_notw{Q15{(q15_t)0xA57E}, Q15{(q15_t)0x5A82}};
  

/* Nb Multiplications = 5, 
Nb Additions = 24,
Nb Negates = 12,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 3

*/
 __STATIC_FORCEINLINE void run_ip_notw(int stageLength,complex<Q15> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[8];
complex<Q15> res[8];


 complex<Q15> tmp_0;
 complex<Q15> tmp_1;
 complex<Q15> tmp_2;
 complex<Q15> tmp_3;
 complex<Q15> tmp_4;
 complex<Q15> tmp_5;
 complex<Q15> tmp_6;
 complex<Q15> tmp_7;
 complex<Q15> tmp_8;
 complex<Q15> tmp_9;
 complex<Q15> tmp_10;
 complex<Q31> tmp_11;
 complex<Q15> tmp_12;
 complex<Q31> tmp_13;
 complex<Q31> tmp_14;
 complex<Q15> tmp_15;
 complex<Q15> tmp_16;
 complex<Q15> tmp_17;
 complex<Q15> tmp_18;
 complex<Q31> tmp_19;
 complex<Q31> tmp_20;


a[0] = memory<complex<Q15>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<Q15>>::load(in,4 * stageLength * strideFactor);
a[5] = memory<complex<Q15>>::load(in,5 * stageLength * strideFactor);
a[6] = memory<complex<Q15>>::load(in,6 * stageLength * strideFactor);
a[7] = memory<complex<Q15>>::load(in,7 * stageLength * strideFactor);


tmp_0 = shadd(a[0] , a[4]) ;
tmp_1 = shadd(a[2] , a[6]) ;
tmp_2 = shadd(tmp_0 , tmp_1) ;
tmp_3 = shadd(a[1] , a[5]) ;
tmp_4 = shadd(a[3] , a[7]) ;
tmp_5 = shadd(tmp_3 , tmp_4) ;
res[0] = shadd(tmp_2 , tmp_5) ;
tmp_6 = shsub(a[0] , a[4]) ;
tmp_7 = shsub(a[2] , a[6]) ;
tmp_8 =  rot(tmp_7)  ;
tmp_9 = shadd(tmp_6 , tmp_8) ;
tmp_10 = shsub(a[3] , a[7]) ;
tmp_11 = (C2_8_notw) * tmp_10 ;
tmp_12 = shsub(a[1] , a[5]) ;
tmp_13 = (C1_8_notw) * tmp_12 ;
tmp_14 = (tmp_11 + tmp_13) ;
res[1] =  narrow<1,complex<Q15>>::run(( widen<complex<Q15>>::run(tmp_9)   +  decq(tmp_14,1)  ) )  ;
tmp_15 = shsub(tmp_0 , tmp_1) ;
tmp_16 = shsub(tmp_3 , tmp_4) ;
tmp_17 =  rot(tmp_16)  ;
res[2] = shadd(tmp_15 , tmp_17) ;
tmp_18 = shsub(tmp_6 , tmp_8) ;
tmp_19 = (tmp_13 - tmp_11) ;
tmp_20 =  rot(tmp_19)  ;
res[3] =  narrow<1,complex<Q15>>::run(( widen<complex<Q15>>::run(tmp_18)   +  decq(tmp_20,1)  ) )  ;
res[4] = shsub(tmp_2 , tmp_5) ;
res[5] =  narrow<1,complex<Q15>>::run(( widen<complex<Q15>>::run(tmp_9)   -  decq(tmp_14,1)  ) )  ;
res[6] = shsub(tmp_15 , tmp_17) ;
res[7] =  narrow<1,complex<Q15>>::run(( widen<complex<Q15>>::run(tmp_18)   -  decq(tmp_20,1)  ) )  ;


memory<complex<Q15>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<Q15>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<Q15>>::store(in,2 * stageLength * strideFactor , res[2]);
memory<complex<Q15>>::store(in,3 * stageLength * strideFactor , res[3]);
memory<complex<Q15>>::store(in,4 * stageLength * strideFactor , res[4]);
memory<complex<Q15>>::store(in,5 * stageLength * strideFactor , res[5]);
memory<complex<Q15>>::store(in,6 * stageLength * strideFactor , res[6]);
memory<complex<Q15>>::store(in,7 * stageLength * strideFactor , res[7]);


};

/* Nb Multiplications = 12, 
Nb Additions = 24,
Nb Negates = 12,
Nb Conjugates = 7,
*/
/*

Output format : Shifted right by 3

*/
 __STATIC_FORCEINLINE void run_ip_tw(int stageLength,const complex<Q15> *w,complex<Q15> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[8];
complex<Q15> res[8];


 complex<Q15> tmp_0;
 complex<Q15> tmp_1;
 complex<Q15> tmp_2;
 complex<Q15> tmp_3;
 complex<Q15> tmp_4;
 complex<Q15> tmp_5;
 complex<Q15> tmp_6;
 complex<Q15> tmp_7;
 complex<Q15> tmp_8;
 complex<Q15> tmp_9;
 complex<Q15> tmp_10;
 complex<Q31> tmp_11;
 complex<Q15> tmp_12;
 complex<Q31> tmp_13;
 complex<Q31> tmp_14;
 complex<Q15> tmp_15;
 complex<Q15> tmp_16;
 complex<Q15> tmp_17;
 complex<Q15> tmp_18;
 complex<Q31> tmp_19;
 complex<Q31> tmp_20;


a[0] = memory<complex<Q15>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<Q15>>::load(in,4 * stageLength * strideFactor);
a[5] = memory<complex<Q15>>::load(in,5 * stageLength * strideFactor);
a[6] = memory<complex<Q15>>::load(in,6 * stageLength * strideFactor);
a[7] = memory<complex<Q15>>::load(in,7 * stageLength * strideFactor);


tmp_0 = shadd(a[0] , a[4]) ;
tmp_1 = shadd(a[2] , a[6]) ;
tmp_2 = shadd(tmp_0 , tmp_1) ;
tmp_3 = shadd(a[1] , a[5]) ;
tmp_4 = shadd(a[3] , a[7]) ;
tmp_5 = shadd(tmp_3 , tmp_4) ;
res[0] = shadd(tmp_2 , tmp_5) ;
tmp_6 = shsub(a[0] , a[4]) ;
tmp_7 = shsub(a[2] , a[6]) ;
tmp_8 =  rot(tmp_7)  ;
tmp_9 = shadd(tmp_6 , tmp_8) ;
tmp_10 = shsub(a[3] , a[7]) ;
tmp_11 = (C2_8_tw) * tmp_10 ;
tmp_12 = shsub(a[1] , a[5]) ;
tmp_13 = (C1_8_tw) * tmp_12 ;
tmp_14 = (tmp_11 + tmp_13) ;
res[1] =  narrow<1,complex<Q15>>::run(multconj( narrow<NORMALNARROW,complex<Q15>>::run(( widen<complex<Q15>>::run(tmp_9)   +  decq(tmp_14,1)  ) )   , w[0]) )  ;
tmp_15 = shsub(tmp_0 , tmp_1) ;
tmp_16 = shsub(tmp_3 , tmp_4) ;
tmp_17 =  rot(tmp_16)  ;
res[2] =  narrow<NORMALNARROW,complex<Q15>>::run(multconj(shadd(tmp_15 , tmp_17)  , w[1]) )  ;
tmp_18 = shsub(tmp_6 , tmp_8) ;
tmp_19 = (tmp_13 - tmp_11) ;
tmp_20 =  rot(tmp_19)  ;
res[3] =  narrow<1,complex<Q15>>::run(multconj( narrow<NORMALNARROW,complex<Q15>>::run(( widen<complex<Q15>>::run(tmp_18)   +  decq(tmp_20,1)  ) )   , w[2]) )  ;
res[4] =  narrow<NORMALNARROW,complex<Q15>>::run(multconj(shsub(tmp_2 , tmp_5)  , w[3]) )  ;
res[5] =  narrow<1,complex<Q15>>::run(multconj( narrow<NORMALNARROW,complex<Q15>>::run(( widen<complex<Q15>>::run(tmp_9)   -  decq(tmp_14,1)  ) )   , w[4]) )  ;
res[6] =  narrow<NORMALNARROW,complex<Q15>>::run(multconj(shsub(tmp_15 , tmp_17)  , w[5]) )  ;
res[7] =  narrow<1,complex<Q15>>::run(multconj( narrow<NORMALNARROW,complex<Q15>>::run(( widen<complex<Q15>>::run(tmp_18)   -  decq(tmp_20,1)  ) )   , w[6]) )  ;


memory<complex<Q15>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<Q15>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<Q15>>::store(in,2 * stageLength * strideFactor , res[2]);
memory<complex<Q15>>::store(in,3 * stageLength * strideFactor , res[3]);
memory<complex<Q15>>::store(in,4 * stageLength * strideFactor , res[4]);
memory<complex<Q15>>::store(in,5 * stageLength * strideFactor , res[5]);
memory<complex<Q15>>::store(in,6 * stageLength * strideFactor , res[6]);
memory<complex<Q15>>::store(in,7 * stageLength * strideFactor , res[7]);


};

/* Nb Multiplications = 5, 
Nb Additions = 24,
Nb Negates = 12,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 3

*/
 __STATIC_FORCEINLINE void run_op_notw(int stageLength,const complex<Q15> *in,complex<Q15> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[8];
complex<Q15> res[8];


 complex<Q15> tmp_0;
 complex<Q15> tmp_1;
 complex<Q15> tmp_2;
 complex<Q15> tmp_3;
 complex<Q15> tmp_4;
 complex<Q15> tmp_5;
 complex<Q15> tmp_6;
 complex<Q15> tmp_7;
 complex<Q15> tmp_8;
 complex<Q15> tmp_9;
 complex<Q15> tmp_10;
 complex<Q31> tmp_11;
 complex<Q15> tmp_12;
 complex<Q31> tmp_13;
 complex<Q31> tmp_14;
 complex<Q15> tmp_15;
 complex<Q15> tmp_16;
 complex<Q15> tmp_17;
 complex<Q15> tmp_18;
 complex<Q31> tmp_19;
 complex<Q31> tmp_20;


a[0] = memory<complex<Q15>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<Q15>>::load(in,4 * stageLength * strideFactor);
a[5] = memory<complex<Q15>>::load(in,5 * stageLength * strideFactor);
a[6] = memory<complex<Q15>>::load(in,6 * stageLength * strideFactor);
a[7] = memory<complex<Q15>>::load(in,7 * stageLength * strideFactor);


tmp_0 = shadd(a[0] , a[4]) ;
tmp_1 = shadd(a[2] , a[6]) ;
tmp_2 = shadd(tmp_0 , tmp_1) ;
tmp_3 = shadd(a[1] , a[5]) ;
tmp_4 = shadd(a[3] , a[7]) ;
tmp_5 = shadd(tmp_3 , tmp_4) ;
res[0] = shadd(tmp_2 , tmp_5) ;
tmp_6 = shsub(a[0] , a[4]) ;
tmp_7 = shsub(a[2] , a[6]) ;
tmp_8 =  rot(tmp_7)  ;
tmp_9 = shadd(tmp_6 , tmp_8) ;
tmp_10 = shsub(a[3] , a[7]) ;
tmp_11 = (C2_8_notw) * tmp_10 ;
tmp_12 = shsub(a[1] , a[5]) ;
tmp_13 = (C1_8_notw) * tmp_12 ;
tmp_14 = (tmp_11 + tmp_13) ;
res[1] =  narrow<1,complex<Q15>>::run(( widen<complex<Q15>>::run(tmp_9)   +  decq(tmp_14,1)  ) )  ;
tmp_15 = shsub(tmp_0 , tmp_1) ;
tmp_16 = shsub(tmp_3 , tmp_4) ;
tmp_17 =  rot(tmp_16)  ;
res[2] = shadd(tmp_15 , tmp_17) ;
tmp_18 = shsub(tmp_6 , tmp_8) ;
tmp_19 = (tmp_13 - tmp_11) ;
tmp_20 =  rot(tmp_19)  ;
res[3] =  narrow<1,complex<Q15>>::run(( widen<complex<Q15>>::run(tmp_18)   +  decq(tmp_20,1)  ) )  ;
res[4] = shsub(tmp_2 , tmp_5) ;
res[5] =  narrow<1,complex<Q15>>::run(( widen<complex<Q15>>::run(tmp_9)   -  decq(tmp_14,1)  ) )  ;
res[6] = shsub(tmp_15 , tmp_17) ;
res[7] =  narrow<1,complex<Q15>>::run(( widen<complex<Q15>>::run(tmp_18)   -  decq(tmp_20,1)  ) )  ;


memory<complex<Q15>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<Q15>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<Q15>>::store(out,2 * stageLength * strideFactor , res[2]);
memory<complex<Q15>>::store(out,3 * stageLength * strideFactor , res[3]);
memory<complex<Q15>>::store(out,4 * stageLength * strideFactor , res[4]);
memory<complex<Q15>>::store(out,5 * stageLength * strideFactor , res[5]);
memory<complex<Q15>>::store(out,6 * stageLength * strideFactor , res[6]);
memory<complex<Q15>>::store(out,7 * stageLength * strideFactor , res[7]);


};

/* Nb Multiplications = 12, 
Nb Additions = 24,
Nb Negates = 12,
Nb Conjugates = 7,
*/
/*

Output format : Shifted right by 3

*/
 __STATIC_FORCEINLINE void run_op_tw(int stageLength,const complex<Q15> *w,const complex<Q15> *in,complex<Q15> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[8];
complex<Q15> res[8];


 complex<Q15> tmp_0;
 complex<Q15> tmp_1;
 complex<Q15> tmp_2;
 complex<Q15> tmp_3;
 complex<Q15> tmp_4;
 complex<Q15> tmp_5;
 complex<Q15> tmp_6;
 complex<Q15> tmp_7;
 complex<Q15> tmp_8;
 complex<Q15> tmp_9;
 complex<Q15> tmp_10;
 complex<Q31> tmp_11;
 complex<Q15> tmp_12;
 complex<Q31> tmp_13;
 complex<Q31> tmp_14;
 complex<Q15> tmp_15;
 complex<Q15> tmp_16;
 complex<Q15> tmp_17;
 complex<Q15> tmp_18;
 complex<Q31> tmp_19;
 complex<Q31> tmp_20;


a[0] = memory<complex<Q15>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<Q15>>::load(in,4 * stageLength * strideFactor);
a[5] = memory<complex<Q15>>::load(in,5 * stageLength * strideFactor);
a[6] = memory<complex<Q15>>::load(in,6 * stageLength * strideFactor);
a[7] = memory<complex<Q15>>::load(in,7 * stageLength * strideFactor);


tmp_0 = shadd(a[0] , a[4]) ;
tmp_1 = shadd(a[2] , a[6]) ;
tmp_2 = shadd(tmp_0 , tmp_1) ;
tmp_3 = shadd(a[1] , a[5]) ;
tmp_4 = shadd(a[3] , a[7]) ;
tmp_5 = shadd(tmp_3 , tmp_4) ;
res[0] = shadd(tmp_2 , tmp_5) ;
tmp_6 = shsub(a[0] , a[4]) ;
tmp_7 = shsub(a[2] , a[6]) ;
tmp_8 =  rot(tmp_7)  ;
tmp_9 = shadd(tmp_6 , tmp_8) ;
tmp_10 = shsub(a[3] , a[7]) ;
tmp_11 = (C2_8_tw) * tmp_10 ;
tmp_12 = shsub(a[1] , a[5]) ;
tmp_13 = (C1_8_tw) * tmp_12 ;
tmp_14 = (tmp_11 + tmp_13) ;
res[1] =  narrow<1,complex<Q15>>::run(multconj( narrow<NORMALNARROW,complex<Q15>>::run(( widen<complex<Q15>>::run(tmp_9)   +  decq(tmp_14,1)  ) )   , w[0]) )  ;
tmp_15 = shsub(tmp_0 , tmp_1) ;
tmp_16 = shsub(tmp_3 , tmp_4) ;
tmp_17 =  rot(tmp_16)  ;
res[2] =  narrow<NORMALNARROW,complex<Q15>>::run(multconj(shadd(tmp_15 , tmp_17)  , w[1]) )  ;
tmp_18 = shsub(tmp_6 , tmp_8) ;
tmp_19 = (tmp_13 - tmp_11) ;
tmp_20 =  rot(tmp_19)  ;
res[3] =  narrow<1,complex<Q15>>::run(multconj( narrow<NORMALNARROW,complex<Q15>>::run(( widen<complex<Q15>>::run(tmp_18)   +  decq(tmp_20,1)  ) )   , w[2]) )  ;
res[4] =  narrow<NORMALNARROW,complex<Q15>>::run(multconj(shsub(tmp_2 , tmp_5)  , w[3]) )  ;
res[5] =  narrow<1,complex<Q15>>::run(multconj( narrow<NORMALNARROW,complex<Q15>>::run(( widen<complex<Q15>>::run(tmp_9)   -  decq(tmp_14,1)  ) )   , w[4]) )  ;
res[6] =  narrow<NORMALNARROW,complex<Q15>>::run(multconj(shsub(tmp_15 , tmp_17)  , w[5]) )  ;
res[7] =  narrow<1,complex<Q15>>::run(multconj( narrow<NORMALNARROW,complex<Q15>>::run(( widen<complex<Q15>>::run(tmp_18)   -  decq(tmp_20,1)  ) )   , w[6]) )  ;


memory<complex<Q15>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<Q15>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<Q15>>::store(out,2 * stageLength * strideFactor , res[2]);
memory<complex<Q15>>::store(out,3 * stageLength * strideFactor , res[3]);
memory<complex<Q15>>::store(out,4 * stageLength * strideFactor , res[4]);
memory<complex<Q15>>::store(out,5 * stageLength * strideFactor , res[5]);
memory<complex<Q15>>::store(out,6 * stageLength * strideFactor , res[6]);
memory<complex<Q15>>::store(out,7 * stageLength * strideFactor , res[7]);


};

/* Nb Multiplications = 5, 
Nb Additions = 24,
Nb Negates = 12,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 3

*/
 __STATIC_FORCEINLINE void run_ip_last(complex<Q15> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[8];
complex<Q15> res[8];


 complex<Q15> tmp_0;
 complex<Q15> tmp_1;
 complex<Q15> tmp_2;
 complex<Q15> tmp_3;
 complex<Q15> tmp_4;
 complex<Q15> tmp_5;
 complex<Q15> tmp_6;
 complex<Q15> tmp_7;
 complex<Q15> tmp_8;
 complex<Q15> tmp_9;
 complex<Q15> tmp_10;
 complex<Q31> tmp_11;
 complex<Q15> tmp_12;
 complex<Q31> tmp_13;
 complex<Q31> tmp_14;
 complex<Q15> tmp_15;
 complex<Q15> tmp_16;
 complex<Q15> tmp_17;
 complex<Q15> tmp_18;
 complex<Q31> tmp_19;
 complex<Q31> tmp_20;


a[0] = memory<complex<Q15>>::load(in,0 * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * strideFactor);
a[4] = memory<complex<Q15>>::load(in,4 * strideFactor);
a[5] = memory<complex<Q15>>::load(in,5 * strideFactor);
a[6] = memory<complex<Q15>>::load(in,6 * strideFactor);
a[7] = memory<complex<Q15>>::load(in,7 * strideFactor);


tmp_0 = shadd(a[0] , a[4]) ;
tmp_1 = shadd(a[2] , a[6]) ;
tmp_2 = shadd(tmp_0 , tmp_1) ;
tmp_3 = shadd(a[1] , a[5]) ;
tmp_4 = shadd(a[3] , a[7]) ;
tmp_5 = shadd(tmp_3 , tmp_4) ;
res[0] = shadd(tmp_2 , tmp_5) ;
tmp_6 = shsub(a[0] , a[4]) ;
tmp_7 = shsub(a[2] , a[6]) ;
tmp_8 =  rot(tmp_7)  ;
tmp_9 = shadd(tmp_6 , tmp_8) ;
tmp_10 = shsub(a[3] , a[7]) ;
tmp_11 = (C2_8_notw) * tmp_10 ;
tmp_12 = shsub(a[1] , a[5]) ;
tmp_13 = (C1_8_notw) * tmp_12 ;
tmp_14 = (tmp_11 + tmp_13) ;
res[1] =  narrow<1,complex<Q15>>::run(( widen<complex<Q15>>::run(tmp_9)   +  decq(tmp_14,1)  ) )  ;
tmp_15 = shsub(tmp_0 , tmp_1) ;
tmp_16 = shsub(tmp_3 , tmp_4) ;
tmp_17 =  rot(tmp_16)  ;
res[2] = shadd(tmp_15 , tmp_17) ;
tmp_18 = shsub(tmp_6 , tmp_8) ;
tmp_19 = (tmp_13 - tmp_11) ;
tmp_20 =  rot(tmp_19)  ;
res[3] =  narrow<1,complex<Q15>>::run(( widen<complex<Q15>>::run(tmp_18)   +  decq(tmp_20,1)  ) )  ;
res[4] = shsub(tmp_2 , tmp_5) ;
res[5] =  narrow<1,complex<Q15>>::run(( widen<complex<Q15>>::run(tmp_9)   -  decq(tmp_14,1)  ) )  ;
res[6] = shsub(tmp_15 , tmp_17) ;
res[7] =  narrow<1,complex<Q15>>::run(( widen<complex<Q15>>::run(tmp_18)   -  decq(tmp_20,1)  ) )  ;


memory<complex<Q15>>::store(in,0 * strideFactor , res[0]);
memory<complex<Q15>>::store(in,1 * strideFactor , res[1]);
memory<complex<Q15>>::store(in,2 * strideFactor , res[2]);
memory<complex<Q15>>::store(in,3 * strideFactor , res[3]);
memory<complex<Q15>>::store(in,4 * strideFactor , res[4]);
memory<complex<Q15>>::store(in,5 * strideFactor , res[5]);
memory<complex<Q15>>::store(in,6 * strideFactor , res[6]);
memory<complex<Q15>>::store(in,7 * strideFactor , res[7]);


};

/* Nb Multiplications = 5, 
Nb Additions = 24,
Nb Negates = 12,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 3

*/
 __STATIC_FORCEINLINE void run_op_last(const complex<Q15> *in,complex<Q15> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[8];
complex<Q15> res[8];


 complex<Q15> tmp_0;
 complex<Q15> tmp_1;
 complex<Q15> tmp_2;
 complex<Q15> tmp_3;
 complex<Q15> tmp_4;
 complex<Q15> tmp_5;
 complex<Q15> tmp_6;
 complex<Q15> tmp_7;
 complex<Q15> tmp_8;
 complex<Q15> tmp_9;
 complex<Q15> tmp_10;
 complex<Q31> tmp_11;
 complex<Q15> tmp_12;
 complex<Q31> tmp_13;
 complex<Q31> tmp_14;
 complex<Q15> tmp_15;
 complex<Q15> tmp_16;
 complex<Q15> tmp_17;
 complex<Q15> tmp_18;
 complex<Q31> tmp_19;
 complex<Q31> tmp_20;


a[0] = memory<complex<Q15>>::load(in,0 * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * strideFactor);
a[4] = memory<complex<Q15>>::load(in,4 * strideFactor);
a[5] = memory<complex<Q15>>::load(in,5 * strideFactor);
a[6] = memory<complex<Q15>>::load(in,6 * strideFactor);
a[7] = memory<complex<Q15>>::load(in,7 * strideFactor);


tmp_0 = shadd(a[0] , a[4]) ;
tmp_1 = shadd(a[2] , a[6]) ;
tmp_2 = shadd(tmp_0 , tmp_1) ;
tmp_3 = shadd(a[1] , a[5]) ;
tmp_4 = shadd(a[3] , a[7]) ;
tmp_5 = shadd(tmp_3 , tmp_4) ;
res[0] = shadd(tmp_2 , tmp_5) ;
tmp_6 = shsub(a[0] , a[4]) ;
tmp_7 = shsub(a[2] , a[6]) ;
tmp_8 =  rot(tmp_7)  ;
tmp_9 = shadd(tmp_6 , tmp_8) ;
tmp_10 = shsub(a[3] , a[7]) ;
tmp_11 = (C2_8_notw) * tmp_10 ;
tmp_12 = shsub(a[1] , a[5]) ;
tmp_13 = (C1_8_notw) * tmp_12 ;
tmp_14 = (tmp_11 + tmp_13) ;
res[1] =  narrow<1,complex<Q15>>::run(( widen<complex<Q15>>::run(tmp_9)   +  decq(tmp_14,1)  ) )  ;
tmp_15 = shsub(tmp_0 , tmp_1) ;
tmp_16 = shsub(tmp_3 , tmp_4) ;
tmp_17 =  rot(tmp_16)  ;
res[2] = shadd(tmp_15 , tmp_17) ;
tmp_18 = shsub(tmp_6 , tmp_8) ;
tmp_19 = (tmp_13 - tmp_11) ;
tmp_20 =  rot(tmp_19)  ;
res[3] =  narrow<1,complex<Q15>>::run(( widen<complex<Q15>>::run(tmp_18)   +  decq(tmp_20,1)  ) )  ;
res[4] = shsub(tmp_2 , tmp_5) ;
res[5] =  narrow<1,complex<Q15>>::run(( widen<complex<Q15>>::run(tmp_9)   -  decq(tmp_14,1)  ) )  ;
res[6] = shsub(tmp_15 , tmp_17) ;
res[7] =  narrow<1,complex<Q15>>::run(( widen<complex<Q15>>::run(tmp_18)   -  decq(tmp_20,1)  ) )  ;


memory<complex<Q15>>::store(out,0 * strideFactor , res[0]);
memory<complex<Q15>>::store(out,1 * strideFactor , res[1]);
memory<complex<Q15>>::store(out,2 * strideFactor , res[2]);
memory<complex<Q15>>::store(out,3 * strideFactor , res[3]);
memory<complex<Q15>>::store(out,4 * strideFactor , res[4]);
memory<complex<Q15>>::store(out,5 * strideFactor , res[5]);
memory<complex<Q15>>::store(out,6 * strideFactor , res[6]);
memory<complex<Q15>>::store(out,7 * strideFactor , res[7]);


};

};

#endif /* #ifndef _RADIX_Q15_8_H */