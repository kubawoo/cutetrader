#include "clicommand.h"

CliCommandManager::CliCommandManager(common::ITwsClient *client, QObject *parent)
    : QObject{parent}
    , _client(client)
{
    auto timeCommand = new ServerTimeCliCommand(client, this);
    connect(timeCommand, &CliCommand::finished, this, &CliCommandManager::commandResult);
    _commands["time"] = timeCommand;
}

void CliCommandManager::command(const QString &cmd)
{
    // TODO split by white space
    if (_commands.contains(cmd)) {
        _commands[cmd]->execute({}); // parse params
    } else {
        emit commandDone("Need help for >> " + cmd + " << ?");
    }
}

void CliCommandManager::commandResult(const QString &cmd)
{
    emit commandDone(cmd);
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
