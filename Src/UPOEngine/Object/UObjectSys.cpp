#include "UObjectSys.h"
#include "UObject.h"
#include "../Core/UMemory.h"
#include "../Meta/UMeta.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	Object* ObjectSys::NewObject(const ClassInfo* classInfo)
	{
		UASSERT(classInfo);
		Object* newObj = (Object*)MemAlloc(classInfo->GetSize());

		newObj->mClassInfo = (ClassInfo*)classInfo;
		newObj->mRefData = nullptr;

		classInfo->CallDefaultConstructor((void*)newObj);

		return newObj;
	}
	//////////////////////////////////////////////////////////////////////////
	void ObjectSys::DeleteObject(Object* object)
	{
		UASSERT(object);

		const ClassInfo* ci = object->GetClassInfo();
		ci->CallDestructor(object);
		MemFree(object);
	}

};