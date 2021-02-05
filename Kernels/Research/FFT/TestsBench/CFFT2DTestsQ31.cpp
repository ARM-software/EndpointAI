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
#include "CFFT2DTestsQ31.h"
#include <stdio.h>
#include "Error.h"
#include "Test.h"
//#include "FFTDebug.h"
#include "FixedPointScaling.h"


#define SNR_THRESHOLD 120

    void CFFT2DTestsQ31::test_cfft2d_q31()
    {
       const q31_t *inp = input.ptr();
       arm_status status;

       q31_t *outfftp = outputfft.ptr();
       q31_t *tmpoutp=tmpout.ptr();


       int fftscaling ,testscaling;

       const cfftconfig<FFTSCI::Q31> *configRows = cfft_config<FFTSCI::Q31>(this->rows);
       const cfftconfig<FFTSCI::Q31> *configCols = cfft_config<FFTSCI::Q31>(this->cols);

       fftscaling = configRows->format + configCols->format;
       if (this->ifft)
       {
          testscaling = 2;
       }
       else
       {
          testscaling = scaling(this->rows) + scaling(this->cols);
       }
      
       //printMath((complex<q31_t>*)inp,this->rows,this->cols);

       if (this->ifft)
       {
         status=cifft2D((FFTSCI::Q31*)inp,(FFTSCI::Q31*)outfftp,this->rows,this->cols);
       }
       else
       {
         status=cfft2D((FFTSCI::Q31*)inp,(FFTSCI::Q31*)outfftp,this->rows,this->cols);
       }

       //printMath((complex<q31_t>*)outfftp,this->rows,this->cols);

       
       ASSERT_TRUE(status==ARM_MATH_SUCCESS);

       if ((testscaling-fftscaling) >= 0)
       {
          for(Testing::nbSamples_t i=0; i < outputfft.nbSamples(); i ++)
          {
             // The input of the IFFT is the scaled FFT
             // So the output will be iput signal scaled + the
             // scaling due to the FFT itself.
             tmpoutp[i] = outfftp[i] >> (testscaling-fftscaling);
          }
       }
       else
       {
          for(Testing::nbSamples_t i=0; i < outputfft.nbSamples(); i ++)
          {
             // The input of the IFFT is the scaled FFT
             // So the output will be iput signal scaled + the
             // scaling due to the FFT itself.
             tmpoutp[i] = ((q63_t)outfftp[i] << (fftscaling - testscaling));
          }
       }

       ASSERT_SNR(tmpout,ref,(q31_t)SNR_THRESHOLD);
       ASSERT_EMPTY_TAIL(outputfft);


        
    } 

    void CFFT2DTestsQ31::setUp(Testing::testID_t id,std::vector<Testing::param_t>& paramsArgs,Client::PatternMgr *mgr)
    {

       (void)paramsArgs;
       switch(id)
       {
             case CFFT2DTestsQ31::TEST_CFFT2D_Q31_1:
             
               input.reload(CFFT2DTestsQ31::INPUTS_CFFT2D_NOISY_1_Q31_ID,mgr);
               ref.reload(  CFFT2DTestsQ31::REF_CFFT2D_NOISY_1_Q31_ID,mgr);
             
               this->ifft=0;
               this->rows=4;
               this->cols=9;
             
             break;
             
             case CFFT2DTestsQ31::TEST_CFFT2D_Q31_2:
             
               input.reload(CFFT2DTestsQ31::INPUTS_CFFT2D_NOISY_2_Q31_ID,mgr);
               ref.reload(  CFFT2DTestsQ31::REF_CFFT2D_NOISY_2_Q31_ID,mgr);
             
               this->ifft=0;
               this->rows=8;
               this->cols=4;
             
             break;

             case CFFT2DTestsQ31::TEST_CFFT2D_Q31_3:
             
               input.reload(CFFT2DTestsQ31::INPUTS_CIFFT2D_NOISY_1_Q31_ID,mgr);
               ref.reload(  CFFT2DTestsQ31::REF_CIFFT2D_NOISY_1_Q31_ID,mgr);
             
               this->ifft=1;
               this->rows=4;
               this->cols=9;
             
             break;
             
             case CFFT2DTestsQ31::TEST_CFFT2D_Q31_4:
             
               input.reload(CFFT2DTestsQ31::INPUTS_CIFFT2D_NOISY_2_Q31_ID,mgr);
               ref.reload(  CFFT2DTestsQ31::REF_CIFFT2D_NOISY_2_Q31_ID,mgr);
             
               this->ifft=1;
               this->rows=8;
               this->cols=4;
             
             break;

       }
       outputfft.create(ref.nbSamples(),CFFT2DTestsQ31::OUTPUT_CFFT2D_Q31_ID,mgr);
       tmpout.create(ref.nbSamples(),CFFT2DTestsQ31::TMPOUT_CFFT_Q31_ID,mgr);

    }

    void CFFT2DTestsQ31::tearDown(Testing::testID_t id,Client::PatternMgr *mgr)
    {
        (void)id;
        outputfft.dump(mgr);
    }
