/*
 *  I2C Middleware for CH32X035(rev1+)
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

#include "i2c.h"
#include "ch32x035.h"

#ifdef DEBUG
#define DPRINT printf
#else
#define DPRINT
#endif

void I2C_BusInit()
{
	GPIO_InitTypeDef GPIO_InitStructure =
	{ 0 };
	I2C_InitTypeDef I2C_InitTSturcture =
	{ 0 };
	// Clock on
#if I2C_REMAP==I2C_REMAP_2 || I2C_REMAP==I2C_REMAP_3 || I2C_REMAP==I2C_REMAP_4 || I2C_REMAP==I2C_REMAP_5
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
#elif I2C_REMAP==I2C_REMAP_1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
#elif I2C_REMAP==I2C_REMAP_NONE
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
#else
#error I2C_REMAP not defined correctly.
#endif
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	// GPIO init
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
#if I2C_REMAP==I2C_REMAP_1
	GPIO_PinRemapConfig(GPIO_PartialRemap1_I2C1, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
#elif I2C_REMAP==I2C_REMAP_2
	GPIO_PinRemapConfig(GPIO_PartialRemap2_I2C1, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_16 | GPIO_Pin_17;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
#elif I2C_REMAP==I2C_REMAP_3
	GPIO_PinRemapConfig(GPIO_PartialRemap3_I2C1, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_18 | GPIO_Pin_19;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
#elif I2C_REMAP==I2C_REMAP_4
	GPIO_PinRemapConfig(GPIO_PartialRemap4_I2C1, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_16 | GPIO_Pin_17;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
#elif I2C_REMAP==I2C_REMAP_5
	GPIO_PinRemapConfig(GPIO_FullRemap_I2C1, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_18 | GPIO_Pin_19;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
#elif I2C_REMAP==I2C_REMAP_NONE
	GPIO_PinRemapConfig(GPIO_PartialRemap2_I2C1, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
#else
#error I2C_REMAP not defined correctly.
#endif
	// I2C periph init
	I2C_InitTSturcture.I2C_ClockSpeed = I2C_BUSFREQ;
	I2C_InitTSturcture.I2C_Mode = I2C_Mode_I2C;
	I2C_InitTSturcture.I2C_DutyCycle = (
	I2C_BUSFREQ > 100000 ? I2C_DutyCycle_16_9 : I2C_DutyCycle_2);
	I2C_InitTSturcture.I2C_OwnAddress1 = 0x01; // unused
	I2C_InitTSturcture.I2C_Ack = I2C_Ack_Enable;
	I2C_InitTSturcture.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_Init(I2C1, &I2C_InitTSturcture);
	I2C_Cmd(I2C1, ENABLE);
	I2C_AcknowledgeConfig(I2C1, ENABLE);
}

void I2C_BusReset(void)
{
	// I2C periph down
	I2C_SoftwareResetCmd(I2C1, ENABLE);
	I2C_SoftwareResetCmd(I2C1, DISABLE);
	I2C_DeInit(I2C1);
	// Force generate STOP sequence
	GPIO_InitTypeDef GPIO_InitStructure =
	{ 0 };
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
#if I2C_REMAP==I2C_REMAP_1
	GPIO_PinRemapConfig(GPIO_PartialRemap1_I2C1, DISABLE);
	GPIO_ResetBits(GPIOA, GPIO_Pin_13 | GPIO_Pin_14);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	GPIO_SetBits(GPIOA, GPIO_Pin_13);
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	GPIO_SetBits(GPIOA, GPIO_Pin_14);
#elif I2C_REMAP==I2C_REMAP_2
	GPIO_PinRemapConfig(GPIO_PartialRemap2_I2C1, DISABLE);
	GPIO_ResetBits(GPIOC, GPIO_Pin_16 | GPIO_Pin_17);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_16 | GPIO_Pin_17;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	GPIO_SetBits(GPIOC, GPIO_Pin_16);
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	GPIO_SetBits(GPIOC, GPIO_Pin_17);
#elif I2C_REMAP==I2C_REMAP_3
	GPIO_PinRemapConfig(GPIO_PartialRemap3_I2C1, DISABLE);
	GPIO_ResetBits(GPIOC, GPIO_Pin_18 | GPIO_Pin_19);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_18 | GPIO_Pin_19;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	GPIO_SetBits(GPIOC, GPIO_Pin_19);
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	GPIO_SetBits(GPIOC, GPIO_Pin_18);
#elif I2C_REMAP==I2C_REMAP_4
	GPIO_PinRemapConfig(GPIO_PartialRemap4_I2C1, DISABLE);
	GPIO_ResetBits(GPIOC, GPIO_Pin_16 | GPIO_Pin_17);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_16 | GPIO_Pin_17;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	GPIO_SetBits(GPIOC, GPIO_Pin_17);
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	GPIO_SetBits(GPIOC, GPIO_Pin_16);
#elif I2C_REMAP==I2C_REMAP_5
	GPIO_PinRemapConfig(GPIO_FulllRemap_I2C1, DISABLE);
	GPIO_ResetBits(GPIOC, GPIO_Pin_18 | GPIO_Pin_19);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_18 | GPIO_Pin_19;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	GPIO_SetBits(GPIOC, GPIO_Pin_18);
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	GPIO_SetBits(GPIOC, GPIO_Pin_19);
#elif I2C_REMAP==I2C_REMAP_NONE
	GPIO_ResetBits(GPIOA, GPIO_Pin_10 | GPIO_Pin_11);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	GPIO_SetBits(GPIOA, GPIO_Pin_10);
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	GPIO_SetBits(GPIOA, GPIO_Pin_11);
#else
#error I2C_REMAP not defined correctly.
#endif
	// reconfig I2C periph
	I2C_BusInit();
}

void I2C_WriteBytes(uint8_t devAddr, uint8_t *pData, uint16_t dataLen)
{
	volatile I2C_Stage state = I2C_START;
	volatile uint16_t retryCnt = 0;
	volatile uint8_t stopFlag = 0;

	// Auto ACK on
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	while(!(stopFlag))
	{
		switch(state)
		{
			case I2C_START:
			// Wait for bus becomes free
			while((I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) != RESET) && (retryCnt < I2C_RETRY_TIMEOUT))
			{
				retryCnt++;
			}
			if(retryCnt < I2C_RETRY_TIMEOUT)
			{
				// Generate START sequence
				I2C_GenerateSTART(I2C1, ENABLE);
				retryCnt = 0;
				state = I2C_SEND_ADDRESS;
			}
			else
			{
				I2C_BusReset(); // COMMENT THIS WHILE IN MULTI-HOST I2C
				retryCnt = 0;
				state = I2C_START;
				DPRINT("[I2C] bus is busy!\r\n");
			}
			break;

			case I2C_SEND_ADDRESS:
			// Wait until START sequence done
			while((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) && (retryCnt < I2C_RETRY_TIMEOUT))
			{
				retryCnt++;
			}
			if(retryCnt < I2C_RETRY_TIMEOUT)
			{
				// Address device
				I2C_Send7bitAddress(I2C1, (uint8_t)devAddr,I2C_Direction_Transmitter);
				retryCnt = 0;
				state = I2C_CLEAR_ADDRESS_FLAG;
			}
			else
			{
				retryCnt = 0;
				state = I2C_START;
				DPRINT("[I2C] START seq send timeout!\r\n");
			}
			break;

			case I2C_CLEAR_ADDRESS_FLAG:
			// Wait for device's ACK
			while((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) && (retryCnt < I2C_RETRY_TIMEOUT))
			{
				retryCnt++;
			}
			if(retryCnt < I2C_RETRY_TIMEOUT)
			{
				retryCnt = 0;
				state = I2C_TRANSMIT_DATA;
			}
			else
			{
				retryCnt = 0;
				state = I2C_START;
				DPRINT("[I2C] Device didn't ack.\r\n");
			}
			break;

			case I2C_TRANSMIT_DATA:
			for (uint16_t i = 0; i < dataLen; i++)
			{
				I2C_SendData(I2C1, *(pData + i));
				// Wait for last data transmission
				while((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) && (retryCnt < I2C_RETRY_TIMEOUT))
				{
					retryCnt++;
				}
				if(retryCnt < I2C_RETRY_TIMEOUT)
				{
					retryCnt = 0;
				}
				else
				{
					retryCnt = 0;
					state = I2C_START;
					DPRINT("[I2C] Host send timeout.\r\n");
				}
			}
			retryCnt = 0;
			state = I2C_STOP;
			break;

			case I2C_STOP:
			// Generate STOP sequence
			I2C_GenerateSTOP(I2C1, ENABLE);
			stopFlag = 1;
			retryCnt = 0;
			break;

			default:
			// Should not run to here.
			state = I2C_START;
			stopFlag = 1;
			retryCnt = 0;
			DPRINT("[I2C] Wrong state.\r\n");
			break;
		}
	}
}

void I2C_WriteReg8(uint8_t devAddr, uint8_t writeAddr, uint8_t *pData,
		uint16_t dataLen)
{
	volatile I2C_Stage state = I2C_START;
	volatile uint16_t retryCnt = 0;
	volatile uint8_t stopFlag = 0;

	// Auto ACK on
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	while(!(stopFlag))
	{
		switch(state)
		{
			case I2C_START:
			// Wait for bus becomes free
			while((I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) != RESET) && (retryCnt < I2C_RETRY_TIMEOUT))
			{
				retryCnt++;
			}
			if(retryCnt < I2C_RETRY_TIMEOUT)
			{
				// Generate START sequence
				I2C_GenerateSTART(I2C1, ENABLE);
				retryCnt = 0;
				state = I2C_SEND_ADDRESS;
			}
			else
			{
				I2C_BusReset(); // COMMENT THIS WHILE IN MULTI-HOST I2C
				retryCnt = 0;
				state = I2C_START;
				DPRINT("[I2C] bus is busy!\r\n");
			}
			break;

			case I2C_SEND_ADDRESS:
			// Wait until START sequence done
			while((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) && (retryCnt < I2C_RETRY_TIMEOUT))
			{
				retryCnt++;
			}
			if(retryCnt < I2C_RETRY_TIMEOUT)
			{
				// Address device
				I2C_Send7bitAddress(I2C1, (uint8_t)devAddr,I2C_Direction_Transmitter);
				retryCnt = 0;
				state = I2C_CLEAR_ADDRESS_FLAG;
			}
			else
			{
				retryCnt = 0;
				state = I2C_START;
				DPRINT("[I2C] START seq send timeout!\r\n");
			}
			break;

			case I2C_CLEAR_ADDRESS_FLAG:
			// Wait for device's ACK
			while((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) && (retryCnt < I2C_RETRY_TIMEOUT))
			{
				retryCnt++;
			}
			if(retryCnt < I2C_RETRY_TIMEOUT)
			{
				retryCnt = 0;
				state = I2C_TRANSMIT_DATA;
			}
			else
			{
				retryCnt = 0;
				state = I2C_START;
				DPRINT("[I2C] Device didn't ack.\r\n");
			}
			break;

			case I2C_TRANSMIT_DATA:
			// Wait for TX buffer becomes free
			while((!I2C_GetFlagStatus(I2C1, I2C_FLAG_TXE)) && (retryCnt < I2C_RETRY_TIMEOUT))
			{
				retryCnt++;
			}
			if(retryCnt < I2C_RETRY_TIMEOUT)
			{
				// Send Register addr
				I2C_SendData(I2C1, (uint8_t)writeAddr);
				retryCnt = 0;
			}
			else
			{
				retryCnt = 0;
				state = I2C_START;
				DPRINT("[I2C] Host TX buffer is busy while sending reg addr.\r\n");
			}
			// Wait for last data transmission
			while((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) && (retryCnt < I2C_RETRY_TIMEOUT))
			{
				retryCnt++;
			}
			if(retryCnt < I2C_RETRY_TIMEOUT)
			{
				retryCnt = 0;
			}
			else
			{
				retryCnt = 0;
				state = I2C_START;
				DPRINT("[I2C] Host send reg addr timeout.\r\n");
			}
			for (uint16_t i = 0; i < dataLen; i++)
			{
				I2C_SendData(I2C1, *(pData + i));
				// Wait for data transmission
				while((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) && (retryCnt < I2C_RETRY_TIMEOUT))
				{
					retryCnt++;
				}
				if(retryCnt < I2C_RETRY_TIMEOUT)
				{
					retryCnt = 0;
				}
				else
				{
					retryCnt = 0;
					state = I2C_START;
					DPRINT("[I2C] Host send timeout.\r\n");
				}
			}
			retryCnt = 0;
			state = I2C_STOP;
			break;

			case I2C_STOP:
			// Generate STOP sequence
			I2C_GenerateSTOP(I2C1, ENABLE);
			stopFlag = 1;
			retryCnt = 0;
			break;

			default:
			// Should not run to here.
			state = I2C_START;
			stopFlag = 1;
			retryCnt = 0;
			DPRINT("[I2C] Wrong state.\r\n");
			break;
		}
	}

}

void I2C_ReadReg8(uint8_t devAddr, uint8_t readAddr, uint8_t *pData,
		uint16_t dataLen)
{
	volatile I2C_Stage state = I2C_START;
	volatile uint8_t isReadPhase = 0;
	volatile uint16_t retryCnt = 0;
	volatile uint8_t stopFlag = 0;

	// Auto ACK on
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	while(!(stopFlag))
	{
		switch(state)
		{
			case I2C_START:
			if(RESET == isReadPhase) // Write Phase
			{
				// Wait for bus becomes free
				while((I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) != RESET) && (retryCnt < I2C_RETRY_TIMEOUT))
				{
					retryCnt++;
				}
				if(retryCnt < I2C_RETRY_TIMEOUT)
				{
					// Decide to send ACK or not for the next byte
					if(2 == dataLen)
					{
						I2C1->CTLR1|=I2C_CTLR1_POS; // ack next byte
					}
				}
				else
				{
					I2C_BusReset(); // COMMENT THIS WHILE IN MULTI-HOST I2C
					retryCnt = 0;
					state = I2C_START;
					DPRINT("[I2C] bus is busy!\r\n");
				}
			}
			// Generate START sequence
			I2C_GenerateSTART(I2C1, ENABLE);
			retryCnt = 0;
			state = I2C_SEND_ADDRESS;
			break;

			case I2C_SEND_ADDRESS:
			// Wait until START sequence done
			while((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) && (retryCnt < I2C_RETRY_TIMEOUT))
			{
				retryCnt++;
			}
			if(retryCnt < I2C_RETRY_TIMEOUT)
			{
				if(RESET == isReadPhase)
				{ // Write Phase
					I2C_Send7bitAddress(I2C1, (uint8_t)devAddr,I2C_Direction_Transmitter);
					state = I2C_CLEAR_ADDRESS_FLAG;
				}
				else
				{ // Read Phase
					I2C_Send7bitAddress(I2C1, (uint8_t)devAddr,I2C_Direction_Receiver);
					if(dataLen < 2)
					{
						// Auto ACK off
						I2C_AcknowledgeConfig(I2C1, DISABLE);
					}
					state = I2C_CLEAR_ADDRESS_FLAG;
				}
				retryCnt = 0;
			}
			else
			{
				retryCnt = 0;
				state = I2C_START;
				isReadPhase = 0;
				DPRINT("[I2C] START seq send timeout!\r\n");
			}
			break;

			case I2C_CLEAR_ADDRESS_FLAG:
			// Wait for device's ACK
			while((!I2C_GetFlagStatus(I2C1, I2C_FLAG_ADDR)) && (retryCnt < I2C_RETRY_TIMEOUT))
			{
				retryCnt++;
			}
			if(retryCnt < I2C_RETRY_TIMEOUT)
			{
				I2C_ClearFlag(I2C1, I2C_FLAG_ADDR);
				if((SET == isReadPhase) && (1 == dataLen))
				{
					// Generate STOP sequence at last byte
					I2C_GenerateSTOP(I2C1, ENABLE);
				}
				retryCnt = 0;
				state = I2C_TRANSMIT_DATA;
			}
			else
			{
				retryCnt = 0;
				state = I2C_START;
				isReadPhase = 0;
				DPRINT("[I2C] Device didn't ack.\r\n");
			}
			break;

			case I2C_TRANSMIT_DATA:
			if(RESET == isReadPhase) // Write Phase
			{
				// Wait for TX buffer becomes free
				while((!I2C_GetFlagStatus(I2C1, I2C_FLAG_TXE)) && (retryCnt < I2C_RETRY_TIMEOUT))
				{
					retryCnt++;
				}
				if(retryCnt < I2C_RETRY_TIMEOUT)
				{
					// Send Register addr
					I2C_SendData(I2C1, readAddr);
					retryCnt = 0;
				}
				else
				{
					retryCnt = 0;
					state = I2C_START;
					isReadPhase = 0;
					DPRINT("[I2C] Host TX buffer is busy while sending reg addr.\r\n");
				}
				// Wait for last data transmission
				while((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) && (retryCnt < I2C_RETRY_TIMEOUT))
				{
					retryCnt++;
				}
				if(retryCnt < I2C_RETRY_TIMEOUT)
				{
					retryCnt = 0;
					state = I2C_START;
					isReadPhase = 1;
				}
				else
				{
					retryCnt = 0;
					state = I2C_START;
					isReadPhase = 0;
					DPRINT("[I2C] Host send reg addr timeout.\r\n");
				}
			}
			else // Read Phase
			{
				while(dataLen)
				{
					if(1 == dataLen)
					{
						// Auto ACK off
						I2C_AcknowledgeConfig(I2C1, DISABLE);
						(void)I2C1->STAR2;
						// Generate STOP sequence at last byte
						I2C_GenerateSTOP(I2C1, ENABLE);
					}
					// Wait for recv done
					while((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))&& (retryCnt < I2C_RETRY_TIMEOUT))
					{
						retryCnt++;
					}
					if(retryCnt > I2C_RETRY_TIMEOUT)
					{
						retryCnt = 0;
						state = I2C_START;
						isReadPhase = 0;
						DPRINT("[I2C] Host receive timeout.\r\n");
					}
					else
					{
						// Take data to buffer
						*pData = I2C_ReceiveData(I2C1);
						pData++;
						dataLen--;
						retryCnt = 0;
					}
				}
				// Received all expected data. Generate STOP sequence
				I2C_GenerateSTOP(I2C1, ENABLE);
				retryCnt = 0;
				state = I2C_STOP;
			}
			break;

			case I2C_STOP:
			// Already generated STOP sequence, so just clear state
			retryCnt = 0;
			stopFlag = 1;
			break;

			default:
			// Should not run to here.
			state = I2C_START;
			isReadPhase = 0;
			retryCnt = 0;
			stopFlag = 1;
			DPRINT("[I2C] Wrong state.\r\n");
			break;
		}
	}
}
