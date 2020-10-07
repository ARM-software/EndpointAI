/* ----------------------------------------------------------------------
 * Project:      KalmanApp
 * Title:        Kalman.hpp
 * Description:  Kalman loop
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
#ifndef KALMAN_H
#define KALMAN_H

#include "dsp/basic_math_functions.h"
#include "dsp/matrix_functions.h"


#define NBSTATE 4 // q0, q1, q2, q3
#define NBCONTROL 3 // wx, wy, wz
#define NBOBS 6 // ax, ay, az, mx, my, mz


/*

Init the CMSIS-DSP instance structures for each matrix

*/
extern void initMatrix();

/*

Init the noise covariance matrices.
(Values coming from sensor measurements and calibration)

*/
extern void initCovariances();

/*

Initialize the initial state of the filter.

*/
extern void initialState();

/*

Kalman filter

*/
extern void stateUpdate(int i,float dt);


/*

Get currently estimated orientation

*/
extern void getQuaternion(float *q0, float *q1, float *q2,float *q3);

#endif
