#include "twsclient/cache.h"
#include <QDateTime>
#include <QDebug>

namespace twsclient {


CacheEntry::CacheEntry(long ttl)
{
    _expiry = QDateTime::currentSecsSinceEpoch() + ttl;
}

CacheEntry::~CacheEntry()
{}

long CacheEntry::expiry()
{
    return _expiry;
}


void Cache::add(long requestId, CacheEntry *entry, const QStringList &keys)
{
    _cache.insert(requestId, entry);
    if(!keys.empty()) {
        _requestCache.insert(buildKey(keys), requestId);
    }
}

CacheEntry *Cache::get(long requestId)
{
    return _cache.value(requestId, nullptr);
}

CacheEntry *Cache::get(const QStringList &keys)
{
    QString key = buildKey(keys);
    long requestId = _requestCache.value(key, -1L);
    return get(requestId);
}

void Cache::cleanup()
{
    qDebug() << "Before cleanup" << _cache.keys() << _requestCache.keys();
    long now = QDateTime::currentSecsSinceEpoch();
    _cache.removeIf([now](std::pair<const long &, CacheEntry *> entry) {
        bool del = entry.second->expiry() < now;
        if(del) {
            delete entry.second;
        }
        return del;
    });

    _requestCache.removeIf([this](std::pair<const QString &, const long &> entry) {
        return !_cache.contains(entry.second);
    });
    qDebug() << "After cleanup" << _cache.keys() << _requestCache.keys();
}


QString Cache::buildKey(const QStringList &list)
{
    return list.join("_");
}

ContractDetailsCacheEntry::ContractDetailsCacheEntry()
    : CacheEntry(300)
{

}

ContractDetailsCacheEntry::~ContractDetailsCacheEntry()
{

}

}
