#include "data/model.h"

namespace data {

Security::Security()
    :_id(-1), _symbol(""), _contractId(-1)
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

int Security::contractId() const
{
    return _contractId;
}

void Security::setContractId(int id)
{
    _contractId = id;
}

Security &Security::withContractId(int id)
{
    _contractId = id;
    return *this;
}

Quote::Quote()
    :_id(-1), _securityId(-1)
{
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

int Quote::securityId() const
{
    return _securityId;
}

void Quote::setSecurityId(int newSecurityId)
{
    _securityId = newSecurityId;
}

Quote &Quote::withSecurityId(int newSecurityId)
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

double Quote::open() const
{
    return _open;
}

void Quote::setOpen(double newOpen)
{
    _open = newOpen;
}

Quote &Quote::withOpen(double newOpen)
{
    _open = newOpen;
    return *this;
}

double Quote::close() const
{
    return _close;
}

void Quote::setClose(double newClose)
{
    _close = newClose;
}

Quote &Quote::withClose(double newClose)
{
    _close = newClose;
    return *this;
}

double Quote::low() const
{
    return _low;
}

void Quote::setLow(double newLow)
{
    _low = newLow;
}

Quote &Quote::withLow(double newLow)
{
    _low = newLow;
    return *this;
}

double Quote::high() const
{
    return _high;
}

void Quote::setHigh(double newHigh)
{
    _high = newHigh;
}

Quote &Quote::withHigh(double newHigh)
{
    _high = newHigh;
    return *this;
}

int Quote::volume() const
{
    return _volume;
}

void Quote::setVolume(int newVolume)
{
    _volume = newVolume;
}

Quote &Quote::withVolume(int newVolume)
{
    _volume = newVolume;
    return *this;
}

}
