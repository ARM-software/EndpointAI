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

// LOOP CORE
// Used to select the right template specialization.
// A loop is containing all stages for a given radix
static constexpr int FIRST=0; // FIRST GROUP (twiddle is 1)
static constexpr int NORMAL=1; // NORMAL GROUP AND STAGE
static constexpr int LAST=2; // LAST STAGE (twiddle is 1 and samples contiguous)
static constexpr int FIRSTANDLAST=3; // FIRST AND LAST STAGE for the loop

template<int mode,typename T, int factor,int stagemode,int inputstrideenabled, int vectormode>
struct loopcore;



// Generic loop not containing first and last.
// Scalar version (0 as last template argument)
template<int mode,typename T, int factor,int inputstrideenabled>
struct loopcore<mode,T,factor,NORMAL,inputstrideenabled,0>{
     __STATIC_FORCEINLINE int run(const int n,
                int stageLength,
                complex<T> *in,
                complex<T> *out,
                const int stride,
                const complex<T> *twid,
                int inputstride
                )
     {    
          int twidOffset=0;
          (void)out;
          // Except for CFFT 2D, it will always be equal to 1 since
          // inputstride is 0 when no stride is used.
          const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));

          // First group
          for (int i = 0 ; i < n ; i = i + factor*stageLength)
          {
           radix<mode,T,factor,inputstrideenabled>::run_ip_notw(stageLength,&in[i * strideFactor],inputstride);
          }
          twidOffset += stride;

          for(int sample=1; sample < stageLength ; sample++)
          {
             complex<T> w[factor-1];
             const complex<T> *p = &twid[1*twidOffset];
      
             // Twiddle is shared between several groups
             for(int k = 0; k < factor-1 ; k++)
             {
               w[k] = *p;
               p += twidOffset;
             }
      
             // Iterate on the groups
             for (int i = sample ; i < n ; i = i + factor*stageLength)
             {
                 radix<mode,T,factor,inputstrideenabled>::run_ip_tw(stageLength,w,&in[i * strideFactor],inputstride);
             }
             twidOffset += stride;
          }

          return(factor*stride);
     }
};


// Loop containing the first stage
// and first stage is managed differently
template<int mode,typename T,int factor,int inputstrideenabled>
struct loopcore<mode,T,factor,FIRST,inputstrideenabled,0>{
     __STATIC_FORCEINLINE int run(const int n,
                int stageLength,
                complex<T> *in,
                complex<T> *out,
                const int stride,
                const complex<T> *twid,
                int inputstride
                )
     {    
          (void)n;
          (void)stride;
          int twidOffset=0;
          const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));

          // First group
          radix<mode,T,factor,inputstrideenabled>::run_op_notw(stageLength,&in[0],&out[0],inputstride);
          twidOffset += 1;

          for(int sample=1; sample < stageLength ; sample++)
          {
             complex<T> w[factor-1];
             const complex<T> *p = &twid[1*twidOffset];
      
             for(int k = 0; k < factor-1 ; k++)
             {
               w[k] = *p;
               p += twidOffset;
             }
      
             radix<mode,T,factor,inputstrideenabled>::run_op_tw(stageLength,w,&in[sample * strideFactor],&out[sample * strideFactor],inputstride);
             twidOffset += 1;
          }

          return(factor);
     }
};


// Loop containing last stage
template<int mode,typename T,int factor,int inputstrideenabled>
struct loopcore<mode,T,factor,LAST,inputstrideenabled,0>{
     __STATIC_FORCEINLINE int run(const int n,
                int stageLength,
                complex<T> *in,
                complex<T> *out,
                const int stride,
                const complex<T> *twid,
                int inputstride
                )
     {
        (void)stageLength;
        (void)twid;
        (void)out;
        const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));

        for (int i = 0 ; i < n ; i = i + factor)
        {
            radix<mode,T,factor,inputstrideenabled>::run_ip_last(&in[i * strideFactor],inputstride);
        }

        return(factor*stride);
     }
};

// Loop containing first and last stage
template<int mode,typename T,int factor,int inputstrideenabled>
struct loopcore<mode,T,factor,FIRSTANDLAST,inputstrideenabled,0>{
     __STATIC_FORCEINLINE int run(const int n,
                int stageLength,
                complex<T> *in,
                complex<T> *out,
                const int stride,
                const complex<T> *twid,
                int inputstride
                )
     {
        (void)stageLength;
        (void)twid;
        const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));

        for (int i = 0 ; i < n ; i = i + factor)
        {
            radix<mode,T,factor,inputstrideenabled>::run_op_last(&in[i * strideFactor],&out[i * strideFactor],inputstride);
        }

        return(factor*stride);
     }
};