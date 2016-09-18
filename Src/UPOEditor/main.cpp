#include "../UPOEngine/UPOEngine.h"
#include "UMainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    QLocale::setDefault(QLocale::c());

    UPOEd::MainWindow mainWnd;
    mainWnd.show();

    return a.exec();
}
