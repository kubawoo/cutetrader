#include "test_servermessage.h"
#include <twsqapi/servermessage.h>
#include <twsqapi/constants.h>
#include <QTimeZone>
#include <QSharedPointer>

void ServerMessageTest::testServerMessageFactory()
{
    twsqapi::ServerMessageFactory factory;
    QVERIFY(factory.create({}).isNull());
    QVERIFY(factory.create({QString::number(9999999999)}).isNull());
    QVERIFY(factory.create({"123a"}).isNull());
}

void ServerMessageTest::testManagedAccountsMessage()
{
    twsqapi::ServerMessageFactory factory;
    QStringList fields = {QString::number(twsqapi::responses::MANAGED_ACCOUNTS),
                          QString::number(twsqapi::responses::MANAGED_ACCOUNTS_VERSION),
                          "U12345678"};
    auto sm = factory.create(fields);
    QVERIFY(!sm.isNull());
    auto msg = sm.dynamicCast<twsqapi::ManagedAccountsServerMessage>();
    QVERIFY(msg);
    QVERIFY(msg->accounts().size() == 1);
    QVERIFY(msg->accounts()[0] == "U12345678");

    fields = {QString::number(twsqapi::responses::MANAGED_ACCOUNTS),
              QString::number(twsqapi::responses::MANAGED_ACCOUNTS_VERSION),
              "U12345678, U87654321"};
    sm = factory.create(fields);
    QVERIFY(!sm.isNull());
    msg = sm.dynamicCast<twsqapi::ManagedAccountsServerMessage>();
    QVERIFY(msg);
    QVERIFY(msg->accounts().size() == 2);
    QVERIFY(msg->accounts()[0] == "U12345678");
    QVERIFY(msg->accounts()[1] == "U87654321");
}

void ServerMessageTest::testInvalidManagedAccountsMessage()
{
    twsqapi::ServerMessageFactory factory;
    QStringList fields = {QString::number(twsqapi::responses::MANAGED_ACCOUNTS),
                          QString::number(twsqapi::responses::MANAGED_ACCOUNTS_VERSION - 1),
                          "U12345678"};
    QVERIFY(factory.create(fields).isNull());

    fields = {QString::number(twsqapi::responses::MANAGED_ACCOUNTS),
              QString::number(twsqapi::responses::MANAGED_ACCOUNTS_VERSION),
              ""};
    QVERIFY(factory.create(fields).isNull());

    fields = {QString::number(twsqapi::responses::MANAGED_ACCOUNTS),
              QString::number(twsqapi::responses::MANAGED_ACCOUNTS_VERSION)};
    QVERIFY(factory.create(fields).isNull());

    fields = {QString::number(twsqapi::responses::MANAGED_ACCOUNTS),
              QString::number(twsqapi::responses::MANAGED_ACCOUNTS_VERSION),
              "U12345678,"};
    QVERIFY(factory.create(fields).isNull());
}

void ServerMessageTest::testNextValidIdMessage()
{
    twsqapi::ServerMessageFactory factory;
    QStringList fields = {QString::number(twsqapi::responses::NEXT_VALID_ID),
                          QString::number(twsqapi::responses::NEXT_VALID_ID_VERSION),
                          QString::number(123)};

    auto sm = factory.create(fields);
    QVERIFY(!sm.isNull());
    auto msg = sm.dynamicCast<twsqapi::NextValidIdServerMessage>();
    QVERIFY(msg);
    QVERIFY(msg->nextId() == 123);
}

void ServerMessageTest::testInvalidNextValidIdMessage()
{
    twsqapi::ServerMessageFactory factory;
    QStringList fields = {QString::number(twsqapi::responses::NEXT_VALID_ID),
                          QString::number(twsqapi::responses::NEXT_VALID_ID_VERSION - 1),
                          QString::number(123)};
    QVERIFY(factory.create(fields).isNull());

    fields = {QString::number(twsqapi::responses::NEXT_VALID_ID),
              QString::number(twsqapi::responses::NEXT_VALID_ID_VERSION)};
    QVERIFY(factory.create(fields).isNull());

    fields = {QString::number(twsqapi::responses::NEXT_VALID_ID),
              QString::number(twsqapi::responses::NEXT_VALID_ID_VERSION),
              "123a"};
    QVERIFY(factory.create(fields).isNull());

    fields = {QString::number(twsqapi::responses::NEXT_VALID_ID),
              QString::number(twsqapi::responses::NEXT_VALID_ID_VERSION),
              QString::number(123),
              QString::number(123)};
    QVERIFY(factory.create(fields).isNull());
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
    auto sm = factory.create(fields);
    QVERIFY(!sm.isNull());
    auto msg = sm.dynamicCast<twsqapi::ErrorServerMessage>();
    QVERIFY(msg);
    QDateTime expected(QDate(2025, 9, 28), QTime(17, 2, 27), QTimeZone::UTC);
    QCOMPARE(msg->errorTime(), expected);
    QCOMPARE(msg->errorCode(), 2104);
    QCOMPARE(msg->errorId(), -1);
    QCOMPARE(msg->errorMsg(), "Market data farm connection is OK");
    QCOMPARE(msg->errorDetails(), "no details");
}

