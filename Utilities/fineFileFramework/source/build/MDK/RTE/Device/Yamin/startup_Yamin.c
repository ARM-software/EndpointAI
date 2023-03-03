
/*
 * Copyright (c) 2019 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*-----------------------------------------------------------------------------
 * File     startup_Yamin.c
 * Brief    CMSIS Device Startup File for
 *          Yamin Device (AN540)
 * Version  1.0.0
 *----------------------------------------------------------------------------*/

#if defined (YAMIN)
  #include "Yamin.h"
#else
  #error device not specified!
#endif

/*----------------------------------------------------------------------------
  Exception / Interrupt Handler Function Prototype
 *----------------------------------------------------------------------------*/
typedef void( *pFunc )( void );

/*----------------------------------------------------------------------------
  External References
 *----------------------------------------------------------------------------*/
extern uint32_t __INITIAL_SP;
extern uint32_t __STACK_LIMIT;

extern __NO_RETURN void __PROGRAM_START(void);

/*----------------------------------------------------------------------------
  Internal References
 *----------------------------------------------------------------------------*/
void __NO_RETURN Default_Handler(void);
void __NO_RETURN Reset_Handler  (void);

/*----------------------------------------------------------------------------
  Exception / Interrupt Handler
 *----------------------------------------------------------------------------*/
/* Exceptions */
__attribute__ ((weak)) void NMI_Handler                      (void)  {while(1);}
__attribute__ ((weak)) void HardFault_Handler                (void) {while(1);}
__attribute__ ((weak)) void MemManage_Handler                (void) {while(1);}
__attribute__ ((weak)) void BusFault_Handler                 (void) {while(1);}
__attribute__ ((weak)) void UsageFault_Handler               (void) {while(1);}
__attribute__ ((weak)) void SecureFault_Handler              (void) {while(1);}
__attribute__ ((weak)) void SVC_Handler                      (void) {while(1);}
__attribute__ ((weak)) void DebugMon_Handler                 (void) {while(1);}
__attribute__ ((weak)) void PendSV_Handler                   (void) {while(1);}
__attribute__ ((weak)) void SysTick_Handler                  (void) {while(1);}

/* Interrupts */
__attribute__ ((weak)) void NONSEC_WATCHDOG_RESET_IRQHandler (void) {while(1);}
__attribute__ ((weak)) void NONSEC_WATCHDOG_IRQHandler       (void) {while(1);}
__attribute__ ((weak)) void TIMER0_Handler                (void) {while(1);}
__attribute__ ((weak)) void TIMER1_Handler                (void) {while(1);}
__attribute__ ((weak)) void DUALTIMER_IRQHandler             (void) {while(1);}
__attribute__ ((weak)) void MPC_IRQHandler                   (void) {while(1);}
__attribute__ ((weak)) void PPC_IRQHandler                   (void) {while(1);}
__attribute__ ((weak)) void MSC_IRQHandler                   (void) {while(1);}
__attribute__ ((weak)) void BRIDGE_ERROR_IRQHandler          (void) {while(1);}

__attribute__ ((weak)) void FPIDC_IRQHandler                 (void) {while(1);}
__attribute__ ((weak)) void FPDZC_IRQHandler                 (void) {while(1);}
__attribute__ ((weak)) void FPOFC_IRQHandler                 (void) {while(1);}
__attribute__ ((weak)) void FPUFC_IRQHandler                 (void) {while(1);}
__attribute__ ((weak)) void FPIXC_IRQHandler                 (void) {while(1);}

__attribute__ ((weak)) void UART0RX_IRQHandler               (void) {while(1);}
__attribute__ ((weak)) void UART0TX_IRQHandler               (void) {while(1);}
__attribute__ ((weak)) void USARTx_RX_CPL_Handler            (void) {while(1);}
__attribute__ ((weak)) void USARTx_TX_CPL_Handler            (void) {while(1);}

