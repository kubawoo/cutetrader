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
    _commands["account"] = new AccountSummaryCliCommand(_account, _accountId, this);
    _commands["help"] = new HelpCommand(this);
    _commands["quit"] = new QuitCommand(this);
    _commands["time"] = new ServerTimeCliCommand(_client, this);

    for (auto c : _commands.values()) {
        connect(c, &CliCommand::finished, this, &CliCommandManager::commandResult);
    }
    connect(_commands["quit"], &QuitCommand::finished, this, &CliCommandManager::quitCommand);

    connect(client.data(),
            &common::ITwsClient::accountValueUpdatedSignal,
            &_account,
            &account::Account::updateAccountValue);
    connect(client.data(),
            &common::ITwsClient::portfolioPositionUpdatedSignal,
            &_account,
            &account::Account::updatePortfolioPosition);
    connect(client.data(), &common::ITwsClient::managedAccountsSignal, this, &CliCommandManager::managedAccounts);

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

void CliCommandManager::command(const QString &cmd)
{
    // TODO split cmd by white space
    if (_commands.contains(cmd)) {
        _commands[cmd]->execute({}); // parse params
    } else {
        emit commandDone("Unrecognized command '" + cmd + "'. Type 'help' for available commands.");
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

void CliCommandManager::managedAccounts(const QStringList &accounts)
{
    qDebug() << "accountId: " << _accountId;
    qDebug() << "accounts: " << accounts;
    if (_accountId.isEmpty() && accounts.size() == 1) {
        _accountId = accounts[0];
    }

    if (!accounts.contains(_accountId)) {
        qDebug() << "Account" << _accountId << " not found in " << accounts;
        quitCommand();
        return;
    }
    _client->startClient(_accountId);
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
    emit finished(time.toString());
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
        s += cmd + "\t" + _commandManager->commands()[cmd]->description() + "\n";
    }
    emit finished(s);
}

QuitCommand::QuitCommand(CliCommandManager *mgr)
    : CliCommand(mgr)
    , _commandManager(mgr)
{}

void QuitCommand::execute(const QStringList &params)
{
    emit finished("Quiting...");
}

AccountSummaryCliCommand::AccountSummaryCliCommand(account::Account &account, const QString &accountId, QObject *parent)
    : CliCommand(parent)
    , _account(account)
    , _accountId(accountId)
{}

void AccountSummaryCliCommand::execute(const QStringList &params)
{
    QString s = "Account:\t " + _accountId + "\n";
    s += "Base Currency:\t " + _account.baseCurrency() + "\n";
    s += "Net Liq.:\t " + QString::number(_account.accountInfo(account::AccountInfoType::NetLiquidation)) + "\n";
    //TODO add missing values

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
            s += " " + option.symbol() + " " + QString::number(option.position()) + " "
                 + QString::number(option.marketPrice()) + " " + QString::number(option.marketValue()) + "\n";
        }
        s += "\n";
    }

    s += "Cash:\t " + QString::number(_account.accountInfo(account::AccountInfoType::CashBalance));
    emit finished(s);
}
