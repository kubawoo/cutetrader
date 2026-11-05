#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QCommandLineParser>
#include <QLoggingCategory>
#include <QSqlDatabase>
#include <QThread>
#include <QTimer>
#include "guiappfactory.h"
#include "connectionmanager.h"
#include <account.h>
#include <common.h>
#include <data.h>
#include <mocked.h>

static QObject *connectionManagerSingletonProvider(QQmlEngine *, QJSEngine *) {
    return nullptr; // We'll set the instance manually
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setApplicationName("cutetrader-gui");
    app.setOrganizationName("cutetrader");
    QThread::currentThread()->setObjectName("MainThread");

    QCommandLineParser parser;
    parser.addVersionOption();
    parser.addHelpOption();
    parser.addOptions({{"mocked", "Use mocked API Client (won't connect to TWS)"},
                       {"database", "Database filename", "database", "cutetrader.db"},
                       {"host", "IBKR gateway/TWS host name. Default: localhost", "host", "localhost"},
                       {"port", "IBKR gateway/TWS port number. Default: 4002", "port", "4002"},
                       {"client-id", "IBKR's client ID. Default: 1", "client-id", "1"},
                       {"account-id", "IBKR's account id", "account-id"},
                       {"debug", "Enables debug logging"}});
    parser.process(app);

    QLoggingCategory::defaultCategory()->setEnabled(QtMsgType::QtDebugMsg, parser.isSet("debug"));
    QLoggingCategory::defaultCategory()->setEnabled(QtMsgType::QtCriticalMsg, true);

    QScopedPointer<common::IAppFactory> factory(new GuiAppFactory());

    factory->setupDatabase(parser.value("database"));
    QSqlDatabase db = QSqlDatabase::database();
    qDebug() << "Using" << db.databaseName() << "database";

    data::DbBuilder dbBuilder(db);
    for (auto m : factory->extraMigrations()) {
        dbBuilder.addMigration(m);
    }
    if (!dbBuilder.runMigrations()) {
        qCritical() << "Failed to run database migrations";
        return -1;
    }

    ConnectionManager *connectionManager = new ConnectionManager();
    connectionManager->setHost(parser.value("host"));
    connectionManager->setPort(parser.value("port").toInt());
    connectionManager->setClientId(parser.value("client-id").toInt());
    connectionManager->setAccountId(parser.value("account-id"));
    connectionManager->setUseMock(parser.isSet("mocked"));

    // Register as singleton
    qmlRegisterSingletonInstance("cutetrader", 1, 0, "ConnectionManager", connectionManager);

    QQmlApplicationEngine engine;

    const QUrl url(QStringLiteral("qrc:/cutetrader/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
                     &app, []() { QCoreApplication::exit(-1); },
                     Qt::QueuedConnection);
    engine.load(url);

    if (parser.isSet("mocked")) {
        QTimer::singleShot(500, connectionManager, &ConnectionManager::connectToTws);
    }

    return app.exec();
}