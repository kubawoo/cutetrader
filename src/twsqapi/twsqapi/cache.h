#ifndef TWSQAPI_CACHE_H
#define TWSQAPI_CACHE_H

#include <QMap>
#include <QList>
#include <QMutex>
#include <memory>
#include "common/types.h"

namespace twsqapi {


class CacheEntry
{
public:
    CacheEntry(long expiry);
    virtual ~CacheEntry() = 0;
    long expiry();
    bool ready();
    void setReady();
private:
    long _expiry;
    bool _ready;
};


class ContractDetailsCacheEntry : public CacheEntry
{
public:
    ContractDetailsCacheEntry();
    ~ContractDetailsCacheEntry() override;

    QList<common::ContractDetailsDTO> contractDetails;
};

class HistoricalDataCacheEntry : public CacheEntry
{
public:
    HistoricalDataCacheEntry();
    ~HistoricalDataCacheEntry() override;

    QList<common::BarDTO> bars;
};

class Cache
{
public:
    Cache();
    ~Cache();
    void add(long requestId, std::shared_ptr<CacheEntry> entry, const QStringList& keys);
    std::shared_ptr<CacheEntry> get(long requestId);
    std::pair<long, std::shared_ptr<CacheEntry>> get(const QStringList& keys);
    void removeExpired();
    void clear();
private:
    QString buildKey(const QStringList & list);
    std::shared_ptr<CacheEntry> getUnlocked(long requestId);

    QMutex _mutex;
    QMap<QString, long> _requestCache;
    QMap<long, std::shared_ptr<CacheEntry>> _cache;
};

}
#endif