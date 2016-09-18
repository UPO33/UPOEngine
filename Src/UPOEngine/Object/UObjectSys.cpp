#include "UObjectSys.h"
#include "UObject.h"
#include "../Core/UMemory.h"
#include "../Meta/UMeta.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	Object* ObjectSys::NewObject(const ClassInfo* classInfo)
	{
		Object* newObj = (Object*)MemAlloc(classInfo->GetSize());

		newObj->mClassInfo = classInfo;
		newObj->mRefData = nullptr;

		classInfo->CallDefaultConstructor(newObj);

		return newObj;
	}
	//////////////////////////////////////////////////////////////////////////
	void ObjectSys::DeleteObject(Object* object)
	{
		if (object)
		{
			const ClassInfo* ci = object->GetClassInfo();
			ci->CallDestructor(object);
			MemFree(object);
		}
	}

};