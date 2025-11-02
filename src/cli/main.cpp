#include <QCommandLineParser>
#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QLoggingCategory>
#include <QSqlDatabase>
#include <QThread>
#include "cliappfactory.h"
#include "clicommand.h"
#include "console.h"
#include <account.h>
#include <common.h>
#include <data.h>
#include <mocked.h>

struct AppConfig
{
    QString host;
    int port;
    int clientId;
    QString accountId;
    QSharedPointer<common::ITwsClient> client;
};

void parseCommandLine(QCommandLineParser &parser, QCoreApplication &app)
{
    parser.addVersionOption();
    parser.addHelpOption();
    parser.addOptions({{"mocked", "Use mocked API Client (won't connect to TWS)"},
                       {"database", "Database filename", "database"},
                       {"host", "IBKR gateway/TWS host name", "host", "localhost"},
                       {"port", "IBKR gateway/TWS port number", "port", "4002"},
                       {"client-id", "IBKR's client ID", "client-id", "1"},
                       {"account-id", "IBKR's account id", "account-id"},
                       {"debug", "Enables debug logging"}});
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

bool setupApp(const QCommandLineParser &parser, AppConfig &appConfig)
{
    QLoggingCategory::defaultCategory()->setEnabled(QtMsgType::QtDebugMsg, parser.isSet("debug"));

    bool ok;
    appConfig.host = parser.value("host");
    appConfig.port = parser.value("port").toInt(&ok);
    if (!ok) {
        return false;
    }
    appConfig.clientId = parser.value("client-id").toInt(&ok);
    if (!ok) {
        return false;
    }
    appConfig.accountId = parser.value("account-id");

    QScopedPointer<common::IAppFactory> factory = parser.isSet("mocked")
                                                      ? QScopedPointer<common::IAppFactory>(new mocked::MockAppFactory)
                                                      : QScopedPointer<common::IAppFactory>(new AppFactory());

    appConfig.client.reset(factory->createTwsClient());
    return setupDatabase(factory.get(), parser.value("database"));
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setApplicationName("cutetrader-cli");
    app.setApplicationVersion(QString(__DATE__) + " " + QString(__TIME__));
    QThread::currentThread()->setObjectName("MainThread");

    QCommandLineParser parser;
    parseCommandLine(parser, app);

    AppConfig appConfig;
    if (!setupApp(parser, appConfig)) {
        qDebug() << "Failed to initialize app";
        return -1;
    }

    Console c;
    CliCommandManager commander(appConfig.host,
                                appConfig.port,
                                appConfig.clientId,
                                appConfig.accountId,
                                appConfig.client,
                                &app);

    QObject::connect(&c, &Console::newInput, &commander, &CliCommandManager::command);
    QObject::connect(&commander, &CliCommandManager::quit, &app, &QCoreApplication::quit);
    QObject::connect(&commander, &CliCommandManager::commandDone, &c, &Console::print);

    return app.exec();
}
