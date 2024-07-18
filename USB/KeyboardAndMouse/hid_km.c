/*
 *  HID Keyboard and Mouse library
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

#include <hid_km.h>

// Portable implemention
// - You need to implement things below to integrate into your program.
// ##PORT_IMPLEMENTION_BEGIN##

#include <ch32x035_usbfs_device.h>
extern volatile uint8_t USBFS_Endp_Busy[];

// Define the endpoint for keyboard instance.
#define HID_KB_EP DEF_UEP1

// Define the endpoint for mouse instance.
#define HID_MS_EP DEF_UEP2

// Return busy state for given endpoint. 0 for idle, other for busy.
uint8_t HID_KM_GetEndpointBusy(uint8_t ep)
{
	return USBFS_Endp_Busy[ep];
}

// Prepare data for IN endpoint upload.
uint8_t HID_KM_EpUpload(uint8_t ep, uint8_t *buf, uint16_t len)
{
	return USBFS_Endp_DataUp(ep, buf, len, DEF_UEP_CPY_LOAD);
}

// This should be called while OUT transaction for keyboard has occured.
void HID_KB_EpOut_Callback(uint8_t *buf, uint16_t len){
	HID_KB_LED_Status = buf[0];
}

// ##PORT_IMPLEMENTION_END##

// Descriptors
/* Keyboard Report Descriptor */
const uint8_t KeyRepDesc[] =
{ 0x05, 0x01,                                    // Usage Page (Generic Desktop)
		0x09, 0x06,                                          // Usage (Keyboard)
		0xA1, 0x01,                                  // Collection (Application)
		0x05, 0x07,                                    // Usage Page (Key Codes)
		0x19, 0xE0,                                       // Usage Minimum (224)
		0x29, 0xE7,                                       // Usage Maximum (231)
		0x15, 0x00,                                       // Logical Minimum (0)
		0x25, 0x01,                                       // Logical Maximum (1)
		0x75, 0x01,                                           // Report Size (1)
		0x95, 0x08,                                          // Report Count (8)
		0x81, 0x02,                            // Input (Data,Variable,Absolute)
		0x95, 0x01,                                          // Report Count (1)
		0x75, 0x08,                                           // Report Size (8)
		0x81, 0x01,                                          // Input (Constant)
		0x95, 0x03,                                          // Report Count (3)
		0x75, 0x01,                                           // Report Size (1)
		0x05, 0x08,                                         // Usage Page (LEDs)
		0x19, 0x01,                                         // Usage Minimum (1)
		0x29, 0x03,                                         // Usage Maximum (3)
		0x91, 0x02,                           // Output (Data,Variable,Absolute)
		0x95, 0x05,                                          // Report Count (5)
		0x75, 0x01,                                           // Report Size (1)
		0x91, 0x01,                          // Output (Constant,Array,Absolute)
		0x95, 0x06,                                          // Report Count (6)
		0x75, 0x08,                                           // Report Size (8)
		0x26, 0xFF, 0x00,                               // Logical Maximum (255)
		0x05, 0x07,                                    // Usage Page (Key Codes)
		0x19, 0x00,                                         // Usage Minimum (0)
		0x29, 0x91,                                       // Usage Maximum (145)
		0x81, 0x00,                                // Input(Data,Array,Absolute)
		0xC0                                                  // End Collection
		};

/* Mouse Report Descriptor */
const uint8_t MouseRepDesc[] =
{ 0x05, 0x01,                                    // Usage Page (Generic Desktop)
		0x09, 0x02,                                             // Usage (Mouse)
		0xA1, 0x01,                                  // Collection (Application)
		0x09, 0x01,                                           // Usage (Pointer)
		0xA1, 0x00,                                     // Collection (Physical)
		0x05, 0x09,                                       // Usage Page (Button)
		0x19, 0x01,                                  // Usage Minimum (Button 1)
		0x29, 0x03,                                  // Usage Maximum (Button 3)
		0x15, 0x00,                                       // Logical Minimum (0)
		0x25, 0x01,                                       // Logical Maximum (1)
		0x75, 0x01,                                           // Report Size (1)
		0x95, 0x03,                                          // Report Count (3)
		0x81, 0x02,                            // Input (Data,Variable,Absolute)
		0x75, 0x05,                                           // Report Size (5)
		0x95, 0x01,                                          // Report Count (1)
		0x81, 0x01,                           // Input (Constant,Array,Absolute)
		0x05, 0x01,                              // Usage Page (Generic Desktop)
		0x09, 0x30,                                             // Usage (X)
		0x09, 0x31,                                             // Usage (Y)
		0x09, 0x38,                                             // Usage (Wheel)
		0x15, 0x81,                                    // Logical Minimum (-127)
		0x25, 0x7F,                                     // Logical Maximum (127)
		0x75, 0x08,                                           // Report Size (8)
		0x95, 0x03,                                          // Report Count (3)
		0x81, 0x06,                            // Input (Data,Variable,Relative)
		0xC0,                                                  // End Collection
		0xC0                                                  // End Collection
		};

