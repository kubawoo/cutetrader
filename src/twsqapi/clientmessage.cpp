#include "twsqapi/clientmessage.h"
#include "twsqapi/constants.h"

#include <QDebug>

namespace twsqapi {

ClientMessage::ClientMessage() {}

ClientMessage::~ClientMessage() {}

QByteArray ClientMessage::encode() const {
    QString str = fields().join('\0');
    if (terminateWithNull()) {
        str += '\0';
    }

    QByteArray msg;
    msg.reserve(64); // TODO: allocate exact size?
    _writeSize(msg, str.size());
    msg.append(str.toUtf8());
    // TODO return pointer / smart pointer instead
    return msg;
}

QStringList ClientMessage::fields() const { return {}; }

bool ClientMessage::terminateWithNull() const { return true; }

void ClientMessage::_writeSize(QByteArray &msg, uint32_t size) const {
    for (int i = client::MSG_LENGTH_FIELD_SIZE - 1; i >= 0; i--) {
        char c = size >> 8 * i;
        msg.append(c);
    }
}

HandshakeClientMessage::HandshakeClientMessage(const QString &opts)
    : _connectionOptions(opts) {
    qDebug() << "REQUEST: HandshakeClientMessage";
}

HandshakeClientMessage::~HandshakeClientMessage() {}

QByteArray HandshakeClientMessage::encode() const {
    QByteArray bytes = ClientMessage::encode();
    bytes.prepend('\0');
    bytes.prepend(client::API_HEADER);
    return bytes;
}

QStringList HandshakeClientMessage::fields() const {
    return {client::SERVER_VERSION_PREFIX +
            QString::number(client::SERVER_MIN_VERSION) +
            client::SERVER_VERSIONS_DELIMITER +
            QString::number(client::SERVER_MAX_VERSION) +
            (_connectionOptions.isEmpty() ? "" : (' ' + _connectionOptions))};
}

bool HandshakeClientMessage::terminateWithNull() const { return false; }

StartApiClientMessage::StartApiClientMessage(int clientId)
    : _clientId(clientId) {
    qDebug() << "REQUEST: StartApiClientMessage";
}

QStringList StartApiClientMessage::fields() const {
    return {QString::number(requests::START_API),
            QString::number(requests::START_API_VERSION),
            QString::number(_clientId), ""};
}

StartAccountUpdatesClientMessage::StartAccountUpdatesClientMessage(
    const QString &account)
    : _account(account) {
    qDebug() << "REQUEST: StartAccountUpdatesClientMessage";
}

QStringList StartAccountUpdatesClientMessage::fields() const {
    return {QString::number(requests::REQ_ACCT_DATA),
            QString::number(requests::REQ_ACCT_DATA_VERSION),
            QString::number(1), // 1==TRUE (start), 0==FALSE (stop)
            _account};
}

RequestCurrentTimeClientMessage::RequestCurrentTimeClientMessage() {}

QStringList RequestCurrentTimeClientMessage::fields() const {
    return {QString::number(requests::REQ_CURRENT_TIME),
            QString::number(requests::REQ_CURRENT_TIME_VERSION)};
}

RequestMatchingSymbolsClientMessage::RequestMatchingSymbolsClientMessage(
    const int reqId, const QString &pattern)
    : _reqId(reqId), _pattern(pattern) {}

QStringList RequestMatchingSymbolsClientMessage::fields() const {
    return {QString::number(requests::REQ_MATCHING_SYMBOLS),
            QString::number(_reqId), _pattern};
}

RequestContractDetailsClientMessage::RequestContractDetailsClientMessage(
    const int reqId, const long contractId)
    : _reqId(reqId), _contractId(contractId) {}

QStringList RequestContractDetailsClientMessage::fields() const {
    return {
        QString::number(requests::REQ_CONTRACT_DATA),
        QString::number(requests::REQ_CONTRACT_DATA_VERSION),
        QString::number(_reqId),
        QString::number(_contractId),
        "", // symbol,
        "", // secType
        "",      // lastTradeDateOrContractMonth
        "0",      // strike
        "",      // right
        "",      // multiplayer
        "SMART", // exchange
        "",      // primary exchange
        "",      // currency
        "",      // localSymbol
        "",      // tradingClass
        "0",      // include expired
        "",      // secIdType
        "",      // secId
        ""       // issuerID
    };
}

} // namespace twsqapi
