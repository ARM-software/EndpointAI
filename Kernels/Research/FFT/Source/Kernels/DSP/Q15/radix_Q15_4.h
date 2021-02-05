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

#ifndef _RADIX_Q15_4_H
#define _RADIX_Q15_4_H

#include "complex.h"

/**************

DIRECT FFT 

***************/
template<int inputstrideenabled> 
struct radix<DIRECT,Q15, 4,inputstrideenabled>{

static constexpr complex<Q15> C0_4_tw{Q15{(q15_t)0x0000}, Q15{(q15_t)0x7FFF}};
  
static constexpr complex<Q15> C0_4_notw{Q15{(q15_t)0x0000}, Q15{(q15_t)0x7FFF}};
  

/* Nb Multiplications = 1, 
Nb Additions = 8,
Nb Negates = 4,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 2

*/
 __STATIC_FORCEINLINE void run_ip_notw(int stageLength,complex<Q15> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[4];
complex<Q15> res[4];


 complex<Q15> tmp_0;
 complex<Q15> tmp_1;
 complex<Q15> tmp_2;
 complex<Q15> tmp_3;
 complex<Q15> tmp_4;


a[0] = memory<complex<Q15>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * stageLength * strideFactor);


tmp_0 = shadd(a[0] , a[2]) ;
tmp_1 = shadd(a[1] , a[3]) ;
res[0] = shadd(tmp_0 , tmp_1) ;
tmp_2 = shsub(a[0] , a[2]) ;
tmp_3 = shsub(a[1] , a[3]) ;
tmp_4 =  rot(tmp_3)  ;
res[1] = shsub(tmp_2 , tmp_4) ;
res[2] = shsub(tmp_0 , tmp_1) ;
res[3] = shadd(tmp_2 , tmp_4) ;


memory<complex<Q15>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<Q15>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<Q15>>::store(in,2 * stageLength * strideFactor , res[2]);
memory<complex<Q15>>::store(in,3 * stageLength * strideFactor , res[3]);


};

/* Nb Multiplications = 4, 
Nb Additions = 8,
Nb Negates = 4,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 2

*/
 __STATIC_FORCEINLINE void run_ip_tw(int stageLength,const complex<Q15> *w,complex<Q15> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[4];
complex<Q15> res[4];


 complex<Q15> tmp_0;
 complex<Q15> tmp_1;
 complex<Q15> tmp_2;
 complex<Q15> tmp_3;
 complex<Q15> tmp_4;


a[0] = memory<complex<Q15>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * stageLength * strideFactor);


tmp_0 = shadd(a[0] , a[2]) ;
tmp_1 = shadd(a[1] , a[3]) ;
res[0] = shadd(tmp_0 , tmp_1) ;
tmp_2 = shsub(a[0] , a[2]) ;
tmp_3 = shsub(a[1] , a[3]) ;
tmp_4 =  rot(tmp_3)  ;
res[1] =  narrow<NORMALNARROW,complex<Q15>>::run(w[0] * shsub(tmp_2 , tmp_4)  )  ;
res[2] =  narrow<NORMALNARROW,complex<Q15>>::run(w[1] * shsub(tmp_0 , tmp_1)  )  ;
res[3] =  narrow<NORMALNARROW,complex<Q15>>::run(w[2] * shadd(tmp_2 , tmp_4)  )  ;


memory<complex<Q15>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<Q15>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<Q15>>::store(in,2 * stageLength * strideFactor , res[2]);
memory<complex<Q15>>::store(in,3 * stageLength * strideFactor , res[3]);


};

/* Nb Multiplications = 1, 
Nb Additions = 8,
Nb Negates = 4,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 2

*/
 __STATIC_FORCEINLINE void run_op_notw(int stageLength,const complex<Q15> *in,complex<Q15> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[4];
complex<Q15> res[4];


 complex<Q15> tmp_0;
 complex<Q15> tmp_1;
 complex<Q15> tmp_2;
 complex<Q15> tmp_3;
 complex<Q15> tmp_4;


a[0] = memory<complex<Q15>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * stageLength * strideFactor);


tmp_0 = shadd(a[0] , a[2]) ;
tmp_1 = shadd(a[1] , a[3]) ;
res[0] = shadd(tmp_0 , tmp_1) ;
tmp_2 = shsub(a[0] , a[2]) ;
tmp_3 = shsub(a[1] , a[3]) ;
tmp_4 =  rot(tmp_3)  ;
res[1] = shsub(tmp_2 , tmp_4) ;
res[2] = shsub(tmp_0 , tmp_1) ;
res[3] = shadd(tmp_2 , tmp_4) ;


