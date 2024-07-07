#ifndef ACCOUNTINFO_H
#define ACCOUNTINFO_H

#include <QString>
#include <QMap>


enum class AccountInfoType
{
    NONE,
    AccruedDividend,    //Total portfolio value of dividends accrued
    CashBalance,        //Cash recognized at the time of trade + futures PNL
    Cushion,            //Excess liquidity as a percentage of net liquidation value
    ExcessLiquidity,    //This value shows your margin cushion, before liquidation
    FuturesPNL,         //Real-time changes in futures value since last settlement
    InitMarginReq,      //Initial Margin requirement of whole portfolio
    MaintMarginReq,     //Maintenance Margin requirement of whole portfolio
    NetLiquidation,     //The basis for determining the price of the assets in your account
    OptionMarketValue,  //Real-time mark-to-market value of options
    UnrealizedPnL       //The difference between the current market value of your open positions and the average cost, or Value – Average Cost
};

class AccountInfo
{
public:
    AccountInfo();

    AccountInfoType updateValue(const QString & type, const QString & value);
    double value(AccountInfoType type);

    QString accountId();
    void setAccountId(const QString & accountId);

    QString baseCurrency();
    void setBaseCurrency(const QString & currency);

private:
        QString _accountId;
        QString _baseCurrency;

        QMap<AccountInfoType, double> _values;
        QMap<QString, AccountInfoType> _mappings;
};

#endif // ACCOUNTINFO_H
