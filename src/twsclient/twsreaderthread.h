#ifndef TWSREADERTHREAD_H
#define TWSREADERTHREAD_H

#include <QThread>
#include <QObject>
#include <QTimer>
#include "twsclient.h"

class TwsReaderThread : public QThread
{
    Q_OBJECT
public:
    explicit TwsReaderThread(TwsClient * client);
    ~TwsReaderThread();
    void run();

private:
    QTimer * setupTimer(int msec, void (TwsClient::*funcPtr)(void));

    TwsClient * _client;
    QTimer * _readTimer;
    QTimer * _cleanupTimer;
};

#endif // TWSREADERTHREAD_H
