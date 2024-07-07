#ifndef TWSCLIENT_H
#define TWSCLIENT_H

#include <QObject>
#include <QDateTime>
#include "DefaultEWrapper.h"
#include "EReaderOSSignal.h"
#include "EReader.h"
#include "cache.h"
#include "accountinfo.h"

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

public:
    bool connect(const QString& host, int port, int clientId = 0);
    void disconnect();
    bool isConnected();
    long requestHistoricalData(const Contract &contract, const QString &endDateTime,
                               const QString &durationString, const QString &barSizeSetting);
    double accountInfo(AccountInfoType type);
    QString accountBaseCurrency();


public:
    // EWrapper methods
    virtual void nextValidId(OrderId orderId) override;
    virtual void currentTime(long time) override;
    virtual void managedAccounts( const std::string& accountsList) override;
    virtual void error(int id, int errorCode, const std::string& errorString,
                       const std::string& advancedOrderRejectJson) override;
    virtual void updateAccountValue(const std::string& key, const std::string& val,
        const std::string& currency, const std::string& accountName) override;
    virtual void updatePortfolio( const Contract& contract, Decimal position,
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


signals:
    void connectedSignal();
    void disconnectedSignal();
    void currentTimeSignal(const QDateTime& time);
    void managedAccountsSignal(const QStringList & accounts);
    void historicalDataReadySignal(long requestId, QList<Bar> *bars);
    void accountInfoUpdated(AccountInfoType type);


private:
    EReaderOSSignal _readerSignal;
    EClientSocket * const _client;
    long _nextOrderId;
    EReader * _reader;
    bool _connected;
    AccountInfo _account;
    int _requestId;
    Cache _cache;
};

#endif // TWSCLIENT_H
