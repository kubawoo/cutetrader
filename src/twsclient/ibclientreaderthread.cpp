#include "ibclientreaderthread.h"

IBClientReaderThread::IBClientReaderThread(IBClient * client)
    : QThread{nullptr},
      client(client),
      timer(nullptr)
{
    this->setObjectName("IBClientReaderThread");
}

IBClientReaderThread::~IBClientReaderThread() {

}

void IBClientReaderThread::run() {
    timer = new QTimer();
    timer->moveToThread(this);
    QObject::connect(timer, &QTimer::timeout, client, &IBClient::checkMessages);
    QObject::connect(this, &QThread::finished, timer, &QTimer::stop);
    QObject::connect(this, &QThread::finished, timer, &QTimer::deleteLater);
    timer->start(250);
    exec();
}
