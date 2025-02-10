#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "watchlisttab.h"
#include <QException>
#include <QMessageBox>
#include <QSqlDatabase>

MainWindow::MainWindow(QApplication * app, QWidget *parent)
    : QMainWindow(parent), _app(app),
      _ui(new Ui::MainWindow),
      _client(new twsclient::TwsClient),
      _readerThread(new twsclient::TwsReaderThread(_client)),
      _connectDialog(new ConnectDialog(_client, this)),
      _statusBarAccount(new QLabel),
      _statusBarAccountUpdateTime(new QLabel)
{
    this->setEnabled(false);
    _client->moveToThread(_readerThread);
    _ui->setupUi(this);
    _ui->stocksTableWidget->setColumnHidden(0, true);
    _ui->optionsTableWidget->setColumnHidden(0, true);
    _ui->futuresTableWidget->setColumnHidden(0, true);

    connect(_client, &twsclient::TwsClient::accountValueUpdatedSignal, &_account, &account::Account::updateAccountValue);
    connect(_client, &twsclient::TwsClient::portfolioPositionUpdatedSignal, &_account, &account::Account::updatePortfolioPosition);
    connect(_client, &twsclient::TwsClient::updateAccountTimeSignal, this, &MainWindow::updateAccountTime);

    connect(&_account, &account::Account::accountValueUpdated, this, &MainWindow::accountInfoUpdated);
    connect(&_account, &account::Account::stockPositionUpdated, this, &MainWindow::stockPositionUpdated);
    connect(&_account, &account::Account::optionPositionUpdated, this, &MainWindow::optionPositionUpdated);
    connect(&_account, &account::Account::futurePositionUpdated, this, &MainWindow::futurePositionUpdated);
    connect(_connectDialog, &ConnectDialog::accountSelectedSignal, this, &MainWindow::clientConnected);
    connect(_connectDialog, &ConnectDialog::rejected, this, &MainWindow::close);


    _ui->statusbar->addPermanentWidget(_statusBarAccount);
    _ui->statusbar->addPermanentWidget(_statusBarAccountUpdateTime);

    _readerThread->start();
    QTimer::singleShot(0, this, &MainWindow::init);
}

MainWindow::~MainWindow()
{
    delete _ui;
    delete _connectDialog;
}

void MainWindow::clientConnected(const QString & accountId) {
    this->setEnabled(true);
    _statusBarAccount->setText("Account: " + accountId);
    _ui->statusbar->showMessage("Connected", 5000);
    _client->startClient(accountId);
}


void MainWindow::quit()
{
    qDebug() << "Quiting...";
    _readerThread->quit();
    _readerThread->wait(1000);
    delete _readerThread;
    _client->disconnect();
    delete _client;
    _dataManager.close();
}

void MainWindow::accountInfoUpdated(account::AccountInfoType type, double value)
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
    _ui->stocksTableWidget->setItem(row, 3, new QTableWidgetItem(toString(stock.marketValue())));
    _ui->stocksTableWidget->setItem(row, 4, new QTableWidgetItem(toString(stock.unrealizedPNL())));
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
    _ui->optionsTableWidget->setItem(row, 4, new QTableWidgetItem(toString(option.marketValue())));
    _ui->optionsTableWidget->setItem(row, 5, new QTableWidgetItem(toString(option.unrealizedPNL())));
}

void MainWindow::futurePositionUpdated(const account::Future &future)
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

void MainWindow::updateAccountTime(const QTime & time)
{
    _statusBarAccountUpdateTime->setText("Last update: " + time.toString("HH:mm"));
}

void MainWindow::init()
{
    if(!setupDatabase()) {
        auto ret = QMessageBox::critical(this, "Error", "Failed to initialize database",
                                         QMessageBox::StandardButton::Close);
        _app->quit();
    }

    _connectDialog->setEnabled(true);
    _connectDialog->show();

    _watchlistTab = new WatchlistTab(_client, &_dataManager, this);
    _ui->mainTabWidget->addTab(_watchlistTab, "Watchlist");
    _watchlistTab->init();
}



bool MainWindow::setupDatabase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("cutetrader.db");
    db.open();

    _dataManager = data::DataManager(db);
    data::DbBuilder dbBuilder(db);
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

QString MainWindow::toString(double x)
{
    return QString::number(x, 'f', 2);
}


