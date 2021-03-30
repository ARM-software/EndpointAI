/*
 * Copyright:
 * ----------------------------------------------------------------
 * This confidential and proprietary software may be used only as
 * authorised by a licensing agreement from ARM Limited
 *   (C) COPYRIGHT 2013 ARM Limited
 *       ALL RIGHTS RESERVED
 * The entire notice above must be reproduced on all authorised
 * copies and copies may only be made to the extent permitted
 * by a licensing agreement from ARM Limited.
 * ----------------------------------------------------------------
 * File:     tsc_i2c_mps3.c
 * Release:  Version 1.0
 * ----------------------------------------------------------------
 */

/*
 * Code implementation file for the Touch Screen I2C interface.
 */

#include "SMM_MPS3.h"
#include "TSC_I2C_MPS3.h"

#define TSC_TSU            17           // Setup delay 600nS min

// Write 8 bits of data to the serial bus
void TSC_I2C_send_byte(unsigned char c)
{
    int loop;

    MPS3_TS_I2C->CONTROLC = SCL;
    i2c_delay(TSC_TSU);

    for (loop = 0; loop < 8; loop++)
    {
        if (c & (1 << (7 - loop)))
            MPS3_TS_I2C->CONTROLS = SDA;
        else
            MPS3_TS_I2C->CONTROLC = SDA;
        i2c_delay(TSC_TSU);
        MPS3_TS_I2C->CONTROLS = SCL;
        i2c_delay(TSC_TSU);
        MPS3_TS_I2C->CONTROLC = SCL;
        i2c_delay(TSC_TSU);
    }

    MPS3_TS_I2C->CONTROLS = SDA;
    i2c_delay(TSC_TSU);
}

// Read 8 bits of data from the serial bus
unsigned char TSC_I2C_receive_byte(void)
{
    int data, loop;

    MPS3_TS_I2C->CONTROLS = SDA;
	i2c_delay(TSC_TSU);

	data = 0;

    for (loop = 0; loop < 8; loop++)
    {
        MPS3_TS_I2C->CONTROLS = SCL;
        i2c_delay(TSC_TSU);
        if ((MPS3_TS_I2C->CONTROL & SDA))
            data += (1 << (7 - loop));
        MPS3_TS_I2C->CONTROLC = SCL;
        i2c_delay(TSC_TSU);
    }

    MPS3_TS_I2C->CONTROLC = SDA;
    i2c_delay(TSC_TSU);

    return data;
}

// Read the acknowledge bit
int TSC_I2C_receive_ack(void)
{
    int nack;

    i2c_delay(TSC_TSU);
    MPS3_TS_I2C->CONTROLS = SDA;
    i2c_delay(TSC_TSU);
    MPS3_TS_I2C->CONTROLC = SCL;
    i2c_delay(TSC_TSU);
    MPS3_TS_I2C->CONTROLS = SCL;
    i2c_delay(TSC_TSU);
    nack = MPS3_TS_I2C->CONTROL & SDA;
    i2c_delay(TSC_TSU);
    MPS3_TS_I2C->CONTROLC = SCL;
    i2c_delay(TSC_TSU);
    MPS3_TS_I2C->CONTROLS = SDA;
    i2c_delay(TSC_TSU);
    if(nack==0)
        return 1;

    return 0;
}

// Write the acknowledge bit
void TSC_I2C_send_ack(void)
{
    i2c_delay(TSC_TSU);
    MPS3_TS_I2C->CONTROLC = SCL;
    i2c_delay(TSC_TSU);
    MPS3_TS_I2C->CONTROLS = SDA;
    i2c_delay(TSC_TSU);
    MPS3_TS_I2C->CONTROLS = SCL;
    i2c_delay(TSC_TSU);
    MPS3_TS_I2C->CONTROLC = SCL;
    i2c_delay(TSC_TSU);
    MPS3_TS_I2C->CONTROLC = SDA;
    i2c_delay(TSC_TSU);
}

