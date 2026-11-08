#include "clicommand.h"
#include <QTimer>

CliCommandManager::CliCommandManager(const QString &host,
                                     int port,
                                     int clientId,
                                     const QString &accountId,
                                     QSharedPointer<common::ITwsClient> client,
                                     QObject *parent)
    : QObject{parent}
    , _host(host)
    , _port(port)
    , _clientId(clientId)
    , _accountId(accountId)
    , _client(client)
{
    QSqlDatabase db = QSqlDatabase::database();
    _dataManager = data::DataManager(db);

    setupCommands();

    connect(client.data(),
            &common::ITwsClient::accountValueUpdatedSignal,
            &_account,
            &account::Account::updateAccountValue);
    connect(client.data(),
            &common::ITwsClient::portfolioPositionUpdatedSignal,
            &_account,
            &account::Account::updatePortfolioPosition);
    connect(client.data(), &common::ITwsClient::managedAccountsSignal, this, &CliCommandManager::onManagedAccounts);
    connect(client.data(), &common::ITwsClient::errorSignal, this, &CliCommandManager::onError);

    QTimer::singleShot(0, this, &CliCommandManager::start);
}

void CliCommandManager::start()
{
    qDebug() << "About to connect to " << _host + ":" + QString::number(_port)
             << "with clientId=" + QString::number(_clientId);
    _client->connect(_host, _port, _clientId);
    if (!_client->isConnected()) {
        qDebug() << "Failed to connect";
        quitCommand();
    }
}

void CliCommandManager::onError(const QString &reason, bool fatal)
{
    if (fatal) {
        qCritical() << reason;
        quitCommand();
    } else {
        qWarning() << reason;
    }
}

void CliCommandManager::command(const QString &cmd)
{
    QStringList args = cmd.split(' ', Qt::SkipEmptyParts);
    QString command = args[0];
    QStringList params = args.sliced(1);

    if (_commands.contains(command)) {
        _commands[command]->execute(params); // parse params
    } else {
        emit commandDone("Unrecognized command '" + command + "'. Type 'help' for available commands.");
    }
}

void CliCommandManager::commandResult(const QString &cmd)
{
    emit commandDone(cmd);
}

void CliCommandManager::quitCommand()
{
    emit quit();
}

void CliCommandManager::onManagedAccounts(const QStringList &accounts)
{
    qDebug() << "accountId: " << _accountId;
    qDebug() << "accounts: " << accounts;
    if (_accountId.isEmpty()) {
        if(accounts.size() == 1) {
            _accountId = accounts[0];
        } else {
            qCritical() << "Please specify account, Available accounts: " << accounts;
            quitCommand();
            return;
        }
    }

    if (!accounts.contains(_accountId)) {
        qCritical() << "Account" << _accountId << " not found in " << accounts;
        quitCommand();
        return;
    }
    _client->startClient(_accountId);
}

void CliCommandManager::setupCommands()
{
    _commands["account"] = new AccountSummaryCliCommand(_account, _accountId, this);
    _commands["help"] = new HelpCommand(this);
    _commands["time"] = new ServerTimeCliCommand(_client, this);
    _commands["watchlist"] = new WatchlistCliCommand(_dataManager, this);
    _commands["search"] = new SearchSymbolsCliCommand(_client, this);
    _commands["historical"] = new HistoricalDataCliCommand(_client, this);
    _commands["cancel-historical"] = new CancelHistoricalDataCliCommand(_client, this);
    _commands["orders"] = new OrderListCliCommand(_client, this);

    for (auto c : _commands.values()) {
        connect(c, &CliCommand::finished, this, &CliCommandManager::commandResult);
    }

    _commands["quit"] = new QuitCommand(this);
    connect(_commands["quit"], &QuitCommand::finished, this, &CliCommandManager::quitCommand);
}

ServerTimeCliCommand::ServerTimeCliCommand(QSharedPointer<common::ITwsClient> client, QObject *parent)
    : CliCommand{parent}
    , _client(client)
{
    QObject::connect(_client.data(), &common::ITwsClient::currentTimeSignal, this, &ServerTimeCliCommand::serverTime);
}

void ServerTimeCliCommand::execute(const QStringList &params)
{
    _client->requestCurrentTime();
}

void ServerTimeCliCommand::serverTime(const QDateTime &time)
{
    emit finished(time.toString(Qt::DateFormat::ISODate));
}

CliCommand::CliCommand(QObject *parent)
    : QObject{parent}
{}

HelpCommand::HelpCommand(CliCommandManager *mgr)
    : CliCommand(mgr)
    , _commandManager(mgr)
{}

void HelpCommand::execute(const QStringList &params)
{
    QStringList commands = _commandManager->commands().keys();
    commands.sort();
    QString s;
    for (auto cmd : commands) {
        s += cmd + "\n\t" + _commandManager->commands()[cmd]->description() + "\n";
    }
    emit finished(s);
}

QuitCommand::QuitCommand(CliCommandManager *mgr)
    : CliCommand(mgr)
    , _commandManager(mgr)
{}

