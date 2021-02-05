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
#include "CFFTBenchmarksQ15.h"
#include "Error.h"



#include "fft.h"


void CFFTBenchmarksQ15::test_cfft_radix2_q15()
{     

  cfft((FFTSCI::Q15*)in,(FFTSCI::Q15*)out,this->nbSamples);  
} 

void CFFTBenchmarksQ15::test_cfft_radix3_q15()
{     
  cfft((FFTSCI::Q15*)in,(FFTSCI::Q15*)out,this->nbSamples);
} 


void CFFTBenchmarksQ15::test_cfft_radix5_q15()
{     
  cfft((FFTSCI::Q15*)in,(FFTSCI::Q15*)out,this->nbSamples);
} 

void CFFTBenchmarksQ15::test_cfft_radix_mixed_q15()
{     
  cfft((FFTSCI::Q15*)in,(FFTSCI::Q15*)out,this->nbSamples);
} 


void CFFTBenchmarksQ15::test_cifft_radix2_q15()
{     

  cifft((FFTSCI::Q15*)in,(FFTSCI::Q15*)out,this->nbSamples);  
} 

void CFFTBenchmarksQ15::test_cifft_radix3_q15()
{     
  cifft((FFTSCI::Q15*)in,(FFTSCI::Q15*)out,this->nbSamples);
} 


void CFFTBenchmarksQ15::test_cifft_radix5_q15()
{     
  cifft((FFTSCI::Q15*)in,(FFTSCI::Q15*)out,this->nbSamples);
} 


void CFFTBenchmarksQ15::test_cifft_radix_mixed_q15()
{     
  cifft((FFTSCI::Q15*)in,(FFTSCI::Q15*)out,this->nbSamples);
} 

 
    void CFFTBenchmarksQ15::setUp(Testing::testID_t id,std::vector<Testing::param_t>& params,Client::PatternMgr *mgr)
    {

       (void)id;
       std::vector<Testing::param_t>::iterator it = params.begin();
       this->nbSamples = *it++;
       
       switch(id)
       {
        case TEST_CFFT_RADIX2_Q15_1:

           input1.reload(CFFTBenchmarksQ15::INPUTC_Q15_ID,mgr);
           output.create(2*this->nbSamples,CFFTBenchmarksQ15::OUT_Q15_ID,mgr);

           in = input1.ptr();
           out = output.ptr();



          
        break;

        case TEST_CFFT_RADIX3_Q15_2:

           input1.reload(CFFTBenchmarksQ15::INPUTC_Q15_ID,mgr);
           output.create(2*this->nbSamples,CFFTBenchmarksQ15::OUT_Q15_ID,mgr);

           in = input1.ptr();
           out = output.ptr();

        break;

        case TEST_CFFT_RADIX5_Q15_3:

           input1.reload(CFFTBenchmarksQ15::INPUTC_Q15_ID,mgr);
           output.create(2*this->nbSamples,CFFTBenchmarksQ15::OUT_Q15_ID,mgr);

           in = input1.ptr();
           out = output.ptr();


          
        break;

        case TEST_CFFT_RADIX_MIXED_Q15_4:

           input1.reload(CFFTBenchmarksQ15::INPUTC_Q15_ID,mgr);
           output.create(2*this->nbSamples,CFFTBenchmarksQ15::OUT_Q15_ID,mgr);

           in = input1.ptr();
           out = output.ptr();


          
        break;

        case TEST_CIFFT_RADIX2_Q15_5:

           input1.reload(CFFTBenchmarksQ15::INPUTC_IFFT_Q15_ID,mgr);
           output.create(2*this->nbSamples,CFFTBenchmarksQ15::OUT_Q15_ID,mgr);

           in = input1.ptr();
           out = output.ptr();



          
        break;

        case TEST_CIFFT_RADIX3_Q15_6:

           input1.reload(CFFTBenchmarksQ15::INPUTC_IFFT_Q15_ID,mgr);
           output.create(2*this->nbSamples,CFFTBenchmarksQ15::OUT_Q15_ID,mgr);

           in = input1.ptr();
           out = output.ptr();

        break;

        case TEST_CIFFT_RADIX5_Q15_7:

           input1.reload(CFFTBenchmarksQ15::INPUTC_IFFT_Q15_ID,mgr);
           output.create(2*this->nbSamples,CFFTBenchmarksQ15::OUT_Q15_ID,mgr);

           in = input1.ptr();
           out = output.ptr();


          
        break;

        case TEST_CIFFT_RADIX_MIXED_Q15_8:

           input1.reload(CFFTBenchmarksQ15::INPUTC_IFFT_Q15_ID,mgr);
           output.create(2*this->nbSamples,CFFTBenchmarksQ15::OUT_Q15_ID,mgr);

           in = input1.ptr();
           out = output.ptr();


          
        break;

       
       }

     


    }

    void CFFTBenchmarksQ15::tearDown(Testing::testID_t id,Client::PatternMgr *mgr)
    {
      (void)id;
      (void)mgr;
    }
