#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <dbbuilder.h>
#include <QException>
#include <QMessageBox>
#include <QSqlDatabase>

MainWindow::MainWindow(QApplication * app, QWidget *parent)
    : QMainWindow(parent), _app(app),
      ui(new Ui::MainWindow),
      client(new TwsClient),
      readerThread(new TwsReaderThread(client))
{
    client->moveToThread(readerThread);
    ui->setupUi(this);

    connect(ui->connectPushButton, &QPushButton::clicked, this, &MainWindow::connectClient);
    connect(ui->disconnectPushButton, &QPushButton::clicked, this, &MainWindow::disconnectClient);
    connect(ui->checkTimePushButton, &QPushButton::clicked, client, &TwsClient::requestCurrentTime);
    connect(client, &TwsClient::accountInfoUpdated, this, &MainWindow::accountInfoUpdated);

    readerThread->start();

    QTimer::singleShot(0, this, &MainWindow::init);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectClient() {
  const QString &host = ui->hostLineEdit->text();
  int port = ui->portSpinBox->value();
  bool connected = client->connect(host, port);
  if (connected) {
    ui->connectPushButton->setText("Connected");
    ui->connectPushButton->setEnabled(false);
    ui->hostLineEdit->setEnabled(false);
    ui->portSpinBox->setEnabled(false);
    ui->checkTimePushButton->setEnabled(true);
  } else {
    qDebug() << "Failed to connect";
  }
}

void MainWindow::disconnectClient() {
    client->disconnect();
    ui->connectPushButton->setText("Connect");
    ui->connectPushButton->setEnabled(true);
    ui->hostLineEdit->setEnabled(true);
    ui->portSpinBox->setEnabled(true);
    ui->checkTimePushButton->setEnabled(false);
}

void MainWindow::quit()
{
    qDebug() << "Quiting...";
    readerThread->quit();
    readerThread->wait(1000);
    delete readerThread;
    client->disconnect();
    delete client;
    _db.close();
}

void MainWindow::accountInfoUpdated(AccountInfoType type)
{
    switch(type) {
    case AccountInfoType::NetLiquidation:
        ui->netLiquidation->setText(QString::number(client->accountInfo(AccountInfoType::NetLiquidation)));
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
