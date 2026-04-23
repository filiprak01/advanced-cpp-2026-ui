#pragma once

#include <QHash>
#include <QList>
#include <QObject>

#include "models/MessageData.h"

/**
 * @brief Przechowuje historię wiadomości dla każdego kanału.
 *
 * Singleton. Wiadomości są indeksowane po ID kanału.
 * Emituje messagesUpdated(int channelId) po zmianie.
 */
class MessageState : public QObject {
    Q_OBJECT
public:
    /**
     * @brief Konstruktor.
     * @param parent Rodzic QObject/QWidget.
     */
    explicit MessageState(QObject *parent = nullptr);
    ~MessageState() override;

    QList<MessageData> messagesForChannel(int channelId) const;

public slots:
    void setMessagesForChannel(int channelId, const QList<MessageData> &messages);
    void appendMessage(const MessageData &message);

signals:
    void messagesUpdated(int channelId);

private:
    QHash<int, QList<MessageData>> m_messagesByChannel;
};
