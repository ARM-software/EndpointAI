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

#ifndef _RADIX_F16_5_H
#define _RADIX_F16_5_H

#include "complex.h"

/**************

DIRECT FFT 

***************/
template<int inputstrideenabled> 
struct radix<DIRECT,float16_t, 5,inputstrideenabled>{

static constexpr float16_t C0_5_tw{0.30901699437494745f16};
  static constexpr float16_t C1_5_tw{0.8090169943749475f16};
  static constexpr float16_t C2_5_tw{0.5877852522924731f16};
  static constexpr float16_t C3_5_tw{0.9510565162951535f16};
  
static constexpr float16_t C0_5_notw{0.30901699437494745f16};
  static constexpr float16_t C1_5_notw{0.8090169943749475f16};
  static constexpr float16_t C2_5_notw{0.5877852522924731f16};
  static constexpr float16_t C3_5_notw{0.9510565162951535f16};
  

/* Nb Multiplications = 24, 
Nb Additions = 48,
Nb Negates = 12,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_notw(int stageLength,complex<float16_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[5];
complex<float16_t> res[5];


 float16_t tmp_0;
 float16_t tmp_1;
 float16_t tmp_2;
 float16_t tmp_3;
 float16_t tmp_4;
 float16_t tmp_5;
 float16_t tmp_6;
 float16_t tmp_7;
 float16_t tmp_8;
 float16_t tmp_9;
 float16_t tmp_10;
 float16_t tmp_11;
 float16_t tmp_12;
 float16_t tmp_13;
 float16_t tmp_14;
 float16_t tmp_15;
 float16_t tmp_16;
 float16_t tmp_17;
 float16_t tmp_18;
 float16_t tmp_19;
 float16_t tmp_20;
 float16_t tmp_21;
 float16_t tmp_22;
 float16_t tmp_23;
 float16_t tmp_24;
 float16_t tmp_25;
 float16_t tmp_26;
 float16_t tmp_27;
 float16_t tmp_28;
 float16_t tmp_29;
 float16_t tmp_30;
 float16_t tmp_31;
 float16_t tmp_32;
 float16_t tmp_33;
 float16_t tmp_34;
 float16_t tmp_35;
 float16_t tmp_36;
 float16_t tmp_37;
 float16_t tmp_38;
 float16_t tmp_39;


a[0] = memory<complex<float16_t>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<float16_t>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<float16_t>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<float16_t>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<float16_t>>::load(in,4 * stageLength * strideFactor);


res[0].re = (a[0].re + a[1].re + a[2].re + a[3].re + a[4].re);
tmp_0 = (a[1].re + a[4].re);
tmp_1 =  (C0_5_notw) * tmp_0 ;
tmp_2 = (a[2].re + a[3].re);
tmp_3 =  (C1_5_notw) * tmp_2 ;
tmp_4 = (tmp_1 - tmp_3);
tmp_5 =  (C2_5_notw) * a[2].im ;
tmp_6 =  (C3_5_notw) * a[1].im ;
tmp_7 = (tmp_5 + tmp_6);
tmp_8 =  (C2_5_notw) * a[3].im ;
tmp_9 =  (C3_5_notw) * a[4].im ;
tmp_10 = (tmp_8 + tmp_9);
res[1].re = (tmp_4 + tmp_7 + a[0].re - tmp_10);
tmp_11 =  (C0_5_notw) * tmp_2 ;
tmp_12 =  (C1_5_notw) * tmp_0 ;
tmp_13 = (tmp_11 - tmp_12);
tmp_14 =  (C2_5_notw) * a[1].im ;
tmp_15 =  (C3_5_notw) * a[3].im ;
tmp_16 = (tmp_14 + tmp_15);
tmp_17 =  (C2_5_notw) * a[4].im ;
tmp_18 =  (C3_5_notw) * a[2].im ;
tmp_19 = (tmp_17 + tmp_18);
res[2].re = (tmp_13 + tmp_16 + a[0].re - tmp_19);
res[3].re = (tmp_13 + tmp_19 + a[0].re - tmp_16);
res[4].re = (tmp_4 + tmp_10 + a[0].re - tmp_7);
res[0].im = (a[0].im + a[1].im + a[2].im + a[3].im + a[4].im);
tmp_20 = (a[1].im + a[4].im);
tmp_21 =  (C0_5_notw) * tmp_20 ;
tmp_22 = (a[2].im + a[3].im);
tmp_23 =  (C1_5_notw) * tmp_22 ;
tmp_24 = (tmp_21 - tmp_23);
tmp_25 =  (C2_5_notw) * a[3].re ;
tmp_26 =  (C3_5_notw) * a[4].re ;
tmp_27 = (tmp_25 + tmp_26);
tmp_28 =  (C2_5_notw) * a[2].re ;
tmp_29 =  (C3_5_notw) * a[1].re ;
tmp_30 = (tmp_28 + tmp_29);
res[1].im = (tmp_24 + tmp_27 + a[0].im - tmp_30);
tmp_31 =  (C0_5_notw) * tmp_22 ;
tmp_32 =  (C1_5_notw) * tmp_20 ;
tmp_33 = (tmp_31 - tmp_32);
tmp_34 =  (C2_5_notw) * a[4].re ;
tmp_35 =  (C3_5_notw) * a[2].re ;
tmp_36 = (tmp_34 + tmp_35);
tmp_37 =  (C2_5_notw) * a[1].re ;
tmp_38 =  (C3_5_notw) * a[3].re ;
tmp_39 = (tmp_37 + tmp_38);
res[2].im = (tmp_33 + tmp_36 + a[0].im - tmp_39);
res[3].im = (tmp_33 + tmp_39 + a[0].im - tmp_36);
res[4].im = (tmp_24 + tmp_30 + a[0].im - tmp_27);


memory<complex<float16_t>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<float16_t>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<float16_t>>::store(in,2 * stageLength * strideFactor , res[2]);
memory<complex<float16_t>>::store(in,3 * stageLength * strideFactor , res[3]);
memory<complex<float16_t>>::store(in,4 * stageLength * strideFactor , res[4]);


};

/* Nb Multiplications = 40, 
Nb Additions = 56,
Nb Negates = 16,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_tw(int stageLength,const complex<float16_t> *w,complex<float16_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[5];
complex<float16_t> res[5];


 float16_t tmp_0;
 float16_t tmp_1;
 float16_t tmp_2;
 float16_t tmp_3;
 float16_t tmp_4;
 float16_t tmp_5;
 float16_t tmp_6;
 float16_t tmp_7;
 float16_t tmp_8;
 float16_t tmp_9;
 float16_t tmp_10;
 float16_t tmp_11;
 float16_t tmp_12;
 float16_t tmp_13;
 float16_t tmp_14;
 float16_t tmp_15;
 float16_t tmp_16;
 float16_t tmp_17;
 float16_t tmp_18;
 float16_t tmp_19;
 float16_t tmp_20;
 float16_t tmp_21;
 float16_t tmp_22;
 float16_t tmp_23;
 float16_t tmp_24;
 float16_t tmp_25;
 float16_t tmp_26;
 float16_t tmp_27;
 float16_t tmp_28;
 float16_t tmp_29;
 float16_t tmp_30;
 float16_t tmp_31;
 float16_t tmp_32;
 float16_t tmp_33;
 float16_t tmp_34;
 float16_t tmp_35;
 float16_t tmp_36;
 float16_t tmp_37;
 float16_t tmp_38;
 float16_t tmp_39;
 float16_t tmp_40;
 float16_t tmp_41;
 float16_t tmp_42;
 float16_t tmp_43;
 float16_t tmp_44;
 float16_t tmp_45;
 float16_t tmp_46;
 float16_t tmp_47;


a[0] = memory<complex<float16_t>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<float16_t>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<float16_t>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<float16_t>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<float16_t>>::load(in,4 * stageLength * strideFactor);


res[0].re = (a[0].re + a[1].re + a[2].re + a[3].re + a[4].re);
tmp_0 = (a[1].re + a[4].re);
tmp_1 =  (C0_5_tw) * tmp_0 ;
tmp_2 = (a[2].re + a[3].re);
tmp_3 =  (C1_5_tw) * tmp_2 ;
tmp_4 = (tmp_1 - tmp_3);
tmp_5 =  (C2_5_tw) * a[2].im ;
tmp_6 =  (C3_5_tw) * a[1].im ;
tmp_7 = (tmp_5 + tmp_6);
tmp_8 =  (C2_5_tw) * a[3].im ;
tmp_9 =  (C3_5_tw) * a[4].im ;
tmp_10 = (tmp_8 + tmp_9);
tmp_11 = (tmp_4 + tmp_7 + a[0].re - tmp_10);
tmp_12 = (a[1].im + a[4].im);
tmp_13 =  (C0_5_tw) * tmp_12 ;
tmp_14 = (a[2].im + a[3].im);
tmp_15 =  (C1_5_tw) * tmp_14 ;
tmp_16 = (tmp_13 - tmp_15);
tmp_17 =  (C2_5_tw) * a[3].re ;
tmp_18 =  (C3_5_tw) * a[4].re ;
tmp_19 = (tmp_17 + tmp_18);
tmp_20 =  (C2_5_tw) * a[2].re ;
tmp_21 =  (C3_5_tw) * a[1].re ;
tmp_22 = (tmp_20 + tmp_21);
tmp_23 = (tmp_16 + tmp_19 + a[0].im - tmp_22);
res[1].re = ( w[0].re * tmp_11  -  w[0].im * tmp_23 );
tmp_24 =  (C0_5_tw) * tmp_2 ;
tmp_25 =  (C1_5_tw) * tmp_0 ;
tmp_26 = (tmp_24 - tmp_25);
tmp_27 =  (C2_5_tw) * a[1].im ;
tmp_28 =  (C3_5_tw) * a[3].im ;
tmp_29 = (tmp_27 + tmp_28);
tmp_30 =  (C2_5_tw) * a[4].im ;
tmp_31 =  (C3_5_tw) * a[2].im ;
tmp_32 = (tmp_30 + tmp_31);
tmp_33 = (tmp_26 + tmp_29 + a[0].re - tmp_32);
tmp_34 =  (C0_5_tw) * tmp_14 ;
tmp_35 =  (C1_5_tw) * tmp_12 ;
tmp_36 = (tmp_34 - tmp_35);
tmp_37 =  (C2_5_tw) * a[4].re ;
tmp_38 =  (C3_5_tw) * a[2].re ;
tmp_39 = (tmp_37 + tmp_38);
tmp_40 =  (C2_5_tw) * a[1].re ;
tmp_41 =  (C3_5_tw) * a[3].re ;
tmp_42 = (tmp_40 + tmp_41);
tmp_43 = (tmp_36 + tmp_39 + a[0].im - tmp_42);
res[2].re = ( w[1].re * tmp_33  -  w[1].im * tmp_43 );
tmp_44 = (tmp_26 + tmp_32 + a[0].re - tmp_29);
tmp_45 = (tmp_36 + tmp_42 + a[0].im - tmp_39);
res[3].re = ( w[2].re * tmp_44  -  w[2].im * tmp_45 );
tmp_46 = (tmp_4 + tmp_10 + a[0].re - tmp_7);
tmp_47 = (tmp_16 + tmp_22 + a[0].im - tmp_19);
res[4].re = ( w[3].re * tmp_46  -  w[3].im * tmp_47 );
res[0].im = (a[0].im + a[1].im + a[2].im + a[3].im + a[4].im);
res[1].im = ( w[0].re * tmp_23  +  w[0].im * tmp_11 );
res[2].im = ( w[1].re * tmp_43  +  w[1].im * tmp_33 );
res[3].im = ( w[2].re * tmp_45  +  w[2].im * tmp_44 );
res[4].im = ( w[3].re * tmp_47  +  w[3].im * tmp_46 );


memory<complex<float16_t>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<float16_t>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<float16_t>>::store(in,2 * stageLength * strideFactor , res[2]);
memory<complex<float16_t>>::store(in,3 * stageLength * strideFactor , res[3]);
memory<complex<float16_t>>::store(in,4 * stageLength * strideFactor , res[4]);


};

/* Nb Multiplications = 24, 
Nb Additions = 48,
Nb Negates = 12,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_notw(int stageLength,const complex<float16_t> *in,complex<float16_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[5];
complex<float16_t> res[5];


 float16_t tmp_0;
 float16_t tmp_1;
 float16_t tmp_2;
 float16_t tmp_3;
 float16_t tmp_4;
 float16_t tmp_5;
 float16_t tmp_6;
 float16_t tmp_7;
 float16_t tmp_8;
 float16_t tmp_9;
 float16_t tmp_10;
 float16_t tmp_11;
 float16_t tmp_12;
 float16_t tmp_13;
 float16_t tmp_14;
 float16_t tmp_15;
 float16_t tmp_16;
 float16_t tmp_17;
 float16_t tmp_18;
 float16_t tmp_19;
 float16_t tmp_20;
 float16_t tmp_21;
 float16_t tmp_22;
 float16_t tmp_23;
 float16_t tmp_24;
 float16_t tmp_25;
 float16_t tmp_26;
 float16_t tmp_27;
 float16_t tmp_28;
 float16_t tmp_29;
 float16_t tmp_30;
 float16_t tmp_31;
 float16_t tmp_32;
 float16_t tmp_33;
 float16_t tmp_34;
 float16_t tmp_35;
 float16_t tmp_36;
 float16_t tmp_37;
 float16_t tmp_38;
 float16_t tmp_39;


a[0] = memory<complex<float16_t>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<float16_t>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<float16_t>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<float16_t>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<float16_t>>::load(in,4 * stageLength * strideFactor);


res[0].re = (a[0].re + a[1].re + a[2].re + a[3].re + a[4].re);
tmp_0 = (a[1].re + a[4].re);
tmp_1 =  (C0_5_notw) * tmp_0 ;
tmp_2 = (a[2].re + a[3].re);
tmp_3 =  (C1_5_notw) * tmp_2 ;
tmp_4 = (tmp_1 - tmp_3);
tmp_5 =  (C2_5_notw) * a[2].im ;
tmp_6 =  (C3_5_notw) * a[1].im ;
tmp_7 = (tmp_5 + tmp_6);
tmp_8 =  (C2_5_notw) * a[3].im ;
tmp_9 =  (C3_5_notw) * a[4].im ;
tmp_10 = (tmp_8 + tmp_9);
res[1].re = (tmp_4 + tmp_7 + a[0].re - tmp_10);
tmp_11 =  (C0_5_notw) * tmp_2 ;
tmp_12 =  (C1_5_notw) * tmp_0 ;
tmp_13 = (tmp_11 - tmp_12);
tmp_14 =  (C2_5_notw) * a[1].im ;
tmp_15 =  (C3_5_notw) * a[3].im ;
tmp_16 = (tmp_14 + tmp_15);
tmp_17 =  (C2_5_notw) * a[4].im ;
tmp_18 =  (C3_5_notw) * a[2].im ;
tmp_19 = (tmp_17 + tmp_18);
res[2].re = (tmp_13 + tmp_16 + a[0].re - tmp_19);
res[3].re = (tmp_13 + tmp_19 + a[0].re - tmp_16);
res[4].re = (tmp_4 + tmp_10 + a[0].re - tmp_7);
res[0].im = (a[0].im + a[1].im + a[2].im + a[3].im + a[4].im);
tmp_20 = (a[1].im + a[4].im);
tmp_21 =  (C0_5_notw) * tmp_20 ;
tmp_22 = (a[2].im + a[3].im);
tmp_23 =  (C1_5_notw) * tmp_22 ;
tmp_24 = (tmp_21 - tmp_23);
tmp_25 =  (C2_5_notw) * a[3].re ;
tmp_26 =  (C3_5_notw) * a[4].re ;
tmp_27 = (tmp_25 + tmp_26);
tmp_28 =  (C2_5_notw) * a[2].re ;
tmp_29 =  (C3_5_notw) * a[1].re ;
tmp_30 = (tmp_28 + tmp_29);
res[1].im = (tmp_24 + tmp_27 + a[0].im - tmp_30);
tmp_31 =  (C0_5_notw) * tmp_22 ;
tmp_32 =  (C1_5_notw) * tmp_20 ;
tmp_33 = (tmp_31 - tmp_32);
tmp_34 =  (C2_5_notw) * a[4].re ;
tmp_35 =  (C3_5_notw) * a[2].re ;
tmp_36 = (tmp_34 + tmp_35);
tmp_37 =  (C2_5_notw) * a[1].re ;
tmp_38 =  (C3_5_notw) * a[3].re ;
tmp_39 = (tmp_37 + tmp_38);
res[2].im = (tmp_33 + tmp_36 + a[0].im - tmp_39);
res[3].im = (tmp_33 + tmp_39 + a[0].im - tmp_36);
res[4].im = (tmp_24 + tmp_30 + a[0].im - tmp_27);


memory<complex<float16_t>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<float16_t>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<float16_t>>::store(out,2 * stageLength * strideFactor , res[2]);
memory<complex<float16_t>>::store(out,3 * stageLength * strideFactor , res[3]);
memory<complex<float16_t>>::store(out,4 * stageLength * strideFactor , res[4]);


};

/* Nb Multiplications = 40, 
Nb Additions = 56,
Nb Negates = 16,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_tw(int stageLength,const complex<float16_t> *w,const complex<float16_t> *in,complex<float16_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[5];
complex<float16_t> res[5];


 float16_t tmp_0;
 float16_t tmp_1;
 float16_t tmp_2;
 float16_t tmp_3;
 float16_t tmp_4;
 float16_t tmp_5;
 float16_t tmp_6;
 float16_t tmp_7;
 float16_t tmp_8;
 float16_t tmp_9;
 float16_t tmp_10;
 float16_t tmp_11;
 float16_t tmp_12;
 float16_t tmp_13;
 float16_t tmp_14;
 float16_t tmp_15;
 float16_t tmp_16;
 float16_t tmp_17;
 float16_t tmp_18;
 float16_t tmp_19;
 float16_t tmp_20;
 float16_t tmp_21;
 float16_t tmp_22;
 float16_t tmp_23;
 float16_t tmp_24;
 float16_t tmp_25;
 float16_t tmp_26;
 float16_t tmp_27;
 float16_t tmp_28;
 float16_t tmp_29;
 float16_t tmp_30;
 float16_t tmp_31;
 float16_t tmp_32;
 float16_t tmp_33;
 float16_t tmp_34;
 float16_t tmp_35;
 float16_t tmp_36;
 float16_t tmp_37;
 float16_t tmp_38;
 float16_t tmp_39;
 float16_t tmp_40;
 float16_t tmp_41;
 float16_t tmp_42;
 float16_t tmp_43;
 float16_t tmp_44;
 float16_t tmp_45;
 float16_t tmp_46;
 float16_t tmp_47;


a[0] = memory<complex<float16_t>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<float16_t>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<float16_t>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<float16_t>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<float16_t>>::load(in,4 * stageLength * strideFactor);


res[0].re = (a[0].re + a[1].re + a[2].re + a[3].re + a[4].re);
tmp_0 = (a[1].re + a[4].re);
tmp_1 =  (C0_5_tw) * tmp_0 ;
tmp_2 = (a[2].re + a[3].re);
tmp_3 =  (C1_5_tw) * tmp_2 ;
tmp_4 = (tmp_1 - tmp_3);
tmp_5 =  (C2_5_tw) * a[2].im ;
tmp_6 =  (C3_5_tw) * a[1].im ;
tmp_7 = (tmp_5 + tmp_6);
tmp_8 =  (C2_5_tw) * a[3].im ;
tmp_9 =  (C3_5_tw) * a[4].im ;
tmp_10 = (tmp_8 + tmp_9);
tmp_11 = (tmp_4 + tmp_7 + a[0].re - tmp_10);
tmp_12 = (a[1].im + a[4].im);
tmp_13 =  (C0_5_tw) * tmp_12 ;
tmp_14 = (a[2].im + a[3].im);
tmp_15 =  (C1_5_tw) * tmp_14 ;
tmp_16 = (tmp_13 - tmp_15);
tmp_17 =  (C2_5_tw) * a[3].re ;
tmp_18 =  (C3_5_tw) * a[4].re ;
tmp_19 = (tmp_17 + tmp_18);
tmp_20 =  (C2_5_tw) * a[2].re ;
tmp_21 =  (C3_5_tw) * a[1].re ;
tmp_22 = (tmp_20 + tmp_21);
tmp_23 = (tmp_16 + tmp_19 + a[0].im - tmp_22);
res[1].re = ( w[0].re * tmp_11  -  w[0].im * tmp_23 );
tmp_24 =  (C0_5_tw) * tmp_2 ;
tmp_25 =  (C1_5_tw) * tmp_0 ;
tmp_26 = (tmp_24 - tmp_25);
tmp_27 =  (C2_5_tw) * a[1].im ;
tmp_28 =  (C3_5_tw) * a[3].im ;
tmp_29 = (tmp_27 + tmp_28);
tmp_30 =  (C2_5_tw) * a[4].im ;
tmp_31 =  (C3_5_tw) * a[2].im ;
tmp_32 = (tmp_30 + tmp_31);
tmp_33 = (tmp_26 + tmp_29 + a[0].re - tmp_32);
tmp_34 =  (C0_5_tw) * tmp_14 ;
tmp_35 =  (C1_5_tw) * tmp_12 ;
tmp_36 = (tmp_34 - tmp_35);
tmp_37 =  (C2_5_tw) * a[4].re ;
tmp_38 =  (C3_5_tw) * a[2].re ;
tmp_39 = (tmp_37 + tmp_38);
tmp_40 =  (C2_5_tw) * a[1].re ;
tmp_41 =  (C3_5_tw) * a[3].re ;
tmp_42 = (tmp_40 + tmp_41);
tmp_43 = (tmp_36 + tmp_39 + a[0].im - tmp_42);
res[2].re = ( w[1].re * tmp_33  -  w[1].im * tmp_43 );
tmp_44 = (tmp_26 + tmp_32 + a[0].re - tmp_29);
tmp_45 = (tmp_36 + tmp_42 + a[0].im - tmp_39);
res[3].re = ( w[2].re * tmp_44  -  w[2].im * tmp_45 );
tmp_46 = (tmp_4 + tmp_10 + a[0].re - tmp_7);
tmp_47 = (tmp_16 + tmp_22 + a[0].im - tmp_19);
res[4].re = ( w[3].re * tmp_46  -  w[3].im * tmp_47 );
res[0].im = (a[0].im + a[1].im + a[2].im + a[3].im + a[4].im);
res[1].im = ( w[0].re * tmp_23  +  w[0].im * tmp_11 );
res[2].im = ( w[1].re * tmp_43  +  w[1].im * tmp_33 );
res[3].im = ( w[2].re * tmp_45  +  w[2].im * tmp_44 );
res[4].im = ( w[3].re * tmp_47  +  w[3].im * tmp_46 );


memory<complex<float16_t>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<float16_t>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<float16_t>>::store(out,2 * stageLength * strideFactor , res[2]);
memory<complex<float16_t>>::store(out,3 * stageLength * strideFactor , res[3]);
memory<complex<float16_t>>::store(out,4 * stageLength * strideFactor , res[4]);


};

/* Nb Multiplications = 24, 
Nb Additions = 48,
Nb Negates = 12,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_last(complex<float16_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[5];
complex<float16_t> res[5];


 float16_t tmp_0;
 float16_t tmp_1;
 float16_t tmp_2;
 float16_t tmp_3;
 float16_t tmp_4;
 float16_t tmp_5;
 float16_t tmp_6;
 float16_t tmp_7;
 float16_t tmp_8;
 float16_t tmp_9;
 float16_t tmp_10;
 float16_t tmp_11;
 float16_t tmp_12;
 float16_t tmp_13;
 float16_t tmp_14;
 float16_t tmp_15;
 float16_t tmp_16;
 float16_t tmp_17;
 float16_t tmp_18;
 float16_t tmp_19;
 float16_t tmp_20;
 float16_t tmp_21;
 float16_t tmp_22;
 float16_t tmp_23;
 float16_t tmp_24;
 float16_t tmp_25;
 float16_t tmp_26;
 float16_t tmp_27;
 float16_t tmp_28;
 float16_t tmp_29;
 float16_t tmp_30;
 float16_t tmp_31;
 float16_t tmp_32;
 float16_t tmp_33;
 float16_t tmp_34;
 float16_t tmp_35;
 float16_t tmp_36;
 float16_t tmp_37;
 float16_t tmp_38;
 float16_t tmp_39;


a[0] = memory<complex<float16_t>>::load(in,0 * strideFactor);
a[1] = memory<complex<float16_t>>::load(in,1 * strideFactor);
a[2] = memory<complex<float16_t>>::load(in,2 * strideFactor);
a[3] = memory<complex<float16_t>>::load(in,3 * strideFactor);
a[4] = memory<complex<float16_t>>::load(in,4 * strideFactor);


res[0].re = (a[0].re + a[1].re + a[2].re + a[3].re + a[4].re);
tmp_0 = (a[1].re + a[4].re);
tmp_1 =  (C0_5_notw) * tmp_0 ;
tmp_2 = (a[2].re + a[3].re);
tmp_3 =  (C1_5_notw) * tmp_2 ;
tmp_4 = (tmp_1 - tmp_3);
tmp_5 =  (C2_5_notw) * a[2].im ;
tmp_6 =  (C3_5_notw) * a[1].im ;
tmp_7 = (tmp_5 + tmp_6);
tmp_8 =  (C2_5_notw) * a[3].im ;
tmp_9 =  (C3_5_notw) * a[4].im ;
tmp_10 = (tmp_8 + tmp_9);
res[1].re = (tmp_4 + tmp_7 + a[0].re - tmp_10);
tmp_11 =  (C0_5_notw) * tmp_2 ;
tmp_12 =  (C1_5_notw) * tmp_0 ;
tmp_13 = (tmp_11 - tmp_12);
tmp_14 =  (C2_5_notw) * a[1].im ;
tmp_15 =  (C3_5_notw) * a[3].im ;
tmp_16 = (tmp_14 + tmp_15);
tmp_17 =  (C2_5_notw) * a[4].im ;
tmp_18 =  (C3_5_notw) * a[2].im ;
tmp_19 = (tmp_17 + tmp_18);
res[2].re = (tmp_13 + tmp_16 + a[0].re - tmp_19);
res[3].re = (tmp_13 + tmp_19 + a[0].re - tmp_16);
res[4].re = (tmp_4 + tmp_10 + a[0].re - tmp_7);
res[0].im = (a[0].im + a[1].im + a[2].im + a[3].im + a[4].im);
tmp_20 = (a[1].im + a[4].im);
tmp_21 =  (C0_5_notw) * tmp_20 ;
tmp_22 = (a[2].im + a[3].im);
tmp_23 =  (C1_5_notw) * tmp_22 ;
tmp_24 = (tmp_21 - tmp_23);
tmp_25 =  (C2_5_notw) * a[3].re ;
tmp_26 =  (C3_5_notw) * a[4].re ;
tmp_27 = (tmp_25 + tmp_26);
tmp_28 =  (C2_5_notw) * a[2].re ;
tmp_29 =  (C3_5_notw) * a[1].re ;
tmp_30 = (tmp_28 + tmp_29);
res[1].im = (tmp_24 + tmp_27 + a[0].im - tmp_30);
tmp_31 =  (C0_5_notw) * tmp_22 ;
tmp_32 =  (C1_5_notw) * tmp_20 ;
tmp_33 = (tmp_31 - tmp_32);
tmp_34 =  (C2_5_notw) * a[4].re ;
tmp_35 =  (C3_5_notw) * a[2].re ;
tmp_36 = (tmp_34 + tmp_35);
tmp_37 =  (C2_5_notw) * a[1].re ;
tmp_38 =  (C3_5_notw) * a[3].re ;
tmp_39 = (tmp_37 + tmp_38);
res[2].im = (tmp_33 + tmp_36 + a[0].im - tmp_39);
res[3].im = (tmp_33 + tmp_39 + a[0].im - tmp_36);
res[4].im = (tmp_24 + tmp_30 + a[0].im - tmp_27);


memory<complex<float16_t>>::store(in,0 * strideFactor , res[0]);
memory<complex<float16_t>>::store(in,1 * strideFactor , res[1]);
memory<complex<float16_t>>::store(in,2 * strideFactor , res[2]);
memory<complex<float16_t>>::store(in,3 * strideFactor , res[3]);
memory<complex<float16_t>>::store(in,4 * strideFactor , res[4]);


};

/* Nb Multiplications = 24, 
Nb Additions = 48,
Nb Negates = 12,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_last(const complex<float16_t> *in,complex<float16_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[5];
complex<float16_t> res[5];


 float16_t tmp_0;
 float16_t tmp_1;
 float16_t tmp_2;
 float16_t tmp_3;
 float16_t tmp_4;
 float16_t tmp_5;
 float16_t tmp_6;
 float16_t tmp_7;
 float16_t tmp_8;
 float16_t tmp_9;
 float16_t tmp_10;
 float16_t tmp_11;
 float16_t tmp_12;
 float16_t tmp_13;
 float16_t tmp_14;
 float16_t tmp_15;
 float16_t tmp_16;
 float16_t tmp_17;
 float16_t tmp_18;
 float16_t tmp_19;
 float16_t tmp_20;
 float16_t tmp_21;
 float16_t tmp_22;
 float16_t tmp_23;
 float16_t tmp_24;
 float16_t tmp_25;
 float16_t tmp_26;
 float16_t tmp_27;
 float16_t tmp_28;
 float16_t tmp_29;
 float16_t tmp_30;
 float16_t tmp_31;
 float16_t tmp_32;
 float16_t tmp_33;
 float16_t tmp_34;
 float16_t tmp_35;
 float16_t tmp_36;
 float16_t tmp_37;
 float16_t tmp_38;
 float16_t tmp_39;


a[0] = memory<complex<float16_t>>::load(in,0 * strideFactor);
a[1] = memory<complex<float16_t>>::load(in,1 * strideFactor);
a[2] = memory<complex<float16_t>>::load(in,2 * strideFactor);
a[3] = memory<complex<float16_t>>::load(in,3 * strideFactor);
a[4] = memory<complex<float16_t>>::load(in,4 * strideFactor);


res[0].re = (a[0].re + a[1].re + a[2].re + a[3].re + a[4].re);
tmp_0 = (a[1].re + a[4].re);
tmp_1 =  (C0_5_notw) * tmp_0 ;
tmp_2 = (a[2].re + a[3].re);
tmp_3 =  (C1_5_notw) * tmp_2 ;
tmp_4 = (tmp_1 - tmp_3);
tmp_5 =  (C2_5_notw) * a[2].im ;
tmp_6 =  (C3_5_notw) * a[1].im ;
tmp_7 = (tmp_5 + tmp_6);
tmp_8 =  (C2_5_notw) * a[3].im ;
tmp_9 =  (C3_5_notw) * a[4].im ;
tmp_10 = (tmp_8 + tmp_9);
res[1].re = (tmp_4 + tmp_7 + a[0].re - tmp_10);
tmp_11 =  (C0_5_notw) * tmp_2 ;
tmp_12 =  (C1_5_notw) * tmp_0 ;
tmp_13 = (tmp_11 - tmp_12);
tmp_14 =  (C2_5_notw) * a[1].im ;
tmp_15 =  (C3_5_notw) * a[3].im ;
tmp_16 = (tmp_14 + tmp_15);
tmp_17 =  (C2_5_notw) * a[4].im ;
tmp_18 =  (C3_5_notw) * a[2].im ;
tmp_19 = (tmp_17 + tmp_18);
res[2].re = (tmp_13 + tmp_16 + a[0].re - tmp_19);
res[3].re = (tmp_13 + tmp_19 + a[0].re - tmp_16);
res[4].re = (tmp_4 + tmp_10 + a[0].re - tmp_7);
res[0].im = (a[0].im + a[1].im + a[2].im + a[3].im + a[4].im);
tmp_20 = (a[1].im + a[4].im);
tmp_21 =  (C0_5_notw) * tmp_20 ;
tmp_22 = (a[2].im + a[3].im);
tmp_23 =  (C1_5_notw) * tmp_22 ;
tmp_24 = (tmp_21 - tmp_23);
tmp_25 =  (C2_5_notw) * a[3].re ;
tmp_26 =  (C3_5_notw) * a[4].re ;
tmp_27 = (tmp_25 + tmp_26);
tmp_28 =  (C2_5_notw) * a[2].re ;
tmp_29 =  (C3_5_notw) * a[1].re ;
tmp_30 = (tmp_28 + tmp_29);
res[1].im = (tmp_24 + tmp_27 + a[0].im - tmp_30);
tmp_31 =  (C0_5_notw) * tmp_22 ;
tmp_32 =  (C1_5_notw) * tmp_20 ;
tmp_33 = (tmp_31 - tmp_32);
tmp_34 =  (C2_5_notw) * a[4].re ;
tmp_35 =  (C3_5_notw) * a[2].re ;
tmp_36 = (tmp_34 + tmp_35);
tmp_37 =  (C2_5_notw) * a[1].re ;
tmp_38 =  (C3_5_notw) * a[3].re ;
tmp_39 = (tmp_37 + tmp_38);
res[2].im = (tmp_33 + tmp_36 + a[0].im - tmp_39);
res[3].im = (tmp_33 + tmp_39 + a[0].im - tmp_36);
res[4].im = (tmp_24 + tmp_30 + a[0].im - tmp_27);


memory<complex<float16_t>>::store(out,0 * strideFactor , res[0]);
memory<complex<float16_t>>::store(out,1 * strideFactor , res[1]);
memory<complex<float16_t>>::store(out,2 * strideFactor , res[2]);
memory<complex<float16_t>>::store(out,3 * strideFactor , res[3]);
memory<complex<float16_t>>::store(out,4 * strideFactor , res[4]);


};

};

/**************

INVERSE FFT 

***************/
template<int inputstrideenabled> 
struct radix<INVERSE,float16_t, 5,inputstrideenabled>{

static constexpr float16_t C0_5_tw{0.30901699437494745f16};
  static constexpr float16_t C1_5_tw{0.8090169943749475f16};
  static constexpr float16_t C2_5_tw{0.5877852522924731f16};
  static constexpr float16_t C3_5_tw{0.9510565162951535f16};
  
static constexpr float16_t C0_5_notw{0.30901699437494745f16};
  static constexpr float16_t C1_5_notw{0.8090169943749475f16};
  static constexpr float16_t C2_5_notw{0.5877852522924731f16};
  static constexpr float16_t C3_5_notw{0.9510565162951535f16};
  

/* Nb Multiplications = 24, 
Nb Additions = 48,
Nb Negates = 12,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_notw(int stageLength,complex<float16_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[5];
complex<float16_t> res[5];


 float16_t tmp_0;
 float16_t tmp_1;
 float16_t tmp_2;
 float16_t tmp_3;
 float16_t tmp_4;
 float16_t tmp_5;
 float16_t tmp_6;
 float16_t tmp_7;
 float16_t tmp_8;
 float16_t tmp_9;
 float16_t tmp_10;
 float16_t tmp_11;
 float16_t tmp_12;
 float16_t tmp_13;
 float16_t tmp_14;
 float16_t tmp_15;
 float16_t tmp_16;
 float16_t tmp_17;
 float16_t tmp_18;
 float16_t tmp_19;
 float16_t tmp_20;
 float16_t tmp_21;
 float16_t tmp_22;
 float16_t tmp_23;
 float16_t tmp_24;
 float16_t tmp_25;
 float16_t tmp_26;
 float16_t tmp_27;
 float16_t tmp_28;
 float16_t tmp_29;
 float16_t tmp_30;
 float16_t tmp_31;
 float16_t tmp_32;
 float16_t tmp_33;
 float16_t tmp_34;
 float16_t tmp_35;
 float16_t tmp_36;
 float16_t tmp_37;
 float16_t tmp_38;
 float16_t tmp_39;


a[0] = memory<complex<float16_t>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<float16_t>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<float16_t>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<float16_t>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<float16_t>>::load(in,4 * stageLength * strideFactor);


res[0].re = (a[0].re + a[1].re + a[2].re + a[3].re + a[4].re);
tmp_0 = (a[1].re + a[4].re);
tmp_1 =  (C0_5_notw) * tmp_0 ;
tmp_2 = (a[2].re + a[3].re);
tmp_3 =  (C1_5_notw) * tmp_2 ;
tmp_4 = (tmp_1 - tmp_3);
tmp_5 =  (C2_5_notw) * a[3].im ;
tmp_6 =  (C3_5_notw) * a[4].im ;
tmp_7 = (tmp_5 + tmp_6);
tmp_8 =  (C2_5_notw) * a[2].im ;
tmp_9 =  (C3_5_notw) * a[1].im ;
tmp_10 = (tmp_8 + tmp_9);
res[1].re = (tmp_4 + tmp_7 + a[0].re - tmp_10);
tmp_11 =  (C0_5_notw) * tmp_2 ;
tmp_12 =  (C1_5_notw) * tmp_0 ;
tmp_13 = (tmp_11 - tmp_12);
tmp_14 =  (C2_5_notw) * a[4].im ;
tmp_15 =  (C3_5_notw) * a[2].im ;
tmp_16 = (tmp_14 + tmp_15);
tmp_17 =  (C2_5_notw) * a[1].im ;
tmp_18 =  (C3_5_notw) * a[3].im ;
tmp_19 = (tmp_17 + tmp_18);
res[2].re = (tmp_13 + tmp_16 + a[0].re - tmp_19);
res[3].re = (tmp_13 + tmp_19 + a[0].re - tmp_16);
res[4].re = (tmp_4 + tmp_10 + a[0].re - tmp_7);
res[0].im = (a[0].im + a[1].im + a[2].im + a[3].im + a[4].im);
tmp_20 = (a[1].im + a[4].im);
tmp_21 =  (C0_5_notw) * tmp_20 ;
tmp_22 = (a[2].im + a[3].im);
tmp_23 =  (C1_5_notw) * tmp_22 ;
tmp_24 = (tmp_21 - tmp_23);
tmp_25 =  (C2_5_notw) * a[2].re ;
tmp_26 =  (C3_5_notw) * a[1].re ;
tmp_27 = (tmp_25 + tmp_26);
tmp_28 =  (C2_5_notw) * a[3].re ;
tmp_29 =  (C3_5_notw) * a[4].re ;
tmp_30 = (tmp_28 + tmp_29);
res[1].im = (tmp_24 + tmp_27 + a[0].im - tmp_30);
tmp_31 =  (C0_5_notw) * tmp_22 ;
tmp_32 =  (C1_5_notw) * tmp_20 ;
tmp_33 = (tmp_31 - tmp_32);
tmp_34 =  (C2_5_notw) * a[1].re ;
tmp_35 =  (C3_5_notw) * a[3].re ;
tmp_36 = (tmp_34 + tmp_35);
tmp_37 =  (C2_5_notw) * a[4].re ;
tmp_38 =  (C3_5_notw) * a[2].re ;
tmp_39 = (tmp_37 + tmp_38);
res[2].im = (tmp_33 + tmp_36 + a[0].im - tmp_39);
res[3].im = (tmp_33 + tmp_39 + a[0].im - tmp_36);
res[4].im = (tmp_24 + tmp_30 + a[0].im - tmp_27);


memory<complex<float16_t>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<float16_t>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<float16_t>>::store(in,2 * stageLength * strideFactor , res[2]);
memory<complex<float16_t>>::store(in,3 * stageLength * strideFactor , res[3]);
memory<complex<float16_t>>::store(in,4 * stageLength * strideFactor , res[4]);


};

/* Nb Multiplications = 40, 
Nb Additions = 56,
Nb Negates = 16,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_tw(int stageLength,const complex<float16_t> *w,complex<float16_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[5];
complex<float16_t> res[5];


 float16_t tmp_0;
 float16_t tmp_1;
 float16_t tmp_2;
 float16_t tmp_3;
 float16_t tmp_4;
 float16_t tmp_5;
 float16_t tmp_6;
 float16_t tmp_7;
 float16_t tmp_8;
 float16_t tmp_9;
 float16_t tmp_10;
 float16_t tmp_11;
 float16_t tmp_12;
 float16_t tmp_13;
 float16_t tmp_14;
 float16_t tmp_15;
 float16_t tmp_16;
 float16_t tmp_17;
 float16_t tmp_18;
 float16_t tmp_19;
 float16_t tmp_20;
 float16_t tmp_21;
 float16_t tmp_22;
 float16_t tmp_23;
 float16_t tmp_24;
 float16_t tmp_25;
 float16_t tmp_26;
 float16_t tmp_27;
 float16_t tmp_28;
 float16_t tmp_29;
 float16_t tmp_30;
 float16_t tmp_31;
 float16_t tmp_32;
 float16_t tmp_33;
 float16_t tmp_34;
 float16_t tmp_35;
 float16_t tmp_36;
 float16_t tmp_37;
 float16_t tmp_38;
 float16_t tmp_39;
 float16_t tmp_40;
 float16_t tmp_41;
 float16_t tmp_42;
 float16_t tmp_43;
 float16_t tmp_44;
 float16_t tmp_45;
 float16_t tmp_46;
 float16_t tmp_47;


a[0] = memory<complex<float16_t>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<float16_t>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<float16_t>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<float16_t>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<float16_t>>::load(in,4 * stageLength * strideFactor);


res[0].re = (a[0].re + a[1].re + a[2].re + a[3].re + a[4].re);
tmp_0 = (a[1].re + a[4].re);
tmp_1 =  (C0_5_tw) * tmp_0 ;
tmp_2 = (a[2].re + a[3].re);
tmp_3 =  (C1_5_tw) * tmp_2 ;
tmp_4 = (tmp_1 - tmp_3);
tmp_5 =  (C2_5_tw) * a[3].im ;
tmp_6 =  (C3_5_tw) * a[4].im ;
tmp_7 = (tmp_5 + tmp_6);
tmp_8 =  (C2_5_tw) * a[2].im ;
tmp_9 =  (C3_5_tw) * a[1].im ;
tmp_10 = (tmp_8 + tmp_9);
tmp_11 = (tmp_4 + tmp_7 + a[0].re - tmp_10);
tmp_12 = (a[1].im + a[4].im);
tmp_13 =  (C0_5_tw) * tmp_12 ;
tmp_14 = (a[2].im + a[3].im);
tmp_15 =  (C1_5_tw) * tmp_14 ;
tmp_16 = (tmp_13 - tmp_15);
tmp_17 =  (C2_5_tw) * a[2].re ;
tmp_18 =  (C3_5_tw) * a[1].re ;
tmp_19 = (tmp_17 + tmp_18);
tmp_20 =  (C2_5_tw) * a[3].re ;
tmp_21 =  (C3_5_tw) * a[4].re ;
tmp_22 = (tmp_20 + tmp_21);
tmp_23 = (tmp_16 + tmp_19 + a[0].im - tmp_22);
res[1].re = ( w[0].re * tmp_11  +  w[0].im * tmp_23 );
tmp_24 =  (C0_5_tw) * tmp_2 ;
tmp_25 =  (C1_5_tw) * tmp_0 ;
tmp_26 = (tmp_24 - tmp_25);
tmp_27 =  (C2_5_tw) * a[4].im ;
tmp_28 =  (C3_5_tw) * a[2].im ;
tmp_29 = (tmp_27 + tmp_28);
tmp_30 =  (C2_5_tw) * a[1].im ;
tmp_31 =  (C3_5_tw) * a[3].im ;
tmp_32 = (tmp_30 + tmp_31);
tmp_33 = (tmp_26 + tmp_29 + a[0].re - tmp_32);
tmp_34 =  (C0_5_tw) * tmp_14 ;
tmp_35 =  (C1_5_tw) * tmp_12 ;
tmp_36 = (tmp_34 - tmp_35);
tmp_37 =  (C2_5_tw) * a[1].re ;
tmp_38 =  (C3_5_tw) * a[3].re ;
tmp_39 = (tmp_37 + tmp_38);
tmp_40 =  (C2_5_tw) * a[4].re ;
tmp_41 =  (C3_5_tw) * a[2].re ;
tmp_42 = (tmp_40 + tmp_41);
tmp_43 = (tmp_36 + tmp_39 + a[0].im - tmp_42);
res[2].re = ( w[1].re * tmp_33  +  w[1].im * tmp_43 );
tmp_44 = (tmp_26 + tmp_32 + a[0].re - tmp_29);
tmp_45 = (tmp_36 + tmp_42 + a[0].im - tmp_39);
res[3].re = ( w[2].re * tmp_44  +  w[2].im * tmp_45 );
tmp_46 = (tmp_4 + tmp_10 + a[0].re - tmp_7);
tmp_47 = (tmp_16 + tmp_22 + a[0].im - tmp_19);
res[4].re = ( w[3].re * tmp_46  +  w[3].im * tmp_47 );
res[0].im = (a[0].im + a[1].im + a[2].im + a[3].im + a[4].im);
res[1].im = ( w[0].re * tmp_23  -  w[0].im * tmp_11 );
res[2].im = ( w[1].re * tmp_43  -  w[1].im * tmp_33 );
res[3].im = ( w[2].re * tmp_45  -  w[2].im * tmp_44 );
res[4].im = ( w[3].re * tmp_47  -  w[3].im * tmp_46 );


memory<complex<float16_t>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<float16_t>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<float16_t>>::store(in,2 * stageLength * strideFactor , res[2]);
memory<complex<float16_t>>::store(in,3 * stageLength * strideFactor , res[3]);
memory<complex<float16_t>>::store(in,4 * stageLength * strideFactor , res[4]);


};

/* Nb Multiplications = 24, 
Nb Additions = 48,
Nb Negates = 12,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_notw(int stageLength,const complex<float16_t> *in,complex<float16_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[5];
complex<float16_t> res[5];


 float16_t tmp_0;
 float16_t tmp_1;
 float16_t tmp_2;
 float16_t tmp_3;
 float16_t tmp_4;
 float16_t tmp_5;
 float16_t tmp_6;
 float16_t tmp_7;
 float16_t tmp_8;
 float16_t tmp_9;
 float16_t tmp_10;
 float16_t tmp_11;
 float16_t tmp_12;
 float16_t tmp_13;
 float16_t tmp_14;
 float16_t tmp_15;
 float16_t tmp_16;
 float16_t tmp_17;
 float16_t tmp_18;
 float16_t tmp_19;
 float16_t tmp_20;
 float16_t tmp_21;
 float16_t tmp_22;
 float16_t tmp_23;
 float16_t tmp_24;
 float16_t tmp_25;
 float16_t tmp_26;
 float16_t tmp_27;
 float16_t tmp_28;
 float16_t tmp_29;
 float16_t tmp_30;
 float16_t tmp_31;
 float16_t tmp_32;
 float16_t tmp_33;
 float16_t tmp_34;
 float16_t tmp_35;
 float16_t tmp_36;
 float16_t tmp_37;
 float16_t tmp_38;
 float16_t tmp_39;


a[0] = memory<complex<float16_t>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<float16_t>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<float16_t>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<float16_t>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<float16_t>>::load(in,4 * stageLength * strideFactor);


res[0].re = (a[0].re + a[1].re + a[2].re + a[3].re + a[4].re);
tmp_0 = (a[1].re + a[4].re);
tmp_1 =  (C0_5_notw) * tmp_0 ;
tmp_2 = (a[2].re + a[3].re);
tmp_3 =  (C1_5_notw) * tmp_2 ;
tmp_4 = (tmp_1 - tmp_3);
tmp_5 =  (C2_5_notw) * a[3].im ;
tmp_6 =  (C3_5_notw) * a[4].im ;
tmp_7 = (tmp_5 + tmp_6);
tmp_8 =  (C2_5_notw) * a[2].im ;
tmp_9 =  (C3_5_notw) * a[1].im ;
tmp_10 = (tmp_8 + tmp_9);
res[1].re = (tmp_4 + tmp_7 + a[0].re - tmp_10);
tmp_11 =  (C0_5_notw) * tmp_2 ;
tmp_12 =  (C1_5_notw) * tmp_0 ;
tmp_13 = (tmp_11 - tmp_12);
tmp_14 =  (C2_5_notw) * a[4].im ;
tmp_15 =  (C3_5_notw) * a[2].im ;
tmp_16 = (tmp_14 + tmp_15);
tmp_17 =  (C2_5_notw) * a[1].im ;
tmp_18 =  (C3_5_notw) * a[3].im ;
tmp_19 = (tmp_17 + tmp_18);
res[2].re = (tmp_13 + tmp_16 + a[0].re - tmp_19);
res[3].re = (tmp_13 + tmp_19 + a[0].re - tmp_16);
res[4].re = (tmp_4 + tmp_10 + a[0].re - tmp_7);
res[0].im = (a[0].im + a[1].im + a[2].im + a[3].im + a[4].im);
tmp_20 = (a[1].im + a[4].im);
tmp_21 =  (C0_5_notw) * tmp_20 ;
tmp_22 = (a[2].im + a[3].im);
tmp_23 =  (C1_5_notw) * tmp_22 ;
tmp_24 = (tmp_21 - tmp_23);
tmp_25 =  (C2_5_notw) * a[2].re ;
tmp_26 =  (C3_5_notw) * a[1].re ;
tmp_27 = (tmp_25 + tmp_26);
tmp_28 =  (C2_5_notw) * a[3].re ;
tmp_29 =  (C3_5_notw) * a[4].re ;
tmp_30 = (tmp_28 + tmp_29);
res[1].im = (tmp_24 + tmp_27 + a[0].im - tmp_30);
tmp_31 =  (C0_5_notw) * tmp_22 ;
tmp_32 =  (C1_5_notw) * tmp_20 ;
tmp_33 = (tmp_31 - tmp_32);
tmp_34 =  (C2_5_notw) * a[1].re ;
tmp_35 =  (C3_5_notw) * a[3].re ;
tmp_36 = (tmp_34 + tmp_35);
tmp_37 =  (C2_5_notw) * a[4].re ;
tmp_38 =  (C3_5_notw) * a[2].re ;
tmp_39 = (tmp_37 + tmp_38);
res[2].im = (tmp_33 + tmp_36 + a[0].im - tmp_39);
res[3].im = (tmp_33 + tmp_39 + a[0].im - tmp_36);
res[4].im = (tmp_24 + tmp_30 + a[0].im - tmp_27);


memory<complex<float16_t>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<float16_t>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<float16_t>>::store(out,2 * stageLength * strideFactor , res[2]);
memory<complex<float16_t>>::store(out,3 * stageLength * strideFactor , res[3]);
memory<complex<float16_t>>::store(out,4 * stageLength * strideFactor , res[4]);


};

/* Nb Multiplications = 40, 
Nb Additions = 56,
Nb Negates = 16,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_tw(int stageLength,const complex<float16_t> *w,const complex<float16_t> *in,complex<float16_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[5];
complex<float16_t> res[5];


 float16_t tmp_0;
 float16_t tmp_1;
 float16_t tmp_2;
 float16_t tmp_3;
 float16_t tmp_4;
 float16_t tmp_5;
 float16_t tmp_6;
 float16_t tmp_7;
 float16_t tmp_8;
 float16_t tmp_9;
 float16_t tmp_10;
 float16_t tmp_11;
 float16_t tmp_12;
 float16_t tmp_13;
 float16_t tmp_14;
 float16_t tmp_15;
 float16_t tmp_16;
 float16_t tmp_17;
 float16_t tmp_18;
 float16_t tmp_19;
 float16_t tmp_20;
 float16_t tmp_21;
 float16_t tmp_22;
 float16_t tmp_23;
 float16_t tmp_24;
 float16_t tmp_25;
 float16_t tmp_26;
 float16_t tmp_27;
 float16_t tmp_28;
 float16_t tmp_29;
 float16_t tmp_30;
 float16_t tmp_31;
 float16_t tmp_32;
 float16_t tmp_33;
 float16_t tmp_34;
 float16_t tmp_35;
 float16_t tmp_36;
 float16_t tmp_37;
 float16_t tmp_38;
 float16_t tmp_39;
 float16_t tmp_40;
 float16_t tmp_41;
 float16_t tmp_42;
 float16_t tmp_43;
 float16_t tmp_44;
 float16_t tmp_45;
 float16_t tmp_46;
 float16_t tmp_47;


a[0] = memory<complex<float16_t>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<float16_t>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<float16_t>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<float16_t>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<float16_t>>::load(in,4 * stageLength * strideFactor);


res[0].re = (a[0].re + a[1].re + a[2].re + a[3].re + a[4].re);
tmp_0 = (a[1].re + a[4].re);
tmp_1 =  (C0_5_tw) * tmp_0 ;
tmp_2 = (a[2].re + a[3].re);
tmp_3 =  (C1_5_tw) * tmp_2 ;
tmp_4 = (tmp_1 - tmp_3);
tmp_5 =  (C2_5_tw) * a[3].im ;
tmp_6 =  (C3_5_tw) * a[4].im ;
tmp_7 = (tmp_5 + tmp_6);
tmp_8 =  (C2_5_tw) * a[2].im ;
tmp_9 =  (C3_5_tw) * a[1].im ;
tmp_10 = (tmp_8 + tmp_9);
tmp_11 = (tmp_4 + tmp_7 + a[0].re - tmp_10);
tmp_12 = (a[1].im + a[4].im);
tmp_13 =  (C0_5_tw) * tmp_12 ;
tmp_14 = (a[2].im + a[3].im);
tmp_15 =  (C1_5_tw) * tmp_14 ;
tmp_16 = (tmp_13 - tmp_15);
tmp_17 =  (C2_5_tw) * a[2].re ;
tmp_18 =  (C3_5_tw) * a[1].re ;
tmp_19 = (tmp_17 + tmp_18);
tmp_20 =  (C2_5_tw) * a[3].re ;
tmp_21 =  (C3_5_tw) * a[4].re ;
tmp_22 = (tmp_20 + tmp_21);
tmp_23 = (tmp_16 + tmp_19 + a[0].im - tmp_22);
res[1].re = ( w[0].re * tmp_11  +  w[0].im * tmp_23 );
tmp_24 =  (C0_5_tw) * tmp_2 ;
tmp_25 =  (C1_5_tw) * tmp_0 ;
tmp_26 = (tmp_24 - tmp_25);
tmp_27 =  (C2_5_tw) * a[4].im ;
tmp_28 =  (C3_5_tw) * a[2].im ;
tmp_29 = (tmp_27 + tmp_28);
tmp_30 =  (C2_5_tw) * a[1].im ;
tmp_31 =  (C3_5_tw) * a[3].im ;
tmp_32 = (tmp_30 + tmp_31);
tmp_33 = (tmp_26 + tmp_29 + a[0].re - tmp_32);
tmp_34 =  (C0_5_tw) * tmp_14 ;
tmp_35 =  (C1_5_tw) * tmp_12 ;
tmp_36 = (tmp_34 - tmp_35);
tmp_37 =  (C2_5_tw) * a[1].re ;
tmp_38 =  (C3_5_tw) * a[3].re ;
tmp_39 = (tmp_37 + tmp_38);
tmp_40 =  (C2_5_tw) * a[4].re ;
tmp_41 =  (C3_5_tw) * a[2].re ;
tmp_42 = (tmp_40 + tmp_41);
tmp_43 = (tmp_36 + tmp_39 + a[0].im - tmp_42);
res[2].re = ( w[1].re * tmp_33  +  w[1].im * tmp_43 );
tmp_44 = (tmp_26 + tmp_32 + a[0].re - tmp_29);
tmp_45 = (tmp_36 + tmp_42 + a[0].im - tmp_39);
res[3].re = ( w[2].re * tmp_44  +  w[2].im * tmp_45 );
tmp_46 = (tmp_4 + tmp_10 + a[0].re - tmp_7);
tmp_47 = (tmp_16 + tmp_22 + a[0].im - tmp_19);
res[4].re = ( w[3].re * tmp_46  +  w[3].im * tmp_47 );
res[0].im = (a[0].im + a[1].im + a[2].im + a[3].im + a[4].im);
res[1].im = ( w[0].re * tmp_23  -  w[0].im * tmp_11 );
res[2].im = ( w[1].re * tmp_43  -  w[1].im * tmp_33 );
res[3].im = ( w[2].re * tmp_45  -  w[2].im * tmp_44 );
res[4].im = ( w[3].re * tmp_47  -  w[3].im * tmp_46 );


memory<complex<float16_t>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<float16_t>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<float16_t>>::store(out,2 * stageLength * strideFactor , res[2]);
memory<complex<float16_t>>::store(out,3 * stageLength * strideFactor , res[3]);
memory<complex<float16_t>>::store(out,4 * stageLength * strideFactor , res[4]);


};

/* Nb Multiplications = 24, 
Nb Additions = 48,
Nb Negates = 12,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_last(complex<float16_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[5];
complex<float16_t> res[5];


 float16_t tmp_0;
 float16_t tmp_1;
 float16_t tmp_2;
 float16_t tmp_3;
 float16_t tmp_4;
 float16_t tmp_5;
 float16_t tmp_6;
 float16_t tmp_7;
 float16_t tmp_8;
 float16_t tmp_9;
 float16_t tmp_10;
 float16_t tmp_11;
 float16_t tmp_12;
 float16_t tmp_13;
 float16_t tmp_14;
 float16_t tmp_15;
 float16_t tmp_16;
 float16_t tmp_17;
 float16_t tmp_18;
 float16_t tmp_19;
 float16_t tmp_20;
 float16_t tmp_21;
 float16_t tmp_22;
 float16_t tmp_23;
 float16_t tmp_24;
 float16_t tmp_25;
 float16_t tmp_26;
 float16_t tmp_27;
 float16_t tmp_28;
 float16_t tmp_29;
 float16_t tmp_30;
 float16_t tmp_31;
 float16_t tmp_32;
 float16_t tmp_33;
 float16_t tmp_34;
 float16_t tmp_35;
 float16_t tmp_36;
 float16_t tmp_37;
 float16_t tmp_38;
 float16_t tmp_39;


a[0] = memory<complex<float16_t>>::load(in,0 * strideFactor);
a[1] = memory<complex<float16_t>>::load(in,1 * strideFactor);
a[2] = memory<complex<float16_t>>::load(in,2 * strideFactor);
a[3] = memory<complex<float16_t>>::load(in,3 * strideFactor);
a[4] = memory<complex<float16_t>>::load(in,4 * strideFactor);


res[0].re = (a[0].re + a[1].re + a[2].re + a[3].re + a[4].re);
tmp_0 = (a[1].re + a[4].re);
tmp_1 =  (C0_5_notw) * tmp_0 ;
tmp_2 = (a[2].re + a[3].re);
tmp_3 =  (C1_5_notw) * tmp_2 ;
tmp_4 = (tmp_1 - tmp_3);
tmp_5 =  (C2_5_notw) * a[3].im ;
tmp_6 =  (C3_5_notw) * a[4].im ;
tmp_7 = (tmp_5 + tmp_6);
tmp_8 =  (C2_5_notw) * a[2].im ;
tmp_9 =  (C3_5_notw) * a[1].im ;
tmp_10 = (tmp_8 + tmp_9);
res[1].re = (tmp_4 + tmp_7 + a[0].re - tmp_10);
tmp_11 =  (C0_5_notw) * tmp_2 ;
tmp_12 =  (C1_5_notw) * tmp_0 ;
tmp_13 = (tmp_11 - tmp_12);
tmp_14 =  (C2_5_notw) * a[4].im ;
tmp_15 =  (C3_5_notw) * a[2].im ;
tmp_16 = (tmp_14 + tmp_15);
tmp_17 =  (C2_5_notw) * a[1].im ;
tmp_18 =  (C3_5_notw) * a[3].im ;
tmp_19 = (tmp_17 + tmp_18);
res[2].re = (tmp_13 + tmp_16 + a[0].re - tmp_19);
res[3].re = (tmp_13 + tmp_19 + a[0].re - tmp_16);
res[4].re = (tmp_4 + tmp_10 + a[0].re - tmp_7);
res[0].im = (a[0].im + a[1].im + a[2].im + a[3].im + a[4].im);
tmp_20 = (a[1].im + a[4].im);
tmp_21 =  (C0_5_notw) * tmp_20 ;
tmp_22 = (a[2].im + a[3].im);
tmp_23 =  (C1_5_notw) * tmp_22 ;
tmp_24 = (tmp_21 - tmp_23);
tmp_25 =  (C2_5_notw) * a[2].re ;
tmp_26 =  (C3_5_notw) * a[1].re ;
tmp_27 = (tmp_25 + tmp_26);
tmp_28 =  (C2_5_notw) * a[3].re ;
tmp_29 =  (C3_5_notw) * a[4].re ;
tmp_30 = (tmp_28 + tmp_29);
res[1].im = (tmp_24 + tmp_27 + a[0].im - tmp_30);
tmp_31 =  (C0_5_notw) * tmp_22 ;
tmp_32 =  (C1_5_notw) * tmp_20 ;
tmp_33 = (tmp_31 - tmp_32);
tmp_34 =  (C2_5_notw) * a[1].re ;
tmp_35 =  (C3_5_notw) * a[3].re ;
tmp_36 = (tmp_34 + tmp_35);
tmp_37 =  (C2_5_notw) * a[4].re ;
tmp_38 =  (C3_5_notw) * a[2].re ;
tmp_39 = (tmp_37 + tmp_38);
res[2].im = (tmp_33 + tmp_36 + a[0].im - tmp_39);
res[3].im = (tmp_33 + tmp_39 + a[0].im - tmp_36);
res[4].im = (tmp_24 + tmp_30 + a[0].im - tmp_27);


memory<complex<float16_t>>::store(in,0 * strideFactor , res[0]);
memory<complex<float16_t>>::store(in,1 * strideFactor , res[1]);
memory<complex<float16_t>>::store(in,2 * strideFactor , res[2]);
memory<complex<float16_t>>::store(in,3 * strideFactor , res[3]);
memory<complex<float16_t>>::store(in,4 * strideFactor , res[4]);


};

/* Nb Multiplications = 24, 
Nb Additions = 48,
Nb Negates = 12,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_last(const complex<float16_t> *in,complex<float16_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[5];
complex<float16_t> res[5];


 float16_t tmp_0;
 float16_t tmp_1;
 float16_t tmp_2;
 float16_t tmp_3;
 float16_t tmp_4;
 float16_t tmp_5;
 float16_t tmp_6;
 float16_t tmp_7;
 float16_t tmp_8;
 float16_t tmp_9;
 float16_t tmp_10;
 float16_t tmp_11;
 float16_t tmp_12;
 float16_t tmp_13;
 float16_t tmp_14;
 float16_t tmp_15;
 float16_t tmp_16;
 float16_t tmp_17;
 float16_t tmp_18;
 float16_t tmp_19;
 float16_t tmp_20;
 float16_t tmp_21;
 float16_t tmp_22;
 float16_t tmp_23;
 float16_t tmp_24;
 float16_t tmp_25;
 float16_t tmp_26;
 float16_t tmp_27;
 float16_t tmp_28;
 float16_t tmp_29;
 float16_t tmp_30;
 float16_t tmp_31;
 float16_t tmp_32;
 float16_t tmp_33;
 float16_t tmp_34;
 float16_t tmp_35;
 float16_t tmp_36;
 float16_t tmp_37;
 float16_t tmp_38;
 float16_t tmp_39;


a[0] = memory<complex<float16_t>>::load(in,0 * strideFactor);
a[1] = memory<complex<float16_t>>::load(in,1 * strideFactor);
a[2] = memory<complex<float16_t>>::load(in,2 * strideFactor);
a[3] = memory<complex<float16_t>>::load(in,3 * strideFactor);
a[4] = memory<complex<float16_t>>::load(in,4 * strideFactor);


res[0].re = (a[0].re + a[1].re + a[2].re + a[3].re + a[4].re);
tmp_0 = (a[1].re + a[4].re);
tmp_1 =  (C0_5_notw) * tmp_0 ;
tmp_2 = (a[2].re + a[3].re);
tmp_3 =  (C1_5_notw) * tmp_2 ;
tmp_4 = (tmp_1 - tmp_3);
tmp_5 =  (C2_5_notw) * a[3].im ;
tmp_6 =  (C3_5_notw) * a[4].im ;
tmp_7 = (tmp_5 + tmp_6);
tmp_8 =  (C2_5_notw) * a[2].im ;
tmp_9 =  (C3_5_notw) * a[1].im ;
tmp_10 = (tmp_8 + tmp_9);
res[1].re = (tmp_4 + tmp_7 + a[0].re - tmp_10);
tmp_11 =  (C0_5_notw) * tmp_2 ;
tmp_12 =  (C1_5_notw) * tmp_0 ;
tmp_13 = (tmp_11 - tmp_12);
tmp_14 =  (C2_5_notw) * a[4].im ;
tmp_15 =  (C3_5_notw) * a[2].im ;
tmp_16 = (tmp_14 + tmp_15);
tmp_17 =  (C2_5_notw) * a[1].im ;
tmp_18 =  (C3_5_notw) * a[3].im ;
tmp_19 = (tmp_17 + tmp_18);
res[2].re = (tmp_13 + tmp_16 + a[0].re - tmp_19);
res[3].re = (tmp_13 + tmp_19 + a[0].re - tmp_16);
res[4].re = (tmp_4 + tmp_10 + a[0].re - tmp_7);
res[0].im = (a[0].im + a[1].im + a[2].im + a[3].im + a[4].im);
tmp_20 = (a[1].im + a[4].im);
tmp_21 =  (C0_5_notw) * tmp_20 ;
tmp_22 = (a[2].im + a[3].im);
tmp_23 =  (C1_5_notw) * tmp_22 ;
tmp_24 = (tmp_21 - tmp_23);
tmp_25 =  (C2_5_notw) * a[2].re ;
tmp_26 =  (C3_5_notw) * a[1].re ;
tmp_27 = (tmp_25 + tmp_26);
tmp_28 =  (C2_5_notw) * a[3].re ;
tmp_29 =  (C3_5_notw) * a[4].re ;
tmp_30 = (tmp_28 + tmp_29);
res[1].im = (tmp_24 + tmp_27 + a[0].im - tmp_30);
tmp_31 =  (C0_5_notw) * tmp_22 ;
tmp_32 =  (C1_5_notw) * tmp_20 ;
tmp_33 = (tmp_31 - tmp_32);
tmp_34 =  (C2_5_notw) * a[1].re ;
tmp_35 =  (C3_5_notw) * a[3].re ;
tmp_36 = (tmp_34 + tmp_35);
tmp_37 =  (C2_5_notw) * a[4].re ;
tmp_38 =  (C3_5_notw) * a[2].re ;
tmp_39 = (tmp_37 + tmp_38);
res[2].im = (tmp_33 + tmp_36 + a[0].im - tmp_39);
res[3].im = (tmp_33 + tmp_39 + a[0].im - tmp_36);
res[4].im = (tmp_24 + tmp_30 + a[0].im - tmp_27);


memory<complex<float16_t>>::store(out,0 * strideFactor , res[0]);
memory<complex<float16_t>>::store(out,1 * strideFactor , res[1]);
memory<complex<float16_t>>::store(out,2 * strideFactor , res[2]);
memory<complex<float16_t>>::store(out,3 * strideFactor , res[3]);
memory<complex<float16_t>>::store(out,4 * strideFactor , res[4]);


};

};

#endif /* #ifndef _RADIX_F16_5_H */