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
#include "RFFTTestsQ15.h"
#include <stdio.h>
#include "Error.h"
#include "Test.h"
//#include "FFTDebug.h"
#include "FixedPointScaling.h"

/* Default thresholds */
#define SNR_THRESHOLD 33
#define LOW_SNR_THRESHOLD 29

#define IFFT_SNR_THRESHOLD 24

/* For very long RFFT : 4096 samples 
For noisy patterns. */

#define LONG_SNR_THRESHOLD 25


    void RFFTTestsQ15::test_rfft_q15()
    {
       q15_t *inp = input.ptr();
       arm_status status=ARM_MATH_SUCCESS;

       q15_t *outfftp = outputfft.ptr();
       q15_t *tmpptr = tmp.ptr();

       q15_t *tmpoutp=tmpout.ptr();
       int fftscaling ,testscaling;

       const cfftconfig<FFTSCI::Q15> *config;

       if (this->ifft)
       {
          testscaling = 2;
          config = cfft_config<FFTSCI::Q15>(outputfft.nbSamples()>>1);
       }
       else
       {
          testscaling = scaling(input.nbSamples()>>1);
          config = cfft_config<FFTSCI::Q15>(input.nbSamples()>>1);
       }

       fftscaling = config->format;


       
       //printMath((complex<q15_t>*)inp,input.nbSamples()>>1);

       
       if (this->ifft)
       {
         status=rifft((FFTSCI::Q15*)inp,(FFTSCI::Q15*)tmpptr,(FFTSCI::Q15*)outfftp,outputfft.nbSamples());
                  
       }
       else
       {
         status=rfft((FFTSCI::Q15*)inp,(FFTSCI::Q15*)tmpptr,(FFTSCI::Q15*)outfftp,input.nbSamples());
       }

       
       //if (status == ARM_MATH_SUCCESS)
       //{
       //   printMath((complex<q15_t>*)outfftp,outputfft.nbSamples()>>1);
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
       //printMath((complex<q15_t>*)tmpoutp,outputfft.nbSamples()>>1);
       //printf("ref=");
       //printMath((complex<q15_t>*)ref.ptr(),outputfft.nbSamples()>>1);

       ASSERT_SNR(tmpout,ref,(q15_t)snr);
       ASSERT_EMPTY_TAIL(outputfft);

    } 

    void RFFTTestsQ15::setUp(Testing::testID_t id,std::vector<Testing::param_t>& paramsArgs,Client::PatternMgr *mgr)
    {

       (void)paramsArgs;

       this->snr = SNR_THRESHOLD;
      

       switch(id)
       {
case RFFTTestsQ15::TEST_RFFT_Q15_1:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_NOISY_4_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_NOISY_4_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_2:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_NOISY_6_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_NOISY_6_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_3:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_NOISY_8_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_NOISY_8_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_4:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_NOISY_10_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_NOISY_10_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_5:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_NOISY_12_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_NOISY_12_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_6:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_NOISY_16_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_NOISY_16_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_7:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_NOISY_18_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_NOISY_18_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_8:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_NOISY_20_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_NOISY_20_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_9:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_NOISY_24_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_NOISY_24_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_10:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_NOISY_30_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_NOISY_30_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_11:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_NOISY_32_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_NOISY_32_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_12:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_NOISY_36_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_NOISY_36_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_13:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_NOISY_40_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_NOISY_40_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_14:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_NOISY_48_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_NOISY_48_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_15:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_NOISY_50_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_NOISY_50_Q15_ID,mgr);
  
    this->ifft=0;
    this->snr = LOW_SNR_THRESHOLD;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_16:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_NOISY_54_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_NOISY_54_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_17:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_NOISY_60_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_NOISY_60_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_18:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_NOISY_64_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_NOISY_64_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_19:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_NOISY_72_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_NOISY_72_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_20:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_NOISY_80_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_NOISY_80_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_21:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_NOISY_90_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_NOISY_90_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_22:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_NOISY_96_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_NOISY_96_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_23:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_NOISY_100_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_NOISY_100_Q15_ID,mgr);
  
    this->ifft=0;
    this->snr = LOW_SNR_THRESHOLD;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_24:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_NOISY_108_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_NOISY_108_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_25:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_NOISY_120_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_NOISY_120_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_26:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_NOISY_128_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_NOISY_128_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_27:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_NOISY_144_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_NOISY_144_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_28:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_NOISY_150_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_NOISY_150_Q15_ID,mgr);
  
    this->ifft=0;
    this->snr = LOW_SNR_THRESHOLD;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_29:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_NOISY_160_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_NOISY_160_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_30:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_NOISY_180_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_NOISY_180_Q15_ID,mgr);
  
    this->ifft=0;
    this->snr = LOW_SNR_THRESHOLD;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_31:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_NOISY_192_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_NOISY_192_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_32:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_NOISY_200_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_NOISY_200_Q15_ID,mgr);
  
    this->ifft=0;
    this->snr = LOW_SNR_THRESHOLD;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_33:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_NOISY_216_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_NOISY_216_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_34:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_NOISY_240_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_NOISY_240_Q15_ID,mgr);
  
    this->ifft=0;
    this->snr = LOW_SNR_THRESHOLD;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_35:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_NOISY_256_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_NOISY_256_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_36:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_NOISY_288_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_NOISY_288_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_37:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_NOISY_320_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_NOISY_320_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_38:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_NOISY_384_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_NOISY_384_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_39:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_NOISY_512_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_NOISY_512_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_40:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_NOISY_4096_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_NOISY_4096_Q15_ID,mgr);
  
    this->ifft=0;
    this->snr=LONG_SNR_THRESHOLD;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_41:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_STEP_4_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_STEP_4_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_42:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_STEP_6_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_STEP_6_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_43:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_STEP_8_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_STEP_8_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_44:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_STEP_10_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_STEP_10_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_45:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_STEP_12_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_STEP_12_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_46:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_STEP_16_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_STEP_16_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_47:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_STEP_18_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_STEP_18_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_48:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_STEP_20_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_STEP_20_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_49:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_STEP_24_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_STEP_24_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_50:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_STEP_30_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_STEP_30_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_51:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_STEP_32_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_STEP_32_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_52:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_STEP_36_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_STEP_36_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_53:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_STEP_40_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_STEP_40_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_54:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_STEP_48_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_STEP_48_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_55:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_STEP_50_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_STEP_50_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_56:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_STEP_54_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_STEP_54_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_57:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_STEP_60_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_STEP_60_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_58:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_STEP_64_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_STEP_64_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_59:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_STEP_72_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_STEP_72_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_60:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_STEP_80_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_STEP_80_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_61:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_STEP_90_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_STEP_90_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_62:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_STEP_96_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_STEP_96_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_63:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_STEP_100_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_STEP_100_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_64:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_STEP_108_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_STEP_108_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_65:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_STEP_120_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_STEP_120_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_66:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_STEP_128_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_STEP_128_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_67:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_STEP_144_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_STEP_144_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_68:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_STEP_150_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_STEP_150_Q15_ID,mgr);
  
    this->ifft=0;
    this->snr = LOW_SNR_THRESHOLD;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_69:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_STEP_160_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_STEP_160_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_70:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_STEP_180_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_STEP_180_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_71:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_STEP_192_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_STEP_192_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_72:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_STEP_200_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_STEP_200_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_73:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_STEP_216_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_STEP_216_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_74:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_STEP_240_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_STEP_240_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_75:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_STEP_256_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_STEP_256_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_76:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_STEP_288_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_STEP_288_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_77:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_STEP_320_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_STEP_320_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_78:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_STEP_384_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_STEP_384_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_79:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_STEP_512_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_STEP_512_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_80:
  
    input.reload(RFFTTestsQ15::INPUTS_RFFT_STEP_4096_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RFFT_STEP_4096_Q15_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_81:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_NOISY_4_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_NOISY_4_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_82:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_NOISY_6_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_NOISY_6_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_83:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_NOISY_8_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_NOISY_8_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_84:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_NOISY_10_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_NOISY_10_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_85:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_NOISY_12_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_NOISY_12_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_86:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_NOISY_16_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_NOISY_16_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_87:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_NOISY_18_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_NOISY_18_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_88:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_NOISY_20_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_NOISY_20_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_89:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_NOISY_24_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_NOISY_24_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_90:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_NOISY_30_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_NOISY_30_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_91:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_NOISY_32_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_NOISY_32_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_92:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_NOISY_36_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_NOISY_36_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_93:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_NOISY_40_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_NOISY_40_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_94:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_NOISY_48_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_NOISY_48_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_95:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_NOISY_50_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_NOISY_50_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_96:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_NOISY_54_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_NOISY_54_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_97:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_NOISY_60_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_NOISY_60_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_98:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_NOISY_64_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_NOISY_64_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_99:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_NOISY_72_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_NOISY_72_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_100:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_NOISY_80_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_NOISY_80_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_101:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_NOISY_90_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_NOISY_90_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_102:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_NOISY_96_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_NOISY_96_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_103:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_NOISY_100_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_NOISY_100_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_104:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_NOISY_108_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_NOISY_108_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_105:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_NOISY_120_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_NOISY_120_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_106:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_NOISY_128_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_NOISY_128_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_107:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_NOISY_144_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_NOISY_144_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_108:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_NOISY_150_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_NOISY_150_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_109:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_NOISY_160_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_NOISY_160_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_110:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_NOISY_180_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_NOISY_180_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_111:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_NOISY_192_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_NOISY_192_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_112:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_NOISY_200_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_NOISY_200_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_113:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_NOISY_216_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_NOISY_216_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_114:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_NOISY_240_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_NOISY_240_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_115:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_NOISY_256_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_NOISY_256_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_116:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_NOISY_288_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_NOISY_288_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_117:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_NOISY_320_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_NOISY_320_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_118:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_NOISY_384_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_NOISY_384_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_119:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_NOISY_512_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_NOISY_512_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_120:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_NOISY_4096_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_NOISY_4096_Q15_ID,mgr);
  
    this->ifft=1;
    this->snr=LONG_SNR_THRESHOLD;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_121:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_STEP_4_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_STEP_4_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_122:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_STEP_6_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_STEP_6_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_123:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_STEP_8_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_STEP_8_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_124:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_STEP_10_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_STEP_10_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_125:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_STEP_12_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_STEP_12_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_126:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_STEP_16_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_STEP_16_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_127:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_STEP_18_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_STEP_18_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_128:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_STEP_20_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_STEP_20_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_129:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_STEP_24_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_STEP_24_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_130:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_STEP_30_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_STEP_30_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_131:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_STEP_32_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_STEP_32_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_132:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_STEP_36_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_STEP_36_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_133:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_STEP_40_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_STEP_40_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_134:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_STEP_48_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_STEP_48_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_135:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_STEP_50_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_STEP_50_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_136:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_STEP_54_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_STEP_54_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_137:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_STEP_60_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_STEP_60_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_138:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_STEP_64_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_STEP_64_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_139:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_STEP_72_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_STEP_72_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_140:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_STEP_80_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_STEP_80_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_141:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_STEP_90_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_STEP_90_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_142:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_STEP_96_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_STEP_96_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_143:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_STEP_100_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_STEP_100_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_144:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_STEP_108_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_STEP_108_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_145:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_STEP_120_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_STEP_120_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_146:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_STEP_128_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_STEP_128_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_147:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_STEP_144_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_STEP_144_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_148:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_STEP_150_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_STEP_150_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_149:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_STEP_160_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_STEP_160_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_150:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_STEP_180_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_STEP_180_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_151:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_STEP_192_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_STEP_192_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_152:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_STEP_200_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_STEP_200_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_153:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_STEP_216_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_STEP_216_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_154:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_STEP_240_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_STEP_240_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_155:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_STEP_256_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_STEP_256_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_156:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_STEP_288_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_STEP_288_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_157:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_STEP_320_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_STEP_320_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_158:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_STEP_384_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_STEP_384_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_159:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_STEP_512_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_STEP_512_Q15_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsQ15::TEST_RFFT_Q15_160:
  
    input.reload(RFFTTestsQ15::INPUTS_RIFFT_STEP_4096_Q15_ID,mgr);
    ref.reload(  RFFTTestsQ15::REF_RIFFT_STEP_4096_Q15_ID,mgr);
  
    this->ifft=1;
    this->snr=LONG_SNR_THRESHOLD;

  break;
  
  

       }

        if (this->ifft)
       {
         if (this->snr != LOW_SNR_THRESHOLD)
         {
           this->snr = IFFT_SNR_THRESHOLD;
         }
       }
       
        if (this->ifft==0)
        {
           outputfft.create(ref.nbSamples(),RFFTTestsQ15::OUTPUT_RFFT_Q15_ID,mgr);
           tmp.create(input.nbSamples(),RFFTTestsQ15::TMP_Q15_ID,mgr);

           tmpout.create(ref.nbSamples(),RFFTTestsQ15::TMPOUT_RFFT_Q15_ID,mgr);

        }
        else
        {
          outputfft.create(ref.nbSamples(),RFFTTestsQ15::OUTPUT_RFFT_Q15_ID,mgr);
          tmp.create(ref.nbSamples(),RFFTTestsQ15::TMP_Q15_ID,mgr);

          tmpout.create(ref.nbSamples(),RFFTTestsQ15::TMPOUT_RFFT_Q15_ID,mgr);

        }

    }

    void RFFTTestsQ15::tearDown(Testing::testID_t id,Client::PatternMgr *mgr)
    {
        (void)id;
        outputfft.dump(mgr);
    }
