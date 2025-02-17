#include "twsclient/twsclientmock.h"

namespace twsclient {

TwsClientMock::TwsClientMock(QObject *parent)
    : ITwsClient(parent),
      _timer{new QTimer}
{
    QObject::connect(_timer, &QTimer::timeout, this, &TwsClientMock::_timerTask);
}

TwsClientMock::~TwsClientMock()
{
    _timer->stop();
    delete _timer;
}


void TwsClientMock::requestCurrentTime()
{
    emit currentTimeSignal(QDateTime::currentDateTime());
}


void TwsClientMock::requestManagedAccounts()
{
    emit managedAccountsSignal({"Mocked#1", "Mocked#2"});
}

void TwsClientMock::startClient(const QString &accountId)
{
    if(_connected) {
        _timer->start(60 * 1000); // every minute
        _timerTask();
    }
}

void TwsClientMock::requestOpenOrders()
{

}

bool TwsClientMock::connect(const QString &host, int port, int clientId)
{
    if(!_connected) {
        _connected = true;
        emit connectedSignal();
        requestManagedAccounts();
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

void TwsClientMock::_timerTask()
{
    if(_connected) {
        emit updateAccountTimeSignal(QTime::currentTime());
    }
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