void QuitCommand::execute(const QStringList &params)
{
    emit finished();
}

AccountSummaryCliCommand::AccountSummaryCliCommand(account::Account &account, const QString &accountId, QObject *parent)
    : CliCommand(parent)
    , _account(account)
    , _accountId(accountId)
{}

void AccountSummaryCliCommand::execute(const QStringList &params)
{
    QString s = "Account:\t\t " + _accountId + "\n";
    s += "Base Currency:\t\t " + _account.baseCurrency() + "\n";
    s += "Net Liquidation:\t " + QString::number(_account.accountInfo(account::AccountInfoType::NetLiquidation)) + "\n";
    s += "Excess Liquidity:\t " + QString::number(_account.accountInfo(account::AccountInfoType::ExcessLiquidity))
         + "\n";
    s += "Maint Margin Req:\t " + QString::number(_account.accountInfo(account::AccountInfoType::MaintMarginReq))
         + "\n";
    s += "Stock Market Value:\t " + QString::number(_account.accountInfo(account::AccountInfoType::StockMarketValue))
         + "\n";
    s += "Unrealized PnL:\t\t " + QString::number(_account.accountInfo(account::AccountInfoType::UnrealizedPnL)) + "\n";

    s += "\n\nPortfolio postions:\n";
    if (!_account.portfolio().stocks().empty()) {
        s += "Stocks:\n";
        for (auto stock : _account.portfolio().stocks()) {
            s += " " + stock.symbol() + " " + QString::number(stock.position()) + " "
                 + QString::number(stock.marketPrice()) + " " + QString::number(stock.marketValue()) + "\n";
        }
        s += "\n";
    }
    if (!_account.portfolio().futures().empty()) {
        s += "Futures:\n";
        for (auto future : _account.portfolio().futures()) {
            s += " " + future.symbol() + " " + QString::number(future.position()) + " "
                 + QString::number(future.marketPrice()) + " " + QString::number(future.marketValue()) + "\n";
        }
        s += "\n";
    }
    if (!_account.portfolio().options().empty()) {
        s += "Options:\n";
        for (auto option : _account.portfolio().options()) {
            s += " " + option.symbol() + " " + option.typeAsString() + " strike=" + QString::number(option.strike())
                 + " exp=" + option.expiration().toString("yyyy-MM-dd")
                 + " " + QString::number(option.position()) + " "
                 + QString::number(option.marketPrice()) + " " + QString::number(option.marketValue()) + "\n";
        }
        s += "\n";
    }

    s += "Cash:\t " + QString::number(_account.accountInfo(account::AccountInfoType::CashBalance));
    emit finished(s);
}

WatchlistCliCommand::WatchlistCliCommand(data::DataManager &dataManager, QObject *parent)
    : CliCommand(parent)
    , _dataManager(dataManager)
{}

void WatchlistCliCommand::execute(const QStringList &params)
{
    if (!params.empty() && params[0] == "show") {
        QString s;
        for (auto i : _dataManager.getAllSecurities()) {
            s += i.symbol() + "\n";
        }
        emit finished(s);
    } else {
        QString s = "Usage:\n\twatchlist [action] [options]\n\nAvailable actions:\n\tshow - displays the watchlist";
        emit finished(s);
    }
}

SearchSymbolsCliCommand::SearchSymbolsCliCommand(QSharedPointer<common::ITwsClient> client, QObject *parent)
    : CliCommand{parent}
    , _client{client}
{
    QObject::connect(_client.data(), &common::ITwsClient::matchingSymbolsReadySignal,
                     this, &SearchSymbolsCliCommand::onMatchingSymbolsReady);
}

void SearchSymbolsCliCommand::execute(const QStringList &params)
{
    if (params.empty()) {
        emit finished("Usage: search <symbol pattern>");
        return;
    }
    if (_waiting) {
        emit finished("Already waiting for search results. Please wait.");
        return;
    }
    _waiting = true;
    _client->requestMatchingSymbols(params[0], &_reqId);
}

void SearchSymbolsCliCommand::onMatchingSymbolsReady(const int &requestId, const QList<common::ContractDetailsDTO> &details)
{
    if (!_waiting || requestId != _reqId)
        return;
    _waiting = false;

    QString s;
    if (details.isEmpty()) {
        s = "No symbols found";
    } else {
        s += "Contract ID\tSymbol\tDescription\tCurrency\n";
        s += "----------------------------------------\n";
        for (auto const &d : details) {
            s += QString::number(d.contractId) + "\t" + d.symbol + "\t" + d.description + "\t" + d.currency + "\n";
        }
    }
    emit finished(s);
}

HistoricalDataCliCommand::HistoricalDataCliCommand(QSharedPointer<common::ITwsClient> client, QObject *parent)
    : CliCommand{parent}
    , _client(client)
{
    QObject::connect(_client.data(), &common::ITwsClient::historicalDataReadySignal,
                     this, &HistoricalDataCliCommand::onHistoricalDataReady);
}

