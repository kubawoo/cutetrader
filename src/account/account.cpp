#include "account/account.h"

namespace account {

Account::Account(QObject * parent)
    : QObject(parent),
      _baseCurrency("USD")
{

}

QString Account::accountId()
{
    return _accountId;
}

void Account::setAccountId(const QString &accountId)
{
    _accountId = accountId;
}

QString Account::baseCurrency()
{
    return _baseCurrency;
}

void Account::setBaseCurrency(const QString &currency)
{
    _baseCurrency = currency;
}

double Account::accountInfo(AccountInfoType type)
{
    return _accountInfo.value(type);
}


void Account::updateAccountValue(const QString & key, const QString & value, const QString & currency)
{
    qDebug() << "Account::updateAccountValue" << key << value << currency;

    if(currency == _baseCurrency || currency.isEmpty()) {
        AccountInfoType updatedType = _accountInfo.updateValue(key, value);
        if(updatedType != AccountInfoType::NONE) {
            emit accountValueUpdated(updatedType, _accountInfo.value(updatedType));
        }
    }

}

//void TwsClient::updatePortfolio( const Contract& contract, Decimal position,
//    double marketPrice, double marketValue, double averageCost,
//    double unrealizedPNL, double realizedPNL, const std::string& accountName)
//{
//    qDebug() << "updatePortfolio" << contract.secType.c_str()
//             << contract.symbol.c_str() << DecimalFunctions::decimalToDouble(position)
//             << marketPrice << marketValue << averageCost
//             <<unrealizedPNL << realizedPNL << accountName.c_str();
//    if(contract.secType == "OPT") {
//        qDebug() << "Option:" << contract.lastTradeDateOrContractMonth.c_str()
//                 << contract.lastTradeDate.c_str()
//                 << contract.strike
//                 << contract.right.c_str()
//                 << contract.multiplier.c_str()
//                 << contract.comboLegsDescrip.c_str();

//        QDate expiration = QDate::currentDate(); //TODO
//        OptionType type = contract.right == "C" ? OptionType::CALL : OptionType::PUT;
//        double mul = 100.0; //TODO

//        Option opt(contract.conId, contract.symbol.c_str(), DecimalFunctions::decimalToDouble(position),
//                   marketPrice, marketValue, averageCost, unrealizedPNL, realizedPNL, expiration, contract.strike,
//                   type, mul);
//        _portfolio.updatePortfolio(opt);

//    } else if(contract.secType == "STK") {
//        Stock stk(contract.conId, contract.symbol.c_str(), DecimalFunctions::decimalToDouble(position),
//                   marketPrice, marketValue, averageCost, unrealizedPNL, realizedPNL);
//        _portfolio.updatePortfolio(stk);
//    }
//}


}
