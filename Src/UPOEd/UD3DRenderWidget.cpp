#include "UD3DRenderWidget.h"


namespace UPOEd
{
	//////////////////////////////////////////////////////////////////////////
	D3DRenderWidget::D3DRenderWidget(QWidget* parent /*= nullptr*/)
		: QWidget(parent)
	{
		setAttribute(Qt::WA_PaintOnScreen);
		setAttribute(Qt::WA_NativeWindow);
		mWindowHandle = (void*)(winId());
	}
	//////////////////////////////////////////////////////////////////////////
	D3DRenderWidget::~D3DRenderWidget()
	{

	}

	void D3DRenderWidget::resizeEvent(QResizeEvent* evt)
	{
		ULOG_MESSAGE("resizing %,  %", width(), height());
	}

	void D3DRenderWidget::paintEvent(QPaintEvent* evt)
	{
		
	}

	void D3DRenderWidget::GetWinSize(Vec2I& out)
	{
		out.mX = width();
		out.mY = height();
	}
}