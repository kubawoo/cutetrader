#include "mainwindow.h"
#include "./ui_mainwindow.h"
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
    _ui->stocksTableWidget->setColumnHidden(0, true);
    _ui->optionsTableWidget->setColumnHidden(0, true);

    connect(_ui->connectPushButton, &QPushButton::clicked, this, &MainWindow::connectClient);
    connect(_ui->disconnectPushButton, &QPushButton::clicked, this, &MainWindow::disconnectClient);
    connect(_ui->checkTimePushButton, &QPushButton::clicked, _client, &twsclient::TwsClient::requestCurrentTime);
    connect(_client, &twsclient::TwsClient::accountValueUpdatedSignal, &_account, &account::Account::updateAccountValue);
    connect(_client, &twsclient::TwsClient::managedAccountSignal, &_account, &account::Account::setAccountId);
    connect(_client, &twsclient::TwsClient::portfolioPositionUpdatedSignal, &_account, &account::Account::updatePortfolioPosition);
    connect(&_account, &account::Account::accountValueUpdated, this, &MainWindow::accountInfoUpdated);
    connect(&_account, &account::Account::stockPositionUpdated, this, &MainWindow::stockPositionUpdated);
    connect(&_account, &account::Account::optionPositionUpdated, this, &MainWindow::optionPositionUpdated);

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

void MainWindow::stockPositionUpdated(const account::Stock &stock)
{
    int row = findExistingRow(_ui->stocksTableWidget, stock.contractId());
    if(row < 0) {
        row = _ui->stocksTableWidget->rowCount();
        _ui->stocksTableWidget->setRowCount(row + 1);
    }

    _ui->stocksTableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(stock.contractId())));
    _ui->stocksTableWidget->setItem(row, 1, new QTableWidgetItem(stock.symbol()));
    _ui->stocksTableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(stock.position())));
    _ui->stocksTableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(stock.marketValue())));
}

void MainWindow::optionPositionUpdated(const account::Option &option)
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
    _ui->optionsTableWidget->setItem(row, 4, new QTableWidgetItem(QString::number(option.marketValue())));
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

int MainWindow::findExistingRow(QTableWidget *table, long contractId)
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
