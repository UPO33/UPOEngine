#pragma once

#include "../Engine/UEngine.h"

namespace UPO
{
	//////////////////////////////////////////////////////////////////////////
	class IEditor
	{
	public:
		virtual Entity* SelectedEntity() { return nullptr; }
		virtual Entity* SelectedAsset() { return nullptr; }
	};
};