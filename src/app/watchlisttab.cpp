#include "watchlisttab.h"
#include "ui_watchlisttab.h"


WatchlistTab::WatchlistTab(twsclient::TwsClient *client, data::DataManager *dataManager, QWidget *parent)
  : QWidget(parent),
    _ui(new Ui::WatchlistTab),
    _client(client),
    _dataManager(dataManager),
    _addSecurityDialog(new AddSecurityDialog(client, this))
{
    _ui->setupUi(this);
    _ui->watchlistTableWidget->setColumnHidden(0, true);

    connect(_ui->addPushButton, &QPushButton::clicked, this, &WatchlistTab::addSecurity);
    connect(_addSecurityDialog, &AddSecurityDialog::addSecuritySignal, this, &WatchlistTab::securityAdded);
    connect(_ui->deletePushButton, &QPushButton::clicked, this, &WatchlistTab::deleteSecurity);

}

WatchlistTab::~WatchlistTab()
{
    delete _addSecurityDialog;
    delete _ui;
}

void WatchlistTab::init()
{
    reloadSecurities();
}

void WatchlistTab::addSecurity()
{
    _addSecurityDialog->setEnabled(true);
    _addSecurityDialog->show();
}

void WatchlistTab::securityAdded(const common::ContractDetailsDTO &details)
{
    qDebug() << "securityAdded" << details.symbol;
    data::Security security;
    security.withSymbol(details.symbol).withContractId(details.contractId);
    _dataManager->createSecurity(security);
    reloadSecurities();
}

void WatchlistTab::deleteSecurity()
{
    int row = _ui->watchlistTableWidget->currentRow();
    qDebug() << "deleteSecurity" << "row" << row;

    if(row < 0) {
        return;
    }

    QString id = _ui->watchlistTableWidget->item(row, 0)->text();

    bool ok;
    int idInt = id.toInt(&ok);

    if(ok) {
        _dataManager->removeSecurity(idInt);
        reloadSecurities();
    }
}

void WatchlistTab::reloadSecurities()
{
    _ui->watchlistTableWidget->clearContents();
    _ui->watchlistTableWidget->setRowCount(0);
    QList<data::Security> securities = _dataManager->getAllSecurities();
    for(data::Security security : securities) {
        int row = _ui->watchlistTableWidget->rowCount();
        _ui->watchlistTableWidget->setRowCount(row + 1);

        _ui->watchlistTableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(security.id())));
        _ui->watchlistTableWidget->setItem(row, 1, new QTableWidgetItem(security.symbol()));
    }
}
