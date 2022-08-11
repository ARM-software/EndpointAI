# README

![CMSIS](http://www.keil.com/pack/doc/CMSIS/General/html/CMSIS_Logo_Final.png)


[CMSIS-DSP](https://github.com/ARM-software/CMSIS-DSP) with ARMv8.1M **Helium** assembly (relies on [CMSIS-5](https://github.com/ARM-software/CMSIS_5) Core)


12th order IIR filter for single-precision floating-point values



## BUILD

 - GCC 10.3-2021.10 example

```cpp
  arm-none-eabi-gcc -DUSE_ASM -mcpu=cortex-m55 -O3 -mfloat-abi=hard -c arm_iir8th_order_f32.c -o arm_iir8th_order_f32.o -I . -I$(CMSIS_DSP_ROOT)/PrivateInclude/ -I $(CMSIS_DSP_ROOT)/Include/ -I $(CMSIS_5_ROOT)/CMSIS/Core/Include/Include
  arm-none-eabi-gcc -DUSE_ASM -mcpu=cortex-m55 -O3 -mfloat-abi=hard -c arm_iir8th_order_init_f32.c -o arm_iir8th_order_init_f32.o -I . -I$(CMSIS_DSP_ROOT)/PrivateInclude/ -I $(CMSIS_DSP_ROOT)/Include/ -I $(CMSIS_5_ROOT)/CMSIS/Core/Include/Include
 ```

 - Arm Compiler 6.18 example

```cpp
 armclang -target arm-arm-none-eabi -mthumb -std=gnu99  -mcpu=cortex-m55 -Ofast -mfloat-abi=hard -DUSE_ASM -c arm_iir8th_order_f32.c -o arm_iir8th_order_f32.o -I . -I$(CMSIS_DSP_ROOT)/PrivateInclude/ -I $(CMSIS_DSP_ROOT)/Include/ -I $(CMSIS_5_ROOT)/CMSIS/Core/Include/Include
 armclang -target arm-arm-none-eabi -mthumb -std=gnu99  -mcpu=cortex-m55 -Ofast -mfloat-abi=hard -DUSE_ASM -c arm_iir8th_order_init_f32.c -o arm_iir8th_order_init_f32.o -I . -I$(CMSIS_DSP_ROOT)/PrivateInclude/ -I $(CMSIS_DSP_ROOT)/Include/ -I $(CMSIS_5_ROOT)/CMSIS/Core/Include/Include
```

## TESTBENCH

```cpp

#include "arm_iir8th_order_f32.h"

    /* Use matlab(c) to design a 8th order lowpass digital filter, cutoff = 0.3 */

    /*
      [b,a] = butter(8,0.3); 
      a = [
            1.00000000000000, 
            -3.18463950057501, 5.18287545236711, -5.21619517783751, 3.49549635777094, 
            -1.57296427233537, 0.46074179130744, -0.07977993867340, 0.00622565787270, 
          ]

      b = [
           0.00035843894491, 0.00286751155928, 0.01003629045747, 0.02007258091495, 
           0.02509072614368, 0.02007258091495, 0.01003629045747, 0.00286751155928, 
           0.00035843894491
       ]
     */

    /* create C arrays. Numerator fist element = 1.0f is eluded */       
    float32_t       iir8BckCoef_f32[IIR_8_ORDER] = {
        /*  1 */ -3.18463950057501, 5.18287545236711, -5.21619517783751, 3.49549635777094,
                 -1.57296427233537, 0.46074179130744, -0.07977993867340, 0.00622565787270
       };

    float32_t       iir8FwdCoef_f32[IIR_8_ORDER + 1] = {
        0.00035843894491, 0.00286751155928, 0.01003629045747, 0.02007258091495,
        0.02509072614368, 0.02007258091495, 0.01003629045747, 0.00286751155928,
        0.00035843894491,
    };

    
    arm_iir8_instance_f32 iirInstVCTR;

    /* allocate IIR states for Ref and Vector */
    float32_t iirStateFwdVCTR[MAX_BUF_SZ + IIR_8_ORDER] = { 0 };
    float32_t iirStateBkwdVCTR[MAX_BUF_SZ + IIR_8_ORDER] = { 0 };

    /* MVE extra storage for modified coefs */
    float32_t modCoefMve[IIR_8_COEF_BUF_SZ] = { 0 };

    /* input / output storage */
    float32_t Input[MAX_BUF_SZ], *pInput = Input;
    float32_t dstVCTR[MAX_BUF_SZ], *pdstVCTR = dstVCTR;

    /* MVE filter init */
    arm_iir8_init_f32_mve(&iirInstVCTR, iir8FwdCoef_f32,
                         iir8BckCoef_f32, iirStateFwdVCTR, iirStateBkwdVCTR,
                         modCoefMve, blockSize);

    /* generate white noise */
    set_input(RAND, blockSize, pInput, powf(2.0, -28.0), float32_t);

    /* MVE filter run */
    arm_iir8_f32_mve(&iirInstVCTR, pInput, pdstVCTR, blockSize);

```