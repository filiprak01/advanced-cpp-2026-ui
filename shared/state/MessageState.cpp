#include "MessageState.h"

MessageState::MessageState(QObject *parent)
    : QObject(parent)
{
}

MessageState::~MessageState() = default;

QList<MessageData> MessageState::messagesForChannel(int channelId) const
{
    return m_messagesByChannel.value(channelId);
}

void MessageState::setMessagesForChannel(int channelId, const QList<MessageData> &messages)
{
    m_messagesByChannel[channelId] = messages;
    emit messagesUpdated(channelId);
}

void MessageState::appendMessage(const MessageData &message)
{
    m_messagesByChannel[message.channelId].append(message);
    emit messagesUpdated(message.channelId);
}
