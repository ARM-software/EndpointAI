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

#ifndef _RADIX_Q31_4_H
#define _RADIX_Q31_4_H

#include "complex.h"

/**************

DIRECT FFT 

***************/
template<int inputstrideenabled> 
struct radix<DIRECT,Q31, 4,inputstrideenabled>{




/* Nb Multiplications = 0, 
Nb Additions = 16,
Nb Negates = 8,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 2

*/
 __STATIC_FORCEINLINE void run_ip_notw(int stageLength,complex<Q31> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q31> a[4];
complex<Q31> res[4];


 Q31 tmp_0;
 Q31 tmp_1;
 Q31 tmp_2;
 Q31 tmp_3;
 Q31 tmp_4;
 Q31 tmp_5;
 Q31 tmp_6;
 Q31 tmp_7;


a[0] = memory<complex<Q31>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q31>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q31>>::load(in,3 * stageLength * strideFactor);


tmp_0 = ( decq(a[0].re,1)   +  decq(a[2].re,1)  ) ;
tmp_1 = ( decq(a[1].re,1)   +  decq(a[3].re,1)  ) ;
res[0].re = ( decq(tmp_0,1)   +  decq(tmp_1,1)  ) ;
tmp_2 = ( decq(a[0].re,1)   -  decq(a[2].re,1)  ) ;
tmp_3 = ( decq(a[1].im,1)   -  decq(a[3].im,1)  ) ;
res[1].re = ( decq(tmp_2,1)   +  decq(tmp_3,1)  ) ;
res[2].re = ( decq(tmp_0,1)   -  decq(tmp_1,1)  ) ;
res[3].re = ( decq(tmp_2,1)   -  decq(tmp_3,1)  ) ;
tmp_4 = ( decq(a[0].im,1)   +  decq(a[2].im,1)  ) ;
tmp_5 = ( decq(a[1].im,1)   +  decq(a[3].im,1)  ) ;
res[0].im = ( decq(tmp_4,1)   +  decq(tmp_5,1)  ) ;
tmp_6 = ( decq(a[0].im,1)   -  decq(a[2].im,1)  ) ;
tmp_7 = ( decq(a[1].re,1)   -  decq(a[3].re,1)  ) ;
res[1].im = ( decq(tmp_6,1)   -  decq(tmp_7,1)  ) ;
res[2].im = ( decq(tmp_4,1)   -  decq(tmp_5,1)  ) ;
res[3].im = ( decq(tmp_7,1)   +  decq(tmp_6,1)  ) ;


memory<complex<Q31>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<Q31>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<Q31>>::store(in,2 * stageLength * strideFactor , res[2]);
memory<complex<Q31>>::store(in,3 * stageLength * strideFactor , res[3]);


};

/* Nb Multiplications = 12, 
Nb Additions = 22,
Nb Negates = 11,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 2

*/
 __STATIC_FORCEINLINE void run_ip_tw(int stageLength,const complex<Q31> *w,complex<Q31> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q31> a[4];
complex<Q31> res[4];


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


a[0] = memory<complex<Q31>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q31>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q31>>::load(in,3 * stageLength * strideFactor);


tmp_0 = ( decq(a[0].re,1)   +  decq(a[2].re,1)  ) ;
tmp_1 = ( decq(a[1].re,1)   +  decq(a[3].re,1)  ) ;
res[0].re = ( decq(tmp_0,1)   +  decq(tmp_1,1)  ) ;
tmp_2 = ( decq(a[0].re,1)   -  decq(a[2].re,1)  ) ;
tmp_3 = ( decq(a[1].im,1)   -  decq(a[3].im,1)  ) ;
tmp_4 = ( decq(tmp_2,1)   +  decq(tmp_3,1)  ) ;
tmp_5 = ( decq(a[0].im,1)   -  decq(a[2].im,1)  ) ;
tmp_6 = ( decq(a[1].re,1)   -  decq(a[3].re,1)  ) ;
tmp_7 = ( decq(tmp_5,1)   -  decq(tmp_6,1)  ) ;
res[1].re =  narrow<NORMALNARROW,Q31>::run((w[0].re * tmp_4  - w[0].im * tmp_7 ) )  ;
tmp_8 = ( decq(tmp_0,1)   -  decq(tmp_1,1)  ) ;
tmp_9 = ( decq(a[0].im,1)   +  decq(a[2].im,1)  ) ;
tmp_10 = ( decq(a[1].im,1)   +  decq(a[3].im,1)  ) ;
tmp_11 = ( decq(tmp_9,1)   -  decq(tmp_10,1)  ) ;
res[2].re =  narrow<NORMALNARROW,Q31>::run((w[1].re * tmp_8  - w[1].im * tmp_11 ) )  ;
tmp_12 = ( decq(tmp_2,1)   -  decq(tmp_3,1)  ) ;
tmp_13 = ( decq(tmp_6,1)   +  decq(tmp_5,1)  ) ;
res[3].re =  narrow<NORMALNARROW,Q31>::run((w[2].re * tmp_12  - w[2].im * tmp_13 ) )  ;
res[0].im = ( decq(tmp_9,1)   +  decq(tmp_10,1)  ) ;
res[1].im =  narrow<NORMALNARROW,Q31>::run((w[0].re * tmp_7  + w[0].im * tmp_4 ) )  ;
res[2].im =  narrow<NORMALNARROW,Q31>::run((w[1].re * tmp_11  + w[1].im * tmp_8 ) )  ;
res[3].im =  narrow<NORMALNARROW,Q31>::run((w[2].re * tmp_13  + w[2].im * tmp_12 ) )  ;


memory<complex<Q31>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<Q31>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<Q31>>::store(in,2 * stageLength * strideFactor , res[2]);
memory<complex<Q31>>::store(in,3 * stageLength * strideFactor , res[3]);


};

/* Nb Multiplications = 0, 
Nb Additions = 16,
Nb Negates = 8,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 2

*/
 __STATIC_FORCEINLINE void run_op_notw(int stageLength,const complex<Q31> *in,complex<Q31> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q31> a[4];
complex<Q31> res[4];


 Q31 tmp_0;
 Q31 tmp_1;
 Q31 tmp_2;
 Q31 tmp_3;
 Q31 tmp_4;
 Q31 tmp_5;
 Q31 tmp_6;
 Q31 tmp_7;


a[0] = memory<complex<Q31>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q31>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q31>>::load(in,3 * stageLength * strideFactor);


tmp_0 = ( decq(a[0].re,1)   +  decq(a[2].re,1)  ) ;
tmp_1 = ( decq(a[1].re,1)   +  decq(a[3].re,1)  ) ;
res[0].re = ( decq(tmp_0,1)   +  decq(tmp_1,1)  ) ;
tmp_2 = ( decq(a[0].re,1)   -  decq(a[2].re,1)  ) ;
tmp_3 = ( decq(a[1].im,1)   -  decq(a[3].im,1)  ) ;
res[1].re = ( decq(tmp_2,1)   +  decq(tmp_3,1)  ) ;
res[2].re = ( decq(tmp_0,1)   -  decq(tmp_1,1)  ) ;
res[3].re = ( decq(tmp_2,1)   -  decq(tmp_3,1)  ) ;
tmp_4 = ( decq(a[0].im,1)   +  decq(a[2].im,1)  ) ;
tmp_5 = ( decq(a[1].im,1)   +  decq(a[3].im,1)  ) ;
res[0].im = ( decq(tmp_4,1)   +  decq(tmp_5,1)  ) ;
tmp_6 = ( decq(a[0].im,1)   -  decq(a[2].im,1)  ) ;
tmp_7 = ( decq(a[1].re,1)   -  decq(a[3].re,1)  ) ;
res[1].im = ( decq(tmp_6,1)   -  decq(tmp_7,1)  ) ;
res[2].im = ( decq(tmp_4,1)   -  decq(tmp_5,1)  ) ;
res[3].im = ( decq(tmp_7,1)   +  decq(tmp_6,1)  ) ;


memory<complex<Q31>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<Q31>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<Q31>>::store(out,2 * stageLength * strideFactor , res[2]);
memory<complex<Q31>>::store(out,3 * stageLength * strideFactor , res[3]);


};

/* Nb Multiplications = 12, 
Nb Additions = 22,
Nb Negates = 11,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 2

*/
 __STATIC_FORCEINLINE void run_op_tw(int stageLength,const complex<Q31> *w,const complex<Q31> *in,complex<Q31> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q31> a[4];
complex<Q31> res[4];


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


a[0] = memory<complex<Q31>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q31>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q31>>::load(in,3 * stageLength * strideFactor);


tmp_0 = ( decq(a[0].re,1)   +  decq(a[2].re,1)  ) ;
tmp_1 = ( decq(a[1].re,1)   +  decq(a[3].re,1)  ) ;
res[0].re = ( decq(tmp_0,1)   +  decq(tmp_1,1)  ) ;
tmp_2 = ( decq(a[0].re,1)   -  decq(a[2].re,1)  ) ;
tmp_3 = ( decq(a[1].im,1)   -  decq(a[3].im,1)  ) ;
tmp_4 = ( decq(tmp_2,1)   +  decq(tmp_3,1)  ) ;
tmp_5 = ( decq(a[0].im,1)   -  decq(a[2].im,1)  ) ;
tmp_6 = ( decq(a[1].re,1)   -  decq(a[3].re,1)  ) ;
tmp_7 = ( decq(tmp_5,1)   -  decq(tmp_6,1)  ) ;
res[1].re =  narrow<NORMALNARROW,Q31>::run((w[0].re * tmp_4  - w[0].im * tmp_7 ) )  ;
tmp_8 = ( decq(tmp_0,1)   -  decq(tmp_1,1)  ) ;
tmp_9 = ( decq(a[0].im,1)   +  decq(a[2].im,1)  ) ;
tmp_10 = ( decq(a[1].im,1)   +  decq(a[3].im,1)  ) ;
tmp_11 = ( decq(tmp_9,1)   -  decq(tmp_10,1)  ) ;
res[2].re =  narrow<NORMALNARROW,Q31>::run((w[1].re * tmp_8  - w[1].im * tmp_11 ) )  ;
tmp_12 = ( decq(tmp_2,1)   -  decq(tmp_3,1)  ) ;
tmp_13 = ( decq(tmp_6,1)   +  decq(tmp_5,1)  ) ;
res[3].re =  narrow<NORMALNARROW,Q31>::run((w[2].re * tmp_12  - w[2].im * tmp_13 ) )  ;
res[0].im = ( decq(tmp_9,1)   +  decq(tmp_10,1)  ) ;
res[1].im =  narrow<NORMALNARROW,Q31>::run((w[0].re * tmp_7  + w[0].im * tmp_4 ) )  ;
res[2].im =  narrow<NORMALNARROW,Q31>::run((w[1].re * tmp_11  + w[1].im * tmp_8 ) )  ;
res[3].im =  narrow<NORMALNARROW,Q31>::run((w[2].re * tmp_13  + w[2].im * tmp_12 ) )  ;


memory<complex<Q31>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<Q31>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<Q31>>::store(out,2 * stageLength * strideFactor , res[2]);
memory<complex<Q31>>::store(out,3 * stageLength * strideFactor , res[3]);


};

/* Nb Multiplications = 0, 
Nb Additions = 16,
Nb Negates = 8,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 2

*/
 __STATIC_FORCEINLINE void run_ip_last(complex<Q31> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q31> a[4];
complex<Q31> res[4];


 Q31 tmp_0;
 Q31 tmp_1;
 Q31 tmp_2;
 Q31 tmp_3;
 Q31 tmp_4;
 Q31 tmp_5;
 Q31 tmp_6;
 Q31 tmp_7;


a[0] = memory<complex<Q31>>::load(in,0 * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * strideFactor);
a[2] = memory<complex<Q31>>::load(in,2 * strideFactor);
a[3] = memory<complex<Q31>>::load(in,3 * strideFactor);


tmp_0 = ( decq(a[0].re,1)   +  decq(a[2].re,1)  ) ;
tmp_1 = ( decq(a[1].re,1)   +  decq(a[3].re,1)  ) ;
res[0].re = ( decq(tmp_0,1)   +  decq(tmp_1,1)  ) ;
tmp_2 = ( decq(a[0].re,1)   -  decq(a[2].re,1)  ) ;
tmp_3 = ( decq(a[1].im,1)   -  decq(a[3].im,1)  ) ;
res[1].re = ( decq(tmp_2,1)   +  decq(tmp_3,1)  ) ;
res[2].re = ( decq(tmp_0,1)   -  decq(tmp_1,1)  ) ;
res[3].re = ( decq(tmp_2,1)   -  decq(tmp_3,1)  ) ;
tmp_4 = ( decq(a[0].im,1)   +  decq(a[2].im,1)  ) ;
tmp_5 = ( decq(a[1].im,1)   +  decq(a[3].im,1)  ) ;
res[0].im = ( decq(tmp_4,1)   +  decq(tmp_5,1)  ) ;
tmp_6 = ( decq(a[0].im,1)   -  decq(a[2].im,1)  ) ;
tmp_7 = ( decq(a[1].re,1)   -  decq(a[3].re,1)  ) ;
res[1].im = ( decq(tmp_6,1)   -  decq(tmp_7,1)  ) ;
res[2].im = ( decq(tmp_4,1)   -  decq(tmp_5,1)  ) ;
res[3].im = ( decq(tmp_7,1)   +  decq(tmp_6,1)  ) ;


memory<complex<Q31>>::store(in,0 * strideFactor , res[0]);
memory<complex<Q31>>::store(in,1 * strideFactor , res[1]);
memory<complex<Q31>>::store(in,2 * strideFactor , res[2]);
memory<complex<Q31>>::store(in,3 * strideFactor , res[3]);


};

/* Nb Multiplications = 0, 
Nb Additions = 16,
Nb Negates = 8,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 2

*/
 __STATIC_FORCEINLINE void run_op_last(const complex<Q31> *in,complex<Q31> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q31> a[4];
complex<Q31> res[4];


 Q31 tmp_0;
 Q31 tmp_1;
 Q31 tmp_2;
 Q31 tmp_3;
 Q31 tmp_4;
 Q31 tmp_5;
 Q31 tmp_6;
 Q31 tmp_7;


a[0] = memory<complex<Q31>>::load(in,0 * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * strideFactor);
a[2] = memory<complex<Q31>>::load(in,2 * strideFactor);
a[3] = memory<complex<Q31>>::load(in,3 * strideFactor);


tmp_0 = ( decq(a[0].re,1)   +  decq(a[2].re,1)  ) ;
tmp_1 = ( decq(a[1].re,1)   +  decq(a[3].re,1)  ) ;
res[0].re = ( decq(tmp_0,1)   +  decq(tmp_1,1)  ) ;
tmp_2 = ( decq(a[0].re,1)   -  decq(a[2].re,1)  ) ;
tmp_3 = ( decq(a[1].im,1)   -  decq(a[3].im,1)  ) ;
res[1].re = ( decq(tmp_2,1)   +  decq(tmp_3,1)  ) ;
res[2].re = ( decq(tmp_0,1)   -  decq(tmp_1,1)  ) ;
res[3].re = ( decq(tmp_2,1)   -  decq(tmp_3,1)  ) ;
tmp_4 = ( decq(a[0].im,1)   +  decq(a[2].im,1)  ) ;
tmp_5 = ( decq(a[1].im,1)   +  decq(a[3].im,1)  ) ;
res[0].im = ( decq(tmp_4,1)   +  decq(tmp_5,1)  ) ;
tmp_6 = ( decq(a[0].im,1)   -  decq(a[2].im,1)  ) ;
tmp_7 = ( decq(a[1].re,1)   -  decq(a[3].re,1)  ) ;
res[1].im = ( decq(tmp_6,1)   -  decq(tmp_7,1)  ) ;
res[2].im = ( decq(tmp_4,1)   -  decq(tmp_5,1)  ) ;
res[3].im = ( decq(tmp_7,1)   +  decq(tmp_6,1)  ) ;


memory<complex<Q31>>::store(out,0 * strideFactor , res[0]);
memory<complex<Q31>>::store(out,1 * strideFactor , res[1]);
memory<complex<Q31>>::store(out,2 * strideFactor , res[2]);
memory<complex<Q31>>::store(out,3 * strideFactor , res[3]);


};

};