memory<complex<Q15>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<Q15>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<Q15>>::store(out,2 * stageLength * strideFactor , res[2]);
memory<complex<Q15>>::store(out,3 * stageLength * strideFactor , res[3]);


};

/* Nb Multiplications = 4, 
Nb Additions = 8,
Nb Negates = 4,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 2

*/
 __STATIC_FORCEINLINE void run_op_tw(int stageLength,const complex<Q15> *w,const complex<Q15> *in,complex<Q15> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[4];
complex<Q15> res[4];


 complex<Q15> tmp_0;
 complex<Q15> tmp_1;
 complex<Q15> tmp_2;
 complex<Q15> tmp_3;
 complex<Q15> tmp_4;


a[0] = memory<complex<Q15>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * stageLength * strideFactor);


tmp_0 = shadd(a[0] , a[2]) ;
tmp_1 = shadd(a[1] , a[3]) ;
res[0] = shadd(tmp_0 , tmp_1) ;
tmp_2 = shsub(a[0] , a[2]) ;
tmp_3 = shsub(a[1] , a[3]) ;
tmp_4 =  rot(tmp_3)  ;
res[1] =  narrow<NORMALNARROW,complex<Q15>>::run(w[0] * shsub(tmp_2 , tmp_4)  )  ;
res[2] =  narrow<NORMALNARROW,complex<Q15>>::run(w[1] * shsub(tmp_0 , tmp_1)  )  ;
res[3] =  narrow<NORMALNARROW,complex<Q15>>::run(w[2] * shadd(tmp_2 , tmp_4)  )  ;


memory<complex<Q15>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<Q15>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<Q15>>::store(out,2 * stageLength * strideFactor , res[2]);
memory<complex<Q15>>::store(out,3 * stageLength * strideFactor , res[3]);


};

/* Nb Multiplications = 1, 
Nb Additions = 8,
Nb Negates = 4,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 2

*/
 __STATIC_FORCEINLINE void run_ip_last(complex<Q15> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[4];
complex<Q15> res[4];


 complex<Q15> tmp_0;
 complex<Q15> tmp_1;
 complex<Q15> tmp_2;
 complex<Q15> tmp_3;
 complex<Q15> tmp_4;


a[0] = memory<complex<Q15>>::load(in,0 * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * strideFactor);


tmp_0 = shadd(a[0] , a[2]) ;
tmp_1 = shadd(a[1] , a[3]) ;
res[0] = shadd(tmp_0 , tmp_1) ;
tmp_2 = shsub(a[0] , a[2]) ;
tmp_3 = shsub(a[1] , a[3]) ;
tmp_4 =  rot(tmp_3)  ;
res[1] = shsub(tmp_2 , tmp_4) ;
res[2] = shsub(tmp_0 , tmp_1) ;
res[3] = shadd(tmp_2 , tmp_4) ;


memory<complex<Q15>>::store(in,0 * strideFactor , res[0]);
memory<complex<Q15>>::store(in,1 * strideFactor , res[1]);
memory<complex<Q15>>::store(in,2 * strideFactor , res[2]);
memory<complex<Q15>>::store(in,3 * strideFactor , res[3]);


};

/* Nb Multiplications = 1, 
Nb Additions = 8,
Nb Negates = 4,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 2

*/
 __STATIC_FORCEINLINE void run_op_last(const complex<Q15> *in,complex<Q15> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[4];
complex<Q15> res[4];


 complex<Q15> tmp_0;
 complex<Q15> tmp_1;
 complex<Q15> tmp_2;
 complex<Q15> tmp_3;
 complex<Q15> tmp_4;


a[0] = memory<complex<Q15>>::load(in,0 * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * strideFactor);


tmp_0 = shadd(a[0] , a[2]) ;
tmp_1 = shadd(a[1] , a[3]) ;
res[0] = shadd(tmp_0 , tmp_1) ;
tmp_2 = shsub(a[0] , a[2]) ;
tmp_3 = shsub(a[1] , a[3]) ;
tmp_4 =  rot(tmp_3)  ;
res[1] = shsub(tmp_2 , tmp_4) ;
res[2] = shsub(tmp_0 , tmp_1) ;
res[3] = shadd(tmp_2 , tmp_4) ;


memory<complex<Q15>>::store(out,0 * strideFactor , res[0]);
memory<complex<Q15>>::store(out,1 * strideFactor , res[1]);
memory<complex<Q15>>::store(out,2 * strideFactor , res[2]);
memory<complex<Q15>>::store(out,3 * strideFactor , res[3]);


};

};

