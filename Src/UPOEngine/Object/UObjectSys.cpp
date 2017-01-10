#include "UObjectSys.h"
#include "UObject.h"
#include "../Core/UMemory.h"
#include "../Meta/UMeta.h"

namespace UPO
{
#ifdef USEOBJLUT
	static const unsigned INVALID_OBJECT_INDEX = ~0;
	static const unsigned MAX_OBJECT = 8000;

	//////////////////////////////////////////////////////////////////////////
	class ObjectContext
	{
	public:
		struct Elem
		{
			unsigned	mID;
			union 
			{
				Object*		mInstance;
				unsigned	mNextFreeIndex;
			};
		};

		unsigned	mIDCounter;
		unsigned	mFirstFreeIndex;
		unsigned	mNumObject;
		Elem*		mObjects;


		ObjectContext()
		{
			mObjects = new Elem[MAX_OBJECT];
			UASSERT(mObjects);

			mFirstFreeIndex = 0;
			mIDCounter = 2;
			mNumObject = 0;

			for (unsigned i = 0; i < MAX_OBJECT; i++)
			{
				mObjects[i].mID = mIDCounter;
				mObjects[i].mNextFreeIndex = i + 1;
			}
			mObjects[MAX_OBJECT - 1].mNextFreeIndex = INVALID_OBJECT_INDEX;
		}

		~ObjectContext()
		{
			delete mObjects;
			mObjects = nullptr;
		}
		//////////////////////////////////////////////////////////////////////////
		Object* NewObject(const ClassInfo* classInfo)
		{
			UASSERT(mFirstFreeIndex != INVALID_OBJECT_INDEX);	//object system is full :(
			UASSERT(classInfo);

			Object* newObj = (Object*)MemAlloc(classInfo->GetSize());

			unsigned index = mFirstFreeIndex;
			unsigned id = mIDCounter++;
			
			//0 and 1 are used for invalid handle and deleted object
			if (mIDCounter == 0) mIDCounter = 2;

			mFirstFreeIndex = mObjects[mFirstFreeIndex].mNextFreeIndex;

			newObj->mClassInfo = (ClassInfo*)classInfo;
			newObj->mObjectID = id;
			newObj->mObjectIndex = index;

			mObjects[index].mInstance = newObj;
			mObjects[index].mID = id;

			classInfo->Call_DefaultConstructor((void*)newObj);

			return newObj;
		}
		//////////////////////////////////////////////////////////////////////////
		void DeleteObject(Object* object)
		{
			UASSERT(object);

			const ClassInfo* ci = object->GetClassInfo();
			UASSERT(ci);
			ci->Call_Destructor(object);

			unsigned index = object->mObjectIndex;
			unsigned id = object->mObjectID;
			UASSERT(index != INVALID_OBJECT_INDEX && id != 1);	//check if object deleted previously 

			//invalidate object so that calling delete again will cause assert
			object->mObjectIndex = INVALID_OBJECT_INDEX;
			object->mObjectID = 1;

			mObjects[index].mID = 1;
			mObjects[index].mNextFreeIndex = mFirstFreeIndex;
			mFirstFreeIndex = index;

			MemFree(object);
		}
	};

	ObjectContext gObjectContext;

	Object* ObjectPtr::Get() const
	{
		if (mID == gObjectContext.mObjects[mIndex].mID)
			return gObjectContext.mObjects[mIndex].mInstance;
		return nullptr;
	}

	bool ObjectPtr::IsValid() const
	{
		return mID == gObjectContext.mObjects[mIndex].mID;
	}
#endif



	Object* ObjectSys::AllocObject(const ClassInfo* classInfo)
	{
		return (Object*)_aligned_malloc(classInfo->GetSize(), classInfo->GetAlign());
	}



	void ObjectSys::FreeObject(Object* object)
	{
		_aligned_free(object);
	}
	ObjectSys::ObjectSys()
	{

	}

	ObjectSys::~ObjectSys()
	{

	}

	//////////////////////////////////////////////////////////////////////////
	ObjectSys* ObjectSys::Get()
	{
		static TInstance<ObjectSys> Ins;
		return Ins;
	}

	//////////////////////////////////////////////////////////////////////////
	Object* ObjectSys::NewObject(const ClassInfo* classInfo)
	{
		UASSERT(classInfo);
		Object* newObj = AllocObject(classInfo);
		UASSERT(newObj);
		newObj->mClassInfo = (ClassInfo*)classInfo;
		newObj->mRefData = nullptr;

		classInfo->Call_DefaultConstructor((void*)newObj);

		return newObj;
	}

	Object* ObjectSys::NewObject(const ClassInfo* classInfo, size_t classSize)
	{
		UASSERT(classInfo && classSize);
		Object* newObj = (Object*)_aligned_malloc(classSize, classInfo->GetAlign());
		UASSERT(newObj);
		newObj->mClassInfo = (ClassInfo*)classInfo;
		newObj->mRefData = nullptr;

		classInfo->Call_DefaultConstructor((void*)newObj);

		return newObj;
	}

	//////////////////////////////////////////////////////////////////////////
	void ObjectSys::DeleteObject(Object* object)
	{
		UASSERT(object);
		const ClassInfo* ci = object->GetClassInfo();
		UASSERT(ci);
		ci->Call_Destructor(object);
		FreeObject(object);
	}




};