#include "accountinfo.h"
#include <QDebug>

namespace account {

QMap<QString, AccountInfoType>  AccountInfo::_MAPPINGS {
    {"AccruedCash", AccountInfoType::AccruedCash},
    {"AccruedDividend", AccountInfoType::AccruedDividend},
    {"AvailableFunds", AccountInfoType::AvailableFunds},
    {"Billable", AccountInfoType::Billable},
    {"BuyingPower", AccountInfoType::BuyingPower},
    {"CashBalance", AccountInfoType::CashBalance},
    {"CorporateBondValue", AccountInfoType::CorporateBondValue},
    {"Cushion", AccountInfoType::Cushion},
    {"EquityWithLoanValue", AccountInfoType::EquityWithLoanValue},
    {"ExcessLiquidity", AccountInfoType::ExcessLiquidity},
    {"FundValue", AccountInfoType::FundValue},
    {"FutureOptionValue", AccountInfoType::FutureOptionValue},
    {"FuturesPNL", AccountInfoType::FuturesPNL},
    {"GrossPositionValue", AccountInfoType::GrossPositionValue},
    {"InitMarginReq", AccountInfoType::InitMarginReq},
    {"Leverage-S", AccountInfoType::Leverage},
    {"MaintMarginReq", AccountInfoType::MaintMarginReq},
    {"NetDividend", AccountInfoType::NetDividend},
    {"NetLiquidation", AccountInfoType::NetLiquidation},
    {"OptionMarketValue", AccountInfoType::OptionMarketValue},
    {"RealizedPnL", AccountInfoType::RealizedPnL},
    {"StockMarketValue", AccountInfoType::StockMarketValue},
    {"TBondValue", AccountInfoType::TBondValue},
    {"TBillValue", AccountInfoType::TBillValue},
    {"TotalCashBalance", AccountInfoType::TotalCashBalance},
    {"UnrealizedPnL", AccountInfoType::UnrealizedPnL}
};

AccountInfo::AccountInfo()
{
}

AccountInfoType AccountInfo::updateValue(const QString &type, const QString & value)
{
    if(!_MAPPINGS.contains(type)) {
        return AccountInfoType::NONE;
    }

    bool ok;
    double val = value.toDouble(&ok);
    if(ok) {
        AccountInfoType t = _MAPPINGS.value(type);
        _values.insert(t, val);
        qDebug() << "Updated" << type << "to" << val;
        return t;
    } else {
        qDebug() << "Failed to convert" << value << "to double";
        return AccountInfoType::NONE;
    }
}

double AccountInfo::value(AccountInfoType type)
{
    return _values.value(type, 0.0);
}

}