void ServerMessageTest::testInvalidErrorMessage()
{
    twsqapi::ServerMessageFactory factory;
    QStringList fields = {QString::number(twsqapi::responses::ERR_MSG)};
    QVERIFY(factory.create(fields).isNull());

    fields = {QString::number(twsqapi::responses::ERR_MSG),
              "123a",
              "321",
              "msg"
              "",
              "1759078947"};
    QVERIFY(factory.create(fields).isNull());

    fields = {QString::number(twsqapi::responses::ERR_MSG),
              "123",
              "321a",
              "msg"
              "",
              "1759078947"};
    QVERIFY(factory.create(fields).isNull());

    fields = {QString::number(twsqapi::responses::ERR_MSG),
              "123",
              "321",
              "msg"
              "",
              "1759078947a"};
    QVERIFY(factory.create(fields).isNull());
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
    auto sm = factory.create(fields);
    QVERIFY(!sm.isNull());
    auto msg = sm.dynamicCast<twsqapi::AccountValueServerMessage>();
    QVERIFY(msg);
    QCOMPARE(msg->key(), "key");
    QCOMPARE(msg->value(), "value");
    QCOMPARE(msg->currency(), "USD");
    QCOMPARE(msg->accountName(), "U12345678");
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
    QVERIFY(factory.create(fields).isNull());

    fields = {QString::number(twsqapi::responses::ACCT_VALUE),
              QString::number(twsqapi::responses::ACCT_VALUE_VERSION),
              "key",
              "value",
              "USD"};
    QVERIFY(factory.create(fields).isNull());

    fields = {QString::number(twsqapi::responses::ACCT_VALUE), "123a", "key", "value", "USD", "U12345678"};
    QVERIFY(factory.create(fields).isNull());
}

void ServerMessageTest::testCurrentTimeMessage()
{
    twsqapi::ServerMessageFactory factory;
    QStringList fields = {
        QString::number(twsqapi::responses::CURRENT_TIME),
        QString::number(twsqapi::responses::CURRENT_TIME_VERSION),
        "1759078947"
    };
    auto sm = factory.create(fields);
    QVERIFY(!sm.isNull());
    auto msg = sm.dynamicCast<twsqapi::CurrentTimeServerMessage>();
    QVERIFY(msg);
    QVERIFY(msg->dateTime().isValid());
    QDateTime expected(QDate(2025, 9, 28), QTime(17, 2, 27), QTimeZone::UTC);
    QCOMPARE(msg->dateTime(), expected);
}


