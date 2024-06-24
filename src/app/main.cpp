#include <QApplication>
#include <QDebug>

#include "mainwindow.h"


int main(int argc, char *argv[])
{
    QThread::currentThread()->setObjectName("MainThread");
    QApplication a(argc, argv);
    MainWindow w;
    QObject::connect(&a, &QCoreApplication::aboutToQuit, &w, &MainWindow::quit);
    w.show();
    return a.exec();
}

