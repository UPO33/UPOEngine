#include "UInput.h"

namespace UPO
{
	inline unsigned EKeyToWin32VK(EKeyCode key)
	{
		static unsigned LUT[256];
		static bool Initilized = false;
		if (!Initilized)
		{
			Initilized = true;

			LUT[EKC_Null] = 0;

			LUT[EKC_0] = '0';
			LUT[EKC_1] = '1';
			LUT[EKC_2] = '2';
			LUT[EKC_3] = '3';
			LUT[EKC_4] = '4';
			LUT[EKC_5] = '5';
			LUT[EKC_6] = '6';
			LUT[EKC_7] = '7';
			LUT[EKC_8] = '8';
			LUT[EKC_9] = '9';

			LUT[EKC_A] = 'A';
			LUT[EKC_B] = 'B';
			LUT[EKC_C] = 'C';
			LUT[EKC_D] = 'D';
			LUT[EKC_E] = 'E';
			LUT[EKC_F] = 'F';
			LUT[EKC_G] = 'G';
			LUT[EKC_H] = 'H';
			LUT[EKC_I] = 'I';
			LUT[EKC_J] = 'J';
			LUT[EKC_K] = 'K';
			LUT[EKC_L] = 'L';
			LUT[EKC_M] = 'M';
			LUT[EKC_N] = 'N';
			LUT[EKC_O] = 'O';
			LUT[EKC_P] = 'P';
			LUT[EKC_Q] = 'Q';
			LUT[EKC_R] = 'R';
			LUT[EKC_S] = 'S';
			LUT[EKC_T] = 'T';
			LUT[EKC_U] = 'U';
			LUT[EKC_V] = 'V';
			LUT[EKC_W] = 'W';
			LUT[EKC_X] = 'X';
			LUT[EKC_Y] = 'Y';
			LUT[EKC_Z] = 'Z';

			LUT[EKC_Return] = VK_RETURN;
			LUT[EKC_Esc] = VK_ESCAPE;
			LUT[EKC_Backspace] = VK_BACK;
			LUT[EKC_Tab] = VK_TAB;
			LUT[EKC_Space] = VK_SPACE;

			LUT[EKC_Exclaim] = '!';
			LUT[EKC_At] = '@';
			LUT[EKC_Hash] = '#';
			LUT[EKC_Dollar] = '$';
			LUT[EKC_Percent] = '%';
			LUT[EKC_Caret] = '^';
			LUT[EKC_Ampersand] = '&';
			LUT[EKC_Asterisk] = '*';

			LUT[EKC_LefParen] = '(';
			LUT[EKC_RightParen] = ')';

			LUT[EKC_Minus] = VK_SUBTRACT;
			LUT[EKC_Plus] = VK_ADD;
			LUT[EKC_Equals] = '=';

			LUT[EKC_Quote] = '\'';
			LUT[EKC_DoubleQuote] = '"';

			LUT[EKC_Less] = '<';
			LUT[EKC_Greater] = '>';

			LUT[EKC_Underscore] = '_';
			LUT[EKC_Comma] = ';';
			LUT[EKC_Dot] = '.';
			LUT[EKC_Slash] = '/';
			LUT[EKC_Colon] = ':';
			LUT[EKC_Semicolon] = ';';


			LUT[EKC_LeftBracket] = '[';
			LUT[EKC_RightBracket] = ']';
			LUT[EKC_Backslash] = '\\';
			LUT[EKC_Backquote] = '`';

			LUT[EKC_Right] = VK_RIGHT;
			LUT[EKC_Left] = VK_LEFT;
			LUT[EKC_Up] = VK_UP;
			LUT[EKC_Down] = VK_DOWN;
			LUT[EKC_CapsLock] = VK_CAPITAL;
			LUT[EKC_LeftShift] = VK_LSHIFT;
			LUT[EKC_RightShift] = VK_RSHIFT;
			LUT[EKC_LeftCtrl] = VK_LCONTROL;
			LUT[EKC_RightCtrl] = VK_RCONTROL;
			LUT[EKC_LeftAlt] = VK_LMENU;
			LUT[EKC_RightAlt] = VK_RMENU;

			LUT[EKC_F1] = VK_F1;
			LUT[EKC_F2] = VK_F2;
			LUT[EKC_F3] = VK_F3;
			LUT[EKC_F4] = VK_F4;
			LUT[EKC_F5] = VK_F5;
			LUT[EKC_F6] = VK_F6;
			LUT[EKC_F7] = VK_F7;
			LUT[EKC_F8] = VK_F8;
			LUT[EKC_F9] = VK_F9;
			LUT[EKC_F10] = VK_F10;
			LUT[EKC_F11] = VK_F11;
			LUT[EKC_F12] = VK_F12;

			LUT[EKC_PrintScreen] = VK_SNAPSHOT;
			LUT[EKC_ScrollLock] = VK_SCROLL;
			LUT[EKC_Pause] = VK_PAUSE;
			LUT[EKC_Insert] = VK_INSERT;
			LUT[EKC_Home] = VK_HOME;
			LUT[EKC_Pageup] = VK_PRIOR;
			LUT[EKC_Delete] = VK_DELETE;
			LUT[EKC_End] = VK_END;
			LUT[EKC_Pagedown] = VK_NEXT;

			LUT[EKC_RightWin] = VK_RWIN;
			LUT[EKC_LeftWin] = VK_LWIN;
		}

		return LUT[(unsigned)key];
	}

