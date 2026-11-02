#ifndef CLICOMMAND_H
#define CLICOMMAND_H

#include <QObject>
#include <account.h>
#include <common.h>
#include <data.h>

class CliCommand : public QObject
{
    Q_OBJECT
public:
    explicit CliCommand(QObject *parent = nullptr);
    virtual QString description() = 0;
    QStringList parametersDescription() { return {}; }
public slots:
    virtual void execute(const QStringList &params) = 0;
signals:
    void finished(const QString &txt = "");
};

class CliCommandManager : public QObject
{
    Q_OBJECT
public:
    explicit CliCommandManager(const QString &host,
                               int port,
                               int clientId,
                               const QString &accountId,
                               QSharedPointer<common::ITwsClient> client,
                               QObject *parent = nullptr);
    const QMap<QString, CliCommand *> &commands() { return _commands; }

public slots:
    void command(const QString &cmd);
    void commandResult(const QString &cmd);
    void quitCommand();

private slots:
    void start();
    void onManagedAccounts(const QStringList &accounts);
    void onError(const QString &reason, bool fatal);

private:
    void setupCommands();

signals:
    void quit();
    void commandDone(const QString &result);

private:
    QString _host;
    int _port;
    int _clientId;
    QString _accountId;
    QSharedPointer<common::ITwsClient> _client;
    QMap<QString, CliCommand *> _commands;
    account::Account _account;
    data::DataManager _dataManager;
};

class QuitCommand : public CliCommand
{
    Q_OBJECT
public:
    explicit QuitCommand(CliCommandManager *mgr);
    QString description() override { return "Closes the application"; }
public slots:
    void execute(const QStringList &params) override;

private:
    CliCommandManager *_commandManager;
};

class HelpCommand : public CliCommand
{
    Q_OBJECT
public:
    explicit HelpCommand(CliCommandManager *mgr);
    QString description() override { return "Displays command information"; }
public slots:
    void execute(const QStringList &params) override;

private:
    CliCommandManager *_commandManager;
};

class ServerTimeCliCommand : public CliCommand
{
    Q_OBJECT
public:
    explicit ServerTimeCliCommand(QSharedPointer<common::ITwsClient> client, QObject *parent = nullptr);
    QString description() override { return "Returns current server time"; }
public slots:
    void execute(const QStringList &params) override;
private slots:
    void serverTime(const QDateTime &time);

private:
    QSharedPointer<common::ITwsClient> _client;
};

class AccountSummaryCliCommand : public CliCommand
{
    Q_OBJECT
public:
    explicit AccountSummaryCliCommand(account::Account &account, const QString &accountId, QObject *parent = nullptr);
    QString description() override { return "Provides an overview of the account and portfolio"; }
public slots:
    void execute(const QStringList &params) override;

private:
    account::Account &_account;
    const QString &_accountId;
};

class WatchlistCliCommand : public CliCommand
{
    Q_OBJECT
public:
    explicit WatchlistCliCommand(data::DataManager &dataManager, QObject *parent = nullptr);
    QString description() override { return "Displays watchlist"; }
public slots:
    void execute(const QStringList &params) override;

private:
    data::DataManager &_dataManager;
};

class SearchSymbolsCliCommand : public CliCommand
{
    Q_OBJECT
public:
    explicit SearchSymbolsCliCommand(QSharedPointer<common::ITwsClient> client, QObject *parent = nullptr);
    QString description() override { return "Search for symbols matching a pattern"; }
    QStringList parametersDescription() { return {"<pattern>"}; }
public slots:
    void execute(const QStringList &params) override;
private slots:
    void onMatchingSymbolsReady(const int &requestId, const QList<common::ContractDetailsDTO> &details);

private:
    QSharedPointer<common::ITwsClient> _client;
    int _reqId = 0;
    bool _waiting = false;
};

class HistoricalDataCliCommand : public CliCommand
{
    Q_OBJECT
public:
    explicit HistoricalDataCliCommand(QSharedPointer<common::ITwsClient> client, QObject *parent = nullptr);
    QString description() override { return "Request historical data for a contract"; }
    QStringList parametersDescription() { return {"<contractId> [endDateTime] [durationStr] [barSizeSetting] [whatToShow] [useRTH] [formatDate]"}; }
public slots:
    void execute(const QStringList &params) override;
private slots:
    void onHistoricalDataReady(const int &requestId, const QList<common::BarDTO> &bars);

private:
    QSharedPointer<common::ITwsClient> _client;
    int _reqId = 0;
    bool _waiting = false;
};

#endif // CLICOMMAND_H
