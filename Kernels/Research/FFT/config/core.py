# Description of the configuration for a core
# The radix to use
# The scalar implementation for a given datatype (C or DSP)
# The vector implementation for a given type
# Note that a radix (let's take 4 as example) can be implemented
# as a normal FFT or length 4 and so the output is in the right order.
# Or it could be implemented as 2 FFT and the output would be permuted.
# It can be controlled with radixPerms.
# If radixPerms[4] was  [2,2] then the computation of the permutation would use
# 2,2 as factors but 4 would be used as radix for the FFT computation.
# NOTE : It is not currently implemented.
class FFT:
    def __init__(self):
        self._radix = [2]
        self._radixPerms = {}

    def getPermFactor(self,x):
        if x in self._radixPerms:
           return(self._radixPerms[x])
        else:
           return([x])

    # Scalar mode : Pure C or with DSP extensions
    def mode(self,datatype):
        return("C")

    # Has vector
    def hasVector(self,datatype):
        return(False)

    # Vector architecture
    def vectorArchi(self,datatype):
        return("None")
    
    # Radix supported for a given datatype and length
    # Only 2,3,4,5,6,8 are supported.
    # They should be in decreasing order.
    def radix(self,datatype,nb):
        if datatype == "q15":
           return([5,4,3,2])
        if datatype == "q31":
           return([5,4,3,2])
        return([5,4,3,2])


class M0(FFT):
    def __init__(self):
        FFT.__init__(self)

    def radix(self,datatype,nb):
        if datatype == "q15":
           return([5,4,3,2])
        if datatype == "q31":
           return([5,4,3,2])
        # For float, we have no vectorization and we use a radix 8
        # for better performance.
        return([8,5,4,3,2])


class M4(FFT):
    def __init__(self):
        FFT.__init__(self)
       
    def mode(self,datatype):
        if datatype == "q15":
            return("DSP")
        return("C")

    def radix(self,datatype,nb):
        if datatype == "q15":
           return([5,4,3,2])
        if datatype == "q31":
           return([5,4,3,2])
        
        if nb == 512:
           return([8,5,4,3,2])
        else:
           return([5,4,3,2])

class M7(M4):
    def __init__(self):
        M4.__init__(self)

    def radix(self,datatype,nb):
        if datatype == "q15":
           return([5,4,3,2])
        if datatype == "q31":
           return([5,4,3,2])
        
        return([8,5,4,3,2])
        
       
   
class M33(M4):
    def __init__(self):
        M4.__init__(self)

    def radix(self,datatype,nb):
        if datatype == "q15":
           return([5,4,3,2])
        if datatype == "q31":
           return([5,4,3,2])
        
        if nb > 512:
           return([5,4,3,2])
        else:
           return([8,5,4,3,2])

class M55NOMVE(M4):
    def __init__(self):
        M4.__init__(self)

    def radix(self,datatype,nb):
        # Radix 6 not accurate with q15 and q31.
        if datatype == "q15":
           return([5,4,3,2])
        if datatype == "q31":
           return([5,4,3,2])
        if nb > 512:
           return([5,4,3,2])
        else:
           return([8,5,4,3,2])
       
class M55(M55NOMVE):
    def __init__(self):
        M55NOMVE.__init__(self)
       
    # Vector version for floar.
    # Scalar for other datatypes
    def hasVector(self,datatype):
        if datatype == "f32":
           return(True)
        if datatype == "f16":
           return(True)
        return(False)

    # Helium extensions
    def vectorArchi(self,datatype):
        return("Helium")

    def radix(self,datatype,nb):
        # Radix 6 not accurate with q15 and q31.
        if datatype == "q15":
           return([5,4,3,2])
        if datatype == "q31":
           return([5,4,3,2])
        # Short radixes for vectorization (register pressure)
        return([5,4,3,2])
        

class A32(FFT):
    def __init__(self):
        FFT.__init__(self)
       
    def mode(self,datatype):
        if datatype == "q15":
            return("DSP")
        return("C")

    def hasVector(self,datatype):
        if datatype == "f32":
           return(True)
        return(False)

    def vectorArchi(self,datatype):
        return("Neon")


    def radix(self,datatype,nb):
        # Radix 6 not accurate with q15 and q31.
        if datatype == "q15":
           return([5,4,3,2])
        if datatype == "q31":
           return([5,4,3,2])
        ## Short radixes for vectorization
        return([5,4,3,2])


class R8(M4):
    def __init__(self):
        M4.__init__(self)

    def radix(self,datatype,nb):
        if datatype == "q15":
           return([5,4,3,2])
        if datatype == "q31":
           return([5,4,3,2])

        if nb <= 256:
           return([5,4,3,2])
        else:
           return([8,5,4,3,2])

class R52(A32):
    def __init__(self):
        A32.__init__(self)

    def radix(self,datatype,nb):
        if datatype == "q15":
           return([5,4,3,2])
        if datatype == "q31":
           return([5,4,3,2])

        return([8,5,4,3,2])