// Keyboard Variables
volatile uint8_t KB_Packet_Buf[8] =
{ 0x00 };
volatile uint8_t HID_KB_LED_Status = 0x00;
// Mouse Variables
volatile uint8_t MS_Packet_Buf[4] =
{ 0x00 };

// Keyboard Functions
static uint8_t HID_KB_UploadPacket()
{
	while(HID_KM_GetEndpointBusy(HID_KB_EP))
	{	;}
	return READY
			== (HID_KM_EpUpload(HID_KB_EP, KB_Packet_Buf, sizeof(KB_Packet_Buf)));
}

void HID_KB_KeyDown(HID_KBKeyCode key)
{
	uint8_t i;
	for (i = 2; i < 8; i++)
	{                                                  // look for unused slot
		if (KB_Packet_Buf[i] == 0x00)
		{
			KB_Packet_Buf[i] = (uint8_t) key;
			break;
		}
	}
	if (i == 8)
	{
		// no slot.
		;
	}
	else
	{
		HID_KB_UploadPacket();
	}
}

void HID_KB_KeyUp(HID_KBKeyCode key)
{
	uint8_t i;
	for (i = 2; i < 8; i++)
	{                                                  // look for used slot
		if (KB_Packet_Buf[i] == (uint8_t) key)
		{
			KB_Packet_Buf[i] = 0x00;
			break;
		}
	}
	if (i == 8)
	{
		// not pressed at all.
		;
	}
	else
	{
		HID_KB_UploadPacket();
	}
}

void HID_KB_KeyAllUp(void)
{
	KB_Packet_Buf[2] = 0x00;
	KB_Packet_Buf[3] = 0x00;
	KB_Packet_Buf[4] = 0x00;
	KB_Packet_Buf[5] = 0x00;
	KB_Packet_Buf[6] = 0x00;
	KB_Packet_Buf[7] = 0x00;
	HID_KB_UploadPacket();
}

void HID_KB_ModKeyDown(uint8_t modkey)
{
	KB_Packet_Buf[0] |= (modkey);
	HID_KB_UploadPacket();
}

void HID_KB_ModKeyUp(uint8_t modkey)
{
	KB_Packet_Buf[0] &= (~(modkey));
	HID_KB_UploadPacket();
}

void HID_KB_ModKeyAllUp(void)
{
	KB_Packet_Buf[0] = 0x00;
	HID_KB_UploadPacket();
}

void HID_KB_ClickKey(uint8_t modkey, HID_KBKeyCode key)
{
	if (!modkey && !key)
		return;
	if (modkey)
		HID_KB_ModKeyDown(modkey);
	HID_KB_KeyDown(key);
	HID_KB_KeyUp(key);
	if (modkey)
		HID_KB_ModKeyUp(modkey);
}