/**************

INVERSE FFT 

***************/
template<int inputstrideenabled> 
struct radix<INVERSE,Q15, 4,inputstrideenabled>{

static constexpr complex<Q15> C0_4_tw{Q15{(q15_t)0x0000}, Q15{(q15_t)0x7FFF}};
  
static constexpr complex<Q15> C0_4_notw{Q15{(q15_t)0x0000}, Q15{(q15_t)0x7FFF}};
  

/* Nb Multiplications = 1, 
Nb Additions = 8,
Nb Negates = 4,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 2

*/
 __STATIC_FORCEINLINE void run_ip_notw(int stageLength,complex<Q15> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[4];
complex<Q15> res[4];


 complex<Q15> tmp_0;
 complex<Q15> tmp_1;
 complex<Q15> tmp_2;
 complex<Q15> tmp_3;
 complex<Q15> tmp_4;


a[0] = memory<complex<Q15>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * stageLength * strideFactor);


tmp_0 = shadd(a[0] , a[2]) ;
tmp_1 = shadd(a[1] , a[3]) ;
res[0] = shadd(tmp_0 , tmp_1) ;
tmp_2 = shsub(a[0] , a[2]) ;
tmp_3 = shsub(a[1] , a[3]) ;
tmp_4 =  rot(tmp_3)  ;
res[1] = shadd(tmp_2 , tmp_4) ;
res[2] = shsub(tmp_0 , tmp_1) ;
res[3] = shsub(tmp_2 , tmp_4) ;


memory<complex<Q15>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<Q15>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<Q15>>::store(in,2 * stageLength * strideFactor , res[2]);
memory<complex<Q15>>::store(in,3 * stageLength * strideFactor , res[3]);


};

/* Nb Multiplications = 4, 
Nb Additions = 8,
Nb Negates = 4,
Nb Conjugates = 3,
*/
/*

Output format : Shifted right by 2

*/
 __STATIC_FORCEINLINE void run_ip_tw(int stageLength,const complex<Q15> *w,complex<Q15> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[4];
complex<Q15> res[4];


 complex<Q15> tmp_0;
 complex<Q15> tmp_1;
 complex<Q15> tmp_2;
 complex<Q15> tmp_3;
 complex<Q15> tmp_4;


a[0] = memory<complex<Q15>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * stageLength * strideFactor);


tmp_0 = shadd(a[0] , a[2]) ;
tmp_1 = shadd(a[1] , a[3]) ;
res[0] = shadd(tmp_0 , tmp_1) ;
tmp_2 = shsub(a[0] , a[2]) ;
tmp_3 = shsub(a[1] , a[3]) ;
tmp_4 =  rot(tmp_3)  ;
res[1] =  narrow<NORMALNARROW,complex<Q15>>::run(multconj(shadd(tmp_2 , tmp_4)  , w[0]) )  ;
res[2] =  narrow<NORMALNARROW,complex<Q15>>::run(multconj(shsub(tmp_0 , tmp_1)  , w[1]) )  ;
res[3] =  narrow<NORMALNARROW,complex<Q15>>::run(multconj(shsub(tmp_2 , tmp_4)  , w[2]) )  ;


memory<complex<Q15>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<Q15>>::store(in,1 * stageLength * strideFactor , res[1]);
memory<complex<Q15>>::store(in,2 * stageLength * strideFactor , res[2]);
memory<complex<Q15>>::store(in,3 * stageLength * strideFactor , res[3]);


};

/* Nb Multiplications = 1, 
Nb Additions = 8,
Nb Negates = 4,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 2

*/
 __STATIC_FORCEINLINE void run_op_notw(int stageLength,const complex<Q15> *in,complex<Q15> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[4];
complex<Q15> res[4];


 complex<Q15> tmp_0;
 complex<Q15> tmp_1;
 complex<Q15> tmp_2;
 complex<Q15> tmp_3;
 complex<Q15> tmp_4;


a[0] = memory<complex<Q15>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * stageLength * strideFactor);


tmp_0 = shadd(a[0] , a[2]) ;
tmp_1 = shadd(a[1] , a[3]) ;
res[0] = shadd(tmp_0 , tmp_1) ;
tmp_2 = shsub(a[0] , a[2]) ;
tmp_3 = shsub(a[1] , a[3]) ;
tmp_4 =  rot(tmp_3)  ;
res[1] = shadd(tmp_2 , tmp_4) ;
res[2] = shsub(tmp_0 , tmp_1) ;
res[3] = shsub(tmp_2 , tmp_4) ;


memory<complex<Q15>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<Q15>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<Q15>>::store(out,2 * stageLength * strideFactor , res[2]);
memory<complex<Q15>>::store(out,3 * stageLength * strideFactor , res[3]);


};

