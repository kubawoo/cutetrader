#ifndef ADDSECURITYDIALOG_H
#define ADDSECURITYDIALOG_H

#include <QDialog>
#include <twsclient/itwsclient.h>
#include <common/types.h>

namespace Ui {
class AddSecurityDialog;
}

class AddSecurityDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddSecurityDialog(twsclient::ITwsClient * client, QWidget *parent = nullptr);
    ~AddSecurityDialog();

signals:
    void addSecuritySignal(const common::ContractDetailsDTO & details);

private slots:
    void search();
    void symbolsFound(int reqId, const QList<common::ContractDetailsDTO> & securities);
    void addSymbol();
    void symbolChanged(int i);

private:
    Ui::AddSecurityDialog *_ui;
    twsclient::ITwsClient * _client;
    QList<common::ContractDetailsDTO> _securities;
    int _reqId;
};

#endif // ADDSECURITYDIALOG_H
