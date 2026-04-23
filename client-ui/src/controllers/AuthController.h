#pragma once

#include <QObject>
#include <QString>

#include "controllers/ConnectionController.h"
#include "services/AuthService.h"
#include "state/SessionState.h"

/**
 * @brief Obsługuje logikę logowania i rejestracji po stronie UI.
 *
 * Deleguje operacje do AuthService i aktualizuje SessionState
 * po pomyślnym uwierzytelnieniu.
 */
class AuthController : public QObject {
    Q_OBJECT
public:
    /**
     * @brief Konstruktor.
     * @param connectionController Kontroler utrzymujacy polaczenie z backendem.
     * @param parent Rodzic QObject/QWidget.
     */
    explicit AuthController(ConnectionController *connectionController, QObject *parent = nullptr);
    ~AuthController() override;

    SessionState *sessionState();
    const SessionState *sessionState() const;

signals:
    void loginSucceeded(const QString &username);
    void registerSucceeded(const QString &username);
    void authError(const QString &message);

public slots:
    void login(const QString &username, const QString &password);
    void registerUser(const QString &username, const QString &password);
    void logout();

private:
    bool ensureConnected();
    void completeAuthentication(const QString &username);
    void requestSynchronization();

    ConnectionController *m_connectionController{nullptr};
    AuthService m_authService;
    SessionState m_sessionState;
};
