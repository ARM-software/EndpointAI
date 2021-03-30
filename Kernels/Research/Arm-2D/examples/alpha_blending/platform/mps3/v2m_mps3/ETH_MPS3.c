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
 * File:     eth_mps3.c
 * Release:  Version 1.0
 * ----------------------------------------------------------------
 */

/*
 * Code implementation file for the LAN Ethernet interface.
 */

#include <stdio.h>

#include "ETH_MPS3.h"
#include "platform.h"

// SMSC9220 low-level operations
unsigned int smsc9220_mac_regread(unsigned char regoffset, unsigned int *data)
{
    unsigned int val, maccmd;
    int timedout;
    int error;

    error = 0;
    val = SMSC9220->MAC_CSR_CMD;
    if(!(val & ((unsigned int)1 << 31))) {    // Make sure there's no pending operation
        maccmd = 0;
        maccmd |= regoffset;
        maccmd |= ((unsigned int)1 << 30);     // Indicates read
        maccmd |= ((unsigned int)1 << 31);     // Start bit
        SMSC9220->MAC_CSR_CMD = maccmd;        // Start operation

        timedout = 50;
        do {
            val = SMSC9220->BYTE_TEST;  // A no-op read.
            delay_ms(1);
            timedout--;
        } while(timedout && (SMSC9220->MAC_CSR_CMD & ((unsigned int)1 << 31)));

        if(!timedout) {
            debug("Error: SMSC9220 MAC CSR read operation timed out.\n");
            error = 1;
        }
        else
            *data = SMSC9220->MAC_CSR_DATA;
    } else {
        debug("Warning: SMSC9220 MAC CSR is busy. No data read.\n");
        *data = 0;
    }
    return error;
}

unsigned int smsc9220_mac_regwrite(unsigned char regoffset, unsigned int data)
{
    unsigned int read, maccmd;
    int timedout;
    int error;

    error = 0;
    read = SMSC9220->MAC_CSR_CMD;
    if(!(read & ((unsigned int)1 << 31))) { // Make sure there's no pending operation
        SMSC9220->MAC_CSR_DATA = data;      // Store data.
        maccmd = 0;
        maccmd |= regoffset;
        maccmd &= ~((unsigned int)1 << 30); // Clear indicates write
        maccmd |= ((unsigned int)1 << 31);  // Indicate start of operation
        SMSC9220->MAC_CSR_CMD = maccmd;

        timedout = 50;
        do {
            read = SMSC9220->BYTE_TEST;     // A no-op read.
            delay_ms(1);
            timedout--;
        } while(timedout && (SMSC9220->MAC_CSR_CMD & ((unsigned int)1 << 31)));

        if(!timedout) {
            debug("Error: SMSC9220 MAC CSR write operation timed out.\n");
            error = 1;
        }
    } else {
       debug("Warning: SMSC9220 MAC CSR is busy. No data written.\n");
    }
    return error;
}

unsigned int smsc9220_phy_regread(unsigned char regoffset, unsigned short *data)
{
    unsigned int val, phycmd; int error;
    int timedout;

    error = 0;

    smsc9220_mac_regread(SMSC9220_MAC_MII_ACC, &val);

    if(!(val & 1)) {    // Not busy
        phycmd = 0;
        phycmd |= (1 << 11);                 // 1 to [15:11]
        phycmd |= ((regoffset & 0x1F) << 6); // Put regoffset to [10:6]
        phycmd &= ~(1 << 1);                 // Clear [1] indicates read.
        phycmd |= (1 << 0);                  // Set [0] indicates operation start

        smsc9220_mac_regwrite(SMSC9220_MAC_MII_ACC, phycmd);

        val = 0;
        timedout = 50;
        do {
            delay_ms(1);
            timedout--;
            smsc9220_mac_regread(SMSC9220_MAC_MII_ACC,&val);
        } while(timedout && (val & ((unsigned int)1 << 0)));

        if(!timedout) {
            debug("Error: SMSC9220 MAC MII read operation timed out.\n");
            error = 1;
        }
        else
            smsc9220_mac_regread(SMSC9220_MAC_MII_DATA, (unsigned int *)data);

    } else {
        debug("Warning: SMSC9220 MAC MII is busy. No data read.\n");
        *data = 0;
    }
    return error;
}

