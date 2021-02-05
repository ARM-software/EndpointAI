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

#ifndef _RADIX_Q31_8_H
#define _RADIX_Q31_8_H

#include "complex.h"

/**************

DIRECT FFT 

***************/
template<int inputstrideenabled> 
struct radix<DIRECT,Q31, 8,inputstrideenabled>{

static constexpr Q31 C0_8_tw{(q31_t)0x5A82799A};
  
static constexpr Q31 C0_8_notw{(q31_t)0x5A82799A};
  

/* Nb Multiplications = 4, 
Nb Additions = 52,
Nb Negates = 26,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 3

*/
 __STATIC_FORCEINLINE void run_ip_notw(int stageLength,complex<Q31> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q31> a[8];
complex<Q31> res[8];


 Q31 tmp_0;
 Q31 tmp_1;
 Q31 tmp_2;
 Q31 tmp_3;
 Q31 tmp_4;
 Q31 tmp_5;
 Q31 tmp_6;
 Q31 tmp_7;
 Q31 tmp_8;
 Q31 tmp_9;
 Q31 tmp_10;
 Q31 tmp_11;
 Q31 tmp_12;
 Q31 tmp_13;
 Q31 tmp_14;
 Q31 tmp_15;
 Q63 tmp_16;
 Q31 tmp_17;
 Q31 tmp_18;
 Q31 tmp_19;
 Q31 tmp_20;
 Q31 tmp_21;
 Q31 tmp_22;
 Q31 tmp_23;
 Q31 tmp_24;
 Q63 tmp_25;
 Q31 tmp_26;
 Q31 tmp_27;
 Q31 tmp_28;
 Q31 tmp_29;
 Q31 tmp_30;
 Q31 tmp_31;
 Q31 tmp_32;
 Q31 tmp_33;
 Q63 tmp_34;
 Q31 tmp_35;
 Q31 tmp_36;
 Q31 tmp_37;
 Q31 tmp_38;
 Q63 tmp_39;


a[0] = memory<complex<Q31>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q31>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q31>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<Q31>>::load(in,4 * stageLength * strideFactor);
a[5] = memory<complex<Q31>>::load(in,5 * stageLength * strideFactor);
a[6] = memory<complex<Q31>>::load(in,6 * stageLength * strideFactor);
a[7] = memory<complex<Q31>>::load(in,7 * stageLength * strideFactor);


tmp_0 = ( decq(a[0].re,1)   +  decq(a[4].re,1)  ) ;
tmp_1 = ( decq(a[2].re,1)   +  decq(a[6].re,1)  ) ;
tmp_2 = ( decq(tmp_0,1)   +  decq(tmp_1,1)  ) ;
tmp_3 = ( decq(a[1].re,1)   +  decq(a[5].re,1)  ) ;
tmp_4 = ( decq(a[3].re,1)   +  decq(a[7].re,1)  ) ;
tmp_5 = ( decq(tmp_3,1)   +  decq(tmp_4,1)  ) ;
res[0].re = ( decq(tmp_2,1)   +  decq(tmp_5,1)  ) ;
tmp_6 = ( decq(a[0].re,1)   -  decq(a[4].re,1)  ) ;
tmp_7 = ( decq(a[2].im,1)   -  decq(a[6].im,1)  ) ;
tmp_8 = ( decq(tmp_6,1)   +  decq(tmp_7,1)  ) ;
tmp_9 = ( decq(a[1].re,1)   -  decq(a[5].re,1)  ) ;
tmp_10 = ( decq(a[1].im,1)   -  decq(a[5].im,1)  ) ;
tmp_11 = ( decq(tmp_9,1)   +  decq(tmp_10,1)  ) ;
tmp_12 = ( decq(a[7].re,1)   -  decq(a[3].re,1)  ) ;
tmp_13 = ( decq(a[3].im,1)   -  decq(a[7].im,1)  ) ;
tmp_14 = ( decq(tmp_12,1)   +  decq(tmp_13,1)  ) ;
tmp_15 = ( decq(tmp_11,1)   +  decq(tmp_14,1)  ) ;
tmp_16 = (C0_8_notw) * tmp_15 ;
res[1].re =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_8)  ,1)   + tmp_16) )  ;
tmp_17 = ( decq(tmp_0,1)   -  decq(tmp_1,1)  ) ;
tmp_18 = ( decq(a[1].im,1)   +  decq(a[5].im,1)  ) ;
tmp_19 = ( decq(a[3].im,1)   +  decq(a[7].im,1)  ) ;
tmp_20 = ( decq(tmp_18,1)   -  decq(tmp_19,1)  ) ;
res[2].re = ( decq(tmp_17,1)   +  decq(tmp_20,1)  ) ;
tmp_21 = ( decq(tmp_6,1)   -  decq(tmp_7,1)  ) ;
tmp_22 = ( decq(tmp_10,1)   -  decq(tmp_9,1)  ) ;
tmp_23 = ( decq(tmp_12,1)   -  decq(tmp_13,1)  ) ;
tmp_24 = ( decq(tmp_22,1)   -  decq(tmp_23,1)  ) ;
tmp_25 = (C0_8_notw) * tmp_24 ;
res[3].re =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_21)  ,1)   + tmp_25) )  ;
res[4].re = ( decq(tmp_2,1)   -  decq(tmp_5,1)  ) ;
res[5].re =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_8)  ,1)   - tmp_16) )  ;
res[6].re = ( decq(tmp_17,1)   -  decq(tmp_20,1)  ) ;
res[7].re =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_21)  ,1)   - tmp_25) )  ;
tmp_26 = ( decq(a[0].im,1)   +  decq(a[4].im,1)  ) ;
tmp_27 = ( decq(a[2].im,1)   +  decq(a[6].im,1)  ) ;
tmp_28 = ( decq(tmp_26,1)   +  decq(tmp_27,1)  ) ;
tmp_29 = ( decq(tmp_18,1)   +  decq(tmp_19,1)  ) ;
res[0].im = ( decq(tmp_28,1)   +  decq(tmp_29,1)  ) ;
tmp_30 = ( decq(a[0].im,1)   -  decq(a[4].im,1)  ) ;
tmp_31 = ( decq(a[2].re,1)   -  decq(a[6].re,1)  ) ;
tmp_32 = ( decq(tmp_30,1)   -  decq(tmp_31,1)  ) ;
tmp_33 = ( decq(tmp_23,1)   +  decq(tmp_22,1)  ) ;
tmp_34 = (C0_8_notw) * tmp_33 ;
res[1].im =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_32)  ,1)   + tmp_34) )  ;
tmp_35 = ( decq(tmp_26,1)   -  decq(tmp_27,1)  ) ;
tmp_36 = ( decq(tmp_3,1)   -  decq(tmp_4,1)  ) ;
res[2].im = ( decq(tmp_35,1)   -  decq(tmp_36,1)  ) ;
tmp_37 = ( decq(tmp_31,1)   +  decq(tmp_30,1)  ) ;
tmp_38 = ( decq(tmp_11,1)   -  decq(tmp_14,1)  ) ;
tmp_39 = (C0_8_notw) * tmp_38 ;
res[3].im =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_37)  ,1)   - tmp_39) )  ;
res[4].im = ( decq(tmp_28,1)   -  decq(tmp_29,1)  ) ;
res[5].im =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_32)  ,1)   - tmp_34) )  ;
res[6].im = ( decq(tmp_36,1)   +  decq(tmp_35,1)  ) ;
res[7].im =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_37)  ,1)   + tmp_39) )  ;


memory<complex<Q31>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<Q31>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<Q31>>::store(in,2 * stageLength * strideFactor , res[2]);
memory<complex<Q31>>::store(in,3 * stageLength * strideFactor , res[3]);
memory<complex<Q31>>::store(in,4 * stageLength * strideFactor , res[4]);
memory<complex<Q31>>::store(in,5 * stageLength * strideFactor , res[5]);
memory<complex<Q31>>::store(in,6 * stageLength * strideFactor , res[6]);
memory<complex<Q31>>::store(in,7 * stageLength * strideFactor , res[7]);


};

