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

When vector implementations of the radixes are existing, we have here
some vector implementations for some of the cases of the loopcores.

When an implementation is not given here, the scalar version is used by default

*/


template<int mode,typename T, int factor,int inputstrideenabled>
struct loopcore<mode,T,factor,NORMAL,inputstrideenabled,1>{
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
          int blkCnt;
          int sample=0;
          (void)out;
          const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));

#if defined(DEBUG)
          printf("VECTOR NORMAL\n");
#endif
          
          blkCnt = (stageLength) >> vector<T>::loop_shift;
          for(int j=0; j < blkCnt ; j++)
          {

             VECTORTYPE<T> w[factor-1];
             const complex<T> *p = &twid[1*twidOffset];
      
             for(int k = 0; k < factor-1 ; k++)
             {
               w[k] = vector<T>::gather_offset((T *)p,(k+1)*stride,0);
               p += twidOffset;
             }
     
             for (int i = sample ; i < n ; i = i + factor*stageLength)
             {
                 radixVector<mode,T,factor,inputstrideenabled>::run_ip_tw(stageLength,w,&in[i * strideFactor],inputstride);
             }
             twidOffset += vector<T>::nb_complexes*stride;
             sample += vector<T>::nb_complexes;
          }

          blkCnt = (stageLength) & vector<T>::tail_mask;
          for(int j=0; j < blkCnt ; j++)
          {
             complex<T> w[factor-1];
             const complex<T> *p = &twid[1*twidOffset];
      
             for(int k = 0; k < factor-1 ; k++)
             {
               w[k] = *p;
               p += twidOffset;
             }
      
             for (int i = sample ; i < n ; i = i + factor*stageLength)
             {
                 radix<mode,T,factor,inputstrideenabled>::run_ip_tw(stageLength,w,&in[i * strideFactor],inputstride);
             }
             twidOffset += stride;
             sample++;
          }

          return(factor*stride);
     }
};


template<int mode,typename T,int factor,int inputstrideenabled>
struct loopcore<mode,T,factor,FIRST,inputstrideenabled,1>{
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
          int blkCnt;
          int sample=0;

          /* Twiddle stride is equal to 1 in this case */

          const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));

#if defined(DEBUG)
          printf("VECTOR FIRST\n");
#endif
          
          
          blkCnt = (stageLength) >> vector<T>::loop_shift;
          for(int j=0; j < blkCnt ; j++)
          {

             VECTORTYPE<T> w[factor-1];
             const complex<T> *p = &twid[1*twidOffset];
      
             for(int k = 0; k < factor-1 ; k++)
             {
               w[k] = vector<T>::gather_offset((T *)p,(k+1),0);
               p += twidOffset;
             }
      
             radixVector<mode,T,factor,inputstrideenabled>::run_op_tw(stageLength,w,&in[sample * strideFactor],&out[sample * strideFactor],inputstride);
             twidOffset += vector<T>::nb_complexes;
             sample += vector<T>::nb_complexes;
          }

          blkCnt = (stageLength) & vector<T>::tail_mask;
          for(int j=0; j < blkCnt ; j++)
          {
             complex<T> w[factor-1];
             const complex<T> *p = &twid[1*twidOffset];
      
             for(int k = 0; k < factor-1 ; k++)
             {
               w[k] = *p;
               p += twidOffset;
             }
      
             radix<mode,T,factor,inputstrideenabled>::run_op_tw(stageLength,w,&in[sample * strideFactor],&out[sample * strideFactor],inputstride);
             twidOffset ++;
             sample ++;
          }

          return(factor);
     }
};