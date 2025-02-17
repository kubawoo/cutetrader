#ifndef TWSCLIENT_TWSREADERTHREAD_H
#define TWSCLIENT_TWSREADERTHREAD_H

#include <QThread>
#include <QObject>
#include <QTimer>


namespace twsclient {

class TwsClient;

class TwsReaderThread : public QThread
{
    Q_OBJECT
public:
    explicit TwsReaderThread(TwsClient * client);
    ~TwsReaderThread();
    void run();

private:
    QTimer * setupTimer(int msec, void (TwsClient::*funcPtr)());

    TwsClient * _client;
    QTimer * _readTimer;
    QTimer * _cleanupTimer;
};
}
#endif
