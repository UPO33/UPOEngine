#pragma once

#include "../Core/UBasic.h"
#include "../Core/UVector.h"
#include "../Core/UArray.h"

#include "../GFXCore/UGFXCore.h"

namespace UPO
{
	class UAPI PrimitiveBatch 
	{
	public:
		static const unsigned MAX_LINE_PER_FRAME = 8000;

		struct Line
		{
			Vec3	mPosition0;
			Color	mColor0;
			Vec3	mPosition1;
			Color	mColor1;
		};

		TArray<Line>		mLines[2];

		volatile unsigned	mGSIndex = 0;
		GFXVertexBufferRef	mLinesBuffer;
		GFXInputLayoutRef	mInputLayout;

		PrimitiveBatch();
		~PrimitiveBatch();

		void DrawLine(const Vec3& point0, const Color32& color0, const Vec3& point1, const Color32& color1);
		void DrawWireBox(const Vec3& min, const Vec3& max, const Color32& color);

		void CreateRenderResoures();

		void Swap();
		void Render();

	};
};