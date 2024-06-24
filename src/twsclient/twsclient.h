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
    void requestCurrentTime();
    void startAccountUpdates();
    void stopAccountUpdates();
    void startPositionsUpdates();
    void stopPositionsUpdates();
    void requestManagedAccounts();

public:
    bool connect(const QString& host, int port, int clientId = 0);
    void disconnect();
    void setConnectOptions(const QString&);
    bool isConnected();

public:
    // EWrapper methods
    virtual void nextValidId(OrderId orderId);
    virtual void currentTime(long time);
    virtual void managedAccounts( const std::string& accountsList);
    virtual void error(int id, int errorCode, const std::string& errorString, const std::string& advancedOrderRejectJson);
    virtual void updateAccountValue(const std::string& key, const std::string& val,
        const std::string& currency, const std::string& accountName);
    virtual void updatePortfolio( const Contract& contract, Decimal position,
        double marketPrice, double marketValue, double averageCost,
        double unrealizedPNL, double realizedPNL, const std::string& accountName);
    virtual void updateAccountTime(const std::string& timeStamp);
    virtual void accountDownloadEnd(const std::string& accountName);
    virtual void position( const std::string& account, const Contract& contract, Decimal position, double avgCost);
    virtual void positionEnd();



signals:
    void connectedSignal();
    void disconnectedSignal();
    void nextValidIdSignal(long orderId);
    void currentTimeSignal(const QDateTime& time);
    void managedAccountsSignal(const QStringList & accounts);



private:
    EReaderOSSignal readerSignal;
    EClientSocket * const client;
    OrderId nextOrderId;
    EReader * reader;
    bool connected;
    QString account;
};

#endif // TWSCLIENT_H
