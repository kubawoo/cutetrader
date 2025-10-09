#ifndef TWSQAPI_SERVERMESSAGE_H
#define TWSQAPI_SERVERMESSAGE_H

#include <QStringList>
#include <QTime>
#include "common/types.h"
#include "constants.h"

namespace twsqapi {
class ServerMessage
{
public:
    ServerMessage(const QStringList &fields,
                  int id,
                  int version = responses::NO_VERSION,
                  int fieldsCount = responses::UNKNOWN_FIELDS_COUNT);
    virtual ~ServerMessage();

    int id() { return _id; }
    bool valid() { return _valid; }
private:
    int _id;
    int _version;
    int _fieldsCount;
protected:
    bool _valid;
};


class ServerMessageFactory
{
public:
    ServerMessage * create(const QStringList & fields);
};

class ManagedAccountsServerMessage : public ServerMessage
{
public:
    ManagedAccountsServerMessage(const QStringList & fields);
    const QStringList & accounts() { return _accounts;}
private:
    QStringList _accounts;
};

class NextValidIdServerMessage : public ServerMessage
{
public:
    NextValidIdServerMessage(const QStringList & fields);
    const int nextId() { return _nextId; }
private:
    int _nextId;
};

class ErrorServerMessage : public ServerMessage
{
public:
    ErrorServerMessage(const QStringList & fields);
    const int errorId(){ return _errorId;}
    const int errorCode(){return _errorCode;}
    const QString & errorMsg(){return _errorMsg;}
    const QString & errorDetails(){ return _errorDetails; }
    const QDateTime &errorTime() { return _errorTime; }

private:
    int _errorId;
    int _errorCode;
    QString _errorMsg;
    QString _errorDetails;
    QDateTime _errorTime;
};


class AccountValueServerMessage : public ServerMessage
{
public:
    AccountValueServerMessage(const QStringList & fields);
    const QString & key() { return _key; }
    const QString & value() { return _value; }
    const QString & currency() { return _currency; }
    const QString & accountName() { return _accountName; }
private:
    QString _key;
    QString _value;
    QString _currency;
    QString _accountName;
};

class AccountUpdateTimeServerMessage : public ServerMessage
{
public:
    AccountUpdateTimeServerMessage(const QStringList & fields);
    const QTime &time() { return _time; };
private:
    QTime _time;
};

class AccountDownloadEndServerMessage : public ServerMessage
{
public:
    AccountDownloadEndServerMessage(const QStringList & fields);
    const QString &account() { return _account; };
private:
    QString _account;
};

class CurrentTimeServerMessage : public ServerMessage
{
public:
    CurrentTimeServerMessage(const QStringList & fields);
    const QDateTime & dateTime() { return _dateTime; };
private:
    QDateTime _dateTime;
};


class SymbolSamplesSrverMessage : public ServerMessage
{
public:
    SymbolSamplesSrverMessage(const QStringList & fields);
    int requestId() { return _reqId; }
    const QList<common::ContractDetailsDTO>& contracts() { return _contracts; }
private:
    int _reqId;
    QList<common::ContractDetailsDTO> _contracts;
};

class ContractDataServerMessage : public ServerMessage
{
public:
    ContractDataServerMessage(const QStringList & fields);
    int requestId() { return _reqId; }
    const common::ContractDetailsDTO & contract() { return _contract; }
private:
    int _reqId;
    common::ContractDetailsDTO _contract;
};

class ContractDataEndServerMessage : public ServerMessage
{
public:
    ContractDataEndServerMessage(const QStringList & fields);
    int requestId() { return _reqId; }
private:
    int _reqId;
};

class PortfolioValueServerMessage : public ServerMessage
{
public:
    PortfolioValueServerMessage(const QStringList & fields);
    const common::PortfolioPositionDTO & position() { return _position;}
private:
    common::PortfolioPositionDTO _position;
};


}

#endif
