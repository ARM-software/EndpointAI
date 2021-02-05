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

// LOOP CONFIG
static constexpr int STARTONLY=0;
static constexpr int ENDONLY=1;
static constexpr int BOTH=2;
static constexpr int NORMALLOOP=3;
static constexpr int ONESTAGE=4; // BOTH start and end but only one stage

/*****************

NORMAL LOOP

*****************/
template <int mode,typename T,int loopmode, int factor,int inputstrideenabled>
struct loop{
  __STATIC_FORCEINLINE int run(const uint16_t *lengths,
    const int n,
    const int nbFactors,
    int stride,
    complex<T> *in,
    complex<T> *out,
    const complex<T> *twid,
    int inputstride)
  {
    int stage;
    (void)in;

    // Iteration on all stages for the given radix
    for(stage = 0; stage < nbFactors; stage ++)
    {
          #if defined(DEBUG)
             printf("  normal stage = %d\n",stage);
          #endif

          int stageLength = lengths[stage];
          stride=loopcore<mode,T,factor,NORMAL,inputstrideenabled,vector<T>::vector_enabled>::run(n,stageLength,out,out,stride,twid,inputstride);
    }
    return(stride);
  }
};

/*****************

START ONLY LOOP

*****************/
template <int mode,typename T,int factor,int inputstrideenabled>
struct loop<mode,T,STARTONLY,factor,inputstrideenabled>{
  __STATIC_FORCEINLINE int run(const uint16_t *lengths,
    const int n,
    const int nbFactors,
    int stride,
    complex<T> *in,
    complex<T> *out,
    const complex<T> *twid,
    int inputstride)
   {
    int stage;

    int stageLength = lengths[0];
    #if defined(DEBUG)
             printf("  first stage 0\n");
    #endif
    stride=loopcore<mode,T,factor,FIRST,inputstrideenabled,vector<T>::vector_enabled>::run(n,stageLength,in,out,stride,twid,inputstride);

    for(stage = 1; stage < nbFactors; stage ++)
    {
          #if defined(DEBUG)
             printf("  normal stage = %d\n",stage);
          #endif

          stageLength = lengths[stage];
          stride=loopcore<mode,T,factor,NORMAL,inputstrideenabled,vector<T>::vector_enabled>::run(n,stageLength,out,out,stride,twid,inputstride);
    }
    return(stride);
  }
};

/*****************

END ONLY LOOP

*****************/
template <int mode, typename T, int factor,int inputstrideenabled>
struct loop<mode,T,ENDONLY,factor,inputstrideenabled>{
  __STATIC_FORCEINLINE int run(const uint16_t *lengths,
    const int n,
    const int nbFactors,
    int stride,
    complex<T> *in,
    complex<T> *out,
    const complex<T> *twid,
    int inputstride)
  {
    int stage;
    (void)in;


    int stageLength;

    for(stage = 0; stage < nbFactors-1; stage ++)
    {
          #if defined(DEBUG)
             printf("  normal stage = %d\n",stage);
          #endif

          stageLength = lengths[stage];
          stride=loopcore<mode,T,factor,NORMAL,inputstrideenabled,vector<T>::vector_enabled>::run(n,stageLength,out,out,stride,twid,inputstride);
    }

    stageLength = lengths[stage];
    #if defined(DEBUG)
             printf("  last stage = %d\n",stage);
    #endif
    stride=loopcore<mode,T,factor,LAST,inputstrideenabled,0>::run(n,stageLength,out,out,stride,twid,inputstride);

    return(stride);
  }
};