__attribute__ ((weak)) void UART1RX_IRQHandler               (void) {while(1);}
__attribute__ ((weak)) void UART1TX_IRQHandler               (void) {while(1);}
__attribute__ ((weak)) void UART2RX_IRQHandler               (void) {while(1);}
__attribute__ ((weak)) void UART2TX_IRQHandler               (void) {while(1);}
__attribute__ ((weak)) void UART3RX_IRQHandler               (void) {while(1);}
__attribute__ ((weak)) void UART3TX_IRQHandler               (void) {while(1);}
__attribute__ ((weak)) void UART4RX_IRQHandler               (void) {while(1);}
__attribute__ ((weak)) void UART4TX_IRQHandler               (void) {while(1);}
__attribute__ ((weak)) void UART0_IRQHandler                 (void) {while(1);}
__attribute__ ((weak)) void UART1_IRQHandler                 (void) {while(1);}
__attribute__ ((weak)) void UART2_IRQHandler                 (void) {while(1);}
__attribute__ ((weak)) void UART3_IRQHandler                 (void) {while(1);}
__attribute__ ((weak)) void UART4_IRQHandler                 (void) {while(1);}
__attribute__ ((weak)) void UARTOVF_IRQHandler               (void) {while(1);}
__attribute__ ((weak)) void ETHERNET_IRQHandler              (void) {while(1);}
__attribute__ ((weak)) void I2S_Handler                  (void) {while(1);}
__attribute__ ((weak)) void TSC_IRQHandler                   (void) {while(1);}
__attribute__ ((weak)) void USB_IRQHandler                   (void) {while(1);}
__attribute__ ((weak)) void SPI2_IRQHandler                  (void) {while(1);}
__attribute__ ((weak)) void SPI3_IRQHandler                  (void) {while(1);}
__attribute__ ((weak)) void SPI4_IRQHandler                  (void) {while(1);}
__attribute__ ((weak)) void GPIO0_IRQHandler                 (void) {while(1);}
__attribute__ ((weak)) void GPIO1_IRQHandler                 (void) {while(1);}
__attribute__ ((weak)) void GPIO2_IRQHandler                 (void) {while(1);}
__attribute__ ((weak)) void GPIO3_IRQHandler                 (void) {while(1);}
__attribute__ ((weak)) void GPIO0_0_IRQHandler               (void) {while(1);}
__attribute__ ((weak)) void GPIO0_1_IRQHandler               (void) {while(1);}
__attribute__ ((weak)) void GPIO0_2_IRQHandler               (void) {while(1);}
__attribute__ ((weak)) void GPIO0_3_IRQHandler               (void) {while(1);}
__attribute__ ((weak)) void GPIO0_4_IRQHandler               (void) {while(1);}
__attribute__ ((weak)) void GPIO0_5_IRQHandler               (void) {while(1);}
__attribute__ ((weak)) void GPIO0_6_IRQHandler               (void) {while(1);}
__attribute__ ((weak)) void GPIO0_7_IRQHandler               (void) {while(1);}
__attribute__ ((weak)) void GPIO0_8_IRQHandler               (void) {while(1);}
__attribute__ ((weak)) void GPIO0_9_IRQHandler               (void) {while(1);}
__attribute__ ((weak)) void GPIO0_10_IRQHandler              (void) {while(1);}
__attribute__ ((weak)) void GPIO0_11_IRQHandler              (void) {while(1);}
__attribute__ ((weak)) void GPIO0_12_IRQHandler              (void) {while(1);}
__attribute__ ((weak)) void GPIO0_13_IRQHandler              (void) {while(1);}
__attribute__ ((weak)) void GPIO0_14_IRQHandler              (void) {while(1);}
__attribute__ ((weak)) void GPIO0_15_IRQHandler              (void) {while(1);}
__attribute__ ((weak)) void GPIO1_0_IRQHandler               (void) {while(1);}
__attribute__ ((weak)) void GPIO1_1_IRQHandler               (void) {while(1);}
__attribute__ ((weak)) void GPIO1_2_IRQHandler               (void) {while(1);}
__attribute__ ((weak)) void GPIO1_3_IRQHandler               (void) {while(1);}
__attribute__ ((weak)) void GPIO1_4_IRQHandler               (void) {while(1);}
__attribute__ ((weak)) void GPIO1_5_IRQHandler               (void) {while(1);}
__attribute__ ((weak)) void GPIO1_6_IRQHandler               (void) {while(1);}
__attribute__ ((weak)) void GPIO1_7_IRQHandler               (void) {while(1);}
__attribute__ ((weak)) void GPIO1_8_IRQHandler               (void) {while(1);}
__attribute__ ((weak)) void GPIO1_9_IRQHandler               (void) {while(1);}
__attribute__ ((weak)) void GPIO1_10_IRQHandler              (void) {while(1);}
__attribute__ ((weak)) void GPIO1_11_IRQHandler              (void) {while(1);}
__attribute__ ((weak)) void GPIO1_12_IRQHandler              (void) {while(1);}
__attribute__ ((weak)) void GPIO1_13_IRQHandler              (void) {while(1);}
__attribute__ ((weak)) void GPIO1_14_IRQHandler              (void) {while(1);}
__attribute__ ((weak)) void GPIO1_15_IRQHandler              (void) {while(1);}
__attribute__ ((weak)) void GPIO2_0_IRQHandler               (void) {while(1);}
__attribute__ ((weak)) void GPIO2_1_IRQHandler               (void) {while(1);}
__attribute__ ((weak)) void GPIO2_2_IRQHandler               (void) {while(1);}
__attribute__ ((weak)) void GPIO2_3_IRQHandler               (void) {while(1);}
__attribute__ ((weak)) void GPIO2_4_IRQHandler               (void) {while(1);}
__attribute__ ((weak)) void GPIO2_5_IRQHandler               (void) {while(1);}
__attribute__ ((weak)) void GPIO2_6_IRQHandler               (void) {while(1);}
__attribute__ ((weak)) void GPIO2_7_IRQHandler               (void) {while(1);}
__attribute__ ((weak)) void GPIO2_8_IRQHandler               (void) {while(1);}
__attribute__ ((weak)) void GPIO2_9_IRQHandler               (void) {while(1);}
__attribute__ ((weak)) void GPIO2_10_IRQHandler              (void) {while(1);}
__attribute__ ((weak)) void GPIO2_11_IRQHandler              (void) {while(1);}
__attribute__ ((weak)) void GPIO2_12_IRQHandler              (void) {while(1);}
__attribute__ ((weak)) void GPIO2_13_IRQHandler              (void) {while(1);}
__attribute__ ((weak)) void GPIO2_14_IRQHandler              (void) {while(1);}
__attribute__ ((weak)) void GPIO2_15_IRQHandler              (void) {while(1);}
__attribute__ ((weak)) void GPIO3_0_IRQHandler               (void) {while(1);}
__attribute__ ((weak)) void GPIO3_1_IRQHandler               (void) {while(1);}
__attribute__ ((weak)) void GPIO3_2_IRQHandler               (void) {while(1);}
__attribute__ ((weak)) void GPIO3_3_IRQHandler               (void) {while(1);}
__attribute__ ((weak)) void UART5RX_IRQHandler               (void) {while(1);}
__attribute__ ((weak)) void UART5TX_IRQHandler               (void) {while(1);}
__attribute__ ((weak)) void UART5_IRQHandler                 (void) {while(1);}
__attribute__ ((weak)) void HDCLCD_IRQHandler                (void) {while(1);}


