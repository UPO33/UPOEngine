#pragma once

#include "UEntity.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class GFXTexture2D;


	//////////////////////////////////////////////////////////////////////////
	class UAPI HitProxyBase
	{
	public:
		static const unsigned MaxSize = 128;

		static void* operator new (size_t);
		static void operator delete(void*);

		int mPriority = 0;

		virtual ~HitProxyBase(){}
		
		template<typename T> T* Cast() const { return dynamic_cast<T*>(const_cast<HitProxyBase*>(this)); }
	};

	//////////////////////////////////////////////////////////////////////////
	class UAPI HPObject : public HitProxyBase
	{
	public:
		ObjectPtr	mObject;

		HPObject() {}
		HPObject(Object* object) : mObject(object) {}
	};
	//////////////////////////////////////////////////////////////////////////
	class UAPI HPEntity : public HitProxyBase
	{
	public:
		TObjectPtr<Entity>	mEntity;

		HPEntity(){}
		HPEntity(Entity* entity) : mEntity(entity) {}
	};

	//////////////////////////////////////////////////////////////////////////
	class UAPI HitSelectionCanvas
	{
		TArray<HitProxyBase*>	mProxies;
		GFXTexture2D*			mReadTexture;
		Vec2I					mSize;
		HitProxyBase*			mTakenHit;
		
	public:


		HitSelectionCanvas(Vec2I size);
		~HitSelectionCanvas();

		HitProxyBase* GetTakenHit();

		void SetMousePosition(Vec2I mousePosition);
		void Resize(Vec2I newSize);

		unsigned RegisterProxy(HitProxyBase*);
		void ClearProxeis();
		void CatchAt(Vec2 position, GFXTexture2D* IDBuffer);
	};

}