#pragma once

#include "../Core/UBasic.h"
#include "../Core/UVector.h"
#include "../Core/UArray.h"

#include "../GFXCore/UGFXCore.h"

#include "UHitSelection.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class AStaticMesh;
	class World;
	class HPBase;

	//////////////////////////////////////////////////////////////////////////
	class UAPI PrimitiveBatch 
	{
	public:

#pragma pack(push, 1)
		struct LineVertex
		{
			Vec3		mPosition;
			Color32		mColor;
			unsigned	mHitID;
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


		struct ShpereItem
		{
			Vec3		mPosition;
			float		mRadius;
			Color32		mColor;
			unsigned	mHitID;
			
		};

		typedef TArray<ShpereItem> SphereItemArray;

		


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

		SphereItemArray			mShperes[2];
		SphereItemArray*		mGTSphere;
		SphereItemArray*		mRTSphere;
		GFXInputLayout*			mILWireSphere;
		GFXVertexBuffer*		mVBCircleLines;
		GFXInputLayout*			mILCircleDraw;

		HPBase*					mHitProxy;
		unsigned				mHitProxyID;

		StaticMeshItemArray* GetGTMeshes() { return mGTMeshes; }
		StaticMeshItemArray* GetRTMeshes() { return mRTMeshes; }

		LineVertexArray* GetGTLines() { return (LineVertexArray*)mGTLines; }
		LineVertexArray* GetRTLines() { return (LineVertexArray*)mRTLines; }

	public:
		PrimitiveBatch(World* owner);
		~PrimitiveBatch();

		
		void Swap();

		void SetHitProxy(HPBaseRef hp);
		HPBase* GetHitProxy() const;



		void DrawLine(const Vec3& point0, const Vec3& point1, const Color32& color);
		void DrawLine(const Vec3& point0, const Color32& color0, const Vec3& point1, const Color32& color1);
		void DrawWireBox(const Vec3& min, const Vec3& max, const Color32& color);
		void DrawWireBox(const Vec3& extent, const Transform& transform, const Color32& color);
		void DrawWireMesh(const AStaticMesh* mesh, const Transform& transform, const Color& color, float lifeTimeSeconds);
		void DrawWireFrustum(const Matrix4& frustum, const Color32& color);
		void DrawWireSphere(const Vec3& position, float radius, Color32 color);
		void CreateRenderResoures();
		void DrawCircle(const Vec3& position, float radius, Color32 color, const Vec3& xAxis = Vec3(1,0,0), const Vec3& zAxis = Vec3(0,0,1));
		void Tick(float delta);
		void Render();

	};
};