#include <QMetaObject>
#include <QObject>

#include <gtest/gtest.h>

#include "controllers/MessageController.h"

namespace
{
bool deliverBackendResponse(MessageController &controller, const QString &message)
{
    return QMetaObject::invokeMethod(&controller,
                                     "handleBackendResponse",
                                     Qt::DirectConnection,
                                     Q_ARG(QString, message));
}
}  // namespace

TEST(MessageControllerTest, SyncResponseGroupsMessagesByChannel)
{
    MessageController controller(nullptr);

    ASSERT_TRUE(deliverBackendResponse(
        controller,
        QStringLiteral(R"({
            "type":"sync_response",
            "status":"ok",
            "payload":{
                "channels":[
                    {"channelId":1,"name":"general","messageIds":[10]},
                    {"channelId":2,"name":"backend","messageIds":[20]}
                ],
                "messages":[
                    {"id":10,"senderName":"filip","content":"hello"},
                    {"id":20,"senderName":"ola","content":"backend note"}
                ]
            }
        })")));

    const QList<MessageData> generalMessages = controller.state()->messagesForChannel(1);
    const QList<MessageData> backendMessages = controller.state()->messagesForChannel(2);

    ASSERT_EQ(1, generalMessages.size());
    ASSERT_EQ(1, backendMessages.size());
    EXPECT_EQ(QStringLiteral("hello"), generalMessages.first().content);
    EXPECT_EQ(QStringLiteral("backend note"), backendMessages.first().content);
}

TEST(MessageControllerTest, NewMessageAppendsToChannel)
{
    MessageController controller(nullptr);

    ASSERT_TRUE(deliverBackendResponse(
        controller,
        QStringLiteral(R"({"type":"new_message","payload":{"messageId":30,"channelId":1,"senderName":"filip","content":"new"}})")));

    const QList<MessageData> messages = controller.state()->messagesForChannel(1);
    ASSERT_EQ(1, messages.size());
    EXPECT_EQ(30, messages.first().id);
    EXPECT_EQ(QStringLiteral("new"), messages.first().content);
}

TEST(MessageControllerTest, EditedAndRemovedMessagesUseMessageIdMapping)
{
    MessageController controller(nullptr);
    ASSERT_TRUE(deliverBackendResponse(
        controller,
        QStringLiteral(R"({"type":"new_message","payload":{"messageId":30,"channelId":1,"senderName":"filip","content":"new"}})")));

    ASSERT_TRUE(deliverBackendResponse(
        controller,
        QStringLiteral(R"({"type":"message_edited","payload":{"messageId":30,"newContent":"edited"}})")));

    QList<MessageData> messages = controller.state()->messagesForChannel(1);
    ASSERT_EQ(1, messages.size());
    EXPECT_EQ(QStringLiteral("edited"), messages.first().content);
    EXPECT_TRUE(messages.first().isEdited);

    ASSERT_TRUE(deliverBackendResponse(
        controller,
        QStringLiteral(R"({"type":"message_removed","payload":{"messageId":30}})")));

    EXPECT_TRUE(controller.state()->messagesForChannel(1).isEmpty());
}
