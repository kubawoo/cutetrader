#include "watchlist.h"
#include "ui_watchlist.h"

Watchlist::Watchlist(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Watchlist)
{
    ui->setupUi(this);
}

Watchlist::~Watchlist()
{
    delete ui;
}
