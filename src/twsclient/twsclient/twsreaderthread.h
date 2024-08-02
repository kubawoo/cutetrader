#ifndef TWSCLIENT_TWSREADERTHREAD_H
#define TWSCLIENT_TWSREADERTHREAD_H

#include <QThread>
#include <QObject>
#include <QTimer>
#include "twsclient.h"

namespace twsclient {

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
}
#endif
