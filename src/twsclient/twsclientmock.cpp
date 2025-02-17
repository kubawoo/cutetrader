#include "twsclient/twsclientmock.h"

namespace twsclient {

TwsClientMock::TwsClientMock(QObject *parent)
    : ITwsClient(parent)
{

}

TwsClientMock::~TwsClientMock()
{

}


void TwsClientMock::requestCurrentTime()
{
    emit currentTimeSignal(QDateTime::currentDateTime());
}


void TwsClientMock::requestManagedAccounts()
{

}

void TwsClientMock::startClient(const QString &accountId)
{

}

void TwsClientMock::requestOpenOrders()
{

}

bool TwsClientMock::connect(const QString &host, int port, int clientId)
{
    if(!_connected) {
        _connected = true;
        emit connectedSignal();
        QStringList accounts = {"Mocked#1", "Mocked#2"};
        emit managedAccountsSignal(accounts);
        return true;
    }
    return false;
}

void TwsClientMock::disconnect()
{
    _connected = false;
}

bool TwsClientMock::isConnected()
{
    return _connected;
}


void TwsClientMock::requestContractDetails(long contractId, int * reqId)
{
    _setRequestId(reqId, _requestId++);
}

void TwsClientMock::requestMatchingSymbols(const QString &pattern, int * reqId)
{
    _setRequestId(reqId, _requestId++);
}



}