	EKeyCode Win32VKToEKeyCode(unsigned vk)
	{
		static EKeyCode LUT[256];
		static bool Initilized = false;
		if (!Initilized)
		{
			Initilized = true;

			LUT['0'] = EKC_0;
			LUT['1'] = EKC_1;
			LUT['2'] = EKC_2;
			LUT['3'] = EKC_3;
			LUT['4'] = EKC_4;
			LUT['5'] = EKC_5;
			LUT['6'] = EKC_6;
			LUT['7'] = EKC_7;
			LUT['8'] = EKC_8;
			LUT['9'] = EKC_9;

			LUT['A'] = EKC_A;
			LUT['B'] = EKC_B;
			LUT['C'] = EKC_C;
			LUT['D'] = EKC_D;
			LUT['E'] = EKC_E;
			LUT['F'] = EKC_F;
			LUT['G'] = EKC_G;
			LUT['H'] = EKC_H;
			LUT['I'] = EKC_I;
			LUT['J'] = EKC_J;
			LUT['K'] = EKC_K;
			LUT['L'] = EKC_L;
			LUT['M'] = EKC_M;
			LUT['N'] = EKC_N;
			LUT['O'] = EKC_O;
			LUT['P'] = EKC_P;
			LUT['Q'] = EKC_Q;
			LUT['R'] = EKC_R;
			LUT['S'] = EKC_S;
			LUT['T'] = EKC_T;
			LUT['U'] = EKC_U;
			LUT['V'] = EKC_V;
			LUT['W'] = EKC_W;
			LUT['X'] = EKC_X;
			LUT['Y'] = EKC_Y;
			LUT['Z'] = EKC_Z;

			LUT[VK_RETURN] = EKC_Return;
			LUT[VK_ESCAPE] = EKC_Esc;
			LUT[VK_BACK] = EKC_Backspace;
			LUT[VK_TAB] = EKC_Tab;
			LUT[VK_SPACE] = EKC_Space;

			// 			LUT[] = EKC_Exclaim;
			// 			LUT[] = EKC_At;
			// 			LUT[] = EKC_Hash;
			// 			LUT[] = EKC_Dollar;
			// 			LUT[] = EKC_Percent;
			// 			LUT[] = EKC_Caret;
			// 			LUT[] = EKC_Ampersand;
			// 			LUT[] = EKC_Asterisk;

			// 			LUT[] = EKC_LefParen;
			// 			LUT[] = EKC_RightParen;

			// 			LUT[] = EKC_Minus;
			// 			LUT[] = EKC_Plus;
			// 			LUT[] = EKC_Equals;

			// 			LUT[] = EKC_Quote;
			// 			LUT[] = EKC_DoubleQuote;

			// 			LUT[] = EKC_Less;
			// 			LUT[] = EKC_Greater;

			// 			LUT[] = EKC_Underscore;
			// 			LUT[] = EKC_Comma;
			// 			LUT[] = EKC_Dot;
			// 			LUT[] = EKC_Slash;
			// 			LUT[] = EKC_Colon;
			// 			LUT[] = EKC_Semicolon;


			// 			LUT[] = EKC_LeftBracket;
			// 			LUT[] = EKC_RightBracket;
			// 			LUT[] = EKC_Backslash;
			// 			LUT[] = EKC_Backquote;

			LUT[VK_RIGHT] = EKC_Right;
			LUT[VK_LEFT] = EKC_Left;
			LUT[VK_UP] = EKC_Up;
			LUT[VK_DOWN] = EKC_Down;
			LUT[VK_CAPITAL] = EKC_CapsLock;

			LUT[VK_LSHIFT] = EKC_LeftShift;
			LUT[VK_RSHIFT] = EKC_RightShift;

			LUT[VK_LCONTROL] = EKC_LeftCtrl;
			LUT[VK_RCONTROL] = EKC_RightCtrl;

			LUT[VK_LMENU] = EKC_LeftAlt;
			LUT[VK_RMENU] = EKC_RightAlt;

			LUT[VK_F1] = EKC_F1;
			LUT[VK_F2] = EKC_F2;
			LUT[VK_F3] = EKC_F3;
			LUT[VK_F4] = EKC_F4;
			LUT[VK_F5] = EKC_F5;
			LUT[VK_F6] = EKC_F6;
			LUT[VK_F7] = EKC_F7;
			LUT[VK_F8] = EKC_F8;
			LUT[VK_F9] = EKC_F9;
			LUT[VK_F10] = EKC_F10;
			LUT[VK_F11] = EKC_F11;
			LUT[VK_F12] = EKC_F12;

			LUT[VK_SNAPSHOT] = EKC_PrintScreen;
			LUT[VK_SCROLL] = EKC_ScrollLock;
			LUT[VK_PAUSE] = EKC_Pause;
			LUT[VK_INSERT] = EKC_Insert;
			LUT[VK_HOME] = EKC_Home;
			LUT[VK_PRIOR] = EKC_Pageup;
			LUT[VK_DELETE] = EKC_Delete;
			LUT[VK_END] = EKC_End;
			LUT[VK_NEXT] = EKC_Pagedown;

			LUT[VK_RWIN] = EKC_RightWin;
			LUT[VK_LWIN] = EKC_LeftWin;
		}
		return LUT[vk];
	}

	
	struct InputData
	{
		bool mkeys[256] = {};
		float mMousePositionX = 0;
		float mMousePositionY = 0;
		bool mMouseBtn[8] = {};
		int mMouseWheelDelta = 0;
	};
	InputData gCurInputData;
	InputData gPreInputData;

