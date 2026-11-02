#include "test_servermessage.h"
#include <twsqapi/servermessage.h>
#include <twsqapi/constants.h>

void ServerMessageTest::testServerMessageFactory()
{
    twsqapi::ServerMessageFactory factory;
    QVERIFY(!factory.create({}));
    QVERIFY(!factory.create({QString::number(9999999999)}));
    QVERIFY(!factory.create({"123a"}));
}

void ServerMessageTest::testManagedAccountsMessage()
{
    twsqapi::ServerMessageFactory factory;
    QStringList fields = {QString::number(twsqapi::responses::MANAGED_ACCOUNTS),
                          QString::number(twsqapi::responses::MANAGED_ACCOUNTS_VERSION),
                          "U12345678"};
    twsqapi::ServerMessage *sm = factory.create(fields);
    twsqapi::ManagedAccountsServerMessage *msg = dynamic_cast<twsqapi::ManagedAccountsServerMessage *>(sm);
    QVERIFY(msg);
    QVERIFY(msg->accounts().size() == 1);
    QVERIFY(msg->accounts()[0] == "U12345678");
    delete sm;

    fields = {QString::number(twsqapi::responses::MANAGED_ACCOUNTS),
              QString::number(twsqapi::responses::MANAGED_ACCOUNTS_VERSION),
              "U12345678, U87654321"};
    sm = factory.create(fields);
    msg = dynamic_cast<twsqapi::ManagedAccountsServerMessage *>(sm);
    QVERIFY(msg);
    QVERIFY(msg->accounts().size() == 2);
    QVERIFY(msg->accounts()[0] == "U12345678");
    QVERIFY(msg->accounts()[1] == "U87654321");
    delete sm;
}

void ServerMessageTest::testInvalidManagedAccountsMessage()
{
    twsqapi::ServerMessageFactory factory;
    QStringList fields = {QString::number(twsqapi::responses::MANAGED_ACCOUNTS),
                          QString::number(twsqapi::responses::MANAGED_ACCOUNTS_VERSION - 1),
                          "U12345678"};
    QVERIFY(!factory.create(fields));

    fields = {QString::number(twsqapi::responses::MANAGED_ACCOUNTS),
              QString::number(twsqapi::responses::MANAGED_ACCOUNTS_VERSION),
              ""};
    QVERIFY(!factory.create(fields));

    fields = {QString::number(twsqapi::responses::MANAGED_ACCOUNTS),
              QString::number(twsqapi::responses::MANAGED_ACCOUNTS_VERSION)};
    QVERIFY(!factory.create(fields));

    fields = {QString::number(twsqapi::responses::MANAGED_ACCOUNTS),
              QString::number(twsqapi::responses::MANAGED_ACCOUNTS_VERSION),
              "U12345678,"};
    QVERIFY(!factory.create(fields));
}

void ServerMessageTest::testNextValidIdMessage()
{
    twsqapi::ServerMessageFactory factory;
    QStringList fields = {QString::number(twsqapi::responses::NEXT_VALID_ID),
                          QString::number(twsqapi::responses::NEXT_VALID_ID_VERSION),
                          QString::number(123)};

    twsqapi::ServerMessage *sm = factory.create(fields);
    twsqapi::NextValidIdServerMessage *msg = dynamic_cast<twsqapi::NextValidIdServerMessage *>(sm);
    QVERIFY(msg);
    QVERIFY(msg->nextId() == 123);
    delete sm;
}

void ServerMessageTest::testInvalidNextValidIdMessage()
{
    twsqapi::ServerMessageFactory factory;
    QStringList fields = {QString::number(twsqapi::responses::NEXT_VALID_ID),
                          QString::number(twsqapi::responses::NEXT_VALID_ID_VERSION - 1),
                          QString::number(123)};
    QVERIFY(!factory.create(fields));

    fields = {QString::number(twsqapi::responses::NEXT_VALID_ID),
              QString::number(twsqapi::responses::NEXT_VALID_ID_VERSION)};
    QVERIFY(!factory.create(fields));

    fields = {QString::number(twsqapi::responses::NEXT_VALID_ID),
              QString::number(twsqapi::responses::NEXT_VALID_ID_VERSION),
              "123a"};
    QVERIFY(!factory.create(fields));

    fields = {QString::number(twsqapi::responses::NEXT_VALID_ID),
              QString::number(twsqapi::responses::NEXT_VALID_ID_VERSION),
              QString::number(123),
              QString::number(123)};
    QVERIFY(!factory.create(fields));
}

