#include "twsqapi/twsclient.h"

#include "twsqapi/constants.h"

namespace twsqapi {

TwsClient::TwsClient(QObject *parent)
    : common::ITwsClient{parent},
      _client(new SocketClient(this)),
      _nextOrderId(-1),
      _requestId(0)
{
    QObject::connect(_client, &SocketClient::message, this, &TwsClient::onMessage);
    QObject::connect(_client, &SocketClient::error, this, &TwsClient::onError);
}

TwsClient::~TwsClient()
{

}

bool TwsClient::connect(const QString &host, int port, int clientId)
{
    return _client->connect(host, port, clientId);
}

bool TwsClient::isConnected()
{
    return _client->isConnected();
}

void TwsClient::disconnect()
{
    _client->disconnect();
}

void TwsClient::startClient(const QString &accountId)
{
    _accountId = accountId;
    _client->send(StartAccountUpdatesClientMessage(_accountId));
}

void TwsClient::requestCurrentTime()
{
    _client->send(RequestCurrentTimeClientMessage());
}

void TwsClient::requestManagedAccounts()
{

}

void TwsClient::requestOpenOrders()
{

}

void TwsClient::requestContractDetails(long contractId, int *reqId)
{
    QStringList keys = {"requestContractDetails", QString::number(contractId)};
    auto cacheEntry = _cache.get(keys);
    if(cacheEntry.first >= 0 && cacheEntry.second && cacheEntry.second->ready()) {
        long requestId = cacheEntry.first;
        ContractDetailsCacheEntry * entry =  dynamic_cast<ContractDetailsCacheEntry*>(cacheEntry.second);
        qDebug() << "Returning from cache";
        _setRequestId(reqId, requestId);
        emit contractDetailReadySignal(requestId, entry->contractDetails);
    } else {
        qDebug() << "requestContractDetails" << _requestId;
        _setRequestId(reqId, _requestId);
        _cache.add(_requestId, new ContractDetailsCacheEntry(), keys);
        _client->send(RequestContractDetailsClientMessage(_requestId++, contractId));
    }
}

void TwsClient::requestMatchingSymbols(const QString &pattern, int *reqId)
{
    QStringList keys = {"requestMatchingSymbols", pattern};
    auto cacheEntry = _cache.get(keys);

    if(cacheEntry.first >= 0 && cacheEntry.second && cacheEntry.second->ready()) {
        long requestId = cacheEntry.first;
        ContractDetailsCacheEntry * entry =  dynamic_cast<ContractDetailsCacheEntry*>(cacheEntry.second);
        qDebug() << "Returning from cache" << keys;
        _setRequestId(reqId, requestId);
        emit matchingSymbolsReadySignal(requestId, entry->contractDetails);
    } else {
        _setRequestId(reqId, _requestId);
        _cache.add(_requestId, new ContractDetailsCacheEntry(), keys);
        _client->send(RequestMatchingSymbolsClientMessage(_requestId++, pattern));
    }
}

void TwsClient::onMessage(QSharedPointer<ServerMessage> msg)
{
    qDebug() << "TwsClient::onMessage" << msg->id();
    switch(msg->id()) {
    case responses::MANAGED_ACCOUNTS: {
        _handleManagedAccounts(msg.dynamicCast<ManagedAccountsServerMessage>().get());
        break;
    }
    case responses::NEXT_VALID_ID: {
        _handleNextValidId(msg.dynamicCast<NextValidIdServerMessage>().get());
        break;
    }
    case responses::ERR_MSG: {
        _handleErrorMsg(msg.dynamicCast<ErrorServerMessage>().get());
        break;
    }
    case responses::ACCT_VALUE: {
        _handleAccountValue(msg.dynamicCast<AccountValueServerMessage>().get());
        break;
    }
    case responses::ACCT_UPDATE_TIME: {
        _handleAccountUpdateTime(msg.dynamicCast<AccountUpdateTimeServerMessage>().get());
        break;
    }
    case responses::ACCT_DOWNLOAD_END: {
        break;
    }
    case responses::CURRENT_TIME: {
        _handleCurrentTime(msg.dynamicCast<CurrentTimeServerMessage>().get());
        break;
    }
    case responses::SYMBOL_SAMPLES: {
        _handleSymbolSamples(msg.dynamicCast<SymbolSamplesSrverMessage>().get());
        break;
    }
    case responses::CONTRACT_DATA: {
        _handleContractData(msg.dynamicCast<ContractDataServerMessage>().get());
        break;
    }
    case responses::CONTRACT_DATA_END: {
        _handleContractDataEnd(msg.dynamicCast<ContractDataEndServerMessage>().get());
        break;
    }
    case responses::PORTFOLIO_VALUE: {
        _handlePortfolioValue(msg.dynamicCast<PortfolioValueServerMessage>().get());
        break;
    }
    default:
        qDebug() << "TwsClient::onMessage unsupported msg id" << msg->id();
    }
}

void TwsClient::_handleManagedAccounts(ManagedAccountsServerMessage *msg)
{
    emit managedAccountsSignal(msg->accounts());
}

void TwsClient::_handleNextValidId(NextValidIdServerMessage *msg)
{
    _nextOrderId = msg->nextId();
}

void TwsClient::_handleErrorMsg(ErrorServerMessage *msg)
{
    //TODO
    qDebug() << "TwsClient::_handleErrorMsg" << msg->errorId() << msg->errorCode() << msg->errorMsg()
             << msg->errorDetails() << msg->errorTime();
}

void TwsClient::_handleAccountValue(AccountValueServerMessage *msg)
{
    emit accountValueUpdatedSignal(msg->key(), msg->value(), msg->currency());
}

void TwsClient::_handleAccountUpdateTime(AccountUpdateTimeServerMessage *msg)
{
    emit updateAccountTimeSignal(msg->time());
}

void TwsClient::_handleCurrentTime(CurrentTimeServerMessage *msg)
{
    qDebug() << "Got server current time:" << msg->dateTime();
    emit currentTimeSignal(msg->dateTime());
}

void TwsClient::_handleSymbolSamples(SymbolSamplesSrverMessage *msg) {
    qDebug() << "symbolSamples";
    int reqId = msg->requestId();
    ContractDetailsCacheEntry * entry =  dynamic_cast<ContractDetailsCacheEntry*>(_cache.get(reqId));
    if(entry) {
        entry->contractDetails.append(msg->contracts());
        entry->setReady();
        emit matchingSymbolsReadySignal(reqId, entry->contractDetails);
    }
}

void TwsClient::_handleContractData(ContractDataServerMessage *msg)
{
    ContractDetailsCacheEntry * entry =  dynamic_cast<ContractDetailsCacheEntry*>(_cache.get(msg->requestId()));
    if(entry) {
        entry->contractDetails.append(msg->contract());
    }
}

void TwsClient::_handleContractDataEnd(ContractDataEndServerMessage *msg)
{
    ContractDetailsCacheEntry * entry =  dynamic_cast<ContractDetailsCacheEntry*>(_cache.get(msg->requestId()));
    if(entry) {
        entry->setReady();
        emit contractDetailReadySignal(msg->requestId(), entry->contractDetails);
    }
}

void TwsClient::_handlePortfolioValue(PortfolioValueServerMessage *msg)
{
    emit portfolioPositionUpdatedSignal(msg->position());
}

void TwsClient::onError(const QString &reason, bool fatal)
{

}

}
