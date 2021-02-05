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
#include "CMSISCFFTBenchmarksF32.h"
#include "Error.h"


void CMSISCFFTBenchmarksF32::test_cfft_radix2_f32()
{     

  arm_cfft_f32(&(this->cfftInstance), this->pDst, 0,1);

} 



void CMSISCFFTBenchmarksF32::test_cifft_radix2_f32()
{     
  arm_cfft_f32(&(this->cfftInstance), this->pDst, 1,1);
} 


 
    void CMSISCFFTBenchmarksF32::setUp(Testing::testID_t id,std::vector<Testing::param_t>& params,Client::PatternMgr *mgr)
    {


       std::vector<Testing::param_t>::iterator it = params.begin();
       this->nbSamples = *it++;
       
       switch(id)
       {
        case TEST_CFFT_RADIX2_F32_1:

          input1.reload(CMSISCFFTBenchmarksF32::INPUTC_F32_ID,mgr,2*this->nbSamples);
          output.create(2*this->nbSamples,CMSISCFFTBenchmarksF32::OUT_F32_ID,mgr);

          this->pSrc=input1.ptr();
          this->pDst=output.ptr();

          status=arm_cfft_init_f32(&cfftInstance,this->nbSamples);
          if (status != ARM_MATH_SUCCESS)
          {
            printf("Error init CMSIS CFFT F32 benchmark for size %d\n",this->nbSamples);
          }
          memcpy(this->pDst,this->pSrc,2*sizeof(float32_t)*this->nbSamples);

        break;

        
        case TEST_CIFFT_RADIX2_F32_2:

          input1.reload(CMSISCFFTBenchmarksF32::INPUTC_F32_ID,mgr,2*this->nbSamples);
          output.create(2*this->nbSamples,CMSISCFFTBenchmarksF32::OUT_F32_ID,mgr);

          this->pSrc=input1.ptr();
          this->pDst=output.ptr();

          status=arm_cfft_init_f32(&cfftInstance,this->nbSamples);
          if (status != ARM_MATH_SUCCESS)
          {
            printf("Error init CMSIS CIFFT F32 benchmark for size %d\n",this->nbSamples);
          }
          memcpy(this->pDst,this->pSrc,2*sizeof(float32_t)*this->nbSamples);


        break;
       
       }

     


    }

    void CMSISCFFTBenchmarksF32::tearDown(Testing::testID_t id,Client::PatternMgr *mgr)
    {
      (void)id;
      (void)mgr;
    }
