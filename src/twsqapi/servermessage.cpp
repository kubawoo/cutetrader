#include "twsqapi/servermessage.h"
#include "twsqapi/constants.h"

#include <QDebug>
#include "common/utils.h"

namespace twsqapi {

ServerMessage::ServerMessage(int id)
    :_id(id)
{

}

ServerMessage::~ServerMessage()
{

}


ServerMessage *ServerMessageFactory::create(const QStringList & fields)
{
    bool ok;
    int id = fields[0].toInt(&ok);

    if(!ok) {
        qDebug() << "Failed to parse msg" << fields;
        return nullptr;
    }


    switch(id) {
    case responses::MANAGED_ACCOUNTS:
        qDebug() << "RESPONSE: ManagedAccountsServerMessage";
        return new ManagedAccountsServerMessage(fields);
    case responses::NEXT_VALID_ID:
        qDebug() << "RESPONSE: NextValidIdServerMessage";
        return new NextValidIdServerMessage(fields);
    case responses::ERR_MSG:
        qDebug() << "RESPONSE: ErrorServerMessage";
        return new ErrorServerMessage(fields);
    case responses::ACCT_VALUE:
        qDebug() << "RESPONSE: AccountValueServerMessage";
        return new AccountValueServerMessage(fields);
    case responses::ACCT_UPDATE_TIME:
        qDebug() << "RESPONSE: AccountUpdateTimeServerMessage";
        return new AccountUpdateTimeServerMessage(fields);
    case responses::ACCT_DOWNLOAD_END:
        qDebug() << "RESPONSE: AccountDownloadEndServerMessage";
        return new AccountDownloadEndServerMessage(fields);
    case responses::CURRENT_TIME:
        qDebug() << "RESPONSE: AccountDownloadEndServerMessage";
        return new CurrentTimeServerMessage(fields);
    case responses::SYMBOL_SAMPLES:
        qDebug() << "RESPONSE: SymbolSamplesSrverMessage";
        return new SymbolSamplesSrverMessage(fields);
    case responses::CONTRACT_DATA:
        qDebug() << "RESPONSE: ContractDataServerMessage";
        return new ContractDataServerMessage(fields);
    case responses::CONTRACT_DATA_END:
        qDebug() << "RESPONSE: ContractDataEndServerMessage";
        return new ContractDataEndServerMessage(fields);
    case responses::PORTFOLIO_VALUE:
        qDebug() << "RESPONSE: PortfolioValueServerMessage";
        return new PortfolioValueServerMessage(fields);
    default:
        qDebug() << "Unsupported server msg" << id;
        for(int i = 0; i < fields.length(); i++) {
            qDebug() << i << fields[i];
        }
        return nullptr;
    }

}

ManagedAccountsServerMessage::ManagedAccountsServerMessage(const QStringList &fields)
    : ServerMessage(responses::MANAGED_ACCOUNTS)
{
    _accounts.append(fields[2].split(','));
}

NextValidIdServerMessage::NextValidIdServerMessage(const QStringList &fields)
    : ServerMessage(responses::NEXT_VALID_ID)
{
    bool ok;
    _nextId = fields[2].toInt(&ok);
    if(!ok) {
        qDebug() << "Failed to parse msg" << fields;
    }
}

ErrorServerMessage::ErrorServerMessage(const QStringList &fields)
    : ServerMessage(responses::ERR_MSG)
{
    bool ok;
    _errorId = fields[1].toInt(&ok);
    if(!ok) {
        qDebug() << "Failed to parse msg" << fields;
    }

    _errorCode = fields[2].toInt(&ok);
    if(!ok) {
        qDebug() << "Failed to parse msg" << fields;
    }

    _errorMsg = fields[3];
    _errorDetails = fields[4];
    _errorTime = QTime::currentTime();
}


AccountValueServerMessage::AccountValueServerMessage(const QStringList &fields)
    : ServerMessage(responses::ACCT_VALUE)
{
    _key = fields[2];
    _value = fields[3];
    _currency = fields[4];
    _accountName = fields[5];
}

AccountUpdateTimeServerMessage::AccountUpdateTimeServerMessage(const QStringList &fields)
: ServerMessage(responses::ACCT_UPDATE_TIME)
{
    QString timestamp = fields[2];
    qDebug() << "updateAccountTime" << timestamp;
    _time = QTime::fromString(timestamp, "HH:mm");
}

AccountDownloadEndServerMessage::AccountDownloadEndServerMessage(const QStringList &fields)
: ServerMessage(responses::ACCT_DOWNLOAD_END)
{
    _account = fields[2];
}

CurrentTimeServerMessage::CurrentTimeServerMessage(const QStringList &fields)
    : ServerMessage(responses::CURRENT_TIME)
{
    bool ok;
    long timestamp = fields[2].toLong(&ok);
    if(!ok) {
        qDebug() << "failed to parse msg" << fields;
    }
    _dateTime = QDateTime::fromSecsSinceEpoch(timestamp, Qt::LocalTime);
}

SymbolSamplesSrverMessage::SymbolSamplesSrverMessage(const QStringList &fields)
    : ServerMessage(responses::SYMBOL_SAMPLES)
{
    qDebug() << "Got matching symbols msg" << fields.length() << fields;

    int idx = 1;
    _reqId = fields[idx++].toInt();
    int count = fields[idx++].toInt();
    for(int i = 0; i < count; i++) {
        common::ContractDetailsDTO dto;
        dto.contractId = fields[idx++].toLong();
        dto.symbol = fields[idx++];
        dto.securityType = common::Utils::securityTypeFromString(fields[idx++]);
        dto.primaryExchange = fields[idx++];
        dto.currency = fields[idx++];
        dto.derivatives = QList<common::SecurityType>();
        int derivativesCount = fields[idx++].toInt();
        for(int j = 0; j < derivativesCount; j++) {
            common::SecurityType st = common::Utils::securityTypeFromString(fields[idx++]);
            if(st != common::SecurityType::UNSUPPORTED) {
                dto.derivatives.append(st);
            }
        }
        dto.description = fields[idx++];
        dto.issuerId = fields[idx++];
        if(dto.contractId > 0) {
            _contracts.append(dto);
        }
    }

}

ContractDataServerMessage::ContractDataServerMessage(const QStringList &fields)
    :ServerMessage(responses::CONTRACT_DATA)
{
    _reqId = fields[1].toInt();
    _contract.contractId = fields[13].toInt();
    _contract.symbol = fields[2];
    _contract.currency = fields[9];
    _contract.description = fields[20];
    _contract.primaryExchange = fields[21];
    _contract.securityType = common::Utils::securityTypeFromString(fields[3]);

}

ContractDataEndServerMessage::ContractDataEndServerMessage(const QStringList &fields)
    :ServerMessage(responses::CONTRACT_DATA_END)
{
    _reqId = fields[2].toInt();
}

PortfolioValueServerMessage::PortfolioValueServerMessage(const QStringList &fields)
    :ServerMessage(responses::PORTFOLIO_VALUE)
{
    _position.contractId = fields[2].toInt();
    _position.symbol = fields[3];
    _position.securityType = common::Utils::securityTypeFromString(fields[4]);
    _position.position = fields[13].toInt();
    _position.marketPrice = fields[14].toDouble();
    _position.marketValue = fields[15].toDouble();
    _position.averageCost = fields[16].toDouble();
    _position.unrealizedPNL = fields[17].toDouble();
    _position.realizedPNL = fields[18].toDouble();

    if(_position.securityType == common::SecurityType::OPTION
        || _position.securityType == common::SecurityType::FUTURE_OPTION
        || _position.securityType == common::SecurityType::FUTURE) {
        _position.expiration = QDate::fromString(fields[5], "yyyyMMdd");
    }

    if(_position.securityType == common::SecurityType::OPTION
        || _position.securityType == common::SecurityType::FUTURE_OPTION) {
        _position.right = common::Utils::optionTypeFromString(fields[7]);
        _position.multiplier = fields[8].toDouble();;
        _position.strike = fields[6].toDouble();
    }

}



}
