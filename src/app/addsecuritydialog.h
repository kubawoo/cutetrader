#ifndef ADDSECURITYDIALOG_H
#define ADDSECURITYDIALOG_H

#include <QDialog>
#include <common.h>

namespace Ui {
class AddSecurityDialog;
}

class AddSecurityDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddSecurityDialog(QSharedPointer<common::ITwsClient> client, QWidget *parent = nullptr);
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
    QSharedPointer<common::ITwsClient> _client;
    QList<common::ContractDetailsDTO> _securities;
    int _reqId;
};

#endif // ADDSECURITYDIALOG_H
