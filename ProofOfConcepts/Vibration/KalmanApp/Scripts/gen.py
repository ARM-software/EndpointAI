# ----------------------------------------------------------------------
# Project:      KalmanApp
# Title:        gen.py
# Description:  Generation of C code for Kalman
#
# -------------------------------------------------------------------- 
#
# Copyright (C) 2010-2020 ARM Limited or its affiliates. All rights reserved.
#
# SPDX-License-Identifier: Apache-2.0
#
# Licensed under the Apache License, Version 2.0 (the License); you may
# not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an AS IS BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

#
# This script is used to generate macros.h
# If you change the model or add new sensors : you'll need to modify this script
#

import argparse
from sympy import symbols
from sympy.algebras.quaternion import Quaternion
from sympy import *
from sympy.utilities.codegen import codegen


def rotationFromUnitQuaternion(q):
    return(Matrix([[q.a**2 + q.b**2 - q.c**2 - q.d**2, 2 *(q.b *q.c - q.a *q.d), 
        2 *(q.b *q.d + q.a *q.c)],
 [2* (q.b *q.c + q.a *q.d), q.a**2 - q.b**2 + q.c**2 - q.d**2, 2* (q.c *q.d - q.a *q.b)],
 [2 *(q.b *q.d - q.a *q.c), 2* (q.c *q.d + q.a* q.b), q.a**2 - q.b**2 - q.c**2 + q.d**2]]))

# In C code, there is a SQ macro to compute the square
# So we customize the code generator so that SQ is generated instead of pow
custom={'Pow': [
 (lambda b, e: e == 2, lambda b, e: 'SQ(%s)' % b),
 (lambda b, e: e != 2, 'pow')]}

parser = argparse.ArgumentParser(description='Parse gen description')


parser.add_argument('-code', nargs='?',type = str, default="macros.h",help="Code")

args = parser.parse_args()

q0, q1, q2, q3, wx, wy, wz, dt, nq0, nq1, nq2, nq3 = symbols('Q0 Q1 Q2 Q3 WX WY WZ dt NQ0 NQ1 NQ2 NQ3')

# To add new sensors or change the state model you'll have to :
# Update the list of state variable and control variable
# Update the definition of f and h
# 
# Customize this script for using oldstate and newstate instead of Qx and NQx
# since macro for Qx and NQx are only defined for x in [0,3]
# Or update the C code with new macros for the additional state variables
#

# State variables in the code
stateVars=[q0,q1,q2,q3]

# Updated state variables in the code (predictino step)
newStateVars=[nq0,nq1,nq2,nq3]
controlVars=[wx,wy,wz]

q = Quaternion(q0,q1,q2,q3)
w = Quaternion(0,wx,wy,wz)

# This formula is expression how the quaternion is changed
# for a given rotation speed. It is taking into account the fact that
# the rotation speed is in local coordinates.
delta = dt / 2.0 * q *w 

# Equation 2.0.1
f = delta + q

# Convert the quaternion to a vector
fv=Matrix([f.a, f.b, f.c, f.d])

# Equation 2.0.4 (F_k in documentation)
FM = fv.jacobian(stateVars)

# Equation 2.0.5
FQ = fv.jacobian(controlVars)


r = rotationFromUnitQuaternion(Quaternion(nq0,nq1,nq2,nq3))

g = Matrix([[0],[0],[-9.81]])
m = Matrix([[-1],[0],[0]])

ha = r.transpose() * g
hm = r.transpose() * m

# Equation 3.0.8
h = ha.col_join(hm)

# Equation 3.0.9
H = h.jacobian(newStateVars)

def genh(he):
    i = 0
    print("#define h_update()",end="",file=he)
    for r in h:
        print(" \\",file=he)
        print(" predictedobs[%d] = %s;"  % (i, ccode(r,user_functions=custom)),end="",file=he)
        i = i + 1
    print("\n",file=he)
    
def genH(he):
   i=0
   rows = len(h)
   cols = len(newStateVars)
   print("#define H_update()",end="",file=he)
   for r in range(0,rows):
     for c in range(0,cols):
        print(" \\",file=he)
        print(" M(H, %d, %d) = %s;" % (r,c,ccode(H[r,c])),end="",file=he)
   print("\n",file=he)

def genF(h):
   i=0
   rows = len(fv)
   cols = len(stateVars)
   print("#define F_update()",end="",file=h)
   for r in range(0,rows):
     for c in range(0,cols):
        print(" \\",file=h)
        print(" M(F, %d, %d) = %s;" % (r,c,ccode(FM[r,c])),end="",file=h)
   print("\n",file=h)

def genFQ(h):
   i=0
   rows = len(fv)
   cols = len(controlVars)
   print("#define FQ_update()",end="",file=h)
   for r in range(0,rows):
     for c in range(0,cols):
        print(" \\",file=h)
        print(" M(FQ, %d, %d) = %s;" % (r,c,ccode(FQ[r,c])),end="",file=h)
   print("\n",file=h)

def genf(h):
   i=0
   print("#define f_update()",end="",file=h)
   for row in fv:
      print(" \\",file=h)
      print((" NQ%d = " + ccode(row) + ";") % i,end="",file=h)
      i = i + 1
   print("\n",file=h)

if args.code:
  with open(args.code,"w") as header:
    print("#ifndef _MACROS_H",file=header)
    print("#define _MACROS_H\n",file=header)
    genf(header)
    genF(header)
    genFQ(header)
    genh(header)
    genH(header)
    print("#endif /* #ifndef _MACROS_H */",file=header)


