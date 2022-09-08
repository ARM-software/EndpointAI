# README

![CMSIS](http://www.keil.com/pack/doc/CMSIS/General/html/CMSIS_Logo_Final.png)


[CMSIS-DSP](https://github.com/ARM-software/CMSIS_5/tree/develop/CMSIS/DSP) with ARMv8.1M **Helium** assembly


Single-precision DF1 Biquad processing function

- This version allows boosting F32 DF1 Biquad performance when using compilers having suboptimal Helium intrinsic code generation and can be sustitued to CMSIS DSP intrinsic variant.
- It also provide efficient Neon Implementations
- Helium and Neon functions have respectively _mve and _neon prefix

[see API details](https://www.keil.com/pack/doc/CMSIS/DSP/html/group__BiquadCascadeDF1.html)

## BUILD

GCC 10.3-2021.10 examples (ASM enabled)
  - Cortex-m55 with MVE

```cpp
 arm-none-eabi-gcc -DUSE_ASM -mcpu=cortex-m55 -O3 -mfloat-abi=hard -flax-vector-conversions -c arm_biquad_cascade_df1_f32.c -o arm_biquad_cascade_df1_f32.o -I$(CMSIS5ROOT)/CMSIS/DSP/PrivateInclude/ -I $(CMSIS5ROOT)/CMSIS/DSP/Include/ -I $(CMSIS5ROOT)/CMSIS/Core/Include/
 ```

   - Cortex-A53 with aarch64

```cpp
 aarch64-none-elf-gcc -DARM_MATH_NEON -DUSE_ASM  -Ofast -I -I$(CMSIS5ROOT)/CMSIS/DSP/PrivateInclude/ -I $(CMSIS5ROOT)/CMSIS/DSP/Include/ -I $(CMSIS5ROOT)/CMSIS/Core/Include -mcpu=cortex-a53   -c arm_biquad_cascade_df1_f32.c
 ```

   - Cortex-A53 with aarch32

```cpp
 arm-none-eabi-gcc -DARM_MATH_NEON -DUSE_ASM  -Ofast -I$(CMSIS5ROOT)/CMSIS/DSP/PrivateInclude/ -I $(CMSIS5ROOT)/CMSIS/DSP/Include/ -I $(CMSIS5ROOT)/CMSIS/Core/Include -mcpu=cortex-a53 -mfloat-abi=hard  -c arm_biquad_cascade_df1_f32.c
 ```

Arm Compiler examples (Intrinsics)

   - Cortex-A53 with aarch64

```cpp
armclang -DARM_MATH_NEON  -g -Ofast -I -I$(CMSIS5ROOT)/CMSIS/DSP/PrivateInclude/ -I $(CMSIS5ROOT)/CMSIS/DSP/Include/ -I $(CMSIS5ROOT)/CMSIS/Core/Include --target=aarch64-arm-none-eabi  -c arm_biquad_cascade_df1_f32.c
 ```

   - Cortex-A53 with aarch32

```cpp
armclang -DARM_MATH_NEON  -g -Ofast -I -I$(CMSIS5ROOT)/CMSIS/DSP/PrivateInclude/ -I $(CMSIS5ROOT)/CMSIS/DSP/Include/ -I $(CMSIS5ROOT)/CMSIS/Core/Include --target=arm-arm-none-eabi  -c arm_biquad_cascade_df1_f32.c
 ```