void HistoricalDataCliCommand::execute(const QStringList &params)
{
    if (params.isEmpty()) {
        emit finished("Usage: historical <symbol|contractId> [endDateTime] [durationStr] [barSizeSetting] [whatToShow] [useRTH] [formatDate]");
        return;
    }
    if (_waiting) {
        emit finished("Already waiting for historical data. Please wait.");
        return;
    }
    _waiting = true;
    _symbol = params[0];
    QString endDateTime = params.size() > 1 ? params[1] : QString();
    QString durationStr = params.size() > 2 ? params[2] : QString("1 M");
    QString barSizeSetting = params.size() > 3 ? params[3] : QString("1 day");
    QString whatToShow = params.size() > 4 ? params[4] : QString("TRADES");
    int useRTH = params.size() > 5 ? params[5].toInt() : 1;
    int formatDate = params.size() > 6 ? params[6].toInt() : 2;

    // Check if params[0] is a numeric contractId or a symbol
    bool ok = false;
    long contractId = params[0].toLong(&ok);
    if (ok) {
        _client->requestHistoricalData(contractId, endDateTime, durationStr, barSizeSetting,
                                       whatToShow, useRTH, formatDate, &_reqId);
    } else {
        _client->requestHistoricalData(params[0], endDateTime, durationStr, barSizeSetting,
                                       whatToShow, useRTH, formatDate, &_reqId);
    }
    emit finished("Requested historical data (reqId: " + QString::number(_reqId) + ")");
}

void HistoricalDataCliCommand::onHistoricalDataReady(const int &requestId, const QList<common::BarDTO> &bars,
                                                     const QString &description)
{
    if (!_waiting || requestId != _reqId)
        return;
    _waiting = false;

    QString s;
    if (bars.isEmpty()) {
        s = "No historical data";
    } else {
        if (!description.isEmpty()) {
            s += "Company: " + description + "\n\n";
        }
        s += "Symbol: " + _symbol + "\n\n";
        s += "Time\tOpen\tHigh\tLow\tClose\tVolume\tWAP\tCount\tAverage\n";
        s += "------------------------------------------------------------\n";
        for (auto const &b : bars) {
            s += b.time + "\t" + QString::number(b.open) + "\t" + QString::number(b.high) + "\t"
                 + QString::number(b.low) + "\t" + QString::number(b.close) + "\t"
                 + QString::number(b.volume) + "\t" + QString::number(b.wap) + "\t"
                 + QString::number(b.barCount) + "\t" + QString::number(b.average) + "\n";
        }
    }
    emit finished(s);
}

CancelHistoricalDataCliCommand::CancelHistoricalDataCliCommand(QSharedPointer<common::ITwsClient> client, QObject *parent)
    : CliCommand{parent}
    , _client(client)
{}

void CancelHistoricalDataCliCommand::execute(const QStringList &params)
{
    if (params.isEmpty()) {
        emit finished("Usage: cancel-historical <reqId>");
        return;
    }
    bool ok = false;
    int reqId = params[0].toInt(&ok);
    if (!ok) {
        emit finished("Invalid request ID: " + params[0]);
        return;
    }
    _client->cancelHistoricalData(reqId);
    emit finished("Cancelled historical data request " + QString::number(reqId));
}

OrderListCliCommand::OrderListCliCommand(QSharedPointer<common::ITwsClient> client, QObject *parent)
    : CliCommand{parent}
    , _client(client)
{
    QObject::connect(_client.data(), &common::ITwsClient::openOrderSignal,
                     this, &OrderListCliCommand::onOpenOrdersReady);
}

void OrderListCliCommand::execute(const QStringList &params)
{
    Q_UNUSED(params);
    if (_waiting) {
        emit finished("Already waiting for orders. Please wait.");
        return;
    }
    _waiting = true;
    _client->requestOpenOrders();
}

void OrderListCliCommand::onOpenOrdersReady(const int &requestId, const QList<common::OrderDTO> &orders)
{
    if (!_waiting)
        return;
    _waiting = false;

    QString s;
    if (orders.isEmpty()) {
        s = "No open orders";
    } else {
        s += "OrderId\tContractId\tSymbol\tAction\tQty\tType\tLmtPrice\tAuxPrice\tTIF\tStatus\tFilled\tRemaining\tAvgFillPrice\tAccount\n";
        s += "----------------------------------------------------------------------------------------------------\n";
        for (auto const &o : orders) {
            s += QString::number(o.orderId) + "\t" + QString::number(o.contractId) + "\t" + o.symbol + "\t" + o.action + "\t" + QString::number(o.totalQuantity) + "\t" + o.orderType + "\t" + QString::number(o.lmtPrice) + "\t" + QString::number(o.auxPrice) + "\t" + o.tif + "\t" + o.status + "\t" + QString::number(o.filled) + "\t" + QString::number(o.remaining) + "\t" + QString::number(o.avgFillPrice) + "\t" + o.account + "\n";
        }
    }
    emit finished(s);
}
