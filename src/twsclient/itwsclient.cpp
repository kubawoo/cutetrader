#include "twsclient/itwsclient.h"

namespace twsclient {

ITwsClient::ITwsClient(QObject *parent)
    : QObject{parent}
{

}

twsclient::ITwsClient::~ITwsClient()
{

}

}
