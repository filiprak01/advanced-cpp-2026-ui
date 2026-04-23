#pragma once
#include <QObject>

/**
 * @brief Obsługuje pobieranie historii i wysyłanie wiadomości.
 *
 * Komunikuje się z serwerem w celu pobrania historii kanału
 * oraz wysyłania nowych wiadomości przez WebSocketService.
 */
class MessageService : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Konstruktor.
     * @param parent Rodzic QObject/QWidget.
     */
    explicit MessageService(QObject *parent = nullptr);
    ~MessageService() override;
};
