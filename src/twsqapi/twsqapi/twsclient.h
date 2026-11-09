#ifndef TWSQAPI_TWSCLIENT_H
#define TWSQAPI_TWSCLIENT_H

#include <QObject>
#include <QMap>
#include <QTimer>
#include <memory>
#include "socketclient.h"
#include "cache.h"

#include <common/itwsclient.h>


namespace twsqapi {

struct PendingHistoricalDataRequest {
    long contractId;
    QString endDateTime;
    QString durationStr;
    QString barSizeSetting;
    QString whatToShow;
    int useRTH;
    int formatDate;
    int reqId;
};

class TwsClient : public common::ITwsClient
{
    Q_OBJECT
public:
    explicit TwsClient(QObject *parent = nullptr);
    ~TwsClient();

    bool connect(const QString& host, int port, int clientId = 0) override;
    bool isConnected() override;

public slots:
    void disconnect() override;
    void startClient(const QString & accountId) override;
    void requestCurrentTime() override;
    void requestManagedAccounts() override;
    void requestOpenOrders() override;
    void requestContractDetails(long contractId, int * reqId = nullptr) override;
    void requestMatchingSymbols(const QString & pattern, int * reqId = nullptr) override;
    void requestHistoricalData(long contractId, const QString & endDateTime,
                               const QString & durationStr, const QString & barSizeSetting,
                               const QString & whatToShow, int useRTH, int formatDate,
                               int * reqId = nullptr) override;
    void requestHistoricalData(const QString & symbol, const QString & endDateTime,
                               const QString & durationStr, const QString & barSizeSetting,
                               const QString & whatToShow, int useRTH, int formatDate,
                               int * reqId = nullptr) override;



private slots:
    void onMessage(std::shared_ptr<ServerMessage> msg);
    void onError(const QString & reason, bool fatal = false);
    void onHandshakeCompleted();
    void onDisconnected();

private:
    struct PendingSymbolHistoricalDataRequest {
        int originalReqId;
        QString symbol;
        QString endDateTime;
        QString durationStr;
        QString barSizeSetting;
        QString whatToShow;
        int useRTH;
        int formatDate;
    };

    QMap<int, PendingSymbolHistoricalDataRequest> _pendingSymbolHistoricalDataRequests;
    QMap<int, QString> _historicalDataDescriptions;
    QMap<int, QTimer*> _historicalDataTimers;

private slots:
    void _onHistoricalDataTimeout(int reqId);

private:
    void _handleManagedAccounts(ManagedAccountsServerMessage *msg);
    void _handleNextValidId(NextValidIdServerMessage *msg);
    void _handleErrorMsg(ErrorServerMessage *msg);
    void _handleAccountValue(AccountValueServerMessage *msg);
    void _handleAccountUpdateTime(AccountUpdateTimeServerMessage *msg);
    void _handleCurrentTime(CurrentTimeServerMessage *msg);
    void _handleSymbolSamples(SymbolSamplesServerMessage *msg);
    void _handleContractData(ContractDataServerMessage *msg);
    void _handleContractDataEnd(ContractDataEndServerMessage *msg);
    void _handlePortfolioValue(PortfolioValueServerMessage *msg);
    void _handleHistoricalData(HistoricalDataServerMessage *msg);
    void _handleHistoricalDataUpdate(HistoricalDataUpdateServerMessage *msg);
    void _handleHistoricalDataEnd(HistoricalDataEndServerMessage *msg);
    void _handleOrderStatus(OrderStatusServerMessage *msg);
    void _handleOpenOrder(OpenOrderServerMessage *msg);
    void _handleOpenOrderEnd(OpenOrderEndServerMessage *msg);

    void _sendHistoricalDataRequest(const PendingHistoricalDataRequest &pending,
                                     const common::ContractDetailsDTO &contractDetails);
    QString _securityTypeToIbString(common::SecurityType type) const;

private:
    SocketClient * _client;
    QString _accountId;
    long _nextOrderId;
    Cache _cache;
    unsigned int _requestId;
    QMap<long, QList<PendingHistoricalDataRequest>> _pendingHistoricalDataRequests;
    QList<common::OrderDTO> _pendingOpenOrders;
    int _openOrdersReqId = 0;
};

}

#endif