/* Nb Multiplications = 4, 
Nb Additions = 8,
Nb Negates = 4,
Nb Conjugates = 3,
*/
/*

Output format : Shifted right by 2

*/
 __STATIC_FORCEINLINE void run_op_tw(int stageLength,const complex<Q15> *w,const complex<Q15> *in,complex<Q15> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[4];
complex<Q15> res[4];


 complex<Q15> tmp_0;
 complex<Q15> tmp_1;
 complex<Q15> tmp_2;
 complex<Q15> tmp_3;
 complex<Q15> tmp_4;


a[0] = memory<complex<Q15>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * stageLength * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * stageLength * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * stageLength * strideFactor);


tmp_0 = shadd(a[0] , a[2]) ;
tmp_1 = shadd(a[1] , a[3]) ;
res[0] = shadd(tmp_0 , tmp_1) ;
tmp_2 = shsub(a[0] , a[2]) ;
tmp_3 = shsub(a[1] , a[3]) ;
tmp_4 =  rot(tmp_3)  ;
res[1] =  narrow<NORMALNARROW,complex<Q15>>::run(multconj(shadd(tmp_2 , tmp_4)  , w[0]) )  ;
res[2] =  narrow<NORMALNARROW,complex<Q15>>::run(multconj(shsub(tmp_0 , tmp_1)  , w[1]) )  ;
res[3] =  narrow<NORMALNARROW,complex<Q15>>::run(multconj(shsub(tmp_2 , tmp_4)  , w[2]) )  ;


memory<complex<Q15>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<Q15>>::store(out,1 * stageLength * strideFactor , res[1]);
memory<complex<Q15>>::store(out,2 * stageLength * strideFactor , res[2]);
memory<complex<Q15>>::store(out,3 * stageLength * strideFactor , res[3]);


};

/* Nb Multiplications = 1, 
Nb Additions = 8,
Nb Negates = 4,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 2

*/
 __STATIC_FORCEINLINE void run_ip_last(complex<Q15> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[4];
complex<Q15> res[4];


 complex<Q15> tmp_0;
 complex<Q15> tmp_1;
 complex<Q15> tmp_2;
 complex<Q15> tmp_3;
 complex<Q15> tmp_4;


a[0] = memory<complex<Q15>>::load(in,0 * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * strideFactor);


tmp_0 = shadd(a[0] , a[2]) ;
tmp_1 = shadd(a[1] , a[3]) ;
res[0] = shadd(tmp_0 , tmp_1) ;
tmp_2 = shsub(a[0] , a[2]) ;
tmp_3 = shsub(a[1] , a[3]) ;
tmp_4 =  rot(tmp_3)  ;
res[1] = shadd(tmp_2 , tmp_4) ;
res[2] = shsub(tmp_0 , tmp_1) ;
res[3] = shsub(tmp_2 , tmp_4) ;


memory<complex<Q15>>::store(in,0 * strideFactor , res[0]);
memory<complex<Q15>>::store(in,1 * strideFactor , res[1]);
memory<complex<Q15>>::store(in,2 * strideFactor , res[2]);
memory<complex<Q15>>::store(in,3 * strideFactor , res[3]);


};

/* Nb Multiplications = 1, 
Nb Additions = 8,
Nb Negates = 4,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 2

*/
 __STATIC_FORCEINLINE void run_op_last(const complex<Q15> *in,complex<Q15> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[4];
complex<Q15> res[4];


 complex<Q15> tmp_0;
 complex<Q15> tmp_1;
 complex<Q15> tmp_2;
 complex<Q15> tmp_3;
 complex<Q15> tmp_4;


a[0] = memory<complex<Q15>>::load(in,0 * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * strideFactor);
a[2] = memory<complex<Q15>>::load(in,2 * strideFactor);
a[3] = memory<complex<Q15>>::load(in,3 * strideFactor);


tmp_0 = shadd(a[0] , a[2]) ;
tmp_1 = shadd(a[1] , a[3]) ;
res[0] = shadd(tmp_0 , tmp_1) ;
tmp_2 = shsub(a[0] , a[2]) ;
tmp_3 = shsub(a[1] , a[3]) ;
tmp_4 =  rot(tmp_3)  ;
res[1] = shadd(tmp_2 , tmp_4) ;
res[2] = shsub(tmp_0 , tmp_1) ;
res[3] = shsub(tmp_2 , tmp_4) ;


memory<complex<Q15>>::store(out,0 * strideFactor , res[0]);
memory<complex<Q15>>::store(out,1 * strideFactor , res[1]);
memory<complex<Q15>>::store(out,2 * strideFactor , res[2]);
memory<complex<Q15>>::store(out,3 * strideFactor , res[3]);


};

};

#endif /* #ifndef _RADIX_Q15_4_H */