#include "MessageController.h"

#include <QDateTime>

MessageController::MessageController(ConnectionService *connectionService, QObject *parent)
    : QObject(parent)
    , m_connectionService(connectionService)
    , m_messageState(this)
{
    if (m_connectionService) {
        connect(m_connectionService,
                &ConnectionService::backendResponse,
                this,
                &MessageController::handleBackendResponse);
    }
}

MessageController::~MessageController() = default;

MessageState *MessageController::state()
{
    return &m_messageState;
}

const MessageState *MessageController::state() const
{
    return &m_messageState;
}

void MessageController::sendMessage(int channelId, const QString &content)
{
    if (!m_connectionService || channelId < 0 || content.trimmed().isEmpty()) {
        return;
    }

    try {
        m_connectionService->backend().sendMessage(channelId, content.toStdString());
    } catch (const std::exception &ex) {
        emit messageError(QString::fromUtf8(ex.what()));
    }
}

void MessageController::handleBackendResponse(const QString &message)
{
    json response;
    try {
        response = json::parse(message.toStdString());
    } catch (const json::parse_error&) {
        return;
    }

    const std::string type = response.value("type", "");
    const json payload = response.value("payload", json::object());

    if (type == "sync_response") {
        const auto messagesByChannel = messagesFromSyncPayload(payload);
        for (auto it = messagesByChannel.constBegin(); it != messagesByChannel.constEnd(); ++it) {
            m_messageState.setMessagesForChannel(it.key(), it.value());
        }
        return;
    }

    if (type == "new_message") {
        MessageData message = messageFromJson(payload, payload.value("channelId", -1));
        if (message.id < 0 || message.channelId < 0) {
            return;
        }
        m_messageIdToChannelId.insert(message.id, message.channelId);
        m_messageState.appendMessage(message);
        return;
    }

    if (type == "message_edited") {
        editMessage(payload);
        return;
    }

    if (type == "message_removed") {
        removeMessage(payload);
    }
}

MessageData MessageController::messageFromJson(const json &message, int fallbackChannelId) const
{
    MessageData data;
    data.id = message.value("messageId", message.value("id", -1));
    data.channelId = message.value("channelId", fallbackChannelId);
    data.senderName = QString::fromStdString(message.value("senderName", ""));
    data.content = QString::fromStdString(message.value("content", ""));
    data.timestamp = QDateTime::currentDateTime();
    data.isOwnMessage = m_connectionService && data.senderName.toStdString() == m_connectionService->backend().getCurrentUser();
    data.isEdited = message.value("isEdited", false);
    return data;
}

QHash<int, QList<MessageData>> MessageController::messagesFromSyncPayload(const json &payload)
{
    m_messageIdToChannelId.clear();

    QHash<int, int> messageIdToChannelId;
    QHash<int, QList<MessageData>> messagesByChannel;
    for (const auto &channel : payload.value("channels", json::array())) {
        const int channelId = channel.value("channelId", -1);
        if (channelId >= 0) {
            messagesByChannel[channelId] = {};
        }
        for (const int messageId : channel.value("messageIds", std::vector<int>{})) {
            messageIdToChannelId.insert(messageId, channelId);
        }
    }

    for (const auto &message : payload.value("messages", json::array())) {
        const int messageId = message.value("messageId", message.value("id", -1));
        const int channelId = message.value("channelId", messageIdToChannelId.value(messageId, -1));
        if (channelId < 0) {
            continue;
        }
        MessageData messageData = messageFromJson(message, channelId);
        messagesByChannel[channelId].append(messageData);
        if (messageData.id >= 0) {
            m_messageIdToChannelId.insert(messageData.id, channelId);
        }
    }

    return messagesByChannel;
}

void MessageController::editMessage(const json &payload)
{
    const int messageId = payload.value("messageId", -1);
    const int channelId = payload.value("channelId", m_messageIdToChannelId.value(messageId, -1));
    if (messageId < 0 || channelId < 0) {
        return;
    }

    QList<MessageData> messages = m_messageState.messagesForChannel(channelId);
    for (MessageData &message : messages) {
        if (message.id == messageId) {
            message.content = QString::fromStdString(payload.value("newContent", message.content.toStdString()));
            message.isEdited = true;
            m_messageState.setMessagesForChannel(channelId, messages);
            return;
        }
    }
}

void MessageController::removeMessage(const json &payload)
{
    const int messageId = payload.value("messageId", -1);
    const int channelId = payload.value("channelId", m_messageIdToChannelId.value(messageId, -1));
    if (messageId < 0 || channelId < 0) {
        return;
    }

    QList<MessageData> messages = m_messageState.messagesForChannel(channelId);
    for (auto it = messages.begin(); it != messages.end(); ++it) {
        if (it->id == messageId) {
            messages.erase(it);
            m_messageIdToChannelId.remove(messageId);
            m_messageState.setMessagesForChannel(channelId, messages);
            return;
        }
    }
}
