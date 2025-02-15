#include "connectdialog.h"
#include "ui_connectdialog.h"
#include <QPushButton>

ConnectDialog::ConnectDialog(twsclient::ITwsClient *client, QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::ConnectDialog),
    _client(client)
{
    _ui->setupUi(this);
    _ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    QPushButton * connectPushButton = _ui->buttonBox->addButton("Connect", QDialogButtonBox::HelpRole);
    connect(connectPushButton, &QPushButton::clicked, this, &ConnectDialog::connectClient);
    connect(_client, &twsclient::ITwsClient::managedAccountsSignal, this, &ConnectDialog::managedAccounts);
    connect(this, &ConnectDialog::accepted, this, &ConnectDialog::accountSelected);
}

ConnectDialog::~ConnectDialog()
{
    delete _ui;
}

void ConnectDialog::connectClient() {
    const QString &host = _ui->hostLineEdit->text();
    int port = _ui->portSpinBox->value();
    bool connected = _client->connect(host, port);
    if (connected) {
      _ui->accountComboBox->setEnabled(true);
    } else {
        qDebug() << "Failed to connect";
    }
}

void ConnectDialog::managedAccounts(const QStringList accounts) {
    _ui->accountComboBox->addItems(accounts);
    _ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
}

void ConnectDialog::accountSelected() {
    emit accountSelectedSignal(_ui->accountComboBox->currentText());
}
