/* ----------------------------------------------------------------------
 * Project:      KalmanApp
 * Title:        IO.cpp
 * Description:  Sensor Inputs
 *
 * -------------------------------------------------------------------- */
/*
 * Copyright (C) 2010-2020 ARM Limited or its affiliates. All rights reserved.
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
#include "IO.hpp"
#include "arm_math_types.h"
#include "data.h"

void getAccelerometerData(int i,float *ax, float *ay, float *az)
{
   *ax = inputData[BLOCKSIZE*i + 3];
   *ay = inputData[BLOCKSIZE*i + 4];
   *az = inputData[BLOCKSIZE*i + 5];
}


void getMagnetometerData(int i, float *mx, float *my, float *mz)
{
  
  *mx = inputData[BLOCKSIZE*i + 6];
  *my = inputData[BLOCKSIZE*i + 7];
  *mz = inputData[BLOCKSIZE*i + 8];


}

void getGyroData(int i,float *wx, float *wy, float *wz)
{
  *wx = inputData[BLOCKSIZE*i + 0];
  *wy = inputData[BLOCKSIZE*i + 1];
  *wz = inputData[BLOCKSIZE*i + 2];

}

float32_t dt(int i)
{
    return(inputData[BLOCKSIZE*i + 9]);
}

