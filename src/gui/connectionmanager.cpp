#include "connectionmanager.h"
#include <mocked.h>
#include <twsqapi/twsclient.h>
#include <QDebug>
#include <QTimer>

ConnectionManager::ConnectionManager(QObject *parent)
    : QObject(parent)
{}

ConnectionManager::~ConnectionManager()
{
    disconnectFromTws();
}

bool ConnectionManager::connected() const
{
    return m_connected;
}

QString ConnectionManager::status() const
{
    return m_status;
}

QString ConnectionManager::host() const
{
    return m_host;
}

void ConnectionManager::setHost(const QString &host)
{
    if (m_host != host) {
        m_host = host;
        emit hostChanged();
    }
}

int ConnectionManager::port() const
{
    return m_port;
}

void ConnectionManager::setPort(int port)
{
    if (m_port != port) {
        m_port = port;
        emit portChanged();
    }
}

int ConnectionManager::clientId() const
{
    return m_clientId;
}

void ConnectionManager::setClientId(int clientId)
{
    if (m_clientId != clientId) {
        m_clientId = clientId;
        emit clientIdChanged();
    }
}

QString ConnectionManager::accountId() const
{
    return m_accountId;
}

void ConnectionManager::setAccountId(const QString &accountId)
{
    if (m_accountId != accountId) {
        m_accountId = accountId;
        emit accountIdChanged();
    }
}

bool ConnectionManager::useMock() const
{
    return m_useMock;
}

void ConnectionManager::setUseMock(bool useMock)
{
    if (m_useMock != useMock) {
        m_useMock = useMock;
        emit useMockChanged();
    }
}

common::ITwsClient * ConnectionManager::client() const
{
    return m_client;
}

void ConnectionManager::connectToTws()
{
    qDebug() << "ConnectionManager::connectToTws() called, m_useMock =", m_useMock,
              "m_host =", m_host, "m_port =", m_port, "m_clientId =", m_clientId;

    if (m_client) {
        cleanupConnections();
        delete m_client;
        m_client = nullptr;
    }

    m_client = m_useMock 
        ? static_cast<common::ITwsClient*>(new mocked::TwsClientMock(this))
        : static_cast<common::ITwsClient*>(new twsqapi::TwsClient(this));
    setupConnections();

    m_status = "Connecting...";
    emit statusChanged(m_status);

    bool ok = m_client->connect(m_host, m_port, m_clientId);
    qDebug() << "ConnectionManager::connectToTws() connect result =", ok;
    if (!ok) {
        m_status = "Connection failed";
        emit statusChanged(m_status);
        emit errorOccurred("Failed to connect to TWS");
    }
}

void ConnectionManager::disconnectFromTws()
{
    if (m_client) {
        m_client->disconnect();
        cleanupConnections();
        delete m_client;
        m_client = nullptr;
    }
    m_connected = false;
    emit connectedChanged();
    m_status = "Disconnected";
    emit statusChanged(m_status);
}

void ConnectionManager::requestAccountData()
{
    if (!m_client || !m_connected) {
        emit errorOccurred("Not connected to TWS");
        return;
    }
    m_client->requestCurrentTime();
}

void ConnectionManager::requestPortfolio()
{
    if (!m_client || !m_connected) {
        emit errorOccurred("Not connected to TWS");
        return;
    }
    m_portfolioPositions.clear();
    m_client->requestOpenOrders();
}

void ConnectionManager::requestOpenOrders()
{
    if (!m_client || !m_connected) {
        emit errorOccurred("Not connected to TWS");
        return;
    }
    m_client->requestOpenOrders();
}

void ConnectionManager::setupConnections()
{
    connect(m_client, &common::ITwsClient::connectedSignal, this, &ConnectionManager::onConnected);
    connect(m_client, &common::ITwsClient::disconnectedSignal, this, &ConnectionManager::onDisconnected);
    connect(m_client, &common::ITwsClient::errorSignal, this, &ConnectionManager::onError);
    connect(m_client, &common::ITwsClient::managedAccountsSignal, this, &ConnectionManager::onManagedAccounts);
    connect(m_client, &common::ITwsClient::accountValueUpdatedSignal, this, &ConnectionManager::onAccountValueUpdated);
    connect(m_client, &common::ITwsClient::portfolioPositionUpdatedSignal, this, &ConnectionManager::onPortfolioPositionUpdated);
    connect(m_client, &common::ITwsClient::updateAccountTimeSignal, this, &ConnectionManager::onUpdateAccountTime);
}

