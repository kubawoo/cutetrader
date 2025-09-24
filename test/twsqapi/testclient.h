#ifndef TEST_TWSQAPI_TESTCLIENT_H
#define TEST_TWSQAPI_TESTCLIENT_H

#include <twsqapi/baseclient.h>

#include <QBuffer>

class TestApiClient : public twsqapi::BaseClient
{
public:
    explicit TestApiClient(QObject * parent = nullptr);
    ~TestApiClient();

    void connect(QByteArray * input, QByteArray * output, int clientId = 0);
    void read();
    bool handshakeDone();
    int getApiVersion();


private:
    QBuffer * _inputBuffer;
    QBuffer * _outputBuffer;
};



#endif