void ServerMessageTest::testInvalidCurrentTimeMessage()
{
    twsqapi::ServerMessageFactory factory;
    QStringList fields = {
        QString::number(twsqapi::responses::CURRENT_TIME),
        QString::number(twsqapi::responses::CURRENT_TIME_VERSION-1),
        "1759078947"
    };
    QVERIFY(factory.create(fields).isNull());

    fields = {
        QString::number(twsqapi::responses::CURRENT_TIME),
        QString::number(twsqapi::responses::CURRENT_TIME_VERSION),
        "1234abc"
    };
    QVERIFY(factory.create(fields).isNull());

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

    auto sm = factory.create(fields);
    QVERIFY(!sm.isNull());
    auto msg = sm.dynamicCast<twsqapi::SymbolSamplesServerMessage>();

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
    QVERIFY(factory.create(fields).isNull());

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
    QVERIFY(factory.create(fields).isNull());

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
    QVERIFY(factory.create(fields).isNull());

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
    QVERIFY(factory.create(fields).isNull());

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
    QVERIFY(factory.create(fields).isNull());

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
    QVERIFY(factory.create(fields).isNull());

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
    QVERIFY(factory.create(fields).isNull());
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

    auto sm = factory.create(fields);
    QVERIFY(!sm.isNull());
    auto msg = sm.dynamicCast<twsqapi::ContractDataServerMessage>();

    QVERIFY(msg);
    QCOMPARE(msg->requestId(), 123);
    const common::ContractDetailsDTO &contract = msg->contract();
    QCOMPARE(contract.contractId, static_cast<long>(1));
    QCOMPARE(contract.symbol, "AAPL");
    QCOMPARE(contract.securityType, common::SecurityType::STOCK);
    QCOMPARE(contract.primaryExchange, "NASDAQ");
    QCOMPARE(contract.currency, "USD");
    QCOMPARE(contract.description, "Apple Inc.");
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
    QVERIFY(factory.create(fields).isNull());

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
    QVERIFY(factory.create(fields).isNull());

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
    QVERIFY(factory.create(fields).isNull());

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
    QVERIFY(factory.create(fields).isNull());
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

    auto sm = factory.create(fields);
    QVERIFY(!sm.isNull());
    auto msg = sm.dynamicCast<twsqapi::ContractDataEndServerMessage>();

    QVERIFY(msg);
    QCOMPARE(msg->requestId(), 123);
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
    QVERIFY(factory.create(fields).isNull());

    // Invalid request id test case (non-numeric)
    fields = {
        QString::number(twsqapi::responses::CONTRACT_DATA_END),
        QString::number(twsqapi::responses::CONTRACT_DATA_END_VERSION),
        "abc" // request id
    };
    QVERIFY(factory.create(fields).isNull());
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

    auto sm = factory.create(fields);
    QVERIFY(!sm.isNull());
    auto msg = sm.dynamicCast<twsqapi::PortfolioValueServerMessage>();

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
    QVERIFY(factory.create(fields).isNull());

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
    QVERIFY(factory.create(fields).isNull());

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
    QVERIFY(factory.create(fields).isNull());

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
    QVERIFY(factory.create(fields).isNull());

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
    QVERIFY(factory.create(fields).isNull());

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
    QVERIFY(factory.create(fields).isNull());

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
    QVERIFY(factory.create(fields).isNull());

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
    QVERIFY(factory.create(fields).isNull());

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
    QVERIFY(factory.create(fields).isNull());

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
    QVERIFY(factory.create(fields).isNull());
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

    auto sm = factory.create(fields);
    QVERIFY(!sm.isNull());
    auto msg = sm.dynamicCast<twsqapi::AccountUpdateTimeServerMessage>();

    QVERIFY(msg);
    QCOMPARE(msg->time().hour(), 14);
    QCOMPARE(msg->time().minute(), 30);
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
    QVERIFY(factory.create(fields).isNull());

    // Missing time field
    fields = {
        QString::number(twsqapi::responses::ACCT_UPDATE_TIME),
        QString::number(twsqapi::responses::ACCT_UPDATE_TIME_VERSION)
    };
    QVERIFY(factory.create(fields).isNull());
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

    auto sm = factory.create(fields);
    QVERIFY(!sm.isNull());
    auto msg = sm.dynamicCast<twsqapi::AccountDownloadEndServerMessage>();

    QVERIFY(msg);
    QCOMPARE(msg->account(), "U12345678");
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
    QVERIFY(factory.create(fields).isNull());

    // Missing account field
    fields = {
        QString::number(twsqapi::responses::ACCT_DOWNLOAD_END),
        QString::number(twsqapi::responses::ACCT_DOWNLOAD_END_VERSION)
    };
    QVERIFY(factory.create(fields).isNull());
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

    auto sm = factory.create(fields);
    QVERIFY(!sm.isNull());
    auto msg = sm.dynamicCast<twsqapi::HistoricalDataServerMessage>();

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
    QVERIFY(factory.create(fields).isNull());

    // Non-numeric itemCount
    fields = {
        QString::number(twsqapi::responses::HISTORICAL_DATA),
        QString::number(100),   // reqId
        "abc",                // itemCount
    };
    QVERIFY(factory.create(fields).isNull());
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

    auto sm = factory.create(fields);
    QVERIFY(!sm.isNull());
    auto msg = sm.dynamicCast<twsqapi::HistoricalDataEndServerMessage>();

    QVERIFY(msg);
    QCOMPARE(msg->requestId(), 100);
    QCOMPARE(msg->startDate(), "20230101");
    QCOMPARE(msg->endDate(), "20230131");
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
    QVERIFY(factory.create(fields).isNull());
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

    auto sm = factory.create(fields);
    QVERIFY(!sm.isNull());
    auto msg = sm.dynamicCast<twsqapi::HistoricalDataUpdateServerMessage>();

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
    QVERIFY(factory.create(fields).isNull());

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
    QVERIFY(factory.create(fields).isNull());
}

