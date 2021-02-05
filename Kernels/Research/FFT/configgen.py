#
#
# Copyright (C) 2010-2021 ARM Limited or its affiliates. All rights reserved.
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

import argparse
import os.path
import numpy as np
from sympy.combinatorics import Permutation
from config.fft import *
from config.core import *
import config.io as io
import yaml

parser = argparse.ArgumentParser(description='Config generation')

parser.add_argument('-d', nargs='?',type = str, default="Generated", help="Generated folder")
parser.add_argument('-twid', nargs='?',type = str, default="twiddle", help="Name of twiddle file")
parser.add_argument('-perm', nargs='?',type = str, default="perm", help="Name of permutation file")
parser.add_argument('-config', nargs='?',type = str, default="config", help="Name of configuration file")
parser.add_argument('-mode', nargs='?',type = str, default="mode", help="Name of mode configuration file (DSPE, C ...)")
parser.add_argument('-r', nargs='?',type = str, default=".", help="Root folder")


parser.add_argument('others', nargs=argparse.REMAINDER)

args = parser.parse_args()

if args.others:
    with open(args.others[0],"r") as f:
       configfile=yaml.safe_load(f)


# Global config describing all the configurations for a core.
# It is representating the yaml config file.
class ConfigDescription:
   
    def __init__(self,core):
        self._twiddlesForDataType={} 
        # Permutations tables for size and datatype.
        # We try to reuse permutation tables.
        self._perms={} 
        
        # ID used to idebtify the table and name the arrays in the
        # generated C++
        self._twiddleID=0
        self._permID=0 
        self._configID=0
        self._core=core 
        # List of all radix used to avoid compiling all radixes in the C++ code later
        self._radixUsed=set([])
        # List of all datatype used to avoid compiling all datatype versions in
        # the C++ later
        self._datatypeUsed=set([])

        # Configurations for size and datype
        self._configs={}

    @property 
    def core(self):
        return(self._core)

    # C , DSP. It is the scalar version of the radix to select in C++ code
    def coreMode(self,datatype):
      return(self._core.mode(datatype))

    # Has Vector version (tail still use scalar so both versions are used)
    def hasVector(self,datatype):
      return(self._core.hasVector(datatype))

    # Helium, Neon or None
    # Vector version to select (if any) in the C++ code.
    def vectorArchi(self,datatype):
      return(self._core.vectorArchi(datatype))

    @property 
    def configs(self):
        return(self._configs)

    # Record that a datatype is used in the configuration file
    def addUsedDatatype(self,datatype):
        self._datatypeUsed.add(datatype)

    # Add factors of a FFT length to identify the used radixes
    # (Factors are only expressed with the supported radixes)
    def addUsedFactor(self,factors):
        # Add used factors
        # to define compilation flags and avoid instantiating
        # templates when radix is not used
        for f in factors:
            self._radixUsed.add(f)

    # Add factors and permutations for a given FFT length and datatype
    def addFactors(self,selectedCore,nb,datatype):
        factors=Perm.getFactors(selectedCore,nb,datatype)
        if not tuple(factors) in self._perms:
           perm=Perm(selectedCore,nb,datatype)
           self._perms[tuple(factors)]=perm
           # Record all used factors
           self.addUsedFactor(factors)
           
           return(perm)
        else:
            return(self._perms[tuple(factors)])


    # Add twidlles for a given datatype and fft length and kind of
    # transform (RFFT needs different twiddles)
    def addTwiddles(self,transform,datatype,l):
            if not ((transform,datatype,l) in self._twiddlesForDataType):
                twid = Twiddle(transform,l,datatype,self.coreMode(datatype))
                self._twiddlesForDataType[(transform,datatype,l)] = twid 
                return(twid)
            else:
                return(self._twiddlesForDataType[(transform,datatype,l)])
            

    # Add a configuration : twiddle, permutation, CFFT or RFFT, core mode (C or DSP)
    def addConfig(self,transform,twid,perm):
       newConf = Config(transform,twid,perm,self.coreMode(twid.datatype)) 
       nb = twid.nbSamples 
       datatype=twid.datatype

       if transform in self.configs:
         if datatype in self.configs[transform]:
            self.configs[transform][datatype][nb] = newConf
         else:
            self.configs[transform][datatype] = {} 
            self.configs[transform][datatype][nb] = newConf
       else:
        self.configs[transform]={} 
        self.configs[transform][datatype] = {} 
        self.configs[transform][datatype][nb] = newConf

    # Write the twiddle files
    # All the twidlle arrays and their headers
    def writeTwiddle(self):
        headerpath=os.path.join(args.d,args.twid) + ".h"
        cpath=os.path.join(args.d,args.twid) + ".cpp"
        with open(headerpath,"w") as h:
            with open(cpath,"w") as c:
                 print("#include \"fft_types.h\"",file=c)
                 print(io.cheader % args.twid,file=c)
                 print(io.hheader % ("TWIDDLE_H","TWIDDLE_H"),file=h)

                 print("using namespace FFTSCI;",file=h)

                 for k in self._twiddlesForDataType:
                     transform,dt,n = k 
                     ctype = getCtype(dt)
                     twid = self._twiddlesForDataType[k];

                     if ctype == "float16_t":
                         print("#if defined(ARM_FLOAT16_SUPPORTED)",file=c) 
                         print("#if defined(ARM_FLOAT16_SUPPORTED)",file=h)

                     twid.writeTwidHeader(h)
                     twid.writeTwidCode(c)

                     if ctype == "float16_t":
                        print("#endif\n",file=c) 
                        print("#endif\n",file=h) 

            print(io.hfooter % ("TWIDDLE_H"),file=h)
        
        
    # Write the permutation file
    # All the permutation arrays
    def writePerms(self):
        headerpath=os.path.join(args.d,args.perm) + ".h"
        cpath=os.path.join(args.d,args.perm) + ".cpp"
        with open(headerpath,"w") as h:
            with open(cpath,"w") as c:
                print("#include \"fft_types.h\"",file=c)
                print(io.cheader % args.perm,file=c)
                print(io.hheader % ("PERM_H","PERM_H"),file=h)
                for p in self._perms:
                   perm=self._perms[p]
                   perm.writePermHeader(h)
                   perm.writePermCode(c)
                print(io.hfooter % ("PERM_H"),file=h)




    # Write the configurations structures
    def writeConfigStructs(self,c,ctype,transform,confs):
        for nb in confs:
            conf=confs[nb]
            conf.writeConfigCode(c)

            

    # Write the initialization code
    # (The switch/case to select  configuration based upon the transform length)
    def writeConfigCode(self,c,ctype,transform,confs):
        if (transform == "CFFT"):
            print(io.cfftconfigFuncStart % (ctype,ctype),file=c)
        else:
            print(io.rfftconfigFuncStart % (ctype,ctype),file=c)
        sortedConfs=sorted(confs.keys())
        for nb in sortedConfs:
            conf=confs[nb]
            print(io.fftCase % (nb,conf.configID), file=c)

        print(io.fftconfigFuncStop,file=c)

    # Add header to rfft or cfft for a given datatype.
    # It is a reference to the temple to avoid instantiating the template twice.
    def addTransformDatatypeHeader(self,transform,datatype,h):
        print("\n#define FUNC_%s_%s\n" % (transform,datatype.upper()),file=h)

        ctype=getCtype(datatype)
        if ctype == "float16_t":
             print("#if defined(ARM_FLOAT16_SUPPORTED)",file=h) 

        if (transform=="RFFT"):
          print("extern template const rfftconfig<%s> *rfft_config<%s>(uint32_t nbSamples);" % 
              (ctype,ctype),file=h)
        else:
          print("extern template const cfftconfig<%s> *cfft_config<%s>(uint32_t nbSamples);" % 
              (ctype,ctype),file=h)
        
        if ctype == "float16_t":
            print("#endif\n",file=h) 

    # Write configurations headers and configuration code.
    # Define the scalar mode (C or DSPE), the datatypes
    # The vector mode, the vector archi (Helium or Neon)
    # Those #defines are used to only include and instantiate the required code 
    # in C++ side
    def writeConfigs(self):
        headerpath=os.path.join(args.d,args.config) + ".h"
        headerfuncpath=os.path.join(args.d,args.config) + "_func.h"
        modepath=os.path.join(args.d,args.mode) + ".h"
        cpath=os.path.join(args.d,args.config) + ".cpp"
        with open(modepath,"w") as h:

          print(io.modeheader % ("MODE_H","MODE_H"),file=h)

          print("",file=h)

          hasAnyVector = False

          for datatype in self._datatypeUsed:
                
                if self.coreMode(datatype)=="DSP":
                   print("#define DSPE_%s 1\n" % datatype.upper(),file=h)

                if self.hasVector(datatype):
                   print("#define VECTOR_%s 1\n" % datatype.upper(),file=h)
                   hasAnyVector = True

          if hasAnyVector:
                print("#define HASVECTOR 1\n",file=h)
                if self.vectorArchi(datatype)=="Helium":
                   print("#define HELIUM_VECTOR 1\n",file=h)
                if self.vectorArchi(datatype)=="Neon":
                   print("#define NEON_VECTOR 1\n",file=h)

          print(io.modefooter % ("MODE_H"),file=h)


        with open(headerpath,"w") as h:
            print(io.hheader % ("CONFIG_H","CONFIG_H"),file=h)

            
            print("",file=h)
            for radix in self._radixUsed:
                print("#define RADIX%d\n" % radix,file=h)
            for datatype in self._datatypeUsed:
                print("#define DATATYPE_%s\n" % datatype.upper(),file=h)
              

            print(io.hfooter % ("CONFIG_H"),file=h)


        with open(headerfuncpath,"w") as h:
            print(io.hheader % ("CONFIG_FUNC_H","CONFIG_FUNC_H"),file=h)

            print("",file=h)

            print("using namespace FFTSCI;",file=h)
            
            for transform in self.configs:
                for datatype in self.configs[transform]:

                    self.addTransformDatatypeHeader(transform,datatype,h)

            print(io.hfooter % ("CONFIG_FUNC_H"),file=h)

        with open(cpath,"w") as c:
            print("#include \"fft_types.h\"",file=c)
            print("#include \"perm.h\"",file=c)
            print("#include \"twiddle.h\"\n",file=c)

    
            # Write factors arrays used in several config
            for p in self._perms:
                perm=self._perms[p]
                perm.writeFactorDesc(c)

            for transform in self.configs:
                for datatype in self.configs[transform]:
                    ctype=getCtype(datatype)
                    if ctype == "float16_t":
                         print("#if defined(ARM_FLOAT16_SUPPORTED)",file=c) 

                    self.writeConfigStructs(c,ctype,transform,self.configs[transform][datatype])
                    self.writeConfigCode(c,ctype,transform,self.configs[transform][datatype])
    
                    if ctype == "float16_t":
                        print("#endif\n",file=c) 

            print("",file=c)

    # Generate the twiddle, permutation and configuration files.
    def generate(self):
        self.writeTwiddle()
        self.writePerms() 
        self.writeConfigs()

