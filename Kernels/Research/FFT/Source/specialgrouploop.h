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

Special implementation for the radix 2 scalar first stage.
It is faster than the generic implementation and use some
symmetry of the twiddles.


*/

template<int mode,typename T,int inputstrideenabled>
struct loopcore<mode,T,2,FIRST,inputstrideenabled,0>{
     __STATIC_FORCEINLINE int run(const int n,
                int stageLength,
                complex<T> *in,
                complex<T> *out,
                const int stride,
                const complex<T> *twid,
                int inputstride
                )
     {    
          (void)stride;
          (void)n;
          complex<T> *p = (complex<T> *)twid;
          complex<T> wb;
          const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));

          if ((stageLength & 1) == 0)
          {
              int n2 = stageLength >> 1;
    
    
              for(int sample=0; sample < n2 ; sample++)
              {
                wb = -rot(*p);
                
                radix<mode,T,2,inputstrideenabled>::run_op_tw(stageLength,p,&in[sample * strideFactor],&out[sample * strideFactor],inputstride);
                radix<mode,T,2,inputstrideenabled>::run_op_tw(stageLength,&wb,&in[(sample + n2) * strideFactor],&out[(sample + n2) * strideFactor],inputstride);
                p++;
              }
          }
          else
          {
            radix<mode,T,2,inputstrideenabled>::run_op_notw(stageLength,&in[0],&out[0],inputstride);
            p++;

            for(int sample=1; sample < stageLength ; sample++)
            {
                radix<mode,T,2,inputstrideenabled>::run_op_tw(stageLength,p,&in[sample * strideFactor],&out[sample * strideFactor],inputstride);
                p++;
            }
          }


          return(2);
     }
};
