#include "mocked/twsclientmock.h"
#include <QRandomGenerator>

using common::SecurityType;
using common::ContractDetailsDTO;
using common::OrderDTO;

namespace mocked {

TwsClientMock::TwsClientMock(QObject *parent)
    : ITwsClient(parent),
      _connected(false),
      _requestId(0),
      _timer{new QTimer(this)}
{
    qDebug() << "Constructing" << this;
    QObject::connect(_timer, &QTimer::timeout, this, &TwsClientMock::_timerTask);

    _contractsMap = {
        {1, {1, "SPY", "USD", "SPDR S&P 500 ETF Trust", "ARCA", "", SecurityType::STOCK, {SecurityType::OPTION}}},
        {2, {2, "TLT", "USD", "iShares 20+ Year Treasury Bond ETF", "ARCA", "", SecurityType::STOCK, {SecurityType::OPTION}}},
        {3, {3, "IWM", "USD", "iShares Russell 2000 ETF", "ARCA", "", SecurityType::STOCK, {SecurityType::OPTION}}},
        {4, {4, "EWZ", "USD", "Ishares Msci Brazil ETF", "ARCA", "", SecurityType::STOCK, {SecurityType::OPTION}}},
        {5, {5, "TSLA", "USD", "Tesla Inc.", "NYSE", "", SecurityType::STOCK, {SecurityType::OPTION}}},
        {6, {6, "AAPL", "USD", "Apple Inc.", "NYSE", "", SecurityType::STOCK, {SecurityType::OPTION}}}
    };

    // Sample open orders
    _sampleOrders = {
        {1001, 1, "SPY", "SPY", SecurityType::STOCK, "USD", "BUY", 100, "LMT", 450.00, 0.0, "DAY", "mocked1", QDate(), "Submitted", 0, 100, 0.0},
        {1002, 2, "TLT", "TLT", SecurityType::STOCK, "USD", "SELL", 50, "MKT", 0.0, 0.0, "DAY", "mocked1", QDate(), "PreSubmitted", 0, 50, 0.0},
        {1003, 5, "TSLA", "TSLA", SecurityType::STOCK, "USD", "BUY", 10, "LMT", 250.00, 0.0, "GTC", "mocked1", QDate(), "PendingSubmit", 0, 10, 0.0},
        {1004, 6, "AAPL", "AAPL", SecurityType::STOCK, "USD", "SELL", 25, "STP", 0.0, 175.00, "DAY", "mocked1", QDate(), "Submitted", 0, 25, 0.0},
        {1005, 3, "IWM", "IWM", SecurityType::STOCK, "USD", "BUY", 75, "LMT", 200.50, 0.0, "DAY", "mocked1", QDate(), "Filled", 75, 0, 200.50}
    };
}

TwsClientMock::~TwsClientMock()
{
    qDebug() << "Destroying" << this;
    _timer->stop();
    // _timer is parented to this, Qt will delete it automatically
}


void TwsClientMock::requestCurrentTime()
{
    emit currentTimeSignal(QDateTime::currentDateTime());
}


void TwsClientMock::requestManagedAccounts()
{
    emit managedAccountsSignal({"mocked1", "mocked2"});
}

void TwsClientMock::startClient(const QString &accountId)
{
    if(_connected) {
        _timer->start(60 * 1000); // every minute
        _timerTask();
        requestOpenOrders();
    }
}

void TwsClientMock::requestOpenOrders()
{
    int reqId = _requestId++;
    emit openOrderSignal(reqId, _sampleOrders);
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
    qDebug() << "disconnecting";
    _connected = false;
}

bool TwsClientMock::isConnected()
{
    return _connected;
}

void TwsClientMock::_timerTask()
{
    if(_connected) {

        double spy = (500 + QRandomGenerator::global()->generateDouble() * 200)*100;
        double netLiq = 40000+spy;
        double mntMargin = spy * 0.25;

        emit updateAccountTimeSignal(QTime::currentTime());
        emit accountValueUpdatedSignal("NetLiquidation", QString::number(netLiq), "USD");
        emit accountValueUpdatedSignal("ExcessLiquidity", QString::number(netLiq - mntMargin), "USD");
        emit accountValueUpdatedSignal("MaintMarginReq", QString::number(mntMargin), "USD");
        emit accountValueUpdatedSignal("StockMarketValue", QString::number(spy), "USD");
        emit accountValueUpdatedSignal("UnrealizedPnL", QString::number(netLiq-100000), "USD");
        emit accountValueUpdatedSignal("CashBalance", "40000", "USD");

        common::PortfolioPositionDTO positionDto;
        positionDto.contractId = 1;
        positionDto.symbol = "SPY";
        positionDto.securityType = SecurityType::STOCK;
        positionDto.position = 100;
        positionDto.unrealizedPNL = spy - 60000;
        positionDto.marketValue = spy;
        positionDto.marketPrice = positionDto.marketValue / positionDto.position;
        emit portfolioPositionUpdatedSignal(positionDto);

    }
}


void TwsClientMock::requestContractDetails(long contractId, int * reqId)
{
    _setRequestId(reqId, _requestId++);


    if(_contractsMap.contains(contractId)) {
        qDebug() << "Yes, it's there";
        emit contractDetailReadySignal(*reqId, {_contractsMap[contractId]});
    }
}

void TwsClientMock::requestMatchingSymbols(const QString &pattern, int * reqId)
{
    _setRequestId(reqId, _requestId++);

    QList<ContractDetailsDTO> dtos;

    for(auto dto : _contractsMap.values()) {
        if(dto.symbol.toLower().contains(pattern.toLower())) {
            dtos << dto;
        }
    }

    emit matchingSymbolsReadySignal(*reqId, dtos);

}

static QList<common::BarDTO> generateMockBars(long contractId, const QString &symbol,
                                                            const QString &endDateTime,
                                                            const QString &durationStr,
                                                            const QString &barSizeSetting)
{
    Q_UNUSED(endDateTime)
    Q_UNUSED(durationStr)
    Q_UNUSED(barSizeSetting)

    QList<common::BarDTO> bars;
    double basePrice = 400.0;

    // Set base price based on symbol/contractId
    switch (contractId) {
        case 1: basePrice = 450.0; break; // SPY
        case 2: basePrice = 100.0; break; // TLT
        case 3: basePrice = 200.0; break; // IWM
        case 4: basePrice = 35.0; break;  // EWZ
        case 5: basePrice = 250.0; break; // TSLA
        case 6: basePrice = 180.0; break; // AAPL
        default:
            if (symbol.toUpper() == "SPY") basePrice = 450.0;
            else if (symbol.toUpper() == "TLT") basePrice = 100.0;
            else if (symbol.toUpper() == "IWM") basePrice = 200.0;
            else if (symbol.toUpper() == "EWZ") basePrice = 35.0;
            else if (symbol.toUpper() == "TSLA") basePrice = 250.0;
            else if (symbol.toUpper() == "AAPL") basePrice = 180.0;
            break;
    }

    // Generate ~20 bars of mock data
    QDateTime baseTime = QDateTime::currentDateTime().addDays(-20);
    for (int i = 0; i < 20; ++i) {
        common::BarDTO bar;
        double variation = (QRandomGenerator::global()->generateDouble() - 0.5) * 10.0;
        double open = basePrice + variation;
        double high = open + QRandomGenerator::global()->generateDouble() * 5.0;
        double low = open - QRandomGenerator::global()->generateDouble() * 5.0;
        double close = low + QRandomGenerator::global()->generateDouble() * (high - low);
        double volume = 1000000 + QRandomGenerator::global()->generateDouble() * 5000000;
        double wap = (high + low + close) / 3.0;
        int barCount = 1000 + QRandomGenerator::global()->generate() % 5000;

        bar.time = baseTime.addDays(i).toString("yyyy-MM-dd");
        bar.open = open;
        bar.high = high;
        bar.low = low;
        bar.close = close;
        bar.volume = volume;
        bar.wap = wap;
        bar.barCount = barCount;

        bars << bar;
        basePrice = close; // Next day starts near previous close
    }
    return bars;
}

void TwsClientMock::requestHistoricalData(long contractId, const QString & endDateTime,
                                          const QString & durationStr, const QString & barSizeSetting,
                                          const QString & whatToShow, int useRTH, int formatDate,
                                          int * reqId)
{
    Q_UNUSED(whatToShow)
    Q_UNUSED(useRTH)
    Q_UNUSED(formatDate)
    _setRequestId(reqId, _requestId++);

    QString symbol;
    QString description;
    if (_contractsMap.contains(contractId)) {
        symbol = _contractsMap[contractId].symbol;
        description = _contractsMap[contractId].description;
    }

    QList<common::BarDTO> bars = generateMockBars(contractId, symbol, endDateTime, durationStr, barSizeSetting);
    emit historicalDataReadySignal(*reqId, bars, description);
}

void TwsClientMock::requestHistoricalData(const QString & symbol, const QString & endDateTime,
                                          const QString & durationStr, const QString & barSizeSetting,
                                          const QString & whatToShow, int useRTH, int formatDate,
                                          int * reqId)
{
    Q_UNUSED(whatToShow)
    Q_UNUSED(useRTH)
    Q_UNUSED(formatDate)
    _setRequestId(reqId, _requestId++);

    long contractId = 0;
    QString description;
    for (auto dto : _contractsMap.values()) {
        if (dto.symbol.toLower() == symbol.toLower()) {
            contractId = dto.contractId;
            description = dto.description;
            break;
        }
    }

    QList<common::BarDTO> bars = generateMockBars(contractId, symbol, endDateTime, durationStr, barSizeSetting);
    emit historicalDataReadySignal(*reqId, bars, description);
}

void TwsClientMock::cancelHistoricalData(int reqId)
{
    Q_UNUSED(reqId)
    // Mock implementation - no-op
}
}