/* Nb Multiplications = 32, 
Nb Additions = 66,
Nb Negates = 33,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 3

*/
 __STATIC_FORCEINLINE void run_ip_tw(int stageLength,const complex<Q31> *w,complex<Q31> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q31> a[8];
complex<Q31> res[8];


 Q31 tmp_0;
 Q31 tmp_1;
 Q31 tmp_2;
 Q31 tmp_3;
 Q31 tmp_4;
 Q31 tmp_5;
 Q31 tmp_6;
 Q31 tmp_7;
 Q31 tmp_8;
 Q31 tmp_9;
 Q31 tmp_10;
 Q31 tmp_11;
 Q31 tmp_12;
 Q31 tmp_13;
 Q31 tmp_14;
 Q31 tmp_15;
 Q63 tmp_16;
 Q63 tmp_17;
 Q31 tmp_18;
 Q31 tmp_19;
 Q31 tmp_20;
 Q31 tmp_21;
 Q31 tmp_22;
 Q31 tmp_23;
 Q63 tmp_24;
 Q63 tmp_25;
 Q31 tmp_26;
 Q31 tmp_27;
 Q31 tmp_28;
 Q31 tmp_29;
 Q31 tmp_30;
 Q31 tmp_31;
 Q31 tmp_32;
 Q31 tmp_33;
 Q31 tmp_34;
 Q31 tmp_35;
 Q31 tmp_36;
 Q31 tmp_37;
 Q63 tmp_38;
 Q63 tmp_39;
 Q31 tmp_40;
 Q31 tmp_41;
 Q63 tmp_42;
 Q63 tmp_43;
 Q31 tmp_44;
 Q31 tmp_45;
 Q31 tmp_46;
 Q31 tmp_47;
 Q63 tmp_48;
 Q63 tmp_49;
 Q31 tmp_50;
 Q31 tmp_51;
 Q63 tmp_52;
 Q63 tmp_53;


a[0] = memory<complex<Q31>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q31>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q31>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<Q31>>::load(in,4 * stageLength * strideFactor);
a[5] = memory<complex<Q31>>::load(in,5 * stageLength * strideFactor);
a[6] = memory<complex<Q31>>::load(in,6 * stageLength * strideFactor);
a[7] = memory<complex<Q31>>::load(in,7 * stageLength * strideFactor);


tmp_0 = ( decq(a[0].re,1)   +  decq(a[4].re,1)  ) ;
tmp_1 = ( decq(a[2].re,1)   +  decq(a[6].re,1)  ) ;
tmp_2 = ( decq(tmp_0,1)   +  decq(tmp_1,1)  ) ;
tmp_3 = ( decq(a[1].re,1)   +  decq(a[5].re,1)  ) ;
tmp_4 = ( decq(a[3].re,1)   +  decq(a[7].re,1)  ) ;
tmp_5 = ( decq(tmp_3,1)   +  decq(tmp_4,1)  ) ;
res[0].re = ( decq(tmp_2,1)   +  decq(tmp_5,1)  ) ;
tmp_6 = ( decq(a[0].re,1)   -  decq(a[4].re,1)  ) ;
tmp_7 = ( decq(a[2].im,1)   -  decq(a[6].im,1)  ) ;
tmp_8 = ( decq(tmp_6,1)   +  decq(tmp_7,1)  ) ;
tmp_9 = ( decq(a[1].re,1)   -  decq(a[5].re,1)  ) ;
tmp_10 = ( decq(a[1].im,1)   -  decq(a[5].im,1)  ) ;
tmp_11 = ( decq(tmp_9,1)   +  decq(tmp_10,1)  ) ;
tmp_12 = ( decq(a[7].re,1)   -  decq(a[3].re,1)  ) ;
tmp_13 = ( decq(a[3].im,1)   -  decq(a[7].im,1)  ) ;
tmp_14 = ( decq(tmp_12,1)   +  decq(tmp_13,1)  ) ;
tmp_15 = ( decq(tmp_11,1)   +  decq(tmp_14,1)  ) ;
tmp_16 = (C0_8_tw) * tmp_15 ;
tmp_17 = ( decq( widen<Q31>::run(tmp_8)  ,1)   + tmp_16) ;
tmp_18 = ( decq(a[0].im,1)   -  decq(a[4].im,1)  ) ;
tmp_19 = ( decq(a[2].re,1)   -  decq(a[6].re,1)  ) ;
tmp_20 = ( decq(tmp_18,1)   -  decq(tmp_19,1)  ) ;
tmp_21 = ( decq(tmp_12,1)   -  decq(tmp_13,1)  ) ;
tmp_22 = ( decq(tmp_10,1)   -  decq(tmp_9,1)  ) ;
tmp_23 = ( decq(tmp_21,1)   +  decq(tmp_22,1)  ) ;
tmp_24 = (C0_8_tw) * tmp_23 ;
tmp_25 = ( decq( widen<Q31>::run(tmp_20)  ,1)   + tmp_24) ;
res[1].re =  narrow<NORMALNARROW,Q31>::run((w[0].re *  narrow<NORMALNARROW,Q31>::run(tmp_17)    - w[0].im *  narrow<NORMALNARROW,Q31>::run(tmp_25)   ) )  ;
tmp_26 = ( decq(tmp_0,1)   -  decq(tmp_1,1)  ) ;
tmp_27 = ( decq(a[1].im,1)   +  decq(a[5].im,1)  ) ;
tmp_28 = ( decq(a[3].im,1)   +  decq(a[7].im,1)  ) ;
tmp_29 = ( decq(tmp_27,1)   -  decq(tmp_28,1)  ) ;
tmp_30 = ( decq(tmp_26,1)   +  decq(tmp_29,1)  ) ;
tmp_31 = ( decq(a[0].im,1)   +  decq(a[4].im,1)  ) ;
tmp_32 = ( decq(a[2].im,1)   +  decq(a[6].im,1)  ) ;
tmp_33 = ( decq(tmp_31,1)   -  decq(tmp_32,1)  ) ;
tmp_34 = ( decq(tmp_3,1)   -  decq(tmp_4,1)  ) ;
tmp_35 = ( decq(tmp_33,1)   -  decq(tmp_34,1)  ) ;
res[2].re =  narrow<NORMALNARROW,Q31>::run((w[1].re * tmp_30  - w[1].im * tmp_35 ) )  ;
tmp_36 = ( decq(tmp_6,1)   -  decq(tmp_7,1)  ) ;
tmp_37 = ( decq(tmp_22,1)   -  decq(tmp_21,1)  ) ;
tmp_38 = (C0_8_tw) * tmp_37 ;
tmp_39 = ( decq( widen<Q31>::run(tmp_36)  ,1)   + tmp_38) ;
tmp_40 = ( decq(tmp_19,1)   +  decq(tmp_18,1)  ) ;
tmp_41 = ( decq(tmp_11,1)   -  decq(tmp_14,1)  ) ;
tmp_42 = (C0_8_tw) * tmp_41 ;
tmp_43 = ( decq( widen<Q31>::run(tmp_40)  ,1)   - tmp_42) ;
res[3].re =  narrow<NORMALNARROW,Q31>::run((w[2].re *  narrow<NORMALNARROW,Q31>::run(tmp_39)    - w[2].im *  narrow<NORMALNARROW,Q31>::run(tmp_43)   ) )  ;
tmp_44 = ( decq(tmp_2,1)   -  decq(tmp_5,1)  ) ;
tmp_45 = ( decq(tmp_31,1)   +  decq(tmp_32,1)  ) ;
tmp_46 = ( decq(tmp_27,1)   +  decq(tmp_28,1)  ) ;
tmp_47 = ( decq(tmp_45,1)   -  decq(tmp_46,1)  ) ;
res[4].re =  narrow<NORMALNARROW,Q31>::run((w[3].re * tmp_44  - w[3].im * tmp_47 ) )  ;
tmp_48 = ( decq( widen<Q31>::run(tmp_8)  ,1)   - tmp_16) ;
tmp_49 = ( decq( widen<Q31>::run(tmp_20)  ,1)   - tmp_24) ;
res[5].re =  narrow<NORMALNARROW,Q31>::run((w[4].re *  narrow<NORMALNARROW,Q31>::run(tmp_48)    - w[4].im *  narrow<NORMALNARROW,Q31>::run(tmp_49)   ) )  ;
tmp_50 = ( decq(tmp_26,1)   -  decq(tmp_29,1)  ) ;
tmp_51 = ( decq(tmp_34,1)   +  decq(tmp_33,1)  ) ;
res[6].re =  narrow<NORMALNARROW,Q31>::run((w[5].re * tmp_50  - w[5].im * tmp_51 ) )  ;
tmp_52 = ( decq( widen<Q31>::run(tmp_36)  ,1)   - tmp_38) ;
tmp_53 = ( decq( widen<Q31>::run(tmp_40)  ,1)   + tmp_42) ;
res[7].re =  narrow<NORMALNARROW,Q31>::run((w[6].re *  narrow<NORMALNARROW,Q31>::run(tmp_52)    - w[6].im *  narrow<NORMALNARROW,Q31>::run(tmp_53)   ) )  ;
res[0].im = ( decq(tmp_45,1)   +  decq(tmp_46,1)  ) ;
res[1].im =  narrow<NORMALNARROW,Q31>::run((w[0].re *  narrow<NORMALNARROW,Q31>::run(tmp_25)    + w[0].im *  narrow<NORMALNARROW,Q31>::run(tmp_17)   ) )  ;
res[2].im =  narrow<NORMALNARROW,Q31>::run((w[1].re * tmp_35  + w[1].im * tmp_30 ) )  ;
res[3].im =  narrow<NORMALNARROW,Q31>::run((w[2].re *  narrow<NORMALNARROW,Q31>::run(tmp_43)    + w[2].im *  narrow<NORMALNARROW,Q31>::run(tmp_39)   ) )  ;
res[4].im =  narrow<NORMALNARROW,Q31>::run((w[3].re * tmp_47  + w[3].im * tmp_44 ) )  ;
res[5].im =  narrow<NORMALNARROW,Q31>::run((w[4].re *  narrow<NORMALNARROW,Q31>::run(tmp_49)    + w[4].im *  narrow<NORMALNARROW,Q31>::run(tmp_48)   ) )  ;
res[6].im =  narrow<NORMALNARROW,Q31>::run((w[5].re * tmp_51  + w[5].im * tmp_50 ) )  ;
res[7].im =  narrow<NORMALNARROW,Q31>::run((w[6].re *  narrow<NORMALNARROW,Q31>::run(tmp_53)    + w[6].im *  narrow<NORMALNARROW,Q31>::run(tmp_52)   ) )  ;


memory<complex<Q31>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<Q31>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<Q31>>::store(in,2 * stageLength * strideFactor , res[2]);
memory<complex<Q31>>::store(in,3 * stageLength * strideFactor , res[3]);
memory<complex<Q31>>::store(in,4 * stageLength * strideFactor , res[4]);
memory<complex<Q31>>::store(in,5 * stageLength * strideFactor , res[5]);
memory<complex<Q31>>::store(in,6 * stageLength * strideFactor , res[6]);
memory<complex<Q31>>::store(in,7 * stageLength * strideFactor , res[7]);


};

/* Nb Multiplications = 4, 
Nb Additions = 52,
Nb Negates = 26,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 3

*/
 __STATIC_FORCEINLINE void run_op_notw(int stageLength,const complex<Q31> *in,complex<Q31> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q31> a[8];
complex<Q31> res[8];


 Q31 tmp_0;
 Q31 tmp_1;
 Q31 tmp_2;
 Q31 tmp_3;
 Q31 tmp_4;
 Q31 tmp_5;
 Q31 tmp_6;
 Q31 tmp_7;
 Q31 tmp_8;
 Q31 tmp_9;
 Q31 tmp_10;
 Q31 tmp_11;
 Q31 tmp_12;
 Q31 tmp_13;
 Q31 tmp_14;
 Q31 tmp_15;
 Q63 tmp_16;
 Q31 tmp_17;
 Q31 tmp_18;
 Q31 tmp_19;
 Q31 tmp_20;
 Q31 tmp_21;
 Q31 tmp_22;
 Q31 tmp_23;
 Q31 tmp_24;
 Q63 tmp_25;
 Q31 tmp_26;
 Q31 tmp_27;
 Q31 tmp_28;
 Q31 tmp_29;
 Q31 tmp_30;
 Q31 tmp_31;
 Q31 tmp_32;
 Q31 tmp_33;
 Q63 tmp_34;
 Q31 tmp_35;
 Q31 tmp_36;
 Q31 tmp_37;
 Q31 tmp_38;
 Q63 tmp_39;


a[0] = memory<complex<Q31>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q31>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q31>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<Q31>>::load(in,4 * stageLength * strideFactor);
a[5] = memory<complex<Q31>>::load(in,5 * stageLength * strideFactor);
a[6] = memory<complex<Q31>>::load(in,6 * stageLength * strideFactor);
a[7] = memory<complex<Q31>>::load(in,7 * stageLength * strideFactor);


tmp_0 = ( decq(a[0].re,1)   +  decq(a[4].re,1)  ) ;
tmp_1 = ( decq(a[2].re,1)   +  decq(a[6].re,1)  ) ;
tmp_2 = ( decq(tmp_0,1)   +  decq(tmp_1,1)  ) ;
tmp_3 = ( decq(a[1].re,1)   +  decq(a[5].re,1)  ) ;
tmp_4 = ( decq(a[3].re,1)   +  decq(a[7].re,1)  ) ;
tmp_5 = ( decq(tmp_3,1)   +  decq(tmp_4,1)  ) ;
res[0].re = ( decq(tmp_2,1)   +  decq(tmp_5,1)  ) ;
tmp_6 = ( decq(a[0].re,1)   -  decq(a[4].re,1)  ) ;
tmp_7 = ( decq(a[2].im,1)   -  decq(a[6].im,1)  ) ;
tmp_8 = ( decq(tmp_6,1)   +  decq(tmp_7,1)  ) ;
tmp_9 = ( decq(a[1].re,1)   -  decq(a[5].re,1)  ) ;
tmp_10 = ( decq(a[1].im,1)   -  decq(a[5].im,1)  ) ;
tmp_11 = ( decq(tmp_9,1)   +  decq(tmp_10,1)  ) ;
tmp_12 = ( decq(a[7].re,1)   -  decq(a[3].re,1)  ) ;
tmp_13 = ( decq(a[3].im,1)   -  decq(a[7].im,1)  ) ;
tmp_14 = ( decq(tmp_12,1)   +  decq(tmp_13,1)  ) ;
tmp_15 = ( decq(tmp_11,1)   +  decq(tmp_14,1)  ) ;
tmp_16 = (C0_8_notw) * tmp_15 ;
res[1].re =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_8)  ,1)   + tmp_16) )  ;
tmp_17 = ( decq(tmp_0,1)   -  decq(tmp_1,1)  ) ;
tmp_18 = ( decq(a[1].im,1)   +  decq(a[5].im,1)  ) ;
tmp_19 = ( decq(a[3].im,1)   +  decq(a[7].im,1)  ) ;
tmp_20 = ( decq(tmp_18,1)   -  decq(tmp_19,1)  ) ;
res[2].re = ( decq(tmp_17,1)   +  decq(tmp_20,1)  ) ;
tmp_21 = ( decq(tmp_6,1)   -  decq(tmp_7,1)  ) ;
tmp_22 = ( decq(tmp_10,1)   -  decq(tmp_9,1)  ) ;
tmp_23 = ( decq(tmp_12,1)   -  decq(tmp_13,1)  ) ;
tmp_24 = ( decq(tmp_22,1)   -  decq(tmp_23,1)  ) ;
tmp_25 = (C0_8_notw) * tmp_24 ;
res[3].re =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_21)  ,1)   + tmp_25) )  ;
res[4].re = ( decq(tmp_2,1)   -  decq(tmp_5,1)  ) ;
res[5].re =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_8)  ,1)   - tmp_16) )  ;
res[6].re = ( decq(tmp_17,1)   -  decq(tmp_20,1)  ) ;
res[7].re =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_21)  ,1)   - tmp_25) )  ;
tmp_26 = ( decq(a[0].im,1)   +  decq(a[4].im,1)  ) ;
tmp_27 = ( decq(a[2].im,1)   +  decq(a[6].im,1)  ) ;
tmp_28 = ( decq(tmp_26,1)   +  decq(tmp_27,1)  ) ;
tmp_29 = ( decq(tmp_18,1)   +  decq(tmp_19,1)  ) ;
res[0].im = ( decq(tmp_28,1)   +  decq(tmp_29,1)  ) ;
tmp_30 = ( decq(a[0].im,1)   -  decq(a[4].im,1)  ) ;
tmp_31 = ( decq(a[2].re,1)   -  decq(a[6].re,1)  ) ;
tmp_32 = ( decq(tmp_30,1)   -  decq(tmp_31,1)  ) ;
tmp_33 = ( decq(tmp_23,1)   +  decq(tmp_22,1)  ) ;
tmp_34 = (C0_8_notw) * tmp_33 ;
res[1].im =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_32)  ,1)   + tmp_34) )  ;
tmp_35 = ( decq(tmp_26,1)   -  decq(tmp_27,1)  ) ;
tmp_36 = ( decq(tmp_3,1)   -  decq(tmp_4,1)  ) ;
res[2].im = ( decq(tmp_35,1)   -  decq(tmp_36,1)  ) ;
tmp_37 = ( decq(tmp_31,1)   +  decq(tmp_30,1)  ) ;
tmp_38 = ( decq(tmp_11,1)   -  decq(tmp_14,1)  ) ;
tmp_39 = (C0_8_notw) * tmp_38 ;
res[3].im =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_37)  ,1)   - tmp_39) )  ;
res[4].im = ( decq(tmp_28,1)   -  decq(tmp_29,1)  ) ;
res[5].im =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_32)  ,1)   - tmp_34) )  ;
res[6].im = ( decq(tmp_36,1)   +  decq(tmp_35,1)  ) ;
res[7].im =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_37)  ,1)   + tmp_39) )  ;


memory<complex<Q31>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<Q31>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<Q31>>::store(out,2 * stageLength * strideFactor , res[2]);
memory<complex<Q31>>::store(out,3 * stageLength * strideFactor , res[3]);
memory<complex<Q31>>::store(out,4 * stageLength * strideFactor , res[4]);
memory<complex<Q31>>::store(out,5 * stageLength * strideFactor , res[5]);
memory<complex<Q31>>::store(out,6 * stageLength * strideFactor , res[6]);
memory<complex<Q31>>::store(out,7 * stageLength * strideFactor , res[7]);


};

