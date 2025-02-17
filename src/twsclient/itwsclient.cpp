#include "twsclient/itwsclient.h"

namespace twsclient {

ITwsClient::ITwsClient(QObject *parent)
    : QObject{parent}
{

}

ITwsClient::~ITwsClient()
{

}

void ITwsClient::setRequestId(int *holder, int value)
{
    if(holder) {
        qDebug() << "setRequestId" << value;
        *holder = value;
    }
}

}
