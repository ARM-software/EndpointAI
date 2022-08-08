# README

![CMSIS](http://www.keil.com/pack/doc/CMSIS/General/html/CMSIS_Logo_Final.png)


[CMSIS-DSP](https://github.com/ARM-software/CMSIS-DSP) with ARMv8.1M **Helium** assembly (relies on [CMSIS-5](https://github.com/ARM-software/CMSIS_5) Core)


Fast vectorized cosine calculation for Q.31 fixed-point values



## BUILD

 - GCC 10.3-2021.10 example

```cpp
 arm-none-eabi-gcc -DUSE_ASM -mcpu=cortex-m55 -O3 -mfloat-abi=hard -c arm_vcos_q31.c -o arm_vcos_q31.o -I .. -I$(CMSIS_DSP_ROOT)/PrivateInclude/ -I $(CMSIS_DSP_ROOT)/Include/ -I $(CMSIS_5_ROOT)/CMSIS/Core/Include/
 ```

