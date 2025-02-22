#ifndef WATCHLISTTAB_H
#define WATCHLISTTAB_H

#include <QWidget>
#include <data.h>
#include "addsecuritydialog.h"

namespace Ui {
class WatchlistTab;
}

class WatchlistTab : public QWidget
{
    Q_OBJECT

public:
    explicit WatchlistTab(QSharedPointer<common::ITwsClient> client, data::DataManager * dataManager, QWidget *parent = nullptr);
    ~WatchlistTab();
    void init();

public slots:
    void securityDetailsReady(int reqId, const QList<common::ContractDetailsDTO> &details);

private:
    void reloadSecurities();

private slots:
    void addSecurity();
    void securityAdded(const common::ContractDetailsDTO &details);
    void deleteSecurity();
    void securitySelected(int row);

private:
    Ui::WatchlistTab *_ui;
    QSharedPointer<common::ITwsClient> _client;
    data::DataManager * _dataManager;
    AddSecurityDialog * _addSecurityDialog;
    int _reqId;
};

#endif // WATCHLISTTAB_H
