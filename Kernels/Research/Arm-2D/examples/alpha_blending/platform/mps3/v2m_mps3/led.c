/*-----------------------------------------------------------------------------
 * Name:    LED.c
 * Purpose: Low level LED functions
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

#include "SMM_MPS3.h"                   // MPS3 common header
#include "led.h"                        // led driver

/*----------------------------------------------------------------------------
  Initialize LED pins
 *----------------------------------------------------------------------------*/
void LED_Initialize (uint32_t port) {
  uint32_t n;
  uint32_t num_leds;
  if (port == FPGA_LEDS)
      num_leds = NUM_FPGA_LEDS;
  else
      num_leds = NUM_MCC_LEDS;

  for (n = 0; n < num_leds; n++) {
    LED_Off (n, port);
  }
}


/*----------------------------------------------------------------------------
  Uninitialize LED pins
 *----------------------------------------------------------------------------*/
void LED_Uninitialize (void) {

}

/*-----------------------------------------------------------------------------
  Turns on requested LED
    num - LED number
 *----------------------------------------------------------------------------*/
void LED_On (uint32_t num, uint32_t port) {

  if (port == FPGA_LEDS){
    if (num < NUM_FPGA_LEDS) {
      MPS3_FPGAIO->LED |=  (1ul << num);
    }
  }
  else {
    if (num < NUM_MCC_LEDS) {
      MPS3_SCC->LEDS |=  (1ul << num);
    }
  }
}

/*-----------------------------------------------------------------------------
  Turns off requested LED
    num - LED number
 *----------------------------------------------------------------------------*/
void LED_Off (uint32_t num, uint32_t port) {

  if (port == FPGA_LEDS){
    if (num < NUM_FPGA_LEDS) {
      MPS3_FPGAIO->LED &= ~(1ul << num);
    }
  }
  else {
    if (num < NUM_MCC_LEDS) {
      MPS3_SCC->LEDS &= ~(1ul << num);
    }
  }
}

/*-----------------------------------------------------------------------------
  Write value to LEDs
    val - value to be displayed on LEDs
 *----------------------------------------------------------------------------*/
void LED_Out (uint32_t val, uint32_t port) {
  if (port == FPGA_LEDS)
    MPS3_FPGAIO->LED  = val & NUM_FPGA_LEDS_MASK;
  else
    MPS3_SCC->LEDS = val & NUM_MCC_LEDS_MASK;
}

/*-----------------------------------------------------------------------------
  Get number of available LEDs
 *----------------------------------------------------------------------------*/
uint32_t LED_NumLeds (uint32_t port) {
  if (port == FPGA_LEDS)
    return (NUM_FPGA_LEDS);
  else
      return (NUM_MCC_LEDS);
}
