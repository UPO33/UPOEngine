#pragma once

#include "../UPOEngine/UPOEngine.h"

#define MODULE1API UMODULE_EXPORT

namespace UPO
{
	class MODULE1API Module1Class
	{
		UCLASS(Module1Class, void)

	};

	UCLASS_BEGIN_IMPL(Module1Class)
		UCLASS_END_IMPL(Module1Class)

};
extern "C"
{
	MODULE1API void OnModuleLoaded()
	{
		ULOG_WARN("m2 num types %d", UPO::MetaSys::Get()->GetAllTypes().Length());
		auto types = UPO::MetaSys::Get()->GetAllTypes();
		for (size_t i = 0; i < types.Length(); i++)
		{
			if (types[i]->IsClass())
			{
				ULOG_WARN("Class %s", types[i]->Cast<UPO::ClassInfo>()->GetName().CStr());
			}
		}
	}
};