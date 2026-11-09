#include <QCommandLineParser>
#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QLoggingCategory>
#include <QMap>
#include <QSqlDatabase>
#include <QThread>
#include <memory>
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
    std::shared_ptr<common::ITwsClient> client;
};

void parseCommandLine(QCommandLineParser &parser, QCoreApplication &app)
{
    parser.addVersionOption();
    parser.addHelpOption();
    parser.addOptions({{"mocked", "Use mocked API Client (won't connect to TWS)"},
                       {"database", "Database filename", "database"},
                       {"host", "IBKR gateway/TWS host name. Default: localhost", "host", "localhost"},
                       {"port", "IBKR gateway/TWS port number. Default: 4002", "port", "4002"},
                       {"client-id", "IBKR's client ID. Default: 1", "client-id", "1"},
                       {"account-id", "IBKR's account id. Required if multiple accounts available", "account-id"},
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
    QLoggingCategory::defaultCategory()->setEnabled(QtMsgType::QtCriticalMsg, true);

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

    std::unique_ptr<common::IAppFactory> factory;
    if (parser.isSet("mocked")) {
        factory = std::make_unique<mocked::MockAppFactory>();
    } else {
        factory = std::make_unique<AppFactory>();
    }

    appConfig.client = std::shared_ptr<common::ITwsClient>(factory->createTwsClient());
    return setupDatabase(factory.get(), parser.value("database"));
}

#ifndef GIT_COMMIT_HASH
#define GIT_COMMIT_HASH "unknown"
#endif

// Parse __DATE__ (format: "MMM DD YYYY" or "MMM  D YYYY") to ISO format "YYYY-MM-DD"
static QString parseBuildDate(const char *dateStr)
{
    QString s = QString::fromLatin1(dateStr).simplified(); // "Nov 3 2026"
    QStringList parts = s.split(' ');
    if (parts.size() != 3)
        return QString::fromLatin1(dateStr);

    static const QMap<QString, QString> monthMap = {
        {"Jan", "01"}, {"Feb", "02"}, {"Mar", "03"}, {"Apr", "04"},
        {"May", "05"}, {"Jun", "06"}, {"Jul", "07"}, {"Aug", "08"},
        {"Sep", "09"}, {"Oct", "10"}, {"Nov", "11"}, {"Dec", "12"}
    };

    QString month = monthMap.value(parts[0], "01");
    QString day = parts[1].rightJustified(2, '0');
    QString year = parts[2];
    return year + "-" + month + "-" + day;
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setApplicationName("cutetrader-cli");
    app.setApplicationVersion(QString("ver. %1 built on %2 %3")
                                  .arg(GIT_COMMIT_HASH)
                                  .arg(parseBuildDate(__DATE__))
                                  .arg(__TIME__));
    QThread::currentThread()->setObjectName("MainThread");

    QCommandLineParser parser;
    parseCommandLine(parser, app);

    AppConfig appConfig;
    if (!setupApp(parser, appConfig)) {
        qCritical() << "Failed to initialize app";
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