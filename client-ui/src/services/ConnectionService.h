#pragma once
#include <QObject>
#include <QString>
#include <QStringList>
#include <QTimer>
#include <client/client_backend.hpp>
/**
 * @brief Trzyma klienta backendowego i udostepnia operacje polaczenia dla client-ui.
 *
 * Serwis nalezy wylacznie do warstwy klienta. Laduje konfiguracje
 * backendu, zarzadza zyciem instancji ClientBackend i wystawia
 * prosty interfejs dla kontrolerow oraz pozostalych serwisow UI.
 */
class ConnectionService : public QObject
{
    Q_OBJECT
   public:
    explicit ConnectionService(QObject* parent = nullptr, QString configPath = QStringLiteral("config/client.json"));
    ~ConnectionService() override;

    ClientBackend &backend();
    const ClientBackend &backend() const;

    QString configPath() const;
    bool isConnected() const;
    connection::ConnectionState connectionState() const;

   public slots:
    void connectToServer();
    void disconnectFromServer();
    void refreshConnectionState();

   signals:
    void connectionStateChanged(connection::ConnectionState state);
    void connectionError(const QString &message);
    void backendResponse(const QString &message);

   private:
    static ClientConfig loadClientConfig(const QString &configPath);
    void publishConnectionState(connection::ConnectionState state);

    QString m_configPath;
    QTimer* m_polling_timer;
    ClientBackend m_clientBackend;
    connection::ConnectionState m_lastKnownState{connection::ConnectionState::Disconnected};

   private slots:
    void pollBackend();
};
