#include "twsclient/twsclient.h"
#include "EClientSocket.h"

#include <QDebug>
#include <QThread>

namespace twsclient {

TwsClient::TwsClient(QObject *parent)
    : QObject{parent},
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

    disconnect();
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

//long TwsClient::requestHistoricalData(const Contract &contract, const QString &endDateTime,
//                                      const QString &durationString, const QString &barSizeSetting)
//{
//    _client->reqHistoricalData(_requestId, contract, endDateTime.toStdString(),
//                               durationString.toStdString(), barSizeSetting.toStdString(),
//                               "TRADES", 1, 1, false, TagValueListSPtr());
//    return _requestId++;
//}

long TwsClient::requestContractDetails(long contractId)
{
    QStringList keys = {"requestContractDetails", QString::number(contractId)};
    ContractDetailsCacheEntry * entry =  dynamic_cast<ContractDetailsCacheEntry*>(_cache.get(keys));
    if(entry) {
        qDebug() << "Returning from cache";
        emit contractDetailReadySignal(entry->contractDetails);
        return 0;
    }


    _cache.add(_requestId, new ContractDetailsCacheEntry(), keys);
    _client->reqContractDetails(_requestId, buildContract(contractId));
    return _requestId++;
}

long TwsClient::requestMatchingSymbols(const QString &pattern)
{
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
    } else if(contract.secType == "OPT") {
        qDebug() << "Option:" << contract.lastTradeDateOrContractMonth.c_str()
                 << contract.lastTradeDate.c_str()
                 << contract.strike
                 << contract.right.c_str()
                 << contract.multiplier.c_str();


        QDate expiration = QDate::fromString(contract.lastTradeDateOrContractMonth.c_str(), "yyyyMMdd");
        common::OptionType type = contract.right == "C" ? common::OptionType::CALL : common::OptionType::PUT;
        double mul = QString(contract.multiplier.c_str()).toDouble();

        positionDto.securityType = common::SecurityType::OPTION;
        positionDto.expiration = expiration;
        positionDto.right = type;
        positionDto.multiplier = mul;
        positionDto.strike = contract.strike;
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
    qDebug() << "contractDetails" << contractDetails.contract.symbol.c_str();
    ContractDetailsCacheEntry * entry =  dynamic_cast<ContractDetailsCacheEntry*>(_cache.get(reqId));
    common::ContractDetailsDTO dto;
    dto.contractId = contractDetails.contract.conId;
    dto.currency = contractDetails.contract.currency.c_str();
    dto.symbol = contractDetails.contract.symbol.c_str();
    dto.description = contractDetails.contract.description.c_str();
    entry->contractDetails.append(dto);
}

void TwsClient::contractDetailsEnd(int reqId)
{
    qDebug() << "contractDetailsEnd";
    ContractDetailsCacheEntry * entry =  dynamic_cast<ContractDetailsCacheEntry*>(_cache.get(reqId));
    emit contractDetailReadySignal(entry->contractDetails);
}

void TwsClient::symbolSamples(int reqId, const std::vector<ContractDescription> &contractDescriptions)
{
    qDebug() << "symbolSamples";
    for(ContractDescription cd : contractDescriptions) {
        if(cd.contract.conId < 0) {
            continue;
        }

        qDebug() << cd.contract.conId << cd.contract.symbol.c_str()
                 << cd.contract.currency.c_str() << cd.contract.exchange.c_str() << cd.contract.description.c_str();

    }
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
    _cache.cleanup();
}

}
