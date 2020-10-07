/* ----------------------------------------------------------------------
 * Project:      KalmanApp
 * Title:        Kalman.cpp
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
#include "Kalman.hpp"
#include "IO.hpp"
#include "macros.h"


static float32_t oldstate[NBSTATE]={0},newstate[NBSTATE]={0};
static float32_t obs[NBOBS]={0};
static float32_t predictedobs[NBOBS]={0};

static float32_t y[NBSTATE]={0};
static float32_t FData[NBSTATE*NBSTATE]={0};

static float32_t PData[NBSTATE*NBSTATE]={0};
static float32_t NPData[NBSTATE*NBSTATE]={0};
static float32_t TempData1_NS_NS[NBSTATE*NBSTATE]={0};
static float32_t TempData2_NS_NS[NBSTATE*NBSTATE]={0};
static float32_t TempData1_NS_NC[NBSTATE*NBCONTROL]={0};
static float32_t TempData2_NC_NS[NBCONTROL*NBSTATE]={0};
static float32_t TempData3_NS_NS[NBSTATE*NBSTATE]={0};
static float32_t IdentityData_NS_NS[NBSTATE*NBSTATE]={0};

static float32_t QData[NBSTATE*NBSTATE]={0};
static float32_t HData[NBOBS*NBSTATE]={0};
static float32_t RData[NBOBS*NBOBS]={0};
static float32_t SData[NBOBS*NBOBS]={0};
static float64_t SData_f64[NBOBS*NBOBS]={0};
static float32_t TempData4_NO_NS[NBOBS*NBSTATE]={0};
static float32_t TempData5_NS_NO[NBOBS*NBSTATE]={0};
static float32_t TempData6_NO_NO[NBOBS*NBOBS]={0};
static float32_t TempData7_NS_NO[NBOBS*NBSTATE]={0};
static float32_t TempData8_NO_NO[NBOBS*NBOBS]={0};
static float64_t TempData8_NO_NO_f64[NBOBS*NBOBS]={0};
static float32_t KData[NBSTATE*NBOBS]={0};
static float32_t FQData[NBSTATE*NBCONTROL]={0};

static arm_matrix_instance_f32 P,NP,Temp1_NS_NS,Temp2_NS_NS,Temp3_NS_NS,Identity_NS_NS;
static arm_matrix_instance_f32 F,H,Temp4_NO_NS,Temp5_NS_NO,Temp6_NO_NO,Temp7_NS_NO,Temp8_NO_NO,K;
static arm_matrix_instance_f32 Q,FQ,Temp1_NS_NC,Temp2_NC_NS;
static arm_matrix_instance_f32 R,S;
static arm_matrix_instance_f64 S_f64,Temp8_NO_NO_f64;
static arm_matrix_instance_f32 OmegaErrors;

static float simuWX,simuWY,simuWZ,simuAX,simuAY,simuAZ,simuMX,simuMY,simuMZ;

/*
 * 
 * Matrix of covariance for rotation speed.
 * It is multiplied later by a Jacobian derived from f function.
 * 
 */
static float32_t omegaErrorData[NBCONTROL*NBCONTROL]={
  3.584790670015625e-6f,0.0f,0.0f,
  0.0f,2.694048756112697e-6,0.0f,
  0.0f,0.0f,2.2012498557809965e-6};

  static float32_t omegaCurrentErrorData[NBCONTROL*NBCONTROL]={0.0f};

/*

mag covariance resulting from calibration of the sensor.
The magnetometer without calibration is giving samples on an ellipse.
The ellipse is transformed into a sphere. It explain why this matrix
is not diagonal. The transform has introduced some correlations.

*/
  static float32_t magCovariance[9]={0.00007552285540319504,5.736934183525208e-6,-3.0309694223071724e-6,5.736934183525208e-6,0.00007282836085134982,-4.395545365959635e-6,-3.030969422307172e-6,-4.395545365959635e-6,0.0002450072771936285};

#define M(A,r,c) (A.pData[r * A.numCols + c])

