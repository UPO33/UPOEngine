#pragma once

#include "../UPOEngine/UPOEngine.h"

#define MODULE1API UMODULE_EXPORT

namespace UPO
{
	class MODULE1API Module2Class
	{
		UCLASS(Module2Class, void)

		Vec3 mVar;
	};

	UCLASS_BEGIN_IMPL(Module2Class)
	UPROPERTY(mVar)
	UCLASS_END_IMPL(Module2Class)

};
extern "C" MODULE1API void OnModuleLoaded()
{
	return;

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