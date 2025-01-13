#include "addsecuritydialog.h"
#include "ui_addsecuritydialog.h"

AddSecurityDialog::AddSecurityDialog(twsclient::TwsClient * client, QWidget *parent) :
    _client(client),
    QDialog(parent),
    _ui(new Ui::AddSecurityDialog)
{
    _ui->setupUi(this);

    connect(_ui->searchButton, &QPushButton::clicked, this, &AddSecurityDialog::search);
    connect(_client, &twsclient::TwsClient::matchingSymbolsReadySignal, this, &AddSecurityDialog::symbolsFound);
    connect(_ui->securitiesList, &QListWidget::currentRowChanged, this, &AddSecurityDialog::symbolChanged);
    connect(this, &AddSecurityDialog::accepted, this, &AddSecurityDialog::addSymbol);
}

AddSecurityDialog::~AddSecurityDialog()
{
    delete _ui;
}

void AddSecurityDialog::search() {
    QString pattern = _ui->searchEdit->text();
    if(!pattern.isEmpty()) {
        _client->requestMatchingSymbols(pattern);
    }
}

void AddSecurityDialog::symbolsFound(int reqId, const QList<common::ContractDetailsDTO> &securities)
{
    _ui->securitiesList->clear();
    _securities = securities;
    for(auto s : _securities) {
        _ui->securitiesList->addItem(s.symbol);
    }

    if(_ui->securitiesList->count() > 0) {
        _ui->securitiesList->setCurrentRow(0);
    }
}

void AddSecurityDialog::addSymbol()
{
    auto selected = _ui->securitiesList->currentRow();
    if(selected >= 0) {
        qDebug() << "Adding security " << selected;
        emit addSecuritySignal(_securities.at(selected));
    }
}

void AddSecurityDialog::symbolChanged(int i)
{
    common::ContractDetailsDTO details = _securities.at(i);
    _ui->idLabel->setText(QString::number(details.contractId));
    _ui->symbolLabel->setText(details.symbol);
    _ui->currencyLabel->setText(details.currency);
    _ui->descriptionLabel->setText(details.description);
}
