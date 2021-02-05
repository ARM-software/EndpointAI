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
#include "RFFTTestsF16.h"
#include <stdio.h>
#include "Error.h"
#include "Test.h"
//#include "FFTDebug.h"

#define SNR_THRESHOLD 58

    void RFFTTestsF16::test_rfft_f16()
    {
       float16_t *inp = input.ptr();
       arm_status status=ARM_MATH_SUCCESS;

       float16_t *outfftp = outputfft.ptr();
       float16_t *tmpptr = tmp.ptr();

       if (this->ifft)
       {
         status=rifft(inp,tmpptr,outfftp,outputfft.nbSamples());
       }
       else
       {
         

         //status=cfft(inp,outfftp,input.nbSamples()>>1);
         //printf("\nINNER FFT\n");
         //printMath((complex<float16_t>*)outfftp,input.nbSamples()>>1);


         status=rfft(inp,tmpptr,outfftp,input.nbSamples());
       }

       //printf("\nRESULT\n");
       //if (status == ARM_MATH_SUCCESS)
       //{
       //   printMath((complex<float16_t>*)outfftp,outputfft.nbSamples()>>1);
       //}


        
    } 

    void RFFTTestsF16::setUp(Testing::testID_t id,std::vector<Testing::param_t>& paramsArgs,Client::PatternMgr *mgr)
    {

       (void)paramsArgs;
       switch(id)
       {
case RFFTTestsF16::TEST_RFFT_F16_1:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_NOISY_4_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_NOISY_4_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_2:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_NOISY_6_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_NOISY_6_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_3:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_NOISY_8_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_NOISY_8_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_4:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_NOISY_10_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_NOISY_10_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_5:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_NOISY_12_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_NOISY_12_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_6:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_NOISY_16_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_NOISY_16_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_7:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_NOISY_18_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_NOISY_18_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_8:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_NOISY_20_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_NOISY_20_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_9:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_NOISY_24_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_NOISY_24_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_10:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_NOISY_30_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_NOISY_30_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_11:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_NOISY_32_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_NOISY_32_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_12:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_NOISY_36_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_NOISY_36_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_13:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_NOISY_40_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_NOISY_40_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_14:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_NOISY_48_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_NOISY_48_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_15:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_NOISY_50_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_NOISY_50_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_16:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_NOISY_54_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_NOISY_54_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_17:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_NOISY_60_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_NOISY_60_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_18:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_NOISY_64_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_NOISY_64_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_19:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_NOISY_72_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_NOISY_72_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_20:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_NOISY_80_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_NOISY_80_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_21:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_NOISY_90_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_NOISY_90_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_22:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_NOISY_96_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_NOISY_96_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_23:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_NOISY_100_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_NOISY_100_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_24:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_NOISY_108_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_NOISY_108_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_25:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_NOISY_120_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_NOISY_120_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_26:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_NOISY_128_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_NOISY_128_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_27:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_NOISY_144_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_NOISY_144_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_28:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_NOISY_150_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_NOISY_150_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_29:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_NOISY_160_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_NOISY_160_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_30:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_NOISY_180_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_NOISY_180_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_31:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_NOISY_192_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_NOISY_192_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_32:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_NOISY_200_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_NOISY_200_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_33:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_NOISY_216_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_NOISY_216_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_34:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_NOISY_240_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_NOISY_240_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_35:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_NOISY_256_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_NOISY_256_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_36:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_NOISY_288_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_NOISY_288_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_37:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_NOISY_320_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_NOISY_320_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_38:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_NOISY_384_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_NOISY_384_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_39:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_NOISY_512_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_NOISY_512_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_40:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_NOISY_4096_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_NOISY_4096_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_41:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_STEP_4_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_STEP_4_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_42:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_STEP_6_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_STEP_6_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_43:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_STEP_8_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_STEP_8_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_44:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_STEP_10_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_STEP_10_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_45:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_STEP_12_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_STEP_12_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_46:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_STEP_16_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_STEP_16_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_47:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_STEP_18_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_STEP_18_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_48:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_STEP_20_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_STEP_20_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_49:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_STEP_24_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_STEP_24_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_50:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_STEP_30_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_STEP_30_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_51:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_STEP_32_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_STEP_32_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_52:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_STEP_36_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_STEP_36_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_53:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_STEP_40_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_STEP_40_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_54:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_STEP_48_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_STEP_48_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_55:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_STEP_50_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_STEP_50_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_56:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_STEP_54_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_STEP_54_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_57:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_STEP_60_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_STEP_60_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_58:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_STEP_64_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_STEP_64_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_59:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_STEP_72_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_STEP_72_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_60:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_STEP_80_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_STEP_80_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_61:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_STEP_90_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_STEP_90_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_62:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_STEP_96_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_STEP_96_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_63:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_STEP_100_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_STEP_100_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_64:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_STEP_108_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_STEP_108_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_65:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_STEP_120_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_STEP_120_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_66:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_STEP_128_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_STEP_128_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_67:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_STEP_144_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_STEP_144_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_68:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_STEP_150_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_STEP_150_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_69:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_STEP_160_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_STEP_160_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_70:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_STEP_180_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_STEP_180_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_71:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_STEP_192_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_STEP_192_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_72:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_STEP_200_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_STEP_200_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_73:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_STEP_216_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_STEP_216_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_74:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_STEP_240_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_STEP_240_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_75:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_STEP_256_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_STEP_256_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_76:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_STEP_288_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_STEP_288_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_77:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_STEP_320_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_STEP_320_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_78:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_STEP_384_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_STEP_384_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_79:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_STEP_512_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_STEP_512_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_80:
  
    input.reload(RFFTTestsF16::INPUTS_RFFT_STEP_4096_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RFFT_STEP_4096_F16_ID,mgr);
  
    this->ifft=0;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_81:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_NOISY_4_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_NOISY_4_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_82:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_NOISY_6_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_NOISY_6_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_83:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_NOISY_8_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_NOISY_8_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_84:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_NOISY_10_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_NOISY_10_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_85:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_NOISY_12_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_NOISY_12_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_86:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_NOISY_16_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_NOISY_16_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_87:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_NOISY_18_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_NOISY_18_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_88:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_NOISY_20_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_NOISY_20_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_89:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_NOISY_24_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_NOISY_24_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_90:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_NOISY_30_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_NOISY_30_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_91:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_NOISY_32_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_NOISY_32_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_92:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_NOISY_36_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_NOISY_36_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_93:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_NOISY_40_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_NOISY_40_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_94:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_NOISY_48_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_NOISY_48_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_95:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_NOISY_50_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_NOISY_50_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_96:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_NOISY_54_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_NOISY_54_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_97:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_NOISY_60_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_NOISY_60_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_98:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_NOISY_64_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_NOISY_64_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_99:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_NOISY_72_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_NOISY_72_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_100:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_NOISY_80_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_NOISY_80_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_101:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_NOISY_90_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_NOISY_90_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_102:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_NOISY_96_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_NOISY_96_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_103:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_NOISY_100_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_NOISY_100_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_104:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_NOISY_108_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_NOISY_108_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_105:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_NOISY_120_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_NOISY_120_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_106:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_NOISY_128_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_NOISY_128_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_107:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_NOISY_144_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_NOISY_144_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_108:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_NOISY_150_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_NOISY_150_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_109:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_NOISY_160_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_NOISY_160_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_110:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_NOISY_180_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_NOISY_180_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_111:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_NOISY_192_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_NOISY_192_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_112:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_NOISY_200_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_NOISY_200_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_113:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_NOISY_216_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_NOISY_216_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_114:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_NOISY_240_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_NOISY_240_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_115:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_NOISY_256_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_NOISY_256_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_116:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_NOISY_288_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_NOISY_288_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_117:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_NOISY_320_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_NOISY_320_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_118:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_NOISY_384_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_NOISY_384_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_119:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_NOISY_512_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_NOISY_512_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_120:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_NOISY_4096_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_NOISY_4096_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_121:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_STEP_4_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_STEP_4_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_122:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_STEP_6_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_STEP_6_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_123:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_STEP_8_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_STEP_8_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_124:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_STEP_10_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_STEP_10_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_125:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_STEP_12_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_STEP_12_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_126:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_STEP_16_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_STEP_16_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_127:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_STEP_18_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_STEP_18_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_128:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_STEP_20_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_STEP_20_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_129:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_STEP_24_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_STEP_24_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_130:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_STEP_30_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_STEP_30_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_131:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_STEP_32_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_STEP_32_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_132:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_STEP_36_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_STEP_36_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_133:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_STEP_40_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_STEP_40_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_134:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_STEP_48_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_STEP_48_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_135:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_STEP_50_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_STEP_50_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_136:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_STEP_54_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_STEP_54_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_137:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_STEP_60_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_STEP_60_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_138:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_STEP_64_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_STEP_64_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_139:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_STEP_72_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_STEP_72_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_140:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_STEP_80_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_STEP_80_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_141:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_STEP_90_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_STEP_90_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_142:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_STEP_96_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_STEP_96_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_143:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_STEP_100_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_STEP_100_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_144:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_STEP_108_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_STEP_108_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_145:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_STEP_120_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_STEP_120_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_146:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_STEP_128_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_STEP_128_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_147:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_STEP_144_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_STEP_144_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_148:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_STEP_150_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_STEP_150_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_149:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_STEP_160_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_STEP_160_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_150:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_STEP_180_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_STEP_180_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_151:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_STEP_192_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_STEP_192_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_152:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_STEP_200_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_STEP_200_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_153:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_STEP_216_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_STEP_216_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_154:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_STEP_240_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_STEP_240_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_155:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_STEP_256_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_STEP_256_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_156:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_STEP_288_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_STEP_288_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_157:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_STEP_320_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_STEP_320_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_158:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_STEP_384_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_STEP_384_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_159:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_STEP_512_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_STEP_512_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  case RFFTTestsF16::TEST_RFFT_F16_160:
  
    input.reload(RFFTTestsF16::INPUTS_RIFFT_STEP_4096_F16_ID,mgr);
    ref.reload(  RFFTTestsF16::REF_RIFFT_STEP_4096_F16_ID,mgr);
  
    this->ifft=1;
  
  break;
  
  

       }
       if (this->ifft==0)
        {
           outputfft.create(ref.nbSamples(),RFFTTestsF16::OUTPUT_RFFT_F16_ID,mgr);
           tmp.create(input.nbSamples(),RFFTTestsF16::TMP_F16_ID,mgr);
        }
        else
        {
          outputfft.create(ref.nbSamples(),RFFTTestsF16::OUTPUT_RFFT_F16_ID,mgr);
          tmp.create(ref.nbSamples(),RFFTTestsF16::TMP_F16_ID,mgr);
        }

    }

    void RFFTTestsF16::tearDown(Testing::testID_t id,Client::PatternMgr *mgr)
    {
        (void)id;
        outputfft.dump(mgr);
    }
