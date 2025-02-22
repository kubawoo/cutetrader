#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QSqlDatabase>
#include <data.h>
#include <account.h>
#include "connectdialog.h"
#include "watchlisttab.h"
#include "portfoliotab.h"
#include "statusbar.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QApplication *a, QSharedPointer<common::ITwsClient> client, QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void clientConnected(const QString & accountId);
    void quit();
    void updateAccountTime(const QTime & time);

private slots:
    void init();

private:
    Ui::MainWindow *_ui;
    QSharedPointer<common::ITwsClient> _client;
    data::DataManager _dataManager;
    account::Account _account;
    QApplication * _app;
    ConnectDialog *_connectDialog;
    StatusBar * _statusBar;

    PortfolioTab * _portfolioTab;
    WatchlistTab * _watchlistTab;
};
#endif // MAINWINDOW_H
