#include "portfolio.h"


Portfolio::Portfolio()
{

}

double Portfolio::cash() const
{
    return _cash;
}

void Portfolio::setCash(double newCash)
{
    _cash = newCash;
}

void Portfolio::updatePortfolio(const Stock &stock)
{
//TODO
    qDebug() << "Updating portfolio stock";
}

void Portfolio::updatePortfolio(const Option &option)
{
//TODO
    qDebug() << "Updating portfolio option";

}

BaseSecurity::BaseSecurity(long contractId, const QString &symbol, double position, double marketPrice, double marketValue,
                   double averageCost, double unrealizedPNL, double realizedPNL)
    : _contractId(contractId),
    _symbol(symbol),
    _position(position),
    _marketPrice(marketPrice),
    _marketValue(marketValue),
    _averageCost(averageCost),
    _unrealizedPNL(unrealizedPNL),
    _realizedPNL(realizedPNL)
{}

long BaseSecurity::contractId() const
{
    return _contractId;
}

QString BaseSecurity::symbol() const
{
    return _symbol;
}

double BaseSecurity::position() const
{
    return _position;
}

double BaseSecurity::marketPrice() const
{
    return _marketPrice;
}

double BaseSecurity::marketValue() const
{
    return _marketValue;
}

double BaseSecurity::averageCost() const
{
    return _averageCost;
}

double BaseSecurity::unrealizedPNL() const
{
    return _unrealizedPNL;
}

double BaseSecurity::realizedPNL() const
{
    return _realizedPNL;
}

Stock::Stock(long contractId, const QString &symbol, double position, double marketPrice, double marketValue,
             double averageCost, double unrealizedPNL, double realizedPNL)
: BaseSecurity(contractId, symbol, position, marketPrice, marketValue, averageCost, unrealizedPNL, realizedPNL)
{}

Option::Option(long contractId, const QString &symbol, double position, double marketPrice, double marketValue,
               double averageCost, double unrealizedPNL, double realizedPNL, const QDate &expiration,
               double strike, OptionType type, double multiplier)
    : BaseSecurity(contractId, symbol, position, marketPrice, marketValue, averageCost, unrealizedPNL, realizedPNL),
      _expiration(expiration),
      _strike(strike),
      _type(type),
      _multiplier(multiplier)
{

}

QDate Option::expiration() const
{
    return _expiration;
}

double Option::strike() const
{
    return _strike;
}

OptionType Option::type() const
{
    return _type;
}

double Option::multiplier() const
{
    return _multiplier;
}

