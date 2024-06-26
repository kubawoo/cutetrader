#ifndef TWSCLIENT_H
#define TWSCLIENT_H

#include <QObject>
#include <QDateTime>
#include "DefaultEWrapper.h"
#include "EReaderOSSignal.h"
#include "EReader.h"

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
    long requestHistoricalData(const Contract &contract, const QString &endDateTime,
                               const QString &durationString, const QString &barSizeSetting);

public:
    bool connect(const QString& host, int port, int clientId = 0);
    void disconnect();
    bool isConnected();

public:
    // EWrapper methods
    virtual void nextValidId(OrderId orderId);
    virtual void currentTime(long time);
    virtual void managedAccounts( const std::string& accountsList);
    virtual void error(int id, int errorCode, const std::string& errorString,
                       const std::string& advancedOrderRejectJson);
    virtual void updateAccountValue(const std::string& key, const std::string& val,
        const std::string& currency, const std::string& accountName);
    virtual void updatePortfolio( const Contract& contract, Decimal position,
        double marketPrice, double marketValue, double averageCost,
        double unrealizedPNL, double realizedPNL, const std::string& accountName);
    virtual void updateAccountTime(const std::string& timeStamp);
    virtual void accountDownloadEnd(const std::string& accountName);
    virtual void position( const std::string& account, const Contract& contract, Decimal position, double avgCost);
    virtual void positionEnd();
    virtual void historicalData(long reqId, const Bar& bar);
    virtual void historicalDataEnd(long reqId, const std::string& startDateStr, const std::string& endDateStr);




signals:
    void connectedSignal();
    void disconnectedSignal();
    void nextValidIdSignal(long orderId);
    void currentTimeSignal(const QDateTime& time);
    void managedAccountsSignal(const QStringList & accounts);



private:
    EReaderOSSignal _readerSignal;
    EClientSocket * const _client;
    long _nextOrderId;
    EReader * _reader;
    bool _connected;
    QString _account;
    long _requestId;
};

#endif // TWSCLIENT_H
