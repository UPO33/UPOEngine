#include "UMainViewport.h"

namespace UPOEd
{

	MainViewport::MainViewport(QWidget* parent /*= nullptr*/) : D3DRenderWidget(parent)
	{

	}

	void MainViewport::Tick()
	{
		this->Render();
	}

}