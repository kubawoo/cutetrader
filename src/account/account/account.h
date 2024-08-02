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
    QString accountId();
    QString baseCurrency();


public slots:
    void updateAccountValue(const QString & key, const QString & value, const QString & currency);
    void updatePortfolioPosition(const common::PortfolioPositionDTO & position);
    void setAccountId(const QString & accountId);
    void setBaseCurrency(const QString & currency);

signals:
    void accountValueUpdated(AccountInfoType type, double value);
    void stockPositionUpdated(const Stock & stock);
    void optionPositionUpdated(const Option & option);

private:
    Stock updateStockPosition(const common::PortfolioPositionDTO & position);
    Option updateOptionPosition(const common::PortfolioPositionDTO & position);


    QString _accountId;
    QString _baseCurrency;
    AccountInfo _accountInfo;
    Portfolio _portfolio;
};

}

#endif
