#include "ChannelState.h"

#include <algorithm>

ChannelState::ChannelState(QObject *parent)
    : QObject(parent)
{
}

ChannelState::~ChannelState() = default;

const QList<ChannelData> &ChannelState::channels() const
{
    return m_channels;
}

const QStringList &ChannelState::availableUsernames() const
{
    return m_availableUsernames;
}

int ChannelState::currentChannelId() const
{
    return m_currentChannelId;
}

ChannelData ChannelState::currentChannel() const
{
    return channelById(m_currentChannelId);
}

ChannelData ChannelState::channelById(int channelId) const
{
    for (const ChannelData &channel : m_channels) {
        if (channel.id == channelId) {
            return channel;
        }
    }

    return {};
}

void ChannelState::setChannels(const QList<ChannelData> &channels)
{
    m_channels = channels;
    emit channelsUpdated();

    const bool currentStillExists = m_currentChannelId >= 0 && channelById(m_currentChannelId).id == m_currentChannelId;
    const int nextChannelId = m_channels.isEmpty() ? -1 : m_channels.first().id;
    if (!currentStillExists && m_currentChannelId != nextChannelId) {
        m_currentChannelId = nextChannelId;
        emit currentChannelChanged(m_currentChannelId);
    }
}

void ChannelState::setAvailableUsernames(const QStringList &usernames)
{
    QStringList normalized;
    for (const QString &username : usernames) {
        const QString trimmed = username.trimmed();
        if (!trimmed.isEmpty() && !normalized.contains(trimmed)) {
            normalized.append(trimmed);
        }
    }

    std::sort(normalized.begin(), normalized.end());
    if (m_availableUsernames == normalized) {
        return;
    }

    m_availableUsernames = normalized;
    emit availableUsernamesUpdated();
}

void ChannelState::setCurrentChannelId(int channelId)
{
    if (m_currentChannelId == channelId) {
        return;
    }

    m_currentChannelId = channelId;
    emit currentChannelChanged(m_currentChannelId);
}
