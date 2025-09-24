#ifndef TWSQAPI_TWSCLIENT_H
#define TWSQAPI_TWSCLIENT_H

#include <QObject>
#include "socketclient.h"
#include "cache.h"

#include <common/itwsclient.h>


namespace twsqapi {

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


private slots:
    void onMessage(QSharedPointer<ServerMessage> msg);
    void onError(const QString & reason, bool fatal = false);

private:
    void _handleManagedAccounts(ManagedAccountsServerMessage *msg);
    void _handleNextValidId(NextValidIdServerMessage *msg);
    void _handleErrorMsg(ErrorServerMessage *msg);
    void _handleAccountValue(AccountValueServerMessage *msg);
    void _handleAccountUpdateTime(AccountUpdateTimeServerMessage *msg);
    void _handleCurrentTime(CurrentTimeServerMessage *msg);
    void _handleSymbolSamples(SymbolSamplesSrverMessage *msg);
    void _handleContractData(ContractDataServerMessage *msg);
    void _handleContractDataEnd(ContractDataEndServerMessage *msg);
    void _handlePortfolioValue(PortfolioValueServerMessage *msg);

private:
    SocketClient * _client;
    QString _accountId;
    long _nextOrderId;
    Cache _cache;
    unsigned int _requestId;
};

}

#endif
