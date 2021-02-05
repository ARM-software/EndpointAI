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

#ifndef _RADIX_Q31_2_H
#define _RADIX_Q31_2_H

#include "complex.h"

/**************

DIRECT FFT 

***************/
template<int inputstrideenabled> 
struct radix<DIRECT,Q31, 2,inputstrideenabled>{




/* Nb Multiplications = 0, 
Nb Additions = 4,
Nb Negates = 2,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 1

*/
 __STATIC_FORCEINLINE void run_ip_notw(int stageLength,complex<Q31> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q31> a[2];
complex<Q31> res[2];




a[0] = memory<complex<Q31>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * stageLength * strideFactor);


res[0].re = ( decq(a[0].re,1)   +  decq(a[1].re,1)  ) ;
res[1].re = ( decq(a[0].re,1)   -  decq(a[1].re,1)  ) ;
res[0].im = ( decq(a[0].im,1)   +  decq(a[1].im,1)  ) ;
res[1].im = ( decq(a[0].im,1)   -  decq(a[1].im,1)  ) ;


memory<complex<Q31>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<Q31>>::store(in,1 * stageLength * strideFactor , res[1]);


};

/* Nb Multiplications = 4, 
Nb Additions = 6,
Nb Negates = 3,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 1

*/
 __STATIC_FORCEINLINE void run_ip_tw(int stageLength,const complex<Q31> *w,complex<Q31> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q31> a[2];
complex<Q31> res[2];


 Q31 tmp_0;
 Q31 tmp_1;


a[0] = memory<complex<Q31>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * stageLength * strideFactor);


res[0].re = ( decq(a[0].re,1)   +  decq(a[1].re,1)  ) ;
tmp_0 = ( decq(a[0].re,1)   -  decq(a[1].re,1)  ) ;
tmp_1 = ( decq(a[0].im,1)   -  decq(a[1].im,1)  ) ;
res[1].re =  narrow<NORMALNARROW,Q31>::run((w[0].re * tmp_0  - w[0].im * tmp_1 ) )  ;
res[0].im = ( decq(a[0].im,1)   +  decq(a[1].im,1)  ) ;
res[1].im =  narrow<NORMALNARROW,Q31>::run((w[0].re * tmp_1  + w[0].im * tmp_0 ) )  ;


memory<complex<Q31>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<Q31>>::store(in,1 * stageLength * strideFactor , res[1]);


};

/* Nb Multiplications = 0, 
Nb Additions = 4,
Nb Negates = 2,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 1

*/
 __STATIC_FORCEINLINE void run_op_notw(int stageLength,const complex<Q31> *in,complex<Q31> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q31> a[2];
complex<Q31> res[2];




a[0] = memory<complex<Q31>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * stageLength * strideFactor);


res[0].re = ( decq(a[0].re,1)   +  decq(a[1].re,1)  ) ;
res[1].re = ( decq(a[0].re,1)   -  decq(a[1].re,1)  ) ;
res[0].im = ( decq(a[0].im,1)   +  decq(a[1].im,1)  ) ;
res[1].im = ( decq(a[0].im,1)   -  decq(a[1].im,1)  ) ;


memory<complex<Q31>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<Q31>>::store(out,1 * stageLength * strideFactor , res[1]);


};

/* Nb Multiplications = 4, 
Nb Additions = 6,
Nb Negates = 3,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 1

*/
 __STATIC_FORCEINLINE void run_op_tw(int stageLength,const complex<Q31> *w,const complex<Q31> *in,complex<Q31> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q31> a[2];
complex<Q31> res[2];


 Q31 tmp_0;
 Q31 tmp_1;


a[0] = memory<complex<Q31>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * stageLength * strideFactor);


res[0].re = ( decq(a[0].re,1)   +  decq(a[1].re,1)  ) ;
tmp_0 = ( decq(a[0].re,1)   -  decq(a[1].re,1)  ) ;
tmp_1 = ( decq(a[0].im,1)   -  decq(a[1].im,1)  ) ;
res[1].re =  narrow<NORMALNARROW,Q31>::run((w[0].re * tmp_0  - w[0].im * tmp_1 ) )  ;
res[0].im = ( decq(a[0].im,1)   +  decq(a[1].im,1)  ) ;
res[1].im =  narrow<NORMALNARROW,Q31>::run((w[0].re * tmp_1  + w[0].im * tmp_0 ) )  ;