void ServerMessageTest::testErrorMessage()
{
    twsqapi::ServerMessageFactory factory;
    QStringList fields = {QString::number(twsqapi::responses::ERR_MSG),
                          QString::number(-1),
                          QString::number(2104),
                          "Market data farm connection is OK",
                          "no details",
                          "1759078947"};
    twsqapi::ServerMessage *sm = factory.create(fields);
    twsqapi::ErrorServerMessage *msg = dynamic_cast<twsqapi::ErrorServerMessage *>(sm);
    QVERIFY(msg);
    QDateTime expected(QDate(2025, 9, 28), QTime(19, 2, 27));
    QCOMPARE(msg->errorTime(), expected);
    QCOMPARE(msg->errorCode(), 2104);
    QCOMPARE(msg->errorId(), -1);
    QCOMPARE(msg->errorMsg(), "Market data farm connection is OK");
    QCOMPARE(msg->errorDetails(), "no details");
    delete sm;
}

void ServerMessageTest::testInvalidErrorMessage()
{
    twsqapi::ServerMessageFactory factory;
    QStringList fields = {QString::number(twsqapi::responses::ERR_MSG)};
    QVERIFY(!factory.create(fields));

    fields = {QString::number(twsqapi::responses::ERR_MSG),
              "123a",
              "321",
              "msg"
              "",
              "1759078947"};
    QVERIFY(!factory.create(fields));

    fields = {QString::number(twsqapi::responses::ERR_MSG),
              "123",
              "321a",
              "msg"
              "",
              "1759078947"};
    QVERIFY(!factory.create(fields));

    fields = {QString::number(twsqapi::responses::ERR_MSG),
              "123",
              "321",
              "msg"
              "",
              "1759078947a"};
    QVERIFY(!factory.create(fields));
}

void ServerMessageTest::testAccountValueMessage()
{
    twsqapi::ServerMessageFactory factory;
    QStringList fields = {QString::number(twsqapi::responses::ACCT_VALUE),
                          QString::number(twsqapi::responses::ACCT_VALUE_VERSION),
                          "key",
                          "value",
                          "USD",
                          "U12345678"};
    twsqapi::ServerMessage *sm = factory.create(fields);
    twsqapi::AccountValueServerMessage *msg = dynamic_cast<twsqapi::AccountValueServerMessage *>(sm);
    QVERIFY(msg);
    QCOMPARE(msg->key(), "key");
    QCOMPARE(msg->value(), "value");
    QCOMPARE(msg->currency(), "USD");
    QCOMPARE(msg->accountName(), "U12345678");
    delete sm;
}

void ServerMessageTest::testInvalidAccountValueMessage()
{
    twsqapi::ServerMessageFactory factory;
    QStringList fields = {QString::number(twsqapi::responses::ACCT_VALUE),
                          QString::number(twsqapi::responses::ACCT_VALUE_VERSION - 1),
                          "key",
                          "value",
                          "USD",
                          "U12345678"};
    QVERIFY(!factory.create(fields));

    fields = {QString::number(twsqapi::responses::ACCT_VALUE),
              QString::number(twsqapi::responses::ACCT_VALUE_VERSION),
              "key",
              "value",
              "USD"};
    QVERIFY(!factory.create(fields));

    fields = {QString::number(twsqapi::responses::ACCT_VALUE), "123a", "key", "value", "USD", "U12345678"};
    QVERIFY(!factory.create(fields));
}

void ServerMessageTest::testCurrentTimeMessage()
{
    twsqapi::ServerMessageFactory factory;
    QStringList fields = {
        QString::number(twsqapi::responses::CURRENT_TIME),
        QString::number(twsqapi::responses::CURRENT_TIME_VERSION),
        "1759078947"
    };
    twsqapi::ServerMessage* sm = factory.create(fields);
    twsqapi::CurrentTimeServerMessage *msg = dynamic_cast<twsqapi::CurrentTimeServerMessage*>(sm);
    QVERIFY(msg);
    QVERIFY(msg->dateTime().isValid());
    QDateTime expected(QDate(2025, 9, 28), QTime(19, 2, 27));
    QCOMPARE(msg->dateTime(), expected);
    delete sm;
}


void ServerMessageTest::testInvalidCurrentTimeMessage()
{
    twsqapi::ServerMessageFactory factory;
    QStringList fields = {
        QString::number(twsqapi::responses::CURRENT_TIME),
        QString::number(twsqapi::responses::CURRENT_TIME_VERSION-1),
        "1759078947"
    };
    QVERIFY(!factory.create(fields));

    fields = {
        QString::number(twsqapi::responses::CURRENT_TIME),
        QString::number(twsqapi::responses::CURRENT_TIME_VERSION),
        "1234abc"
    };
    QVERIFY(!factory.create(fields));

}