// Some macros to make the code a bit more readable.
// With the code generatiom from sympy those macros could be removed.
// But since we do not have additional state variable and want to make it
// clear the relationship with the quaternion, the naming Qx and NQx has
// been kept also in the sympy script 
#define Q0 oldstate[0]
#define Q1 oldstate[1]
#define Q2 oldstate[2]
#define Q3 oldstate[3]


#define NQ0 newstate[0]
#define NQ1 newstate[1]
#define NQ2 newstate[2]
#define NQ3 newstate[3]


// Predicted observations
#define PAX predictedobs[0]
#define PAY predictedobs[1]
#define PAZ predictedobs[2]
#define PMX predictedobs[3]
#define PMY predictedobs[4]
#define PMZ predictedobs[5]

#define g 9.81f

void normalizeQuat(float *q0, float *q1, float* q2, float *q3)
{

  /*

  This function could be improved and some checks added like testing if the norm is too small.

  */

  float qNorm;  

  qNorm = sqrtf(*q0 * *q0 + *q1 * *q1 + *q2 * *q2 + *q3 * *q3);
    
  qNorm = 1.0F / qNorm;
    
  *q0 *= qNorm;
  *q1 *= qNorm;
  *q2 *= qNorm;
  *q3 *= qNorm;

  

}

void initMatrix()
{
 int i;
 P.numRows = NBSTATE;
 P.numCols = NBSTATE; 
 P.pData = PData;

 Identity_NS_NS.numRows = NBSTATE;
 Identity_NS_NS.numCols = NBSTATE; 
 Identity_NS_NS.pData = IdentityData_NS_NS;
 for(i=0;i<NBSTATE;i++)
 {
   M(Identity_NS_NS,i,i) = 1.0f;
 }

 Temp1_NS_NS.numRows = NBSTATE;
 Temp1_NS_NS.numCols = NBSTATE; 
 Temp1_NS_NS.pData = TempData1_NS_NS;

 Temp2_NS_NS.numRows = NBSTATE;
 Temp2_NS_NS.numCols = NBSTATE; 
 Temp2_NS_NS.pData = TempData2_NS_NS;

 Temp1_NS_NC.numRows = NBSTATE;
 Temp1_NS_NC.numCols = NBCONTROL; 
 Temp1_NS_NC.pData = TempData1_NS_NC;

 Temp2_NC_NS.numRows = NBCONTROL;
 Temp2_NC_NS.numCols = NBSTATE; 
 Temp2_NC_NS.pData = TempData2_NC_NS;

 Temp3_NS_NS.numRows = NBSTATE;
 Temp3_NS_NS.numCols = NBSTATE; 
 Temp3_NS_NS.pData = TempData3_NS_NS;

 Temp4_NO_NS.numRows = NBOBS;
 Temp4_NO_NS.numCols = NBSTATE; 
 Temp4_NO_NS.pData = TempData4_NO_NS;

 Temp5_NS_NO.numRows = NBSTATE;
 Temp5_NS_NO.numCols = NBOBS; 
 Temp5_NS_NO.pData = TempData5_NS_NO;

 Temp6_NO_NO.numRows = NBOBS;
 Temp6_NO_NO.numCols = NBOBS; 
 Temp6_NO_NO.pData = TempData6_NO_NO;

 Temp7_NS_NO.numRows = NBSTATE;
 Temp7_NS_NO.numCols = NBOBS; 
 Temp7_NS_NO.pData = TempData7_NS_NO;

 Temp8_NO_NO.numRows = NBOBS;
 Temp8_NO_NO.numCols = NBOBS; 
 Temp8_NO_NO.pData = TempData8_NO_NO;

 Temp8_NO_NO_f64.numRows = NBOBS;
 Temp8_NO_NO_f64.numCols = NBOBS; 
 Temp8_NO_NO_f64.pData = TempData8_NO_NO_f64;

 NP.numRows = NBSTATE;
 NP.numCols = NBSTATE; 
 NP.pData = NPData;

 F.numRows = NBSTATE;
 F.numCols = NBSTATE; 
 F.pData = FData;

 FQ.numRows = NBSTATE;
 FQ.numCols = NBCONTROL; 
 FQ.pData = FQData;

 H.numRows = NBOBS;
 H.numCols = NBSTATE; 
 H.pData = HData;


 K.numRows = NBSTATE;
 K.numCols = NBOBS; 
 K.pData = KData;

 Q.numRows = NBSTATE;
 Q.numCols = NBSTATE; 
 Q.pData = QData;

 R.numRows = NBOBS;
 R.numCols = NBOBS; 
 R.pData = RData;

 S.numRows = NBOBS;
 S.numCols = NBOBS; 
 S.pData = SData;

 S_f64.numRows = NBOBS;
 S_f64.numCols = NBOBS; 
 S_f64.pData = SData_f64;


 OmegaErrors.numRows = NBCONTROL;
 OmegaErrors.numCols = NBCONTROL;
 OmegaErrors.pData = omegaCurrentErrorData;


}

