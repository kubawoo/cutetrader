#ifndef TWSCLIENT_CACHE_H
#define TWSCLIENT_CACHE_H

#include <QMap>
#include <QList>
#include "common/types.h"

namespace twsclient {


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

class Cache
{
public:
    void add(long requestId, CacheEntry * entry, const QStringList& keys);
    CacheEntry * get(long requestId);
    QPair<long, CacheEntry *> get(const QStringList& keys);
    void removeExpired();
    void clear();
private:
    QString buildKey(const QStringList & list);

    QMap<QString, long> _requestCache;
    QMap<long, CacheEntry *> _cache;
};

}
#endif
