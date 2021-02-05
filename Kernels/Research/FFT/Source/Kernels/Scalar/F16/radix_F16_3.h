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

#ifndef _RADIX_F16_3_H
#define _RADIX_F16_3_H

#include "complex.h"

/**************

DIRECT FFT 

***************/
template<int inputstrideenabled> 
struct radix<DIRECT,float16_t, 3,inputstrideenabled>{

static constexpr float16_t C0_3_tw{0.8660254037844386f16};
  static constexpr float16_t C1_3_tw{0.5f16};
  
static constexpr float16_t C0_3_notw{0.8660254037844386f16};
  static constexpr float16_t C1_3_notw{0.5f16};
  

/* Nb Multiplications = 6, 
Nb Additions = 18,
Nb Negates = 8,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_notw(int stageLength,complex<float16_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[3];
complex<float16_t> res[3];


 float16_t tmp_0;
 float16_t tmp_1;
 float16_t tmp_2;
 float16_t tmp_3;


a[0] = memory<complex<float16_t>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<float16_t>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<float16_t>>::load(in,2 * stageLength * strideFactor);


res[0].re = (a[0].re + a[1].re + a[2].re);
tmp_0 = (a[1].re + a[2].re);
tmp_1 =  (C1_3_notw) * tmp_0 ;
res[1].re = ( (C0_3_notw) * (a[1].im - a[2].im)  + a[0].re - tmp_1);
res[2].re = ( (C0_3_notw) * (a[2].im - a[1].im)  + a[0].re - tmp_1);
res[0].im = (a[0].im + a[1].im + a[2].im);
tmp_2 = (a[1].im + a[2].im);
tmp_3 =  (C1_3_notw) * tmp_2 ;
res[1].im = ( (C0_3_notw) * (a[2].re - a[1].re)  + a[0].im - tmp_3);
res[2].im = ( (C0_3_notw) * (a[1].re - a[2].re)  + a[0].im - tmp_3);


memory<complex<float16_t>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<float16_t>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<float16_t>>::store(in,2 * stageLength * strideFactor , res[2]);


};

/* Nb Multiplications = 14, 
Nb Additions = 22,
Nb Negates = 10,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_tw(int stageLength,const complex<float16_t> *w,complex<float16_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[3];
complex<float16_t> res[3];


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


a[0] = memory<complex<float16_t>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<float16_t>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<float16_t>>::load(in,2 * stageLength * strideFactor);


res[0].re = (a[0].re + a[1].re + a[2].re);
tmp_0 = (a[1].im - a[2].im);
tmp_1 =  (C0_3_tw) * tmp_0 ;
tmp_2 = (a[1].re + a[2].re);
tmp_3 =  (C1_3_tw) * tmp_2 ;
tmp_4 = (tmp_1 + a[0].re - tmp_3);
tmp_5 = (a[2].re - a[1].re);
tmp_6 =  (C0_3_tw) * tmp_5 ;
tmp_7 = (a[1].im + a[2].im);
tmp_8 =  (C1_3_tw) * tmp_7 ;
tmp_9 = (tmp_6 + a[0].im - tmp_8);
res[1].re = ( w[0].re * tmp_4  -  w[0].im * tmp_9 );
tmp_10 = (a[2].im - a[1].im);
tmp_11 =  (C0_3_tw) * tmp_10 ;
tmp_12 = (tmp_11 + a[0].re - tmp_3);
tmp_13 = (a[1].re - a[2].re);
tmp_14 =  (C0_3_tw) * tmp_13 ;
tmp_15 = (tmp_14 + a[0].im - tmp_8);
res[2].re = ( w[1].re * tmp_12  -  w[1].im * tmp_15 );
res[0].im = (a[0].im + a[1].im + a[2].im);
res[1].im = ( w[0].re * tmp_9  +  w[0].im * tmp_4 );
res[2].im = ( w[1].re * tmp_15  +  w[1].im * tmp_12 );


memory<complex<float16_t>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<float16_t>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<float16_t>>::store(in,2 * stageLength * strideFactor , res[2]);


};

/* Nb Multiplications = 6, 
Nb Additions = 18,
Nb Negates = 8,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_notw(int stageLength,const complex<float16_t> *in,complex<float16_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[3];
complex<float16_t> res[3];


 float16_t tmp_0;
 float16_t tmp_1;
 float16_t tmp_2;
 float16_t tmp_3;


a[0] = memory<complex<float16_t>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<float16_t>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<float16_t>>::load(in,2 * stageLength * strideFactor);


res[0].re = (a[0].re + a[1].re + a[2].re);
tmp_0 = (a[1].re + a[2].re);
tmp_1 =  (C1_3_notw) * tmp_0 ;
res[1].re = ( (C0_3_notw) * (a[1].im - a[2].im)  + a[0].re - tmp_1);
res[2].re = ( (C0_3_notw) * (a[2].im - a[1].im)  + a[0].re - tmp_1);
res[0].im = (a[0].im + a[1].im + a[2].im);
tmp_2 = (a[1].im + a[2].im);
tmp_3 =  (C1_3_notw) * tmp_2 ;
res[1].im = ( (C0_3_notw) * (a[2].re - a[1].re)  + a[0].im - tmp_3);
res[2].im = ( (C0_3_notw) * (a[1].re - a[2].re)  + a[0].im - tmp_3);


memory<complex<float16_t>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<float16_t>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<float16_t>>::store(out,2 * stageLength * strideFactor , res[2]);


};

/* Nb Multiplications = 14, 
Nb Additions = 22,
Nb Negates = 10,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_tw(int stageLength,const complex<float16_t> *w,const complex<float16_t> *in,complex<float16_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[3];
complex<float16_t> res[3];


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


a[0] = memory<complex<float16_t>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<float16_t>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<float16_t>>::load(in,2 * stageLength * strideFactor);


res[0].re = (a[0].re + a[1].re + a[2].re);
tmp_0 = (a[1].im - a[2].im);
tmp_1 =  (C0_3_tw) * tmp_0 ;
tmp_2 = (a[1].re + a[2].re);
tmp_3 =  (C1_3_tw) * tmp_2 ;
tmp_4 = (tmp_1 + a[0].re - tmp_3);
tmp_5 = (a[2].re - a[1].re);
tmp_6 =  (C0_3_tw) * tmp_5 ;
tmp_7 = (a[1].im + a[2].im);
tmp_8 =  (C1_3_tw) * tmp_7 ;
tmp_9 = (tmp_6 + a[0].im - tmp_8);
res[1].re = ( w[0].re * tmp_4  -  w[0].im * tmp_9 );
tmp_10 = (a[2].im - a[1].im);
tmp_11 =  (C0_3_tw) * tmp_10 ;
tmp_12 = (tmp_11 + a[0].re - tmp_3);
tmp_13 = (a[1].re - a[2].re);
tmp_14 =  (C0_3_tw) * tmp_13 ;
tmp_15 = (tmp_14 + a[0].im - tmp_8);
res[2].re = ( w[1].re * tmp_12  -  w[1].im * tmp_15 );
res[0].im = (a[0].im + a[1].im + a[2].im);
res[1].im = ( w[0].re * tmp_9  +  w[0].im * tmp_4 );
res[2].im = ( w[1].re * tmp_15  +  w[1].im * tmp_12 );


memory<complex<float16_t>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<float16_t>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<float16_t>>::store(out,2 * stageLength * strideFactor , res[2]);


};

/* Nb Multiplications = 6, 
Nb Additions = 18,
Nb Negates = 8,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_last(complex<float16_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[3];
complex<float16_t> res[3];


 float16_t tmp_0;
 float16_t tmp_1;
 float16_t tmp_2;
 float16_t tmp_3;


a[0] = memory<complex<float16_t>>::load(in,0 * strideFactor);
a[1] = memory<complex<float16_t>>::load(in,1 * strideFactor);
a[2] = memory<complex<float16_t>>::load(in,2 * strideFactor);


res[0].re = (a[0].re + a[1].re + a[2].re);
tmp_0 = (a[1].re + a[2].re);
tmp_1 =  (C1_3_notw) * tmp_0 ;
res[1].re = ( (C0_3_notw) * (a[1].im - a[2].im)  + a[0].re - tmp_1);
res[2].re = ( (C0_3_notw) * (a[2].im - a[1].im)  + a[0].re - tmp_1);
res[0].im = (a[0].im + a[1].im + a[2].im);
tmp_2 = (a[1].im + a[2].im);
tmp_3 =  (C1_3_notw) * tmp_2 ;
res[1].im = ( (C0_3_notw) * (a[2].re - a[1].re)  + a[0].im - tmp_3);
res[2].im = ( (C0_3_notw) * (a[1].re - a[2].re)  + a[0].im - tmp_3);


memory<complex<float16_t>>::store(in,0 * strideFactor , res[0]);
memory<complex<float16_t>>::store(in,1 * strideFactor , res[1]);
memory<complex<float16_t>>::store(in,2 * strideFactor , res[2]);


};

/* Nb Multiplications = 6, 
Nb Additions = 18,
Nb Negates = 8,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_last(const complex<float16_t> *in,complex<float16_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[3];
complex<float16_t> res[3];


 float16_t tmp_0;
 float16_t tmp_1;
 float16_t tmp_2;
 float16_t tmp_3;


a[0] = memory<complex<float16_t>>::load(in,0 * strideFactor);
a[1] = memory<complex<float16_t>>::load(in,1 * strideFactor);
a[2] = memory<complex<float16_t>>::load(in,2 * strideFactor);


res[0].re = (a[0].re + a[1].re + a[2].re);
tmp_0 = (a[1].re + a[2].re);
tmp_1 =  (C1_3_notw) * tmp_0 ;
res[1].re = ( (C0_3_notw) * (a[1].im - a[2].im)  + a[0].re - tmp_1);
res[2].re = ( (C0_3_notw) * (a[2].im - a[1].im)  + a[0].re - tmp_1);
res[0].im = (a[0].im + a[1].im + a[2].im);
tmp_2 = (a[1].im + a[2].im);
tmp_3 =  (C1_3_notw) * tmp_2 ;
res[1].im = ( (C0_3_notw) * (a[2].re - a[1].re)  + a[0].im - tmp_3);
res[2].im = ( (C0_3_notw) * (a[1].re - a[2].re)  + a[0].im - tmp_3);


memory<complex<float16_t>>::store(out,0 * strideFactor , res[0]);
memory<complex<float16_t>>::store(out,1 * strideFactor , res[1]);
memory<complex<float16_t>>::store(out,2 * strideFactor , res[2]);


};

};

/**************

INVERSE FFT 

***************/
template<int inputstrideenabled> 
struct radix<INVERSE,float16_t, 3,inputstrideenabled>{

static constexpr float16_t C0_3_tw{0.8660254037844386f16};
  static constexpr float16_t C1_3_tw{0.5f16};
  
static constexpr float16_t C0_3_notw{0.8660254037844386f16};
  static constexpr float16_t C1_3_notw{0.5f16};
  

/* Nb Multiplications = 6, 
Nb Additions = 18,
Nb Negates = 8,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_notw(int stageLength,complex<float16_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[3];
complex<float16_t> res[3];


 float16_t tmp_0;
 float16_t tmp_1;
 float16_t tmp_2;
 float16_t tmp_3;


a[0] = memory<complex<float16_t>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<float16_t>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<float16_t>>::load(in,2 * stageLength * strideFactor);


res[0].re = (a[0].re + a[1].re + a[2].re);
tmp_0 = (a[1].re + a[2].re);
tmp_1 =  (C1_3_notw) * tmp_0 ;
res[1].re = ( (C0_3_notw) * (a[2].im - a[1].im)  + a[0].re - tmp_1);
res[2].re = ( (C0_3_notw) * (a[1].im - a[2].im)  + a[0].re - tmp_1);
res[0].im = (a[0].im + a[1].im + a[2].im);
tmp_2 = (a[1].im + a[2].im);
tmp_3 =  (C1_3_notw) * tmp_2 ;
res[1].im = ( (C0_3_notw) * (a[1].re - a[2].re)  + a[0].im - tmp_3);
res[2].im = ( (C0_3_notw) * (a[2].re - a[1].re)  + a[0].im - tmp_3);


memory<complex<float16_t>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<float16_t>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<float16_t>>::store(in,2 * stageLength * strideFactor , res[2]);


};

/* Nb Multiplications = 14, 
Nb Additions = 22,
Nb Negates = 10,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_tw(int stageLength,const complex<float16_t> *w,complex<float16_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[3];
complex<float16_t> res[3];


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


a[0] = memory<complex<float16_t>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<float16_t>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<float16_t>>::load(in,2 * stageLength * strideFactor);


res[0].re = (a[0].re + a[1].re + a[2].re);
tmp_0 = (a[2].im - a[1].im);
tmp_1 =  (C0_3_tw) * tmp_0 ;
tmp_2 = (a[1].re + a[2].re);
tmp_3 =  (C1_3_tw) * tmp_2 ;
tmp_4 = (tmp_1 + a[0].re - tmp_3);
tmp_5 = (a[1].re - a[2].re);
tmp_6 =  (C0_3_tw) * tmp_5 ;
tmp_7 = (a[1].im + a[2].im);
tmp_8 =  (C1_3_tw) * tmp_7 ;
tmp_9 = (tmp_6 + a[0].im - tmp_8);
res[1].re = ( w[0].re * tmp_4  +  w[0].im * tmp_9 );
tmp_10 = (a[1].im - a[2].im);
tmp_11 =  (C0_3_tw) * tmp_10 ;
tmp_12 = (tmp_11 + a[0].re - tmp_3);
tmp_13 = (a[2].re - a[1].re);
tmp_14 =  (C0_3_tw) * tmp_13 ;
tmp_15 = (tmp_14 + a[0].im - tmp_8);
res[2].re = ( w[1].re * tmp_12  +  w[1].im * tmp_15 );
res[0].im = (a[0].im + a[1].im + a[2].im);
res[1].im = ( w[0].re * tmp_9  -  w[0].im * tmp_4 );
res[2].im = ( w[1].re * tmp_15  -  w[1].im * tmp_12 );


memory<complex<float16_t>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<float16_t>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<float16_t>>::store(in,2 * stageLength * strideFactor , res[2]);


};

/* Nb Multiplications = 6, 
Nb Additions = 18,
Nb Negates = 8,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_notw(int stageLength,const complex<float16_t> *in,complex<float16_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[3];
complex<float16_t> res[3];


 float16_t tmp_0;
 float16_t tmp_1;
 float16_t tmp_2;
 float16_t tmp_3;


a[0] = memory<complex<float16_t>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<float16_t>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<float16_t>>::load(in,2 * stageLength * strideFactor);


res[0].re = (a[0].re + a[1].re + a[2].re);
tmp_0 = (a[1].re + a[2].re);
tmp_1 =  (C1_3_notw) * tmp_0 ;
res[1].re = ( (C0_3_notw) * (a[2].im - a[1].im)  + a[0].re - tmp_1);
res[2].re = ( (C0_3_notw) * (a[1].im - a[2].im)  + a[0].re - tmp_1);
res[0].im = (a[0].im + a[1].im + a[2].im);
tmp_2 = (a[1].im + a[2].im);
tmp_3 =  (C1_3_notw) * tmp_2 ;
res[1].im = ( (C0_3_notw) * (a[1].re - a[2].re)  + a[0].im - tmp_3);
res[2].im = ( (C0_3_notw) * (a[2].re - a[1].re)  + a[0].im - tmp_3);


memory<complex<float16_t>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<float16_t>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<float16_t>>::store(out,2 * stageLength * strideFactor , res[2]);


};

/* Nb Multiplications = 14, 
Nb Additions = 22,
Nb Negates = 10,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_tw(int stageLength,const complex<float16_t> *w,const complex<float16_t> *in,complex<float16_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[3];
complex<float16_t> res[3];


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


a[0] = memory<complex<float16_t>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<float16_t>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<float16_t>>::load(in,2 * stageLength * strideFactor);


res[0].re = (a[0].re + a[1].re + a[2].re);
tmp_0 = (a[2].im - a[1].im);
tmp_1 =  (C0_3_tw) * tmp_0 ;
tmp_2 = (a[1].re + a[2].re);
tmp_3 =  (C1_3_tw) * tmp_2 ;
tmp_4 = (tmp_1 + a[0].re - tmp_3);
tmp_5 = (a[1].re - a[2].re);
tmp_6 =  (C0_3_tw) * tmp_5 ;
tmp_7 = (a[1].im + a[2].im);
tmp_8 =  (C1_3_tw) * tmp_7 ;
tmp_9 = (tmp_6 + a[0].im - tmp_8);
res[1].re = ( w[0].re * tmp_4  +  w[0].im * tmp_9 );
tmp_10 = (a[1].im - a[2].im);
tmp_11 =  (C0_3_tw) * tmp_10 ;
tmp_12 = (tmp_11 + a[0].re - tmp_3);
tmp_13 = (a[2].re - a[1].re);
tmp_14 =  (C0_3_tw) * tmp_13 ;
tmp_15 = (tmp_14 + a[0].im - tmp_8);
res[2].re = ( w[1].re * tmp_12  +  w[1].im * tmp_15 );
res[0].im = (a[0].im + a[1].im + a[2].im);
res[1].im = ( w[0].re * tmp_9  -  w[0].im * tmp_4 );
res[2].im = ( w[1].re * tmp_15  -  w[1].im * tmp_12 );


memory<complex<float16_t>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<float16_t>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<float16_t>>::store(out,2 * stageLength * strideFactor , res[2]);


};

/* Nb Multiplications = 6, 
Nb Additions = 18,
Nb Negates = 8,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_last(complex<float16_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[3];
complex<float16_t> res[3];


 float16_t tmp_0;
 float16_t tmp_1;
 float16_t tmp_2;
 float16_t tmp_3;


a[0] = memory<complex<float16_t>>::load(in,0 * strideFactor);
a[1] = memory<complex<float16_t>>::load(in,1 * strideFactor);
a[2] = memory<complex<float16_t>>::load(in,2 * strideFactor);


res[0].re = (a[0].re + a[1].re + a[2].re);
tmp_0 = (a[1].re + a[2].re);
tmp_1 =  (C1_3_notw) * tmp_0 ;
res[1].re = ( (C0_3_notw) * (a[2].im - a[1].im)  + a[0].re - tmp_1);
res[2].re = ( (C0_3_notw) * (a[1].im - a[2].im)  + a[0].re - tmp_1);
res[0].im = (a[0].im + a[1].im + a[2].im);
tmp_2 = (a[1].im + a[2].im);
tmp_3 =  (C1_3_notw) * tmp_2 ;
res[1].im = ( (C0_3_notw) * (a[1].re - a[2].re)  + a[0].im - tmp_3);
res[2].im = ( (C0_3_notw) * (a[2].re - a[1].re)  + a[0].im - tmp_3);


memory<complex<float16_t>>::store(in,0 * strideFactor , res[0]);
memory<complex<float16_t>>::store(in,1 * strideFactor , res[1]);
memory<complex<float16_t>>::store(in,2 * strideFactor , res[2]);


};

/* Nb Multiplications = 6, 
Nb Additions = 18,
Nb Negates = 8,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_last(const complex<float16_t> *in,complex<float16_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[3];
complex<float16_t> res[3];


 float16_t tmp_0;
 float16_t tmp_1;
 float16_t tmp_2;
 float16_t tmp_3;


a[0] = memory<complex<float16_t>>::load(in,0 * strideFactor);
a[1] = memory<complex<float16_t>>::load(in,1 * strideFactor);
a[2] = memory<complex<float16_t>>::load(in,2 * strideFactor);


res[0].re = (a[0].re + a[1].re + a[2].re);
tmp_0 = (a[1].re + a[2].re);
tmp_1 =  (C1_3_notw) * tmp_0 ;
res[1].re = ( (C0_3_notw) * (a[2].im - a[1].im)  + a[0].re - tmp_1);
res[2].re = ( (C0_3_notw) * (a[1].im - a[2].im)  + a[0].re - tmp_1);
res[0].im = (a[0].im + a[1].im + a[2].im);
tmp_2 = (a[1].im + a[2].im);
tmp_3 =  (C1_3_notw) * tmp_2 ;
res[1].im = ( (C0_3_notw) * (a[1].re - a[2].re)  + a[0].im - tmp_3);
res[2].im = ( (C0_3_notw) * (a[2].re - a[1].re)  + a[0].im - tmp_3);


memory<complex<float16_t>>::store(out,0 * strideFactor , res[0]);
memory<complex<float16_t>>::store(out,1 * strideFactor , res[1]);
memory<complex<float16_t>>::store(out,2 * strideFactor , res[2]);


};

};

#endif /* #ifndef _RADIX_F16_3_H */