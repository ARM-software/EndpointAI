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

/*


Code for the FFT permutation

*/

#ifndef REVERSAL_H
#define REVERSAL_H

#include "fft_types.h"

template<typename T,int inputstrideenabled,int vectormode>
struct bitreversal{};

/*

A FFT permutation may not always be vectorizable.
So there is an argument in the function to know if the
vector or scalar version must be used.

A FFT permutation is vectorizable only if the sequence of
transpositions are (when taken by pair) independent.

It is detected in the Python configuration code.

This is used in the vector version of the bitreversal.

Here it is the scalar only version.

*/
template<typename T,int inputstrideenabled>
struct bitreversal<T,inputstrideenabled,0>{
  __STATIC_FORCEINLINE void run(
        T *pSrc,
  const int bitRevLen,
  const uint16_t *pBitRevTab,
  const uint16_t isVectorizable,
  const int inputstride)
{
  T tmp;
  int a,b;
  int i;
  (void)isVectorizable;
  
  const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));
    

  for (i = 0; i < bitRevLen; )
  {
     /* Index in a complex array. So they have been pre-multiplied by 2
     in the Python script */
     a = pBitRevTab[i    ] ;
     b = pBitRevTab[i + 1] ;

     //real
     tmp = pSrc[a * strideFactor];
     pSrc[a * strideFactor] = pSrc[b * strideFactor];
     pSrc[b * strideFactor] = tmp;

     //complex
     tmp = pSrc[a * strideFactor + 1];
     pSrc[a * strideFactor + 1] = pSrc[b * strideFactor + 1];
     pSrc[b * strideFactor + 1] = tmp;

     i += 2;
  }
}
};


#endif


