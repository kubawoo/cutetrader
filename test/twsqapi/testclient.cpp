#include "testclient.h"
#include <twsqapi/clientmessage.h>

#include <QByteArray>
#include <QBuffer>
#include <QDebug>

TestApiClient::TestApiClient(QObject *parent)
    : twsqapi::BaseClient(parent),
      _inputBuffer(nullptr),
      _outputBuffer(nullptr)
{

}

TestApiClient::~TestApiClient()
{
    qDebug() << "TestApiClient::~TestApiClient()";
    if(_inputBuffer) {
        _inputBuffer->close();
        delete _inputBuffer;
    }
    if(_outputBuffer) {
        _outputBuffer->close();
        delete _outputBuffer;
    }
}

void TestApiClient::connect(QByteArray * input, QByteArray * output, int clientId)
{
    qDebug() << "TestApiClient::connect()";
    _inputBuffer = new QBuffer(input);
    _outputBuffer = new QBuffer(output);
    _inputBuffer->open(QIODevice::ReadOnly);
    _outputBuffer->open(QIODevice::WriteOnly);
    _init(_inputBuffer, _outputBuffer, clientId);
    _connectHandshake("+PACEAPI");
}

void TestApiClient::read()
{
    _readData();
    _inputBuffer->buffer().clear();
    _processBuffer();
}

bool TestApiClient::handshakeDone()
{
    return _isHandshakeDone();
}

int TestApiClient::getApiVersion()
{
    return _getApiVersion();
}

