#include "twsclient.h"
#include "EClientSocket.h"

#include <QDebug>
#include <QThread>

TwsClient::TwsClient(QObject *parent)
    : QObject{parent},
      readerSignal(200),
      client(new EClientSocket(this, &readerSignal)),
      nextOrderId(0),
      reader(nullptr)
{

}


TwsClient::~TwsClient()
{
    // destroy the reader before the client
    if(reader) {
        delete reader;
    }

    disconnect();
    delete client;
}


bool TwsClient::connect(const QString &host, int port, int clientId)
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

void TwsClient::disconnect()
{
    if(isConnected()) {
        qDebug() << "Trying to disconnect client" << client->clientId();
        client->eDisconnect();
        emit disconnectedSignal();
        qDebug().nospace() << "Client disconnected";
        stopAccountUpdates(); //TODO: remove
        stopPositionsUpdates();
    } else {
        qDebug() << "Already disconnected";
    }
}

bool TwsClient::isConnected()
{
    bool res = client->isConnected();
    if(connected && !res) {
        connected = false;
        emit disconnectedSignal();
    }
    return res;
}

void TwsClient::setConnectOptions(const QString& connectOptions)
{
    client->setConnectOptions(connectOptions.toStdString());
}

void TwsClient::nextValidId( OrderId orderId)
{
    qDebug().nospace() << "Next Valid Id:" << orderId;
    nextOrderId = orderId;
    emit nextValidIdSignal(orderId);
     startAccountUpdates(); //TODO: remove
     startPositionsUpdates();
}

void TwsClient::requestCurrentTime() {
    qDebug() << "requestCurrentTime";
    client->reqCurrentTime();
}

void TwsClient::startAccountUpdates() {
    client->reqAccountUpdates(true, account.toStdString());
}
void TwsClient::stopAccountUpdates() {
    client->reqAccountUpdates(false, account.toStdString());
}
void TwsClient::startPositionsUpdates()
{
    client->reqPositions();
}
void TwsClient::stopPositionsUpdates()
{
    client->cancelPositions();
}

void TwsClient::requestManagedAccounts()
{

}

void TwsClient::currentTime(long time)
{
    auto dateTime = QDateTime::fromSecsSinceEpoch(time, Qt::LocalTime);
    qDebug() << "Current time is" << dateTime;
    emit currentTimeSignal(dateTime);
}

void TwsClient::managedAccounts( const std::string& accountsList)
{
    qDebug() << "managedAccounts" << accountsList.c_str();
    QStringList accounts = QString::fromStdString(accountsList).split(",");
    this->account = accounts[0];
    emit managedAccountsSignal(accounts);
}

void TwsClient::error(int id, int errorCode, const std::string &errorString, const std::string &advancedOrderRejectJson)
{
    qDebug() << "error" << id << errorCode << errorString.c_str() << advancedOrderRejectJson.c_str();
//    emit errorSignal();
}

void TwsClient::updateAccountValue(const std::string& key, const std::string& val,
    const std::string& currency, const std::string& accountName)
{
    qDebug() << "updateAccountValue" << key.c_str() << val.c_str() << currency.c_str() << accountName.c_str();
}

void TwsClient::updatePortfolio( const Contract& contract, Decimal position,
    double marketPrice, double marketValue, double averageCost,
    double unrealizedPNL, double realizedPNL, const std::string& accountName)
{
    qDebug() << "updatePortfolio" << contract.symbol.c_str() << DecimalFunctions::decimalToDouble(position)
             << marketPrice << marketValue << averageCost
             <<unrealizedPNL << realizedPNL << accountName.c_str();
}


void TwsClient::updateAccountTime(const std::string& timeStamp)
{
    qDebug() << "updateAccountTime" << timeStamp.c_str();
}

void TwsClient::accountDownloadEnd(const std::string& accountName)
{
    qDebug() << "accountDownloadEnd" << accountName.c_str();
}

void TwsClient::position(const std::string &account, const Contract &contract, Decimal position, double avgCost)
{
    qDebug() << "position" << account.c_str() << contract.symbol.c_str() << DecimalFunctions::decimalToDouble(position)
             << avgCost;
}

void TwsClient::positionEnd()
{
    qDebug() << "positionEnd";
}


void TwsClient::checkMessages()
{
    if(reader && isConnected()) {
        readerSignal.waitForSignal();
        reader->processMsgs();
    }
}
