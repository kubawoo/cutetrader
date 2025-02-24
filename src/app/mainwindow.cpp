#include <QException>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QTimer>
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "watchlisttab.h"

MainWindow::MainWindow(QApplication * app, QSharedPointer<common::ITwsClient> client, QWidget *parent)
    : QMainWindow(parent),
      _app(app),
      _ui(new Ui::MainWindow),
      _client(client),
      _connectDialog(new ConnectDialog(_client, this))
{
    qDebug() << "Constructing" << this;
    this->setEnabled(false);
    _ui->setupUi(this);
    _statusBar = new StatusBar(_ui->statusbar);

    connect(_client.get(), &common::ITwsClient::accountValueUpdatedSignal, &_account, &account::Account::updateAccountValue);
    connect(_client.get(), &common::ITwsClient::portfolioPositionUpdatedSignal, &_account, &account::Account::updatePortfolioPosition);
    connect(_client.get(), &common::ITwsClient::updateAccountTimeSignal, this, &MainWindow::updateAccountTime);
    connect(_client.get(), &common::ITwsClient::updateAccountTimeSignal, _statusBar, &StatusBar::setUpdateTime);
    connect(_connectDialog, &ConnectDialog::accountSelectedSignal, _statusBar, &StatusBar::setAccount);

    connect(_connectDialog, &ConnectDialog::accountSelectedSignal, this, &MainWindow::clientConnected);
    connect(_connectDialog, &ConnectDialog::rejected, this, &MainWindow::close);

    QSqlDatabase db = QSqlDatabase::database();
    _dataManager = data::DataManager(db);

    QTimer::singleShot(0, this, &MainWindow::init);
}

MainWindow::~MainWindow()
{
    qDebug() << "Destroying" << this;
    delete _ui;
}

void MainWindow::clientConnected(const QString & accountId) {
    this->setEnabled(true);
    _statusBar->showMessage("Connected", 5000);
    _client->startClient(accountId);
}


void MainWindow::quit()
{
    qDebug() << "Quiting...";
    _client->disconnect();
    _dataManager.close();
}


void MainWindow::updateAccountTime(const QTime & time)
{
}

void MainWindow::init()
{
    _connectDialog->setEnabled(true);
    _connectDialog->show();

    _ui->mainTabWidget->removeTab(0);
    _portfolioTab = new PortfolioTab(&_account, this);
    _ui->mainTabWidget->addTab(_portfolioTab, "Portfolio");
    _portfolioTab->init();

    _watchlistTab = new WatchlistTab(_client, &_dataManager, this);
    _ui->mainTabWidget->addTab(_watchlistTab, "Watchlist");
    _watchlistTab->init();
}


