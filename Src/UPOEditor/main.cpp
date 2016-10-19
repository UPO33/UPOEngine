#include "../UPOEngine/UPOEngine.h"
#include "UMainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setStyle(QStyleFactory::create("Fusion"));
	
	//ULOG_FATAL("");
	//UASSERT(false);
	//UASSERT(false);

    QLocale::setDefault(QLocale::c());

    UPOEd::MainWindow mainWnd;
    mainWnd.showMaximized();

	while (mainWnd.isVisible())
	{
		QApplication::processEvents();

		mainWnd.Tick();

		UPO::Thread::Sleep(30);
	}
	
	QApplication::exit();
}
