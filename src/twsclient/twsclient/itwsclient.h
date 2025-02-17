#ifndef TWSCLIENT_ITWSCLIENT_H
#define TWSCLIENT_ITWSCLIENT_H

#include <QObject>
#include "DefaultEWrapper.h"
#include <common/types.h>

namespace twsclient {

class ITwsClient : public QObject, public DefaultEWrapper
{
    Q_OBJECT
public:
    explicit ITwsClient(QObject *parent = nullptr);
    virtual ~ITwsClient();

public slots:
    virtual void requestCurrentTime() = 0;
    virtual void startAccountUpdates() = 0;
    virtual void stopAccountUpdates() = 0;
    virtual void startPositionsUpdates() = 0;
    virtual void stopPositionsUpdates() = 0;
    virtual void requestManagedAccounts() = 0;
    virtual void startClient(const QString & accountId) = 0;
    virtual void requestOpenOrders() = 0;

public:
    virtual bool connect(const QString& host, int port, int clientId = 0) = 0;
    virtual void disconnect() = 0;
    virtual bool isConnected() = 0;
    virtual void clearCache() = 0;
//    int requestHistoricalData(const Contract &contract, const QString &endDateTime,
//                               const QString &durationString, const QString &barSizeSetting);
    virtual void requestContractDetails(long contractId, int * reqId = nullptr) = 0;
    virtual void requestMatchingSymbols(const QString & pattern, int * reqId = nullptr) = 0;




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
    void updateAccountTimeSignal(const QTime & time);

protected:
    void setRequestId(int * holder, int value);


};
}

#endif // TWSCLIENT_ITWSCLIENT_H