/*****************

BOTH LOOP

*****************/
template <int mode, typename T, int factor,int inputstrideenabled>
struct loop<mode,T,BOTH,factor,inputstrideenabled>{
  __STATIC_FORCEINLINE int run(const uint16_t *lengths,
    const int n,
    const int nbFactors,
    int stride,
    complex<T> *in,
    complex<T> *out,
    const complex<T> *twid,
    int inputstride)
  {
    int stage;


    int stageLength = lengths[0];
    #if defined(DEBUG)
             printf("  first stage 0\n");
    #endif
    stride=loopcore<mode,T,factor,FIRST,inputstrideenabled,vector<T>::vector_enabled>::run(n,stageLength,in,out,stride,twid,inputstride);

    for(stage = 1; stage < nbFactors-1; stage ++)
    {
          #if defined(DEBUG)
             printf("  normal stage = %d\n",stage);
          #endif

          stageLength = lengths[stage];
          stride=loopcore<mode,T,factor,NORMAL,inputstrideenabled,vector<T>::vector_enabled>::run(n,stageLength,out,out,stride,twid,inputstride);
    }

    stageLength = lengths[stage];
    #if defined(DEBUG)
             printf("  last stage = %d\n",stage);
    #endif
    stride=loopcore<mode,T,factor,LAST,inputstrideenabled,0>::run(n,stageLength,out,out,stride,twid,inputstride);

    return(stride);
  }
};

/*****************

ONE STAGE

*****************/
template <int mode, typename T,int factor,int inputstrideenabled>
struct loop<mode,T,ONESTAGE,factor,inputstrideenabled>{
  __STATIC_FORCEINLINE int run(const uint16_t *lengths,
    const int n,
    const int nbFactors,
    int stride,
    complex<T> *in,
    complex<T> *out,
    const complex<T> *twid,
    int inputstride)
  {
    (void)nbFactors;


    #if defined(DEBUG)
             printf("  first and last stage\n");
    #endif
    int stageLength = lengths[0];
    stride=loopcore<mode,T,factor,FIRSTANDLAST,inputstrideenabled,0>::run(n,stageLength,in,out,stride,twid,inputstride);

    return(stride);
  }
};


// Select the radix.
// If new radix are added to the kernels and python, this switch / case must be
// modified and the new radix added here too.
template <int mode,typename T, int loopmode,int inputstrideenabled>
struct factorSwitch{
  static int run(
    const cfftconfig<T> *config,
    const int n,
    const int factor,
    const int nbFactors,
    const int stage,
    const int stride,
    complex<T> *in,
    complex<T> *out,
    const complex<T> *twid,
    int inputstride)
  {
     // nbFactors is the exponent
     // For instance, for a FFT of length 16, we get 4^2
     // factor would 4 and nbFactors 2,
    #if defined(DEBUG)
     printf("factor = %d, nbFactors=%d\n",factor,nbFactors);
    #endif
        switch(factor)
        {
          #if defined(RADIX2)
            case 2:
              return(loop<mode,T,loopmode,2,inputstrideenabled>::run(&config->lengths[stage],n,nbFactors,stride,in,out,twid,inputstride));
            break;
          #endif
            
          #if defined(RADIX3)
            case 3:
              return(loop<mode,T,loopmode,3,inputstrideenabled>::run(&config->lengths[stage],n,nbFactors,stride,in,out,twid,inputstride));
            break;
          #endif
            
          #if defined(RADIX4)
            case 4:
              return(loop<mode,T,loopmode,4,inputstrideenabled>::run(&config->lengths[stage],n,nbFactors,stride,in,out,twid,inputstride));
            break;
          #endif
            
          #if defined(RADIX5)
            case 5:
              return(loop<mode,T,loopmode,5,inputstrideenabled>::run(&config->lengths[stage],n,nbFactors,stride,in,out,twid,inputstride));
            break;
          #endif
            
          #if defined(RADIX6)
            case 6:
              return(loop<mode,T,loopmode,6,inputstrideenabled>::run(&config->lengths[stage],n,nbFactors,stride,in,out,twid,inputstride));
            break;
          #endif 

          #if defined(RADIX8)
            case 8:
              return(loop<mode,T,loopmode,8,inputstrideenabled>::run(&config->lengths[stage],n,nbFactors,stride,in,out,twid,inputstride));
            break;
          #endif

        }
        return(stride);
  }
};