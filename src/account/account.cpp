#include "account/account.h"

namespace account {

Account::Account(QObject * parent)
    : QObject(parent),
      _baseCurrency("USD")
{

}


QString Account::baseCurrency()
{
    return _baseCurrency;
}

void Account::setBaseCurrency(const QString &currency)
{
    _baseCurrency = currency;
}

Stock Account::updateStockPosition(const common::PortfolioPositionDTO &position)
{
    Stock stock(position.contractId, position.symbol, position.position, position.marketPrice, position.marketValue,
                position.averageCost, position.unrealizedPNL, position.realizedPNL);
    _portfolio.updatePortfolio(stock);
    return stock;
}

Option Account::updateOptionPosition(const common::PortfolioPositionDTO &position)
{
    Option option(position.contractId, position.symbol, position.position, position.marketPrice, position.marketValue,
                position.averageCost, position.unrealizedPNL, position.realizedPNL, position.expiration,
                  position.strike, position.right, position.multiplier);
    _portfolio.updatePortfolio(option);
    return option;
}


Future Account::updateFuturePosition(const common::PortfolioPositionDTO &position)
{
    Future future(position.contractId, position.symbol, position.position, position.marketPrice, position.marketValue,
                position.averageCost, position.unrealizedPNL, position.realizedPNL, position.expiration);
    _portfolio.updatePortfolio(future);
    return future;
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

void Account::updatePortfolioPosition(const common::PortfolioPositionDTO &position)
{
    switch(position.securityType) {
    case common::SecurityType::STOCK: {
        Stock stock = updateStockPosition(position);
        qDebug() << "updatePortfolioPosition stock" << stock.symbol();
        emit stockPositionUpdated(stock);
        break;
    }
    case common::SecurityType::OPTION: {
        Option option = updateOptionPosition(position);
        qDebug() << "updatePortfolioPosition option" << option.symbol();
        emit optionPositionUpdated(option);
        break;
    }
    case common::SecurityType::FUTURE: {
        Future future = updateFuturePosition(position);
        qDebug() << "updatePortfolioPosition future" << future.symbol();
        emit futurePositionUpdated(future);
        break;
    }
    default:
        break;
    }
}


}
