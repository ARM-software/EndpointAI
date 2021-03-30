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
 * File:     aaci_i2c_mps3.h
 * Release:  Version 1.0
 * ----------------------------------------------------------------
 */

/*
 * Header file for the AACI (Advanced Audio CODEC) interface.
 */

#ifndef _AACI_I2C_H_
#define _AACI_I2C_H_

// AACI I2C controller I2C address
#define AAIC_I2C_ADDR          0x96

// AACI I2C controller register addresses
#define AAIC_I2C_CRID          0x01
#define AAIC_I2C_CRPWRC1       0x02
#define AAIC_I2C_CRPWRC2       0x03
#define AAIC_I2C_CRPWRC3       0x04
#define AAIC_I2C_CLKCRTL       0x05
#define AAIC_I2C_INTFCRTL1     0x06
#define AAIC_I2C_INTFCRTL2     0x07
#define AAIC_I2C_INPUTASEL     0x08
#define AAIC_I2C_INPUTBSEL     0x09



// External function types for the Audio I2C
unsigned char AAIC_I2C_read (unsigned char reg_addr, unsigned char sadr);
void          AAIC_I2C_write(unsigned char reg_addr, unsigned char data_byte, unsigned char sadr);

#endif
