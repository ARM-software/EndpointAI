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
 * File:     aaci_i2c_mps3.c
 * Release:  Version 1.0
 * ----------------------------------------------------------------
 */

/*
 * Code implementation file for the AACI (Advanced Audio CODEC) I2C interface.
 */

#include "SMM_MPS3.h"
#include "AAIC_I2C_MPS3.h"

#define AAIC_TSU            32           // Setup delay 1000nS min

// Write 8 bits of data to the serial bus
void AAIC_I2C_send_byte(unsigned char c)
{
    int loop;

    for (loop = 0; loop < 8; loop++) {
        i2c_delay(AAIC_TSU);
        MPS3_AAIC_I2C->CONTROLC = SCL;
        i2c_delay(AAIC_TSU);
        if (c & (1 << (7 - loop)))
            MPS3_AAIC_I2C->CONTROLS = SDA;
        else
            MPS3_AAIC_I2C->CONTROLC = SDA;
        i2c_delay(AAIC_TSU);
        MPS3_AAIC_I2C->CONTROLS = SCL;
        i2c_delay(AAIC_TSU);
        MPS3_AAIC_I2C->CONTROLC = SCL;
    }

    i2c_delay(AAIC_TSU);
    MPS3_AAIC_I2C->CONTROLS = SDA;
    i2c_delay(AAIC_TSU);
}

// Read 8 bits of data from the serial bus
unsigned char AAIC_I2C_receive_byte(void)
{
    int data, loop;

    MPS3_AAIC_I2C->CONTROLS = SDA;
    data         = 0;

    for (loop = 0; loop < 8; loop++) {
        i2c_delay(AAIC_TSU);
        MPS3_AAIC_I2C->CONTROLC = SCL;
        i2c_delay(AAIC_TSU);
        MPS3_AAIC_I2C->CONTROLS = SCL | SDA;
        i2c_delay(AAIC_TSU);
        if ((MPS3_AAIC_I2C->CONTROL & SDA))
            data += (1 << (7 - loop));
        i2c_delay(AAIC_TSU);
        MPS3_AAIC_I2C->CONTROLC = SCL;
    }

    i2c_delay(AAIC_TSU);
    MPS3_AAIC_I2C->CONTROLC = SDA;
    i2c_delay(AAIC_TSU);

    return data;
}

// Read the acknowledge bit
int AAIC_I2C_receive_ack(void)
{
    int nack;

    i2c_delay(AAIC_TSU);
    MPS3_AAIC_I2C->CONTROLS = SDA;
    i2c_delay(AAIC_TSU);
    MPS3_AAIC_I2C->CONTROLC = SCL;
    i2c_delay(AAIC_TSU);
    MPS3_AAIC_I2C->CONTROLS = SCL;
    i2c_delay(AAIC_TSU);
    nack = MPS3_AAIC_I2C->CONTROL & SDA;
    i2c_delay(AAIC_TSU);
    MPS3_AAIC_I2C->CONTROLC = SCL;
    i2c_delay(AAIC_TSU);
    MPS3_AAIC_I2C->CONTROLS = SDA;
    i2c_delay(AAIC_TSU);
    if(nack==0)
        return 1;

    return 0;
}

// Write the acknowledge bit
void AAIC_I2C_send_ack(void)
{
    i2c_delay(AAIC_TSU);
    MPS3_AAIC_I2C->CONTROLC = SCL;
    i2c_delay(AAIC_TSU);
    MPS3_AAIC_I2C->CONTROLS = SDA;
    i2c_delay(AAIC_TSU);
    MPS3_AAIC_I2C->CONTROLS = SCL;
    i2c_delay(AAIC_TSU);
    MPS3_AAIC_I2C->CONTROLC = SCL;
    i2c_delay(AAIC_TSU);
    MPS3_AAIC_I2C->CONTROLC = SDA;
    i2c_delay(AAIC_TSU);
}

// Write data stream and read one byte
unsigned char AAIC_I2C_read(unsigned char reg_addr, unsigned char sadr)
{
    unsigned char rxdata;

    // Start bit
    i2c_delay(AAIC_TSU);
    MPS3_AAIC_I2C->CONTROLS = SDA | SCL;
    i2c_delay(AAIC_TSU);
    MPS3_AAIC_I2C->CONTROLC = SDA;
    i2c_delay(AAIC_TSU);

    // Set serial and register address
    AAIC_I2C_send_byte(sadr);
    AAIC_I2C_receive_ack();
    AAIC_I2C_send_byte(reg_addr);
    AAIC_I2C_receive_ack();

    // Stop bit
    i2c_delay(AAIC_TSU);
    MPS3_AAIC_I2C->CONTROLC = SDA;
    i2c_delay(AAIC_TSU);
    MPS3_AAIC_I2C->CONTROLS = SCL;
    i2c_delay(AAIC_TSU);
    MPS3_AAIC_I2C->CONTROLS = SDA;
    i2c_delay(AAIC_TSU);

    // Start bit
    i2c_delay(AAIC_TSU);
    MPS3_AAIC_I2C->CONTROLC = SDA;
    i2c_delay(AAIC_TSU);
    MPS3_AAIC_I2C->CONTROLC = SCL;
    i2c_delay(AAIC_TSU);

    // Read from serial address
    AAIC_I2C_send_byte(sadr | 1);
    AAIC_I2C_receive_ack();
    rxdata = AAIC_I2C_receive_byte();
    AAIC_I2C_send_ack();

    // Actual stop bit
    i2c_delay(AAIC_TSU);
    MPS3_AAIC_I2C->CONTROLC = SDA;
    i2c_delay(AAIC_TSU);
    MPS3_AAIC_I2C->CONTROLS = SCL;
    i2c_delay(AAIC_TSU);
    MPS3_AAIC_I2C->CONTROLS = SDA;
    i2c_delay(AAIC_TSU);

    return rxdata;
}

// Write data stream and write one byte
void AAIC_I2C_write(unsigned char reg_addr, unsigned char data_byte, unsigned char sadr)
{
    // Start bit
    i2c_delay(AAIC_TSU);
    MPS3_AAIC_I2C->CONTROLS = SDA | SCL;
    i2c_delay(AAIC_TSU);
    MPS3_AAIC_I2C->CONTROLC = SDA;
    i2c_delay(AAIC_TSU);

    // Set serial and register address
    AAIC_I2C_send_byte(sadr);
    AAIC_I2C_receive_ack();
    AAIC_I2C_send_byte(reg_addr);
    AAIC_I2C_receive_ack();
    AAIC_I2C_send_byte(data_byte);
    AAIC_I2C_receive_ack();

    // Actual stop bit
    i2c_delay(AAIC_TSU);
    MPS3_AAIC_I2C->CONTROLC = SDA;
    i2c_delay(AAIC_TSU);
    MPS3_AAIC_I2C->CONTROLS = SCL;
    i2c_delay(AAIC_TSU);
    MPS3_AAIC_I2C->CONTROLS = SDA;
    i2c_delay(AAIC_TSU);
}
