#include "clicommand.h"

CliCommandManager::CliCommandManager(common::ITwsClient *client, QObject *parent)
    : QObject{parent}
    , _client(client)
{
    _commands["help"] = new HelpCommand(this);
    _commands["quit"] = new QuitCommand(this);
    _commands["time"] = new ServerTimeCliCommand(client, this);

    for (auto c : _commands.values()) {
        connect(c, &CliCommand::finished, this, &CliCommandManager::commandResult);
    }
    connect(_commands["quit"], &QuitCommand::finished, this, &CliCommandManager::quitCommand);
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

ServerTimeCliCommand::ServerTimeCliCommand(common::ITwsClient *client, QObject *parent)
    : CliCommand{parent}
    , _client(client)
{
    QObject::connect(_client, &common::ITwsClient::currentTimeSignal, this, &ServerTimeCliCommand::serverTime);
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