unsigned int smsc9220_phy_regwrite(unsigned char regoffset, unsigned short data)
{
    unsigned int val, phycmd; int error;
    int timedout;

    error = 0;

    smsc9220_mac_regread(SMSC9220_MAC_MII_ACC, &val);

    if(!(val & 1)) {    // Not busy
        smsc9220_mac_regwrite(SMSC9220_MAC_MII_DATA, (data & 0xFFFF)); // Load the data
        phycmd = 0;
        phycmd |= (1 << 11);                    // 1 to [15:11]
        phycmd |= ((regoffset & 0x1F) << 6);     // Put regoffset to [10:6]
        phycmd |= (1 << 1);                     // Set [1] indicates write.
        phycmd |= (1 << 0);                     // Set [0] indicates operation start
        smsc9220_mac_regwrite(SMSC9220_MAC_MII_ACC, phycmd);   // Start operation

        phycmd = 0;
        timedout = 50;

        do {

            delay_ms(1);
            timedout--;
            smsc9220_mac_regread(SMSC9220_MAC_MII_ACC, &phycmd);
        } while(timedout && (phycmd & (1 << 0)));

        if(!timedout) {
            debug("Error: SMSC9220 MAC MII write operation timed out.\n");
            error = 1;
        }

    } else {
        debug("Warning: SMSC9220 MAC MII is busy. No data written.\n");
    }
    return error;
}

// Returns smsc9220 id.
unsigned int smsc9220_read_id(void)
{
    return SMSC9220->ID_REV;
}

// Initiates a soft reset, returns failure or success.
unsigned int smsc9220_soft_reset(void)
{
    int timedout;

    timedout = 10;
    // Soft reset
    SMSC9220->HW_CFG |= 1;

    do {
        delay_ms(1);
        timedout--;
    } while(timedout && (SMSC9220->HW_CFG & 1));

    if(!timedout)
        return 1;

    return 0;
}

void smsc9220_set_txfifo(unsigned int val)
{
    // 2kb minimum, 14kb maximum
    if(val < 2 || val > 14)
        return;

    SMSC9220->HW_CFG = val << 16;
}


unsigned int smsc9220_wait_eeprom(void)
{
    int timedout;

    timedout = 50;

    do {
        delay_ms(1);
        timedout--;

    } while(timedout && (SMSC9220->E2P_CMD & ((unsigned int) 1 << 31)));

    if(!timedout)
        return 1;

    return 0;
}

/* initialise irqs */
void smsc9220_init_irqs(void)
{
	SMSC9220->INT_EN    = 0x0;
	SMSC9220->INT_STS   = 0xFFFFFFFF;   // clear all interrupts
	SMSC9220->IRQ_CFG   = 0x22000100;   // irq deassertion at 220 usecs and master IRQ enable.
}

unsigned int smsc9220_check_phy(void)
{
    unsigned short phyid1, phyid2;

    smsc9220_phy_regread(SMSC9220_PHY_ID1,&phyid1);
    smsc9220_phy_regread(SMSC9220_PHY_ID2,&phyid2);
    debug("PHY ID1: %#08x, PHY ID2: %#08x\n\n",phyid1, phyid2);
    return ((phyid1 == 0xFFFF && phyid2 == 0xFFFF) ||
            (phyid1 == 0x0 && phyid2 == 0x0));
}

unsigned int smsc9220_reset_phy(void)
{
    unsigned short read;
    int error;

    error = 0;
    if(smsc9220_phy_regread(SMSC9220_PHY_BCONTROL, &read)) {
        debug("Error: PHY BCONTROL read failed.\n");
        error = 1;
        return error;
    }

    read |= (1 << 15);
    if(smsc9220_phy_regwrite(SMSC9220_PHY_BCONTROL, read)) {
        debug("Error: PHY BCONTROL write failed.\n");
        error = 1;
        return error;
    }
    return 0;
}

