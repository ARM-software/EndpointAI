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

/*

Include template specializations for the supported scalar radix

*/

#ifndef RADIX_H
#define RADIX_H

#ifdef DATATYPE_F64
  #include "Scalar/F64/radix_F64_2.h"
  #include "Scalar/F64/radix_F64_3.h"
  #include "Scalar/F64/radix_F64_4.h"
  #include "Scalar/F64/radix_F64_5.h"
  #include "Scalar/F64/radix_F64_6.h"
  #include "Scalar/F64/radix_F64_8.h"
#endif

#ifdef DATATYPE_F32
  #include "Scalar/F32/radix_F32_2.h"
  #include "Scalar/F32/radix_F32_3.h"
  #include "Scalar/F32/radix_F32_4.h"
  #include "Scalar/F32/radix_F32_5.h"
  #include "Scalar/F32/radix_F32_6.h"
  #include "Scalar/F32/radix_F32_8.h"
#endif

#if defined(ARM_FLOAT16_SUPPORTED)
#ifdef DATATYPE_F16
  #include "Scalar/F16/radix_F16_2.h"
  #include "Scalar/F16/radix_F16_3.h"
  #include "Scalar/F16/radix_F16_4.h"
  #include "Scalar/F16/radix_F16_5.h"
  #include "Scalar/F16/radix_F16_6.h"
  #include "Scalar/F16/radix_F16_8.h"
#endif
#endif



#ifdef DATATYPE_Q31
  #include "Scalar/Q31/radix_Q31_2.h"
  #include "Scalar/Q31/radix_Q31_3.h"
  #include "Scalar/Q31/radix_Q31_4.h"
  #include "Scalar/Q31/radix_Q31_5.h"
  #include "Scalar/Q31/radix_Q31_6.h"
  #include "Scalar/Q31/radix_Q31_8.h"
#endif

#ifdef DATATYPE_Q15
  #ifdef DSPE_Q15
      #include "DSP/Q15/radix_Q15_2.h"
      #include "DSP/Q15/radix_Q15_3.h"
      #include "DSP/Q15/radix_Q15_4.h"
      #include "DSP/Q15/radix_Q15_5.h"
      #include "DSP/Q15/radix_Q15_6.h"
      #include "DSP/Q15/radix_Q15_8.h"
  #else
      #include "Scalar/Q15/radix_Q15_2.h"
      #include "Scalar/Q15/radix_Q15_3.h"
      #include "Scalar/Q15/radix_Q15_4.h"
      #include "Scalar/Q15/radix_Q15_5.h"
      #include "Scalar/Q15/radix_Q15_6.h"
      #include "Scalar/Q15/radix_Q15_8.h"
  #endif
#endif

#ifdef DATATYPE_Q7
  #include "Scalar/Q7/radix_Q7_2.h"
  #include "Scalar/Q7/radix_Q7_3.h"
  #include "Scalar/Q7/radix_Q7_4.h"
  #include "Scalar/Q7/radix_Q7_5.h"
  #include "Scalar/Q7/radix_Q7_6.h"
  #include "Scalar/Q7/radix_Q7_8.h"
#endif

#endif