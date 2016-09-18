#pragma once

#include <QtWidgets/QWidget>

namespace UPOEd
{
	//////////////////////////////////////////////////////////////////////////
	class D3DRenderWidget : public QWidget 
	{

	public:
		D3DRenderWidget(QWidget* parent = nullptr);
		virtual ~D3DRenderWidget();
		virtual QPaintEngine* paintEngine() const { return nullptr; }
	protected:
		virtual void resizeEvent(QResizeEvent* evt){}
		virtual void paintEvent(QPaintEvent* evt){}

		void CreateDevice();
	};
};