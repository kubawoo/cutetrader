#ifndef ACCOUNT_ACCOUNT_H
#define ACCOUNT_ACCOUNT_H

#include "accountinfo.h"
#include "portfolio.h"

namespace account {

class Account : public QObject
{
        Q_OBJECT
public:
    explicit Account(QObject * parent = nullptr);

    double accountInfo(AccountInfoType type);
    QString baseCurrency();


public slots:
    void updateAccountValue(const QString & key, const QString & value, const QString & currency);
    void updatePortfolioPosition(const common::PortfolioPositionDTO & position);
    void setBaseCurrency(const QString & currency);

signals:
    void accountValueUpdated(AccountInfoType type, double value);
    void stockPositionUpdated(const Stock & stock);
    void optionPositionUpdated(const Option & option);
    void futurePositionUpdated(const Future & future);

private:
    Stock updateStockPosition(const common::PortfolioPositionDTO & position);
    Option updateOptionPosition(const common::PortfolioPositionDTO & position);
    Future updateFuturePosition(const common::PortfolioPositionDTO & position);

    QString _baseCurrency;
    AccountInfo _accountInfo;
    Portfolio _portfolio;
};

}

#endif