/* Nb Multiplications = 32, 
Nb Additions = 66,
Nb Negates = 33,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 3

*/
 __STATIC_FORCEINLINE void run_op_tw(int stageLength,const complex<Q31> *w,const complex<Q31> *in,complex<Q31> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q31> a[8];
complex<Q31> res[8];


 Q31 tmp_0;
 Q31 tmp_1;
 Q31 tmp_2;
 Q31 tmp_3;
 Q31 tmp_4;
 Q31 tmp_5;
 Q31 tmp_6;
 Q31 tmp_7;
 Q31 tmp_8;
 Q31 tmp_9;
 Q31 tmp_10;
 Q31 tmp_11;
 Q31 tmp_12;
 Q31 tmp_13;
 Q31 tmp_14;
 Q31 tmp_15;
 Q63 tmp_16;
 Q63 tmp_17;
 Q31 tmp_18;
 Q31 tmp_19;
 Q31 tmp_20;
 Q31 tmp_21;
 Q31 tmp_22;
 Q31 tmp_23;
 Q63 tmp_24;
 Q63 tmp_25;
 Q31 tmp_26;
 Q31 tmp_27;
 Q31 tmp_28;
 Q31 tmp_29;
 Q31 tmp_30;
 Q31 tmp_31;
 Q31 tmp_32;
 Q31 tmp_33;
 Q31 tmp_34;
 Q31 tmp_35;
 Q31 tmp_36;
 Q31 tmp_37;
 Q63 tmp_38;
 Q63 tmp_39;
 Q31 tmp_40;
 Q31 tmp_41;
 Q63 tmp_42;
 Q63 tmp_43;
 Q31 tmp_44;
 Q31 tmp_45;
 Q31 tmp_46;
 Q31 tmp_47;
 Q63 tmp_48;
 Q63 tmp_49;
 Q31 tmp_50;
 Q31 tmp_51;
 Q63 tmp_52;
 Q63 tmp_53;


a[0] = memory<complex<Q31>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q31>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q31>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<Q31>>::load(in,4 * stageLength * strideFactor);
a[5] = memory<complex<Q31>>::load(in,5 * stageLength * strideFactor);
a[6] = memory<complex<Q31>>::load(in,6 * stageLength * strideFactor);
a[7] = memory<complex<Q31>>::load(in,7 * stageLength * strideFactor);


tmp_0 = ( decq(a[0].re,1)   +  decq(a[4].re,1)  ) ;
tmp_1 = ( decq(a[2].re,1)   +  decq(a[6].re,1)  ) ;
tmp_2 = ( decq(tmp_0,1)   +  decq(tmp_1,1)  ) ;
tmp_3 = ( decq(a[1].re,1)   +  decq(a[5].re,1)  ) ;
tmp_4 = ( decq(a[3].re,1)   +  decq(a[7].re,1)  ) ;
tmp_5 = ( decq(tmp_3,1)   +  decq(tmp_4,1)  ) ;
res[0].re = ( decq(tmp_2,1)   +  decq(tmp_5,1)  ) ;
tmp_6 = ( decq(a[0].re,1)   -  decq(a[4].re,1)  ) ;
tmp_7 = ( decq(a[2].im,1)   -  decq(a[6].im,1)  ) ;
tmp_8 = ( decq(tmp_6,1)   +  decq(tmp_7,1)  ) ;
tmp_9 = ( decq(a[1].re,1)   -  decq(a[5].re,1)  ) ;
tmp_10 = ( decq(a[1].im,1)   -  decq(a[5].im,1)  ) ;
tmp_11 = ( decq(tmp_9,1)   +  decq(tmp_10,1)  ) ;
tmp_12 = ( decq(a[7].re,1)   -  decq(a[3].re,1)  ) ;
tmp_13 = ( decq(a[3].im,1)   -  decq(a[7].im,1)  ) ;
tmp_14 = ( decq(tmp_12,1)   +  decq(tmp_13,1)  ) ;
tmp_15 = ( decq(tmp_11,1)   +  decq(tmp_14,1)  ) ;
tmp_16 = (C0_8_tw) * tmp_15 ;
tmp_17 = ( decq( widen<Q31>::run(tmp_8)  ,1)   + tmp_16) ;
tmp_18 = ( decq(a[0].im,1)   -  decq(a[4].im,1)  ) ;
tmp_19 = ( decq(a[2].re,1)   -  decq(a[6].re,1)  ) ;
tmp_20 = ( decq(tmp_18,1)   -  decq(tmp_19,1)  ) ;
tmp_21 = ( decq(tmp_12,1)   -  decq(tmp_13,1)  ) ;
tmp_22 = ( decq(tmp_10,1)   -  decq(tmp_9,1)  ) ;
tmp_23 = ( decq(tmp_21,1)   +  decq(tmp_22,1)  ) ;
tmp_24 = (C0_8_tw) * tmp_23 ;
tmp_25 = ( decq( widen<Q31>::run(tmp_20)  ,1)   + tmp_24) ;
res[1].re =  narrow<NORMALNARROW,Q31>::run((w[0].re *  narrow<NORMALNARROW,Q31>::run(tmp_17)    - w[0].im *  narrow<NORMALNARROW,Q31>::run(tmp_25)   ) )  ;
tmp_26 = ( decq(tmp_0,1)   -  decq(tmp_1,1)  ) ;
tmp_27 = ( decq(a[1].im,1)   +  decq(a[5].im,1)  ) ;
tmp_28 = ( decq(a[3].im,1)   +  decq(a[7].im,1)  ) ;
tmp_29 = ( decq(tmp_27,1)   -  decq(tmp_28,1)  ) ;
tmp_30 = ( decq(tmp_26,1)   +  decq(tmp_29,1)  ) ;
tmp_31 = ( decq(a[0].im,1)   +  decq(a[4].im,1)  ) ;
tmp_32 = ( decq(a[2].im,1)   +  decq(a[6].im,1)  ) ;
tmp_33 = ( decq(tmp_31,1)   -  decq(tmp_32,1)  ) ;
tmp_34 = ( decq(tmp_3,1)   -  decq(tmp_4,1)  ) ;
tmp_35 = ( decq(tmp_33,1)   -  decq(tmp_34,1)  ) ;
res[2].re =  narrow<NORMALNARROW,Q31>::run((w[1].re * tmp_30  - w[1].im * tmp_35 ) )  ;
tmp_36 = ( decq(tmp_6,1)   -  decq(tmp_7,1)  ) ;
tmp_37 = ( decq(tmp_22,1)   -  decq(tmp_21,1)  ) ;
tmp_38 = (C0_8_tw) * tmp_37 ;
tmp_39 = ( decq( widen<Q31>::run(tmp_36)  ,1)   + tmp_38) ;
tmp_40 = ( decq(tmp_19,1)   +  decq(tmp_18,1)  ) ;
tmp_41 = ( decq(tmp_11,1)   -  decq(tmp_14,1)  ) ;
tmp_42 = (C0_8_tw) * tmp_41 ;
tmp_43 = ( decq( widen<Q31>::run(tmp_40)  ,1)   - tmp_42) ;
res[3].re =  narrow<NORMALNARROW,Q31>::run((w[2].re *  narrow<NORMALNARROW,Q31>::run(tmp_39)    - w[2].im *  narrow<NORMALNARROW,Q31>::run(tmp_43)   ) )  ;
tmp_44 = ( decq(tmp_2,1)   -  decq(tmp_5,1)  ) ;
tmp_45 = ( decq(tmp_31,1)   +  decq(tmp_32,1)  ) ;
tmp_46 = ( decq(tmp_27,1)   +  decq(tmp_28,1)  ) ;
tmp_47 = ( decq(tmp_45,1)   -  decq(tmp_46,1)  ) ;
res[4].re =  narrow<NORMALNARROW,Q31>::run((w[3].re * tmp_44  - w[3].im * tmp_47 ) )  ;
tmp_48 = ( decq( widen<Q31>::run(tmp_8)  ,1)   - tmp_16) ;
tmp_49 = ( decq( widen<Q31>::run(tmp_20)  ,1)   - tmp_24) ;
res[5].re =  narrow<NORMALNARROW,Q31>::run((w[4].re *  narrow<NORMALNARROW,Q31>::run(tmp_48)    - w[4].im *  narrow<NORMALNARROW,Q31>::run(tmp_49)   ) )  ;
tmp_50 = ( decq(tmp_26,1)   -  decq(tmp_29,1)  ) ;
tmp_51 = ( decq(tmp_34,1)   +  decq(tmp_33,1)  ) ;
res[6].re =  narrow<NORMALNARROW,Q31>::run((w[5].re * tmp_50  - w[5].im * tmp_51 ) )  ;
tmp_52 = ( decq( widen<Q31>::run(tmp_36)  ,1)   - tmp_38) ;
tmp_53 = ( decq( widen<Q31>::run(tmp_40)  ,1)   + tmp_42) ;
res[7].re =  narrow<NORMALNARROW,Q31>::run((w[6].re *  narrow<NORMALNARROW,Q31>::run(tmp_52)    - w[6].im *  narrow<NORMALNARROW,Q31>::run(tmp_53)   ) )  ;
res[0].im = ( decq(tmp_45,1)   +  decq(tmp_46,1)  ) ;
res[1].im =  narrow<NORMALNARROW,Q31>::run((w[0].re *  narrow<NORMALNARROW,Q31>::run(tmp_25)    + w[0].im *  narrow<NORMALNARROW,Q31>::run(tmp_17)   ) )  ;
res[2].im =  narrow<NORMALNARROW,Q31>::run((w[1].re * tmp_35  + w[1].im * tmp_30 ) )  ;
res[3].im =  narrow<NORMALNARROW,Q31>::run((w[2].re *  narrow<NORMALNARROW,Q31>::run(tmp_43)    + w[2].im *  narrow<NORMALNARROW,Q31>::run(tmp_39)   ) )  ;
res[4].im =  narrow<NORMALNARROW,Q31>::run((w[3].re * tmp_47  + w[3].im * tmp_44 ) )  ;
res[5].im =  narrow<NORMALNARROW,Q31>::run((w[4].re *  narrow<NORMALNARROW,Q31>::run(tmp_49)    + w[4].im *  narrow<NORMALNARROW,Q31>::run(tmp_48)   ) )  ;
res[6].im =  narrow<NORMALNARROW,Q31>::run((w[5].re * tmp_51  + w[5].im * tmp_50 ) )  ;
res[7].im =  narrow<NORMALNARROW,Q31>::run((w[6].re *  narrow<NORMALNARROW,Q31>::run(tmp_53)    + w[6].im *  narrow<NORMALNARROW,Q31>::run(tmp_52)   ) )  ;


memory<complex<Q31>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<Q31>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<Q31>>::store(out,2 * stageLength * strideFactor , res[2]);
memory<complex<Q31>>::store(out,3 * stageLength * strideFactor , res[3]);
memory<complex<Q31>>::store(out,4 * stageLength * strideFactor , res[4]);
memory<complex<Q31>>::store(out,5 * stageLength * strideFactor , res[5]);
memory<complex<Q31>>::store(out,6 * stageLength * strideFactor , res[6]);
memory<complex<Q31>>::store(out,7 * stageLength * strideFactor , res[7]);


};

