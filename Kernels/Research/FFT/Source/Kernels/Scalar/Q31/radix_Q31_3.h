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

#ifndef _RADIX_Q31_3_H
#define _RADIX_Q31_3_H

#include "complex.h"

/**************

DIRECT FFT 

***************/
template<int inputstrideenabled> 
struct radix<DIRECT,Q31, 3,inputstrideenabled>{

static constexpr Q31 C0_3_tw{(q31_t)0x6ED9EBA1};
  static constexpr Q31 C1_3_tw{(q31_t)0x40000000};
  
static constexpr Q31 C0_3_notw{(q31_t)0x6ED9EBA1};
  static constexpr Q31 C1_3_notw{(q31_t)0x40000000};
  

/* Nb Multiplications = 6, 
Nb Additions = 18,
Nb Negates = 8,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 2

*/
 __STATIC_FORCEINLINE void run_ip_notw(int stageLength,complex<Q31> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q31> a[3];
complex<Q31> res[3];


 Q31 tmp_0;
 Q63 tmp_1;
 Q31 tmp_2;
 Q63 tmp_3;
 Q31 tmp_4;
 Q63 tmp_5;
 Q63 tmp_6;
 Q31 tmp_7;
 Q63 tmp_8;
 Q63 tmp_9;


a[0] = memory<complex<Q31>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q31>>::load(in,2 * stageLength * strideFactor);


tmp_4 = ( decq( decq(a[0].re,1)  ,1)   +  decq(( decq(a[1].re,1)   +  decq(a[2].re,1)  ) ,1)  ) ;
res[0].re = tmp_4;
tmp_0 = ( decq(a[1].re,1)   +  decq(a[2].re,1)  ) ;
tmp_1 = (C1_3_notw) * tmp_0 ;
tmp_5 = ((C0_3_notw) * ( decq(a[1].im,1)   -  decq(a[2].im,1)  )   + ( decq( widen<Q31>::run(a[0].re)  ,1)   - tmp_1) ) ;
res[1].re =  narrow<1,Q31>::run(tmp_5)  ;
tmp_6 = ((C0_3_notw) * ( decq(a[2].im,1)   -  decq(a[1].im,1)  )   + ( decq( widen<Q31>::run(a[0].re)  ,1)   - tmp_1) ) ;
res[2].re =  narrow<1,Q31>::run(tmp_6)  ;
tmp_7 = ( decq( decq(a[0].im,1)  ,1)   +  decq(( decq(a[1].im,1)   +  decq(a[2].im,1)  ) ,1)  ) ;
res[0].im = tmp_7;
tmp_2 = ( decq(a[1].im,1)   +  decq(a[2].im,1)  ) ;
tmp_3 = (C1_3_notw) * tmp_2 ;
tmp_8 = ((C0_3_notw) * ( decq(a[2].re,1)   -  decq(a[1].re,1)  )   + ( decq( widen<Q31>::run(a[0].im)  ,1)   - tmp_3) ) ;
res[1].im =  narrow<1,Q31>::run(tmp_8)  ;
tmp_9 = ((C0_3_notw) * ( decq(a[1].re,1)   -  decq(a[2].re,1)  )   + ( decq( widen<Q31>::run(a[0].im)  ,1)   - tmp_3) ) ;
res[2].im =  narrow<1,Q31>::run(tmp_9)  ;


memory<complex<Q31>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<Q31>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<Q31>>::store(in,2 * stageLength * strideFactor , res[2]);


};

/* Nb Multiplications = 14, 
Nb Additions = 22,
Nb Negates = 10,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 2

*/
 __STATIC_FORCEINLINE void run_ip_tw(int stageLength,const complex<Q31> *w,complex<Q31> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q31> a[3];
complex<Q31> res[3];


 Q31 tmp_0;
 Q63 tmp_1;
 Q31 tmp_2;
 Q63 tmp_3;
 Q63 tmp_4;
 Q31 tmp_5;
 Q63 tmp_6;
 Q31 tmp_7;
 Q63 tmp_8;
 Q63 tmp_9;
 Q31 tmp_10;
 Q63 tmp_11;
 Q63 tmp_12;
 Q31 tmp_13;
 Q63 tmp_14;
 Q63 tmp_15;
 Q31 tmp_16;
 Q63 tmp_17;
 Q63 tmp_18;
 Q63 tmp_19;
 Q63 tmp_20;
 Q31 tmp_21;


a[0] = memory<complex<Q31>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q31>>::load(in,2 * stageLength * strideFactor);


tmp_16 = ( decq( decq(a[0].re,1)  ,1)   +  decq(( decq(a[1].re,1)   +  decq(a[2].re,1)  ) ,1)  ) ;
res[0].re = tmp_16;
tmp_0 = ( decq(a[1].im,1)   -  decq(a[2].im,1)  ) ;
tmp_1 = (C0_3_tw) * tmp_0 ;
tmp_2 = ( decq(a[1].re,1)   +  decq(a[2].re,1)  ) ;
tmp_3 = (C1_3_tw) * tmp_2 ;
tmp_17 = (tmp_1 + ( decq( widen<Q31>::run(a[0].re)  ,1)   - tmp_3) ) ;
tmp_4 = tmp_17;
tmp_5 = ( decq(a[2].re,1)   -  decq(a[1].re,1)  ) ;
tmp_6 = (C0_3_tw) * tmp_5 ;
tmp_7 = ( decq(a[1].im,1)   +  decq(a[2].im,1)  ) ;
tmp_8 = (C1_3_tw) * tmp_7 ;
tmp_18 = (tmp_6 + ( decq( widen<Q31>::run(a[0].im)  ,1)   - tmp_8) ) ;
tmp_9 = tmp_18;
res[1].re =  narrow<1,Q31>::run((w[0].re *  narrow<NORMALNARROW,Q31>::run(tmp_4)    - w[0].im *  narrow<NORMALNARROW,Q31>::run(tmp_9)   ) )  ;
tmp_10 = ( decq(a[2].im,1)   -  decq(a[1].im,1)  ) ;
tmp_11 = (C0_3_tw) * tmp_10 ;
tmp_19 = (tmp_11 + ( decq( widen<Q31>::run(a[0].re)  ,1)   - tmp_3) ) ;
tmp_12 = tmp_19;
tmp_13 = ( decq(a[1].re,1)   -  decq(a[2].re,1)  ) ;
tmp_14 = (C0_3_tw) * tmp_13 ;
tmp_20 = (tmp_14 + ( decq( widen<Q31>::run(a[0].im)  ,1)   - tmp_8) ) ;
tmp_15 = tmp_20;
res[2].re =  narrow<1,Q31>::run((w[1].re *  narrow<NORMALNARROW,Q31>::run(tmp_12)    - w[1].im *  narrow<NORMALNARROW,Q31>::run(tmp_15)   ) )  ;
tmp_21 = ( decq( decq(a[0].im,1)  ,1)   +  decq(( decq(a[1].im,1)   +  decq(a[2].im,1)  ) ,1)  ) ;
res[0].im = tmp_21;
res[1].im =  narrow<1,Q31>::run((w[0].re *  narrow<NORMALNARROW,Q31>::run(tmp_9)    + w[0].im *  narrow<NORMALNARROW,Q31>::run(tmp_4)   ) )  ;
res[2].im =  narrow<1,Q31>::run((w[1].re *  narrow<NORMALNARROW,Q31>::run(tmp_15)    + w[1].im *  narrow<NORMALNARROW,Q31>::run(tmp_12)   ) )  ;


memory<complex<Q31>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<Q31>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<Q31>>::store(in,2 * stageLength * strideFactor , res[2]);


};

/* Nb Multiplications = 6, 
Nb Additions = 18,
Nb Negates = 8,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 2

*/
 __STATIC_FORCEINLINE void run_op_notw(int stageLength,const complex<Q31> *in,complex<Q31> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q31> a[3];
complex<Q31> res[3];


 Q31 tmp_0;
 Q63 tmp_1;
 Q31 tmp_2;
 Q63 tmp_3;
 Q31 tmp_4;
 Q63 tmp_5;
 Q63 tmp_6;
 Q31 tmp_7;
 Q63 tmp_8;
 Q63 tmp_9;


a[0] = memory<complex<Q31>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q31>>::load(in,2 * stageLength * strideFactor);


tmp_4 = ( decq( decq(a[0].re,1)  ,1)   +  decq(( decq(a[1].re,1)   +  decq(a[2].re,1)  ) ,1)  ) ;
res[0].re = tmp_4;
tmp_0 = ( decq(a[1].re,1)   +  decq(a[2].re,1)  ) ;
tmp_1 = (C1_3_notw) * tmp_0 ;
tmp_5 = ((C0_3_notw) * ( decq(a[1].im,1)   -  decq(a[2].im,1)  )   + ( decq( widen<Q31>::run(a[0].re)  ,1)   - tmp_1) ) ;
res[1].re =  narrow<1,Q31>::run(tmp_5)  ;
tmp_6 = ((C0_3_notw) * ( decq(a[2].im,1)   -  decq(a[1].im,1)  )   + ( decq( widen<Q31>::run(a[0].re)  ,1)   - tmp_1) ) ;
res[2].re =  narrow<1,Q31>::run(tmp_6)  ;
tmp_7 = ( decq( decq(a[0].im,1)  ,1)   +  decq(( decq(a[1].im,1)   +  decq(a[2].im,1)  ) ,1)  ) ;
res[0].im = tmp_7;
tmp_2 = ( decq(a[1].im,1)   +  decq(a[2].im,1)  ) ;
tmp_3 = (C1_3_notw) * tmp_2 ;
tmp_8 = ((C0_3_notw) * ( decq(a[2].re,1)   -  decq(a[1].re,1)  )   + ( decq( widen<Q31>::run(a[0].im)  ,1)   - tmp_3) ) ;
res[1].im =  narrow<1,Q31>::run(tmp_8)  ;
tmp_9 = ((C0_3_notw) * ( decq(a[1].re,1)   -  decq(a[2].re,1)  )   + ( decq( widen<Q31>::run(a[0].im)  ,1)   - tmp_3) ) ;
res[2].im =  narrow<1,Q31>::run(tmp_9)  ;


memory<complex<Q31>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<Q31>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<Q31>>::store(out,2 * stageLength * strideFactor , res[2]);


};

/* Nb Multiplications = 14, 
Nb Additions = 22,
Nb Negates = 10,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 2

*/
 __STATIC_FORCEINLINE void run_op_tw(int stageLength,const complex<Q31> *w,const complex<Q31> *in,complex<Q31> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q31> a[3];
complex<Q31> res[3];


 Q31 tmp_0;
 Q63 tmp_1;
 Q31 tmp_2;
 Q63 tmp_3;
 Q63 tmp_4;
 Q31 tmp_5;
 Q63 tmp_6;
 Q31 tmp_7;
 Q63 tmp_8;
 Q63 tmp_9;
 Q31 tmp_10;
 Q63 tmp_11;
 Q63 tmp_12;
 Q31 tmp_13;
 Q63 tmp_14;
 Q63 tmp_15;
 Q31 tmp_16;
 Q63 tmp_17;
 Q63 tmp_18;
 Q63 tmp_19;
 Q63 tmp_20;
 Q31 tmp_21;


a[0] = memory<complex<Q31>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q31>>::load(in,2 * stageLength * strideFactor);


tmp_16 = ( decq( decq(a[0].re,1)  ,1)   +  decq(( decq(a[1].re,1)   +  decq(a[2].re,1)  ) ,1)  ) ;
res[0].re = tmp_16;
tmp_0 = ( decq(a[1].im,1)   -  decq(a[2].im,1)  ) ;
tmp_1 = (C0_3_tw) * tmp_0 ;
tmp_2 = ( decq(a[1].re,1)   +  decq(a[2].re,1)  ) ;
tmp_3 = (C1_3_tw) * tmp_2 ;
tmp_17 = (tmp_1 + ( decq( widen<Q31>::run(a[0].re)  ,1)   - tmp_3) ) ;
tmp_4 = tmp_17;
tmp_5 = ( decq(a[2].re,1)   -  decq(a[1].re,1)  ) ;
tmp_6 = (C0_3_tw) * tmp_5 ;
tmp_7 = ( decq(a[1].im,1)   +  decq(a[2].im,1)  ) ;
tmp_8 = (C1_3_tw) * tmp_7 ;
tmp_18 = (tmp_6 + ( decq( widen<Q31>::run(a[0].im)  ,1)   - tmp_8) ) ;
tmp_9 = tmp_18;
res[1].re =  narrow<1,Q31>::run((w[0].re *  narrow<NORMALNARROW,Q31>::run(tmp_4)    - w[0].im *  narrow<NORMALNARROW,Q31>::run(tmp_9)   ) )  ;
tmp_10 = ( decq(a[2].im,1)   -  decq(a[1].im,1)  ) ;
tmp_11 = (C0_3_tw) * tmp_10 ;
tmp_19 = (tmp_11 + ( decq( widen<Q31>::run(a[0].re)  ,1)   - tmp_3) ) ;
tmp_12 = tmp_19;
tmp_13 = ( decq(a[1].re,1)   -  decq(a[2].re,1)  ) ;
tmp_14 = (C0_3_tw) * tmp_13 ;
tmp_20 = (tmp_14 + ( decq( widen<Q31>::run(a[0].im)  ,1)   - tmp_8) ) ;
tmp_15 = tmp_20;
res[2].re =  narrow<1,Q31>::run((w[1].re *  narrow<NORMALNARROW,Q31>::run(tmp_12)    - w[1].im *  narrow<NORMALNARROW,Q31>::run(tmp_15)   ) )  ;
tmp_21 = ( decq( decq(a[0].im,1)  ,1)   +  decq(( decq(a[1].im,1)   +  decq(a[2].im,1)  ) ,1)  ) ;
res[0].im = tmp_21;
res[1].im =  narrow<1,Q31>::run((w[0].re *  narrow<NORMALNARROW,Q31>::run(tmp_9)    + w[0].im *  narrow<NORMALNARROW,Q31>::run(tmp_4)   ) )  ;
res[2].im =  narrow<1,Q31>::run((w[1].re *  narrow<NORMALNARROW,Q31>::run(tmp_15)    + w[1].im *  narrow<NORMALNARROW,Q31>::run(tmp_12)   ) )  ;


memory<complex<Q31>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<Q31>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<Q31>>::store(out,2 * stageLength * strideFactor , res[2]);


};

/* Nb Multiplications = 6, 
Nb Additions = 18,
Nb Negates = 8,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 2

*/
 __STATIC_FORCEINLINE void run_ip_last(complex<Q31> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q31> a[3];
complex<Q31> res[3];


 Q31 tmp_0;
 Q63 tmp_1;
 Q31 tmp_2;
 Q63 tmp_3;
 Q31 tmp_4;
 Q63 tmp_5;
 Q63 tmp_6;
 Q31 tmp_7;
 Q63 tmp_8;
 Q63 tmp_9;


a[0] = memory<complex<Q31>>::load(in,0 * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * strideFactor);
a[2] = memory<complex<Q31>>::load(in,2 * strideFactor);


tmp_4 = ( decq( decq(a[0].re,1)  ,1)   +  decq(( decq(a[1].re,1)   +  decq(a[2].re,1)  ) ,1)  ) ;
res[0].re = tmp_4;
tmp_0 = ( decq(a[1].re,1)   +  decq(a[2].re,1)  ) ;
tmp_1 = (C1_3_notw) * tmp_0 ;
tmp_5 = ((C0_3_notw) * ( decq(a[1].im,1)   -  decq(a[2].im,1)  )   + ( decq( widen<Q31>::run(a[0].re)  ,1)   - tmp_1) ) ;
res[1].re =  narrow<1,Q31>::run(tmp_5)  ;
tmp_6 = ((C0_3_notw) * ( decq(a[2].im,1)   -  decq(a[1].im,1)  )   + ( decq( widen<Q31>::run(a[0].re)  ,1)   - tmp_1) ) ;
res[2].re =  narrow<1,Q31>::run(tmp_6)  ;
tmp_7 = ( decq( decq(a[0].im,1)  ,1)   +  decq(( decq(a[1].im,1)   +  decq(a[2].im,1)  ) ,1)  ) ;
res[0].im = tmp_7;
tmp_2 = ( decq(a[1].im,1)   +  decq(a[2].im,1)  ) ;
tmp_3 = (C1_3_notw) * tmp_2 ;
tmp_8 = ((C0_3_notw) * ( decq(a[2].re,1)   -  decq(a[1].re,1)  )   + ( decq( widen<Q31>::run(a[0].im)  ,1)   - tmp_3) ) ;
res[1].im =  narrow<1,Q31>::run(tmp_8)  ;
tmp_9 = ((C0_3_notw) * ( decq(a[1].re,1)   -  decq(a[2].re,1)  )   + ( decq( widen<Q31>::run(a[0].im)  ,1)   - tmp_3) ) ;
res[2].im =  narrow<1,Q31>::run(tmp_9)  ;


memory<complex<Q31>>::store(in,0 * strideFactor , res[0]);
memory<complex<Q31>>::store(in,1 * strideFactor , res[1]);
memory<complex<Q31>>::store(in,2 * strideFactor , res[2]);


};

/* Nb Multiplications = 6, 
Nb Additions = 18,
Nb Negates = 8,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 2

*/
 __STATIC_FORCEINLINE void run_op_last(const complex<Q31> *in,complex<Q31> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q31> a[3];
complex<Q31> res[3];


 Q31 tmp_0;
 Q63 tmp_1;
 Q31 tmp_2;
 Q63 tmp_3;
 Q31 tmp_4;
 Q63 tmp_5;
 Q63 tmp_6;
 Q31 tmp_7;
 Q63 tmp_8;
 Q63 tmp_9;


a[0] = memory<complex<Q31>>::load(in,0 * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * strideFactor);
a[2] = memory<complex<Q31>>::load(in,2 * strideFactor);


tmp_4 = ( decq( decq(a[0].re,1)  ,1)   +  decq(( decq(a[1].re,1)   +  decq(a[2].re,1)  ) ,1)  ) ;
res[0].re = tmp_4;
tmp_0 = ( decq(a[1].re,1)   +  decq(a[2].re,1)  ) ;
tmp_1 = (C1_3_notw) * tmp_0 ;
tmp_5 = ((C0_3_notw) * ( decq(a[1].im,1)   -  decq(a[2].im,1)  )   + ( decq( widen<Q31>::run(a[0].re)  ,1)   - tmp_1) ) ;
res[1].re =  narrow<1,Q31>::run(tmp_5)  ;
tmp_6 = ((C0_3_notw) * ( decq(a[2].im,1)   -  decq(a[1].im,1)  )   + ( decq( widen<Q31>::run(a[0].re)  ,1)   - tmp_1) ) ;
res[2].re =  narrow<1,Q31>::run(tmp_6)  ;
tmp_7 = ( decq( decq(a[0].im,1)  ,1)   +  decq(( decq(a[1].im,1)   +  decq(a[2].im,1)  ) ,1)  ) ;
res[0].im = tmp_7;
tmp_2 = ( decq(a[1].im,1)   +  decq(a[2].im,1)  ) ;
tmp_3 = (C1_3_notw) * tmp_2 ;
tmp_8 = ((C0_3_notw) * ( decq(a[2].re,1)   -  decq(a[1].re,1)  )   + ( decq( widen<Q31>::run(a[0].im)  ,1)   - tmp_3) ) ;
res[1].im =  narrow<1,Q31>::run(tmp_8)  ;
tmp_9 = ((C0_3_notw) * ( decq(a[1].re,1)   -  decq(a[2].re,1)  )   + ( decq( widen<Q31>::run(a[0].im)  ,1)   - tmp_3) ) ;
res[2].im =  narrow<1,Q31>::run(tmp_9)  ;


memory<complex<Q31>>::store(out,0 * strideFactor , res[0]);
memory<complex<Q31>>::store(out,1 * strideFactor , res[1]);
memory<complex<Q31>>::store(out,2 * strideFactor , res[2]);


};

};

