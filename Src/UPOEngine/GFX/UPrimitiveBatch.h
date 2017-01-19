#pragma once

#include "../Core/UBasic.h"
#include "../Core/UVector.h"
#include "../Core/UArray.h"

#include "../GFXCore/UGFXCore.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class AStaticMesh;


	//////////////////////////////////////////////////////////////////////////
	class UAPI PrimitiveBatch 
	{
	public:

#pragma pack(push, 1)
		struct LineVertex
		{
			Vec3		mPosition;
			Color32		mColor;
		};
#pragma pack(pop)

		typedef TArray<LineVertex> LineVertexArray;


		struct StaticMeshItem
		{
			Transform			mTransform;
			Vec4				mColor;
			const AStaticMesh*	mMesh;
			float				mLifeTime;
		};

		LineVertexArray				mLines[2];
		volatile LineVertexArray*	mGTLines;
		volatile LineVertexArray*	mRTLines;

		GFXVertexBuffer*			mLinesBuffer;
		GFXInputLayout*				mInputLayout;
		GFXConstantBuffer*			mConstantBuffer;
		TinyLock					mSwapLock;

		typedef TArray<StaticMeshItem>	StaticMeshItemArray;

		StaticMeshItemArray		mMeshes[2];
		StaticMeshItemArray*	mGTMeshes;
		StaticMeshItemArray*	mRTMeshes;

		StaticMeshItemArray* GetGTMeshes() { return mGTMeshes; }
		StaticMeshItemArray* GetRTMeshes() { return mRTMeshes; }

		PrimitiveBatch();
		~PrimitiveBatch();

		LineVertexArray* GetGTLines() { return (LineVertexArray*) mGTLines; }
		LineVertexArray* GetRTLines() { return (LineVertexArray*) mRTLines; }

		void DrawLine(const Vec3& point0, const Color32& color0, const Vec3& point1, const Color32& color1);
		void DrawWireBox(const Vec3& min, const Vec3& max, const Color32& color);
		void DrawWireMesh(const AStaticMesh* mesh, const Transform& transform, const Color& color, float lifeTimeSeconds);
		void CreateRenderResoures();

		void Swap();
		void Render();

	};
};