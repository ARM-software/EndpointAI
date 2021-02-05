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

#ifndef _RADIX_Q15_2_H
#define _RADIX_Q15_2_H

#include "complex.h"

/**************

DIRECT FFT 

***************/
template<int inputstrideenabled> 
struct radix<DIRECT,Q15, 2,inputstrideenabled>{




/* Nb Multiplications = 0, 
Nb Additions = 2,
Nb Negates = 1,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 1

*/
 __STATIC_FORCEINLINE void run_ip_notw(int stageLength,complex<Q15> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[2];
complex<Q15> res[2];




a[0] = memory<complex<Q15>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * stageLength * strideFactor);


res[0] = shadd(a[0] , a[1]) ;
res[1] = shsub(a[0] , a[1]) ;


memory<complex<Q15>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<Q15>>::store(in,1 * stageLength * strideFactor , res[1]);


};

/* Nb Multiplications = 1, 
Nb Additions = 2,
Nb Negates = 1,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 1

*/
 __STATIC_FORCEINLINE void run_ip_tw(int stageLength,const complex<Q15> *w,complex<Q15> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[2];
complex<Q15> res[2];




a[0] = memory<complex<Q15>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * stageLength * strideFactor);


res[0] = shadd(a[0] , a[1]) ;
res[1] =  narrow<NORMALNARROW,complex<Q15>>::run(w[0] * shsub(a[0] , a[1])  )  ;


memory<complex<Q15>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<Q15>>::store(in,1 * stageLength * strideFactor , res[1]);


};

/* Nb Multiplications = 0, 
Nb Additions = 2,
Nb Negates = 1,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 1

*/
 __STATIC_FORCEINLINE void run_op_notw(int stageLength,const complex<Q15> *in,complex<Q15> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[2];
complex<Q15> res[2];




a[0] = memory<complex<Q15>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * stageLength * strideFactor);


res[0] = shadd(a[0] , a[1]) ;
res[1] = shsub(a[0] , a[1]) ;


memory<complex<Q15>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<Q15>>::store(out,1 * stageLength * strideFactor , res[1]);


};

/* Nb Multiplications = 1, 
Nb Additions = 2,
Nb Negates = 1,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 1

*/
 __STATIC_FORCEINLINE void run_op_tw(int stageLength,const complex<Q15> *w,const complex<Q15> *in,complex<Q15> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[2];
complex<Q15> res[2];




a[0] = memory<complex<Q15>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * stageLength * strideFactor);


res[0] = shadd(a[0] , a[1]) ;
res[1] =  narrow<NORMALNARROW,complex<Q15>>::run(w[0] * shsub(a[0] , a[1])  )  ;


memory<complex<Q15>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<Q15>>::store(out,1 * stageLength * strideFactor , res[1]);


};

/* Nb Multiplications = 0, 
Nb Additions = 2,
Nb Negates = 1,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 1

*/
 __STATIC_FORCEINLINE void run_ip_last(complex<Q15> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[2];
complex<Q15> res[2];




a[0] = memory<complex<Q15>>::load(in,0 * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * strideFactor);


res[0] = shadd(a[0] , a[1]) ;
res[1] = shsub(a[0] , a[1]) ;


memory<complex<Q15>>::store(in,0 * strideFactor , res[0]);
memory<complex<Q15>>::store(in,1 * strideFactor , res[1]);


};

/* Nb Multiplications = 0, 
Nb Additions = 2,
Nb Negates = 1,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 1

*/
 __STATIC_FORCEINLINE void run_op_last(const complex<Q15> *in,complex<Q15> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[2];
complex<Q15> res[2];




a[0] = memory<complex<Q15>>::load(in,0 * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * strideFactor);


res[0] = shadd(a[0] , a[1]) ;
res[1] = shsub(a[0] , a[1]) ;


memory<complex<Q15>>::store(out,0 * strideFactor , res[0]);
memory<complex<Q15>>::store(out,1 * strideFactor , res[1]);


};

};

