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

#ifndef _RADIX_F16_2_H
#define _RADIX_F16_2_H

#include "complex.h"

/**************

DIRECT FFT 

***************/
template<int inputstrideenabled> 
struct radix<DIRECT,float16_t, 2,inputstrideenabled>{




/* Nb Multiplications = 0, 
Nb Additions = 4,
Nb Negates = 2,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_notw(int stageLength,complex<float16_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[2];
complex<float16_t> res[2];




a[0] = memory<complex<float16_t>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<float16_t>>::load(in,1 * stageLength * strideFactor);


res[0].re = (a[0].re + a[1].re);
res[1].re = (a[0].re - a[1].re);
res[0].im = (a[0].im + a[1].im);
res[1].im = (a[0].im - a[1].im);


memory<complex<float16_t>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<float16_t>>::store(in,1 * stageLength * strideFactor , res[1]);


};

/* Nb Multiplications = 4, 
Nb Additions = 6,
Nb Negates = 3,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_tw(int stageLength,const complex<float16_t> *w,complex<float16_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[2];
complex<float16_t> res[2];


 float16_t tmp_0;
 float16_t tmp_1;


a[0] = memory<complex<float16_t>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<float16_t>>::load(in,1 * stageLength * strideFactor);


res[0].re = (a[0].re + a[1].re);
tmp_0 = (a[0].re - a[1].re);
tmp_1 = (a[0].im - a[1].im);
res[1].re = ( w[0].re * tmp_0  -  w[0].im * tmp_1 );
res[0].im = (a[0].im + a[1].im);
res[1].im = ( w[0].re * tmp_1  +  w[0].im * tmp_0 );


memory<complex<float16_t>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<float16_t>>::store(in,1 * stageLength * strideFactor , res[1]);


};

/* Nb Multiplications = 0, 
Nb Additions = 4,
Nb Negates = 2,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_notw(int stageLength,const complex<float16_t> *in,complex<float16_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[2];
complex<float16_t> res[2];




a[0] = memory<complex<float16_t>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<float16_t>>::load(in,1 * stageLength * strideFactor);


res[0].re = (a[0].re + a[1].re);
res[1].re = (a[0].re - a[1].re);
res[0].im = (a[0].im + a[1].im);
res[1].im = (a[0].im - a[1].im);


memory<complex<float16_t>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<float16_t>>::store(out,1 * stageLength * strideFactor , res[1]);


};

/* Nb Multiplications = 4, 
Nb Additions = 6,
Nb Negates = 3,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_tw(int stageLength,const complex<float16_t> *w,const complex<float16_t> *in,complex<float16_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[2];
complex<float16_t> res[2];


 float16_t tmp_0;
 float16_t tmp_1;


a[0] = memory<complex<float16_t>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<float16_t>>::load(in,1 * stageLength * strideFactor);


res[0].re = (a[0].re + a[1].re);
tmp_0 = (a[0].re - a[1].re);
tmp_1 = (a[0].im - a[1].im);
res[1].re = ( w[0].re * tmp_0  -  w[0].im * tmp_1 );
res[0].im = (a[0].im + a[1].im);
res[1].im = ( w[0].re * tmp_1  +  w[0].im * tmp_0 );


memory<complex<float16_t>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<float16_t>>::store(out,1 * stageLength * strideFactor , res[1]);


};

/* Nb Multiplications = 0, 
Nb Additions = 4,
Nb Negates = 2,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_last(complex<float16_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[2];
complex<float16_t> res[2];




a[0] = memory<complex<float16_t>>::load(in,0 * strideFactor);
a[1] = memory<complex<float16_t>>::load(in,1 * strideFactor);


res[0].re = (a[0].re + a[1].re);
res[1].re = (a[0].re - a[1].re);
res[0].im = (a[0].im + a[1].im);
res[1].im = (a[0].im - a[1].im);


memory<complex<float16_t>>::store(in,0 * strideFactor , res[0]);
memory<complex<float16_t>>::store(in,1 * strideFactor , res[1]);


};

/* Nb Multiplications = 0, 
Nb Additions = 4,
Nb Negates = 2,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_last(const complex<float16_t> *in,complex<float16_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[2];
complex<float16_t> res[2];




a[0] = memory<complex<float16_t>>::load(in,0 * strideFactor);
a[1] = memory<complex<float16_t>>::load(in,1 * strideFactor);


res[0].re = (a[0].re + a[1].re);
res[1].re = (a[0].re - a[1].re);
res[0].im = (a[0].im + a[1].im);
res[1].im = (a[0].im - a[1].im);


memory<complex<float16_t>>::store(out,0 * strideFactor , res[0]);
memory<complex<float16_t>>::store(out,1 * strideFactor , res[1]);


};

};

/**************

INVERSE FFT 

***************/
template<int inputstrideenabled> 
struct radix<INVERSE,float16_t, 2,inputstrideenabled>{




/* Nb Multiplications = 0, 
Nb Additions = 4,
Nb Negates = 2,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_notw(int stageLength,complex<float16_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[2];
complex<float16_t> res[2];




a[0] = memory<complex<float16_t>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<float16_t>>::load(in,1 * stageLength * strideFactor);


res[0].re = (a[0].re + a[1].re);
res[1].re = (a[0].re - a[1].re);
res[0].im = (a[0].im + a[1].im);
res[1].im = (a[0].im - a[1].im);


memory<complex<float16_t>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<float16_t>>::store(in,1 * stageLength * strideFactor , res[1]);


};

/* Nb Multiplications = 4, 
Nb Additions = 6,
Nb Negates = 3,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_tw(int stageLength,const complex<float16_t> *w,complex<float16_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[2];
complex<float16_t> res[2];


 float16_t tmp_0;
 float16_t tmp_1;


a[0] = memory<complex<float16_t>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<float16_t>>::load(in,1 * stageLength * strideFactor);


res[0].re = (a[0].re + a[1].re);
tmp_0 = (a[0].re - a[1].re);
tmp_1 = (a[0].im - a[1].im);
res[1].re = ( w[0].re * tmp_0  +  w[0].im * tmp_1 );
res[0].im = (a[0].im + a[1].im);
res[1].im = ( w[0].re * tmp_1  -  w[0].im * tmp_0 );


memory<complex<float16_t>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<float16_t>>::store(in,1 * stageLength * strideFactor , res[1]);


};

/* Nb Multiplications = 0, 
Nb Additions = 4,
Nb Negates = 2,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_notw(int stageLength,const complex<float16_t> *in,complex<float16_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[2];
complex<float16_t> res[2];




a[0] = memory<complex<float16_t>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<float16_t>>::load(in,1 * stageLength * strideFactor);


res[0].re = (a[0].re + a[1].re);
res[1].re = (a[0].re - a[1].re);
res[0].im = (a[0].im + a[1].im);
res[1].im = (a[0].im - a[1].im);


memory<complex<float16_t>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<float16_t>>::store(out,1 * stageLength * strideFactor , res[1]);


};

/* Nb Multiplications = 4, 
Nb Additions = 6,
Nb Negates = 3,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_tw(int stageLength,const complex<float16_t> *w,const complex<float16_t> *in,complex<float16_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[2];
complex<float16_t> res[2];


 float16_t tmp_0;
 float16_t tmp_1;


a[0] = memory<complex<float16_t>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<float16_t>>::load(in,1 * stageLength * strideFactor);


res[0].re = (a[0].re + a[1].re);
tmp_0 = (a[0].re - a[1].re);
tmp_1 = (a[0].im - a[1].im);
res[1].re = ( w[0].re * tmp_0  +  w[0].im * tmp_1 );
res[0].im = (a[0].im + a[1].im);
res[1].im = ( w[0].re * tmp_1  -  w[0].im * tmp_0 );


memory<complex<float16_t>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<float16_t>>::store(out,1 * stageLength * strideFactor , res[1]);


};

/* Nb Multiplications = 0, 
Nb Additions = 4,
Nb Negates = 2,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_ip_last(complex<float16_t> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[2];
complex<float16_t> res[2];




a[0] = memory<complex<float16_t>>::load(in,0 * strideFactor);
a[1] = memory<complex<float16_t>>::load(in,1 * strideFactor);


res[0].re = (a[0].re + a[1].re);
res[1].re = (a[0].re - a[1].re);
res[0].im = (a[0].im + a[1].im);
res[1].im = (a[0].im - a[1].im);


memory<complex<float16_t>>::store(in,0 * strideFactor , res[0]);
memory<complex<float16_t>>::store(in,1 * strideFactor , res[1]);


};

/* Nb Multiplications = 0, 
Nb Additions = 4,
Nb Negates = 2,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 0

*/
 __STATIC_FORCEINLINE void run_op_last(const complex<float16_t> *in,complex<float16_t> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<float16_t> a[2];
complex<float16_t> res[2];




a[0] = memory<complex<float16_t>>::load(in,0 * strideFactor);
a[1] = memory<complex<float16_t>>::load(in,1 * strideFactor);


res[0].re = (a[0].re + a[1].re);
res[1].re = (a[0].re - a[1].re);
res[0].im = (a[0].im + a[1].im);
res[1].im = (a[0].im - a[1].im);


memory<complex<float16_t>>::store(out,0 * strideFactor , res[0]);
memory<complex<float16_t>>::store(out,1 * strideFactor , res[1]);


};

};

#endif /* #ifndef _RADIX_F16_2_H */