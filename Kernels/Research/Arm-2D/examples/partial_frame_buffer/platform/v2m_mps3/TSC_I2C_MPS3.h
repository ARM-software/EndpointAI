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
 * File:     tsc_i2c_mps3.h
 * Release:  Version 1.0
 * ----------------------------------------------------------------
 */

/*
 *            Touch Screen Controller Support
 *            ===============================
 */

#ifndef _TSC_I2C_H_
#define _TSC_I2C_H_

// TSC I2C controller
#define TSC_I2C_ADDR          0x82
#define TSC_I2C_CID           0x0811

// TSC I2C controller registers
#define TSC_I2C_CRID          0x00


// TSSPCPSR Clock prescale register
#define TSSPCPSR_DFLT      0x0002      // Clock prescale (use with SCR)

// TSC defaults
#define TSC_XOFF           20          // X offset
#define TSC_YOFF           20          // Y offset
#define TSC_MAXVAL         37000       // 0x0FFF * 10 with TSC to LCD scaling

// External function types


// External function types for the TSC I2C
unsigned int TSC_I2C_read (unsigned char reg_addr, unsigned char sadr, unsigned char bytes);
void         TSC_I2C_write(unsigned char reg_addr, unsigned char data_byte, unsigned char sadr);
void         TSC_I2C_clear(void);

#endif
