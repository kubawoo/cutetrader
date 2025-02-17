#ifndef PORTFOLIOTAB_H
#define PORTFOLIOTAB_H

#include <QWidget>
#include <QTableWidget>
#include <twsclient/itwsclient.h>
#include <account.h>

namespace Ui {
class PortfolioTab;
}

class PortfolioTab : public QWidget
{
    Q_OBJECT

public:
    explicit PortfolioTab(twsclient::ITwsClient * client, account::Account * account, QWidget *parent = nullptr);
    ~PortfolioTab();
    void init();

private slots:
    void accountInfoUpdated(account::AccountInfoType type, double value);
    void stockPositionUpdated(const account::Stock & stock);
    void optionPositionUpdated(const account::Option & option);
    void futurePositionUpdated(const account::Future & future);

private:
    int findExistingRow(QTableWidget * table, long contractId);
    QString toString(double x);

private:
    Ui::PortfolioTab *_ui;
    twsclient::ITwsClient * _client;
};

#endif // PORTFOLIOTAB_H
