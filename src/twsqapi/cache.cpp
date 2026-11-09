#include "twsqapi/cache.h"
#include <QDateTime>
#include <QDebug>
#include <QMutexLocker>

namespace twsqapi {


Cache::Cache(QObject *parent)
    : QObject(parent)
{
    _cleanupTimer.setInterval(60000); // 60 seconds
    QObject::connect(&_cleanupTimer, &QTimer::timeout, this, [this]() {
        removeExpired();
    });
    _cleanupTimer.start();
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


void Cache::add(long requestId, std::shared_ptr<CacheEntry> entry, const QStringList &keys)
{
    QMutexLocker locker(&_mutex);
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

std::shared_ptr<CacheEntry> Cache::get(long requestId)
{
    QMutexLocker locker(&_mutex);
    return getUnlocked(requestId);
}

std::shared_ptr<CacheEntry> Cache::getUnlocked(long requestId)
{
    auto it = _cache.find(requestId);
    if (it != _cache.end()) {
        return it.value();
    }
    return std::shared_ptr<CacheEntry>();
}

std::pair<long, std::shared_ptr<CacheEntry>> Cache::get(const QStringList &keys)
{
    QMutexLocker locker(&_mutex);
    QString key = buildKey(keys);
    long requestId = _requestCache.value(key, -1L);
    if(requestId >= 0) {
        return std::pair<long, std::shared_ptr<CacheEntry>>(requestId, getUnlocked(requestId));
    }
    return std::pair<long, std::shared_ptr<CacheEntry>>(requestId, std::shared_ptr<CacheEntry>());
}

void Cache::removeExpired()
{
    QMutexLocker locker(&_mutex);
    qDebug() << "Before cleanup" << _cache.keys() << _requestCache.keys();
    long now = QDateTime::currentSecsSinceEpoch();
    _cache.removeIf([now](std::pair<const long &, std::shared_ptr<CacheEntry>> entry) {
        return entry.second && entry.second->expiry() < now;
    });

    _requestCache.removeIf([this](std::pair<const QString &, const long &> entry) {
        return !_cache.contains(entry.second);
    });
    qDebug() << "After cleanup" << _cache.keys() << _requestCache.keys();
}

void Cache::clear()
{
    QMutexLocker locker(&_mutex);
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