void ServerMessageTest::testOpenOrderMessage()
{
    twsqapi::ServerMessageFactory factory;

    // Simplified OPEN_ORDER message matching the parser's expected format.
    // Field indices (after msgId=5 at index 0):
    // [1] orderId, [2] contractId, [3] symbol, [4] securityType,
    // [5] expiration, [6-8] padding, [9] currency, [10] localSymbol,
    // [11] padding, [12] action, [13] totalQuantity, [14] orderType,
    // [15] lmtPrice, [16] auxPrice, [17] tif, [18] padding, [19] account
    QStringList fields = {QString::number(twsqapi::responses::OPEN_ORDER),
        QString("-2"),                    // [1] orderId
        QString("814279626"),             // [2] contractId
        QString("F"),                     // [3] symbol
        QString("OPT"),                   // [4] securityType
        QString("20280121"),              // [5] expiration
        QString(""), QString(""), QString(""), QString(""), // [6][7][8][9] padding
        QString("USD"),                   // [10] currency
        QString("F     280121C00015000"), // [11] localSymbol
        QString(""),                      // [12] padding
        QString("BUY"),                   // [13] action
        QString("1"),                     // [14] totalQuantity
        QString("LMT"),                   // [15] orderType
        QString("0.05"),                  // [16] lmtPrice
        QString("0.0"),                   // [17] auxPrice
        QString("GTC"),                   // [18] tif
        QString(""),                      // [19] padding
        QString("DUE062120")              // [20] account
    };

    auto sm = factory.create(fields);
    QVERIFY(!sm.isNull());
    auto msg = sm.dynamicCast<twsqapi::OpenOrderServerMessage>();

    QVERIFY(msg);
    const common::OrderDTO &order = msg->order();
    QCOMPARE(order.orderId, -2);
    QCOMPARE(order.contractId, static_cast<long>(814279626));
    QCOMPARE(order.symbol, QString("F"));
    QCOMPARE(order.securityType, common::SecurityType::OPTION);
    QCOMPARE(order.currency, QString("USD"));
    QCOMPARE(order.action, QString("BUY"));
    QCOMPARE(order.totalQuantity, 1.0);
    QCOMPARE(order.orderType, QString("LMT"));
    QCOMPARE(order.lmtPrice, 0.05);
    QCOMPARE(order.auxPrice, 0.0);
    QCOMPARE(order.tif, QString("GTC"));
    QCOMPARE(order.account, QString("DUE062120"));
}

void ServerMessageTest::testInvalidOpenOrderMessage()
{
    twsqapi::ServerMessageFactory factory;

    // Missing fields after the message id
    QStringList fields = {QString::number(twsqapi::responses::OPEN_ORDER)};
    QVERIFY(factory.create(fields).isNull());

    // Non-numeric order id
    fields = {QString::number(twsqapi::responses::OPEN_ORDER), "abc"};
    QVERIFY(factory.create(fields).isNull());
}

void ServerMessageTest::testOrderStatusMessage()
{
    twsqapi::ServerMessageFactory factory;

    // Captured ORDER_STATUS message (server version 198, >= 131 so no version
    // field and mktCapPrice is present).
    QStringList fields = {
        QString::number(twsqapi::responses::ORDER_STATUS),
        "-2",            // orderId
        "Submitted",     // status
        "0",             // filled
        "1",             // remaining
        "0",             // avgFillPrice
        "1532091745",    // permId
        "0",             // parentId
        "0",             // lastFillPrice
        "0",             // clientId
        "",              // whyHeld
        "0"              // mktCapPrice
    };

    auto sm = factory.create(fields);
    QVERIFY(!sm.isNull());
    auto msg = sm.dynamicCast<twsqapi::OrderStatusServerMessage>();

    QVERIFY(msg);
    const common::OrderStatusDTO &status = msg->orderStatus();
    QCOMPARE(status.orderId, -2);
    QCOMPARE(status.status, QString("Submitted"));
    QCOMPARE(status.filled, 0.0);
    QCOMPARE(status.remaining, 1.0);
    QCOMPARE(status.avgFillPrice, 0.0);
    QCOMPARE(status.permId, static_cast<long>(1532091745));
    QCOMPARE(status.parentId, 0);
    QCOMPARE(status.lastFillPrice, 0.0);
    QCOMPARE(status.clientId, 0);
    QCOMPARE(status.whyHeld, QString(""));
    QCOMPARE(status.mktCapPrice, 0.0);
}

void ServerMessageTest::testInvalidOrderStatusMessage()
{
    twsqapi::ServerMessageFactory factory;

    // Missing fields after the message id
    QStringList fields = {QString::number(twsqapi::responses::ORDER_STATUS)};
    QVERIFY(factory.create(fields).isNull());

    // Non-numeric order id
    fields = {QString::number(twsqapi::responses::ORDER_STATUS), "abc"};
    QVERIFY(factory.create(fields).isNull());
}