/**************

INVERSE FFT 

***************/
template<int inputstrideenabled> 
struct radix<INVERSE,Q15, 2,inputstrideenabled>{




/* Nb Multiplications = 0, 
Nb Additions = 2,
Nb Negates = 1,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 1

*/
 __STATIC_FORCEINLINE void run_ip_notw(int stageLength,complex<Q15> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[2];
complex<Q15> res[2];




a[0] = memory<complex<Q15>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * stageLength * strideFactor);


res[0] = shadd(a[0] , a[1]) ;
res[1] = shsub(a[0] , a[1]) ;


memory<complex<Q15>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<Q15>>::store(in,1 * stageLength * strideFactor , res[1]);


};

/* Nb Multiplications = 1, 
Nb Additions = 2,
Nb Negates = 1,
Nb Conjugates = 1,
*/
/*

Output format : Shifted right by 1

*/
 __STATIC_FORCEINLINE void run_ip_tw(int stageLength,const complex<Q15> *w,complex<Q15> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[2];
complex<Q15> res[2];




a[0] = memory<complex<Q15>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * stageLength * strideFactor);


res[0] = shadd(a[0] , a[1]) ;
res[1] =  narrow<NORMALNARROW,complex<Q15>>::run(multconj(shsub(a[0] , a[1])  , w[0]) )  ;


memory<complex<Q15>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<Q15>>::store(in,1 * stageLength * strideFactor , res[1]);


};

/* Nb Multiplications = 0, 
Nb Additions = 2,
Nb Negates = 1,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 1

*/
 __STATIC_FORCEINLINE void run_op_notw(int stageLength,const complex<Q15> *in,complex<Q15> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[2];
complex<Q15> res[2];




a[0] = memory<complex<Q15>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * stageLength * strideFactor);


res[0] = shadd(a[0] , a[1]) ;
res[1] = shsub(a[0] , a[1]) ;


memory<complex<Q15>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<Q15>>::store(out,1 * stageLength * strideFactor , res[1]);


};

/* Nb Multiplications = 1, 
Nb Additions = 2,
Nb Negates = 1,
Nb Conjugates = 1,
*/
/*

Output format : Shifted right by 1

*/
 __STATIC_FORCEINLINE void run_op_tw(int stageLength,const complex<Q15> *w,const complex<Q15> *in,complex<Q15> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[2];
complex<Q15> res[2];




a[0] = memory<complex<Q15>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * stageLength * strideFactor);


res[0] = shadd(a[0] , a[1]) ;
res[1] =  narrow<NORMALNARROW,complex<Q15>>::run(multconj(shsub(a[0] , a[1])  , w[0]) )  ;


memory<complex<Q15>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<Q15>>::store(out,1 * stageLength * strideFactor , res[1]);


};

/* Nb Multiplications = 0, 
Nb Additions = 2,
Nb Negates = 1,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 1

*/
 __STATIC_FORCEINLINE void run_ip_last(complex<Q15> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[2];
complex<Q15> res[2];




a[0] = memory<complex<Q15>>::load(in,0 * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * strideFactor);


res[0] = shadd(a[0] , a[1]) ;
res[1] = shsub(a[0] , a[1]) ;


memory<complex<Q15>>::store(in,0 * strideFactor , res[0]);
memory<complex<Q15>>::store(in,1 * strideFactor , res[1]);


};

/* Nb Multiplications = 0, 
Nb Additions = 2,
Nb Negates = 1,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 1

*/
 __STATIC_FORCEINLINE void run_op_last(const complex<Q15> *in,complex<Q15> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q15> a[2];
complex<Q15> res[2];




a[0] = memory<complex<Q15>>::load(in,0 * strideFactor);
a[1] = memory<complex<Q15>>::load(in,1 * strideFactor);


res[0] = shadd(a[0] , a[1]) ;
res[1] = shsub(a[0] , a[1]) ;


memory<complex<Q15>>::store(out,0 * strideFactor , res[0]);
memory<complex<Q15>>::store(out,1 * strideFactor , res[1]);


};

};

#endif /* #ifndef _RADIX_Q15_2_H */