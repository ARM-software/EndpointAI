/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        Math functions.c
 * Description:  SIN, COS, TAN, ASIN, ACOS, ATAN, SQRT, INVSQRT, TANH, LOG2, POW2
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

#include "arm_math_fast.h"

//----------------------------------------------------------------
extern float sin_l1_c(float x);
extern float cos_l1_c(float x);
extern float sin_pi_c(float x);
extern float cos_pi_c(float x);
extern float atan_l1_c(float x);
extern float atan2_l1_c(float y, float x);
extern float tanh_l1_c(float x);
extern float atanh_l1_c(float x);
extern float exp_l1_c(float x);
extern float invsqrt_l1_c(float x);
extern float log_l1_c(float x);

uint32_t SIN_L1, ATAN_L1, TANH_L1, ATANH_L1;
float ATAN_L1div2, ATANH_L1div2, TANH_L1div2;
float PERIOD, PERIOD_COS, PERIODdiv2Pi, PERIODdiv2, PERIOD0p75;
uint32_t EXP_L1;
float EXP_L1div2, LOG_L1float;
uint32_t LOG_L1;

#define NMAX_COEFS 10000
const float sin_l1[NMAX_COEFS];
const float sin_pi_l1[NMAX_COEFS];
const float tan_l1[NMAX_COEFS];
const float atan_l1[NMAX_COEFS];
const float tanh_l1[NMAX_COEFS];
const float atanh_l1[NMAX_COEFS];
const float exp_l1[NMAX_COEFS];
const float log_l1[NMAX_COEFS];


/*-------------------------------------------------------------------------*/
void main (int argc, char *argv[])
{
    FILE* ptf_in, * ptf_out, * ptf_coef;
    float x, y, datain, dataout, C1, C2;
    int i, j;

    if (argc < 2) {
        printf("format ""fast_mat sin   \n");
        exit (-1);
    }

    if (argc < 6) { printf("format ""fast_mat sin <float input> <float output> <float coef> \n"); exit(-1); }
    if (NULL == (ptf_out  = fopen(argv[3], "wt"))) exit(0);
    if (NULL == (ptf_in   = fopen(argv[2], "rt"))) exit(0);
    if (NULL == (ptf_coef = fopen(argv[4], "rt"))) exit(0);   // 
    C1 = (float)atof(argv[5]); 
    C2 = (float)atof(argv[6]);

/*=========================================================================================================*/
if (0 == strcmp("sin", argv[1]) || 0 == strcmp("cos", argv[1]))
{
    PERIOD = C1; PERIOD_COS = 1.25f * PERIOD;
    SIN_L1 = (uint32_t) C2;

    j = 0; do{ i = fscanf(ptf_coef, "%ff,\n", &(sin_l1[j++])); } while (i>0);

    do
    {   i = fscanf(ptf_in, "%f\n", &datain); if (i < 1) break;
        if (0 == strcmp("sin", argv[1]))    dataout = sin_l1_c(datain);
        else                                dataout = cos_l1_c(datain);
	    fprintf (ptf_out, "%12.9f\n", dataout);
    } while (1);
}
/*=========================================================================================================*/
if (0 == strcmp("sinpi", argv[1]) || 0 == strcmp("cospi", argv[1]))
{
    PERIOD = C1; PERIOD0p75 = PERIOD * 0.75f; PERIODdiv2Pi = PERIOD / (2.0f * PI); PERIODdiv2 = PERIOD / 2.0f;
    SIN_L1 = (uint32_t)C2;

    j = 0; do { i = fscanf(ptf_coef, "%ff,\n", &(sin_pi_l1[j++])); } while (i > 0);

    do
    {   i = fscanf(ptf_in, "%f\n", &datain); if (i < 1) break;
        if (0 == strcmp("sinpi", argv[1]))  dataout = sin_pi_c(datain);
        else                                dataout = cos_pi_c(datain);
        fprintf(ptf_out, "%12.9f\n", dataout);
    } while (1);
}
/*=========================================================================================================*/
if (0 == strcmp("atan", argv[1]) || 0 == strcmp("atan2", argv[1]))
{
    ATAN_L1 = (uint32_t)C1; ATAN_L1div2 = ATAN_L1 / 2.0f;

    j = 0; do { i = fscanf(ptf_coef, "%ff,\n", &(atan_l1[j++])); } while (i > 0);

    do
    {   if (0 == strcmp("atan", argv[1])) {
            i = fscanf(ptf_in, "%f\n", &x); if (i < 1) break;
            dataout = atan_l1_c(x);
        }
        else {
            i = fscanf(ptf_in, "%f %f\n", &y, &x); if (i < 1) break;
            dataout = atan2_l1_c(y,x);
        }
        fprintf(ptf_out, "%12.9f\n", dataout);
    } while (1);
}
/*=========================================================================================================*/
if (0 == strcmp("tanh", argv[1]) )
{
    TANH_L1 = (uint32_t)C1; TANH_L1div2 = TANH_L1 / 2.0f;

    j = 0; do { i = fscanf(ptf_coef, "%ff,\n", &(tanh_l1[j++])); } while (i > 0);
    do
    {   i = fscanf(ptf_in, "%f\n", &x); if (i < 1) break;
        dataout = tanh_l1_c(x);
        fprintf(ptf_out, "%12.9f\n", dataout);
    } while (1);
}
/*=========================================================================================================*/
if (0 == strcmp("atanh", argv[1]))
{
    ATANH_L1 = (uint32_t)C1; ATANH_L1div2 = ATANH_L1 / 2.0f;

    j = 0; do { i = fscanf(ptf_coef, "%ff,\n", &(atanh_l1[j++])); } while (i > 0);
    do
    {
        i = fscanf(ptf_in, "%f\n", &x); if (i < 1) break;
        dataout = atanh_l1_c(x);
        fprintf(ptf_out, "%12.9f\n", dataout);
    } while (1);
}
/*=========================================================================================================*/
if (0 == strcmp("invsqrt", argv[1]))
{
    do
    {
        i = fscanf(ptf_in, "%f\n", &x); if (i < 1) break;
        dataout = invsqrt_l1_c(x);
        fprintf(ptf_out, "%12.9f\n", dataout);
    } while (1);
}
/*=========================================================================================================*/
if (0 == strcmp("exp", argv[1]))
{
    EXP_L1 = (uint32_t)C1; EXP_L1div2 = EXP_L1 / 2.0f;

    j = 0; do { i = fscanf(ptf_coef, "%ff,\n", &(exp_l1[j++])); } while (i > 0);

    do
    {
        i = fscanf(ptf_in, "%f\n", &x); if (i < 1) break;
        dataout = exp_l1_c(x);
        fprintf(ptf_out, "%12.9f\n", dataout);
    } while (1);
}
/*=========================================================================================================*/
if (0 == strcmp("log", argv[1]))
{
    LOG_L1 = (uint32_t)C1; LOG_L1float = (float)LOG_L1;

    j = 0; do { i = fscanf(ptf_coef, "%ff,\n", &(log_l1[j++])); } while (i > 0);

    do
    {
        i = fscanf(ptf_in, "%f\n", &x); if (i < 1) break;
        dataout = log_l1_c(x);
        fprintf(ptf_out, "%12.9f\n", dataout);
    } while (1);
}
/*=========================================================================================================*/
    exit(-3); 
}