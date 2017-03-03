#include "UMainViewport.h"
#include "UPropertyBrowser.h"
#include "UMainWindow.h"

#include "ui_MainViewport.h"
#include "../UPOEngine/Engine/UInput.h"
#include "../UPOEngine/Engine/UHitSelection.h"

#include "UPropertyBrowser.h"

namespace UPOEd
{
	//////////////////////////////////////////////////////////////////////////
	EKeyCode ToEKeyCode(Qt::MouseButton btn)
	{
		switch (btn)
		{
		case Qt::LeftButton: return EKeyCode::EKC_MouseLeft;
		case Qt::RightButton: return EKeyCode::EKC_MouseRight;
		case Qt::MiddleButton: return EKeyCode::EKC_MouseMiddle;
		}
		return EKeyCode::EKC_Null;
	}


	RenderViewportWidget::RenderViewportWidget(QWidget* parent /*= nullptr*/) : D3DRenderWidget(parent)
	{
		this->setFocusPolicy(Qt::FocusPolicy::ClickFocus);
		this->setMouseTracking(true);
		this->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);

		// 		connect(this, &MainViewport::customContextMenuRequested, this, [this](const QPoint& p) {
		// 			PropertyBrowserWidget* pbw = new PropertyBrowserWidget(this);
		// 			pbw->AttachObject(NewObject<TestObject>());
		// 			pbw->setAutoFillBackground(false);
		// 			pbw->setGeometry(p.x(), p.y(), 100, 100);
		// 			pbw->show();

		// 			QMenu* menu = new QMenu(this);
		// 			menu->addAction("asdad");
		// 			menu->addAction("asdadsd");
		// 			menu->addAction("asdadsdf");
		// 			menu->popup(p);
		// 		});
	}

	void RenderViewportWidget::mousePressEvent(QMouseEvent *event)
	{
		if (GetInputState()) GetInputState()->SetKeyState(ToEKeyCode(event->button()), true);

		mOnMousePress.InvokeAll(event);
	}

	void RenderViewportWidget::mouseReleaseEvent(QMouseEvent *event)
	{
		if (GetInputState()) GetInputState()->SetKeyState(ToEKeyCode(event->button()), false);

		mOnMouseRelease.InvokeAll(event);
	}

	void RenderViewportWidget::mouseMoveEvent(QMouseEvent *event)
	{
		if (GetInputState()) GetInputState()->SetMousePos(event->x(), event->y());

		mOnMouseMove.InvokeAll(event);
	}

	void RenderViewportWidget::wheelEvent(QWheelEvent *event)
	{
		if (GetInputState()) GetInputState()->SetMouseWheelDelta(event->delta());
	}


#ifdef UPLATFORM_WIN
	EKeyCode UGetEKeyCodeFromQKeyEvent(QKeyEvent* keyEvent)
	{
		unsigned virtualKey = keyEvent->nativeVirtualKey();
		unsigned scanCode = keyEvent->nativeScanCode();
		unsigned nativeModifiers = keyEvent->nativeModifiers();

		unsigned new_vk = virtualKey;
		int extended = (nativeModifiers & 0x01000000) != 0;

		switch (virtualKey)
		{
		case VK_SHIFT:
			new_vk = MapVirtualKey(scanCode, MAPVK_VSC_TO_VK_EX);
			break;
		case VK_CONTROL:
			new_vk = extended ? VK_RCONTROL : VK_LCONTROL;
			break;
		case VK_MENU:
			new_vk = extended ? VK_RMENU : VK_LMENU;
			break;
		default:
			new_vk = virtualKey;
			break;
		}

		return UWin32VKToEKeyCode(new_vk);
	}
