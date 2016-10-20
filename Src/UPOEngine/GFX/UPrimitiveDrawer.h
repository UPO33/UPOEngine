#pragma once

#include "URenderer.h"

#if 0
namespace UPO
{
	//////////////////////////////////////////////////////////////////////////fwd decl
	class Renderer;

	//////////////////////////////////////////////////////////////////////////
	class UAPI PrimitiveDrawer
	{
	public:
		void DrawLine(const Vec3& p0, const Vec3& p1, Color32 color = Color32::WHITE);
		
		struct LineVertex
		{
			Vec3 position;
			Color32 color;
		};

		static const unsigned MAX_LINE_IN_BUFFER = 2048;

		Renderer*			mRenderer = nullptr;
		GFXShader*			mVSLineDraw = nullptr;
		GFXShader*			mPSLineDraw = nullptr;
		GFXVertexBuffer*	mLineBuffer = nullptr;
		GFXInputLayout*		mLineDrawLayout = nullptr;
		LineVertex			mLineVertices[MAX_LINE_IN_BUFFER];
		unsigned			mNumLineVertex = 0;
		

		bool Init(Renderer*);
		void Release();

		void Frame();
		void Test();
	};

};
#endif 