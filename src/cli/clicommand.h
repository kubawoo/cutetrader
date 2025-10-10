#ifndef CLICOMMAND_H
#define CLICOMMAND_H

#include <QObject>
#include <common.h>

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
    void finished(const QString &txt);
};

class CliCommandManager : public QObject
{
    Q_OBJECT
public:
    explicit CliCommandManager(common::ITwsClient *client, QObject *parent = nullptr);

public slots:
    void command(const QString &cmd);
    void commandResult(const QString &cmd);

signals:
    void commandDone(const QString &result);

private:
    common::ITwsClient *_client;
    QMap<QString, CliCommand *> _commands;
};

class ServerTimeCliCommand : public CliCommand
{
    Q_OBJECT
public:
    explicit ServerTimeCliCommand(common::ITwsClient *client, QObject *parent = nullptr);
    QString description() override { return "Returns current server time"; }
public slots:
    void execute(const QStringList &params) override;
private slots:
    void serverTime(const QDateTime &time);

private:
    common::ITwsClient *_client;
};

#endif // CLICOMMAND_H
