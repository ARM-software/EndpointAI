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
#include "RFFTTestsF32.h"
#include <stdio.h>
#include "Error.h"
#include "Test.h"
//#include "FFTDebug.h"

#define SNR_THRESHOLD 120

    void RFFTTestsF32::test_rfft_f32()
    {
       float32_t *inp = input.ptr();
       arm_status status=ARM_MATH_SUCCESS;

       float32_t *outfftp = outputfft.ptr();
       float32_t *tmpptr = tmp.ptr();

       //printMath(inp,input.nbSamples());
       
       //printMath((complex<float32_t>*)inp,input.nbSamples()>>1);

       
       if (this->ifft)
       {
         status=rifft(inp,tmpptr,outfftp,outputfft.nbSamples());
       }
       else
       {
         

         //status=cfft(inp,outfftp,input.nbSamples()>>1);
         //printf("\nINNER FFT\n");
         //printMath((complex<float32_t>*)outfftp,input.nbSamples()>>1);


         status=rfft(inp,tmpptr,outfftp,input.nbSamples());
       }

       /*
       printf("\nRESULT\n");
       if (status == ARM_MATH_SUCCESS)
       {
          printMath((complex<float32_t>*)outfftp,outputfft.nbSamples()>>1);
       }
       */

       
       ASSERT_TRUE(status==ARM_MATH_SUCCESS);
       ASSERT_SNR(outputfft,ref,(float32_t)SNR_THRESHOLD);
       ASSERT_EMPTY_TAIL(outputfft);


        
    } 

    void RFFTTestsF32::setUp(Testing::testID_t id,std::vector<Testing::param_t>& paramsArgs,Client::PatternMgr *mgr)
    {

       (void)paramsArgs;
       switch(id)
       {
case RFFTTestsF32::TEST_RFFT_F32_1:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_NOISY_4_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_NOISY_4_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_2:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_NOISY_6_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_NOISY_6_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_3:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_NOISY_8_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_NOISY_8_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_4:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_NOISY_10_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_NOISY_10_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_5:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_NOISY_12_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_NOISY_12_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_6:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_NOISY_16_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_NOISY_16_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_7:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_NOISY_18_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_NOISY_18_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_8:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_NOISY_20_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_NOISY_20_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_9:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_NOISY_24_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_NOISY_24_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_10:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_NOISY_30_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_NOISY_30_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_11:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_NOISY_32_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_NOISY_32_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_12:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_NOISY_36_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_NOISY_36_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_13:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_NOISY_40_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_NOISY_40_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_14:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_NOISY_48_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_NOISY_48_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_15:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_NOISY_50_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_NOISY_50_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_16:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_NOISY_54_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_NOISY_54_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_17:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_NOISY_60_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_NOISY_60_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_18:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_NOISY_64_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_NOISY_64_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_19:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_NOISY_72_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_NOISY_72_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_20:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_NOISY_80_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_NOISY_80_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_21:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_NOISY_90_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_NOISY_90_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_22:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_NOISY_96_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_NOISY_96_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_23:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_NOISY_100_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_NOISY_100_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_24:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_NOISY_108_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_NOISY_108_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_25:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_NOISY_120_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_NOISY_120_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_26:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_NOISY_128_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_NOISY_128_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_27:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_NOISY_144_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_NOISY_144_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_28:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_NOISY_150_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_NOISY_150_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_29:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_NOISY_160_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_NOISY_160_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_30:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_NOISY_180_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_NOISY_180_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_31:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_NOISY_192_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_NOISY_192_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_32:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_NOISY_200_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_NOISY_200_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_33:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_NOISY_216_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_NOISY_216_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_34:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_NOISY_240_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_NOISY_240_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_35:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_NOISY_256_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_NOISY_256_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_36:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_NOISY_288_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_NOISY_288_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_37:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_NOISY_320_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_NOISY_320_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_38:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_NOISY_384_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_NOISY_384_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_39:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_NOISY_512_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_NOISY_512_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_40:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_NOISY_4096_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_NOISY_4096_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_41:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_STEP_4_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_STEP_4_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_42:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_STEP_6_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_STEP_6_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_43:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_STEP_8_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_STEP_8_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_44:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_STEP_10_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_STEP_10_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_45:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_STEP_12_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_STEP_12_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_46:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_STEP_16_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_STEP_16_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_47:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_STEP_18_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_STEP_18_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_48:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_STEP_20_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_STEP_20_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_49:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_STEP_24_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_STEP_24_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_50:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_STEP_30_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_STEP_30_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_51:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_STEP_32_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_STEP_32_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_52:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_STEP_36_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_STEP_36_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_53:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_STEP_40_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_STEP_40_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_54:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_STEP_48_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_STEP_48_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_55:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_STEP_50_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_STEP_50_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_56:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_STEP_54_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_STEP_54_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_57:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_STEP_60_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_STEP_60_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_58:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_STEP_64_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_STEP_64_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_59:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_STEP_72_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_STEP_72_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_60:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_STEP_80_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_STEP_80_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_61:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_STEP_90_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_STEP_90_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_62:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_STEP_96_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_STEP_96_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_63:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_STEP_100_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_STEP_100_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_64:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_STEP_108_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_STEP_108_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_65:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_STEP_120_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_STEP_120_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_66:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_STEP_128_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_STEP_128_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_67:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_STEP_144_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_STEP_144_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_68:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_STEP_150_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_STEP_150_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_69:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_STEP_160_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_STEP_160_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_70:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_STEP_180_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_STEP_180_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_71:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_STEP_192_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_STEP_192_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_72:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_STEP_200_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_STEP_200_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_73:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_STEP_216_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_STEP_216_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_74:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_STEP_240_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_STEP_240_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_75:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_STEP_256_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_STEP_256_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_76:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_STEP_288_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_STEP_288_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_77:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_STEP_320_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_STEP_320_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_78:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_STEP_384_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_STEP_384_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_79:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_STEP_512_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_STEP_512_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_80:
  
    input.reload(RFFTTestsF32::INPUTS_RFFT_STEP_4096_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RFFT_STEP_4096_F32_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_81:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_NOISY_4_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_NOISY_4_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_82:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_NOISY_6_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_NOISY_6_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_83:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_NOISY_8_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_NOISY_8_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_84:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_NOISY_10_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_NOISY_10_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_85:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_NOISY_12_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_NOISY_12_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_86:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_NOISY_16_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_NOISY_16_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_87:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_NOISY_18_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_NOISY_18_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_88:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_NOISY_20_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_NOISY_20_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_89:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_NOISY_24_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_NOISY_24_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_90:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_NOISY_30_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_NOISY_30_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_91:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_NOISY_32_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_NOISY_32_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_92:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_NOISY_36_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_NOISY_36_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_93:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_NOISY_40_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_NOISY_40_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_94:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_NOISY_48_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_NOISY_48_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_95:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_NOISY_50_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_NOISY_50_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_96:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_NOISY_54_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_NOISY_54_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_97:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_NOISY_60_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_NOISY_60_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_98:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_NOISY_64_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_NOISY_64_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_99:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_NOISY_72_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_NOISY_72_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_100:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_NOISY_80_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_NOISY_80_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_101:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_NOISY_90_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_NOISY_90_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_102:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_NOISY_96_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_NOISY_96_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_103:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_NOISY_100_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_NOISY_100_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_104:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_NOISY_108_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_NOISY_108_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_105:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_NOISY_120_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_NOISY_120_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_106:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_NOISY_128_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_NOISY_128_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_107:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_NOISY_144_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_NOISY_144_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_108:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_NOISY_150_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_NOISY_150_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_109:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_NOISY_160_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_NOISY_160_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_110:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_NOISY_180_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_NOISY_180_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_111:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_NOISY_192_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_NOISY_192_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_112:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_NOISY_200_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_NOISY_200_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_113:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_NOISY_216_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_NOISY_216_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_114:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_NOISY_240_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_NOISY_240_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_115:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_NOISY_256_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_NOISY_256_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_116:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_NOISY_288_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_NOISY_288_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_117:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_NOISY_320_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_NOISY_320_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_118:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_NOISY_384_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_NOISY_384_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_119:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_NOISY_512_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_NOISY_512_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_120:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_NOISY_4096_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_NOISY_4096_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_121:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_STEP_4_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_STEP_4_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_122:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_STEP_6_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_STEP_6_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_123:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_STEP_8_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_STEP_8_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_124:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_STEP_10_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_STEP_10_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_125:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_STEP_12_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_STEP_12_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_126:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_STEP_16_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_STEP_16_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_127:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_STEP_18_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_STEP_18_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_128:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_STEP_20_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_STEP_20_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_129:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_STEP_24_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_STEP_24_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_130:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_STEP_30_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_STEP_30_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_131:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_STEP_32_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_STEP_32_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_132:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_STEP_36_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_STEP_36_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_133:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_STEP_40_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_STEP_40_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_134:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_STEP_48_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_STEP_48_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_135:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_STEP_50_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_STEP_50_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_136:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_STEP_54_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_STEP_54_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_137:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_STEP_60_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_STEP_60_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_138:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_STEP_64_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_STEP_64_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_139:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_STEP_72_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_STEP_72_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_140:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_STEP_80_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_STEP_80_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_141:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_STEP_90_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_STEP_90_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_142:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_STEP_96_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_STEP_96_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_143:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_STEP_100_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_STEP_100_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_144:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_STEP_108_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_STEP_108_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_145:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_STEP_120_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_STEP_120_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_146:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_STEP_128_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_STEP_128_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_147:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_STEP_144_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_STEP_144_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_148:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_STEP_150_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_STEP_150_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_149:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_STEP_160_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_STEP_160_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_150:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_STEP_180_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_STEP_180_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_151:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_STEP_192_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_STEP_192_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_152:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_STEP_200_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_STEP_200_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_153:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_STEP_216_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_STEP_216_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_154:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_STEP_240_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_STEP_240_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_155:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_STEP_256_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_STEP_256_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_156:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_STEP_288_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_STEP_288_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_157:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_STEP_320_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_STEP_320_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_158:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_STEP_384_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_STEP_384_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_159:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_STEP_512_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_STEP_512_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF32::TEST_RFFT_F32_160:
  
    input.reload(RFFTTestsF32::INPUTS_RIFFT_STEP_4096_F32_ID,mgr);
    ref.reload(  RFFTTestsF32::REF_RIFFT_STEP_4096_F32_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  

       }
        if (this->ifft==0)
        {
           outputfft.create(ref.nbSamples(),RFFTTestsF32::OUTPUT_RFFT_F32_ID,mgr);
           tmp.create(input.nbSamples(),RFFTTestsF32::TMP_F32_ID,mgr);
        }
        else
        {
          outputfft.create(ref.nbSamples(),RFFTTestsF32::OUTPUT_RFFT_F32_ID,mgr);
          tmp.create(ref.nbSamples(),RFFTTestsF32::TMP_F32_ID,mgr);
        }

    }

    void RFFTTestsF32::tearDown(Testing::testID_t id,Client::PatternMgr *mgr)
    {
        (void)id;
        outputfft.dump(mgr);
    }