/* Nb Multiplications = 4, 
Nb Additions = 52,
Nb Negates = 26,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 3

*/
 __STATIC_FORCEINLINE void run_ip_last(complex<Q31> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q31> a[8];
complex<Q31> res[8];


 Q31 tmp_0;
 Q31 tmp_1;
 Q31 tmp_2;
 Q31 tmp_3;
 Q31 tmp_4;
 Q31 tmp_5;
 Q31 tmp_6;
 Q31 tmp_7;
 Q31 tmp_8;
 Q31 tmp_9;
 Q31 tmp_10;
 Q31 tmp_11;
 Q31 tmp_12;
 Q31 tmp_13;
 Q31 tmp_14;
 Q31 tmp_15;
 Q63 tmp_16;
 Q31 tmp_17;
 Q31 tmp_18;
 Q31 tmp_19;
 Q31 tmp_20;
 Q31 tmp_21;
 Q31 tmp_22;
 Q31 tmp_23;
 Q31 tmp_24;
 Q63 tmp_25;
 Q31 tmp_26;
 Q31 tmp_27;
 Q31 tmp_28;
 Q31 tmp_29;
 Q31 tmp_30;
 Q31 tmp_31;
 Q31 tmp_32;
 Q31 tmp_33;
 Q63 tmp_34;
 Q31 tmp_35;
 Q31 tmp_36;
 Q31 tmp_37;
 Q31 tmp_38;
 Q63 tmp_39;


a[0] = memory<complex<Q31>>::load(in,0 * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * strideFactor);
a[2] = memory<complex<Q31>>::load(in,2 * strideFactor);
a[3] = memory<complex<Q31>>::load(in,3 * strideFactor);
a[4] = memory<complex<Q31>>::load(in,4 * strideFactor);
a[5] = memory<complex<Q31>>::load(in,5 * strideFactor);
a[6] = memory<complex<Q31>>::load(in,6 * strideFactor);
a[7] = memory<complex<Q31>>::load(in,7 * strideFactor);


tmp_0 = ( decq(a[0].re,1)   +  decq(a[4].re,1)  ) ;
tmp_1 = ( decq(a[2].re,1)   +  decq(a[6].re,1)  ) ;
tmp_2 = ( decq(tmp_0,1)   +  decq(tmp_1,1)  ) ;
tmp_3 = ( decq(a[1].re,1)   +  decq(a[5].re,1)  ) ;
tmp_4 = ( decq(a[3].re,1)   +  decq(a[7].re,1)  ) ;
tmp_5 = ( decq(tmp_3,1)   +  decq(tmp_4,1)  ) ;
res[0].re = ( decq(tmp_2,1)   +  decq(tmp_5,1)  ) ;
tmp_6 = ( decq(a[0].re,1)   -  decq(a[4].re,1)  ) ;
tmp_7 = ( decq(a[2].im,1)   -  decq(a[6].im,1)  ) ;
tmp_8 = ( decq(tmp_6,1)   +  decq(tmp_7,1)  ) ;
tmp_9 = ( decq(a[1].re,1)   -  decq(a[5].re,1)  ) ;
tmp_10 = ( decq(a[1].im,1)   -  decq(a[5].im,1)  ) ;
tmp_11 = ( decq(tmp_9,1)   +  decq(tmp_10,1)  ) ;
tmp_12 = ( decq(a[7].re,1)   -  decq(a[3].re,1)  ) ;
tmp_13 = ( decq(a[3].im,1)   -  decq(a[7].im,1)  ) ;
tmp_14 = ( decq(tmp_12,1)   +  decq(tmp_13,1)  ) ;
tmp_15 = ( decq(tmp_11,1)   +  decq(tmp_14,1)  ) ;
tmp_16 = (C0_8_notw) * tmp_15 ;
res[1].re =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_8)  ,1)   + tmp_16) )  ;
tmp_17 = ( decq(tmp_0,1)   -  decq(tmp_1,1)  ) ;
tmp_18 = ( decq(a[1].im,1)   +  decq(a[5].im,1)  ) ;
tmp_19 = ( decq(a[3].im,1)   +  decq(a[7].im,1)  ) ;
tmp_20 = ( decq(tmp_18,1)   -  decq(tmp_19,1)  ) ;
res[2].re = ( decq(tmp_17,1)   +  decq(tmp_20,1)  ) ;
tmp_21 = ( decq(tmp_6,1)   -  decq(tmp_7,1)  ) ;
tmp_22 = ( decq(tmp_10,1)   -  decq(tmp_9,1)  ) ;
tmp_23 = ( decq(tmp_12,1)   -  decq(tmp_13,1)  ) ;
tmp_24 = ( decq(tmp_22,1)   -  decq(tmp_23,1)  ) ;
tmp_25 = (C0_8_notw) * tmp_24 ;
res[3].re =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_21)  ,1)   + tmp_25) )  ;
res[4].re = ( decq(tmp_2,1)   -  decq(tmp_5,1)  ) ;
res[5].re =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_8)  ,1)   - tmp_16) )  ;
res[6].re = ( decq(tmp_17,1)   -  decq(tmp_20,1)  ) ;
res[7].re =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_21)  ,1)   - tmp_25) )  ;
tmp_26 = ( decq(a[0].im,1)   +  decq(a[4].im,1)  ) ;
tmp_27 = ( decq(a[2].im,1)   +  decq(a[6].im,1)  ) ;
tmp_28 = ( decq(tmp_26,1)   +  decq(tmp_27,1)  ) ;
tmp_29 = ( decq(tmp_18,1)   +  decq(tmp_19,1)  ) ;
res[0].im = ( decq(tmp_28,1)   +  decq(tmp_29,1)  ) ;
tmp_30 = ( decq(a[0].im,1)   -  decq(a[4].im,1)  ) ;
tmp_31 = ( decq(a[2].re,1)   -  decq(a[6].re,1)  ) ;
tmp_32 = ( decq(tmp_30,1)   -  decq(tmp_31,1)  ) ;
tmp_33 = ( decq(tmp_23,1)   +  decq(tmp_22,1)  ) ;
tmp_34 = (C0_8_notw) * tmp_33 ;
res[1].im =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_32)  ,1)   + tmp_34) )  ;
tmp_35 = ( decq(tmp_26,1)   -  decq(tmp_27,1)  ) ;
tmp_36 = ( decq(tmp_3,1)   -  decq(tmp_4,1)  ) ;
res[2].im = ( decq(tmp_35,1)   -  decq(tmp_36,1)  ) ;
tmp_37 = ( decq(tmp_31,1)   +  decq(tmp_30,1)  ) ;
tmp_38 = ( decq(tmp_11,1)   -  decq(tmp_14,1)  ) ;
tmp_39 = (C0_8_notw) * tmp_38 ;
res[3].im =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_37)  ,1)   - tmp_39) )  ;
res[4].im = ( decq(tmp_28,1)   -  decq(tmp_29,1)  ) ;
res[5].im =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_32)  ,1)   - tmp_34) )  ;
res[6].im = ( decq(tmp_36,1)   +  decq(tmp_35,1)  ) ;
res[7].im =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_37)  ,1)   + tmp_39) )  ;


memory<complex<Q31>>::store(in,0 * strideFactor , res[0]);
memory<complex<Q31>>::store(in,1 * strideFactor , res[1]);
memory<complex<Q31>>::store(in,2 * strideFactor , res[2]);
memory<complex<Q31>>::store(in,3 * strideFactor , res[3]);
memory<complex<Q31>>::store(in,4 * strideFactor , res[4]);
memory<complex<Q31>>::store(in,5 * strideFactor , res[5]);
memory<complex<Q31>>::store(in,6 * strideFactor , res[6]);
memory<complex<Q31>>::store(in,7 * strideFactor , res[7]);


};

/* Nb Multiplications = 4, 
Nb Additions = 52,
Nb Negates = 26,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 3

*/
 __STATIC_FORCEINLINE void run_op_last(const complex<Q31> *in,complex<Q31> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q31> a[8];
complex<Q31> res[8];


 Q31 tmp_0;
 Q31 tmp_1;
 Q31 tmp_2;
 Q31 tmp_3;
 Q31 tmp_4;
 Q31 tmp_5;
 Q31 tmp_6;
 Q31 tmp_7;
 Q31 tmp_8;
 Q31 tmp_9;
 Q31 tmp_10;
 Q31 tmp_11;
 Q31 tmp_12;
 Q31 tmp_13;
 Q31 tmp_14;
 Q31 tmp_15;
 Q63 tmp_16;
 Q31 tmp_17;
 Q31 tmp_18;
 Q31 tmp_19;
 Q31 tmp_20;
 Q31 tmp_21;
 Q31 tmp_22;
 Q31 tmp_23;
 Q31 tmp_24;
 Q63 tmp_25;
 Q31 tmp_26;
 Q31 tmp_27;
 Q31 tmp_28;
 Q31 tmp_29;
 Q31 tmp_30;
 Q31 tmp_31;
 Q31 tmp_32;
 Q31 tmp_33;
 Q63 tmp_34;
 Q31 tmp_35;
 Q31 tmp_36;
 Q31 tmp_37;
 Q31 tmp_38;
 Q63 tmp_39;


a[0] = memory<complex<Q31>>::load(in,0 * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * strideFactor);
a[2] = memory<complex<Q31>>::load(in,2 * strideFactor);
a[3] = memory<complex<Q31>>::load(in,3 * strideFactor);
a[4] = memory<complex<Q31>>::load(in,4 * strideFactor);
a[5] = memory<complex<Q31>>::load(in,5 * strideFactor);
a[6] = memory<complex<Q31>>::load(in,6 * strideFactor);
a[7] = memory<complex<Q31>>::load(in,7 * strideFactor);


tmp_0 = ( decq(a[0].re,1)   +  decq(a[4].re,1)  ) ;
tmp_1 = ( decq(a[2].re,1)   +  decq(a[6].re,1)  ) ;
tmp_2 = ( decq(tmp_0,1)   +  decq(tmp_1,1)  ) ;
tmp_3 = ( decq(a[1].re,1)   +  decq(a[5].re,1)  ) ;
tmp_4 = ( decq(a[3].re,1)   +  decq(a[7].re,1)  ) ;
tmp_5 = ( decq(tmp_3,1)   +  decq(tmp_4,1)  ) ;
res[0].re = ( decq(tmp_2,1)   +  decq(tmp_5,1)  ) ;
tmp_6 = ( decq(a[0].re,1)   -  decq(a[4].re,1)  ) ;
tmp_7 = ( decq(a[2].im,1)   -  decq(a[6].im,1)  ) ;
tmp_8 = ( decq(tmp_6,1)   +  decq(tmp_7,1)  ) ;
tmp_9 = ( decq(a[1].re,1)   -  decq(a[5].re,1)  ) ;
tmp_10 = ( decq(a[1].im,1)   -  decq(a[5].im,1)  ) ;
tmp_11 = ( decq(tmp_9,1)   +  decq(tmp_10,1)  ) ;
tmp_12 = ( decq(a[7].re,1)   -  decq(a[3].re,1)  ) ;
tmp_13 = ( decq(a[3].im,1)   -  decq(a[7].im,1)  ) ;
tmp_14 = ( decq(tmp_12,1)   +  decq(tmp_13,1)  ) ;
tmp_15 = ( decq(tmp_11,1)   +  decq(tmp_14,1)  ) ;
tmp_16 = (C0_8_notw) * tmp_15 ;
res[1].re =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_8)  ,1)   + tmp_16) )  ;
tmp_17 = ( decq(tmp_0,1)   -  decq(tmp_1,1)  ) ;
tmp_18 = ( decq(a[1].im,1)   +  decq(a[5].im,1)  ) ;
tmp_19 = ( decq(a[3].im,1)   +  decq(a[7].im,1)  ) ;
tmp_20 = ( decq(tmp_18,1)   -  decq(tmp_19,1)  ) ;
res[2].re = ( decq(tmp_17,1)   +  decq(tmp_20,1)  ) ;
tmp_21 = ( decq(tmp_6,1)   -  decq(tmp_7,1)  ) ;
tmp_22 = ( decq(tmp_10,1)   -  decq(tmp_9,1)  ) ;
tmp_23 = ( decq(tmp_12,1)   -  decq(tmp_13,1)  ) ;
tmp_24 = ( decq(tmp_22,1)   -  decq(tmp_23,1)  ) ;
tmp_25 = (C0_8_notw) * tmp_24 ;
res[3].re =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_21)  ,1)   + tmp_25) )  ;
res[4].re = ( decq(tmp_2,1)   -  decq(tmp_5,1)  ) ;
res[5].re =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_8)  ,1)   - tmp_16) )  ;
res[6].re = ( decq(tmp_17,1)   -  decq(tmp_20,1)  ) ;
res[7].re =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_21)  ,1)   - tmp_25) )  ;
tmp_26 = ( decq(a[0].im,1)   +  decq(a[4].im,1)  ) ;
tmp_27 = ( decq(a[2].im,1)   +  decq(a[6].im,1)  ) ;
tmp_28 = ( decq(tmp_26,1)   +  decq(tmp_27,1)  ) ;
tmp_29 = ( decq(tmp_18,1)   +  decq(tmp_19,1)  ) ;
res[0].im = ( decq(tmp_28,1)   +  decq(tmp_29,1)  ) ;
tmp_30 = ( decq(a[0].im,1)   -  decq(a[4].im,1)  ) ;
tmp_31 = ( decq(a[2].re,1)   -  decq(a[6].re,1)  ) ;
tmp_32 = ( decq(tmp_30,1)   -  decq(tmp_31,1)  ) ;
tmp_33 = ( decq(tmp_23,1)   +  decq(tmp_22,1)  ) ;
tmp_34 = (C0_8_notw) * tmp_33 ;
res[1].im =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_32)  ,1)   + tmp_34) )  ;
tmp_35 = ( decq(tmp_26,1)   -  decq(tmp_27,1)  ) ;
tmp_36 = ( decq(tmp_3,1)   -  decq(tmp_4,1)  ) ;
res[2].im = ( decq(tmp_35,1)   -  decq(tmp_36,1)  ) ;
tmp_37 = ( decq(tmp_31,1)   +  decq(tmp_30,1)  ) ;
tmp_38 = ( decq(tmp_11,1)   -  decq(tmp_14,1)  ) ;
tmp_39 = (C0_8_notw) * tmp_38 ;
res[3].im =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_37)  ,1)   - tmp_39) )  ;
res[4].im = ( decq(tmp_28,1)   -  decq(tmp_29,1)  ) ;
res[5].im =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_32)  ,1)   - tmp_34) )  ;
res[6].im = ( decq(tmp_36,1)   +  decq(tmp_35,1)  ) ;
res[7].im =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_37)  ,1)   + tmp_39) )  ;


memory<complex<Q31>>::store(out,0 * strideFactor , res[0]);
memory<complex<Q31>>::store(out,1 * strideFactor , res[1]);
memory<complex<Q31>>::store(out,2 * strideFactor , res[2]);
memory<complex<Q31>>::store(out,3 * strideFactor , res[3]);
memory<complex<Q31>>::store(out,4 * strideFactor , res[4]);
memory<complex<Q31>>::store(out,5 * strideFactor , res[5]);
memory<complex<Q31>>::store(out,6 * strideFactor , res[6]);
memory<complex<Q31>>::store(out,7 * strideFactor , res[7]);


};

};

