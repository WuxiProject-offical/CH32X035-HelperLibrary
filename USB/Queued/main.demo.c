/*
 *  USB Queued process Demo
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

#include "ch32x035.h"
#include <ch32x035_usbfs_device.h>
#include "debug.h"

#include "usbqueue.h"

uint8_t USBQueue_UserProcessor(uint8_t *inData,
		uint8_t inLen, uint8_t *outData)
{
	// This simply return received data.
	uint8_t outLen = 0;
	outLen=inLen;
	memcpy(outData,inData,inLen);
	return outLen;
}

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	SystemCoreClockUpdate();
	Delay_Init();
	USART_Printf_Init(460800);
	printf("SystemClk:%d\r\n", SystemCoreClock);
	printf("ChipID:%08x\r\n", DBGMCU_GetCHIPID());
	{ // reset USB bus
		AFIO->CTLR = AFIO->CTLR & ~(UDP_PUE_MASK | UDM_PUE_MASK | USB_IOEN);
		Delay_Ms(10);
	}

	USBFS_RCC_Init();
	USBFS_Device_Init(ENABLE, PWR_VDD_SupplyVoltage());
	NVIC_EnableIRQ(USBFS_IRQn);

	while(1)
	{
		USBQueue_DoProcess();
	}
}