void ServerMessageTest::testSymbolSamplesServerMessage()
{
    twsqapi::ServerMessageFactory factory;

    // Valid input test case
    QStringList fields = {QString::number(twsqapi::responses::SYMBOL_SAMPLES),
                          QString::number(123), // request id
                          QString::number(2),   // count of contracts

                          // First contract
                          "1",          // contract id
                          "AAPL",       // symbol
                          "STK",        // security type
                          "NASDAQ",     // primary exchange
                          "USD",        // currency
                          "0",          // derivatives count (none)
                          "Apple Inc.", // description
                          "12345",      // issuer id

                          // Second contract with OPT derivative
                          "2",      // contract id
                          "MSFT",   // symbol
                          "STK",    // security type
                          "NASDAQ", // primary exchange
                          "USD",    // currency
                          "1",      // derivatives count (one)
                          "OPT",    // derivative type
                          "Microsoft Inc.",
                          "54321"};

    twsqapi::ServerMessage *sm = factory.create(fields);
    twsqapi::SymbolSamplesServerMessage *msg = dynamic_cast<twsqapi::SymbolSamplesServerMessage *>(sm);

    QVERIFY(msg);
    QCOMPARE(msg->requestId(), 123);
    QCOMPARE(msg->contracts().size(), 2);

    // First contract
    const common::ContractDetailsDTO &contract1 = msg->contracts()[0];
    QCOMPARE(contract1.contractId, static_cast<long>(1));
    QCOMPARE(contract1.symbol, "AAPL");
    QCOMPARE(contract1.securityType, common::SecurityType::STOCK);
    QCOMPARE(contract1.primaryExchange, "NASDAQ");
    QCOMPARE(contract1.currency, "USD");
    QVERIFY(contract1.derivatives.isEmpty());
    QCOMPARE(contract1.description, "Apple Inc.");
    QCOMPARE(contract1.issuerId, "12345");

    // Second contract with OPT derivative
    const common::ContractDetailsDTO &contract2 = msg->contracts()[1];
    QCOMPARE(contract2.contractId, static_cast<long>(2));
    QCOMPARE(contract2.symbol, "MSFT");
    QCOMPARE(contract2.securityType, common::SecurityType::STOCK);
    QCOMPARE(contract2.primaryExchange, "NASDAQ");
    QCOMPARE(contract2.currency, "USD");
    QCOMPARE(contract2.derivatives.size(), 1);
    QCOMPARE(contract2.derivatives[0], common::SecurityType::OPTION);
    QCOMPARE(contract2.description, "Microsoft Inc.");
    QCOMPARE(contract2.issuerId, "54321");

    // Clean up
    delete sm;
}

void ServerMessageTest::testInvalidSymbolSamplesServerMessage()
{
    twsqapi::ServerMessageFactory factory;

    // Invalid request id (non-numeric)
    QStringList fields = {
        QString::number(twsqapi::responses::SYMBOL_SAMPLES),
        "abc",              // request id
        QString::number(1), // count of contracts
        "1",                // contract id
        "AAPL",             // symbol
        QString("STK"),     // security type
        "NASDAQ",           // primary exchange
        "USD",              // currency
        "0",                // derivatives count (none)
        "Apple Inc.",       // description
        "12345"             // issuer id
    };
    QVERIFY(!factory.create(fields));

    // Invalid count of contracts (non-numeric)
    fields = {
        QString::number(twsqapi::responses::SYMBOL_SAMPLES),
        QString::number(123), // request id
        "abc",                // count of contracts
        "1",                  // contract id
        "AAPL",               // symbol
        QString("STK"),       // security type
        "NASDAQ",             // primary exchange
        "USD",                // currency
        "0",                  // derivatives count (none)
        "Apple Inc.",         // description
        "12345"               // issuer id
    };
    QVERIFY(!factory.create(fields));

    // Invalid contract id (non-numeric)
    fields = {
        QString::number(twsqapi::responses::SYMBOL_SAMPLES),
        QString::number(123), // request id
        QString::number(1),   // count of contracts
        "abc",                // contract id
        "AAPL",               // symbol
        QString("STK"),       // security type
        "NASDAQ",             // primary exchange
        "USD",                // currency
        "0",                  // derivatives count (none)
        "Apple Inc.",         // description
        "12345"               // issuer id
    };
    QVERIFY(!factory.create(fields));

    // Invalid security type
    fields = {
        QString::number(twsqapi::responses::SYMBOL_SAMPLES),
        QString::number(123), // request id
        QString::number(1),   // count of contracts
        "1",                  // contract id
        "AAPL",               // symbol
        "abc",                // security type
        "NASDAQ",             // primary exchange
        "USD",                // currency
        "0",                  // derivatives count (none)
        "Apple Inc.",         // description
        "12345"               // issuer id
    };
    QVERIFY(!factory.create(fields));

    // Invalid derivatives count (non-numeric)
    fields = {
        QString::number(twsqapi::responses::SYMBOL_SAMPLES),
        QString::number(123), // request id
        QString::number(1),   // count of contracts
        "1",                  // contract id
        "AAPL",               // symbol
        QString("STK"),       // security type
        "NASDAQ",             // primary exchange
        "USD",                // currency
        "abc",                // derivatives count (none)
        "Apple Inc.",         // description
        "12345"               // issuer id
    };
    QVERIFY(!factory.create(fields));

    // Invalid derivative type (non-numeric)
    fields = {QString::number(twsqapi::responses::SYMBOL_SAMPLES),
              QString::number(123), // request id
              QString::number(1),   // count of contracts
              "1",                  // contract id
              "AAPL",               // symbol
              QString("STK"),       // security type
              "NASDAQ",             // primary exchange
              "USD",                // currency
              QString::number(1),   // derivatives count (one)
              "abc",                // derivative type
              "Apple Inc",
              "issuerId"};
    QVERIFY(!factory.create(fields));

    // missing description and issuer id
    fields = {
        QString::number(twsqapi::responses::SYMBOL_SAMPLES),
        QString::number(123), // request id
        QString::number(1),   // count of contracts
        "1",                  // contract id
        "AAPL",               // symbol
        QString("STK"),       // security type
        "NASDAQ",             // primary exchange
        "USD",                // currency
        QString::number(1),   // derivatives count (one)
        "OPT"                 // derivative type
    };
    QVERIFY(!factory.create(fields));
}

