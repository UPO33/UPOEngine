#pragma once

#include "../Core/UCore.h"
#include <SDL.h>

namespace UPO
{
	enum EMouseButton
	{
		EMB_Right,
		EMB_Left,
		EMB_Middle,
		EMB_WheelForward,
		EMB_WheelBackward,
	};

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

		EKC_Right = 200,
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

	};

	UAPI const char* EnumToStr(EKeyCode);
	UAPI EKeyCode Win32VKToEKeyCode(unsigned vk);

	class UAPI Input
	{
	public:
		static bool IsKeyDown(EKeyCode key);
		static bool IsKeyPressed(EKeyCode key);
		static bool IsKeyReleased(EKeyCode key);
		
		static void SetMousePos(int x, int y);
		static void Tick();
		static void GetMousePosition(int& outX, int& outY);
		static void GetMouseVelocity(int& outX, int& outY);
		//positive value indicates that the wheel was rotated forward, negative indicates backward
		static int  GetMouseWheelDelta();

		static bool IsMouseDown(EMouseButton btn);
		static bool IsMousePressed(EMouseButton btn);
		static bool IsMouseReleased(EMouseButton btn);
		

		static void SetMouseState(EMouseButton btn, bool down);
		static void SetKeyState(EKeyCode key, bool down);
		static void SetMouseWheelDelta(int value);
	};
};