/**************

INVERSE FFT 

***************/
template<int inputstrideenabled> 
struct radix<INVERSE,Q31, 8,inputstrideenabled>{

static constexpr Q31 C0_8_tw{(q31_t)0x5A82799A};
  
static constexpr Q31 C0_8_notw{(q31_t)0x5A82799A};
  

/* Nb Multiplications = 4, 
Nb Additions = 52,
Nb Negates = 26,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 3

*/
 __STATIC_FORCEINLINE void run_ip_notw(int stageLength,complex<Q31> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q31> a[8];
complex<Q31> res[8];


 Q31 tmp_0;
 Q31 tmp_1;
 Q31 tmp_2;
 Q31 tmp_3;
 Q31 tmp_4;
 Q31 tmp_5;
 Q31 tmp_6;
 Q31 tmp_7;
 Q31 tmp_8;
 Q31 tmp_9;
 Q31 tmp_10;
 Q31 tmp_11;
 Q31 tmp_12;
 Q31 tmp_13;
 Q31 tmp_14;
 Q31 tmp_15;
 Q63 tmp_16;
 Q31 tmp_17;
 Q31 tmp_18;
 Q31 tmp_19;
 Q31 tmp_20;
 Q31 tmp_21;
 Q31 tmp_22;
 Q31 tmp_23;
 Q31 tmp_24;
 Q63 tmp_25;
 Q31 tmp_26;
 Q31 tmp_27;
 Q31 tmp_28;
 Q31 tmp_29;
 Q31 tmp_30;
 Q31 tmp_31;
 Q31 tmp_32;
 Q31 tmp_33;
 Q63 tmp_34;
 Q31 tmp_35;
 Q31 tmp_36;
 Q31 tmp_37;
 Q31 tmp_38;
 Q63 tmp_39;


a[0] = memory<complex<Q31>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q31>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q31>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<Q31>>::load(in,4 * stageLength * strideFactor);
a[5] = memory<complex<Q31>>::load(in,5 * stageLength * strideFactor);
a[6] = memory<complex<Q31>>::load(in,6 * stageLength * strideFactor);
a[7] = memory<complex<Q31>>::load(in,7 * stageLength * strideFactor);


tmp_0 = ( decq(a[0].re,1)   +  decq(a[4].re,1)  ) ;
tmp_1 = ( decq(a[2].re,1)   +  decq(a[6].re,1)  ) ;
tmp_2 = ( decq(tmp_0,1)   +  decq(tmp_1,1)  ) ;
tmp_3 = ( decq(a[1].re,1)   +  decq(a[5].re,1)  ) ;
tmp_4 = ( decq(a[3].re,1)   +  decq(a[7].re,1)  ) ;
tmp_5 = ( decq(tmp_3,1)   +  decq(tmp_4,1)  ) ;
res[0].re = ( decq(tmp_2,1)   +  decq(tmp_5,1)  ) ;
tmp_6 = ( decq(a[0].re,1)   -  decq(a[4].re,1)  ) ;
tmp_7 = ( decq(a[2].im,1)   -  decq(a[6].im,1)  ) ;
tmp_8 = ( decq(tmp_6,1)   -  decq(tmp_7,1)  ) ;
tmp_9 = ( decq(a[1].re,1)   -  decq(a[5].re,1)  ) ;
tmp_10 = ( decq(a[1].im,1)   -  decq(a[5].im,1)  ) ;
tmp_11 = ( decq(tmp_9,1)   -  decq(tmp_10,1)  ) ;
tmp_12 = ( decq(a[7].im,1)   -  decq(a[3].im,1)  ) ;
tmp_13 = ( decq(a[3].re,1)   -  decq(a[7].re,1)  ) ;
tmp_14 = ( decq(tmp_12,1)   -  decq(tmp_13,1)  ) ;
tmp_15 = ( decq(tmp_11,1)   +  decq(tmp_14,1)  ) ;
tmp_16 = (C0_8_notw) * tmp_15 ;
res[1].re =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_8)  ,1)   + tmp_16) )  ;
tmp_17 = ( decq(tmp_0,1)   -  decq(tmp_1,1)  ) ;
tmp_18 = ( decq(a[1].im,1)   +  decq(a[5].im,1)  ) ;
tmp_19 = ( decq(a[3].im,1)   +  decq(a[7].im,1)  ) ;
tmp_20 = ( decq(tmp_18,1)   -  decq(tmp_19,1)  ) ;
res[2].re = ( decq(tmp_17,1)   -  decq(tmp_20,1)  ) ;
tmp_21 = ( decq(tmp_6,1)   +  decq(tmp_7,1)  ) ;
tmp_22 = ( decq(tmp_9,1)   +  decq(tmp_10,1)  ) ;
tmp_23 = ( decq(tmp_13,1)   +  decq(tmp_12,1)  ) ;
tmp_24 = ( decq(tmp_22,1)   -  decq(tmp_23,1)  ) ;
tmp_25 = (C0_8_notw) * tmp_24 ;
res[3].re =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_21)  ,1)   - tmp_25) )  ;
res[4].re = ( decq(tmp_2,1)   -  decq(tmp_5,1)  ) ;
res[5].re =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_8)  ,1)   - tmp_16) )  ;
res[6].re = ( decq(tmp_17,1)   +  decq(tmp_20,1)  ) ;
res[7].re =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_21)  ,1)   + tmp_25) )  ;
tmp_26 = ( decq(a[0].im,1)   +  decq(a[4].im,1)  ) ;
tmp_27 = ( decq(a[2].im,1)   +  decq(a[6].im,1)  ) ;
tmp_28 = ( decq(tmp_26,1)   +  decq(tmp_27,1)  ) ;
tmp_29 = ( decq(tmp_18,1)   +  decq(tmp_19,1)  ) ;
res[0].im = ( decq(tmp_28,1)   +  decq(tmp_29,1)  ) ;
tmp_30 = ( decq(a[2].re,1)   -  decq(a[6].re,1)  ) ;
tmp_31 = ( decq(a[0].im,1)   -  decq(a[4].im,1)  ) ;
tmp_32 = ( decq(tmp_30,1)   +  decq(tmp_31,1)  ) ;
tmp_33 = ( decq(tmp_22,1)   +  decq(tmp_23,1)  ) ;
tmp_34 = (C0_8_notw) * tmp_33 ;
res[1].im =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_32)  ,1)   + tmp_34) )  ;
tmp_35 = ( decq(tmp_3,1)   -  decq(tmp_4,1)  ) ;
tmp_36 = ( decq(tmp_26,1)   -  decq(tmp_27,1)  ) ;
res[2].im = ( decq(tmp_35,1)   +  decq(tmp_36,1)  ) ;
tmp_37 = ( decq(tmp_31,1)   -  decq(tmp_30,1)  ) ;
tmp_38 = ( decq(tmp_11,1)   -  decq(tmp_14,1)  ) ;
tmp_39 = (C0_8_notw) * tmp_38 ;
res[3].im =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_37)  ,1)   + tmp_39) )  ;
res[4].im = ( decq(tmp_28,1)   -  decq(tmp_29,1)  ) ;
res[5].im =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_32)  ,1)   - tmp_34) )  ;
res[6].im = ( decq(tmp_36,1)   -  decq(tmp_35,1)  ) ;
res[7].im =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_37)  ,1)   - tmp_39) )  ;


memory<complex<Q31>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<Q31>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<Q31>>::store(in,2 * stageLength * strideFactor , res[2]);
memory<complex<Q31>>::store(in,3 * stageLength * strideFactor , res[3]);
memory<complex<Q31>>::store(in,4 * stageLength * strideFactor , res[4]);
memory<complex<Q31>>::store(in,5 * stageLength * strideFactor , res[5]);
memory<complex<Q31>>::store(in,6 * stageLength * strideFactor , res[6]);
memory<complex<Q31>>::store(in,7 * stageLength * strideFactor , res[7]);


};

