#ifndef TWSCLIENT_TWSCLIENTMOCK_H
#define TWSCLIENT_TWSCLIENTMOCK_H

#include <common.h>
#include <QTimer>

namespace mocked {

class TwsClientMock : public common::ITwsClient
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
    void requestHistoricalData(long contractId, const QString & endDateTime,
                               const QString & durationStr, const QString & barSizeSetting,
                               const QString & whatToShow, int useRTH, int formatDate,
                               int * reqId = nullptr) override;

public:
    bool connect(const QString& host, int port, int clientId = 0) override;
    bool isConnected() override;

private:
    void _timerTask();


private:
    bool _connected;
    int _requestId;
    QTimer * _timer;

    QMap<long, common::ContractDetailsDTO> _contractsMap;
};
}

#endif // TWSCLIENT_TWSCLIENTMOCK_H
