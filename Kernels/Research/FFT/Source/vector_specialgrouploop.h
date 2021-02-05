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

template<int mode,typename T,int inputstrideenabled>
struct loopcore<mode,T,2,FIRST,inputstrideenabled,1>{
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

#if defined(DEBUG)
          printf("VECTOR SPECIAL 2\n");
#endif
          
          const int strideFactor = (1 + inputstrideenabled * (inputstride - 1));

          if ((stageLength & 1) == 0)
          {
              int sample=0;
              int n2 = stageLength >> 1;

              int blkCnt;

              blkCnt = n2 >> vector<T>::loop_shift;

              for(int i=0; i < blkCnt ; i++)
              {
                VECTORTYPE<T> vw[1],vwb[1];

                vw[0] = vload((T*)p);
                vwb[0] = vnegate(vrot(vw[0]));
                
                radixVector<mode,T,2,inputstrideenabled>::run_op_tw(stageLength,vw,&in[sample * strideFactor],&out[sample * strideFactor],inputstride);
                radixVector<mode,T,2,inputstrideenabled>::run_op_tw(stageLength,vwb,&in[(sample + n2) * strideFactor],&out[(sample + n2) * strideFactor],inputstride);
                
                p += vector<T>::nb_complexes;
                sample += vector<T>::nb_complexes;
              }

              blkCnt = n2 & vector<T>::tail_mask;
              for(int i=0; i < blkCnt ; i++)
              {
                wb = -rot(*p);
                
                radix<mode,T,2,inputstrideenabled>::run_op_tw(stageLength,p,&in[sample * strideFactor],&out[sample * strideFactor],inputstride);
                radix<mode,T,2,inputstrideenabled>::run_op_tw(stageLength,&wb,&in[(sample + n2) * strideFactor],&out[(sample + n2) * strideFactor],inputstride);
                p++;
                sample++;
              }

          }
          else
          {
            int sample=0;


            int blkCnt;

            blkCnt = stageLength >> vector<T>::loop_shift;
            for(int i=0; i < blkCnt ; i++)
            {
                VECTORTYPE<T> vw[1];

                vw[0] = vload((T*)p);
                radixVector<mode,T,2,inputstrideenabled>::run_op_tw(stageLength,vw,&in[sample * strideFactor],&out[sample * strideFactor],inputstride);
                p += vector<T>::nb_complexes;
                sample += vector<T>::nb_complexes;
            }

            blkCnt = stageLength & vector<T>::tail_mask;
            for(int i=0; i < blkCnt ; i++)
            {
                radix<mode,T,2,inputstrideenabled>::run_op_tw(stageLength,p,&in[sample * strideFactor],&out[sample * strideFactor],inputstride);
                p++;
                sample++;
            }
          }


          return(2);
     }
};
