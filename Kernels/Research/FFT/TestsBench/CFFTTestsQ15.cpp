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
#include "CFFTTestsQ15.h"
#include <stdio.h>
#include "Error.h"
#include "Test.h"
#include "FFTDebug.h"
#include "FixedPointScaling.h"


#define SNR_THRESHOLD 26
#define IFFT_SNR_THRESHOLD 20

#define BAD_SNR_THRESHOLD 15

    void CFFTTestsQ15::test_cfft_q15()
    {

       const q15_t *inp = input.ptr();
       arm_status status;


       q15_t *outfftp = outputfft.ptr();
       q15_t *tmpoutp=tmpout.ptr();
       int fftscaling ,testscaling;

       const cfftconfig<FFTSCI::Q15> *config = cfft_config<FFTSCI::Q15>(input.nbSamples()>>1);

       fftscaling = config->format;
       if (this->ifft)
       {
          testscaling = 2;
       }
       else
       {
          testscaling = scaling(input.nbSamples()>>1);
       }


       //printMath((complex<q15_t>*)inp,input.nbSamples()>>1);

       if (this->ifft)
       {
         status=cifft((FFTSCI::Q15*)inp,(FFTSCI::Q15*)outfftp,input.nbSamples()>>1);
       }
       else
       {
         status=cfft((FFTSCI::Q15*)inp,(FFTSCI::Q15*)outfftp,input.nbSamples()>>1);
         //status=arm_cfft_init_q15(&cfftInstance,input.nbSamples()>>1);
          
         //memcpy(outfftp,inp,sizeof(float32_t)*input.nbSamples());
         //arm_cfft_q15(&(this->cfftInstance), outfftp, 0,1);
       }

       //printMath((complex<q15_t>*)outfftp,input.nbSamples()>>1);

       
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
             tmpoutp[i] = __SSAT(((q31_t)outfftp[i] << (fftscaling - testscaling)),16);
          }
       }

       //printMath((complex<q15_t>*)tmpoutp,input.nbSamples()>>1);

       ASSERT_SNR(tmpout,ref,(q15_t)this->snr);
       ASSERT_EMPTY_TAIL(outputfft);

    } 

    void CFFTTestsQ15::setUp(Testing::testID_t id,std::vector<Testing::param_t>& paramsArgs,Client::PatternMgr *mgr)
    {

       this->snr=SNR_THRESHOLD;

      

       (void)paramsArgs;
       switch(id)
       {
case CFFTTestsQ15::TEST_CFFT_Q15_1:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_NOISY_2_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_NOISY_2_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_2:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_NOISY_3_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_NOISY_3_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_3:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_NOISY_4_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_NOISY_4_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_4:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_NOISY_5_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_NOISY_5_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_5:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_NOISY_6_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_NOISY_6_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_6:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_NOISY_8_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_NOISY_8_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_7:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_NOISY_9_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_NOISY_9_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_8:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_NOISY_10_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_NOISY_10_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_9:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_NOISY_12_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_NOISY_12_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_10:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_NOISY_15_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_NOISY_15_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_11:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_NOISY_16_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_NOISY_16_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_12:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_NOISY_18_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_NOISY_18_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_13:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_NOISY_20_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_NOISY_20_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_14:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_NOISY_24_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_NOISY_24_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_15:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_NOISY_25_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_NOISY_25_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_16:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_NOISY_27_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_NOISY_27_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_17:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_NOISY_30_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_NOISY_30_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_18:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_NOISY_32_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_NOISY_32_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_19:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_NOISY_36_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_NOISY_36_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_20:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_NOISY_40_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_NOISY_40_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_21:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_NOISY_45_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_NOISY_45_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_22:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_NOISY_48_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_NOISY_48_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_23:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_NOISY_50_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_NOISY_50_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_24:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_NOISY_54_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_NOISY_54_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_25:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_NOISY_60_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_NOISY_60_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_26:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_NOISY_64_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_NOISY_64_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_27:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_NOISY_72_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_NOISY_72_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_28:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_NOISY_75_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_NOISY_75_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_29:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_NOISY_80_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_NOISY_80_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_30:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_NOISY_90_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_NOISY_90_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_31:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_NOISY_96_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_NOISY_96_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_32:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_NOISY_100_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_NOISY_100_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_33:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_NOISY_108_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_NOISY_108_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_34:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_NOISY_120_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_NOISY_120_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_35:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_NOISY_125_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_NOISY_125_Q15_ID,mgr);
  
    this->ifft=0;
    this->snr=BAD_SNR_THRESHOLD;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_36:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_NOISY_128_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_NOISY_128_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_37:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_NOISY_144_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_NOISY_144_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_38:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_NOISY_150_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_NOISY_150_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_39:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_NOISY_160_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_NOISY_160_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_40:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_NOISY_180_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_NOISY_180_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_41:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_NOISY_192_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_NOISY_192_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_42:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_NOISY_200_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_NOISY_200_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_43:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_NOISY_216_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_NOISY_216_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_44:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_NOISY_240_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_NOISY_240_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_45:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_NOISY_256_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_NOISY_256_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_46:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_NOISY_288_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_NOISY_288_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_47:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_NOISY_320_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_NOISY_320_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_48:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_NOISY_384_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_NOISY_384_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_49:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_NOISY_512_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_NOISY_512_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_50:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_NOISY_4096_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_NOISY_4096_Q15_ID,mgr);
  
    this->ifft=0;
    this->snr=BAD_SNR_THRESHOLD;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_51:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_STEP_2_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_STEP_2_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_52:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_STEP_3_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_STEP_3_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_53:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_STEP_4_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_STEP_4_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_54:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_STEP_5_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_STEP_5_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_55:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_STEP_6_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_STEP_6_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_56:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_STEP_8_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_STEP_8_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_57:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_STEP_9_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_STEP_9_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_58:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_STEP_10_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_STEP_10_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_59:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_STEP_12_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_STEP_12_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_60:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_STEP_15_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_STEP_15_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_61:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_STEP_16_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_STEP_16_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_62:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_STEP_18_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_STEP_18_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_63:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_STEP_20_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_STEP_20_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_64:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_STEP_24_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_STEP_24_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_65:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_STEP_25_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_STEP_25_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_66:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_STEP_27_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_STEP_27_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_67:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_STEP_30_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_STEP_30_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_68:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_STEP_32_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_STEP_32_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_69:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_STEP_36_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_STEP_36_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_70:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_STEP_40_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_STEP_40_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_71:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_STEP_45_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_STEP_45_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_72:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_STEP_48_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_STEP_48_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_73:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_STEP_50_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_STEP_50_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_74:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_STEP_54_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_STEP_54_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_75:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_STEP_60_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_STEP_60_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_76:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_STEP_64_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_STEP_64_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_77:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_STEP_72_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_STEP_72_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_78:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_STEP_75_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_STEP_75_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_79:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_STEP_80_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_STEP_80_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_80:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_STEP_90_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_STEP_90_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_81:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_STEP_96_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_STEP_96_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_82:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_STEP_100_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_STEP_100_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_83:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_STEP_108_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_STEP_108_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_84:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_STEP_120_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_STEP_120_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_85:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_STEP_125_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_STEP_125_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_86:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_STEP_128_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_STEP_128_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_87:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_STEP_144_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_STEP_144_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_88:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_STEP_150_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_STEP_150_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_89:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_STEP_160_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_STEP_160_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_90:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_STEP_180_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_STEP_180_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_91:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_STEP_192_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_STEP_192_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_92:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_STEP_200_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_STEP_200_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_93:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_STEP_216_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_STEP_216_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_94:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_STEP_240_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_STEP_240_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_95:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_STEP_256_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_STEP_256_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_96:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_STEP_288_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_STEP_288_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_97:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_STEP_320_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_STEP_320_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_98:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_STEP_384_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_STEP_384_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_99:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_STEP_512_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_STEP_512_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_100:
  
    input.reload(CFFTTestsQ15::INPUTS_CFFT_STEP_4096_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CFFT_STEP_4096_Q15_ID,mgr);
  
    this->ifft=0;
    this->snr=BAD_SNR_THRESHOLD;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_101:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_NOISY_2_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_NOISY_2_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_102:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_NOISY_3_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_NOISY_3_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_103:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_NOISY_4_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_NOISY_4_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_104:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_NOISY_5_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_NOISY_5_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_105:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_NOISY_6_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_NOISY_6_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_106:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_NOISY_8_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_NOISY_8_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_107:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_NOISY_9_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_NOISY_9_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_108:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_NOISY_10_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_NOISY_10_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_109:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_NOISY_12_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_NOISY_12_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_110:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_NOISY_15_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_NOISY_15_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_111:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_NOISY_16_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_NOISY_16_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_112:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_NOISY_18_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_NOISY_18_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_113:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_NOISY_20_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_NOISY_20_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_114:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_NOISY_24_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_NOISY_24_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_115:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_NOISY_25_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_NOISY_25_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_116:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_NOISY_27_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_NOISY_27_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_117:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_NOISY_30_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_NOISY_30_Q15_ID,mgr);
  
    this->ifft=1;
    this->snr=BAD_SNR_THRESHOLD;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_118:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_NOISY_32_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_NOISY_32_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_119:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_NOISY_36_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_NOISY_36_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_120:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_NOISY_40_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_NOISY_40_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_121:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_NOISY_45_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_NOISY_45_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_122:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_NOISY_48_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_NOISY_48_Q15_ID,mgr);
  
    this->ifft=1;
    this->snr=BAD_SNR_THRESHOLD;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_123:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_NOISY_50_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_NOISY_50_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_124:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_NOISY_54_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_NOISY_54_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_125:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_NOISY_60_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_NOISY_60_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_126:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_NOISY_64_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_NOISY_64_Q15_ID,mgr);
  
    this->ifft=1;
    this->snr=BAD_SNR_THRESHOLD;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_127:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_NOISY_72_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_NOISY_72_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_128:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_NOISY_75_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_NOISY_75_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_129:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_NOISY_80_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_NOISY_80_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_130:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_NOISY_90_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_NOISY_90_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_131:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_NOISY_96_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_NOISY_96_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_132:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_NOISY_100_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_NOISY_100_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_133:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_NOISY_108_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_NOISY_108_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_134:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_NOISY_120_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_NOISY_120_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_135:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_NOISY_125_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_NOISY_125_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_136:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_NOISY_128_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_NOISY_128_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_137:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_NOISY_144_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_NOISY_144_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_138:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_NOISY_150_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_NOISY_150_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_139:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_NOISY_160_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_NOISY_160_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_140:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_NOISY_180_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_NOISY_180_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_141:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_NOISY_192_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_NOISY_192_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_142:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_NOISY_200_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_NOISY_200_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_143:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_NOISY_216_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_NOISY_216_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_144:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_NOISY_240_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_NOISY_240_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_145:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_NOISY_256_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_NOISY_256_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_146:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_NOISY_288_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_NOISY_288_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_147:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_NOISY_320_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_NOISY_320_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_148:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_NOISY_384_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_NOISY_384_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_149:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_NOISY_512_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_NOISY_512_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_150:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_NOISY_4096_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_NOISY_4096_Q15_ID,mgr);
  
    this->ifft=1;
    this->snr=BAD_SNR_THRESHOLD;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_151:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_STEP_2_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_STEP_2_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_152:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_STEP_3_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_STEP_3_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_153:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_STEP_4_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_STEP_4_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_154:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_STEP_5_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_STEP_5_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_155:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_STEP_6_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_STEP_6_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_156:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_STEP_8_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_STEP_8_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_157:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_STEP_9_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_STEP_9_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_158:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_STEP_10_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_STEP_10_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_159:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_STEP_12_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_STEP_12_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_160:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_STEP_15_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_STEP_15_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_161:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_STEP_16_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_STEP_16_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_162:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_STEP_18_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_STEP_18_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_163:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_STEP_20_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_STEP_20_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_164:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_STEP_24_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_STEP_24_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_165:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_STEP_25_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_STEP_25_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_166:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_STEP_27_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_STEP_27_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_167:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_STEP_30_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_STEP_30_Q15_ID,mgr);
  
    this->ifft=1;
    this->snr=BAD_SNR_THRESHOLD;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_168:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_STEP_32_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_STEP_32_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_169:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_STEP_36_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_STEP_36_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_170:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_STEP_40_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_STEP_40_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_171:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_STEP_45_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_STEP_45_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_172:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_STEP_48_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_STEP_48_Q15_ID,mgr);
  
    this->ifft=1;
    this->snr=BAD_SNR_THRESHOLD;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_173:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_STEP_50_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_STEP_50_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_174:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_STEP_54_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_STEP_54_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_175:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_STEP_60_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_STEP_60_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_176:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_STEP_64_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_STEP_64_Q15_ID,mgr);
  
    this->ifft=1;
    this->snr=BAD_SNR_THRESHOLD;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_177:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_STEP_72_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_STEP_72_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_178:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_STEP_75_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_STEP_75_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_179:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_STEP_80_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_STEP_80_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_180:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_STEP_90_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_STEP_90_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_181:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_STEP_96_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_STEP_96_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_182:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_STEP_100_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_STEP_100_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_183:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_STEP_108_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_STEP_108_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_184:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_STEP_120_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_STEP_120_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_185:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_STEP_125_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_STEP_125_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_186:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_STEP_128_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_STEP_128_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_187:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_STEP_144_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_STEP_144_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_188:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_STEP_150_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_STEP_150_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_189:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_STEP_160_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_STEP_160_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_190:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_STEP_180_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_STEP_180_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_191:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_STEP_192_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_STEP_192_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_192:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_STEP_200_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_STEP_200_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_193:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_STEP_216_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_STEP_216_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_194:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_STEP_240_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_STEP_240_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_195:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_STEP_256_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_STEP_256_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_196:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_STEP_288_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_STEP_288_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_197:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_STEP_320_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_STEP_320_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_198:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_STEP_384_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_STEP_384_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_199:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_STEP_512_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_STEP_512_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case CFFTTestsQ15::TEST_CFFT_Q15_200:
  
    input.reload(CFFTTestsQ15::INPUTS_CIFFT_STEP_4096_Q15_ID,mgr);
    ref.reload(  CFFTTestsQ15::REF_CIFFT_STEP_4096_Q15_ID,mgr);
  
    this->ifft=1;
    this->snr=BAD_SNR_THRESHOLD;
  
  break;
  
  


       }

        if (this->ifft)
       {
         if (this->snr != BAD_SNR_THRESHOLD)
         {
           this->snr = IFFT_SNR_THRESHOLD;
         }
         
       }
       
        outputfft.create(ref.nbSamples(),CFFTTestsQ15::OUTPUT_CFFT_Q15_ID,mgr);
        tmpin.create(input.nbSamples(),CFFTTestsQ15::TMPIN_CFFT_Q15_ID,mgr);
        tmpout.create(ref.nbSamples(),CFFTTestsQ15::TMPOUT_CFFT_Q15_ID,mgr);


    }

    void CFFTTestsQ15::tearDown(Testing::testID_t id,Client::PatternMgr *mgr)
    {
        (void)id;
        outputfft.dump(mgr);
    }