void ServerMessageTest::testContractDataServerMessage()
{
    twsqapi::ServerMessageFactory factory;

    // Valid input test case
    QStringList fields = {
        QString::number(twsqapi::responses::CONTRACT_DATA),
        QString::number(123), // request id
        "AAPL",               // symbol (field index 2)
        "STK",                // security type (field index 3)
        "",
        "",
        "",
        "",
        "",
        "USD", // currency (field index 9)
        "",
        "",
        "",
        QString::number(1), // contract id (field index 13)
        "",
        "",
        "",
        "",
        "",
        "",
        "Apple Inc.", // description (field index 20)
        "NASDAQ"      // primary exchange (field index 21)
    };

    twsqapi::ServerMessage *sm = factory.create(fields);
    twsqapi::ContractDataServerMessage *msg = dynamic_cast<twsqapi::ContractDataServerMessage *>(sm);

    QVERIFY(msg);
    QCOMPARE(msg->requestId(), 123);
    const common::ContractDetailsDTO &contract = msg->contract();
    QCOMPARE(contract.contractId, static_cast<long>(1));
    QCOMPARE(contract.symbol, "AAPL");
    QCOMPARE(contract.securityType, common::SecurityType::STOCK);
    QCOMPARE(contract.primaryExchange, "NASDAQ");
    QCOMPARE(contract.currency, "USD");
    QCOMPARE(contract.description, "Apple Inc.");

    // Clean up
    delete sm;
}

void ServerMessageTest::testInvalidContractDataServerMessage()
{
    twsqapi::ServerMessageFactory factory;

    // Invalid request id (non-numeric)
    QStringList fields = {
        QString::number(twsqapi::responses::CONTRACT_DATA),
        "abc",  // request id
        "AAPL", // symbol (field index 2)
        "STK",  // security type (field index 3)
        "",
        "",
        "",
        "",
        "",
        "USD", // currency (field index 9)
        "",
        "",
        "",
        QString::number(1), // contract id (field index 13)
        "",
        "",
        "",
        "",
        "",
        "",
        "Apple Inc.", // description (field index 20)
        "NASDAQ"      // primary exchange (field index 21)
    };
    QVERIFY(!factory.create(fields));

    // Invalid contract id (non-numeric)
    fields = {
        QString::number(twsqapi::responses::CONTRACT_DATA),
        QString::number(123), // request id
        "AAPL",               // symbol (field index 2)
        "STK",                // security type (field index 3)
        "",
        "",
        "",
        "",
        "",
        "USD", // currency (field index 9)
        "",
        "",
        "",
        "abc", // contract id (field index 13)
        "",
        "",
        "",
        "",
        "",
        "",
        "Apple Inc.", // description (field index 20)
        "NASDAQ"      // primary exchange (field index 21)
    };
    QVERIFY(!factory.create(fields));

    // Invalid primary exchange (non-numeric)
    fields = {QString::number(twsqapi::responses::CONTRACT_DATA),
              QString::number(123), // request id
              "AAPL",
              "STK",
              // Other fields...
              "USD",
              "Apple Inc.",
              "abc", // primary exchange (non-numeric)

              // Contract details
              QString::number(1),
              "AAPL",
              "STK",
              "USD",
              "Apple Inc.",
              "NASDAQ",
              QString::number(1)};
    QVERIFY(!factory.create(fields));

    // Invalid security type
    fields = {
        QString::number(twsqapi::responses::CONTRACT_DATA),
        QString::number(123), // request id
        "AAPL",               // symbol (field index 2)
        "abc",                // security type (field index 3)
        "",
        "",
        "",
        "",
        "",
        "USD", // currency (field index 9)
        "",
        "",
        "",
        QString::number(1), // contract id (field index 13)
        "",
        "",
        "",
        "",
        "",
        "",
        "Apple Inc.", // description (field index 20)
        "NASDAQ"      // primary exchange (field index 21)
    };
    QVERIFY(!factory.create(fields));
}

#include <QDebug>
#include <QStringList>

