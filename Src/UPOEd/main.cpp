#include "../UPOEngine/UPOEngine.h"
#include "UMainWindow.h"
#include "UEditorConfig.h"
#include "UD3DRenderWidget.h"

#include <qfilesystemwatcher.h>



#include "../UPOEngine/GFXCore/UGFXCore.h"

namespace UPOEd
{
	using namespace UPO;
	QFileSystemWatcher* fsw = new QFileSystemWatcher(nullptr);
	

	class EngineEd : public IEngineInterface
	{
		QApplication* mQApp;
		UPOEd::MainWindow* mMainWindow;

		//////////////////////////////////////////////////////////////////////////
		virtual bool OnInit() override
		{
			mQApp = new QApplication(__argc, __argv);

			QLocale::setDefault(QLocale::c());
			QApplication::setStyle("Fusion");
			/////loading style sheet
			{
				UPO::Buffer styleSheet;
				if (UPO::File::OpenReadFull(UPOEd::GEditorConfig()->AsName("Style").CStr(), styleSheet))
				{
					QString strSheet = QString::fromLatin1((const char*)styleSheet.Data(), (int)styleSheet.Size());
					mQApp->setStyleSheet(strSheet);
				}
			}

			mMainWindow = new UPOEd::MainWindow();
			mMainWindow->showMaximized();
			return true;
		}

		virtual bool OnTick() override
		{
			QApplication::processEvents();

			mMainWindow->Tick();

			return mMainWindow->isVisible();
		}

		virtual bool OnRelease() override
		{
			QApplication::exit();
			delete mMainWindow;
			return true;
		}

		virtual GameWindow* OnCreateGameWindow() override
		{
			return mMainWindow->mMainViewport;
		}

		virtual void OnReleaseGameWindow() override
		{
		}

	};
}


int main(int argc, char *argv[])
{
// 	UPO::File theFile;
// 	theFile.Open("C:/asd/asd.txt", UPO::EFileOpenMode::Write);
// 	UASSERT(theFile.IsOpen());

	UPO::LaunchEngine(new UPOEd::EngineEd);
}
