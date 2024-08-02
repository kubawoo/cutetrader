#ifndef ACCOUNT_ACCOUNTINFO_H
#define ACCOUNT_ACCOUNTINFO_H

#include <QString>
#include <QMap>

namespace account {

enum class AccountInfoType
{
    NONE,
    AccruedCash,        //Total accrued cash value of stock, commodities and securities
    AccruedDividend,    //Total portfolio value of dividends accrued
    AvailableFunds,     //This value tells what you have available for trading
    Billable,           //Total portfolio value of treasury bills
    BuyingPower,        //Cash Account: Minimum (Equity with Loan Value, Previous Day Equity with Loan Value)-Initial
                        //   Margin, Standard Margin Account: Minimum (Equity with Loan Value, Previous Day Equity
                        //   with Loan Value) – Initial Margin *4
    CashBalance,        //Cash recognized at the time of trade + futures PNL
    CorporateBondValue, //Value of non-Government bonds such as corporate bonds and municipal bonds
    Cushion,            //Excess liquidity as a percentage of net liquidation value
    DayTradesRemaining, //Number of Open/Close trades one could do before Pattern Day Trading is detected
    EquityWithLoanValue,//Forms the basis for determining whether a client has the necessary assets to either initiate
                        //   or maintain security positions
    ExcessLiquidity,    //This value shows your margin cushion, before liquidation
    FundValue,          //Value of funds value (money market funds + mutual funds)
    FutureOptionValue,  //Real-time market-to-market value of futures options
    FuturesPNL,         //Real-time changes in futures value since last settlement
    GrossPositionValue, //Gross Position Value in securities segment
    InitMarginReq,      //Initial Margin requirement of whole portfolio
    Leverage,           //GrossPositionValue / NetLiquidation in security segment
    MaintMarginReq,     //Maintenance Margin requirement of whole portfolio
    NetDividend,        //The sum of the Dividend Payable/Receivable Values for the securities and commodities segments
                        //   of the account
    NetLiquidation,     //The basis for determining the price of the assets in your account
    OptionMarketValue,  //Real-time mark-to-market value of options
    RealizedPnL,        //Shows your profit on closed positions, which is the difference between your entry execution
                        //   cost and exit execution costs, or (execution price + commissions to open the positions)
                        //   – (execution price + commissions to close the position)
    StockMarketValue,   //Real-time mark-to-market value of stock
    TBondValue,         //Value of treasury bonds
    TBillValue,         //Value of treasury bills
    TotalCashBalance,   //Total Cash Balance including Future PNL
    UnrealizedPnL       //The difference between the current market value of your open positions and the average cost,
                        //   or Value – Average Cost
};

class AccountInfo
{
public:
    AccountInfo();

    AccountInfoType updateValue(const QString & type, const QString & value);
    double value(AccountInfoType type);

private:
    static QMap<QString, AccountInfoType> _MAPPINGS;

    QMap<AccountInfoType, double> _values;
};
}

#endif
