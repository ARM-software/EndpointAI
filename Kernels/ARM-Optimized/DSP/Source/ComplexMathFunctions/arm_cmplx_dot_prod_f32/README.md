# README

![CMSIS](http://www.keil.com/pack/doc/CMSIS/General/html/CMSIS_Logo_Final.png)


[CMSIS-DSP](https://github.com/ARM-software/CMSIS_5/tree/develop/CMSIS/DSP) with ARMv8.1M **Helium** assembly


Single-precision floating-point complex dot-product processing function

This version allows boosting F32 complex dot-product performance when using compilers having suboptimal Helium intrinsic code generation and can be sustitued to CMSIS DSP intrinsic variant.

[see API details](https://www.keil.com/pack/doc/CMSIS/DSP/html/group__cmplx__dot__prod.html)

## BUILD

 - GCC 10.3-2021.10 example

```cpp
 arm-none-eabi-gcc -DUSE_ASM -mcpu=cortex-m55 -O3 -mfloat-abi=hard -c arm_cmplx_dot_prod_f32.c -o arm_cmplx_dot_prod_f32.o -I$(CMSIS5ROOT)/CMSIS/DSP/PrivateInclude/ -I $(CMSIS5ROOT)/CMSIS/DSP/Include/ -I $(CMSIS5ROOT)/CMSIS/Core/Include/
 ```

