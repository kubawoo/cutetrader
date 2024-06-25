#include <QApplication>
#include <QDebug>
#include <QSqlDatabase>

#include <dbbuilder.h>
#include <migrations.h>

#include "mainwindow.h"


int main(int argc, char *argv[])
{
    QThread::currentThread()->setObjectName("MainThread");
    QApplication a(argc, argv);
    MainWindow w;
    QObject::connect(&a, &QCoreApplication::aboutToQuit, &w, &MainWindow::quit);
    w.show();

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("cutetrader.db");
    DbBuilder::instance().addMigration(new Migration_001);
    DbBuilder::instance().addMigration(new Migration_002);
    DbBuilder::instance().runMigrations();

    return a.exec();
}

