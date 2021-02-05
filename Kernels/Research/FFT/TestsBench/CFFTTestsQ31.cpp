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
#include "CFFTTestsQ31.h"
#include <stdio.h>
#include "Error.h"
#include "Test.h"
//#include "FFTDebug.h"
#include "FixedPointScaling.h"

/* Default thresholds */
#define SNR_THRESHOLD 89
#define IFFT_SNR_THRESHOLD 70


#define LONG_SNR_THRESHOLD 40


    void CFFTTestsQ31::test_cfft_q31()
    {
       const q31_t *inp = input.ptr();
       arm_status status;


       q31_t *outfftp = outputfft.ptr();
       q31_t *tmpoutp=tmpout.ptr();
       int fftscaling ,testscaling;

       const cfftconfig<FFTSCI::Q31> *config = cfft_config<FFTSCI::Q31>(input.nbSamples()>>1);

       fftscaling = config->format;
       if (this->ifft)
       {
          testscaling = 2;
       }
       else
       {
          testscaling = scaling(input.nbSamples()>>1);
       }


       //printMath((complex<q31_t>*)inp,input.nbSamples()>>1);

       if (this->ifft)
       {
         status=cifft((FFTSCI::Q31*)inp,(FFTSCI::Q31*)outfftp,input.nbSamples()>>1);
       }
       else
       {
         status=cfft((FFTSCI::Q31*)inp,(FFTSCI::Q31*)outfftp,input.nbSamples()>>1);
         //status=arm_cfft_init_q31(&cfftInstance,input.nbSamples()>>1);
          
         //memcpy(outfftp,inp,sizeof(float32_t)*input.nbSamples());
         //arm_cfft_q31(&(this->cfftInstance), outfftp, 0,1);
       }

       //printMath((complex<q31_t>*)outfftp,input.nbSamples()>>1);

       
       ASSERT_TRUE(status==ARM_MATH_SUCCESS);

       if ((testscaling-fftscaling) > 0)
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


       ASSERT_SNR(tmpout,ref,snr);
       ASSERT_EMPTY_TAIL(outputfft);


        
    } 

    void CFFTTestsQ31::setUp(Testing::testID_t id,std::vector<Testing::param_t>& paramsArgs,Client::PatternMgr *mgr)
    {

       (void)paramsArgs;

       this->snr = SNR_THRESHOLD;
       

       switch(id)
       {
case CFFTTestsQ31::TEST_CFFT_Q31_1:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_NOISY_2_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_NOISY_2_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_2:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_NOISY_3_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_NOISY_3_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_3:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_NOISY_4_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_NOISY_4_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_4:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_NOISY_5_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_NOISY_5_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_5:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_NOISY_6_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_NOISY_6_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_6:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_NOISY_8_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_NOISY_8_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_7:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_NOISY_9_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_NOISY_9_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_8:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_NOISY_10_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_NOISY_10_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_9:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_NOISY_12_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_NOISY_12_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_10:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_NOISY_15_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_NOISY_15_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_11:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_NOISY_16_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_NOISY_16_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_12:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_NOISY_18_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_NOISY_18_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_13:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_NOISY_20_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_NOISY_20_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_14:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_NOISY_24_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_NOISY_24_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_15:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_NOISY_25_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_NOISY_25_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_16:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_NOISY_27_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_NOISY_27_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_17:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_NOISY_30_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_NOISY_30_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_18:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_NOISY_32_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_NOISY_32_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_19:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_NOISY_36_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_NOISY_36_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_20:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_NOISY_40_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_NOISY_40_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_21:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_NOISY_45_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_NOISY_45_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_22:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_NOISY_48_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_NOISY_48_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_23:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_NOISY_50_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_NOISY_50_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_24:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_NOISY_54_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_NOISY_54_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_25:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_NOISY_60_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_NOISY_60_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_26:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_NOISY_64_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_NOISY_64_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  

  case CFFTTestsQ31::TEST_CFFT_Q31_27:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_NOISY_72_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_NOISY_72_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_28:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_NOISY_75_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_NOISY_75_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_29:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_NOISY_80_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_NOISY_80_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_30:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_NOISY_90_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_NOISY_90_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_31:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_NOISY_96_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_NOISY_96_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_32:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_NOISY_100_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_NOISY_100_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_33:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_NOISY_108_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_NOISY_108_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_34:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_NOISY_120_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_NOISY_120_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_35:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_NOISY_125_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_NOISY_125_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_36:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_NOISY_128_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_NOISY_128_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_37:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_NOISY_144_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_NOISY_144_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_38:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_NOISY_150_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_NOISY_150_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_39:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_NOISY_160_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_NOISY_160_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_40:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_NOISY_180_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_NOISY_180_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_41:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_NOISY_192_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_NOISY_192_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_42:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_NOISY_200_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_NOISY_200_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_43:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_NOISY_216_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_NOISY_216_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_44:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_NOISY_240_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_NOISY_240_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_45:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_NOISY_256_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_NOISY_256_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_46:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_NOISY_288_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_NOISY_288_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_47:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_NOISY_320_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_NOISY_320_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_48:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_NOISY_384_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_NOISY_384_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_49:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_NOISY_512_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_NOISY_512_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_50:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_NOISY_4096_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_NOISY_4096_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_51:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_STEP_2_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_STEP_2_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_52:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_STEP_3_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_STEP_3_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_53:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_STEP_4_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_STEP_4_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_54:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_STEP_5_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_STEP_5_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_55:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_STEP_6_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_STEP_6_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_56:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_STEP_8_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_STEP_8_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_57:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_STEP_9_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_STEP_9_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_58:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_STEP_10_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_STEP_10_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_59:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_STEP_12_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_STEP_12_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_60:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_STEP_15_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_STEP_15_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_61:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_STEP_16_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_STEP_16_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_62:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_STEP_18_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_STEP_18_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_63:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_STEP_20_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_STEP_20_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_64:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_STEP_24_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_STEP_24_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_65:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_STEP_25_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_STEP_25_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_66:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_STEP_27_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_STEP_27_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_67:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_STEP_30_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_STEP_30_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_68:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_STEP_32_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_STEP_32_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_69:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_STEP_36_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_STEP_36_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_70:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_STEP_40_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_STEP_40_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_71:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_STEP_45_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_STEP_45_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  

  case CFFTTestsQ31::TEST_CFFT_Q31_72:
    input.reload(CFFTTestsQ31::INPUTS_CFFT_STEP_48_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_STEP_48_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_73:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_STEP_50_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_STEP_50_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_74:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_STEP_54_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_STEP_54_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_75:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_STEP_60_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_STEP_60_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_76:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_STEP_64_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_STEP_64_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_77:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_STEP_72_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_STEP_72_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_78:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_STEP_75_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_STEP_75_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_79:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_STEP_80_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_STEP_80_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_80:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_STEP_90_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_STEP_90_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_81:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_STEP_96_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_STEP_96_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_82:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_STEP_100_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_STEP_100_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_83:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_STEP_108_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_STEP_108_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_84:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_STEP_120_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_STEP_120_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_85:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_STEP_125_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_STEP_125_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_86:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_STEP_128_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_STEP_128_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_87:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_STEP_144_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_STEP_144_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_88:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_STEP_150_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_STEP_150_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_89:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_STEP_160_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_STEP_160_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_90:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_STEP_180_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_STEP_180_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_91:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_STEP_192_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_STEP_192_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_92:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_STEP_200_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_STEP_200_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_93:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_STEP_216_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_STEP_216_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_94:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_STEP_240_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_STEP_240_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_95:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_STEP_256_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_STEP_256_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_96:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_STEP_288_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_STEP_288_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_97:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_STEP_320_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_STEP_320_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_98:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_STEP_384_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_STEP_384_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_99:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_STEP_512_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_STEP_512_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_100:
  
    input.reload(CFFTTestsQ31::INPUTS_CFFT_STEP_4096_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CFFT_STEP_4096_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_101:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_NOISY_2_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_NOISY_2_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_102:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_NOISY_3_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_NOISY_3_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_103:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_NOISY_4_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_NOISY_4_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_104:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_NOISY_5_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_NOISY_5_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_105:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_NOISY_6_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_NOISY_6_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_106:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_NOISY_8_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_NOISY_8_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_107:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_NOISY_9_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_NOISY_9_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_108:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_NOISY_10_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_NOISY_10_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_109:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_NOISY_12_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_NOISY_12_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_110:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_NOISY_15_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_NOISY_15_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_111:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_NOISY_16_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_NOISY_16_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_112:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_NOISY_18_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_NOISY_18_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_113:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_NOISY_20_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_NOISY_20_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_114:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_NOISY_24_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_NOISY_24_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_115:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_NOISY_25_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_NOISY_25_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_116:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_NOISY_27_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_NOISY_27_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_117:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_NOISY_30_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_NOISY_30_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_118:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_NOISY_32_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_NOISY_32_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_119:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_NOISY_36_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_NOISY_36_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_120:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_NOISY_40_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_NOISY_40_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_121:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_NOISY_45_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_NOISY_45_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_122:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_NOISY_48_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_NOISY_48_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_123:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_NOISY_50_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_NOISY_50_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_124:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_NOISY_54_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_NOISY_54_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_125:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_NOISY_60_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_NOISY_60_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_126:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_NOISY_64_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_NOISY_64_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_127:
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_NOISY_72_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_NOISY_72_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_128:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_NOISY_75_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_NOISY_75_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_129:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_NOISY_80_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_NOISY_80_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_130:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_NOISY_90_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_NOISY_90_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_131:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_NOISY_96_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_NOISY_96_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_132:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_NOISY_100_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_NOISY_100_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_133:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_NOISY_108_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_NOISY_108_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_134:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_NOISY_120_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_NOISY_120_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_135:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_NOISY_125_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_NOISY_125_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_136:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_NOISY_128_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_NOISY_128_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_137:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_NOISY_144_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_NOISY_144_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_138:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_NOISY_150_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_NOISY_150_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_139:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_NOISY_160_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_NOISY_160_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_140:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_NOISY_180_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_NOISY_180_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_141:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_NOISY_192_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_NOISY_192_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_142:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_NOISY_200_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_NOISY_200_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_143:
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_NOISY_216_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_NOISY_216_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_144:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_NOISY_240_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_NOISY_240_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_145:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_NOISY_256_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_NOISY_256_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_146:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_NOISY_288_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_NOISY_288_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_147:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_NOISY_320_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_NOISY_320_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_148:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_NOISY_384_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_NOISY_384_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_149:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_NOISY_512_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_NOISY_512_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_150:
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_NOISY_4096_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_NOISY_4096_Q31_ID,mgr);
  
    this->ifft=1;
    this->snr=LONG_SNR_THRESHOLD;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_151:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_STEP_2_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_STEP_2_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_152:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_STEP_3_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_STEP_3_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_153:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_STEP_4_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_STEP_4_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_154:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_STEP_5_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_STEP_5_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_155:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_STEP_6_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_STEP_6_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_156:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_STEP_8_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_STEP_8_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_157:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_STEP_9_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_STEP_9_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_158:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_STEP_10_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_STEP_10_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_159:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_STEP_12_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_STEP_12_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_160:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_STEP_15_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_STEP_15_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_161:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_STEP_16_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_STEP_16_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_162:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_STEP_18_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_STEP_18_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_163:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_STEP_20_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_STEP_20_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_164:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_STEP_24_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_STEP_24_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_165:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_STEP_25_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_STEP_25_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_166:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_STEP_27_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_STEP_27_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_167:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_STEP_30_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_STEP_30_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_168:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_STEP_32_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_STEP_32_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_169:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_STEP_36_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_STEP_36_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_170:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_STEP_40_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_STEP_40_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_171:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_STEP_45_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_STEP_45_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_172:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_STEP_48_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_STEP_48_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_173:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_STEP_50_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_STEP_50_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_174:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_STEP_54_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_STEP_54_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_175:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_STEP_60_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_STEP_60_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_176:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_STEP_64_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_STEP_64_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_177:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_STEP_72_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_STEP_72_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_178:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_STEP_75_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_STEP_75_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_179:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_STEP_80_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_STEP_80_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_180:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_STEP_90_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_STEP_90_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_181:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_STEP_96_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_STEP_96_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_182:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_STEP_100_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_STEP_100_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_183:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_STEP_108_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_STEP_108_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_184:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_STEP_120_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_STEP_120_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_185:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_STEP_125_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_STEP_125_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_186:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_STEP_128_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_STEP_128_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_187:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_STEP_144_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_STEP_144_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_188:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_STEP_150_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_STEP_150_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_189:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_STEP_160_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_STEP_160_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_190:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_STEP_180_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_STEP_180_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_191:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_STEP_192_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_STEP_192_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_192:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_STEP_200_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_STEP_200_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_193:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_STEP_216_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_STEP_216_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_194:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_STEP_240_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_STEP_240_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_195:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_STEP_256_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_STEP_256_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_196:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_STEP_288_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_STEP_288_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_197:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_STEP_320_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_STEP_320_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_198:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_STEP_384_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_STEP_384_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_199:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_STEP_512_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_STEP_512_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ31::TEST_CFFT_Q31_200:
  
    input.reload(CFFTTestsQ31::INPUTS_CIFFT_STEP_4096_Q31_ID,mgr);
    ref.reload(  CFFTTestsQ31::REF_CIFFT_STEP_4096_Q31_ID,mgr);
  
    this->ifft=1;
    this->snr=LONG_SNR_THRESHOLD;
  
  break;
  
  


       }

       if (this->ifft)
       {
         if (this->snr != LONG_SNR_THRESHOLD)
         {
            this->snr = IFFT_SNR_THRESHOLD;
         }
       }
       
        outputfft.create(ref.nbSamples(),CFFTTestsQ31::OUTPUT_CFFT_Q31_ID,mgr);
        tmpin.create(input.nbSamples(),CFFTTestsQ31::TMPIN_CFFT_Q31_ID,mgr);
        tmpout.create(ref.nbSamples(),CFFTTestsQ31::TMPOUT_CFFT_Q31_ID,mgr);


    }

    void CFFTTestsQ31::tearDown(Testing::testID_t id,Client::PatternMgr *mgr)
    {
        (void)id;
        outputfft.dump(mgr);
    }
