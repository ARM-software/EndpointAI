/*
 * Copyright:
 * ----------------------------------------------------------------
 * This confidential and proprietary software may be used only as
 * authorised by a licensing agreement from ARM Limited
 *   (C) COPYRIGHT 2014 ARM Limited
 *       ALL RIGHTS RESERVED
 * The entire notice above must be reproduced on all authorised
 * copies and copies may only be made to the extent permitted
 * by a licensing agreement from ARM Limited.
 * ----------------------------------------------------------------
 * File:     fpga.c
 * Release:  Version 1.0
 * ----------------------------------------------------------------
 */

/*
 * Code implementation file for the fpga functions.
 */

#include "SMM_MPS3.h"                   // MPS3 common header

// Function to delay n*ticks (25MHz = 40nS per tick)
// Used for I2C drivers
void i2c_delay(unsigned int tick)
{
	unsigned int end;
	unsigned int start;

    start = MPS3_FPGAIO->COUNTER;
    end   = start + (tick);

    if(end >= start)  {
        while (MPS3_FPGAIO->COUNTER >= start && MPS3_FPGAIO->COUNTER < end);
    } else {
        while (MPS3_FPGAIO->COUNTER >= start);
        while (MPS3_FPGAIO->COUNTER < end);
    }
}

/* Sleep function to delay n*mS
 * Uses FPGA counter.
 */
void delay_ms(unsigned int msec)
{
	unsigned int end;
	unsigned int start;

    start = MPS3_FPGAIO->COUNTER;
    end   = start + (32 * msec * 1000);

    if(end >= start) {
        while (MPS3_FPGAIO->COUNTER >= start && MPS3_FPGAIO->COUNTER < end);
    } else {
        while (MPS3_FPGAIO->COUNTER >= start);
        while (MPS3_FPGAIO->COUNTER < end);
    }
}

/* Sleep function to delay n*uS
 */
void delay_us(unsigned int usec)
{
	unsigned int end;
	unsigned int start;

    start = MPS3_FPGAIO->COUNTER;
    end   = start + (32 * usec);

    if(end >= start) {
        while (MPS3_FPGAIO->COUNTER >= start && MPS3_FPGAIO->COUNTER < end);
    } else {
        while (MPS3_FPGAIO->COUNTER >= start);
        while (MPS3_FPGAIO->COUNTER < end);
    }
}

