#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <dbbuilder.h>
#include <QException>
#include <QMessageBox>
#include <QSqlDatabase>

MainWindow::MainWindow(QApplication * app, QWidget *parent)
    : QMainWindow(parent), _app(app),
      _ui(new Ui::MainWindow),
      _client(new twsclient::TwsClient),
      _readerThread(new twsclient::TwsReaderThread(_client))
{
    _client->moveToThread(_readerThread);
    _ui->setupUi(this);

    connect(_ui->connectPushButton, &QPushButton::clicked, this, &MainWindow::connectClient);
    connect(_ui->disconnectPushButton, &QPushButton::clicked, this, &MainWindow::disconnectClient);
    connect(_ui->checkTimePushButton, &QPushButton::clicked, _client, &twsclient::TwsClient::requestCurrentTime);
    connect(_client, &twsclient::TwsClient::accountValueUpdatedSignal, &_account, &account::Account::updateAccountValue);
    connect(_client, &twsclient::TwsClient::managedAccountSignal, &_account, &account::Account::setAccountId);
    connect(&_account, &account::Account::accountValueUpdated, this, &MainWindow::accountInfoUpdated);

    _readerThread->start();

    QTimer::singleShot(0, this, &MainWindow::init);
}

MainWindow::~MainWindow()
{
    delete _ui;
}

void MainWindow::connectClient() {
  const QString &host = _ui->hostLineEdit->text();
  int port = _ui->portSpinBox->value();
  bool connected = _client->connect(host, port);
  if (connected) {
    _ui->connectPushButton->setText("Connected");
    _ui->connectPushButton->setEnabled(false);
    _ui->hostLineEdit->setEnabled(false);
    _ui->portSpinBox->setEnabled(false);
    _ui->checkTimePushButton->setEnabled(true);
  } else {
    qDebug() << "Failed to connect";
  }
}

void MainWindow::disconnectClient() {
    _client->disconnect();
    _ui->connectPushButton->setText("Connect");
    _ui->connectPushButton->setEnabled(true);
    _ui->hostLineEdit->setEnabled(true);
    _ui->portSpinBox->setEnabled(true);
    _ui->checkTimePushButton->setEnabled(false);
}

void MainWindow::quit()
{
    qDebug() << "Quiting...";
    _readerThread->quit();
    _readerThread->wait(1000);
    delete _readerThread;
    _client->disconnect();
    delete _client;
    _db.close();
}

void MainWindow::accountInfoUpdated(account::AccountInfoType type, double value)
{
    switch(type) {
    case account::AccountInfoType::NetLiquidation:
        _ui->netLiquidation->setText(QString::number(value));
        break;
    default:
        break;
    }
}

void MainWindow::init()
{
    if(!setupDatabase()) {
        auto ret = QMessageBox::critical(this, "Error", "Failed to initialize database",
                                         QMessageBox::StandardButton::Close);
        _app->quit();
    }
}

bool MainWindow::setupDatabase()
{
    _db = QSqlDatabase::addDatabase("QSQLITE");
    _db.setDatabaseName("cutetrader.db");
    _db.open();

    _dataManager = data::DataManager(_db);
    data::DbBuilder dbBuilder(_db);
    return dbBuilder.runMigrations();
}
