#include "twsqapi/cache.h"
#include <QDateTime>
#include <QDebug>

namespace twsqapi {


Cache::Cache()
{
}

CacheEntry::CacheEntry(long ttl)
{
    _expiry = QDateTime::currentSecsSinceEpoch() + ttl;
    _ready = false;
}

CacheEntry::~CacheEntry()
{}

long CacheEntry::expiry()
{
    return _expiry;
}

bool CacheEntry::ready()
{
    return _ready;
}

void CacheEntry::setReady()
{
    _ready = true;
}


void Cache::add(long requestId, QSharedPointer<CacheEntry> entry, const QStringList &keys)
{
    // If there's already an entry for this requestId, it will be replaced
    _cache.insert(requestId, entry);
    if(!keys.empty()) {
        QString key = buildKey(keys);
        // If there's already an entry for this key, remove the old requestId from _cache
        if (_requestCache.contains(key)) {
            long oldRequestId = _requestCache[key];
            if (oldRequestId != requestId) {
                _cache.remove(oldRequestId);
            }
        }
        _requestCache.insert(key, requestId);
    }
}

QSharedPointer<CacheEntry> Cache::get(long requestId)
{
    return _cache.value(requestId, QSharedPointer<CacheEntry>());
}

QPair<long, QSharedPointer<CacheEntry>> Cache::get(const QStringList &keys)
{
    QString key = buildKey(keys);
    long requestId = _requestCache.value(key, -1L);
    if(requestId >= 0) {
        return QPair<long, QSharedPointer<CacheEntry>>(requestId, get(requestId));
    }
    return QPair<long, QSharedPointer<CacheEntry>>(requestId, QSharedPointer<CacheEntry>());
}

void Cache::removeExpired()
{
    qDebug() << "Before cleanup" << _cache.keys() << _requestCache.keys();
    long now = QDateTime::currentSecsSinceEpoch();
    _cache.removeIf([now](std::pair<const long &, QSharedPointer<CacheEntry>> entry) {
        return entry.second && entry.second->expiry() < now;
    });

    _requestCache.removeIf([this](std::pair<const QString &, const long &> entry) {
        return !_cache.contains(entry.second);
    });
    qDebug() << "After cleanup" << _cache.keys() << _requestCache.keys();
}

void Cache::clear()
{
    _cache.clear();
    _requestCache.clear();
}


QString Cache::buildKey(const QStringList &list)
{
    return list.join("_").toLower();
}

Cache::~Cache()
{
    clear();
}

ContractDetailsCacheEntry::ContractDetailsCacheEntry()
    : CacheEntry(300)
{
}

ContractDetailsCacheEntry::~ContractDetailsCacheEntry()
{
}

HistoricalDataCacheEntry::HistoricalDataCacheEntry()
    : CacheEntry(300)
{
}

HistoricalDataCacheEntry::~HistoricalDataCacheEntry()
{
}

}