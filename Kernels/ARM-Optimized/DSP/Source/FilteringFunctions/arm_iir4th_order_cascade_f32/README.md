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
#include "arm_iir4th_order_cascade_f32.h"

    /*
     * Use matlab(c) to design a cascade of 3 x 4th order Butterworth Low Pass filters
     *
     * [b3,a3] = butter(4,0.3);
     * [b2,a2] = butter(4,0.2);
     * [b1,a1] = butter(4,0.1);
     * H1=dfilt.df2t(b1,a1);
     * H2=dfilt.df2t(b2,a2);
     * H3=dfilt.df2t(b3,a3);
     * Hcas=dfilt.cascade(H1,H2,H3)
     */

   #define IIR4_NB_CASCADES   3

    /* IIR4 coefficients arrays 
       @par           Coefficient Ordering
       Similartly to DF1 F32 Biquads, the coefficients are stored in the following order:
       <pre>
       {b00, b01, b02, b03, b04, a01, a02, a03, a04, b10, b11, b12, b13, b14, a11, a12, a13, a14 ...}
       </pre>
     */

    struct arm_iir4_coef {
        float32_t       num[IIR_4_ORDER + 1];
        float32_t       den[IIR_4_ORDER];
    };

    struct arm_iir4_coef iirCoeffs_f32[IIR4_NB_CASCADES] = {
        /* butter(4,0.3); */
        [0] = {
               .num = {
                       0.0185630106268972f, 0.0742520425075889f, 0.111378063761383f,
                       0.0742520425075889f, 0.0185630106268972f},
               .den = {
                       /* 1 */ -1.57039885122817f, 1.27561332498328f, -0.484403368335085f,
                       0.0761970646103324f}
               },
        /* butter(4,0.2); */
        [1] = {
               .num = {
                       0.00482434335771623f, 0.0192973734308649f, 0.0289460601462974f,
                       0.0192973734308649f, 0.00482434335771623f},
               .den = {
                       /* 1 */ -2.36951300718204f, 2.31398841441588f, -1.05466540587857f,
                       0.187379492368185f}
               },
        /* butter(4,0.1); */
        [2] = {
               .num = {
                       0.000416599204406579f, 0.00166639681762631f, 0.00249959522643947f,
                       0.00166639681762631f, 0.000416599204406579f},
               /* butter(4,0.1); */
               .den = {
                       /*1 */ -3.18063854887472f, 3.86119434899422f, -2.11215535511097f,
                       0.438265142261981f}
               }
    };

    /* init. IIR 4 cascade */

    /* allocate cascaded IIR instances */
    arm_iir4_casd_inst_f32 iirCasdVCTR;

    /* allocate IIR states */
    /* MVE context requires extra storage to hold matrix converted form */
    float32_t iir4CascStateMve[IIR4_NB_CASCADES * IIR4_F32_CASC_MVE_CONTEXT_SZ];

    /* input / output storage */
    float32_t Input[MAX_BUF_SZ], *pInput = Input;
    float32_t dstVCTR[MAX_BUF_SZ], *pdstVCTR = dstVCTR;

    /*
     * init cascade IIR instances
     */
    arm_iir4_cascade_init_f32_mve(&iirCasdVCTR, IIR4_NB_CASCADES,
                                  (float32_t *) iirCoeffs_f32, iir4CascStateMve);



    /* generate white noise input */
    set_input(RAND, blockSize, pInput, powf(2.0, -28.0), float32_t);

    
    /* MVE filter run */
    arm_iir4_cascade_f32_mve(&iirCasdVCTR, pInput, pdstVCTR, blockSize);
    

```