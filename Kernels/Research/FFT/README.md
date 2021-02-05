# README

__It is a work in progress__. 

See the [limitation](#limitations) section

## INTRODUCTION

The goal of this project is to provide a FFT framework  more flexible than
the [CMSIS-DSP](https://github.com/ARM-software/CMSIS_5/tree/develop/CMSIS/DSP) one but with similar features and performances:

   * Several transforms : Complex FFT, Real FFT, Complex FFT 2D ;
   * More radixes : 2,3,4,5,6,8 (enabled depending on the core and FFT length) ;
   * Several datatypes : f64, f32, f16, q31, q15
   * More lengths
   * Configuration scripts

The new features compared to the [CMSIS-DSP](https://github.com/ARM-software/CMSIS_5/tree/develop/CMSIS/DSP) are the support of more radixes, the FFT 2D and the configuration scripts.

## FOLDER HIERARCHY

### Folders 
 * `config` : Python code for the configuration
 * `Generated` : Source and headers generated for a configuration
 * `Example` : Example and Makefile to build
 * `Source` : The source for the FFT
 * `Include` , `TestBench` , `Patterns` , `Parameters` : Tests using [CMSIS-DSP](https://github.com/ARM-software/CMSIS_5/tree/develop/CMSIS/DSP) test framework.

### Files in root folder:

 * `configgen.py` : Python script for configuration
 * `FFT.py` : Python script to regenerate the patterns
 * `config.yaml` : Configuration file
 * `CMakeLists.txt` : To build with the [CMSIS-DSP](https://github.com/ARM-software/CMSIS_5/tree/develop/CMSIS/DSP) test framework.
 * `fft_bench.txt` and `fft_test.txt` : Configurations for the [CMSIS-DSP](https://github.com/ARM-software/CMSIS_5/tree/develop/CMSIS/DSP) test framework.

## CONFIGURATION 

A FFT of a given length and datatype is using a twiddle table and a permutation table.
In this framework, a configuration script is provided to generate only the tables required for the FFTs the user needs.

It can be run with:

  > python3 configgen.py -d Generated config.yaml 

The headers and source files will be generated in folder "Generated".

The configuration is described in config.yaml.

The syntax of this configuration is easy. Here is an example:

```yaml
M55NOMVE:
  CFFT:
    f32: [128,256]
    f16: [128,256]
    q31: [128,256]
    q15: [128,256]
  RFFT:
    f32: [128,256]
    f16: [128,256]
    q31: [128,256]
    q15: [128,256]
# [rows, columns]
  CFFT2D:
    f32 : [[4,9],[8,4],[64,4]]
    f16 : [[4,9],[8,4],[64,4]]
    q31 : [[4,9],[8,4],[64,4]]
    q15 : [[4,9],[8,4],[64,4]]
```

First, the core is listed. The supported cores are currently:

 * M0, M4, M7, M33, M55NOMVE, M55, A32

The M55NOMVE is a scalar configuration with a FPU.

Then, the transforms are listed (CFFT, RFFT, CFFT2D).

For each transform, the datatypes and the lengths.

The length of a RFFT must be __even__. The script will not check it.

If you wanted only a CFFT of length 256 for f32, the configuration would just be:

```yaml
M55NOMVE:
  CFFT:
    f32: [256]
```

The script will generate:

  * The twiddle tables
  * The permutation tables to reorder the output of the transform
  * The initialization code (structures and init functions)

Using the fft in a file is then easy:

```cpp
#include "fft.h"

#define NB 256
arm_status test()
{
    arm_status status;
    status=cfft(input,output,NB);
    return(status);
}
```

Contrary to the [CMSIS-DSP](https://github.com/ARM-software/CMSIS_5/tree/develop/CMSIS/DSP), this FFT will not modify its input buffer.

But, it can also be used in place : so the output and input buffers can be the same.

The API of cfft is:

```cpp
extern template arm_status cfft<float32_t>(const float32_t *in,float32_t* out, uint16_t N);
```

C++ templates are used a lot to avoid code duplication and to allow to more easily
customize the implementation.

## BUILDING

Once the FFT has been configured and all needed headers and source files generated, building is easy.

An example `Makefile` is available into the Example folder.

There are 4 source files:

  * `Source/fft.cpp`
  * `Generated/config.cpp`
  * `Generated/perm.cpp` 
  * `Generated/twiddle.cpp` 

And only a few include directories.

Include directories from the FFT:

  * `Generated`
  * `Source`
  * `Source/Kernels​`	

Include directories from [CMSIS](https://github.com/ARM-software/CMSIS_5)

  * `$(CORE)/Include`
  * `$(DSP)/Include` 

Where `$(CORE)` is the CMSIS Core or Core_A folder and `$(DSP)` the DSP folder.

## LIMITATIONS

__It is a work in progress__. 

If you want an efficient Helium FFT, you should continue to use 
the [CMSIS-DSP](https://github.com/ARM-software/CMSIS_5/tree/develop/CMSIS/DSP) implementation.

If you want an efficient Neon FFT, you should continue to use the [Ne10](https://github.com/projectNe10/Ne10) implementation.

__The vectorial implementation of this FFT framework is still far from having good performances__.

For the scalar:

 * Scalar float32 has good peformances 
 * Scalar float64 and float16 have not yet been benchmarked
 * Q31 has worse performance than the [CMSIS-DSP](https://github.com/ARM-software/CMSIS_5/tree/develop/CMSIS/DSP)  version but is implementing more complex kernels for more accuracy and it is not really needed. So a future version will get closer to [CMSIS-DSP](https://github.com/ARM-software/CMSIS_5/tree/develop/CMSIS/DSP) accuracy and performance 
 * Q15 is a bit better than [CMSIS-DSP](https://github.com/ARM-software/CMSIS_5/tree/develop/CMSIS/DSP).

 The measurements have been done with ARM clang 6.15.

 No tests have been done with gcc (not even trying to build).

 ## CUSTOMIZATIONS

 ### Python

 The file `codegen/core.py` is defining the supported cores.

 It is easy to add new ones. Create a class inheriting from FFT and customize it to define:

   * The scalar mode : Pure C or with DSP extensions
   * Are vector instruction supported
   * Define the vector extension (Helium or Neon)
   * Define the list of radixes depending on the datatype and FFT length

Fixed point arithmetic has not yet any vector version.

Then, the file configgen.py, in the root folder, must be modified:

  * Add the new core to the KNOWNCORES list 
  * Modify the main loop just a few line below to create the core object

 ### C++ Templates

 `stageloop.h` is containing the templates to compute the stages.
 There are different cases for the first and last stage.

 Since we pack all iterations of the same radix in a loop, several stages are in fact computed. So, the different cases depends on the presence of the first and/or last stage in this loop.

 Then, each stage is computed by the templates in `vector_grouploop.h` and `vector_specialgrouploop.h`.

 Those loops are loading the twiddle and calling the radix kernels.

 The `vector_specialgrouploop.h` is containing a specialized implementation for radix 2, first stage.

Customization is possible by writing specializations of those templates for specific values of the parameters.

## CODE GENERATION

The kernels for the radixes have been generated with a code generator developed in Haskell that we have created for this.

It won't be made public because it is too complex and really not clean.

It is reusing some ideas from FFTW but it is a different and less sophisticated implementation. But it is supporting fixed point arithmetic and the vectorization strategy used in those FFTs.








