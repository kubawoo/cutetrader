#ifndef TEST_TWSQAPI_TESTCACHE_H
#define TEST_TWSQAPI_TESTCACHE_H

#include <QObject>

class CacheTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testClearDeletesEntries();
    void testDestructorDeletesEntries();
    void testRemoveExpiredDeletesEntries();
    void testAddReplacesExistingEntry();
};

#endif