#ifndef TWSQAPI_APICLIENT_H
#define TWSQAPI_APICLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QMutex>

#include "baseclient.h"
#include <common/itwsclient.h>


namespace twsqapi {

class SocketClient : public BaseClient
{
    Q_OBJECT
public:
    explicit SocketClient(QObject *parent = nullptr);
    ~SocketClient();

    bool connect(const QString &host, int port, int clientId);
    void disconnect();
    bool isConnected();


private:
    QTcpSocket _socket;

};

}
#endif
