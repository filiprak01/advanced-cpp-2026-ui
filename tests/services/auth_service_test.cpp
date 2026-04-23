#include <QMetaObject>
#include <QObject>

#include <gtest/gtest.h>

#include "services/AuthService.h"

namespace
{
bool deliverBackendResponse(AuthService &service, const QString &message)
{
    return QMetaObject::invokeMethod(&service,
                                     "handleBackendResponse",
                                     Qt::DirectConnection,
                                     Q_ARG(QString, message));
}
}  // namespace

TEST(AuthServiceTest, LoginResponseEmitsSucceededUsernameFromPayload)
{
    AuthService service(nullptr);
    QString capturedUsername;
    int successCount = 0;
    int errorCount = 0;

    QObject::connect(&service, &AuthService::loginSucceeded, [&](const QString &username) {
        capturedUsername = username;
        ++successCount;
    });
    QObject::connect(&service, &AuthService::authError, [&](const QString&) {
        ++errorCount;
    });

    ASSERT_TRUE(deliverBackendResponse(
        service,
        QStringLiteral(R"({"type":"login_response","status":"ok","payload":{"userName":"filip"}})")));

    EXPECT_EQ(1, successCount);
    EXPECT_EQ(QStringLiteral("filip"), capturedUsername);
    EXPECT_EQ(0, errorCount);
}

TEST(AuthServiceTest, ErrorResponseEmitsPayloadReason)
{
    AuthService service(nullptr);
    QString capturedError;
    int errorCount = 0;

    QObject::connect(&service, &AuthService::authError, [&](const QString &message) {
        capturedError = message;
        ++errorCount;
    });

    ASSERT_TRUE(deliverBackendResponse(
        service,
        QStringLiteral(R"({"type":"error","status":"error","payload":{"reason":"invalid password"}})")));

    EXPECT_EQ(1, errorCount);
    EXPECT_EQ(QStringLiteral("invalid password"), capturedError);
}

TEST(AuthServiceTest, RegisterResponseEmitsRegisterAndLoginSucceeded)
{
    AuthService service(nullptr);
    QString registeredUsername;
    QString loggedUsername;
    int registerCount = 0;
    int loginCount = 0;

    QObject::connect(&service, &AuthService::registerSucceeded, [&](const QString &username) {
        registeredUsername = username;
        ++registerCount;
    });
    QObject::connect(&service, &AuthService::loginSucceeded, [&](const QString &username) {
        loggedUsername = username;
        ++loginCount;
    });

    ASSERT_TRUE(deliverBackendResponse(
        service,
        QStringLiteral(R"({"type":"register_response","status":"ok","payload":{"userName":"filip","users":["filip"]}})")));

    EXPECT_EQ(1, registerCount);
    EXPECT_EQ(1, loginCount);
    EXPECT_EQ(QStringLiteral("filip"), registeredUsername);
    EXPECT_EQ(QStringLiteral("filip"), loggedUsername);
}

TEST(AuthServiceTest, NonJsonResponseIsIgnored)
{
    AuthService service(nullptr);
    int successCount = 0;
    int errorCount = 0;

    QObject::connect(&service, &AuthService::loginSucceeded, [&](const QString&) {
        ++successCount;
    });
    QObject::connect(&service, &AuthService::authError, [&](const QString&) {
        ++errorCount;
    });

    ASSERT_TRUE(deliverBackendResponse(service, QStringLiteral("not json")));

    EXPECT_EQ(0, successCount);
    EXPECT_EQ(0, errorCount);
}

TEST(AuthServiceTest, LoginWithoutConnectionServiceEmitsReadableError)
{
    AuthService service(nullptr);
    QString capturedError;
    int errorCount = 0;

    QObject::connect(&service, &AuthService::authError, [&](const QString &message) {
        capturedError = message;
        ++errorCount;
    });

    service.login(QStringLiteral("filip"), QStringLiteral("secret"));

    EXPECT_EQ(1, errorCount);
    EXPECT_EQ(QStringLiteral("Usluga polaczenia nie jest dostepna."), capturedError);
}