/* Advertise all speeds and pause capabilities */
void smsc9220_advertise_cap(void)
{
    unsigned short aneg_adv;
    aneg_adv = 0;


    smsc9220_phy_regread(SMSC9220_PHY_ANEG_ADV, &aneg_adv);
    debug("advertise_cap: PHY_ANEG_ADV before write: %#08x\n",aneg_adv);
    aneg_adv |= 0xDE0;

    smsc9220_phy_regwrite(SMSC9220_PHY_ANEG_ADV, aneg_adv);
    smsc9220_phy_regread(SMSC9220_PHY_ANEG_ADV, &aneg_adv);
    debug("advertise_cap: PHY_ANEG_ADV: after write: %#08x\n",aneg_adv);
    return;
}

void smsc9220_establish_link(void)
{
    unsigned short bcr;

    smsc9220_phy_regread(SMSC9220_PHY_BCONTROL, &bcr);
    debug("establish link: PHY_BCONTROL before write: %#08x\n",bcr);
    bcr |= (1 << 12) | (1 << 9);
    smsc9220_phy_regwrite(SMSC9220_PHY_BCONTROL, bcr);
    smsc9220_phy_regread(SMSC9220_PHY_BCONTROL, &bcr);
    debug("establish link: PHY_BCONTROL after write: %#08x\n", bcr);

    {
        unsigned int hw_cfg;

        hw_cfg = 0;
        hw_cfg = SMSC9220->HW_CFG;

        hw_cfg &= 0xF0000;
        hw_cfg |= (1 << 20);
        SMSC9220->HW_CFG = hw_cfg;
    }

    return;
}

void smsc9220_enable_xmit(void)
{
    SMSC9220->TX_CFG = 0x2; // Enable trasmission
    return;
}

void smsc9220_enable_mac_xmit(void)
{
    unsigned int mac_cr;

    mac_cr = 0;
    smsc9220_mac_regread(SMSC9220_MAC_CR, &mac_cr);

    mac_cr |= (1 << 3);     // xmit enable
    mac_cr |= (1 << 28);    // Heartbeat disable

    smsc9220_mac_regwrite(SMSC9220_MAC_CR, mac_cr);
    return;
}

void smsc9220_enable_mac_recv(void)
{
    unsigned int mac_cr;

    mac_cr = 0;
    smsc9220_mac_regread(SMSC9220_MAC_CR, &mac_cr);
    mac_cr |= (1 << 2);     // Recv enable
    smsc9220_mac_regwrite(SMSC9220_MAC_CR, mac_cr);

    return;
}


unsigned int smsc9220_check_ready(void)
{
    return !(SMSC9220->PMT_CTRL & 1);
}

/* Generate a soft irq */
void smsc9220_set_soft_int(void)
{
    SMSC9220->INT_EN |= 0x80000000;
}

/* clear soft irq */
void smsc9220_clear_soft_int(void)
{
    SMSC9220->INT_STS |= 0x80000000;
}


