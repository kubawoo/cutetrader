#ifndef ACCOUNT_PORTFOLIO_H
#define ACCOUNT_PORTFOLIO_H

#include <QList>
#include <QDate>

namespace account {

class BaseSecurity
{
public:
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
    Stock(long contractId, const QString &symbol, double position, double marketPrice, double marketValue,
             double averageCost, double unrealizedPNL, double realizedPNL);
};

enum class OptionType {
    PUT, CALL
};

class Option : public BaseSecurity
{
public:
    Option(long contractId, const QString &symbol, double position, double marketPrice, double marketValue,
           double averageCost, double unrealizedPNL, double realizedPNL,
           const QDate & expiration, double strike, OptionType type, double multiplier);
    QDate expiration() const;
    double strike() const;
    OptionType type() const;
    double multiplier() const;

private:
    QDate _expiration;
    double _strike;
    OptionType _type;
    double _multiplier;
};

class Portfolio
{
public:
    Portfolio();

    double cash() const;
    void setCash(double newCash);

    void updatePortfolio(const Stock & stock);
    void updatePortfolio(const Option & option);

private:
    QList<Stock> _stocks;
    QList<Option> _options;
    double _cash;
};
}


#endif
