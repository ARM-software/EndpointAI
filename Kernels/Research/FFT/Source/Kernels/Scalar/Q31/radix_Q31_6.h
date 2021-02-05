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

#ifndef _RADIX_Q31_6_H
#define _RADIX_Q31_6_H

#include "complex.h"

/**************

DIRECT FFT 

***************/
template<int inputstrideenabled> 
struct radix<DIRECT,Q31, 6,inputstrideenabled>{

static constexpr Q31 C0_6_tw{(q31_t)0x6ED9EBA1};
  static constexpr Q31 C1_6_tw{(q31_t)0x40000000};
  
static constexpr Q31 C0_6_notw{(q31_t)0x6ED9EBA1};
  static constexpr Q31 C1_6_notw{(q31_t)0x40000000};
  

/* Nb Multiplications = 20, 
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


complex<Q31> a[6];
complex<Q31> res[6];


 Q31 tmp_0;
 Q31 tmp_1;
 Q31 tmp_2;
 Q31 tmp_3;
 Q63 tmp_4;
 Q31 tmp_5;
 Q63 tmp_6;
 Q63 tmp_7;
 Q31 tmp_8;
 Q63 tmp_9;
 Q31 tmp_10;
 Q63 tmp_11;
 Q63 tmp_12;
 Q31 tmp_13;
 Q31 tmp_14;
 Q31 tmp_15;
 Q31 tmp_16;
 Q63 tmp_17;
 Q63 tmp_18;
 Q63 tmp_19;
 Q63 tmp_20;
 Q63 tmp_21;
 Q63 tmp_22;
 Q63 tmp_23;
 Q63 tmp_24;
 Q63 tmp_25;
 Q31 tmp_26;
 Q31 tmp_27;
 Q31 tmp_28;
 Q63 tmp_29;
 Q63 tmp_30;
 Q63 tmp_31;
 Q31 tmp_32;
 Q63 tmp_33;
 Q63 tmp_34;
 Q63 tmp_35;
 Q63 tmp_36;
 Q63 tmp_37;
 Q31 tmp_38;
 Q63 tmp_39;
 Q63 tmp_40;
 Q63 tmp_41;
 Q63 tmp_42;
 Q63 tmp_43;


a[0] = memory<complex<Q31>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q31>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q31>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<Q31>>::load(in,4 * stageLength * strideFactor);
a[5] = memory<complex<Q31>>::load(in,5 * stageLength * strideFactor);


tmp_0 = ( decq(a[0].re,1)   +  decq(a[3].re,1)  ) ;
tmp_1 = ( decq(a[1].re,1)   +  decq(a[4].re,1)  ) ;
tmp_2 = ( decq(a[2].re,1)   +  decq(a[5].re,1)  ) ;
tmp_32 = ( decq( decq(tmp_0,1)  ,1)   +  decq(( decq(tmp_1,1)   +  decq(tmp_2,1)  ) ,1)  ) ;
res[0].re = tmp_32;
tmp_3 = ( decq(a[1].re,1)   -  decq(a[4].re,1)  ) ;
tmp_4 = (C1_6_notw) * tmp_3 ;
tmp_5 = ( decq(a[1].im,1)   -  decq(a[4].im,1)  ) ;
tmp_6 = (C0_6_notw) * tmp_5 ;
tmp_7 = (tmp_4 + tmp_6) ;
tmp_8 = ( decq(a[2].im,1)   -  decq(a[5].im,1)  ) ;
tmp_9 = (C0_6_notw) * tmp_8 ;
tmp_10 = ( decq(a[2].re,1)   -  decq(a[5].re,1)  ) ;
tmp_11 = (C1_6_notw) * tmp_10 ;
tmp_12 = (tmp_9 - tmp_11) ;
tmp_13 = ( decq(a[0].re,1)   -  decq(a[3].re,1)  ) ;
tmp_33 = (tmp_7 + (tmp_12 +  widen<Q31>::run(tmp_13)  ) ) ;
res[1].re =  narrow<2,Q31>::run(tmp_33)  ;
tmp_14 = ( decq(a[1].im,1)   +  decq(a[4].im,1)  ) ;
tmp_15 = ( decq(a[2].im,1)   +  decq(a[5].im,1)  ) ;
tmp_16 = ( decq(tmp_1,1)   +  decq(tmp_2,1)  ) ;
tmp_17 = (C1_6_notw) * tmp_16 ;
tmp_34 = ( decq( widen<Q31>::run(tmp_0)  ,1)   + ((C0_6_notw) * ( decq(tmp_14,1)   -  decq(tmp_15,1)  )   - tmp_17) ) ;
res[2].re =  narrow<1,Q31>::run(tmp_34)  ;
tmp_18 = (C1_6_notw) * tmp_5 ;
tmp_19 = (C0_6_notw) * tmp_3 ;
tmp_20 = (tmp_18 - tmp_19) ;
tmp_21 = (C1_6_notw) * tmp_8 ;
tmp_22 = (C0_6_notw) * tmp_10 ;
tmp_23 = ( (-(tmp_21))   - tmp_22) ;
tmp_24 = (tmp_7 + tmp_12) ;
tmp_25 = (C1_6_notw) *  narrow<NORMALNARROW,Q31>::run(tmp_24)   ;
tmp_35 = ( widen<Q31>::run(tmp_13)   + ((C0_6_notw) *  narrow<NORMALNARROW,Q31>::run((tmp_20 - tmp_23) )    - tmp_25) ) ;
res[3].re =  narrow<2,Q31>::run(tmp_35)  ;
tmp_36 = ( decq( widen<Q31>::run(tmp_0)  ,1)   + ((C0_6_notw) * ( decq(tmp_15,1)   -  decq(tmp_14,1)  )   - tmp_17) ) ;
res[4].re =  narrow<1,Q31>::run(tmp_36)  ;
tmp_37 = ( widen<Q31>::run(tmp_13)   + ((C0_6_notw) *  narrow<NORMALNARROW,Q31>::run((tmp_23 - tmp_20) )    - tmp_25) ) ;
res[5].re =  narrow<2,Q31>::run(tmp_37)  ;
tmp_26 = ( decq(a[0].im,1)   +  decq(a[3].im,1)  ) ;
tmp_38 = ( decq( decq(tmp_26,1)  ,1)   +  decq(( decq(tmp_14,1)   +  decq(tmp_15,1)  ) ,1)  ) ;
res[0].im = tmp_38;
tmp_27 = ( decq(a[0].im,1)   -  decq(a[3].im,1)  ) ;
tmp_39 = (tmp_20 + (tmp_23 +  widen<Q31>::run(tmp_27)  ) ) ;
res[1].im =  narrow<2,Q31>::run(tmp_39)  ;
tmp_28 = ( decq(tmp_14,1)   +  decq(tmp_15,1)  ) ;
tmp_29 = (C1_6_notw) * tmp_28 ;
tmp_40 = ( decq( widen<Q31>::run(tmp_26)  ,1)   + ((C0_6_notw) * ( decq(tmp_2,1)   -  decq(tmp_1,1)  )   - tmp_29) ) ;
res[2].im =  narrow<1,Q31>::run(tmp_40)  ;
tmp_30 = (tmp_20 + tmp_23) ;
tmp_31 = (C1_6_notw) *  narrow<NORMALNARROW,Q31>::run(tmp_30)   ;
tmp_41 = ( widen<Q31>::run(tmp_27)   + ((C0_6_notw) *  narrow<NORMALNARROW,Q31>::run((tmp_12 - tmp_7) )    - tmp_31) ) ;
res[3].im =  narrow<2,Q31>::run(tmp_41)  ;
tmp_42 = ( decq( widen<Q31>::run(tmp_26)  ,1)   + ((C0_6_notw) * ( decq(tmp_1,1)   -  decq(tmp_2,1)  )   - tmp_29) ) ;
res[4].im =  narrow<1,Q31>::run(tmp_42)  ;
tmp_43 = ( widen<Q31>::run(tmp_27)   + ((C0_6_notw) *  narrow<NORMALNARROW,Q31>::run((tmp_7 - tmp_12) )    - tmp_31) ) ;
res[5].im =  narrow<2,Q31>::run(tmp_43)  ;


memory<complex<Q31>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<Q31>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<Q31>>::store(in,2 * stageLength * strideFactor , res[2]);
memory<complex<Q31>>::store(in,3 * stageLength * strideFactor , res[3]);
memory<complex<Q31>>::store(in,4 * stageLength * strideFactor , res[4]);
memory<complex<Q31>>::store(in,5 * stageLength * strideFactor , res[5]);


};

/* Nb Multiplications = 40, 
Nb Additions = 62,
Nb Negates = 31,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 3

*/
 __STATIC_FORCEINLINE void run_ip_tw(int stageLength,const complex<Q31> *w,complex<Q31> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q31> a[6];
complex<Q31> res[6];


 Q31 tmp_0;
 Q31 tmp_1;
 Q31 tmp_2;
 Q31 tmp_3;
 Q63 tmp_4;
 Q31 tmp_5;
 Q63 tmp_6;
 Q63 tmp_7;
 Q31 tmp_8;
 Q63 tmp_9;
 Q31 tmp_10;
 Q63 tmp_11;
 Q63 tmp_12;
 Q31 tmp_13;
 Q63 tmp_14;
 Q63 tmp_15;
 Q63 tmp_16;
 Q63 tmp_17;
 Q63 tmp_18;
 Q63 tmp_19;
 Q63 tmp_20;
 Q31 tmp_21;
 Q63 tmp_22;
 Q31 tmp_23;
 Q31 tmp_24;
 Q31 tmp_25;
 Q63 tmp_26;
 Q31 tmp_27;
 Q63 tmp_28;
 Q63 tmp_29;
 Q31 tmp_30;
 Q31 tmp_31;
 Q63 tmp_32;
 Q31 tmp_33;
 Q63 tmp_34;
 Q63 tmp_35;
 Q63 tmp_36;
 Q63 tmp_37;
 Q63 tmp_38;
 Q63 tmp_39;
 Q63 tmp_40;
 Q63 tmp_41;
 Q63 tmp_42;
 Q63 tmp_43;
 Q63 tmp_44;
 Q63 tmp_45;
 Q31 tmp_46;
 Q63 tmp_47;
 Q63 tmp_48;
 Q31 tmp_49;
 Q63 tmp_50;
 Q63 tmp_51;
 Q63 tmp_52;
 Q63 tmp_53;
 Q63 tmp_54;
 Q63 tmp_55;
 Q63 tmp_56;
 Q63 tmp_57;
 Q31 tmp_58;
 Q63 tmp_59;
 Q63 tmp_60;
 Q63 tmp_61;
 Q63 tmp_62;
 Q63 tmp_63;
 Q63 tmp_64;
 Q63 tmp_65;
 Q63 tmp_66;
 Q63 tmp_67;
 Q63 tmp_68;
 Q31 tmp_69;


a[0] = memory<complex<Q31>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q31>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q31>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<Q31>>::load(in,4 * stageLength * strideFactor);
a[5] = memory<complex<Q31>>::load(in,5 * stageLength * strideFactor);


tmp_0 = ( decq(a[0].re,1)   +  decq(a[3].re,1)  ) ;
tmp_1 = ( decq(a[1].re,1)   +  decq(a[4].re,1)  ) ;
tmp_2 = ( decq(a[2].re,1)   +  decq(a[5].re,1)  ) ;
tmp_58 = ( decq( decq(tmp_0,1)  ,1)   +  decq(( decq(tmp_1,1)   +  decq(tmp_2,1)  ) ,1)  ) ;
res[0].re = tmp_58;
tmp_3 = ( decq(a[1].re,1)   -  decq(a[4].re,1)  ) ;
tmp_4 = (C1_6_tw) * tmp_3 ;
tmp_5 = ( decq(a[1].im,1)   -  decq(a[4].im,1)  ) ;
tmp_6 = (C0_6_tw) * tmp_5 ;
tmp_7 = (tmp_4 + tmp_6) ;
tmp_8 = ( decq(a[2].im,1)   -  decq(a[5].im,1)  ) ;
tmp_9 = (C0_6_tw) * tmp_8 ;
tmp_10 = ( decq(a[2].re,1)   -  decq(a[5].re,1)  ) ;
tmp_11 = (C1_6_tw) * tmp_10 ;
tmp_12 = (tmp_9 - tmp_11) ;
tmp_13 = ( decq(a[0].re,1)   -  decq(a[3].re,1)  ) ;
tmp_59 = (tmp_7 + (tmp_12 +  widen<Q31>::run(tmp_13)  ) ) ;
tmp_14 = tmp_59;
tmp_15 = (C1_6_tw) * tmp_5 ;
tmp_16 = (C0_6_tw) * tmp_3 ;
tmp_17 = (tmp_15 - tmp_16) ;
tmp_18 = (C1_6_tw) * tmp_8 ;
tmp_19 = (C0_6_tw) * tmp_10 ;
tmp_20 = ( (-(tmp_18))   - tmp_19) ;
tmp_21 = ( decq(a[0].im,1)   -  decq(a[3].im,1)  ) ;
tmp_60 = (tmp_17 + (tmp_20 +  widen<Q31>::run(tmp_21)  ) ) ;
tmp_22 = tmp_60;
res[1].re =  narrow<2,Q31>::run((w[0].re *  narrow<NORMALNARROW,Q31>::run(tmp_14)    - w[0].im *  narrow<NORMALNARROW,Q31>::run(tmp_22)   ) )  ;
tmp_23 = ( decq(a[1].im,1)   +  decq(a[4].im,1)  ) ;
tmp_24 = ( decq(a[2].im,1)   +  decq(a[5].im,1)  ) ;
tmp_25 = ( decq(tmp_23,1)   -  decq(tmp_24,1)  ) ;
tmp_26 = (C0_6_tw) * tmp_25 ;
tmp_27 = ( decq(tmp_1,1)   +  decq(tmp_2,1)  ) ;
tmp_28 = (C1_6_tw) * tmp_27 ;
tmp_61 = ( decq( widen<Q31>::run(tmp_0)  ,1)   + (tmp_26 - tmp_28) ) ;
tmp_29 = tmp_61;
tmp_30 = ( decq(a[0].im,1)   +  decq(a[3].im,1)  ) ;
tmp_31 = ( decq(tmp_2,1)   -  decq(tmp_1,1)  ) ;
tmp_32 = (C0_6_tw) * tmp_31 ;
tmp_33 = ( decq(tmp_23,1)   +  decq(tmp_24,1)  ) ;
tmp_34 = (C1_6_tw) * tmp_33 ;
tmp_62 = ( decq( widen<Q31>::run(tmp_30)  ,1)   + (tmp_32 - tmp_34) ) ;
tmp_35 = tmp_62;
res[2].re =  narrow<1,Q31>::run((w[1].re *  narrow<NORMALNARROW,Q31>::run(tmp_29)    - w[1].im *  narrow<NORMALNARROW,Q31>::run(tmp_35)   ) )  ;
tmp_36 = (tmp_17 - tmp_20) ;
tmp_37 = (C0_6_tw) *  narrow<NORMALNARROW,Q31>::run(tmp_36)   ;
tmp_38 = (tmp_7 + tmp_12) ;
tmp_39 = (C1_6_tw) *  narrow<NORMALNARROW,Q31>::run(tmp_38)   ;
tmp_63 = ( widen<Q31>::run(tmp_13)   + (tmp_37 - tmp_39) ) ;
tmp_40 = tmp_63;
tmp_41 = (tmp_12 - tmp_7) ;
tmp_42 = (C0_6_tw) *  narrow<NORMALNARROW,Q31>::run(tmp_41)   ;
tmp_43 = (tmp_17 + tmp_20) ;
tmp_44 = (C1_6_tw) *  narrow<NORMALNARROW,Q31>::run(tmp_43)   ;
tmp_64 = ( widen<Q31>::run(tmp_21)   + (tmp_42 - tmp_44) ) ;
tmp_45 = tmp_64;
res[3].re =  narrow<2,Q31>::run((w[2].re *  narrow<NORMALNARROW,Q31>::run(tmp_40)    - w[2].im *  narrow<NORMALNARROW,Q31>::run(tmp_45)   ) )  ;
tmp_46 = ( decq(tmp_24,1)   -  decq(tmp_23,1)  ) ;
tmp_47 = (C0_6_tw) * tmp_46 ;
tmp_65 = ( decq( widen<Q31>::run(tmp_0)  ,1)   + (tmp_47 - tmp_28) ) ;
tmp_48 = tmp_65;
tmp_49 = ( decq(tmp_1,1)   -  decq(tmp_2,1)  ) ;
tmp_50 = (C0_6_tw) * tmp_49 ;
tmp_66 = ( decq( widen<Q31>::run(tmp_30)  ,1)   + (tmp_50 - tmp_34) ) ;
tmp_51 = tmp_66;
res[4].re =  narrow<1,Q31>::run((w[3].re *  narrow<NORMALNARROW,Q31>::run(tmp_48)    - w[3].im *  narrow<NORMALNARROW,Q31>::run(tmp_51)   ) )  ;
tmp_52 = (tmp_20 - tmp_17) ;
tmp_53 = (C0_6_tw) *  narrow<NORMALNARROW,Q31>::run(tmp_52)   ;
tmp_67 = ( widen<Q31>::run(tmp_13)   + (tmp_53 - tmp_39) ) ;
tmp_54 = tmp_67;
tmp_55 = (tmp_7 - tmp_12) ;
tmp_56 = (C0_6_tw) *  narrow<NORMALNARROW,Q31>::run(tmp_55)   ;
tmp_68 = ( widen<Q31>::run(tmp_21)   + (tmp_56 - tmp_44) ) ;
tmp_57 = tmp_68;
res[5].re =  narrow<2,Q31>::run((w[4].re *  narrow<NORMALNARROW,Q31>::run(tmp_54)    - w[4].im *  narrow<NORMALNARROW,Q31>::run(tmp_57)   ) )  ;
tmp_69 = ( decq( decq(tmp_30,1)  ,1)   +  decq(( decq(tmp_23,1)   +  decq(tmp_24,1)  ) ,1)  ) ;
res[0].im = tmp_69;
res[1].im =  narrow<2,Q31>::run((w[0].re *  narrow<NORMALNARROW,Q31>::run(tmp_22)    + w[0].im *  narrow<NORMALNARROW,Q31>::run(tmp_14)   ) )  ;
res[2].im =  narrow<1,Q31>::run((w[1].re *  narrow<NORMALNARROW,Q31>::run(tmp_35)    + w[1].im *  narrow<NORMALNARROW,Q31>::run(tmp_29)   ) )  ;
res[3].im =  narrow<2,Q31>::run((w[2].re *  narrow<NORMALNARROW,Q31>::run(tmp_45)    + w[2].im *  narrow<NORMALNARROW,Q31>::run(tmp_40)   ) )  ;
res[4].im =  narrow<1,Q31>::run((w[3].re *  narrow<NORMALNARROW,Q31>::run(tmp_51)    + w[3].im *  narrow<NORMALNARROW,Q31>::run(tmp_48)   ) )  ;
res[5].im =  narrow<2,Q31>::run((w[4].re *  narrow<NORMALNARROW,Q31>::run(tmp_57)    + w[4].im *  narrow<NORMALNARROW,Q31>::run(tmp_54)   ) )  ;


memory<complex<Q31>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<Q31>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<Q31>>::store(in,2 * stageLength * strideFactor , res[2]);
memory<complex<Q31>>::store(in,3 * stageLength * strideFactor , res[3]);
memory<complex<Q31>>::store(in,4 * stageLength * strideFactor , res[4]);
memory<complex<Q31>>::store(in,5 * stageLength * strideFactor , res[5]);


};

/* Nb Multiplications = 20, 
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


complex<Q31> a[6];
complex<Q31> res[6];


 Q31 tmp_0;
 Q31 tmp_1;
 Q31 tmp_2;
 Q31 tmp_3;
 Q63 tmp_4;
 Q31 tmp_5;
 Q63 tmp_6;
 Q63 tmp_7;
 Q31 tmp_8;
 Q63 tmp_9;
 Q31 tmp_10;
 Q63 tmp_11;
 Q63 tmp_12;
 Q31 tmp_13;
 Q31 tmp_14;
 Q31 tmp_15;
 Q31 tmp_16;
 Q63 tmp_17;
 Q63 tmp_18;
 Q63 tmp_19;
 Q63 tmp_20;
 Q63 tmp_21;
 Q63 tmp_22;
 Q63 tmp_23;
 Q63 tmp_24;
 Q63 tmp_25;
 Q31 tmp_26;
 Q31 tmp_27;
 Q31 tmp_28;
 Q63 tmp_29;
 Q63 tmp_30;
 Q63 tmp_31;
 Q31 tmp_32;
 Q63 tmp_33;
 Q63 tmp_34;
 Q63 tmp_35;
 Q63 tmp_36;
 Q63 tmp_37;
 Q31 tmp_38;
 Q63 tmp_39;
 Q63 tmp_40;
 Q63 tmp_41;
 Q63 tmp_42;
 Q63 tmp_43;


a[0] = memory<complex<Q31>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q31>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q31>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<Q31>>::load(in,4 * stageLength * strideFactor);
a[5] = memory<complex<Q31>>::load(in,5 * stageLength * strideFactor);


tmp_0 = ( decq(a[0].re,1)   +  decq(a[3].re,1)  ) ;
tmp_1 = ( decq(a[1].re,1)   +  decq(a[4].re,1)  ) ;
tmp_2 = ( decq(a[2].re,1)   +  decq(a[5].re,1)  ) ;
tmp_32 = ( decq( decq(tmp_0,1)  ,1)   +  decq(( decq(tmp_1,1)   +  decq(tmp_2,1)  ) ,1)  ) ;
res[0].re = tmp_32;
tmp_3 = ( decq(a[1].re,1)   -  decq(a[4].re,1)  ) ;
tmp_4 = (C1_6_notw) * tmp_3 ;
tmp_5 = ( decq(a[1].im,1)   -  decq(a[4].im,1)  ) ;
tmp_6 = (C0_6_notw) * tmp_5 ;
tmp_7 = (tmp_4 + tmp_6) ;
tmp_8 = ( decq(a[2].im,1)   -  decq(a[5].im,1)  ) ;
tmp_9 = (C0_6_notw) * tmp_8 ;
tmp_10 = ( decq(a[2].re,1)   -  decq(a[5].re,1)  ) ;
tmp_11 = (C1_6_notw) * tmp_10 ;
tmp_12 = (tmp_9 - tmp_11) ;
tmp_13 = ( decq(a[0].re,1)   -  decq(a[3].re,1)  ) ;
tmp_33 = (tmp_7 + (tmp_12 +  widen<Q31>::run(tmp_13)  ) ) ;
res[1].re =  narrow<2,Q31>::run(tmp_33)  ;
tmp_14 = ( decq(a[1].im,1)   +  decq(a[4].im,1)  ) ;
tmp_15 = ( decq(a[2].im,1)   +  decq(a[5].im,1)  ) ;
tmp_16 = ( decq(tmp_1,1)   +  decq(tmp_2,1)  ) ;
tmp_17 = (C1_6_notw) * tmp_16 ;
tmp_34 = ( decq( widen<Q31>::run(tmp_0)  ,1)   + ((C0_6_notw) * ( decq(tmp_14,1)   -  decq(tmp_15,1)  )   - tmp_17) ) ;
res[2].re =  narrow<1,Q31>::run(tmp_34)  ;
tmp_18 = (C1_6_notw) * tmp_5 ;
tmp_19 = (C0_6_notw) * tmp_3 ;
tmp_20 = (tmp_18 - tmp_19) ;
tmp_21 = (C1_6_notw) * tmp_8 ;
tmp_22 = (C0_6_notw) * tmp_10 ;
tmp_23 = ( (-(tmp_21))   - tmp_22) ;
tmp_24 = (tmp_7 + tmp_12) ;
tmp_25 = (C1_6_notw) *  narrow<NORMALNARROW,Q31>::run(tmp_24)   ;
tmp_35 = ( widen<Q31>::run(tmp_13)   + ((C0_6_notw) *  narrow<NORMALNARROW,Q31>::run((tmp_20 - tmp_23) )    - tmp_25) ) ;
res[3].re =  narrow<2,Q31>::run(tmp_35)  ;
tmp_36 = ( decq( widen<Q31>::run(tmp_0)  ,1)   + ((C0_6_notw) * ( decq(tmp_15,1)   -  decq(tmp_14,1)  )   - tmp_17) ) ;
res[4].re =  narrow<1,Q31>::run(tmp_36)  ;
tmp_37 = ( widen<Q31>::run(tmp_13)   + ((C0_6_notw) *  narrow<NORMALNARROW,Q31>::run((tmp_23 - tmp_20) )    - tmp_25) ) ;
res[5].re =  narrow<2,Q31>::run(tmp_37)  ;
tmp_26 = ( decq(a[0].im,1)   +  decq(a[3].im,1)  ) ;
tmp_38 = ( decq( decq(tmp_26,1)  ,1)   +  decq(( decq(tmp_14,1)   +  decq(tmp_15,1)  ) ,1)  ) ;
res[0].im = tmp_38;
tmp_27 = ( decq(a[0].im,1)   -  decq(a[3].im,1)  ) ;
tmp_39 = (tmp_20 + (tmp_23 +  widen<Q31>::run(tmp_27)  ) ) ;
res[1].im =  narrow<2,Q31>::run(tmp_39)  ;
tmp_28 = ( decq(tmp_14,1)   +  decq(tmp_15,1)  ) ;
tmp_29 = (C1_6_notw) * tmp_28 ;
tmp_40 = ( decq( widen<Q31>::run(tmp_26)  ,1)   + ((C0_6_notw) * ( decq(tmp_2,1)   -  decq(tmp_1,1)  )   - tmp_29) ) ;
res[2].im =  narrow<1,Q31>::run(tmp_40)  ;
tmp_30 = (tmp_20 + tmp_23) ;
tmp_31 = (C1_6_notw) *  narrow<NORMALNARROW,Q31>::run(tmp_30)   ;
tmp_41 = ( widen<Q31>::run(tmp_27)   + ((C0_6_notw) *  narrow<NORMALNARROW,Q31>::run((tmp_12 - tmp_7) )    - tmp_31) ) ;
res[3].im =  narrow<2,Q31>::run(tmp_41)  ;
tmp_42 = ( decq( widen<Q31>::run(tmp_26)  ,1)   + ((C0_6_notw) * ( decq(tmp_1,1)   -  decq(tmp_2,1)  )   - tmp_29) ) ;
res[4].im =  narrow<1,Q31>::run(tmp_42)  ;
tmp_43 = ( widen<Q31>::run(tmp_27)   + ((C0_6_notw) *  narrow<NORMALNARROW,Q31>::run((tmp_7 - tmp_12) )    - tmp_31) ) ;
res[5].im =  narrow<2,Q31>::run(tmp_43)  ;


memory<complex<Q31>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<Q31>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<Q31>>::store(out,2 * stageLength * strideFactor , res[2]);
memory<complex<Q31>>::store(out,3 * stageLength * strideFactor , res[3]);
memory<complex<Q31>>::store(out,4 * stageLength * strideFactor , res[4]);
memory<complex<Q31>>::store(out,5 * stageLength * strideFactor , res[5]);


};

/* Nb Multiplications = 40, 
Nb Additions = 62,
Nb Negates = 31,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 3

*/
 __STATIC_FORCEINLINE void run_op_tw(int stageLength,const complex<Q31> *w,const complex<Q31> *in,complex<Q31> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q31> a[6];
complex<Q31> res[6];


 Q31 tmp_0;
 Q31 tmp_1;
 Q31 tmp_2;
 Q31 tmp_3;
 Q63 tmp_4;
 Q31 tmp_5;
 Q63 tmp_6;
 Q63 tmp_7;
 Q31 tmp_8;
 Q63 tmp_9;
 Q31 tmp_10;
 Q63 tmp_11;
 Q63 tmp_12;
 Q31 tmp_13;
 Q63 tmp_14;
 Q63 tmp_15;
 Q63 tmp_16;
 Q63 tmp_17;
 Q63 tmp_18;
 Q63 tmp_19;
 Q63 tmp_20;
 Q31 tmp_21;
 Q63 tmp_22;
 Q31 tmp_23;
 Q31 tmp_24;
 Q31 tmp_25;
 Q63 tmp_26;
 Q31 tmp_27;
 Q63 tmp_28;
 Q63 tmp_29;
 Q31 tmp_30;
 Q31 tmp_31;
 Q63 tmp_32;
 Q31 tmp_33;
 Q63 tmp_34;
 Q63 tmp_35;
 Q63 tmp_36;
 Q63 tmp_37;
 Q63 tmp_38;
 Q63 tmp_39;
 Q63 tmp_40;
 Q63 tmp_41;
 Q63 tmp_42;
 Q63 tmp_43;
 Q63 tmp_44;
 Q63 tmp_45;
 Q31 tmp_46;
 Q63 tmp_47;
 Q63 tmp_48;
 Q31 tmp_49;
 Q63 tmp_50;
 Q63 tmp_51;
 Q63 tmp_52;
 Q63 tmp_53;
 Q63 tmp_54;
 Q63 tmp_55;
 Q63 tmp_56;
 Q63 tmp_57;
 Q31 tmp_58;
 Q63 tmp_59;
 Q63 tmp_60;
 Q63 tmp_61;
 Q63 tmp_62;
 Q63 tmp_63;
 Q63 tmp_64;
 Q63 tmp_65;
 Q63 tmp_66;
 Q63 tmp_67;
 Q63 tmp_68;
 Q31 tmp_69;


a[0] = memory<complex<Q31>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q31>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q31>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<Q31>>::load(in,4 * stageLength * strideFactor);
a[5] = memory<complex<Q31>>::load(in,5 * stageLength * strideFactor);


tmp_0 = ( decq(a[0].re,1)   +  decq(a[3].re,1)  ) ;
tmp_1 = ( decq(a[1].re,1)   +  decq(a[4].re,1)  ) ;
tmp_2 = ( decq(a[2].re,1)   +  decq(a[5].re,1)  ) ;
tmp_58 = ( decq( decq(tmp_0,1)  ,1)   +  decq(( decq(tmp_1,1)   +  decq(tmp_2,1)  ) ,1)  ) ;
res[0].re = tmp_58;
tmp_3 = ( decq(a[1].re,1)   -  decq(a[4].re,1)  ) ;
tmp_4 = (C1_6_tw) * tmp_3 ;
tmp_5 = ( decq(a[1].im,1)   -  decq(a[4].im,1)  ) ;
tmp_6 = (C0_6_tw) * tmp_5 ;
tmp_7 = (tmp_4 + tmp_6) ;
tmp_8 = ( decq(a[2].im,1)   -  decq(a[5].im,1)  ) ;
tmp_9 = (C0_6_tw) * tmp_8 ;
tmp_10 = ( decq(a[2].re,1)   -  decq(a[5].re,1)  ) ;
tmp_11 = (C1_6_tw) * tmp_10 ;
tmp_12 = (tmp_9 - tmp_11) ;
tmp_13 = ( decq(a[0].re,1)   -  decq(a[3].re,1)  ) ;
tmp_59 = (tmp_7 + (tmp_12 +  widen<Q31>::run(tmp_13)  ) ) ;
tmp_14 = tmp_59;
tmp_15 = (C1_6_tw) * tmp_5 ;
tmp_16 = (C0_6_tw) * tmp_3 ;
tmp_17 = (tmp_15 - tmp_16) ;
tmp_18 = (C1_6_tw) * tmp_8 ;
tmp_19 = (C0_6_tw) * tmp_10 ;
tmp_20 = ( (-(tmp_18))   - tmp_19) ;
tmp_21 = ( decq(a[0].im,1)   -  decq(a[3].im,1)  ) ;
tmp_60 = (tmp_17 + (tmp_20 +  widen<Q31>::run(tmp_21)  ) ) ;
tmp_22 = tmp_60;
res[1].re =  narrow<2,Q31>::run((w[0].re *  narrow<NORMALNARROW,Q31>::run(tmp_14)    - w[0].im *  narrow<NORMALNARROW,Q31>::run(tmp_22)   ) )  ;
tmp_23 = ( decq(a[1].im,1)   +  decq(a[4].im,1)  ) ;
tmp_24 = ( decq(a[2].im,1)   +  decq(a[5].im,1)  ) ;
tmp_25 = ( decq(tmp_23,1)   -  decq(tmp_24,1)  ) ;
tmp_26 = (C0_6_tw) * tmp_25 ;
tmp_27 = ( decq(tmp_1,1)   +  decq(tmp_2,1)  ) ;
tmp_28 = (C1_6_tw) * tmp_27 ;
tmp_61 = ( decq( widen<Q31>::run(tmp_0)  ,1)   + (tmp_26 - tmp_28) ) ;
tmp_29 = tmp_61;
tmp_30 = ( decq(a[0].im,1)   +  decq(a[3].im,1)  ) ;
tmp_31 = ( decq(tmp_2,1)   -  decq(tmp_1,1)  ) ;
tmp_32 = (C0_6_tw) * tmp_31 ;
tmp_33 = ( decq(tmp_23,1)   +  decq(tmp_24,1)  ) ;
tmp_34 = (C1_6_tw) * tmp_33 ;
tmp_62 = ( decq( widen<Q31>::run(tmp_30)  ,1)   + (tmp_32 - tmp_34) ) ;
tmp_35 = tmp_62;
res[2].re =  narrow<1,Q31>::run((w[1].re *  narrow<NORMALNARROW,Q31>::run(tmp_29)    - w[1].im *  narrow<NORMALNARROW,Q31>::run(tmp_35)   ) )  ;
tmp_36 = (tmp_17 - tmp_20) ;
tmp_37 = (C0_6_tw) *  narrow<NORMALNARROW,Q31>::run(tmp_36)   ;
tmp_38 = (tmp_7 + tmp_12) ;
tmp_39 = (C1_6_tw) *  narrow<NORMALNARROW,Q31>::run(tmp_38)   ;
tmp_63 = ( widen<Q31>::run(tmp_13)   + (tmp_37 - tmp_39) ) ;
tmp_40 = tmp_63;
tmp_41 = (tmp_12 - tmp_7) ;
tmp_42 = (C0_6_tw) *  narrow<NORMALNARROW,Q31>::run(tmp_41)   ;
tmp_43 = (tmp_17 + tmp_20) ;
tmp_44 = (C1_6_tw) *  narrow<NORMALNARROW,Q31>::run(tmp_43)   ;
tmp_64 = ( widen<Q31>::run(tmp_21)   + (tmp_42 - tmp_44) ) ;
tmp_45 = tmp_64;
res[3].re =  narrow<2,Q31>::run((w[2].re *  narrow<NORMALNARROW,Q31>::run(tmp_40)    - w[2].im *  narrow<NORMALNARROW,Q31>::run(tmp_45)   ) )  ;
tmp_46 = ( decq(tmp_24,1)   -  decq(tmp_23,1)  ) ;
tmp_47 = (C0_6_tw) * tmp_46 ;
tmp_65 = ( decq( widen<Q31>::run(tmp_0)  ,1)   + (tmp_47 - tmp_28) ) ;
tmp_48 = tmp_65;
tmp_49 = ( decq(tmp_1,1)   -  decq(tmp_2,1)  ) ;
tmp_50 = (C0_6_tw) * tmp_49 ;
tmp_66 = ( decq( widen<Q31>::run(tmp_30)  ,1)   + (tmp_50 - tmp_34) ) ;
tmp_51 = tmp_66;
res[4].re =  narrow<1,Q31>::run((w[3].re *  narrow<NORMALNARROW,Q31>::run(tmp_48)    - w[3].im *  narrow<NORMALNARROW,Q31>::run(tmp_51)   ) )  ;
tmp_52 = (tmp_20 - tmp_17) ;
tmp_53 = (C0_6_tw) *  narrow<NORMALNARROW,Q31>::run(tmp_52)   ;
tmp_67 = ( widen<Q31>::run(tmp_13)   + (tmp_53 - tmp_39) ) ;
tmp_54 = tmp_67;
tmp_55 = (tmp_7 - tmp_12) ;
tmp_56 = (C0_6_tw) *  narrow<NORMALNARROW,Q31>::run(tmp_55)   ;
tmp_68 = ( widen<Q31>::run(tmp_21)   + (tmp_56 - tmp_44) ) ;
tmp_57 = tmp_68;
res[5].re =  narrow<2,Q31>::run((w[4].re *  narrow<NORMALNARROW,Q31>::run(tmp_54)    - w[4].im *  narrow<NORMALNARROW,Q31>::run(tmp_57)   ) )  ;
tmp_69 = ( decq( decq(tmp_30,1)  ,1)   +  decq(( decq(tmp_23,1)   +  decq(tmp_24,1)  ) ,1)  ) ;
res[0].im = tmp_69;
res[1].im =  narrow<2,Q31>::run((w[0].re *  narrow<NORMALNARROW,Q31>::run(tmp_22)    + w[0].im *  narrow<NORMALNARROW,Q31>::run(tmp_14)   ) )  ;
res[2].im =  narrow<1,Q31>::run((w[1].re *  narrow<NORMALNARROW,Q31>::run(tmp_35)    + w[1].im *  narrow<NORMALNARROW,Q31>::run(tmp_29)   ) )  ;
res[3].im =  narrow<2,Q31>::run((w[2].re *  narrow<NORMALNARROW,Q31>::run(tmp_45)    + w[2].im *  narrow<NORMALNARROW,Q31>::run(tmp_40)   ) )  ;
res[4].im =  narrow<1,Q31>::run((w[3].re *  narrow<NORMALNARROW,Q31>::run(tmp_51)    + w[3].im *  narrow<NORMALNARROW,Q31>::run(tmp_48)   ) )  ;
res[5].im =  narrow<2,Q31>::run((w[4].re *  narrow<NORMALNARROW,Q31>::run(tmp_57)    + w[4].im *  narrow<NORMALNARROW,Q31>::run(tmp_54)   ) )  ;


memory<complex<Q31>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<Q31>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<Q31>>::store(out,2 * stageLength * strideFactor , res[2]);
memory<complex<Q31>>::store(out,3 * stageLength * strideFactor , res[3]);
memory<complex<Q31>>::store(out,4 * stageLength * strideFactor , res[4]);
memory<complex<Q31>>::store(out,5 * stageLength * strideFactor , res[5]);


};

/* Nb Multiplications = 20, 
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


complex<Q31> a[6];
complex<Q31> res[6];


 Q31 tmp_0;
 Q31 tmp_1;
 Q31 tmp_2;
 Q31 tmp_3;
 Q63 tmp_4;
 Q31 tmp_5;
 Q63 tmp_6;
 Q63 tmp_7;
 Q31 tmp_8;
 Q63 tmp_9;
 Q31 tmp_10;
 Q63 tmp_11;
 Q63 tmp_12;
 Q31 tmp_13;
 Q31 tmp_14;
 Q31 tmp_15;
 Q31 tmp_16;
 Q63 tmp_17;
 Q63 tmp_18;
 Q63 tmp_19;
 Q63 tmp_20;
 Q63 tmp_21;
 Q63 tmp_22;
 Q63 tmp_23;
 Q63 tmp_24;
 Q63 tmp_25;
 Q31 tmp_26;
 Q31 tmp_27;
 Q31 tmp_28;
 Q63 tmp_29;
 Q63 tmp_30;
 Q63 tmp_31;
 Q31 tmp_32;
 Q63 tmp_33;
 Q63 tmp_34;
 Q63 tmp_35;
 Q63 tmp_36;
 Q63 tmp_37;
 Q31 tmp_38;
 Q63 tmp_39;
 Q63 tmp_40;
 Q63 tmp_41;
 Q63 tmp_42;
 Q63 tmp_43;


a[0] = memory<complex<Q31>>::load(in,0 * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * strideFactor);
a[2] = memory<complex<Q31>>::load(in,2 * strideFactor);
a[3] = memory<complex<Q31>>::load(in,3 * strideFactor);
a[4] = memory<complex<Q31>>::load(in,4 * strideFactor);
a[5] = memory<complex<Q31>>::load(in,5 * strideFactor);


tmp_0 = ( decq(a[0].re,1)   +  decq(a[3].re,1)  ) ;
tmp_1 = ( decq(a[1].re,1)   +  decq(a[4].re,1)  ) ;
tmp_2 = ( decq(a[2].re,1)   +  decq(a[5].re,1)  ) ;
tmp_32 = ( decq( decq(tmp_0,1)  ,1)   +  decq(( decq(tmp_1,1)   +  decq(tmp_2,1)  ) ,1)  ) ;
res[0].re = tmp_32;
tmp_3 = ( decq(a[1].re,1)   -  decq(a[4].re,1)  ) ;
tmp_4 = (C1_6_notw) * tmp_3 ;
tmp_5 = ( decq(a[1].im,1)   -  decq(a[4].im,1)  ) ;
tmp_6 = (C0_6_notw) * tmp_5 ;
tmp_7 = (tmp_4 + tmp_6) ;
tmp_8 = ( decq(a[2].im,1)   -  decq(a[5].im,1)  ) ;
tmp_9 = (C0_6_notw) * tmp_8 ;
tmp_10 = ( decq(a[2].re,1)   -  decq(a[5].re,1)  ) ;
tmp_11 = (C1_6_notw) * tmp_10 ;
tmp_12 = (tmp_9 - tmp_11) ;
tmp_13 = ( decq(a[0].re,1)   -  decq(a[3].re,1)  ) ;
tmp_33 = (tmp_7 + (tmp_12 +  widen<Q31>::run(tmp_13)  ) ) ;
res[1].re =  narrow<2,Q31>::run(tmp_33)  ;
tmp_14 = ( decq(a[1].im,1)   +  decq(a[4].im,1)  ) ;
tmp_15 = ( decq(a[2].im,1)   +  decq(a[5].im,1)  ) ;
tmp_16 = ( decq(tmp_1,1)   +  decq(tmp_2,1)  ) ;
tmp_17 = (C1_6_notw) * tmp_16 ;
tmp_34 = ( decq( widen<Q31>::run(tmp_0)  ,1)   + ((C0_6_notw) * ( decq(tmp_14,1)   -  decq(tmp_15,1)  )   - tmp_17) ) ;
res[2].re =  narrow<1,Q31>::run(tmp_34)  ;
tmp_18 = (C1_6_notw) * tmp_5 ;
tmp_19 = (C0_6_notw) * tmp_3 ;
tmp_20 = (tmp_18 - tmp_19) ;
tmp_21 = (C1_6_notw) * tmp_8 ;
tmp_22 = (C0_6_notw) * tmp_10 ;
tmp_23 = ( (-(tmp_21))   - tmp_22) ;
tmp_24 = (tmp_7 + tmp_12) ;
tmp_25 = (C1_6_notw) *  narrow<NORMALNARROW,Q31>::run(tmp_24)   ;
tmp_35 = ( widen<Q31>::run(tmp_13)   + ((C0_6_notw) *  narrow<NORMALNARROW,Q31>::run((tmp_20 - tmp_23) )    - tmp_25) ) ;
res[3].re =  narrow<2,Q31>::run(tmp_35)  ;
tmp_36 = ( decq( widen<Q31>::run(tmp_0)  ,1)   + ((C0_6_notw) * ( decq(tmp_15,1)   -  decq(tmp_14,1)  )   - tmp_17) ) ;
res[4].re =  narrow<1,Q31>::run(tmp_36)  ;
tmp_37 = ( widen<Q31>::run(tmp_13)   + ((C0_6_notw) *  narrow<NORMALNARROW,Q31>::run((tmp_23 - tmp_20) )    - tmp_25) ) ;
res[5].re =  narrow<2,Q31>::run(tmp_37)  ;
tmp_26 = ( decq(a[0].im,1)   +  decq(a[3].im,1)  ) ;
tmp_38 = ( decq( decq(tmp_26,1)  ,1)   +  decq(( decq(tmp_14,1)   +  decq(tmp_15,1)  ) ,1)  ) ;
res[0].im = tmp_38;
tmp_27 = ( decq(a[0].im,1)   -  decq(a[3].im,1)  ) ;
tmp_39 = (tmp_20 + (tmp_23 +  widen<Q31>::run(tmp_27)  ) ) ;
res[1].im =  narrow<2,Q31>::run(tmp_39)  ;
tmp_28 = ( decq(tmp_14,1)   +  decq(tmp_15,1)  ) ;
tmp_29 = (C1_6_notw) * tmp_28 ;
tmp_40 = ( decq( widen<Q31>::run(tmp_26)  ,1)   + ((C0_6_notw) * ( decq(tmp_2,1)   -  decq(tmp_1,1)  )   - tmp_29) ) ;
res[2].im =  narrow<1,Q31>::run(tmp_40)  ;
tmp_30 = (tmp_20 + tmp_23) ;
tmp_31 = (C1_6_notw) *  narrow<NORMALNARROW,Q31>::run(tmp_30)   ;
tmp_41 = ( widen<Q31>::run(tmp_27)   + ((C0_6_notw) *  narrow<NORMALNARROW,Q31>::run((tmp_12 - tmp_7) )    - tmp_31) ) ;
res[3].im =  narrow<2,Q31>::run(tmp_41)  ;
tmp_42 = ( decq( widen<Q31>::run(tmp_26)  ,1)   + ((C0_6_notw) * ( decq(tmp_1,1)   -  decq(tmp_2,1)  )   - tmp_29) ) ;
res[4].im =  narrow<1,Q31>::run(tmp_42)  ;
tmp_43 = ( widen<Q31>::run(tmp_27)   + ((C0_6_notw) *  narrow<NORMALNARROW,Q31>::run((tmp_7 - tmp_12) )    - tmp_31) ) ;
res[5].im =  narrow<2,Q31>::run(tmp_43)  ;


memory<complex<Q31>>::store(in,0 * strideFactor , res[0]);
memory<complex<Q31>>::store(in,1 * strideFactor , res[1]);
memory<complex<Q31>>::store(in,2 * strideFactor , res[2]);
memory<complex<Q31>>::store(in,3 * strideFactor , res[3]);
memory<complex<Q31>>::store(in,4 * strideFactor , res[4]);
memory<complex<Q31>>::store(in,5 * strideFactor , res[5]);


};

/* Nb Multiplications = 20, 
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


complex<Q31> a[6];
complex<Q31> res[6];


 Q31 tmp_0;
 Q31 tmp_1;
 Q31 tmp_2;
 Q31 tmp_3;
 Q63 tmp_4;
 Q31 tmp_5;
 Q63 tmp_6;
 Q63 tmp_7;
 Q31 tmp_8;
 Q63 tmp_9;
 Q31 tmp_10;
 Q63 tmp_11;
 Q63 tmp_12;
 Q31 tmp_13;
 Q31 tmp_14;
 Q31 tmp_15;
 Q31 tmp_16;
 Q63 tmp_17;
 Q63 tmp_18;
 Q63 tmp_19;
 Q63 tmp_20;
 Q63 tmp_21;
 Q63 tmp_22;
 Q63 tmp_23;
 Q63 tmp_24;
 Q63 tmp_25;
 Q31 tmp_26;
 Q31 tmp_27;
 Q31 tmp_28;
 Q63 tmp_29;
 Q63 tmp_30;
 Q63 tmp_31;
 Q31 tmp_32;
 Q63 tmp_33;
 Q63 tmp_34;
 Q63 tmp_35;
 Q63 tmp_36;
 Q63 tmp_37;
 Q31 tmp_38;
 Q63 tmp_39;
 Q63 tmp_40;
 Q63 tmp_41;
 Q63 tmp_42;
 Q63 tmp_43;


a[0] = memory<complex<Q31>>::load(in,0 * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * strideFactor);
a[2] = memory<complex<Q31>>::load(in,2 * strideFactor);
a[3] = memory<complex<Q31>>::load(in,3 * strideFactor);
a[4] = memory<complex<Q31>>::load(in,4 * strideFactor);
a[5] = memory<complex<Q31>>::load(in,5 * strideFactor);


tmp_0 = ( decq(a[0].re,1)   +  decq(a[3].re,1)  ) ;
tmp_1 = ( decq(a[1].re,1)   +  decq(a[4].re,1)  ) ;
tmp_2 = ( decq(a[2].re,1)   +  decq(a[5].re,1)  ) ;
tmp_32 = ( decq( decq(tmp_0,1)  ,1)   +  decq(( decq(tmp_1,1)   +  decq(tmp_2,1)  ) ,1)  ) ;
res[0].re = tmp_32;
tmp_3 = ( decq(a[1].re,1)   -  decq(a[4].re,1)  ) ;
tmp_4 = (C1_6_notw) * tmp_3 ;
tmp_5 = ( decq(a[1].im,1)   -  decq(a[4].im,1)  ) ;
tmp_6 = (C0_6_notw) * tmp_5 ;
tmp_7 = (tmp_4 + tmp_6) ;
tmp_8 = ( decq(a[2].im,1)   -  decq(a[5].im,1)  ) ;
tmp_9 = (C0_6_notw) * tmp_8 ;
tmp_10 = ( decq(a[2].re,1)   -  decq(a[5].re,1)  ) ;
tmp_11 = (C1_6_notw) * tmp_10 ;
tmp_12 = (tmp_9 - tmp_11) ;
tmp_13 = ( decq(a[0].re,1)   -  decq(a[3].re,1)  ) ;
tmp_33 = (tmp_7 + (tmp_12 +  widen<Q31>::run(tmp_13)  ) ) ;
res[1].re =  narrow<2,Q31>::run(tmp_33)  ;
tmp_14 = ( decq(a[1].im,1)   +  decq(a[4].im,1)  ) ;
tmp_15 = ( decq(a[2].im,1)   +  decq(a[5].im,1)  ) ;
tmp_16 = ( decq(tmp_1,1)   +  decq(tmp_2,1)  ) ;
tmp_17 = (C1_6_notw) * tmp_16 ;
tmp_34 = ( decq( widen<Q31>::run(tmp_0)  ,1)   + ((C0_6_notw) * ( decq(tmp_14,1)   -  decq(tmp_15,1)  )   - tmp_17) ) ;
res[2].re =  narrow<1,Q31>::run(tmp_34)  ;
tmp_18 = (C1_6_notw) * tmp_5 ;
tmp_19 = (C0_6_notw) * tmp_3 ;
tmp_20 = (tmp_18 - tmp_19) ;
tmp_21 = (C1_6_notw) * tmp_8 ;
tmp_22 = (C0_6_notw) * tmp_10 ;
tmp_23 = ( (-(tmp_21))   - tmp_22) ;
tmp_24 = (tmp_7 + tmp_12) ;
tmp_25 = (C1_6_notw) *  narrow<NORMALNARROW,Q31>::run(tmp_24)   ;
tmp_35 = ( widen<Q31>::run(tmp_13)   + ((C0_6_notw) *  narrow<NORMALNARROW,Q31>::run((tmp_20 - tmp_23) )    - tmp_25) ) ;
res[3].re =  narrow<2,Q31>::run(tmp_35)  ;
tmp_36 = ( decq( widen<Q31>::run(tmp_0)  ,1)   + ((C0_6_notw) * ( decq(tmp_15,1)   -  decq(tmp_14,1)  )   - tmp_17) ) ;
res[4].re =  narrow<1,Q31>::run(tmp_36)  ;
tmp_37 = ( widen<Q31>::run(tmp_13)   + ((C0_6_notw) *  narrow<NORMALNARROW,Q31>::run((tmp_23 - tmp_20) )    - tmp_25) ) ;
res[5].re =  narrow<2,Q31>::run(tmp_37)  ;
tmp_26 = ( decq(a[0].im,1)   +  decq(a[3].im,1)  ) ;
tmp_38 = ( decq( decq(tmp_26,1)  ,1)   +  decq(( decq(tmp_14,1)   +  decq(tmp_15,1)  ) ,1)  ) ;
res[0].im = tmp_38;
tmp_27 = ( decq(a[0].im,1)   -  decq(a[3].im,1)  ) ;
tmp_39 = (tmp_20 + (tmp_23 +  widen<Q31>::run(tmp_27)  ) ) ;
res[1].im =  narrow<2,Q31>::run(tmp_39)  ;
tmp_28 = ( decq(tmp_14,1)   +  decq(tmp_15,1)  ) ;
tmp_29 = (C1_6_notw) * tmp_28 ;
tmp_40 = ( decq( widen<Q31>::run(tmp_26)  ,1)   + ((C0_6_notw) * ( decq(tmp_2,1)   -  decq(tmp_1,1)  )   - tmp_29) ) ;
res[2].im =  narrow<1,Q31>::run(tmp_40)  ;
tmp_30 = (tmp_20 + tmp_23) ;
tmp_31 = (C1_6_notw) *  narrow<NORMALNARROW,Q31>::run(tmp_30)   ;
tmp_41 = ( widen<Q31>::run(tmp_27)   + ((C0_6_notw) *  narrow<NORMALNARROW,Q31>::run((tmp_12 - tmp_7) )    - tmp_31) ) ;
res[3].im =  narrow<2,Q31>::run(tmp_41)  ;
tmp_42 = ( decq( widen<Q31>::run(tmp_26)  ,1)   + ((C0_6_notw) * ( decq(tmp_1,1)   -  decq(tmp_2,1)  )   - tmp_29) ) ;
res[4].im =  narrow<1,Q31>::run(tmp_42)  ;
tmp_43 = ( widen<Q31>::run(tmp_27)   + ((C0_6_notw) *  narrow<NORMALNARROW,Q31>::run((tmp_7 - tmp_12) )    - tmp_31) ) ;
res[5].im =  narrow<2,Q31>::run(tmp_43)  ;


memory<complex<Q31>>::store(out,0 * strideFactor , res[0]);
memory<complex<Q31>>::store(out,1 * strideFactor , res[1]);
memory<complex<Q31>>::store(out,2 * strideFactor , res[2]);
memory<complex<Q31>>::store(out,3 * strideFactor , res[3]);
memory<complex<Q31>>::store(out,4 * strideFactor , res[4]);
memory<complex<Q31>>::store(out,5 * strideFactor , res[5]);


};

};

/**************

INVERSE FFT 

***************/
template<int inputstrideenabled> 
struct radix<INVERSE,Q31, 6,inputstrideenabled>{

static constexpr Q31 C0_6_tw{(q31_t)0x6ED9EBA1};
  static constexpr Q31 C1_6_tw{(q31_t)0x40000000};
  
static constexpr Q31 C0_6_notw{(q31_t)0x6ED9EBA1};
  static constexpr Q31 C1_6_notw{(q31_t)0x40000000};
  

/* Nb Multiplications = 20, 
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


complex<Q31> a[6];
complex<Q31> res[6];


 Q31 tmp_0;
 Q31 tmp_1;
 Q31 tmp_2;
 Q31 tmp_3;
 Q63 tmp_4;
 Q31 tmp_5;
 Q63 tmp_6;
 Q63 tmp_7;
 Q31 tmp_8;
 Q63 tmp_9;
 Q31 tmp_10;
 Q63 tmp_11;
 Q63 tmp_12;
 Q31 tmp_13;
 Q31 tmp_14;
 Q31 tmp_15;
 Q31 tmp_16;
 Q63 tmp_17;
 Q63 tmp_18;
 Q63 tmp_19;
 Q63 tmp_20;
 Q63 tmp_21;
 Q63 tmp_22;
 Q63 tmp_23;
 Q63 tmp_24;
 Q63 tmp_25;
 Q31 tmp_26;
 Q31 tmp_27;
 Q31 tmp_28;
 Q63 tmp_29;
 Q63 tmp_30;
 Q63 tmp_31;
 Q31 tmp_32;
 Q63 tmp_33;
 Q63 tmp_34;
 Q63 tmp_35;
 Q63 tmp_36;
 Q63 tmp_37;
 Q31 tmp_38;
 Q63 tmp_39;
 Q63 tmp_40;
 Q63 tmp_41;
 Q63 tmp_42;
 Q63 tmp_43;


a[0] = memory<complex<Q31>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q31>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q31>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<Q31>>::load(in,4 * stageLength * strideFactor);
a[5] = memory<complex<Q31>>::load(in,5 * stageLength * strideFactor);


tmp_0 = ( decq(a[0].re,1)   +  decq(a[3].re,1)  ) ;
tmp_1 = ( decq(a[1].re,1)   +  decq(a[4].re,1)  ) ;
tmp_2 = ( decq(a[2].re,1)   +  decq(a[5].re,1)  ) ;
tmp_32 = ( decq( decq(tmp_0,1)  ,1)   +  decq(( decq(tmp_1,1)   +  decq(tmp_2,1)  ) ,1)  ) ;
res[0].re = tmp_32;
tmp_3 = ( decq(a[1].re,1)   -  decq(a[4].re,1)  ) ;
tmp_4 = (C1_6_notw) * tmp_3 ;
tmp_5 = ( decq(a[1].im,1)   -  decq(a[4].im,1)  ) ;
tmp_6 = (C0_6_notw) * tmp_5 ;
tmp_7 = (tmp_4 - tmp_6) ;
tmp_8 = ( decq(a[2].re,1)   -  decq(a[5].re,1)  ) ;
tmp_9 = (C1_6_notw) * tmp_8 ;
tmp_10 = ( decq(a[2].im,1)   -  decq(a[5].im,1)  ) ;
tmp_11 = (C0_6_notw) * tmp_10 ;
tmp_12 = ( (-(tmp_9))   - tmp_11) ;
tmp_13 = ( decq(a[0].re,1)   -  decq(a[3].re,1)  ) ;
tmp_33 = (tmp_7 + (tmp_12 +  widen<Q31>::run(tmp_13)  ) ) ;
res[1].re =  narrow<2,Q31>::run(tmp_33)  ;
tmp_14 = ( decq(a[2].im,1)   +  decq(a[5].im,1)  ) ;
tmp_15 = ( decq(a[1].im,1)   +  decq(a[4].im,1)  ) ;
tmp_16 = ( decq(tmp_1,1)   +  decq(tmp_2,1)  ) ;
tmp_17 = (C1_6_notw) * tmp_16 ;
tmp_34 = ( decq( widen<Q31>::run(tmp_0)  ,1)   + ((C0_6_notw) * ( decq(tmp_14,1)   -  decq(tmp_15,1)  )   - tmp_17) ) ;
res[2].re =  narrow<1,Q31>::run(tmp_34)  ;
tmp_18 = (C0_6_notw) * tmp_8 ;
tmp_19 = (C1_6_notw) * tmp_10 ;
tmp_20 = (tmp_18 - tmp_19) ;
tmp_21 = (C1_6_notw) * tmp_5 ;
tmp_22 = (C0_6_notw) * tmp_3 ;
tmp_23 = (tmp_21 + tmp_22) ;
tmp_24 = (tmp_7 + tmp_12) ;
tmp_25 = (C1_6_notw) *  narrow<NORMALNARROW,Q31>::run(tmp_24)   ;
tmp_35 = ( widen<Q31>::run(tmp_13)   + ((C0_6_notw) *  narrow<NORMALNARROW,Q31>::run((tmp_20 - tmp_23) )    - tmp_25) ) ;
res[3].re =  narrow<2,Q31>::run(tmp_35)  ;
tmp_36 = ( decq( widen<Q31>::run(tmp_0)  ,1)   + ((C0_6_notw) * ( decq(tmp_15,1)   -  decq(tmp_14,1)  )   - tmp_17) ) ;
res[4].re =  narrow<1,Q31>::run(tmp_36)  ;
tmp_37 = ( widen<Q31>::run(tmp_13)   + ((C0_6_notw) *  narrow<NORMALNARROW,Q31>::run((tmp_23 - tmp_20) )    - tmp_25) ) ;
res[5].re =  narrow<2,Q31>::run(tmp_37)  ;
tmp_26 = ( decq(a[0].im,1)   +  decq(a[3].im,1)  ) ;
tmp_38 = ( decq( decq(tmp_26,1)  ,1)   +  decq(( decq(tmp_15,1)   +  decq(tmp_14,1)  ) ,1)  ) ;
res[0].im = tmp_38;
tmp_27 = ( decq(a[0].im,1)   -  decq(a[3].im,1)  ) ;
tmp_39 = (tmp_23 + (tmp_20 +  widen<Q31>::run(tmp_27)  ) ) ;
res[1].im =  narrow<2,Q31>::run(tmp_39)  ;
tmp_28 = ( decq(tmp_15,1)   +  decq(tmp_14,1)  ) ;
tmp_29 = (C1_6_notw) * tmp_28 ;
tmp_40 = ( decq( widen<Q31>::run(tmp_26)  ,1)   + ((C0_6_notw) * ( decq(tmp_1,1)   -  decq(tmp_2,1)  )   - tmp_29) ) ;
res[2].im =  narrow<1,Q31>::run(tmp_40)  ;
tmp_30 = (tmp_23 + tmp_20) ;
tmp_31 = (C1_6_notw) *  narrow<NORMALNARROW,Q31>::run(tmp_30)   ;
tmp_41 = ( widen<Q31>::run(tmp_27)   + ((C0_6_notw) *  narrow<NORMALNARROW,Q31>::run((tmp_7 - tmp_12) )    - tmp_31) ) ;
res[3].im =  narrow<2,Q31>::run(tmp_41)  ;
tmp_42 = ( decq( widen<Q31>::run(tmp_26)  ,1)   + ((C0_6_notw) * ( decq(tmp_2,1)   -  decq(tmp_1,1)  )   - tmp_29) ) ;
res[4].im =  narrow<1,Q31>::run(tmp_42)  ;
tmp_43 = ( widen<Q31>::run(tmp_27)   + ((C0_6_notw) *  narrow<NORMALNARROW,Q31>::run((tmp_12 - tmp_7) )    - tmp_31) ) ;
res[5].im =  narrow<2,Q31>::run(tmp_43)  ;


memory<complex<Q31>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<Q31>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<Q31>>::store(in,2 * stageLength * strideFactor , res[2]);
memory<complex<Q31>>::store(in,3 * stageLength * strideFactor , res[3]);
memory<complex<Q31>>::store(in,4 * stageLength * strideFactor , res[4]);
memory<complex<Q31>>::store(in,5 * stageLength * strideFactor , res[5]);


};

/* Nb Multiplications = 40, 
Nb Additions = 62,
Nb Negates = 31,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 3

*/
 __STATIC_FORCEINLINE void run_ip_tw(int stageLength,const complex<Q31> *w,complex<Q31> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q31> a[6];
complex<Q31> res[6];


 Q31 tmp_0;
 Q31 tmp_1;
 Q31 tmp_2;
 Q31 tmp_3;
 Q63 tmp_4;
 Q31 tmp_5;
 Q63 tmp_6;
 Q63 tmp_7;
 Q31 tmp_8;
 Q63 tmp_9;
 Q31 tmp_10;
 Q63 tmp_11;
 Q63 tmp_12;
 Q31 tmp_13;
 Q63 tmp_14;
 Q63 tmp_15;
 Q63 tmp_16;
 Q63 tmp_17;
 Q63 tmp_18;
 Q63 tmp_19;
 Q63 tmp_20;
 Q31 tmp_21;
 Q63 tmp_22;
 Q31 tmp_23;
 Q31 tmp_24;
 Q31 tmp_25;
 Q63 tmp_26;
 Q31 tmp_27;
 Q63 tmp_28;
 Q63 tmp_29;
 Q31 tmp_30;
 Q31 tmp_31;
 Q63 tmp_32;
 Q31 tmp_33;
 Q63 tmp_34;
 Q63 tmp_35;
 Q63 tmp_36;
 Q63 tmp_37;
 Q63 tmp_38;
 Q63 tmp_39;
 Q63 tmp_40;
 Q63 tmp_41;
 Q63 tmp_42;
 Q63 tmp_43;
 Q63 tmp_44;
 Q63 tmp_45;
 Q31 tmp_46;
 Q63 tmp_47;
 Q63 tmp_48;
 Q31 tmp_49;
 Q63 tmp_50;
 Q63 tmp_51;
 Q63 tmp_52;
 Q63 tmp_53;
 Q63 tmp_54;
 Q63 tmp_55;
 Q63 tmp_56;
 Q63 tmp_57;
 Q31 tmp_58;
 Q63 tmp_59;
 Q63 tmp_60;
 Q63 tmp_61;
 Q63 tmp_62;
 Q63 tmp_63;
 Q63 tmp_64;
 Q63 tmp_65;
 Q63 tmp_66;
 Q63 tmp_67;
 Q63 tmp_68;
 Q31 tmp_69;


a[0] = memory<complex<Q31>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q31>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q31>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<Q31>>::load(in,4 * stageLength * strideFactor);
a[5] = memory<complex<Q31>>::load(in,5 * stageLength * strideFactor);


tmp_0 = ( decq(a[0].re,1)   +  decq(a[3].re,1)  ) ;
tmp_1 = ( decq(a[1].re,1)   +  decq(a[4].re,1)  ) ;
tmp_2 = ( decq(a[2].re,1)   +  decq(a[5].re,1)  ) ;
tmp_58 = ( decq( decq(tmp_0,1)  ,1)   +  decq(( decq(tmp_1,1)   +  decq(tmp_2,1)  ) ,1)  ) ;
res[0].re = tmp_58;
tmp_3 = ( decq(a[1].re,1)   -  decq(a[4].re,1)  ) ;
tmp_4 = (C1_6_tw) * tmp_3 ;
tmp_5 = ( decq(a[1].im,1)   -  decq(a[4].im,1)  ) ;
tmp_6 = (C0_6_tw) * tmp_5 ;
tmp_7 = (tmp_4 - tmp_6) ;
tmp_8 = ( decq(a[2].re,1)   -  decq(a[5].re,1)  ) ;
tmp_9 = (C1_6_tw) * tmp_8 ;
tmp_10 = ( decq(a[2].im,1)   -  decq(a[5].im,1)  ) ;
tmp_11 = (C0_6_tw) * tmp_10 ;
tmp_12 = ( (-(tmp_9))   - tmp_11) ;
tmp_13 = ( decq(a[0].re,1)   -  decq(a[3].re,1)  ) ;
tmp_59 = (tmp_7 + (tmp_12 +  widen<Q31>::run(tmp_13)  ) ) ;
tmp_14 = tmp_59;
tmp_15 = (C1_6_tw) * tmp_5 ;
tmp_16 = (C0_6_tw) * tmp_3 ;
tmp_17 = (tmp_15 + tmp_16) ;
tmp_18 = (C0_6_tw) * tmp_8 ;
tmp_19 = (C1_6_tw) * tmp_10 ;
tmp_20 = (tmp_18 - tmp_19) ;
tmp_21 = ( decq(a[0].im,1)   -  decq(a[3].im,1)  ) ;
tmp_60 = (tmp_17 + (tmp_20 +  widen<Q31>::run(tmp_21)  ) ) ;
tmp_22 = tmp_60;
res[1].re =  narrow<2,Q31>::run((w[0].re *  narrow<NORMALNARROW,Q31>::run(tmp_14)    + w[0].im *  narrow<NORMALNARROW,Q31>::run(tmp_22)   ) )  ;
tmp_23 = ( decq(a[2].im,1)   +  decq(a[5].im,1)  ) ;
tmp_24 = ( decq(a[1].im,1)   +  decq(a[4].im,1)  ) ;
tmp_25 = ( decq(tmp_23,1)   -  decq(tmp_24,1)  ) ;
tmp_26 = (C0_6_tw) * tmp_25 ;
tmp_27 = ( decq(tmp_1,1)   +  decq(tmp_2,1)  ) ;
tmp_28 = (C1_6_tw) * tmp_27 ;
tmp_61 = ( decq( widen<Q31>::run(tmp_0)  ,1)   + (tmp_26 - tmp_28) ) ;
tmp_29 = tmp_61;
tmp_30 = ( decq(a[0].im,1)   +  decq(a[3].im,1)  ) ;
tmp_31 = ( decq(tmp_1,1)   -  decq(tmp_2,1)  ) ;
tmp_32 = (C0_6_tw) * tmp_31 ;
tmp_33 = ( decq(tmp_24,1)   +  decq(tmp_23,1)  ) ;
tmp_34 = (C1_6_tw) * tmp_33 ;
tmp_62 = ( decq( widen<Q31>::run(tmp_30)  ,1)   + (tmp_32 - tmp_34) ) ;
tmp_35 = tmp_62;
res[2].re =  narrow<1,Q31>::run((w[1].re *  narrow<NORMALNARROW,Q31>::run(tmp_29)    + w[1].im *  narrow<NORMALNARROW,Q31>::run(tmp_35)   ) )  ;
tmp_36 = (tmp_20 - tmp_17) ;
tmp_37 = (C0_6_tw) *  narrow<NORMALNARROW,Q31>::run(tmp_36)   ;
tmp_38 = (tmp_7 + tmp_12) ;
tmp_39 = (C1_6_tw) *  narrow<NORMALNARROW,Q31>::run(tmp_38)   ;
tmp_63 = ( widen<Q31>::run(tmp_13)   + (tmp_37 - tmp_39) ) ;
tmp_40 = tmp_63;
tmp_41 = (tmp_7 - tmp_12) ;
tmp_42 = (C0_6_tw) *  narrow<NORMALNARROW,Q31>::run(tmp_41)   ;
tmp_43 = (tmp_17 + tmp_20) ;
tmp_44 = (C1_6_tw) *  narrow<NORMALNARROW,Q31>::run(tmp_43)   ;
tmp_64 = ( widen<Q31>::run(tmp_21)   + (tmp_42 - tmp_44) ) ;
tmp_45 = tmp_64;
res[3].re =  narrow<2,Q31>::run((w[2].re *  narrow<NORMALNARROW,Q31>::run(tmp_40)    + w[2].im *  narrow<NORMALNARROW,Q31>::run(tmp_45)   ) )  ;
tmp_46 = ( decq(tmp_24,1)   -  decq(tmp_23,1)  ) ;
tmp_47 = (C0_6_tw) * tmp_46 ;
tmp_65 = ( decq( widen<Q31>::run(tmp_0)  ,1)   + (tmp_47 - tmp_28) ) ;
tmp_48 = tmp_65;
tmp_49 = ( decq(tmp_2,1)   -  decq(tmp_1,1)  ) ;
tmp_50 = (C0_6_tw) * tmp_49 ;
tmp_66 = ( decq( widen<Q31>::run(tmp_30)  ,1)   + (tmp_50 - tmp_34) ) ;
tmp_51 = tmp_66;
res[4].re =  narrow<1,Q31>::run((w[3].re *  narrow<NORMALNARROW,Q31>::run(tmp_48)    + w[3].im *  narrow<NORMALNARROW,Q31>::run(tmp_51)   ) )  ;
tmp_52 = (tmp_17 - tmp_20) ;
tmp_53 = (C0_6_tw) *  narrow<NORMALNARROW,Q31>::run(tmp_52)   ;
tmp_67 = ( widen<Q31>::run(tmp_13)   + (tmp_53 - tmp_39) ) ;
tmp_54 = tmp_67;
tmp_55 = (tmp_12 - tmp_7) ;
tmp_56 = (C0_6_tw) *  narrow<NORMALNARROW,Q31>::run(tmp_55)   ;
tmp_68 = ( widen<Q31>::run(tmp_21)   + (tmp_56 - tmp_44) ) ;
tmp_57 = tmp_68;
res[5].re =  narrow<2,Q31>::run((w[4].re *  narrow<NORMALNARROW,Q31>::run(tmp_54)    + w[4].im *  narrow<NORMALNARROW,Q31>::run(tmp_57)   ) )  ;
tmp_69 = ( decq( decq(tmp_30,1)  ,1)   +  decq(( decq(tmp_24,1)   +  decq(tmp_23,1)  ) ,1)  ) ;
res[0].im = tmp_69;
res[1].im =  narrow<2,Q31>::run((w[0].re *  narrow<NORMALNARROW,Q31>::run(tmp_22)    - w[0].im *  narrow<NORMALNARROW,Q31>::run(tmp_14)   ) )  ;
res[2].im =  narrow<1,Q31>::run((w[1].re *  narrow<NORMALNARROW,Q31>::run(tmp_35)    - w[1].im *  narrow<NORMALNARROW,Q31>::run(tmp_29)   ) )  ;
res[3].im =  narrow<2,Q31>::run((w[2].re *  narrow<NORMALNARROW,Q31>::run(tmp_45)    - w[2].im *  narrow<NORMALNARROW,Q31>::run(tmp_40)   ) )  ;
res[4].im =  narrow<1,Q31>::run((w[3].re *  narrow<NORMALNARROW,Q31>::run(tmp_51)    - w[3].im *  narrow<NORMALNARROW,Q31>::run(tmp_48)   ) )  ;
res[5].im =  narrow<2,Q31>::run((w[4].re *  narrow<NORMALNARROW,Q31>::run(tmp_57)    - w[4].im *  narrow<NORMALNARROW,Q31>::run(tmp_54)   ) )  ;


memory<complex<Q31>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<Q31>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<Q31>>::store(in,2 * stageLength * strideFactor , res[2]);
memory<complex<Q31>>::store(in,3 * stageLength * strideFactor , res[3]);
memory<complex<Q31>>::store(in,4 * stageLength * strideFactor , res[4]);
memory<complex<Q31>>::store(in,5 * stageLength * strideFactor , res[5]);


};

/* Nb Multiplications = 20, 
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


complex<Q31> a[6];
complex<Q31> res[6];


 Q31 tmp_0;
 Q31 tmp_1;
 Q31 tmp_2;
 Q31 tmp_3;
 Q63 tmp_4;
 Q31 tmp_5;
 Q63 tmp_6;
 Q63 tmp_7;
 Q31 tmp_8;
 Q63 tmp_9;
 Q31 tmp_10;
 Q63 tmp_11;
 Q63 tmp_12;
 Q31 tmp_13;
 Q31 tmp_14;
 Q31 tmp_15;
 Q31 tmp_16;
 Q63 tmp_17;
 Q63 tmp_18;
 Q63 tmp_19;
 Q63 tmp_20;
 Q63 tmp_21;
 Q63 tmp_22;
 Q63 tmp_23;
 Q63 tmp_24;
 Q63 tmp_25;
 Q31 tmp_26;
 Q31 tmp_27;
 Q31 tmp_28;
 Q63 tmp_29;
 Q63 tmp_30;
 Q63 tmp_31;
 Q31 tmp_32;
 Q63 tmp_33;
 Q63 tmp_34;
 Q63 tmp_35;
 Q63 tmp_36;
 Q63 tmp_37;
 Q31 tmp_38;
 Q63 tmp_39;
 Q63 tmp_40;
 Q63 tmp_41;
 Q63 tmp_42;
 Q63 tmp_43;


a[0] = memory<complex<Q31>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q31>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q31>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<Q31>>::load(in,4 * stageLength * strideFactor);
a[5] = memory<complex<Q31>>::load(in,5 * stageLength * strideFactor);


tmp_0 = ( decq(a[0].re,1)   +  decq(a[3].re,1)  ) ;
tmp_1 = ( decq(a[1].re,1)   +  decq(a[4].re,1)  ) ;
tmp_2 = ( decq(a[2].re,1)   +  decq(a[5].re,1)  ) ;
tmp_32 = ( decq( decq(tmp_0,1)  ,1)   +  decq(( decq(tmp_1,1)   +  decq(tmp_2,1)  ) ,1)  ) ;
res[0].re = tmp_32;
tmp_3 = ( decq(a[1].re,1)   -  decq(a[4].re,1)  ) ;
tmp_4 = (C1_6_notw) * tmp_3 ;
tmp_5 = ( decq(a[1].im,1)   -  decq(a[4].im,1)  ) ;
tmp_6 = (C0_6_notw) * tmp_5 ;
tmp_7 = (tmp_4 - tmp_6) ;
tmp_8 = ( decq(a[2].re,1)   -  decq(a[5].re,1)  ) ;
tmp_9 = (C1_6_notw) * tmp_8 ;
tmp_10 = ( decq(a[2].im,1)   -  decq(a[5].im,1)  ) ;
tmp_11 = (C0_6_notw) * tmp_10 ;
tmp_12 = ( (-(tmp_9))   - tmp_11) ;
tmp_13 = ( decq(a[0].re,1)   -  decq(a[3].re,1)  ) ;
tmp_33 = (tmp_7 + (tmp_12 +  widen<Q31>::run(tmp_13)  ) ) ;
res[1].re =  narrow<2,Q31>::run(tmp_33)  ;
tmp_14 = ( decq(a[2].im,1)   +  decq(a[5].im,1)  ) ;
tmp_15 = ( decq(a[1].im,1)   +  decq(a[4].im,1)  ) ;
tmp_16 = ( decq(tmp_1,1)   +  decq(tmp_2,1)  ) ;
tmp_17 = (C1_6_notw) * tmp_16 ;
tmp_34 = ( decq( widen<Q31>::run(tmp_0)  ,1)   + ((C0_6_notw) * ( decq(tmp_14,1)   -  decq(tmp_15,1)  )   - tmp_17) ) ;
res[2].re =  narrow<1,Q31>::run(tmp_34)  ;
tmp_18 = (C0_6_notw) * tmp_8 ;
tmp_19 = (C1_6_notw) * tmp_10 ;
tmp_20 = (tmp_18 - tmp_19) ;
tmp_21 = (C1_6_notw) * tmp_5 ;
tmp_22 = (C0_6_notw) * tmp_3 ;
tmp_23 = (tmp_21 + tmp_22) ;
tmp_24 = (tmp_7 + tmp_12) ;
tmp_25 = (C1_6_notw) *  narrow<NORMALNARROW,Q31>::run(tmp_24)   ;
tmp_35 = ( widen<Q31>::run(tmp_13)   + ((C0_6_notw) *  narrow<NORMALNARROW,Q31>::run((tmp_20 - tmp_23) )    - tmp_25) ) ;
res[3].re =  narrow<2,Q31>::run(tmp_35)  ;
tmp_36 = ( decq( widen<Q31>::run(tmp_0)  ,1)   + ((C0_6_notw) * ( decq(tmp_15,1)   -  decq(tmp_14,1)  )   - tmp_17) ) ;
res[4].re =  narrow<1,Q31>::run(tmp_36)  ;
tmp_37 = ( widen<Q31>::run(tmp_13)   + ((C0_6_notw) *  narrow<NORMALNARROW,Q31>::run((tmp_23 - tmp_20) )    - tmp_25) ) ;
res[5].re =  narrow<2,Q31>::run(tmp_37)  ;
tmp_26 = ( decq(a[0].im,1)   +  decq(a[3].im,1)  ) ;
tmp_38 = ( decq( decq(tmp_26,1)  ,1)   +  decq(( decq(tmp_15,1)   +  decq(tmp_14,1)  ) ,1)  ) ;
res[0].im = tmp_38;
tmp_27 = ( decq(a[0].im,1)   -  decq(a[3].im,1)  ) ;
tmp_39 = (tmp_23 + (tmp_20 +  widen<Q31>::run(tmp_27)  ) ) ;
res[1].im =  narrow<2,Q31>::run(tmp_39)  ;
tmp_28 = ( decq(tmp_15,1)   +  decq(tmp_14,1)  ) ;
tmp_29 = (C1_6_notw) * tmp_28 ;
tmp_40 = ( decq( widen<Q31>::run(tmp_26)  ,1)   + ((C0_6_notw) * ( decq(tmp_1,1)   -  decq(tmp_2,1)  )   - tmp_29) ) ;
res[2].im =  narrow<1,Q31>::run(tmp_40)  ;
tmp_30 = (tmp_23 + tmp_20) ;
tmp_31 = (C1_6_notw) *  narrow<NORMALNARROW,Q31>::run(tmp_30)   ;
tmp_41 = ( widen<Q31>::run(tmp_27)   + ((C0_6_notw) *  narrow<NORMALNARROW,Q31>::run((tmp_7 - tmp_12) )    - tmp_31) ) ;
res[3].im =  narrow<2,Q31>::run(tmp_41)  ;
tmp_42 = ( decq( widen<Q31>::run(tmp_26)  ,1)   + ((C0_6_notw) * ( decq(tmp_2,1)   -  decq(tmp_1,1)  )   - tmp_29) ) ;
res[4].im =  narrow<1,Q31>::run(tmp_42)  ;
tmp_43 = ( widen<Q31>::run(tmp_27)   + ((C0_6_notw) *  narrow<NORMALNARROW,Q31>::run((tmp_12 - tmp_7) )    - tmp_31) ) ;
res[5].im =  narrow<2,Q31>::run(tmp_43)  ;


memory<complex<Q31>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<Q31>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<Q31>>::store(out,2 * stageLength * strideFactor , res[2]);
memory<complex<Q31>>::store(out,3 * stageLength * strideFactor , res[3]);
memory<complex<Q31>>::store(out,4 * stageLength * strideFactor , res[4]);
memory<complex<Q31>>::store(out,5 * stageLength * strideFactor , res[5]);


};

/* Nb Multiplications = 40, 
Nb Additions = 62,
Nb Negates = 31,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 3

*/
 __STATIC_FORCEINLINE void run_op_tw(int stageLength,const complex<Q31> *w,const complex<Q31> *in,complex<Q31> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q31> a[6];
complex<Q31> res[6];


 Q31 tmp_0;
 Q31 tmp_1;
 Q31 tmp_2;
 Q31 tmp_3;
 Q63 tmp_4;
 Q31 tmp_5;
 Q63 tmp_6;
 Q63 tmp_7;
 Q31 tmp_8;
 Q63 tmp_9;
 Q31 tmp_10;
 Q63 tmp_11;
 Q63 tmp_12;
 Q31 tmp_13;
 Q63 tmp_14;
 Q63 tmp_15;
 Q63 tmp_16;
 Q63 tmp_17;
 Q63 tmp_18;
 Q63 tmp_19;
 Q63 tmp_20;
 Q31 tmp_21;
 Q63 tmp_22;
 Q31 tmp_23;
 Q31 tmp_24;
 Q31 tmp_25;
 Q63 tmp_26;
 Q31 tmp_27;
 Q63 tmp_28;
 Q63 tmp_29;
 Q31 tmp_30;
 Q31 tmp_31;
 Q63 tmp_32;
 Q31 tmp_33;
 Q63 tmp_34;
 Q63 tmp_35;
 Q63 tmp_36;
 Q63 tmp_37;
 Q63 tmp_38;
 Q63 tmp_39;
 Q63 tmp_40;
 Q63 tmp_41;
 Q63 tmp_42;
 Q63 tmp_43;
 Q63 tmp_44;
 Q63 tmp_45;
 Q31 tmp_46;
 Q63 tmp_47;
 Q63 tmp_48;
 Q31 tmp_49;
 Q63 tmp_50;
 Q63 tmp_51;
 Q63 tmp_52;
 Q63 tmp_53;
 Q63 tmp_54;
 Q63 tmp_55;
 Q63 tmp_56;
 Q63 tmp_57;
 Q31 tmp_58;
 Q63 tmp_59;
 Q63 tmp_60;
 Q63 tmp_61;
 Q63 tmp_62;
 Q63 tmp_63;
 Q63 tmp_64;
 Q63 tmp_65;
 Q63 tmp_66;
 Q63 tmp_67;
 Q63 tmp_68;
 Q31 tmp_69;


a[0] = memory<complex<Q31>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q31>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q31>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<Q31>>::load(in,4 * stageLength * strideFactor);
a[5] = memory<complex<Q31>>::load(in,5 * stageLength * strideFactor);


tmp_0 = ( decq(a[0].re,1)   +  decq(a[3].re,1)  ) ;
tmp_1 = ( decq(a[1].re,1)   +  decq(a[4].re,1)  ) ;
tmp_2 = ( decq(a[2].re,1)   +  decq(a[5].re,1)  ) ;
tmp_58 = ( decq( decq(tmp_0,1)  ,1)   +  decq(( decq(tmp_1,1)   +  decq(tmp_2,1)  ) ,1)  ) ;
res[0].re = tmp_58;
tmp_3 = ( decq(a[1].re,1)   -  decq(a[4].re,1)  ) ;
tmp_4 = (C1_6_tw) * tmp_3 ;
tmp_5 = ( decq(a[1].im,1)   -  decq(a[4].im,1)  ) ;
tmp_6 = (C0_6_tw) * tmp_5 ;
tmp_7 = (tmp_4 - tmp_6) ;
tmp_8 = ( decq(a[2].re,1)   -  decq(a[5].re,1)  ) ;
tmp_9 = (C1_6_tw) * tmp_8 ;
tmp_10 = ( decq(a[2].im,1)   -  decq(a[5].im,1)  ) ;
tmp_11 = (C0_6_tw) * tmp_10 ;
tmp_12 = ( (-(tmp_9))   - tmp_11) ;
tmp_13 = ( decq(a[0].re,1)   -  decq(a[3].re,1)  ) ;
tmp_59 = (tmp_7 + (tmp_12 +  widen<Q31>::run(tmp_13)  ) ) ;
tmp_14 = tmp_59;
tmp_15 = (C1_6_tw) * tmp_5 ;
tmp_16 = (C0_6_tw) * tmp_3 ;
tmp_17 = (tmp_15 + tmp_16) ;
tmp_18 = (C0_6_tw) * tmp_8 ;
tmp_19 = (C1_6_tw) * tmp_10 ;
tmp_20 = (tmp_18 - tmp_19) ;
tmp_21 = ( decq(a[0].im,1)   -  decq(a[3].im,1)  ) ;
tmp_60 = (tmp_17 + (tmp_20 +  widen<Q31>::run(tmp_21)  ) ) ;
tmp_22 = tmp_60;
res[1].re =  narrow<2,Q31>::run((w[0].re *  narrow<NORMALNARROW,Q31>::run(tmp_14)    + w[0].im *  narrow<NORMALNARROW,Q31>::run(tmp_22)   ) )  ;
tmp_23 = ( decq(a[2].im,1)   +  decq(a[5].im,1)  ) ;
tmp_24 = ( decq(a[1].im,1)   +  decq(a[4].im,1)  ) ;
tmp_25 = ( decq(tmp_23,1)   -  decq(tmp_24,1)  ) ;
tmp_26 = (C0_6_tw) * tmp_25 ;
tmp_27 = ( decq(tmp_1,1)   +  decq(tmp_2,1)  ) ;
tmp_28 = (C1_6_tw) * tmp_27 ;
tmp_61 = ( decq( widen<Q31>::run(tmp_0)  ,1)   + (tmp_26 - tmp_28) ) ;
tmp_29 = tmp_61;
tmp_30 = ( decq(a[0].im,1)   +  decq(a[3].im,1)  ) ;
tmp_31 = ( decq(tmp_1,1)   -  decq(tmp_2,1)  ) ;
tmp_32 = (C0_6_tw) * tmp_31 ;
tmp_33 = ( decq(tmp_24,1)   +  decq(tmp_23,1)  ) ;
tmp_34 = (C1_6_tw) * tmp_33 ;
tmp_62 = ( decq( widen<Q31>::run(tmp_30)  ,1)   + (tmp_32 - tmp_34) ) ;
tmp_35 = tmp_62;
res[2].re =  narrow<1,Q31>::run((w[1].re *  narrow<NORMALNARROW,Q31>::run(tmp_29)    + w[1].im *  narrow<NORMALNARROW,Q31>::run(tmp_35)   ) )  ;
tmp_36 = (tmp_20 - tmp_17) ;
tmp_37 = (C0_6_tw) *  narrow<NORMALNARROW,Q31>::run(tmp_36)   ;
tmp_38 = (tmp_7 + tmp_12) ;
tmp_39 = (C1_6_tw) *  narrow<NORMALNARROW,Q31>::run(tmp_38)   ;
tmp_63 = ( widen<Q31>::run(tmp_13)   + (tmp_37 - tmp_39) ) ;
tmp_40 = tmp_63;
tmp_41 = (tmp_7 - tmp_12) ;
tmp_42 = (C0_6_tw) *  narrow<NORMALNARROW,Q31>::run(tmp_41)   ;
tmp_43 = (tmp_17 + tmp_20) ;
tmp_44 = (C1_6_tw) *  narrow<NORMALNARROW,Q31>::run(tmp_43)   ;
tmp_64 = ( widen<Q31>::run(tmp_21)   + (tmp_42 - tmp_44) ) ;
tmp_45 = tmp_64;
res[3].re =  narrow<2,Q31>::run((w[2].re *  narrow<NORMALNARROW,Q31>::run(tmp_40)    + w[2].im *  narrow<NORMALNARROW,Q31>::run(tmp_45)   ) )  ;
tmp_46 = ( decq(tmp_24,1)   -  decq(tmp_23,1)  ) ;
tmp_47 = (C0_6_tw) * tmp_46 ;
tmp_65 = ( decq( widen<Q31>::run(tmp_0)  ,1)   + (tmp_47 - tmp_28) ) ;
tmp_48 = tmp_65;
tmp_49 = ( decq(tmp_2,1)   -  decq(tmp_1,1)  ) ;
tmp_50 = (C0_6_tw) * tmp_49 ;
tmp_66 = ( decq( widen<Q31>::run(tmp_30)  ,1)   + (tmp_50 - tmp_34) ) ;
tmp_51 = tmp_66;
res[4].re =  narrow<1,Q31>::run((w[3].re *  narrow<NORMALNARROW,Q31>::run(tmp_48)    + w[3].im *  narrow<NORMALNARROW,Q31>::run(tmp_51)   ) )  ;
tmp_52 = (tmp_17 - tmp_20) ;
tmp_53 = (C0_6_tw) *  narrow<NORMALNARROW,Q31>::run(tmp_52)   ;
tmp_67 = ( widen<Q31>::run(tmp_13)   + (tmp_53 - tmp_39) ) ;
tmp_54 = tmp_67;
tmp_55 = (tmp_12 - tmp_7) ;
tmp_56 = (C0_6_tw) *  narrow<NORMALNARROW,Q31>::run(tmp_55)   ;
tmp_68 = ( widen<Q31>::run(tmp_21)   + (tmp_56 - tmp_44) ) ;
tmp_57 = tmp_68;
res[5].re =  narrow<2,Q31>::run((w[4].re *  narrow<NORMALNARROW,Q31>::run(tmp_54)    + w[4].im *  narrow<NORMALNARROW,Q31>::run(tmp_57)   ) )  ;
tmp_69 = ( decq( decq(tmp_30,1)  ,1)   +  decq(( decq(tmp_24,1)   +  decq(tmp_23,1)  ) ,1)  ) ;
res[0].im = tmp_69;
res[1].im =  narrow<2,Q31>::run((w[0].re *  narrow<NORMALNARROW,Q31>::run(tmp_22)    - w[0].im *  narrow<NORMALNARROW,Q31>::run(tmp_14)   ) )  ;
res[2].im =  narrow<1,Q31>::run((w[1].re *  narrow<NORMALNARROW,Q31>::run(tmp_35)    - w[1].im *  narrow<NORMALNARROW,Q31>::run(tmp_29)   ) )  ;
res[3].im =  narrow<2,Q31>::run((w[2].re *  narrow<NORMALNARROW,Q31>::run(tmp_45)    - w[2].im *  narrow<NORMALNARROW,Q31>::run(tmp_40)   ) )  ;
res[4].im =  narrow<1,Q31>::run((w[3].re *  narrow<NORMALNARROW,Q31>::run(tmp_51)    - w[3].im *  narrow<NORMALNARROW,Q31>::run(tmp_48)   ) )  ;
res[5].im =  narrow<2,Q31>::run((w[4].re *  narrow<NORMALNARROW,Q31>::run(tmp_57)    - w[4].im *  narrow<NORMALNARROW,Q31>::run(tmp_54)   ) )  ;


memory<complex<Q31>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<Q31>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<Q31>>::store(out,2 * stageLength * strideFactor , res[2]);
memory<complex<Q31>>::store(out,3 * stageLength * strideFactor , res[3]);
memory<complex<Q31>>::store(out,4 * stageLength * strideFactor , res[4]);
memory<complex<Q31>>::store(out,5 * stageLength * strideFactor , res[5]);


};

/* Nb Multiplications = 20, 
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


complex<Q31> a[6];
complex<Q31> res[6];


 Q31 tmp_0;
 Q31 tmp_1;
 Q31 tmp_2;
 Q31 tmp_3;
 Q63 tmp_4;
 Q31 tmp_5;
 Q63 tmp_6;
 Q63 tmp_7;
 Q31 tmp_8;
 Q63 tmp_9;
 Q31 tmp_10;
 Q63 tmp_11;
 Q63 tmp_12;
 Q31 tmp_13;
 Q31 tmp_14;
 Q31 tmp_15;
 Q31 tmp_16;
 Q63 tmp_17;
 Q63 tmp_18;
 Q63 tmp_19;
 Q63 tmp_20;
 Q63 tmp_21;
 Q63 tmp_22;
 Q63 tmp_23;
 Q63 tmp_24;
 Q63 tmp_25;
 Q31 tmp_26;
 Q31 tmp_27;
 Q31 tmp_28;
 Q63 tmp_29;
 Q63 tmp_30;
 Q63 tmp_31;
 Q31 tmp_32;
 Q63 tmp_33;
 Q63 tmp_34;
 Q63 tmp_35;
 Q63 tmp_36;
 Q63 tmp_37;
 Q31 tmp_38;
 Q63 tmp_39;
 Q63 tmp_40;
 Q63 tmp_41;
 Q63 tmp_42;
 Q63 tmp_43;


a[0] = memory<complex<Q31>>::load(in,0 * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * strideFactor);
a[2] = memory<complex<Q31>>::load(in,2 * strideFactor);
a[3] = memory<complex<Q31>>::load(in,3 * strideFactor);
a[4] = memory<complex<Q31>>::load(in,4 * strideFactor);
a[5] = memory<complex<Q31>>::load(in,5 * strideFactor);


tmp_0 = ( decq(a[0].re,1)   +  decq(a[3].re,1)  ) ;
tmp_1 = ( decq(a[1].re,1)   +  decq(a[4].re,1)  ) ;
tmp_2 = ( decq(a[2].re,1)   +  decq(a[5].re,1)  ) ;
tmp_32 = ( decq( decq(tmp_0,1)  ,1)   +  decq(( decq(tmp_1,1)   +  decq(tmp_2,1)  ) ,1)  ) ;
res[0].re = tmp_32;
tmp_3 = ( decq(a[1].re,1)   -  decq(a[4].re,1)  ) ;
tmp_4 = (C1_6_notw) * tmp_3 ;
tmp_5 = ( decq(a[1].im,1)   -  decq(a[4].im,1)  ) ;
tmp_6 = (C0_6_notw) * tmp_5 ;
tmp_7 = (tmp_4 - tmp_6) ;
tmp_8 = ( decq(a[2].re,1)   -  decq(a[5].re,1)  ) ;
tmp_9 = (C1_6_notw) * tmp_8 ;
tmp_10 = ( decq(a[2].im,1)   -  decq(a[5].im,1)  ) ;
tmp_11 = (C0_6_notw) * tmp_10 ;
tmp_12 = ( (-(tmp_9))   - tmp_11) ;
tmp_13 = ( decq(a[0].re,1)   -  decq(a[3].re,1)  ) ;
tmp_33 = (tmp_7 + (tmp_12 +  widen<Q31>::run(tmp_13)  ) ) ;
res[1].re =  narrow<2,Q31>::run(tmp_33)  ;
tmp_14 = ( decq(a[2].im,1)   +  decq(a[5].im,1)  ) ;
tmp_15 = ( decq(a[1].im,1)   +  decq(a[4].im,1)  ) ;
tmp_16 = ( decq(tmp_1,1)   +  decq(tmp_2,1)  ) ;
tmp_17 = (C1_6_notw) * tmp_16 ;
tmp_34 = ( decq( widen<Q31>::run(tmp_0)  ,1)   + ((C0_6_notw) * ( decq(tmp_14,1)   -  decq(tmp_15,1)  )   - tmp_17) ) ;
res[2].re =  narrow<1,Q31>::run(tmp_34)  ;
tmp_18 = (C0_6_notw) * tmp_8 ;
tmp_19 = (C1_6_notw) * tmp_10 ;
tmp_20 = (tmp_18 - tmp_19) ;
tmp_21 = (C1_6_notw) * tmp_5 ;
tmp_22 = (C0_6_notw) * tmp_3 ;
tmp_23 = (tmp_21 + tmp_22) ;
tmp_24 = (tmp_7 + tmp_12) ;
tmp_25 = (C1_6_notw) *  narrow<NORMALNARROW,Q31>::run(tmp_24)   ;
tmp_35 = ( widen<Q31>::run(tmp_13)   + ((C0_6_notw) *  narrow<NORMALNARROW,Q31>::run((tmp_20 - tmp_23) )    - tmp_25) ) ;
res[3].re =  narrow<2,Q31>::run(tmp_35)  ;
tmp_36 = ( decq( widen<Q31>::run(tmp_0)  ,1)   + ((C0_6_notw) * ( decq(tmp_15,1)   -  decq(tmp_14,1)  )   - tmp_17) ) ;
res[4].re =  narrow<1,Q31>::run(tmp_36)  ;
tmp_37 = ( widen<Q31>::run(tmp_13)   + ((C0_6_notw) *  narrow<NORMALNARROW,Q31>::run((tmp_23 - tmp_20) )    - tmp_25) ) ;
res[5].re =  narrow<2,Q31>::run(tmp_37)  ;
tmp_26 = ( decq(a[0].im,1)   +  decq(a[3].im,1)  ) ;
tmp_38 = ( decq( decq(tmp_26,1)  ,1)   +  decq(( decq(tmp_15,1)   +  decq(tmp_14,1)  ) ,1)  ) ;
res[0].im = tmp_38;
tmp_27 = ( decq(a[0].im,1)   -  decq(a[3].im,1)  ) ;
tmp_39 = (tmp_23 + (tmp_20 +  widen<Q31>::run(tmp_27)  ) ) ;
res[1].im =  narrow<2,Q31>::run(tmp_39)  ;
tmp_28 = ( decq(tmp_15,1)   +  decq(tmp_14,1)  ) ;
tmp_29 = (C1_6_notw) * tmp_28 ;
tmp_40 = ( decq( widen<Q31>::run(tmp_26)  ,1)   + ((C0_6_notw) * ( decq(tmp_1,1)   -  decq(tmp_2,1)  )   - tmp_29) ) ;
res[2].im =  narrow<1,Q31>::run(tmp_40)  ;
tmp_30 = (tmp_23 + tmp_20) ;
tmp_31 = (C1_6_notw) *  narrow<NORMALNARROW,Q31>::run(tmp_30)   ;
tmp_41 = ( widen<Q31>::run(tmp_27)   + ((C0_6_notw) *  narrow<NORMALNARROW,Q31>::run((tmp_7 - tmp_12) )    - tmp_31) ) ;
res[3].im =  narrow<2,Q31>::run(tmp_41)  ;
tmp_42 = ( decq( widen<Q31>::run(tmp_26)  ,1)   + ((C0_6_notw) * ( decq(tmp_2,1)   -  decq(tmp_1,1)  )   - tmp_29) ) ;
res[4].im =  narrow<1,Q31>::run(tmp_42)  ;
tmp_43 = ( widen<Q31>::run(tmp_27)   + ((C0_6_notw) *  narrow<NORMALNARROW,Q31>::run((tmp_12 - tmp_7) )    - tmp_31) ) ;
res[5].im =  narrow<2,Q31>::run(tmp_43)  ;


memory<complex<Q31>>::store(in,0 * strideFactor , res[0]);
memory<complex<Q31>>::store(in,1 * strideFactor , res[1]);
memory<complex<Q31>>::store(in,2 * strideFactor , res[2]);
memory<complex<Q31>>::store(in,3 * strideFactor , res[3]);
memory<complex<Q31>>::store(in,4 * strideFactor , res[4]);
memory<complex<Q31>>::store(in,5 * strideFactor , res[5]);


};

/* Nb Multiplications = 20, 
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


complex<Q31> a[6];
complex<Q31> res[6];


 Q31 tmp_0;
 Q31 tmp_1;
 Q31 tmp_2;
 Q31 tmp_3;
 Q63 tmp_4;
 Q31 tmp_5;
 Q63 tmp_6;
 Q63 tmp_7;
 Q31 tmp_8;
 Q63 tmp_9;
 Q31 tmp_10;
 Q63 tmp_11;
 Q63 tmp_12;
 Q31 tmp_13;
 Q31 tmp_14;
 Q31 tmp_15;
 Q31 tmp_16;
 Q63 tmp_17;
 Q63 tmp_18;
 Q63 tmp_19;
 Q63 tmp_20;
 Q63 tmp_21;
 Q63 tmp_22;
 Q63 tmp_23;
 Q63 tmp_24;
 Q63 tmp_25;
 Q31 tmp_26;
 Q31 tmp_27;
 Q31 tmp_28;
 Q63 tmp_29;
 Q63 tmp_30;
 Q63 tmp_31;
 Q31 tmp_32;
 Q63 tmp_33;
 Q63 tmp_34;
 Q63 tmp_35;
 Q63 tmp_36;
 Q63 tmp_37;
 Q31 tmp_38;
 Q63 tmp_39;
 Q63 tmp_40;
 Q63 tmp_41;
 Q63 tmp_42;
 Q63 tmp_43;


a[0] = memory<complex<Q31>>::load(in,0 * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * strideFactor);
a[2] = memory<complex<Q31>>::load(in,2 * strideFactor);
a[3] = memory<complex<Q31>>::load(in,3 * strideFactor);
a[4] = memory<complex<Q31>>::load(in,4 * strideFactor);
a[5] = memory<complex<Q31>>::load(in,5 * strideFactor);


tmp_0 = ( decq(a[0].re,1)   +  decq(a[3].re,1)  ) ;
tmp_1 = ( decq(a[1].re,1)   +  decq(a[4].re,1)  ) ;
tmp_2 = ( decq(a[2].re,1)   +  decq(a[5].re,1)  ) ;
tmp_32 = ( decq( decq(tmp_0,1)  ,1)   +  decq(( decq(tmp_1,1)   +  decq(tmp_2,1)  ) ,1)  ) ;
res[0].re = tmp_32;
tmp_3 = ( decq(a[1].re,1)   -  decq(a[4].re,1)  ) ;
tmp_4 = (C1_6_notw) * tmp_3 ;
tmp_5 = ( decq(a[1].im,1)   -  decq(a[4].im,1)  ) ;
tmp_6 = (C0_6_notw) * tmp_5 ;
tmp_7 = (tmp_4 - tmp_6) ;
tmp_8 = ( decq(a[2].re,1)   -  decq(a[5].re,1)  ) ;
tmp_9 = (C1_6_notw) * tmp_8 ;
tmp_10 = ( decq(a[2].im,1)   -  decq(a[5].im,1)  ) ;
tmp_11 = (C0_6_notw) * tmp_10 ;
tmp_12 = ( (-(tmp_9))   - tmp_11) ;
tmp_13 = ( decq(a[0].re,1)   -  decq(a[3].re,1)  ) ;
tmp_33 = (tmp_7 + (tmp_12 +  widen<Q31>::run(tmp_13)  ) ) ;
res[1].re =  narrow<2,Q31>::run(tmp_33)  ;
tmp_14 = ( decq(a[2].im,1)   +  decq(a[5].im,1)  ) ;
tmp_15 = ( decq(a[1].im,1)   +  decq(a[4].im,1)  ) ;
tmp_16 = ( decq(tmp_1,1)   +  decq(tmp_2,1)  ) ;
tmp_17 = (C1_6_notw) * tmp_16 ;
tmp_34 = ( decq( widen<Q31>::run(tmp_0)  ,1)   + ((C0_6_notw) * ( decq(tmp_14,1)   -  decq(tmp_15,1)  )   - tmp_17) ) ;
res[2].re =  narrow<1,Q31>::run(tmp_34)  ;
tmp_18 = (C0_6_notw) * tmp_8 ;
tmp_19 = (C1_6_notw) * tmp_10 ;
tmp_20 = (tmp_18 - tmp_19) ;
tmp_21 = (C1_6_notw) * tmp_5 ;
tmp_22 = (C0_6_notw) * tmp_3 ;
tmp_23 = (tmp_21 + tmp_22) ;
tmp_24 = (tmp_7 + tmp_12) ;
tmp_25 = (C1_6_notw) *  narrow<NORMALNARROW,Q31>::run(tmp_24)   ;
tmp_35 = ( widen<Q31>::run(tmp_13)   + ((C0_6_notw) *  narrow<NORMALNARROW,Q31>::run((tmp_20 - tmp_23) )    - tmp_25) ) ;
res[3].re =  narrow<2,Q31>::run(tmp_35)  ;
tmp_36 = ( decq( widen<Q31>::run(tmp_0)  ,1)   + ((C0_6_notw) * ( decq(tmp_15,1)   -  decq(tmp_14,1)  )   - tmp_17) ) ;
res[4].re =  narrow<1,Q31>::run(tmp_36)  ;
tmp_37 = ( widen<Q31>::run(tmp_13)   + ((C0_6_notw) *  narrow<NORMALNARROW,Q31>::run((tmp_23 - tmp_20) )    - tmp_25) ) ;
res[5].re =  narrow<2,Q31>::run(tmp_37)  ;
tmp_26 = ( decq(a[0].im,1)   +  decq(a[3].im,1)  ) ;
tmp_38 = ( decq( decq(tmp_26,1)  ,1)   +  decq(( decq(tmp_15,1)   +  decq(tmp_14,1)  ) ,1)  ) ;
res[0].im = tmp_38;
tmp_27 = ( decq(a[0].im,1)   -  decq(a[3].im,1)  ) ;
tmp_39 = (tmp_23 + (tmp_20 +  widen<Q31>::run(tmp_27)  ) ) ;
res[1].im =  narrow<2,Q31>::run(tmp_39)  ;
tmp_28 = ( decq(tmp_15,1)   +  decq(tmp_14,1)  ) ;
tmp_29 = (C1_6_notw) * tmp_28 ;
tmp_40 = ( decq( widen<Q31>::run(tmp_26)  ,1)   + ((C0_6_notw) * ( decq(tmp_1,1)   -  decq(tmp_2,1)  )   - tmp_29) ) ;
res[2].im =  narrow<1,Q31>::run(tmp_40)  ;
tmp_30 = (tmp_23 + tmp_20) ;
tmp_31 = (C1_6_notw) *  narrow<NORMALNARROW,Q31>::run(tmp_30)   ;
tmp_41 = ( widen<Q31>::run(tmp_27)   + ((C0_6_notw) *  narrow<NORMALNARROW,Q31>::run((tmp_7 - tmp_12) )    - tmp_31) ) ;
res[3].im =  narrow<2,Q31>::run(tmp_41)  ;
tmp_42 = ( decq( widen<Q31>::run(tmp_26)  ,1)   + ((C0_6_notw) * ( decq(tmp_2,1)   -  decq(tmp_1,1)  )   - tmp_29) ) ;
res[4].im =  narrow<1,Q31>::run(tmp_42)  ;
tmp_43 = ( widen<Q31>::run(tmp_27)   + ((C0_6_notw) *  narrow<NORMALNARROW,Q31>::run((tmp_12 - tmp_7) )    - tmp_31) ) ;
res[5].im =  narrow<2,Q31>::run(tmp_43)  ;


memory<complex<Q31>>::store(out,0 * strideFactor , res[0]);
memory<complex<Q31>>::store(out,1 * strideFactor , res[1]);
memory<complex<Q31>>::store(out,2 * strideFactor , res[2]);
memory<complex<Q31>>::store(out,3 * strideFactor , res[3]);
memory<complex<Q31>>::store(out,4 * strideFactor , res[4]);
memory<complex<Q31>>::store(out,5 * strideFactor , res[5]);


};

};

#endif /* #ifndef _RADIX_Q31_6_H */