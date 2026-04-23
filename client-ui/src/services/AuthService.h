#pragma once

#include <QObject>
#include <QString>

#include "services/ConnectionService.h"

/**
 * @brief Wysyła żądania auth przez ClientBackend i interpretuje odpowiedzi backendu.
 */
class AuthService : public QObject
{
    Q_OBJECT
   public:
    explicit AuthService(ConnectionService* connectionService, QObject* parent = nullptr);
    ~AuthService() override;

    ConnectionService* connectionService() const;
    void setConnectionService(ConnectionService* connectionService);

   public slots:
    void login(const QString &username, const QString &password);
    void registerUser(const QString &username, const QString &password);

   signals:
    void loginSucceeded(const QString &username);
    void registerSucceeded(const QString &username);
    void authError(const QString &message);

   private slots:
    void handleBackendResponse(const QString &message);

   private:
    QString errorMessageFromJson(const json &response) const;

    ConnectionService* m_connectionService{nullptr};
    QString m_pendingLoginUsername;
    QString m_pendingRegisterUsername;
};
