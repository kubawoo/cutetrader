#ifndef TEST_SERVERMESSAGE_H
#define TEST_SERVERMESSAGE_H

#include <QTest>

class ServerMessageTest : public QObject
{
    Q_OBJECT

private slots:
    void testServerMessageFactory();
    void testManagedAccountsMessage();
    void testInvalidManagedAccountsMessage();
    void testNextValidIdMessage();
    void testInvalidNextValidIdMessage();
    void testErrorMessage();
    void testInvalidErrorMessage();
    void testAccountValueMessage();
    void testInvalidAccountValueMessage();
    void testCurrentTimeMessage();
    void testInvalidCurrentTimeMessage();
    void testSymbolSamplesServerMessage();
    void testInvalidSymbolSamplesServerMessage();
    void testContractDataServerMessage();
    void testInvalidContractDataServerMessage();
    void testContractDataEndServerMessage();
    void testInvalidContractDataEndServerMessage();
    void testPortfolioValueServerMessage();
    void testInvalidPortfolioValueServerMessage();
    void testAccountUpdateTimeMessage();
    void testInvalidAccountUpdateTimeMessage();
    void testAccountDownloadEndMessage();
    void testInvalidAccountDownloadEndMessage();
    void testHistoricalDataMessage();
    void testInvalidHistoricalDataMessage();
    void testHistoricalDataEndMessage();
    void testInvalidHistoricalDataEndMessage();
    void testHistoricalDataUpdateMessage();
    void testInvalidHistoricalDataUpdateMessage();
    void testOrderStatusMessage();
    void testInvalidOrderStatusMessage();
    void testOpenOrderMessage();
    void testInvalidOpenOrderMessage();
};

#endif // TEST_SERVERMESSAGE_H
