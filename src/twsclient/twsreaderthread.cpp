#include "twsclient/twsreaderthread.h"

namespace twsclient {


TwsReaderThread::TwsReaderThread(ITwsClient * client)
    : QThread{nullptr},
      _client(client),
      _readTimer(nullptr),
      _cleanupTimer(nullptr)
{
    this->setObjectName("TwsReaderThread");
}

TwsReaderThread::~TwsReaderThread() {

}

void TwsReaderThread::run() {
    _readTimer = setupTimer(250, &ITwsClient::checkMessages);
    _cleanupTimer = setupTimer(60000, &ITwsClient::cleanup);
    exec();
}

QTimer *TwsReaderThread::setupTimer(int msec, void (ITwsClient::*funcPtr)(void))
{
    QTimer * timer = new QTimer();
    timer->moveToThread(this);
    QObject::connect(timer, &QTimer::timeout, _client, funcPtr);
    QObject::connect(this, &QThread::finished, timer, &QTimer::stop);
    QObject::connect(this, &QThread::finished, timer, &QTimer::deleteLater);
    timer->start(msec);
    return timer;
}


}
