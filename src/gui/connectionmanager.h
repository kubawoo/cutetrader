#ifndef CONNECTION_MANAGER_H
#define CONNECTION_MANAGER_H

#include <QObject>
#include <common.h>

class ConnectionManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)
    Q_PROPERTY(QString status READ status NOTIFY statusChanged)
    Q_PROPERTY(QString host READ host WRITE setHost NOTIFY hostChanged)
    Q_PROPERTY(int port READ port WRITE setPort NOTIFY portChanged)
    Q_PROPERTY(int clientId READ clientId WRITE setClientId NOTIFY clientIdChanged)
    Q_PROPERTY(QString accountId READ accountId WRITE setAccountId NOTIFY accountIdChanged)
    Q_PROPERTY(bool useMock READ useMock WRITE setUseMock NOTIFY useMockChanged)

public:
    explicit ConnectionManager(QObject *parent = nullptr);
    ~ConnectionManager();

    bool connected() const;
    QString status() const;

    QString host() const;
    void setHost(const QString &host);

    int port() const;
    void setPort(int port);

    int clientId() const;
    void setClientId(int clientId);

    QString accountId() const;
    void setAccountId(const QString &accountId);

    bool useMock() const;
    void setUseMock(bool useMock);

    Q_INVOKABLE void connectToTws();
    Q_INVOKABLE void disconnectFromTws();
    Q_INVOKABLE void requestAccountData();
    Q_INVOKABLE void requestPortfolio();
    Q_INVOKABLE void requestOpenOrders();

    common::ITwsClient * client() const;

signals:
    void connectedChanged();
    void statusChanged(const QString &status);
    void hostChanged();
    void portChanged();
    void clientIdChanged();
    void accountIdChanged();
    void useMockChanged();
    void accountDataReceived(const QVariantMap &data);
    void portfolioReceived(const QVariantList &positions);
    void openOrdersReceived(const QVariantList &orders);
    void errorOccurred(const QString &message);

private slots:
    void onConnected();
    void onDisconnected();
    void onError(const QString &reason, bool fatal);
    void onManagedAccounts(const QStringList &accounts);
    void onAccountValueUpdated(const QString &key, const QString &value, const QString &currency);
    void onPortfolioPositionUpdated(const common::PortfolioPositionDTO &position);
    void onUpdateAccountTime(const QTime &time);
    void onOpenOrders(const int &requestId, const QList<common::OrderDTO> &orders);

private:
    void setupConnections();
    void cleanupConnections();

    common::ITwsClient *m_client = nullptr;
    bool m_connected = false;
    QString m_status = "Disconnected";
    QString m_host = "localhost";
    int m_port = 4002;
    int m_clientId = 1;
    QString m_accountId;
    bool m_useMock = false;
    QStringList m_availableAccounts;
    QVariantMap m_accountData;
    QVariantList m_portfolioPositions;
    QVariantList m_openOrders;
};

#endif // CONNECTION_MANAGER_H