#ifndef CACHE_H
#define CACHE_H

#include <QMap>
#include <QList>
#include "bar.h"

namespace twsclient {

class CacheEntry
{
public:
    CacheEntry();
    CacheEntry(long expiry);
    long expiry();
    QList<Bar> *bars();

private:
    int _expiry;
    QList<Bar> _bars;
};

class Cache
{
public:
    void cleanup();
    void addBar(long requestId, const Bar &bar);
    QList<Bar> *bars(long requestId);
    void remove(long requestId);
private:
    QMap<long, CacheEntry> _cache;
};

}
#endif // CACHE_H