#else
#error
#endif // UPLATFORM_WIN

	void RenderViewportWidget::keyPressEvent(QKeyEvent *event)
	{
		EKeyCode key = UGetEKeyCodeFromQKeyEvent(event);
		if (GetInputState()) GetInputState()->SetKeyState(key, true);

	}

	void RenderViewportWidget::keyReleaseEvent(QKeyEvent *event)
	{
		EKeyCode key = UGetEKeyCodeFromQKeyEvent(event);
		if (GetInputState()) GetInputState()->SetKeyState(key, false);
	}

	void RenderViewportWidget::focusInEvent(QFocusEvent *event)
	{
		ULOG_MESSAGE("The method or operation is not implemented.");
		event->accept();
	}

	void RenderViewportWidget::focusOutEvent(QFocusEvent *event)
	{
		ULOG_MESSAGE("The method or operation is not implemented.");
		event->accept();
	}

	void RenderViewportWidget::enterEvent(QEvent *event)
	{
		ULOG_MESSAGE("The method or operation is not implemented.");
	}

	void RenderViewportWidget::leaveEvent(QEvent *event)
	{
		ULOG_MESSAGE("The method or operation is not implemented.");
	}

	void RenderViewportWidget::Tick()
	{
		this->GetInputState()->Tick();
		this->GetInputState()->SetMouseWheelDelta(0);
	}


	class ObjOptionsWidget : public Object
	{
		UCLASS(ObjOptionsWidget, Object)

	public:
		GameWindowOptions mOptions;

		void MetaAfterPropertyChange(const PropertyInfo*)
		{

		}
		void MetaBeforePropertyChange(const PropertyInfo*)
		{

		}
	};

	UCLASS_BEGIN_IMPL(ObjOptionsWidget)
	UPROPERTY(mOptions)
	UCLASS_END_IMPL(ObjOptionsWidget)


	//////////////////////////////////////////////////////////////////////////
	MainViewport::MainViewport(QWidget* parent /*= nullptr*/) : QWidget(parent)
	{
		ui = new Ui_MainViewport;
		ui->setupUi(this);
		ui->mOptionsWidget->setVisible(false);
		
		QToolBar* toolbar = new QToolBar;
		ui->mToolsWidget->layout()->addWidget(toolbar);

		//toggle options widget
		{
			QAction* actionToggle = toolbar->addAction(">>");
			connect(actionToggle, &QAction::triggered, this, [this, actionToggle](bool) {
				ui->mOptionsWidget->setVisible(!ui->mOptionsWidget->isVisible());
				actionToggle->setText(ui->mOptionsWidget->isVisible() ? "<<" : ">>");
			});
		}



		mOptionsWidgetObject = NewObject<ObjOptionsWidget>();
		ui->mOptionsWidget->AttachObject(mOptionsWidgetObject);
		ui->mOptionsWidget->mOnMetaAfterPropertyChange.BindLambda([this](const PropertyInfo* prp){
			this->GetViewport()->mOptions = mOptionsWidgetObject->Cast<ObjOptionsWidget>()->mOptions;
		});

		ui->mViewport->mOnMousePress.BindLambda([this](QMouseEvent* ev) {
			ViewportMousePress(ui->mViewport, ev);
		});
		ui->mViewport->mOnMouseMove.BindLambda([this](QMouseEvent* ev) {
			ViewportMouseMove(ui->mViewport, ev);
		});
		ui->mViewport->mOnMouseRelease.BindLambda([this](QMouseEvent* ev) {
			ViewportMouseRelease(ui->mViewport, ev);
		});
		

	}

	MainViewport::~MainViewport()
	{
		SafeDeleteObject(mOptionsWidgetObject);
	}

	RenderViewportWidget* MainViewport::GetViewport() const
	{
		return ui->mViewport; 
	};


	void MainViewport::SetWorld(World* world)
	{
		GetViewport()->SetWorld(world);
	}

	void MainViewport::Tick()
	{
		ui->mViewport->Tick();
		ui->mOptionsWidget->Tick();
	}

	void MainViewport::ToggleOptionsWidgetVisibility()
	{
		
	}

	void MainViewport::ViewportMousePress(RenderViewportWidget* viewport, QMouseEvent* mouseEvent)
	{

	}

	void MainViewport::ViewportMouseMove(RenderViewportWidget* viewport, QMouseEvent* mouseEvent)
	{
		if (auto hitSelection = viewport->GetHitSelection())
		{
			if (auto takenHit = hitSelection->GetTakenHit())
			{
				QApplication::setOverrideCursor(ToQT(takenHit->GetHoverCursor()));
			}
			else
			{
				QApplication::restoreOverrideCursor();
			}
		}
	}

	void MainViewport::ViewportMouseRelease(RenderViewportWidget* viewport, QMouseEvent* mouseEvent)
	{
		if (mouseEvent->modifiers() == Qt::KeyboardModifier::ControlModifier)
		{
		}

		if(mouseEvent->button() == Qt::MouseButton::LeftButton)
		{
			

			//deselecting 
			gMainWindow->mEntityBrowser->GetWidget()->SelectEntity((EntityBrowserItem*)nullptr, true);


			if (auto hitSelection = viewport->GetHitSelection())
			{
				if (auto takenHit = hitSelection->GetTakenHit())
				{
					if (auto hpEntity = UCast<HPEntity>(takenHit))	//is entity hit?
					{
						if (Entity* entity = hpEntity->mEntity)
						{
							ULOG_MESSAGE("Entity Selected [%]", hpEntity->mEntity->GetName());
							gMainWindow->mEntityBrowser->GetWidget()->SelectEntity(entity);
						}
					}
					else
					{

					}
				}
			}
		}
		
	}

	Qt::CursorShape ToQT(ECursorShape in)
	{
		return (Qt::CursorShape)in;
	}


}