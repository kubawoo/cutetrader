#ifndef TWSCLIENT_TWSCLIENTMOCK_H
#define TWSCLIENT_TWSCLIENTMOCK_H

#include "itwsclient.h"

namespace twsclient {

class TwsClientMock : public ITwsClient
{
    Q_OBJECT
public:
    explicit TwsClientMock(QObject *parent = nullptr);
    ~TwsClientMock();

public slots:
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

private:
    bool _connected;
};
}

#endif // TWSCLIENT_TWSCLIENTMOCK_H
