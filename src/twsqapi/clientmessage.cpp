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

RequestHistoricalDataClientMessage::RequestHistoricalDataClientMessage(
    const int reqId, const long contractId,
    const QString & endDateTime,
    const QString & barSizeSetting,
    const QString & durationStr,
    const QString & whatToShow,
    int useRTH,
    int formatDate,
    int serverVersion,
    const QString & symbol,
    const QString & secType,
    const QString & lastTradeDateOrContractMonth,
    const QString & strike,
    const QString & right,
    const QString & multiplier,
    const QString & exchange,
    const QString & primaryExchange,
    const QString & currency,
    const QString & localSymbol,
    const QString & tradingClass,
    const QString & includeExpired)
    : _reqId(reqId), _contractId(contractId),
      _endDateTime(endDateTime), _barSizeSetting(barSizeSetting),
      _durationStr(durationStr), _whatToShow(whatToShow),
      _useRTH(useRTH), _formatDate(formatDate),
      _serverVersion(serverVersion),
      _symbol(symbol), _secType(secType),
      _lastTradeDateOrContractMonth(lastTradeDateOrContractMonth),
      _strike(strike), _right(right), _multiplier(multiplier),
      _exchange(exchange), _primaryExchange(primaryExchange),
      _currency(currency), _localSymbol(localSymbol),
      _tradingClass(tradingClass), _includeExpired(includeExpired) {}

QStringList RequestHistoricalDataClientMessage::fields() const {
    QStringList result;
    result << QString::number(requests::REQ_HISTORICAL_DATA);

    // VERSION field is only sent for server versions < MIN_SERVER_VER_SYNT_REALTIME_BARS (124)
    constexpr int MIN_SERVER_VER_SYNT_REALTIME_BARS = 124;
    if (_serverVersion < MIN_SERVER_VER_SYNT_REALTIME_BARS) {
        result << QString::number(requests::REQ_HISTORICAL_DATA_VERSION);
    }

    result << QString::number(_reqId);

    // Contract fields
    // conId is sent for server versions >= MIN_SERVER_VER_TRADING_CLASS (68)
    constexpr int MIN_SERVER_VER_TRADING_CLASS = 68;
    if (_serverVersion >= MIN_SERVER_VER_TRADING_CLASS) {
        result << QString::number(_contractId);
    }
    result << _symbol;
    result << _secType;
    result << _lastTradeDateOrContractMonth;
    result << (_strike.isEmpty() ? "0" : _strike);
    result << _right;
    result << _multiplier;
    result << (_exchange.isEmpty() ? "SMART" : _exchange);
    result << _primaryExchange;
    result << _currency;
    result << _localSymbol;
    if (_serverVersion >= MIN_SERVER_VER_TRADING_CLASS) {
        result << _tradingClass;
    }
    // includeExpired is sent for server versions >= 31
    result << (_includeExpired.isEmpty() ? "0" : _includeExpired);

    // Historical data fields
    // endDateTime and barSizeSetting are sent for server versions >= 20
    result << _endDateTime;
    result << _barSizeSetting;
    result << _durationStr;
    result << QString::number(_useRTH);
    result << _whatToShow;
    // formatDate is sent for server versions >= 16
    result << QString::number(_formatDate);

    // Version 6+ fields
    result << "0"; // keepUpToDate
    result << "";  // chartOptions

    return result;
}

CancelHistoricalDataClientMessage::CancelHistoricalDataClientMessage(const int reqId)
    : _reqId(reqId) {
    qDebug() << "REQUEST: CancelHistoricalDataClientMessage" << reqId;
}

QStringList CancelHistoricalDataClientMessage::fields() const {
    return {QString::number(requests::CANCEL_HISTORICAL_DATA),
            QString::number(requests::CANCEL_HISTORICAL_DATA_VERSION),
            QString::number(_reqId)};
}

RequestOpenOrdersClientMessage::RequestOpenOrdersClientMessage(int reqId)
    : _reqId(reqId) {
    qDebug() << "REQUEST: RequestOpenOrdersClientMessage" << reqId;
}

QStringList RequestOpenOrdersClientMessage::fields() const {
    return {QString::number(requests::REQ_OPEN_ORDERS),
            QString::number(requests::REQ_OPEN_ORDERS_VERSION),
            QString::number(_reqId)};
}

} // namespace twsqapi