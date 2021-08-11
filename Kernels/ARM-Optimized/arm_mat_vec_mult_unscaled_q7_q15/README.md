# Mixed 8-bit matrix by 16-bit vector Multiplication

Result remains unscaled in Q22 and is held in a 32-bit vector.

- This routine uses **CMSIS DSP** data types and structures.
  - https://arm-software.github.io/CMSIS_5/DSP/html/arm__math__types_8h.html  
  - https://arm-software.github.io/CMSIS_5/DSP/html/structarm__matrix__instance__q7.html
- Arm Compiler 6.16 / LLVM are able to autovectorize the plain C routine.
- MVE 5x unrolled Intrinsics / Asm variants are provided to get optimal performance
- **USE_ASM** compile option enables the Asm variant (faster)
- GCC 10.3 and below should use the Asm variant to get optimal performance

## Build

CMSIS_5 paths for DSP/Core have to be provided 

 - Arm Compiler 6.16 and above / LLVM

`armclang -target arm-arm-none-eabi -mthumb -mfloat-abi=hard -std=gnu99 -D__GNUC_ -DUSE_ASM -mcpu=cortex-m55 -Ofast  -ffast-math -I <CMSIS5_PATH>/CMSIS/DSP/Include/ -I <CMSIS5_PATH>/Core/Include/ -S arm_mat_vec_mult_unscaled_q7_q15.c -o - `

 - GCC 10 and above

 `arm-none-eabi-gcc  -Ofast -mfloat-abi=hard -mlittle-endian -mthumb -DUSE_ASM -mcpu=cortex-m55 -flax-vector-conversions  -Ofast -ffast-math -I <CMSIS5_PATH>/CMSIS/DSP/Include/ -I <CMSIS5_PATH>/Core/Include/ -S arm_mat_vec_mult_unscaled_q7_q15.c -o -`

*note*
 - -mcpu=cortex-m55 implies ARM_MATH_MVEI / ARM_MATH_MVEF (see arm_math_types.h)