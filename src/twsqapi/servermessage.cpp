#include "twsqapi/servermessage.h"
#include "twsqapi/constants.h"

#include <QDebug>
#include "common/utils.h"

#define CHECK_VALID(x) \
    { \
        if (!x) \
            return; \
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

    CHECK_VALID(_valid);

    int i = fields[0].toInt(&_valid);
    _valid = _valid && i == _id;
    CHECK_VALID(_valid);

    if (_version > responses::NO_VERSION) {
        int v = fields[1].toInt(&_valid);
        _valid = _valid && v == _version;
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
        qDebug() << "RESPONSE: SymbolSamplesSrverMessage";
        msg =  new SymbolSamplesSrverMessage(fields);
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
    CHECK_VALID(_valid);
    _valid = !fields[2].isEmpty();
    CHECK_VALID(_valid);

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
    CHECK_VALID(_valid);
    _nextId = fields[2].toInt(&_valid);
}

ErrorServerMessage::ErrorServerMessage(const QStringList &fields)
    : ServerMessage(fields, responses::ERR_MSG, responses::NO_VERSION, responses::ERR_MSG_FIELDS)
{
    CHECK_VALID(_valid);

    _errorId = fields[1].toInt(&_valid);
    CHECK_VALID(_valid);

    _errorCode = fields[2].toInt(&_valid);
    CHECK_VALID(_valid);

    _errorMsg = fields[3];
    _errorDetails = fields[4];
    long timestamp = fields[5].toLong(&_valid);
    CHECK_VALID(_valid);

    _errorTime = QDateTime::fromSecsSinceEpoch(timestamp);
}


AccountValueServerMessage::AccountValueServerMessage(const QStringList &fields)
    : ServerMessage(fields, responses::ACCT_VALUE, responses::ACCT_VALUE_VERSION, responses::ACCT_VALUE_FIELDS)
{
    CHECK_VALID(_valid);
    _key = fields[2];
    _value = fields[3];
    _currency = fields[4];
    _accountName = fields[5];
}

AccountUpdateTimeServerMessage::AccountUpdateTimeServerMessage(const QStringList &fields)
: ServerMessage(fields, responses::ACCT_UPDATE_TIME, responses::ACCT_UPDATE_TIME_VERSION, responses::ACCT_UPDATE_TIME_FIELDS)
{
    CHECK_VALID(_valid);
    QString timestamp = fields[2];
    _time = QTime::fromString(timestamp, "HH:mm");
}

AccountDownloadEndServerMessage::AccountDownloadEndServerMessage(const QStringList &fields)
: ServerMessage(fields, responses::ACCT_DOWNLOAD_END, responses::ACCT_DOWNLOAD_END_VERSION, responses::ACCT_DOWNLOAD_END_FIELDS)
{
    CHECK_VALID(_valid);
    _account = fields[2];
}

CurrentTimeServerMessage::CurrentTimeServerMessage(const QStringList &fields)
    : ServerMessage(fields, responses::CURRENT_TIME, responses::CURRENT_TIME_VERSION, responses::CURRENT_TIME_FIELDS)
{
    CHECK_VALID(_valid);
    long timestamp = fields[2].toLong(&_valid);
    CHECK_VALID(_valid);

    _dateTime = QDateTime::fromSecsSinceEpoch(timestamp);
}

SymbolSamplesSrverMessage::SymbolSamplesSrverMessage(const QStringList &fields)
    : ServerMessage(fields, responses::SYMBOL_SAMPLES)
{
    CHECK_VALID(_valid);

    int idx = 1;
    _reqId = fields[idx++].toInt(&_valid);
    CHECK_VALID(_valid);

    int count = fields[idx++].toInt(&_valid);
    CHECK_VALID(_valid);

    _valid = fields.length() >= idx + count * 7;
    CHECK_VALID(_valid);

    for (int i = 0; i < count; i++) {
        common::ContractDetailsDTO dto;
        dto.contractId = fields[idx++].toLong(&_valid);
        CHECK_VALID(_valid);

        dto.symbol = fields[idx++];
        dto.securityType = common::Utils::securityTypeFromString(fields[idx++]);
        _valid = dto.securityType != common::SecurityType::UNSUPPORTED;
        CHECK_VALID(_valid);

        dto.primaryExchange = fields[idx++];
        dto.currency = fields[idx++];

        dto.derivatives = QList<common::SecurityType>();
        int derivativesCount = fields[idx++].toInt(&_valid);
        CHECK_VALID(_valid);

        _valid = fields.length() >= idx + derivativesCount + 2;
        CHECK_VALID(_valid);

        for(int j = 0; j < derivativesCount; j++) {
            common::SecurityType st = common::Utils::securityTypeFromString(fields[idx++]);
            _valid = st != common::SecurityType::UNSUPPORTED;
            CHECK_VALID(_valid);
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
    CHECK_VALID(_valid);
    _valid = fields.length() >= 22; // at least 22 fields, might be more
    CHECK_VALID(_valid);
    _reqId = fields[1].toInt(&_valid);
    CHECK_VALID(_valid);

    _contract.contractId = fields[13].toInt(&_valid);
    CHECK_VALID(_valid);

    _contract.symbol = fields[2];
    _contract.currency = fields[9];
    _contract.description = fields[20];
    _contract.primaryExchange = fields[21];
    _contract.securityType = common::Utils::securityTypeFromString(fields[3]);
    _valid = _contract.securityType != common::SecurityType::UNSUPPORTED;
    CHECK_VALID(_valid);
}

ContractDataEndServerMessage::ContractDataEndServerMessage(const QStringList &fields)
    : ServerMessage(fields,
                    responses::CONTRACT_DATA_END,
                    responses::CONTRACT_DATA_END_VERSION,
                    responses::CONTRACT_DATA_END_FIELDS)
{
    CHECK_VALID(_valid);
    _reqId = fields[2].toInt(&_valid);
}

PortfolioValueServerMessage::PortfolioValueServerMessage(const QStringList &fields)
    :ServerMessage(fields, responses::PORTFOLIO_VALUE, responses::PORTFOLIO_VALUE_VERSION, responses::PORTFOLIO_VALUE_FIELDS)
{
    CHECK_VALID(_valid);
    _position.contractId = fields[2].toInt(&_valid);
    CHECK_VALID(_valid);

    _position.symbol = fields[3];

    _position.securityType = common::Utils::securityTypeFromString(fields[4]);
    _valid = _position.securityType != common::SecurityType::UNSUPPORTED;
    CHECK_VALID(_valid);

    _position.position = fields[13].toInt(&_valid);
    CHECK_VALID(_valid);

    _position.marketPrice = fields[14].toDouble(&_valid);
    CHECK_VALID(_valid);

    _position.marketValue = fields[15].toDouble(&_valid);
    CHECK_VALID(_valid);

    _position.averageCost = fields[16].toDouble(&_valid);
    CHECK_VALID(_valid);

    _position.unrealizedPNL = fields[17].toDouble(&_valid);
    CHECK_VALID(_valid);

    _position.realizedPNL = fields[18].toDouble(&_valid);
    CHECK_VALID(_valid);

    if (_position.securityType == common::SecurityType::OPTION
        || _position.securityType == common::SecurityType::FUTURE_OPTION
        || _position.securityType == common::SecurityType::FUTURE) {
        _position.expiration = QDate::fromString(fields[5], "yyyyMMdd");
        _valid = _position.expiration.isValid();
        CHECK_VALID(_valid);
    }

    if (_position.securityType == common::SecurityType::OPTION
        || _position.securityType == common::SecurityType::FUTURE_OPTION) {
        _position.right = common::Utils::optionTypeFromString(fields[7]);
        _position.multiplier = fields[8].toDouble(&_valid);
        CHECK_VALID(_valid);
        _position.strike = fields[6].toDouble(&_valid);
        CHECK_VALID(_valid);
    }
}

}
