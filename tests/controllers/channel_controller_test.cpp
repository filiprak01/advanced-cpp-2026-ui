#include <QMetaObject>
#include <QObject>

#include <gtest/gtest.h>

#include "controllers/ChannelController.h"

namespace
{
bool deliverBackendResponse(ChannelController &controller, const QString &message)
{
    return QMetaObject::invokeMethod(&controller,
                                     "handleBackendResponse",
                                     Qt::DirectConnection,
                                     Q_ARG(QString, message));
}
}  // namespace

TEST(ChannelControllerTest, SyncResponseFillsChannelState)
{
    ChannelController controller(nullptr);

    ASSERT_TRUE(deliverBackendResponse(
        controller,
        QStringLiteral(R"({
            "type":"sync_response",
            "status":"ok",
            "payload":{
                "channels":[
                    {
                        "channelId":1,
                        "name":"general",
                        "isPrivate":false,
                        "isActive":true,
                        "maxUsers":32,
                        "userIds":["filip","ola"],
                        "messageIds":[10,11]
                    }
                ],
                "messages":[]
            }
        })")));

    const QList<ChannelData> &channels = controller.state()->channels();
    ASSERT_EQ(1, channels.size());
    EXPECT_EQ(1, controller.state()->currentChannelId());
    EXPECT_EQ(QStringLiteral("general"), channels.first().name);
    EXPECT_EQ(2, channels.first().memberCount);
    EXPECT_TRUE(channels.first().usernames.contains(QStringLiteral("filip")));
    EXPECT_TRUE(channels.first().messageIds.contains(10));
}

TEST(ChannelControllerTest, SyncResponseStoresAvailableUsernames)
{
    ChannelController controller(nullptr);

    ASSERT_TRUE(deliverBackendResponse(
        controller,
        QStringLiteral(R"({
            "type":"sync_response",
            "status":"ok",
            "payload":{
                "channels":[],
                "messages":[],
                "users":["ola","filip"]
            }
        })")));

    EXPECT_EQ(QStringList({QStringLiteral("filip"), QStringLiteral("ola")}),
              controller.state()->availableUsernames());
}

TEST(ChannelControllerTest, ChannelEditedUpdatesExistingChannel)
{
    ChannelController controller(nullptr);
    ASSERT_TRUE(deliverBackendResponse(
        controller,
        QStringLiteral(R"({"type":"sync_response","payload":{"channels":[{"channelId":1,"name":"general"}]}})")));

    ASSERT_TRUE(deliverBackendResponse(
        controller,
        QStringLiteral(R"({"type":"channel_edited","payload":{"channelId":1,"newName":"backend"}})")));

    EXPECT_EQ(QStringLiteral("backend"), controller.state()->channelById(1).name);
}

TEST(ChannelControllerTest, ChannelCreatedAddsChannelAndSelectsItWhenListWasEmpty)
{
    ChannelController controller(nullptr);

    ASSERT_TRUE(deliverBackendResponse(
        controller,
        QStringLiteral(R"({
            "type":"channel_created",
            "payload":{
                "channelId":5,
                "name":"mvp",
                "isPrivate":false,
                "userIds":["filip"],
                "messageIds":[]
            }
        })")));

    const QList<ChannelData> &channels = controller.state()->channels();
    ASSERT_EQ(1, channels.size());
    EXPECT_EQ(5, channels.first().id);
    EXPECT_EQ(QStringLiteral("mvp"), channels.first().name);
    EXPECT_EQ(5, controller.state()->currentChannelId());
}

TEST(ChannelControllerTest, ChannelRemovedDeletesChannelAndMovesCurrentSelection)
{
    ChannelController controller(nullptr);
    ASSERT_TRUE(deliverBackendResponse(
        controller,
        QStringLiteral(R"({"type":"sync_response","payload":{"channels":[{"channelId":1,"name":"general"},{"channelId":2,"name":"backend"}]}})")));
    controller.selectChannel(2);

    ASSERT_TRUE(deliverBackendResponse(
        controller,
        QStringLiteral(R"({"type":"channel_removed","payload":{"channelId":2}})")));

    EXPECT_EQ(1, controller.state()->channels().size());
    EXPECT_EQ(1, controller.state()->currentChannelId());
}

TEST(ChannelControllerTest, ChannelRemovedClearsCurrentSelectionWhenLastChannelIsDeleted)
{
    ChannelController controller(nullptr);
    ASSERT_TRUE(deliverBackendResponse(
        controller,
        QStringLiteral(R"({"type":"sync_response","payload":{"channels":[{"channelId":1,"name":"general"}]}})")));

    ASSERT_TRUE(deliverBackendResponse(
        controller,
        QStringLiteral(R"({"type":"channel_removed","payload":{"channelId":1}})")));

    EXPECT_TRUE(controller.state()->channels().isEmpty());
    EXPECT_EQ(-1, controller.state()->currentChannelId());
}

TEST(ChannelControllerTest, ErrorResponseEmitsChannelErrorReason)
{
    ChannelController controller(nullptr);
    QString capturedError;
    int errorCount = 0;

    QObject::connect(&controller, &ChannelController::channelError, [&](const QString &message) {
        capturedError = message;
        ++errorCount;
    });

    ASSERT_TRUE(deliverBackendResponse(
        controller,
        QStringLiteral(R"({"type":"error","payload":{"reason":"channel_not_found"}})")));

    EXPECT_EQ(1, errorCount);
    EXPECT_EQ(QStringLiteral("channel_not_found"), capturedError);
}

TEST(ChannelControllerTest, RegisterResponseUpdatesAvailableUsernames)
{
    ChannelController controller(nullptr);

    ASSERT_TRUE(deliverBackendResponse(
        controller,
        QStringLiteral(R"({"type":"register_response","payload":{"userName":"ola","users":["filip","ola"]}})")));

    EXPECT_EQ(QStringList({QStringLiteral("filip"), QStringLiteral("ola")}),
              controller.state()->availableUsernames());
}

TEST(ChannelControllerTest, UsersUpdatedAcceptsObjectPayloads)
{
    ChannelController controller(nullptr);

    ASSERT_TRUE(deliverBackendResponse(
        controller,
        QStringLiteral(R"({"type":"users_updated","payload":{"users":[{"username":"filip"},{"userName":"ola"}]}})")));

    EXPECT_EQ(QStringList({QStringLiteral("filip"), QStringLiteral("ola")}),
              controller.state()->availableUsernames());
}
