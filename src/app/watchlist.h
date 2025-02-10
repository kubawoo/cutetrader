#ifndef WATCHLIST_H
#define WATCHLIST_H

#include <QWidget>

namespace Ui {
class Watchlist;
}

class Watchlist : public QWidget
{
    Q_OBJECT

public:
    explicit Watchlist(QWidget *parent = nullptr);
    ~Watchlist();

private:
    Ui::Watchlist *ui;
};

#endif // WATCHLIST_H
