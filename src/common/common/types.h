#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

#include <QList>
#include <QDate>

namespace common {


enum class SecurityType {
    UNSUPPORTED,
    STOCK,
    OPTION,
    FUTURE,
    FUTURE_OPTION,
    INDEX,
    CFD,
    BAG,
    BOND,
    CASH,
    CMDTY,
    CONTFUT,
    CRYPTO,
    FUND,
    IOPT,
    NEWS,
    WAR
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
    QString primaryExchange;
    QString issuerId;
    SecurityType securityType;
    QList<SecurityType> derivatives;
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

struct BarDTO
{
    QString time;
    double open;
    double high;
    double low;
    double close;
    double wap;
    double volume;
    int barCount;
    double average;
};

struct OrderDTO
{
    int orderId;
    long contractId;
    QString symbol;
    common::SecurityType securityType;
    QString currency;
    QString action; // BUY, SELL
    double totalQuantity;
    QString orderType; // MKT, LMT, STP, etc.
    double lmtPrice;
    double auxPrice;
    QString tif; // DAY, GTC, etc.
    QString status; // PendingSubmit, Submitted, Filled, Cancelled, etc.
    double filled;
    double remaining;
    double avgFillPrice;
    QString lastFillTime;
    QString account;
};

}


#endif // COMMON_TYPES_H
