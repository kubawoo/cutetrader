#include "test_datamanager.h"
#include <dbbuilder.h>
#include <datamanager.h>
#include "../testrunner/helper.h"

using namespace data;

void DataManagerTest::testSecurities()
{
    auto securities = _manager.getAllSecurities();
    QCOMPARE(securities.size(), 0);

    Security spy = Security().withSymbol("SPY");
    Security tlt = Security().withSymbol("TLT");

    bool ok = _manager.createSecurity(spy);
    QVERIFY(ok);

    ok = _manager.createSecurity(tlt);
    QVERIFY(ok);

    securities = _manager.getAllSecurities();
    QCOMPARE(securities.size(), 2);
    QCOMPARE(securities[0].id(), 1);
    QCOMPARE(securities[0].symbol(), "SPY");
    QCOMPARE(securities[1].id(), 2);
    QCOMPARE(securities[1].symbol(), "TLT");

    spy = _manager.getSecurity(1);
    QCOMPARE(spy.id(), 1);
    QCOMPARE(spy.symbol(), "SPY");

    tlt = _manager.getSecurity("TLT");
    QCOMPARE(tlt.id(), 2);
    QCOMPARE(tlt.symbol(), "TLT");

    ok = _manager.removeSecurity(2);
    QVERIFY(ok);

    securities = _manager.getAllSecurities();
    QCOMPARE(securities.size(), 1);
    QCOMPARE(securities[0].id(), 1);
    QCOMPARE(securities[0].symbol(), "SPY");

    Security iwm = Security().withSymbol("IWM");

    ok = _manager.createSecurity(iwm);
    QVERIFY(ok);

    securities = _manager.getAllSecurities();
    QCOMPARE(securities.size(), 2);
    QCOMPARE(securities[0].id(), 2);
    QCOMPARE(securities[0].symbol(), "IWM");
    QCOMPARE(securities[1].id(), 1);
    QCOMPARE(securities[1].symbol(), "SPY");

    ok = _manager.createSecurity(spy);
    QVERIFY(!ok);
    securities = _manager.getAllSecurities();
    QCOMPARE(securities.size(), 2);
    QCOMPARE(securities[0].id(), 2);
    QCOMPARE(securities[0].symbol(), "IWM");
    QCOMPARE(securities[1].id(), 1);
    QCOMPARE(securities[1].symbol(), "SPY");
}

void DataManagerTest::testQuotes()
{
    Security spy = Security().withSymbol("SPY");

    bool ok = _manager.createSecurity(spy);
    QVERIFY(ok);

    spy = _manager.getSecurity(1);
    QCOMPARE(spy.id(), 1);
    QCOMPARE(spy.symbol(), "SPY");

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

    ok = _manager.removeSecurity(spy.id());
    QVERIFY(ok);

    quotes = _manager.getQuotes(spy);
    QCOMPARE(quotes.size(), 0);

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


