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
#ifndef _FFTDEBUG_H

#include "fft_types.h"
#include <cstdio>

template<typename T>
void printValue(const T);

template<>
void printValue(const float32_t v)
{
    printf("%f",v);
}

template<>
void printValue(const q31_t v)
{
    printf("0x%08X",v);
}

template<>
void printValue(const q15_t v)
{
    printf("0x%04x",v);
}

template<>
void printValue(const FFTSCI::Q31 v)
{
    printValue(v.value);
}

template<>
void printValue(const FFTSCI::Q15 v)
{
    printValue(v.value);
}

template<>
void printValue(const complex<float32_t> t)
{
    if (t.imag() <0)
    {
      printf("%f - I %f",t.real(),-t.imag());
    }
    else
    {
       printf("%f + I %f",t.real(),t.imag());
    }
}

template<>
void printValue(const complex<q31_t> t)
{
    if (t.imag() <0)
    {
      printf("%f - I %f",1.0*t.real()/ 0x80000000,-1.0*t.imag()/ 0x80000000);
    }
    else
    {
       printf("%f + I %f",1.0*t.real()/ 0x80000000,1.0*t.imag()/ 0x80000000);
    }
}

template<>
void printValue(const complex<q15_t> t)
{
    if (t.imag() <0)
    {
      printf("%f - I %f",1.0*t.real()/ 0x8000,-1.0*t.imag()/ 0x8000);
    }
    else
    {
       printf("%f + I %f",1.0*t.real()/ 0x8000,1.0*t.imag()/ 0x8000);
    }
}

template<>
void printValue(const complex<FFTSCI::Q15> t)
{
    if (t.imag().value <0)
    {
      printf("%f - I %f",1.0*t.real().value/ 0x8000,-1.0*t.imag().value/ 0x8000);
    }
    else
    {
       printf("%f + I %f",1.0*t.real().value/ 0x8000,1.0*t.imag().value/ 0x8000);
    }
}

template<>
void printValue(const complex<FFTSCI::Q31> t)
{
    if (t.imag().value <0)
    {
      printf("%f - I %f",1.0*t.real().value/ 0x80000000,-1.0*t.imag().value/ 0x80000000);
    }
    else
    {
       printf("%f + I %f",1.0*t.real().value/ 0x80000000,1.0*t.imag().value/ 0x80000000);
    }
}

template<typename T>
void printMath(T *t, int n)
{
    int nb=0;
    printf("{");
    for(int i=0; i < n ; i++)
    {
        printValue(t[i]);
        if (i < n - 1)
        {
            printf(",");
        }
        nb ++;
        if (nb == 10)
        {
            nb=0;
            printf("\n");
        }
    }

    printf("}\n");
};

template<typename T>
void printMath(T *t, int rows, int cols)
{
    printf("{");
    for(int j=0; j < rows ; j++)
    {
       printMath(&t[j*cols],cols);
       if (j < rows-1)
       {
           printf(",\n ");
       }
       else
       {
         printf("\n ");
       }
    }
    printf("}\n");
};

#endif