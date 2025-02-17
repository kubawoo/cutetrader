#include "twsclient/itwsclient.h"

namespace twsclient {

ITwsClient::ITwsClient(QObject *parent)
    : QObject{parent}
{

}

ITwsClient::~ITwsClient()
{

}

void ITwsClient::_setRequestId(int *holder, int value)
{
    if(holder) {
        *holder = value;
    }
}

}
