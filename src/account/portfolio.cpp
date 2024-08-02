#include "account/portfolio.h"

namespace account {

Portfolio::Portfolio()
{

}


void Portfolio::updatePortfolio(const Stock &stock)
{
    qDebug() << "Updating portfolio stock";
    _stocks[stock.contractId()] = stock;
}

void Portfolio::updatePortfolio(const Option &option)
{
    qDebug() << "Updating portfolio option";
    _options[option.contractId()] = option;
}

QList<Stock> Portfolio::stocks()
{
    return _stocks.values();
}

QList<Option> Portfolio::options()
{
    return _options.values();
}

BaseSecurity::BaseSecurity()
    :_contractId(-1)
{

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

Stock::Stock()
{

}

Stock::Stock(long contractId, const QString &symbol, double position, double marketPrice, double marketValue,
             double averageCost, double unrealizedPNL, double realizedPNL)
: BaseSecurity(contractId, symbol, position, marketPrice, marketValue, averageCost, unrealizedPNL, realizedPNL)
{}

Option::Option()
{

}

Option::Option(long contractId, const QString &symbol, double position, double marketPrice, double marketValue,
               double averageCost, double unrealizedPNL, double realizedPNL, const QDate &expiration,
               double strike, common::OptionType type, double multiplier)
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

common::OptionType Option::type() const
{
    return _type;
}

double Option::multiplier() const
{
    return _multiplier;
}

}

