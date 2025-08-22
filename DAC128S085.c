/*
 * DAC128S085.c		12_bit resolution DAC
 *
 *  Created on: Jun 11, 2024
 *      Author: pyxis
 *
 *      Driver for TI 8-Channel 12-bit DAC (DAC128S085)
 *    - Assumes power-on default of WRM mode (register write doesn't enable output)
 *    - Assumes SPI setup using STM32CubeMX:
 *      - Motorola, 16-bits, MSB first
 *      - Clock Polarity (CPOL) - Low
 *      - Clock Phase (CPHA) - 1 Edge
 *      - NSS Mode (Chip Select) - Enabled, Signal Type - Hardware
 *
 *   -------- Example usage --------
 *
 *  // Set channel A(0) register to maximum value and enable output
 *  E_DAC_STATUS dacStatus = DAC128S085_DirectOutput(0, 4095);
 *
 *  // Set channel H(7) register to half rail, no output enabled
 *  E_DAC_STATUS dacStatus = DAC128S085_SetChannelReg(7, 2048);
 *
 *  // Enable outputs for channels A, B, C
 *  uint16_t chBits = (UPDATE_SEL | CH_A_EN | CH_B_EN | CH_C_EN);
 *  E_DAC_STATUS dacStatus = DAC128S085_SetChannelOutputs(chBits);
 *
 *  VOUTA,B,C,D = VREF1 × (D / 4096)
 *  VOUTE,F,G,H = VREF2 × (D / 4096)
 */


#include "DAC128S085.h"

void DAC128S085_SendData(uint8_t data)
{
	for(uint8_t i = 0; i<8 ; i++)
	{
		if(data & 0x80)
		{
			DAC128S085_DIN_HIGH();
			delay_us(1);
		}
		else
		{
			DAC128S085_DIN_LOW();
			delay_us(1);
		}

		DAC128S085_SCLK_HIGH();
		data <<= 1;
		DAC128S085_SCLK_LOW();
	}
}

void DAC128S085_SendCommand(unsigned int data)
{
	uint8_t Hdata,Ldata;

	Hdata=data>>8;
	Ldata=data&255;

	/*	precondition	SYNC = CS */
	DAC128S085_SYNC_LOW();
	DAC128S085_SendData(Hdata);
	DAC128S085_SendData(Ldata);
	DAC128S085_SYNC_HIGH();
	delay_us(2);
}


void DAC128S085_SendCommand3(unsigned int data,unsigned int data2,unsigned int data3)
{
/*								CAUTION
	Observe that the data destined for the Daisy Chained devices (1)… (N) appears
	in reverse order from the actual physical device arrangement;
	This simply arises from the fact that the first data word has to travel the
	farthest down the chain. However, the bit order within each data
	word is not reversed: it is still MSB first.
*/
	uint8_t Hdata,Ldata;
	uint8_t Hdata2,Ldata2;
	uint8_t Hdata3,Ldata3;

	Hdata = data>>8;
	Ldata = data&255;

	Hdata2 = data2>>8;
	Ldata2 = data2&255;

	Hdata3 = data3>>8;
	Ldata3 = data3&255;

	/*	precondition	SYNC = CS */
	DAC128S085_SYNC_LOW();

	DAC128S085_SendData(Hdata);
	DAC128S085_SendData(Ldata);

	DAC128S085_SendData(Hdata2);
	DAC128S085_SendData(Ldata2);

	DAC128S085_SendData(Hdata3);
	DAC128S085_SendData(Ldata3);

	DAC128S085_SYNC_HIGH();
	delay_us(2);
}

/*====================================================================================================*
**函數 : DAC128S085_ChangeMode
**功能 : 設定工作模式
**使用 : DAC128S085_ChangeMode(DAC_MODE_WTM);
====================================================================================================*/


void DAC128S085_ChangeMode( uint16_t OpMode )
{
	DAC128S085_SendCommand3(OpMode,OpMode,OpMode);
}

/*====================================================================================================*/
/*====================================================================================================*
**函數 : DAC128S085_WriteDACData
**功能 : Write DAC Data
**輸入 : WriteCmd
**輸出 : None
**使用 : DAC128S085_WriteDACData(DAC_CH1, 1024);
**====================================================================================================*/
/*====================================================================================================*/

void DAC128S085_WriteDACData3( uint16_t DAC_Ch, uint16_t Code )
{
	uint16_t data = DAC_Ch | Code;
	DAC128S085_SendCommand3(data, data, data);
}


void DAC128S085_WriteDACData( uint16_t DAC_Ch, uint16_t Code1, uint16_t Code2, uint16_t Code3 )
{
	uint16_t data1 = DAC_Ch | Code1;
	uint16_t data2 = DAC_Ch | Code2;
	uint16_t data3 = DAC_Ch | Code3;

	DAC128S085_SendCommand3(data1,data2,data3);
}



void DAC_128S085_Init()
{
	DAC128S085_ChangeMode(WTM_MODE);
	DAC128S085_WriteDACData3(DAC_CHA, 0);
	DAC128S085_WriteDACData3(DAC_CHB, 0);
	DAC128S085_WriteDACData3(DAC_CHC, 0);
	DAC128S085_WriteDACData3(DAC_CHD, 0);
	DAC128S085_WriteDACData3(DAC_CHE, 0);
	DAC128S085_WriteDACData3(DAC_CHF, 0);
	DAC128S085_WriteDACData3(DAC_CHG, 0);
	DAC128S085_WriteDACData3(DAC_CHH, 0);
}



