////////////////////////////////////////////////////////////////////////////////
// Filename: inputclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_


////////////////////////////////////////////////////////////////////////////////
// Class name: InputClass
////////////////////////////////////////////////////////////////////////////////
class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	void Initialize();

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	bool IsKeyDown(unsigned int);
	
	void SetMousePos(int, int);
	float GetMouseVelocityX();
	float GetMouseVelocityY();

	void Frame();
private:
	bool m_keys[256];
	float mMousePositionX = 0;
	float mPreMousePositionX = 0;
	float mMousePositionY = 0;
	float mPreMousePositionY = 0;
};

extern InputClass* gInput;

#endif