unsigned int smsc9220_recv_packet(unsigned int *recvbuf, unsigned int *index)
{
    unsigned int rxfifo_inf;    // Tells us the status of rx payload and status fifos.
    unsigned int rxfifo_stat;

    unsigned int pktsize;
    unsigned int dwords_to_read;

    debug("recv_packet start: recvbuf: %#08x index: %d\n",
            (unsigned int)recvbuf,*index);

    rxfifo_inf = SMSC9220->RX_FIFO_INF;
    debug("%d bytes of data available in rx fifo.\n",rxfifo_inf & 0xFFFF);

    if(rxfifo_inf & 0xFFFF) { // If there's data
        rxfifo_stat = SMSC9220->RX_STAT_PORT;
        if(rxfifo_stat != 0) {   // Fetch status of this packet
            pktsize = ((rxfifo_stat >> 16) & 0x3FFF);
            debug("recv_packet: rxfifo_stat: %#08x, pktsize (bytes): %u\n",rxfifo_stat, pktsize);
            if(rxfifo_stat & (1 << 15)) {
                debug("Error occured during receiving of packets on the bus.\n");
                return 1;
            } else {
                /* Below formula (recommended by SMSC9220 code)
                 * gives 1 more than required. This is perhaps because
                 * a last word is needed for not word aligned packets.
                 */
                dwords_to_read = (pktsize + 3) >> 2;
                debug("recv_packet: dwords_to_read: %u\n",dwords_to_read);
                // PIO copy of data received:
                while(dwords_to_read > 0) {
                    recvbuf[*index] = SMSC9220->RX_DATA_PORT;
                    debug("recv_packet: Received word[%d]: %#08x\n",*index,recvbuf[*index]);
                    (*index)++;
                    dwords_to_read--;
                }
            }
        } else {
            debug("Error: rx fifo status reads zero where data is available.\n");
            return 1;
        }
    } else {
        debug("Error: No data available in rx FIFO\n");
        return 1;
    }

    rxfifo_stat = SMSC9220->RX_STAT_PORT;
    rxfifo_inf = SMSC9220->RX_FIFO_INF;
    debug("recv_packet: After second read, rxfifo_stat: %#08x, rxfifo_inf: %#08x\n",
               rxfifo_stat, rxfifo_inf);
    debug("recv_packet end: recvbuf: %#08x index: %d\n", (unsigned int)recvbuf,*index);

    return 0;
}


// Does the actual transfer of data to FIFO, note it does no
// fifo availability checking. This should be done by caller.
// Assumes the whole frame is transferred at once as a single segment
void smsc9220_xmit_packet(unsigned char * pkt, unsigned int length)
{
    unsigned int txcmd_a, txcmd_b;
    unsigned int dwords_to_write;
    volatile unsigned int dwritten;
    unsigned int *pktptr;
    volatile unsigned int xmit_stat, xmit_stat2, xmit_inf;
    int i;

    pktptr = (unsigned int *) pkt;
    txcmd_a = 0;
    txcmd_b = 0;

    txcmd_a |= (1 << 12) | (1 << 13);   // First and last segments
    txcmd_a |= length & 0x7FF;          // [10:0] contains length

    txcmd_b |= ((length & 0xFFFF) << 16); // [31:16] contains length
    txcmd_b |= length & 0x7FF;          // [10:0] also contains length

    debug("txcmd_a: %#08x\n", txcmd_a);
    debug("txcmd_b: %#08x\n", txcmd_b);

    SMSC9220->TX_DATA_PORT = txcmd_a;
    SMSC9220->TX_DATA_PORT = txcmd_b;
    dwritten = dwords_to_write = (length + 3) >> 2;

    // PIO Copy to FIFO. Could replace this with DMA.
    while(dwords_to_write > 0) {
         SMSC9220->TX_DATA_PORT = *pktptr;
         debug("Transmitting word[%d]: %#08x\n",dwritten-dwords_to_write,*pktptr);
         pktptr++;
         dwords_to_write--;
    }

    xmit_stat = SMSC9220->TX_STAT_PORT;
        debug("Finished transfer. TX_STATUS_WORD: %#08x\n",xmit_stat);
    xmit_stat2 = SMSC9220->TX_STAT_PORT;
    xmit_inf = SMSC9220->TX_FIFO_INF;
        debug("After popping TX_STAT: %#08x, TX_INF: %#08x\n\n",xmit_stat2, xmit_inf);

    if(xmit_stat2 != 0 ) {
            debug("The second read of TX_STAT is non-zero. Retry reading a few more times.\n");
        for(i = 0; i < 6; i++) {
            xmit_stat2 = SMSC9220->TX_STAT_PORT;
                debug("Retry %d: TX_STAT: %#08x\n",i+1,xmit_stat2);
        }
    }
}
