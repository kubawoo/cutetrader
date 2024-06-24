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
    TwsClient * client;
    QTimer * timer;
};

#endif // TWSREADERTHREAD_H
