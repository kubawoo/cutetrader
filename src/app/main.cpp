#include <QApplication>
#include "mainwindow.h"
#include <QDebug>
#include "twsclient.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QThread::currentThread()->setObjectName("MainThread");

    qDebug() << "CLI command";
    for(int i = 0; i < argc; i++) {
        qDebug() << i << argv[i];
    }



    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    twsclient::ITwsClient * client;
    //TODO: add proper args parsing
    if(argc > 1 && QString(argv[1]) == "--mocked") {
        client = new twsclient::TwsClientMock;
        db.setDatabaseName("cutetrader_mocked.db");
    } else {
        client = new twsclient::TwsClient;
        db.setDatabaseName("cutetrader.db");
    }
    db.open();


    MainWindow w(&a, client);
    QObject::connect(&a, &QCoreApplication::aboutToQuit, &w, &MainWindow::quit);
    w.show();
    return a.exec();
}