void ServerMessageTest::testContractDataEndServerMessage()
{
    twsqapi::ServerMessageFactory factory;

    // Valid input test case
    QStringList fields = {
        QString::number(twsqapi::responses::CONTRACT_DATA_END),
        QString::number(twsqapi::responses::CONTRACT_DATA_END_VERSION),
        QString::number(123) // request id
    };

    twsqapi::ServerMessage *sm = factory.create(fields);
    twsqapi::ContractDataEndServerMessage *msg = dynamic_cast<twsqapi::ContractDataEndServerMessage *>(sm);

    QVERIFY(msg);
    QCOMPARE(msg->requestId(), 123);

    // Clean up
    delete sm;
}

void ServerMessageTest::testInvalidContractDataEndServerMessage()
{
    twsqapi::ServerMessageFactory factory;

    // Invalid version test case (non-numeric)
    QStringList fields = {
        QString::number(twsqapi::responses::CONTRACT_DATA_END),
        "abc",               // version
        QString::number(123) // request id
    };
    QVERIFY(!factory.create(fields));

    // Invalid request id test case (non-numeric)
    fields = {
        QString::number(twsqapi::responses::CONTRACT_DATA_END),
        QString::number(twsqapi::responses::CONTRACT_DATA_END_VERSION),
        "abc" // request id
    };
    QVERIFY(!factory.create(fields));
}

void ServerMessageTest::testPortfolioValueServerMessage()
{
    twsqapi::ServerMessageFactory factory;

    // Valid input test case
    QStringList fields = {QString::number(twsqapi::responses::PORTFOLIO_VALUE),
                          QString::number(twsqapi::responses::PORTFOLIO_VALUE_VERSION),
                          "123",          // contract id
                          "AAPL",         // symbol
                          QString("STK"), // security type
                          "20230615",     // expiration (for options)
                          "150.00",       // strike (for options)
                          "C",            // right (for options)
                          "100",          // multiplier (for options)
                          "",
                          "",
                          "",
                          "",
                          QString::number(1), // position
                          "150.00",           // market price
                          "150.00",           // market value
                          "120.00",           // average cost
                          "30.00",            // unrealized PNL
                          "10.00",            // realized PNL
                          ""};

    twsqapi::ServerMessage *sm = factory.create(fields);
    twsqapi::PortfolioValueServerMessage *msg = dynamic_cast<twsqapi::PortfolioValueServerMessage *>(sm);

    QVERIFY(msg);
    const common::PortfolioPositionDTO &position = msg->position();
    QCOMPARE(position.contractId, static_cast<long>(123));
    QCOMPARE(position.symbol, "AAPL");
    QCOMPARE(position.securityType, common::SecurityType::STOCK);
    QCOMPARE(position.position, 1.0);
    QCOMPARE(position.marketPrice, 150.0);
    QCOMPARE(position.marketValue, 150.0);
    QCOMPARE(position.averageCost, 120.0);
    QCOMPARE(position.unrealizedPNL, 30.0);
    QCOMPARE(position.realizedPNL, 10.0);

    // Clean up
    delete sm;
}

