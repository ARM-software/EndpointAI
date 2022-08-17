# README

![CMSIS](http://www.keil.com/pack/doc/CMSIS/General/html/CMSIS_Logo_Final.png)


[CMSIS-DSP](https://github.com/ARM-software/CMSIS-DSP) with ARMv8.1M **Helium** assembly (relies on [CMSIS-5](https://github.com/ARM-software/CMSIS_5) Core)


 - IIR filter for Q.31 fixed-point values
 - To be used with care for high order filters as precision is decreasing compared to 64-bit float references



## BUILD

 - GCC 10.3-2021.10 example

```cpp
  arm-none-eabi-gcc -DUSE_ASM -mcpu=cortex-m55 -O3 -mfloat-abi=hard -c arm_iir_q31.c -o arm_iir_q31.o -I . -I$(CMSIS_DSP_ROOT)/PrivateInclude/ -I $(CMSIS_DSP_ROOT)/Include/ -I $(CMSIS_5_ROOT)/CMSIS/Core/Include/Include
  arm-none-eabi-gcc -DUSE_ASM -mcpu=cortex-m55 -O3 -mfloat-abi=hard -c arm_iir_init_q31.c -o arm_iir_init_q31.o -I . -I$(CMSIS_DSP_ROOT)/PrivateInclude/ -I $(CMSIS_DSP_ROOT)/Include/ -I $(CMSIS_5_ROOT)/CMSIS/Core/Include/Include
 ```

 - Arm Compiler 6.18 example

```cpp
 armclang -target arm-arm-none-eabi -mthumb -std=gnu99  -mcpu=cortex-m55 -Ofast -mfloat-abi=hard -DUSE_ASM -c arm_iir_q31.c -o arm_iir_q31.o -I . -I$(CMSIS_DSP_ROOT)/PrivateInclude/ -I $(CMSIS_DSP_ROOT)/Include/ -I $(CMSIS_5_ROOT)/CMSIS/Core/Include/Include
 armclang -target arm-arm-none-eabi -mthumb -std=gnu99  -mcpu=cortex-m55 -Ofast -mfloat-abi=hard -DUSE_ASM -c arm_iir_init_q31.c -o arm_iir_init_q31.o -I . -I$(CMSIS_DSP_ROOT)/PrivateInclude/ -I $(CMSIS_DSP_ROOT)/Include/ -I $(CMSIS_5_ROOT)/CMSIS/Core/Include
```

## TESTBENCH

```cpp

#include "arm_iir_q31.h"

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
		max(a) = 5.18287545236711
		max(b) = 0.0250907261436828

		a and b can be expressed in Q3.28, so postshift need to be set to 3
		afx = round(a*2^28)
		bfx = round(b*2^28)
     */

#define IIR_8_ORDER 8
#define IIR_8_SCALE 3

    /* create C arrays. Numerator fist element (= 1 in Q3.28) is eluded */
    q31_t       iir8BckCoef_q31[IIR_8_ORDER] = {
        /*  1 */ -854870157, 1391267535, -1400211731, 938315159,
                  -422239382, 123679433, -21415764, 1671187
    };

    q31_t       iir8FwdCoef_q31[IIR_8_ORDER + 1] = {
            96218, 769742, 2694096, 5388192,
            6735241, 5388192, 2694096, 769742,
            96218
    }


    arm_iir8_instance_q31 iirInstVCTR;

    /* allocate IIR states */
    q31_t iirStateFwdVCTR[MAX_BUF_SZ + IIR_8_ORDER] = { 0 };
    q31_t iirStateBkwdVCTR[MAX_BUF_SZ + IIR_8_ORDER] = { 0 };

    /* MVE extra storage for modified coefs */
    q31_t modCoefMve[IIR_COEF_BUF_SZ_F32(IIR_8_ORDER)] = { 0 };

    /* input / output storage */
    q31_t Input[MAX_BUF_SZ], *pInput = Input;
    q31_t dstVCTR[MAX_BUF_SZ], *pdstVCTR = dstVCTR;

    /* MVE filter init */
    arm_iir_init_q31_mve(&iirInstVCTR, IIR_8_ORDER, iir8FwdCoef_q31,
                         iir8BckCoef_q31, iirStateFwdVCTR, iirStateBkwdVCTR,
                         modCoefMve, blockSize, IIR_8_SCALE);

    /* generate white noise */
    set_input(RAND, blockSize, pInput, powf(2.0, -8.0), q31_t);

    /* MVE filter run */
    arm_iir_q31_mve(&iirInstVCTR, pInput, pdstVCTR, blockSize);

```