#include <QApplication>
#include <QDebug>
#include <QSqlDatabase>

#include <dbbuilder.h>
#include <migrations.h>

#include "mainwindow.h"

void setupDatabase() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("cutetrader.db");
    db.open();

    DbBuilder dbBuilder(db);
    dbBuilder.addMigration(new Migration_001);
    dbBuilder.addMigration(new Migration_002);
    dbBuilder.runMigrations();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QThread::currentThread()->setObjectName("MainThread");

    setupDatabase();

    MainWindow w;
    QObject::connect(&a, &QCoreApplication::aboutToQuit, &w, &MainWindow::quit);
    w.show();
    return a.exec();
}

