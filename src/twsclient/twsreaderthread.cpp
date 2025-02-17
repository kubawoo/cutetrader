#include "twsclient/twsreaderthread.h"
#include "twsclient/twsclient.h"
namespace twsclient {


TwsReaderThread::TwsReaderThread(TwsClient * client)
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
    _readTimer = setupTimer(250, &TwsClient::checkMessages);
    _cleanupTimer = setupTimer(60000, &TwsClient::cleanup);
    exec();
}


QTimer *TwsReaderThread::setupTimer(int msec, void (TwsClient::*funcPtr)())
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
