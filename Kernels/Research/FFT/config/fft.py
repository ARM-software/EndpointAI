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

from sympy.ntheory import factorint
import numpy as np
from sympy.combinatorics import Permutation
import io 
import math
from config.strtools import *
import itertools
import struct
import config.formats

# Conversion of double to fixed point values
#
# - 8000 gives 8000 in C (int16)
# So when it is multiplied it will give the wrong sign for the result
# of the multiplication except if DSPE instructions with saturation are used
# to compute the negate (and we should get 7FFF).
#
# So for cortex-m without DSP extension, we should try to use 8001
# It is done but not yet tested.

def to_q63(v,dspe):
    r = int(round(v * 2**63))
    if (r > 0x07FFFFFFFFFFFFFFF):
      r = 0x07FFFFFFFFFFFFFFF
    if (r < -0x08000000000000000):
      if dspe:
         r = -0x08000000000000000
      else:
         r = -0x07FFFFFFFFFFFFFFF
    return ("0x%s" % format(struct.unpack('<Q', struct.pack('<q', r))[0],'016X'))

def to_q31(v,dspe):
    r = int(round(v * 2**31))
    if (r > 0x07FFFFFFF):
      r = 0x07FFFFFFF
    if (r < -0x080000000):
      if dspe:
        r = -0x080000000
      else:
        r = -0x07FFFFFFF
    return ("0x%s" % format(struct.unpack('<I', struct.pack('<i', r))[0],'08X'))

def to_q15(v,dspe):
    r = int(round(v * 2**15))
    if (r > 0x07FFF):
      r = 0x07FFF
    if (r < -0x08000):
      if dspe:
         r = -0x08000
      else:
         r = -0x07FFF
    return ("0x%s" % format(struct.unpack('<H', struct.pack('<h', r))[0],'04X'))

def to_q7(v,dspe):
    r = int(round(v * 2**7))
    if (r > 0x07F):
      r = 0x07F
    if (r < -0x080):#
      if dspe:
         r = -0x080
      else:
         r = -0x07F
    return ("0x%s" % format(struct.unpack('<B', struct.pack('<b', r))[0],'02X'))



Q7=1
Q15=2
Q31=3
F16=4
F32=5
F64=6

# In the final C++ code, we have a loop for a given radix.
# The input list here has not grouped the factors.
# The list need to be transformed into a list of pair.
# The pair being (radix,exponent)
def groupFactors(factors):
    n = 0 
    current=-1
    result=[] 
    for f in factors:
        if f != current:
            if current != -1:
               result = result + [current,n]
            current=f 
            n=1
        else:
            n=n+1 
    result = result + [current,n]
    return(result)

# Compute the grouped factors for the the FFT length originaln
# where the only possible radix are in primitiveFactors list.
def getFactors(primitiveFactors,originaln):
    factors=[]
    length=[]
    primitiveFactors.sort(reverse=True)
    n = originaln
    while (n > 1) and primitiveFactors:
        if (n % primitiveFactors[0] == 0):
            factors.append(primitiveFactors[0])
            n = n // primitiveFactors[0]
        else:
            primitiveFactors=primitiveFactors[1:]

    # When lowest factors are at the beginning (like 2)
    # we use a special implementation of the loopcore template
    # and it is removing some cycles.
    # So, we will get (for instance) 2x8x8x8 instead of 8x8x8x2
    factors.reverse()

    for f in factors:
        originaln = originaln // f
        length.append(originaln)

    groupedfactors=groupFactors(factors)
       
    return(groupedfactors,factors,length)

# Apply the radix decomposition to compute the input -> output permutation
# computed by the FFT.
def radixReverse(f,n):
    a=np.array(range(0,n)).reshape(f)
    r = list(range(0,len(f)))
    r.reverse()
    r = tuple(r)
    a = np.transpose(a,r)
    return(a.reshape(n))

def radixPermutation(factors,n):
    a = radixReverse(factors,n)
    tps = []
    vectorizable=True
    for c in Permutation.from_sequence(a).cyclic_form:
       if (len(c)>2):
          vectorizable = False
       for i in range(len(c)-1,0,-1):
           # 2 because those are indexes in an array of complex numbers but
           # with a real type.
           tps.append([2*c[i], 2*c[i-1]])

    return(np.array(tps,dtype=int).flatten(),vectorizable)
    

