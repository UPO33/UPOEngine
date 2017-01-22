#pragma once

#include "../Core/UBasic.h"
#include "../Core/UVector.h"
#include "../Core/UArray.h"

#include "../GFXCore/UGFXCore.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class AStaticMesh;
	class World;

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
		
		typedef TArray<StaticMeshItem>	StaticMeshItemArray;


		//////////////////////////////////////////////////////////////////////////memvars
		World*						mOwner;
		float						mDelta;

		LineVertexArray				mLines[2];
		volatile LineVertexArray*	mGTLines;
		volatile LineVertexArray*	mRTLines;

		GFXVertexBuffer*			mLinesBuffer;
		GFXInputLayout*				mInputLayout;
		GFXConstantBuffer*			mConstantBuffer;
		TinyLock					mSwapLock;

		

		StaticMeshItemArray		mMeshes[2];
		StaticMeshItemArray*	mGTMeshes;
		StaticMeshItemArray*	mRTMeshes;

		StaticMeshItemArray* GetGTMeshes() { return mGTMeshes; }
		StaticMeshItemArray* GetRTMeshes() { return mRTMeshes; }

		LineVertexArray* GetGTLines() { return (LineVertexArray*)mGTLines; }
		LineVertexArray* GetRTLines() { return (LineVertexArray*)mRTLines; }

	public:
		PrimitiveBatch(World* owner);
		~PrimitiveBatch();



		void DrawLine(const Vec3& point0, const Color32& color0, const Vec3& point1, const Color32& color1);
		void DrawWireBox(const Vec3& min, const Vec3& max, const Color32& color);
		void DrawWireMesh(const AStaticMesh* mesh, const Transform& transform, const Color& color, float lifeTimeSeconds);
		void CreateRenderResoures();

		void Tick(float delta);
		void Render();

	};
};