#include "twsqapi/servermessage.h"
#include "twsqapi/constants.h"

#include <QDebug>
#include <QTimeZone>
#include "common/utils.h"

#define CHECK_VALID(x, msg) \
    { \
        if (!(x)) { \
            qDebug() << "Validation failed:" << msg; \
            return; \
        } \
    }

namespace twsqapi {

ServerMessage::ServerMessage(const QStringList& fields, int id, int version, int fieldsCount)
    :_id(id), _version(version), _fieldsCount(fieldsCount), _valid(true)
{
    if (_fieldsCount > responses::UNKNOWN_FIELDS_COUNT) {
        //check exact number of fields
        _valid = _fieldsCount == fields.length();
    } else {
        //some messages may have variable number of fields. but must not be empty
        _valid = !fields.empty();
    }

    CHECK_VALID(_valid, "Field count mismatch or empty fields");

    int i = fields[0].toInt(&_valid);
    _valid = _valid && i == _id;
    CHECK_VALID(_valid, "Message ID mismatch");

    if (_version > responses::NO_VERSION) {
        int v = fields[1].toInt(&_valid);
        _valid = _valid && v == _version;
        CHECK_VALID(_valid, "Message version mismatch");
    }
}

ServerMessage::~ServerMessage()
{

}


ServerMessage *ServerMessageFactory::create(const QStringList & fields)
{
    bool ok = !fields.empty();
    int id;
    if(ok) {
        id = fields[0].toInt(&ok);
    }

    if(!ok) {
        qDebug() << "Failed to parse msg" << fields;
        return nullptr;
    }

    ServerMessage * msg = nullptr;

    switch(id) {
    case responses::MANAGED_ACCOUNTS:
        qDebug() << "RESPONSE: ManagedAccountsServerMessage";
        msg = new ManagedAccountsServerMessage(fields);
        break;
    case responses::NEXT_VALID_ID:
        qDebug() << "RESPONSE: NextValidIdServerMessage";
        msg =  new NextValidIdServerMessage(fields);
        break;
    case responses::ERR_MSG:
        qDebug() << "RESPONSE: ErrorServerMessage";
        msg =  new ErrorServerMessage(fields);
        break;
    case responses::ACCT_VALUE:
        qDebug() << "RESPONSE: AccountValueServerMessage";
        msg =  new AccountValueServerMessage(fields);
        break;
    case responses::ACCT_UPDATE_TIME:
        qDebug() << "RESPONSE: AccountUpdateTimeServerMessage";
        msg =  new AccountUpdateTimeServerMessage(fields);
        break;
    case responses::ACCT_DOWNLOAD_END:
        qDebug() << "RESPONSE: AccountDownloadEndServerMessage";
        msg =  new AccountDownloadEndServerMessage(fields);
        break;
    case responses::CURRENT_TIME:
        qDebug() << "RESPONSE: CurrentTimeServerMessage";
        msg =  new CurrentTimeServerMessage(fields);
        break;
    case responses::SYMBOL_SAMPLES:
        qDebug() << "RESPONSE: SymbolSamplesServerMessage";
        msg =  new SymbolSamplesServerMessage(fields);
        break;
    case responses::CONTRACT_DATA:
        qDebug() << "RESPONSE: ContractDataServerMessage";
        msg =  new ContractDataServerMessage(fields);
        break;
    case responses::CONTRACT_DATA_END:
        qDebug() << "RESPONSE: ContractDataEndServerMessage";
        msg =  new ContractDataEndServerMessage(fields);
        break;
    case responses::PORTFOLIO_VALUE:
        qDebug() << "RESPONSE: PortfolioValueServerMessage";
        msg =  new PortfolioValueServerMessage(fields);
        break;
    case responses::HISTORICAL_DATA:
        qDebug() << "RESPONSE: HistoricalDataServerMessage";
        msg =  new HistoricalDataServerMessage(fields);
        break;
    case responses::HISTORICAL_DATA_END:
        qDebug() << "RESPONSE: HistoricalDataEndServerMessage";
        msg =  new HistoricalDataEndServerMessage(fields);
        break;
    case responses::HISTORICAL_DATA_UPDATE:
        qDebug() << "RESPONSE: HistoricalDataUpdateServerMessage";
        msg =  new HistoricalDataUpdateServerMessage(fields);
        break;
    default:
        qDebug() << "Unsupported server msg" << id;
        for(int i = 0; i < fields.length(); i++) {
            qDebug() << i << fields[i];
        }
        break;
    }

    if(!msg->valid()) {
        qDebug() << "Invalid msg";
        delete msg;
        for(int i = 0; i < fields.length(); i++) {
            qDebug() << i << fields[i];
        }
        return nullptr;
    }
    return msg;
}

ManagedAccountsServerMessage::ManagedAccountsServerMessage(const QStringList &fields)
    : ServerMessage(fields, responses::MANAGED_ACCOUNTS, responses::MANAGED_ACCOUNTS_VERSION, responses::MANAGED_ACCOUNTS_FIELDS)
{
    CHECK_VALID(_valid, "Base validation failed");
    _valid = !fields[2].isEmpty();
    CHECK_VALID(_valid, "Accounts field is empty");

    for (auto s : fields[2].split(',')) {
        if (s.trimmed().isEmpty()) {
            _valid = false;
            break;
        } else {
            _accounts.append(s.trimmed());
        }
    }
}

NextValidIdServerMessage::NextValidIdServerMessage(const QStringList &fields)
    : ServerMessage(fields, responses::NEXT_VALID_ID, responses::NEXT_VALID_ID_VERSION, responses::NEXT_VALID_ID_FIELDS)
{
    CHECK_VALID(_valid, "Base validation failed");
    _nextId = fields[2].toInt(&_valid);
    CHECK_VALID(_valid, "Invalid next valid ID");
}

ErrorServerMessage::ErrorServerMessage(const QStringList &fields)
    : ServerMessage(fields, responses::ERR_MSG, responses::NO_VERSION, responses::ERR_MSG_FIELDS)
{
    CHECK_VALID(_valid, "Base validation failed");

    _errorId = fields[1].toInt(&_valid);
    CHECK_VALID(_valid, "Invalid error ID");

    _errorCode = fields[2].toInt(&_valid);
    CHECK_VALID(_valid, "Invalid error code");

    _errorMsg = fields[3];
    _errorDetails = fields[4];
    long timestamp = fields[5].toLong(&_valid);
    CHECK_VALID(_valid, "Invalid error timestamp");

    _errorTime = QDateTime::fromSecsSinceEpoch(timestamp, QTimeZone::UTC);
}


AccountValueServerMessage::AccountValueServerMessage(const QStringList &fields)
    : ServerMessage(fields, responses::ACCT_VALUE, responses::ACCT_VALUE_VERSION, responses::ACCT_VALUE_FIELDS)
{
    CHECK_VALID(_valid, "Base validation failed");
    _key = fields[2];
    _value = fields[3];
    _currency = fields[4];
    _accountName = fields[5];
}

AccountUpdateTimeServerMessage::AccountUpdateTimeServerMessage(const QStringList &fields)
: ServerMessage(fields, responses::ACCT_UPDATE_TIME, responses::ACCT_UPDATE_TIME_VERSION, responses::ACCT_UPDATE_TIME_FIELDS)
{
    CHECK_VALID(_valid, "Base validation failed");
    QString timestamp = fields[2];
    _time = QTime::fromString(timestamp, "HH:mm");
}

AccountDownloadEndServerMessage::AccountDownloadEndServerMessage(const QStringList &fields)
: ServerMessage(fields, responses::ACCT_DOWNLOAD_END, responses::ACCT_DOWNLOAD_END_VERSION, responses::ACCT_DOWNLOAD_END_FIELDS)
{
    CHECK_VALID(_valid, "Base validation failed");
    _account = fields[2];
}

CurrentTimeServerMessage::CurrentTimeServerMessage(const QStringList &fields)
    : ServerMessage(fields, responses::CURRENT_TIME, responses::CURRENT_TIME_VERSION, responses::CURRENT_TIME_FIELDS)
{
    CHECK_VALID(_valid, "Base validation failed");
    long timestamp = fields[2].toLong(&_valid);
    CHECK_VALID(_valid, "Invalid current time timestamp");

    _dateTime = QDateTime::fromSecsSinceEpoch(timestamp, QTimeZone::UTC);
}

SymbolSamplesServerMessage::SymbolSamplesServerMessage(const QStringList &fields)
    : ServerMessage(fields, responses::SYMBOL_SAMPLES)
{
    CHECK_VALID(_valid, "Base validation failed");

    int idx = 1;
    _reqId = fields[idx++].toInt(&_valid);
    CHECK_VALID(_valid, "Invalid request ID");

    int count = fields[idx++].toInt(&_valid);
    CHECK_VALID(_valid, "Invalid contract count");

    _valid = fields.length() >= idx + count * 8; // minimum 8 fields per contract
    CHECK_VALID(_valid, "Insufficient fields for contract data");

    for (int i = 0; i < count; i++) {
        common::ContractDetailsDTO dto;
        dto.contractId = fields[idx++].toLong(&_valid);
        CHECK_VALID(_valid, "Invalid contract ID");

        dto.symbol = fields[idx++];
        dto.securityType = common::Utils::securityTypeFromString(fields[idx++]);
        _valid = dto.securityType != common::SecurityType::UNSUPPORTED;
        CHECK_VALID(_valid, "Unsupported security type: " + fields[idx-1]);

        dto.primaryExchange = fields[idx++];
        dto.currency = fields[idx++];

        dto.derivatives = QList<common::SecurityType>();
        int derivativesCount = fields[idx++].toInt(&_valid);
        CHECK_VALID(_valid, "Invalid derivatives count");

        _valid = fields.length() >= idx + derivativesCount + 2;
        CHECK_VALID(_valid, "Insufficient fields for derivatives and trailing data");

        for(int j = 0; j < derivativesCount; j++) {
            common::SecurityType st = common::Utils::securityTypeFromString(fields[idx++]);
            _valid = st != common::SecurityType::UNSUPPORTED;
            CHECK_VALID(_valid, "Unsupported derivative security type: " + fields[idx-1]);
            dto.derivatives.append(st);
        }
        dto.description = fields[idx++];
        dto.issuerId = fields[idx++];
        if(dto.contractId > 0) {
            _contracts.append(dto);
        }
    }
}

ContractDataServerMessage::ContractDataServerMessage(const QStringList &fields)
    : ServerMessage(fields, responses::CONTRACT_DATA, responses::NO_VERSION, responses::UNKNOWN_FIELDS_COUNT)
{
    CHECK_VALID(_valid, "Base validation failed");
    _valid = fields.length() >= 22; // at least 22 fields, might be more
    CHECK_VALID(_valid, "Insufficient fields for contract data (need at least 22)");
    _reqId = fields[1].toInt(&_valid);
    CHECK_VALID(_valid, "Invalid request ID");

    _contract.contractId = fields[13].toInt(&_valid);
    CHECK_VALID(_valid, "Invalid contract ID");

    _contract.symbol = fields[2];
    _contract.currency = fields[9];
    _contract.description = fields[20];
    _contract.primaryExchange = fields[21];
    _contract.securityType = common::Utils::securityTypeFromString(fields[3]);
    _valid = _contract.securityType != common::SecurityType::UNSUPPORTED;
    CHECK_VALID(_valid, "Unsupported security type: " + fields[3]);
}

ContractDataEndServerMessage::ContractDataEndServerMessage(const QStringList &fields)
    : ServerMessage(fields,
                    responses::CONTRACT_DATA_END,
                    responses::CONTRACT_DATA_END_VERSION,
                    responses::CONTRACT_DATA_END_FIELDS)
{
    CHECK_VALID(_valid, "Base validation failed");
    _reqId = fields[2].toInt(&_valid);
    CHECK_VALID(_valid, "Invalid request ID");
}

PortfolioValueServerMessage::PortfolioValueServerMessage(const QStringList &fields)
    :ServerMessage(fields, responses::PORTFOLIO_VALUE, responses::PORTFOLIO_VALUE_VERSION, responses::PORTFOLIO_VALUE_FIELDS)
{
    CHECK_VALID(_valid, "Base validation failed");
    _position.contractId = fields[2].toInt(&_valid);
    CHECK_VALID(_valid, "Invalid contract ID");

    _position.symbol = fields[3];

    _position.securityType = common::Utils::securityTypeFromString(fields[4]);
    _valid = _position.securityType != common::SecurityType::UNSUPPORTED;
    CHECK_VALID(_valid, "Unsupported security type: " + fields[4]);

    _position.position = fields[13].toInt(&_valid);
    CHECK_VALID(_valid, "Invalid position");

    _position.marketPrice = fields[14].toDouble(&_valid);
    CHECK_VALID(_valid, "Invalid market price");

    _position.marketValue = fields[15].toDouble(&_valid);
    CHECK_VALID(_valid, "Invalid market value");

    _position.averageCost = fields[16].toDouble(&_valid);
    CHECK_VALID(_valid, "Invalid average cost");

    _position.unrealizedPNL = fields[17].toDouble(&_valid);
    CHECK_VALID(_valid, "Invalid unrealized PNL");

    _position.realizedPNL = fields[18].toDouble(&_valid);
    CHECK_VALID(_valid, "Invalid realized PNL");

    if (_position.securityType == common::SecurityType::OPTION
        || _position.securityType == common::SecurityType::FUTURE_OPTION
        || _position.securityType == common::SecurityType::FUTURE) {
        _position.expiration = QDate::fromString(fields[5], "yyyyMMdd");
        _valid = _position.expiration.isValid();
        CHECK_VALID(_valid, "Invalid expiration date");
    }

    if (_position.securityType == common::SecurityType::OPTION
        || _position.securityType == common::SecurityType::FUTURE_OPTION) {
        _position.right = common::Utils::optionTypeFromString(fields[7]);
        _position.multiplier = fields[8].toDouble(&_valid);
        CHECK_VALID(_valid, "Invalid multiplier");
        _position.strike = fields[6].toDouble(&_valid);
        CHECK_VALID(_valid, "Invalid strike");
    }
}

HistoricalDataServerMessage::HistoricalDataServerMessage(const QStringList &fields)
    : ServerMessage(fields, responses::HISTORICAL_DATA, responses::HISTORICAL_DATA_VERSION, responses::HISTORICAL_DATA_FIELDS)
{
    CHECK_VALID(_valid, "Base validation failed");
    // For server version >= 124, there's no version field in the response
    // Field 0: msgId (17)
    // Field 1: reqId
    // Field 2: itemCount
    // Field 3+: bars (8 fields each: time, open, high, low, close, volume, wap, count)
    _reqId = fields[1].toInt(&_valid);
    CHECK_VALID(_valid, "Invalid reqId");

    int itemCount = fields[2].toInt(&_valid);
    CHECK_VALID(_valid, "Invalid itemCount");

    // Parse multiple bars: each bar has 8 fields for server version >= 124
    // Fields per bar: time, open, high, low, close, volume, wap, count
    const int fieldsPerBar = 8;
    const int startIdx = 3;
    int numBars = qMin(itemCount, (fields.size() - startIdx) / fieldsPerBar);

    for (int i = 0; i < numBars; ++i) {
        int idx = startIdx + i * fieldsPerBar;
        if (idx + fieldsPerBar - 1 >= fields.size()) {
            break;
        }
        common::BarDTO bar;
        bar.time = fields[idx];
        bar.open = fields[idx + 1].toDouble(&_valid);
        if (!_valid) break;
        bar.high = fields[idx + 2].toDouble(&_valid);
        if (!_valid) break;
        bar.low = fields[idx + 3].toDouble(&_valid);
        if (!_valid) break;
        bar.close = fields[idx + 4].toDouble(&_valid);
        if (!_valid) break;
        bar.volume = fields[idx + 5].toDouble(&_valid);
        if (!_valid) break;
        bar.wap = fields[idx + 6].toDouble(&_valid);
        if (!_valid) break;
        bar.barCount = fields[idx + 7].toInt(&_valid);
        if (!_valid) break;
        _bars.append(bar);
    }
}

HistoricalDataEndServerMessage::HistoricalDataEndServerMessage(const QStringList &fields)
    : ServerMessage(fields, responses::HISTORICAL_DATA_END, responses::HISTORICAL_DATA_END_VERSION, responses::HISTORICAL_DATA_END_FIELDS)
{
    CHECK_VALID(_valid, "Base validation failed");
    // For server version >= 124, there's no version field in the response
    // Field 0: msgId (108)
    // Field 1: reqId
    // Field 2: startDate
    // Field 3: endDate
    _reqId = fields[1].toInt(&_valid);
    CHECK_VALID(_valid, "Invalid reqId");
    _startDate = fields[2];
    _endDate = fields[3];
}

HistoricalDataUpdateServerMessage::HistoricalDataUpdateServerMessage(const QStringList &fields)
    : ServerMessage(fields, responses::HISTORICAL_DATA_UPDATE, responses::HISTORICAL_DATA_UPDATE_VERSION, responses::HISTORICAL_DATA_UPDATE_FIELDS)
{
    CHECK_VALID(_valid, "Base validation failed");
    // For server version >= 124, there's no version field in the response
    // Field 0: msgId (90)
    // Field 1: reqId
    // Field 2: bar.count
    // Field 3: bar.time
    // Field 4: bar.open
    // Field 5: bar.close
    // Field 6: bar.high
    // Field 7: bar.low
    // Field 8: bar.wap
    // Field 9: bar.volume
    _reqId = fields[1].toInt(&_valid);
    CHECK_VALID(_valid, "Invalid reqId");

    _bar.barCount = fields[2].toInt(&_valid);
    CHECK_VALID(_valid, "Invalid barCount");
    _bar.time = fields[3];
    _bar.open = fields[4].toDouble(&_valid);
    CHECK_VALID(_valid, "Invalid open");
    _bar.close = fields[5].toDouble(&_valid);
    CHECK_VALID(_valid, "Invalid close");
    _bar.high = fields[6].toDouble(&_valid);
    CHECK_VALID(_valid, "Invalid high");
    _bar.low = fields[7].toDouble(&_valid);
    CHECK_VALID(_valid, "Invalid low");
    _bar.wap = fields[8].toDouble(&_valid);
    CHECK_VALID(_valid, "Invalid wap");
    _bar.volume = fields[9].toDouble(&_valid);
    CHECK_VALID(_valid, "Invalid volume");
}

}