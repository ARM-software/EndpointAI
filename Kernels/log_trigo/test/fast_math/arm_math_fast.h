/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        Math_sin.c
 * Description:  sinus
 *
 * $Date:        January 2025
 * $Revision:    V.0.0.1
 *
 * Target Processor: 
 * -------------------------------------------------------------------- */
/*
 * Copyright (C) 2010-2025 ARM Limited or its affiliates. All rights reserved.
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

#include <stdint.h>

#define MATLAB_SIMULATION 1
#define PI 3.141592653589793f


extern const float sin_l1[];
extern const float sin_pi_l1[];
extern const float atan_l1[];
extern const float tanh_l1[];
extern const float atanh_l1[];
extern const float exp_l1[];
extern const float log_l1[];

//-----------------------------
#if MATLAB_SIMULATION

#ifdef _MSC_VER 
#define _CRT_SECURE_NO_DEPRECATE
#endif

extern float PERIOD, PERIOD_COS;    
extern float PERIODdiv2Pi, PERIODdiv2, PERIOD0p75;    

extern uint32_t ATAN_L1;
extern float ATAN_L1div2;
extern float ATANH_L1div2;  
extern uint32_t TANH_L1;
extern float TANH_L1div2;   
extern uint32_t EXP_L1;
extern float EXP_L1div2;
extern uint32_t LOG_L1;
extern float LOG_L1float;


#else
//-----------------------------

#define PERIOD 100                      // sine cosine
#define PERIOD_COS 125
#define SIN_L1_SIZE 452
#define PERIOD_COS 125                  
#define PERIOD_COS 125

#define SINPI_L1_SIZE 252               // sine cosine on the reduced [-pi +pi] interval
#define PERIODdiv2Pi (float)(PERIOD/(2.0*PI))
#define PERIODdiv2 (float)(PERIOD/2.0f)
#define PERIOD0p75 (float)(PERIOD*0.75f)

#define ATAN_L1 100                     // arc tangent and atan2
#define ATAN_L1div2 50.0f
#define ATAN_L1_SIZE 402

#define ATANH_L1 100                    // arc tangent hyperbolic
#define ATANH_L1div2 50.0f
#define ATANH_L1_SIZE 202

#define TANH_L1 100                     // hyperbolic tangent
#define TANH_L1div2 50.0f
#define TANH_L1_SIZE 402

#define EXP_L1 100                      // exponential
#define EXP_L1div2 50.0f
#define EXP_L1_SIZE 402

#define LOG_L1 100                      // logarithm  
#define LOG_L1float 100.0f
#define LOG_L1_SIZE 402

#endif  // MATLAB_SIMULATION
//=======================================================================================

