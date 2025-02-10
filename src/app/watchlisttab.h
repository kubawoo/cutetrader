#ifndef WATCHLISTTAB_H
#define WATCHLISTTAB_H

#include <QWidget>
#include <data.h>
#include <twsclient.h>
#include "addsecuritydialog.h"

namespace Ui {
class WatchlistTab;
}

class WatchlistTab : public QWidget
{
    Q_OBJECT

public:
    explicit WatchlistTab(twsclient::TwsClient * client, data::DataManager * dataManager, QWidget *parent = nullptr);
    ~WatchlistTab();
    void init();

private:
    void reloadSecurities();

private slots:
    void addSecurity();
    void securityAdded(const common::ContractDetailsDTO &details);
    void deleteSecurity();

private:
    Ui::WatchlistTab *_ui;
    twsclient::TwsClient * _client;
    data::DataManager * _dataManager;
    AddSecurityDialog * _addSecurityDialog;
};

#endif // WATCHLISTTAB_H