memory<complex<Q31>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<Q31>>::store(out,1 * stageLength * strideFactor , res[1]);


};

/* Nb Multiplications = 0, 
Nb Additions = 4,
Nb Negates = 2,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 1

*/
 __STATIC_FORCEINLINE void run_ip_last(complex<Q31> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q31> a[2];
complex<Q31> res[2];




a[0] = memory<complex<Q31>>::load(in,0 * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * strideFactor);


res[0].re = ( decq(a[0].re,1)   +  decq(a[1].re,1)  ) ;
res[1].re = ( decq(a[0].re,1)   -  decq(a[1].re,1)  ) ;
res[0].im = ( decq(a[0].im,1)   +  decq(a[1].im,1)  ) ;
res[1].im = ( decq(a[0].im,1)   -  decq(a[1].im,1)  ) ;


memory<complex<Q31>>::store(in,0 * strideFactor , res[0]);
memory<complex<Q31>>::store(in,1 * strideFactor , res[1]);


};

/* Nb Multiplications = 0, 
Nb Additions = 4,
Nb Negates = 2,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 1

*/
 __STATIC_FORCEINLINE void run_op_last(const complex<Q31> *in,complex<Q31> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q31> a[2];
complex<Q31> res[2];




a[0] = memory<complex<Q31>>::load(in,0 * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * strideFactor);


res[0].re = ( decq(a[0].re,1)   +  decq(a[1].re,1)  ) ;
res[1].re = ( decq(a[0].re,1)   -  decq(a[1].re,1)  ) ;
res[0].im = ( decq(a[0].im,1)   +  decq(a[1].im,1)  ) ;
res[1].im = ( decq(a[0].im,1)   -  decq(a[1].im,1)  ) ;


memory<complex<Q31>>::store(out,0 * strideFactor , res[0]);
memory<complex<Q31>>::store(out,1 * strideFactor , res[1]);


};

};

/**************

INVERSE FFT 

***************/
template<int inputstrideenabled> 
struct radix<INVERSE,Q31, 2,inputstrideenabled>{




/* Nb Multiplications = 0, 
Nb Additions = 4,
Nb Negates = 2,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 1

*/
 __STATIC_FORCEINLINE void run_ip_notw(int stageLength,complex<Q31> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q31> a[2];
complex<Q31> res[2];




a[0] = memory<complex<Q31>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * stageLength * strideFactor);


res[0].re = ( decq(a[0].re,1)   +  decq(a[1].re,1)  ) ;
res[1].re = ( decq(a[0].re,1)   -  decq(a[1].re,1)  ) ;
res[0].im = ( decq(a[0].im,1)   +  decq(a[1].im,1)  ) ;
res[1].im = ( decq(a[0].im,1)   -  decq(a[1].im,1)  ) ;


memory<complex<Q31>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<Q31>>::store(in,1 * stageLength * strideFactor , res[1]);


};

/* Nb Multiplications = 4, 
Nb Additions = 6,
Nb Negates = 3,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 1

*/
 __STATIC_FORCEINLINE void run_ip_tw(int stageLength,const complex<Q31> *w,complex<Q31> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q31> a[2];
complex<Q31> res[2];


 Q31 tmp_0;
 Q31 tmp_1;


a[0] = memory<complex<Q31>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * stageLength * strideFactor);


res[0].re = ( decq(a[0].re,1)   +  decq(a[1].re,1)  ) ;
tmp_0 = ( decq(a[0].re,1)   -  decq(a[1].re,1)  ) ;
tmp_1 = ( decq(a[0].im,1)   -  decq(a[1].im,1)  ) ;
res[1].re =  narrow<NORMALNARROW,Q31>::run((w[0].re * tmp_0  + w[0].im * tmp_1 ) )  ;
res[0].im = ( decq(a[0].im,1)   +  decq(a[1].im,1)  ) ;
res[1].im =  narrow<NORMALNARROW,Q31>::run((w[0].re * tmp_1  - w[0].im * tmp_0 ) )  ;


memory<complex<Q31>>::store(in,0 * stageLength * strideFactor , res[0]);
memory<complex<Q31>>::store(in,1 * stageLength * strideFactor , res[1]);


};