void HID_KB_TypeString(uint8_t *str, uint16_t len)
{
	for (uint16_t i = 0; i < len; i++)
	{
		if (str[i] & 0x80)
			continue; // skip non-ASCII char
		if (str[i] == 0x00)
			break; // end
		uint8_t modkey = 0x00;
		HID_KBKeyCode keycode = 0x00;
		if (str[i] >= 0x31 && str[i] <= 0x39)
		{ // num1-9
			keycode =
					(HID_KBKeyCode) (HID_KBKeyCode_1_Excalm + (str[i] - 0x31));
		}
		else if (str[i] == 0x30)
		{ // num0
			keycode = HID_KBKeyCode_0_RRndBracket;
		}
		else if (str[i] >= 0x61 && str[i] <= 0x7a)
		{ // lowerCase letter
			modkey =
					HID_KB_CheckLEDStatus(HID_KBLed_Caps) ?
							HID_KBModKey_LShift : 0x00;
			keycode = (HID_KBKeyCode) (HID_KBKeyCode_A + (str[i] - 0x61));
		}
		else if (str[i] >= 0x41 && str[i] <= 0x5a)
		{ // upperCase letter
			modkey =
					HID_KB_CheckLEDStatus(HID_KBLed_Caps) ?
							0x00 : HID_KBModKey_LShift;
			keycode = (HID_KBKeyCode) (HID_KBKeyCode_A + (str[i] - 0x41));
		}
		else
		{
			switch (str[i])
			{
			// controls
			case 0x08: // Backspace
				keycode = HID_KBKeyCode_Backspace;
				break;
			case 0x09: // H-Tab
				keycode = HID_KBKeyCode_Tab;
				break;
			case 0x0a: // Newline
				keycode = HID_KBKeyCode_Enter;
				break;
			case 0x1b: // Escape
				keycode = HID_KBKeyCode_ESC;
				break;
			case 0x7f: // Del
				keycode = HID_KBKeyCode_Del;
				break;
				// chars
			case ' ':
				keycode = HID_KBKeyCode_Space;
				break;
			case '!':
				modkey = HID_KBModKey_LShift;
				keycode = HID_KBKeyCode_1_Excalm;
				break;
			case '"':
				modkey = HID_KBModKey_LShift;
				keycode = HID_KBKeyCode_Quote_DQuote;
				break;
			case '#':
				modkey = HID_KBModKey_LShift;
				keycode = HID_KBKeyCode_3_Sharp;
				break;
			case '$':
				modkey = HID_KBModKey_LShift;
				keycode = HID_KBKeyCode_4_Dollar;
				break;
			case '%':
				modkey = HID_KBModKey_LShift;
				keycode = HID_KBKeyCode_5_Percent;
				break;
			case '&':
				modkey = HID_KBModKey_LShift;
				keycode = HID_KBKeyCode_7_And;
				break;
			case '\'':
				keycode = HID_KBKeyCode_Quote_DQuote;
				break;
			case '(':
				modkey = HID_KBModKey_LShift;
				keycode = HID_KBKeyCode_9_LRndBracket;
				break;
			case ')':
				modkey = HID_KBModKey_LShift;
				keycode = HID_KBKeyCode_0_RRndBracket;
				break;
			case '*':
				modkey = HID_KBModKey_LShift;
				keycode = HID_KBKeyCode_8_Asterisk;
				break;
			case '+':
				modkey = HID_KBModKey_LShift;
				keycode = HID_KBKeyCode_Equal_Plus;
				break;
			case ',':
				keycode = HID_KBKeyCode_Comma_Less;
				break;
			case '-':
				keycode = HID_KBKeyCode_Minus_Underscore;
				break;
			case '.':
				keycode = HID_KBKeyCode_Dot_More;
				break;
			case '/':
				keycode = HID_KBKeyCode_Slash_Question;
				break;
			case ':':
				modkey = HID_KBModKey_LShift;
				keycode = HID_KBKeyCode_SemiColon_Colon;
				break;
			case ';':
				keycode = HID_KBKeyCode_SemiColon_Colon;
				break;
			case '<':
				modkey = HID_KBModKey_LShift;
				keycode = HID_KBKeyCode_Comma_Less;
				break;
			case '=':
				keycode = HID_KBKeyCode_Equal_Plus;
				break;
			case '>':
				modkey = HID_KBModKey_LShift;
				keycode = HID_KBKeyCode_Dot_More;
				break;
			case '?':
				modkey = HID_KBModKey_LShift;
				keycode = HID_KBKeyCode_Slash_Question;
				break;
			case '@':
				modkey = HID_KBModKey_LShift;
				keycode = HID_KBKeyCode_2_At;
				break;
			case '[':
				keycode = HID_KBKeyCode_LSqrBracket_LCurBracket;
				break;
			case '\\':
				keycode = HID_KBKeyCode_BackSlash_Bar;
				break;
			case ']':
				keycode = HID_KBKeyCode_RSqrBracket_RCurBracket;
				break;
			case '^':
				modkey = HID_KBModKey_LShift;
				keycode = HID_KBKeyCode_6_Caret;
				break;
			case '_':
				modkey = HID_KBModKey_LShift;
				keycode = HID_KBKeyCode_Minus_Underscore;
				break;
			case '`':
				keycode = HID_KBKeyCode_BackQuote_Tidle;
				break;
			case '{':
				modkey = HID_KBModKey_LShift;
				keycode = HID_KBKeyCode_LSqrBracket_LCurBracket;
				break;
			case '|':
				modkey = HID_KBModKey_LShift;
				keycode = HID_KBKeyCode_BackSlash_Bar;
				break;
			case '}':
				modkey = HID_KBModKey_LShift;
				keycode = HID_KBKeyCode_RSqrBracket_RCurBracket;
				break;
			case '~':
				modkey = HID_KBModKey_LShift;
				keycode = HID_KBKeyCode_BackQuote_Tidle;
				break;
			default:
				break;
			}
		}
		HID_KB_ClickKey(modkey, keycode);
	}
	HID_KB_KeyAllUp();
}

