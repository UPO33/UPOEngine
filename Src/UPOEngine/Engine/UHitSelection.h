#pragma once

#include "UEntity.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class GFXTexture2D;


	//////////////////////////////////////////////////////////////////////////
	class UAPI HPBase : public RefCountable
	{
	public:
		static const unsigned MaxSize = 128;

		static void* operator new (size_t);
		static void operator delete(void*);

		int mPriority = 0;

		virtual ~HPBase(){}
		
		template<typename T> T* Cast() const { return dynamic_cast<T*>(const_cast<HPBase*>(this)); }
	};
	
	typedef TSmartPtr<HPBase> HPBaseRef;


	//////////////////////////////////////////////////////////////////////////
	class UAPI HPObject : public HPBase
	{
	public:
		ObjectPtr	mObject;

		HPObject() {}
		HPObject(Object* object) : mObject(object) {}
	};
	//////////////////////////////////////////////////////////////////////////
	class UAPI HPEntity : public HPBase
	{
	public:
		TObjectPtr<Entity>	mEntity;

		HPEntity(){}
		HPEntity(Entity* entity) : mEntity(entity) {}
	};

	//////////////////////////////////////////////////////////////////////////
	class UAPI HitSelectionCanvas
	{
		TArray<HPBaseRef>		mProxies[2];
		unsigned				mProxiesIndexGTWrite;
		TinyLock				mSwapLock;

		TArray<HPBaseRef>	mRTProxies;
		GFXTexture2D*		mReadTexture;
		Vec2I				mSize;
		HPBaseRef			mTakenHit;
		
	public:

		static const unsigned MaxRTProxy = 100000;

		HitSelectionCanvas(Vec2I size);
		~HitSelectionCanvas();

		HPBaseRef GetTakenHit();

		void SetMousePosition(Vec2I mousePosition);
		void Resize(Vec2I newSize);

		unsigned RegisterRTProxy(HPBaseRef);
		unsigned RegisterProxy(HPBaseRef);
		
		void ClearProxeis();
		void ClearRTProxeis();
		void CatchAt(Vec2 position, GFXTexture2D* IDBuffer);
	};

}