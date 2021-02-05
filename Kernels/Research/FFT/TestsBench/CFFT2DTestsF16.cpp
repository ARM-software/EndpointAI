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
#include "CFFT2DTestsF16.h"
#include <stdio.h>
#include "Error.h"
#include "Test.h"
//#include "FFTDebug.h"

#define SNR_THRESHOLD 60

    void CFFT2DTestsF16::test_cfft2d_f16()
    {
       const float16_t *inp = input.ptr();
       arm_status status;

       float16_t *outfftp = outputfft.ptr();

       //printMath((complex<float16_t>*)inp,input.nbSamples()>>1);

       if (this->ifft)
       {
         status=cifft2D(inp,outfftp,this->rows,this->cols);
       }
       else
       {
         status=cfft2D(inp,outfftp,this->rows,this->cols);
       }

       //printMath((complex<float16_t>*)outfftp,input.nbSamples()>>1);

       
       ASSERT_TRUE(status==ARM_MATH_SUCCESS);
       ASSERT_SNR(outputfft,ref,(float16_t)SNR_THRESHOLD);
       ASSERT_EMPTY_TAIL(outputfft);


        
    } 

    void CFFT2DTestsF16::setUp(Testing::testID_t id,std::vector<Testing::param_t>& paramsArgs,Client::PatternMgr *mgr)
    {

       (void)paramsArgs;
       switch(id)
       {
             case CFFT2DTestsF16::TEST_CFFT2D_F16_1:
             
               input.reload(CFFT2DTestsF16::INPUTS_CFFT2D_NOISY_1_F16_ID,mgr);
               ref.reload(  CFFT2DTestsF16::REF_CFFT2D_NOISY_1_F16_ID,mgr);
             
               this->ifft=0;
               this->rows=4;
               this->cols=9;
             
             break;
             
             case CFFT2DTestsF16::TEST_CFFT2D_F16_2:
             
               input.reload(CFFT2DTestsF16::INPUTS_CFFT2D_NOISY_2_F16_ID,mgr);
               ref.reload(  CFFT2DTestsF16::REF_CFFT2D_NOISY_2_F16_ID,mgr);
             
               this->ifft=0;
               this->rows=8;
               this->cols=4;
             
             break;

             case CFFT2DTestsF16::TEST_CFFT2D_F16_3:
             
               input.reload(CFFT2DTestsF16::INPUTS_CIFFT2D_NOISY_1_F16_ID,mgr);
               ref.reload(  CFFT2DTestsF16::REF_CIFFT2D_NOISY_1_F16_ID,mgr);
             
               this->ifft=1;
               this->rows=4;
               this->cols=9;
             
             break;
             
             case CFFT2DTestsF16::TEST_CFFT2D_F16_4:
             
               input.reload(CFFT2DTestsF16::INPUTS_CIFFT2D_NOISY_2_F16_ID,mgr);
               ref.reload(  CFFT2DTestsF16::REF_CIFFT2D_NOISY_2_F16_ID,mgr);
             
               this->ifft=1;
               this->rows=8;
               this->cols=4;
             
             break;


       }
       outputfft.create(ref.nbSamples(),CFFT2DTestsF16::OUTPUT_CFFT2D_F16_ID,mgr);

    }

    void CFFT2DTestsF16::tearDown(Testing::testID_t id,Client::PatternMgr *mgr)
    {
        (void)id;
        outputfft.dump(mgr);
    }
