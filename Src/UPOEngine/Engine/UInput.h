#pragma once

#include "../Core/UBasic.h"

namespace UPO
{
	enum EKeyCode
	{
		EKC_Null = 0,
		EKC_Any = 1,

		EKC_0 = '0',
		EKC_1 = '1',
		EKC_2 = '2',
		EKC_3 = '3',
		EKC_4 = '4',
		EKC_5 = '5',
		EKC_6 = '6',
		EKC_7 = '7',
		EKC_8 = '8',
		EKC_9 = '9',

		EKC_A = 'A',
		EKC_B = 'B',
		EKC_C = 'C',
		EKC_D = 'D',
		EKC_E = 'E',
		EKC_F = 'F',
		EKC_G = 'G',
		EKC_H = 'H',
		EKC_I = 'I',
		EKC_J = 'J',
		EKC_K = 'K',
		EKC_L = 'L',
		EKC_M = 'M',
		EKC_N = 'N',
		EKC_O = 'O',
		EKC_P = 'P',
		EKC_Q = 'Q',
		EKC_R = 'R',
		EKC_S = 'S',
		EKC_T = 'T',
		EKC_U = 'U',
		EKC_V = 'V',
		EKC_W = 'W',
		EKC_X = 'X',
		EKC_Y = 'Y',
		EKC_Z = 'Z',

		EKC_Return = '\r',
		EKC_Esc = '\033',
		EKC_Backspace = '\b',
		EKC_Tab = '\t',
		EKC_Space = ' ',

		EKC_Exclaim = '!',
		EKC_At = '@',
		EKC_Hash = '#',
		EKC_Dollar = '$',
		EKC_Percent = '%',
		EKC_Caret = '^',
		EKC_Ampersand = '&',
		EKC_Asterisk = '*',

		EKC_LefParen = '(',
		EKC_RightParen = ')',

		EKC_Minus = '-',
		EKC_Plus = '+',
		EKC_Equals = '=',

		EKC_Quote = '\'',
		EKC_DoubleQuote = '"',

		EKC_Less = '<',
		EKC_Greater = '>',

		EKC_Underscore = '_',
		EKC_Comma = ',',
		EKC_Dot = '.',
		EKC_Slash = '/',
		EKC_Colon = ':',
		EKC_Semicolon = ';',


		EKC_LeftBracket = '[',
		EKC_RightBracket = ']',
		EKC_Backslash = '\\',
		EKC_Backquote = '`',

		EKC_Right = 180,
		EKC_Left,
		EKC_Up,
		EKC_Down,
		EKC_CapsLock,

		EKC_LeftShift,
		EKC_RightShift,

		EKC_LeftCtrl,
		EKC_RightCtrl,

		EKC_LeftAlt,
		EKC_RightAlt,

		EKC_F1,
		EKC_F2,
		EKC_F3,
		EKC_F4,
		EKC_F5,
		EKC_F6,
		EKC_F7,
		EKC_F8,
		EKC_F9,
		EKC_F10,
		EKC_F11,
		EKC_F12,

		EKC_PrintScreen,
		EKC_ScrollLock,
		EKC_Pause,
		EKC_Insert,
		EKC_Home,
		EKC_Pageup,
		EKC_Delete,
		EKC_End,
		EKC_Pagedown,

		EKC_RightWin,
		EKC_LeftWin,
		
		EKC_Numlock,
		EKC_Numpad0,
		EKC_Numpad1,
		EKC_Numpad2,
		EKC_Numpad3,
		EKC_Numpad4,
		EKC_Numpad5,
		EKC_Numpad6,
		EKC_Numpad7,
		EKC_Numpad8,
		EKC_Numpad9,
		EKC_Add,
		EKC_Subtract,
		EKC_Multiply,
		EKC_Divide,
		EKC_Decimal,

		EKC_MouseRight,
		EKC_MouseLeft,
		EKC_MouseMiddle,
		EKC_MouseWheelForward,
		EKC_MouseWheelBackward,
	};

	UAPI const char* EnumToStr(EKeyCode);


	

	class UAPI Input
	{
	public:
		static bool IsKeyDown(EKeyCode key);
		static bool IsKeyPressed(EKeyCode key);
		static bool IsKeyReleased(EKeyCode key);
		

		static void GetMousePosition(int& outX, int& outY);
		static void GetMouseVelocity(int& outX, int& outY);
		//positive value indicates that the wheel was rotated forward, negative indicates backward
		static int  GetMouseWheelDelta();




		static void SetKeyState(EKeyCode key, bool down);
		static void SetMouseWheelDelta(int value);
		static void SetMousePos(int x, int y);
		static void Tick();
	};



#ifdef UPLATFORM_WIN
	UAPI EKeyCode UWin32VKToEKeyCode(unsigned vk);
#else

#endif

};