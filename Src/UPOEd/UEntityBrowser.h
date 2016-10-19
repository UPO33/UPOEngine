#pragma once

#include "UCommonEd.h"


namespace UPOEd
{

	class EntityBrowserDW : public QDockWidget
	{
	public:
		World* mWorld;
		EntityBrowserDW(QWidget* parent = nullptr);
	};
};