# README

![CMSIS](http://www.keil.com/pack/doc/CMSIS/General/html/CMSIS_Logo_Final.png)


[CMSIS-DSP](https://github.com/ARM-software/CMSIS-DSP) with ARMv8.1M **Helium** assembly (relies on [CMSIS-5](https://github.com/ARM-software/CMSIS_5) Core)


Csscade of 4th order IIR filters for single-precision floating-point values



## BUILD

 - GCC 10.3-2021.10 example

```cpp
  arm-none-eabi-gcc -DUSE_ASM -mcpu=cortex-m55 -O3 -mfloat-abi=hard -c arm_iir4th_order_cascade_f32.c -o arm_iir4th_order_cascade_f32.o -I . -I$(CMSIS_DSP_ROOT)/PrivateInclude/ -I $(CMSIS_DSP_ROOT)/Include/ -I $(CMSIS_5_ROOT)/CMSIS/Core/Include/Include
  arm-none-eabi-gcc -DUSE_ASM -mcpu=cortex-m55 -O3 -mfloat-abi=hard -c arm_iir4th_order_cascade_init_f32.c -o arm_iir4th_order_cascade_init_f32.o -I . -I$(CMSIS_DSP_ROOT)/PrivateInclude/ -I $(CMSIS_DSP_ROOT)/Include/ -I $(CMSIS_5_ROOT)/CMSIS/Core/Include/Include
 ```

 - Arm Compiler 6.18 example

```cpp
 armclang -target arm-arm-none-eabi -mthumb -std=gnu99  -mcpu=cortex-m55 -Ofast -mfloat-abi=hard -DUSE_ASM -c arm_iir4th_order_cascade_f32.c -o arm_iir4th_order_cascade_f32.o -I . -I$(CMSIS_DSP_ROOT)/PrivateInclude/ -I $(CMSIS_DSP_ROOT)/Include/ -I $(CMSIS_5_ROOT)/CMSIS/Core/Include/Include
 armclang -target arm-arm-none-eabi -mthumb -std=gnu99  -mcpu=cortex-m55 -Ofast -mfloat-abi=hard -DUSE_ASM -c arm_iir4th_order_cascade_init_f32.c -o arm_iir4th_order_cascade_init_f32.o -I . -I$(CMSIS_DSP_ROOT)/PrivateInclude/ -I $(CMSIS_DSP_ROOT)/Include/ -I $(CMSIS_5_ROOT)/CMSIS/Core/Include/Include
```

## TESTBENCH

```cpp
#include "arm_iir12th_order_f32.h"

    /* Use matlab(c) to design a 12th order lowpass digital filter, cutoff = 0.3 */

    /*
      [b,a] = butter(12,0.3); 
      a = [
        1.00000000000000, 
        -4.78968579333577, 11.64546405834363, -18.34602138357789, 20.56366202043100, 
        -17.11188912134385, 10.77028655928594, -5.14107049712862, 1.84024534425086, 
        -0.48028403826895, 0.08654072168022, -0.00964629980804, 0.00050214181632
        ]
      b = [
           0.00000686125790, 0.00008233509476, 0.00045284302118, 0.00150947673727, 
           0.00339632265886, 0.00543411625418, 0.00633980229654, 0.00543411625418, 
           0.00339632265886, 0.00150947673727, 0.00045284302118, 0.00008233509476, 
           0.00000686125790, 
       ]
     */

    /* create C arrays. Numerator fist element = 1.0f is eluded */       
    float32_t       iir12BckCoef_f32[IIR_12_ORDER] = {
        /*  1 */ -4.78968579333577, 11.6454640583436, -18.3460213835779,
        20.563662020431, -17.1118891213439, 10.7702865592859, -5.14107049712862,
        1.84024534425086, -0.48028403826895, 0.0865407216802232, -0.00964629980804202,
        0.000502141816315784
    };

    float32_t       iir12FwdCoef_f32[IIR_12_ORDER + 1] = {
        6.86125789668875e-06, 8.2335094760265e-05, 0.000452843021181457, 0.00150947673727152,
        0.00339632265886093, 0.00543411625417749, 0.0063398022965404, 0.00543411625417749,
        0.00339632265886093, 0.00150947673727152, 0.000452843021181457, 8.2335094760265e-05,
        6.86125789668875e-06
    };

    
    arm_iir12_instance_f32 iirInstVCTR;

    /* allocate IIR states for Ref and Vector */
    float32_t iirStateFwdVCTR[MAX_BUF_SZ + IIR_12_ORDER] = { 0 };
    float32_t iirStateBkwdVCTR[MAX_BUF_SZ + IIR_12_ORDER] = { 0 };

    /* MVE extra storage for modified coefs */
    float32_t modCoefMve[IIR_12_COEF_BUF_SZ] = { 0 };

    /* input / output storage */
    float32_t Input[MAX_BUF_SZ], *pInput = Input;
    float32_t dstVCTR[MAX_BUF_SZ + EXTRA_TAIL_CHK], *pdstVCTR = dstVCTR;

    /* MVE filter init */
    arm_iir12_init_f32_mve(&iirInstVCTR, iir12FwdCoef_f32,
                         iir12BckCoef_f32, iirStateFwdVCTR, iirStateBkwdVCTR,
                         modCoefMve, blockSize);


    /* generate white noise */
    set_input(RAND, blockSize, pInput, powf(2.0, -28.0), float32_t);

    /* MVE filter run */
    arm_iir12_f32_mve(&iirInstVCTR, pInput, pdstVCTR, blockSize);

```