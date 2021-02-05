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

static constexpr Q15 C0_5_tw{(q15_t)0x278E};
  static constexpr Q15 C1_5_tw{(q15_t)0x678E};
  static constexpr Q15 C2_5_tw{(q15_t)0x4B3D};
  static constexpr Q15 C3_5_tw{(q15_t)0x79BC};
  
static constexpr Q15 C0_5_notw{(q15_t)0x278E};
  static constexpr Q15 C1_5_notw{(q15_t)0x678E};
  static constexpr Q15 C2_5_notw{(q15_t)0x4B3D};
  static constexpr Q15 C3_5_notw{(q15_t)0x79BC};
  

/* Nb Multiplications = 24, 
Nb Additions = 48,
Nb Negates = 12,
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


 Q15 tmp_0;
 Q31 tmp_1;
 Q15 tmp_2;
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
 Q31 tmp_16;
 Q31 tmp_17;
 Q31 tmp_18;
 Q31 tmp_19;
 Q15 tmp_20;
 Q31 tmp_21;
 Q15 tmp_22;
 Q31 tmp_23;
 Q31 tmp_24;
 Q31 tmp_25;
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
 Q31 tmp_38;
 Q31 tmp_39;
 Q15 tmp_40;
 Q31 tmp_41;
 Q31 tmp_42;
 Q31 tmp_43;
 Q31 tmp_44;
 Q15 tmp_45;
 Q31 tmp_46;
 Q31 tmp_47;
 Q31 tmp_48;
 Q31 tmp_49;


a[0] = memory<complex<Q15>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<Q15>>::load(in,4 * stageLength * strideFactor);


tmp_40 = ( decq( decq(a[0].re,1)  ,3)   +  decq(( decq( decq(a[1].re,1)  ,2)   +  decq(( decq( decq(a[2].re,1)  ,1)   +  decq(( decq(a[3].re,1)   +  decq(a[4].re,1)  ) ,1)  ) ,1)  ) ,1)  ) ;
res[0].re = tmp_40;
tmp_0 = ( decq(a[1].re,1)   +  decq(a[4].re,1)  ) ;
tmp_1 = (C0_5_notw) * tmp_0 ;
tmp_2 = ( decq(a[2].re,1)   +  decq(a[3].re,1)  ) ;
tmp_3 = (C1_5_notw) * tmp_2 ;
tmp_4 = (tmp_1 - tmp_3) ;
tmp_5 = (C2_5_notw) * a[2].im ;
tmp_6 = (C3_5_notw) * a[1].im ;
tmp_7 = (tmp_5 + tmp_6) ;
tmp_8 = (C2_5_notw) * a[3].im ;
tmp_9 = (C3_5_notw) * a[4].im ;
tmp_10 = (tmp_8 + tmp_9) ;
tmp_41 = (tmp_4 +  decq((tmp_7 + ( widen<Q15>::run(a[0].re)   - tmp_10) ) ,1)  ) ;
res[1].re =  narrow<3,Q15>::run(tmp_41)  ;
tmp_11 = (C0_5_notw) * tmp_2 ;
tmp_12 = (C1_5_notw) * tmp_0 ;
tmp_13 = (tmp_11 - tmp_12) ;
tmp_14 = (C2_5_notw) * a[1].im ;
tmp_15 = (C3_5_notw) * a[3].im ;
tmp_16 = (tmp_14 + tmp_15) ;
tmp_17 = (C2_5_notw) * a[4].im ;
tmp_18 = (C3_5_notw) * a[2].im ;
tmp_19 = (tmp_17 + tmp_18) ;
tmp_42 = (tmp_13 +  decq((tmp_16 + ( widen<Q15>::run(a[0].re)   - tmp_19) ) ,1)  ) ;
res[2].re =  narrow<3,Q15>::run(tmp_42)  ;
tmp_43 = (tmp_13 +  decq((tmp_19 + ( widen<Q15>::run(a[0].re)   - tmp_16) ) ,1)  ) ;
res[3].re =  narrow<3,Q15>::run(tmp_43)  ;
tmp_44 = (tmp_4 +  decq((tmp_10 + ( widen<Q15>::run(a[0].re)   - tmp_7) ) ,1)  ) ;
res[4].re =  narrow<3,Q15>::run(tmp_44)  ;
tmp_45 = ( decq( decq(a[0].im,1)  ,3)   +  decq(( decq( decq(a[1].im,1)  ,2)   +  decq(( decq( decq(a[2].im,1)  ,1)   +  decq(( decq(a[3].im,1)   +  decq(a[4].im,1)  ) ,1)  ) ,1)  ) ,1)  ) ;
res[0].im = tmp_45;
tmp_20 = ( decq(a[1].im,1)   +  decq(a[4].im,1)  ) ;
tmp_21 = (C0_5_notw) * tmp_20 ;
tmp_22 = ( decq(a[2].im,1)   +  decq(a[3].im,1)  ) ;
tmp_23 = (C1_5_notw) * tmp_22 ;
tmp_24 = (tmp_21 - tmp_23) ;
tmp_25 = (C2_5_notw) * a[3].re ;
tmp_26 = (C3_5_notw) * a[4].re ;
tmp_27 = (tmp_25 + tmp_26) ;
tmp_28 = (C2_5_notw) * a[2].re ;
tmp_29 = (C3_5_notw) * a[1].re ;
tmp_30 = (tmp_28 + tmp_29) ;
tmp_46 = (tmp_24 +  decq((tmp_27 + ( widen<Q15>::run(a[0].im)   - tmp_30) ) ,1)  ) ;
res[1].im =  narrow<3,Q15>::run(tmp_46)  ;
tmp_31 = (C0_5_notw) * tmp_22 ;
tmp_32 = (C1_5_notw) * tmp_20 ;
tmp_33 = (tmp_31 - tmp_32) ;
tmp_34 = (C2_5_notw) * a[4].re ;
tmp_35 = (C3_5_notw) * a[2].re ;
tmp_36 = (tmp_34 + tmp_35) ;
tmp_37 = (C2_5_notw) * a[1].re ;
tmp_38 = (C3_5_notw) * a[3].re ;
tmp_39 = (tmp_37 + tmp_38) ;
tmp_47 = (tmp_33 +  decq((tmp_36 + ( widen<Q15>::run(a[0].im)   - tmp_39) ) ,1)  ) ;
res[2].im =  narrow<3,Q15>::run(tmp_47)  ;
tmp_48 = (tmp_33 +  decq((tmp_39 + ( widen<Q15>::run(a[0].im)   - tmp_36) ) ,1)  ) ;
res[3].im =  narrow<3,Q15>::run(tmp_48)  ;
tmp_49 = (tmp_24 +  decq((tmp_30 + ( widen<Q15>::run(a[0].im)   - tmp_27) ) ,1)  ) ;
res[4].im =  narrow<3,Q15>::run(tmp_49)  ;


memory<complex<Q15>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<Q15>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<Q15>>::store(in,2 * stageLength * strideFactor , res[2]);
memory<complex<Q15>>::store(in,3 * stageLength * strideFactor , res[3]);
memory<complex<Q15>>::store(in,4 * stageLength * strideFactor , res[4]);


};

/* Nb Multiplications = 40, 
Nb Additions = 56,
Nb Negates = 16,
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


 Q15 tmp_0;
 Q31 tmp_1;
 Q15 tmp_2;
 Q31 tmp_3;
 Q31 tmp_4;
 Q31 tmp_5;
 Q31 tmp_6;
 Q31 tmp_7;
 Q31 tmp_8;
 Q31 tmp_9;
 Q31 tmp_10;
 Q31 tmp_11;
 Q15 tmp_12;
 Q31 tmp_13;
 Q15 tmp_14;
 Q31 tmp_15;
 Q31 tmp_16;
 Q31 tmp_17;
 Q31 tmp_18;
 Q31 tmp_19;
 Q31 tmp_20;
 Q31 tmp_21;
 Q31 tmp_22;
 Q31 tmp_23;
 Q31 tmp_24;
 Q31 tmp_25;
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
 Q31 tmp_38;
 Q31 tmp_39;
 Q31 tmp_40;
 Q31 tmp_41;
 Q31 tmp_42;
 Q31 tmp_43;
 Q31 tmp_44;
 Q31 tmp_45;
 Q31 tmp_46;
 Q31 tmp_47;
 Q15 tmp_48;
 Q31 tmp_49;
 Q31 tmp_50;
 Q31 tmp_51;
 Q31 tmp_52;
 Q31 tmp_53;
 Q31 tmp_54;
 Q31 tmp_55;
 Q31 tmp_56;
 Q15 tmp_57;


a[0] = memory<complex<Q15>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<Q15>>::load(in,4 * stageLength * strideFactor);


tmp_48 = ( decq( decq(a[0].re,1)  ,3)   +  decq(( decq( decq(a[1].re,1)  ,2)   +  decq(( decq( decq(a[2].re,1)  ,1)   +  decq(( decq(a[3].re,1)   +  decq(a[4].re,1)  ) ,1)  ) ,1)  ) ,1)  ) ;
res[0].re = tmp_48;
tmp_0 = ( decq(a[1].re,1)   +  decq(a[4].re,1)  ) ;
tmp_1 = (C0_5_tw) * tmp_0 ;
tmp_2 = ( decq(a[2].re,1)   +  decq(a[3].re,1)  ) ;
tmp_3 = (C1_5_tw) * tmp_2 ;
tmp_4 = (tmp_1 - tmp_3) ;
tmp_5 = (C2_5_tw) * a[2].im ;
tmp_6 = (C3_5_tw) * a[1].im ;
tmp_7 = (tmp_5 + tmp_6) ;
tmp_8 = (C2_5_tw) * a[3].im ;
tmp_9 = (C3_5_tw) * a[4].im ;
tmp_10 = (tmp_8 + tmp_9) ;
tmp_49 = (tmp_4 +  decq((tmp_7 + ( widen<Q15>::run(a[0].re)   - tmp_10) ) ,1)  ) ;
tmp_11 = tmp_49;
tmp_12 = ( decq(a[1].im,1)   +  decq(a[4].im,1)  ) ;
tmp_13 = (C0_5_tw) * tmp_12 ;
tmp_14 = ( decq(a[2].im,1)   +  decq(a[3].im,1)  ) ;
tmp_15 = (C1_5_tw) * tmp_14 ;
tmp_16 = (tmp_13 - tmp_15) ;
tmp_17 = (C2_5_tw) * a[3].re ;
tmp_18 = (C3_5_tw) * a[4].re ;
tmp_19 = (tmp_17 + tmp_18) ;
tmp_20 = (C2_5_tw) * a[2].re ;
tmp_21 = (C3_5_tw) * a[1].re ;
tmp_22 = (tmp_20 + tmp_21) ;
tmp_50 = (tmp_16 +  decq((tmp_19 + ( widen<Q15>::run(a[0].im)   - tmp_22) ) ,1)  ) ;
tmp_23 = tmp_50;
res[1].re =  narrow<3,Q15>::run((w[0].re *  narrow<NORMALNARROW,Q15>::run(tmp_11)    - w[0].im *  narrow<NORMALNARROW,Q15>::run(tmp_23)   ) )  ;
tmp_24 = (C0_5_tw) * tmp_2 ;
tmp_25 = (C1_5_tw) * tmp_0 ;
tmp_26 = (tmp_24 - tmp_25) ;
tmp_27 = (C2_5_tw) * a[1].im ;
tmp_28 = (C3_5_tw) * a[3].im ;
tmp_29 = (tmp_27 + tmp_28) ;
tmp_30 = (C2_5_tw) * a[4].im ;
tmp_31 = (C3_5_tw) * a[2].im ;
tmp_32 = (tmp_30 + tmp_31) ;
tmp_51 = (tmp_26 +  decq((tmp_29 + ( widen<Q15>::run(a[0].re)   - tmp_32) ) ,1)  ) ;
tmp_33 = tmp_51;
tmp_34 = (C0_5_tw) * tmp_14 ;
tmp_35 = (C1_5_tw) * tmp_12 ;
tmp_36 = (tmp_34 - tmp_35) ;
tmp_37 = (C2_5_tw) * a[4].re ;
tmp_38 = (C3_5_tw) * a[2].re ;
tmp_39 = (tmp_37 + tmp_38) ;
tmp_40 = (C2_5_tw) * a[1].re ;
tmp_41 = (C3_5_tw) * a[3].re ;
tmp_42 = (tmp_40 + tmp_41) ;
tmp_52 = (tmp_36 +  decq((tmp_39 + ( widen<Q15>::run(a[0].im)   - tmp_42) ) ,1)  ) ;
tmp_43 = tmp_52;
res[2].re =  narrow<3,Q15>::run((w[1].re *  narrow<NORMALNARROW,Q15>::run(tmp_33)    - w[1].im *  narrow<NORMALNARROW,Q15>::run(tmp_43)   ) )  ;
tmp_53 = (tmp_26 +  decq((tmp_32 + ( widen<Q15>::run(a[0].re)   - tmp_29) ) ,1)  ) ;
tmp_44 = tmp_53;
tmp_54 = (tmp_36 +  decq((tmp_42 + ( widen<Q15>::run(a[0].im)   - tmp_39) ) ,1)  ) ;
tmp_45 = tmp_54;
res[3].re =  narrow<3,Q15>::run((w[2].re *  narrow<NORMALNARROW,Q15>::run(tmp_44)    - w[2].im *  narrow<NORMALNARROW,Q15>::run(tmp_45)   ) )  ;
tmp_55 = (tmp_4 +  decq((tmp_10 + ( widen<Q15>::run(a[0].re)   - tmp_7) ) ,1)  ) ;
tmp_46 = tmp_55;
tmp_56 = (tmp_16 +  decq((tmp_22 + ( widen<Q15>::run(a[0].im)   - tmp_19) ) ,1)  ) ;
tmp_47 = tmp_56;
res[4].re =  narrow<3,Q15>::run((w[3].re *  narrow<NORMALNARROW,Q15>::run(tmp_46)    - w[3].im *  narrow<NORMALNARROW,Q15>::run(tmp_47)   ) )  ;
tmp_57 = ( decq( decq(a[0].im,1)  ,3)   +  decq(( decq( decq(a[1].im,1)  ,2)   +  decq(( decq( decq(a[2].im,1)  ,1)   +  decq(( decq(a[3].im,1)   +  decq(a[4].im,1)  ) ,1)  ) ,1)  ) ,1)  ) ;
res[0].im = tmp_57;
res[1].im =  narrow<3,Q15>::run((w[0].re *  narrow<NORMALNARROW,Q15>::run(tmp_23)    + w[0].im *  narrow<NORMALNARROW,Q15>::run(tmp_11)   ) )  ;
res[2].im =  narrow<3,Q15>::run((w[1].re *  narrow<NORMALNARROW,Q15>::run(tmp_43)    + w[1].im *  narrow<NORMALNARROW,Q15>::run(tmp_33)   ) )  ;
res[3].im =  narrow<3,Q15>::run((w[2].re *  narrow<NORMALNARROW,Q15>::run(tmp_45)    + w[2].im *  narrow<NORMALNARROW,Q15>::run(tmp_44)   ) )  ;
res[4].im =  narrow<3,Q15>::run((w[3].re *  narrow<NORMALNARROW,Q15>::run(tmp_47)    + w[3].im *  narrow<NORMALNARROW,Q15>::run(tmp_46)   ) )  ;


memory<complex<Q15>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<Q15>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<Q15>>::store(in,2 * stageLength * strideFactor , res[2]);
memory<complex<Q15>>::store(in,3 * stageLength * strideFactor , res[3]);
memory<complex<Q15>>::store(in,4 * stageLength * strideFactor , res[4]);


};

/* Nb Multiplications = 24, 
Nb Additions = 48,
Nb Negates = 12,
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


 Q15 tmp_0;
 Q31 tmp_1;
 Q15 tmp_2;
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
 Q31 tmp_16;
 Q31 tmp_17;
 Q31 tmp_18;
 Q31 tmp_19;
 Q15 tmp_20;
 Q31 tmp_21;
 Q15 tmp_22;
 Q31 tmp_23;
 Q31 tmp_24;
 Q31 tmp_25;
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
 Q31 tmp_38;
 Q31 tmp_39;
 Q15 tmp_40;
 Q31 tmp_41;
 Q31 tmp_42;
 Q31 tmp_43;
 Q31 tmp_44;
 Q15 tmp_45;
 Q31 tmp_46;
 Q31 tmp_47;
 Q31 tmp_48;
 Q31 tmp_49;


a[0] = memory<complex<Q15>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<Q15>>::load(in,4 * stageLength * strideFactor);


tmp_40 = ( decq( decq(a[0].re,1)  ,3)   +  decq(( decq( decq(a[1].re,1)  ,2)   +  decq(( decq( decq(a[2].re,1)  ,1)   +  decq(( decq(a[3].re,1)   +  decq(a[4].re,1)  ) ,1)  ) ,1)  ) ,1)  ) ;
res[0].re = tmp_40;
tmp_0 = ( decq(a[1].re,1)   +  decq(a[4].re,1)  ) ;
tmp_1 = (C0_5_notw) * tmp_0 ;
tmp_2 = ( decq(a[2].re,1)   +  decq(a[3].re,1)  ) ;
tmp_3 = (C1_5_notw) * tmp_2 ;
tmp_4 = (tmp_1 - tmp_3) ;
tmp_5 = (C2_5_notw) * a[2].im ;
tmp_6 = (C3_5_notw) * a[1].im ;
tmp_7 = (tmp_5 + tmp_6) ;
tmp_8 = (C2_5_notw) * a[3].im ;
tmp_9 = (C3_5_notw) * a[4].im ;
tmp_10 = (tmp_8 + tmp_9) ;
tmp_41 = (tmp_4 +  decq((tmp_7 + ( widen<Q15>::run(a[0].re)   - tmp_10) ) ,1)  ) ;
res[1].re =  narrow<3,Q15>::run(tmp_41)  ;
tmp_11 = (C0_5_notw) * tmp_2 ;
tmp_12 = (C1_5_notw) * tmp_0 ;
tmp_13 = (tmp_11 - tmp_12) ;
tmp_14 = (C2_5_notw) * a[1].im ;
tmp_15 = (C3_5_notw) * a[3].im ;
tmp_16 = (tmp_14 + tmp_15) ;
tmp_17 = (C2_5_notw) * a[4].im ;
tmp_18 = (C3_5_notw) * a[2].im ;
tmp_19 = (tmp_17 + tmp_18) ;
tmp_42 = (tmp_13 +  decq((tmp_16 + ( widen<Q15>::run(a[0].re)   - tmp_19) ) ,1)  ) ;
res[2].re =  narrow<3,Q15>::run(tmp_42)  ;
tmp_43 = (tmp_13 +  decq((tmp_19 + ( widen<Q15>::run(a[0].re)   - tmp_16) ) ,1)  ) ;
res[3].re =  narrow<3,Q15>::run(tmp_43)  ;
tmp_44 = (tmp_4 +  decq((tmp_10 + ( widen<Q15>::run(a[0].re)   - tmp_7) ) ,1)  ) ;
res[4].re =  narrow<3,Q15>::run(tmp_44)  ;
tmp_45 = ( decq( decq(a[0].im,1)  ,3)   +  decq(( decq( decq(a[1].im,1)  ,2)   +  decq(( decq( decq(a[2].im,1)  ,1)   +  decq(( decq(a[3].im,1)   +  decq(a[4].im,1)  ) ,1)  ) ,1)  ) ,1)  ) ;
res[0].im = tmp_45;
tmp_20 = ( decq(a[1].im,1)   +  decq(a[4].im,1)  ) ;
tmp_21 = (C0_5_notw) * tmp_20 ;
tmp_22 = ( decq(a[2].im,1)   +  decq(a[3].im,1)  ) ;
tmp_23 = (C1_5_notw) * tmp_22 ;
tmp_24 = (tmp_21 - tmp_23) ;
tmp_25 = (C2_5_notw) * a[3].re ;
tmp_26 = (C3_5_notw) * a[4].re ;
tmp_27 = (tmp_25 + tmp_26) ;
tmp_28 = (C2_5_notw) * a[2].re ;
tmp_29 = (C3_5_notw) * a[1].re ;
tmp_30 = (tmp_28 + tmp_29) ;
tmp_46 = (tmp_24 +  decq((tmp_27 + ( widen<Q15>::run(a[0].im)   - tmp_30) ) ,1)  ) ;
res[1].im =  narrow<3,Q15>::run(tmp_46)  ;
tmp_31 = (C0_5_notw) * tmp_22 ;
tmp_32 = (C1_5_notw) * tmp_20 ;
tmp_33 = (tmp_31 - tmp_32) ;
tmp_34 = (C2_5_notw) * a[4].re ;
tmp_35 = (C3_5_notw) * a[2].re ;
tmp_36 = (tmp_34 + tmp_35) ;
tmp_37 = (C2_5_notw) * a[1].re ;
tmp_38 = (C3_5_notw) * a[3].re ;
tmp_39 = (tmp_37 + tmp_38) ;
tmp_47 = (tmp_33 +  decq((tmp_36 + ( widen<Q15>::run(a[0].im)   - tmp_39) ) ,1)  ) ;
res[2].im =  narrow<3,Q15>::run(tmp_47)  ;
tmp_48 = (tmp_33 +  decq((tmp_39 + ( widen<Q15>::run(a[0].im)   - tmp_36) ) ,1)  ) ;
res[3].im =  narrow<3,Q15>::run(tmp_48)  ;
tmp_49 = (tmp_24 +  decq((tmp_30 + ( widen<Q15>::run(a[0].im)   - tmp_27) ) ,1)  ) ;
res[4].im =  narrow<3,Q15>::run(tmp_49)  ;


memory<complex<Q15>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<Q15>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<Q15>>::store(out,2 * stageLength * strideFactor , res[2]);
memory<complex<Q15>>::store(out,3 * stageLength * strideFactor , res[3]);
memory<complex<Q15>>::store(out,4 * stageLength * strideFactor , res[4]);


};

/* Nb Multiplications = 40, 
Nb Additions = 56,
Nb Negates = 16,
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


 Q15 tmp_0;
 Q31 tmp_1;
 Q15 tmp_2;
 Q31 tmp_3;
 Q31 tmp_4;
 Q31 tmp_5;
 Q31 tmp_6;
 Q31 tmp_7;
 Q31 tmp_8;
 Q31 tmp_9;
 Q31 tmp_10;
 Q31 tmp_11;
 Q15 tmp_12;
 Q31 tmp_13;
 Q15 tmp_14;
 Q31 tmp_15;
 Q31 tmp_16;
 Q31 tmp_17;
 Q31 tmp_18;
 Q31 tmp_19;
 Q31 tmp_20;
 Q31 tmp_21;
 Q31 tmp_22;
 Q31 tmp_23;
 Q31 tmp_24;
 Q31 tmp_25;
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
 Q31 tmp_38;
 Q31 tmp_39;
 Q31 tmp_40;
 Q31 tmp_41;
 Q31 tmp_42;
 Q31 tmp_43;
 Q31 tmp_44;
 Q31 tmp_45;
 Q31 tmp_46;
 Q31 tmp_47;
 Q15 tmp_48;
 Q31 tmp_49;
 Q31 tmp_50;
 Q31 tmp_51;
 Q31 tmp_52;
 Q31 tmp_53;
 Q31 tmp_54;
 Q31 tmp_55;
 Q31 tmp_56;
 Q15 tmp_57;


a[0] = memory<complex<Q15>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<Q15>>::load(in,4 * stageLength * strideFactor);


tmp_48 = ( decq( decq(a[0].re,1)  ,3)   +  decq(( decq( decq(a[1].re,1)  ,2)   +  decq(( decq( decq(a[2].re,1)  ,1)   +  decq(( decq(a[3].re,1)   +  decq(a[4].re,1)  ) ,1)  ) ,1)  ) ,1)  ) ;
res[0].re = tmp_48;
tmp_0 = ( decq(a[1].re,1)   +  decq(a[4].re,1)  ) ;
tmp_1 = (C0_5_tw) * tmp_0 ;
tmp_2 = ( decq(a[2].re,1)   +  decq(a[3].re,1)  ) ;
tmp_3 = (C1_5_tw) * tmp_2 ;
tmp_4 = (tmp_1 - tmp_3) ;
tmp_5 = (C2_5_tw) * a[2].im ;
tmp_6 = (C3_5_tw) * a[1].im ;
tmp_7 = (tmp_5 + tmp_6) ;
tmp_8 = (C2_5_tw) * a[3].im ;
tmp_9 = (C3_5_tw) * a[4].im ;
tmp_10 = (tmp_8 + tmp_9) ;
tmp_49 = (tmp_4 +  decq((tmp_7 + ( widen<Q15>::run(a[0].re)   - tmp_10) ) ,1)  ) ;
tmp_11 = tmp_49;
tmp_12 = ( decq(a[1].im,1)   +  decq(a[4].im,1)  ) ;
tmp_13 = (C0_5_tw) * tmp_12 ;
tmp_14 = ( decq(a[2].im,1)   +  decq(a[3].im,1)  ) ;
tmp_15 = (C1_5_tw) * tmp_14 ;
tmp_16 = (tmp_13 - tmp_15) ;
tmp_17 = (C2_5_tw) * a[3].re ;
tmp_18 = (C3_5_tw) * a[4].re ;
tmp_19 = (tmp_17 + tmp_18) ;
tmp_20 = (C2_5_tw) * a[2].re ;
tmp_21 = (C3_5_tw) * a[1].re ;
tmp_22 = (tmp_20 + tmp_21) ;
tmp_50 = (tmp_16 +  decq((tmp_19 + ( widen<Q15>::run(a[0].im)   - tmp_22) ) ,1)  ) ;
tmp_23 = tmp_50;
res[1].re =  narrow<3,Q15>::run((w[0].re *  narrow<NORMALNARROW,Q15>::run(tmp_11)    - w[0].im *  narrow<NORMALNARROW,Q15>::run(tmp_23)   ) )  ;
tmp_24 = (C0_5_tw) * tmp_2 ;
tmp_25 = (C1_5_tw) * tmp_0 ;
tmp_26 = (tmp_24 - tmp_25) ;
tmp_27 = (C2_5_tw) * a[1].im ;
tmp_28 = (C3_5_tw) * a[3].im ;
tmp_29 = (tmp_27 + tmp_28) ;
tmp_30 = (C2_5_tw) * a[4].im ;
tmp_31 = (C3_5_tw) * a[2].im ;
tmp_32 = (tmp_30 + tmp_31) ;
tmp_51 = (tmp_26 +  decq((tmp_29 + ( widen<Q15>::run(a[0].re)   - tmp_32) ) ,1)  ) ;
tmp_33 = tmp_51;
tmp_34 = (C0_5_tw) * tmp_14 ;
tmp_35 = (C1_5_tw) * tmp_12 ;
tmp_36 = (tmp_34 - tmp_35) ;
tmp_37 = (C2_5_tw) * a[4].re ;
tmp_38 = (C3_5_tw) * a[2].re ;
tmp_39 = (tmp_37 + tmp_38) ;
tmp_40 = (C2_5_tw) * a[1].re ;
tmp_41 = (C3_5_tw) * a[3].re ;
tmp_42 = (tmp_40 + tmp_41) ;
tmp_52 = (tmp_36 +  decq((tmp_39 + ( widen<Q15>::run(a[0].im)   - tmp_42) ) ,1)  ) ;
tmp_43 = tmp_52;
res[2].re =  narrow<3,Q15>::run((w[1].re *  narrow<NORMALNARROW,Q15>::run(tmp_33)    - w[1].im *  narrow<NORMALNARROW,Q15>::run(tmp_43)   ) )  ;
tmp_53 = (tmp_26 +  decq((tmp_32 + ( widen<Q15>::run(a[0].re)   - tmp_29) ) ,1)  ) ;
tmp_44 = tmp_53;
tmp_54 = (tmp_36 +  decq((tmp_42 + ( widen<Q15>::run(a[0].im)   - tmp_39) ) ,1)  ) ;
tmp_45 = tmp_54;
res[3].re =  narrow<3,Q15>::run((w[2].re *  narrow<NORMALNARROW,Q15>::run(tmp_44)    - w[2].im *  narrow<NORMALNARROW,Q15>::run(tmp_45)   ) )  ;
tmp_55 = (tmp_4 +  decq((tmp_10 + ( widen<Q15>::run(a[0].re)   - tmp_7) ) ,1)  ) ;
tmp_46 = tmp_55;
tmp_56 = (tmp_16 +  decq((tmp_22 + ( widen<Q15>::run(a[0].im)   - tmp_19) ) ,1)  ) ;
tmp_47 = tmp_56;
res[4].re =  narrow<3,Q15>::run((w[3].re *  narrow<NORMALNARROW,Q15>::run(tmp_46)    - w[3].im *  narrow<NORMALNARROW,Q15>::run(tmp_47)   ) )  ;
tmp_57 = ( decq( decq(a[0].im,1)  ,3)   +  decq(( decq( decq(a[1].im,1)  ,2)   +  decq(( decq( decq(a[2].im,1)  ,1)   +  decq(( decq(a[3].im,1)   +  decq(a[4].im,1)  ) ,1)  ) ,1)  ) ,1)  ) ;
res[0].im = tmp_57;
res[1].im =  narrow<3,Q15>::run((w[0].re *  narrow<NORMALNARROW,Q15>::run(tmp_23)    + w[0].im *  narrow<NORMALNARROW,Q15>::run(tmp_11)   ) )  ;
res[2].im =  narrow<3,Q15>::run((w[1].re *  narrow<NORMALNARROW,Q15>::run(tmp_43)    + w[1].im *  narrow<NORMALNARROW,Q15>::run(tmp_33)   ) )  ;
res[3].im =  narrow<3,Q15>::run((w[2].re *  narrow<NORMALNARROW,Q15>::run(tmp_45)    + w[2].im *  narrow<NORMALNARROW,Q15>::run(tmp_44)   ) )  ;
res[4].im =  narrow<3,Q15>::run((w[3].re *  narrow<NORMALNARROW,Q15>::run(tmp_47)    + w[3].im *  narrow<NORMALNARROW,Q15>::run(tmp_46)   ) )  ;


memory<complex<Q15>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<Q15>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<Q15>>::store(out,2 * stageLength * strideFactor , res[2]);
memory<complex<Q15>>::store(out,3 * stageLength * strideFactor , res[3]);
memory<complex<Q15>>::store(out,4 * stageLength * strideFactor , res[4]);


};

/* Nb Multiplications = 24, 
Nb Additions = 48,
Nb Negates = 12,
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


 Q15 tmp_0;
 Q31 tmp_1;
 Q15 tmp_2;
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
 Q31 tmp_16;
 Q31 tmp_17;
 Q31 tmp_18;
 Q31 tmp_19;
 Q15 tmp_20;
 Q31 tmp_21;
 Q15 tmp_22;
 Q31 tmp_23;
 Q31 tmp_24;
 Q31 tmp_25;
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
 Q31 tmp_38;
 Q31 tmp_39;
 Q15 tmp_40;
 Q31 tmp_41;
 Q31 tmp_42;
 Q31 tmp_43;
 Q31 tmp_44;
 Q15 tmp_45;
 Q31 tmp_46;
 Q31 tmp_47;
 Q31 tmp_48;
 Q31 tmp_49;


a[0] = memory<complex<Q15>>::load(in,0 * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * strideFactor);
a[4] = memory<complex<Q15>>::load(in,4 * strideFactor);


tmp_40 = ( decq( decq(a[0].re,1)  ,3)   +  decq(( decq( decq(a[1].re,1)  ,2)   +  decq(( decq( decq(a[2].re,1)  ,1)   +  decq(( decq(a[3].re,1)   +  decq(a[4].re,1)  ) ,1)  ) ,1)  ) ,1)  ) ;
res[0].re = tmp_40;
tmp_0 = ( decq(a[1].re,1)   +  decq(a[4].re,1)  ) ;
tmp_1 = (C0_5_notw) * tmp_0 ;
tmp_2 = ( decq(a[2].re,1)   +  decq(a[3].re,1)  ) ;
tmp_3 = (C1_5_notw) * tmp_2 ;
tmp_4 = (tmp_1 - tmp_3) ;
tmp_5 = (C2_5_notw) * a[2].im ;
tmp_6 = (C3_5_notw) * a[1].im ;
tmp_7 = (tmp_5 + tmp_6) ;
tmp_8 = (C2_5_notw) * a[3].im ;
tmp_9 = (C3_5_notw) * a[4].im ;
tmp_10 = (tmp_8 + tmp_9) ;
tmp_41 = (tmp_4 +  decq((tmp_7 + ( widen<Q15>::run(a[0].re)   - tmp_10) ) ,1)  ) ;
res[1].re =  narrow<3,Q15>::run(tmp_41)  ;
tmp_11 = (C0_5_notw) * tmp_2 ;
tmp_12 = (C1_5_notw) * tmp_0 ;
tmp_13 = (tmp_11 - tmp_12) ;
tmp_14 = (C2_5_notw) * a[1].im ;
tmp_15 = (C3_5_notw) * a[3].im ;
tmp_16 = (tmp_14 + tmp_15) ;
tmp_17 = (C2_5_notw) * a[4].im ;
tmp_18 = (C3_5_notw) * a[2].im ;
tmp_19 = (tmp_17 + tmp_18) ;
tmp_42 = (tmp_13 +  decq((tmp_16 + ( widen<Q15>::run(a[0].re)   - tmp_19) ) ,1)  ) ;
res[2].re =  narrow<3,Q15>::run(tmp_42)  ;
tmp_43 = (tmp_13 +  decq((tmp_19 + ( widen<Q15>::run(a[0].re)   - tmp_16) ) ,1)  ) ;
res[3].re =  narrow<3,Q15>::run(tmp_43)  ;
tmp_44 = (tmp_4 +  decq((tmp_10 + ( widen<Q15>::run(a[0].re)   - tmp_7) ) ,1)  ) ;
res[4].re =  narrow<3,Q15>::run(tmp_44)  ;
tmp_45 = ( decq( decq(a[0].im,1)  ,3)   +  decq(( decq( decq(a[1].im,1)  ,2)   +  decq(( decq( decq(a[2].im,1)  ,1)   +  decq(( decq(a[3].im,1)   +  decq(a[4].im,1)  ) ,1)  ) ,1)  ) ,1)  ) ;
res[0].im = tmp_45;
tmp_20 = ( decq(a[1].im,1)   +  decq(a[4].im,1)  ) ;
tmp_21 = (C0_5_notw) * tmp_20 ;
tmp_22 = ( decq(a[2].im,1)   +  decq(a[3].im,1)  ) ;
tmp_23 = (C1_5_notw) * tmp_22 ;
tmp_24 = (tmp_21 - tmp_23) ;
tmp_25 = (C2_5_notw) * a[3].re ;
tmp_26 = (C3_5_notw) * a[4].re ;
tmp_27 = (tmp_25 + tmp_26) ;
tmp_28 = (C2_5_notw) * a[2].re ;
tmp_29 = (C3_5_notw) * a[1].re ;
tmp_30 = (tmp_28 + tmp_29) ;
tmp_46 = (tmp_24 +  decq((tmp_27 + ( widen<Q15>::run(a[0].im)   - tmp_30) ) ,1)  ) ;
res[1].im =  narrow<3,Q15>::run(tmp_46)  ;
tmp_31 = (C0_5_notw) * tmp_22 ;
tmp_32 = (C1_5_notw) * tmp_20 ;
tmp_33 = (tmp_31 - tmp_32) ;
tmp_34 = (C2_5_notw) * a[4].re ;
tmp_35 = (C3_5_notw) * a[2].re ;
tmp_36 = (tmp_34 + tmp_35) ;
tmp_37 = (C2_5_notw) * a[1].re ;
tmp_38 = (C3_5_notw) * a[3].re ;
tmp_39 = (tmp_37 + tmp_38) ;
tmp_47 = (tmp_33 +  decq((tmp_36 + ( widen<Q15>::run(a[0].im)   - tmp_39) ) ,1)  ) ;
res[2].im =  narrow<3,Q15>::run(tmp_47)  ;
tmp_48 = (tmp_33 +  decq((tmp_39 + ( widen<Q15>::run(a[0].im)   - tmp_36) ) ,1)  ) ;
res[3].im =  narrow<3,Q15>::run(tmp_48)  ;
tmp_49 = (tmp_24 +  decq((tmp_30 + ( widen<Q15>::run(a[0].im)   - tmp_27) ) ,1)  ) ;
res[4].im =  narrow<3,Q15>::run(tmp_49)  ;


memory<complex<Q15>>::store(in,0 * strideFactor , res[0]);
memory<complex<Q15>>::store(in,1 * strideFactor , res[1]);
memory<complex<Q15>>::store(in,2 * strideFactor , res[2]);
memory<complex<Q15>>::store(in,3 * strideFactor , res[3]);
memory<complex<Q15>>::store(in,4 * strideFactor , res[4]);


};

/* Nb Multiplications = 24, 
Nb Additions = 48,
Nb Negates = 12,
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


 Q15 tmp_0;
 Q31 tmp_1;
 Q15 tmp_2;
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
 Q31 tmp_16;
 Q31 tmp_17;
 Q31 tmp_18;
 Q31 tmp_19;
 Q15 tmp_20;
 Q31 tmp_21;
 Q15 tmp_22;
 Q31 tmp_23;
 Q31 tmp_24;
 Q31 tmp_25;
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
 Q31 tmp_38;
 Q31 tmp_39;
 Q15 tmp_40;
 Q31 tmp_41;
 Q31 tmp_42;
 Q31 tmp_43;
 Q31 tmp_44;
 Q15 tmp_45;
 Q31 tmp_46;
 Q31 tmp_47;
 Q31 tmp_48;
 Q31 tmp_49;


a[0] = memory<complex<Q15>>::load(in,0 * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * strideFactor);
a[4] = memory<complex<Q15>>::load(in,4 * strideFactor);


tmp_40 = ( decq( decq(a[0].re,1)  ,3)   +  decq(( decq( decq(a[1].re,1)  ,2)   +  decq(( decq( decq(a[2].re,1)  ,1)   +  decq(( decq(a[3].re,1)   +  decq(a[4].re,1)  ) ,1)  ) ,1)  ) ,1)  ) ;
res[0].re = tmp_40;
tmp_0 = ( decq(a[1].re,1)   +  decq(a[4].re,1)  ) ;
tmp_1 = (C0_5_notw) * tmp_0 ;
tmp_2 = ( decq(a[2].re,1)   +  decq(a[3].re,1)  ) ;
tmp_3 = (C1_5_notw) * tmp_2 ;
tmp_4 = (tmp_1 - tmp_3) ;
tmp_5 = (C2_5_notw) * a[2].im ;
tmp_6 = (C3_5_notw) * a[1].im ;
tmp_7 = (tmp_5 + tmp_6) ;
tmp_8 = (C2_5_notw) * a[3].im ;
tmp_9 = (C3_5_notw) * a[4].im ;
tmp_10 = (tmp_8 + tmp_9) ;
tmp_41 = (tmp_4 +  decq((tmp_7 + ( widen<Q15>::run(a[0].re)   - tmp_10) ) ,1)  ) ;
res[1].re =  narrow<3,Q15>::run(tmp_41)  ;
tmp_11 = (C0_5_notw) * tmp_2 ;
tmp_12 = (C1_5_notw) * tmp_0 ;
tmp_13 = (tmp_11 - tmp_12) ;
tmp_14 = (C2_5_notw) * a[1].im ;
tmp_15 = (C3_5_notw) * a[3].im ;
tmp_16 = (tmp_14 + tmp_15) ;
tmp_17 = (C2_5_notw) * a[4].im ;
tmp_18 = (C3_5_notw) * a[2].im ;
tmp_19 = (tmp_17 + tmp_18) ;
tmp_42 = (tmp_13 +  decq((tmp_16 + ( widen<Q15>::run(a[0].re)   - tmp_19) ) ,1)  ) ;
res[2].re =  narrow<3,Q15>::run(tmp_42)  ;
tmp_43 = (tmp_13 +  decq((tmp_19 + ( widen<Q15>::run(a[0].re)   - tmp_16) ) ,1)  ) ;
res[3].re =  narrow<3,Q15>::run(tmp_43)  ;
tmp_44 = (tmp_4 +  decq((tmp_10 + ( widen<Q15>::run(a[0].re)   - tmp_7) ) ,1)  ) ;
res[4].re =  narrow<3,Q15>::run(tmp_44)  ;
tmp_45 = ( decq( decq(a[0].im,1)  ,3)   +  decq(( decq( decq(a[1].im,1)  ,2)   +  decq(( decq( decq(a[2].im,1)  ,1)   +  decq(( decq(a[3].im,1)   +  decq(a[4].im,1)  ) ,1)  ) ,1)  ) ,1)  ) ;
res[0].im = tmp_45;
tmp_20 = ( decq(a[1].im,1)   +  decq(a[4].im,1)  ) ;
tmp_21 = (C0_5_notw) * tmp_20 ;
tmp_22 = ( decq(a[2].im,1)   +  decq(a[3].im,1)  ) ;
tmp_23 = (C1_5_notw) * tmp_22 ;
tmp_24 = (tmp_21 - tmp_23) ;
tmp_25 = (C2_5_notw) * a[3].re ;
tmp_26 = (C3_5_notw) * a[4].re ;
tmp_27 = (tmp_25 + tmp_26) ;
tmp_28 = (C2_5_notw) * a[2].re ;
tmp_29 = (C3_5_notw) * a[1].re ;
tmp_30 = (tmp_28 + tmp_29) ;
tmp_46 = (tmp_24 +  decq((tmp_27 + ( widen<Q15>::run(a[0].im)   - tmp_30) ) ,1)  ) ;
res[1].im =  narrow<3,Q15>::run(tmp_46)  ;
tmp_31 = (C0_5_notw) * tmp_22 ;
tmp_32 = (C1_5_notw) * tmp_20 ;
tmp_33 = (tmp_31 - tmp_32) ;
tmp_34 = (C2_5_notw) * a[4].re ;
tmp_35 = (C3_5_notw) * a[2].re ;
tmp_36 = (tmp_34 + tmp_35) ;
tmp_37 = (C2_5_notw) * a[1].re ;
tmp_38 = (C3_5_notw) * a[3].re ;
tmp_39 = (tmp_37 + tmp_38) ;
tmp_47 = (tmp_33 +  decq((tmp_36 + ( widen<Q15>::run(a[0].im)   - tmp_39) ) ,1)  ) ;
res[2].im =  narrow<3,Q15>::run(tmp_47)  ;
tmp_48 = (tmp_33 +  decq((tmp_39 + ( widen<Q15>::run(a[0].im)   - tmp_36) ) ,1)  ) ;
res[3].im =  narrow<3,Q15>::run(tmp_48)  ;
tmp_49 = (tmp_24 +  decq((tmp_30 + ( widen<Q15>::run(a[0].im)   - tmp_27) ) ,1)  ) ;
res[4].im =  narrow<3,Q15>::run(tmp_49)  ;


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

static constexpr Q15 C0_5_tw{(q15_t)0x278E};
  static constexpr Q15 C1_5_tw{(q15_t)0x678E};
  static constexpr Q15 C2_5_tw{(q15_t)0x4B3D};
  static constexpr Q15 C3_5_tw{(q15_t)0x79BC};
  
static constexpr Q15 C0_5_notw{(q15_t)0x278E};
  static constexpr Q15 C1_5_notw{(q15_t)0x678E};
  static constexpr Q15 C2_5_notw{(q15_t)0x4B3D};
  static constexpr Q15 C3_5_notw{(q15_t)0x79BC};
  

/* Nb Multiplications = 24, 
Nb Additions = 48,
Nb Negates = 12,
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


 Q15 tmp_0;
 Q31 tmp_1;
 Q15 tmp_2;
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
 Q31 tmp_16;
 Q31 tmp_17;
 Q31 tmp_18;
 Q31 tmp_19;
 Q15 tmp_20;
 Q31 tmp_21;
 Q15 tmp_22;
 Q31 tmp_23;
 Q31 tmp_24;
 Q31 tmp_25;
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
 Q31 tmp_38;
 Q31 tmp_39;
 Q15 tmp_40;
 Q31 tmp_41;
 Q31 tmp_42;
 Q31 tmp_43;
 Q31 tmp_44;
 Q15 tmp_45;
 Q31 tmp_46;
 Q31 tmp_47;
 Q31 tmp_48;
 Q31 tmp_49;


a[0] = memory<complex<Q15>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<Q15>>::load(in,4 * stageLength * strideFactor);


tmp_40 = ( decq( decq(a[0].re,1)  ,3)   +  decq(( decq( decq(a[1].re,1)  ,2)   +  decq(( decq( decq(a[2].re,1)  ,1)   +  decq(( decq(a[3].re,1)   +  decq(a[4].re,1)  ) ,1)  ) ,1)  ) ,1)  ) ;
res[0].re = tmp_40;
tmp_0 = ( decq(a[1].re,1)   +  decq(a[4].re,1)  ) ;
tmp_1 = (C0_5_notw) * tmp_0 ;
tmp_2 = ( decq(a[2].re,1)   +  decq(a[3].re,1)  ) ;
tmp_3 = (C1_5_notw) * tmp_2 ;
tmp_4 = (tmp_1 - tmp_3) ;
tmp_5 = (C2_5_notw) * a[3].im ;
tmp_6 = (C3_5_notw) * a[4].im ;
tmp_7 = (tmp_5 + tmp_6) ;
tmp_8 = (C2_5_notw) * a[2].im ;
tmp_9 = (C3_5_notw) * a[1].im ;
tmp_10 = (tmp_8 + tmp_9) ;
tmp_41 = (tmp_4 +  decq((tmp_7 + ( widen<Q15>::run(a[0].re)   - tmp_10) ) ,1)  ) ;
res[1].re =  narrow<3,Q15>::run(tmp_41)  ;
tmp_11 = (C0_5_notw) * tmp_2 ;
tmp_12 = (C1_5_notw) * tmp_0 ;
tmp_13 = (tmp_11 - tmp_12) ;
tmp_14 = (C2_5_notw) * a[4].im ;
tmp_15 = (C3_5_notw) * a[2].im ;
tmp_16 = (tmp_14 + tmp_15) ;
tmp_17 = (C2_5_notw) * a[1].im ;
tmp_18 = (C3_5_notw) * a[3].im ;
tmp_19 = (tmp_17 + tmp_18) ;
tmp_42 = (tmp_13 +  decq((tmp_16 + ( widen<Q15>::run(a[0].re)   - tmp_19) ) ,1)  ) ;
res[2].re =  narrow<3,Q15>::run(tmp_42)  ;
tmp_43 = (tmp_13 +  decq((tmp_19 + ( widen<Q15>::run(a[0].re)   - tmp_16) ) ,1)  ) ;
res[3].re =  narrow<3,Q15>::run(tmp_43)  ;
tmp_44 = (tmp_4 +  decq((tmp_10 + ( widen<Q15>::run(a[0].re)   - tmp_7) ) ,1)  ) ;
res[4].re =  narrow<3,Q15>::run(tmp_44)  ;
tmp_45 = ( decq( decq(a[0].im,1)  ,3)   +  decq(( decq( decq(a[1].im,1)  ,2)   +  decq(( decq( decq(a[2].im,1)  ,1)   +  decq(( decq(a[3].im,1)   +  decq(a[4].im,1)  ) ,1)  ) ,1)  ) ,1)  ) ;
res[0].im = tmp_45;
tmp_20 = ( decq(a[1].im,1)   +  decq(a[4].im,1)  ) ;
tmp_21 = (C0_5_notw) * tmp_20 ;
tmp_22 = ( decq(a[2].im,1)   +  decq(a[3].im,1)  ) ;
tmp_23 = (C1_5_notw) * tmp_22 ;
tmp_24 = (tmp_21 - tmp_23) ;
tmp_25 = (C2_5_notw) * a[2].re ;
tmp_26 = (C3_5_notw) * a[1].re ;
tmp_27 = (tmp_25 + tmp_26) ;
tmp_28 = (C2_5_notw) * a[3].re ;
tmp_29 = (C3_5_notw) * a[4].re ;
tmp_30 = (tmp_28 + tmp_29) ;
tmp_46 = (tmp_24 +  decq((tmp_27 + ( widen<Q15>::run(a[0].im)   - tmp_30) ) ,1)  ) ;
res[1].im =  narrow<3,Q15>::run(tmp_46)  ;
tmp_31 = (C0_5_notw) * tmp_22 ;
tmp_32 = (C1_5_notw) * tmp_20 ;
tmp_33 = (tmp_31 - tmp_32) ;
tmp_34 = (C2_5_notw) * a[1].re ;
tmp_35 = (C3_5_notw) * a[3].re ;
tmp_36 = (tmp_34 + tmp_35) ;
tmp_37 = (C2_5_notw) * a[4].re ;
tmp_38 = (C3_5_notw) * a[2].re ;
tmp_39 = (tmp_37 + tmp_38) ;
tmp_47 = (tmp_33 +  decq((tmp_36 + ( widen<Q15>::run(a[0].im)   - tmp_39) ) ,1)  ) ;
res[2].im =  narrow<3,Q15>::run(tmp_47)  ;
tmp_48 = (tmp_33 +  decq((tmp_39 + ( widen<Q15>::run(a[0].im)   - tmp_36) ) ,1)  ) ;
res[3].im =  narrow<3,Q15>::run(tmp_48)  ;
tmp_49 = (tmp_24 +  decq((tmp_30 + ( widen<Q15>::run(a[0].im)   - tmp_27) ) ,1)  ) ;
res[4].im =  narrow<3,Q15>::run(tmp_49)  ;


memory<complex<Q15>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<Q15>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<Q15>>::store(in,2 * stageLength * strideFactor , res[2]);
memory<complex<Q15>>::store(in,3 * stageLength * strideFactor , res[3]);
memory<complex<Q15>>::store(in,4 * stageLength * strideFactor , res[4]);


};

/* Nb Multiplications = 40, 
Nb Additions = 56,
Nb Negates = 16,
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


 Q15 tmp_0;
 Q31 tmp_1;
 Q15 tmp_2;
 Q31 tmp_3;
 Q31 tmp_4;
 Q31 tmp_5;
 Q31 tmp_6;
 Q31 tmp_7;
 Q31 tmp_8;
 Q31 tmp_9;
 Q31 tmp_10;
 Q31 tmp_11;
 Q15 tmp_12;
 Q31 tmp_13;
 Q15 tmp_14;
 Q31 tmp_15;
 Q31 tmp_16;
 Q31 tmp_17;
 Q31 tmp_18;
 Q31 tmp_19;
 Q31 tmp_20;
 Q31 tmp_21;
 Q31 tmp_22;
 Q31 tmp_23;
 Q31 tmp_24;
 Q31 tmp_25;
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
 Q31 tmp_38;
 Q31 tmp_39;
 Q31 tmp_40;
 Q31 tmp_41;
 Q31 tmp_42;
 Q31 tmp_43;
 Q31 tmp_44;
 Q31 tmp_45;
 Q31 tmp_46;
 Q31 tmp_47;
 Q15 tmp_48;
 Q31 tmp_49;
 Q31 tmp_50;
 Q31 tmp_51;
 Q31 tmp_52;
 Q31 tmp_53;
 Q31 tmp_54;
 Q31 tmp_55;
 Q31 tmp_56;
 Q15 tmp_57;


a[0] = memory<complex<Q15>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<Q15>>::load(in,4 * stageLength * strideFactor);


tmp_48 = ( decq( decq(a[0].re,1)  ,3)   +  decq(( decq( decq(a[1].re,1)  ,2)   +  decq(( decq( decq(a[2].re,1)  ,1)   +  decq(( decq(a[3].re,1)   +  decq(a[4].re,1)  ) ,1)  ) ,1)  ) ,1)  ) ;
res[0].re = tmp_48;
tmp_0 = ( decq(a[1].re,1)   +  decq(a[4].re,1)  ) ;
tmp_1 = (C0_5_tw) * tmp_0 ;
tmp_2 = ( decq(a[2].re,1)   +  decq(a[3].re,1)  ) ;
tmp_3 = (C1_5_tw) * tmp_2 ;
tmp_4 = (tmp_1 - tmp_3) ;
tmp_5 = (C2_5_tw) * a[3].im ;
tmp_6 = (C3_5_tw) * a[4].im ;
tmp_7 = (tmp_5 + tmp_6) ;
tmp_8 = (C2_5_tw) * a[2].im ;
tmp_9 = (C3_5_tw) * a[1].im ;
tmp_10 = (tmp_8 + tmp_9) ;
tmp_49 = (tmp_4 +  decq((tmp_7 + ( widen<Q15>::run(a[0].re)   - tmp_10) ) ,1)  ) ;
tmp_11 = tmp_49;
tmp_12 = ( decq(a[1].im,1)   +  decq(a[4].im,1)  ) ;
tmp_13 = (C0_5_tw) * tmp_12 ;
tmp_14 = ( decq(a[2].im,1)   +  decq(a[3].im,1)  ) ;
tmp_15 = (C1_5_tw) * tmp_14 ;
tmp_16 = (tmp_13 - tmp_15) ;
tmp_17 = (C2_5_tw) * a[2].re ;
tmp_18 = (C3_5_tw) * a[1].re ;
tmp_19 = (tmp_17 + tmp_18) ;
tmp_20 = (C2_5_tw) * a[3].re ;
tmp_21 = (C3_5_tw) * a[4].re ;
tmp_22 = (tmp_20 + tmp_21) ;
tmp_50 = (tmp_16 +  decq((tmp_19 + ( widen<Q15>::run(a[0].im)   - tmp_22) ) ,1)  ) ;
tmp_23 = tmp_50;
res[1].re =  narrow<3,Q15>::run((w[0].re *  narrow<NORMALNARROW,Q15>::run(tmp_11)    + w[0].im *  narrow<NORMALNARROW,Q15>::run(tmp_23)   ) )  ;
tmp_24 = (C0_5_tw) * tmp_2 ;
tmp_25 = (C1_5_tw) * tmp_0 ;
tmp_26 = (tmp_24 - tmp_25) ;
tmp_27 = (C2_5_tw) * a[4].im ;
tmp_28 = (C3_5_tw) * a[2].im ;
tmp_29 = (tmp_27 + tmp_28) ;
tmp_30 = (C2_5_tw) * a[1].im ;
tmp_31 = (C3_5_tw) * a[3].im ;
tmp_32 = (tmp_30 + tmp_31) ;
tmp_51 = (tmp_26 +  decq((tmp_29 + ( widen<Q15>::run(a[0].re)   - tmp_32) ) ,1)  ) ;
tmp_33 = tmp_51;
tmp_34 = (C0_5_tw) * tmp_14 ;
tmp_35 = (C1_5_tw) * tmp_12 ;
tmp_36 = (tmp_34 - tmp_35) ;
tmp_37 = (C2_5_tw) * a[1].re ;
tmp_38 = (C3_5_tw) * a[3].re ;
tmp_39 = (tmp_37 + tmp_38) ;
tmp_40 = (C2_5_tw) * a[4].re ;
tmp_41 = (C3_5_tw) * a[2].re ;
tmp_42 = (tmp_40 + tmp_41) ;
tmp_52 = (tmp_36 +  decq((tmp_39 + ( widen<Q15>::run(a[0].im)   - tmp_42) ) ,1)  ) ;
tmp_43 = tmp_52;
res[2].re =  narrow<3,Q15>::run((w[1].re *  narrow<NORMALNARROW,Q15>::run(tmp_33)    + w[1].im *  narrow<NORMALNARROW,Q15>::run(tmp_43)   ) )  ;
tmp_53 = (tmp_26 +  decq((tmp_32 + ( widen<Q15>::run(a[0].re)   - tmp_29) ) ,1)  ) ;
tmp_44 = tmp_53;
tmp_54 = (tmp_36 +  decq((tmp_42 + ( widen<Q15>::run(a[0].im)   - tmp_39) ) ,1)  ) ;
tmp_45 = tmp_54;
res[3].re =  narrow<3,Q15>::run((w[2].re *  narrow<NORMALNARROW,Q15>::run(tmp_44)    + w[2].im *  narrow<NORMALNARROW,Q15>::run(tmp_45)   ) )  ;
tmp_55 = (tmp_4 +  decq((tmp_10 + ( widen<Q15>::run(a[0].re)   - tmp_7) ) ,1)  ) ;
tmp_46 = tmp_55;
tmp_56 = (tmp_16 +  decq((tmp_22 + ( widen<Q15>::run(a[0].im)   - tmp_19) ) ,1)  ) ;
tmp_47 = tmp_56;
res[4].re =  narrow<3,Q15>::run((w[3].re *  narrow<NORMALNARROW,Q15>::run(tmp_46)    + w[3].im *  narrow<NORMALNARROW,Q15>::run(tmp_47)   ) )  ;
tmp_57 = ( decq( decq(a[0].im,1)  ,3)   +  decq(( decq( decq(a[1].im,1)  ,2)   +  decq(( decq( decq(a[2].im,1)  ,1)   +  decq(( decq(a[3].im,1)   +  decq(a[4].im,1)  ) ,1)  ) ,1)  ) ,1)  ) ;
res[0].im = tmp_57;
res[1].im =  narrow<3,Q15>::run((w[0].re *  narrow<NORMALNARROW,Q15>::run(tmp_23)    - w[0].im *  narrow<NORMALNARROW,Q15>::run(tmp_11)   ) )  ;
res[2].im =  narrow<3,Q15>::run((w[1].re *  narrow<NORMALNARROW,Q15>::run(tmp_43)    - w[1].im *  narrow<NORMALNARROW,Q15>::run(tmp_33)   ) )  ;
res[3].im =  narrow<3,Q15>::run((w[2].re *  narrow<NORMALNARROW,Q15>::run(tmp_45)    - w[2].im *  narrow<NORMALNARROW,Q15>::run(tmp_44)   ) )  ;
res[4].im =  narrow<3,Q15>::run((w[3].re *  narrow<NORMALNARROW,Q15>::run(tmp_47)    - w[3].im *  narrow<NORMALNARROW,Q15>::run(tmp_46)   ) )  ;


memory<complex<Q15>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<Q15>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<Q15>>::store(in,2 * stageLength * strideFactor , res[2]);
memory<complex<Q15>>::store(in,3 * stageLength * strideFactor , res[3]);
memory<complex<Q15>>::store(in,4 * stageLength * strideFactor , res[4]);


};

/* Nb Multiplications = 24, 
Nb Additions = 48,
Nb Negates = 12,
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


 Q15 tmp_0;
 Q31 tmp_1;
 Q15 tmp_2;
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
 Q31 tmp_16;
 Q31 tmp_17;
 Q31 tmp_18;
 Q31 tmp_19;
 Q15 tmp_20;
 Q31 tmp_21;
 Q15 tmp_22;
 Q31 tmp_23;
 Q31 tmp_24;
 Q31 tmp_25;
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
 Q31 tmp_38;
 Q31 tmp_39;
 Q15 tmp_40;
 Q31 tmp_41;
 Q31 tmp_42;
 Q31 tmp_43;
 Q31 tmp_44;
 Q15 tmp_45;
 Q31 tmp_46;
 Q31 tmp_47;
 Q31 tmp_48;
 Q31 tmp_49;


a[0] = memory<complex<Q15>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<Q15>>::load(in,4 * stageLength * strideFactor);


tmp_40 = ( decq( decq(a[0].re,1)  ,3)   +  decq(( decq( decq(a[1].re,1)  ,2)   +  decq(( decq( decq(a[2].re,1)  ,1)   +  decq(( decq(a[3].re,1)   +  decq(a[4].re,1)  ) ,1)  ) ,1)  ) ,1)  ) ;
res[0].re = tmp_40;
tmp_0 = ( decq(a[1].re,1)   +  decq(a[4].re,1)  ) ;
tmp_1 = (C0_5_notw) * tmp_0 ;
tmp_2 = ( decq(a[2].re,1)   +  decq(a[3].re,1)  ) ;
tmp_3 = (C1_5_notw) * tmp_2 ;
tmp_4 = (tmp_1 - tmp_3) ;
tmp_5 = (C2_5_notw) * a[3].im ;
tmp_6 = (C3_5_notw) * a[4].im ;
tmp_7 = (tmp_5 + tmp_6) ;
tmp_8 = (C2_5_notw) * a[2].im ;
tmp_9 = (C3_5_notw) * a[1].im ;
tmp_10 = (tmp_8 + tmp_9) ;
tmp_41 = (tmp_4 +  decq((tmp_7 + ( widen<Q15>::run(a[0].re)   - tmp_10) ) ,1)  ) ;
res[1].re =  narrow<3,Q15>::run(tmp_41)  ;
tmp_11 = (C0_5_notw) * tmp_2 ;
tmp_12 = (C1_5_notw) * tmp_0 ;
tmp_13 = (tmp_11 - tmp_12) ;
tmp_14 = (C2_5_notw) * a[4].im ;
tmp_15 = (C3_5_notw) * a[2].im ;
tmp_16 = (tmp_14 + tmp_15) ;
tmp_17 = (C2_5_notw) * a[1].im ;
tmp_18 = (C3_5_notw) * a[3].im ;
tmp_19 = (tmp_17 + tmp_18) ;
tmp_42 = (tmp_13 +  decq((tmp_16 + ( widen<Q15>::run(a[0].re)   - tmp_19) ) ,1)  ) ;
res[2].re =  narrow<3,Q15>::run(tmp_42)  ;
tmp_43 = (tmp_13 +  decq((tmp_19 + ( widen<Q15>::run(a[0].re)   - tmp_16) ) ,1)  ) ;
res[3].re =  narrow<3,Q15>::run(tmp_43)  ;
tmp_44 = (tmp_4 +  decq((tmp_10 + ( widen<Q15>::run(a[0].re)   - tmp_7) ) ,1)  ) ;
res[4].re =  narrow<3,Q15>::run(tmp_44)  ;
tmp_45 = ( decq( decq(a[0].im,1)  ,3)   +  decq(( decq( decq(a[1].im,1)  ,2)   +  decq(( decq( decq(a[2].im,1)  ,1)   +  decq(( decq(a[3].im,1)   +  decq(a[4].im,1)  ) ,1)  ) ,1)  ) ,1)  ) ;
res[0].im = tmp_45;
tmp_20 = ( decq(a[1].im,1)   +  decq(a[4].im,1)  ) ;
tmp_21 = (C0_5_notw) * tmp_20 ;
tmp_22 = ( decq(a[2].im,1)   +  decq(a[3].im,1)  ) ;
tmp_23 = (C1_5_notw) * tmp_22 ;
tmp_24 = (tmp_21 - tmp_23) ;
tmp_25 = (C2_5_notw) * a[2].re ;
tmp_26 = (C3_5_notw) * a[1].re ;
tmp_27 = (tmp_25 + tmp_26) ;
tmp_28 = (C2_5_notw) * a[3].re ;
tmp_29 = (C3_5_notw) * a[4].re ;
tmp_30 = (tmp_28 + tmp_29) ;
tmp_46 = (tmp_24 +  decq((tmp_27 + ( widen<Q15>::run(a[0].im)   - tmp_30) ) ,1)  ) ;
res[1].im =  narrow<3,Q15>::run(tmp_46)  ;
tmp_31 = (C0_5_notw) * tmp_22 ;
tmp_32 = (C1_5_notw) * tmp_20 ;
tmp_33 = (tmp_31 - tmp_32) ;
tmp_34 = (C2_5_notw) * a[1].re ;
tmp_35 = (C3_5_notw) * a[3].re ;
tmp_36 = (tmp_34 + tmp_35) ;
tmp_37 = (C2_5_notw) * a[4].re ;
tmp_38 = (C3_5_notw) * a[2].re ;
tmp_39 = (tmp_37 + tmp_38) ;
tmp_47 = (tmp_33 +  decq((tmp_36 + ( widen<Q15>::run(a[0].im)   - tmp_39) ) ,1)  ) ;
res[2].im =  narrow<3,Q15>::run(tmp_47)  ;
tmp_48 = (tmp_33 +  decq((tmp_39 + ( widen<Q15>::run(a[0].im)   - tmp_36) ) ,1)  ) ;
res[3].im =  narrow<3,Q15>::run(tmp_48)  ;
tmp_49 = (tmp_24 +  decq((tmp_30 + ( widen<Q15>::run(a[0].im)   - tmp_27) ) ,1)  ) ;
res[4].im =  narrow<3,Q15>::run(tmp_49)  ;


memory<complex<Q15>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<Q15>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<Q15>>::store(out,2 * stageLength * strideFactor , res[2]);
memory<complex<Q15>>::store(out,3 * stageLength * strideFactor , res[3]);
memory<complex<Q15>>::store(out,4 * stageLength * strideFactor , res[4]);


};

/* Nb Multiplications = 40, 
Nb Additions = 56,
Nb Negates = 16,
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


 Q15 tmp_0;
 Q31 tmp_1;
 Q15 tmp_2;
 Q31 tmp_3;
 Q31 tmp_4;
 Q31 tmp_5;
 Q31 tmp_6;
 Q31 tmp_7;
 Q31 tmp_8;
 Q31 tmp_9;
 Q31 tmp_10;
 Q31 tmp_11;
 Q15 tmp_12;
 Q31 tmp_13;
 Q15 tmp_14;
 Q31 tmp_15;
 Q31 tmp_16;
 Q31 tmp_17;
 Q31 tmp_18;
 Q31 tmp_19;
 Q31 tmp_20;
 Q31 tmp_21;
 Q31 tmp_22;
 Q31 tmp_23;
 Q31 tmp_24;
 Q31 tmp_25;
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
 Q31 tmp_38;
 Q31 tmp_39;
 Q31 tmp_40;
 Q31 tmp_41;
 Q31 tmp_42;
 Q31 tmp_43;
 Q31 tmp_44;
 Q31 tmp_45;
 Q31 tmp_46;
 Q31 tmp_47;
 Q15 tmp_48;
 Q31 tmp_49;
 Q31 tmp_50;
 Q31 tmp_51;
 Q31 tmp_52;
 Q31 tmp_53;
 Q31 tmp_54;
 Q31 tmp_55;
 Q31 tmp_56;
 Q15 tmp_57;


a[0] = memory<complex<Q15>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<Q15>>::load(in,4 * stageLength * strideFactor);


tmp_48 = ( decq( decq(a[0].re,1)  ,3)   +  decq(( decq( decq(a[1].re,1)  ,2)   +  decq(( decq( decq(a[2].re,1)  ,1)   +  decq(( decq(a[3].re,1)   +  decq(a[4].re,1)  ) ,1)  ) ,1)  ) ,1)  ) ;
res[0].re = tmp_48;
tmp_0 = ( decq(a[1].re,1)   +  decq(a[4].re,1)  ) ;
tmp_1 = (C0_5_tw) * tmp_0 ;
tmp_2 = ( decq(a[2].re,1)   +  decq(a[3].re,1)  ) ;
tmp_3 = (C1_5_tw) * tmp_2 ;
tmp_4 = (tmp_1 - tmp_3) ;
tmp_5 = (C2_5_tw) * a[3].im ;
tmp_6 = (C3_5_tw) * a[4].im ;
tmp_7 = (tmp_5 + tmp_6) ;
tmp_8 = (C2_5_tw) * a[2].im ;
tmp_9 = (C3_5_tw) * a[1].im ;
tmp_10 = (tmp_8 + tmp_9) ;
tmp_49 = (tmp_4 +  decq((tmp_7 + ( widen<Q15>::run(a[0].re)   - tmp_10) ) ,1)  ) ;
tmp_11 = tmp_49;
tmp_12 = ( decq(a[1].im,1)   +  decq(a[4].im,1)  ) ;
tmp_13 = (C0_5_tw) * tmp_12 ;
tmp_14 = ( decq(a[2].im,1)   +  decq(a[3].im,1)  ) ;
tmp_15 = (C1_5_tw) * tmp_14 ;
tmp_16 = (tmp_13 - tmp_15) ;
tmp_17 = (C2_5_tw) * a[2].re ;
tmp_18 = (C3_5_tw) * a[1].re ;
tmp_19 = (tmp_17 + tmp_18) ;
tmp_20 = (C2_5_tw) * a[3].re ;
tmp_21 = (C3_5_tw) * a[4].re ;
tmp_22 = (tmp_20 + tmp_21) ;
tmp_50 = (tmp_16 +  decq((tmp_19 + ( widen<Q15>::run(a[0].im)   - tmp_22) ) ,1)  ) ;
tmp_23 = tmp_50;
res[1].re =  narrow<3,Q15>::run((w[0].re *  narrow<NORMALNARROW,Q15>::run(tmp_11)    + w[0].im *  narrow<NORMALNARROW,Q15>::run(tmp_23)   ) )  ;
tmp_24 = (C0_5_tw) * tmp_2 ;
tmp_25 = (C1_5_tw) * tmp_0 ;
tmp_26 = (tmp_24 - tmp_25) ;
tmp_27 = (C2_5_tw) * a[4].im ;
tmp_28 = (C3_5_tw) * a[2].im ;
tmp_29 = (tmp_27 + tmp_28) ;
tmp_30 = (C2_5_tw) * a[1].im ;
tmp_31 = (C3_5_tw) * a[3].im ;
tmp_32 = (tmp_30 + tmp_31) ;
tmp_51 = (tmp_26 +  decq((tmp_29 + ( widen<Q15>::run(a[0].re)   - tmp_32) ) ,1)  ) ;
tmp_33 = tmp_51;
tmp_34 = (C0_5_tw) * tmp_14 ;
tmp_35 = (C1_5_tw) * tmp_12 ;
tmp_36 = (tmp_34 - tmp_35) ;
tmp_37 = (C2_5_tw) * a[1].re ;
tmp_38 = (C3_5_tw) * a[3].re ;
tmp_39 = (tmp_37 + tmp_38) ;
tmp_40 = (C2_5_tw) * a[4].re ;
tmp_41 = (C3_5_tw) * a[2].re ;
tmp_42 = (tmp_40 + tmp_41) ;
tmp_52 = (tmp_36 +  decq((tmp_39 + ( widen<Q15>::run(a[0].im)   - tmp_42) ) ,1)  ) ;
tmp_43 = tmp_52;
res[2].re =  narrow<3,Q15>::run((w[1].re *  narrow<NORMALNARROW,Q15>::run(tmp_33)    + w[1].im *  narrow<NORMALNARROW,Q15>::run(tmp_43)   ) )  ;
tmp_53 = (tmp_26 +  decq((tmp_32 + ( widen<Q15>::run(a[0].re)   - tmp_29) ) ,1)  ) ;
tmp_44 = tmp_53;
tmp_54 = (tmp_36 +  decq((tmp_42 + ( widen<Q15>::run(a[0].im)   - tmp_39) ) ,1)  ) ;
tmp_45 = tmp_54;
res[3].re =  narrow<3,Q15>::run((w[2].re *  narrow<NORMALNARROW,Q15>::run(tmp_44)    + w[2].im *  narrow<NORMALNARROW,Q15>::run(tmp_45)   ) )  ;
tmp_55 = (tmp_4 +  decq((tmp_10 + ( widen<Q15>::run(a[0].re)   - tmp_7) ) ,1)  ) ;
tmp_46 = tmp_55;
tmp_56 = (tmp_16 +  decq((tmp_22 + ( widen<Q15>::run(a[0].im)   - tmp_19) ) ,1)  ) ;
tmp_47 = tmp_56;
res[4].re =  narrow<3,Q15>::run((w[3].re *  narrow<NORMALNARROW,Q15>::run(tmp_46)    + w[3].im *  narrow<NORMALNARROW,Q15>::run(tmp_47)   ) )  ;
tmp_57 = ( decq( decq(a[0].im,1)  ,3)   +  decq(( decq( decq(a[1].im,1)  ,2)   +  decq(( decq( decq(a[2].im,1)  ,1)   +  decq(( decq(a[3].im,1)   +  decq(a[4].im,1)  ) ,1)  ) ,1)  ) ,1)  ) ;
res[0].im = tmp_57;
res[1].im =  narrow<3,Q15>::run((w[0].re *  narrow<NORMALNARROW,Q15>::run(tmp_23)    - w[0].im *  narrow<NORMALNARROW,Q15>::run(tmp_11)   ) )  ;
res[2].im =  narrow<3,Q15>::run((w[1].re *  narrow<NORMALNARROW,Q15>::run(tmp_43)    - w[1].im *  narrow<NORMALNARROW,Q15>::run(tmp_33)   ) )  ;
res[3].im =  narrow<3,Q15>::run((w[2].re *  narrow<NORMALNARROW,Q15>::run(tmp_45)    - w[2].im *  narrow<NORMALNARROW,Q15>::run(tmp_44)   ) )  ;
res[4].im =  narrow<3,Q15>::run((w[3].re *  narrow<NORMALNARROW,Q15>::run(tmp_47)    - w[3].im *  narrow<NORMALNARROW,Q15>::run(tmp_46)   ) )  ;


memory<complex<Q15>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<Q15>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<Q15>>::store(out,2 * stageLength * strideFactor , res[2]);
memory<complex<Q15>>::store(out,3 * stageLength * strideFactor , res[3]);
memory<complex<Q15>>::store(out,4 * stageLength * strideFactor , res[4]);


};

/* Nb Multiplications = 24, 
Nb Additions = 48,
Nb Negates = 12,
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


 Q15 tmp_0;
 Q31 tmp_1;
 Q15 tmp_2;
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
 Q31 tmp_16;
 Q31 tmp_17;
 Q31 tmp_18;
 Q31 tmp_19;
 Q15 tmp_20;
 Q31 tmp_21;
 Q15 tmp_22;
 Q31 tmp_23;
 Q31 tmp_24;
 Q31 tmp_25;
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
 Q31 tmp_38;
 Q31 tmp_39;
 Q15 tmp_40;
 Q31 tmp_41;
 Q31 tmp_42;
 Q31 tmp_43;
 Q31 tmp_44;
 Q15 tmp_45;
 Q31 tmp_46;
 Q31 tmp_47;
 Q31 tmp_48;
 Q31 tmp_49;


a[0] = memory<complex<Q15>>::load(in,0 * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * strideFactor);
a[4] = memory<complex<Q15>>::load(in,4 * strideFactor);


tmp_40 = ( decq( decq(a[0].re,1)  ,3)   +  decq(( decq( decq(a[1].re,1)  ,2)   +  decq(( decq( decq(a[2].re,1)  ,1)   +  decq(( decq(a[3].re,1)   +  decq(a[4].re,1)  ) ,1)  ) ,1)  ) ,1)  ) ;
res[0].re = tmp_40;
tmp_0 = ( decq(a[1].re,1)   +  decq(a[4].re,1)  ) ;
tmp_1 = (C0_5_notw) * tmp_0 ;
tmp_2 = ( decq(a[2].re,1)   +  decq(a[3].re,1)  ) ;
tmp_3 = (C1_5_notw) * tmp_2 ;
tmp_4 = (tmp_1 - tmp_3) ;
tmp_5 = (C2_5_notw) * a[3].im ;
tmp_6 = (C3_5_notw) * a[4].im ;
tmp_7 = (tmp_5 + tmp_6) ;
tmp_8 = (C2_5_notw) * a[2].im ;
tmp_9 = (C3_5_notw) * a[1].im ;
tmp_10 = (tmp_8 + tmp_9) ;
tmp_41 = (tmp_4 +  decq((tmp_7 + ( widen<Q15>::run(a[0].re)   - tmp_10) ) ,1)  ) ;
res[1].re =  narrow<3,Q15>::run(tmp_41)  ;
tmp_11 = (C0_5_notw) * tmp_2 ;
tmp_12 = (C1_5_notw) * tmp_0 ;
tmp_13 = (tmp_11 - tmp_12) ;
tmp_14 = (C2_5_notw) * a[4].im ;
tmp_15 = (C3_5_notw) * a[2].im ;
tmp_16 = (tmp_14 + tmp_15) ;
tmp_17 = (C2_5_notw) * a[1].im ;
tmp_18 = (C3_5_notw) * a[3].im ;
tmp_19 = (tmp_17 + tmp_18) ;
tmp_42 = (tmp_13 +  decq((tmp_16 + ( widen<Q15>::run(a[0].re)   - tmp_19) ) ,1)  ) ;
res[2].re =  narrow<3,Q15>::run(tmp_42)  ;
tmp_43 = (tmp_13 +  decq((tmp_19 + ( widen<Q15>::run(a[0].re)   - tmp_16) ) ,1)  ) ;
res[3].re =  narrow<3,Q15>::run(tmp_43)  ;
tmp_44 = (tmp_4 +  decq((tmp_10 + ( widen<Q15>::run(a[0].re)   - tmp_7) ) ,1)  ) ;
res[4].re =  narrow<3,Q15>::run(tmp_44)  ;
tmp_45 = ( decq( decq(a[0].im,1)  ,3)   +  decq(( decq( decq(a[1].im,1)  ,2)   +  decq(( decq( decq(a[2].im,1)  ,1)   +  decq(( decq(a[3].im,1)   +  decq(a[4].im,1)  ) ,1)  ) ,1)  ) ,1)  ) ;
res[0].im = tmp_45;
tmp_20 = ( decq(a[1].im,1)   +  decq(a[4].im,1)  ) ;
tmp_21 = (C0_5_notw) * tmp_20 ;
tmp_22 = ( decq(a[2].im,1)   +  decq(a[3].im,1)  ) ;
tmp_23 = (C1_5_notw) * tmp_22 ;
tmp_24 = (tmp_21 - tmp_23) ;
tmp_25 = (C2_5_notw) * a[2].re ;
tmp_26 = (C3_5_notw) * a[1].re ;
tmp_27 = (tmp_25 + tmp_26) ;
tmp_28 = (C2_5_notw) * a[3].re ;
tmp_29 = (C3_5_notw) * a[4].re ;
tmp_30 = (tmp_28 + tmp_29) ;
tmp_46 = (tmp_24 +  decq((tmp_27 + ( widen<Q15>::run(a[0].im)   - tmp_30) ) ,1)  ) ;
res[1].im =  narrow<3,Q15>::run(tmp_46)  ;
tmp_31 = (C0_5_notw) * tmp_22 ;
tmp_32 = (C1_5_notw) * tmp_20 ;
tmp_33 = (tmp_31 - tmp_32) ;
tmp_34 = (C2_5_notw) * a[1].re ;
tmp_35 = (C3_5_notw) * a[3].re ;
tmp_36 = (tmp_34 + tmp_35) ;
tmp_37 = (C2_5_notw) * a[4].re ;
tmp_38 = (C3_5_notw) * a[2].re ;
tmp_39 = (tmp_37 + tmp_38) ;
tmp_47 = (tmp_33 +  decq((tmp_36 + ( widen<Q15>::run(a[0].im)   - tmp_39) ) ,1)  ) ;
res[2].im =  narrow<3,Q15>::run(tmp_47)  ;
tmp_48 = (tmp_33 +  decq((tmp_39 + ( widen<Q15>::run(a[0].im)   - tmp_36) ) ,1)  ) ;
res[3].im =  narrow<3,Q15>::run(tmp_48)  ;
tmp_49 = (tmp_24 +  decq((tmp_30 + ( widen<Q15>::run(a[0].im)   - tmp_27) ) ,1)  ) ;
res[4].im =  narrow<3,Q15>::run(tmp_49)  ;


memory<complex<Q15>>::store(in,0 * strideFactor , res[0]);
memory<complex<Q15>>::store(in,1 * strideFactor , res[1]);
memory<complex<Q15>>::store(in,2 * strideFactor , res[2]);
memory<complex<Q15>>::store(in,3 * strideFactor , res[3]);
memory<complex<Q15>>::store(in,4 * strideFactor , res[4]);


};

/* Nb Multiplications = 24, 
Nb Additions = 48,
Nb Negates = 12,
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


 Q15 tmp_0;
 Q31 tmp_1;
 Q15 tmp_2;
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
 Q31 tmp_16;
 Q31 tmp_17;
 Q31 tmp_18;
 Q31 tmp_19;
 Q15 tmp_20;
 Q31 tmp_21;
 Q15 tmp_22;
 Q31 tmp_23;
 Q31 tmp_24;
 Q31 tmp_25;
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
 Q31 tmp_38;
 Q31 tmp_39;
 Q15 tmp_40;
 Q31 tmp_41;
 Q31 tmp_42;
 Q31 tmp_43;
 Q31 tmp_44;
 Q15 tmp_45;
 Q31 tmp_46;
 Q31 tmp_47;
 Q31 tmp_48;
 Q31 tmp_49;


a[0] = memory<complex<Q15>>::load(in,0 * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * strideFactor);
a[4] = memory<complex<Q15>>::load(in,4 * strideFactor);


tmp_40 = ( decq( decq(a[0].re,1)  ,3)   +  decq(( decq( decq(a[1].re,1)  ,2)   +  decq(( decq( decq(a[2].re,1)  ,1)   +  decq(( decq(a[3].re,1)   +  decq(a[4].re,1)  ) ,1)  ) ,1)  ) ,1)  ) ;
res[0].re = tmp_40;
tmp_0 = ( decq(a[1].re,1)   +  decq(a[4].re,1)  ) ;
tmp_1 = (C0_5_notw) * tmp_0 ;
tmp_2 = ( decq(a[2].re,1)   +  decq(a[3].re,1)  ) ;
tmp_3 = (C1_5_notw) * tmp_2 ;
tmp_4 = (tmp_1 - tmp_3) ;
tmp_5 = (C2_5_notw) * a[3].im ;
tmp_6 = (C3_5_notw) * a[4].im ;
tmp_7 = (tmp_5 + tmp_6) ;
tmp_8 = (C2_5_notw) * a[2].im ;
tmp_9 = (C3_5_notw) * a[1].im ;
tmp_10 = (tmp_8 + tmp_9) ;
tmp_41 = (tmp_4 +  decq((tmp_7 + ( widen<Q15>::run(a[0].re)   - tmp_10) ) ,1)  ) ;
res[1].re =  narrow<3,Q15>::run(tmp_41)  ;
tmp_11 = (C0_5_notw) * tmp_2 ;
tmp_12 = (C1_5_notw) * tmp_0 ;
tmp_13 = (tmp_11 - tmp_12) ;
tmp_14 = (C2_5_notw) * a[4].im ;
tmp_15 = (C3_5_notw) * a[2].im ;
tmp_16 = (tmp_14 + tmp_15) ;
tmp_17 = (C2_5_notw) * a[1].im ;
tmp_18 = (C3_5_notw) * a[3].im ;
tmp_19 = (tmp_17 + tmp_18) ;
tmp_42 = (tmp_13 +  decq((tmp_16 + ( widen<Q15>::run(a[0].re)   - tmp_19) ) ,1)  ) ;
res[2].re =  narrow<3,Q15>::run(tmp_42)  ;
tmp_43 = (tmp_13 +  decq((tmp_19 + ( widen<Q15>::run(a[0].re)   - tmp_16) ) ,1)  ) ;
res[3].re =  narrow<3,Q15>::run(tmp_43)  ;
tmp_44 = (tmp_4 +  decq((tmp_10 + ( widen<Q15>::run(a[0].re)   - tmp_7) ) ,1)  ) ;
res[4].re =  narrow<3,Q15>::run(tmp_44)  ;
tmp_45 = ( decq( decq(a[0].im,1)  ,3)   +  decq(( decq( decq(a[1].im,1)  ,2)   +  decq(( decq( decq(a[2].im,1)  ,1)   +  decq(( decq(a[3].im,1)   +  decq(a[4].im,1)  ) ,1)  ) ,1)  ) ,1)  ) ;
res[0].im = tmp_45;
tmp_20 = ( decq(a[1].im,1)   +  decq(a[4].im,1)  ) ;
tmp_21 = (C0_5_notw) * tmp_20 ;
tmp_22 = ( decq(a[2].im,1)   +  decq(a[3].im,1)  ) ;
tmp_23 = (C1_5_notw) * tmp_22 ;
tmp_24 = (tmp_21 - tmp_23) ;
tmp_25 = (C2_5_notw) * a[2].re ;
tmp_26 = (C3_5_notw) * a[1].re ;
tmp_27 = (tmp_25 + tmp_26) ;
tmp_28 = (C2_5_notw) * a[3].re ;
tmp_29 = (C3_5_notw) * a[4].re ;
tmp_30 = (tmp_28 + tmp_29) ;
tmp_46 = (tmp_24 +  decq((tmp_27 + ( widen<Q15>::run(a[0].im)   - tmp_30) ) ,1)  ) ;
res[1].im =  narrow<3,Q15>::run(tmp_46)  ;
tmp_31 = (C0_5_notw) * tmp_22 ;
tmp_32 = (C1_5_notw) * tmp_20 ;
tmp_33 = (tmp_31 - tmp_32) ;
tmp_34 = (C2_5_notw) * a[1].re ;
tmp_35 = (C3_5_notw) * a[3].re ;
tmp_36 = (tmp_34 + tmp_35) ;
tmp_37 = (C2_5_notw) * a[4].re ;
tmp_38 = (C3_5_notw) * a[2].re ;
tmp_39 = (tmp_37 + tmp_38) ;
tmp_47 = (tmp_33 +  decq((tmp_36 + ( widen<Q15>::run(a[0].im)   - tmp_39) ) ,1)  ) ;
res[2].im =  narrow<3,Q15>::run(tmp_47)  ;
tmp_48 = (tmp_33 +  decq((tmp_39 + ( widen<Q15>::run(a[0].im)   - tmp_36) ) ,1)  ) ;
res[3].im =  narrow<3,Q15>::run(tmp_48)  ;
tmp_49 = (tmp_24 +  decq((tmp_30 + ( widen<Q15>::run(a[0].im)   - tmp_27) ) ,1)  ) ;
res[4].im =  narrow<3,Q15>::run(tmp_49)  ;


memory<complex<Q15>>::store(out,0 * strideFactor , res[0]);
memory<complex<Q15>>::store(out,1 * strideFactor , res[1]);
memory<complex<Q15>>::store(out,2 * strideFactor , res[2]);
memory<complex<Q15>>::store(out,3 * strideFactor , res[3]);
memory<complex<Q15>>::store(out,4 * strideFactor , res[4]);


};

};

#endif /* #ifndef _RADIX_Q15_5_H */