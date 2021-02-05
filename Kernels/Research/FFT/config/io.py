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

import os.path
from config.strtools import *

modeheader="""#ifndef %s
#define %s


"""


modefooter="""
#endif /* #ifdef %s */
"""

hheader="""#ifndef %s
#define %s




"""

hfooter="""


#endif /* #ifdef %s */
"""

cheader="""
#include "%s.h"
"""





cfftconfigFuncStart="""
template<>
const cfftconfig<%s> *cfft_config<%s>(uint32_t nbSamples)
{
    switch(nbSamples)
    {""" 

rfftconfigFuncStart="""
template<>
const rfftconfig<%s> *rfft_config<%s>(uint32_t nbSamples)
{
    switch(nbSamples)
    {""" 

fftconfigFuncStop="""
    }
    return(NULL);
}
"""

fftCase="""        case %d:
            return(&config%d);
        break;"""

#def writeConfig(c,ffts):
#    print(fftconfigFuncStart, file=c)
#    for fft in ffts:
#      print(fftCase % (fft.nbSamples,fft.configid), file=c)
#
#    print(fftconfigFuncStop, file=c)
    

#def writeCode(args,ffts):
#    ### Init FFTs
#    for f in ffts:
#        f.initFFT()
#
#    #### Twiddles
#    headerpath=os.path.join(args.d,args.twid) + ".h"
#    cpath=os.path.join(args.d,args.twid) + ".cpp"
#    with open(headerpath,"w") as h:
#        print(hheader % ("TWIDDLE_H","TWIDDLE_H"),file=h)
#        for fft in ffts:
#            fft.writeTwidHeader(h)
#        print(hfooter % ("TWIDDLE_H"),file=h)
#    
#    with open(cpath,"w") as c:
#        print(cheader % args.twid,file=c)
#        for fft in ffts:
#            fft.writeTwidCode(c)
#
#    #### PERM
#    headerpath=os.path.join(args.d,args.perm) + ".h"
#    cpath=os.path.join(args.d,args.perm) + ".cpp"
#    with open(headerpath,"w") as h:
#        print(hheader % ("PERM_H","PERM_H"),file=h)
#        for fft in ffts:
#            fft.writePermHeader(h)
#        print(hfooter % ("PERM_H"),file=h)
#    
#    with open(cpath,"w") as c:
#        print(cheader % args.perm,file=c)
#        for fft in ffts:
#            fft.writePermCode(c)
#
#    ### CONFIGURATIONS
#    headerpath=os.path.join(args.d,args.config) + ".h"
#    cpath=os.path.join(args.d,args.config) + ".cpp"
#    with open(headerpath,"w") as h:
#        print(hheader % ("CONFIG_H","CONFIG_H"),file=h)
#    
#        print("extern template const fftconfig<float32_t> *fft_config(uint32_t nbSamples);",file=h)
#        radixUsed = set([])
#        for fft in ffts:
#            fft.writeConfigHeader(h)
#            radixUsed |= fft.usedFactors
#
#        for radix in radixUsed:
#            print("#define RADIX%d\n" % radix,file=h)
#        print(hfooter % ("CONFIG_H"),file=h)
#
#
#    with open(cpath,"w") as h:
#        #print(cheader %  args.config,file=h)
#        print("#include \"fft_types.h\"",file=h)
#        print("#include \"perm.h\"",file=h)
#        print("#include \"twiddle.h\"\n",file=h)
#
#        for fft in ffts:
#            fft.writeConfigCode(h)
#
#        print("",file=h)
#        writeConfig(h,ffts)

    
