#pragma once

#include "UCommonEd.h"
#include "UD3DRenderWidget.h"

namespace UPOEd
{
	class MainViewport : public D3DRenderWidget
	{
	public:
		MainViewport(QWidget* parent = nullptr);
		void Tick();

	};
};