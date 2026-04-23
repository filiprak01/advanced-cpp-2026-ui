#include "ConnectionController.h"

#include <memory>

namespace
{
QString localizeConnectionError(const QString &message)
{
    if (message.contains(QStringLiteral("connect() failed"), Qt::CaseInsensitive) ||
        message.contains(QStringLiteral("Connection refused"), Qt::CaseInsensitive))
    {
        return QStringLiteral("Nie udalo sie polaczyc z serwerem. Sprawdz, czy backend dziala na porcie 8091.");
    }

    if (message.contains(QStringLiteral("SocketConnection"), Qt::CaseInsensitive))
    {
        return QStringLiteral("Blad polaczenia z serwerem. Sprawdz host, port i uruchomiony backend.");
    }

    return message;
}
}  // namespace

ConnectionController::ConnectionController(QObject* parent)
    : QObject(parent), m_connectionService(std::make_unique<ConnectionService>(this))
{
    connect(m_connectionService.get(), &ConnectionService::connectionStateChanged, this,
            &ConnectionController::handleConnectionStateChanged);
    connect(m_connectionService.get(), &ConnectionService::connectionError, this,
            &ConnectionController::handleConnectionError);

    syncState(m_connectionService->connectionState());
}

ConnectionController::~ConnectionController() = default;

void ConnectionController::connectToServer()
{
    clearLastError();
    m_connectionService->connectToServer();
}

void ConnectionController::disconnectFromServer()
{
    m_connectionService->disconnectFromServer();
}

void ConnectionController::clearLastError()
{
    if (m_lastError.isEmpty())
    {
        return;
    }

    m_lastError.clear();
    emit stateChanged();
}

void ConnectionController::handleConnectionStateChanged(connection::ConnectionState state)
{
    syncState(state);
}

void ConnectionController::handleConnectionError(const QString &message)
{
    const QString localizedMessage = localizeConnectionError(message);
    if (m_lastError == localizedMessage)
    {
        return;
    }

    m_lastError = localizedMessage;
    emit stateChanged();
}

void ConnectionController::syncState(connection::ConnectionState state)
{
    const QString nextStatusText = statusTextForState(state);
    const bool stateChangedLocally = m_currentState != state;
    const bool statusTextChanged = m_connectionText != nextStatusText;
    const bool shouldClearError = state != connection::ConnectionState::ConnectionLost && !m_lastError.isEmpty();

    m_currentState = state;
    m_connectionText = nextStatusText;

    if (state != connection::ConnectionState::ConnectionLost)
    {
        m_lastError.clear();
    }

    if (stateChangedLocally || statusTextChanged || shouldClearError)
    {
        emit stateChanged();
    }
}

QString ConnectionController::statusTextForState(connection::ConnectionState state) const
{
    switch (state)
    {
        case connection::ConnectionState::Connecting:
            return QStringLiteral("Laczenie z serwerem...");
        case connection::ConnectionState::Connected:
            return QStringLiteral("Polaczono");
        case connection::ConnectionState::ConnectionLost:
            return QStringLiteral("Utracono polaczenie");
        case connection::ConnectionState::Disconnected:
        default:
            return QStringLiteral("Rozlaczono");
    }
}
