#include "accountinfo.h"
#include <QDebug>

AccountInfo::AccountInfo()
    : _accountId(""),
      _baseCurrency("USD")
{
    _mappings.insert("AccruedDividend", AccountInfoType::AccruedDividend);
    _mappings.insert("CashBalance", AccountInfoType::CashBalance);
    _mappings.insert("Cushion", AccountInfoType::Cushion);
    _mappings.insert("ExcessLiquidity", AccountInfoType::ExcessLiquidity);
    _mappings.insert("FuturesPNL", AccountInfoType::FuturesPNL);
    _mappings.insert("InitMarginReq", AccountInfoType::InitMarginReq);
    _mappings.insert("MaintMarginReq", AccountInfoType::MaintMarginReq);
    _mappings.insert("NetLiquidation", AccountInfoType::NetLiquidation);
    _mappings.insert("OptionMarketValue", AccountInfoType::OptionMarketValue);
    _mappings.insert("UnrealizedPnL", AccountInfoType::UnrealizedPnL);
}

void AccountInfo::updateValue(const QString &type, const QString & value)
{
    if(!_mappings.contains(type)) {
        return;
    }


    bool ok;
    double val = value.toDouble(&ok);
    if(ok) {
        AccountInfoType t = _mappings.value(type);
        _values.insert(t, val);
    } else {
        qDebug() << "Failed to convert" << value << "to double";
    }
    qDebug() << "Updated" << type << "to" << val;
}

double AccountInfo::value(AccountInfoType type)
{
    return _values.value(type, 0.0);
}

QString AccountInfo::accountId()
{
    return _accountId;
}

void AccountInfo::setAccountId(const QString &accountId)
{
    _accountId = accountId;
}

QString AccountInfo::baseCurrency()
{
    return _baseCurrency;
}

void AccountInfo::setBaseCurrency(const QString &currency)
{
    _baseCurrency = currency;
}




