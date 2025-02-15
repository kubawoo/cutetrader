#include "addsecuritydialog.h"
#include "ui_addsecuritydialog.h"
#include <common/utils.h>

AddSecurityDialog::AddSecurityDialog(twsclient::ITwsClient * client, QWidget *parent) :
    _client(client),
    QDialog(parent),
    _ui(new Ui::AddSecurityDialog),
    _reqId(0)
{
    _ui->setupUi(this);

    connect(_ui->searchButton, &QPushButton::clicked, this, &AddSecurityDialog::search);
    connect(_client, &twsclient::ITwsClient::matchingSymbolsReadySignal, this, &AddSecurityDialog::symbolsFound);
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
        _reqId = _client->requestMatchingSymbols(pattern);
    }
}

void AddSecurityDialog::symbolsFound(int reqId, const QList<common::ContractDetailsDTO> &securities)
{
    if(_reqId != reqId) {
        qDebug() << "Got invalid reqId. Expected" << _reqId << "but got" << reqId;
        return;
    }

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
    if(i >= 0) {
        common::ContractDetailsDTO details = _securities.at(i);
        _ui->idLabel->setText(QString::number(details.contractId));
        _ui->symbolLabel->setText(details.symbol);
        _ui->currencyLabel->setText(details.currency);
        _ui->descriptionLabel->setText(details.description);
        _ui->typeLabel->setText(common::Utils::securityTypeToString(details.securityType));
        QStringList derivatives;
        for(auto d: details.derivatives) {
            derivatives.append(common::Utils::securityTypeToString(d));
        }
        _ui->derivativesLabel->setText(derivatives.join(", "));
    } else {
        _ui->idLabel->setText("");
        _ui->symbolLabel->setText("");
        _ui->currencyLabel->setText("");
        _ui->descriptionLabel->setText("");
        _ui->typeLabel->setText("");
        _ui->derivativesLabel->setText("");
    }
}
