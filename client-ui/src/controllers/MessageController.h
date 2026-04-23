#pragma once

#include <QHash>
#include <QObject>
#include <QString>

#include "services/ConnectionService.h"
#include "state/MessageState.h"

/**
 * @brief Obsługuje wysyłanie i odbieranie wiadomości.
 *
 * Wysyła wiadomości przez ClientBackend i aktualizuje MessageState
 * po odpowiedziach z backendu.
 */
class MessageController : public QObject {
    Q_OBJECT
public:
    /**
     * @brief Konstruktor.
     * @param connectionService Serwis udostepniajacy ClientBackend.
     * @param parent Rodzic QObject/QWidget.
     */
    explicit MessageController(ConnectionService *connectionService, QObject *parent = nullptr);
    ~MessageController() override;

    MessageState *state();
    const MessageState *state() const;

signals:
    void messageError(const QString &message);

public slots:
    void sendMessage(int channelId, const QString &content);

private slots:
    void handleBackendResponse(const QString &message);

private:
    MessageData messageFromJson(const json &message, int fallbackChannelId = -1) const;
    QHash<int, QList<MessageData>> messagesFromSyncPayload(const json &payload);
    void editMessage(const json &payload);
    void removeMessage(const json &payload);

    ConnectionService *m_connectionService{nullptr};
    MessageState m_messageState;
    QHash<int, int> m_messageIdToChannelId;
};
