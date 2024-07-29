#include "test_datamanager.h"
#include <dbbuilder.h>
#include <datamanager.h>

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
    QCOMPARE(securities[0].id(), 1);
    QCOMPARE(securities[0].symbol(), "SPY");
    QCOMPARE(securities[1].id(), 2);
    QCOMPARE(securities[1].symbol(), "IWM");

    ok = _manager.createSecurity(spy);
    QVERIFY(!ok);
    securities = _manager.getAllSecurities();
    QCOMPARE(securities.size(), 2);
    QCOMPARE(securities[0].id(), 1);
    QCOMPARE(securities[0].symbol(), "SPY");
    QCOMPARE(securities[1].id(), 2);
    QCOMPARE(securities[1].symbol(), "IWM");
}

void DataManagerTest::testQuotes()
{
    Security spy = Security().withSymbol("SPY");

    bool ok = _manager.createSecurity(spy);
    QVERIFY(ok);

    spy = _manager.getSecurity(1);
    QCOMPARE(spy.id(), 1);
    QCOMPARE(spy.symbol(), "SPY");

    //TODO: add tests
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


