/*
 *  SPI Middleware for CH32X035
 *  Copyright (C) 2022-2024  WuxiProject
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "spi.h"

void SPI_QuickInit()
{
	// GPIO init
	GPIO_InitTypeDef GPIO_InitStructure =
	{ 0 };
#if SPI_REMAP==SPI_REMAP_1 // SCK/PB15, MISO/PA8, MOSI/PA9
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOB| RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_PartialRemap1_SPI1, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
#if SPI_DIR == SPI_DIR_2LINE_RX || SPI_DIR == SPI_DIR_1LINE_RX
	;
#elif SPI_DIR == SPI_DIR_1LINE_TX || SPI_DIR == SPI_DIR_2LINE_FULLDUPLEX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
#else
#error SPI_DIR not defined correctly.
#endif
#if SPI_DIR == SPI_DIR_2LINE_RX || SPI_DIR == SPI_DIR_1LINE_RX || SPI_DIR == SPI_DIR_2LINE_FULLDUPLEX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif
#elif SPI_REMAP==SPI_REMAP_2 // SCK/PA11, MISO/PA9, MOSI/PA10
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_PartialRemap2_SPI1, ENABLE);

#if SPI_DIR == SPI_DIR_2LINE_RX || SPI_DIR == SPI_DIR_1LINE_RX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11
#elif SPI_DIR == SPI_DIR_1LINE_TX || SPI_DIR == SPI_DIR_2LINE_FULLDUPLEX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_10;
#else
#error SPI_DIR not defined correctly.
#endif
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
#if SPI_DIR == SPI_DIR_2LINE_RX || SPI_DIR == SPI_DIR_1LINE_RX || SPI_DIR == SPI_DIR_2LINE_FULLDUPLEX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif
#elif SPI_REMAP==SPI_REMAP_3 // SCK/PC5, MISO/PC6, MOSI/PC7
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_FullRemap_SPI1, ENABLE);

#if SPI_DIR == SPI_DIR_2LINE_RX || SPI_DIR == SPI_DIR_1LINE_RX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5
#elif SPI_DIR == SPI_DIR_1LINE_TX || SPI_DIR == SPI_DIR_2LINE_FULLDUPLEX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
#else
#error SPI_DIR not defined correctly.
#endif
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
#if SPI_DIR == SPI_DIR_2LINE_RX || SPI_DIR == SPI_DIR_1LINE_RX || SPI_DIR == SPI_DIR_2LINE_FULLDUPLEX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
#endif
#elif SPI_REMAP==SPI_REMAP_NONE // SCK/PA5, MISO/PA6, MOSI/PA7
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

#if SPI_DIR == SPI_DIR_2LINE_RX || SPI_DIR == SPI_DIR_1LINE_RX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5
#elif SPI_DIR == SPI_DIR_1LINE_TX ||  SPI_DIR == SPI_DIR_2LINE_FULLDUPLEX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
#else
#error SPI_DIR not defined correctly.
#endif
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
#if SPI_DIR == SPI_DIR_2LINE_RX || SPI_DIR == SPI_DIR_1LINE_RX || SPI_DIR == SPI_DIR_2LINE_FULLDUPLEX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif
#else
#error SPI_REMAP not defined correctly.
#endif

	// SPI init
	SPI_InitTypeDef SPI_InitStructure =
	{ 0 };
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
#if SPI_DIR == SPI_DIR_2LINE_RX
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_RxOnly;
#elif SPI_DIR == SPI_DIR_1LINE_RX
	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Rx;
#elif SPI_DIR == SPI_DIR_1LINE_TX
	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
#elif SPI_DIR == SPI_DIR_2LINE_FULLDUPLEX
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
#else
#error SPI_DIR not defined correctly.
#endif
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CLKPOL;
	SPI_InitStructure.SPI_CPHA = SPI_CLKEDGE;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_CLKPSC;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);
	SPI_Cmd(SPI1, ENABLE);
}

uint8_t SPI_TinyTransaction(uint8_t dataTx)
{
	uint32_t timeout = SPI_RETRY_TIMEOUT;
	while (RESET == SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE))
	{
		if (--timeout == 0)
		break;
	}
	SPI_I2S_SendData(SPI1, dataTx);
	timeout = SPI_RETRY_TIMEOUT;
	while (RESET == SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE))
	{
		if (--timeout == 0)
		break;
	}
	return SPI_I2S_ReceiveData(SPI1);
}

void SPI_HalfDuplexTransaction(uint8_t *dataTx, uint16_t cntTx, uint8_t *dataRx,
		uint16_t cntRx)
{
	for (uint16_t i = 0; i < cntTx; i++)
		(void) SPI_TinyTransaction(dataTx[i]);
	for (uint16_t i = 0; i < cntRx; i++)
		dataRx[i] = SPI_TinyTransaction(0xff);

}

void SPI_FullDuplexTransaction(uint8_t *dataTx, uint8_t *dataRx, uint16_t cnt)
{
	for (uint16_t i = 0; i < cnt; i++)
		dataRx[i] = SPI_TinyTransaction(dataTx[i]);
}

uint32_t SPI_WaitForTransaction()
{
	uint32_t timeout = SPI_RETRY_TIMEOUT;
	while (SET == SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY))
	{
		if (--timeout == 0)
		break;
	}
	return timeout;
}

