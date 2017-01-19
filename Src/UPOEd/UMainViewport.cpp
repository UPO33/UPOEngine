#include "UMainViewport.h"
#include "UPropertyBrowser.h"
#include "UMainWindow.h"

#include "../UPOEngine/Engine/UInput.h"


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


	MainViewport::MainViewport(QWidget* parent /*= nullptr*/) : D3DRenderWidget(parent)
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

	void MainViewport::mousePressEvent(QMouseEvent *event)
	{
		if (GetInputState()) GetInputState()->SetKeyState(ToEKeyCode(event->button()), true);
	}

	void MainViewport::mouseReleaseEvent(QMouseEvent *event)
	{
		if (GetInputState()) GetInputState()->SetKeyState(ToEKeyCode(event->button()), false);
	}

	void MainViewport::mouseMoveEvent(QMouseEvent *event)
	{
		if (GetInputState()) GetInputState()->SetMousePos(event->x(), event->y());
	}

	void MainViewport::wheelEvent(QWheelEvent *event)
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

	void MainViewport::keyPressEvent(QKeyEvent *event)
	{
		EKeyCode key = UGetEKeyCodeFromQKeyEvent(event);
		ULOG_MESSAGE("key %", EnumToStr(key));
		if (GetInputState()) GetInputState()->SetKeyState(key, true);

	}

	void MainViewport::keyReleaseEvent(QKeyEvent *event)
	{
		EKeyCode key = UGetEKeyCodeFromQKeyEvent(event);
		ULOG_MESSAGE("key %", EnumToStr(key));
		if (GetInputState()) GetInputState()->SetKeyState(key, false);
	}

	void MainViewport::focusInEvent(QFocusEvent *event)
	{
		ULOG_MESSAGE("The method or operation is not implemented.");
		event->accept();
	}

	void MainViewport::focusOutEvent(QFocusEvent *event)
	{
		ULOG_MESSAGE("The method or operation is not implemented.");
		event->accept();
	}

	void MainViewport::enterEvent(QEvent *event)
	{
		ULOG_MESSAGE("The method or operation is not implemented.");
	}

	void MainViewport::leaveEvent(QEvent *event)
	{
		ULOG_MESSAGE("The method or operation is not implemented.");
	}

	void MainViewport::Tick()
	{
		this->GetInputState()->Tick();
		this->GetInputState()->SetMouseWheelDelta(0);
	}

}