// Write data stream and read one/two bytes
unsigned int TSC_I2C_read(unsigned char reg_addr, unsigned char sadr, unsigned char bytes)
{
	unsigned int rxdata, loop;

    // Start bit
    i2c_delay(TSC_TSU);
    MPS3_TS_I2C->CONTROLS = SDA | SCL;
    i2c_delay(TSC_TSU);
    MPS3_TS_I2C->CONTROLC = SDA;
    i2c_delay(TSC_TSU);

    // Set serial and register address
    TSC_I2C_send_byte(sadr);
    TSC_I2C_receive_ack();
    TSC_I2C_send_byte(reg_addr);
    TSC_I2C_receive_ack();

    // Stop bit
    i2c_delay(TSC_TSU);
    MPS3_TS_I2C->CONTROLC = SDA;
    i2c_delay(TSC_TSU);
    MPS3_TS_I2C->CONTROLS = SCL;
    i2c_delay(TSC_TSU);
    MPS3_TS_I2C->CONTROLS = SDA;
    i2c_delay(TSC_TSU);

    // Start bit
    i2c_delay(TSC_TSU);
    MPS3_TS_I2C->CONTROLC = SDA;
    i2c_delay(TSC_TSU);
    MPS3_TS_I2C->CONTROLC = SCL;
    i2c_delay(TSC_TSU);

    // Read from serial address
    TSC_I2C_send_byte(sadr | 1);
    TSC_I2C_receive_ack();
    rxdata = (TSC_I2C_receive_byte() & 0xFF);

    // Read multiple bytes
    if ((bytes > 1) && (bytes < 5))
    {
    	for (loop = 0; loop < (bytes - 1); loop++)
    	{
        	// Send ACK
            i2c_delay(TSC_TSU);
            MPS3_TS_I2C->CONTROLC = SDA;
            i2c_delay(TSC_TSU);
            MPS3_TS_I2C->CONTROLS = SCL;
            i2c_delay(TSC_TSU);
            MPS3_TS_I2C->CONTROLC = SCL;
            i2c_delay(TSC_TSU);

            // Next byte
    	    rxdata = ((rxdata << 8) & 0xFFFFFF00);
            rxdata |= (TSC_I2C_receive_byte() & 0xFF);
    	}
    }
    TSC_I2C_send_ack();

    // Actual stop bit
    i2c_delay(TSC_TSU);
    MPS3_TS_I2C->CONTROLC = SDA;
    i2c_delay(TSC_TSU);
    MPS3_TS_I2C->CONTROLS = SCL;
    i2c_delay(TSC_TSU);
    MPS3_TS_I2C->CONTROLS = SDA;
    i2c_delay(TSC_TSU);

    return rxdata;
}

// Write data stream and write one byte
void TSC_I2C_write(unsigned char reg_addr, unsigned char data_byte, unsigned char sadr)
{
    // Start bit
    i2c_delay(TSC_TSU);
    MPS3_TS_I2C->CONTROLS = SDA | SCL;
    i2c_delay(TSC_TSU);
    MPS3_TS_I2C->CONTROLC = SDA;
    i2c_delay(TSC_TSU);

    // Set serial and register address
    TSC_I2C_send_byte(sadr);
    TSC_I2C_receive_ack();
    TSC_I2C_send_byte(reg_addr);
    TSC_I2C_receive_ack();
    TSC_I2C_send_byte(data_byte);
    TSC_I2C_receive_ack();

    // Actual stop bit
    i2c_delay(TSC_TSU);
    MPS3_TS_I2C->CONTROLC = SDA;
    i2c_delay(TSC_TSU);
    MPS3_TS_I2C->CONTROLS = SCL;
    i2c_delay(TSC_TSU);
    MPS3_TS_I2C->CONTROLS = SDA;
    i2c_delay(TSC_TSU);
}

// Clear the TSC I2C interface as there is no RESET control
void TSC_I2C_clear(void)
{
    unsigned int loop;
    
	for (loop = 0; loop < 5; loop++)
	{
		// Start/Stop bit
		i2c_delay(TSC_TSU);
		MPS3_TS_I2C->CONTROLC = SDA;
		i2c_delay(TSC_TSU);
		MPS3_TS_I2C->CONTROLC = SCL;
		i2c_delay(TSC_TSU);
		MPS3_TS_I2C->CONTROLS = SCL;
		i2c_delay(TSC_TSU);
		MPS3_TS_I2C->CONTROLS = SDA;
		i2c_delay(TSC_TSU);
	}
}

