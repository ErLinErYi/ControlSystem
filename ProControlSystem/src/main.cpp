#include "ProControlSystem.h"
#include <QtWidgets/QApplication>
#include <qfile.h>

int main(int argc, char *argv[])
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QApplication a(argc, argv);

    QFile myFile("res/CSViewStyle.qss");
    if (myFile.open(QIODevice::ReadOnly))
    {
        a.setStyleSheet(myFile.readAll());
        myFile.close();
    }

    ProControlSystem w;
    w.show();
    return a.exec();
}
