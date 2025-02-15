#include "twsclient/cache.h"
#include <QDateTime>
#include <QDebug>

namespace twsclient {


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

QPair<long, CacheEntry *> Cache::get(const QStringList &keys)
{
    QString key = buildKey(keys);
    long requestId = _requestCache.value(key, -1L);
    if(requestId >= 0) {
        return QPair<long, CacheEntry *> (requestId, get(requestId));
    }
    return QPair<long, CacheEntry *>(requestId, nullptr);
}

void Cache::removeExpired()
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

void Cache::clear()
{
    _cache.clear();
    _requestCache.clear();
}


QString Cache::buildKey(const QStringList &list)
{
    return list.join("_").toLower();
}

ContractDetailsCacheEntry::ContractDetailsCacheEntry()
    : CacheEntry(300)
{

}

ContractDetailsCacheEntry::~ContractDetailsCacheEntry()
{

}

}
