#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qtablewidget.h"
#include <QMainWindow>
#include <QLabel>
#include <twsclient.h>
#include <QSqlDatabase>
#include <data.h>
#include <account.h>
#include "connectdialog.h"
#include "addsecuritydialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QApplication *a, QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void clientConnected(const QString & accountId);
    void quit();
    void accountInfoUpdated(account::AccountInfoType type, double value);
    void stockPositionUpdated(const account::Stock & stock);
    void optionPositionUpdated(const account::Option & option);
    void futurePositionUpdated(const account::Future & future);
    void updateAccountTime(const QTime & time);

private slots:
    void init();
    void addSecurity();
    void securityAdded(const common::ContractDetailsDTO &details);
    void deleteSecurity();

private:
    bool setupDatabase();
    int findExistingRow(QTableWidget * table, long contractId);
    QString toString(double x);
    void reloadSecurities();

    Ui::MainWindow *_ui;
    twsclient::TwsClient * _client;
    twsclient::TwsReaderThread * _readerThread;
    QSqlDatabase _db;
    data::DataManager _dataManager;
    account::Account _account;
    QApplication * _app;
    ConnectDialog *_connectDialog;
    AddSecurityDialog * _addSecurityDialog;
    QLabel * _statusBarAccount;
    QLabel * _statusBarAccountUpdateTime;

};
#endif // MAINWINDOW_H
