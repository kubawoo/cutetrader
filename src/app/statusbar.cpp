#include "statusbar.h"
#include <QDebug>

StatusBar::StatusBar(QStatusBar* statusBar)
    : QObject{statusBar},
      _statusBar(statusBar),
      _accountLabel(new QLabel(statusBar)),
      _updateTimeLabel(new QLabel(statusBar))
{
    qDebug() << "Constructing" << this;
    statusBar->addPermanentWidget(_accountLabel);
    statusBar->addPermanentWidget(_updateTimeLabel);
}

StatusBar::~StatusBar()
{
    qDebug() << "Destroying" << this;
}

void StatusBar::setAccount(const QString &account)
{
    _accountLabel->setText("Account: " + account);
}

void StatusBar::setUpdateTime(const QTime &time)
{
    _updateTimeLabel->setText("Last update: " + time.toString("HH:mm"));
}

void StatusBar::showMessage(const QString &text, int timeout)
{
    _statusBar->showMessage(text, timeout);
}

void StatusBar::clearMessage()
{
    _statusBar->clearMessage();
}


