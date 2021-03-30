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
 * File:     aaci_i2s_mps3.c
 * Release:  Version 1.0
 * ----------------------------------------------------------------
 */

/*
 * Code implementation file for the AACI (Advanced Audio CODEC) I2S interface.
 */

#include "AAIC_I2S_MPS3.h"

/*******************************************************************************
* Function to Configure I2S interface                                          *
*   Parameter: pI2S - pointer to I2S registers                                 *
*              pconfig - pointer to configuration structure                    *
*   Return:    1, if parameters out of range                                   *
*              0, if parameters configured                                     *
*******************************************************************************/
int i2s_config(MPS3_I2S_TypeDef *pI2S, I2S_config *pconfig)
{
  uint32_t i2s_control_val;
  
  // Mask reset bits
  i2s_control_val  =  pI2S->CONTROL & (I2S_CONTROL_FIFORST_Msk | I2S_CONTROL_CODECRST_Msk);  
  // TX water level range from 3 to 0
  if (pconfig->tx_waterlevel > 3) return 1; // Error if out of range
  // RX water level range from 4 to 1
  if ((pconfig->rx_waterlevel > 4)||(pconfig->rx_waterlevel == 0)) return 1; // Error if out of range
  
  i2s_control_val  = (pconfig->rx_waterlevel << I2S_CONTROL_RXWLVL_Pos) | 
                     (pconfig->tx_waterlevel << I2S_CONTROL_TXWLVL_Pos) |
                     (pconfig->rx_int_enable & 0x1) << I2S_CONTROL_RXIRQEN_Pos |
                     (pconfig->rx_enable & 0x1) << I2S_CONTROL_RXEN_Pos    |
                     (pconfig->tx_int_enable & 0x1) << I2S_CONTROL_TXIRQEN_Pos |
                     (pconfig->tx_enable & 0x1) << I2S_CONTROL_TXEN_Pos;

  pI2S->CONTROL  = i2s_control_val;
  return 0;
}

/*******************************************************************************
* Functions to return state of staus register bits                             *
*   Parameter: pI2S - pointer to I2S registers                                 *
*   Return:    state of required bit                                           *
*******************************************************************************/
int i2s_tx_fifo_empty(MPS3_I2S_TypeDef *pI2S){
  return (pI2S->STATUS & I2S_STATUS_TXEmpty_Msk) >> I2S_STATUS_TXEmpty_Pos;
  }

int i2s_tx_fifo_full(MPS3_I2S_TypeDef *pI2S){
  return (pI2S->STATUS & I2S_STATUS_TXFull_Msk)  >> I2S_STATUS_TXFull_Pos;
  }

int i2s_rx_fifo_empty(MPS3_I2S_TypeDef *pI2S){
  return (pI2S->STATUS & I2S_STATUS_RXEmpty_Msk) >> I2S_STATUS_RXEmpty_Pos;
  }

int i2s_rx_fifo_full(MPS3_I2S_TypeDef *pI2S){
  return (pI2S->STATUS & I2S_STATUS_RXFull_Msk)  >> I2S_STATUS_RXFull_Pos;
  }

int i2s_rx_irq_alert(MPS3_I2S_TypeDef *pI2S){
  return (pI2S->STATUS & I2S_STATUS_RXIRQ_Msk)  >> I2S_STATUS_RXIRQ_Pos;
  }

int i2s_tx_irq_alert(MPS3_I2S_TypeDef *pI2S){
  return (pI2S->STATUS & I2S_STATUS_TXIRQ_Msk)  >> I2S_STATUS_TXIRQ_Pos;
  }

// Stop Transmit 
int i2s_tx_stop(MPS3_I2S_TypeDef *pI2S){
  pI2S->CONTROL &= ~I2S_CONTROL_TXEN_Msk;
  return 0;
  }

// Stop Receive
int i2s_rx_stop(MPS3_I2S_TypeDef *pI2S){
  pI2S->CONTROL &= ~I2S_CONTROL_RXEN_Msk;
  return 0;
  }

// Return TX error status  
int i2s_get_tx_error(MPS3_I2S_TypeDef *pI2S){
  return ((pI2S->ERROR & I2S_ERROR_TXERR_Msk) >> I2S_ERROR_TXERR_Pos);
  }  

// Return RX error status  
int i2s_get_rx_error(MPS3_I2S_TypeDef *pI2S){
  return ((pI2S->ERROR & I2S_ERROR_RXERR_Msk) >> I2S_ERROR_RXERR_Pos);
  }  
  
// Clear TX error status  
void i2s_clear_tx_error(MPS3_I2S_TypeDef *pI2S){
  pI2S->ERRORCLR = I2S_ERROR_TXERR_Msk;
  }  

// Clear RX error status  
void i2s_clear_rx_error(MPS3_I2S_TypeDef *pI2S){
  pI2S->ERRORCLR = I2S_ERROR_RXERR_Msk;
  }  

// Reset Fifo  
void i2s_fifo_reset(MPS3_I2S_TypeDef *pI2S){
  pI2S->CONTROL |= I2S_CONTROL_FIFORST_Msk;
  pI2S->CONTROL &= ~I2S_CONTROL_FIFORST_Msk;
  }

// Set Fifo Reset 
void i2s_fifo_set_reset(MPS3_I2S_TypeDef *pI2S){
  pI2S->CONTROL |= I2S_CONTROL_FIFORST_Msk;
  }

// Clear Fifo Reset 
void i2s_fifo_clear_reset(MPS3_I2S_TypeDef *pI2S){
  pI2S->CONTROL &= ~I2S_CONTROL_FIFORST_Msk;
  }

// Reset Codec  
void i2s_codec_reset(MPS3_I2S_TypeDef *pI2S){
  int i;
  pI2S->CONTROL |= I2S_CONTROL_CODECRST_Msk;
  for (i=0;i<6;i++) { // delay loop
    __NOP();
    }
  pI2S->CONTROL &= ~I2S_CONTROL_CODECRST_Msk;
  }

// Set Codec Reset  
void i2s_codec_set_reset(MPS3_I2S_TypeDef *pI2S){
  pI2S->CONTROL |= I2S_CONTROL_CODECRST_Msk;
  }

// Clear Codec Reset  
  void i2s_codec_clear_reset(MPS3_I2S_TypeDef *pI2S){
  pI2S->CONTROL &= ~I2S_CONTROL_CODECRST_Msk;
  }

// Set Speed
int i2s_speed_config(MPS3_I2S_TypeDef *pI2S, uint32_t divide_ratio){
  if (divide_ratio < 18) return 1; // Error: Divide ratio too small to send all bits
  if (divide_ratio > 0x3FF) return 1; // Error: Divide ratio too large (only 10 bits)
  
  pI2S->DIVIDE = divide_ratio;
  return 0;
  }
