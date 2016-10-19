#include "../UPOEngine/UPOEngine.h"
#include "UMainWindow.h"
#include "UEditorConfig.h"
#include "UD3DRenderWidget.h"
#include "../UPOEngine/GFXCore/URenderer.h"

int main(int argc, char *argv[])
{
	using namespace UPO;

    QApplication a(argc, argv);
    //QApplication::setStyle(QStyleFactory::create("Fusion"));
	
	//ULOG_FATAL("");
	//UASSERT(false);
	//UASSERT(false);

    QLocale::setDefault(QLocale::c());

	UPO::Buffer styleSheet;
	if (UPO::File::OpenReadFull(UPOEd::EditorConfig::Get()->AsName("Style").CStr(), styleSheet))
	{
		QString strSheet = QString::fromLatin1((const char*)styleSheet.Data(), (int)styleSheet.Size());
		a.setStyleSheet(strSheet);
	}
	UPOEd::EditorConfig::Get()->PrintDbg();
	
#if 1
    UPOEd::MainWindow mainWnd;
    mainWnd.showMaximized();


	while (mainWnd.isVisible())
	{
		QApplication::processEvents();

		mainWnd.Tick();

		GFXRenderer::Get()->RenderFrame();

		Thread::Sleep(30);
	}
#else
	UPOEd::D3DRenderWidget* rw = new UPOEd::D3DRenderWidget;
	rw->show();

#endif // 0

	QApplication::exit();
}
