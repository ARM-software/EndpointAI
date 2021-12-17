# README

![CMSIS](http://www.keil.com/pack/doc/CMSIS/General/html/CMSIS_Logo_Final.png)


[CMSIS-DSP](https://github.com/ARM-software/CMSIS_5/tree/develop/CMSIS/DSP) with ARMv8.1M **Helium** assembly


Half-precision DF1 Biquad processing function

This version allows boosting F16 DF1 Biquad performance when using compilers having suboptimal Helium intrinsic code generation and can be sustitued to CMSIS DSP intrinsic variant.

[see API details](https://www.keil.com/pack/doc/CMSIS/DSP/html/group__BiquadCascadeDF1.html)

## BUILD

 - GCC 10.3-2021.10 example

```cpp
 arm-none-eabi-gcc -DUSE_ASM -mcpu=cortex-m55 -O3 -mfloat-abi=hard -flax-vector-conversions -c arm_biquad_cascade_df1_f16.c -o arm_biquad_cascade_df1_f16.o -I$(CMSIS5ROOT)/CMSIS/DSP/PrivateInclude/ -I $(CMSIS5ROOT)/CMSIS/DSP/Include/ -I $(CMSIS5ROOT)/CMSIS/Core/Include/
 ```