# When a RFFT is defined in the yaml file, it will require
# some CFFT to be defined.
# This is done by the function so that the user do not have to
# list the CFFT in the yaml. They are implied by the RFFT setting.
def addCFFTForRFFT(configfile,core):
  # ADD CFFT needed for RFFT 
  if ("RFFT" in configfile[core]) and not ("CFFT" in configfile[core]):
       configfile[core]["CFFT"] = {}
  for transform,transformData in configfile[core].items():
      if transform == "RFFT":
         for datatype,datatypeLengths in transformData.items():
             cfftLengths=[x // 2 for x in datatypeLengths]
             
             cfft=configfile[core]["CFFT"]
             if not (datatype in cfft):
               cfft[datatype] = {}
   
             cfft[datatype]= list(set(cfft[datatype]) | set(cfftLengths))

# A CFFT2D of size nxm is implifying CFFT od size n and m
# With this function, those CFFT do not have to be written in
# the yaml file. They are implied by the CFFT2D settings.
def addCFFTForCFFT2D(configfile,core):
  # ADD CFFT needed for RFFT 
  if ("CFFT2D" in configfile[core]) and not ("CFFT" in configfile[core]):
       configfile[core]["CFFT"] = {}
  for transform,transformData in configfile[core].items():
      if transform == "CFFT2D":
         for datatype,datatypeLengths in transformData.items():
             cfftLengthRows=[x[0]  for x in datatypeLengths]
             cfftLengthCols=[x[1]  for x in datatypeLengths]
             
             cfft=configfile[core]["CFFT"]
             if not (datatype in cfft):
               cfft[datatype] = {}
   
             cfft[datatype]= list(set(cfft[datatype]) | set(cfftLengthRows) | set(cfftLengthCols))
     
# Known cores defined in fft.py
KNOWNCORES=['M0','M4','M7','M33','M55','M55NOMVE','A32','R8','R52']

# Supported datatypes.
# (They are not yet all supported in the C++ code)
KNOWNTYPES=['f64','f32','f16','q31','q15','q7']

# Known transforms.
KNOWNTRANSFORMS=['CFFT','RFFT','CFFT2D']

# Parsing of the YAML configuration file.
cores=list(configfile.keys())
if len(cores)>1:
    print("You can only specify one core")
else:
       core=cores[0]
       # Parse the know core or do nothing
       if core in KNOWNCORES:
           print("Selected core = %s" % core)
           config=None 
           selectedCore=None
           
           if core == 'M0':
              selectedCore=M0()

           if core == 'M4':
              selectedCore=M4()

           if core == 'M7':
              selectedCore=M7()

           if core == 'M33':
              selectedCore=M33()

           if core == 'M55':
              selectedCore=M55()

           if core == 'M55NOMVE':
              selectedCore=M55NOMVE()

           if core == 'A32':
              selectedCore=A32()

           if core == 'R8':
              selectedCore=R8()

           if core == 'R52':
              selectedCore=R52()
           
           # Create a config description object to track all the
           # configurations for this code
           config=ConfigDescription(selectedCore)
           
           # Add missing CFFT for RFFT and CFFT2D
           # Like that other steps don't have to do anything special
           # to manage those CFTT.
           addCFFTForRFFT(configfile,core) 
           addCFFTForCFFT2D(configfile,core)

           # Iterate of the transform (CFFT, RFFT, CFFT2D)
           for transform,transformData in configfile[core].items():
               # CFFT2D are transformed into 2 CFFT so already taken into account
               # So we don't process the CFF2D.
               # Only RFFT (they have a special config) and CFFT
               if transform in KNOWNTRANSFORMS and (transform != "CFFT2D") :
                   print("  TRANSFORM = %s" %transform);
                   # Parse all datatypes for the current transform
                   for datatype,datatypeLengths in transformData.items():
                       if datatype in KNOWNTYPES:
                           print("    DATATYPE = %s" % datatype)
                           # Record that this datatype is used 
                           # to enable in the C++ code instantiation for this
                           # datatype
                           config.addUsedDatatype(datatype)
                           # Add twiddle for all lengths for this datatype
                           # Normally RFFT, CFFT check should be added

                           # Add list of factors to be used to compute a 
                           # permutation to reuse permutation between
                           # different FFT datatypes if they have same factor list
                           # Permutations are expressed in samples so are only dependent
                           # on the length decomposition not directly on the
                           # datatype.
                           for nb in datatypeLengths:
                               # Add twiddle for the transform
                               twid=config.addTwiddles(transform,datatype,nb)

                               # If the transform is a CFFT then we need also
                               # a permutation table.
                               if transform != "RFFT":
                                  perm=config.addFactors(selectedCore,nb,datatype)
                               else:
                                  perm=None
                               config.addConfig(transform,twid,perm)
                       else:
                           print("%s is an unsupported datatype" % datatype)
               else:
                  if transform != "CFFT2D":
                     print("%s is an unsupported transform" % transform)
       else:
          print("%s is an unsupported core. You may customize this script and core.py to add it" % core)  
                           
# Parse the YAM and generate all source files
config.generate()

