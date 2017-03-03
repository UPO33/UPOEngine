#pragma once

#include "UCommonEd.h"




namespace UPOEd
{
	//////////////////////////////////////////////////////////////////////////
	class D3DRenderWidget : public QWidget , public GameWindow
	{
	public:
		D3DRenderWidget(QWidget* parent = nullptr);
		virtual ~D3DRenderWidget();
		virtual QPaintEngine* paintEngine() const override { return nullptr; }
	protected:
		virtual void resizeEvent(QResizeEvent* evt) override;
		virtual void paintEvent(QPaintEvent* evt) override;

		virtual void GetWinSize(Vec2I& out) override;

	};
};