/*
 * DAC128S085.h
 *
 *  Created on: Jun 11, 2024
 *      Author: ryeonghwi
 */

#ifndef _DAC128S085_H_
#define _DAC128S085_H_

#include "main.h"
#include "stm32g4xx.h"

#define DAC_VREF	2.50f

// b1010 - Update output of selected channel
#define UPDATE_SEL  0xA000

#define DAC_CHA ((uint16_t)0x0000)
#define DAC_CHB ((uint16_t)0x1000)
#define DAC_CHC ((uint16_t)0x2000)
#define DAC_CHD ((uint16_t)0x3000)
#define DAC_CHE ((uint16_t)0x4000)
#define DAC_CHF ((uint16_t)0x5000)
#define DAC_CHG ((uint16_t)0x6000)
#define DAC_CHH ((uint16_t)0x7000)

// DAC mode commands [15:12]
#define WRM_MODE    ((uint16_t)0x8000)  // b1000 - Write to registers only mode
#define WTM_MODE    ((uint16_t)0x9000)  // b1011 - Write through to output mode

// DAC special write commands [15:12]
#define CHA_WRITE   ((uint16_t)0xB000)  // b1011 - Write through for chA, other outputs updated from registers
#define BROADCAST   ((uint16_t)0xC000)  // b1100 - Write through value to all outputs

// Command formatting
#define DAC_CH_MASK     0x7
#define DAC_CH_SHIFT    12  // DAC number in Bits [14:12] of 16-bit command
#define DAC_CH_MAX      8

#define DAC_VAL_MASK    0xFFF  // Lowest 12 bits of the 16-bit command
#define DAC_VAL_MAX     4095


#define softwareSPI

#ifdef  softwareSPI
#define DAC128S085_SYNC_HIGH()			HAL_GPIO_WritePin(SYNC_GPIO_Port, SYNC_Pin, GPIO_PIN_SET)
#define DAC128S085_SYNC_LOW()			HAL_GPIO_WritePin(SYNC_GPIO_Port, SYNC_Pin, GPIO_PIN_RESET)

#define DAC128S085_SCLK_HIGH()			HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, GPIO_PIN_SET)
#define DAC128S085_SCLK_LOW()			HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, GPIO_PIN_RESET)

#define DAC128S085_DIN_HIGH()			HAL_GPIO_WritePin(DIN_GPIO_Port, DIN_Pin, GPIO_PIN_SET)
#define DAC128S085_DIN_LOW()			HAL_GPIO_WritePin(DIN_GPIO_Port, DIN_Pin, GPIO_PIN_RESET)

#else

extern SPI_HandleTypeDef hspi1;
// HAL SPI Specific
#define SPI_TIMEOUT_MS  10
#define SPI_TX_SIZE     1  // 1 * 16-bit SPI transfer



#endif

extern void DAC_128S085_Init();
extern void DAC128S085_WriteDACData3( uint16_t DAC_Ch, uint16_t Code );
extern void DAC128S085_WriteDACData( uint16_t DAC_Ch, uint16_t Code1, uint16_t Code2, uint16_t Code3 );
#endif /* INC_DAC128S085_H_ */
