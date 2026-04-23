#pragma once

#include <QObject>
#include <QString>
#include <client/connection_state.hpp>
#include <memory>

#include "services/ConnectionService.h"

/**
 * @brief Zarządza stanem połączenia klienta w warstwie UI.
 *
 * Utrzymuje wskaźnik do ConnectionService, reaguje na sygnały
 * backendu i udostępnia bieżący stan połączenia, tekst statusu
 * oraz ostatni błąd dla widoków UI.
 */
class ConnectionController : public QObject
{
    Q_OBJECT
   public:
    /**
     * @brief Konstruktor.
     * @param parent Rodzic QObject/QWidget.
     */
    explicit ConnectionController(QObject* parent = nullptr);
    ~ConnectionController() override;

    ConnectionService* connectionService()
    {
        return m_connectionService.get();
    }

    const ConnectionService* connectionService() const
    {
        return m_connectionService.get();
    }

    connection::ConnectionState currentState() const
    {
        return m_currentState;
    }

    const QString &connectionText() const
    {
        return m_connectionText;
    }

    const QString &lastError() const
    {
        return m_lastError;
    }

   signals:
    void stateChanged();

   public slots:
    void connectToServer();
    void disconnectFromServer();
    void clearLastError();

   private slots:
    void handleConnectionStateChanged(connection::ConnectionState state);
    void handleConnectionError(const QString &message);

   private:
    void syncState(connection::ConnectionState state);
    QString statusTextForState(connection::ConnectionState state) const;

    std::unique_ptr<ConnectionService> m_connectionService;
    connection::ConnectionState m_currentState{connection::ConnectionState::Disconnected};
    QString m_connectionText{QStringLiteral("Rozlaczono")};
    QString m_lastError;
};
