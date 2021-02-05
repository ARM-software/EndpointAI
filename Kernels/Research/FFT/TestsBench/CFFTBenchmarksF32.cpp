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
#include "CFFTBenchmarksF32.h"
#include "Error.h"



void CFFTBenchmarksF32::test_cfft_radix2_f32()
{     

  cfft(in,out,this->nbSamples);  
} 

void CFFTBenchmarksF32::test_cfft_radix3_f32()
{     
  cfft(in,out,this->nbSamples);
} 


void CFFTBenchmarksF32::test_cfft_radix5_f32()
{     
  cfft(in,out,this->nbSamples);
} 

void CFFTBenchmarksF32::test_cfft_radix_mixed_f32()
{     
  cfft(in,out,this->nbSamples);
} 


void CFFTBenchmarksF32::test_cifft_radix2_f32()
{     

  cifft(in,out,this->nbSamples);  
} 

void CFFTBenchmarksF32::test_cifft_radix3_f32()
{     
  cifft(in,out,this->nbSamples);
} 


void CFFTBenchmarksF32::test_cifft_radix5_f32()
{     
  cifft(in,out,this->nbSamples);
} 


void CFFTBenchmarksF32::test_cifft_radix_mixed_f32()
{     
  cifft(in,out,this->nbSamples);
} 

 
    void CFFTBenchmarksF32::setUp(Testing::testID_t id,std::vector<Testing::param_t>& params,Client::PatternMgr *mgr)
    {

       (void)id;
       std::vector<Testing::param_t>::iterator it = params.begin();
       this->nbSamples = *it++;
       
       switch(id)
       {
        case TEST_CFFT_RADIX2_F32_1:

           input1.reload(CFFTBenchmarksF32::INPUTC_F32_ID,mgr);
           output.create(2*this->nbSamples,CFFTBenchmarksF32::OUT_F32_ID,mgr);

           in = input1.ptr();
           out = output.ptr();



          
        break;

        case TEST_CFFT_RADIX3_F32_2:

           input1.reload(CFFTBenchmarksF32::INPUTC_F32_ID,mgr);
           output.create(2*this->nbSamples,CFFTBenchmarksF32::OUT_F32_ID,mgr);

           in = input1.ptr();
           out = output.ptr();

        break;

        case TEST_CFFT_RADIX5_F32_3:

           input1.reload(CFFTBenchmarksF32::INPUTC_F32_ID,mgr);
           output.create(2*this->nbSamples,CFFTBenchmarksF32::OUT_F32_ID,mgr);

           in = input1.ptr();
           out = output.ptr();


          
        break;

        case TEST_CFFT_RADIX_MIXED_F32_4:

           input1.reload(CFFTBenchmarksF32::INPUTC_F32_ID,mgr);
           output.create(2*this->nbSamples,CFFTBenchmarksF32::OUT_F32_ID,mgr);

           in = input1.ptr();
           out = output.ptr();


          
        break;

        case TEST_CIFFT_RADIX2_F32_5:

           input1.reload(CFFTBenchmarksF32::INPUTC_IFFT_F32_ID,mgr);
           output.create(2*this->nbSamples,CFFTBenchmarksF32::OUT_F32_ID,mgr);

           in = input1.ptr();
           out = output.ptr();



          
        break;

        case TEST_CIFFT_RADIX3_F32_6:

           input1.reload(CFFTBenchmarksF32::INPUTC_IFFT_F32_ID,mgr);
           output.create(2*this->nbSamples,CFFTBenchmarksF32::OUT_F32_ID,mgr);

           in = input1.ptr();
           out = output.ptr();

        break;

        case TEST_CIFFT_RADIX5_F32_7:

           input1.reload(CFFTBenchmarksF32::INPUTC_IFFT_F32_ID,mgr);
           output.create(2*this->nbSamples,CFFTBenchmarksF32::OUT_F32_ID,mgr);

           in = input1.ptr();
           out = output.ptr();


          
        break;

        case TEST_CIFFT_RADIX_MIXED_F32_8:

           input1.reload(CFFTBenchmarksF32::INPUTC_IFFT_F32_ID,mgr);
           output.create(2*this->nbSamples,CFFTBenchmarksF32::OUT_F32_ID,mgr);

           in = input1.ptr();
           out = output.ptr();


          
        break;

       
       }

     


    }

    void CFFTBenchmarksF32::tearDown(Testing::testID_t id,Client::PatternMgr *mgr)
    {
      (void)id;
      (void)mgr;
    }
