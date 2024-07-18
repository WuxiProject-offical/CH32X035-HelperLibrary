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

#ifndef _HID_KM_H_
#define _HID_KM_H_

#include "string.h"
#include "stdint.h"

/* Descriptors */
extern const uint8_t KeyRepDesc[];
extern const uint8_t MouseRepDesc[];

/* Key codes */
typedef enum
{
	HID_KBKeyCode_A = 0x04,
	HID_KBKeyCode_B,
	HID_KBKeyCode_C,
	HID_KBKeyCode_D,
	HID_KBKeyCode_E,
	HID_KBKeyCode_F,
	HID_KBKeyCode_G,
	HID_KBKeyCode_H,
	HID_KBKeyCode_I,
	HID_KBKeyCode_J,
	HID_KBKeyCode_K,
	HID_KBKeyCode_L,
	HID_KBKeyCode_M,
	HID_KBKeyCode_N,
	HID_KBKeyCode_O,
	HID_KBKeyCode_P,
	HID_KBKeyCode_Q,
	HID_KBKeyCode_R,
	HID_KBKeyCode_S,
	HID_KBKeyCode_T,
	HID_KBKeyCode_U,
	HID_KBKeyCode_V,
	HID_KBKeyCode_W,
	HID_KBKeyCode_X,
	HID_KBKeyCode_Y,
	HID_KBKeyCode_Z,
	HID_KBKeyCode_1_Excalm,
	HID_KBKeyCode_2_At,
	HID_KBKeyCode_3_Sharp,
	HID_KBKeyCode_4_Dollar,
	HID_KBKeyCode_5_Percent,
	HID_KBKeyCode_6_Caret,
	HID_KBKeyCode_7_And,
	HID_KBKeyCode_8_Asterisk,
	HID_KBKeyCode_9_LRndBracket,
	HID_KBKeyCode_0_RRndBracket,
	HID_KBKeyCode_Enter,
	HID_KBKeyCode_ESC,
	HID_KBKeyCode_Backspace,
	HID_KBKeyCode_Tab,
	HID_KBKeyCode_Space,
	HID_KBKeyCode_Minus_Underscore,
	HID_KBKeyCode_Equal_Plus,
	HID_KBKeyCode_LSqrBracket_LCurBracket,
	HID_KBKeyCode_RSqrBracket_RCurBracket,
	HID_KBKeyCode_BackSlash_Bar,
	HID_KBKeyCode_NUS_Sharp_Tidle,
	HID_KBKeyCode_SemiColon_Colon,
	HID_KBKeyCode_Quote_DQuote,
	HID_KBKeyCode_BackQuote_Tidle,
	HID_KBKeyCode_Comma_Less,
	HID_KBKeyCode_Dot_More,
	HID_KBKeyCode_Slash_Question,
	HID_KBKeyCode_CapsLock,
	HID_KBKeyCode_F1,
	HID_KBKeyCode_F2,
	HID_KBKeyCode_F3,
	HID_KBKeyCode_F4,
	HID_KBKeyCode_F5,
	HID_KBKeyCode_F6,
	HID_KBKeyCode_F7,
	HID_KBKeyCode_F8,
	HID_KBKeyCode_F9,
	HID_KBKeyCode_F10,
	HID_KBKeyCode_F11,
	HID_KBKeyCode_F12,
	HID_KBKeyCode_PrtScr,
	HID_KBKeyCode_ScrLock,
	HID_KBKeyCode_Pause,
	HID_KBKeyCode_Insert,
	HID_KBKeyCode_Home,
	HID_KBKeyCode_PgUp,
	HID_KBKeyCode_Del,
	HID_KBKeyCode_End,
	HID_KBKeyCode_PgDn,
	HID_KBKeyCode_RArrow,
	HID_KBKeyCode_LArrow,
	HID_KBKeyCode_DownArrow,
	HID_KBKeyCode_UpArrow,
	HID_KBKeyCode_P_NumLock,
	HID_KBKeyCode_P_Slash,
	HID_KBKeyCode_P_Asterisk,
	HID_KBKeyCode_P_Minus,
	HID_KBKeyCode_P_Plus,
	HID_KBKeyCode_P_Enter,
	HID_KBKeyCode_P_1_End,
	HID_KBKeyCode_P_2_DArrow,
	HID_KBKeyCode_P_3_PgDn,
	HID_KBKeyCode_P_4_LArrow,
	HID_KBKeyCode_P_5,
	HID_KBKeyCode_P_6_RArrow,
	HID_KBKeyCode_P_7_Homw,
	HID_KBKeyCode_P_8_UArrow,
	HID_KBKeyCode_P_9_PgUp,
	HID_KBKeyCode_P_0_Insert,
	HID_KBKeyCode_P_Dot_Del,
	HID_KBKeyCode_NUS_BackSlash_Bar,
	HID_KBKeyCode_Application,
} HID_KBKeyCode;

typedef enum
{
	HID_KBLed_Nums = 0x01, HID_KBLed_Caps, HID_KBLed_Scroll,
} HID_KBLed;

typedef enum
{
	HID_MSKeyCode_Left = 0x01, HID_MSKeyCode_Right, HID_MSKeyCode_Middle,
} HID_MSKeyCode;

static const uint8_t HID_KBModKey_LCtrl = 0x01;
static const uint8_t HID_KBModKey_LShift = 0x02;
static const uint8_t HID_KBModKey_LAlt = 0x04;
static const uint8_t HID_KBModKey_LSuper = 0x08;
static const uint8_t HID_KBModKey_RCtrl = 0x10;
static const uint8_t HID_KBModKey_RShift = 0x20;
static const uint8_t HID_KBModKey_RAlt = 0x40;
static const uint8_t HID_KBModKey_RSuper = 0x80;

/* Keyboard functions */
extern void HID_KB_KeyDown(HID_KBKeyCode key);
extern void HID_KB_KeyUp(HID_KBKeyCode key);
extern void HID_KB_KeyAllUp(void);
extern void HID_KB_ModKeyDown(uint8_t modkey);
extern void HID_KB_ModKeyUp(uint8_t modkey);
extern void HID_KB_ModKeyAllUp(void);
extern void HID_KB_ClickKey(uint8_t modkey, HID_KBKeyCode key);
extern void HID_KB_TypeString(uint8_t *str, uint16_t len);
extern uint8_t HID_KB_CheckLEDStatus(HID_KBLed led);
extern void HID_KB_EpOut_Callback(uint8_t *buf, uint16_t len);

/* Mouse functions */
extern void HID_MS_KeyDown(HID_MSKeyCode key);
extern void HID_MS_KeyUp(HID_MSKeyCode key);
extern void HID_MS_KeyAllUp();
extern void HID_MS_ClickKey(HID_MSKeyCode key);
extern void HID_MS_DoubleClickKey(HID_MSKeyCode key);
extern void HID_MS_Move(int8_t x, int8_t y);
extern void HID_MS_Scroll(int8_t wheel);
extern void HID_MS_Drag(HID_MSKeyCode key, int8_t x, int8_t y);

#endif /* _HID_KM_H_ */
