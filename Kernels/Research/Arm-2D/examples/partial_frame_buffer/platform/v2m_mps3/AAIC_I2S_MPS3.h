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
 * File:     aaci_i2s_mps3.h
 * Release:  Version 1.0
 * ----------------------------------------------------------------
 */

/*
 * Header file for the AACI (Advanced Audio CODEC) I2S interface.
 */

#ifndef _AACI_I2S_H_
#define _AACI_I2S_H_

#include <stdint.h>

#include "SMM_MPS3.h"

// I2S configuration structure
typedef struct
{
    uint32_t    tx_enable;
    uint32_t    tx_int_enable;
    uint32_t    tx_waterlevel;
    uint32_t    rx_enable;
    uint32_t    rx_int_enable;
    uint32_t    rx_waterlevel;
}I2S_config;


// External function types for the Audio I2S
extern int i2s_config(MPS3_I2S_TypeDef *pI2S, I2S_config *pconfig);
extern int i2s_tx_fifo_empty(MPS3_I2S_TypeDef * pI2S);
extern int i2s_tx_fifo_full(MPS3_I2S_TypeDef * pI2S);
extern int i2s_rx_fifo_empty(MPS3_I2S_TypeDef * pI2S);
extern int i2s_rx_fifo_full(MPS3_I2S_TypeDef * pI2S);
extern int i2s_rx_irq_alert(MPS3_I2S_TypeDef * pI2S);
extern int i2s_tx_irq_alert(MPS3_I2S_TypeDef * pI2S);
extern int i2s_tx_stop(MPS3_I2S_TypeDef * pI2S);
extern int i2s_rx_stop(MPS3_I2S_TypeDef * pI2S);
// Return TX error status
extern int i2s_get_tx_error(MPS3_I2S_TypeDef * pI2S);
// Return RX error status
extern int i2s_get_rx_error(MPS3_I2S_TypeDef * pI2S);
// Clear TX error status
extern void i2s_clear_tx_error(MPS3_I2S_TypeDef * pI2S);
// Clear RX error status
extern void i2s_clear_rx_error(MPS3_I2S_TypeDef * pI2S);
extern void i2s_fifo_reset(MPS3_I2S_TypeDef *pI2S);
extern void i2s_codec_reset(MPS3_I2S_TypeDef *pI2S);
extern int i2s_speed_config(MPS3_I2S_TypeDef *pI2S, uint32_t divide_ratio);
// Set Fifo Reset
extern void i2s_fifo_set_reset(MPS3_I2S_TypeDef *pI2S);
// Clear Fifo Reset
extern void i2s_fifo_clear_reset(MPS3_I2S_TypeDef *pI2S);
// Set Codec Reset
extern void i2s_codec_set_reset(MPS3_I2S_TypeDef *pI2S);
// Clear Codec Reset
extern void i2s_codec_clear_reset(MPS3_I2S_TypeDef *pI2S);

#endif
