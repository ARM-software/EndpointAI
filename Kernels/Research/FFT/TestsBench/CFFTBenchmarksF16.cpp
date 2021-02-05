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
#include "CFFTBenchmarksF16.h"
#include "Error.h"



#include "fft.h"


void CFFTBenchmarksF16::test_cfft_radix2_f16()
{     

  cfft(in,out,this->nbSamples);  
} 

void CFFTBenchmarksF16::test_cfft_radix3_f16()
{     
  cfft(in,out,this->nbSamples);
} 


void CFFTBenchmarksF16::test_cfft_radix5_f16()
{     
  cfft(in,out,this->nbSamples);
} 

void CFFTBenchmarksF16::test_cfft_radix_mixed_f16()
{     
  cfft(in,out,this->nbSamples);
} 


void CFFTBenchmarksF16::test_cifft_radix2_f16()
{     

  cifft(in,out,this->nbSamples);  
} 

void CFFTBenchmarksF16::test_cifft_radix3_f16()
{     
  cifft(in,out,this->nbSamples);
} 


void CFFTBenchmarksF16::test_cifft_radix5_f16()
{     
  cifft(in,out,this->nbSamples);
} 


void CFFTBenchmarksF16::test_cifft_radix_mixed_f16()
{     
  cifft(in,out,this->nbSamples);
} 

 
    void CFFTBenchmarksF16::setUp(Testing::testID_t id,std::vector<Testing::param_t>& params,Client::PatternMgr *mgr)
    {

       (void)id;
       std::vector<Testing::param_t>::iterator it = params.begin();
       this->nbSamples = *it++;
       
       switch(id)
       {
        case TEST_CFFT_RADIX2_F16_1:

           input1.reload(CFFTBenchmarksF16::INPUTC_F16_ID,mgr);
           output.create(2*this->nbSamples,CFFTBenchmarksF16::OUT_F16_ID,mgr);

           in = input1.ptr();
           out = output.ptr();



          
        break;

        case TEST_CFFT_RADIX3_F16_2:

           input1.reload(CFFTBenchmarksF16::INPUTC_F16_ID,mgr);
           output.create(2*this->nbSamples,CFFTBenchmarksF16::OUT_F16_ID,mgr);

           in = input1.ptr();
           out = output.ptr();

        break;

        case TEST_CFFT_RADIX5_F16_3:

           input1.reload(CFFTBenchmarksF16::INPUTC_F16_ID,mgr);
           output.create(2*this->nbSamples,CFFTBenchmarksF16::OUT_F16_ID,mgr);

           in = input1.ptr();
           out = output.ptr();


          
        break;

        case TEST_CFFT_RADIX_MIXED_F16_4:

           input1.reload(CFFTBenchmarksF16::INPUTC_F16_ID,mgr);
           output.create(2*this->nbSamples,CFFTBenchmarksF16::OUT_F16_ID,mgr);

           in = input1.ptr();
           out = output.ptr();


          
        break;

        case TEST_CIFFT_RADIX2_F16_5:

           input1.reload(CFFTBenchmarksF16::INPUTC_IFFT_F16_ID,mgr);
           output.create(2*this->nbSamples,CFFTBenchmarksF16::OUT_F16_ID,mgr);

           in = input1.ptr();
           out = output.ptr();



          
        break;

        case TEST_CIFFT_RADIX3_F16_6:

           input1.reload(CFFTBenchmarksF16::INPUTC_IFFT_F16_ID,mgr);
           output.create(2*this->nbSamples,CFFTBenchmarksF16::OUT_F16_ID,mgr);

           in = input1.ptr();
           out = output.ptr();

        break;

        case TEST_CIFFT_RADIX5_F16_7:

           input1.reload(CFFTBenchmarksF16::INPUTC_IFFT_F16_ID,mgr);
           output.create(2*this->nbSamples,CFFTBenchmarksF16::OUT_F16_ID,mgr);

           in = input1.ptr();
           out = output.ptr();


          
        break;

        case TEST_CIFFT_RADIX_MIXED_F16_8:

           input1.reload(CFFTBenchmarksF16::INPUTC_IFFT_F16_ID,mgr);
           output.create(2*this->nbSamples,CFFTBenchmarksF16::OUT_F16_ID,mgr);

           in = input1.ptr();
           out = output.ptr();


          
        break;

       
       }

     


    }

    void CFFTBenchmarksF16::tearDown(Testing::testID_t id,Client::PatternMgr *mgr)
    {
      (void)id;
      (void)mgr;
    }
