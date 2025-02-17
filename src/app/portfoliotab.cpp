#include "portfoliotab.h"
#include "ui_portfoliotab.h"

PortfolioTab::PortfolioTab(twsclient::ITwsClient * client, account::Account* account, QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::PortfolioTab),
    _client(client)
{
    _ui->setupUi(this);
    _ui->stocksTableWidget->setColumnHidden(0, true);
    _ui->optionsTableWidget->setColumnHidden(0, true);
    _ui->futuresTableWidget->setColumnHidden(0, true);

    connect(account, &account::Account::accountValueUpdated, this, &PortfolioTab::accountInfoUpdated);
    connect(account, &account::Account::stockPositionUpdated, this, &PortfolioTab::stockPositionUpdated);
    connect(account, &account::Account::optionPositionUpdated, this, &PortfolioTab::optionPositionUpdated);
    connect(account, &account::Account::futurePositionUpdated, this, &PortfolioTab::futurePositionUpdated);
}

PortfolioTab::~PortfolioTab()
{
    delete _ui;
}

void PortfolioTab::init()
{

}

void PortfolioTab::accountInfoUpdated(account::AccountInfoType type, double value)
{
    switch(type) {
    case account::AccountInfoType::NetLiquidation:
        _ui->netLiquidation->setText(toString(value));
        break;
    case account::AccountInfoType::MaintMarginReq:
        _ui->maintananceMargin->setText(toString(value));
        break;
    case account::AccountInfoType::ExcessLiquidity:
        _ui->excessLiquidity->setText(toString(value));
        break;
    case account::AccountInfoType::CashBalance:
        _ui->cashBalance->setText(toString(value));
        break;
    case account::AccountInfoType::StockMarketValue:
        _ui->stocksValue->setText(toString(value));
        break;
    case account::AccountInfoType::OptionMarketValue:
        _ui->optionsValue->setText(toString(value));
        break;
    case account::AccountInfoType::FuturesPNL:
        _ui->futuresPNL->setText(toString(value));
        break;
    case account::AccountInfoType::RealizedPnL:
        _ui->realizedPNL->setText(toString(value));
        break;
    case account::AccountInfoType::UnrealizedPnL:
        _ui->unrealizedPNL->setText(toString(value));
        break;
    default:
        break;
    }
}

void PortfolioTab::stockPositionUpdated(const account::Stock &stock)
{
    int row = findExistingRow(_ui->stocksTableWidget, stock.contractId());
    if(row < 0) {
        row = _ui->stocksTableWidget->rowCount();
        _ui->stocksTableWidget->setRowCount(row + 1);
    }

    _ui->stocksTableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(stock.contractId())));
    _ui->stocksTableWidget->setItem(row, 1, new QTableWidgetItem(stock.symbol()));
    _ui->stocksTableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(stock.position())));
    _ui->stocksTableWidget->setItem(row, 3, new QTableWidgetItem(toString(stock.marketValue())));
    _ui->stocksTableWidget->setItem(row, 4, new QTableWidgetItem(toString(stock.unrealizedPNL())));
}

void PortfolioTab::optionPositionUpdated(const account::Option &option)
{
    int row = findExistingRow(_ui->optionsTableWidget, option.contractId());
    if(row < 0) {
        row = _ui->optionsTableWidget->rowCount();
        _ui->optionsTableWidget->setRowCount(row + 1);
    }

    QString symbol = QString("%1 %2@%3").arg(option.symbol()).arg(option.typeAsString()).arg(QString::number(option.strike()));

    _ui->optionsTableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(option.contractId())));
    _ui->optionsTableWidget->setItem(row, 1, new QTableWidgetItem(symbol));
    _ui->optionsTableWidget->setItem(row, 2, new QTableWidgetItem(option.expiration().toString(Qt::DateFormat::ISODate)));
    _ui->optionsTableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(option.position())));
    _ui->optionsTableWidget->setItem(row, 4, new QTableWidgetItem(toString(option.marketValue())));
    _ui->optionsTableWidget->setItem(row, 5, new QTableWidgetItem(toString(option.unrealizedPNL())));
}

void PortfolioTab::futurePositionUpdated(const account::Future &future)
{
    int row = findExistingRow(_ui->futuresTableWidget, future.contractId());
    if(row < 0) {
        row = _ui->futuresTableWidget->rowCount();
        _ui->futuresTableWidget->setRowCount(row + 1);
    }

    _ui->futuresTableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(future.contractId())));
    _ui->futuresTableWidget->setItem(row, 1, new QTableWidgetItem(future.symbol()));
    _ui->futuresTableWidget->setItem(row, 2, new QTableWidgetItem(future.expiration().toString(Qt::DateFormat::ISODate)));
    _ui->futuresTableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(future.position())));
    _ui->futuresTableWidget->setItem(row, 4, new QTableWidgetItem(toString(future.marketValue())));
    _ui->futuresTableWidget->setItem(row, 5, new QTableWidgetItem(toString(future.unrealizedPNL())));
}


int PortfolioTab::findExistingRow(QTableWidget *table, long contractId)
{
    QString id = QString::number(contractId);
    for(int row = 0; row < table->rowCount(); row++) {
        auto item = table->item(row, 0);
        if(id == item->text()) {
            return row;
        }
    }
    return -1;
}

QString PortfolioTab::toString(double x)
{
    return QString::number(x, 'f', 2);
}
