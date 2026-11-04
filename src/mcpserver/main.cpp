// Copyright (C) 2025 Signal Slot Inc.
// SPDX-License-Identifier: BSD-3-Clause

#include <QCommandLineParser>
#include <QCoreApplication>
#include <QDebug>
#include <QFuture>
#include <QJsonObject>
#include <QJsonArray>
#include <QPromise>
#include <QTimer>

#include <QtMcpServer/QMcpServer>
#include <QtMcpCommon/QMcpCallToolRequest>
#include <QtMcpCommon/QMcpCallToolResult>
#include <QtMcpCommon/QMcpCallToolResultContent>
#include <QtMcpCommon/QMcpTextContent>
#include <QtMcpCommon/QMcpJSONRPCErrorError>
#include <QtMcpCommon/QMcpTool>
#include <QtMcpCommon/QMcpToolInputSchema>

#include <common/itwsclient.h>
#include <twsqapi/twsclient.h>
#include <mocked/twsclientmock.h>

using namespace Qt::Literals::StringLiterals;

// Tool object that can be registered for tool listing
class TwsToolSet : public QObject
{
    Q_OBJECT
public:
    explicit TwsToolSet(QObject *parent = nullptr) : QObject(parent) {}

    Q_INVOKABLE QFuture<QMcpCallToolResult> getServerTime()
    {
        QPromise<QMcpCallToolResult> promise;
        promise.start();
        QMcpCallToolResult result;
        result.setContent({QMcpTextContent("Time will be fetched async")});
        promise.addResult(result);
        promise.finish();
        return promise.future();
    }

    Q_INVOKABLE QFuture<QMcpCallToolResult> getAccountDetails()
    {
        QPromise<QMcpCallToolResult> promise;
        promise.start();
        QMcpCallToolResult result;
        result.setContent({QMcpTextContent("Account details will be fetched async")});
        promise.addResult(result);
        promise.finish();
        return promise.future();
    }
};

class TwsMcpServer : public QMcpServer
{
    Q_OBJECT
public:
    explicit TwsMcpServer(common::ITwsClient *twsClient, const QString &backend = "stdio", QObject *parent = nullptr)
        : QMcpServer(backend, parent)
        , _twsClient(twsClient)
    {
        // Note: Real TwsClient doesn't emit connectedSignal/disconnectedSignal
        // It relies on isConnected() which checks the underlying socket state
        // Mocked client does emit these signals for compatibility
        connect(_twsClient, &common::ITwsClient::connectedSignal, this, [this]() {
            qDebug() << "TWS connected (signal)";
            emit started();
        });
        connect(_twsClient, &common::ITwsClient::disconnectedSignal, this, [this]() {
            qDebug() << "TWS disconnected (signal)";
        });
        connect(_twsClient, &common::ITwsClient::errorSignal, this, [this](const QString &reason, bool fatal) {
            qWarning() << "TWS error:" << reason << "fatal:" << fatal;
        });

        // Cache account data from signals
        connect(_twsClient, &common::ITwsClient::accountValueUpdatedSignal, this, [this](const QString &key, const QString &value, const QString &currency) {
            _accountValues[key] = QString("%1 %2").arg(value, currency);
        });
        connect(_twsClient, &common::ITwsClient::portfolioPositionUpdatedSignal, this, [this](const common::PortfolioPositionDTO &position) {
            // Use contractId as key to support multiple positions for same symbol (e.g., stock + option)
            _positions[position.contractId] = position;
        });
        connect(_twsClient, &common::ITwsClient::updateAccountTimeSignal, this, [this](const QTime &time) {
            _accountUpdateTime = time;
        });
        connect(_twsClient, &common::ITwsClient::managedAccountsSignal, this, [this](const QStringList &accounts) {
            qDebug() << "Received managed accounts:" << accounts;
            _managedAccounts = accounts;
            // Auto-subscribe to first account if we have one and haven't started yet
            if (!accounts.isEmpty() && !_accountSubscribed) {
                _twsClient->startClient(accounts.first());
                _accountSubscribed = true;
            }
        });

        // Register tool set for tool listing
        _toolSet = new TwsToolSet(this);
        registerToolSet(_toolSet, {
            { "getServerTime", "Returns the current server time from TWS" },
            { "getAccountDetails", "Returns account summary and positions" }
        });

        // Register request handler for custom tools
        addRequestHandler([this](const QUuid &session, const QMcpCallToolRequest &req,
                                 QMcpJSONRPCErrorError *error) -> QFuture<QMcpCallToolResult> {
            Q_UNUSED(error);
            const auto params = req.params();
            const QString toolName = params.name();
            const QJsonObject args = params.arguments();

            if (toolName == "getServerTime") {
                return getServerTimeAsync();
            }
            if (toolName == "getAccountDetails") {
                return getAccountDetailsAsync();
            }

            // Fallback: return error
            QPromise<QMcpCallToolResult> promise;
            promise.start();
            QMcpCallToolResult result;
            result.setIsError(true);
            result.setContent({QMcpTextContent(QString("Unknown tool: %1").arg(toolName))});
            promise.addResult(result);
            promise.finish();
            return promise.future();
        });
    }