/*----------------------------------------------------------------------------
  Exception / Interrupt Vector table
 *----------------------------------------------------------------------------*/

#if defined ( __GNUC__ )
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif

extern const pFunc __VECTOR_TABLE[496];
       const pFunc __VECTOR_TABLE[496] __VECTOR_TABLE_ATTRIBUTE = {
  (pFunc)(&__INITIAL_SP),                   /*     Initial Stack Pointer */
  Reset_Handler,                            /*     Reset Handler */
  NMI_Handler,                              /* -14 NMI Handler */
  HardFault_Handler,                        /* -13 Hard Fault Handler */
  MemManage_Handler,                        /* -12 MPU Fault Handler */
  BusFault_Handler,                         /* -11 Bus Fault Handler */
  UsageFault_Handler,                       /* -10 Usage Fault Handler */
  SecureFault_Handler,                      /*  -9 Secure Fault Handler */
  0,                                        /*     Reserved */
  0,                                        /*     Reserved */
  0,                                        /*     Reserved */
  SVC_Handler,                              /*  -5 SVCall Handler */
  DebugMon_Handler,                         /*  -4 Debug Monitor Handler */
  0,                                        /*     Reserved */
  PendSV_Handler,                           /*  -2 PendSV Handler */
  SysTick_Handler,                          /*  -1 SysTick Handler */

  /* Interrupts */
  NONSEC_WATCHDOG_RESET_IRQHandler,         /*   0: Non-Secure Watchdog Reset Interrupt */
  NONSEC_WATCHDOG_IRQHandler,               /*   1: Non-Secure Watchdog Interrupt */
  0,                                        /*   2: Reserved */
  TIMER0_Handler,                        /*   3: TIMER 0 Interrupt */
  TIMER1_Handler,                        /*   4: TIMER 1 Interrupt */
  DUALTIMER_IRQHandler,                     /*   5: Dual Timer Interrupt */
  0,                                        /*   6: Reserved */
  0,                                        /*   7: Reserved */
  0,                                        /*   8: Reserved */
  MPC_IRQHandler,                           /*   9: MPC Combined (Secure) Interrupt */
  PPC_IRQHandler,                           /*  10: PPC Combined (Secure) Interrupt */
  MSC_IRQHandler,                           /*  11: MSC Combined (Secure) Interrput */
  BRIDGE_ERROR_IRQHandler,                  /*  12: Bridge Error Combined (Secure) Interrupt */
  0,                                        /*  13: Reserved */
  0,                                        /*  14: Reserved */
  0,                                        /*  15: Reserved */
  0,                                        /*  16: Reserved */
  0,                                        /*  17: Reserved */
  0,                                        /*  18: Reserved */
  0,                                        /*  19: Reserved */
  0,                                        /*  20: Reserved */
  0,                                        /*  21: Reserved */
  0,                                        /*  22: Reserved */
  0,                                        /*  23: Reserved */
  0,                                        /*  24: Reserved */
  0,                                        /*  25: Reserved */
  0,                                        /*  26: Reserved */
  FPIDC_IRQHandler,                         /*  27: Masked floating-point divide-by-zero exception */
  FPDZC_IRQHandler,                         /*  28: Masked floating-point underflow exception */
  FPOFC_IRQHandler,                         /*  29: Masked floating-point overflow exception */
  FPUFC_IRQHandler,                         /*  30: Masked floating-point input denormal exception */
  FPIXC_IRQHandler,                         /*  31: Masked floating-point inexact exception */

  USARTx_RX_CPL_Handler,                       /*  32: UART 0 Receive Interrupt */
  USARTx_TX_CPL_Handler,                       /*  33: UART 0 Transmit Interrupt */
  UART1RX_IRQHandler,                       /*  34: UART 1 Receive Interrupt */
  UART1TX_IRQHandler,                       /*  35: UART 1 Transmit Interrupt */
  UART2RX_IRQHandler,                       /*  36: UART 2 Receive Interrupt */
  UART2TX_IRQHandler,                       /*  37: UART 2 Transmit Interrupt */
  UART3RX_IRQHandler,                       /*  38: UART 3 Receive Interrupt */
  UART3TX_IRQHandler,                       /*  39: UART 3 Transmit Interrupt */
  UART4RX_IRQHandler,                       /*  40: UART 4 Receive Interrupt */
  UART4TX_IRQHandler,                       /*  41: UART 4 Transmit Interrupt */
  UART0_IRQHandler,                         /*  42: UART 0 Combined Interrupt */
  UART1_IRQHandler,                         /*  43: UART 1 Combined Interrupt */
  UART2_IRQHandler,                         /*  44: UART 2 Combined Interrupt */
  UART3_IRQHandler,                         /*  45: UART 3 Combined Interrupt */
  UART4_IRQHandler,                         /*  46: UART 4 Combined Interrupt */
  UARTOVF_IRQHandler,                       /*  47: UART Overflow (0, 1, 2, 3, 4 & 5) Interrupt */
  ETHERNET_IRQHandler,                      /*  48: Ethernet Interrupt */
  I2S_Handler,                          /*  49: FPGA Audio I2S Interrupt */
  TSC_IRQHandler,                           /*  50: Touch Screen Interrupt */
  USB_IRQHandler,                           /*  51: USB Interrupt */
  SPI2_IRQHandler,                          /*  52: SPI ADC Interrupt */
  SPI3_IRQHandler,                          /*  53: SPI (Shield 0) Interrupt */
  SPI4_IRQHandler,                          /*  54: SPI (Shield 1) Interrupt */
  0,                                        /*  55: Reserved */
  0,                                        /*  56: Reserved */
  0,                                        /*  57: Reserved */
  0,                                        /*  58: Reserved */
  0,                                        /*  59: Reserved */
  0,                                        /*  60: Reserved */
  0,                                        /*  61: Reserved */
  0,                                        /*  62: Reserved */
  0,                                        /*  63: Reserved */
  0,                                        /*  64: Reserved */
  0,                                        /*  65: Reserved */
  0,                                        /*  66: Reserved */
  0,                                        /*  67: Reserved */
  GPIO0_IRQHandler,                         /*  68: GPIO 0 Combined Interrupt */
  GPIO1_IRQHandler,                         /*  69: GPIO 1 Combined Interrupt */
  GPIO2_IRQHandler,                         /*  70: GPIO 2 Combined Interrupt */
  GPIO3_IRQHandler,                         /*  71: GPIO 3 Combined Interrupt */
  GPIO0_0_IRQHandler,                       /*  72: GPIO 0 individual interrupt 0 */
  GPIO0_1_IRQHandler,                       /*  73: GPIO 0 individual interrupt 1 */
  GPIO0_2_IRQHandler,                       /*  74: GPIO 0 individual interrupt 2 */
  GPIO0_3_IRQHandler,                       /*  75: GPIO 0 individual interrupt 3 */
  GPIO0_4_IRQHandler,                       /*  76: GPIO 0 individual interrupt 4 */
  GPIO0_5_IRQHandler,                       /*  77: GPIO 0 individual interrupt 5 */
  GPIO0_6_IRQHandler,                       /*  78: GPIO 0 individual interrupt 6 */
  GPIO0_7_IRQHandler,                       /*  79: GPIO 0 individual interrupt 7 */
  GPIO0_8_IRQHandler,                       /*  80: GPIO 0 individual interrupt 8 */
  GPIO0_9_IRQHandler,                       /*  81: GPIO 0 individual interrupt 9 */
  GPIO0_10_IRQHandler,                      /*  82: GPIO 0 individual interrupt 10 */
  GPIO0_11_IRQHandler,                      /*  83: GPIO 0 individual interrupt 11 */
  GPIO0_12_IRQHandler,                      /*  84: GPIO 0 individual interrupt 12 */
  GPIO0_13_IRQHandler,                      /*  85: GPIO 0 individual interrupt 13 */
  GPIO0_14_IRQHandler,                      /*  86: GPIO 0 individual interrupt 14 */
  GPIO0_15_IRQHandler,                      /*  87: GPIO 0 individual interrupt 15 */
  GPIO1_0_IRQHandler,                       /*  88: GPIO 1 individual interrupt 0 */
  GPIO1_1_IRQHandler,                       /*  89: GPIO 1 individual interrupt 1 */
  GPIO1_2_IRQHandler,                       /*  90: GPIO 1 individual interrupt 2 */
  GPIO1_3_IRQHandler,                       /*  91: GPIO 1 individual interrupt 3 */
  GPIO1_4_IRQHandler,                       /*  92: GPIO 1 individual interrupt 4 */
  GPIO1_5_IRQHandler,                       /*  93: GPIO 1 individual interrupt 5 */
  GPIO1_6_IRQHandler,                       /*  94: GPIO 1 individual interrupt 6 */
  GPIO1_7_IRQHandler,                       /*  95: GPIO 1 individual interrupt 7 */
  GPIO1_8_IRQHandler,                       /*  96: GPIO 1 individual interrupt 8 */
  GPIO1_9_IRQHandler,                       /*  97: GPIO 1 individual interrupt 9 */
  GPIO1_10_IRQHandler,                      /*  98: GPIO 1 individual interrupt 10 */
  GPIO1_11_IRQHandler,                      /*  99: GPIO 1 individual interrupt 11 */
  GPIO1_12_IRQHandler,                      /* 100: GPIO 1 individual interrupt 12 */
  GPIO1_13_IRQHandler,                      /* 101: GPIO 1 individual interrupt 13 */
  GPIO1_14_IRQHandler,                      /* 102: GPIO 1 individual interrupt 14 */
  GPIO1_15_IRQHandler,                      /* 103: GPIO 1 individual interrupt 15 */
  GPIO2_0_IRQHandler,                       /* 104: GPIO 2 individual interrupt 0 */
  GPIO2_1_IRQHandler,                       /* 105: GPIO 2 individual interrupt 1 */
  GPIO2_2_IRQHandler,                       /* 106: GPIO 2 individual interrupt 2 */
  GPIO2_3_IRQHandler,                       /* 107: GPIO 2 individual interrupt 3 */
  GPIO2_4_IRQHandler,                       /* 108: GPIO 2 individual interrupt 4 */
  GPIO2_5_IRQHandler,                       /* 109: GPIO 2 individual interrupt 5 */
  GPIO2_6_IRQHandler,                       /* 110: GPIO 2 individual interrupt 6 */
  GPIO2_7_IRQHandler,                       /* 111: GPIO 2 individual interrupt 7 */
  GPIO2_8_IRQHandler,                       /* 112: GPIO 2 individual interrupt 8 */
  GPIO2_9_IRQHandler,                       /* 113: GPIO 2 individual interrupt 9 */
  GPIO2_10_IRQHandler,                      /* 114: GPIO 2 individual interrupt 10 */
  GPIO2_11_IRQHandler,                      /* 115: GPIO 2 individual interrupt 11 */
  GPIO2_12_IRQHandler,                      /* 116: GPIO 2 individual interrupt 12 */
  GPIO2_13_IRQHandler,                      /* 117: GPIO 2 individual interrupt 13 */
  GPIO2_14_IRQHandler,                      /* 118: GPIO 2 individual interrupt 14 */
  GPIO2_15_IRQHandler,                      /* 119: GPIO 2 individual interrupt 15 */
  GPIO3_0_IRQHandler,                       /* 120: GPIO 3 individual interrupt 0 */
  GPIO3_1_IRQHandler,                       /* 121: GPIO 3 individual interrupt 1 */
  GPIO3_2_IRQHandler,                       /* 122: GPIO 3 individual interrupt 2 */
  GPIO3_3_IRQHandler,                       /* 123: GPIO 3 individual interrupt 3 */
  UART5RX_IRQHandler,                       /* 124: UART 5 Receive Interrupt */
  UART5TX_IRQHandler,                       /* 125: UART 5 Transmit Interrupt */
  UART5_IRQHandler,                         /* 126: UART 5 Combined Interrupt */
  HDCLCD_IRQHandler                         /* 127: HDCLCD Interrupt */
};

#if defined ( __GNUC__ )
#pragma GCC diagnostic pop
#endif

/*----------------------------------------------------------------------------
  Reset Handler called on controller reset
 *----------------------------------------------------------------------------*/
void __NO_RETURN Reset_Handler(void)
{
  __set_MSPLIM((uint32_t)(&__STACK_LIMIT));

  SystemInit();                             /* CMSIS System Initialization */
  __PROGRAM_START();                        /* Enter PreMain (C library entry point) */
}
