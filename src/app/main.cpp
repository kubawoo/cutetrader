#include <QApplication>
#include <QDebug>

#include <linearregression.h>
#include "mainwindow.h"

void test() {
    QList<qreal> x,y;
    x.append(2); y.append(5);
    x.append(5); y.append(7);
    x.append(2); y.append(6);
    x.append(8); y.append(9);
    x.append(2); y.append(7);

    qDebug() << LinearRegression::calculate(x, y);
}


int main(int argc, char *argv[])
{
       test();
    QThread::currentThread()->setObjectName("MainThread");
    QApplication a(argc, argv);
    MainWindow w;
    QObject::connect(&a, &QCoreApplication::aboutToQuit, &w, &MainWindow::quit);
    w.show();
    return a.exec();
}

