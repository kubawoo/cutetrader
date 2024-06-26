#include "cache.h"
#include <QDateTime>
#include <QDebug>

CacheEntry::CacheEntry()
    : CacheEntry(0)
{

}

CacheEntry::CacheEntry(long expiry)
    : _expiry(expiry)
{

}

long CacheEntry::expiry()
{
    return _expiry;
}

QList<Bar> *CacheEntry::bars()
{
    return &_bars;
}

void Cache::cleanup()
{
    qDebug() << "Before cleanup" << _cache.keys();
    long now = QDateTime::currentSecsSinceEpoch();
    _cache.removeIf([now](std::pair<const long &, CacheEntry &> entry)
                    {
                        return entry.second.expiry() < now;
    });
    qDebug() << "After cleanup" << _cache.keys();

}

void Cache::addBar(long requestId, const Bar &bar)
{
    if(!_cache.contains(requestId)) {
        long now = QDateTime::currentSecsSinceEpoch();
        _cache.insert(requestId, CacheEntry(now + 300)); // 5 minutes
    }
    _cache.value(requestId).bars()->append(bar);
}

QList<Bar> *Cache::bars(long requestId)
{
    return _cache[requestId].bars();
}
