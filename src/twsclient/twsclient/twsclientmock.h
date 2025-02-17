#ifndef TWSCLIENT_TWSCLIENTMOCK_H
#define TWSCLIENT_TWSCLIENTMOCK_H

#include "itwsclient.h"
#include <QTimer>

namespace twsclient {

class TwsClientMock : public ITwsClient
{
    Q_OBJECT
public:
    explicit TwsClientMock(QObject *parent = nullptr);
    ~TwsClientMock();

public slots:
    void startClient(const QString & accountId) override;
    void disconnect() override;
    void requestCurrentTime() override;
    void requestManagedAccounts() override;
    void requestOpenOrders() override;
    void requestContractDetails(long contractId, int * reqId = nullptr) override;
    void requestMatchingSymbols(const QString & pattern, int * reqId = nullptr) override;

public:
    bool connect(const QString& host, int port, int clientId = 0) override;
    bool isConnected() override;
//    int requestHistoricalData(const Contract &contract, const QString &endDateTime,
//                               const QString &durationString, const QString &barSizeSetting);

private:
    void _timerTask();


private:
    bool _connected;
    int _requestId;
    QTimer * _timer;
};
}

#endif // TWSCLIENT_TWSCLIENTMOCK_H
