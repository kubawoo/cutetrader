#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <twsclient.h>
#include <twsreaderthread.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void connectClient();
    void disconnectClient();
    void quit();
    void accountInfoUpdated(AccountInfoType type);

private:
    Ui::MainWindow *ui;
    TwsClient * client;
    TwsReaderThread * readerThread;
};
#endif // MAINWINDOW_H
