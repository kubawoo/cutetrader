#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <QObject>
#include <QStatusBar>
#include <QLabel>
#include <QTime>

class StatusBar : public QObject
{
    Q_OBJECT
public:
    explicit StatusBar(QStatusBar* statusBar);
    ~StatusBar();

public slots:
    void setAccount(const QString & account);
    void setUpdateTime(const QTime & time);
    void showMessage(const QString &text, int timeout = 0);
    void clearMessage();

private:
    QStatusBar * _statusBar;
    QLabel * _accountLabel;
    QLabel * _updateTimeLabel;

};

#endif // STATUSBAR_H
