#include "twsclient/twsclient.h"
#include "EClientSocket.h"

#include <QDebug>
#include <QThread>

namespace twsclient {

TwsClient::TwsClient(QObject *parent)
    : ITwsClient{parent},
      _readerSignal(200),
      _client(new EClientSocket(this, &_readerSignal)),
      _nextOrderId(-1),
      _reader(nullptr),
      _requestId(0)
{}


TwsClient::~TwsClient()
{
    // destroy the reader before the client
    if(_reader) {
        delete _reader;
    }

    delete _client;
}


bool TwsClient::connect(const QString &host, int port, int clientId)
{
    // trying to connect
    qDebug().nospace() <<  "Connecting to " << host << ":" << port << " with clientId=" << clientId;

    _client->setConnectOptions("+PACEAPI");
    bool connected = _client->eConnect( host.toStdString().c_str(), port, clientId);

    if (connected) {
        qDebug().nospace() << "Connected to " << _client->host().c_str() << ":" << _client->port()
                           << "Server version: " << _client->EClient::serverVersion();
        _reader = new EReader(_client, &_readerSignal);
        _reader->start();
    } else {
        qDebug().nospace() << "Failed connecting to " << host << ":" << port;
    }

    return connected;
}

void TwsClient::disconnect()
{
    if(isConnected()) {
        qDebug() << "Trying to disconnect client" << _client->clientId();
        _client->eDisconnect();
        _connected = false;
        emit disconnectedSignal();
        qDebug().nospace() << "Client disconnected";
    } else {
        qDebug() << "Already disconnected";
    }
}

bool TwsClient::isConnected()
{
    bool res = _client->isConnected();
    if(_connected && !res) {
        _connected = false;
        emit disconnectedSignal();
    }
    return res;
}

void TwsClient::clearCache()
{
    _cache.clear();
}


void TwsClient::nextValidId(OrderId orderId)
{
    qDebug().nospace() << "Next Valid Id:" << orderId;
    _nextOrderId = orderId;
    if(!_connected) {
        _connected = true;
        emit connectedSignal();
    }
}

void TwsClient::requestCurrentTime() {
    qDebug() << "requestCurrentTime";
    _client->reqCurrentTime();
}

void TwsClient::startAccountUpdates() {
    _client->reqAccountUpdates(true, _accountId);
}
void TwsClient::stopAccountUpdates() {
    _client->reqAccountUpdates(false, _accountId);
}
void TwsClient::startPositionsUpdates()
{
    _client->reqPositions();
}
void TwsClient::stopPositionsUpdates()
{
    _client->cancelPositions();
}

void TwsClient::requestManagedAccounts()
{
    _client->reqManagedAccts();
}


void TwsClient::startClient(const QString &accountId)
{
    _accountId = accountId.toStdString();
    startAccountUpdates();
}

void TwsClient::requestOpenOrders()
{
    qDebug() << "requesting all open orders";
    _client->reqAllOpenOrders();
}

//int TwsClient::requestHistoricalData(const Contract &contract, const QString &endDateTime,
//                                      const QString &durationString, const QString &barSizeSetting)
//{
//    _client->reqHistoricalData(_requestId, contract, endDateTime.toStdString(),
//                               durationString.toStdString(), barSizeSetting.toStdString(),
//                               "TRADES", 1, 1, false, TagValueListSPtr());
//    return _requestId++;
//}

int TwsClient::requestContractDetails(long contractId)
{
    QStringList keys = {"requestContractDetails", QString::number(contractId)};
    auto cacheEntry = _cache.get(keys);

    if(cacheEntry.first >= 0 && cacheEntry.second && cacheEntry.second->ready()) {
        long requestId = cacheEntry.first;
        ContractDetailsCacheEntry * entry =  dynamic_cast<ContractDetailsCacheEntry*>(cacheEntry.second);
        qDebug() << "Returning from cache";
        emit contractDetailReadySignal(requestId, entry->contractDetails);
        return requestId;
    }

    _cache.add(_requestId, new ContractDetailsCacheEntry(), keys);
    _client->reqContractDetails(_requestId, buildContract(contractId));
    return _requestId++;
}

int TwsClient::requestMatchingSymbols(const QString &pattern)
{
    QStringList keys = {"requestMatchingSymbols", pattern};
    auto cacheEntry = _cache.get(keys);

    if(cacheEntry.first >= 0 && cacheEntry.second && cacheEntry.second->ready()) {
        long requestId = cacheEntry.first;
        ContractDetailsCacheEntry * entry =  dynamic_cast<ContractDetailsCacheEntry*>(cacheEntry.second);
        qDebug() << "Returning from cache" << keys;
        emit matchingSymbolsReadySignal(requestId, entry->contractDetails);
        return requestId;
    }

    _cache.add(_requestId, new ContractDetailsCacheEntry(), keys);
    _client->reqMatchingSymbols(_requestId, pattern.toStdString());
    return _requestId++;
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
    emit managedAccountsSignal(accounts);
}

void TwsClient::error(int id, time_t errorTime, int errorCode, const std::string &errorString, const std::string &advancedOrderRejectJson)
{
    qDebug() << "error" << id << errorCode << errorString.c_str() << advancedOrderRejectJson.c_str();
    //    emit errorSignal();
}


void TwsClient::updateAccountTime(const std::string& timeStamp)
{
    qDebug() << "updateAccountTime" << timeStamp.c_str();
    QTime time = QTime::fromString(timeStamp.c_str(), "HH:mm");
    emit updateAccountTimeSignal(time);
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

void TwsClient::updateAccountValue(const std::string& key, const std::string& val,
                                   const std::string& currency, const std::string& accountName)
{
    qDebug() << "updateAccountValue" << key.c_str() << val.c_str() << currency.c_str() << accountName.c_str();

    if(accountName == _accountId) {
        emit accountValueUpdatedSignal(key.c_str(), val.c_str(), currency.c_str());
    }

}

void TwsClient::updatePortfolio(const Contract& contract, Decimal position,
                                double marketPrice, double marketValue, double averageCost,
                                double unrealizedPNL, double realizedPNL, const std::string& accountName)
{
    qDebug() << "updatePortfolio" << contract.secType.c_str()
             << contract.symbol.c_str() << DecimalFunctions::decimalToDouble(position)
             << marketPrice << marketValue << averageCost
             << unrealizedPNL << realizedPNL << accountName.c_str();


    if(accountName != _accountId) {
        return;
    }

    common::PortfolioPositionDTO positionDto;
    positionDto.contractId = contract.conId;
    positionDto.symbol = contract.symbol.c_str();
    positionDto.position = DecimalFunctions::decimalToDouble(position);
    positionDto.marketPrice = marketPrice;
    positionDto.marketValue = marketValue;
    positionDto.averageCost = averageCost;
    positionDto.unrealizedPNL = unrealizedPNL;
    positionDto.realizedPNL = realizedPNL;

    if(contract.secType == "STK") {
        positionDto.securityType = common::SecurityType::STOCK;
    } else if(contract.secType == "OPT" || contract.secType == "FOP") {
        qDebug() << "Option:" << contract.lastTradeDateOrContractMonth.c_str()
                 << contract.strike
                 << contract.right.c_str()
                 << contract.multiplier.c_str();

        positionDto.securityType = contract.secType == "OPT"
                ? common::SecurityType::OPTION : common::SecurityType::FUTURE_OPTION;

        QDate expiration = QDate::fromString(contract.lastTradeDateOrContractMonth.c_str(), "yyyyMMdd");
        common::OptionType type = contract.right == "C" ? common::OptionType::CALL : common::OptionType::PUT;
        double mul = QString(contract.multiplier.c_str()).toDouble();

        positionDto.expiration = expiration;
        positionDto.right = type;
        positionDto.multiplier = mul;
        positionDto.strike = contract.strike;
    } else if(contract.secType == "FUT") {
        qDebug() << "Future:" << contract.lastTradeDateOrContractMonth.c_str()
                 << contract.lastTradeDate.c_str();

        QDate expiration = QDate::fromString(contract.lastTradeDateOrContractMonth.c_str(), "yyyyMMdd");

        positionDto.securityType = common::SecurityType::FUTURE;
        positionDto.expiration = expiration;
    } else {
        qDebug() << "Unknown security type" << contract.secType.c_str();
        return;
    }

    emit portfolioPositionUpdatedSignal(positionDto);
}

void TwsClient::historicalData(long reqId, const Bar &bar)
{
    qDebug() << "historicalData" << reqId << bar.time.c_str() << bar.close;
//    _cache.addBar(reqId, bar);
}

void TwsClient::historicalDataEnd(int reqId, const std::string &startDateStr, const std::string &endDateStr)
{
    qDebug() << "historicalDataEnd" << reqId << startDateStr.c_str() << endDateStr.c_str();
//    emit historicalDataReadySignal(reqId, _cache.bars(reqId));
}

void TwsClient::contractDetails(int reqId, const ContractDetails &contractDetails)
{
    Contract c = contractDetails.contract;
    qDebug() << "contractDetails contract" << c.conId << c.symbol.c_str() << c.secType.c_str()
             << c.lastTradeDateOrContractMonth.c_str() << c.lastTradeDate.c_str()
             << c.strike << c.right.c_str() << c.multiplier.c_str() << c.exchange.c_str()
             << c.primaryExchange.c_str() << c.currency.c_str() << c.localSymbol.c_str()
             << c.tradingClass.c_str();

    qDebug() << "contractDetails details" << contractDetails.marketName.c_str()
             << contractDetails.minTick << contractDetails.priceMagnifier << contractDetails.orderTypes.c_str()
             << contractDetails.validExchanges.c_str() << contractDetails.underConId << contractDetails.longName.c_str()
             << contractDetails.contractMonth.c_str() << contractDetails.industry.c_str() << contractDetails.category.c_str()
             << contractDetails.subcategory.c_str() << contractDetails.timeZoneId.c_str() << contractDetails.tradingHours.c_str()
             << contractDetails.liquidHours.c_str() << contractDetails.evRule.c_str() << contractDetails.evMultiplier
             << contractDetails.aggGroup << contractDetails.underSymbol.c_str() << contractDetails.stockType.c_str();



    ContractDetailsCacheEntry * entry =  dynamic_cast<ContractDetailsCacheEntry*>(_cache.get(reqId));
    common::ContractDetailsDTO dto;
    dto.contractId = contractDetails.contract.conId;
    dto.currency = contractDetails.contract.currency.c_str();
    dto.symbol = contractDetails.contract.symbol.c_str();
    dto.description = contractDetails.longName.c_str();
    entry->contractDetails.append(dto);
}

void TwsClient::contractDetailsEnd(int reqId)
{
    qDebug() << "contractDetailsEnd";
    ContractDetailsCacheEntry * entry =  dynamic_cast<ContractDetailsCacheEntry*>(_cache.get(reqId));
    entry->setReady();
    emit contractDetailReadySignal(reqId, entry->contractDetails);
}

void TwsClient::symbolSamples(int reqId, const std::vector<ContractDescription> &contractDescriptions)
{
    qDebug() << "symbolSamples";
    ContractDetailsCacheEntry * entry =  dynamic_cast<ContractDetailsCacheEntry*>(_cache.get(reqId));

    for(ContractDescription cd : contractDescriptions) {
        if(cd.contract.conId < 0) {
            continue;
        }

        qDebug() << cd.contract.conId << cd.contract.symbol.c_str()
                 << cd.contract.currency.c_str() << cd.contract.description.c_str()
                 << cd.contract.secType.c_str() << cd.contract.primaryExchange.c_str()
                 << cd.contract.issuerId.c_str();


        common::ContractDetailsDTO dto;
        dto.contractId = cd.contract.conId;
        dto.currency = cd.contract.currency.c_str();
        dto.symbol = cd.contract.symbol.c_str();
        dto.description = cd.contract.description.c_str();
        dto.securityType = common::Utils::securityTypeFromString(cd.contract.secType.c_str());
        for(auto d : cd.derivativeSecTypes) {
            qDebug() << d.c_str();
            common::SecurityType der = common::Utils::securityTypeFromString(d.c_str());
            if(der != common::SecurityType::UNSUPPORTED) {
                dto.derivatives.append(der);
            }
        }
        entry->contractDetails.append(dto);
    }
    entry->setReady();
    emit matchingSymbolsReadySignal(reqId, entry->contractDetails);
}

void TwsClient::openOrder(OrderId orderId, const Contract &contract, const Order &order, const OrderState &orderState)
{
    qDebug() << "openOrder" << orderId << contract.symbol.c_str();
}

void TwsClient::openOrderEnd()
{
    qDebug() << "openOrderEnd";
}

void TwsClient::orderStatus(OrderId orderId, const std::string &status, Decimal filled, Decimal remaining,
                            double avgFillPrice, long long permId, int parentId, double lastFillPrice,
                            int clientId, const std::string &whyHeld, double mktCapPrice)
{
    qDebug() << "order status";
}

void TwsClient::bondContractDetails(int reqId, const ContractDetails &contractDetails)
{
    qDebug() << "bondContractDetails" << contractDetails.contract.symbol.c_str()
             << contractDetails.contract.secType.c_str() << contractDetails.cusip.c_str()
             << contractDetails.coupon << contractDetails.maturity.c_str()
                <<  contractDetails.issueDate.c_str()
                <<  contractDetails.ratings.c_str()
                <<  contractDetails.bondType.c_str();
}


Contract TwsClient::buildContract(long contractId)
{
    Contract c;
    c.conId = contractId;
    c.exchange = "SMART";
    return c;
}


void TwsClient::checkMessages()
{
    if(_reader && isConnected()) {
        _readerSignal.waitForSignal();
        _reader->processMsgs();
    }
}

void TwsClient::cleanup()
{
    qDebug() << "Running cleanup task";
    _cache.removeExpired();
}

}
