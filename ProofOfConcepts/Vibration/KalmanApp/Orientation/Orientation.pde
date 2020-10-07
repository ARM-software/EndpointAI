/* ----------------------------------------------------------------------
 * Project:      KalmanApp
 * Title:        Orientation.pde
 * Description:  Visualization of the board orientation
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

Table table;

int id;

float q0,q1,q2,q3;

void setup()
{
  size(600, 600, P3D);
  ortho();

  table = loadTable("../build/output.csv");
  
  textSize(16); // set text size
  textMode(SHAPE); // set text mode to shape
  
  id = 0;
  
}

float SQ(float x)
{
  return(x*x);
}

void axis()
{
  textSize(32);
  
stroke(255, 0, 0);
line(0, 0, 0, 150, 0, 0);
stroke(0, 255, 0);
line(0, 0, 0, 0, 150, 0);
stroke(0, 0, 255);
line(0, 0, 0, 0, 0, 150);


pushMatrix();
translate(150,0,0);
rotateZ(radians(-40));
rotateX(radians(-90));
text("X (S)",0,0,0);
popMatrix();

pushMatrix();
translate(0,150,0);
rotateZ(radians(-40));
rotateX(radians(-90));
text("Y (E)",0,0,0);
popMatrix();

pushMatrix();
translate(0,0,150);
rotateZ(radians(-40));
rotateX(radians(-90));
text("Z (U)",0,0,0);
popMatrix();

}

void draw()
{
  hint(ENABLE_DEPTH_TEST);
  TableRow row = table.getRow(id);
  
  id = id + 1;
  if (id >= table.getRowCount()-10)
  {
    id = table.getRowCount()-10;
  }

  q0 = row.getFloat(0);
  q1 = row.getFloat(1);
  q2 = row.getFloat(2);
  q3 = row.getFloat(3);
  
  background(255); 
  ambientLight(128, 128, 128);
  directionalLight(255,255,255, 0, 0,1);
  lightFalloff(1, 0, 0);
  lightSpecular(0, 0, 0);
  
   

  translate(width/2, height/2); // set position to centre
  scale(1,1,-1.0);
  rotateX(radians(180));
  rotateZ(radians(-10));
  rotateX(radians(-45));
  rotateZ(radians(70));

  axis();

  pushMatrix(); // begin object

  applyMatrix(SQ(q0) + SQ(q1) - SQ(q2) - SQ(q3), -2*(q1*q2 - q0*q3), -2*(q0*q2 + q1*q3), 0,
  -2*(q1*q2 + q0*q3), SQ(q0) - SQ(q1) + SQ(q2) - SQ(q3), -2*(-(q0*q1) + q2*q3), 0,
  2*(-(q0*q2) + q1*q3), 2*(q0*q1 + q2*q3), (SQ(q0) - SQ(q1) - SQ(q2) + SQ(q3)),0,
  0,0,0,1
  );

  drawBoard();
  

  popMatrix(); // end of object

   hint(DISABLE_DEPTH_TEST);
   
   delay(3);
}



void drawBoard()
{
  pushMatrix();
  
  rotateX(radians(180)); 
  rotateZ(radians(90)); 


  stroke(0); 
  fill(148, 214, 0); 
  box(70, 190, 12); 

  
  
  translate(0, 75, 18);
  

  stroke(128);
  fill(125, 134, 140);
  box(30,30,6);

 

  
  
  popMatrix();
}
