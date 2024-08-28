
#include "mainwindow.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qRegisterMetaType<std::size_t>();
    qRegisterMetaType<LogFile>();
    qRegisterMetaType<LogField>();
    qRegisterMetaType<WorkerDialog>();
    qRegisterMetaType<Blacklists>();

    MainWindow w;
    w.show();
    return a.exec();
}
