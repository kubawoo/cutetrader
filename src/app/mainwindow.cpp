#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
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
