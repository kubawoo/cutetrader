#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "watchlisttab.h"
#include <QException>
#include <QMessageBox>
#include <QSqlDatabase>

MainWindow::MainWindow(QApplication * app, twsclient::ITwsClient *client, QWidget *parent)
    : QMainWindow(parent), _app(app),
      _ui(new Ui::MainWindow),
      _client(client),
      _connectDialog(new ConnectDialog(_client, this)),
      _statusBarAccount(new QLabel),
      _statusBarAccountUpdateTime(new QLabel)
{
    this->setEnabled(false);
    _ui->setupUi(this);


    connect(_client, &twsclient::ITwsClient::accountValueUpdatedSignal, &_account, &account::Account::updateAccountValue);
    connect(_client, &twsclient::ITwsClient::portfolioPositionUpdatedSignal, &_account, &account::Account::updatePortfolioPosition);
    connect(_client, &twsclient::ITwsClient::updateAccountTimeSignal, this, &MainWindow::updateAccountTime);

    connect(_connectDialog, &ConnectDialog::accountSelectedSignal, this, &MainWindow::clientConnected);
    connect(_connectDialog, &ConnectDialog::rejected, this, &MainWindow::close);

    _ui->statusbar->addPermanentWidget(_statusBarAccount);
    _ui->statusbar->addPermanentWidget(_statusBarAccountUpdateTime);

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
    _client->disconnect();
    delete _client;
    _dataManager.close();
}


void MainWindow::updateAccountTime(const QTime & time)
{
    _statusBarAccountUpdateTime->setText("Last update: " + time.toString("HH:mm"));
}

void MainWindow::init()
{
    if(!setupDatabase()) {
        QMessageBox::critical(this, "Error", "Failed to initialize database",
                                         QMessageBox::StandardButton::Close);
        _app->quit();
    }

    _connectDialog->setEnabled(true);
    _connectDialog->show();

    _ui->mainTabWidget->removeTab(0);
    _portfolioTab = new PortfolioTab(_client, &_account, this);
    _ui->mainTabWidget->addTab(_portfolioTab, "Portfolio");
    _portfolioTab->init();

    _watchlistTab = new WatchlistTab(_client, &_dataManager, this);
    _ui->mainTabWidget->addTab(_watchlistTab, "Watchlist");
    _watchlistTab->init();
}


bool MainWindow::setupDatabase()
{
    QSqlDatabase db = QSqlDatabase::database();
    _dataManager = data::DataManager(db);
    data::DbBuilder dbBuilder(db);
    return dbBuilder.runMigrations();
}

