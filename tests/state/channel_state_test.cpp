#include <QObject>

#include <gtest/gtest.h>

#include "state/ChannelState.h"

namespace
{
ChannelData makeChannel(int id, const QString &name)
{
    ChannelData channel;
    channel.id = id;
    channel.name = name;
    channel.usernames = {QStringLiteral("filip"), QStringLiteral("ola")};
    channel.memberCount = channel.usernames.size();
    channel.messageIds = {10, 11};
    return channel;
}
}  // namespace

TEST(ChannelStateTest, StartsWithoutChannels)
{
    ChannelState state;

    EXPECT_TRUE(state.channels().isEmpty());
    EXPECT_EQ(-1, state.currentChannelId());
}

TEST(ChannelStateTest, SetChannelsStoresListAndSelectsFirstChannel)
{
    ChannelState state;
    int channelsUpdatedCount = 0;
    int currentChangedCount = 0;
    int lastCurrentId = -2;

    QObject::connect(&state, &ChannelState::channelsUpdated, [&channelsUpdatedCount] {
        ++channelsUpdatedCount;
    });
    QObject::connect(&state, &ChannelState::currentChannelChanged, [&](int channelId) {
        ++currentChangedCount;
        lastCurrentId = channelId;
    });

    state.setChannels({makeChannel(1, QStringLiteral("general")), makeChannel(2, QStringLiteral("backend"))});

    EXPECT_EQ(2, state.channels().size());
    EXPECT_EQ(1, state.currentChannelId());
    EXPECT_EQ(QStringLiteral("general"), state.currentChannel().name);
    EXPECT_EQ(1, channelsUpdatedCount);
    EXPECT_EQ(1, currentChangedCount);
    EXPECT_EQ(1, lastCurrentId);
}

TEST(ChannelStateTest, KeepsCurrentChannelWhenItStillExists)
{
    ChannelState state;
    state.setChannels({makeChannel(1, QStringLiteral("general")), makeChannel(2, QStringLiteral("backend"))});
    state.setCurrentChannelId(2);

    int currentChangedCount = 0;
    QObject::connect(&state, &ChannelState::currentChannelChanged, [&currentChangedCount](int) {
        ++currentChangedCount;
    });

    state.setChannels({makeChannel(1, QStringLiteral("general")), makeChannel(2, QStringLiteral("backend-updated"))});

    EXPECT_EQ(2, state.currentChannelId());
    EXPECT_EQ(QStringLiteral("backend-updated"), state.currentChannel().name);
    EXPECT_EQ(0, currentChangedCount);
}

TEST(ChannelStateTest, MovesCurrentChannelWhenSelectedChannelDisappears)
{
    ChannelState state;
    state.setChannels({makeChannel(1, QStringLiteral("general")), makeChannel(2, QStringLiteral("backend"))});
    state.setCurrentChannelId(2);

    int lastCurrentId = -2;
    QObject::connect(&state, &ChannelState::currentChannelChanged, [&](int channelId) {
        lastCurrentId = channelId;
    });

    state.setChannels({makeChannel(1, QStringLiteral("general"))});

    EXPECT_EQ(1, state.currentChannelId());
    EXPECT_EQ(1, lastCurrentId);
}

TEST(ChannelStateTest, AvailableUsernamesAreTrimmedDeduplicatedAndSorted)
{
    ChannelState state;
    int updateCount = 0;
    QObject::connect(&state, &ChannelState::availableUsernamesUpdated, [&updateCount] {
        ++updateCount;
    });

    state.setAvailableUsernames({QStringLiteral(" ola "), QStringLiteral("filip"), QStringLiteral("ola"), QString()});

    EXPECT_EQ(QStringList({QStringLiteral("filip"), QStringLiteral("ola")}), state.availableUsernames());
    EXPECT_EQ(1, updateCount);

    state.setAvailableUsernames({QStringLiteral("filip"), QStringLiteral("ola")});
    EXPECT_EQ(1, updateCount);
}
