
#include "mainwindow.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    qRegisterMetaType<size_t>("size_t");

    QApplication::setAttribute( Qt::AA_EnableHighDpiScaling );
    QApplication a(argc, argv);

    MainWindow w;
    w.show();
    return a.exec();
}
