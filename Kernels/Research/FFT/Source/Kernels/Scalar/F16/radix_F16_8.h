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

#ifndef _RADIX_F16_8_H
#define _RADIX_F16_8_H

#include "complex.h"

/**************

DIRECT FFT 

***************/
template<int inputstrideenabled> 
struct radix<DIRECT,float16_t, 8,inputstrideenabled>{

static constexpr float16_t C0_8_tw{0.7071067811865476f16};
  
static constexpr float16_t C0_8_notw{0.7071067811865476f16};
  

/* Nb Multiplications = 4, 
Nb Additions = 52,
Nb Negates = 26,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_notw(int stageLength,complex<float16_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[8];
complex<float16_t> res[8];


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
a[5] = memory<complex<float16_t>>::load(in,5 * stageLength * strideFactor);
a[6] = memory<complex<float16_t>>::load(in,6 * stageLength * strideFactor);
a[7] = memory<complex<float16_t>>::load(in,7 * stageLength * strideFactor);


tmp_0 = (a[0].re + a[4].re);
tmp_1 = (a[2].re + a[6].re);
tmp_2 = (tmp_0 + tmp_1);
tmp_3 = (a[1].re + a[5].re);
tmp_4 = (a[3].re + a[7].re);
tmp_5 = (tmp_3 + tmp_4);
res[0].re = (tmp_2 + tmp_5);
tmp_6 = (a[0].re - a[4].re);
tmp_7 = (a[2].im - a[6].im);
tmp_8 = (tmp_6 + tmp_7);
tmp_9 = (a[1].re - a[5].re);
tmp_10 = (a[1].im - a[5].im);
tmp_11 = (tmp_9 + tmp_10);
tmp_12 = (a[7].re - a[3].re);
tmp_13 = (a[3].im - a[7].im);
tmp_14 = (tmp_12 + tmp_13);
tmp_15 = (tmp_11 + tmp_14);
tmp_16 =  (C0_8_notw) * tmp_15 ;
res[1].re = (tmp_8 + tmp_16);
tmp_17 = (tmp_0 - tmp_1);
tmp_18 = (a[1].im + a[5].im);
tmp_19 = (a[3].im + a[7].im);
tmp_20 = (tmp_18 - tmp_19);
res[2].re = (tmp_17 + tmp_20);
tmp_21 = (tmp_6 - tmp_7);
tmp_22 = (tmp_10 - tmp_9);
tmp_23 = (tmp_12 - tmp_13);
tmp_24 = (tmp_22 - tmp_23);
tmp_25 =  (C0_8_notw) * tmp_24 ;
res[3].re = (tmp_21 + tmp_25);
res[4].re = (tmp_2 - tmp_5);
res[5].re = (tmp_8 - tmp_16);
res[6].re = (tmp_17 - tmp_20);
res[7].re = (tmp_21 - tmp_25);
tmp_26 = (a[0].im + a[4].im);
tmp_27 = (a[2].im + a[6].im);
tmp_28 = (tmp_26 + tmp_27);
tmp_29 = (tmp_18 + tmp_19);
res[0].im = (tmp_28 + tmp_29);
tmp_30 = (a[0].im - a[4].im);
tmp_31 = (a[2].re - a[6].re);
tmp_32 = (tmp_30 - tmp_31);
tmp_33 = (tmp_23 + tmp_22);
tmp_34 =  (C0_8_notw) * tmp_33 ;
res[1].im = (tmp_32 + tmp_34);
tmp_35 = (tmp_26 - tmp_27);
tmp_36 = (tmp_3 - tmp_4);
res[2].im = (tmp_35 - tmp_36);
tmp_37 = (tmp_31 + tmp_30);
tmp_38 = (tmp_11 - tmp_14);
tmp_39 =  (C0_8_notw) * tmp_38 ;
res[3].im = (tmp_37 - tmp_39);
res[4].im = (tmp_28 - tmp_29);
res[5].im = (tmp_32 - tmp_34);
res[6].im = (tmp_36 + tmp_35);
res[7].im = (tmp_37 + tmp_39);


memory<complex<float16_t>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<float16_t>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<float16_t>>::store(in,2 * stageLength * strideFactor , res[2]);
memory<complex<float16_t>>::store(in,3 * stageLength * strideFactor , res[3]);
memory<complex<float16_t>>::store(in,4 * stageLength * strideFactor , res[4]);
memory<complex<float16_t>>::store(in,5 * stageLength * strideFactor , res[5]);
memory<complex<float16_t>>::store(in,6 * stageLength * strideFactor , res[6]);
memory<complex<float16_t>>::store(in,7 * stageLength * strideFactor , res[7]);


};

/* Nb Multiplications = 32, 
Nb Additions = 66,
Nb Negates = 33,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_tw(int stageLength,const complex<float16_t> *w,complex<float16_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[8];
complex<float16_t> res[8];


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
 float16_t tmp_48;
 float16_t tmp_49;
 float16_t tmp_50;
 float16_t tmp_51;
 float16_t tmp_52;
 float16_t tmp_53;


a[0] = memory<complex<float16_t>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<float16_t>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<float16_t>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<float16_t>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<float16_t>>::load(in,4 * stageLength * strideFactor);
a[5] = memory<complex<float16_t>>::load(in,5 * stageLength * strideFactor);
a[6] = memory<complex<float16_t>>::load(in,6 * stageLength * strideFactor);
a[7] = memory<complex<float16_t>>::load(in,7 * stageLength * strideFactor);


tmp_0 = (a[0].re + a[4].re);
tmp_1 = (a[2].re + a[6].re);
tmp_2 = (tmp_0 + tmp_1);
tmp_3 = (a[1].re + a[5].re);
tmp_4 = (a[3].re + a[7].re);
tmp_5 = (tmp_3 + tmp_4);
res[0].re = (tmp_2 + tmp_5);
tmp_6 = (a[0].re - a[4].re);
tmp_7 = (a[2].im - a[6].im);
tmp_8 = (tmp_6 + tmp_7);
tmp_9 = (a[1].re - a[5].re);
tmp_10 = (a[1].im - a[5].im);
tmp_11 = (tmp_9 + tmp_10);
tmp_12 = (a[7].re - a[3].re);
tmp_13 = (a[3].im - a[7].im);
tmp_14 = (tmp_12 + tmp_13);
tmp_15 = (tmp_11 + tmp_14);
tmp_16 =  (C0_8_tw) * tmp_15 ;
tmp_17 = (tmp_8 + tmp_16);
tmp_18 = (a[0].im - a[4].im);
tmp_19 = (a[2].re - a[6].re);
tmp_20 = (tmp_18 - tmp_19);
tmp_21 = (tmp_12 - tmp_13);
tmp_22 = (tmp_10 - tmp_9);
tmp_23 = (tmp_21 + tmp_22);
tmp_24 =  (C0_8_tw) * tmp_23 ;
tmp_25 = (tmp_20 + tmp_24);
res[1].re = ( w[0].re * tmp_17  -  w[0].im * tmp_25 );
tmp_26 = (tmp_0 - tmp_1);
tmp_27 = (a[1].im + a[5].im);
tmp_28 = (a[3].im + a[7].im);
tmp_29 = (tmp_27 - tmp_28);
tmp_30 = (tmp_26 + tmp_29);
tmp_31 = (a[0].im + a[4].im);
tmp_32 = (a[2].im + a[6].im);
tmp_33 = (tmp_31 - tmp_32);
tmp_34 = (tmp_3 - tmp_4);
tmp_35 = (tmp_33 - tmp_34);
res[2].re = ( w[1].re * tmp_30  -  w[1].im * tmp_35 );
tmp_36 = (tmp_6 - tmp_7);
tmp_37 = (tmp_22 - tmp_21);
tmp_38 =  (C0_8_tw) * tmp_37 ;
tmp_39 = (tmp_36 + tmp_38);
tmp_40 = (tmp_19 + tmp_18);
tmp_41 = (tmp_11 - tmp_14);
tmp_42 =  (C0_8_tw) * tmp_41 ;
tmp_43 = (tmp_40 - tmp_42);
res[3].re = ( w[2].re * tmp_39  -  w[2].im * tmp_43 );
tmp_44 = (tmp_2 - tmp_5);
tmp_45 = (tmp_31 + tmp_32);
tmp_46 = (tmp_27 + tmp_28);
tmp_47 = (tmp_45 - tmp_46);
res[4].re = ( w[3].re * tmp_44  -  w[3].im * tmp_47 );
tmp_48 = (tmp_8 - tmp_16);
tmp_49 = (tmp_20 - tmp_24);
res[5].re = ( w[4].re * tmp_48  -  w[4].im * tmp_49 );
tmp_50 = (tmp_26 - tmp_29);
tmp_51 = (tmp_34 + tmp_33);
res[6].re = ( w[5].re * tmp_50  -  w[5].im * tmp_51 );
tmp_52 = (tmp_36 - tmp_38);
tmp_53 = (tmp_40 + tmp_42);
res[7].re = ( w[6].re * tmp_52  -  w[6].im * tmp_53 );
res[0].im = (tmp_45 + tmp_46);
res[1].im = ( w[0].re * tmp_25  +  w[0].im * tmp_17 );
res[2].im = ( w[1].re * tmp_35  +  w[1].im * tmp_30 );
res[3].im = ( w[2].re * tmp_43  +  w[2].im * tmp_39 );
res[4].im = ( w[3].re * tmp_47  +  w[3].im * tmp_44 );
res[5].im = ( w[4].re * tmp_49  +  w[4].im * tmp_48 );
res[6].im = ( w[5].re * tmp_51  +  w[5].im * tmp_50 );
res[7].im = ( w[6].re * tmp_53  +  w[6].im * tmp_52 );


memory<complex<float16_t>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<float16_t>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<float16_t>>::store(in,2 * stageLength * strideFactor , res[2]);
memory<complex<float16_t>>::store(in,3 * stageLength * strideFactor , res[3]);
memory<complex<float16_t>>::store(in,4 * stageLength * strideFactor , res[4]);
memory<complex<float16_t>>::store(in,5 * stageLength * strideFactor , res[5]);
memory<complex<float16_t>>::store(in,6 * stageLength * strideFactor , res[6]);
memory<complex<float16_t>>::store(in,7 * stageLength * strideFactor , res[7]);


};

/* Nb Multiplications = 4, 
Nb Additions = 52,
Nb Negates = 26,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_notw(int stageLength,const complex<float16_t> *in,complex<float16_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[8];
complex<float16_t> res[8];


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
a[5] = memory<complex<float16_t>>::load(in,5 * stageLength * strideFactor);
a[6] = memory<complex<float16_t>>::load(in,6 * stageLength * strideFactor);
a[7] = memory<complex<float16_t>>::load(in,7 * stageLength * strideFactor);


tmp_0 = (a[0].re + a[4].re);
tmp_1 = (a[2].re + a[6].re);
tmp_2 = (tmp_0 + tmp_1);
tmp_3 = (a[1].re + a[5].re);
tmp_4 = (a[3].re + a[7].re);
tmp_5 = (tmp_3 + tmp_4);
res[0].re = (tmp_2 + tmp_5);
tmp_6 = (a[0].re - a[4].re);
tmp_7 = (a[2].im - a[6].im);
tmp_8 = (tmp_6 + tmp_7);
tmp_9 = (a[1].re - a[5].re);
tmp_10 = (a[1].im - a[5].im);
tmp_11 = (tmp_9 + tmp_10);
tmp_12 = (a[7].re - a[3].re);
tmp_13 = (a[3].im - a[7].im);
tmp_14 = (tmp_12 + tmp_13);
tmp_15 = (tmp_11 + tmp_14);
tmp_16 =  (C0_8_notw) * tmp_15 ;
res[1].re = (tmp_8 + tmp_16);
tmp_17 = (tmp_0 - tmp_1);
tmp_18 = (a[1].im + a[5].im);
tmp_19 = (a[3].im + a[7].im);
tmp_20 = (tmp_18 - tmp_19);
res[2].re = (tmp_17 + tmp_20);
tmp_21 = (tmp_6 - tmp_7);
tmp_22 = (tmp_10 - tmp_9);
tmp_23 = (tmp_12 - tmp_13);
tmp_24 = (tmp_22 - tmp_23);
tmp_25 =  (C0_8_notw) * tmp_24 ;
res[3].re = (tmp_21 + tmp_25);
res[4].re = (tmp_2 - tmp_5);
res[5].re = (tmp_8 - tmp_16);
res[6].re = (tmp_17 - tmp_20);
res[7].re = (tmp_21 - tmp_25);
tmp_26 = (a[0].im + a[4].im);
tmp_27 = (a[2].im + a[6].im);
tmp_28 = (tmp_26 + tmp_27);
tmp_29 = (tmp_18 + tmp_19);
res[0].im = (tmp_28 + tmp_29);
tmp_30 = (a[0].im - a[4].im);
tmp_31 = (a[2].re - a[6].re);
tmp_32 = (tmp_30 - tmp_31);
tmp_33 = (tmp_23 + tmp_22);
tmp_34 =  (C0_8_notw) * tmp_33 ;
res[1].im = (tmp_32 + tmp_34);
tmp_35 = (tmp_26 - tmp_27);
tmp_36 = (tmp_3 - tmp_4);
res[2].im = (tmp_35 - tmp_36);
tmp_37 = (tmp_31 + tmp_30);
tmp_38 = (tmp_11 - tmp_14);
tmp_39 =  (C0_8_notw) * tmp_38 ;
res[3].im = (tmp_37 - tmp_39);
res[4].im = (tmp_28 - tmp_29);
res[5].im = (tmp_32 - tmp_34);
res[6].im = (tmp_36 + tmp_35);
res[7].im = (tmp_37 + tmp_39);


memory<complex<float16_t>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<float16_t>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<float16_t>>::store(out,2 * stageLength * strideFactor , res[2]);
memory<complex<float16_t>>::store(out,3 * stageLength * strideFactor , res[3]);
memory<complex<float16_t>>::store(out,4 * stageLength * strideFactor , res[4]);
memory<complex<float16_t>>::store(out,5 * stageLength * strideFactor , res[5]);
memory<complex<float16_t>>::store(out,6 * stageLength * strideFactor , res[6]);
memory<complex<float16_t>>::store(out,7 * stageLength * strideFactor , res[7]);


};

/* Nb Multiplications = 32, 
Nb Additions = 66,
Nb Negates = 33,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_tw(int stageLength,const complex<float16_t> *w,const complex<float16_t> *in,complex<float16_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[8];
complex<float16_t> res[8];


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
 float16_t tmp_48;
 float16_t tmp_49;
 float16_t tmp_50;
 float16_t tmp_51;
 float16_t tmp_52;
 float16_t tmp_53;


a[0] = memory<complex<float16_t>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<float16_t>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<float16_t>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<float16_t>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<float16_t>>::load(in,4 * stageLength * strideFactor);
a[5] = memory<complex<float16_t>>::load(in,5 * stageLength * strideFactor);
a[6] = memory<complex<float16_t>>::load(in,6 * stageLength * strideFactor);
a[7] = memory<complex<float16_t>>::load(in,7 * stageLength * strideFactor);


tmp_0 = (a[0].re + a[4].re);
tmp_1 = (a[2].re + a[6].re);
tmp_2 = (tmp_0 + tmp_1);
tmp_3 = (a[1].re + a[5].re);
tmp_4 = (a[3].re + a[7].re);
tmp_5 = (tmp_3 + tmp_4);
res[0].re = (tmp_2 + tmp_5);
tmp_6 = (a[0].re - a[4].re);
tmp_7 = (a[2].im - a[6].im);
tmp_8 = (tmp_6 + tmp_7);
tmp_9 = (a[1].re - a[5].re);
tmp_10 = (a[1].im - a[5].im);
tmp_11 = (tmp_9 + tmp_10);
tmp_12 = (a[7].re - a[3].re);
tmp_13 = (a[3].im - a[7].im);
tmp_14 = (tmp_12 + tmp_13);
tmp_15 = (tmp_11 + tmp_14);
tmp_16 =  (C0_8_tw) * tmp_15 ;
tmp_17 = (tmp_8 + tmp_16);
tmp_18 = (a[0].im - a[4].im);
tmp_19 = (a[2].re - a[6].re);
tmp_20 = (tmp_18 - tmp_19);
tmp_21 = (tmp_12 - tmp_13);
tmp_22 = (tmp_10 - tmp_9);
tmp_23 = (tmp_21 + tmp_22);
tmp_24 =  (C0_8_tw) * tmp_23 ;
tmp_25 = (tmp_20 + tmp_24);
res[1].re = ( w[0].re * tmp_17  -  w[0].im * tmp_25 );
tmp_26 = (tmp_0 - tmp_1);
tmp_27 = (a[1].im + a[5].im);
tmp_28 = (a[3].im + a[7].im);
tmp_29 = (tmp_27 - tmp_28);
tmp_30 = (tmp_26 + tmp_29);
tmp_31 = (a[0].im + a[4].im);
tmp_32 = (a[2].im + a[6].im);
tmp_33 = (tmp_31 - tmp_32);
tmp_34 = (tmp_3 - tmp_4);
tmp_35 = (tmp_33 - tmp_34);
res[2].re = ( w[1].re * tmp_30  -  w[1].im * tmp_35 );
tmp_36 = (tmp_6 - tmp_7);
tmp_37 = (tmp_22 - tmp_21);
tmp_38 =  (C0_8_tw) * tmp_37 ;
tmp_39 = (tmp_36 + tmp_38);
tmp_40 = (tmp_19 + tmp_18);
tmp_41 = (tmp_11 - tmp_14);
tmp_42 =  (C0_8_tw) * tmp_41 ;
tmp_43 = (tmp_40 - tmp_42);
res[3].re = ( w[2].re * tmp_39  -  w[2].im * tmp_43 );
tmp_44 = (tmp_2 - tmp_5);
tmp_45 = (tmp_31 + tmp_32);
tmp_46 = (tmp_27 + tmp_28);
tmp_47 = (tmp_45 - tmp_46);
res[4].re = ( w[3].re * tmp_44  -  w[3].im * tmp_47 );
tmp_48 = (tmp_8 - tmp_16);
tmp_49 = (tmp_20 - tmp_24);
res[5].re = ( w[4].re * tmp_48  -  w[4].im * tmp_49 );
tmp_50 = (tmp_26 - tmp_29);
tmp_51 = (tmp_34 + tmp_33);
res[6].re = ( w[5].re * tmp_50  -  w[5].im * tmp_51 );
tmp_52 = (tmp_36 - tmp_38);
tmp_53 = (tmp_40 + tmp_42);
res[7].re = ( w[6].re * tmp_52  -  w[6].im * tmp_53 );
res[0].im = (tmp_45 + tmp_46);
res[1].im = ( w[0].re * tmp_25  +  w[0].im * tmp_17 );
res[2].im = ( w[1].re * tmp_35  +  w[1].im * tmp_30 );
res[3].im = ( w[2].re * tmp_43  +  w[2].im * tmp_39 );
res[4].im = ( w[3].re * tmp_47  +  w[3].im * tmp_44 );
res[5].im = ( w[4].re * tmp_49  +  w[4].im * tmp_48 );
res[6].im = ( w[5].re * tmp_51  +  w[5].im * tmp_50 );
res[7].im = ( w[6].re * tmp_53  +  w[6].im * tmp_52 );


memory<complex<float16_t>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<float16_t>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<float16_t>>::store(out,2 * stageLength * strideFactor , res[2]);
memory<complex<float16_t>>::store(out,3 * stageLength * strideFactor , res[3]);
memory<complex<float16_t>>::store(out,4 * stageLength * strideFactor , res[4]);
memory<complex<float16_t>>::store(out,5 * stageLength * strideFactor , res[5]);
memory<complex<float16_t>>::store(out,6 * stageLength * strideFactor , res[6]);
memory<complex<float16_t>>::store(out,7 * stageLength * strideFactor , res[7]);


};

/* Nb Multiplications = 4, 
Nb Additions = 52,
Nb Negates = 26,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_last(complex<float16_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[8];
complex<float16_t> res[8];


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
a[5] = memory<complex<float16_t>>::load(in,5 * strideFactor);
a[6] = memory<complex<float16_t>>::load(in,6 * strideFactor);
a[7] = memory<complex<float16_t>>::load(in,7 * strideFactor);


tmp_0 = (a[0].re + a[4].re);
tmp_1 = (a[2].re + a[6].re);
tmp_2 = (tmp_0 + tmp_1);
tmp_3 = (a[1].re + a[5].re);
tmp_4 = (a[3].re + a[7].re);
tmp_5 = (tmp_3 + tmp_4);
res[0].re = (tmp_2 + tmp_5);
tmp_6 = (a[0].re - a[4].re);
tmp_7 = (a[2].im - a[6].im);
tmp_8 = (tmp_6 + tmp_7);
tmp_9 = (a[1].re - a[5].re);
tmp_10 = (a[1].im - a[5].im);
tmp_11 = (tmp_9 + tmp_10);
tmp_12 = (a[7].re - a[3].re);
tmp_13 = (a[3].im - a[7].im);
tmp_14 = (tmp_12 + tmp_13);
tmp_15 = (tmp_11 + tmp_14);
tmp_16 =  (C0_8_notw) * tmp_15 ;
res[1].re = (tmp_8 + tmp_16);
tmp_17 = (tmp_0 - tmp_1);
tmp_18 = (a[1].im + a[5].im);
tmp_19 = (a[3].im + a[7].im);
tmp_20 = (tmp_18 - tmp_19);
res[2].re = (tmp_17 + tmp_20);
tmp_21 = (tmp_6 - tmp_7);
tmp_22 = (tmp_10 - tmp_9);
tmp_23 = (tmp_12 - tmp_13);
tmp_24 = (tmp_22 - tmp_23);
tmp_25 =  (C0_8_notw) * tmp_24 ;
res[3].re = (tmp_21 + tmp_25);
res[4].re = (tmp_2 - tmp_5);
res[5].re = (tmp_8 - tmp_16);
res[6].re = (tmp_17 - tmp_20);
res[7].re = (tmp_21 - tmp_25);
tmp_26 = (a[0].im + a[4].im);
tmp_27 = (a[2].im + a[6].im);
tmp_28 = (tmp_26 + tmp_27);
tmp_29 = (tmp_18 + tmp_19);
res[0].im = (tmp_28 + tmp_29);
tmp_30 = (a[0].im - a[4].im);
tmp_31 = (a[2].re - a[6].re);
tmp_32 = (tmp_30 - tmp_31);
tmp_33 = (tmp_23 + tmp_22);
tmp_34 =  (C0_8_notw) * tmp_33 ;
res[1].im = (tmp_32 + tmp_34);
tmp_35 = (tmp_26 - tmp_27);
tmp_36 = (tmp_3 - tmp_4);
res[2].im = (tmp_35 - tmp_36);
tmp_37 = (tmp_31 + tmp_30);
tmp_38 = (tmp_11 - tmp_14);
tmp_39 =  (C0_8_notw) * tmp_38 ;
res[3].im = (tmp_37 - tmp_39);
res[4].im = (tmp_28 - tmp_29);
res[5].im = (tmp_32 - tmp_34);
res[6].im = (tmp_36 + tmp_35);
res[7].im = (tmp_37 + tmp_39);


memory<complex<float16_t>>::store(in,0 * strideFactor , res[0]);
memory<complex<float16_t>>::store(in,1 * strideFactor , res[1]);
memory<complex<float16_t>>::store(in,2 * strideFactor , res[2]);
memory<complex<float16_t>>::store(in,3 * strideFactor , res[3]);
memory<complex<float16_t>>::store(in,4 * strideFactor , res[4]);
memory<complex<float16_t>>::store(in,5 * strideFactor , res[5]);
memory<complex<float16_t>>::store(in,6 * strideFactor , res[6]);
memory<complex<float16_t>>::store(in,7 * strideFactor , res[7]);


};

/* Nb Multiplications = 4, 
Nb Additions = 52,
Nb Negates = 26,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_last(const complex<float16_t> *in,complex<float16_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[8];
complex<float16_t> res[8];


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
a[5] = memory<complex<float16_t>>::load(in,5 * strideFactor);
a[6] = memory<complex<float16_t>>::load(in,6 * strideFactor);
a[7] = memory<complex<float16_t>>::load(in,7 * strideFactor);


tmp_0 = (a[0].re + a[4].re);
tmp_1 = (a[2].re + a[6].re);
tmp_2 = (tmp_0 + tmp_1);
tmp_3 = (a[1].re + a[5].re);
tmp_4 = (a[3].re + a[7].re);
tmp_5 = (tmp_3 + tmp_4);
res[0].re = (tmp_2 + tmp_5);
tmp_6 = (a[0].re - a[4].re);
tmp_7 = (a[2].im - a[6].im);
tmp_8 = (tmp_6 + tmp_7);
tmp_9 = (a[1].re - a[5].re);
tmp_10 = (a[1].im - a[5].im);
tmp_11 = (tmp_9 + tmp_10);
tmp_12 = (a[7].re - a[3].re);
tmp_13 = (a[3].im - a[7].im);
tmp_14 = (tmp_12 + tmp_13);
tmp_15 = (tmp_11 + tmp_14);
tmp_16 =  (C0_8_notw) * tmp_15 ;
res[1].re = (tmp_8 + tmp_16);
tmp_17 = (tmp_0 - tmp_1);
tmp_18 = (a[1].im + a[5].im);
tmp_19 = (a[3].im + a[7].im);
tmp_20 = (tmp_18 - tmp_19);
res[2].re = (tmp_17 + tmp_20);
tmp_21 = (tmp_6 - tmp_7);
tmp_22 = (tmp_10 - tmp_9);
tmp_23 = (tmp_12 - tmp_13);
tmp_24 = (tmp_22 - tmp_23);
tmp_25 =  (C0_8_notw) * tmp_24 ;
res[3].re = (tmp_21 + tmp_25);
res[4].re = (tmp_2 - tmp_5);
res[5].re = (tmp_8 - tmp_16);
res[6].re = (tmp_17 - tmp_20);
res[7].re = (tmp_21 - tmp_25);
tmp_26 = (a[0].im + a[4].im);
tmp_27 = (a[2].im + a[6].im);
tmp_28 = (tmp_26 + tmp_27);
tmp_29 = (tmp_18 + tmp_19);
res[0].im = (tmp_28 + tmp_29);
tmp_30 = (a[0].im - a[4].im);
tmp_31 = (a[2].re - a[6].re);
tmp_32 = (tmp_30 - tmp_31);
tmp_33 = (tmp_23 + tmp_22);
tmp_34 =  (C0_8_notw) * tmp_33 ;
res[1].im = (tmp_32 + tmp_34);
tmp_35 = (tmp_26 - tmp_27);
tmp_36 = (tmp_3 - tmp_4);
res[2].im = (tmp_35 - tmp_36);
tmp_37 = (tmp_31 + tmp_30);
tmp_38 = (tmp_11 - tmp_14);
tmp_39 =  (C0_8_notw) * tmp_38 ;
res[3].im = (tmp_37 - tmp_39);
res[4].im = (tmp_28 - tmp_29);
res[5].im = (tmp_32 - tmp_34);
res[6].im = (tmp_36 + tmp_35);
res[7].im = (tmp_37 + tmp_39);


memory<complex<float16_t>>::store(out,0 * strideFactor , res[0]);
memory<complex<float16_t>>::store(out,1 * strideFactor , res[1]);
memory<complex<float16_t>>::store(out,2 * strideFactor , res[2]);
memory<complex<float16_t>>::store(out,3 * strideFactor , res[3]);
memory<complex<float16_t>>::store(out,4 * strideFactor , res[4]);
memory<complex<float16_t>>::store(out,5 * strideFactor , res[5]);
memory<complex<float16_t>>::store(out,6 * strideFactor , res[6]);
memory<complex<float16_t>>::store(out,7 * strideFactor , res[7]);


};

};

/**************

INVERSE FFT 

***************/
template<int inputstrideenabled> 
struct radix<INVERSE,float16_t, 8,inputstrideenabled>{

static constexpr float16_t C0_8_tw{0.7071067811865476f16};
  
static constexpr float16_t C0_8_notw{0.7071067811865476f16};
  

/* Nb Multiplications = 4, 
Nb Additions = 52,
Nb Negates = 26,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_notw(int stageLength,complex<float16_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[8];
complex<float16_t> res[8];


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
a[5] = memory<complex<float16_t>>::load(in,5 * stageLength * strideFactor);
a[6] = memory<complex<float16_t>>::load(in,6 * stageLength * strideFactor);
a[7] = memory<complex<float16_t>>::load(in,7 * stageLength * strideFactor);


tmp_0 = (a[0].re + a[4].re);
tmp_1 = (a[2].re + a[6].re);
tmp_2 = (tmp_0 + tmp_1);
tmp_3 = (a[1].re + a[5].re);
tmp_4 = (a[3].re + a[7].re);
tmp_5 = (tmp_3 + tmp_4);
res[0].re = (tmp_2 + tmp_5);
tmp_6 = (a[0].re - a[4].re);
tmp_7 = (a[2].im - a[6].im);
tmp_8 = (tmp_6 - tmp_7);
tmp_9 = (a[1].re - a[5].re);
tmp_10 = (a[1].im - a[5].im);
tmp_11 = (tmp_9 - tmp_10);
tmp_12 = (a[7].im - a[3].im);
tmp_13 = (a[3].re - a[7].re);
tmp_14 = (tmp_12 - tmp_13);
tmp_15 = (tmp_11 + tmp_14);
tmp_16 =  (C0_8_notw) * tmp_15 ;
res[1].re = (tmp_8 + tmp_16);
tmp_17 = (tmp_0 - tmp_1);
tmp_18 = (a[1].im + a[5].im);
tmp_19 = (a[3].im + a[7].im);
tmp_20 = (tmp_18 - tmp_19);
res[2].re = (tmp_17 - tmp_20);
tmp_21 = (tmp_6 + tmp_7);
tmp_22 = (tmp_9 + tmp_10);
tmp_23 = (tmp_13 + tmp_12);
tmp_24 = (tmp_22 - tmp_23);
tmp_25 =  (C0_8_notw) * tmp_24 ;
res[3].re = (tmp_21 - tmp_25);
res[4].re = (tmp_2 - tmp_5);
res[5].re = (tmp_8 - tmp_16);
res[6].re = (tmp_17 + tmp_20);
res[7].re = (tmp_21 + tmp_25);
tmp_26 = (a[0].im + a[4].im);
tmp_27 = (a[2].im + a[6].im);
tmp_28 = (tmp_26 + tmp_27);
tmp_29 = (tmp_18 + tmp_19);
res[0].im = (tmp_28 + tmp_29);
tmp_30 = (a[2].re - a[6].re);
tmp_31 = (a[0].im - a[4].im);
tmp_32 = (tmp_30 + tmp_31);
tmp_33 = (tmp_22 + tmp_23);
tmp_34 =  (C0_8_notw) * tmp_33 ;
res[1].im = (tmp_32 + tmp_34);
tmp_35 = (tmp_3 - tmp_4);
tmp_36 = (tmp_26 - tmp_27);
res[2].im = (tmp_35 + tmp_36);
tmp_37 = (tmp_31 - tmp_30);
tmp_38 = (tmp_11 - tmp_14);
tmp_39 =  (C0_8_notw) * tmp_38 ;
res[3].im = (tmp_37 + tmp_39);
res[4].im = (tmp_28 - tmp_29);
res[5].im = (tmp_32 - tmp_34);
res[6].im = (tmp_36 - tmp_35);
res[7].im = (tmp_37 - tmp_39);


memory<complex<float16_t>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<float16_t>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<float16_t>>::store(in,2 * stageLength * strideFactor , res[2]);
memory<complex<float16_t>>::store(in,3 * stageLength * strideFactor , res[3]);
memory<complex<float16_t>>::store(in,4 * stageLength * strideFactor , res[4]);
memory<complex<float16_t>>::store(in,5 * stageLength * strideFactor , res[5]);
memory<complex<float16_t>>::store(in,6 * stageLength * strideFactor , res[6]);
memory<complex<float16_t>>::store(in,7 * stageLength * strideFactor , res[7]);


};

/* Nb Multiplications = 32, 
Nb Additions = 66,
Nb Negates = 33,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_tw(int stageLength,const complex<float16_t> *w,complex<float16_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[8];
complex<float16_t> res[8];


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
 float16_t tmp_48;
 float16_t tmp_49;
 float16_t tmp_50;
 float16_t tmp_51;
 float16_t tmp_52;
 float16_t tmp_53;


a[0] = memory<complex<float16_t>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<float16_t>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<float16_t>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<float16_t>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<float16_t>>::load(in,4 * stageLength * strideFactor);
a[5] = memory<complex<float16_t>>::load(in,5 * stageLength * strideFactor);
a[6] = memory<complex<float16_t>>::load(in,6 * stageLength * strideFactor);
a[7] = memory<complex<float16_t>>::load(in,7 * stageLength * strideFactor);


tmp_0 = (a[0].re + a[4].re);
tmp_1 = (a[2].re + a[6].re);
tmp_2 = (tmp_0 + tmp_1);
tmp_3 = (a[1].re + a[5].re);
tmp_4 = (a[3].re + a[7].re);
tmp_5 = (tmp_3 + tmp_4);
res[0].re = (tmp_2 + tmp_5);
tmp_6 = (a[0].re - a[4].re);
tmp_7 = (a[2].im - a[6].im);
tmp_8 = (tmp_6 - tmp_7);
tmp_9 = (a[1].re - a[5].re);
tmp_10 = (a[1].im - a[5].im);
tmp_11 = (tmp_9 - tmp_10);
tmp_12 = (a[7].im - a[3].im);
tmp_13 = (a[3].re - a[7].re);
tmp_14 = (tmp_12 - tmp_13);
tmp_15 = (tmp_11 + tmp_14);
tmp_16 =  (C0_8_tw) * tmp_15 ;
tmp_17 = (tmp_8 + tmp_16);
tmp_18 = (a[2].re - a[6].re);
tmp_19 = (a[0].im - a[4].im);
tmp_20 = (tmp_18 + tmp_19);
tmp_21 = (tmp_9 + tmp_10);
tmp_22 = (tmp_13 + tmp_12);
tmp_23 = (tmp_21 + tmp_22);
tmp_24 =  (C0_8_tw) * tmp_23 ;
tmp_25 = (tmp_20 + tmp_24);
res[1].re = ( w[0].re * tmp_17  +  w[0].im * tmp_25 );
tmp_26 = (tmp_0 - tmp_1);
tmp_27 = (a[1].im + a[5].im);
tmp_28 = (a[3].im + a[7].im);
tmp_29 = (tmp_27 - tmp_28);
tmp_30 = (tmp_26 - tmp_29);
tmp_31 = (tmp_3 - tmp_4);
tmp_32 = (a[0].im + a[4].im);
tmp_33 = (a[2].im + a[6].im);
tmp_34 = (tmp_32 - tmp_33);
tmp_35 = (tmp_31 + tmp_34);
res[2].re = ( w[1].re * tmp_30  +  w[1].im * tmp_35 );
tmp_36 = (tmp_6 + tmp_7);
tmp_37 = (tmp_21 - tmp_22);
tmp_38 =  (C0_8_tw) * tmp_37 ;
tmp_39 = (tmp_36 - tmp_38);
tmp_40 = (tmp_19 - tmp_18);
tmp_41 = (tmp_11 - tmp_14);
tmp_42 =  (C0_8_tw) * tmp_41 ;
tmp_43 = (tmp_40 + tmp_42);
res[3].re = ( w[2].re * tmp_39  +  w[2].im * tmp_43 );
tmp_44 = (tmp_2 - tmp_5);
tmp_45 = (tmp_32 + tmp_33);
tmp_46 = (tmp_27 + tmp_28);
tmp_47 = (tmp_45 - tmp_46);
res[4].re = ( w[3].re * tmp_44  +  w[3].im * tmp_47 );
tmp_48 = (tmp_8 - tmp_16);
tmp_49 = (tmp_20 - tmp_24);
res[5].re = ( w[4].re * tmp_48  +  w[4].im * tmp_49 );
tmp_50 = (tmp_26 + tmp_29);
tmp_51 = (tmp_34 - tmp_31);
res[6].re = ( w[5].re * tmp_50  +  w[5].im * tmp_51 );
tmp_52 = (tmp_36 + tmp_38);
tmp_53 = (tmp_40 - tmp_42);
res[7].re = ( w[6].re * tmp_52  +  w[6].im * tmp_53 );
res[0].im = (tmp_45 + tmp_46);
res[1].im = ( w[0].re * tmp_25  -  w[0].im * tmp_17 );
res[2].im = ( w[1].re * tmp_35  -  w[1].im * tmp_30 );
res[3].im = ( w[2].re * tmp_43  -  w[2].im * tmp_39 );
res[4].im = ( w[3].re * tmp_47  -  w[3].im * tmp_44 );
res[5].im = ( w[4].re * tmp_49  -  w[4].im * tmp_48 );
res[6].im = ( w[5].re * tmp_51  -  w[5].im * tmp_50 );
res[7].im = ( w[6].re * tmp_53  -  w[6].im * tmp_52 );


memory<complex<float16_t>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<float16_t>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<float16_t>>::store(in,2 * stageLength * strideFactor , res[2]);
memory<complex<float16_t>>::store(in,3 * stageLength * strideFactor , res[3]);
memory<complex<float16_t>>::store(in,4 * stageLength * strideFactor , res[4]);
memory<complex<float16_t>>::store(in,5 * stageLength * strideFactor , res[5]);
memory<complex<float16_t>>::store(in,6 * stageLength * strideFactor , res[6]);
memory<complex<float16_t>>::store(in,7 * stageLength * strideFactor , res[7]);


};

/* Nb Multiplications = 4, 
Nb Additions = 52,
Nb Negates = 26,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_notw(int stageLength,const complex<float16_t> *in,complex<float16_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[8];
complex<float16_t> res[8];


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
a[5] = memory<complex<float16_t>>::load(in,5 * stageLength * strideFactor);
a[6] = memory<complex<float16_t>>::load(in,6 * stageLength * strideFactor);
a[7] = memory<complex<float16_t>>::load(in,7 * stageLength * strideFactor);


tmp_0 = (a[0].re + a[4].re);
tmp_1 = (a[2].re + a[6].re);
tmp_2 = (tmp_0 + tmp_1);
tmp_3 = (a[1].re + a[5].re);
tmp_4 = (a[3].re + a[7].re);
tmp_5 = (tmp_3 + tmp_4);
res[0].re = (tmp_2 + tmp_5);
tmp_6 = (a[0].re - a[4].re);
tmp_7 = (a[2].im - a[6].im);
tmp_8 = (tmp_6 - tmp_7);
tmp_9 = (a[1].re - a[5].re);
tmp_10 = (a[1].im - a[5].im);
tmp_11 = (tmp_9 - tmp_10);
tmp_12 = (a[7].im - a[3].im);
tmp_13 = (a[3].re - a[7].re);
tmp_14 = (tmp_12 - tmp_13);
tmp_15 = (tmp_11 + tmp_14);
tmp_16 =  (C0_8_notw) * tmp_15 ;
res[1].re = (tmp_8 + tmp_16);
tmp_17 = (tmp_0 - tmp_1);
tmp_18 = (a[1].im + a[5].im);
tmp_19 = (a[3].im + a[7].im);
tmp_20 = (tmp_18 - tmp_19);
res[2].re = (tmp_17 - tmp_20);
tmp_21 = (tmp_6 + tmp_7);
tmp_22 = (tmp_9 + tmp_10);
tmp_23 = (tmp_13 + tmp_12);
tmp_24 = (tmp_22 - tmp_23);
tmp_25 =  (C0_8_notw) * tmp_24 ;
res[3].re = (tmp_21 - tmp_25);
res[4].re = (tmp_2 - tmp_5);
res[5].re = (tmp_8 - tmp_16);
res[6].re = (tmp_17 + tmp_20);
res[7].re = (tmp_21 + tmp_25);
tmp_26 = (a[0].im + a[4].im);
tmp_27 = (a[2].im + a[6].im);
tmp_28 = (tmp_26 + tmp_27);
tmp_29 = (tmp_18 + tmp_19);
res[0].im = (tmp_28 + tmp_29);
tmp_30 = (a[2].re - a[6].re);
tmp_31 = (a[0].im - a[4].im);
tmp_32 = (tmp_30 + tmp_31);
tmp_33 = (tmp_22 + tmp_23);
tmp_34 =  (C0_8_notw) * tmp_33 ;
res[1].im = (tmp_32 + tmp_34);
tmp_35 = (tmp_3 - tmp_4);
tmp_36 = (tmp_26 - tmp_27);
res[2].im = (tmp_35 + tmp_36);
tmp_37 = (tmp_31 - tmp_30);
tmp_38 = (tmp_11 - tmp_14);
tmp_39 =  (C0_8_notw) * tmp_38 ;
res[3].im = (tmp_37 + tmp_39);
res[4].im = (tmp_28 - tmp_29);
res[5].im = (tmp_32 - tmp_34);
res[6].im = (tmp_36 - tmp_35);
res[7].im = (tmp_37 - tmp_39);


memory<complex<float16_t>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<float16_t>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<float16_t>>::store(out,2 * stageLength * strideFactor , res[2]);
memory<complex<float16_t>>::store(out,3 * stageLength * strideFactor , res[3]);
memory<complex<float16_t>>::store(out,4 * stageLength * strideFactor , res[4]);
memory<complex<float16_t>>::store(out,5 * stageLength * strideFactor , res[5]);
memory<complex<float16_t>>::store(out,6 * stageLength * strideFactor , res[6]);
memory<complex<float16_t>>::store(out,7 * stageLength * strideFactor , res[7]);


};

/* Nb Multiplications = 32, 
Nb Additions = 66,
Nb Negates = 33,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_tw(int stageLength,const complex<float16_t> *w,const complex<float16_t> *in,complex<float16_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[8];
complex<float16_t> res[8];


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
 float16_t tmp_48;
 float16_t tmp_49;
 float16_t tmp_50;
 float16_t tmp_51;
 float16_t tmp_52;
 float16_t tmp_53;


a[0] = memory<complex<float16_t>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<float16_t>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<float16_t>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<float16_t>>::load(in,3 * stageLength * strideFactor);
a[4] = memory<complex<float16_t>>::load(in,4 * stageLength * strideFactor);
a[5] = memory<complex<float16_t>>::load(in,5 * stageLength * strideFactor);
a[6] = memory<complex<float16_t>>::load(in,6 * stageLength * strideFactor);
a[7] = memory<complex<float16_t>>::load(in,7 * stageLength * strideFactor);


tmp_0 = (a[0].re + a[4].re);
tmp_1 = (a[2].re + a[6].re);
tmp_2 = (tmp_0 + tmp_1);
tmp_3 = (a[1].re + a[5].re);
tmp_4 = (a[3].re + a[7].re);
tmp_5 = (tmp_3 + tmp_4);
res[0].re = (tmp_2 + tmp_5);
tmp_6 = (a[0].re - a[4].re);
tmp_7 = (a[2].im - a[6].im);
tmp_8 = (tmp_6 - tmp_7);
tmp_9 = (a[1].re - a[5].re);
tmp_10 = (a[1].im - a[5].im);
tmp_11 = (tmp_9 - tmp_10);
tmp_12 = (a[7].im - a[3].im);
tmp_13 = (a[3].re - a[7].re);
tmp_14 = (tmp_12 - tmp_13);
tmp_15 = (tmp_11 + tmp_14);
tmp_16 =  (C0_8_tw) * tmp_15 ;
tmp_17 = (tmp_8 + tmp_16);
tmp_18 = (a[2].re - a[6].re);
tmp_19 = (a[0].im - a[4].im);
tmp_20 = (tmp_18 + tmp_19);
tmp_21 = (tmp_9 + tmp_10);
tmp_22 = (tmp_13 + tmp_12);
tmp_23 = (tmp_21 + tmp_22);
tmp_24 =  (C0_8_tw) * tmp_23 ;
tmp_25 = (tmp_20 + tmp_24);
res[1].re = ( w[0].re * tmp_17  +  w[0].im * tmp_25 );
tmp_26 = (tmp_0 - tmp_1);
tmp_27 = (a[1].im + a[5].im);
tmp_28 = (a[3].im + a[7].im);
tmp_29 = (tmp_27 - tmp_28);
tmp_30 = (tmp_26 - tmp_29);
tmp_31 = (tmp_3 - tmp_4);
tmp_32 = (a[0].im + a[4].im);
tmp_33 = (a[2].im + a[6].im);
tmp_34 = (tmp_32 - tmp_33);
tmp_35 = (tmp_31 + tmp_34);
res[2].re = ( w[1].re * tmp_30  +  w[1].im * tmp_35 );
tmp_36 = (tmp_6 + tmp_7);
tmp_37 = (tmp_21 - tmp_22);
tmp_38 =  (C0_8_tw) * tmp_37 ;
tmp_39 = (tmp_36 - tmp_38);
tmp_40 = (tmp_19 - tmp_18);
tmp_41 = (tmp_11 - tmp_14);
tmp_42 =  (C0_8_tw) * tmp_41 ;
tmp_43 = (tmp_40 + tmp_42);
res[3].re = ( w[2].re * tmp_39  +  w[2].im * tmp_43 );
tmp_44 = (tmp_2 - tmp_5);
tmp_45 = (tmp_32 + tmp_33);
tmp_46 = (tmp_27 + tmp_28);
tmp_47 = (tmp_45 - tmp_46);
res[4].re = ( w[3].re * tmp_44  +  w[3].im * tmp_47 );
tmp_48 = (tmp_8 - tmp_16);
tmp_49 = (tmp_20 - tmp_24);
res[5].re = ( w[4].re * tmp_48  +  w[4].im * tmp_49 );
tmp_50 = (tmp_26 + tmp_29);
tmp_51 = (tmp_34 - tmp_31);
res[6].re = ( w[5].re * tmp_50  +  w[5].im * tmp_51 );
tmp_52 = (tmp_36 + tmp_38);
tmp_53 = (tmp_40 - tmp_42);
res[7].re = ( w[6].re * tmp_52  +  w[6].im * tmp_53 );
res[0].im = (tmp_45 + tmp_46);
res[1].im = ( w[0].re * tmp_25  -  w[0].im * tmp_17 );
res[2].im = ( w[1].re * tmp_35  -  w[1].im * tmp_30 );
res[3].im = ( w[2].re * tmp_43  -  w[2].im * tmp_39 );
res[4].im = ( w[3].re * tmp_47  -  w[3].im * tmp_44 );
res[5].im = ( w[4].re * tmp_49  -  w[4].im * tmp_48 );
res[6].im = ( w[5].re * tmp_51  -  w[5].im * tmp_50 );
res[7].im = ( w[6].re * tmp_53  -  w[6].im * tmp_52 );


memory<complex<float16_t>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<float16_t>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<float16_t>>::store(out,2 * stageLength * strideFactor , res[2]);
memory<complex<float16_t>>::store(out,3 * stageLength * strideFactor , res[3]);
memory<complex<float16_t>>::store(out,4 * stageLength * strideFactor , res[4]);
memory<complex<float16_t>>::store(out,5 * stageLength * strideFactor , res[5]);
memory<complex<float16_t>>::store(out,6 * stageLength * strideFactor , res[6]);
memory<complex<float16_t>>::store(out,7 * stageLength * strideFactor , res[7]);


};

/* Nb Multiplications = 4, 
Nb Additions = 52,
Nb Negates = 26,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_last(complex<float16_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[8];
complex<float16_t> res[8];


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
a[5] = memory<complex<float16_t>>::load(in,5 * strideFactor);
a[6] = memory<complex<float16_t>>::load(in,6 * strideFactor);
a[7] = memory<complex<float16_t>>::load(in,7 * strideFactor);


tmp_0 = (a[0].re + a[4].re);
tmp_1 = (a[2].re + a[6].re);
tmp_2 = (tmp_0 + tmp_1);
tmp_3 = (a[1].re + a[5].re);
tmp_4 = (a[3].re + a[7].re);
tmp_5 = (tmp_3 + tmp_4);
res[0].re = (tmp_2 + tmp_5);
tmp_6 = (a[0].re - a[4].re);
tmp_7 = (a[2].im - a[6].im);
tmp_8 = (tmp_6 - tmp_7);
tmp_9 = (a[1].re - a[5].re);
tmp_10 = (a[1].im - a[5].im);
tmp_11 = (tmp_9 - tmp_10);
tmp_12 = (a[7].im - a[3].im);
tmp_13 = (a[3].re - a[7].re);
tmp_14 = (tmp_12 - tmp_13);
tmp_15 = (tmp_11 + tmp_14);
tmp_16 =  (C0_8_notw) * tmp_15 ;
res[1].re = (tmp_8 + tmp_16);
tmp_17 = (tmp_0 - tmp_1);
tmp_18 = (a[1].im + a[5].im);
tmp_19 = (a[3].im + a[7].im);
tmp_20 = (tmp_18 - tmp_19);
res[2].re = (tmp_17 - tmp_20);
tmp_21 = (tmp_6 + tmp_7);
tmp_22 = (tmp_9 + tmp_10);
tmp_23 = (tmp_13 + tmp_12);
tmp_24 = (tmp_22 - tmp_23);
tmp_25 =  (C0_8_notw) * tmp_24 ;
res[3].re = (tmp_21 - tmp_25);
res[4].re = (tmp_2 - tmp_5);
res[5].re = (tmp_8 - tmp_16);
res[6].re = (tmp_17 + tmp_20);
res[7].re = (tmp_21 + tmp_25);
tmp_26 = (a[0].im + a[4].im);
tmp_27 = (a[2].im + a[6].im);
tmp_28 = (tmp_26 + tmp_27);
tmp_29 = (tmp_18 + tmp_19);
res[0].im = (tmp_28 + tmp_29);
tmp_30 = (a[2].re - a[6].re);
tmp_31 = (a[0].im - a[4].im);
tmp_32 = (tmp_30 + tmp_31);
tmp_33 = (tmp_22 + tmp_23);
tmp_34 =  (C0_8_notw) * tmp_33 ;
res[1].im = (tmp_32 + tmp_34);
tmp_35 = (tmp_3 - tmp_4);
tmp_36 = (tmp_26 - tmp_27);
res[2].im = (tmp_35 + tmp_36);
tmp_37 = (tmp_31 - tmp_30);
tmp_38 = (tmp_11 - tmp_14);
tmp_39 =  (C0_8_notw) * tmp_38 ;
res[3].im = (tmp_37 + tmp_39);
res[4].im = (tmp_28 - tmp_29);
res[5].im = (tmp_32 - tmp_34);
res[6].im = (tmp_36 - tmp_35);
res[7].im = (tmp_37 - tmp_39);


memory<complex<float16_t>>::store(in,0 * strideFactor , res[0]);
memory<complex<float16_t>>::store(in,1 * strideFactor , res[1]);
memory<complex<float16_t>>::store(in,2 * strideFactor , res[2]);
memory<complex<float16_t>>::store(in,3 * strideFactor , res[3]);
memory<complex<float16_t>>::store(in,4 * strideFactor , res[4]);
memory<complex<float16_t>>::store(in,5 * strideFactor , res[5]);
memory<complex<float16_t>>::store(in,6 * strideFactor , res[6]);
memory<complex<float16_t>>::store(in,7 * strideFactor , res[7]);


};

/* Nb Multiplications = 4, 
Nb Additions = 52,
Nb Negates = 26,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_last(const complex<float16_t> *in,complex<float16_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[8];
complex<float16_t> res[8];


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
a[5] = memory<complex<float16_t>>::load(in,5 * strideFactor);
a[6] = memory<complex<float16_t>>::load(in,6 * strideFactor);
a[7] = memory<complex<float16_t>>::load(in,7 * strideFactor);


tmp_0 = (a[0].re + a[4].re);
tmp_1 = (a[2].re + a[6].re);
tmp_2 = (tmp_0 + tmp_1);
tmp_3 = (a[1].re + a[5].re);
tmp_4 = (a[3].re + a[7].re);
tmp_5 = (tmp_3 + tmp_4);
res[0].re = (tmp_2 + tmp_5);
tmp_6 = (a[0].re - a[4].re);
tmp_7 = (a[2].im - a[6].im);
tmp_8 = (tmp_6 - tmp_7);
tmp_9 = (a[1].re - a[5].re);
tmp_10 = (a[1].im - a[5].im);
tmp_11 = (tmp_9 - tmp_10);
tmp_12 = (a[7].im - a[3].im);
tmp_13 = (a[3].re - a[7].re);
tmp_14 = (tmp_12 - tmp_13);
tmp_15 = (tmp_11 + tmp_14);
tmp_16 =  (C0_8_notw) * tmp_15 ;
res[1].re = (tmp_8 + tmp_16);
tmp_17 = (tmp_0 - tmp_1);
tmp_18 = (a[1].im + a[5].im);
tmp_19 = (a[3].im + a[7].im);
tmp_20 = (tmp_18 - tmp_19);
res[2].re = (tmp_17 - tmp_20);
tmp_21 = (tmp_6 + tmp_7);
tmp_22 = (tmp_9 + tmp_10);
tmp_23 = (tmp_13 + tmp_12);
tmp_24 = (tmp_22 - tmp_23);
tmp_25 =  (C0_8_notw) * tmp_24 ;
res[3].re = (tmp_21 - tmp_25);
res[4].re = (tmp_2 - tmp_5);
res[5].re = (tmp_8 - tmp_16);
res[6].re = (tmp_17 + tmp_20);
res[7].re = (tmp_21 + tmp_25);
tmp_26 = (a[0].im + a[4].im);
tmp_27 = (a[2].im + a[6].im);
tmp_28 = (tmp_26 + tmp_27);
tmp_29 = (tmp_18 + tmp_19);
res[0].im = (tmp_28 + tmp_29);
tmp_30 = (a[2].re - a[6].re);
tmp_31 = (a[0].im - a[4].im);
tmp_32 = (tmp_30 + tmp_31);
tmp_33 = (tmp_22 + tmp_23);
tmp_34 =  (C0_8_notw) * tmp_33 ;
res[1].im = (tmp_32 + tmp_34);
tmp_35 = (tmp_3 - tmp_4);
tmp_36 = (tmp_26 - tmp_27);
res[2].im = (tmp_35 + tmp_36);
tmp_37 = (tmp_31 - tmp_30);
tmp_38 = (tmp_11 - tmp_14);
tmp_39 =  (C0_8_notw) * tmp_38 ;
res[3].im = (tmp_37 + tmp_39);
res[4].im = (tmp_28 - tmp_29);
res[5].im = (tmp_32 - tmp_34);
res[6].im = (tmp_36 - tmp_35);
res[7].im = (tmp_37 - tmp_39);


memory<complex<float16_t>>::store(out,0 * strideFactor , res[0]);
memory<complex<float16_t>>::store(out,1 * strideFactor , res[1]);
memory<complex<float16_t>>::store(out,2 * strideFactor , res[2]);
memory<complex<float16_t>>::store(out,3 * strideFactor , res[3]);
memory<complex<float16_t>>::store(out,4 * strideFactor , res[4]);
memory<complex<float16_t>>::store(out,5 * strideFactor , res[5]);
memory<complex<float16_t>>::store(out,6 * strideFactor , res[6]);
memory<complex<float16_t>>::store(out,7 * strideFactor , res[7]);


};

};

#endif /* #ifndef _RADIX_F16_8_H */