/**************

INVERSE FFT 

***************/
template<int inputstrideenabled> 
struct radix<INVERSE,Q31, 4,inputstrideenabled>{




/* Nb Multiplications = 0, 
Nb Additions = 16,
Nb Negates = 8,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 2

*/
 __STATIC_FORCEINLINE void run_ip_notw(int stageLength,complex<Q31> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q31> a[4];
complex<Q31> res[4];


 Q31 tmp_0;
 Q31 tmp_1;
 Q31 tmp_2;
 Q31 tmp_3;
 Q31 tmp_4;
 Q31 tmp_5;
 Q31 tmp_6;
 Q31 tmp_7;


a[0] = memory<complex<Q31>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q31>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q31>>::load(in,3 * stageLength * strideFactor);


tmp_0 = ( decq(a[0].re,1)   +  decq(a[2].re,1)  ) ;
tmp_1 = ( decq(a[1].re,1)   +  decq(a[3].re,1)  ) ;
res[0].re = ( decq(tmp_0,1)   +  decq(tmp_1,1)  ) ;
tmp_2 = ( decq(a[0].re,1)   -  decq(a[2].re,1)  ) ;
tmp_3 = ( decq(a[1].im,1)   -  decq(a[3].im,1)  ) ;
res[1].re = ( decq(tmp_2,1)   -  decq(tmp_3,1)  ) ;
res[2].re = ( decq(tmp_0,1)   -  decq(tmp_1,1)  ) ;
res[3].re = ( decq(tmp_2,1)   +  decq(tmp_3,1)  ) ;
tmp_4 = ( decq(a[0].im,1)   +  decq(a[2].im,1)  ) ;
tmp_5 = ( decq(a[1].im,1)   +  decq(a[3].im,1)  ) ;
res[0].im = ( decq(tmp_4,1)   +  decq(tmp_5,1)  ) ;
tmp_6 = ( decq(a[1].re,1)   -  decq(a[3].re,1)  ) ;
tmp_7 = ( decq(a[0].im,1)   -  decq(a[2].im,1)  ) ;
res[1].im = ( decq(tmp_6,1)   +  decq(tmp_7,1)  ) ;
res[2].im = ( decq(tmp_4,1)   -  decq(tmp_5,1)  ) ;
res[3].im = ( decq(tmp_7,1)   -  decq(tmp_6,1)  ) ;


memory<complex<Q31>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<Q31>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<Q31>>::store(in,2 * stageLength * strideFactor , res[2]);
memory<complex<Q31>>::store(in,3 * stageLength * strideFactor , res[3]);


};

/* Nb Multiplications = 12, 
Nb Additions = 22,
Nb Negates = 11,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 2

*/
 __STATIC_FORCEINLINE void run_ip_tw(int stageLength,const complex<Q31> *w,complex<Q31> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q31> a[4];
complex<Q31> res[4];


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


a[0] = memory<complex<Q31>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q31>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q31>>::load(in,3 * stageLength * strideFactor);


tmp_0 = ( decq(a[0].re,1)   +  decq(a[2].re,1)  ) ;
tmp_1 = ( decq(a[1].re,1)   +  decq(a[3].re,1)  ) ;
res[0].re = ( decq(tmp_0,1)   +  decq(tmp_1,1)  ) ;
tmp_2 = ( decq(a[0].re,1)   -  decq(a[2].re,1)  ) ;
tmp_3 = ( decq(a[1].im,1)   -  decq(a[3].im,1)  ) ;
tmp_4 = ( decq(tmp_2,1)   -  decq(tmp_3,1)  ) ;
tmp_5 = ( decq(a[1].re,1)   -  decq(a[3].re,1)  ) ;
tmp_6 = ( decq(a[0].im,1)   -  decq(a[2].im,1)  ) ;
tmp_7 = ( decq(tmp_5,1)   +  decq(tmp_6,1)  ) ;
res[1].re =  narrow<NORMALNARROW,Q31>::run((w[0].re * tmp_4  + w[0].im * tmp_7 ) )  ;
tmp_8 = ( decq(tmp_0,1)   -  decq(tmp_1,1)  ) ;
tmp_9 = ( decq(a[0].im,1)   +  decq(a[2].im,1)  ) ;
tmp_10 = ( decq(a[1].im,1)   +  decq(a[3].im,1)  ) ;
tmp_11 = ( decq(tmp_9,1)   -  decq(tmp_10,1)  ) ;
res[2].re =  narrow<NORMALNARROW,Q31>::run((w[1].re * tmp_8  + w[1].im * tmp_11 ) )  ;
tmp_12 = ( decq(tmp_2,1)   +  decq(tmp_3,1)  ) ;
tmp_13 = ( decq(tmp_6,1)   -  decq(tmp_5,1)  ) ;
res[3].re =  narrow<NORMALNARROW,Q31>::run((w[2].re * tmp_12  + w[2].im * tmp_13 ) )  ;
res[0].im = ( decq(tmp_9,1)   +  decq(tmp_10,1)  ) ;
res[1].im =  narrow<NORMALNARROW,Q31>::run((w[0].re * tmp_7  - w[0].im * tmp_4 ) )  ;
res[2].im =  narrow<NORMALNARROW,Q31>::run((w[1].re * tmp_11  - w[1].im * tmp_8 ) )  ;
res[3].im =  narrow<NORMALNARROW,Q31>::run((w[2].re * tmp_13  - w[2].im * tmp_12 ) )  ;


memory<complex<Q31>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<Q31>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<Q31>>::store(in,2 * stageLength * strideFactor , res[2]);
memory<complex<Q31>>::store(in,3 * stageLength * strideFactor , res[3]);


};

/* Nb Multiplications = 0, 
Nb Additions = 16,
Nb Negates = 8,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 2

*/
 __STATIC_FORCEINLINE void run_op_notw(int stageLength,const complex<Q31> *in,complex<Q31> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q31> a[4];
complex<Q31> res[4];


 Q31 tmp_0;
 Q31 tmp_1;
 Q31 tmp_2;
 Q31 tmp_3;
 Q31 tmp_4;
 Q31 tmp_5;
 Q31 tmp_6;
 Q31 tmp_7;


a[0] = memory<complex<Q31>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q31>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q31>>::load(in,3 * stageLength * strideFactor);


tmp_0 = ( decq(a[0].re,1)   +  decq(a[2].re,1)  ) ;
tmp_1 = ( decq(a[1].re,1)   +  decq(a[3].re,1)  ) ;
res[0].re = ( decq(tmp_0,1)   +  decq(tmp_1,1)  ) ;
tmp_2 = ( decq(a[0].re,1)   -  decq(a[2].re,1)  ) ;
tmp_3 = ( decq(a[1].im,1)   -  decq(a[3].im,1)  ) ;
res[1].re = ( decq(tmp_2,1)   -  decq(tmp_3,1)  ) ;
res[2].re = ( decq(tmp_0,1)   -  decq(tmp_1,1)  ) ;
res[3].re = ( decq(tmp_2,1)   +  decq(tmp_3,1)  ) ;
tmp_4 = ( decq(a[0].im,1)   +  decq(a[2].im,1)  ) ;
tmp_5 = ( decq(a[1].im,1)   +  decq(a[3].im,1)  ) ;
res[0].im = ( decq(tmp_4,1)   +  decq(tmp_5,1)  ) ;
tmp_6 = ( decq(a[1].re,1)   -  decq(a[3].re,1)  ) ;
tmp_7 = ( decq(a[0].im,1)   -  decq(a[2].im,1)  ) ;
res[1].im = ( decq(tmp_6,1)   +  decq(tmp_7,1)  ) ;
res[2].im = ( decq(tmp_4,1)   -  decq(tmp_5,1)  ) ;
res[3].im = ( decq(tmp_7,1)   -  decq(tmp_6,1)  ) ;


memory<complex<Q31>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<Q31>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<Q31>>::store(out,2 * stageLength * strideFactor , res[2]);
memory<complex<Q31>>::store(out,3 * stageLength * strideFactor , res[3]);


};

/* Nb Multiplications = 12, 
Nb Additions = 22,
Nb Negates = 11,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 2

*/
 __STATIC_FORCEINLINE void run_op_tw(int stageLength,const complex<Q31> *w,const complex<Q31> *in,complex<Q31> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q31> a[4];
complex<Q31> res[4];


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


a[0] = memory<complex<Q31>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q31>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q31>>::load(in,3 * stageLength * strideFactor);


tmp_0 = ( decq(a[0].re,1)   +  decq(a[2].re,1)  ) ;
tmp_1 = ( decq(a[1].re,1)   +  decq(a[3].re,1)  ) ;
res[0].re = ( decq(tmp_0,1)   +  decq(tmp_1,1)  ) ;
tmp_2 = ( decq(a[0].re,1)   -  decq(a[2].re,1)  ) ;
tmp_3 = ( decq(a[1].im,1)   -  decq(a[3].im,1)  ) ;
tmp_4 = ( decq(tmp_2,1)   -  decq(tmp_3,1)  ) ;
tmp_5 = ( decq(a[1].re,1)   -  decq(a[3].re,1)  ) ;
tmp_6 = ( decq(a[0].im,1)   -  decq(a[2].im,1)  ) ;
tmp_7 = ( decq(tmp_5,1)   +  decq(tmp_6,1)  ) ;
res[1].re =  narrow<NORMALNARROW,Q31>::run((w[0].re * tmp_4  + w[0].im * tmp_7 ) )  ;
tmp_8 = ( decq(tmp_0,1)   -  decq(tmp_1,1)  ) ;
tmp_9 = ( decq(a[0].im,1)   +  decq(a[2].im,1)  ) ;
tmp_10 = ( decq(a[1].im,1)   +  decq(a[3].im,1)  ) ;
tmp_11 = ( decq(tmp_9,1)   -  decq(tmp_10,1)  ) ;
res[2].re =  narrow<NORMALNARROW,Q31>::run((w[1].re * tmp_8  + w[1].im * tmp_11 ) )  ;
tmp_12 = ( decq(tmp_2,1)   +  decq(tmp_3,1)  ) ;
tmp_13 = ( decq(tmp_6,1)   -  decq(tmp_5,1)  ) ;
res[3].re =  narrow<NORMALNARROW,Q31>::run((w[2].re * tmp_12  + w[2].im * tmp_13 ) )  ;
res[0].im = ( decq(tmp_9,1)   +  decq(tmp_10,1)  ) ;
res[1].im =  narrow<NORMALNARROW,Q31>::run((w[0].re * tmp_7  - w[0].im * tmp_4 ) )  ;
res[2].im =  narrow<NORMALNARROW,Q31>::run((w[1].re * tmp_11  - w[1].im * tmp_8 ) )  ;
res[3].im =  narrow<NORMALNARROW,Q31>::run((w[2].re * tmp_13  - w[2].im * tmp_12 ) )  ;


memory<complex<Q31>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<Q31>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<Q31>>::store(out,2 * stageLength * strideFactor , res[2]);
memory<complex<Q31>>::store(out,3 * stageLength * strideFactor , res[3]);


};

/* Nb Multiplications = 0, 
Nb Additions = 16,
Nb Negates = 8,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 2

*/
 __STATIC_FORCEINLINE void run_ip_last(complex<Q31> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q31> a[4];
complex<Q31> res[4];


 Q31 tmp_0;
 Q31 tmp_1;
 Q31 tmp_2;
 Q31 tmp_3;
 Q31 tmp_4;
 Q31 tmp_5;
 Q31 tmp_6;
 Q31 tmp_7;


a[0] = memory<complex<Q31>>::load(in,0 * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * strideFactor);
a[2] = memory<complex<Q31>>::load(in,2 * strideFactor);
a[3] = memory<complex<Q31>>::load(in,3 * strideFactor);


tmp_0 = ( decq(a[0].re,1)   +  decq(a[2].re,1)  ) ;
tmp_1 = ( decq(a[1].re,1)   +  decq(a[3].re,1)  ) ;
res[0].re = ( decq(tmp_0,1)   +  decq(tmp_1,1)  ) ;
tmp_2 = ( decq(a[0].re,1)   -  decq(a[2].re,1)  ) ;
tmp_3 = ( decq(a[1].im,1)   -  decq(a[3].im,1)  ) ;
res[1].re = ( decq(tmp_2,1)   -  decq(tmp_3,1)  ) ;
res[2].re = ( decq(tmp_0,1)   -  decq(tmp_1,1)  ) ;
res[3].re = ( decq(tmp_2,1)   +  decq(tmp_3,1)  ) ;
tmp_4 = ( decq(a[0].im,1)   +  decq(a[2].im,1)  ) ;
tmp_5 = ( decq(a[1].im,1)   +  decq(a[3].im,1)  ) ;
res[0].im = ( decq(tmp_4,1)   +  decq(tmp_5,1)  ) ;
tmp_6 = ( decq(a[1].re,1)   -  decq(a[3].re,1)  ) ;
tmp_7 = ( decq(a[0].im,1)   -  decq(a[2].im,1)  ) ;
res[1].im = ( decq(tmp_6,1)   +  decq(tmp_7,1)  ) ;
res[2].im = ( decq(tmp_4,1)   -  decq(tmp_5,1)  ) ;
res[3].im = ( decq(tmp_7,1)   -  decq(tmp_6,1)  ) ;


memory<complex<Q31>>::store(in,0 * strideFactor , res[0]);
memory<complex<Q31>>::store(in,1 * strideFactor , res[1]);
memory<complex<Q31>>::store(in,2 * strideFactor , res[2]);
memory<complex<Q31>>::store(in,3 * strideFactor , res[3]);


};

/* Nb Multiplications = 0, 
Nb Additions = 16,
Nb Negates = 8,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 2

*/
 __STATIC_FORCEINLINE void run_op_last(const complex<Q31> *in,complex<Q31> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q31> a[4];
complex<Q31> res[4];


 Q31 tmp_0;
 Q31 tmp_1;
 Q31 tmp_2;
 Q31 tmp_3;
 Q31 tmp_4;
 Q31 tmp_5;
 Q31 tmp_6;
 Q31 tmp_7;


a[0] = memory<complex<Q31>>::load(in,0 * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * strideFactor);
a[2] = memory<complex<Q31>>::load(in,2 * strideFactor);
a[3] = memory<complex<Q31>>::load(in,3 * strideFactor);


tmp_0 = ( decq(a[0].re,1)   +  decq(a[2].re,1)  ) ;
tmp_1 = ( decq(a[1].re,1)   +  decq(a[3].re,1)  ) ;
res[0].re = ( decq(tmp_0,1)   +  decq(tmp_1,1)  ) ;
tmp_2 = ( decq(a[0].re,1)   -  decq(a[2].re,1)  ) ;
tmp_3 = ( decq(a[1].im,1)   -  decq(a[3].im,1)  ) ;
res[1].re = ( decq(tmp_2,1)   -  decq(tmp_3,1)  ) ;
res[2].re = ( decq(tmp_0,1)   -  decq(tmp_1,1)  ) ;
res[3].re = ( decq(tmp_2,1)   +  decq(tmp_3,1)  ) ;
tmp_4 = ( decq(a[0].im,1)   +  decq(a[2].im,1)  ) ;
tmp_5 = ( decq(a[1].im,1)   +  decq(a[3].im,1)  ) ;
res[0].im = ( decq(tmp_4,1)   +  decq(tmp_5,1)  ) ;
tmp_6 = ( decq(a[1].re,1)   -  decq(a[3].re,1)  ) ;
tmp_7 = ( decq(a[0].im,1)   -  decq(a[2].im,1)  ) ;
res[1].im = ( decq(tmp_6,1)   +  decq(tmp_7,1)  ) ;
res[2].im = ( decq(tmp_4,1)   -  decq(tmp_5,1)  ) ;
res[3].im = ( decq(tmp_7,1)   -  decq(tmp_6,1)  ) ;


memory<complex<Q31>>::store(out,0 * strideFactor , res[0]);
memory<complex<Q31>>::store(out,1 * strideFactor , res[1]);
memory<complex<Q31>>::store(out,2 * strideFactor , res[2]);
memory<complex<Q31>>::store(out,3 * strideFactor , res[3]);


};

};

#endif /* #ifndef _RADIX_Q31_4_H */