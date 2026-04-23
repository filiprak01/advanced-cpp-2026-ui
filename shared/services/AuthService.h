#pragma once
#include <QObject>

/**
 * @brief Obsługuje uwierzytelnianie użytkownika przez WebSocket.
 *
 * Serializuje żądania logowania i rejestracji do JSON,
 * wysyła je przez WebSocketService i parsuje odpowiedzi serwera.
 */
class AuthService : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Konstruktor.
     * @param parent Rodzic QObject/QWidget.
     */
    explicit AuthService(QObject *parent = nullptr);
    ~AuthService() override;
};
