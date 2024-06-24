#include "ibclient.h"
#include "EClientSocket.h"

#include <QDebug>
#include <QThread>

IBClient::IBClient(QObject *parent)
    : QObject{parent},
      readerSignal(200),
      client(new EClientSocket(this, &readerSignal)),
      nextOrderId(0),
      reader(nullptr)
{

}


IBClient::~IBClient()
{
    // destroy the reader before the client
    if(reader) {
        delete reader;
    }

    disconnect();
    delete client;
}


bool IBClient::connect(const QString &host, int port, int clientId)
{
    // trying to connect
    qDebug().nospace() <<  "Connecting to " << host << ":" << port << " with clientId=" << clientId;

    connected = client->eConnect( host.toStdString().c_str(), port, clientId);

    if (connected) {
        qDebug().nospace() << "Connected to " << client->host().c_str() << ":" << client->port();
        reader = new EReader(client, &readerSignal);
        reader->start();
        emit connectedSignal();
    } else {
        qDebug().nospace() << "Failed connecting to " << host << ":" << port;
    }

    return connected;
}

void IBClient::disconnect()
{
    if(isConnected()) {
        qDebug() << "Trying to disconnect client" << client->clientId();
        client->eDisconnect();
        emit disconnectedSignal();
        qDebug().nospace() << "Client disconnected";
    } else {
        qDebug() << "Already disconnected";
    }
}

bool IBClient::isConnected()
{
    bool res = client->isConnected();
    if(connected && !res) {
        connected = false;
        emit disconnectedSignal();
    }
    return res;
}

void IBClient::setConnectOptions(const QString& connectOptions)
{
    client->setConnectOptions(connectOptions.toStdString());
}

void IBClient::nextValidId( OrderId orderId)
{
    qDebug().nospace() << "Next Valid Id:" << orderId;
    nextOrderId = orderId;
    emit nextValidIdSignal(orderId);
}

void IBClient::requestCurrentTime() {
    qDebug() << "requestCurrentTime";
    client->reqCurrentTime();
}

void IBClient::currentTime(long time)
{
    auto dateTime = QDateTime::fromSecsSinceEpoch(time, Qt::LocalTime);
    qDebug() << "Current time is" << dateTime;
    emit currentTimeSignal(dateTime);
}

void IBClient::managedAccounts( const std::string& accountsList)
{
    qDebug() << "managedAccounts" << accountsList.c_str();
    //TODO no idea what the separator is, assuming comma for now
    QStringList accounts = QString::fromStdString(accountsList).split(",");
    emit managedAccountsSignal(accounts);
}

void IBClient::error(int id, int errorCode, const std::string &errorString, const std::string &advancedOrderRejectJson)
{
    qDebug() << "error" << id << errorCode << errorString.c_str() << advancedOrderRejectJson.c_str();
//    emit errorSignal();
}

void IBClient::checkMessages()
{
    if(reader && isConnected()) {
        readerSignal.waitForSignal();
        reader->processMsgs();
    }
}
