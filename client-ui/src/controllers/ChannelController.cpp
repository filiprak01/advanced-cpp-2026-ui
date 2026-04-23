#include "ChannelController.h"

#include <QStringList>
#include <algorithm>
#include <exception>
#include <vector>

ChannelController::ChannelController(ConnectionService* connectionService, QObject* parent)
    : QObject(parent)
    , m_connectionService(connectionService)
    , m_channelState(this)
{
    if (m_connectionService) {
        connect(m_connectionService,
                &ConnectionService::backendResponse,
                this,
                &ChannelController::handleBackendResponse);
    }
}

ChannelController::~ChannelController() = default;

ChannelState* ChannelController::state()
{
    return &m_channelState;
}

const ChannelState* ChannelController::state() const
{
    return &m_channelState;
}

void ChannelController::selectChannel(int channelId)
{
    m_channelState.setCurrentChannelId(channelId);
}

void ChannelController::createChannel(const QString &name, const QStringList &usernames)
{
    if (!m_connectionService || name.trimmed().isEmpty()) {
        return;
    }

    std::vector<std::string> members;
    for (const QString &username : usernames) {
        const QString trimmed = username.trimmed();
        if (trimmed.isEmpty()) {
            continue;
        }

        const std::string member = trimmed.toStdString();
        if (std::find(members.begin(), members.end(), member) == members.end()) {
            members.push_back(member);
        }
    }

    try {
        m_connectionService->backend().createChannel(name.trimmed().toStdString(), members, false);
    } catch (const std::exception &ex) {
        emit channelError(QString::fromUtf8(ex.what()));
    }
}

void ChannelController::deleteChannel(int channelId)
{
    if (!m_connectionService || channelId < 1) {
        return;
    }

    try {
        m_connectionService->backend().removeChannel(channelId);
    } catch (const std::exception &ex) {
        emit channelError(QString::fromUtf8(ex.what()));
    }
}

void ChannelController::handleBackendResponse(const QString &message)
{
    json response;
    try {
        response = json::parse(message.toStdString());
    } catch (const json::parse_error&) {
        return;
    }

    const std::string type = response.value("type", "");
    const json payload = response.value("payload", json::object());

    if (type == "error") {
        emit channelError(QString::fromStdString(payload.value("reason", "Channel operation failed.")));
        return;
    }

    if (type == "register_response" || type == "users_updated") {
        updateAvailableUsernamesFromPayload(payload);
        return;
    }

    if (type == "sync_response") {
        m_channelState.setChannels(channelsFromJson(payload.value("channels", json::array())));
        updateAvailableUsernamesFromPayload(payload);
        return;
    }

    if (type == "channel_created" || type == "user_joined_channel") {
        upsertChannel(channelFromJson(payload));
        return;
    }

    if (type == "channel_edited") {
        const int channelId = payload.value("channelId", -1);
        ChannelData channel = m_channelState.channelById(channelId);
        if (channel.id != channelId) {
            return;
        }
        channel.name = QString::fromStdString(payload.value("newName", channel.name.toStdString()));
        upsertChannel(channel);
        return;
    }

    if (type == "channel_removed") {
        removeChannel(payload.value("channelId", -1));
    }
}

ChannelData ChannelController::channelFromJson(const json &channel) const
{
    ChannelData data;
    data.id = channel.value("channelId", -1);
    data.name = QString::fromStdString(channel.value("name", ""));
    data.isPrivate = channel.value("isPrivate", false);
    data.topic = data.isPrivate ? QStringLiteral("rozmowa prywatna") : QStringLiteral("kanal publiczny");
    data.isActive = channel.value("isActive", true);
    data.maxUsers = channel.value("maxUsers", data.isPrivate ? 2 : 32);

    for (const auto &user : channel.value("userIds", std::vector<std::string>{})) {
        data.usernames.append(QString::fromStdString(user));
    }
    data.memberCount = data.usernames.size();

    for (const int messageId : channel.value("messageIds", std::vector<int>{})) {
        data.messageIds.append(messageId);
    }

    return data;
}

QList<ChannelData> ChannelController::channelsFromJson(const json &channels) const
{
    QList<ChannelData> result;
    if (!channels.is_array()) {
        return result;
    }

    for (const auto &channel : channels) {
        result.append(channelFromJson(channel));
    }
    return result;
}

QStringList ChannelController::usernamesFromJson(const json &users) const
{
    QStringList result;
    if (!users.is_array()) {
        return result;
    }

    for (const auto &user : users) {
        if (user.is_string()) {
            result.append(QString::fromStdString(user.get<std::string>()));
            continue;
        }

        if (user.is_object()) {
            const std::string username = user.value("username", user.value("userName", ""));
            if (!username.empty()) {
                result.append(QString::fromStdString(username));
            }
        }
    }

    return result;
}

void ChannelController::updateAvailableUsernamesFromPayload(const json &payload)
{
    if (!payload.contains("users")) {
        return;
    }

    m_channelState.setAvailableUsernames(usernamesFromJson(payload["users"]));
}

void ChannelController::upsertChannel(const ChannelData &channel)
{
    if (channel.id < 0) {
        return;
    }

    QList<ChannelData> channels = m_channelState.channels();
    for (ChannelData &existing : channels) {
        if (existing.id == channel.id) {
            existing = channel;
            m_channelState.setChannels(channels);
            return;
        }
    }

    channels.append(channel);
    m_channelState.setChannels(channels);
}

void ChannelController::removeChannel(int channelId)
{
    QList<ChannelData> channels = m_channelState.channels();
    for (auto it = channels.begin(); it != channels.end(); ++it) {
        if (it->id == channelId) {
            channels.erase(it);
            m_channelState.setChannels(channels);
            return;
        }
    }
}
