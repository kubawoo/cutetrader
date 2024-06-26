#include "model.h"

Security::Security()
{

}

int Security::id() const
{
    return _id;
}

void Security::setId(int id)
{
    _id = id;
}

Security &Security::withId(int id)
{
    _id = id;
    return *this;
}

QString Security::symbol() const
{
    return _symbol;
}

void Security::setSymbol(const QString &symbol)
{
    _symbol = symbol;
}

Security &Security::withSymbol(const QString &symbol)
{
    _symbol = symbol;
    return *this;
}

int Quote::id() const
{
    return _id;
}

void Quote::setId(int newId)
{
    _id = newId;
}

Quote &Quote::withId(int newId)
{
    _id = newId;
    return *this;
}

QString Quote::securityId() const
{
    return _securityId;
}

void Quote::setSecurityId(const QString &newSecurityId)
{
    _securityId = newSecurityId;
}

Quote &Quote::withSecurityId(const QString &newSecurityId)
{
    _securityId = newSecurityId;
    return *this;
}

QDate Quote::date() const
{
    return _date;
}

void Quote::setDate(const QDate &newDate)
{
    _date = newDate;
}

Quote &Quote::withDate(const QDate &newDate)
{
    _date = newDate;
    return *this;
}

qreal Quote::open() const
{
    return _open;
}

void Quote::setOpen(qreal newOpen)
{
    _open = newOpen;
}

Quote &Quote::withOpen(qreal newOpen)
{
    _open = newOpen;
    return *this;
}

qreal Quote::close() const
{
    return _close;
}

void Quote::setClose(qreal newClose)
{
    _close = newClose;
}

Quote &Quote::withClose(qreal newClose)
{
    _close = newClose;
    return *this;
}

qreal Quote::low() const
{
    return _low;
}

void Quote::setLow(qreal newLow)
{
    _low = newLow;
}

Quote &Quote::withLow(qreal newLow)
{
    _low = newLow;
    return *this;
}

qreal Quote::high() const
{
    return _high;
}

void Quote::setHigh(qreal newHigh)
{
    _high = newHigh;
}

Quote &Quote::withHigh(qreal newHigh)
{
    _high = newHigh;
    return *this;
}

long Quote::volume() const
{
    return _volume;
}

void Quote::setVolume(long newVolume)
{
    _volume = newVolume;
}

Quote &Quote::withVolume(long newVolume)
{
    _volume = newVolume;
    return *this;
}
