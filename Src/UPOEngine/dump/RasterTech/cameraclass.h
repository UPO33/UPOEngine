////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_


//////////////
// INCLUDES //
//////////////
#include "../GFXCore/UGFXCore.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: CameraClass
////////////////////////////////////////////////////////////////////////////////
class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();

	void Render(class InputClass* input);
	void GetViewMatrix(D3DXMATRIX&);

private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;

	D3DXVECTOR3 mPosition = D3DXVECTOR3(0,0,-30);
	D3DXVECTOR3 mRotation = D3DXVECTOR3(0,0,0);
	D3DXQUATERNION mRotationQuat = D3DXQUATERNION(0, 0, 0, 1);
	float mMoveSpeed = 10;
	float mRotateSpeed = 0.1f;

	D3DXMATRIX m_viewMatrix;
};

#endif