	bool Input::IsKeyDown(EKeyCode key)
	{
		return gCurInputData.mkeys[key];
	}

	bool Input::IsKeyPressed(EKeyCode key)
	{
		return gCurInputData.mkeys[key] == true && gPreInputData.mkeys[key] == false;
	}

	bool Input::IsKeyReleased(EKeyCode key)
	{
		return gCurInputData.mkeys[key] == false && gPreInputData.mkeys[key] == true;
	}

	void Input::SetKeyState(EKeyCode key, bool down)
	{
		gCurInputData.mkeys[key] = down;
	}


	void Input::SetMouseWheelDelta(int value)
	{
		gCurInputData.mMouseWheelDelta = value;
	}

	void Input::SetMousePos(int x, int y)
	{
		gCurInputData.mMousePositionX = x;
		gCurInputData.mMousePositionY = y;
	}

	void Input::Tick()
	{
		CopyType(gPreInputData, gCurInputData);
	}

	void Input::GetMousePosition(int& outX, int& outY)
	{
		outX = gCurInputData.mMousePositionX;
		outY = gCurInputData.mMousePositionY;
	}

	void Input::GetMouseVelocity(int& outX, int& outY)
	{
		outX = gCurInputData.mMousePositionX - gPreInputData.mMousePositionX;
		outY = gCurInputData.mMousePositionY - gPreInputData.mMousePositionY;
	}

	int Input::GetMouseWheelDelta()
	{
		return gCurInputData.mMouseWheelDelta;
	}

	bool Input::IsMouseDown(EMouseButton btn)
	{
		return gCurInputData.mMouseBtn[(unsigned)btn];
	}

	bool Input::IsMousePressed(EMouseButton btn)
	{
		return gCurInputData.mMouseBtn[btn] == true && gPreInputData.mMouseBtn[btn] == false;
	}

	bool Input::IsMouseReleased(EMouseButton btn)
	{
		return gCurInputData.mMouseBtn[btn] == false && gPreInputData.mMouseBtn[btn] == true;
	}

	void Input::SetMouseState(EMouseButton btn, bool down)
	{
		gCurInputData.mMouseBtn[btn] = down;
	}