uint8_t HID_KB_CheckLEDStatus(HID_KBLed led)
{
	switch (led)
	{
	case HID_KBLed_Nums:
		return (HID_KB_LED_Status & 0x01) ? 1 : 0;
		break;
	case HID_KBLed_Caps:
		return (HID_KB_LED_Status & 0x02) ? 1 : 0;
		break;
	case HID_KBLed_Scroll:
		return (HID_KB_LED_Status & 0x04) ? 1 : 0;
		break;
	default:
		return 0;
	}
}

// Mouse Functions
static uint8_t HID_MS_UploadPacket()
{
	while(HID_KM_GetEndpointBusy(HID_MS_EP))
	{	;}
	return READY
			== (HID_KM_EpUpload(HID_MS_EP, MS_Packet_Buf, sizeof(MS_Packet_Buf)));
}

void HID_MS_KeyDown(HID_MSKeyCode key)
{
	if (key == HID_MSKeyCode_Left)
	{
		MS_Packet_Buf[0] |= 0x01;
	}
	else if (key == HID_MSKeyCode_Right)
	{
		MS_Packet_Buf[0] |= 0x02;
	}
	else if (key == HID_MSKeyCode_Middle)
	{
		MS_Packet_Buf[0] |= 0x04;
	}
	HID_MS_UploadPacket();
}

void HID_MS_KeyUp(HID_MSKeyCode key)
{
	if (key == HID_MSKeyCode_Left)
	{
		MS_Packet_Buf[0] &= (~0x01);
	}
	else if (key == HID_MSKeyCode_Right)
	{
		MS_Packet_Buf[0] |= (~0x02);
	}
	else if (key == HID_MSKeyCode_Middle)
	{
		MS_Packet_Buf[0] |= (~0x04);
	}
	HID_MS_UploadPacket();
}

void HID_MS_KeyAllUp()
{
	MS_Packet_Buf[0] = 0x00;
	HID_MS_UploadPacket();
}

void HID_MS_ClickKey(HID_MSKeyCode key)
{
	HID_MS_KeyDown(key);
	HID_MS_KeyUp(key);
}

void HID_MS_DoubleClickKey(HID_MSKeyCode key)
{
	HID_MS_ClickKey(key);
	HID_MS_ClickKey(key);
}

void HID_MS_Move(int8_t x, int8_t y)
{
	MS_Packet_Buf[1] = x;
	MS_Packet_Buf[2] = y;
	HID_MS_UploadPacket();
	MS_Packet_Buf[1] = 0;
	MS_Packet_Buf[2] = 0;
}

void HID_MS_Scroll(int8_t wheel)
{
	MS_Packet_Buf[3] = wheel;
	HID_MS_UploadPacket();
	MS_Packet_Buf[3] = 0;
}

void HID_MS_Drag(HID_MSKeyCode key, int8_t x, int8_t y)
{
	HID_MS_KeyDown(key);
	HID_MS_Move(x, y);
	HID_MS_KeyUp(key);
}
