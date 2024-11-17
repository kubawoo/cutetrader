#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qtablewidget.h"
#include <QMainWindow>
#include <twsclient.h>
#include <QSqlDatabase>
#include <data.h>
#include <account.h>
#include "connectdialog.h"

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
    void connectClient();
    void quit();
    void accountInfoUpdated(account::AccountInfoType type, double value);
    void stockPositionUpdated(const account::Stock & stock);
    void optionPositionUpdated(const account::Option & option);

private slots:
    void init();

private:
    bool setupDatabase();
    int findExistingRow(QTableWidget * table, long contractId);

    Ui::MainWindow *_ui;
    twsclient::TwsClient * _client;
    twsclient::TwsReaderThread * _readerThread;
    QSqlDatabase _db;
    data::DataManager _dataManager;
    account::Account _account;
    QApplication * _app;
    ConnectDialog *_connectDialog;

};
#endif // MAINWINDOW_H