/* Nb Multiplications = 32, 
Nb Additions = 66,
Nb Negates = 33,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 3

*/
 __STATIC_FORCEINLINE void run_ip_tw(int stageLength,const complex<Q31> *w,complex<Q31> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q31> a[8];
complex<Q31> res[8];


 Q31 tmp_0;
 Q31 tmp_1;
 Q31 tmp_2;
 Q31 tmp_3;
 Q31 tmp_4;
 Q31 tmp_5;
 Q31 tmp_6;
 Q31 tmp_7;
 Q31 tmp_8;
 Q31 tmp_9;
 Q31 tmp_10;
 Q31 tmp_11;
 Q31 tmp_12;
 Q31 tmp_13;
 Q31 tmp_14;
 Q31 tmp_15;
 Q63 tmp_16;
 Q63 tmp_17;
 Q31 tmp_18;
 Q31 tmp_19;
 Q31 tmp_20;
 Q31 tmp_21;
 Q31 tmp_22;
 Q31 tmp_23;
 Q63 tmp_24;
 Q63 tmp_25;
 Q31 tmp_26;
 Q31 tmp_27;
 Q31 tmp_28;
 Q31 tmp_29;
 Q31 tmp_30;
 Q31 tmp_31;
 Q31 tmp_32;
 Q31 tmp_33;
 Q31 tmp_34;
 Q31 tmp_35;
 Q31 tmp_36;
 Q31 tmp_37;
 Q63 tmp_38;
 Q63 tmp_39;
 Q31 tmp_40;
 Q31 tmp_41;
 Q63 tmp_42;
 Q63 tmp_43;
 Q31 tmp_44;
 Q31 tmp_45;
 Q31 tmp_46;
 Q31 tmp_47;
 Q63 tmp_48;
 Q63 tmp_49;
 Q31 tmp_50;
 Q31 tmp_51;
 Q63 tmp_52;
 Q63 tmp_53;


a[0] = memory<complex<Q31>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q31>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q31>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<Q31>>::load(in,4 * stageLength * strideFactor);
a[5] = memory<complex<Q31>>::load(in,5 * stageLength * strideFactor);
a[6] = memory<complex<Q31>>::load(in,6 * stageLength * strideFactor);
a[7] = memory<complex<Q31>>::load(in,7 * stageLength * strideFactor);


tmp_0 = ( decq(a[0].re,1)   +  decq(a[4].re,1)  ) ;
tmp_1 = ( decq(a[2].re,1)   +  decq(a[6].re,1)  ) ;
tmp_2 = ( decq(tmp_0,1)   +  decq(tmp_1,1)  ) ;
tmp_3 = ( decq(a[1].re,1)   +  decq(a[5].re,1)  ) ;
tmp_4 = ( decq(a[3].re,1)   +  decq(a[7].re,1)  ) ;
tmp_5 = ( decq(tmp_3,1)   +  decq(tmp_4,1)  ) ;
res[0].re = ( decq(tmp_2,1)   +  decq(tmp_5,1)  ) ;
tmp_6 = ( decq(a[0].re,1)   -  decq(a[4].re,1)  ) ;
tmp_7 = ( decq(a[2].im,1)   -  decq(a[6].im,1)  ) ;
tmp_8 = ( decq(tmp_6,1)   -  decq(tmp_7,1)  ) ;
tmp_9 = ( decq(a[1].re,1)   -  decq(a[5].re,1)  ) ;
tmp_10 = ( decq(a[1].im,1)   -  decq(a[5].im,1)  ) ;
tmp_11 = ( decq(tmp_9,1)   -  decq(tmp_10,1)  ) ;
tmp_12 = ( decq(a[7].im,1)   -  decq(a[3].im,1)  ) ;
tmp_13 = ( decq(a[3].re,1)   -  decq(a[7].re,1)  ) ;
tmp_14 = ( decq(tmp_12,1)   -  decq(tmp_13,1)  ) ;
tmp_15 = ( decq(tmp_11,1)   +  decq(tmp_14,1)  ) ;
tmp_16 = (C0_8_tw) * tmp_15 ;
tmp_17 = ( decq( widen<Q31>::run(tmp_8)  ,1)   + tmp_16) ;
tmp_18 = ( decq(a[2].re,1)   -  decq(a[6].re,1)  ) ;
tmp_19 = ( decq(a[0].im,1)   -  decq(a[4].im,1)  ) ;
tmp_20 = ( decq(tmp_18,1)   +  decq(tmp_19,1)  ) ;
tmp_21 = ( decq(tmp_9,1)   +  decq(tmp_10,1)  ) ;
tmp_22 = ( decq(tmp_13,1)   +  decq(tmp_12,1)  ) ;
tmp_23 = ( decq(tmp_21,1)   +  decq(tmp_22,1)  ) ;
tmp_24 = (C0_8_tw) * tmp_23 ;
tmp_25 = ( decq( widen<Q31>::run(tmp_20)  ,1)   + tmp_24) ;
res[1].re =  narrow<NORMALNARROW,Q31>::run((w[0].re *  narrow<NORMALNARROW,Q31>::run(tmp_17)    + w[0].im *  narrow<NORMALNARROW,Q31>::run(tmp_25)   ) )  ;
tmp_26 = ( decq(tmp_0,1)   -  decq(tmp_1,1)  ) ;
tmp_27 = ( decq(a[1].im,1)   +  decq(a[5].im,1)  ) ;
tmp_28 = ( decq(a[3].im,1)   +  decq(a[7].im,1)  ) ;
tmp_29 = ( decq(tmp_27,1)   -  decq(tmp_28,1)  ) ;
tmp_30 = ( decq(tmp_26,1)   -  decq(tmp_29,1)  ) ;
tmp_31 = ( decq(tmp_3,1)   -  decq(tmp_4,1)  ) ;
tmp_32 = ( decq(a[0].im,1)   +  decq(a[4].im,1)  ) ;
tmp_33 = ( decq(a[2].im,1)   +  decq(a[6].im,1)  ) ;
tmp_34 = ( decq(tmp_32,1)   -  decq(tmp_33,1)  ) ;
tmp_35 = ( decq(tmp_31,1)   +  decq(tmp_34,1)  ) ;
res[2].re =  narrow<NORMALNARROW,Q31>::run((w[1].re * tmp_30  + w[1].im * tmp_35 ) )  ;
tmp_36 = ( decq(tmp_6,1)   +  decq(tmp_7,1)  ) ;
tmp_37 = ( decq(tmp_21,1)   -  decq(tmp_22,1)  ) ;
tmp_38 = (C0_8_tw) * tmp_37 ;
tmp_39 = ( decq( widen<Q31>::run(tmp_36)  ,1)   - tmp_38) ;
tmp_40 = ( decq(tmp_19,1)   -  decq(tmp_18,1)  ) ;
tmp_41 = ( decq(tmp_11,1)   -  decq(tmp_14,1)  ) ;
tmp_42 = (C0_8_tw) * tmp_41 ;
tmp_43 = ( decq( widen<Q31>::run(tmp_40)  ,1)   + tmp_42) ;
res[3].re =  narrow<NORMALNARROW,Q31>::run((w[2].re *  narrow<NORMALNARROW,Q31>::run(tmp_39)    + w[2].im *  narrow<NORMALNARROW,Q31>::run(tmp_43)   ) )  ;
tmp_44 = ( decq(tmp_2,1)   -  decq(tmp_5,1)  ) ;
tmp_45 = ( decq(tmp_32,1)   +  decq(tmp_33,1)  ) ;
tmp_46 = ( decq(tmp_27,1)   +  decq(tmp_28,1)  ) ;
tmp_47 = ( decq(tmp_45,1)   -  decq(tmp_46,1)  ) ;
res[4].re =  narrow<NORMALNARROW,Q31>::run((w[3].re * tmp_44  + w[3].im * tmp_47 ) )  ;
tmp_48 = ( decq( widen<Q31>::run(tmp_8)  ,1)   - tmp_16) ;
tmp_49 = ( decq( widen<Q31>::run(tmp_20)  ,1)   - tmp_24) ;
res[5].re =  narrow<NORMALNARROW,Q31>::run((w[4].re *  narrow<NORMALNARROW,Q31>::run(tmp_48)    + w[4].im *  narrow<NORMALNARROW,Q31>::run(tmp_49)   ) )  ;
tmp_50 = ( decq(tmp_26,1)   +  decq(tmp_29,1)  ) ;
tmp_51 = ( decq(tmp_34,1)   -  decq(tmp_31,1)  ) ;
res[6].re =  narrow<NORMALNARROW,Q31>::run((w[5].re * tmp_50  + w[5].im * tmp_51 ) )  ;
tmp_52 = ( decq( widen<Q31>::run(tmp_36)  ,1)   + tmp_38) ;
tmp_53 = ( decq( widen<Q31>::run(tmp_40)  ,1)   - tmp_42) ;
res[7].re =  narrow<NORMALNARROW,Q31>::run((w[6].re *  narrow<NORMALNARROW,Q31>::run(tmp_52)    + w[6].im *  narrow<NORMALNARROW,Q31>::run(tmp_53)   ) )  ;
res[0].im = ( decq(tmp_45,1)   +  decq(tmp_46,1)  ) ;
res[1].im =  narrow<NORMALNARROW,Q31>::run((w[0].re *  narrow<NORMALNARROW,Q31>::run(tmp_25)    - w[0].im *  narrow<NORMALNARROW,Q31>::run(tmp_17)   ) )  ;
res[2].im =  narrow<NORMALNARROW,Q31>::run((w[1].re * tmp_35  - w[1].im * tmp_30 ) )  ;
res[3].im =  narrow<NORMALNARROW,Q31>::run((w[2].re *  narrow<NORMALNARROW,Q31>::run(tmp_43)    - w[2].im *  narrow<NORMALNARROW,Q31>::run(tmp_39)   ) )  ;
res[4].im =  narrow<NORMALNARROW,Q31>::run((w[3].re * tmp_47  - w[3].im * tmp_44 ) )  ;
res[5].im =  narrow<NORMALNARROW,Q31>::run((w[4].re *  narrow<NORMALNARROW,Q31>::run(tmp_49)    - w[4].im *  narrow<NORMALNARROW,Q31>::run(tmp_48)   ) )  ;
res[6].im =  narrow<NORMALNARROW,Q31>::run((w[5].re * tmp_51  - w[5].im * tmp_50 ) )  ;
res[7].im =  narrow<NORMALNARROW,Q31>::run((w[6].re *  narrow<NORMALNARROW,Q31>::run(tmp_53)    - w[6].im *  narrow<NORMALNARROW,Q31>::run(tmp_52)   ) )  ;


memory<complex<Q31>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<Q31>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<Q31>>::store(in,2 * stageLength * strideFactor , res[2]);
memory<complex<Q31>>::store(in,3 * stageLength * strideFactor , res[3]);
memory<complex<Q31>>::store(in,4 * stageLength * strideFactor , res[4]);
memory<complex<Q31>>::store(in,5 * stageLength * strideFactor , res[5]);
memory<complex<Q31>>::store(in,6 * stageLength * strideFactor , res[6]);
memory<complex<Q31>>::store(in,7 * stageLength * strideFactor , res[7]);


};

/* Nb Multiplications = 4, 
Nb Additions = 52,
Nb Negates = 26,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 3

*/
 __STATIC_FORCEINLINE void run_op_notw(int stageLength,const complex<Q31> *in,complex<Q31> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q31> a[8];
complex<Q31> res[8];


 Q31 tmp_0;
 Q31 tmp_1;
 Q31 tmp_2;
 Q31 tmp_3;
 Q31 tmp_4;
 Q31 tmp_5;
 Q31 tmp_6;
 Q31 tmp_7;
 Q31 tmp_8;
 Q31 tmp_9;
 Q31 tmp_10;
 Q31 tmp_11;
 Q31 tmp_12;
 Q31 tmp_13;
 Q31 tmp_14;
 Q31 tmp_15;
 Q63 tmp_16;
 Q31 tmp_17;
 Q31 tmp_18;
 Q31 tmp_19;
 Q31 tmp_20;
 Q31 tmp_21;
 Q31 tmp_22;
 Q31 tmp_23;
 Q31 tmp_24;
 Q63 tmp_25;
 Q31 tmp_26;
 Q31 tmp_27;
 Q31 tmp_28;
 Q31 tmp_29;
 Q31 tmp_30;
 Q31 tmp_31;
 Q31 tmp_32;
 Q31 tmp_33;
 Q63 tmp_34;
 Q31 tmp_35;
 Q31 tmp_36;
 Q31 tmp_37;
 Q31 tmp_38;
 Q63 tmp_39;


a[0] = memory<complex<Q31>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q31>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q31>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<Q31>>::load(in,4 * stageLength * strideFactor);
a[5] = memory<complex<Q31>>::load(in,5 * stageLength * strideFactor);
a[6] = memory<complex<Q31>>::load(in,6 * stageLength * strideFactor);
a[7] = memory<complex<Q31>>::load(in,7 * stageLength * strideFactor);


tmp_0 = ( decq(a[0].re,1)   +  decq(a[4].re,1)  ) ;
tmp_1 = ( decq(a[2].re,1)   +  decq(a[6].re,1)  ) ;
tmp_2 = ( decq(tmp_0,1)   +  decq(tmp_1,1)  ) ;
tmp_3 = ( decq(a[1].re,1)   +  decq(a[5].re,1)  ) ;
tmp_4 = ( decq(a[3].re,1)   +  decq(a[7].re,1)  ) ;
tmp_5 = ( decq(tmp_3,1)   +  decq(tmp_4,1)  ) ;
res[0].re = ( decq(tmp_2,1)   +  decq(tmp_5,1)  ) ;
tmp_6 = ( decq(a[0].re,1)   -  decq(a[4].re,1)  ) ;
tmp_7 = ( decq(a[2].im,1)   -  decq(a[6].im,1)  ) ;
tmp_8 = ( decq(tmp_6,1)   -  decq(tmp_7,1)  ) ;
tmp_9 = ( decq(a[1].re,1)   -  decq(a[5].re,1)  ) ;
tmp_10 = ( decq(a[1].im,1)   -  decq(a[5].im,1)  ) ;
tmp_11 = ( decq(tmp_9,1)   -  decq(tmp_10,1)  ) ;
tmp_12 = ( decq(a[7].im,1)   -  decq(a[3].im,1)  ) ;
tmp_13 = ( decq(a[3].re,1)   -  decq(a[7].re,1)  ) ;
tmp_14 = ( decq(tmp_12,1)   -  decq(tmp_13,1)  ) ;
tmp_15 = ( decq(tmp_11,1)   +  decq(tmp_14,1)  ) ;
tmp_16 = (C0_8_notw) * tmp_15 ;
res[1].re =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_8)  ,1)   + tmp_16) )  ;
tmp_17 = ( decq(tmp_0,1)   -  decq(tmp_1,1)  ) ;
tmp_18 = ( decq(a[1].im,1)   +  decq(a[5].im,1)  ) ;
tmp_19 = ( decq(a[3].im,1)   +  decq(a[7].im,1)  ) ;
tmp_20 = ( decq(tmp_18,1)   -  decq(tmp_19,1)  ) ;
res[2].re = ( decq(tmp_17,1)   -  decq(tmp_20,1)  ) ;
tmp_21 = ( decq(tmp_6,1)   +  decq(tmp_7,1)  ) ;
tmp_22 = ( decq(tmp_9,1)   +  decq(tmp_10,1)  ) ;
tmp_23 = ( decq(tmp_13,1)   +  decq(tmp_12,1)  ) ;
tmp_24 = ( decq(tmp_22,1)   -  decq(tmp_23,1)  ) ;
tmp_25 = (C0_8_notw) * tmp_24 ;
res[3].re =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_21)  ,1)   - tmp_25) )  ;
res[4].re = ( decq(tmp_2,1)   -  decq(tmp_5,1)  ) ;
res[5].re =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_8)  ,1)   - tmp_16) )  ;
res[6].re = ( decq(tmp_17,1)   +  decq(tmp_20,1)  ) ;
res[7].re =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_21)  ,1)   + tmp_25) )  ;
tmp_26 = ( decq(a[0].im,1)   +  decq(a[4].im,1)  ) ;
tmp_27 = ( decq(a[2].im,1)   +  decq(a[6].im,1)  ) ;
tmp_28 = ( decq(tmp_26,1)   +  decq(tmp_27,1)  ) ;
tmp_29 = ( decq(tmp_18,1)   +  decq(tmp_19,1)  ) ;
res[0].im = ( decq(tmp_28,1)   +  decq(tmp_29,1)  ) ;
tmp_30 = ( decq(a[2].re,1)   -  decq(a[6].re,1)  ) ;
tmp_31 = ( decq(a[0].im,1)   -  decq(a[4].im,1)  ) ;
tmp_32 = ( decq(tmp_30,1)   +  decq(tmp_31,1)  ) ;
tmp_33 = ( decq(tmp_22,1)   +  decq(tmp_23,1)  ) ;
tmp_34 = (C0_8_notw) * tmp_33 ;
res[1].im =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_32)  ,1)   + tmp_34) )  ;
tmp_35 = ( decq(tmp_3,1)   -  decq(tmp_4,1)  ) ;
tmp_36 = ( decq(tmp_26,1)   -  decq(tmp_27,1)  ) ;
res[2].im = ( decq(tmp_35,1)   +  decq(tmp_36,1)  ) ;
tmp_37 = ( decq(tmp_31,1)   -  decq(tmp_30,1)  ) ;
tmp_38 = ( decq(tmp_11,1)   -  decq(tmp_14,1)  ) ;
tmp_39 = (C0_8_notw) * tmp_38 ;
res[3].im =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_37)  ,1)   + tmp_39) )  ;
res[4].im = ( decq(tmp_28,1)   -  decq(tmp_29,1)  ) ;
res[5].im =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_32)  ,1)   - tmp_34) )  ;
res[6].im = ( decq(tmp_36,1)   -  decq(tmp_35,1)  ) ;
res[7].im =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_37)  ,1)   - tmp_39) )  ;


memory<complex<Q31>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<Q31>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<Q31>>::store(out,2 * stageLength * strideFactor , res[2]);
memory<complex<Q31>>::store(out,3 * stageLength * strideFactor , res[3]);
memory<complex<Q31>>::store(out,4 * stageLength * strideFactor , res[4]);
memory<complex<Q31>>::store(out,5 * stageLength * strideFactor , res[5]);
memory<complex<Q31>>::store(out,6 * stageLength * strideFactor , res[6]);
memory<complex<Q31>>::store(out,7 * stageLength * strideFactor , res[7]);


};