/**************

INVERSE FFT 

***************/
template<int inputstrideenabled> 
struct radix<INVERSE,Q31, 3,inputstrideenabled>{

static constexpr Q31 C0_3_tw{(q31_t)0x6ED9EBA1};
  static constexpr Q31 C1_3_tw{(q31_t)0x40000000};
  
static constexpr Q31 C0_3_notw{(q31_t)0x6ED9EBA1};
  static constexpr Q31 C1_3_notw{(q31_t)0x40000000};
  

/* Nb Multiplications = 6, 
Nb Additions = 18,
Nb Negates = 8,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 2

*/
 __STATIC_FORCEINLINE void run_ip_notw(int stageLength,complex<Q31> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q31> a[3];
complex<Q31> res[3];


 Q31 tmp_0;
 Q63 tmp_1;
 Q31 tmp_2;
 Q63 tmp_3;
 Q31 tmp_4;
 Q63 tmp_5;
 Q63 tmp_6;
 Q31 tmp_7;
 Q63 tmp_8;
 Q63 tmp_9;


a[0] = memory<complex<Q31>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q31>>::load(in,2 * stageLength * strideFactor);


tmp_4 = ( decq( decq(a[0].re,1)  ,1)   +  decq(( decq(a[1].re,1)   +  decq(a[2].re,1)  ) ,1)  ) ;
res[0].re = tmp_4;
tmp_0 = ( decq(a[1].re,1)   +  decq(a[2].re,1)  ) ;
tmp_1 = (C1_3_notw) * tmp_0 ;
tmp_5 = ((C0_3_notw) * ( decq(a[2].im,1)   -  decq(a[1].im,1)  )   + ( decq( widen<Q31>::run(a[0].re)  ,1)   - tmp_1) ) ;
res[1].re =  narrow<1,Q31>::run(tmp_5)  ;
tmp_6 = ((C0_3_notw) * ( decq(a[1].im,1)   -  decq(a[2].im,1)  )   + ( decq( widen<Q31>::run(a[0].re)  ,1)   - tmp_1) ) ;
res[2].re =  narrow<1,Q31>::run(tmp_6)  ;
tmp_7 = ( decq( decq(a[0].im,1)  ,1)   +  decq(( decq(a[1].im,1)   +  decq(a[2].im,1)  ) ,1)  ) ;
res[0].im = tmp_7;
tmp_2 = ( decq(a[1].im,1)   +  decq(a[2].im,1)  ) ;
tmp_3 = (C1_3_notw) * tmp_2 ;
tmp_8 = ((C0_3_notw) * ( decq(a[1].re,1)   -  decq(a[2].re,1)  )   + ( decq( widen<Q31>::run(a[0].im)  ,1)   - tmp_3) ) ;
res[1].im =  narrow<1,Q31>::run(tmp_8)  ;
tmp_9 = ((C0_3_notw) * ( decq(a[2].re,1)   -  decq(a[1].re,1)  )   + ( decq( widen<Q31>::run(a[0].im)  ,1)   - tmp_3) ) ;
res[2].im =  narrow<1,Q31>::run(tmp_9)  ;


memory<complex<Q31>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<Q31>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<Q31>>::store(in,2 * stageLength * strideFactor , res[2]);


};

/* Nb Multiplications = 14, 
Nb Additions = 22,
Nb Negates = 10,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 2

*/
 __STATIC_FORCEINLINE void run_ip_tw(int stageLength,const complex<Q31> *w,complex<Q31> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q31> a[3];
complex<Q31> res[3];


 Q31 tmp_0;
 Q63 tmp_1;
 Q31 tmp_2;
 Q63 tmp_3;
 Q63 tmp_4;
 Q31 tmp_5;
 Q63 tmp_6;
 Q31 tmp_7;
 Q63 tmp_8;
 Q63 tmp_9;
 Q31 tmp_10;
 Q63 tmp_11;
 Q63 tmp_12;
 Q31 tmp_13;
 Q63 tmp_14;
 Q63 tmp_15;
 Q31 tmp_16;
 Q63 tmp_17;
 Q63 tmp_18;
 Q63 tmp_19;
 Q63 tmp_20;
 Q31 tmp_21;


a[0] = memory<complex<Q31>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q31>>::load(in,2 * stageLength * strideFactor);


tmp_16 = ( decq( decq(a[0].re,1)  ,1)   +  decq(( decq(a[1].re,1)   +  decq(a[2].re,1)  ) ,1)  ) ;
res[0].re = tmp_16;
tmp_0 = ( decq(a[2].im,1)   -  decq(a[1].im,1)  ) ;
tmp_1 = (C0_3_tw) * tmp_0 ;
tmp_2 = ( decq(a[1].re,1)   +  decq(a[2].re,1)  ) ;
tmp_3 = (C1_3_tw) * tmp_2 ;
tmp_17 = (tmp_1 + ( decq( widen<Q31>::run(a[0].re)  ,1)   - tmp_3) ) ;
tmp_4 = tmp_17;
tmp_5 = ( decq(a[1].re,1)   -  decq(a[2].re,1)  ) ;
tmp_6 = (C0_3_tw) * tmp_5 ;
tmp_7 = ( decq(a[1].im,1)   +  decq(a[2].im,1)  ) ;
tmp_8 = (C1_3_tw) * tmp_7 ;
tmp_18 = (tmp_6 + ( decq( widen<Q31>::run(a[0].im)  ,1)   - tmp_8) ) ;
tmp_9 = tmp_18;
res[1].re =  narrow<1,Q31>::run((w[0].re *  narrow<NORMALNARROW,Q31>::run(tmp_4)    + w[0].im *  narrow<NORMALNARROW,Q31>::run(tmp_9)   ) )  ;
tmp_10 = ( decq(a[1].im,1)   -  decq(a[2].im,1)  ) ;
tmp_11 = (C0_3_tw) * tmp_10 ;
tmp_19 = (tmp_11 + ( decq( widen<Q31>::run(a[0].re)  ,1)   - tmp_3) ) ;
tmp_12 = tmp_19;
tmp_13 = ( decq(a[2].re,1)   -  decq(a[1].re,1)  ) ;
tmp_14 = (C0_3_tw) * tmp_13 ;
tmp_20 = (tmp_14 + ( decq( widen<Q31>::run(a[0].im)  ,1)   - tmp_8) ) ;
tmp_15 = tmp_20;
res[2].re =  narrow<1,Q31>::run((w[1].re *  narrow<NORMALNARROW,Q31>::run(tmp_12)    + w[1].im *  narrow<NORMALNARROW,Q31>::run(tmp_15)   ) )  ;
tmp_21 = ( decq( decq(a[0].im,1)  ,1)   +  decq(( decq(a[1].im,1)   +  decq(a[2].im,1)  ) ,1)  ) ;
res[0].im = tmp_21;
res[1].im =  narrow<1,Q31>::run((w[0].re *  narrow<NORMALNARROW,Q31>::run(tmp_9)    - w[0].im *  narrow<NORMALNARROW,Q31>::run(tmp_4)   ) )  ;
res[2].im =  narrow<1,Q31>::run((w[1].re *  narrow<NORMALNARROW,Q31>::run(tmp_15)    - w[1].im *  narrow<NORMALNARROW,Q31>::run(tmp_12)   ) )  ;


memory<complex<Q31>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<Q31>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<Q31>>::store(in,2 * stageLength * strideFactor , res[2]);


};

/* Nb Multiplications = 6, 
Nb Additions = 18,
Nb Negates = 8,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 2

*/
 __STATIC_FORCEINLINE void run_op_notw(int stageLength,const complex<Q31> *in,complex<Q31> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q31> a[3];
complex<Q31> res[3];


 Q31 tmp_0;
 Q63 tmp_1;
 Q31 tmp_2;
 Q63 tmp_3;
 Q31 tmp_4;
 Q63 tmp_5;
 Q63 tmp_6;
 Q31 tmp_7;
 Q63 tmp_8;
 Q63 tmp_9;


a[0] = memory<complex<Q31>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q31>>::load(in,2 * stageLength * strideFactor);


tmp_4 = ( decq( decq(a[0].re,1)  ,1)   +  decq(( decq(a[1].re,1)   +  decq(a[2].re,1)  ) ,1)  ) ;
res[0].re = tmp_4;
tmp_0 = ( decq(a[1].re,1)   +  decq(a[2].re,1)  ) ;
tmp_1 = (C1_3_notw) * tmp_0 ;
tmp_5 = ((C0_3_notw) * ( decq(a[2].im,1)   -  decq(a[1].im,1)  )   + ( decq( widen<Q31>::run(a[0].re)  ,1)   - tmp_1) ) ;
res[1].re =  narrow<1,Q31>::run(tmp_5)  ;
tmp_6 = ((C0_3_notw) * ( decq(a[1].im,1)   -  decq(a[2].im,1)  )   + ( decq( widen<Q31>::run(a[0].re)  ,1)   - tmp_1) ) ;
res[2].re =  narrow<1,Q31>::run(tmp_6)  ;
tmp_7 = ( decq( decq(a[0].im,1)  ,1)   +  decq(( decq(a[1].im,1)   +  decq(a[2].im,1)  ) ,1)  ) ;
res[0].im = tmp_7;
tmp_2 = ( decq(a[1].im,1)   +  decq(a[2].im,1)  ) ;
tmp_3 = (C1_3_notw) * tmp_2 ;
tmp_8 = ((C0_3_notw) * ( decq(a[1].re,1)   -  decq(a[2].re,1)  )   + ( decq( widen<Q31>::run(a[0].im)  ,1)   - tmp_3) ) ;
res[1].im =  narrow<1,Q31>::run(tmp_8)  ;
tmp_9 = ((C0_3_notw) * ( decq(a[2].re,1)   -  decq(a[1].re,1)  )   + ( decq( widen<Q31>::run(a[0].im)  ,1)   - tmp_3) ) ;
res[2].im =  narrow<1,Q31>::run(tmp_9)  ;


memory<complex<Q31>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<Q31>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<Q31>>::store(out,2 * stageLength * strideFactor , res[2]);


};

/* Nb Multiplications = 14, 
Nb Additions = 22,
Nb Negates = 10,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 2

*/
 __STATIC_FORCEINLINE void run_op_tw(int stageLength,const complex<Q31> *w,const complex<Q31> *in,complex<Q31> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q31> a[3];
complex<Q31> res[3];


 Q31 tmp_0;
 Q63 tmp_1;
 Q31 tmp_2;
 Q63 tmp_3;
 Q63 tmp_4;
 Q31 tmp_5;
 Q63 tmp_6;
 Q31 tmp_7;
 Q63 tmp_8;
 Q63 tmp_9;
 Q31 tmp_10;
 Q63 tmp_11;
 Q63 tmp_12;
 Q31 tmp_13;
 Q63 tmp_14;
 Q63 tmp_15;
 Q31 tmp_16;
 Q63 tmp_17;
 Q63 tmp_18;
 Q63 tmp_19;
 Q63 tmp_20;
 Q31 tmp_21;


a[0] = memory<complex<Q31>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q31>>::load(in,2 * stageLength * strideFactor);


tmp_16 = ( decq( decq(a[0].re,1)  ,1)   +  decq(( decq(a[1].re,1)   +  decq(a[2].re,1)  ) ,1)  ) ;
res[0].re = tmp_16;
tmp_0 = ( decq(a[2].im,1)   -  decq(a[1].im,1)  ) ;
tmp_1 = (C0_3_tw) * tmp_0 ;
tmp_2 = ( decq(a[1].re,1)   +  decq(a[2].re,1)  ) ;
tmp_3 = (C1_3_tw) * tmp_2 ;
tmp_17 = (tmp_1 + ( decq( widen<Q31>::run(a[0].re)  ,1)   - tmp_3) ) ;
tmp_4 = tmp_17;
tmp_5 = ( decq(a[1].re,1)   -  decq(a[2].re,1)  ) ;
tmp_6 = (C0_3_tw) * tmp_5 ;
tmp_7 = ( decq(a[1].im,1)   +  decq(a[2].im,1)  ) ;
tmp_8 = (C1_3_tw) * tmp_7 ;
tmp_18 = (tmp_6 + ( decq( widen<Q31>::run(a[0].im)  ,1)   - tmp_8) ) ;
tmp_9 = tmp_18;
res[1].re =  narrow<1,Q31>::run((w[0].re *  narrow<NORMALNARROW,Q31>::run(tmp_4)    + w[0].im *  narrow<NORMALNARROW,Q31>::run(tmp_9)   ) )  ;
tmp_10 = ( decq(a[1].im,1)   -  decq(a[2].im,1)  ) ;
tmp_11 = (C0_3_tw) * tmp_10 ;
tmp_19 = (tmp_11 + ( decq( widen<Q31>::run(a[0].re)  ,1)   - tmp_3) ) ;
tmp_12 = tmp_19;
tmp_13 = ( decq(a[2].re,1)   -  decq(a[1].re,1)  ) ;
tmp_14 = (C0_3_tw) * tmp_13 ;
tmp_20 = (tmp_14 + ( decq( widen<Q31>::run(a[0].im)  ,1)   - tmp_8) ) ;
tmp_15 = tmp_20;
res[2].re =  narrow<1,Q31>::run((w[1].re *  narrow<NORMALNARROW,Q31>::run(tmp_12)    + w[1].im *  narrow<NORMALNARROW,Q31>::run(tmp_15)   ) )  ;
tmp_21 = ( decq( decq(a[0].im,1)  ,1)   +  decq(( decq(a[1].im,1)   +  decq(a[2].im,1)  ) ,1)  ) ;
res[0].im = tmp_21;
res[1].im =  narrow<1,Q31>::run((w[0].re *  narrow<NORMALNARROW,Q31>::run(tmp_9)    - w[0].im *  narrow<NORMALNARROW,Q31>::run(tmp_4)   ) )  ;
res[2].im =  narrow<1,Q31>::run((w[1].re *  narrow<NORMALNARROW,Q31>::run(tmp_15)    - w[1].im *  narrow<NORMALNARROW,Q31>::run(tmp_12)   ) )  ;


memory<complex<Q31>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<Q31>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<Q31>>::store(out,2 * stageLength * strideFactor , res[2]);


};

/* Nb Multiplications = 6, 
Nb Additions = 18,
Nb Negates = 8,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 2

*/
 __STATIC_FORCEINLINE void run_ip_last(complex<Q31> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q31> a[3];
complex<Q31> res[3];


 Q31 tmp_0;
 Q63 tmp_1;
 Q31 tmp_2;
 Q63 tmp_3;
 Q31 tmp_4;
 Q63 tmp_5;
 Q63 tmp_6;
 Q31 tmp_7;
 Q63 tmp_8;
 Q63 tmp_9;


a[0] = memory<complex<Q31>>::load(in,0 * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * strideFactor);
a[2] = memory<complex<Q31>>::load(in,2 * strideFactor);


tmp_4 = ( decq( decq(a[0].re,1)  ,1)   +  decq(( decq(a[1].re,1)   +  decq(a[2].re,1)  ) ,1)  ) ;
res[0].re = tmp_4;
tmp_0 = ( decq(a[1].re,1)   +  decq(a[2].re,1)  ) ;
tmp_1 = (C1_3_notw) * tmp_0 ;
tmp_5 = ((C0_3_notw) * ( decq(a[2].im,1)   -  decq(a[1].im,1)  )   + ( decq( widen<Q31>::run(a[0].re)  ,1)   - tmp_1) ) ;
res[1].re =  narrow<1,Q31>::run(tmp_5)  ;
tmp_6 = ((C0_3_notw) * ( decq(a[1].im,1)   -  decq(a[2].im,1)  )   + ( decq( widen<Q31>::run(a[0].re)  ,1)   - tmp_1) ) ;
res[2].re =  narrow<1,Q31>::run(tmp_6)  ;
tmp_7 = ( decq( decq(a[0].im,1)  ,1)   +  decq(( decq(a[1].im,1)   +  decq(a[2].im,1)  ) ,1)  ) ;
res[0].im = tmp_7;
tmp_2 = ( decq(a[1].im,1)   +  decq(a[2].im,1)  ) ;
tmp_3 = (C1_3_notw) * tmp_2 ;
tmp_8 = ((C0_3_notw) * ( decq(a[1].re,1)   -  decq(a[2].re,1)  )   + ( decq( widen<Q31>::run(a[0].im)  ,1)   - tmp_3) ) ;
res[1].im =  narrow<1,Q31>::run(tmp_8)  ;
tmp_9 = ((C0_3_notw) * ( decq(a[2].re,1)   -  decq(a[1].re,1)  )   + ( decq( widen<Q31>::run(a[0].im)  ,1)   - tmp_3) ) ;
res[2].im =  narrow<1,Q31>::run(tmp_9)  ;


memory<complex<Q31>>::store(in,0 * strideFactor , res[0]);
memory<complex<Q31>>::store(in,1 * strideFactor , res[1]);
memory<complex<Q31>>::store(in,2 * strideFactor , res[2]);


};

/* Nb Multiplications = 6, 
Nb Additions = 18,
Nb Negates = 8,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 2

*/
 __STATIC_FORCEINLINE void run_op_last(const complex<Q31> *in,complex<Q31> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q31> a[3];
complex<Q31> res[3];


 Q31 tmp_0;
 Q63 tmp_1;
 Q31 tmp_2;
 Q63 tmp_3;
 Q31 tmp_4;
 Q63 tmp_5;
 Q63 tmp_6;
 Q31 tmp_7;
 Q63 tmp_8;
 Q63 tmp_9;


a[0] = memory<complex<Q31>>::load(in,0 * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * strideFactor);
a[2] = memory<complex<Q31>>::load(in,2 * strideFactor);


tmp_4 = ( decq( decq(a[0].re,1)  ,1)   +  decq(( decq(a[1].re,1)   +  decq(a[2].re,1)  ) ,1)  ) ;
res[0].re = tmp_4;
tmp_0 = ( decq(a[1].re,1)   +  decq(a[2].re,1)  ) ;
tmp_1 = (C1_3_notw) * tmp_0 ;
tmp_5 = ((C0_3_notw) * ( decq(a[2].im,1)   -  decq(a[1].im,1)  )   + ( decq( widen<Q31>::run(a[0].re)  ,1)   - tmp_1) ) ;
res[1].re =  narrow<1,Q31>::run(tmp_5)  ;
tmp_6 = ((C0_3_notw) * ( decq(a[1].im,1)   -  decq(a[2].im,1)  )   + ( decq( widen<Q31>::run(a[0].re)  ,1)   - tmp_1) ) ;
res[2].re =  narrow<1,Q31>::run(tmp_6)  ;
tmp_7 = ( decq( decq(a[0].im,1)  ,1)   +  decq(( decq(a[1].im,1)   +  decq(a[2].im,1)  ) ,1)  ) ;
res[0].im = tmp_7;
tmp_2 = ( decq(a[1].im,1)   +  decq(a[2].im,1)  ) ;
tmp_3 = (C1_3_notw) * tmp_2 ;
tmp_8 = ((C0_3_notw) * ( decq(a[1].re,1)   -  decq(a[2].re,1)  )   + ( decq( widen<Q31>::run(a[0].im)  ,1)   - tmp_3) ) ;
res[1].im =  narrow<1,Q31>::run(tmp_8)  ;
tmp_9 = ((C0_3_notw) * ( decq(a[2].re,1)   -  decq(a[1].re,1)  )   + ( decq( widen<Q31>::run(a[0].im)  ,1)   - tmp_3) ) ;
res[2].im =  narrow<1,Q31>::run(tmp_9)  ;


memory<complex<Q31>>::store(out,0 * strideFactor , res[0]);
memory<complex<Q31>>::store(out,1 * strideFactor , res[1]);
memory<complex<Q31>>::store(out,2 * strideFactor , res[2]);


};

};

#endif /* #ifndef _RADIX_Q31_3_H */