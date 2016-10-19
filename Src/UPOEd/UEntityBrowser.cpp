#include "UEntityBrowser.h"

namespace UPOEd
{

	EntityBrowserDW::EntityBrowserDW(QWidget* parent) : QDockWidget(parent)
	{
		this->setAllowedAreas(Qt::DockWidgetArea::LeftDockWidgetArea | Qt::DockWidgetArea::RightDockWidgetArea);
		this->setWindowTitle("Entities");
		this->setContentsMargins(0, 0, 0, 0);
	}

};