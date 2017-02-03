#include "UEntityFreeCamera.h"
#include "UInput.h"
#include "UCanvas.h"
#include "UGameWindow.h"

#include "../Meta/UMeta.h"


namespace UPO
{
	UCLASS_BEGIN_IMPL(EntityFreeCamera)
	UCLASS_END_IMPL(EntityFreeCamera)






	EntityFreeCamera::EntityFreeCamera()
	{
		mFarClip = 100000;
		mPosition = Vec3(30, 30, -30);
		mRotationEuler = Matrix4::SRotationDir(Vec3::ZERO - mPosition).GetRotationEuler();
	}

	void EntityFreeCamera::OnTick()
	{
		float moveSpeed = GetWorld()->GetGameWindow() ? GetWorld()->GetGameWindow()->mOptions.mEditorCameraSpeed : 100;

		moveSpeed = moveSpeed * GetWorld()->GetDeltaTime();

		auto input = GetWorld()->GetInputState();
		Vec2 mouseVelocity = input->GetMouseVelocity();

		if (input->IsKeyPressed(EKC_LeftCtrl))
		{
			Vec3 vDir = Quat::MakeEuler(mRotationEuler).Rotate(Vec3(0, 0, mFocusDist));
			mFocusPoint = mPosition + vDir;

			mFocusPointRotation = Matrix4::SRotationDir(mPosition - mFocusPoint).GetRotationEuler();
		}
		if (input->IsKeyDown(EKC_LeftCtrl))
		{
			mFocusDist -= Clamp(input->GetMouseWheelDelta(), -5, 5);
			mFocusDist = Clamp(mFocusDist, 10, 10000);

			if (input->IsKeyDown(EKC_MouseRight))
			{
				mFocusPointRotation.mY += mouseVelocity.mX;
				mFocusPointRotation.mX -= mouseVelocity.mY;

				Vec3 v = Quat::MakeEuler(mFocusPointRotation) * Vec3(0, 0, mFocusDist);
				Matrix4 matR, matT;
				matT.MakeTranslation(v);
				mPosition = mFocusPoint + v;
				matR.MakeRotationDir(mFocusPoint - mPosition);
				mRotationEuler = matR.GetRotationEuler();
				SetWorldTransform(matR * matT);
			}
			// 				Vec3 v = Quat::MakeEuler(mFocusPointRotation) * Vec3(0, 0, mFocusDist);
			// 				Matrix4 matR, matT;
			// 				matT.MakeTranslation(v);
			// 				mPosition = mFocusPoint + v;
			// 				matR.MakeRotationDir(mFocusPoint - mPosition);
			// 				mRotationEuler = matR.GetRotationEuler();

		}
		else
		{
			float upWorld = input->IsKeyDown(EKC_E) ? 1 : (input->IsKeyDown(EKC_Q) ? -1 : 0);
			float rightLocal = input->IsKeyDown(EKC_D) ? 1 : (input->IsKeyDown(EKC_A) ? -1 : 0);
			float forwardLocal = input->IsKeyDown(EKC_W) ? 1 : (input->IsKeyDown(EKC_S) ? -1 : 0);

			if (input->IsKeyDown(EKC_MouseMiddle))
			{
				upWorld -= Clamp(mouseVelocity.mY, -1, 1);
				rightLocal = Clamp(mouseVelocity.mX, -1, 1);
			}


			if (input->IsKeyDown(EKC_LeftShift)) moveSpeed *= 4;

			if (input->IsKeyDown(EKC_MouseRight))
			{
				mRotationEuler.mY += mouseVelocity.mX;
				mRotationEuler.mX += mouseVelocity.mY;
				int fovDelta = Clamp(input->GetMouseWheelDelta(), -1, 1) * 5;
				mFieldOfView -= fovDelta;
				mFieldOfView = Clamp(mFieldOfView, 40, 120);
			}




			mPosition.mY += upWorld * moveSpeed;

			mPosition += Quat::MakeEuler(mRotationEuler).Rotate(Vec3(rightLocal, 0, forwardLocal)) * moveSpeed;

			SetWorldTransform(mPosition, mRotationEuler);
		}
	}

#if 1
	void EntityFreeCamera::FocusAt(const AABB& aabb)
	{
		EntityCameraRS* rs = GetRS<EntityCameraRS>();

		float len = aabb.GetExtent().Length();

		Vec3 center = aabb.GetCenter();
		Vec3 toCamera =  (center + Vec3(1,1,-1)) - center;
		toCamera.NormalizeSafe();
		float ff = 2 / tan(rs->mFieldOfView * DEG2RAD / 2);
		ULOG_MESSAGE("ff %", ff);
		Vec3 newCamPos = center + (toCamera * (len * ff));
		mPosition = newCamPos;
		mRotationEuler = Matrix4::SRotationDir(-toCamera).GetRotationEuler();
	}
#endif
#if 0
	void EntityFreeCamera::FocusAt(const AABB& aabb)
	{
		EntityCameraRS* rs = GetRS<EntityCameraRS>();

		Matrix4 mat = rs->mMatrixWorldToClip;

		unsigned numProjPoint = 4;
		Vec4 projected_points[8];
		projected_points[0] = mat.TransformVec4(Vec4(aabb.mMin, 1));
		projected_points[1] = mat.TransformVec4(Vec4(aabb.mMax, 1));
		projected_points[2] = mat.TransformVec4(Vec4(aabb.GetCorner(true, false, false), 1));
		projected_points[3] = mat.TransformVec4(Vec4(aabb.GetCorner(false, true, true), 1));

		Vec4 A1, B1, A2, B2;
		float width = 0, height = 0;
		for (int i = 1; i < numProjPoint - 1; i++)
		{
			for (int j = i; j < numProjPoint; j++)
			{
				float new_width = abs(projected_points[i].mX - projected_points[j].mX);
				float new_height = abs(projected_points[i].mY - projected_points[j].mY);

				if (new_width > width)
				{
					A1 = projected_points[i];
					B1 = projected_points[j];
					width = new_width;
				}
				if (new_height > height)
				{
					A2 = projected_points[i];
					B2 = projected_points[j];
					height = new_height;
				}
			}
		}
		/*
		vec4 A1, B1, A2, B2

			float width, height

			for (i = 1 .. length(projected_points) - 1) do
				for (j = i .. length(projected_points)) do
				   float new_width := abs(projected_points[i].x - projected_points[j].x)
				   float new_height := abs(projected_points[i].y - projected_points[j].y)

				   if (new_width > width) then
					   A1 := projected_points[i]
					   B1 := projected_points[j]
					   width := new_width
				   end
				   if (new_height > height) then
					   A2 := projected_points[i]
					   B2 := projected_points[j]
					   height := new_height
				   end
				end
			end

			vec4 A, B

			float dist
			if (height * aspect > width) then
			dist := height / 2 / tan(FOV / 2)
			A := A1
			B := B1
			else
			dist := widh / 2 / tan(FOV * aspect / 2)
			A := A2
			B := B2
			end
		*/
			Vec4	A, B;
			float aspect = 1;
			float FOV = rs->mFieldOfView * DEG2RAD;
			float dist = 0;
			if (height * aspect > width)
			{
				dist = height / 2 / tan(FOV / 2);
				A = A1;
				B = B1;
			}
			else
			{
				dist = width / 2 / tan(FOV * aspect / 2);
				A = A2;
				B = B2;
			}
	}
#endif // 
};