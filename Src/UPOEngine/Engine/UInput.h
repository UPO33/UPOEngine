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

	class UAPI Input
	{
	public:
		static bool IsKeyDown(int key);
		static bool IsKeyPressed(int key);
		static bool IsKeyReleased(int key);
		
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
		static void SetKeyState(int key, bool down);
		static void SetMouseWheelDelta(int value);
	};
};