#include "twsreaderthread.h"

TwsReaderThread::TwsReaderThread(TwsClient * client)
    : QThread{nullptr},
      client(client),
      timer(nullptr)
{
    this->setObjectName("TwsReaderThread");
}

TwsReaderThread::~TwsReaderThread() {

}

void TwsReaderThread::run() {
    timer = new QTimer();
    timer->moveToThread(this);
    QObject::connect(timer, &QTimer::timeout, client, &TwsClient::checkMessages);
    QObject::connect(this, &QThread::finished, timer, &QTimer::stop);
    QObject::connect(this, &QThread::finished, timer, &QTimer::deleteLater);
    timer->start(250);
    exec();
}
