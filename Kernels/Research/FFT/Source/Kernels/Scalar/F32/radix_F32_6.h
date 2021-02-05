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

#ifndef _RADIX_F32_6_H
#define _RADIX_F32_6_H

#include "complex.h"

/**************

DIRECT FFT 

***************/
template<int inputstrideenabled> 
struct radix<DIRECT,float32_t, 6,inputstrideenabled>{

static constexpr float32_t C0_6_tw{0.8660254037844386f};
  static constexpr float32_t C1_6_tw{0.5f};
  
static constexpr float32_t C0_6_notw{0.8660254037844386f};
  static constexpr float32_t C1_6_notw{0.5f};
  

/* Nb Multiplications = 20, 
Nb Additions = 52,
Nb Negates = 26,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_notw(int stageLength,complex<float32_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float32_t> a[6];
complex<float32_t> res[6];


 float32_t tmp_0;
 float32_t tmp_1;
 float32_t tmp_2;
 float32_t tmp_3;
 float32_t tmp_4;
 float32_t tmp_5;
 float32_t tmp_6;
 float32_t tmp_7;
 float32_t tmp_8;
 float32_t tmp_9;
 float32_t tmp_10;
 float32_t tmp_11;
 float32_t tmp_12;
 float32_t tmp_13;
 float32_t tmp_14;
 float32_t tmp_15;
 float32_t tmp_16;
 float32_t tmp_17;
 float32_t tmp_18;
 float32_t tmp_19;
 float32_t tmp_20;
 float32_t tmp_21;
 float32_t tmp_22;
 float32_t tmp_23;
 float32_t tmp_24;
 float32_t tmp_25;
 float32_t tmp_26;
 float32_t tmp_27;
 float32_t tmp_28;
 float32_t tmp_29;
 float32_t tmp_30;
 float32_t tmp_31;


a[0] = memory<complex<float32_t>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<float32_t>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<float32_t>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<float32_t>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<float32_t>>::load(in,4 * stageLength * strideFactor);
a[5] = memory<complex<float32_t>>::load(in,5 * stageLength * strideFactor);


tmp_0 = (a[0].re + a[3].re);
tmp_1 = (a[1].re + a[4].re);
tmp_2 = (a[2].re + a[5].re);
res[0].re = (tmp_0 + tmp_1 + tmp_2);
tmp_3 = (a[1].re - a[4].re);
tmp_4 =  (C1_6_notw) * tmp_3 ;
tmp_5 = (a[1].im - a[4].im);
tmp_6 =  (C0_6_notw) * tmp_5 ;
tmp_7 = (tmp_4 + tmp_6);
tmp_8 = (a[2].im - a[5].im);
tmp_9 =  (C0_6_notw) * tmp_8 ;
tmp_10 = (a[2].re - a[5].re);
tmp_11 =  (C1_6_notw) * tmp_10 ;
tmp_12 = (tmp_9 - tmp_11);
tmp_13 = (a[0].re - a[3].re);
res[1].re = (tmp_7 + tmp_12 + tmp_13);
tmp_14 = (a[1].im + a[4].im);
tmp_15 = (a[2].im + a[5].im);
tmp_16 = (tmp_1 + tmp_2);
tmp_17 =  (C1_6_notw) * tmp_16 ;
res[2].re = (tmp_0 +  (C0_6_notw) * (tmp_14 - tmp_15)  - tmp_17);
tmp_18 =  (C1_6_notw) * tmp_5 ;
tmp_19 =  (C0_6_notw) * tmp_3 ;
tmp_20 = (tmp_18 - tmp_19);
tmp_21 =  (C1_6_notw) * tmp_8 ;
tmp_22 =  (C0_6_notw) * tmp_10 ;
tmp_23 = ( (-(tmp_21))   - tmp_22);
tmp_24 = (tmp_7 + tmp_12);
tmp_25 =  (C1_6_notw) * tmp_24 ;
res[3].re = (tmp_13 +  (C0_6_notw) * (tmp_20 - tmp_23)  - tmp_25);
res[4].re = (tmp_0 +  (C0_6_notw) * (tmp_15 - tmp_14)  - tmp_17);
res[5].re = (tmp_13 +  (C0_6_notw) * (tmp_23 - tmp_20)  - tmp_25);
tmp_26 = (a[0].im + a[3].im);
res[0].im = (tmp_26 + tmp_14 + tmp_15);
tmp_27 = (a[0].im - a[3].im);
res[1].im = (tmp_20 + tmp_23 + tmp_27);
tmp_28 = (tmp_14 + tmp_15);
tmp_29 =  (C1_6_notw) * tmp_28 ;
res[2].im = (tmp_26 +  (C0_6_notw) * (tmp_2 - tmp_1)  - tmp_29);
tmp_30 = (tmp_20 + tmp_23);
tmp_31 =  (C1_6_notw) * tmp_30 ;
res[3].im = (tmp_27 +  (C0_6_notw) * (tmp_12 - tmp_7)  - tmp_31);
res[4].im = (tmp_26 +  (C0_6_notw) * (tmp_1 - tmp_2)  - tmp_29);
res[5].im = (tmp_27 +  (C0_6_notw) * (tmp_7 - tmp_12)  - tmp_31);


memory<complex<float32_t>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<float32_t>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<float32_t>>::store(in,2 * stageLength * strideFactor , res[2]);
memory<complex<float32_t>>::store(in,3 * stageLength * strideFactor , res[3]);
memory<complex<float32_t>>::store(in,4 * stageLength * strideFactor , res[4]);
memory<complex<float32_t>>::store(in,5 * stageLength * strideFactor , res[5]);


};

/* Nb Multiplications = 40, 
Nb Additions = 62,
Nb Negates = 31,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_tw(int stageLength,const complex<float32_t> *w,complex<float32_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float32_t> a[6];
complex<float32_t> res[6];


 float32_t tmp_0;
 float32_t tmp_1;
 float32_t tmp_2;
 float32_t tmp_3;
 float32_t tmp_4;
 float32_t tmp_5;
 float32_t tmp_6;
 float32_t tmp_7;
 float32_t tmp_8;
 float32_t tmp_9;
 float32_t tmp_10;
 float32_t tmp_11;
 float32_t tmp_12;
 float32_t tmp_13;
 float32_t tmp_14;
 float32_t tmp_15;
 float32_t tmp_16;
 float32_t tmp_17;
 float32_t tmp_18;
 float32_t tmp_19;
 float32_t tmp_20;
 float32_t tmp_21;
 float32_t tmp_22;
 float32_t tmp_23;
 float32_t tmp_24;
 float32_t tmp_25;
 float32_t tmp_26;
 float32_t tmp_27;
 float32_t tmp_28;
 float32_t tmp_29;
 float32_t tmp_30;
 float32_t tmp_31;
 float32_t tmp_32;
 float32_t tmp_33;
 float32_t tmp_34;
 float32_t tmp_35;
 float32_t tmp_36;
 float32_t tmp_37;
 float32_t tmp_38;
 float32_t tmp_39;
 float32_t tmp_40;
 float32_t tmp_41;
 float32_t tmp_42;
 float32_t tmp_43;
 float32_t tmp_44;
 float32_t tmp_45;
 float32_t tmp_46;
 float32_t tmp_47;
 float32_t tmp_48;
 float32_t tmp_49;
 float32_t tmp_50;
 float32_t tmp_51;
 float32_t tmp_52;
 float32_t tmp_53;
 float32_t tmp_54;
 float32_t tmp_55;
 float32_t tmp_56;
 float32_t tmp_57;


a[0] = memory<complex<float32_t>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<float32_t>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<float32_t>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<float32_t>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<float32_t>>::load(in,4 * stageLength * strideFactor);
a[5] = memory<complex<float32_t>>::load(in,5 * stageLength * strideFactor);


tmp_0 = (a[0].re + a[3].re);
tmp_1 = (a[1].re + a[4].re);
tmp_2 = (a[2].re + a[5].re);
res[0].re = (tmp_0 + tmp_1 + tmp_2);
tmp_3 = (a[1].re - a[4].re);
tmp_4 =  (C1_6_tw) * tmp_3 ;
tmp_5 = (a[1].im - a[4].im);
tmp_6 =  (C0_6_tw) * tmp_5 ;
tmp_7 = (tmp_4 + tmp_6);
tmp_8 = (a[2].im - a[5].im);
tmp_9 =  (C0_6_tw) * tmp_8 ;
tmp_10 = (a[2].re - a[5].re);
tmp_11 =  (C1_6_tw) * tmp_10 ;
tmp_12 = (tmp_9 - tmp_11);
tmp_13 = (a[0].re - a[3].re);
tmp_14 = (tmp_7 + tmp_12 + tmp_13);
tmp_15 =  (C1_6_tw) * tmp_5 ;
tmp_16 =  (C0_6_tw) * tmp_3 ;
tmp_17 = (tmp_15 - tmp_16);
tmp_18 =  (C1_6_tw) * tmp_8 ;
tmp_19 =  (C0_6_tw) * tmp_10 ;
tmp_20 = ( (-(tmp_18))   - tmp_19);
tmp_21 = (a[0].im - a[3].im);
tmp_22 = (tmp_17 + tmp_20 + tmp_21);
res[1].re = ( w[0].re * tmp_14  -  w[0].im * tmp_22 );
tmp_23 = (a[1].im + a[4].im);
tmp_24 = (a[2].im + a[5].im);
tmp_25 = (tmp_23 - tmp_24);
tmp_26 =  (C0_6_tw) * tmp_25 ;
tmp_27 = (tmp_1 + tmp_2);
tmp_28 =  (C1_6_tw) * tmp_27 ;
tmp_29 = (tmp_0 + tmp_26 - tmp_28);
tmp_30 = (a[0].im + a[3].im);
tmp_31 = (tmp_2 - tmp_1);
tmp_32 =  (C0_6_tw) * tmp_31 ;
tmp_33 = (tmp_23 + tmp_24);
tmp_34 =  (C1_6_tw) * tmp_33 ;
tmp_35 = (tmp_30 + tmp_32 - tmp_34);
res[2].re = ( w[1].re * tmp_29  -  w[1].im * tmp_35 );
tmp_36 = (tmp_17 - tmp_20);
tmp_37 =  (C0_6_tw) * tmp_36 ;
tmp_38 = (tmp_7 + tmp_12);
tmp_39 =  (C1_6_tw) * tmp_38 ;
tmp_40 = (tmp_13 + tmp_37 - tmp_39);
tmp_41 = (tmp_12 - tmp_7);
tmp_42 =  (C0_6_tw) * tmp_41 ;
tmp_43 = (tmp_17 + tmp_20);
tmp_44 =  (C1_6_tw) * tmp_43 ;
tmp_45 = (tmp_21 + tmp_42 - tmp_44);
res[3].re = ( w[2].re * tmp_40  -  w[2].im * tmp_45 );
tmp_46 = (tmp_24 - tmp_23);
tmp_47 =  (C0_6_tw) * tmp_46 ;
tmp_48 = (tmp_0 + tmp_47 - tmp_28);
tmp_49 = (tmp_1 - tmp_2);
tmp_50 =  (C0_6_tw) * tmp_49 ;
tmp_51 = (tmp_30 + tmp_50 - tmp_34);
res[4].re = ( w[3].re * tmp_48  -  w[3].im * tmp_51 );
tmp_52 = (tmp_20 - tmp_17);
tmp_53 =  (C0_6_tw) * tmp_52 ;
tmp_54 = (tmp_13 + tmp_53 - tmp_39);
tmp_55 = (tmp_7 - tmp_12);
tmp_56 =  (C0_6_tw) * tmp_55 ;
tmp_57 = (tmp_21 + tmp_56 - tmp_44);
res[5].re = ( w[4].re * tmp_54  -  w[4].im * tmp_57 );
res[0].im = (tmp_30 + tmp_23 + tmp_24);
res[1].im = ( w[0].re * tmp_22  +  w[0].im * tmp_14 );
res[2].im = ( w[1].re * tmp_35  +  w[1].im * tmp_29 );
res[3].im = ( w[2].re * tmp_45  +  w[2].im * tmp_40 );
res[4].im = ( w[3].re * tmp_51  +  w[3].im * tmp_48 );
res[5].im = ( w[4].re * tmp_57  +  w[4].im * tmp_54 );


memory<complex<float32_t>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<float32_t>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<float32_t>>::store(in,2 * stageLength * strideFactor , res[2]);
memory<complex<float32_t>>::store(in,3 * stageLength * strideFactor , res[3]);
memory<complex<float32_t>>::store(in,4 * stageLength * strideFactor , res[4]);
memory<complex<float32_t>>::store(in,5 * stageLength * strideFactor , res[5]);


};

/* Nb Multiplications = 20, 
Nb Additions = 52,
Nb Negates = 26,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_notw(int stageLength,const complex<float32_t> *in,complex<float32_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float32_t> a[6];
complex<float32_t> res[6];


 float32_t tmp_0;
 float32_t tmp_1;
 float32_t tmp_2;
 float32_t tmp_3;
 float32_t tmp_4;
 float32_t tmp_5;
 float32_t tmp_6;
 float32_t tmp_7;
 float32_t tmp_8;
 float32_t tmp_9;
 float32_t tmp_10;
 float32_t tmp_11;
 float32_t tmp_12;
 float32_t tmp_13;
 float32_t tmp_14;
 float32_t tmp_15;
 float32_t tmp_16;
 float32_t tmp_17;
 float32_t tmp_18;
 float32_t tmp_19;
 float32_t tmp_20;
 float32_t tmp_21;
 float32_t tmp_22;
 float32_t tmp_23;
 float32_t tmp_24;
 float32_t tmp_25;
 float32_t tmp_26;
 float32_t tmp_27;
 float32_t tmp_28;
 float32_t tmp_29;
 float32_t tmp_30;
 float32_t tmp_31;


a[0] = memory<complex<float32_t>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<float32_t>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<float32_t>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<float32_t>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<float32_t>>::load(in,4 * stageLength * strideFactor);
a[5] = memory<complex<float32_t>>::load(in,5 * stageLength * strideFactor);


tmp_0 = (a[0].re + a[3].re);
tmp_1 = (a[1].re + a[4].re);
tmp_2 = (a[2].re + a[5].re);
res[0].re = (tmp_0 + tmp_1 + tmp_2);
tmp_3 = (a[1].re - a[4].re);
tmp_4 =  (C1_6_notw) * tmp_3 ;
tmp_5 = (a[1].im - a[4].im);
tmp_6 =  (C0_6_notw) * tmp_5 ;
tmp_7 = (tmp_4 + tmp_6);
tmp_8 = (a[2].im - a[5].im);
tmp_9 =  (C0_6_notw) * tmp_8 ;
tmp_10 = (a[2].re - a[5].re);
tmp_11 =  (C1_6_notw) * tmp_10 ;
tmp_12 = (tmp_9 - tmp_11);
tmp_13 = (a[0].re - a[3].re);
res[1].re = (tmp_7 + tmp_12 + tmp_13);
tmp_14 = (a[1].im + a[4].im);
tmp_15 = (a[2].im + a[5].im);
tmp_16 = (tmp_1 + tmp_2);
tmp_17 =  (C1_6_notw) * tmp_16 ;
res[2].re = (tmp_0 +  (C0_6_notw) * (tmp_14 - tmp_15)  - tmp_17);
tmp_18 =  (C1_6_notw) * tmp_5 ;
tmp_19 =  (C0_6_notw) * tmp_3 ;
tmp_20 = (tmp_18 - tmp_19);
tmp_21 =  (C1_6_notw) * tmp_8 ;
tmp_22 =  (C0_6_notw) * tmp_10 ;
tmp_23 = ( (-(tmp_21))   - tmp_22);
tmp_24 = (tmp_7 + tmp_12);
tmp_25 =  (C1_6_notw) * tmp_24 ;
res[3].re = (tmp_13 +  (C0_6_notw) * (tmp_20 - tmp_23)  - tmp_25);
res[4].re = (tmp_0 +  (C0_6_notw) * (tmp_15 - tmp_14)  - tmp_17);
res[5].re = (tmp_13 +  (C0_6_notw) * (tmp_23 - tmp_20)  - tmp_25);
tmp_26 = (a[0].im + a[3].im);
res[0].im = (tmp_26 + tmp_14 + tmp_15);
tmp_27 = (a[0].im - a[3].im);
res[1].im = (tmp_20 + tmp_23 + tmp_27);
tmp_28 = (tmp_14 + tmp_15);
tmp_29 =  (C1_6_notw) * tmp_28 ;
res[2].im = (tmp_26 +  (C0_6_notw) * (tmp_2 - tmp_1)  - tmp_29);
tmp_30 = (tmp_20 + tmp_23);
tmp_31 =  (C1_6_notw) * tmp_30 ;
res[3].im = (tmp_27 +  (C0_6_notw) * (tmp_12 - tmp_7)  - tmp_31);
res[4].im = (tmp_26 +  (C0_6_notw) * (tmp_1 - tmp_2)  - tmp_29);
res[5].im = (tmp_27 +  (C0_6_notw) * (tmp_7 - tmp_12)  - tmp_31);


memory<complex<float32_t>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<float32_t>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<float32_t>>::store(out,2 * stageLength * strideFactor , res[2]);
memory<complex<float32_t>>::store(out,3 * stageLength * strideFactor , res[3]);
memory<complex<float32_t>>::store(out,4 * stageLength * strideFactor , res[4]);
memory<complex<float32_t>>::store(out,5 * stageLength * strideFactor , res[5]);


};

/* Nb Multiplications = 40, 
Nb Additions = 62,
Nb Negates = 31,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_tw(int stageLength,const complex<float32_t> *w,const complex<float32_t> *in,complex<float32_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float32_t> a[6];
complex<float32_t> res[6];


 float32_t tmp_0;
 float32_t tmp_1;
 float32_t tmp_2;
 float32_t tmp_3;
 float32_t tmp_4;
 float32_t tmp_5;
 float32_t tmp_6;
 float32_t tmp_7;
 float32_t tmp_8;
 float32_t tmp_9;
 float32_t tmp_10;
 float32_t tmp_11;
 float32_t tmp_12;
 float32_t tmp_13;
 float32_t tmp_14;
 float32_t tmp_15;
 float32_t tmp_16;
 float32_t tmp_17;
 float32_t tmp_18;
 float32_t tmp_19;
 float32_t tmp_20;
 float32_t tmp_21;
 float32_t tmp_22;
 float32_t tmp_23;
 float32_t tmp_24;
 float32_t tmp_25;
 float32_t tmp_26;
 float32_t tmp_27;
 float32_t tmp_28;
 float32_t tmp_29;
 float32_t tmp_30;
 float32_t tmp_31;
 float32_t tmp_32;
 float32_t tmp_33;
 float32_t tmp_34;
 float32_t tmp_35;
 float32_t tmp_36;
 float32_t tmp_37;
 float32_t tmp_38;
 float32_t tmp_39;
 float32_t tmp_40;
 float32_t tmp_41;
 float32_t tmp_42;
 float32_t tmp_43;
 float32_t tmp_44;
 float32_t tmp_45;
 float32_t tmp_46;
 float32_t tmp_47;
 float32_t tmp_48;
 float32_t tmp_49;
 float32_t tmp_50;
 float32_t tmp_51;
 float32_t tmp_52;
 float32_t tmp_53;
 float32_t tmp_54;
 float32_t tmp_55;
 float32_t tmp_56;
 float32_t tmp_57;


a[0] = memory<complex<float32_t>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<float32_t>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<float32_t>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<float32_t>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<float32_t>>::load(in,4 * stageLength * strideFactor);
a[5] = memory<complex<float32_t>>::load(in,5 * stageLength * strideFactor);


tmp_0 = (a[0].re + a[3].re);
tmp_1 = (a[1].re + a[4].re);
tmp_2 = (a[2].re + a[5].re);
res[0].re = (tmp_0 + tmp_1 + tmp_2);
tmp_3 = (a[1].re - a[4].re);
tmp_4 =  (C1_6_tw) * tmp_3 ;
tmp_5 = (a[1].im - a[4].im);
tmp_6 =  (C0_6_tw) * tmp_5 ;
tmp_7 = (tmp_4 + tmp_6);
tmp_8 = (a[2].im - a[5].im);
tmp_9 =  (C0_6_tw) * tmp_8 ;
tmp_10 = (a[2].re - a[5].re);
tmp_11 =  (C1_6_tw) * tmp_10 ;
tmp_12 = (tmp_9 - tmp_11);
tmp_13 = (a[0].re - a[3].re);
tmp_14 = (tmp_7 + tmp_12 + tmp_13);
tmp_15 =  (C1_6_tw) * tmp_5 ;
tmp_16 =  (C0_6_tw) * tmp_3 ;
tmp_17 = (tmp_15 - tmp_16);
tmp_18 =  (C1_6_tw) * tmp_8 ;
tmp_19 =  (C0_6_tw) * tmp_10 ;
tmp_20 = ( (-(tmp_18))   - tmp_19);
tmp_21 = (a[0].im - a[3].im);
tmp_22 = (tmp_17 + tmp_20 + tmp_21);
res[1].re = ( w[0].re * tmp_14  -  w[0].im * tmp_22 );
tmp_23 = (a[1].im + a[4].im);
tmp_24 = (a[2].im + a[5].im);
tmp_25 = (tmp_23 - tmp_24);
tmp_26 =  (C0_6_tw) * tmp_25 ;
tmp_27 = (tmp_1 + tmp_2);
tmp_28 =  (C1_6_tw) * tmp_27 ;
tmp_29 = (tmp_0 + tmp_26 - tmp_28);
tmp_30 = (a[0].im + a[3].im);
tmp_31 = (tmp_2 - tmp_1);
tmp_32 =  (C0_6_tw) * tmp_31 ;
tmp_33 = (tmp_23 + tmp_24);
tmp_34 =  (C1_6_tw) * tmp_33 ;
tmp_35 = (tmp_30 + tmp_32 - tmp_34);
res[2].re = ( w[1].re * tmp_29  -  w[1].im * tmp_35 );
tmp_36 = (tmp_17 - tmp_20);
tmp_37 =  (C0_6_tw) * tmp_36 ;
tmp_38 = (tmp_7 + tmp_12);
tmp_39 =  (C1_6_tw) * tmp_38 ;
tmp_40 = (tmp_13 + tmp_37 - tmp_39);
tmp_41 = (tmp_12 - tmp_7);
tmp_42 =  (C0_6_tw) * tmp_41 ;
tmp_43 = (tmp_17 + tmp_20);
tmp_44 =  (C1_6_tw) * tmp_43 ;
tmp_45 = (tmp_21 + tmp_42 - tmp_44);
res[3].re = ( w[2].re * tmp_40  -  w[2].im * tmp_45 );
tmp_46 = (tmp_24 - tmp_23);
tmp_47 =  (C0_6_tw) * tmp_46 ;
tmp_48 = (tmp_0 + tmp_47 - tmp_28);
tmp_49 = (tmp_1 - tmp_2);
tmp_50 =  (C0_6_tw) * tmp_49 ;
tmp_51 = (tmp_30 + tmp_50 - tmp_34);
res[4].re = ( w[3].re * tmp_48  -  w[3].im * tmp_51 );
tmp_52 = (tmp_20 - tmp_17);
tmp_53 =  (C0_6_tw) * tmp_52 ;
tmp_54 = (tmp_13 + tmp_53 - tmp_39);
tmp_55 = (tmp_7 - tmp_12);
tmp_56 =  (C0_6_tw) * tmp_55 ;
tmp_57 = (tmp_21 + tmp_56 - tmp_44);
res[5].re = ( w[4].re * tmp_54  -  w[4].im * tmp_57 );
res[0].im = (tmp_30 + tmp_23 + tmp_24);
res[1].im = ( w[0].re * tmp_22  +  w[0].im * tmp_14 );
res[2].im = ( w[1].re * tmp_35  +  w[1].im * tmp_29 );
res[3].im = ( w[2].re * tmp_45  +  w[2].im * tmp_40 );
res[4].im = ( w[3].re * tmp_51  +  w[3].im * tmp_48 );
res[5].im = ( w[4].re * tmp_57  +  w[4].im * tmp_54 );


memory<complex<float32_t>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<float32_t>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<float32_t>>::store(out,2 * stageLength * strideFactor , res[2]);
memory<complex<float32_t>>::store(out,3 * stageLength * strideFactor , res[3]);
memory<complex<float32_t>>::store(out,4 * stageLength * strideFactor , res[4]);
memory<complex<float32_t>>::store(out,5 * stageLength * strideFactor , res[5]);


};

/* Nb Multiplications = 20, 
Nb Additions = 52,
Nb Negates = 26,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_last(complex<float32_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float32_t> a[6];
complex<float32_t> res[6];


 float32_t tmp_0;
 float32_t tmp_1;
 float32_t tmp_2;
 float32_t tmp_3;
 float32_t tmp_4;
 float32_t tmp_5;
 float32_t tmp_6;
 float32_t tmp_7;
 float32_t tmp_8;
 float32_t tmp_9;
 float32_t tmp_10;
 float32_t tmp_11;
 float32_t tmp_12;
 float32_t tmp_13;
 float32_t tmp_14;
 float32_t tmp_15;
 float32_t tmp_16;
 float32_t tmp_17;
 float32_t tmp_18;
 float32_t tmp_19;
 float32_t tmp_20;
 float32_t tmp_21;
 float32_t tmp_22;
 float32_t tmp_23;
 float32_t tmp_24;
 float32_t tmp_25;
 float32_t tmp_26;
 float32_t tmp_27;
 float32_t tmp_28;
 float32_t tmp_29;
 float32_t tmp_30;
 float32_t tmp_31;


a[0] = memory<complex<float32_t>>::load(in,0 * strideFactor);
a[1] = memory<complex<float32_t>>::load(in,1 * strideFactor);
a[2] = memory<complex<float32_t>>::load(in,2 * strideFactor);
a[3] = memory<complex<float32_t>>::load(in,3 * strideFactor);
a[4] = memory<complex<float32_t>>::load(in,4 * strideFactor);
a[5] = memory<complex<float32_t>>::load(in,5 * strideFactor);


tmp_0 = (a[0].re + a[3].re);
tmp_1 = (a[1].re + a[4].re);
tmp_2 = (a[2].re + a[5].re);
res[0].re = (tmp_0 + tmp_1 + tmp_2);
tmp_3 = (a[1].re - a[4].re);
tmp_4 =  (C1_6_notw) * tmp_3 ;
tmp_5 = (a[1].im - a[4].im);
tmp_6 =  (C0_6_notw) * tmp_5 ;
tmp_7 = (tmp_4 + tmp_6);
tmp_8 = (a[2].im - a[5].im);
tmp_9 =  (C0_6_notw) * tmp_8 ;
tmp_10 = (a[2].re - a[5].re);
tmp_11 =  (C1_6_notw) * tmp_10 ;
tmp_12 = (tmp_9 - tmp_11);
tmp_13 = (a[0].re - a[3].re);
res[1].re = (tmp_7 + tmp_12 + tmp_13);
tmp_14 = (a[1].im + a[4].im);
tmp_15 = (a[2].im + a[5].im);
tmp_16 = (tmp_1 + tmp_2);
tmp_17 =  (C1_6_notw) * tmp_16 ;
res[2].re = (tmp_0 +  (C0_6_notw) * (tmp_14 - tmp_15)  - tmp_17);
tmp_18 =  (C1_6_notw) * tmp_5 ;
tmp_19 =  (C0_6_notw) * tmp_3 ;
tmp_20 = (tmp_18 - tmp_19);
tmp_21 =  (C1_6_notw) * tmp_8 ;
tmp_22 =  (C0_6_notw) * tmp_10 ;
tmp_23 = ( (-(tmp_21))   - tmp_22);
tmp_24 = (tmp_7 + tmp_12);
tmp_25 =  (C1_6_notw) * tmp_24 ;
res[3].re = (tmp_13 +  (C0_6_notw) * (tmp_20 - tmp_23)  - tmp_25);
res[4].re = (tmp_0 +  (C0_6_notw) * (tmp_15 - tmp_14)  - tmp_17);
res[5].re = (tmp_13 +  (C0_6_notw) * (tmp_23 - tmp_20)  - tmp_25);
tmp_26 = (a[0].im + a[3].im);
res[0].im = (tmp_26 + tmp_14 + tmp_15);
tmp_27 = (a[0].im - a[3].im);
res[1].im = (tmp_20 + tmp_23 + tmp_27);
tmp_28 = (tmp_14 + tmp_15);
tmp_29 =  (C1_6_notw) * tmp_28 ;
res[2].im = (tmp_26 +  (C0_6_notw) * (tmp_2 - tmp_1)  - tmp_29);
tmp_30 = (tmp_20 + tmp_23);
tmp_31 =  (C1_6_notw) * tmp_30 ;
res[3].im = (tmp_27 +  (C0_6_notw) * (tmp_12 - tmp_7)  - tmp_31);
res[4].im = (tmp_26 +  (C0_6_notw) * (tmp_1 - tmp_2)  - tmp_29);
res[5].im = (tmp_27 +  (C0_6_notw) * (tmp_7 - tmp_12)  - tmp_31);


memory<complex<float32_t>>::store(in,0 * strideFactor , res[0]);
memory<complex<float32_t>>::store(in,1 * strideFactor , res[1]);
memory<complex<float32_t>>::store(in,2 * strideFactor , res[2]);
memory<complex<float32_t>>::store(in,3 * strideFactor , res[3]);
memory<complex<float32_t>>::store(in,4 * strideFactor , res[4]);
memory<complex<float32_t>>::store(in,5 * strideFactor , res[5]);


};

/* Nb Multiplications = 20, 
Nb Additions = 52,
Nb Negates = 26,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_last(const complex<float32_t> *in,complex<float32_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float32_t> a[6];
complex<float32_t> res[6];


 float32_t tmp_0;
 float32_t tmp_1;
 float32_t tmp_2;
 float32_t tmp_3;
 float32_t tmp_4;
 float32_t tmp_5;
 float32_t tmp_6;
 float32_t tmp_7;
 float32_t tmp_8;
 float32_t tmp_9;
 float32_t tmp_10;
 float32_t tmp_11;
 float32_t tmp_12;
 float32_t tmp_13;
 float32_t tmp_14;
 float32_t tmp_15;
 float32_t tmp_16;
 float32_t tmp_17;
 float32_t tmp_18;
 float32_t tmp_19;
 float32_t tmp_20;
 float32_t tmp_21;
 float32_t tmp_22;
 float32_t tmp_23;
 float32_t tmp_24;
 float32_t tmp_25;
 float32_t tmp_26;
 float32_t tmp_27;
 float32_t tmp_28;
 float32_t tmp_29;
 float32_t tmp_30;
 float32_t tmp_31;


a[0] = memory<complex<float32_t>>::load(in,0 * strideFactor);
a[1] = memory<complex<float32_t>>::load(in,1 * strideFactor);
a[2] = memory<complex<float32_t>>::load(in,2 * strideFactor);
a[3] = memory<complex<float32_t>>::load(in,3 * strideFactor);
a[4] = memory<complex<float32_t>>::load(in,4 * strideFactor);
a[5] = memory<complex<float32_t>>::load(in,5 * strideFactor);


tmp_0 = (a[0].re + a[3].re);
tmp_1 = (a[1].re + a[4].re);
tmp_2 = (a[2].re + a[5].re);
res[0].re = (tmp_0 + tmp_1 + tmp_2);
tmp_3 = (a[1].re - a[4].re);
tmp_4 =  (C1_6_notw) * tmp_3 ;
tmp_5 = (a[1].im - a[4].im);
tmp_6 =  (C0_6_notw) * tmp_5 ;
tmp_7 = (tmp_4 + tmp_6);
tmp_8 = (a[2].im - a[5].im);
tmp_9 =  (C0_6_notw) * tmp_8 ;
tmp_10 = (a[2].re - a[5].re);
tmp_11 =  (C1_6_notw) * tmp_10 ;
tmp_12 = (tmp_9 - tmp_11);
tmp_13 = (a[0].re - a[3].re);
res[1].re = (tmp_7 + tmp_12 + tmp_13);
tmp_14 = (a[1].im + a[4].im);
tmp_15 = (a[2].im + a[5].im);
tmp_16 = (tmp_1 + tmp_2);
tmp_17 =  (C1_6_notw) * tmp_16 ;
res[2].re = (tmp_0 +  (C0_6_notw) * (tmp_14 - tmp_15)  - tmp_17);
tmp_18 =  (C1_6_notw) * tmp_5 ;
tmp_19 =  (C0_6_notw) * tmp_3 ;
tmp_20 = (tmp_18 - tmp_19);
tmp_21 =  (C1_6_notw) * tmp_8 ;
tmp_22 =  (C0_6_notw) * tmp_10 ;
tmp_23 = ( (-(tmp_21))   - tmp_22);
tmp_24 = (tmp_7 + tmp_12);
tmp_25 =  (C1_6_notw) * tmp_24 ;
res[3].re = (tmp_13 +  (C0_6_notw) * (tmp_20 - tmp_23)  - tmp_25);
res[4].re = (tmp_0 +  (C0_6_notw) * (tmp_15 - tmp_14)  - tmp_17);
res[5].re = (tmp_13 +  (C0_6_notw) * (tmp_23 - tmp_20)  - tmp_25);
tmp_26 = (a[0].im + a[3].im);
res[0].im = (tmp_26 + tmp_14 + tmp_15);
tmp_27 = (a[0].im - a[3].im);
res[1].im = (tmp_20 + tmp_23 + tmp_27);
tmp_28 = (tmp_14 + tmp_15);
tmp_29 =  (C1_6_notw) * tmp_28 ;
res[2].im = (tmp_26 +  (C0_6_notw) * (tmp_2 - tmp_1)  - tmp_29);
tmp_30 = (tmp_20 + tmp_23);
tmp_31 =  (C1_6_notw) * tmp_30 ;
res[3].im = (tmp_27 +  (C0_6_notw) * (tmp_12 - tmp_7)  - tmp_31);
res[4].im = (tmp_26 +  (C0_6_notw) * (tmp_1 - tmp_2)  - tmp_29);
res[5].im = (tmp_27 +  (C0_6_notw) * (tmp_7 - tmp_12)  - tmp_31);


memory<complex<float32_t>>::store(out,0 * strideFactor , res[0]);
memory<complex<float32_t>>::store(out,1 * strideFactor , res[1]);
memory<complex<float32_t>>::store(out,2 * strideFactor , res[2]);
memory<complex<float32_t>>::store(out,3 * strideFactor , res[3]);
memory<complex<float32_t>>::store(out,4 * strideFactor , res[4]);
memory<complex<float32_t>>::store(out,5 * strideFactor , res[5]);


};

};

/**************

INVERSE FFT 

***************/
template<int inputstrideenabled> 
struct radix<INVERSE,float32_t, 6,inputstrideenabled>{

static constexpr float32_t C0_6_tw{0.8660254037844386f};
  static constexpr float32_t C1_6_tw{0.5f};
  
static constexpr float32_t C0_6_notw{0.8660254037844386f};
  static constexpr float32_t C1_6_notw{0.5f};
  

/* Nb Multiplications = 20, 
Nb Additions = 52,
Nb Negates = 26,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_notw(int stageLength,complex<float32_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float32_t> a[6];
complex<float32_t> res[6];


 float32_t tmp_0;
 float32_t tmp_1;
 float32_t tmp_2;
 float32_t tmp_3;
 float32_t tmp_4;
 float32_t tmp_5;
 float32_t tmp_6;
 float32_t tmp_7;
 float32_t tmp_8;
 float32_t tmp_9;
 float32_t tmp_10;
 float32_t tmp_11;
 float32_t tmp_12;
 float32_t tmp_13;
 float32_t tmp_14;
 float32_t tmp_15;
 float32_t tmp_16;
 float32_t tmp_17;
 float32_t tmp_18;
 float32_t tmp_19;
 float32_t tmp_20;
 float32_t tmp_21;
 float32_t tmp_22;
 float32_t tmp_23;
 float32_t tmp_24;
 float32_t tmp_25;
 float32_t tmp_26;
 float32_t tmp_27;
 float32_t tmp_28;
 float32_t tmp_29;
 float32_t tmp_30;
 float32_t tmp_31;


a[0] = memory<complex<float32_t>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<float32_t>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<float32_t>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<float32_t>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<float32_t>>::load(in,4 * stageLength * strideFactor);
a[5] = memory<complex<float32_t>>::load(in,5 * stageLength * strideFactor);


tmp_0 = (a[0].re + a[3].re);
tmp_1 = (a[1].re + a[4].re);
tmp_2 = (a[2].re + a[5].re);
res[0].re = (tmp_0 + tmp_1 + tmp_2);
tmp_3 = (a[1].re - a[4].re);
tmp_4 =  (C1_6_notw) * tmp_3 ;
tmp_5 = (a[1].im - a[4].im);
tmp_6 =  (C0_6_notw) * tmp_5 ;
tmp_7 = (tmp_4 - tmp_6);
tmp_8 = (a[2].re - a[5].re);
tmp_9 =  (C1_6_notw) * tmp_8 ;
tmp_10 = (a[2].im - a[5].im);
tmp_11 =  (C0_6_notw) * tmp_10 ;
tmp_12 = ( (-(tmp_9))   - tmp_11);
tmp_13 = (a[0].re - a[3].re);
res[1].re = (tmp_7 + tmp_12 + tmp_13);
tmp_14 = (a[2].im + a[5].im);
tmp_15 = (a[1].im + a[4].im);
tmp_16 = (tmp_1 + tmp_2);
tmp_17 =  (C1_6_notw) * tmp_16 ;
res[2].re = (tmp_0 +  (C0_6_notw) * (tmp_14 - tmp_15)  - tmp_17);
tmp_18 =  (C0_6_notw) * tmp_8 ;
tmp_19 =  (C1_6_notw) * tmp_10 ;
tmp_20 = (tmp_18 - tmp_19);
tmp_21 =  (C1_6_notw) * tmp_5 ;
tmp_22 =  (C0_6_notw) * tmp_3 ;
tmp_23 = (tmp_21 + tmp_22);
tmp_24 = (tmp_7 + tmp_12);
tmp_25 =  (C1_6_notw) * tmp_24 ;
res[3].re = (tmp_13 +  (C0_6_notw) * (tmp_20 - tmp_23)  - tmp_25);
res[4].re = (tmp_0 +  (C0_6_notw) * (tmp_15 - tmp_14)  - tmp_17);
res[5].re = (tmp_13 +  (C0_6_notw) * (tmp_23 - tmp_20)  - tmp_25);
tmp_26 = (a[0].im + a[3].im);
res[0].im = (tmp_26 + tmp_15 + tmp_14);
tmp_27 = (a[0].im - a[3].im);
res[1].im = (tmp_23 + tmp_20 + tmp_27);
tmp_28 = (tmp_15 + tmp_14);
tmp_29 =  (C1_6_notw) * tmp_28 ;
res[2].im = (tmp_26 +  (C0_6_notw) * (tmp_1 - tmp_2)  - tmp_29);
tmp_30 = (tmp_23 + tmp_20);
tmp_31 =  (C1_6_notw) * tmp_30 ;
res[3].im = (tmp_27 +  (C0_6_notw) * (tmp_7 - tmp_12)  - tmp_31);
res[4].im = (tmp_26 +  (C0_6_notw) * (tmp_2 - tmp_1)  - tmp_29);
res[5].im = (tmp_27 +  (C0_6_notw) * (tmp_12 - tmp_7)  - tmp_31);


memory<complex<float32_t>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<float32_t>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<float32_t>>::store(in,2 * stageLength * strideFactor , res[2]);
memory<complex<float32_t>>::store(in,3 * stageLength * strideFactor , res[3]);
memory<complex<float32_t>>::store(in,4 * stageLength * strideFactor , res[4]);
memory<complex<float32_t>>::store(in,5 * stageLength * strideFactor , res[5]);


};

/* Nb Multiplications = 40, 
Nb Additions = 62,
Nb Negates = 31,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_tw(int stageLength,const complex<float32_t> *w,complex<float32_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float32_t> a[6];
complex<float32_t> res[6];


 float32_t tmp_0;
 float32_t tmp_1;
 float32_t tmp_2;
 float32_t tmp_3;
 float32_t tmp_4;
 float32_t tmp_5;
 float32_t tmp_6;
 float32_t tmp_7;
 float32_t tmp_8;
 float32_t tmp_9;
 float32_t tmp_10;
 float32_t tmp_11;
 float32_t tmp_12;
 float32_t tmp_13;
 float32_t tmp_14;
 float32_t tmp_15;
 float32_t tmp_16;
 float32_t tmp_17;
 float32_t tmp_18;
 float32_t tmp_19;
 float32_t tmp_20;
 float32_t tmp_21;
 float32_t tmp_22;
 float32_t tmp_23;
 float32_t tmp_24;
 float32_t tmp_25;
 float32_t tmp_26;
 float32_t tmp_27;
 float32_t tmp_28;
 float32_t tmp_29;
 float32_t tmp_30;
 float32_t tmp_31;
 float32_t tmp_32;
 float32_t tmp_33;
 float32_t tmp_34;
 float32_t tmp_35;
 float32_t tmp_36;
 float32_t tmp_37;
 float32_t tmp_38;
 float32_t tmp_39;
 float32_t tmp_40;
 float32_t tmp_41;
 float32_t tmp_42;
 float32_t tmp_43;
 float32_t tmp_44;
 float32_t tmp_45;
 float32_t tmp_46;
 float32_t tmp_47;
 float32_t tmp_48;
 float32_t tmp_49;
 float32_t tmp_50;
 float32_t tmp_51;
 float32_t tmp_52;
 float32_t tmp_53;
 float32_t tmp_54;
 float32_t tmp_55;
 float32_t tmp_56;
 float32_t tmp_57;


a[0] = memory<complex<float32_t>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<float32_t>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<float32_t>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<float32_t>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<float32_t>>::load(in,4 * stageLength * strideFactor);
a[5] = memory<complex<float32_t>>::load(in,5 * stageLength * strideFactor);


tmp_0 = (a[0].re + a[3].re);
tmp_1 = (a[1].re + a[4].re);
tmp_2 = (a[2].re + a[5].re);
res[0].re = (tmp_0 + tmp_1 + tmp_2);
tmp_3 = (a[1].re - a[4].re);
tmp_4 =  (C1_6_tw) * tmp_3 ;
tmp_5 = (a[1].im - a[4].im);
tmp_6 =  (C0_6_tw) * tmp_5 ;
tmp_7 = (tmp_4 - tmp_6);
tmp_8 = (a[2].re - a[5].re);
tmp_9 =  (C1_6_tw) * tmp_8 ;
tmp_10 = (a[2].im - a[5].im);
tmp_11 =  (C0_6_tw) * tmp_10 ;
tmp_12 = ( (-(tmp_9))   - tmp_11);
tmp_13 = (a[0].re - a[3].re);
tmp_14 = (tmp_7 + tmp_12 + tmp_13);
tmp_15 =  (C1_6_tw) * tmp_5 ;
tmp_16 =  (C0_6_tw) * tmp_3 ;
tmp_17 = (tmp_15 + tmp_16);
tmp_18 =  (C0_6_tw) * tmp_8 ;
tmp_19 =  (C1_6_tw) * tmp_10 ;
tmp_20 = (tmp_18 - tmp_19);
tmp_21 = (a[0].im - a[3].im);
tmp_22 = (tmp_17 + tmp_20 + tmp_21);
res[1].re = ( w[0].re * tmp_14  +  w[0].im * tmp_22 );
tmp_23 = (a[2].im + a[5].im);
tmp_24 = (a[1].im + a[4].im);
tmp_25 = (tmp_23 - tmp_24);
tmp_26 =  (C0_6_tw) * tmp_25 ;
tmp_27 = (tmp_1 + tmp_2);
tmp_28 =  (C1_6_tw) * tmp_27 ;
tmp_29 = (tmp_0 + tmp_26 - tmp_28);
tmp_30 = (a[0].im + a[3].im);
tmp_31 = (tmp_1 - tmp_2);
tmp_32 =  (C0_6_tw) * tmp_31 ;
tmp_33 = (tmp_24 + tmp_23);
tmp_34 =  (C1_6_tw) * tmp_33 ;
tmp_35 = (tmp_30 + tmp_32 - tmp_34);
res[2].re = ( w[1].re * tmp_29  +  w[1].im * tmp_35 );
tmp_36 = (tmp_20 - tmp_17);
tmp_37 =  (C0_6_tw) * tmp_36 ;
tmp_38 = (tmp_7 + tmp_12);
tmp_39 =  (C1_6_tw) * tmp_38 ;
tmp_40 = (tmp_13 + tmp_37 - tmp_39);
tmp_41 = (tmp_7 - tmp_12);
tmp_42 =  (C0_6_tw) * tmp_41 ;
tmp_43 = (tmp_17 + tmp_20);
tmp_44 =  (C1_6_tw) * tmp_43 ;
tmp_45 = (tmp_21 + tmp_42 - tmp_44);
res[3].re = ( w[2].re * tmp_40  +  w[2].im * tmp_45 );
tmp_46 = (tmp_24 - tmp_23);
tmp_47 =  (C0_6_tw) * tmp_46 ;
tmp_48 = (tmp_0 + tmp_47 - tmp_28);
tmp_49 = (tmp_2 - tmp_1);
tmp_50 =  (C0_6_tw) * tmp_49 ;
tmp_51 = (tmp_30 + tmp_50 - tmp_34);
res[4].re = ( w[3].re * tmp_48  +  w[3].im * tmp_51 );
tmp_52 = (tmp_17 - tmp_20);
tmp_53 =  (C0_6_tw) * tmp_52 ;
tmp_54 = (tmp_13 + tmp_53 - tmp_39);
tmp_55 = (tmp_12 - tmp_7);
tmp_56 =  (C0_6_tw) * tmp_55 ;
tmp_57 = (tmp_21 + tmp_56 - tmp_44);
res[5].re = ( w[4].re * tmp_54  +  w[4].im * tmp_57 );
res[0].im = (tmp_30 + tmp_24 + tmp_23);
res[1].im = ( w[0].re * tmp_22  -  w[0].im * tmp_14 );
res[2].im = ( w[1].re * tmp_35  -  w[1].im * tmp_29 );
res[3].im = ( w[2].re * tmp_45  -  w[2].im * tmp_40 );
res[4].im = ( w[3].re * tmp_51  -  w[3].im * tmp_48 );
res[5].im = ( w[4].re * tmp_57  -  w[4].im * tmp_54 );


memory<complex<float32_t>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<float32_t>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<float32_t>>::store(in,2 * stageLength * strideFactor , res[2]);
memory<complex<float32_t>>::store(in,3 * stageLength * strideFactor , res[3]);
memory<complex<float32_t>>::store(in,4 * stageLength * strideFactor , res[4]);
memory<complex<float32_t>>::store(in,5 * stageLength * strideFactor , res[5]);


};

/* Nb Multiplications = 20, 
Nb Additions = 52,
Nb Negates = 26,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_notw(int stageLength,const complex<float32_t> *in,complex<float32_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float32_t> a[6];
complex<float32_t> res[6];


 float32_t tmp_0;
 float32_t tmp_1;
 float32_t tmp_2;
 float32_t tmp_3;
 float32_t tmp_4;
 float32_t tmp_5;
 float32_t tmp_6;
 float32_t tmp_7;
 float32_t tmp_8;
 float32_t tmp_9;
 float32_t tmp_10;
 float32_t tmp_11;
 float32_t tmp_12;
 float32_t tmp_13;
 float32_t tmp_14;
 float32_t tmp_15;
 float32_t tmp_16;
 float32_t tmp_17;
 float32_t tmp_18;
 float32_t tmp_19;
 float32_t tmp_20;
 float32_t tmp_21;
 float32_t tmp_22;
 float32_t tmp_23;
 float32_t tmp_24;
 float32_t tmp_25;
 float32_t tmp_26;
 float32_t tmp_27;
 float32_t tmp_28;
 float32_t tmp_29;
 float32_t tmp_30;
 float32_t tmp_31;


a[0] = memory<complex<float32_t>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<float32_t>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<float32_t>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<float32_t>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<float32_t>>::load(in,4 * stageLength * strideFactor);
a[5] = memory<complex<float32_t>>::load(in,5 * stageLength * strideFactor);


tmp_0 = (a[0].re + a[3].re);
tmp_1 = (a[1].re + a[4].re);
tmp_2 = (a[2].re + a[5].re);
res[0].re = (tmp_0 + tmp_1 + tmp_2);
tmp_3 = (a[1].re - a[4].re);
tmp_4 =  (C1_6_notw) * tmp_3 ;
tmp_5 = (a[1].im - a[4].im);
tmp_6 =  (C0_6_notw) * tmp_5 ;
tmp_7 = (tmp_4 - tmp_6);
tmp_8 = (a[2].re - a[5].re);
tmp_9 =  (C1_6_notw) * tmp_8 ;
tmp_10 = (a[2].im - a[5].im);
tmp_11 =  (C0_6_notw) * tmp_10 ;
tmp_12 = ( (-(tmp_9))   - tmp_11);
tmp_13 = (a[0].re - a[3].re);
res[1].re = (tmp_7 + tmp_12 + tmp_13);
tmp_14 = (a[2].im + a[5].im);
tmp_15 = (a[1].im + a[4].im);
tmp_16 = (tmp_1 + tmp_2);
tmp_17 =  (C1_6_notw) * tmp_16 ;
res[2].re = (tmp_0 +  (C0_6_notw) * (tmp_14 - tmp_15)  - tmp_17);
tmp_18 =  (C0_6_notw) * tmp_8 ;
tmp_19 =  (C1_6_notw) * tmp_10 ;
tmp_20 = (tmp_18 - tmp_19);
tmp_21 =  (C1_6_notw) * tmp_5 ;
tmp_22 =  (C0_6_notw) * tmp_3 ;
tmp_23 = (tmp_21 + tmp_22);
tmp_24 = (tmp_7 + tmp_12);
tmp_25 =  (C1_6_notw) * tmp_24 ;
res[3].re = (tmp_13 +  (C0_6_notw) * (tmp_20 - tmp_23)  - tmp_25);
res[4].re = (tmp_0 +  (C0_6_notw) * (tmp_15 - tmp_14)  - tmp_17);
res[5].re = (tmp_13 +  (C0_6_notw) * (tmp_23 - tmp_20)  - tmp_25);
tmp_26 = (a[0].im + a[3].im);
res[0].im = (tmp_26 + tmp_15 + tmp_14);
tmp_27 = (a[0].im - a[3].im);
res[1].im = (tmp_23 + tmp_20 + tmp_27);
tmp_28 = (tmp_15 + tmp_14);
tmp_29 =  (C1_6_notw) * tmp_28 ;
res[2].im = (tmp_26 +  (C0_6_notw) * (tmp_1 - tmp_2)  - tmp_29);
tmp_30 = (tmp_23 + tmp_20);
tmp_31 =  (C1_6_notw) * tmp_30 ;
res[3].im = (tmp_27 +  (C0_6_notw) * (tmp_7 - tmp_12)  - tmp_31);
res[4].im = (tmp_26 +  (C0_6_notw) * (tmp_2 - tmp_1)  - tmp_29);
res[5].im = (tmp_27 +  (C0_6_notw) * (tmp_12 - tmp_7)  - tmp_31);


memory<complex<float32_t>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<float32_t>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<float32_t>>::store(out,2 * stageLength * strideFactor , res[2]);
memory<complex<float32_t>>::store(out,3 * stageLength * strideFactor , res[3]);
memory<complex<float32_t>>::store(out,4 * stageLength * strideFactor , res[4]);
memory<complex<float32_t>>::store(out,5 * stageLength * strideFactor , res[5]);


};

/* Nb Multiplications = 40, 
Nb Additions = 62,
Nb Negates = 31,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_tw(int stageLength,const complex<float32_t> *w,const complex<float32_t> *in,complex<float32_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float32_t> a[6];
complex<float32_t> res[6];


 float32_t tmp_0;
 float32_t tmp_1;
 float32_t tmp_2;
 float32_t tmp_3;
 float32_t tmp_4;
 float32_t tmp_5;
 float32_t tmp_6;
 float32_t tmp_7;
 float32_t tmp_8;
 float32_t tmp_9;
 float32_t tmp_10;
 float32_t tmp_11;
 float32_t tmp_12;
 float32_t tmp_13;
 float32_t tmp_14;
 float32_t tmp_15;
 float32_t tmp_16;
 float32_t tmp_17;
 float32_t tmp_18;
 float32_t tmp_19;
 float32_t tmp_20;
 float32_t tmp_21;
 float32_t tmp_22;
 float32_t tmp_23;
 float32_t tmp_24;
 float32_t tmp_25;
 float32_t tmp_26;
 float32_t tmp_27;
 float32_t tmp_28;
 float32_t tmp_29;
 float32_t tmp_30;
 float32_t tmp_31;
 float32_t tmp_32;
 float32_t tmp_33;
 float32_t tmp_34;
 float32_t tmp_35;
 float32_t tmp_36;
 float32_t tmp_37;
 float32_t tmp_38;
 float32_t tmp_39;
 float32_t tmp_40;
 float32_t tmp_41;
 float32_t tmp_42;
 float32_t tmp_43;
 float32_t tmp_44;
 float32_t tmp_45;
 float32_t tmp_46;
 float32_t tmp_47;
 float32_t tmp_48;
 float32_t tmp_49;
 float32_t tmp_50;
 float32_t tmp_51;
 float32_t tmp_52;
 float32_t tmp_53;
 float32_t tmp_54;
 float32_t tmp_55;
 float32_t tmp_56;
 float32_t tmp_57;


a[0] = memory<complex<float32_t>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<float32_t>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<float32_t>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<float32_t>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<float32_t>>::load(in,4 * stageLength * strideFactor);
a[5] = memory<complex<float32_t>>::load(in,5 * stageLength * strideFactor);


tmp_0 = (a[0].re + a[3].re);
tmp_1 = (a[1].re + a[4].re);
tmp_2 = (a[2].re + a[5].re);
res[0].re = (tmp_0 + tmp_1 + tmp_2);
tmp_3 = (a[1].re - a[4].re);
tmp_4 =  (C1_6_tw) * tmp_3 ;
tmp_5 = (a[1].im - a[4].im);
tmp_6 =  (C0_6_tw) * tmp_5 ;
tmp_7 = (tmp_4 - tmp_6);
tmp_8 = (a[2].re - a[5].re);
tmp_9 =  (C1_6_tw) * tmp_8 ;
tmp_10 = (a[2].im - a[5].im);
tmp_11 =  (C0_6_tw) * tmp_10 ;
tmp_12 = ( (-(tmp_9))   - tmp_11);
tmp_13 = (a[0].re - a[3].re);
tmp_14 = (tmp_7 + tmp_12 + tmp_13);
tmp_15 =  (C1_6_tw) * tmp_5 ;
tmp_16 =  (C0_6_tw) * tmp_3 ;
tmp_17 = (tmp_15 + tmp_16);
tmp_18 =  (C0_6_tw) * tmp_8 ;
tmp_19 =  (C1_6_tw) * tmp_10 ;
tmp_20 = (tmp_18 - tmp_19);
tmp_21 = (a[0].im - a[3].im);
tmp_22 = (tmp_17 + tmp_20 + tmp_21);
res[1].re = ( w[0].re * tmp_14  +  w[0].im * tmp_22 );
tmp_23 = (a[2].im + a[5].im);
tmp_24 = (a[1].im + a[4].im);
tmp_25 = (tmp_23 - tmp_24);
tmp_26 =  (C0_6_tw) * tmp_25 ;
tmp_27 = (tmp_1 + tmp_2);
tmp_28 =  (C1_6_tw) * tmp_27 ;
tmp_29 = (tmp_0 + tmp_26 - tmp_28);
tmp_30 = (a[0].im + a[3].im);
tmp_31 = (tmp_1 - tmp_2);
tmp_32 =  (C0_6_tw) * tmp_31 ;
tmp_33 = (tmp_24 + tmp_23);
tmp_34 =  (C1_6_tw) * tmp_33 ;
tmp_35 = (tmp_30 + tmp_32 - tmp_34);
res[2].re = ( w[1].re * tmp_29  +  w[1].im * tmp_35 );
tmp_36 = (tmp_20 - tmp_17);
tmp_37 =  (C0_6_tw) * tmp_36 ;
tmp_38 = (tmp_7 + tmp_12);
tmp_39 =  (C1_6_tw) * tmp_38 ;
tmp_40 = (tmp_13 + tmp_37 - tmp_39);
tmp_41 = (tmp_7 - tmp_12);
tmp_42 =  (C0_6_tw) * tmp_41 ;
tmp_43 = (tmp_17 + tmp_20);
tmp_44 =  (C1_6_tw) * tmp_43 ;
tmp_45 = (tmp_21 + tmp_42 - tmp_44);
res[3].re = ( w[2].re * tmp_40  +  w[2].im * tmp_45 );
tmp_46 = (tmp_24 - tmp_23);
tmp_47 =  (C0_6_tw) * tmp_46 ;
tmp_48 = (tmp_0 + tmp_47 - tmp_28);
tmp_49 = (tmp_2 - tmp_1);
tmp_50 =  (C0_6_tw) * tmp_49 ;
tmp_51 = (tmp_30 + tmp_50 - tmp_34);
res[4].re = ( w[3].re * tmp_48  +  w[3].im * tmp_51 );
tmp_52 = (tmp_17 - tmp_20);
tmp_53 =  (C0_6_tw) * tmp_52 ;
tmp_54 = (tmp_13 + tmp_53 - tmp_39);
tmp_55 = (tmp_12 - tmp_7);
tmp_56 =  (C0_6_tw) * tmp_55 ;
tmp_57 = (tmp_21 + tmp_56 - tmp_44);
res[5].re = ( w[4].re * tmp_54  +  w[4].im * tmp_57 );
res[0].im = (tmp_30 + tmp_24 + tmp_23);
res[1].im = ( w[0].re * tmp_22  -  w[0].im * tmp_14 );
res[2].im = ( w[1].re * tmp_35  -  w[1].im * tmp_29 );
res[3].im = ( w[2].re * tmp_45  -  w[2].im * tmp_40 );
res[4].im = ( w[3].re * tmp_51  -  w[3].im * tmp_48 );
res[5].im = ( w[4].re * tmp_57  -  w[4].im * tmp_54 );


memory<complex<float32_t>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<float32_t>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<float32_t>>::store(out,2 * stageLength * strideFactor , res[2]);
memory<complex<float32_t>>::store(out,3 * stageLength * strideFactor , res[3]);
memory<complex<float32_t>>::store(out,4 * stageLength * strideFactor , res[4]);
memory<complex<float32_t>>::store(out,5 * stageLength * strideFactor , res[5]);


};

/* Nb Multiplications = 20, 
Nb Additions = 52,
Nb Negates = 26,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_last(complex<float32_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float32_t> a[6];
complex<float32_t> res[6];


 float32_t tmp_0;
 float32_t tmp_1;
 float32_t tmp_2;
 float32_t tmp_3;
 float32_t tmp_4;
 float32_t tmp_5;
 float32_t tmp_6;
 float32_t tmp_7;
 float32_t tmp_8;
 float32_t tmp_9;
 float32_t tmp_10;
 float32_t tmp_11;
 float32_t tmp_12;
 float32_t tmp_13;
 float32_t tmp_14;
 float32_t tmp_15;
 float32_t tmp_16;
 float32_t tmp_17;
 float32_t tmp_18;
 float32_t tmp_19;
 float32_t tmp_20;
 float32_t tmp_21;
 float32_t tmp_22;
 float32_t tmp_23;
 float32_t tmp_24;
 float32_t tmp_25;
 float32_t tmp_26;
 float32_t tmp_27;
 float32_t tmp_28;
 float32_t tmp_29;
 float32_t tmp_30;
 float32_t tmp_31;


a[0] = memory<complex<float32_t>>::load(in,0 * strideFactor);
a[1] = memory<complex<float32_t>>::load(in,1 * strideFactor);
a[2] = memory<complex<float32_t>>::load(in,2 * strideFactor);
a[3] = memory<complex<float32_t>>::load(in,3 * strideFactor);
a[4] = memory<complex<float32_t>>::load(in,4 * strideFactor);
a[5] = memory<complex<float32_t>>::load(in,5 * strideFactor);


tmp_0 = (a[0].re + a[3].re);
tmp_1 = (a[1].re + a[4].re);
tmp_2 = (a[2].re + a[5].re);
res[0].re = (tmp_0 + tmp_1 + tmp_2);
tmp_3 = (a[1].re - a[4].re);
tmp_4 =  (C1_6_notw) * tmp_3 ;
tmp_5 = (a[1].im - a[4].im);
tmp_6 =  (C0_6_notw) * tmp_5 ;
tmp_7 = (tmp_4 - tmp_6);
tmp_8 = (a[2].re - a[5].re);
tmp_9 =  (C1_6_notw) * tmp_8 ;
tmp_10 = (a[2].im - a[5].im);
tmp_11 =  (C0_6_notw) * tmp_10 ;
tmp_12 = ( (-(tmp_9))   - tmp_11);
tmp_13 = (a[0].re - a[3].re);
res[1].re = (tmp_7 + tmp_12 + tmp_13);
tmp_14 = (a[2].im + a[5].im);
tmp_15 = (a[1].im + a[4].im);
tmp_16 = (tmp_1 + tmp_2);
tmp_17 =  (C1_6_notw) * tmp_16 ;
res[2].re = (tmp_0 +  (C0_6_notw) * (tmp_14 - tmp_15)  - tmp_17);
tmp_18 =  (C0_6_notw) * tmp_8 ;
tmp_19 =  (C1_6_notw) * tmp_10 ;
tmp_20 = (tmp_18 - tmp_19);
tmp_21 =  (C1_6_notw) * tmp_5 ;
tmp_22 =  (C0_6_notw) * tmp_3 ;
tmp_23 = (tmp_21 + tmp_22);
tmp_24 = (tmp_7 + tmp_12);
tmp_25 =  (C1_6_notw) * tmp_24 ;
res[3].re = (tmp_13 +  (C0_6_notw) * (tmp_20 - tmp_23)  - tmp_25);
res[4].re = (tmp_0 +  (C0_6_notw) * (tmp_15 - tmp_14)  - tmp_17);
res[5].re = (tmp_13 +  (C0_6_notw) * (tmp_23 - tmp_20)  - tmp_25);
tmp_26 = (a[0].im + a[3].im);
res[0].im = (tmp_26 + tmp_15 + tmp_14);
tmp_27 = (a[0].im - a[3].im);
res[1].im = (tmp_23 + tmp_20 + tmp_27);
tmp_28 = (tmp_15 + tmp_14);
tmp_29 =  (C1_6_notw) * tmp_28 ;
res[2].im = (tmp_26 +  (C0_6_notw) * (tmp_1 - tmp_2)  - tmp_29);
tmp_30 = (tmp_23 + tmp_20);
tmp_31 =  (C1_6_notw) * tmp_30 ;
res[3].im = (tmp_27 +  (C0_6_notw) * (tmp_7 - tmp_12)  - tmp_31);
res[4].im = (tmp_26 +  (C0_6_notw) * (tmp_2 - tmp_1)  - tmp_29);
res[5].im = (tmp_27 +  (C0_6_notw) * (tmp_12 - tmp_7)  - tmp_31);


memory<complex<float32_t>>::store(in,0 * strideFactor , res[0]);
memory<complex<float32_t>>::store(in,1 * strideFactor , res[1]);
memory<complex<float32_t>>::store(in,2 * strideFactor , res[2]);
memory<complex<float32_t>>::store(in,3 * strideFactor , res[3]);
memory<complex<float32_t>>::store(in,4 * strideFactor , res[4]);
memory<complex<float32_t>>::store(in,5 * strideFactor , res[5]);


};

/* Nb Multiplications = 20, 
Nb Additions = 52,
Nb Negates = 26,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_last(const complex<float32_t> *in,complex<float32_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float32_t> a[6];
complex<float32_t> res[6];


 float32_t tmp_0;
 float32_t tmp_1;
 float32_t tmp_2;
 float32_t tmp_3;
 float32_t tmp_4;
 float32_t tmp_5;
 float32_t tmp_6;
 float32_t tmp_7;
 float32_t tmp_8;
 float32_t tmp_9;
 float32_t tmp_10;
 float32_t tmp_11;
 float32_t tmp_12;
 float32_t tmp_13;
 float32_t tmp_14;
 float32_t tmp_15;
 float32_t tmp_16;
 float32_t tmp_17;
 float32_t tmp_18;
 float32_t tmp_19;
 float32_t tmp_20;
 float32_t tmp_21;
 float32_t tmp_22;
 float32_t tmp_23;
 float32_t tmp_24;
 float32_t tmp_25;
 float32_t tmp_26;
 float32_t tmp_27;
 float32_t tmp_28;
 float32_t tmp_29;
 float32_t tmp_30;
 float32_t tmp_31;


a[0] = memory<complex<float32_t>>::load(in,0 * strideFactor);
a[1] = memory<complex<float32_t>>::load(in,1 * strideFactor);
a[2] = memory<complex<float32_t>>::load(in,2 * strideFactor);
a[3] = memory<complex<float32_t>>::load(in,3 * strideFactor);
a[4] = memory<complex<float32_t>>::load(in,4 * strideFactor);
a[5] = memory<complex<float32_t>>::load(in,5 * strideFactor);


tmp_0 = (a[0].re + a[3].re);
tmp_1 = (a[1].re + a[4].re);
tmp_2 = (a[2].re + a[5].re);
res[0].re = (tmp_0 + tmp_1 + tmp_2);
tmp_3 = (a[1].re - a[4].re);
tmp_4 =  (C1_6_notw) * tmp_3 ;
tmp_5 = (a[1].im - a[4].im);
tmp_6 =  (C0_6_notw) * tmp_5 ;
tmp_7 = (tmp_4 - tmp_6);
tmp_8 = (a[2].re - a[5].re);
tmp_9 =  (C1_6_notw) * tmp_8 ;
tmp_10 = (a[2].im - a[5].im);
tmp_11 =  (C0_6_notw) * tmp_10 ;
tmp_12 = ( (-(tmp_9))   - tmp_11);
tmp_13 = (a[0].re - a[3].re);
res[1].re = (tmp_7 + tmp_12 + tmp_13);
tmp_14 = (a[2].im + a[5].im);
tmp_15 = (a[1].im + a[4].im);
tmp_16 = (tmp_1 + tmp_2);
tmp_17 =  (C1_6_notw) * tmp_16 ;
res[2].re = (tmp_0 +  (C0_6_notw) * (tmp_14 - tmp_15)  - tmp_17);
tmp_18 =  (C0_6_notw) * tmp_8 ;
tmp_19 =  (C1_6_notw) * tmp_10 ;
tmp_20 = (tmp_18 - tmp_19);
tmp_21 =  (C1_6_notw) * tmp_5 ;
tmp_22 =  (C0_6_notw) * tmp_3 ;
tmp_23 = (tmp_21 + tmp_22);
tmp_24 = (tmp_7 + tmp_12);
tmp_25 =  (C1_6_notw) * tmp_24 ;
res[3].re = (tmp_13 +  (C0_6_notw) * (tmp_20 - tmp_23)  - tmp_25);
res[4].re = (tmp_0 +  (C0_6_notw) * (tmp_15 - tmp_14)  - tmp_17);
res[5].re = (tmp_13 +  (C0_6_notw) * (tmp_23 - tmp_20)  - tmp_25);
tmp_26 = (a[0].im + a[3].im);
res[0].im = (tmp_26 + tmp_15 + tmp_14);
tmp_27 = (a[0].im - a[3].im);
res[1].im = (tmp_23 + tmp_20 + tmp_27);
tmp_28 = (tmp_15 + tmp_14);
tmp_29 =  (C1_6_notw) * tmp_28 ;
res[2].im = (tmp_26 +  (C0_6_notw) * (tmp_1 - tmp_2)  - tmp_29);
tmp_30 = (tmp_23 + tmp_20);
tmp_31 =  (C1_6_notw) * tmp_30 ;
res[3].im = (tmp_27 +  (C0_6_notw) * (tmp_7 - tmp_12)  - tmp_31);
res[4].im = (tmp_26 +  (C0_6_notw) * (tmp_2 - tmp_1)  - tmp_29);
res[5].im = (tmp_27 +  (C0_6_notw) * (tmp_12 - tmp_7)  - tmp_31);


memory<complex<float32_t>>::store(out,0 * strideFactor , res[0]);
memory<complex<float32_t>>::store(out,1 * strideFactor , res[1]);
memory<complex<float32_t>>::store(out,2 * strideFactor , res[2]);
memory<complex<float32_t>>::store(out,3 * strideFactor , res[3]);
memory<complex<float32_t>>::store(out,4 * strideFactor , res[4]);
memory<complex<float32_t>>::store(out,5 * strideFactor , res[5]);


};

};

#endif /* #ifndef _RADIX_F32_6_H */