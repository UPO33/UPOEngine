#pragma once

#include "UCommonEd.h"

#include "../UPOEngine/GFXCore/UGFXContext.h"



namespace UPOEd
{
	//////////////////////////////////////////////////////////////////////////
	class D3DRenderWidget : public QWidget 
	{
		GFXContext* mContext;

	public:
		D3DRenderWidget(QWidget* parent = nullptr);
		virtual ~D3DRenderWidget();
		virtual QPaintEngine* paintEngine() const override { return nullptr; }
	protected:
		virtual void resizeEvent(QResizeEvent* evt) override;
		virtual void paintEvent(QPaintEvent* evt) override;

		bool CreateDevice();
		void Render();
	};
};