#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <twsclient.h>
#include <twsreaderthread.h>
#include <QSqlDatabase>
#include <datamanager.h>

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
    void disconnectClient();
    void quit();
    void accountInfoUpdated(AccountInfoType type);

private slots:
    void init();

private:
    bool setupDatabase();

    Ui::MainWindow *ui;
    TwsClient * client;
    TwsReaderThread * readerThread;
    QSqlDatabase _db;
    data::DataManager _dataManager;
    QApplication * _app;

};
#endif // MAINWINDOW_H
