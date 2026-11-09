#include "test_cache.h"
#include "twsqapi/cache.h"
#include <QTest>
#include <memory>

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
    cache.add(1, std::make_shared<TrackingCacheEntry>(300), {"test", "key1"});
    cache.add(2, std::make_shared<TrackingCacheEntry>(300), {"test", "key2"});
    cache.add(3, std::make_shared<TrackingCacheEntry>(300), {"test", "key3"});

    // Verify entries exist via keys (but don't hold onto the returned pointers)
    {
        auto entry1 = cache.get({"test", "key1"});
        auto entry2 = cache.get({"test", "key2"});
        auto entry3 = cache.get({"test", "key3"});

        QVERIFY(entry1.first >= 0);
        QVERIFY(entry1.second != nullptr);
        QVERIFY(entry2.first >= 0);
        QVERIFY(entry2.second != nullptr);
        QVERIFY(entry3.first >= 0);
        QVERIFY(entry3.second != nullptr);
    } // Local shared_ptrs go out of scope here

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
    QVERIFY(entryAfter1.second == nullptr);
    QCOMPARE(entryAfter2.first, -1L);
    QVERIFY(entryAfter2.second == nullptr);
    QCOMPARE(entryAfter3.first, -1L);
    QVERIFY(entryAfter3.second == nullptr);
}

void CacheTest::testDestructorDeletesEntries()
{
    g_destructorCallCount = 0;

    {
        twsqapi::Cache cache;

        cache.add(10, std::make_shared<TrackingCacheEntry>(300), {"destructor", "test1"});
        cache.add(11, std::make_shared<TrackingCacheEntry>(300), {"destructor", "test2"});

        {
            auto entry1 = cache.get({"destructor", "test1"});
            auto entry2 = cache.get({"destructor", "test2"});

            QVERIFY(entry1.first >= 0);
            QVERIFY(entry1.second != nullptr);
            QVERIFY(entry2.first >= 0);
            QVERIFY(entry2.second != nullptr);
        } // Local shared_ptrs go out of scope
    } // cache goes out of scope here, destructor called

    // Destructor should delete all 2 entries
    QCOMPARE(g_destructorCallCount, 2);
}

void CacheTest::testRemoveExpiredDeletesEntries()
{
    g_destructorCallCount = 0;

    twsqapi::Cache cache;

    // Add entry with negative TTL (already expired)
    cache.add(100, std::make_shared<TrackingCacheEntry>(-1), {"expired", "test"});

    {
        auto entry = cache.get({"expired", "test"});
        QVERIFY(entry.first >= 0);
        QVERIFY(entry.second != nullptr);
    } // Local shared_ptr goes out of scope

    // removeExpired should delete expired entries
    cache.removeExpired();

    // Should have deleted the expired entry
    QCOMPARE(g_destructorCallCount, 1);

    // Entry should no longer be accessible
    auto entryAfter = cache.get({"expired", "test"});
    QCOMPARE(entryAfter.first, -1L);
    QVERIFY(entryAfter.second == nullptr);
}

void CacheTest::testAddReplacesExistingEntry()
{
    g_destructorCallCount = 0;

    twsqapi::Cache cache;

    // Add first entry (not ready by default)
    long reqId1 = cache.add(200, std::make_shared<TrackingCacheEntry>(300), {"replace", "test"});
    QCOMPARE(reqId1, 200L);

    // Add second entry with same key - should reuse first (not ready)
    long reqId2 = cache.add(201, std::make_shared<TrackingCacheEntry>(300), {"replace", "test"});
    QCOMPARE(reqId2, 200L); // Returns existing requestId

    // Second entry's destructor called because it wasn't stored
    QCOMPARE(g_destructorCallCount, 1);

    // Verify the entry is the first one (release reference immediately)
    {
        auto entry = cache.get({"replace", "test"});
        QCOMPARE(entry.first, 200L);
        QVERIFY(entry.second != nullptr);
        QVERIFY(!entry.second->ready());
    } // entry goes out of scope here

    // Now test with ready entry - should replace
    g_destructorCallCount = 0;
    {
        auto entry = cache.get({"replace", "test"});
        QVERIFY(entry.second != nullptr);
        entry.second->setReady();
    } // Release reference before replace

    long reqId3 = cache.add(202, std::make_shared<TrackingCacheEntry>(300), {"replace", "test"});
    QCOMPARE(reqId3, 202L); // Returns new requestId

    // The old ready entry should have been deleted
    QCOMPARE(g_destructorCallCount, 1);

    // Verify the entry is the new one
    auto newEntry = cache.get({"replace", "test"});
    QCOMPARE(newEntry.first, 202L);
    QVERIFY(newEntry.second != nullptr);
}