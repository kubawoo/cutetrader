#ifndef CONSOLE_H
#define CONSOLE_H

#include <QObject>
#include <QSocketNotifier>

class Console : public QObject
{
    Q_OBJECT
public:
    explicit Console(QObject *parent = nullptr);

public slots:
    void print(const QString &txt);
signals:
    void newInput(const QString &input);
private slots:
    void readLine();

private:
    QSocketNotifier *_notifier;

signals:
};

#endif // CONSOLE_H
