#include "test_datamanager.h"
#include <data/dbbuilder.h>
#include <data/datamanager.h>
#include "../testrunner/helper.h"

using namespace data;

void DataManagerTest::testSecurities()
{
    auto securities = _manager.getAllSecurities();
    QCOMPARE(securities.size(), 0);

    Security spy = Security().withSymbol("SPY").withContractId(123);
    Security tlt = Security().withSymbol("TLT").withContractId(321);

    bool ok = _manager.createSecurity(spy);
    QVERIFY(ok);

    ok = _manager.createSecurity(tlt);
    QVERIFY(ok);

    securities = _manager.getAllSecurities();
    QCOMPARE(securities.size(), 2);
    QCOMPARE(securities[0].id(), 1);
    QCOMPARE(securities[0].symbol(), "SPY");
    QCOMPARE(securities[0].contractId(), 123);
    QCOMPARE(securities[1].id(), 2);
    QCOMPARE(securities[1].symbol(), "TLT");
    QCOMPARE(securities[1].contractId(), 321);


    spy = _manager.getSecurity(1);
    QCOMPARE(spy.id(), 1);
    QCOMPARE(spy.symbol(), "SPY");
    QCOMPARE(spy.contractId(), 123);

    tlt = _manager.getSecurity("TLT");
    QCOMPARE(tlt.id(), 2);
    QCOMPARE(tlt.symbol(), "TLT");
    QCOMPARE(tlt.contractId(), 321);

    ok = _manager.removeSecurity(2);
    QVERIFY(ok);

    securities = _manager.getAllSecurities();
    QCOMPARE(securities.size(), 1);
    QCOMPARE(securities[0].id(), 1);
    QCOMPARE(securities[0].symbol(), "SPY");
    QCOMPARE(securities[0].contractId(), 123);

    Security iwm = Security().withSymbol("IWM").withContractId(456);

    ok = _manager.createSecurity(iwm);
    QVERIFY(ok);

    securities = _manager.getAllSecurities();
    QCOMPARE(securities.size(), 2);
    QCOMPARE(securities[0].id(), 2);
    QCOMPARE(securities[0].symbol(), "IWM");
    QCOMPARE(securities[0].contractId(), 456);
    QCOMPARE(securities[1].id(), 1);
    QCOMPARE(securities[1].symbol(), "SPY");
    QCOMPARE(securities[1].contractId(), 123);

    ok = _manager.createSecurity(spy);
    QVERIFY(!ok);
    securities = _manager.getAllSecurities();
    QCOMPARE(securities.size(), 2);
    QCOMPARE(securities[0].id(), 2);
    QCOMPARE(securities[0].symbol(), "IWM");
    QCOMPARE(securities[0].contractId(), 456);
    QCOMPARE(securities[1].id(), 1);
    QCOMPARE(securities[1].symbol(), "SPY");
    QCOMPARE(securities[1].contractId(), 123);
}

void DataManagerTest::testQuotes()
{
    Security spy = Security().withSymbol("SPY").withContractId(123);

    bool ok = _manager.createSecurity(spy);
    QVERIFY(ok);

    spy = _manager.getSecurity(1);
    QCOMPARE(spy.id(), 1);
    QCOMPARE(spy.symbol(), "SPY");
    QCOMPARE(spy.contractId(), 123);

    Quote q1 = Quote()
            .withSecurityId(1)
            .withDate(QDate(2023,8, 1))
            .withOpen(550.1)
            .withClose(555.2)
            .withHigh(558.3)
            .withLow(545.4)
            .withVolume(1000);

    ok = _manager.createQuote(q1);
    QVERIFY(ok);

    auto quotes = _manager.getQuotes(spy);
    QCOMPARE(quotes.size(), 1);
    QCOMPARE(quotes[0].securityId(), 1);
    kCompare(quotes[0].open(), 550.1, 2);
    kCompare(quotes[0].close(), 555.2, 2);
    kCompare(quotes[0].high(), 558.3, 2);
    kCompare(quotes[0].low(), 545.4, 2);
    QCOMPARE(quotes[0].date(), QDate(2023, 8, 1));
    QCOMPARE(quotes[0].volume(), 1000);


    Quote q2 = Quote()
            .withSecurityId(1)
            .withDate(QDate(2024, 8, 1))
            .withOpen(450.1)
            .withClose(455.2)
            .withHigh(458.3)
            .withLow(445.4)
            .withVolume(4000);

    ok = _manager.createQuote(q2);
    QVERIFY(ok);

    quotes = _manager.getQuotes(spy);
    QCOMPARE(quotes.size(), 2);
    QCOMPARE(quotes[1].securityId(), 1);
    kCompare(quotes[1].open(), 450.1, 2);
    kCompare(quotes[1].close(), 455.2, 2);
    kCompare(quotes[1].high(), 458.3, 2);
    kCompare(quotes[1].low(), 445.4, 2);
    QCOMPARE(quotes[1].date(), QDate(2024, 8, 1));
    QCOMPARE(quotes[1].volume(), 4000);
}