/* Nb Multiplications = 32, 
Nb Additions = 66,
Nb Negates = 33,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 3

*/
 __STATIC_FORCEINLINE void run_op_tw(int stageLength,const complex<Q31> *w,const complex<Q31> *in,complex<Q31> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q31> a[8];
complex<Q31> res[8];


 Q31 tmp_0;
 Q31 tmp_1;
 Q31 tmp_2;
 Q31 tmp_3;
 Q31 tmp_4;
 Q31 tmp_5;
 Q31 tmp_6;
 Q31 tmp_7;
 Q31 tmp_8;
 Q31 tmp_9;
 Q31 tmp_10;
 Q31 tmp_11;
 Q31 tmp_12;
 Q31 tmp_13;
 Q31 tmp_14;
 Q31 tmp_15;
 Q63 tmp_16;
 Q63 tmp_17;
 Q31 tmp_18;
 Q31 tmp_19;
 Q31 tmp_20;
 Q31 tmp_21;
 Q31 tmp_22;
 Q31 tmp_23;
 Q63 tmp_24;
 Q63 tmp_25;
 Q31 tmp_26;
 Q31 tmp_27;
 Q31 tmp_28;
 Q31 tmp_29;
 Q31 tmp_30;
 Q31 tmp_31;
 Q31 tmp_32;
 Q31 tmp_33;
 Q31 tmp_34;
 Q31 tmp_35;
 Q31 tmp_36;
 Q31 tmp_37;
 Q63 tmp_38;
 Q63 tmp_39;
 Q31 tmp_40;
 Q31 tmp_41;
 Q63 tmp_42;
 Q63 tmp_43;
 Q31 tmp_44;
 Q31 tmp_45;
 Q31 tmp_46;
 Q31 tmp_47;
 Q63 tmp_48;
 Q63 tmp_49;
 Q31 tmp_50;
 Q31 tmp_51;
 Q63 tmp_52;
 Q63 tmp_53;


a[0] = memory<complex<Q31>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q31>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q31>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<Q31>>::load(in,4 * stageLength * strideFactor);
a[5] = memory<complex<Q31>>::load(in,5 * stageLength * strideFactor);
a[6] = memory<complex<Q31>>::load(in,6 * stageLength * strideFactor);
a[7] = memory<complex<Q31>>::load(in,7 * stageLength * strideFactor);


tmp_0 = ( decq(a[0].re,1)   +  decq(a[4].re,1)  ) ;
tmp_1 = ( decq(a[2].re,1)   +  decq(a[6].re,1)  ) ;
tmp_2 = ( decq(tmp_0,1)   +  decq(tmp_1,1)  ) ;
tmp_3 = ( decq(a[1].re,1)   +  decq(a[5].re,1)  ) ;
tmp_4 = ( decq(a[3].re,1)   +  decq(a[7].re,1)  ) ;
tmp_5 = ( decq(tmp_3,1)   +  decq(tmp_4,1)  ) ;
res[0].re = ( decq(tmp_2,1)   +  decq(tmp_5,1)  ) ;
tmp_6 = ( decq(a[0].re,1)   -  decq(a[4].re,1)  ) ;
tmp_7 = ( decq(a[2].im,1)   -  decq(a[6].im,1)  ) ;
tmp_8 = ( decq(tmp_6,1)   -  decq(tmp_7,1)  ) ;
tmp_9 = ( decq(a[1].re,1)   -  decq(a[5].re,1)  ) ;
tmp_10 = ( decq(a[1].im,1)   -  decq(a[5].im,1)  ) ;
tmp_11 = ( decq(tmp_9,1)   -  decq(tmp_10,1)  ) ;
tmp_12 = ( decq(a[7].im,1)   -  decq(a[3].im,1)  ) ;
tmp_13 = ( decq(a[3].re,1)   -  decq(a[7].re,1)  ) ;
tmp_14 = ( decq(tmp_12,1)   -  decq(tmp_13,1)  ) ;
tmp_15 = ( decq(tmp_11,1)   +  decq(tmp_14,1)  ) ;
tmp_16 = (C0_8_tw) * tmp_15 ;
tmp_17 = ( decq( widen<Q31>::run(tmp_8)  ,1)   + tmp_16) ;
tmp_18 = ( decq(a[2].re,1)   -  decq(a[6].re,1)  ) ;
tmp_19 = ( decq(a[0].im,1)   -  decq(a[4].im,1)  ) ;
tmp_20 = ( decq(tmp_18,1)   +  decq(tmp_19,1)  ) ;
tmp_21 = ( decq(tmp_9,1)   +  decq(tmp_10,1)  ) ;
tmp_22 = ( decq(tmp_13,1)   +  decq(tmp_12,1)  ) ;
tmp_23 = ( decq(tmp_21,1)   +  decq(tmp_22,1)  ) ;
tmp_24 = (C0_8_tw) * tmp_23 ;
tmp_25 = ( decq( widen<Q31>::run(tmp_20)  ,1)   + tmp_24) ;
res[1].re =  narrow<NORMALNARROW,Q31>::run((w[0].re *  narrow<NORMALNARROW,Q31>::run(tmp_17)    + w[0].im *  narrow<NORMALNARROW,Q31>::run(tmp_25)   ) )  ;
tmp_26 = ( decq(tmp_0,1)   -  decq(tmp_1,1)  ) ;
tmp_27 = ( decq(a[1].im,1)   +  decq(a[5].im,1)  ) ;
tmp_28 = ( decq(a[3].im,1)   +  decq(a[7].im,1)  ) ;
tmp_29 = ( decq(tmp_27,1)   -  decq(tmp_28,1)  ) ;
tmp_30 = ( decq(tmp_26,1)   -  decq(tmp_29,1)  ) ;
tmp_31 = ( decq(tmp_3,1)   -  decq(tmp_4,1)  ) ;
tmp_32 = ( decq(a[0].im,1)   +  decq(a[4].im,1)  ) ;
tmp_33 = ( decq(a[2].im,1)   +  decq(a[6].im,1)  ) ;
tmp_34 = ( decq(tmp_32,1)   -  decq(tmp_33,1)  ) ;
tmp_35 = ( decq(tmp_31,1)   +  decq(tmp_34,1)  ) ;
res[2].re =  narrow<NORMALNARROW,Q31>::run((w[1].re * tmp_30  + w[1].im * tmp_35 ) )  ;
tmp_36 = ( decq(tmp_6,1)   +  decq(tmp_7,1)  ) ;
tmp_37 = ( decq(tmp_21,1)   -  decq(tmp_22,1)  ) ;
tmp_38 = (C0_8_tw) * tmp_37 ;
tmp_39 = ( decq( widen<Q31>::run(tmp_36)  ,1)   - tmp_38) ;
tmp_40 = ( decq(tmp_19,1)   -  decq(tmp_18,1)  ) ;
tmp_41 = ( decq(tmp_11,1)   -  decq(tmp_14,1)  ) ;
tmp_42 = (C0_8_tw) * tmp_41 ;
tmp_43 = ( decq( widen<Q31>::run(tmp_40)  ,1)   + tmp_42) ;
res[3].re =  narrow<NORMALNARROW,Q31>::run((w[2].re *  narrow<NORMALNARROW,Q31>::run(tmp_39)    + w[2].im *  narrow<NORMALNARROW,Q31>::run(tmp_43)   ) )  ;
tmp_44 = ( decq(tmp_2,1)   -  decq(tmp_5,1)  ) ;
tmp_45 = ( decq(tmp_32,1)   +  decq(tmp_33,1)  ) ;
tmp_46 = ( decq(tmp_27,1)   +  decq(tmp_28,1)  ) ;
tmp_47 = ( decq(tmp_45,1)   -  decq(tmp_46,1)  ) ;
res[4].re =  narrow<NORMALNARROW,Q31>::run((w[3].re * tmp_44  + w[3].im * tmp_47 ) )  ;
tmp_48 = ( decq( widen<Q31>::run(tmp_8)  ,1)   - tmp_16) ;
tmp_49 = ( decq( widen<Q31>::run(tmp_20)  ,1)   - tmp_24) ;
res[5].re =  narrow<NORMALNARROW,Q31>::run((w[4].re *  narrow<NORMALNARROW,Q31>::run(tmp_48)    + w[4].im *  narrow<NORMALNARROW,Q31>::run(tmp_49)   ) )  ;
tmp_50 = ( decq(tmp_26,1)   +  decq(tmp_29,1)  ) ;
tmp_51 = ( decq(tmp_34,1)   -  decq(tmp_31,1)  ) ;
res[6].re =  narrow<NORMALNARROW,Q31>::run((w[5].re * tmp_50  + w[5].im * tmp_51 ) )  ;
tmp_52 = ( decq( widen<Q31>::run(tmp_36)  ,1)   + tmp_38) ;
tmp_53 = ( decq( widen<Q31>::run(tmp_40)  ,1)   - tmp_42) ;
res[7].re =  narrow<NORMALNARROW,Q31>::run((w[6].re *  narrow<NORMALNARROW,Q31>::run(tmp_52)    + w[6].im *  narrow<NORMALNARROW,Q31>::run(tmp_53)   ) )  ;
res[0].im = ( decq(tmp_45,1)   +  decq(tmp_46,1)  ) ;
res[1].im =  narrow<NORMALNARROW,Q31>::run((w[0].re *  narrow<NORMALNARROW,Q31>::run(tmp_25)    - w[0].im *  narrow<NORMALNARROW,Q31>::run(tmp_17)   ) )  ;
res[2].im =  narrow<NORMALNARROW,Q31>::run((w[1].re * tmp_35  - w[1].im * tmp_30 ) )  ;
res[3].im =  narrow<NORMALNARROW,Q31>::run((w[2].re *  narrow<NORMALNARROW,Q31>::run(tmp_43)    - w[2].im *  narrow<NORMALNARROW,Q31>::run(tmp_39)   ) )  ;
res[4].im =  narrow<NORMALNARROW,Q31>::run((w[3].re * tmp_47  - w[3].im * tmp_44 ) )  ;
res[5].im =  narrow<NORMALNARROW,Q31>::run((w[4].re *  narrow<NORMALNARROW,Q31>::run(tmp_49)    - w[4].im *  narrow<NORMALNARROW,Q31>::run(tmp_48)   ) )  ;
res[6].im =  narrow<NORMALNARROW,Q31>::run((w[5].re * tmp_51  - w[5].im * tmp_50 ) )  ;
res[7].im =  narrow<NORMALNARROW,Q31>::run((w[6].re *  narrow<NORMALNARROW,Q31>::run(tmp_53)    - w[6].im *  narrow<NORMALNARROW,Q31>::run(tmp_52)   ) )  ;


memory<complex<Q31>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<Q31>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<Q31>>::store(out,2 * stageLength * strideFactor , res[2]);
memory<complex<Q31>>::store(out,3 * stageLength * strideFactor , res[3]);
memory<complex<Q31>>::store(out,4 * stageLength * strideFactor , res[4]);
memory<complex<Q31>>::store(out,5 * stageLength * strideFactor , res[5]);
memory<complex<Q31>>::store(out,6 * stageLength * strideFactor , res[6]);
memory<complex<Q31>>::store(out,7 * stageLength * strideFactor , res[7]);


};

