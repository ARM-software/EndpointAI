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

/***********************

Main implementation of the FFTs

************************/

#ifndef FFT_IMPL_H
#define FFT_IMPL_H




/****

CFFT

*****/

// FFT and permutation of the output
template<typename T,int inputstrideenabled>
arm_status inner_cfft(const T *in,T* out, int n, int inputstride)
{
    const cfftconfig<T> *config=cfft_config<T>(n);
    arm_status status=ARM_MATH_SUCCESS;

    if (config)
    {

        fft<DIRECT,T,inputstrideenabled>::run(config,n,(complex<T>*)in,(complex<T>*)out,inputstride);

        bitreversal<T,inputstrideenabled,vector<T>::vector_enabled>::run(out,config->nbPerms,config->perms,config->reversalVectorizable,inputstride);
    }
    else
    {
      status=ARM_MATH_LENGTH_ERROR;
    }

    return(status);
}

template<typename T>
arm_status cfft(const T *in,T* out, uint16_t n)
{
    return(inner_cfft<T,0>(in,out,n,0));
}

template<typename T,int inputstrideenabled>
arm_status inner_cifft(const T *in,T* out, int n, int inputstride)
{
    const cfftconfig<T> *config=cfft_config<T>(n);
    arm_status status=ARM_MATH_SUCCESS;
    const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));
    


    if (config)
    {
        complex<T> *cout=(complex<T> *)out;

        fft<INVERSE,T,inputstrideenabled>::run(config,n,(complex<T>*)in,(complex<T>*)out,inputstride);

        bitreversal<T,inputstrideenabled,vector<T>::vector_enabled>::run(out,config->nbPerms,config->perms,config->reversalVectorizable,inputstride);

        /* Normalize */
        for(int i =0; i < n ; i++)
        {
           cout[i * strideFactor] = cout[i * strideFactor] * config->normalization ;
        }
    }
    else
    {
      status=ARM_MATH_LENGTH_ERROR;
    }

    return(status);

}

template<typename T>
arm_status cifft(const T *in,T* out, uint16_t n)
{
    return(inner_cifft<T,0>(in,out,n,0));
}

/****

CFFT2D

*****/


template<typename T>
arm_status cfft2D(const T *in,T* out, uint16_t rows,uint16_t cols)
{
    arm_status status=ARM_MATH_SUCCESS;

    for(int row=0; row < rows; row ++)
    {
        status=inner_cfft<T,0>(&in[2*row*cols],&out[2*row*cols],cols,0);
                   
        if (status != ARM_MATH_SUCCESS)
        {
           return(status);
        }
    }

    for(int col=0; col < cols; col ++)
    {
        status=inner_cfft<T,1>(&out[2*col],&out[2*col],rows,cols);
                   
        if (status != ARM_MATH_SUCCESS)
        {
           return(status);
        }
    }

    return(status);
}

template<typename T>
arm_status cifft2D(const T *in,T* out, uint16_t rows,uint16_t cols)
{
    arm_status status=ARM_MATH_SUCCESS;

    for(int row=0; row < rows; row ++)
    {
        status=inner_cifft<T,0>(&in[2*row*cols],&out[2*row*cols],cols,0);
                   
        if (status != ARM_MATH_SUCCESS)
        {
           return(status);
        }
    }

    for(int col=0; col < cols; col ++)
    {
        status=inner_cifft<T,1>(&out[2*col],&out[2*col],rows,cols);
                   
        if (status != ARM_MATH_SUCCESS)
        {
           return(status);
        }
    }

    return(status);
}



/****

RFFT

*****/

// Include template for the RFFT split/merge steps
#include "rfft.h"

template<typename T>
arm_status rfft(const T *in,T* tmp,T* out, uint16_t n)
{
    const rfftconfig<T> *config=rfft_config<T>(n);
    arm_status status=ARM_MATH_SUCCESS;

    if (config)
    {

        status=inner_cfft<T,0>(in,tmp,n>>1,0);
        if (status == ARM_MATH_SUCCESS)
        {
            rfft_split(config,tmp,out,n);
        }
    }
    else
    {
      status=ARM_MATH_LENGTH_ERROR;
    }

    return(status);
}



template<typename T>
arm_status rifft(const T *in,T* tmp,T* out, uint16_t n)
{
    const rfftconfig<T> *config=rfft_config<T>(n);
    arm_status status=ARM_MATH_SUCCESS;

    if (config)
    {

        rfft_merge(config,in,tmp,n);
        status=inner_cifft<T,0>(tmp,out,n>>1,0);
    }
    else
    {
      status=ARM_MATH_LENGTH_ERROR;
    }

    return(status);
}



#endif