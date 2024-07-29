#include <QApplication>
#include "mainwindow.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QThread::currentThread()->setObjectName("MainThread");

    MainWindow w;
    QObject::connect(&a, &QCoreApplication::aboutToQuit, &w, &MainWindow::quit);
    w.show();
    return a.exec();
}