/* Nb Multiplications = 4, 
Nb Additions = 52,
Nb Negates = 26,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 3

*/
 __STATIC_FORCEINLINE void run_ip_last(complex<Q31> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q31> a[8];
complex<Q31> res[8];


 Q31 tmp_0;
 Q31 tmp_1;
 Q31 tmp_2;
 Q31 tmp_3;
 Q31 tmp_4;
 Q31 tmp_5;
 Q31 tmp_6;
 Q31 tmp_7;
 Q31 tmp_8;
 Q31 tmp_9;
 Q31 tmp_10;
 Q31 tmp_11;
 Q31 tmp_12;
 Q31 tmp_13;
 Q31 tmp_14;
 Q31 tmp_15;
 Q63 tmp_16;
 Q31 tmp_17;
 Q31 tmp_18;
 Q31 tmp_19;
 Q31 tmp_20;
 Q31 tmp_21;
 Q31 tmp_22;
 Q31 tmp_23;
 Q31 tmp_24;
 Q63 tmp_25;
 Q31 tmp_26;
 Q31 tmp_27;
 Q31 tmp_28;
 Q31 tmp_29;
 Q31 tmp_30;
 Q31 tmp_31;
 Q31 tmp_32;
 Q31 tmp_33;
 Q63 tmp_34;
 Q31 tmp_35;
 Q31 tmp_36;
 Q31 tmp_37;
 Q31 tmp_38;
 Q63 tmp_39;


a[0] = memory<complex<Q31>>::load(in,0 * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * strideFactor);
a[2] = memory<complex<Q31>>::load(in,2 * strideFactor);
a[3] = memory<complex<Q31>>::load(in,3 * strideFactor);
a[4] = memory<complex<Q31>>::load(in,4 * strideFactor);
a[5] = memory<complex<Q31>>::load(in,5 * strideFactor);
a[6] = memory<complex<Q31>>::load(in,6 * strideFactor);
a[7] = memory<complex<Q31>>::load(in,7 * strideFactor);


tmp_0 = ( decq(a[0].re,1)   +  decq(a[4].re,1)  ) ;
tmp_1 = ( decq(a[2].re,1)   +  decq(a[6].re,1)  ) ;
tmp_2 = ( decq(tmp_0,1)   +  decq(tmp_1,1)  ) ;
tmp_3 = ( decq(a[1].re,1)   +  decq(a[5].re,1)  ) ;
tmp_4 = ( decq(a[3].re,1)   +  decq(a[7].re,1)  ) ;
tmp_5 = ( decq(tmp_3,1)   +  decq(tmp_4,1)  ) ;
res[0].re = ( decq(tmp_2,1)   +  decq(tmp_5,1)  ) ;
tmp_6 = ( decq(a[0].re,1)   -  decq(a[4].re,1)  ) ;
tmp_7 = ( decq(a[2].im,1)   -  decq(a[6].im,1)  ) ;
tmp_8 = ( decq(tmp_6,1)   -  decq(tmp_7,1)  ) ;
tmp_9 = ( decq(a[1].re,1)   -  decq(a[5].re,1)  ) ;
tmp_10 = ( decq(a[1].im,1)   -  decq(a[5].im,1)  ) ;
tmp_11 = ( decq(tmp_9,1)   -  decq(tmp_10,1)  ) ;
tmp_12 = ( decq(a[7].im,1)   -  decq(a[3].im,1)  ) ;
tmp_13 = ( decq(a[3].re,1)   -  decq(a[7].re,1)  ) ;
tmp_14 = ( decq(tmp_12,1)   -  decq(tmp_13,1)  ) ;
tmp_15 = ( decq(tmp_11,1)   +  decq(tmp_14,1)  ) ;
tmp_16 = (C0_8_notw) * tmp_15 ;
res[1].re =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_8)  ,1)   + tmp_16) )  ;
tmp_17 = ( decq(tmp_0,1)   -  decq(tmp_1,1)  ) ;
tmp_18 = ( decq(a[1].im,1)   +  decq(a[5].im,1)  ) ;
tmp_19 = ( decq(a[3].im,1)   +  decq(a[7].im,1)  ) ;
tmp_20 = ( decq(tmp_18,1)   -  decq(tmp_19,1)  ) ;
res[2].re = ( decq(tmp_17,1)   -  decq(tmp_20,1)  ) ;
tmp_21 = ( decq(tmp_6,1)   +  decq(tmp_7,1)  ) ;
tmp_22 = ( decq(tmp_9,1)   +  decq(tmp_10,1)  ) ;
tmp_23 = ( decq(tmp_13,1)   +  decq(tmp_12,1)  ) ;
tmp_24 = ( decq(tmp_22,1)   -  decq(tmp_23,1)  ) ;
tmp_25 = (C0_8_notw) * tmp_24 ;
res[3].re =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_21)  ,1)   - tmp_25) )  ;
res[4].re = ( decq(tmp_2,1)   -  decq(tmp_5,1)  ) ;
res[5].re =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_8)  ,1)   - tmp_16) )  ;
res[6].re = ( decq(tmp_17,1)   +  decq(tmp_20,1)  ) ;
res[7].re =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_21)  ,1)   + tmp_25) )  ;
tmp_26 = ( decq(a[0].im,1)   +  decq(a[4].im,1)  ) ;
tmp_27 = ( decq(a[2].im,1)   +  decq(a[6].im,1)  ) ;
tmp_28 = ( decq(tmp_26,1)   +  decq(tmp_27,1)  ) ;
tmp_29 = ( decq(tmp_18,1)   +  decq(tmp_19,1)  ) ;
res[0].im = ( decq(tmp_28,1)   +  decq(tmp_29,1)  ) ;
tmp_30 = ( decq(a[2].re,1)   -  decq(a[6].re,1)  ) ;
tmp_31 = ( decq(a[0].im,1)   -  decq(a[4].im,1)  ) ;
tmp_32 = ( decq(tmp_30,1)   +  decq(tmp_31,1)  ) ;
tmp_33 = ( decq(tmp_22,1)   +  decq(tmp_23,1)  ) ;
tmp_34 = (C0_8_notw) * tmp_33 ;
res[1].im =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_32)  ,1)   + tmp_34) )  ;
tmp_35 = ( decq(tmp_3,1)   -  decq(tmp_4,1)  ) ;
tmp_36 = ( decq(tmp_26,1)   -  decq(tmp_27,1)  ) ;
res[2].im = ( decq(tmp_35,1)   +  decq(tmp_36,1)  ) ;
tmp_37 = ( decq(tmp_31,1)   -  decq(tmp_30,1)  ) ;
tmp_38 = ( decq(tmp_11,1)   -  decq(tmp_14,1)  ) ;
tmp_39 = (C0_8_notw) * tmp_38 ;
res[3].im =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_37)  ,1)   + tmp_39) )  ;
res[4].im = ( decq(tmp_28,1)   -  decq(tmp_29,1)  ) ;
res[5].im =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_32)  ,1)   - tmp_34) )  ;
res[6].im = ( decq(tmp_36,1)   -  decq(tmp_35,1)  ) ;
res[7].im =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_37)  ,1)   - tmp_39) )  ;


memory<complex<Q31>>::store(in,0 * strideFactor , res[0]);
memory<complex<Q31>>::store(in,1 * strideFactor , res[1]);
memory<complex<Q31>>::store(in,2 * strideFactor , res[2]);
memory<complex<Q31>>::store(in,3 * strideFactor , res[3]);
memory<complex<Q31>>::store(in,4 * strideFactor , res[4]);
memory<complex<Q31>>::store(in,5 * strideFactor , res[5]);
memory<complex<Q31>>::store(in,6 * strideFactor , res[6]);
memory<complex<Q31>>::store(in,7 * strideFactor , res[7]);


};

/* Nb Multiplications = 4, 
Nb Additions = 52,
Nb Negates = 26,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 3

*/
 __STATIC_FORCEINLINE void run_op_last(const complex<Q31> *in,complex<Q31> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q31> a[8];
complex<Q31> res[8];


 Q31 tmp_0;
 Q31 tmp_1;
 Q31 tmp_2;
 Q31 tmp_3;
 Q31 tmp_4;
 Q31 tmp_5;
 Q31 tmp_6;
 Q31 tmp_7;
 Q31 tmp_8;
 Q31 tmp_9;
 Q31 tmp_10;
 Q31 tmp_11;
 Q31 tmp_12;
 Q31 tmp_13;
 Q31 tmp_14;
 Q31 tmp_15;
 Q63 tmp_16;
 Q31 tmp_17;
 Q31 tmp_18;
 Q31 tmp_19;
 Q31 tmp_20;
 Q31 tmp_21;
 Q31 tmp_22;
 Q31 tmp_23;
 Q31 tmp_24;
 Q63 tmp_25;
 Q31 tmp_26;
 Q31 tmp_27;
 Q31 tmp_28;
 Q31 tmp_29;
 Q31 tmp_30;
 Q31 tmp_31;
 Q31 tmp_32;
 Q31 tmp_33;
 Q63 tmp_34;
 Q31 tmp_35;
 Q31 tmp_36;
 Q31 tmp_37;
 Q31 tmp_38;
 Q63 tmp_39;


a[0] = memory<complex<Q31>>::load(in,0 * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * strideFactor);
a[2] = memory<complex<Q31>>::load(in,2 * strideFactor);
a[3] = memory<complex<Q31>>::load(in,3 * strideFactor);
a[4] = memory<complex<Q31>>::load(in,4 * strideFactor);
a[5] = memory<complex<Q31>>::load(in,5 * strideFactor);
a[6] = memory<complex<Q31>>::load(in,6 * strideFactor);
a[7] = memory<complex<Q31>>::load(in,7 * strideFactor);


tmp_0 = ( decq(a[0].re,1)   +  decq(a[4].re,1)  ) ;
tmp_1 = ( decq(a[2].re,1)   +  decq(a[6].re,1)  ) ;
tmp_2 = ( decq(tmp_0,1)   +  decq(tmp_1,1)  ) ;
tmp_3 = ( decq(a[1].re,1)   +  decq(a[5].re,1)  ) ;
tmp_4 = ( decq(a[3].re,1)   +  decq(a[7].re,1)  ) ;
tmp_5 = ( decq(tmp_3,1)   +  decq(tmp_4,1)  ) ;
res[0].re = ( decq(tmp_2,1)   +  decq(tmp_5,1)  ) ;
tmp_6 = ( decq(a[0].re,1)   -  decq(a[4].re,1)  ) ;
tmp_7 = ( decq(a[2].im,1)   -  decq(a[6].im,1)  ) ;
tmp_8 = ( decq(tmp_6,1)   -  decq(tmp_7,1)  ) ;
tmp_9 = ( decq(a[1].re,1)   -  decq(a[5].re,1)  ) ;
tmp_10 = ( decq(a[1].im,1)   -  decq(a[5].im,1)  ) ;
tmp_11 = ( decq(tmp_9,1)   -  decq(tmp_10,1)  ) ;
tmp_12 = ( decq(a[7].im,1)   -  decq(a[3].im,1)  ) ;
tmp_13 = ( decq(a[3].re,1)   -  decq(a[7].re,1)  ) ;
tmp_14 = ( decq(tmp_12,1)   -  decq(tmp_13,1)  ) ;
tmp_15 = ( decq(tmp_11,1)   +  decq(tmp_14,1)  ) ;
tmp_16 = (C0_8_notw) * tmp_15 ;
res[1].re =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_8)  ,1)   + tmp_16) )  ;
tmp_17 = ( decq(tmp_0,1)   -  decq(tmp_1,1)  ) ;
tmp_18 = ( decq(a[1].im,1)   +  decq(a[5].im,1)  ) ;
tmp_19 = ( decq(a[3].im,1)   +  decq(a[7].im,1)  ) ;
tmp_20 = ( decq(tmp_18,1)   -  decq(tmp_19,1)  ) ;
res[2].re = ( decq(tmp_17,1)   -  decq(tmp_20,1)  ) ;
tmp_21 = ( decq(tmp_6,1)   +  decq(tmp_7,1)  ) ;
tmp_22 = ( decq(tmp_9,1)   +  decq(tmp_10,1)  ) ;
tmp_23 = ( decq(tmp_13,1)   +  decq(tmp_12,1)  ) ;
tmp_24 = ( decq(tmp_22,1)   -  decq(tmp_23,1)  ) ;
tmp_25 = (C0_8_notw) * tmp_24 ;
res[3].re =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_21)  ,1)   - tmp_25) )  ;
res[4].re = ( decq(tmp_2,1)   -  decq(tmp_5,1)  ) ;
res[5].re =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_8)  ,1)   - tmp_16) )  ;
res[6].re = ( decq(tmp_17,1)   +  decq(tmp_20,1)  ) ;
res[7].re =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_21)  ,1)   + tmp_25) )  ;
tmp_26 = ( decq(a[0].im,1)   +  decq(a[4].im,1)  ) ;
tmp_27 = ( decq(a[2].im,1)   +  decq(a[6].im,1)  ) ;
tmp_28 = ( decq(tmp_26,1)   +  decq(tmp_27,1)  ) ;
tmp_29 = ( decq(tmp_18,1)   +  decq(tmp_19,1)  ) ;
res[0].im = ( decq(tmp_28,1)   +  decq(tmp_29,1)  ) ;
tmp_30 = ( decq(a[2].re,1)   -  decq(a[6].re,1)  ) ;
tmp_31 = ( decq(a[0].im,1)   -  decq(a[4].im,1)  ) ;
tmp_32 = ( decq(tmp_30,1)   +  decq(tmp_31,1)  ) ;
tmp_33 = ( decq(tmp_22,1)   +  decq(tmp_23,1)  ) ;
tmp_34 = (C0_8_notw) * tmp_33 ;
res[1].im =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_32)  ,1)   + tmp_34) )  ;
tmp_35 = ( decq(tmp_3,1)   -  decq(tmp_4,1)  ) ;
tmp_36 = ( decq(tmp_26,1)   -  decq(tmp_27,1)  ) ;
res[2].im = ( decq(tmp_35,1)   +  decq(tmp_36,1)  ) ;
tmp_37 = ( decq(tmp_31,1)   -  decq(tmp_30,1)  ) ;
tmp_38 = ( decq(tmp_11,1)   -  decq(tmp_14,1)  ) ;
tmp_39 = (C0_8_notw) * tmp_38 ;
res[3].im =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_37)  ,1)   + tmp_39) )  ;
res[4].im = ( decq(tmp_28,1)   -  decq(tmp_29,1)  ) ;
res[5].im =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_32)  ,1)   - tmp_34) )  ;
res[6].im = ( decq(tmp_36,1)   -  decq(tmp_35,1)  ) ;
res[7].im =  narrow<NORMALNARROW,Q31>::run(( decq( widen<Q31>::run(tmp_37)  ,1)   - tmp_39) )  ;


memory<complex<Q31>>::store(out,0 * strideFactor , res[0]);
memory<complex<Q31>>::store(out,1 * strideFactor , res[1]);
memory<complex<Q31>>::store(out,2 * strideFactor , res[2]);
memory<complex<Q31>>::store(out,3 * strideFactor , res[3]);
memory<complex<Q31>>::store(out,4 * strideFactor , res[4]);
memory<complex<Q31>>::store(out,5 * strideFactor , res[5]);
memory<complex<Q31>>::store(out,6 * strideFactor , res[6]);
memory<complex<Q31>>::store(out,7 * strideFactor , res[7]);


};

};

#endif /* #ifndef _RADIX_Q31_8_H */