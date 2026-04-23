#include "AuthController.h"

#include <client/connection_state.hpp>
#include <exception>

AuthController::AuthController(ConnectionController* connectionController, QObject* parent)
    : QObject(parent),
      m_connectionController(connectionController),
      m_authService(connectionController ? connectionController->connectionService() : nullptr, this),
      m_sessionState(this)
{
    connect(&m_authService, &AuthService::loginSucceeded, this,
            [this](const QString &username)
            {
                completeAuthentication(username);
            });
    connect(&m_authService, &AuthService::registerSucceeded, this, &AuthController::registerSucceeded);
    connect(&m_authService, &AuthService::authError, this, &AuthController::authError);
}

AuthController::~AuthController() = default;

SessionState* AuthController::sessionState()
{
    return &m_sessionState;
}

const SessionState* AuthController::sessionState() const
{
    return &m_sessionState;
}

void AuthController::login(const QString &username, const QString &password)
{
    if (!ensureConnected())
    {
        return;
    }

    m_authService.login(username, password);
}

void AuthController::registerUser(const QString &username, const QString &password)
{
    if (!ensureConnected())
    {
        return;
    }

    m_authService.registerUser(username, password);
}

void AuthController::logout()
{
    m_sessionState.logout();
}

bool AuthController::ensureConnected()
{
    if (!m_connectionController)
    {
        emit authError(QStringLiteral("Kontroler polaczenia nie jest dostepny."));
        return false;
    }

    if (m_connectionController->currentState() != connection::ConnectionState::Connected)
    {
        m_connectionController->connectToServer();
    }

    if (m_connectionController->currentState() != connection::ConnectionState::Connected)
    {
        emit authError(m_connectionController->lastError().isEmpty() ? QStringLiteral("Nie udalo sie polaczyc z serwerem.")
                                                                     : m_connectionController->lastError());
        return false;
    }

    return true;
}

void AuthController::completeAuthentication(const QString &username)
{
    if (username.isEmpty())
    {
        emit authError(QStringLiteral("Odpowiedz logowania nie zawiera nazwy uzytkownika."));
        return;
    }

    const bool alreadyAuthenticated = m_sessionState.isLoggedIn() && m_sessionState.username() == username;
    m_sessionState.login(username);

    if (!alreadyAuthenticated)
    {
        emit loginSucceeded(username);
        requestSynchronization();
    }
}

void AuthController::requestSynchronization()
{
    if (!m_connectionController || !m_connectionController->connectionService())
    {
        return;
    }

    try
    {
        m_connectionController->connectionService()->backend().synchronize();
    }
    catch (const std::exception &ex)
    {
        emit authError(QString::fromUtf8(ex.what()));
    }
}
