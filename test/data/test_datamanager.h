#ifndef TEST_DATAMANAGER_H
#define TEST_DATAMANAGER_H


#include <data/datamanager.h>
#include <QTest>
#include <QSqlDatabase>


class DataManagerTest : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();

    void testSecurities();
    void testQuotes();
    void testQuoteWithNoSecurity();
    void testDeleteSecurityWithQuotes();
    void testGetQuotesFrom();
    void testGetQuotesFromTo();
private:
    QSqlDatabase _db;
    data::DataManager _manager;
};


#endif // TEST_DATAMANAGER_H