void ServerMessageTest::testInvalidPortfolioValueServerMessage()
{
    twsqapi::ServerMessageFactory factory;

    // Invalid contract id (non-numeric)
    QStringList fields = {QString::number(twsqapi::responses::PORTFOLIO_VALUE),
                          QString::number(twsqapi::responses::PORTFOLIO_VALUE_VERSION),
                          "abc",          // contract id
                          "AAPL",         // symbol
                          QString("STK"), // security type
                          "20230615",     // expiration (for options)
                          "150.00",       // strike (for options)
                          "C",            // right (for options)
                          "100",          // multiplier (for options)
                          "",
                          "",
                          "",
                          "",
                          QString::number(1), // position
                          "150.00",           // market price
                          "150.00",           // market value
                          "120.00",           // average cost
                          "30.00",            // unrealized PNL
                          "10.00",            // realized PNL
                          ""};
    QVERIFY(!factory.create(fields));

    // Invalid position (non-numeric)
    fields = {QString::number(twsqapi::responses::PORTFOLIO_VALUE),
              QString::number(twsqapi::responses::PORTFOLIO_VALUE_VERSION),
              "123",          // contract id
              "AAPL",         // symbol
              QString("STK"), // security type
              "20230615",     // expiration (for options)
              "150.00",       // strike (for options)
              "C",            // right (for options)
              "100",          // multiplier (for options)
              "",
              "",
              "",
              "",
              "123a",   // position
              "150.00", // market price
              "150.00", // market value
              "120.00", // average cost
              "30.00",  // unrealized PNL
              "10.00",  // realized PNL
              ""};
    QVERIFY(!factory.create(fields));

    // Invalid market price (non-numeric)
    fields = {QString::number(twsqapi::responses::PORTFOLIO_VALUE),
              QString::number(twsqapi::responses::PORTFOLIO_VALUE_VERSION),
              "123",          // contract id
              "AAPL",         // symbol
              QString("STK"), // security type
              "20230615",     // expiration (for options)
              "150.00",       // strike (for options)
              "C",            // right (for options)
              "100",          // multiplier (for options)
              "",
              "",
              "",
              "",
              QString::number(1), // position
              "abc",              // market price
              "150.00",           // market value
              "120.00",           // average cost
              "30.00",            // unrealized PNL
              "10.00",            // realized PNL
              ""};
    QVERIFY(!factory.create(fields));

    // Invalid market value (non-numeric)
    fields = {QString::number(twsqapi::responses::PORTFOLIO_VALUE),
              QString::number(twsqapi::responses::PORTFOLIO_VALUE_VERSION),
              "123",          // contract id
              "AAPL",         // symbol
              QString("STK"), // security type
              "20230615",     // expiration (for options)
              "150.00",       // strike (for options)
              "C",            // right (for options)
              "100",          // multiplier (for options)
              "",
              "",
              "",
              "",
              QString::number(1), // position
              "150.00",           // market price
              "abc",              // market value
              "120.00",           // average cost
              "30.00",            // unrealized PNL
              "10.00",            // realized PNL
              ""};
    QVERIFY(!factory.create(fields));

    // Invalid average cost (non-numeric)
    fields = {QString::number(twsqapi::responses::PORTFOLIO_VALUE),
              QString::number(twsqapi::responses::PORTFOLIO_VALUE_VERSION),
              "123",          // contract id
              "AAPL",         // symbol
              QString("STK"), // security type
              "20230615",     // expiration (for options)
              "150.00",       // strike (for options)
              "C",            // right (for options)
              "100",          // multiplier (for options)
              "",
              "",
              "",
              "",
              QString::number(1), // position
              "150.00",           // market price
              "150.00",           // market value
              "abc",              // average cost
              "30.00",            // unrealized PNL
              "10.00",            // realized PNL
              ""};
    QVERIFY(!factory.create(fields));

    // Invalid unrealized PNL (non-numeric)
    fields = {QString::number(twsqapi::responses::PORTFOLIO_VALUE),
              QString::number(twsqapi::responses::PORTFOLIO_VALUE_VERSION),
              "123",          // contract id
              "AAPL",         // symbol
              QString("STK"), // security type
              "20230615",     // expiration (for options)
              "150.00",       // strike (for options)
              "C",            // right (for options)
              "100",          // multiplier (for options)
              "",
              "",
              "",
              "",
              QString::number(1), // position
              "150.00",           // market price
              "150.00",           // market value
              "120.00",           // average cost
              "abc",              // unrealized PNL
              "10.00",            // realized PNL
              ""};
    QVERIFY(!factory.create(fields));

    // Invalid realized PNL (non-numeric)
    fields = {QString::number(twsqapi::responses::PORTFOLIO_VALUE),
              QString::number(twsqapi::responses::PORTFOLIO_VALUE_VERSION),
              "123",          // contract id
              "AAPL",         // symbol
              QString("STK"), // security type
              "20230615",     // expiration (for options)
              "150.00",       // strike (for options)
              "C",            // right (for options)
              "100",          // multiplier (for options)
              "",
              "",
              "",
              "",
              QString::number(1), // position
              "150.00",           // market price
              "150.00",           // market value
              "120.00",           // average cost
              "30.00",            // unrealized PNL
              "abc",              // realized PNL
              ""};
    QVERIFY(!factory.create(fields));

    // Invalid expiration date (incorrect format)
    fields = {QString::number(twsqapi::responses::PORTFOLIO_VALUE),
              QString::number(twsqapi::responses::PORTFOLIO_VALUE_VERSION),
              "123",          // contract id
              "AAPL",         // symbol
              QString("OPT"), // security type
              "2023-06-15",   // expiration (for options)
              "150.00",       // strike (for options)
              "C",            // right (for options)
              "100",          // multiplier (for options)
              "",
              "",
              "",
              "",
              QString::number(1), // position
              "150.00",           // market price
              "150.00",           // market value
              "120.00",           // average cost
              "30.00",            // unrealized PNL
              "10.00",            // realized PNL
              ""};
    QVERIFY(!factory.create(fields));

    // Invalid strike price (non-numeric)
    fields = {QString::number(twsqapi::responses::PORTFOLIO_VALUE),
              QString::number(twsqapi::responses::PORTFOLIO_VALUE_VERSION),
              "123",          // contract id
              "AAPL",         // symbol
              QString("OPT"), // security type
              "2023-06-15",   // expiration (for options)
              "abc",          // strike (for options)
              "C",            // right (for options)
              "100",          // multiplier (for options)
              "",
              "",
              "",
              "",
              QString::number(1), // position
              "150.00",           // market price
              "150.00",           // market value
              "120.00",           // average cost
              "30.00",            // unrealized PNL
              "10.00",            // realized PNL
              ""};
    QVERIFY(!factory.create(fields));

    // Invalid multiplier (non-numeric)
    fields = {QString::number(twsqapi::responses::PORTFOLIO_VALUE),
              QString::number(twsqapi::responses::PORTFOLIO_VALUE_VERSION),
              "123",          // contract id
              "AAPL",         // symbol
              QString("OPT"), // security type
              "2023-06-15",   // expiration (for options)
              "150.00",       // strike (for options)
              "C",            // right (for options)
              "a100",         // multiplier (for options)
              "",
              "",
              "",
              "",
              QString::number(1), // position
              "150.00",           // market price
              "150.00",           // market value
              "120.00",           // average cost
              "30.00",            // unrealized PNL
              "10.00",            // realized PNL
              ""};
    QVERIFY(!factory.create(fields));
}