/* Nb Multiplications = 0, 
Nb Additions = 4,
Nb Negates = 2,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 1

*/
 __STATIC_FORCEINLINE void run_op_notw(int stageLength,const complex<Q31> *in,complex<Q31> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q31> a[2];
complex<Q31> res[2];




a[0] = memory<complex<Q31>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * stageLength * strideFactor);


res[0].re = ( decq(a[0].re,1)   +  decq(a[1].re,1)  ) ;
res[1].re = ( decq(a[0].re,1)   -  decq(a[1].re,1)  ) ;
res[0].im = ( decq(a[0].im,1)   +  decq(a[1].im,1)  ) ;
res[1].im = ( decq(a[0].im,1)   -  decq(a[1].im,1)  ) ;


memory<complex<Q31>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<Q31>>::store(out,1 * stageLength * strideFactor , res[1]);


};

/* Nb Multiplications = 4, 
Nb Additions = 6,
Nb Negates = 3,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 1

*/
 __STATIC_FORCEINLINE void run_op_tw(int stageLength,const complex<Q31> *w,const complex<Q31> *in,complex<Q31> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q31> a[2];
complex<Q31> res[2];


 Q31 tmp_0;
 Q31 tmp_1;


a[0] = memory<complex<Q31>>::load(in,0 * stageLength * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * stageLength * strideFactor);


res[0].re = ( decq(a[0].re,1)   +  decq(a[1].re,1)  ) ;
tmp_0 = ( decq(a[0].re,1)   -  decq(a[1].re,1)  ) ;
tmp_1 = ( decq(a[0].im,1)   -  decq(a[1].im,1)  ) ;
res[1].re =  narrow<NORMALNARROW,Q31>::run((w[0].re * tmp_0  + w[0].im * tmp_1 ) )  ;
res[0].im = ( decq(a[0].im,1)   +  decq(a[1].im,1)  ) ;
res[1].im =  narrow<NORMALNARROW,Q31>::run((w[0].re * tmp_1  - w[0].im * tmp_0 ) )  ;


memory<complex<Q31>>::store(out,0 * stageLength * strideFactor , res[0]);
memory<complex<Q31>>::store(out,1 * stageLength * strideFactor , res[1]);


};

/* Nb Multiplications = 0, 
Nb Additions = 4,
Nb Negates = 2,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 1

*/
 __STATIC_FORCEINLINE void run_ip_last(complex<Q31> *in,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q31> a[2];
complex<Q31> res[2];




a[0] = memory<complex<Q31>>::load(in,0 * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * strideFactor);


res[0].re = ( decq(a[0].re,1)   +  decq(a[1].re,1)  ) ;
res[1].re = ( decq(a[0].re,1)   -  decq(a[1].re,1)  ) ;
res[0].im = ( decq(a[0].im,1)   +  decq(a[1].im,1)  ) ;
res[1].im = ( decq(a[0].im,1)   -  decq(a[1].im,1)  ) ;


memory<complex<Q31>>::store(in,0 * strideFactor , res[0]);
memory<complex<Q31>>::store(in,1 * strideFactor , res[1]);


};

/* Nb Multiplications = 0, 
Nb Additions = 4,
Nb Negates = 2,
Nb Conjugates = 0,
*/
/*

Output format : Shifted right by 1

*/
 __STATIC_FORCEINLINE void run_op_last(const complex<Q31> *in,complex<Q31> *out,int inputstride)

{
const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));


complex<Q31> a[2];
complex<Q31> res[2];




a[0] = memory<complex<Q31>>::load(in,0 * strideFactor);
a[1] = memory<complex<Q31>>::load(in,1 * strideFactor);


res[0].re = ( decq(a[0].re,1)   +  decq(a[1].re,1)  ) ;
res[1].re = ( decq(a[0].re,1)   -  decq(a[1].re,1)  ) ;
res[0].im = ( decq(a[0].im,1)   +  decq(a[1].im,1)  ) ;
res[1].im = ( decq(a[0].im,1)   -  decq(a[1].im,1)  ) ;


memory<complex<Q31>>::store(out,0 * strideFactor , res[0]);
memory<complex<Q31>>::store(out,1 * strideFactor , res[1]);


};

};

#endif /* #ifndef _RADIX_Q31_2_H */