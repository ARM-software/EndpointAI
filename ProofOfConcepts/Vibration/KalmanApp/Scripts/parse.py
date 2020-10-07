# ----------------------------------------------------------------------
# Project:      KalmanApp
# Title:        parse.py
# Description:  Parsing of sensor log to generate C data file
#
# -------------------------------------------------------------------- */
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
# -------------------------------------------------------------------------

#
# This script is generating data.h and data.c from log.txt
# You should not need to use it unless to are regenerating calibrated sensor
# values from a different board.
# But in that case, you'll also need to update all the noise covariance values
# in the C code. You''ll also have to ensure that the assumption made about the
# x,y,z axis are the same on your board and in this demo.
# If not the case, you may also have to change the expected observations in the
# script gen.py
#
#

import argparse
import re

NB = 10

parser = argparse.ArgumentParser(description='Parse test description')

parser.add_argument('-l', nargs='?',type = str, default="log.txt",help="Log file")
parser.add_argument('-c', nargs='?',type = str, default="data.c",help="C file")
parser.add_argument('-he', nargs='?',type = str, default="data.h",help="h file")
parser.add_argument('-o', nargs='?',type = str, default="ref.csv",help="output reference")


args = parser.parse_args()

allBlocks=[]
startState = {}
outputs=[]

def joinit(iterable, delimiter):
    # Intersperse a delimiter between element of a list
    it = iter(iterable)
    yield next(it)
    for x in it:
        yield delimiter
        yield x

class Block():
    def __init__(self,dt,w,a,m):
        self._dt = dt
        self._w = w 
        self._a = a 
        self._m = m

    @property
    def nb(self):
        return(10)

    def print(self,c):
        print("%s,%s,%s," % (self._w[0],self._w[1],self._w[2]),end='',file=c)
        print("%s,%s,%s," % (self._a[0],self._a[1],self._a[2]),end='',file=c)
        print("%s,%s,%s," % (self._m[0],self._m[1],self._m[2]),end='',file=c)
        print("%s," % self._dt[0],file=c)

def cvar(name,v):
    print("float32_t %s=%f;" % (name,v))

def inputdata(c,h,name):
    global allBlocks
    print("float32_t %s[%d]={" % (name,allBlocks[0].nb*len(allBlocks)),file=c)
    for b in allBlocks:
        b.print(c)
    print("};\n",file=c)

    print("extern float32_t %s[%d];" % (name,allBlocks[0].nb*len(allBlocks)),file=h)
    print("#define BLOCKSIZE %d" % allBlocks[0].nb,file=h)


def outputdata(o,name):
    global outputs
    for r in outputs:
        print("".join(joinit(r,",")),file=o)

CPPSTART="""#ifdef   __cplusplus
extern "C"
{
#endif
"""

CPPSTOP="""#ifdef   __cplusplus
}
#endif
"""

def cvect(c,h,name,v):
    
    print("float32_t %s[%d]={" % (name,len(v)),file=c)
    i = 0
    l = [] 
    for x in v:
        l.append(x)
        i = i + 1
        if i == NB:
            i = 0 
            print("".join(joinit(l,",")) + ",",file=c)
            l = []
    if len(l) > 0:
        print("".join(joinit(l,",")),file=c)
    print("};\n",file=c)

    print("extern float32_t %s[%d];" % (name,len(v)),file=h)

def vector(a):
    return(a.strip(' \n').split(' '))

def addBlock(b):
    global outputs
    global startState 
    global allBlocks
    #0 : P
    #1 : {{0.11,0.00,0.00,0.00}
    #2 : ,{0.00,0.11,0.00,0.00}
    #3 : ,{0.00,0.00,0.11,0.00}
    #4 : ,{0.00,0.00,0.00,0.11}
    #5 : }
    #6 : 0.58 0.41 0.40 0.57
    #7 : Gyro: -1.12 -1.07 1.57
    #8 : Accelerometer: 0.42 -0.54 9.80
    #9 : Magnetometer: -0.74 -0.64 -0.21
    #10 : Orientation: 0.36 0.80 0.38 -0.31
    p=[]
    p.append(re.sub(r'[{}\n]','',b[1]).lstrip(',').split(','))
    p.append(re.sub(r'[{}\n]','',b[2]).lstrip(',').split(','))
    p.append(re.sub(r'[{}\n]','',b[3]).lstrip(',').split(','))
    p.append(re.sub(r'[{}\n]','',b[4]).lstrip(',').split(','))
    p = [item for sublist in p for item in sublist]
    #print(p)

    q = vector(b[6])
    #print(q)

    w=vector(re.sub(r'^Gyro: ','',b[7]))
    #print(w)

    a=vector(re.sub(r'^Accelerometer: ','',b[8]))
    #print(w)

    m=vector(re.sub(r'^Magnetometer: ','',b[9]))
    #print(w)

    dt=vector(re.sub(r'^dt: ','',b[10]))
    #print(dt)

    newq=vector(re.sub(r'^Orientation: ','',b[11]))
    #print(w)

    if not allBlocks :
        startState = {"p":p, "q":q}


    outputs.append(newq)
    allBlocks.append(Block(dt,w,a,m))

if args.l:
    print("Parsing log")
    started = False
    blockOpened=False
    block=[]
    with open(args.l,"r") as inp:
        for line in inp:
            if not started:
                if re.match(r'^P$',line):
                    started=True 
            if started:
                if not blockOpened:
                   if re.match(r'^P$',line):
                      blockOpened = True 
                      block=[]
                if blockOpened:
                    block.append(line)
                    if re.match(r'^Orientation:.*$',line):
                        blockOpened = False
                        block.append(line)
                        addBlock(block)
                        block=[]

    print("Generate C")
    if args.c and args.he:
       with open(args.c,"w") as c:
        with open(args.he,"w") as h:
           print("#ifndef _DATA_H",file=h)
           print("#define _DATA_H",file=h)
           print("#include \"arm_math_types.h\"",file=h)
           print(CPPSTART,file=h)

           print("#include \"data.h\"",file=c)
           cvect(c,h,"initQ",startState["q"])
           cvect(c,h,"initP",startState["p"])
           inputdata(c,h,"inputData")
           print("#define NBSAMPLES %d" % len(allBlocks),file=h)

           print(CPPSTOP,file=h)
           print("#endif /* #ifdef  _DATA_H */",file=h)
    if args.o:
        with open(args.o,"w") as o:
          outputdata(o,"output")


