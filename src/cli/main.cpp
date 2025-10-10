#include <QCommandLineParser>
#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QSqlDatabase>
#include <QThread>
#include "cliappfactory.h"
#include "clicommand.h"
#include "console.h"
#include <common.h>
#include <data.h>
#include <mocked.h>

void parseCommandLine(QCommandLineParser &parser, QCoreApplication &app)
{
    parser.addVersionOption();
    parser.addHelpOption();
    parser.addOptions({
                          {"mocked", "Use mocked API Client (won't connect to TWS)"},
                          {"database", "Database filename", "database"}
                      });
    parser.process(app);
}

bool setupDatabase(common::IAppFactory * factory, const QString & dbName) {
    factory->setupDatabase(dbName);
    QSqlDatabase db = QSqlDatabase::database();
    qDebug() << "Using" << db.databaseName() << "database";
    data::DbBuilder dbBuilder(db);
    for(auto m : factory->extraMigrations()) {
        dbBuilder.addMigration(m);
    }
    return dbBuilder.runMigrations();
}

bool setupApp(const QCommandLineParser & parser, QSharedPointer<common::ITwsClient> & client) {

    QScopedPointer<common::IAppFactory> factory = parser.isSet("mocked")
            ? QScopedPointer<common::IAppFactory>(new mocked::MockAppFactory)
            : QScopedPointer<common::IAppFactory>(new AppFactory());

    client.reset(factory->createTwsClient());
    return setupDatabase(factory.get(), parser.value("database"));
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setApplicationName("cutetrader");
    app.setApplicationVersion(QString(__DATE__) + " " + QString(__TIME__));

    QThread::currentThread()->setObjectName("MainThread");

    QCommandLineParser parser;
    parseCommandLine(parser, app);

    QSharedPointer<common::ITwsClient> client;
    bool ok = setupApp(parser, client);
    if (!ok || client.isNull()) {
        qDebug() << "Failed to initialize app";
        return -1;
    }

    client->connect("localhost", 4002, 1);

    Console c;
    CliCommandManager commander(client.data(), &app);

    QObject::connect(&c, &Console::quit, &app, &QCoreApplication::quit);
    QObject::connect(&c, &Console::newInput, &commander, &CliCommandManager::command);
    QObject::connect(&commander, &CliCommandManager::commandDone, &c, &Console::print);

    return app.exec();
}
