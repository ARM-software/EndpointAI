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
#include "RFFTBenchmarksF32.h"
#include "Error.h"



#include "fft.h"


void RFFTBenchmarksF32::test_rfft_radix2_f32()
{     

  rfft(in,tmpptr,out,this->nbSamples);
} 


void RFFTBenchmarksF32::test_rfft_radix_mixed_f32()
{     
  rfft(in,tmpptr,out,this->nbSamples);
} 


void RFFTBenchmarksF32::test_rifft_radix2_f32()
{     

  rifft(in,tmpptr,out,this->nbSamples);
} 


void RFFTBenchmarksF32::test_rifft_radix_mixed_f32()
{     
  rifft(in,tmpptr,out,this->nbSamples);
} 

 
    void RFFTBenchmarksF32::setUp(Testing::testID_t id,std::vector<Testing::param_t>& params,Client::PatternMgr *mgr)
    {

       (void)id;

       std::vector<Testing::param_t>::iterator it = params.begin();
       this->nbSamples = *it++;
       
       switch(id)
       {
        case TEST_RFFT_RADIX2_F32_1:

           input1.reload(RFFTBenchmarksF32::INPUTR_F32_ID,mgr);
           output.create(2+this->nbSamples,RFFTBenchmarksF32::OUT_F32_ID,mgr);
           tmp.create(this->nbSamples,RFFTBenchmarksF32::TMP_F32_ID,mgr);


           in = input1.ptr();
           out = output.ptr();
           tmpptr=tmp.ptr();



          
        break;

        case TEST_RFFT_RADIX_MIXED_F32_2:

           input1.reload(RFFTBenchmarksF32::INPUTR_F32_ID,mgr);
           output.create(2 + this->nbSamples,RFFTBenchmarksF32::OUT_F32_ID,mgr);
           tmp.create(this->nbSamples,RFFTBenchmarksF32::TMP_F32_ID,mgr);

           in = input1.ptr();
           out = output.ptr();
           tmpptr=tmp.ptr();


          
        break;

        case TEST_RIFFT_RADIX2_F32_3:

           input1.reload(RFFTBenchmarksF32::INPUTR_IFFT_F32_ID,mgr);
           output.create(this->nbSamples,RFFTBenchmarksF32::OUT_F32_ID,mgr);
           tmp.create(this->nbSamples,RFFTBenchmarksF32::TMP_F32_ID,mgr);

           in = input1.ptr();
           out = output.ptr();
           tmpptr=tmp.ptr();

          
        break;

 
        case TEST_RIFFT_RADIX_MIXED_F32_4:

           input1.reload(RFFTBenchmarksF32::INPUTR_IFFT_F32_ID,mgr);
           output.create(this->nbSamples,RFFTBenchmarksF32::OUT_F32_ID,mgr);
           tmp.create(this->nbSamples,RFFTBenchmarksF32::TMP_F32_ID,mgr);

           in = input1.ptr();
           out = output.ptr();
           tmpptr=tmp.ptr();


          
        break;

       
       }

     
    }

    void RFFTBenchmarksF32::tearDown(Testing::testID_t id,Client::PatternMgr *mgr)
    {
      (void)id;
      (void)mgr;
    }