void ServerMessageTest::testAccountUpdateTimeMessage()
{
    twsqapi::ServerMessageFactory factory;

    // Valid input test case
    QStringList fields = {
        QString::number(twsqapi::responses::ACCT_UPDATE_TIME),
        QString::number(twsqapi::responses::ACCT_UPDATE_TIME_VERSION),
        "14:30"
    };

    twsqapi::ServerMessage *sm = factory.create(fields);
    twsqapi::AccountUpdateTimeServerMessage *msg = dynamic_cast<twsqapi::AccountUpdateTimeServerMessage *>(sm);

    QVERIFY(msg);
    QCOMPARE(msg->time().hour(), 14);
    QCOMPARE(msg->time().minute(), 30);

    delete sm;
}

void ServerMessageTest::testInvalidAccountUpdateTimeMessage()
{
    twsqapi::ServerMessageFactory factory;

    // Invalid version
    QStringList fields = {
        QString::number(twsqapi::responses::ACCT_UPDATE_TIME),
        QString::number(twsqapi::responses::ACCT_UPDATE_TIME_VERSION - 1),
        "14:30"
    };
    QVERIFY(!factory.create(fields));

    // Missing time field
    fields = {
        QString::number(twsqapi::responses::ACCT_UPDATE_TIME),
        QString::number(twsqapi::responses::ACCT_UPDATE_TIME_VERSION)
    };
    QVERIFY(!factory.create(fields));
}

void ServerMessageTest::testAccountDownloadEndMessage()
{
    twsqapi::ServerMessageFactory factory;

    // Valid input test case
    QStringList fields = {
        QString::number(twsqapi::responses::ACCT_DOWNLOAD_END),
        QString::number(twsqapi::responses::ACCT_DOWNLOAD_END_VERSION),
        "U12345678"
    };

    twsqapi::ServerMessage *sm = factory.create(fields);
    twsqapi::AccountDownloadEndServerMessage *msg = dynamic_cast<twsqapi::AccountDownloadEndServerMessage *>(sm);

    QVERIFY(msg);
    QCOMPARE(msg->account(), "U12345678");

    delete sm;
}

void ServerMessageTest::testInvalidAccountDownloadEndMessage()
{
    twsqapi::ServerMessageFactory factory;

    // Invalid version
    QStringList fields = {
        QString::number(twsqapi::responses::ACCT_DOWNLOAD_END),
        QString::number(twsqapi::responses::ACCT_DOWNLOAD_END_VERSION - 1),
        "U12345678"
    };
    QVERIFY(!factory.create(fields));

    // Missing account field
    fields = {
        QString::number(twsqapi::responses::ACCT_DOWNLOAD_END),
        QString::number(twsqapi::responses::ACCT_DOWNLOAD_END_VERSION)
    };
    QVERIFY(!factory.create(fields));
}

void ServerMessageTest::testHistoricalDataMessage()
{
    twsqapi::ServerMessageFactory factory;

    // Valid input test case - 2 bars
    QStringList fields = {
        QString::number(twsqapi::responses::HISTORICAL_DATA),
        QString::number(100),       // reqId
        QString::number(2),         // itemCount

        // Bar 1
        "20230101 09:30:00",      // time
        "100.00",                 // open
        "101.00",                 // high
        "99.00",                  // low
        "100.50",                 // close
        "1000",                   // volume
        "100.25",                 // wap
        "1",                      // barCount

        // Bar 2
        "20230101 09:31:00",      // time
        "100.50",                 // open
        "102.00",                 // high
        "100.00",                 // low
        "101.50",                 // close
        "1500",                   // volume
        "100.75",                 // wap
        "2"                       // barCount
    };

    twsqapi::ServerMessage *sm = factory.create(fields);
    twsqapi::HistoricalDataServerMessage *msg = dynamic_cast<twsqapi::HistoricalDataServerMessage *>(sm);

    QVERIFY(msg);
    QCOMPARE(msg->requestId(), 100);
    QCOMPARE(msg->bars().size(), 2);

    const QList<common::BarDTO> &bars = msg->bars();
    QCOMPARE(bars[0].time, "20230101 09:30:00");
    QCOMPARE(bars[0].open, 100.0);
    QCOMPARE(bars[0].high, 101.0);
    QCOMPARE(bars[0].low, 99.0);
    QCOMPARE(bars[0].close, 100.5);
    QCOMPARE(bars[0].volume, 1000.0);
    QCOMPARE(bars[0].wap, 100.25);
    QCOMPARE(bars[0].barCount, 1);

    QCOMPARE(bars[1].time, "20230101 09:31:00");
    QCOMPARE(bars[1].open, 100.5);
    QCOMPARE(bars[1].high, 102.0);
    QCOMPARE(bars[1].low, 100.0);
    QCOMPARE(bars[1].close, 101.5);
    QCOMPARE(bars[1].volume, 1500.0);
    QCOMPARE(bars[1].wap, 100.75);
    QCOMPARE(bars[1].barCount, 2);

    delete sm;
}

