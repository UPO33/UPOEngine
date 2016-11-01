#pragma once


#include "../GFXCore/UGFXCore.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class World;

	//////////////////////////////////////////////////////////////////////////
	class UAPI Renderer
	{
	public:
		virtual bool Init(GFXContext* context) { return false; }
		virtual bool RenderFrame() { return false; }
		virtual bool Release() { return false; }
		virtual void AttachWorld(World* world) {}

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