# CFFT Twiddle table
def cfft_twiddle(n):
    a=2.0*math.pi*np.linspace(0,n,num=n,endpoint=False)/n
    c=np.cos(-a)
    s=np.sin(-a)

    r = np.empty((c.size + s.size,), dtype=c.dtype)
    r[0::2] = c
    r[1::2] = s
    return(r)

# RFFT twiddle for the merge and split steps.
def rfft_twiddle(n):
    a=2.0j*math.pi*np.linspace(0,n//2,num=n // 2,endpoint=False)/n
    z=-1.0j * np.exp(-a)

    r = z.view(dtype=np.float64)

    
    return(r)

# Compute the twiddle tables
def twiddle(transform,n):
    if transform=="CFFT":
        return(cfft_twiddle(n))
    if transform=="RFFT":
        return(rfft_twiddle(n))
    return(None)


NB_ELEMS_PER_LINE=3

# Generate C array content for a given datatype
def printFloat64Array(f,n):
    nb=0
    for s in n:
        print("%.20f, " % s,end="",file=f)
        nb = nb + 1 
        if nb == NB_ELEMS_PER_LINE:
            nb=0
            print("",file=f)

def printFloat32Array(f,n):
    nb=0
    for s in n:
        print("%.20ff, " % s,end="",file=f)
        nb = nb + 1 
        if nb == NB_ELEMS_PER_LINE:
            nb=0
            print("",file=f)

def printFloat16Array(f,n):
    nb=0
    for s in n:
        print("%.8ff16, " % s,end="",file=f)
        nb = nb + 1 
        if nb == NB_ELEMS_PER_LINE:
            nb=0
            print("",file=f)

def printQ31Array(f,mode,n):
    DSPE=False 
    if mode == "DSP":
        DSPE=True
    nb=0
    for s in n:
        print(to_q31(s,DSPE) + ", ",end="",file=f)
        nb = nb + 1 
        if nb == NB_ELEMS_PER_LINE:
            nb=0
            print("",file=f)

def printQ15Array(f,mode,n):
    DSPE=False 
    if mode == "DSP":
        DSPE=True
    nb=0
    for s in n:
        print(to_q15(s,DSPE) + ", ",end="",file=f)
        nb = nb + 1 
        if nb == NB_ELEMS_PER_LINE:
            nb=0
            print("",file=f)

def printQ7Array(f,mode,n):
    DSPE=False 
    if mode == "DSP":
        DSPE=True
    nb=0
    for s in n:
        print(to_q7(s,DSPE) + ", ",end="",file=f)
        nb = nb + 1 
        if nb == NB_ELEMS_PER_LINE:
            nb=0
            print("",file=f)

# Print a C array
# Using the type, dpse mode, name
# (dpse mode is for knowing if 0x8000 must be generated as 8000 or 8001
# to avoid sign issues when multiplying with the twiddles)
def printArray(f,ctype,mode,name,a):
    nbSamples = len(a)
    define = "NB_" + name.upper()
    n = a.reshape(len(a))

    print("__ALIGNED(8) const %s %s[%s]={" % (ctype,name,define),file=f)

    if ctype == "float64_t":
       printFloat64Array(f,n)

    if ctype == "float32_t":
       printFloat32Array(f,n)

    if ctype == "float16_t":
       printFloat16Array(f,n)

    if ctype == "Q31":
       printQ31Array(f,mode,n)

    if ctype == "Q15":
       printQ15Array(f,mode,n)

    if ctype == "Q7":
       printQ7Array(f,mode,n)
    
    print("};",file=f)

# Convert a float value to a given datatype.
def convertToDatatype(r,ctype,mode):
    DSPE=False
    if mode == "DSP":
        DSPE=True
    if ctype == "float64_t":
       result = "%.20f" % r

    if ctype == "float32_t":
       result = "%.20ff" % r

    if ctype == "float16_t":
       result = "%.20ff16" % r

    if ctype == "Q31":
       result = "Q31(%s)" % to_q31(r,DSPE)

    if ctype == "Q15":
       result = "Q15(%s)" % to_q15(r,DSPE)

    if ctype == "Q7":
       result = "Q7(%s)" % to_q7(r,DSPE)

    return(result)

def printArrayHeader(f,ctype,name,nbSamples):
    define = "NB_" + name.upper()

    print("#define %s %d" % (define, nbSamples),file=f)
    print("extern __ALIGNED(8) const %s %s[%s];\n" % (ctype,name,define),file=f)
   

# Print UINT arrays for permutations.
def printUInt32Array(f,name,a):
    nbSamples = len(a)
    define = "NB_" + name.upper()
    n = a.reshape(len(a))

    print("__ALIGNED(8) const uint32_t %s[%s]={" % (name,define),file=f)
    nb=0
    for s in n:
        print("%d, " % s,end="",file=f)
        nb = nb + 1 
        if nb == NB_ELEMS_PER_LINE:
            nb=0
            print("",file=f)

    print("};",file=f)

def printUInt16Array(f,name,a):
    nbSamples = len(a)
    define = "NB_" + name.upper()
    n = a.reshape(len(a))

    print("__ALIGNED(8) const uint16_t %s[%s]={" % (name,define),file=f)
    nb=0
    for s in n:
        print("%d, " % s,end="",file=f)
        nb = nb + 1 
        if nb == NB_ELEMS_PER_LINE:
            nb=0
            print("",file=f)

    print("};",file=f)

def printUInt32ArrayHeader(f,name,a):
    nbSamples = len(a)
    define = "NB_" + name.upper()
    n = a.reshape(len(a))

    print("#define %s %d" % (define, nbSamples),file=f)
    print("extern __ALIGNED(8) const uint32_t %s[%s];\n" % (name,define),file=f)

def printUInt16ArrayHeader(f,name,a):
    nbSamples = len(a)
    define = "NB_" + name.upper()
    n = a.reshape(len(a))

    print("#define %s %d" % (define, nbSamples),file=f)
    print("extern __ALIGNED(8) const uint16_t %s[%s];\n" % (name,define),file=f)


def getCtype(t):
    if t == 'f64':
        return("float64_t")
    if t == 'f32':
        return("float32_t")
    if t == 'f16':
        return("float16_t")
    if t == 'q31':
        return("Q31")
    if t == 'q15':
        return("Q15")
    if t == 'q7':
        return("Q7")
    return("void")

# Configuration structures for CFFT and RFFT
cfftconfig = """cfftconfig<%s> config%d={
   .normalization=%s,
   .nbPerms=%s,
   .perms=perm%d,
   .nbTwiddle=%s,
   .twiddle=twiddle%d,
   .nbGroupedFactors=%d,
   .nbFactors=%d,
   .factors=factors%d,
   .lengths=lengths%d,
   .format=%d,
   .reversalVectorizable=%d
   };"""

rfftconfig = """rfftconfig<%s> config%d={
   .nbTwiddle=%s,
   .twiddle=twiddle%d
   };"""

fftconfigHeader = """extern %sconfig<%s> config%d;"""

fftFactorArray = """const uint16_t factors%d[%d]=%s;\n"""
fftLengthArray = """const uint16_t lengths%d[%d]=%s;\n"""

# Descriptino of a permutation
class Perm:
    PermID = 0

    # Grouped factors and factors.
    def getFactors(core,nb,datatype):
        _groupedFactors,_factors,_lens=getFactors(core.radix(datatype,nb),nb)
        return(_factors)

    
    def __init__(self,core,nb,datatype):
        Perm.PermID = Perm.PermID + 1
        self._nb=nb
        self._id = Perm.PermID
        self._radixUsed=set([])
        self._groupedFactors,self._factors,self._lens=getFactors(core.radix(datatype,nb),nb)
        self._perms = None
        self._core=core
        self._isvectorizable=False

    def permutations(self):
        _permFactors=list(itertools.chain(*[self._core.getPermFactor(x) for x in self._factors]))
        #print(_permFactors)
        self._perms,self._isvectorizable = radixPermutation(_permFactors[::-1],self._nb)

    @property
    def isVectorizable(self):
        return(self._isvectorizable)

    @property
    def permID(self):
        return(self._id)

    @property
    def perms(self):
        if self._perms is not None:
            return(self._perms)
        else:
            self.permutations() 
            return(self._perms)

    @property
    def factors(self):
        return(self._factors)

    @property
    def nbGroupedFactors(self):
        return(int(len(self._groupedFactors)/2))

    @property
    def nbFactors(self):
        return(len(self._factors))

    def writePermHeader(self,h):
        printUInt16ArrayHeader(h,"perm%d" % self.permID,self.perms)

    def writePermCode(self,c):
        printUInt16Array(c,"perm%d" % self.permID,self.perms)

    def writeFactorDesc(self,c):
        radixList="{%s}" % joinStr([str(x) for x in self._groupedFactors])
        lengthList="{%s}" % joinStr([str(x) for x in self._lens])

        print(fftFactorArray % (self.permID,2*self.nbGroupedFactors,radixList),file=c);
        print(fftLengthArray % (self.permID,len(self._lens),lengthList),file=c);

class Twiddle:
    TwiddleId = 0

    def __init__(self,transform,nb,datatype,mode):
        Twiddle.TwiddleId = Twiddle.TwiddleId + 1
        self._id = Twiddle.TwiddleId 
        self._datatype = datatype
        self._nb=nb
        self._twiddle = None
        self._transform=transform
        self._mode=mode

    @property
    def twiddleID(self):
        return(self._id)

    
    @property
    def datatype(self):
        return(self._datatype)

    @property
    def samples(self):
        if self._twiddle is  None:
            self._twiddle=twiddle(self._transform,self._nb)
        return(self._twiddle)

    @property
    def nbSamples(self):
        return(self._nb)

    @property
    def nbTwiddles(self):
        if self._transform=="RFFT":
            return(self._nb // 2)
        else:
           return(self._nb)

    def writeTwidHeader(self,h):
        ctype=getCtype(self.datatype)
        # Twiddle is a complex array so 2*nbSamples must be used
        printArrayHeader(h,ctype,"twiddle%d" % self.twiddleID,2*self.nbTwiddles)

    def writeTwidCode(self,c):
        ctype=getCtype(self.datatype)
        printArray(c,ctype,self._mode,"twiddle%d" % self.twiddleID,self.samples)



class Config:
    ConfigID = 0

    def __init__(self,transform,twiddle,perms,coreMode):
        Config.ConfigID = Config.ConfigID + 1
        self._id = Config.ConfigID 
        self._twiddle=twiddle 
        self._perms=perms
        self._transform=transform
        self._coreMode=coreMode

    @property
    def transform(self):
        return(self._transform)

    @property
    def configID(self):
        return(self._id)

    @property
    def perms(self):
        return(self._perms)

    @property
    def twiddle(self):
        return(self._twiddle)

    @property
    def nbSamples(self):
        return(self.twiddle.nbSamples)

    def writeConfigHeader(self,c):
        ctype=getCtype(self.twiddle.datatype)
        print(fftconfigHeader % (self.transform.lower(),ctype,self.configID),file=c)

    def writeConfigCode(self,c):
        ctype=getCtype(self.twiddle.datatype)
        twiddleLen = "NB_" + ("twiddle%d"% self.twiddle.twiddleID).upper() 

        if self.transform == "RFFT":
            print(rfftconfig % (ctype,self.configID,twiddleLen,self.twiddle.twiddleID),file=c)
        else:
            normfactor = 1.0 / self.twiddle.nbSamples
            normFactorStr = convertToDatatype(normfactor,ctype,self._coreMode)
            permsLen = "NB_" + ("perm%d"% self.perms.permID).upper() 
            
            outputFormat = 0
            #print(self.twiddle.datatype)
            #print(self.twiddle.nbSamples)
            #print(self.perms.factors)

            # For fixed point, each stage will change the output format.
            # We need to cmpute the final format of the FFT
            # and record it in the initialization structure
            # so that the user can easily know how to recover the
            # input format (q31, q15). It is encoded as a shift value.
            # The shift to apply to recover the input format
            # But applying this shift will saturate the result in general.
            if self.twiddle.datatype == "q15" or self.twiddle.datatype == "q31":
                for f in self.perms.factors:
                   #print(f,self.twiddle.datatype,self._coreMode)
                   # The file "formats.py" is decribing the format of each radix
                   # and is used to compute the format of the FFT based
                   # on the decomposition of its length.
                   #
                   # Currently (since there is no vector version for fixed point)
                   # this is not taking into account the format change that may
                   # be implied by the vectorization in case it may be different
                   # from the scalar version.
                   formatForSize = config.formats.formats[f][self._coreMode]
                   outputFormat += formatForSize[self.twiddle.datatype]

            vectorizable=0 
            if self.perms.isVectorizable:
               vectorizable = 1 

            print(cfftconfig % (ctype,self.configID,normFactorStr,permsLen,self.perms.permID,
               twiddleLen,self.twiddle.twiddleID,self.perms.nbGroupedFactors,self.perms.nbFactors,
               self.perms.permID,self.perms.permID,outputFormat,vectorizable
               ),file=c)

