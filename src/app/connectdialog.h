#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QDialog>
#include <QStringList>
#include <twsclient.h>

namespace Ui {
class ConnectDialog;
}

class ConnectDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ConnectDialog(twsclient::TwsClient * client, QWidget *parent = nullptr);
    ~ConnectDialog();

private slots:
    void connectClient();
    void managedAccounts(const QStringList accounts);
    void accountSelected();

signals:
    void accountSelectedSignal(const QString & accountId);

private:
    Ui::ConnectDialog *_ui;
    twsclient::TwsClient * _client;
};

#endif // CONNECTDIALOG_H
