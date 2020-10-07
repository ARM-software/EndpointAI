/* ----------------------------------------------------------------------
 * Project:      KalmanApp
 * Title:        main.cpp
 * Description:  Main loop
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

#include "Kalman.hpp"
#include "IO.hpp"
#include "data.h"
#include <stdio.h>


int main() 
{
float32_t q0,q1,q2,q3;

initMatrix();
initCovariances();
initialState();

int i=0;
/*

Read sensor measurements.

*/
for(int i=0; i < NBSAMPLES; i++)
{
  /*
    One iteration of the Kalman filter using sensor measurements for step i.
    We assume that the sensor values are not available at always the same time.
    So the time increment dt is changing between iterations.

  */
  stateUpdate(i,dt(i));

  /*

  Get computed orientation and dump it.

  */
  getQuaternion(&q0,&q1,&q2,&q3);
  printf("%f,%f,%f,%f\n",q0,q1,q2,q3);
}


}



