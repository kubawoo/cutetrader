#ifndef TWSCLIENT_TWSCLIENT_H
#define TWSCLIENT_TWSCLIENT_H

#include <QObject>
#include <QDateTime>
#include "itwsclient.h"
#include "EReaderOSSignal.h"
#include "EReader.h"
#include "cache.h"
#include <common.h>

namespace twsclient {

class TwsClient : public ITwsClient
{
    Q_OBJECT
public:
    explicit TwsClient(QObject *parent = nullptr);
    ~TwsClient();

public slots:
    void checkMessages() override;
    void cleanup() override;
    void requestCurrentTime() override;
    void startAccountUpdates() override;
    void stopAccountUpdates() override;
    void startPositionsUpdates() override;
    void stopPositionsUpdates() override;
    void requestManagedAccounts() override;
    void startClient(const QString & accountId) override;
    void requestOpenOrders() override;

public:
    bool connect(const QString& host, int port, int clientId = 0) override;
    void disconnect() override;
    bool isConnected() override;
    void clearCache() override;
//    int requestHistoricalData(const Contract &contract, const QString &endDateTime,
//                               const QString &durationString, const QString &barSizeSetting);
    int requestContractDetails(long contractId) override;
    int requestMatchingSymbols(const QString & pattern) override;


public:
    // EWrapper methods
    virtual void nextValidId(OrderId orderId) override;
    virtual void currentTime(long time) override;
    virtual void managedAccounts(const std::string& accountsList) override;
    virtual void error(int id, time_t errorTime, int errorCode, const std::string& errorString,
                       const std::string& advancedOrderRejectJson) override;
    virtual void updateAccountValue(const std::string& key, const std::string& val,
        const std::string& currency, const std::string& accountName) override;
    virtual void updatePortfolio(const Contract& contract, Decimal position,
        double marketPrice, double marketValue, double averageCost,
        double unrealizedPNL, double realizedPNL, const std::string& accountName) override;
    virtual void updateAccountTime(const std::string& timeStamp) override;
    virtual void accountDownloadEnd(const std::string& accountName) override;
    virtual void position( const std::string& account, const Contract& contract,
                           Decimal position, double avgCost) override;
    virtual void positionEnd() override;
    virtual void historicalData(long reqId, const Bar& bar) override;
    virtual void historicalDataEnd(int reqId, const std::string& startDateStr,
                                   const std::string& endDateStr) override;
    virtual void contractDetails(int reqId, const ContractDetails& contractDetails) override;
    virtual void contractDetailsEnd(int reqId) override;
    virtual void symbolSamples(int reqId, const std::vector<ContractDescription> &contractDescriptions) override;
    virtual void openOrder(OrderId orderId, const Contract& contract, const Order& order,
                           const OrderState& orderState) override;
    virtual void openOrderEnd() override;
    virtual void orderStatus( OrderId orderId, const std::string& status, Decimal filled,
        Decimal remaining, double avgFillPrice, long long permId, int parentId,
        double lastFillPrice, int clientId, const std::string& whyHeld, double mktCapPrice) override;
    virtual void bondContractDetails(int reqId, const ContractDetails& contractDetails) override;


private:
    Contract buildContract(long contractId);

    EReaderOSSignal _readerSignal;
    EClientSocket * const _client;
    long _nextOrderId;
    EReader * _reader;
    bool _connected;
    unsigned int _requestId;
    Cache _cache;
    std::string _accountId;
};

}
#endif