void initCovariances()
{
  // Measured variance
  M(R,0,0) = 0.0000377806f ;
  M(R,1,1) = 0.0000413348f;
  M(R,2,2) = 0.0000434697f ;
  
  M(R,3,3) = magCovariance[0];
  M(R,3,4) = magCovariance[1];
  M(R,3,5) = magCovariance[2];

  M(R,4,3) = magCovariance[3];
  M(R,4,4) = magCovariance[4];
  M(R,4,5) = magCovariance[5];

  M(R,5,3) = magCovariance[6];
  M(R,5,4) = magCovariance[7];
  M(R,5,5) = magCovariance[8];

  for(int i=0;i< NBCONTROL*NBCONTROL;i++)
  {
    OmegaErrors.pData[i] = omegaErrorData[i];
  }

  
  // Process variances
  // Computed dynamically from omega errors and FQ matrix.
  
  
}



/*

Covariance update functions to update the covariance
before each filter step.

*/
void refreshCovarianceOmega(float omegaFactor)
{
  for(int i=0;i< NBCONTROL*NBCONTROL;i++)
  {
    OmegaErrors.pData[i] = omegaErrorData[i]*omegaFactor;
  }
}



void refreshAccCovariance(float accFactor)
{
  M(R,0,0) = 0.0000377806f * accFactor;
  M(R,1,1) = 0.0000413348f * accFactor;
  M(R,2,2) = 0.0000434697f * accFactor;
  
}

void refreshMagCovariance(float magFactor)
{

  M(R,3,3) = magCovariance[0] * magFactor;
  M(R,3,4) = magCovariance[1] * magFactor;
  M(R,3,5) = magCovariance[2] * magFactor;

  M(R,4,3) = magCovariance[3] * magFactor;
  M(R,4,4) = magCovariance[4] * magFactor;
  M(R,4,5) = magCovariance[5] * magFactor;

  M(R,5,3) = magCovariance[6] * magFactor;
  M(R,5,4) = magCovariance[7] * magFactor;
  M(R,5,5) = magCovariance[8] * magFactor;
}

void symmetrize(arm_matrix_instance_f32 *f)
{
  int row,col;
  if (f->numRows == f->numCols)
  {
    for(row=0;row < f->numRows; row ++)
    {
      for(col=row+1;col<f->numCols; col ++)
      {
        f->pData[col*f->numCols + row] = f->pData[row*f->numCols + col];
      }
    }
  }
}

void initialState()
{


  Q0 = 0.584261f;
  Q1 = 0.409104f;
  Q2 = 0.402025f;
  Q3 = 0.574151f;
  

  // Variance for 3 sigma equal to 1. Full range of
  // variation of quaternion component
  M(P,0,0)=0.111111f;
  M(P,1,1)=0.111111f;
  M(P,2,2)=0.111111f;
  M(P,3,3)=0.111111f;

  
}


