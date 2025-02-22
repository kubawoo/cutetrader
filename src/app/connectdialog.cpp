#include "connectdialog.h"
#include "ui_connectdialog.h"
#include <QPushButton>
#include <QMessageBox>

ConnectDialog::ConnectDialog(QSharedPointer<common::ITwsClient> client, QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::ConnectDialog),
    _client(client)
{
    qDebug() << "Constructing" << this;
    _ui->setupUi(this);
    _ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    QPushButton * connectPushButton = _ui->buttonBox->addButton("Connect", QDialogButtonBox::HelpRole);
    connect(connectPushButton, &QPushButton::clicked, this, &ConnectDialog::connectClient);
    connect(_client.get(), &common::ITwsClient::managedAccountsSignal, this, &ConnectDialog::managedAccounts);
    connect(this, &ConnectDialog::accepted, this, &ConnectDialog::accountSelected);
}

ConnectDialog::~ConnectDialog()
{
    qDebug() << "Destroying" << this;
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
        QMessageBox::critical(this, "Error", "Connecting to API failed.\n"
                                             "Check TWS/GW is running and accepting connections",
                              QMessageBox::StandardButton::Close);

    }
}

void ConnectDialog::managedAccounts(const QStringList accounts) {
    _ui->accountComboBox->addItems(accounts);
    _ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
}

void ConnectDialog::accountSelected() {
    emit accountSelectedSignal(_ui->accountComboBox->currentText());
}
