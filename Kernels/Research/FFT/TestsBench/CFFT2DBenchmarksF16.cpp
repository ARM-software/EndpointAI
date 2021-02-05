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
#include "CFFT2DBenchmarksF16.h"
#include "Error.h"



#include "fft.h"


void CFFT2DBenchmarksF16::test_cfft2d_radix2_f16()
{     

  cfft2D(in,out,this->rows,this->cols);  
} 


void CFFT2DBenchmarksF16::test_cifft2d_radix2_f16()
{     

  cifft2D(in,out,this->rows,this->cols);  
} 


 
    void CFFT2DBenchmarksF16::setUp(Testing::testID_t id,std::vector<Testing::param_t>& params,Client::PatternMgr *mgr)
    {

       (void)id;
       std::vector<Testing::param_t>::iterator it = params.begin();
       this->rows = *it++;
       this->cols = *it++;
       
       switch(id)
       {
        case TEST_CFFT2D_RADIX2_F16_1:

           input1.reload(CFFT2DBenchmarksF16::INPUTC_F16_ID,mgr);
           output.create(2*this->rows*this->cols,CFFT2DBenchmarksF16::OUT_F16_ID,mgr);

           in = input1.ptr();
           out = output.ptr();



          
        break;

        case TEST_CIFFT2D_RADIX2_F16_2:

           input1.reload(CFFT2DBenchmarksF16::INPUTC_IFFT2D_F16_ID,mgr);
           output.create(2*this->rows*this->cols,CFFT2DBenchmarksF16::OUT_F16_ID,mgr);

           in = input1.ptr();
           out = output.ptr();



          
        break;

       
       }

     


    }

    void CFFT2DBenchmarksF16::tearDown(Testing::testID_t id,Client::PatternMgr *mgr)
    {
      (void)id;
      (void)mgr;
    }
