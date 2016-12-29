#pragma once

#include "UCommonEd.h"
#include "UD3DRenderWidget.h"

namespace UPOEd
{
	class MainViewport : public D3DRenderWidget
	{
	public:
		MainViewport(QWidget* parent = nullptr);

		virtual void mousePressEvent(QMouseEvent *event) override;

		virtual void mouseReleaseEvent(QMouseEvent *event) override;

		virtual void mouseMoveEvent(QMouseEvent *event) override;

		virtual void wheelEvent(QWheelEvent *event) override;

		virtual void keyPressEvent(QKeyEvent *event) override;

		virtual void keyReleaseEvent(QKeyEvent *event) override;

		virtual void focusInEvent(QFocusEvent *event) override;

		virtual void focusOutEvent(QFocusEvent *event) override;

		virtual void enterEvent(QEvent *event) override;

		virtual void leaveEvent(QEvent *event) override;

	};
};