void ConnectionManager::cleanupConnections()
{
    if (m_client) {
        disconnect(m_client, &common::ITwsClient::connectedSignal, this, &ConnectionManager::onConnected);
        disconnect(m_client, &common::ITwsClient::disconnectedSignal, this, &ConnectionManager::onDisconnected);
        disconnect(m_client, &common::ITwsClient::errorSignal, this, &ConnectionManager::onError);
        disconnect(m_client, &common::ITwsClient::managedAccountsSignal, this, &ConnectionManager::onManagedAccounts);
        disconnect(m_client, &common::ITwsClient::accountValueUpdatedSignal, this, &ConnectionManager::onAccountValueUpdated);
        disconnect(m_client, &common::ITwsClient::portfolioPositionUpdatedSignal, this, &ConnectionManager::onPortfolioPositionUpdated);
        disconnect(m_client, &common::ITwsClient::updateAccountTimeSignal, this, &ConnectionManager::onUpdateAccountTime);
    }
}

void ConnectionManager::onConnected()
{
    m_connected = true;
    emit connectedChanged();
    m_status = "Connected";
    emit statusChanged(m_status);

    // Don't request data here - wait for managedAccountsSignal like CLI does
    // The managed accounts will arrive automatically after handshake
}

void ConnectionManager::onDisconnected()
{
    m_connected = false;
    emit connectedChanged();
    m_status = "Disconnected";
    emit statusChanged(m_status);
}

void ConnectionManager::onError(const QString &reason, bool fatal)
{
    QString msg = fatal ? QString("FATAL: %1").arg(reason) : reason;
    emit errorOccurred(msg);
    if (fatal) {
        m_connected = false;
        emit connectedChanged();
        m_status = "Error: " + reason;
        emit statusChanged(m_status);
    }
}

void ConnectionManager::onManagedAccounts(const QStringList &accounts)
{
    m_availableAccounts = accounts;
    if (accounts.isEmpty()) {
        emit errorOccurred("No accounts available");
        return;
    }

    if (m_accountId.isEmpty()) {
        m_accountId = accounts.first();
        emit accountIdChanged();
    }

    m_client->startClient(m_accountId);
    requestAccountData();
}

void ConnectionManager::onAccountValueUpdated(const QString &key, const QString &value, const QString &currency)
{
    m_accountData[key] = QVariantMap{{"value", value}, {"currency", currency}};
    emit accountDataReceived(m_accountData);
}

void ConnectionManager::onPortfolioPositionUpdated(const common::PortfolioPositionDTO &position)
{
    QVariantMap posMap;
    posMap["contractId"] = QVariant::fromValue(static_cast<qlonglong>(position.contractId));
    posMap["symbol"] = position.symbol;
    posMap["securityType"] = static_cast<int>(position.securityType);
    posMap["position"] = position.position;
    posMap["marketPrice"] = position.marketPrice;
    posMap["marketValue"] = position.marketValue;
    posMap["averageCost"] = position.averageCost;
    posMap["unrealizedPNL"] = position.unrealizedPNL;
    posMap["realizedPNL"] = position.realizedPNL;
    posMap["expiration"] = position.expiration.toString(Qt::ISODate);
    posMap["strike"] = position.strike;
    posMap["right"] = static_cast<int>(position.right);
    posMap["multiplier"] = position.multiplier;

    bool found = false;
    for (int i = 0; i < m_portfolioPositions.size(); ++i) {
        if (m_portfolioPositions[i].toMap()["contractId"].toLongLong() == position.contractId) {
            m_portfolioPositions[i] = posMap;
            found = true;
            break;
        }
    }
    if (!found) {
        m_portfolioPositions.append(posMap);
    }
    emit portfolioReceived(m_portfolioPositions);
}

void ConnectionManager::onUpdateAccountTime(const QTime &time)
{
    m_accountData["lastUpdateTime"] = time.toString();
    emit accountDataReceived(m_accountData);
}