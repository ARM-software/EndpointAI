/*-----------------------------------------------------------------------------
 * Name:    LED.h
 * Purpose: Low level LED definitions
 *-----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2013 KEIL - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#ifndef __LED_H
#define __LED_H

#include <stdint.h>

#define FPGA_LEDS 0
#define MCC_LEDS  1

#define NUM_FPGA_LEDS  8
#define NUM_FPGA_LEDS_MASK 0xFF
#define NUM_MCC_LEDS   8
#define NUM_MCC_LEDS_MASK 0xFF

extern void     LED_Initialize   (uint32_t port);
extern void     LED_Uninitialize (void);
extern void     LED_On           (uint32_t num, uint32_t port);
extern void     LED_Off          (uint32_t num, uint32_t port);
extern void     LED_Out          (uint32_t val, uint32_t port);
extern uint32_t LED_NumLeds      (uint32_t port);

#endif /* __LED_H */
