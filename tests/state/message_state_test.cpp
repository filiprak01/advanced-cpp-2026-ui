#include <QObject>

#include <gtest/gtest.h>

#include "state/MessageState.h"

namespace
{
MessageData makeMessage(int id, int channelId, const QString &content)
{
    MessageData message;
    message.id = id;
    message.channelId = channelId;
    message.senderName = QStringLiteral("filip");
    message.content = content;
    return message;
}
}  // namespace

TEST(MessageStateTest, StartsWithoutMessages)
{
    MessageState state;

    EXPECT_TRUE(state.messagesForChannel(1).isEmpty());
}

TEST(MessageStateTest, SetMessagesStoresMessagesForChannel)
{
    MessageState state;
    int updatedChannelId = -1;
    QObject::connect(&state, &MessageState::messagesUpdated, [&](int channelId) {
        updatedChannelId = channelId;
    });

    state.setMessagesForChannel(7, {makeMessage(10, 7, QStringLiteral("hello"))});

    const QList<MessageData> messages = state.messagesForChannel(7);
    ASSERT_EQ(1, messages.size());
    EXPECT_EQ(10, messages.first().id);
    EXPECT_EQ(QStringLiteral("hello"), messages.first().content);
    EXPECT_EQ(7, updatedChannelId);
}

TEST(MessageStateTest, AppendMessageAddsToMatchingChannel)
{
    MessageState state;
    int updateCount = 0;
    QObject::connect(&state, &MessageState::messagesUpdated, [&updateCount](int) {
        ++updateCount;
    });

    state.setMessagesForChannel(7, {makeMessage(10, 7, QStringLiteral("first"))});
    state.appendMessage(makeMessage(11, 7, QStringLiteral("second")));

    const QList<MessageData> messages = state.messagesForChannel(7);
    ASSERT_EQ(2, messages.size());
    EXPECT_EQ(11, messages.last().id);
    EXPECT_EQ(QStringLiteral("second"), messages.last().content);
    EXPECT_EQ(2, updateCount);
}
