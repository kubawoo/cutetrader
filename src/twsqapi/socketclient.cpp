#include "twsqapi/socketclient.h"
#include "twsqapi/constants.h"

#include <QDebug>
#include <QMutexLocker>



namespace twsqapi {

SocketClient::SocketClient(QObject *parent)
    : BaseClient(parent)
{
}

SocketClient::~SocketClient()
{
    qDebug() << "SocketClient::~SocketClient()";
}

bool SocketClient::connect(const QString &host, int port, int clientId)
{
    _init(&_socket, &_socket, clientId);
    _socket.connectToHost(host, port);

    if (!_socket.waitForConnected() || !_socket.isValid() || _socket.state() != QAbstractSocket::ConnectedState) {
        qCritical() << "Failed to connect to host" << _socket.errorString();
        emit error("Failed to connect", true);
        return false;
    } else {
        qDebug() << "Connected to host";
        _connectHandshake(client::PACEAPI_OPT);
    }
    return true;
}

void SocketClient::disconnect()
{
    _socket.disconnectFromHost();
}

bool SocketClient::isConnected()
{
    return _socket.isOpen();
}



}
