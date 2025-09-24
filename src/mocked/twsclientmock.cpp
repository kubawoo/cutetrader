#include "mocked/twsclientmock.h"
#include <QRandomGenerator>

using common::SecurityType;
using common::ContractDetailsDTO;

namespace mocked {

TwsClientMock::TwsClientMock(QObject *parent)
    : ITwsClient(parent),
      _connected(false),
      _requestId(0),
      _timer{new QTimer}
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
}

TwsClientMock::~TwsClientMock()
{
    qDebug() << "Destroying" << this;
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


}
