#ifndef TWSQAPI_CLIENTMESSAGE_H
#define TWSQAPI_CLIENTMESSAGE_H

#include <QByteArray>
#include <QSharedPointer>
#include <QStringList>

namespace twsqapi {

class ClientMessage
{
public:
    ClientMessage();
    virtual ~ClientMessage();

    virtual QByteArray encode() const;

protected:
    virtual QStringList fields() const;
    virtual bool terminateWithNull() const;

private:
    void _writeSize(QByteArray & msg, uint32_t size) const;

private:
};


class HandshakeClientMessage : public ClientMessage
{
public:
    HandshakeClientMessage(const QString & opts);
    ~HandshakeClientMessage();

    QByteArray encode() const override;

protected:
    QStringList fields() const override;
    bool terminateWithNull() const override;


private:
    QString _connectionOptions;

};


class StartApiClientMessage : public ClientMessage
{
public:
    StartApiClientMessage(int clientId);
protected:
    QStringList fields() const override;
private:
    int _clientId;

};


class StartAccountUpdatesClientMessage : public ClientMessage
{
public:
    StartAccountUpdatesClientMessage(const QString & account);
protected:
    QStringList fields() const override;
private:
    QString _account;

};

class RequestCurrentTimeClientMessage : public ClientMessage
{
public:
    RequestCurrentTimeClientMessage();
protected:
    QStringList fields() const override;
};

class RequestMatchingSymbolsClientMessage : public ClientMessage
{
public:
    RequestMatchingSymbolsClientMessage(const int reqId, const QString & pattern);
protected:
    QStringList fields() const override;
private:
    int _reqId;
    QString _pattern;
};

class RequestContractDetailsClientMessage : public ClientMessage
{
public:
    RequestContractDetailsClientMessage(const int reqId, const long contractId);
protected:
    QStringList fields() const override;
private:
    int _reqId;
    long _contractId;
};

}
#endif
