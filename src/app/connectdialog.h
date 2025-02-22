#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QDialog>
#include <QStringList>
#include <common.h>

namespace Ui {
class ConnectDialog;
}

class ConnectDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ConnectDialog(QSharedPointer<common::ITwsClient> client, QWidget *parent = nullptr);
    ~ConnectDialog();

private slots:
    void connectClient();
    void managedAccounts(const QStringList accounts);
    void accountSelected();

signals:
    void accountSelectedSignal(const QString & accountId);

private:
    Ui::ConnectDialog *_ui;
    QSharedPointer<common::ITwsClient> _client;
};

#endif // CONNECTDIALOG_H