    void connectToTws(const QString &host, int port, int clientId)
    {
        _twsHost = host;
        _twsPort = port;
        _twsClientId = clientId;
        _tryConnect();
    }

    void reconnect()
    {
        _accountSubscribed = false;
        _managedAccounts.clear();
        _tryConnect();
    }

    bool isTwsConnected() const { return _twsClient && _twsClient->isConnected(); }

    QHash<QString, QString> toolDescriptions() const override
    {
        return {
            { "getServerTime", "Returns the current server time from TWS" },
            { "getAccountDetails", "Returns account summary and positions" }
        };
    }

signals:
    void twsConnected();
    void twsDisconnected();

private:
    void _tryConnect()
    {
        if (!_twsClient->connect(_twsHost, _twsPort, _twsClientId)) {
            qWarning() << "Failed to connect to TWS at" << _twsHost << ":" << _twsPort;
        }
    }

    QFuture<QMcpCallToolResult> getServerTimeAsync()
    {
        auto promise = std::make_shared<QPromise<QMcpCallToolResult>>();
        promise->start();

        if (!isTwsConnected()) {
            QMcpCallToolResult result;
            result.setIsError(true);
            result.setContent({QMcpTextContent("Not connected to TWS")});
            promise->addResult(result);
            promise->finish();
            return promise->future();
        }

        QMetaObject::Connection conn;
        conn = connect(_twsClient, &common::ITwsClient::currentTimeSignal,
                       this, [this, promise, conn](const QDateTime &time) {
            disconnect(conn);

            QMcpCallToolResult result;
            result.setContent({QMcpTextContent(time.toString(Qt::ISODate))});
            promise->addResult(result);
            promise->finish();
        });

        _twsClient->requestCurrentTime();

        QTimer::singleShot(10000, this, [this, promise, conn]() {
            if (promise->future().isRunning()) {
                disconnect(conn);
                QMcpCallToolResult result;
                result.setIsError(true);
                result.setContent({QMcpTextContent("Timeout waiting for server time")});
                promise->addResult(result);
                promise->finish();
            }
        });

        return promise->future();
    }