	UAPI const char* EnumToStr(EKeyCode keycode)
	{
		static const char* LUT[256] = { 0 };
		static bool Initilized = false;
		if (!Initilized)
		{

			LUT[EKC_Null] = 0;
			LUT[EKC_Any] = "Any";

			LUT[EKC_0] = "0";
			LUT[EKC_1] = "1";
			LUT[EKC_2] = "2";
			LUT[EKC_3] = "3";
			LUT[EKC_4] = "4";
			LUT[EKC_5] = "5";
			LUT[EKC_6] = "6";
			LUT[EKC_7] = "7";
			LUT[EKC_8] = "8";
			LUT[EKC_9] = "9";

			LUT[EKC_A] = "A";
			LUT[EKC_B] = "B";
			LUT[EKC_C] = "C";
			LUT[EKC_D] = "D";
			LUT[EKC_E] = "E";
			LUT[EKC_F] = "F";
			LUT[EKC_G] = "G";
			LUT[EKC_H] = "H";
			LUT[EKC_I] = "I";
			LUT[EKC_J] = "J";
			LUT[EKC_K] = "K";
			LUT[EKC_L] = "L";
			LUT[EKC_M] = "M";
			LUT[EKC_N] = "N";
			LUT[EKC_O] = "O";
			LUT[EKC_P] = "P";
			LUT[EKC_Q] = "Q";
			LUT[EKC_R] = "R";
			LUT[EKC_S] = "S";
			LUT[EKC_T] = "T";
			LUT[EKC_U] = "U";
			LUT[EKC_V] = "V";
			LUT[EKC_W] = "W";
			LUT[EKC_X] = "X";
			LUT[EKC_Y] = "Y";
			LUT[EKC_Z] = "Z";

			LUT[EKC_Return] = "Return";
			LUT[EKC_Esc] = "Esc";
			LUT[EKC_Backspace] = "Backspace";
			LUT[EKC_Tab] = "Tab";
			LUT[EKC_Space] = "Space";

			LUT[EKC_Exclaim] = "!";
			LUT[EKC_At] = "@";
			LUT[EKC_Hash] = "#";
			LUT[EKC_Dollar] = "$";
			LUT[EKC_Percent] = "%";
			LUT[EKC_Caret] = "^";
			LUT[EKC_Ampersand] = "&";
			LUT[EKC_Asterisk] = "*";

			LUT[EKC_LefParen] = "(";
			LUT[EKC_RightParen] = ")";

			LUT[EKC_Minus] = "-";
			LUT[EKC_Plus] = "+";
			LUT[EKC_Equals] = "=";

			LUT[EKC_Quote] = "\'";
			LUT[EKC_DoubleQuote] = "\"";

			LUT[EKC_Less] = "<";
			LUT[EKC_Greater] = ">";

			LUT[EKC_Underscore] = "_";
			LUT[EKC_Comma] = ";";
			LUT[EKC_Dot] = ".";
			LUT[EKC_Slash] = "/";
			LUT[EKC_Colon] = ":";
			LUT[EKC_Semicolon] = ";";


			LUT[EKC_LeftBracket] = "[";
			LUT[EKC_RightBracket] = "]";
			LUT[EKC_Backslash] = "\\";
			LUT[EKC_Backquote] = "`";

			LUT[EKC_Right] = "Right";
			LUT[EKC_Left] = "Left";
			LUT[EKC_Up] = "Up";
			LUT[EKC_Down] = "Down";
			LUT[EKC_CapsLock] = "CapsLock";
			
			LUT[EKC_LeftShift] = "LeftShift";
			LUT[EKC_RightShift] = "RightShift";
			
			LUT[EKC_LeftCtrl] = "LeftCtrl";
			LUT[EKC_RightCtrl] = "RightCtrl";

			LUT[EKC_LeftAlt] = "LeftAlt";
			LUT[EKC_RightAlt] = "RightAlt";

			LUT[EKC_F1] = "F1";
			LUT[EKC_F2] = "F2";
			LUT[EKC_F3] = "F3";
			LUT[EKC_F4] = "F4";
			LUT[EKC_F5] = "F5";
			LUT[EKC_F6] = "F6";
			LUT[EKC_F7] = "F7";
			LUT[EKC_F8] = "F8";
			LUT[EKC_F9] = "F9";
			LUT[EKC_F10] = "F10";
			LUT[EKC_F11] = "F11";
			LUT[EKC_F12] = "F12";

			LUT[EKC_PrintScreen] = "PrintScreen";
			LUT[EKC_ScrollLock] = "ScrollLock";
			LUT[EKC_Pause] = "Pause";
			LUT[EKC_Insert] = "Insert";
			LUT[EKC_Home] = "Home";
			LUT[EKC_Pageup] = "PageUp";
			LUT[EKC_Delete] = "Delete";
			LUT[EKC_End] = "End";
			LUT[EKC_Pagedown] = "PageDown";

			LUT[EKC_RightWin] = "RightWin";
			LUT[EKC_LeftWin] = "LeftWin";
		}



		return LUT[(unsigned)keycode];
	}


};