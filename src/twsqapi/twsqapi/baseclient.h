#ifndef TWSQAPI_BASECLIENT_H
#define TWSQAPI_BASECLIENT_H

#include <QObject>
#include <QIODevice>
#include <QMutex>

#include "clientmessage.h"
#include "servermessage.h"


namespace twsqapi {

class BaseClient : public QObject
{
    Q_OBJECT
public:
    explicit BaseClient(QObject *parent = nullptr);
    ~BaseClient();

    void send(const ClientMessage & msg);
    void send(ClientMessage && msg);

signals:
    void message(QSharedPointer<ServerMessage> msg);
    void error(const QString & reason, bool fatal = false);
    void _dataRead();


protected slots:
    void _readData();
    void _processBuffer();

protected:
    void _init(QIODevice * input, QIODevice * output, int clientId);
    void _connectHandshake(const QString &opts);
    bool _isHandshakeDone();
    int _getApiVersion();

private:
    void _send(const QByteArray & msg);
    void _processData(const QByteArray & data);
    void _handleHandshake(const QStringList & fields);


private:
    QIODevice * _input;
    QIODevice * _output;
    int _clientId;
    bool _handshakeDone;
    QByteArray _buffer;
    QMutex _readMutex;
    QMutex _writeMutex;
    int _apiVersion;
    ServerMessageFactory serverMessageFactory;
};

}
#endif
