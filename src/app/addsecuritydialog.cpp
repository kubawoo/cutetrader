#include "addsecuritydialog.h"
#include "ui_addsecuritydialog.h"
#include <common/utils.h>

AddSecurityDialog::AddSecurityDialog(QSharedPointer<common::ITwsClient> client, QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::AddSecurityDialog),
    _client(client),
    _securities(QList<common::ContractDetailsDTO>()),
    _reqId(0)
{
    qDebug() << "Constructing" << this;
    _ui->setupUi(this);

    connect(_ui->searchButton, &QPushButton::clicked, this, &AddSecurityDialog::search);
    connect(_client.get(), &common::ITwsClient::matchingSymbolsReadySignal, this, &AddSecurityDialog::symbolsFound);
    connect(_ui->securitiesList, &QListWidget::currentRowChanged, this, &AddSecurityDialog::symbolChanged);
    connect(this, &AddSecurityDialog::accepted, this, &AddSecurityDialog::addSymbol);
}

AddSecurityDialog::~AddSecurityDialog()
{
    qDebug() << "Destroying" << this;
    delete _ui;
}

void AddSecurityDialog::search() {
    QString pattern = _ui->searchEdit->text();
    if(!pattern.isEmpty()) {
        _client->requestMatchingSymbols(pattern, &_reqId);
    }
}

void AddSecurityDialog::symbolsFound(int reqId, const QList<common::ContractDetailsDTO> &securities)
{
    if(_reqId != reqId) {
        qDebug() << "AddSecurityDialog::symbolsFound" << "Got invalid reqId. Expected" << _reqId << "but got" << reqId;
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
        _ui->exchangeLabel->setText(details.primaryExchange);
        QStringList derivatives;
        for(auto d: details.derivatives) {
            derivatives.append(common::Utils::securityTypeToString(d));
        }
        if(derivatives.empty()) {
            _ui->derivativesLabel->setText("N/A");
        } else {
            _ui->derivativesLabel->setText(derivatives.join(", "));
        }
    } else {
        _ui->idLabel->setText("");
        _ui->symbolLabel->setText("");
        _ui->currencyLabel->setText("");
        _ui->descriptionLabel->setText("");
        _ui->typeLabel->setText("");
        _ui->derivativesLabel->setText("");
    }
}
