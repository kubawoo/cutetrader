#ifndef TWSCLIENT_TWSCLIENT_H
#define TWSCLIENT_TWSCLIENT_H

#include <QObject>
#include <QDateTime>
#include "DefaultEWrapper.h"
#include "EReaderOSSignal.h"
#include "EReader.h"
#include "cache.h"
#include <common.h>

namespace twsclient {

class TwsClient : public QObject, public DefaultEWrapper
{
    Q_OBJECT
public:
    explicit TwsClient(QObject *parent = nullptr);
    ~TwsClient();

public slots:
    void checkMessages();
    void cleanup();
    void requestCurrentTime();
    void startAccountUpdates();
    void stopAccountUpdates();
    void startPositionsUpdates();
    void stopPositionsUpdates();
    void requestManagedAccounts();
    void startClient(const QString & accountId);

public:
    bool connect(const QString& host, int port, int clientId = 0);
    void disconnect();
    bool isConnected();
//    int requestHistoricalData(const Contract &contract, const QString &endDateTime,
//                               const QString &durationString, const QString &barSizeSetting);
    int requestContractDetails(long contractId);
    int requestMatchingSymbols(const QString & pattern);


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


signals:
    void connectedSignal();
    void disconnectedSignal();
    void currentTimeSignal(const QDateTime & time);
    void managedAccountsSignal(const QStringList accounts);
//    void historicalDataReadySignal(long requestId, QList<Bar> *bars);
    void accountValueUpdatedSignal(const QString & key, const QString & value, const QString & currency);
    void portfolioPositionUpdatedSignal(const common::PortfolioPositionDTO & position);
    void contractDetailReadySignal(const int & requestId, const QList<common::ContractDetailsDTO> & details);
    void matchingSymbolsReadySignal(const int & requestId, const QList<common::ContractDetailsDTO> & details);


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
