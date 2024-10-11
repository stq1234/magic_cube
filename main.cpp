#include "widget.h"
#include <QApplication>
#include <globel.h>
#include <QScreen>

void initializer();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if(QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    initializer();
//    file.open("D:/test.txt");
    Widget w;
//    w.show();
    w.showFullScreen();
//    w.show();

    return a.exec();
}

void initializer()
{

    QScreen* screen = QGuiApplication::primaryScreen();
    QSize screen_size = screen->size();
    application_height = screen_size.height();
    application_width = screen_size.width();
}