void stateUpdate(int i,float dt)
{
  float32_t WX,WY,WZ;

  initCovariances();
  
  // Control
  getGyroData(i,&WX,&WY,&WZ);
  
  // Observations
  getAccelerometerData(i,&obs[0],&obs[1],&obs[2]);
  getMagnetometerData(i,&obs[3],&obs[4],&obs[5]);

  
  // One may want to play with the covariance.
  // For instance : some sensor values may not be always available so we 
  // may want to decrease their importance in the filter and reuse and old value.
  // Or the properties of the sensor may change.
  // Those 2 functions are just given as example : to tune the noise convariance at each
  // estimation step.
  //refreshAccCovariance(100.0f);
  refreshMagCovariance(0.1f);
  
  
  simuWX = WX;
  simuWY = WY;
  simuWZ = WZ;

  simuAX = obs[0];
  simuAY = obs[1];
  simuAZ = obs[2];

  simuMX = obs[3];
  simuMY = obs[4];
  simuMZ = obs[5];

  
  /* 
   *  Prediction step
   */


  // Update state

  f_update(); // Equation 2.0.1

  /* 

    Normalizing the quaternion may not be needed at each update.
    Since it is using a sqrtf and it is costly, one may want to experiment
    which doing this normalization less often.

  */
  normalizeQuat(&NQ0,&NQ1,&NQ2,&NQ3);


   /*
    * Update F matrix
    */

  // Equation 2.0.4
  F_update(); // F_k matrix in equations


  // Equation 2.0.5
  FQ_update(); // G_k


  
  // Update covariance
  // Equation 2.0.2
  arm_mat_mult_f32(&F,&P,&Temp1_NS_NS);
  arm_mat_trans_f32(&F,&Temp2_NS_NS);
  arm_mat_mult_f32(&Temp1_NS_NS,&Temp2_NS_NS,&Temp3_NS_NS);
  
  arm_mat_mult_f32(&FQ,&OmegaErrors,&Temp1_NS_NC);
  arm_mat_trans_f32(&FQ,&Temp2_NC_NS);
  arm_mat_mult_f32(&Temp1_NS_NC,&Temp2_NC_NS,&Q);
  
  arm_mat_add_f32(&Temp3_NS_NS,&Q,&NP);
  symmetrize(&NP);


  /*
   * Update Step
   */

  // Equation 3.0.8
  h_update();

  // Equation 3.0.9
  H_update();

  // Equation 3.0.1
  arm_sub_f32(obs,predictedobs,y,NBOBS);


   // Compute S
   // Equation 3.0.2
   // (obs , state) x (state,state) -> (obs, state) 
   // H . P{k|k-1}
  arm_mat_mult_f32(&H,&NP,&Temp4_NO_NS);
   // (obs,state) -> (state,obs)
  arm_mat_trans_f32(&H,&Temp5_NS_NO);
   // (obs,state) x (state,obs) -> (obs,obs)
   // H . P{k|k-1} . H^t
  arm_mat_mult_f32(&Temp4_NO_NS,&Temp5_NS_NO,&Temp6_NO_NO);
   // H . P{k|k-1} . H^t + R
  arm_mat_add_f32(&Temp6_NO_NO,&R,&S);
  

   // Compute K 
   // Equation 3.0.3
   // (state,state) x (state,obs) -> (state,obs)
   // P{k|k-1} . H^t
  arm_mat_mult_f32(&NP,&Temp5_NS_NO,&Temp7_NS_NO);

   // This is temporary and not the right way.
   // When CMSIS-DSP is including Cholesky, this code will be updated.
  arm_mat_inverse_f32(&S,&Temp8_NO_NO);

   // (state,obs) x (obs,obs) -> (state,obs)
   // P{k|k-1} . H^t . S^-1
  arm_mat_mult_f32(&Temp7_NS_NO,&Temp8_NO_NO,&K);



  // Update state
  // Equation 3.0.4

  arm_mat_vec_mult_f32(&K,y,oldstate);
   // x{k|k-1}  + K y
  arm_add_f32(oldstate,newstate,oldstate,NBSTATE);



   // Update covariance
  // Equation 3.0.5
  arm_mat_mult_f32(&K, &H,&Temp1_NS_NS);
  arm_mat_sub_f32(&Identity_NS_NS,&Temp1_NS_NS,&Temp2_NS_NS);
  arm_mat_mult_f32(&Temp2_NS_NS,&NP,&P);

  symmetrize(&P);


  /*

  Same remark about quaternion normalization. It may not be needed
  to do it each time.

  */
  normalizeQuat(&Q0,&Q1,&Q2,&Q3);

  
}


/*

To read the current orientation from main

*/
void getQuaternion(float *q0, float *q1, float *q2,float *q3)
{
  *q0 = Q0;
  *q1 = Q1;
  *q2 = Q2;
  *q3 = Q3;
}

