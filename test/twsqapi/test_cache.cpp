#include "test_cache.h"
#include "twsqapi/cache.h"
#include <QTest>
#include <QSharedPointer>

namespace {

int g_destructorCallCount = 0;

class TrackingCacheEntry : public twsqapi::CacheEntry
{
public:
    TrackingCacheEntry(long ttl)
        : CacheEntry(ttl)
    {
    }

    ~TrackingCacheEntry() override
    {
        ++g_destructorCallCount;
    }
};

} // anonymous namespace


void CacheTest::initTestCase()
{
    g_destructorCallCount = 0;
}

void CacheTest::cleanupTestCase()
{
    g_destructorCallCount = 0;
}

void CacheTest::testClearDeletesEntries()
{
    twsqapi::Cache cache;

    // Add entries to cache
    cache.add(1, QSharedPointer<TrackingCacheEntry>::create(300), {"test", "key1"});
    cache.add(2, QSharedPointer<TrackingCacheEntry>::create(300), {"test", "key2"});
    cache.add(3, QSharedPointer<TrackingCacheEntry>::create(300), {"test", "key3"});

    // Verify entries exist via keys (but don't hold onto the returned pointers)
    {
        auto entry1 = cache.get({"test", "key1"});
        auto entry2 = cache.get({"test", "key2"});
        auto entry3 = cache.get({"test", "key3"});

        QVERIFY(entry1.first >= 0);
        QVERIFY(!entry1.second.isNull());
        QVERIFY(entry2.first >= 0);
        QVERIFY(!entry2.second.isNull());
        QVERIFY(entry3.first >= 0);
        QVERIFY(!entry3.second.isNull());
    } // Local QSharedPointers go out of scope here

    g_destructorCallCount = 0;

    // Call clear - should delete all entries
    cache.clear();

    // clear() should delete all 3 entries
    QCOMPARE(g_destructorCallCount, 3);

    // Entries should no longer be accessible after clear
    auto entryAfter1 = cache.get({"test", "key1"});
    auto entryAfter2 = cache.get({"test", "key2"});
    auto entryAfter3 = cache.get({"test", "key3"});

    QCOMPARE(entryAfter1.first, -1L);
    QVERIFY(entryAfter1.second.isNull());
    QCOMPARE(entryAfter2.first, -1L);
    QVERIFY(entryAfter2.second.isNull());
    QCOMPARE(entryAfter3.first, -1L);
    QVERIFY(entryAfter3.second.isNull());
}

void CacheTest::testDestructorDeletesEntries()
{
    g_destructorCallCount = 0;

    {
        twsqapi::Cache cache;

        cache.add(10, QSharedPointer<TrackingCacheEntry>::create(300), {"destructor", "test1"});
        cache.add(11, QSharedPointer<TrackingCacheEntry>::create(300), {"destructor", "test2"});

        {
            auto entry1 = cache.get({"destructor", "test1"});
            auto entry2 = cache.get({"destructor", "test2"});

            QVERIFY(entry1.first >= 0);
            QVERIFY(!entry1.second.isNull());
            QVERIFY(entry2.first >= 0);
            QVERIFY(!entry2.second.isNull());
        } // Local QSharedPointers go out of scope
    } // cache goes out of scope here, destructor called

    // Destructor should delete all 2 entries
    QCOMPARE(g_destructorCallCount, 2);
}

void CacheTest::testRemoveExpiredDeletesEntries()
{
    g_destructorCallCount = 0;

    twsqapi::Cache cache;

    // Add entry with negative TTL (already expired)
    cache.add(100, QSharedPointer<TrackingCacheEntry>::create(-1), {"expired", "test"});

    {
        auto entry = cache.get({"expired", "test"});
        QVERIFY(entry.first >= 0);
        QVERIFY(!entry.second.isNull());
    } // Local QSharedPointer goes out of scope

    // removeExpired should delete expired entries
    cache.removeExpired();

    // Should have deleted the expired entry
    QCOMPARE(g_destructorCallCount, 1);

    // Entry should no longer be accessible
    auto entryAfter = cache.get({"expired", "test"});
    QCOMPARE(entryAfter.first, -1L);
    QVERIFY(entryAfter.second.isNull());
}

void CacheTest::testAddReplacesExistingEntry()
{
    g_destructorCallCount = 0;

    twsqapi::Cache cache;

    // Add first entry
    cache.add(200, QSharedPointer<TrackingCacheEntry>::create(300), {"replace", "test"});

    // Add second entry with same key - should replace first
    cache.add(201, QSharedPointer<TrackingCacheEntry>::create(300), {"replace", "test"});

    // The first entry should have been deleted (not leaked)
    QCOMPARE(g_destructorCallCount, 1);
}