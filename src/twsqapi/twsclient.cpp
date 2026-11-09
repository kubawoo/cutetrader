#include "twsqapi/twsclient.h"

#include "twsqapi/constants.h"
#include "common/utils.h"
#include <memory>

namespace twsqapi {

TwsClient::TwsClient(QObject *parent)
    : common::ITwsClient{parent},
      _client(new SocketClient(this)),
      _nextOrderId(-1),
      _requestId(0)
{
    QObject::connect(_client, &SocketClient::message, this, &TwsClient::onMessage);
    QObject::connect(_client, &SocketClient::error, this, &TwsClient::onError);
    QObject::connect(_client, &SocketClient::handshakeCompleted, this, &TwsClient::onHandshakeCompleted);
    QObject::connect(_client, &SocketClient::disconnected, this, &TwsClient::onDisconnected);
}

TwsClient::~TwsClient()
{
    qDebug() << "TwsClient::~TwsClient()";
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
    requestOpenOrders();
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
    _pendingOpenOrders.clear();
    _openOrdersReqId = _requestId++;
    _client->send(RequestOpenOrdersClientMessage(_openOrdersReqId));
}

void TwsClient::requestContractDetails(long contractId, int *reqId)
{
    QStringList keys = {"requestContractDetails", QString::number(contractId)};
    auto cacheEntry = _cache.get(keys);
    if(cacheEntry.first >= 0 && cacheEntry.second && cacheEntry.second->ready()) {
        long requestId = cacheEntry.first;
        auto entry = std::dynamic_pointer_cast<ContractDetailsCacheEntry>(cacheEntry.second);
        qDebug() << "Returning from cache";
        _setRequestId(reqId, requestId);
        emit contractDetailReadySignal(requestId, entry->contractDetails);
    } else {
        qDebug() << "requestContractDetails" << _requestId;
        _setRequestId(reqId, _requestId);
        _cache.add(_requestId, std::make_shared<ContractDetailsCacheEntry>(), keys);
        _client->send(RequestContractDetailsClientMessage(_requestId++, contractId));
    }
}

void TwsClient::requestMatchingSymbols(const QString &pattern, int *reqId)
{
    QStringList keys = {"requestMatchingSymbols", pattern};
    auto cacheEntry = _cache.get(keys);

    if(cacheEntry.first >= 0 && cacheEntry.second && cacheEntry.second->ready()) {
        long requestId = cacheEntry.first;
        auto entry = std::dynamic_pointer_cast<ContractDetailsCacheEntry>(cacheEntry.second);
        qDebug() << "Returning from cache" << keys;
        _setRequestId(reqId, requestId);
        emit matchingSymbolsReadySignal(requestId, entry->contractDetails);
    } else {
        _setRequestId(reqId, _requestId);
        _cache.add(_requestId, std::make_shared<ContractDetailsCacheEntry>(), keys);
        _client->send(RequestMatchingSymbolsClientMessage(_requestId++, pattern));
    }
}

void TwsClient::requestHistoricalData(long contractId, const QString & endDateTime,
                                      const QString & durationStr, const QString & barSizeSetting,
                                      const QString & whatToShow, int useRTH, int formatDate,
                                      int *reqId)
{
    QStringList keys = {"requestHistoricalData", QString::number(contractId), endDateTime,
                        durationStr, barSizeSetting, whatToShow};
    auto cacheEntry = _cache.get(keys);

    if(cacheEntry.first >= 0 && cacheEntry.second && cacheEntry.second->ready()) {
        long requestId = cacheEntry.first;
        auto entry = std::dynamic_pointer_cast<HistoricalDataCacheEntry>(cacheEntry.second);
        qDebug() << "Returning historical data from cache" << keys;
        _setRequestId(reqId, requestId);
        emit historicalDataReadySignal(requestId, entry->bars);
        return;
    }

    // Check if we have contract details cached
    QStringList contractKeys = {"requestContractDetails", QString::number(contractId)};
    auto contractCacheEntry = _cache.get(contractKeys);

    int currentReqId = _requestId++;
    _setRequestId(reqId, currentReqId);

    if(contractCacheEntry.first >= 0 && contractCacheEntry.second && contractCacheEntry.second->ready()) {
        // We have contract details, use them to send historical data request
        auto contractEntry = std::dynamic_pointer_cast<ContractDetailsCacheEntry>(contractCacheEntry.second);
        if(contractEntry && !contractEntry->contractDetails.isEmpty()) {
            PendingHistoricalDataRequest pending;
            pending.contractId = contractId;
            pending.endDateTime = endDateTime;
            pending.durationStr = durationStr;
            pending.barSizeSetting = barSizeSetting;
            pending.whatToShow = whatToShow;
            pending.useRTH = useRTH;
            pending.formatDate = formatDate;
            pending.reqId = currentReqId;

            _sendHistoricalDataRequest(pending, contractEntry->contractDetails.first());
            _cache.add(currentReqId, std::make_shared<HistoricalDataCacheEntry>(), keys);
            return;
        }
    }

    // No contract details cached, request them first
    qDebug() << "No contract details cached, requesting contract details first for" << contractId;
    PendingHistoricalDataRequest pending;
    pending.contractId = contractId;
    pending.endDateTime = endDateTime;
    pending.durationStr = durationStr;
    pending.barSizeSetting = barSizeSetting;
    pending.whatToShow = whatToShow;
    pending.useRTH = useRTH;
    pending.formatDate = formatDate;
    pending.reqId = currentReqId;

    _pendingHistoricalDataRequests[contractId].append(pending);

    // Request contract details
    _cache.add(currentReqId, std::make_shared<ContractDetailsCacheEntry>(), contractKeys);
    _client->send(RequestContractDetailsClientMessage(currentReqId, contractId));
}

void TwsClient::requestHistoricalData(const QString &symbol, const QString & endDateTime,
                                      const QString & durationStr, const QString & barSizeSetting,
                                      const QString & whatToShow, int useRTH, int formatDate,
                                      int *reqId)
{
    int currentReqId = _requestId++;
    _setRequestId(reqId, currentReqId);

    PendingSymbolHistoricalDataRequest pending;
    pending.originalReqId = currentReqId;
    pending.symbol = symbol;
    pending.endDateTime = endDateTime;
    pending.durationStr = durationStr;
    pending.barSizeSetting = barSizeSetting;
    pending.whatToShow = whatToShow;
    pending.useRTH = useRTH;
    pending.formatDate = formatDate;

    // Check cache for matching symbols first
    QStringList keys = {"requestMatchingSymbols", symbol};
    auto cacheEntry = _cache.get(keys);

    if(cacheEntry.first >= 0 && cacheEntry.second && cacheEntry.second->ready()) {
        // Cache hit - resolve immediately
        auto entry = std::dynamic_pointer_cast<ContractDetailsCacheEntry>(cacheEntry.second);
        if(entry && !entry->contractDetails.isEmpty()) {
            const auto &contract = entry->contractDetails.first();
            _historicalDataDescriptions[currentReqId] = contract.description;

            PendingHistoricalDataRequest hdPending;
            hdPending.contractId = contract.contractId;
            hdPending.endDateTime = pending.endDateTime;
            hdPending.durationStr = pending.durationStr;
            hdPending.barSizeSetting = pending.barSizeSetting;
            hdPending.whatToShow = pending.whatToShow;
            hdPending.useRTH = pending.useRTH;
            hdPending.formatDate = pending.formatDate;
            hdPending.reqId = currentReqId;

            _sendHistoricalDataRequest(hdPending, contract);
            QStringList hdKeys = {"requestHistoricalData", QString::number(contract.contractId), pending.endDateTime,
                                  pending.durationStr, pending.barSizeSetting, pending.whatToShow};
            _cache.add(currentReqId, std::make_shared<HistoricalDataCacheEntry>(), hdKeys);
            return;
        }
    }

    // Cache miss - store pending and request matching symbols
    _pendingSymbolHistoricalDataRequests[currentReqId] = pending;

    int matchingReqId;
    requestMatchingSymbols(symbol, &matchingReqId);

    // Re-key by matchingReqId (requestMatchingSymbols may assign a different ID)
    _pendingSymbolHistoricalDataRequests[matchingReqId] = _pendingSymbolHistoricalDataRequests.take(currentReqId);
}

void TwsClient::cancelHistoricalData(int reqId)
{
    qDebug() << "Canceling historical data request" << reqId;
    _client->send(CancelHistoricalDataClientMessage(reqId));
    // Remove description if present
    _historicalDataDescriptions.remove(reqId);
}

void TwsClient::onMessage(std::shared_ptr<ServerMessage> msg)
{
    qDebug() << "TwsClient::onMessage" << msg->id();
    switch(msg->id()) {
    case responses::MANAGED_ACCOUNTS: {
        _handleManagedAccounts(std::dynamic_pointer_cast<ManagedAccountsServerMessage>(msg).get());
        break;
    }
    case responses::NEXT_VALID_ID: {
        _handleNextValidId(std::dynamic_pointer_cast<NextValidIdServerMessage>(msg).get());
        break;
    }
    case responses::ERR_MSG: {
        _handleErrorMsg(std::dynamic_pointer_cast<ErrorServerMessage>(msg).get());
        break;
    }
    case responses::ACCT_VALUE: {
        _handleAccountValue(std::dynamic_pointer_cast<AccountValueServerMessage>(msg).get());
        break;
    }
    case responses::ACCT_UPDATE_TIME: {
        _handleAccountUpdateTime(std::dynamic_pointer_cast<AccountUpdateTimeServerMessage>(msg).get());
        break;
    }
    case responses::ACCT_DOWNLOAD_END: {
        break;
    }
    case responses::CURRENT_TIME: {
        _handleCurrentTime(std::dynamic_pointer_cast<CurrentTimeServerMessage>(msg).get());
        break;
    }
    case responses::SYMBOL_SAMPLES: {
        _handleSymbolSamples(std::dynamic_pointer_cast<SymbolSamplesServerMessage>(msg).get());
        break;
    }
    case responses::CONTRACT_DATA: {
        _handleContractData(std::dynamic_pointer_cast<ContractDataServerMessage>(msg).get());
        break;
    }
    case responses::CONTRACT_DATA_END: {
        _handleContractDataEnd(std::dynamic_pointer_cast<ContractDataEndServerMessage>(msg).get());
        break;
    }
    case responses::PORTFOLIO_VALUE: {
        _handlePortfolioValue(std::dynamic_pointer_cast<PortfolioValueServerMessage>(msg).get());
        break;
    }
    case responses::HISTORICAL_DATA: {
        _handleHistoricalData(std::dynamic_pointer_cast<HistoricalDataServerMessage>(msg).get());
        break;
    }
    case responses::HISTORICAL_DATA_END: {
        _handleHistoricalDataEnd(std::dynamic_pointer_cast<HistoricalDataEndServerMessage>(msg).get());
        break;
    }
    case responses::HISTORICAL_DATA_UPDATE: {
        _handleHistoricalDataUpdate(std::dynamic_pointer_cast<HistoricalDataUpdateServerMessage>(msg).get());
        break;
    }
    case responses::ORDER_STATUS: {
        _handleOrderStatus(std::dynamic_pointer_cast<OrderStatusServerMessage>(msg).get());
        break;
    }
    case responses::OPEN_ORDER: {
        _handleOpenOrder(std::dynamic_pointer_cast<OpenOrderServerMessage>(msg).get());
        break;
    }
    case responses::OPEN_ORDER_END: {
        _handleOpenOrderEnd(std::dynamic_pointer_cast<OpenOrderEndServerMessage>(msg).get());
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

void TwsClient::_handleSymbolSamples(SymbolSamplesServerMessage *msg) {
    qDebug() << "symbolSamples";
    int reqId = msg->requestId();
    auto entry = std::dynamic_pointer_cast<ContractDetailsCacheEntry>(_cache.get(reqId));
    if(entry) {
        entry->contractDetails.append(msg->contracts());
        entry->setReady();
        emit matchingSymbolsReadySignal(reqId, entry->contractDetails);

        // Check if this was a symbol-based historical data lookup
        auto it = _pendingSymbolHistoricalDataRequests.find(reqId);
        if(it != _pendingSymbolHistoricalDataRequests.end()) {
            PendingSymbolHistoricalDataRequest pending = it.value();
            _pendingSymbolHistoricalDataRequests.erase(it);

            if(!entry->contractDetails.isEmpty()) {
                const auto &contract = entry->contractDetails.first();
                _historicalDataDescriptions[pending.originalReqId] = contract.description;

                PendingHistoricalDataRequest hdPending;
                hdPending.contractId = contract.contractId;
                hdPending.endDateTime = pending.endDateTime;
                hdPending.durationStr = pending.durationStr;
                hdPending.barSizeSetting = pending.barSizeSetting;
                hdPending.whatToShow = pending.whatToShow;
                hdPending.useRTH = pending.useRTH;
                hdPending.formatDate = pending.formatDate;
                hdPending.reqId = pending.originalReqId;

                _sendHistoricalDataRequest(hdPending, contract);
                QStringList hdKeys = {"requestHistoricalData", QString::number(contract.contractId), pending.endDateTime,
                                      pending.durationStr, pending.barSizeSetting, pending.whatToShow};
                _cache.add(pending.originalReqId, std::make_shared<HistoricalDataCacheEntry>(), hdKeys);
            }
        }
    }
}

void TwsClient::_handleContractData(ContractDataServerMessage *msg)
{
    auto entry = std::dynamic_pointer_cast<ContractDetailsCacheEntry>(_cache.get(msg->requestId()));
    if(entry) {
        entry->contractDetails.append(msg->contract());
    }
}

void TwsClient::_handleContractDataEnd(ContractDataEndServerMessage *msg)
{
    auto entry = std::dynamic_pointer_cast<ContractDetailsCacheEntry>(_cache.get(msg->requestId()));
    if(entry) {
        entry->setReady();
        emit contractDetailReadySignal(msg->requestId(), entry->contractDetails);

        // Check if there are pending historical data requests for this contract
        long contractId = -1;
        if(!entry->contractDetails.isEmpty()) {
            contractId = entry->contractDetails.first().contractId;
        }

        if(contractId >= 0 && _pendingHistoricalDataRequests.contains(contractId)) {
            QList<PendingHistoricalDataRequest> pendingList = _pendingHistoricalDataRequests.take(contractId);
            for(const PendingHistoricalDataRequest &pending : pendingList) {
                qDebug() << "Sending pending historical data request for contract" << contractId;
                _sendHistoricalDataRequest(pending, entry->contractDetails.first());
                QStringList keys = {"requestHistoricalData", QString::number(contractId), pending.endDateTime,
                                    pending.durationStr, pending.barSizeSetting, pending.whatToShow};
                _cache.add(pending.reqId, std::make_shared<HistoricalDataCacheEntry>(), keys);
            }
        }
    }
}

void TwsClient::_handlePortfolioValue(PortfolioValueServerMessage *msg)
{
    emit portfolioPositionUpdatedSignal(msg->position());
}

void TwsClient::_handleHistoricalData(HistoricalDataServerMessage *msg)
{
    auto entry = std::dynamic_pointer_cast<HistoricalDataCacheEntry>(_cache.get(msg->requestId()));
    if(entry) {
        entry->bars.append(msg->bars());
    }
}

void TwsClient::_handleHistoricalDataUpdate(HistoricalDataUpdateServerMessage *msg)
{
    auto entry = std::dynamic_pointer_cast<HistoricalDataCacheEntry>(_cache.get(msg->requestId()));
    if(entry) {
        entry->bars.append(msg->bar());
    }
}

void TwsClient::_handleHistoricalDataEnd(HistoricalDataEndServerMessage *msg)
{
    auto entry = std::dynamic_pointer_cast<HistoricalDataCacheEntry>(_cache.get(msg->requestId()));
    if(entry) {
        entry->setReady();
        QString description = _historicalDataDescriptions.take(msg->requestId());
        emit historicalDataReadySignal(msg->requestId(), entry->bars, description);
    }
}

void TwsClient::onError(const QString &reason, bool fatal)
{
    emit errorSignal(reason, fatal);
}

void TwsClient::_sendHistoricalDataRequest(const PendingHistoricalDataRequest &pending,
                                            const common::ContractDetailsDTO &contractDetails)
{
    QString secTypeStr = _securityTypeToIbString(contractDetails.securityType);
    if(secTypeStr.isEmpty()) {
        secTypeStr = "STK"; // default to stock
    }

    int serverVersion = _client->apiVersion();
    _client->send(RequestHistoricalDataClientMessage(pending.reqId, pending.contractId,
                                                     pending.endDateTime, pending.barSizeSetting,
                                                     pending.durationStr, pending.whatToShow,
                                                     pending.useRTH, pending.formatDate,
                                                     serverVersion,
                                                     contractDetails.symbol,      // symbol
                                                     secTypeStr,                 // secType
                                                     "",                         // lastTradeDateOrContractMonth
                                                     "",                         // strike
                                                     "",                         // right
                                                     "",                         // multiplier
                                                     "SMART",                    // exchange
                                                     contractDetails.primaryExchange, // primaryExchange
                                                     contractDetails.currency,   // currency
                                                     "",                         // localSymbol
                                                     "",                         // tradingClass
                                                     "0"));                      // includeExpired
}

QString TwsClient::_securityTypeToIbString(common::SecurityType type) const
{
    switch(type) {
    case common::SecurityType::STOCK:
        return "STK";
    case common::SecurityType::OPTION:
        return "OPT";
    case common::SecurityType::FUTURE:
        return "FUT";
    case common::SecurityType::FUTURE_OPTION:
        return "FOP";
    case common::SecurityType::INDEX:
        return "IND";
    case common::SecurityType::CFD:
        return "CFD";
    case common::SecurityType::BAG:
        return "BAG";
    case common::SecurityType::BOND:
        return "BOND";
    case common::SecurityType::CASH:
        return "CASH";
    case common::SecurityType::CMDTY:
        return "CMDTY";
    case common::SecurityType::CONTFUT:
        return "CONTFUT";
    case common::SecurityType::CRYPTO:
        return "CRYPTO";
    case common::SecurityType::FUND:
        return "FUND";
    case common::SecurityType::IOPT:
        return "IOPT";
    case common::SecurityType::NEWS:
        return "NEWS";
    case common::SecurityType::WAR:
        return "WAR";
    default:
        return "";
    }
}

void TwsClient::_handleOrderStatus(OrderStatusServerMessage *msg)
{
    const common::OrderStatusDTO &status = msg->orderStatus();

    // Keep already-received open orders in sync so that the list emitted at
    // OPEN_ORDER_END carries the latest execution state.
    for (common::OrderDTO &order : _pendingOpenOrders) {
        if (order.orderId == status.orderId) {
            order.status = status.status;
            order.filled = status.filled;
            order.remaining = status.remaining;
            order.avgFillPrice = status.avgFillPrice;
            break;
        }
    }

    emit orderStatusSignal(status);
}

void TwsClient::_handleOpenOrder(OpenOrderServerMessage *msg)
{
    _pendingOpenOrders.append(msg->order());
}

void TwsClient::_handleOpenOrderEnd(OpenOrderEndServerMessage *msg)
{
    Q_UNUSED(msg);
    emit openOrderSignal(_openOrdersReqId, _pendingOpenOrders);
    _pendingOpenOrders.clear();
}

void TwsClient::onHandshakeCompleted()
{
    emit connectedSignal();
}

void TwsClient::onDisconnected()
{
    emit disconnectedSignal();
}

}