void ServerMessageTest::testInvalidHistoricalDataMessage()
{
    twsqapi::ServerMessageFactory factory;

    // Non-numeric reqId
    QStringList fields = {
        QString::number(twsqapi::responses::HISTORICAL_DATA),
        "abc",                  // reqId
        QString::number(1),     // itemCount
        "20230101 09:30:00",    // time
        "100.00",               // open
        "101.00",               // high
        "99.00",                // low
        "100.50",               // close
        "1000",                 // volume
        "100.25",               // wap
        "1"                     // barCount
    };
    QVERIFY(!factory.create(fields));

    // Non-numeric itemCount
    fields = {
        QString::number(twsqapi::responses::HISTORICAL_DATA),
        QString::number(100),   // reqId
        "abc",                // itemCount
    };
    QVERIFY(!factory.create(fields));
}

void ServerMessageTest::testHistoricalDataEndMessage()
{
    twsqapi::ServerMessageFactory factory;

    // Valid input test case
    QStringList fields = {
        QString::number(twsqapi::responses::HISTORICAL_DATA_END),
        QString::number(100),     // reqId
        "20230101",               // startDate
        "20230131"                // endDate
    };

    twsqapi::ServerMessage *sm = factory.create(fields);
    twsqapi::HistoricalDataEndServerMessage *msg = dynamic_cast<twsqapi::HistoricalDataEndServerMessage *>(sm);

    QVERIFY(msg);
    QCOMPARE(msg->requestId(), 100);
    QCOMPARE(msg->startDate(), "20230101");
    QCOMPARE(msg->endDate(), "20230131");

    delete sm;
}

void ServerMessageTest::testInvalidHistoricalDataEndMessage()
{
    twsqapi::ServerMessageFactory factory;

    // Non-numeric reqId
    QStringList fields = {
        QString::number(twsqapi::responses::HISTORICAL_DATA_END),
        "abc",                  // reqId
        "20230101",
        "20230131"
    };
    QVERIFY(!factory.create(fields));

    }

void ServerMessageTest::testHistoricalDataUpdateMessage()
{
    twsqapi::ServerMessageFactory factory;

    // Valid input test case
    QStringList fields = {
        QString::number(twsqapi::responses::HISTORICAL_DATA_UPDATE),
        QString::number(100),     // reqId
        QString::number(1),       // barCount
        "20230101 09:30:00",      // time
        "100.00",                 // open
        "100.50",                 // close
        "101.00",                 // high
        "99.00",                  // low
        "100.25",                 // wap
        "1000"                    // volume
    };

    twsqapi::ServerMessage *sm = factory.create(fields);
    twsqapi::HistoricalDataUpdateServerMessage *msg = dynamic_cast<twsqapi::HistoricalDataUpdateServerMessage *>(sm);

    QVERIFY(msg);
    QCOMPARE(msg->requestId(), 100);
    QCOMPARE(msg->bar().barCount, 1);
    QCOMPARE(msg->bar().time, "20230101 09:30:00");
    QCOMPARE(msg->bar().open, 100.0);
    QCOMPARE(msg->bar().close, 100.5);
    QCOMPARE(msg->bar().high, 101.0);
    QCOMPARE(msg->bar().low, 99.0);
    QCOMPARE(msg->bar().wap, 100.25);
    QCOMPARE(msg->bar().volume, 1000.0);

    delete sm;
}

void ServerMessageTest::testInvalidHistoricalDataUpdateMessage()
{
    twsqapi::ServerMessageFactory factory;

    // Non-numeric reqId
    QStringList fields = {
        QString::number(twsqapi::responses::HISTORICAL_DATA_UPDATE),
        "abc",                  // reqId
        QString::number(1),
        "20230101 09:30:00",
        "100.00",
        "100.50",
        "101.00",
        "99.00",
        "100.25",
        "1000"
    };
    QVERIFY(!factory.create(fields));

    // Non-numeric barCount
    fields = {
        QString::number(twsqapi::responses::HISTORICAL_DATA_UPDATE),
        QString::number(100),
        "abc",                  // barCount
        "20230101 09:30:00",
        "100.00",
        "100.50",
        "101.00",
        "99.00",
        "100.25",
        "1000"
    };
    QVERIFY(!factory.create(fields));
}
