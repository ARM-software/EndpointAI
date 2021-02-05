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

#ifndef _RADIX_F16_4_H
#define _RADIX_F16_4_H

#include "complex.h"

/**************

DIRECT FFT 

***************/
template<int inputstrideenabled> 
struct radix<DIRECT,float16_t, 4,inputstrideenabled>{




/* Nb Multiplications = 0, 
Nb Additions = 16,
Nb Negates = 8,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_notw(int stageLength,complex<float16_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[4];
complex<float16_t> res[4];


 float16_t tmp_0;
 float16_t tmp_1;
 float16_t tmp_2;
 float16_t tmp_3;
 float16_t tmp_4;
 float16_t tmp_5;
 float16_t tmp_6;
 float16_t tmp_7;


a[0] = memory<complex<float16_t>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<float16_t>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<float16_t>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<float16_t>>::load(in,3 * stageLength * strideFactor);


tmp_0 = (a[0].re + a[2].re);
tmp_1 = (a[1].re + a[3].re);
res[0].re = (tmp_0 + tmp_1);
tmp_2 = (a[0].re - a[2].re);
tmp_3 = (a[1].im - a[3].im);
res[1].re = (tmp_2 + tmp_3);
res[2].re = (tmp_0 - tmp_1);
res[3].re = (tmp_2 - tmp_3);
tmp_4 = (a[0].im + a[2].im);
tmp_5 = (a[1].im + a[3].im);
res[0].im = (tmp_4 + tmp_5);
tmp_6 = (a[0].im - a[2].im);
tmp_7 = (a[1].re - a[3].re);
res[1].im = (tmp_6 - tmp_7);
res[2].im = (tmp_4 - tmp_5);
res[3].im = (tmp_7 + tmp_6);


memory<complex<float16_t>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<float16_t>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<float16_t>>::store(in,2 * stageLength * strideFactor , res[2]);
memory<complex<float16_t>>::store(in,3 * stageLength * strideFactor , res[3]);


};

/* Nb Multiplications = 12, 
Nb Additions = 22,
Nb Negates = 11,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_tw(int stageLength,const complex<float16_t> *w,complex<float16_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[4];
complex<float16_t> res[4];


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


a[0] = memory<complex<float16_t>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<float16_t>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<float16_t>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<float16_t>>::load(in,3 * stageLength * strideFactor);


tmp_0 = (a[0].re + a[2].re);
tmp_1 = (a[1].re + a[3].re);
res[0].re = (tmp_0 + tmp_1);
tmp_2 = (a[0].re - a[2].re);
tmp_3 = (a[1].im - a[3].im);
tmp_4 = (tmp_2 + tmp_3);
tmp_5 = (a[0].im - a[2].im);
tmp_6 = (a[1].re - a[3].re);
tmp_7 = (tmp_5 - tmp_6);
res[1].re = ( w[0].re * tmp_4  -  w[0].im * tmp_7 );
tmp_8 = (tmp_0 - tmp_1);
tmp_9 = (a[0].im + a[2].im);
tmp_10 = (a[1].im + a[3].im);
tmp_11 = (tmp_9 - tmp_10);
res[2].re = ( w[1].re * tmp_8  -  w[1].im * tmp_11 );
tmp_12 = (tmp_2 - tmp_3);
tmp_13 = (tmp_6 + tmp_5);
res[3].re = ( w[2].re * tmp_12  -  w[2].im * tmp_13 );
res[0].im = (tmp_9 + tmp_10);
res[1].im = ( w[0].re * tmp_7  +  w[0].im * tmp_4 );
res[2].im = ( w[1].re * tmp_11  +  w[1].im * tmp_8 );
res[3].im = ( w[2].re * tmp_13  +  w[2].im * tmp_12 );


memory<complex<float16_t>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<float16_t>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<float16_t>>::store(in,2 * stageLength * strideFactor , res[2]);
memory<complex<float16_t>>::store(in,3 * stageLength * strideFactor , res[3]);


};

/* Nb Multiplications = 0, 
Nb Additions = 16,
Nb Negates = 8,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_notw(int stageLength,const complex<float16_t> *in,complex<float16_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[4];
complex<float16_t> res[4];


 float16_t tmp_0;
 float16_t tmp_1;
 float16_t tmp_2;
 float16_t tmp_3;
 float16_t tmp_4;
 float16_t tmp_5;
 float16_t tmp_6;
 float16_t tmp_7;


a[0] = memory<complex<float16_t>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<float16_t>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<float16_t>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<float16_t>>::load(in,3 * stageLength * strideFactor);


tmp_0 = (a[0].re + a[2].re);
tmp_1 = (a[1].re + a[3].re);
res[0].re = (tmp_0 + tmp_1);
tmp_2 = (a[0].re - a[2].re);
tmp_3 = (a[1].im - a[3].im);
res[1].re = (tmp_2 + tmp_3);
res[2].re = (tmp_0 - tmp_1);
res[3].re = (tmp_2 - tmp_3);
tmp_4 = (a[0].im + a[2].im);
tmp_5 = (a[1].im + a[3].im);
res[0].im = (tmp_4 + tmp_5);
tmp_6 = (a[0].im - a[2].im);
tmp_7 = (a[1].re - a[3].re);
res[1].im = (tmp_6 - tmp_7);
res[2].im = (tmp_4 - tmp_5);
res[3].im = (tmp_7 + tmp_6);


memory<complex<float16_t>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<float16_t>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<float16_t>>::store(out,2 * stageLength * strideFactor , res[2]);
memory<complex<float16_t>>::store(out,3 * stageLength * strideFactor , res[3]);


};

/* Nb Multiplications = 12, 
Nb Additions = 22,
Nb Negates = 11,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_tw(int stageLength,const complex<float16_t> *w,const complex<float16_t> *in,complex<float16_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[4];
complex<float16_t> res[4];


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


a[0] = memory<complex<float16_t>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<float16_t>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<float16_t>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<float16_t>>::load(in,3 * stageLength * strideFactor);


tmp_0 = (a[0].re + a[2].re);
tmp_1 = (a[1].re + a[3].re);
res[0].re = (tmp_0 + tmp_1);
tmp_2 = (a[0].re - a[2].re);
tmp_3 = (a[1].im - a[3].im);
tmp_4 = (tmp_2 + tmp_3);
tmp_5 = (a[0].im - a[2].im);
tmp_6 = (a[1].re - a[3].re);
tmp_7 = (tmp_5 - tmp_6);
res[1].re = ( w[0].re * tmp_4  -  w[0].im * tmp_7 );
tmp_8 = (tmp_0 - tmp_1);
tmp_9 = (a[0].im + a[2].im);
tmp_10 = (a[1].im + a[3].im);
tmp_11 = (tmp_9 - tmp_10);
res[2].re = ( w[1].re * tmp_8  -  w[1].im * tmp_11 );
tmp_12 = (tmp_2 - tmp_3);
tmp_13 = (tmp_6 + tmp_5);
res[3].re = ( w[2].re * tmp_12  -  w[2].im * tmp_13 );
res[0].im = (tmp_9 + tmp_10);
res[1].im = ( w[0].re * tmp_7  +  w[0].im * tmp_4 );
res[2].im = ( w[1].re * tmp_11  +  w[1].im * tmp_8 );
res[3].im = ( w[2].re * tmp_13  +  w[2].im * tmp_12 );


memory<complex<float16_t>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<float16_t>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<float16_t>>::store(out,2 * stageLength * strideFactor , res[2]);
memory<complex<float16_t>>::store(out,3 * stageLength * strideFactor , res[3]);


};

/* Nb Multiplications = 0, 
Nb Additions = 16,
Nb Negates = 8,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_last(complex<float16_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[4];
complex<float16_t> res[4];


 float16_t tmp_0;
 float16_t tmp_1;
 float16_t tmp_2;
 float16_t tmp_3;
 float16_t tmp_4;
 float16_t tmp_5;
 float16_t tmp_6;
 float16_t tmp_7;


a[0] = memory<complex<float16_t>>::load(in,0 * strideFactor);
a[1] = memory<complex<float16_t>>::load(in,1 * strideFactor);
a[2] = memory<complex<float16_t>>::load(in,2 * strideFactor);
a[3] = memory<complex<float16_t>>::load(in,3 * strideFactor);


tmp_0 = (a[0].re + a[2].re);
tmp_1 = (a[1].re + a[3].re);
res[0].re = (tmp_0 + tmp_1);
tmp_2 = (a[0].re - a[2].re);
tmp_3 = (a[1].im - a[3].im);
res[1].re = (tmp_2 + tmp_3);
res[2].re = (tmp_0 - tmp_1);
res[3].re = (tmp_2 - tmp_3);
tmp_4 = (a[0].im + a[2].im);
tmp_5 = (a[1].im + a[3].im);
res[0].im = (tmp_4 + tmp_5);
tmp_6 = (a[0].im - a[2].im);
tmp_7 = (a[1].re - a[3].re);
res[1].im = (tmp_6 - tmp_7);
res[2].im = (tmp_4 - tmp_5);
res[3].im = (tmp_7 + tmp_6);


memory<complex<float16_t>>::store(in,0 * strideFactor , res[0]);
memory<complex<float16_t>>::store(in,1 * strideFactor , res[1]);
memory<complex<float16_t>>::store(in,2 * strideFactor , res[2]);
memory<complex<float16_t>>::store(in,3 * strideFactor , res[3]);


};

/* Nb Multiplications = 0, 
Nb Additions = 16,
Nb Negates = 8,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_last(const complex<float16_t> *in,complex<float16_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[4];
complex<float16_t> res[4];


 float16_t tmp_0;
 float16_t tmp_1;
 float16_t tmp_2;
 float16_t tmp_3;
 float16_t tmp_4;
 float16_t tmp_5;
 float16_t tmp_6;
 float16_t tmp_7;


a[0] = memory<complex<float16_t>>::load(in,0 * strideFactor);
a[1] = memory<complex<float16_t>>::load(in,1 * strideFactor);
a[2] = memory<complex<float16_t>>::load(in,2 * strideFactor);
a[3] = memory<complex<float16_t>>::load(in,3 * strideFactor);


tmp_0 = (a[0].re + a[2].re);
tmp_1 = (a[1].re + a[3].re);
res[0].re = (tmp_0 + tmp_1);
tmp_2 = (a[0].re - a[2].re);
tmp_3 = (a[1].im - a[3].im);
res[1].re = (tmp_2 + tmp_3);
res[2].re = (tmp_0 - tmp_1);
res[3].re = (tmp_2 - tmp_3);
tmp_4 = (a[0].im + a[2].im);
tmp_5 = (a[1].im + a[3].im);
res[0].im = (tmp_4 + tmp_5);
tmp_6 = (a[0].im - a[2].im);
tmp_7 = (a[1].re - a[3].re);
res[1].im = (tmp_6 - tmp_7);
res[2].im = (tmp_4 - tmp_5);
res[3].im = (tmp_7 + tmp_6);


memory<complex<float16_t>>::store(out,0 * strideFactor , res[0]);
memory<complex<float16_t>>::store(out,1 * strideFactor , res[1]);
memory<complex<float16_t>>::store(out,2 * strideFactor , res[2]);
memory<complex<float16_t>>::store(out,3 * strideFactor , res[3]);


};

};

/**************

INVERSE FFT 

***************/
template<int inputstrideenabled> 
struct radix<INVERSE,float16_t, 4,inputstrideenabled>{




/* Nb Multiplications = 0, 
Nb Additions = 16,
Nb Negates = 8,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_notw(int stageLength,complex<float16_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[4];
complex<float16_t> res[4];


 float16_t tmp_0;
 float16_t tmp_1;
 float16_t tmp_2;
 float16_t tmp_3;
 float16_t tmp_4;
 float16_t tmp_5;
 float16_t tmp_6;
 float16_t tmp_7;


a[0] = memory<complex<float16_t>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<float16_t>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<float16_t>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<float16_t>>::load(in,3 * stageLength * strideFactor);


tmp_0 = (a[0].re + a[2].re);
tmp_1 = (a[1].re + a[3].re);
res[0].re = (tmp_0 + tmp_1);
tmp_2 = (a[0].re - a[2].re);
tmp_3 = (a[1].im - a[3].im);
res[1].re = (tmp_2 - tmp_3);
res[2].re = (tmp_0 - tmp_1);
res[3].re = (tmp_2 + tmp_3);
tmp_4 = (a[0].im + a[2].im);
tmp_5 = (a[1].im + a[3].im);
res[0].im = (tmp_4 + tmp_5);
tmp_6 = (a[1].re - a[3].re);
tmp_7 = (a[0].im - a[2].im);
res[1].im = (tmp_6 + tmp_7);
res[2].im = (tmp_4 - tmp_5);
res[3].im = (tmp_7 - tmp_6);


memory<complex<float16_t>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<float16_t>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<float16_t>>::store(in,2 * stageLength * strideFactor , res[2]);
memory<complex<float16_t>>::store(in,3 * stageLength * strideFactor , res[3]);


};

/* Nb Multiplications = 12, 
Nb Additions = 22,
Nb Negates = 11,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_tw(int stageLength,const complex<float16_t> *w,complex<float16_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[4];
complex<float16_t> res[4];


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


a[0] = memory<complex<float16_t>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<float16_t>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<float16_t>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<float16_t>>::load(in,3 * stageLength * strideFactor);


tmp_0 = (a[0].re + a[2].re);
tmp_1 = (a[1].re + a[3].re);
res[0].re = (tmp_0 + tmp_1);
tmp_2 = (a[0].re - a[2].re);
tmp_3 = (a[1].im - a[3].im);
tmp_4 = (tmp_2 - tmp_3);
tmp_5 = (a[1].re - a[3].re);
tmp_6 = (a[0].im - a[2].im);
tmp_7 = (tmp_5 + tmp_6);
res[1].re = ( w[0].re * tmp_4  +  w[0].im * tmp_7 );
tmp_8 = (tmp_0 - tmp_1);
tmp_9 = (a[0].im + a[2].im);
tmp_10 = (a[1].im + a[3].im);
tmp_11 = (tmp_9 - tmp_10);
res[2].re = ( w[1].re * tmp_8  +  w[1].im * tmp_11 );
tmp_12 = (tmp_2 + tmp_3);
tmp_13 = (tmp_6 - tmp_5);
res[3].re = ( w[2].re * tmp_12  +  w[2].im * tmp_13 );
res[0].im = (tmp_9 + tmp_10);
res[1].im = ( w[0].re * tmp_7  -  w[0].im * tmp_4 );
res[2].im = ( w[1].re * tmp_11  -  w[1].im * tmp_8 );
res[3].im = ( w[2].re * tmp_13  -  w[2].im * tmp_12 );


memory<complex<float16_t>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<float16_t>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<float16_t>>::store(in,2 * stageLength * strideFactor , res[2]);
memory<complex<float16_t>>::store(in,3 * stageLength * strideFactor , res[3]);


};

/* Nb Multiplications = 0, 
Nb Additions = 16,
Nb Negates = 8,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_notw(int stageLength,const complex<float16_t> *in,complex<float16_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[4];
complex<float16_t> res[4];


 float16_t tmp_0;
 float16_t tmp_1;
 float16_t tmp_2;
 float16_t tmp_3;
 float16_t tmp_4;
 float16_t tmp_5;
 float16_t tmp_6;
 float16_t tmp_7;


a[0] = memory<complex<float16_t>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<float16_t>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<float16_t>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<float16_t>>::load(in,3 * stageLength * strideFactor);


tmp_0 = (a[0].re + a[2].re);
tmp_1 = (a[1].re + a[3].re);
res[0].re = (tmp_0 + tmp_1);
tmp_2 = (a[0].re - a[2].re);
tmp_3 = (a[1].im - a[3].im);
res[1].re = (tmp_2 - tmp_3);
res[2].re = (tmp_0 - tmp_1);
res[3].re = (tmp_2 + tmp_3);
tmp_4 = (a[0].im + a[2].im);
tmp_5 = (a[1].im + a[3].im);
res[0].im = (tmp_4 + tmp_5);
tmp_6 = (a[1].re - a[3].re);
tmp_7 = (a[0].im - a[2].im);
res[1].im = (tmp_6 + tmp_7);
res[2].im = (tmp_4 - tmp_5);
res[3].im = (tmp_7 - tmp_6);


memory<complex<float16_t>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<float16_t>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<float16_t>>::store(out,2 * stageLength * strideFactor , res[2]);
memory<complex<float16_t>>::store(out,3 * stageLength * strideFactor , res[3]);


};

/* Nb Multiplications = 12, 
Nb Additions = 22,
Nb Negates = 11,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_tw(int stageLength,const complex<float16_t> *w,const complex<float16_t> *in,complex<float16_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[4];
complex<float16_t> res[4];


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


a[0] = memory<complex<float16_t>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<float16_t>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<float16_t>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<float16_t>>::load(in,3 * stageLength * strideFactor);


tmp_0 = (a[0].re + a[2].re);
tmp_1 = (a[1].re + a[3].re);
res[0].re = (tmp_0 + tmp_1);
tmp_2 = (a[0].re - a[2].re);
tmp_3 = (a[1].im - a[3].im);
tmp_4 = (tmp_2 - tmp_3);
tmp_5 = (a[1].re - a[3].re);
tmp_6 = (a[0].im - a[2].im);
tmp_7 = (tmp_5 + tmp_6);
res[1].re = ( w[0].re * tmp_4  +  w[0].im * tmp_7 );
tmp_8 = (tmp_0 - tmp_1);
tmp_9 = (a[0].im + a[2].im);
tmp_10 = (a[1].im + a[3].im);
tmp_11 = (tmp_9 - tmp_10);
res[2].re = ( w[1].re * tmp_8  +  w[1].im * tmp_11 );
tmp_12 = (tmp_2 + tmp_3);
tmp_13 = (tmp_6 - tmp_5);
res[3].re = ( w[2].re * tmp_12  +  w[2].im * tmp_13 );
res[0].im = (tmp_9 + tmp_10);
res[1].im = ( w[0].re * tmp_7  -  w[0].im * tmp_4 );
res[2].im = ( w[1].re * tmp_11  -  w[1].im * tmp_8 );
res[3].im = ( w[2].re * tmp_13  -  w[2].im * tmp_12 );


memory<complex<float16_t>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<float16_t>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<float16_t>>::store(out,2 * stageLength * strideFactor , res[2]);
memory<complex<float16_t>>::store(out,3 * stageLength * strideFactor , res[3]);


};

/* Nb Multiplications = 0, 
Nb Additions = 16,
Nb Negates = 8,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_last(complex<float16_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[4];
complex<float16_t> res[4];


 float16_t tmp_0;
 float16_t tmp_1;
 float16_t tmp_2;
 float16_t tmp_3;
 float16_t tmp_4;
 float16_t tmp_5;
 float16_t tmp_6;
 float16_t tmp_7;


a[0] = memory<complex<float16_t>>::load(in,0 * strideFactor);
a[1] = memory<complex<float16_t>>::load(in,1 * strideFactor);
a[2] = memory<complex<float16_t>>::load(in,2 * strideFactor);
a[3] = memory<complex<float16_t>>::load(in,3 * strideFactor);


tmp_0 = (a[0].re + a[2].re);
tmp_1 = (a[1].re + a[3].re);
res[0].re = (tmp_0 + tmp_1);
tmp_2 = (a[0].re - a[2].re);
tmp_3 = (a[1].im - a[3].im);
res[1].re = (tmp_2 - tmp_3);
res[2].re = (tmp_0 - tmp_1);
res[3].re = (tmp_2 + tmp_3);
tmp_4 = (a[0].im + a[2].im);
tmp_5 = (a[1].im + a[3].im);
res[0].im = (tmp_4 + tmp_5);
tmp_6 = (a[1].re - a[3].re);
tmp_7 = (a[0].im - a[2].im);
res[1].im = (tmp_6 + tmp_7);
res[2].im = (tmp_4 - tmp_5);
res[3].im = (tmp_7 - tmp_6);


memory<complex<float16_t>>::store(in,0 * strideFactor , res[0]);
memory<complex<float16_t>>::store(in,1 * strideFactor , res[1]);
memory<complex<float16_t>>::store(in,2 * strideFactor , res[2]);
memory<complex<float16_t>>::store(in,3 * strideFactor , res[3]);


};

/* Nb Multiplications = 0, 
Nb Additions = 16,
Nb Negates = 8,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_last(const complex<float16_t> *in,complex<float16_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[4];
complex<float16_t> res[4];


 float16_t tmp_0;
 float16_t tmp_1;
 float16_t tmp_2;
 float16_t tmp_3;
 float16_t tmp_4;
 float16_t tmp_5;
 float16_t tmp_6;
 float16_t tmp_7;


a[0] = memory<complex<float16_t>>::load(in,0 * strideFactor);
a[1] = memory<complex<float16_t>>::load(in,1 * strideFactor);
a[2] = memory<complex<float16_t>>::load(in,2 * strideFactor);
a[3] = memory<complex<float16_t>>::load(in,3 * strideFactor);


tmp_0 = (a[0].re + a[2].re);
tmp_1 = (a[1].re + a[3].re);
res[0].re = (tmp_0 + tmp_1);
tmp_2 = (a[0].re - a[2].re);
tmp_3 = (a[1].im - a[3].im);
res[1].re = (tmp_2 - tmp_3);
res[2].re = (tmp_0 - tmp_1);
res[3].re = (tmp_2 + tmp_3);
tmp_4 = (a[0].im + a[2].im);
tmp_5 = (a[1].im + a[3].im);
res[0].im = (tmp_4 + tmp_5);
tmp_6 = (a[1].re - a[3].re);
tmp_7 = (a[0].im - a[2].im);
res[1].im = (tmp_6 + tmp_7);
res[2].im = (tmp_4 - tmp_5);
res[3].im = (tmp_7 - tmp_6);


memory<complex<float16_t>>::store(out,0 * strideFactor , res[0]);
memory<complex<float16_t>>::store(out,1 * strideFactor , res[1]);
memory<complex<float16_t>>::store(out,2 * strideFactor , res[2]);
memory<complex<float16_t>>::store(out,3 * strideFactor , res[3]);


};

};

#endif /* #ifndef _RADIX_F16_4_H */