# Mixed 8-bit matrix by 16-bit vector Multiplication

Result remains unscaled in Q22 and is held in a 32-bit vector.

- This routine uses **CMSIS DSP** data types and structures.
- Arm Compiler 6.16 / LLVM are able to autovectorize the plain C routine.
- MVE 5x unrolled Intrinsics / Asm variants are provided to get optimal performance

## Build

 - Arm Compiler 6.16 and above / LLVM

`armclang -target arm-arm-none-eabi -mthumb -mfloat-abi=hard -std=gnu99 -D__GNUC_ -DUSE_ASM -mcpu=cortex-m55 -Ofast  -ffast-math -I <CMSIS5_PATH>/CMSIS/DSP/Include/ -I <CMSIS5_PATH>/Core/Include/ -S arm_mat_vec_mult_unscaled_q7_q15.c -o - `

 - GCC 10 and above

 `arm-none-eabi-gcc  -Ofast -mfloat-abi=hard -mlittle-endian -mthumb -mcpu=cortex-m55 -flax-vector-conversions  -Ofast -ffast-math -I <CMSIS5_PATH>/CMSIS/DSP/Include/ -I <CMSIS5_PATH>/Core/Include/ -S arm_mat_vec_mult_unscaled_q7_q15.c -o -`

