#ifndef IBCLIENTREADERTHREAD_H
#define IBCLIENTREADERTHREAD_H

#include <QThread>
#include <QObject>
#include <QTimer>
#include "ibclient.h"

class IBClientReaderThread : public QThread
{
    Q_OBJECT
public:
    explicit IBClientReaderThread(IBClient * client);
    ~IBClientReaderThread();
    void run();

private:
    IBClient * client;
    QTimer * timer;
};

#endif // IBCLIENTREADERTHREAD_H
