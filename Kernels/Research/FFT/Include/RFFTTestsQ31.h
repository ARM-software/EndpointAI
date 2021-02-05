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
#include "Test.h"
#include "Pattern.h"

#include "fft.h"

class RFFTTestsQ31:public Client::Suite
    {
        public:
            RFFTTestsQ31(Testing::testID_t id);
            virtual void setUp(Testing::testID_t,std::vector<Testing::param_t>& paramsArgs,Client::PatternMgr *mgr);
            virtual void tearDown(Testing::testID_t,Client::PatternMgr *mgr);
        private:
            #include "RFFTTestsQ31_decl.h"
            
            Client::Pattern<q31_t> input;
            Client::LocalPattern<q31_t> outputfft;
            Client::LocalPattern<q31_t> debug;
            Client::LocalPattern<q31_t> tmp;

            Client::LocalPattern<q31_t> tmpin;
            Client::LocalPattern<q31_t> tmpout;

            Client::RefPattern<q31_t> ref;

            //arm_cfft_instance_q31 varInstCfftQ31;

            int ifft;
            int snr;

            arm_status status;
            
    };
