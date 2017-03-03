#pragma once

#include "UCommonEd.h"
#include "UD3DRenderWidget.h"

class Ui_MainViewport;

namespace UPOEd
{
	class RenderViewportWidget : public D3DRenderWidget
	{
	public:
		TDelegateMulti<void, QMouseEvent*>	mOnMousePress;
		TDelegateMulti<void, QMouseEvent*>	mOnMouseRelease;
		TDelegateMulti<void, QMouseEvent*>	mOnMouseMove;

		RenderViewportWidget(QWidget* parent = nullptr);

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


		void Tick();
	};


	class MainViewport : public QWidget
	{
		Ui_MainViewport*				ui;
		Object*							mOptionsWidgetObject;
		TArray<TObjectPtr<Entity>>		mSelectedEntities;

	public:
		MainViewport(QWidget* parent = nullptr);
		~MainViewport();
		RenderViewportWidget* GetViewport() const;
		void SetWorld(World*);
		void Tick();

		void ToggleOptionsWidgetVisibility();

		void ViewportMouseRelease(RenderViewportWidget* viewport, QMouseEvent* mouseEvent);
	};

};