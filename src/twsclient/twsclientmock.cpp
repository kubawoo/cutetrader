#include "twsclient/twsclientmock.h"

namespace twsclient {

TwsClientMock::TwsClientMock(QObject *parent)
    : ITwsClient(parent)
{

}

TwsClientMock::~TwsClientMock()
{

}

void TwsClientMock::checkMessages()
{

}

void TwsClientMock::cleanup()
{

}

void TwsClientMock::requestCurrentTime()
{
    emit currentTimeSignal(QDateTime::currentDateTime());
}

void TwsClientMock::startAccountUpdates()
{
    //TODO:
}

void TwsClientMock::stopAccountUpdates()
{

}

void TwsClientMock::startPositionsUpdates()
{

}

void TwsClientMock::stopPositionsUpdates()
{

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
    return true;
}

void TwsClientMock::disconnect()
{

}

bool TwsClientMock::isConnected()
{
    return true;
}

void TwsClientMock::clearCache()
{

}

int TwsClientMock::requestContractDetails(long contractId)
{
    return 1;
}

int TwsClientMock::requestMatchingSymbols(const QString &pattern)
{
    return 1;
}



}
