#ifndef ACCOUNT_ACCOUNT_H
#define ACCOUNT_ACCOUNT_H

#include "accountinfo.h"
#include "portfolio.h"
#include <common/types.h>

namespace account {

class Account : public QObject
{
        Q_OBJECT
public:
    explicit Account(QObject * parent = nullptr);

    double accountInfo(AccountInfoType type);
    QString baseCurrency();
    Portfolio &portfolio() { return _portfolio; }
    const QList<common::OrderDTO> &openOrders() const { return _openOrders; }

public slots:
    void updateAccountValue(const QString & key, const QString & value, const QString & currency);
    void updatePortfolioPosition(const common::PortfolioPositionDTO & position);
    void updateOpenOrders(const int &requestId, const QList<common::OrderDTO> & orders);
    void setBaseCurrency(const QString & currency);

signals:
    void accountValueUpdated(AccountInfoType type, double value);
    void stockPositionUpdated(const Stock & stock);
    void optionPositionUpdated(const Option & option);
    void futurePositionUpdated(const Future & future);
    void openOrdersUpdated(const QList<common::OrderDTO> & orders);

private:
    Stock updateStockPosition(const common::PortfolioPositionDTO & position);
    Option updateOptionPosition(const common::PortfolioPositionDTO & position);
    Future updateFuturePosition(const common::PortfolioPositionDTO & position);

    QString _baseCurrency;
    AccountInfo _accountInfo;
    Portfolio _portfolio;
    QList<common::OrderDTO> _openOrders;
};

}

#endif
