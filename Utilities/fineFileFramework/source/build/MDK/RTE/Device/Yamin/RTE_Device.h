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
 * Project:      RTE Device Configuration for ARM IOTKIT device
 * Revision:     V1.0.0
 * -------------------------------------------------------------------------- */

//-------- <<< Use Configuration Wizard in Context Menu >>> --------------------

#ifndef RTE_DEVICE_H_
#define RTE_DEVICE_H_

// <e> USART0 (Universal synchronous asynchronous receiver transmitter) [Driver_USART0]
// <i> Configuration settings for Driver_USART0 in component ::CMSIS Driver:USART
#define RTE_USART0                      0

//   <o> USART0_RX Pin <0=>Not Used <1=>GPIO#_#
#define RTE_USART0_RX_ID                0
#if    (RTE_USART0_RX_ID == 0)
#define RTE_USART0_RX                   0
#elif  (RTE_USART0_RX_ID == 1)
#define RTE_USART0_RX                   1
#else
#error "Invalid USART0_RX Pin Configuration!"
#endif

//   <o> USART0_TX Pin <0=>Not Used <1=>GPIO#_#
#define RTE_USART0_TX_ID                0
#if    (RTE_USART0_TX_ID == 0)
#define RTE_USART0_TX                   0
#elif  (RTE_USART0_TX_ID == 1)
#define RTE_USART0_TX                   1
#else
#error "Invalid USART0_TX Pin Configuration!"
#endif

// </e>


// <e> USART1 (Universal synchronous asynchronous receiver transmitter) [Driver_USART1]
// <i> Configuration settings for Driver_USART1 in component ::CMSIS Driver:USART
#define RTE_USART1                      0

//   <o> USART1_RX Pin <0=>Not Used <1=>GPIO#_#
#define RTE_USART1_RX_ID                0
#if    (RTE_USART1_RX_ID == 0)
#define RTE_USART1_RX                   0
#elif  (RTE_USART1_RX_ID == 1)
#define RTE_USART1_RX                   1
#else
#error "Invalid USART1_RX Pin Configuration!"
#endif

//   <o> USART1_TX Pin <0=>Not Used <1=>GPIO#_#
#define RTE_USART1_TX_ID                0
#if    (RTE_USART1_TX_ID == 0)
#define RTE_USART1_TX                   0
#elif  (RTE_USART1_TX_ID == 1)
#define RTE_USART1_TX                   1
#else
#error "Invalid USART1_TX Pin Configuration!"
#endif

// </e>


// <e> USART2 (Universal synchronous asynchronous receiver transmitter) [Driver_USART2]
// <i> Configuration settings for Driver_USART2 in component ::CMSIS Driver:USART
#define RTE_USART2                      0

//   <o> USART2_RX Pin <0=>Not Used <1=>GPIO#_#
#define RTE_USART2_RX_ID                0
#if    (RTE_USART2_RX_ID == 0)
#define RTE_USART2_RX                   0
#elif  (RTE_USART2_RX_ID == 1)
#define RTE_USART2_RX                   1
#else
#error "Invalid USART2_RX Pin Configuration!"
#endif

//   <o> USART2_TX Pin <0=>Not Used <1=>GPIO#_#
#define RTE_USART2_TX_ID                0
#if    (RTE_USART2_TX_ID == 0)
#define RTE_USART2_TX                   0
#elif  (RTE_USART2_TX_ID == 1)
#define RTE_USART2_TX                   1
#else
#error "Invalid USART2_TX Pin Configuration!"
#endif

// </e>


// <e> USART3 (Universal synchronous asynchronous receiver transmitter) [Driver_USART3]
// <i> Configuration settings for Driver_USART3 in component ::CMSIS Driver:USART
#define RTE_USART3                      0

//   <o> USART3_RX Pin <0=>Not Used <1=>GPIO#_#
#define RTE_USART3_RX_ID                0
#if    (RTE_USART3_RX_ID == 0)
#define RTE_USART3_RX                   0
#elif  (RTE_USART3_RX_ID == 1)
#define RTE_USART3_RX                   1
#else
#error "Invalid USART3_RX Pin Configuration!"
#endif

//   <o> USART3_TX Pin <0=>Not Used <1=>GPIO#_#
#define RTE_USART3_TX_ID                0
#if    (RTE_USART3_TX_ID == 0)
#define RTE_USART3_TX                   0
#elif  (RTE_USART3_TX_ID == 1)
#define RTE_USART3_TX                   1
#else
#error "Invalid USART3_TX Pin Configuration!"
#endif

// </e>


#endif  /* RTE_DEVICE_H_ */
