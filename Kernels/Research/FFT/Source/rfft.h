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

Split/merge for the RFFT

*/

#ifndef RFFT_H
#define RFFT_H



template<typename T>
void rfft_split(const rfftconfig<T> *config,const T *tmp,T* out, unsigned int n)
{
    const unsigned int halflength = n >> 1;
    const complex<T> *pA = (complex<T> *)tmp;
    const complex<T> *pB = pA + halflength;
    complex<T> *pOut =(complex<T> *)out;
    const complex<T> *pTwid =(complex<T> *)config->twiddle;
    complex<T> a;
    complex<T> b;
    complex<FFTSCI::PROD<T>> f;
    complex<FFTSCI::PROD<T>> g;

    a=pA[0];
    pOut[0] = complex<T>(a.real() + a.imag(),constant<T>::zero);

    pOut[halflength] = complex<T>(a.real() - a.imag(),constant<T>::zero);


    for(unsigned int i = 1; i < halflength; i++)
    {
       a=pA[i];
       b=conjugate(pB[-i]);

       f = widen<complex<T>>::run(a + b);
       g = pTwid[i] * (a - b);

       pOut[i] = narrow<1,complex<T>>::run(f+g);

      
    }  
}

template<typename T>
void rfft_merge(const rfftconfig<T> *config,const T *in,T* tmp, unsigned int n)
{
    const unsigned int halflength = n >> 1;
    const complex<T> *pA = (complex<T> *)in;
    const complex<T> *pB = pA + halflength;
    complex<T> *pTmp =(complex<T> *)tmp;
    const complex<T> *pTwid =(complex<T> *)config->twiddle;

    complex<T> a;
    complex<T> b;
    complex<FFTSCI::PROD<T>> f;
    complex<FFTSCI::PROD<T>> g;


    for(unsigned int i = 0; i < halflength; i++)
    {
       a=pA[i];
       b=conjugate(pB[-i]);

       f = widen<complex<T>>::run(a + b);
       g = multconj((a - b) ,(pTwid[i]));

       pTmp[i] = narrow<1,complex<T>>::run(f+g);

    }  

    
}


#endif