#ifndef TWSQAPI_CACHE_H
#define TWSQAPI_CACHE_H

#include <QMap>
#include <QList>
#include <QSharedPointer>
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
    ~ContractDetailsCacheEntry();

    QList<common::ContractDetailsDTO> contractDetails;
};

class HistoricalDataCacheEntry : public CacheEntry
{
public:
    HistoricalDataCacheEntry();
    ~HistoricalDataCacheEntry();

    QList<common::BarDTO> bars;
};

class Cache
{
public:
    Cache();
    ~Cache();
    void add(long requestId, QSharedPointer<CacheEntry> entry, const QStringList& keys);
    QSharedPointer<CacheEntry> get(long requestId);
    QPair<long, QSharedPointer<CacheEntry>> get(const QStringList& keys);
    void removeExpired();
    void clear();
private:
    QString buildKey(const QStringList & list);

    QMap<QString, long> _requestCache;
    QMap<long, QSharedPointer<CacheEntry>> _cache;
};

}
#endif
