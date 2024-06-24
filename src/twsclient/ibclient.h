#ifndef IBCLIENT_H
#define IBCLIENT_H

#include <QObject>
#include <QDateTime>
#include "DefaultEWrapper.h"
#include "EReaderOSSignal.h"
#include "EReader.h"

class IBClient : public QObject, public DefaultEWrapper
{
    Q_OBJECT
public:
    explicit IBClient(QObject *parent = nullptr);
    ~IBClient();

public slots:
    void requestCurrentTime();
    void checkMessages();

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
};

#endif // IBCLIENT_H
