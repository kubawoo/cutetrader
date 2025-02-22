#include <QApplication>
#include <QDebug>
#include <QCommandLineParser>
#include <twsclient.h>
#include <mocked.h>
#include <common.h>
#include "mainwindow.h"
#include "appfactory.h"


void parseCommandLine(QCommandLineParser & parser, QApplication & app) {
    parser.addVersionOption();
    parser.addHelpOption();
    parser.addOptions({
                          {"mocked", "Use mocked API Client (won't connect to TWS)"}
                      });
    parser.process(app);
}

bool setupDatabase(common::IAppFactory * factory) {
    factory->setupDatabase();
    QSqlDatabase db = QSqlDatabase::database();
    data::DbBuilder dbBuilder(db);
    for(auto m : factory->extraMigrations()) {
        dbBuilder.addMigration(m);
    }
    return dbBuilder.runMigrations();
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("cutetrader");
    app.setApplicationVersion(QString(__DATE__) + " " + QString(__TIME__));

    QThread::currentThread()->setObjectName("MainThread");

    QCommandLineParser parser;
    parseCommandLine(parser, app);

    common::IAppFactory * factory;
    if(parser.isSet("mocked")) {
        factory = new mocked::MockAppFactory;
    } else {
        factory = new AppFactory;
    }

    common::ITwsClient * client = factory->createTwsClient();
    bool ok = setupDatabase(factory);
    delete factory;
    if(!ok) {
        qDebug() << "Failed to initialize DB";
        return -1;
    }

    MainWindow w(&app, QSharedPointer<common::ITwsClient>(client));
    QObject::connect(&app, &QCoreApplication::aboutToQuit, &w, &MainWindow::quit);
    w.show();
    return app.exec();
}
