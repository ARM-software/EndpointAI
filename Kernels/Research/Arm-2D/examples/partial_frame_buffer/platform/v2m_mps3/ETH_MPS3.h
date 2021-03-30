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
 * File:     eth_mps3.h
 * Release:  Version 1.0
 * ----------------------------------------------------------------
 */

#ifndef _ETH_MPS3_H_
#define _ETH_MPS3_H_

#include "SMM_MPS3.h"

// Function declarations

unsigned int smsc9220_mac_regread(unsigned char regoffset, unsigned int *data);
unsigned int smsc9220_mac_regwrite(unsigned char regoffset, unsigned int data);
unsigned int smsc9220_phy_regread(unsigned char regoffset, unsigned short *data);
unsigned int smsc9220_phy_regwrite(unsigned char regoffset, unsigned short data);

unsigned int smsc9220_read_id(void);
unsigned int smsc9220_soft_reset(void);
void smsc9220_set_txfifo(unsigned int val);
unsigned int smsc9220_wait_eeprom(void);
void smsc9220_init_irqs(void);
unsigned int smsc9220_check_phy(void);
unsigned int smsc9220_reset_phy(void);

void smsc9220_advertise_cap(void);
void smsc9220_establish_link(void);
void smsc9220_enable_xmit(void);
void smsc9220_enable_mac_xmit(void);
void smsc9220_enable_mac_recv(void);
unsigned int smsc9220_check_ready(void);
void smsc9220_set_soft_int(void);
void smsc9220_clear_soft_int(void);

unsigned int smsc9220_recv_packet(unsigned int *recvbuf, unsigned int *index);
void smsc9220_xmit_packet(unsigned char * pkt, unsigned int length);

#endif
