/* ----------------------------------------------------------------------
 * Project:      KalmanApp
 * Title:        IO.hpp
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
#ifndef _IO_H_
#define _IO_H_

extern void getAccelerometerData(int i,float *ax, float *ay, float *az);
extern void getMagnetometerData(int i,float *mx, float *my, float *mz);
extern void getGyroData(int i,float *wx, float *wy, float *wz);
extern float dt(int i);




#endif
