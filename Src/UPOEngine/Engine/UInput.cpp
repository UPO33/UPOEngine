#include "UInput.h"

namespace UPO
{
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

	bool Input::IsKeyDown(int key)
	{
		return gCurInputData.mkeys[key];
	}

	bool Input::IsKeyPressed(int key)
	{
		return gCurInputData.mkeys[key] == true && gPreInputData.mkeys[key] == false;
	}

	bool Input::IsKeyReleased(int key)
	{
		return gCurInputData.mkeys[key] == false && gPreInputData.mkeys[key] == true;
	}

	void Input::SetKeyState(int key, bool down)
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



};