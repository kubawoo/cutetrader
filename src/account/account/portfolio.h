#ifndef ACCOUNT_PORTFOLIO_H
#define ACCOUNT_PORTFOLIO_H

#include <QMap>
#include <QDate>
#include <QList>
#include <common.h>

namespace account {

class BaseSecurity
{
public:
    BaseSecurity();
    BaseSecurity(long contractId, const QString &symbol, double position, double marketPrice, double marketValue,
             double averageCost, double unrealizedPNL, double realizedPNL);


    long contractId() const;
    QString symbol() const;
    double position() const;
    double marketPrice() const;
    double marketValue() const;
    double averageCost() const;
    double unrealizedPNL() const;
    double realizedPNL() const;

private:
    long _contractId;
    QString _symbol;
    double _position;
    double _marketPrice;
    double _marketValue;
    double _averageCost;
    double _unrealizedPNL;
    double _realizedPNL;
};

class Stock : public BaseSecurity
{
public:
    Stock();
    Stock(long contractId, const QString &symbol, double position, double marketPrice, double marketValue,
             double averageCost, double unrealizedPNL, double realizedPNL);
};


class Option : public BaseSecurity
{
public:
    Option();
    Option(long contractId, const QString &symbol, double position, double marketPrice, double marketValue,
           double averageCost, double unrealizedPNL, double realizedPNL,
           const QDate & expiration, double strike, common::OptionType type, double multiplier);
    QDate expiration() const;
    double strike() const;
    common::OptionType type() const;
    QString typeAsString() const;
    double multiplier() const;

private:
    QDate _expiration;
    double _strike;
    common::OptionType _type;
    double _multiplier;
};

class Portfolio
{
public:
    Portfolio();

    void updatePortfolio(const Stock & stock);
    void updatePortfolio(const Option & option);
    QList<Stock> stocks();
    QList<Option> options();

private:
    QMap<long, Stock> _stocks;
    QMap<long, Option> _options;
};
}


#endif
