#include "twsclient.h"
#include "EClientSocket.h"

#include <QDebug>
#include <QThread>

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
        qDebug().nospace() << "Connected to " << _client->host().c_str() << ":" << _client->port();
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
        startAccountUpdates();
    }
}

void TwsClient::requestCurrentTime() {
    qDebug() << "requestCurrentTime";
    _client->reqCurrentTime();
//    Contract c;
//    c.secType = "STK";
//    c.symbol = "SPY";
//    c.currency = "USD";
//    c.exchange = "SMART";
//    int reqId = requestHistoricalData(c, "", "30 D", "1 day");
//    qDebug() << "requestHistoricalData" << reqId;
    }

void TwsClient::startAccountUpdates() {
    _client->reqAccountUpdates(true, _account.accountId().toStdString());
}
void TwsClient::stopAccountUpdates() {
    _client->reqAccountUpdates(false, _account.accountId().toStdString());
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

long TwsClient::requestHistoricalData(const Contract &contract, const QString &endDateTime,
                                      const QString &durationString, const QString &barSizeSetting)
{
    _client->reqHistoricalData(_requestId, contract, endDateTime.toStdString(),
                              durationString.toStdString(), barSizeSetting.toStdString(),
                              "TRADES", 1, 1, false, TagValueListSPtr());
    return _requestId++;
}

double TwsClient::accountInfo(AccountInfoType type)
{
    return _account.value(type);
}

QString TwsClient::accountBaseCurrency()
{
    return _account.baseCurrency();
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
    this->_account.setAccountId(accounts[0]);
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

    if(accountName == _account.accountId().toStdString()
       && (currency == _account.baseCurrency().toStdString() || currency.empty())) {
        AccountInfoType updated = _account.updateValue(key.c_str(), val.c_str());
        if(updated != AccountInfoType::NONE) {
            emit accountInfoUpdated(updated);
        }
    }

}

void TwsClient::updatePortfolio( const Contract& contract, Decimal position,
    double marketPrice, double marketValue, double averageCost,
    double unrealizedPNL, double realizedPNL, const std::string& accountName)
{
    qDebug() << "updatePortfolio" << contract.secType.c_str()
             << contract.symbol.c_str() << DecimalFunctions::decimalToDouble(position)
             << marketPrice << marketValue << averageCost
             <<unrealizedPNL << realizedPNL << accountName.c_str();
    if(contract.secType == "OPT") {
        qDebug() << "Option:" << contract.lastTradeDateOrContractMonth.c_str()
                 << contract.lastTradeDate.c_str()
                 << contract.strike
                 << contract.right.c_str()
                 << contract.multiplier.c_str()
                 << contract.comboLegsDescrip.c_str();

        QDate expiration = QDate::currentDate(); //TODO
        OptionType type = contract.right == "C" ? OptionType::CALL : OptionType::PUT;
        double mul = 100.0; //TODO

        Option opt(contract.conId, contract.symbol.c_str(), DecimalFunctions::decimalToDouble(position),
                   marketPrice, marketValue, averageCost, unrealizedPNL, realizedPNL, expiration, contract.strike,
                   type, mul);
        _portfolio.updatePortfolio(opt);

    } else if(contract.secType == "STK") {
        Stock stk(contract.conId, contract.symbol.c_str(), DecimalFunctions::decimalToDouble(position),
                   marketPrice, marketValue, averageCost, unrealizedPNL, realizedPNL);
        _portfolio.updatePortfolio(stk);
    }
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

void TwsClient::historicalData(long reqId, const Bar &bar)
{
    qDebug() << "historicalData" << reqId << bar.time.c_str() << bar.close;
    _cache.addBar(reqId, bar);
}

void TwsClient::historicalDataEnd(int reqId, const std::string &startDateStr, const std::string &endDateStr)
{
    qDebug() << "historicalDataEnd" << reqId << startDateStr.c_str() << endDateStr.c_str();
    emit historicalDataReadySignal(reqId, _cache.bars(reqId));
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
