#pragma once


#include "../GFXCore/UGFXCore.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class World;
	class WorldRS;

	//////////////////////////////////////////////////////////////////////////
	class UAPI Renderer
	{
	public:
		class TestQuadRE* mTestQuad = nullptr;
		class WorldRS* mWorld = nullptr;
		class PrimitiveBatch*	mPrimitiveBatch = nullptr;

		virtual bool Init() { return false; }
		virtual bool RenderFrame() { return false; }
		virtual bool Release() { return false; }
		virtual void AttachWorld(WorldRS* world) {}

		virtual bool RenderWorld(WorldRS*) { return false; }

		static Renderer* New();
		static void Delete(Renderer*);

	};


	//////////////////////////////////////////////////////////////////////////
	class RendererElement
	{
		Renderer* mRenderer;

	public:
		RendererElement(Renderer* renderer) : mRenderer(renderer) {}
		virtual ~RendererElement(){}

		Renderer* GetRenderer() const { return mRenderer; }

		virtual void Present(){}
	};
};