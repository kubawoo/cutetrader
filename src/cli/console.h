#ifndef CONSOLE_H
#define CONSOLE_H

#include <QObject>
#include <QSocketNotifier>
#include <QTextStream>

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
    void printPrompt();

private:
    QTextStream _inputStream;
    QTextStream _outputStream;
    QSocketNotifier *_notifier;

signals:
};

#endif // CONSOLE_H
