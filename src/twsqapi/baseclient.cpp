#include "twsqapi/baseclient.h"

#include <QDebug>
#include <QMutexLocker>

#include "twsqapi/constants.h"

namespace twsqapi {

BaseClient::BaseClient(QObject *parent)
    : QObject(parent),
      _input(nullptr),
      _output(nullptr),
      _clientId(-1),
      _handshakeDone(false)
{
    QObject::connect(this, &BaseClient::_dataRead, this, &BaseClient::_processBuffer, Qt::QueuedConnection);
}


BaseClient::~BaseClient()
{
    qDebug() << "BaseClient::~BaseClient()";
}

void BaseClient::_init(QIODevice *input, QIODevice *output, int clientId)
{
    _input = input;
    _output = output;
    _clientId = clientId;
    QObject::connect(_input, &QIODevice::readyRead, this, &BaseClient::_readData);
}

void BaseClient::send(const ClientMessage &msg)
{
    _send(msg.encode());
}

void BaseClient::send(ClientMessage && msg)
{
    _send(msg.encode());
}

void BaseClient::_send(const QByteArray &data)
{
    qDebug() << "BaseClient::_send" << data << data.size();
    qDebug() << data.toHex(' ');

    QMutexLocker locker(&_writeMutex);
    qint64 bytesWritten = 0;
    int retries = 100;
    while(bytesWritten != data.size() && retries-- > 0) {
        bytesWritten += _output->write(data.sliced(bytesWritten));
        qDebug() << "bytesWritten" << bytesWritten;
    }
}

void BaseClient::_readData()
{
    QMutexLocker locker(&_readMutex);

    qint64 bytesAvailavle = _input->bytesAvailable();
    qDebug() << "bytes available for read" << bytesAvailavle;
    qint64 bytesRead = 0;
    int retries = 100;
    while(bytesRead < bytesAvailavle && retries-- > 0) {
        QByteArray data = _input->readAll();
        _buffer.append(data);
        bytesRead += data.length();
    }
    qDebug() << "All data read";
    emit _dataRead();
}

void BaseClient::_processBuffer()
{
    while(!_buffer.isEmpty()) {
        qDebug() << "BaseClient::_processBuffer()";
        if(_buffer.size() <= client::MSG_LENGTH_FIELD_SIZE) {
            //too little data available, wait for more
            qDebug() << "too little data available, wait for more";
            break;
        }

        uint32_t dataLength = 0;
        for(int i = 0; i < client::MSG_LENGTH_FIELD_SIZE; i++) {
            dataLength <<= 8;
            dataLength += (uint8_t) _buffer[i];
        }
        // qDebug() << "dataLength" << dataLength;

        if(_buffer.size() - client::MSG_LENGTH_FIELD_SIZE < dataLength) {
            //not all data arrived, wait for more
            qDebug() << "not all data arrived, wait for more"
                     << "bufferSize" << _buffer.size()
                     << "msgLen" << dataLength;
            break;
        }

        QByteArray data = _buffer.mid(client::MSG_LENGTH_FIELD_SIZE, dataLength);
        _buffer.remove(0, dataLength + client::MSG_LENGTH_FIELD_SIZE);

        // qDebug() << "msg" << data;
        // qDebug() << "buffer" << _buffer;

        _processData(data);
    }
}

void BaseClient::_processData(const QByteArray &data)
{
    QStringList fields = QString(data).split(QChar('\0'));
    if(_handshakeDone) {
        QSharedPointer<ServerMessage> msg = QSharedPointer<ServerMessage>(serverMessageFactory.create(fields));
        if(!msg.isNull()) {
            emit message(msg);
        }
    } else {
        _handleHandshake(fields);
    }
}

void BaseClient::_handleHandshake(const QStringList &fields)
{
    qDebug() << "Got handshake" << fields;
    if(fields.length() != client::HANDSHAKE_FIELDS) {
        qDebug() << "Invalid handshake, got fields:" << fields.length();
        emit error("Invalid handshake message", true);
        return;
    }

    bool ok;
    _apiVersion = fields[0].toInt(&ok);
    qDebug() << "Got server api version" << _apiVersion;
    if(!ok || _apiVersion > client::SERVER_MAX_VERSION || _apiVersion < client::SERVER_MIN_VERSION) {
        qDebug() << "Invalid api version received:" << fields[0];
        emit error("Invalid api version received: " + fields[0], true);
        return;
    }

    if(fields[1].isEmpty()) {
        qDebug() << "Expected server time:" << fields[1];
        emit error("Expected server time " + fields[1], true);
        return;
    }

    if(!fields[2].isEmpty()) {
        qDebug() << "Unexpected characters" << fields[2];
        emit error("Unexpected characters " + fields[2], true);
        return;
    }

    qDebug() << "Server time" << fields[1];

    _handshakeDone = true;
    StartApiClientMessage startApi(_clientId);
    send(startApi);
}

void BaseClient::_connectHandshake(const QString &opts)
{
    send(HandshakeClientMessage(opts));
}

bool BaseClient::_isHandshakeDone()
{
    return _handshakeDone;
}

int BaseClient::_getApiVersion()
{
    return _apiVersion;
}



}
