/*
 *  HID Keyboard and Mouse library Demo
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

#include <ch32x035_usbfs_device.demo.h>
#include <hid_km.h>
#include "debug.h"

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

	while(!USBFS_DevEnumStatus)
	{	;}
	Delay_Ms(1000);
	HID_KB_ClickKey(0, HID_KBKeyCode_End);
	HID_KB_TypeString("\n// CH32X033 KeyTest.\n", 32);
	Delay_Ms(500);
	HID_MS_Move(100, 0);
	Delay_Ms(500);
	HID_MS_Move(-100, 0);

	while(1)
	{
		;
	}
}