    QFuture<QMcpCallToolResult> getAccountDetailsAsync()
    {
        auto promise = std::make_shared<QPromise<QMcpCallToolResult>>();
        promise->start();

        if (!isTwsConnected()) {
            QMcpCallToolResult result;
            result.setIsError(true);
            result.setContent({QMcpTextContent("Not connected to TWS")});
            promise->addResult(result);
            promise->finish();
            return promise->future();
        }

        // Request account updates to refresh data
        if (_managedAccounts.isEmpty()) {
            _twsClient->requestManagedAccounts();
        }

        // If we have accounts but haven't subscribed yet, do it now
        if (!_managedAccounts.isEmpty() && !_accountSubscribed) {
            _twsClient->startClient(_managedAccounts.first());
            _accountSubscribed = true;
        }

        // Build result from cached data - wait a bit for updates to arrive
        QTimer::singleShot(1000, this, [this, promise]() {
            QStringList lines;
            lines << "=== Account Summary ===";
            lines << QString("Update Time: %1").arg(_accountUpdateTime.toString("HH:mm:ss"));
            lines << QString("Accounts: %1").arg(_managedAccounts.join(", "));
            lines << "";

            lines << "--- Values ---";
            if (_accountValues.isEmpty()) {
                lines << "(no account values received yet)";
            } else {
                for (auto it = _accountValues.constBegin(); it != _accountValues.constEnd(); ++it) {
                    lines << QString("%1: %2").arg(it.key(), it.value());
                }
            }
            lines << "";

            lines << "--- Positions ---";
            if (_positions.isEmpty()) {
                lines << "(no positions received yet - make sure startClient was called with valid account ID)";
            } else {
                for (auto it = _positions.constBegin(); it != _positions.constEnd(); ++it) {
                    const auto &pos = it.value();
                    QString secTypeStr;
                    QString extraInfo;
                    switch (pos.securityType) {
                        case common::SecurityType::STOCK: secTypeStr = "STK"; break;
                        case common::SecurityType::OPTION: {
                            secTypeStr = "OPT";
                            QString rightStr = (pos.right == common::OptionType::CALL) ? "CALL" : "PUT";
                            extraInfo = QString(" %1 strike=%2 exp=%3").arg(rightStr).arg(pos.strike).arg(pos.expiration.toString("yyyy-MM-dd"));
                            break;
                        }
                        case common::SecurityType::FUTURE: secTypeStr = "FUT"; break;
                        case common::SecurityType::FUTURE_OPTION: {
                            secTypeStr = "FOP";
                            QString rightStr = (pos.right == common::OptionType::CALL) ? "CALL" : "PUT";
                            extraInfo = QString(" %1 strike=%2 exp=%3").arg(rightStr).arg(pos.strike).arg(pos.expiration.toString("yyyy-MM-dd"));
                            break;
                        }
                        default: secTypeStr = "UNK"; break;
                    }
                    lines << QString("%1 (%2)%3: %4 shares @ %5 = %6 (PnL: %7)")
                        .arg(pos.symbol, secTypeStr, extraInfo,
                             QString::number(pos.position),
                             QString::number(pos.marketPrice),
                             QString::number(pos.marketValue),
                             QString::number(pos.unrealizedPNL));
                }
            }

            QMcpCallToolResult result;
            result.setContent({QMcpTextContent(lines.join("\n"))});
            promise->addResult(result);
            promise->finish();
        });

        return promise->future();
    }

    common::ITwsClient *_twsClient = nullptr;
    TwsToolSet *_toolSet = nullptr;
    QString _twsHost;
    int _twsPort = 4002;
    int _twsClientId = 1;

    // Cached account data
    QHash<QString, QString> _accountValues;
    QHash<long, common::PortfolioPositionDTO> _positions;
    QTime _accountUpdateTime;
    QStringList _managedAccounts;
    bool _accountSubscribed = false;
};

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setApplicationName("cutetrader-mcpserver");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("cutetrader");

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption backendOption(QStringList() << "b" << "backend",
                                   "Backend to use (stdio/sse/streamablehttp).",
                                   "backend", "streamablehttp");
    parser.addOption(backendOption);

    QCommandLineOption addressOption(QStringList() << "a" << "address",
                                   "Address to listen on (host:port).",
                                   "address", "127.0.0.1:8000");
    parser.addOption(addressOption);

    QCommandLineOption hostOption(QStringList() << "H" << "host",
                                "TWS/IB Gateway host.",
                                "host", "localhost");
    parser.addOption(hostOption);

    QCommandLineOption portOption(QStringList() << "p" << "port",
                                "TWS/IB Gateway port.",
                                "port", "4002");
    parser.addOption(portOption);

    QCommandLineOption clientIdOption(QStringList() << "c" << "client-id",
                                    "IBKR client ID.",
                                    "client-id", "1");
    parser.addOption(clientIdOption);

    QCommandLineOption mockedOption("mocked", "Use mocked TWS client (no real TWS connection needed)");
    parser.addOption(mockedOption);

    parser.process(app);

    const QString backend = parser.value(backendOption);
    const QString address = parser.value(addressOption);
    const QString host = parser.value(hostOption);
    const int port = parser.value(portOption).toInt();
    const int clientId = parser.value(clientIdOption).toInt();
    const bool useMocked = parser.isSet(mockedOption);

    // Create TWS client (real or mocked)
    common::ITwsClient *twsClient = nullptr;
    if (useMocked) {
        twsClient = new mocked::TwsClientMock(&app);
        qDebug() << "Using mocked TWS client";
    } else {
        twsClient = new twsqapi::TwsClient(&app);
    }

    TwsMcpServer server(twsClient, backend);
    QObject::connect(&server, &QMcpServer::finished, &app, &QCoreApplication::quit);

    // Connect to TWS (non-blocking, server continues running even if connection fails)
    server.connectToTws(host, port, clientId);

    // Start the MCP server
    // HTTP backends (sse, streamablehttp) need an address
    if (backend == "sse" || backend == "streamablehttp") {
        server.start(address);
    } else {
        server.start();
    }

    return app.exec();
}

#include "main.moc"