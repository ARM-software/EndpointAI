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
#include "CMSISRFFTBenchmarksF32.h"
#include "Error.h"


void CMSISRFFTBenchmarksF32::test_rfft_radix2_f32()
{     

  arm_rfft_fast_f32(&this->rfftFastInstance, this->pTmp, this->pDst, 0);

} 



void CMSISRFFTBenchmarksF32::test_rifft_radix2_f32()
{     
  arm_rfft_fast_f32(&this->rfftFastInstance, this->pTmp, this->pDst, 1);
} 


 
    void CMSISRFFTBenchmarksF32::setUp(Testing::testID_t id,std::vector<Testing::param_t>& params,Client::PatternMgr *mgr)
    {


       std::vector<Testing::param_t>::iterator it = params.begin();
       this->nbSamples = *it++;
       
       switch(id)
       {
        case TEST_RFFT_RADIX2_F32_1:
        // Factor 2 for rifft
            input1.reload(CMSISRFFTBenchmarksF32::INPUTR_F32_ID,mgr,2*this->nbSamples);
            output.create(2*this->nbSamples,CMSISRFFTBenchmarksF32::OUT_F32_ID,mgr);
            tmp.create(this->nbSamples,CMSISRFFTBenchmarksF32::TMP_F32_ID,mgr);

            this->pSrc=input1.ptr();
            this->pDst=output.ptr();
            this->pTmp=tmp.ptr();

            memcpy(this->pTmp,this->pSrc,sizeof(float32_t)*this->nbSamples); 

            status=arm_rfft_fast_init_f32(&this->rfftFastInstance, this->nbSamples);

            if (status != ARM_MATH_SUCCESS)
            {
               printf("Error init CMSIS RFFT F32 benchmark for size %d\n",this->nbSamples);
            }
        break;

        
        case TEST_RIFFT_RADIX2_F32_2:

            input1.reload(CMSISRFFTBenchmarksF32::INPUTR_F32_ID,mgr,2*this->nbSamples);
            output.create(2*this->nbSamples,CMSISRFFTBenchmarksF32::OUT_F32_ID,mgr);
            tmp.create(this->nbSamples,CMSISRFFTBenchmarksF32::TMP_F32_ID,mgr);

            this->pSrc=input1.ptr();
            this->pDst=output.ptr();
            this->pTmp=tmp.ptr();

            memcpy(this->pTmp,this->pSrc,sizeof(float32_t)*this->nbSamples); 

            status=arm_rfft_fast_init_f32(&this->rfftFastInstance, this->nbSamples);
            if (status != ARM_MATH_SUCCESS)
            {
               printf("Error init CMSIS RIFFT F32 benchmark for size %d\n",this->nbSamples);
            }

        break;
       
       }

     


    }

    void CMSISRFFTBenchmarksF32::tearDown(Testing::testID_t id,Client::PatternMgr *mgr)
    {
      (void)id;
      (void)mgr;
    }
