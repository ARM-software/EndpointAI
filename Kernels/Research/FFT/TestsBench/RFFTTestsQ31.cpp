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
#include "RFFTTestsQ31.h"
#include <stdio.h>
#include "Error.h"
#include "Test.h"
//#include "FFTDebug.h"
#include "FixedPointScaling.h"

/* Default thresholds */
#define SNR_THRESHOLD 89
#define IFFT_SNR_THRESHOLD 70

/* For very long RFFT : 4096 samples 
For noisy patterns. */

#define LONG_SNR_THRESHOLD 48


    void RFFTTestsQ31::test_rfft_q31()
    {
       q31_t *inp = input.ptr();
       arm_status status=ARM_MATH_SUCCESS;

       q31_t *outfftp = outputfft.ptr();
       q31_t *tmpptr = tmp.ptr();

       q31_t *tmpoutp=tmpout.ptr();
       int fftscaling ,testscaling;

       const cfftconfig<FFTSCI::Q31> *config;

       if (this->ifft)
       {
          testscaling = 2;
          config = cfft_config<FFTSCI::Q31>(outputfft.nbSamples()>>1);
       }
       else
       {
          testscaling = scaling(input.nbSamples()>>1);
          config = cfft_config<FFTSCI::Q31>(input.nbSamples()>>1);
       }

       fftscaling = config->format;


       
       //printMath((complex<q31_t>*)inp,input.nbSamples()>>1);

       
       if (this->ifft)
       {
         status=rifft((FFTSCI::Q31*)inp,(FFTSCI::Q31*)tmpptr,(FFTSCI::Q31*)outfftp,outputfft.nbSamples());
                  
       }
       else
       {
        
         status=rfft((FFTSCI::Q31*)inp,(FFTSCI::Q31*)tmpptr,(FFTSCI::Q31*)outfftp,input.nbSamples());
       }

       
       //if (status == ARM_MATH_SUCCESS)
       //{
       //   printMath((complex<q31_t>*)outfftp,outputfft.nbSamples()>>1);
       //}
       
       ASSERT_TRUE(status==ARM_MATH_SUCCESS);

       //printf("%d %d\n",testscaling,fftscaling);

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

       //printf("out=");
       //printMath((complex<q31_t>*)tmpoutp,outputfft.nbSamples()>>1);
       //printf("ref=");
       //printMath((complex<q31_t>*)ref.ptr(),outputfft.nbSamples()>>1);

       ASSERT_SNR(tmpout,ref,(q31_t)snr);
       ASSERT_EMPTY_TAIL(outputfft);

    } 

    void RFFTTestsQ31::setUp(Testing::testID_t id,std::vector<Testing::param_t>& paramsArgs,Client::PatternMgr *mgr)
    {

       (void)paramsArgs;

       this->snr = SNR_THRESHOLD;
      

       switch(id)
       {
case RFFTTestsQ31::TEST_RFFT_Q31_1:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_NOISY_4_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_NOISY_4_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_2:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_NOISY_6_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_NOISY_6_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_3:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_NOISY_8_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_NOISY_8_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_4:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_NOISY_10_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_NOISY_10_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_5:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_NOISY_12_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_NOISY_12_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_6:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_NOISY_16_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_NOISY_16_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_7:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_NOISY_18_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_NOISY_18_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_8:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_NOISY_20_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_NOISY_20_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_9:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_NOISY_24_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_NOISY_24_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_10:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_NOISY_30_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_NOISY_30_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_11:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_NOISY_32_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_NOISY_32_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_12:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_NOISY_36_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_NOISY_36_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_13:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_NOISY_40_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_NOISY_40_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_14:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_NOISY_48_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_NOISY_48_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_15:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_NOISY_50_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_NOISY_50_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_16:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_NOISY_54_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_NOISY_54_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_17:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_NOISY_60_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_NOISY_60_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_18:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_NOISY_64_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_NOISY_64_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_19:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_NOISY_72_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_NOISY_72_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_20:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_NOISY_80_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_NOISY_80_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_21:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_NOISY_90_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_NOISY_90_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_22:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_NOISY_96_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_NOISY_96_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_23:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_NOISY_100_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_NOISY_100_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_24:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_NOISY_108_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_NOISY_108_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_25:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_NOISY_120_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_NOISY_120_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_26:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_NOISY_128_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_NOISY_128_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_27:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_NOISY_144_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_NOISY_144_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_28:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_NOISY_150_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_NOISY_150_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_29:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_NOISY_160_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_NOISY_160_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_30:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_NOISY_180_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_NOISY_180_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_31:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_NOISY_192_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_NOISY_192_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_32:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_NOISY_200_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_NOISY_200_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_33:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_NOISY_216_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_NOISY_216_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_34:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_NOISY_240_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_NOISY_240_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_35:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_NOISY_256_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_NOISY_256_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_36:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_NOISY_288_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_NOISY_288_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_37:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_NOISY_320_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_NOISY_320_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_38:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_NOISY_384_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_NOISY_384_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_39:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_NOISY_512_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_NOISY_512_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_40:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_NOISY_4096_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_NOISY_4096_Q31_ID,mgr);
  
    this->ifft=0;
    this->snr=LONG_SNR_THRESHOLD;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_41:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_STEP_4_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_STEP_4_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_42:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_STEP_6_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_STEP_6_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_43:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_STEP_8_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_STEP_8_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_44:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_STEP_10_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_STEP_10_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_45:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_STEP_12_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_STEP_12_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_46:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_STEP_16_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_STEP_16_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_47:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_STEP_18_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_STEP_18_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_48:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_STEP_20_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_STEP_20_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_49:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_STEP_24_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_STEP_24_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_50:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_STEP_30_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_STEP_30_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_51:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_STEP_32_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_STEP_32_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_52:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_STEP_36_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_STEP_36_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_53:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_STEP_40_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_STEP_40_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_54:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_STEP_48_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_STEP_48_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_55:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_STEP_50_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_STEP_50_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_56:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_STEP_54_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_STEP_54_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_57:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_STEP_60_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_STEP_60_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_58:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_STEP_64_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_STEP_64_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_59:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_STEP_72_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_STEP_72_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_60:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_STEP_80_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_STEP_80_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_61:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_STEP_90_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_STEP_90_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_62:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_STEP_96_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_STEP_96_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_63:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_STEP_100_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_STEP_100_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_64:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_STEP_108_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_STEP_108_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_65:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_STEP_120_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_STEP_120_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_66:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_STEP_128_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_STEP_128_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_67:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_STEP_144_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_STEP_144_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_68:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_STEP_150_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_STEP_150_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_69:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_STEP_160_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_STEP_160_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_70:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_STEP_180_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_STEP_180_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_71:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_STEP_192_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_STEP_192_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_72:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_STEP_200_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_STEP_200_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_73:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_STEP_216_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_STEP_216_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_74:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_STEP_240_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_STEP_240_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_75:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_STEP_256_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_STEP_256_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_76:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_STEP_288_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_STEP_288_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_77:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_STEP_320_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_STEP_320_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_78:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_STEP_384_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_STEP_384_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_79:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_STEP_512_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_STEP_512_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_80:
  
    input.reload(RFFTTestsQ31::INPUTS_RFFT_STEP_4096_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RFFT_STEP_4096_Q31_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_81:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_NOISY_4_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_NOISY_4_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_82:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_NOISY_6_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_NOISY_6_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_83:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_NOISY_8_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_NOISY_8_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_84:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_NOISY_10_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_NOISY_10_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_85:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_NOISY_12_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_NOISY_12_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_86:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_NOISY_16_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_NOISY_16_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_87:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_NOISY_18_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_NOISY_18_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_88:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_NOISY_20_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_NOISY_20_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_89:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_NOISY_24_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_NOISY_24_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_90:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_NOISY_30_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_NOISY_30_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_91:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_NOISY_32_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_NOISY_32_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_92:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_NOISY_36_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_NOISY_36_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_93:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_NOISY_40_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_NOISY_40_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_94:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_NOISY_48_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_NOISY_48_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_95:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_NOISY_50_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_NOISY_50_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_96:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_NOISY_54_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_NOISY_54_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_97:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_NOISY_60_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_NOISY_60_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_98:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_NOISY_64_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_NOISY_64_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_99:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_NOISY_72_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_NOISY_72_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_100:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_NOISY_80_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_NOISY_80_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_101:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_NOISY_90_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_NOISY_90_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_102:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_NOISY_96_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_NOISY_96_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_103:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_NOISY_100_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_NOISY_100_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_104:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_NOISY_108_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_NOISY_108_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_105:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_NOISY_120_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_NOISY_120_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_106:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_NOISY_128_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_NOISY_128_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_107:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_NOISY_144_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_NOISY_144_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_108:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_NOISY_150_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_NOISY_150_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_109:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_NOISY_160_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_NOISY_160_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_110:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_NOISY_180_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_NOISY_180_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_111:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_NOISY_192_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_NOISY_192_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_112:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_NOISY_200_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_NOISY_200_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_113:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_NOISY_216_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_NOISY_216_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_114:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_NOISY_240_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_NOISY_240_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_115:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_NOISY_256_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_NOISY_256_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_116:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_NOISY_288_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_NOISY_288_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_117:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_NOISY_320_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_NOISY_320_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_118:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_NOISY_384_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_NOISY_384_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_119:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_NOISY_512_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_NOISY_512_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_120:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_NOISY_4096_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_NOISY_4096_Q31_ID,mgr);
  
    this->ifft=1;
    this->snr=LONG_SNR_THRESHOLD;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_121:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_STEP_4_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_STEP_4_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_122:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_STEP_6_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_STEP_6_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_123:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_STEP_8_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_STEP_8_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_124:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_STEP_10_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_STEP_10_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_125:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_STEP_12_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_STEP_12_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_126:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_STEP_16_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_STEP_16_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_127:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_STEP_18_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_STEP_18_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_128:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_STEP_20_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_STEP_20_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_129:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_STEP_24_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_STEP_24_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_130:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_STEP_30_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_STEP_30_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_131:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_STEP_32_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_STEP_32_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_132:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_STEP_36_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_STEP_36_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_133:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_STEP_40_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_STEP_40_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_134:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_STEP_48_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_STEP_48_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_135:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_STEP_50_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_STEP_50_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_136:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_STEP_54_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_STEP_54_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_137:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_STEP_60_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_STEP_60_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_138:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_STEP_64_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_STEP_64_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_139:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_STEP_72_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_STEP_72_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_140:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_STEP_80_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_STEP_80_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_141:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_STEP_90_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_STEP_90_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_142:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_STEP_96_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_STEP_96_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_143:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_STEP_100_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_STEP_100_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_144:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_STEP_108_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_STEP_108_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_145:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_STEP_120_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_STEP_120_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_146:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_STEP_128_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_STEP_128_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_147:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_STEP_144_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_STEP_144_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_148:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_STEP_150_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_STEP_150_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_149:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_STEP_160_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_STEP_160_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_150:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_STEP_180_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_STEP_180_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_151:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_STEP_192_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_STEP_192_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_152:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_STEP_200_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_STEP_200_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_153:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_STEP_216_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_STEP_216_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_154:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_STEP_240_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_STEP_240_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_155:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_STEP_256_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_STEP_256_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_156:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_STEP_288_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_STEP_288_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_157:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_STEP_320_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_STEP_320_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_158:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_STEP_384_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_STEP_384_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_159:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_STEP_512_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_STEP_512_Q31_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ31::TEST_RFFT_Q31_160:
  
    input.reload(RFFTTestsQ31::INPUTS_RIFFT_STEP_4096_Q31_ID,mgr);
    ref.reload(  RFFTTestsQ31::REF_RIFFT_STEP_4096_Q31_ID,mgr);
  
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

        if (this->ifft==0)
        {
           outputfft.create(ref.nbSamples(),RFFTTestsQ31::OUTPUT_RFFT_Q31_ID,mgr);
           tmp.create(input.nbSamples(),RFFTTestsQ31::TMP_Q31_ID,mgr);

           tmpout.create(ref.nbSamples(),RFFTTestsQ31::TMPOUT_RFFT_Q31_ID,mgr);

        }
        else
        {
          outputfft.create(ref.nbSamples(),RFFTTestsQ31::OUTPUT_RFFT_Q31_ID,mgr);
          tmp.create(ref.nbSamples(),RFFTTestsQ31::TMP_Q31_ID,mgr);

          tmpout.create(ref.nbSamples(),RFFTTestsQ31::TMPOUT_RFFT_Q31_ID,mgr);

        }

    }

    void RFFTTestsQ31::tearDown(Testing::testID_t id,Client::PatternMgr *mgr)
    {
        (void)id;
        outputfft.dump(mgr);
    }