void DataManagerTest::testQuoteWithNoSecurity()
{
    Quote q1 = Quote()
            .withSecurityId(1)
            .withDate(QDate(2023,8, 1))
            .withOpen(550.1)
            .withClose(555.2)
            .withHigh(558.3)
            .withLow(545.4)
            .withVolume(1000);

    bool ok = _manager.createQuote(q1);
    QVERIFY(!ok);
}

void DataManagerTest::testDeleteSecurityWithQuotes()
{
    Security spy = Security().withSymbol("SPY");

    bool ok = _manager.createSecurity(spy);
    QVERIFY(ok);

    spy = _manager.getSecurity(1);
    QCOMPARE(spy.id(), 1);
    QCOMPARE(spy.symbol(), "SPY");

    Quote q1 = Quote()
            .withSecurityId(1)
            .withDate(QDate(2023, 8, 1))
            .withOpen(550.1)
            .withClose(555.2)
            .withHigh(558.3)
            .withLow(545.4)
            .withVolume(1000);

    ok = _manager.createQuote(q1);
    QVERIFY(ok);

    auto quotes = _manager.getQuotes(spy);
    QCOMPARE(quotes.size(), 1);

    ok = _manager.removeSecurity(spy.id());
    QVERIFY(ok);

    quotes = _manager.getQuotes(spy);
    QCOMPARE(quotes.size(), 0);

}

void DataManagerTest::testGetQuotesFrom()
{
    Security spy = Security().withSymbol("SPY");

    bool ok = _manager.createSecurity(spy);
    QVERIFY(ok);

    spy = _manager.getSecurity(1);
    QCOMPARE(spy.id(), 1);
    QCOMPARE(spy.symbol(), "SPY");

    for(int i = 1; i <= 10; i++) {
        Quote qutoe = Quote()
                .withSecurityId(1)
                .withDate(QDate(2023, 8, i))
                .withOpen(550.1)
                .withClose(555.2)
                .withHigh(558.3)
                .withLow(545.4)
                .withVolume(1000);

        ok = _manager.createQuote(qutoe);
        QVERIFY(ok);
    }


    auto quotes = _manager.getQuotes(spy, QDate(2023, 8, 6));
    QCOMPARE(quotes.size(), 5);
    for(int i = 6; i <= 10; i++) {
        QCOMPARE(quotes[i-6].date().day(), i);
    }
}

void DataManagerTest::testGetQuotesFromTo()
{
    Security spy = Security().withSymbol("SPY");

    bool ok = _manager.createSecurity(spy);
    QVERIFY(ok);

    spy = _manager.getSecurity(1);
    QCOMPARE(spy.id(), 1);
    QCOMPARE(spy.symbol(), "SPY");

    for(int i = 1; i <= 10; i++) {
        Quote qutoe = Quote()
                .withSecurityId(1)
                .withDate(QDate(2023, 8, i))
                .withOpen(550.1)
                .withClose(555.2)
                .withHigh(558.3)
                .withLow(545.4)
                .withVolume(1000);

        ok = _manager.createQuote(qutoe);
        QVERIFY(ok);
    }


    auto quotes = _manager.getQuotes(spy, QDate(2023, 8, 6), QDate(2023, 8, 8));
    QCOMPARE(quotes.size(), 2);
    for(int i = 6; i <= 7; i++) {
        QCOMPARE(quotes[i-6].date().day(), i);
    }
}

void DataManagerTest::init()
{
    _db = QSqlDatabase::addDatabase("QSQLITE");
    _db.setDatabaseName(":memory:");
    _db.open();

    data::DbBuilder dbBuilder(_db);
    dbBuilder.runMigrations();
    _manager = data::DataManager(_db);
}

void DataManagerTest::cleanup()
{
    _db.close();
}


