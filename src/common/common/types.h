#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

#include <QList>
#include <QDate>

namespace common {


enum class SecurityType {
    STOCK, OPTION, FUTURE, FUTURE_OPTION
};

enum class OptionType {
    CALL, PUT
};

struct PortfolioPositionDTO
{
    long contractId;
    SecurityType securityType;
    QString symbol;
    double position;
    double marketPrice;
    double marketValue;
    double averageCost;
    double unrealizedPNL;
    double realizedPNL;
    QDate expiration;
    double strike;
    OptionType right;
    double multiplier;
};

struct ContractDetailsDTO
{
    long contractId;
    QString symbol;
    QString currency;
    QString description;
};

struct QuoteDTO
{
    QDate date;
    double open;
    double close;
    double low;
    double high;
    int volume;
};

}


#endif // COMMON_DTO_H
