#include <QObject>

#include <gtest/gtest.h>

#include "state/SessionState.h"

TEST(SessionStateTest, StartsLoggedOut)
{
    SessionState state;

    EXPECT_FALSE(state.isLoggedIn());
    EXPECT_TRUE(state.username().isEmpty());
}

TEST(SessionStateTest, LoginStoresUsernameAndEmitsChanged)
{
    SessionState state;
    int changedCount = 0;
    QObject::connect(&state, &SessionState::changed, [&changedCount] {
        ++changedCount;
    });

    state.login(QStringLiteral("filip"));

    EXPECT_TRUE(state.isLoggedIn());
    EXPECT_EQ(QStringLiteral("filip"), state.username());
    EXPECT_EQ(1, changedCount);
}

TEST(SessionStateTest, LoginWithSameUsernameDoesNotEmitDuplicateChange)
{
    SessionState state;
    int changedCount = 0;
    QObject::connect(&state, &SessionState::changed, [&changedCount] {
        ++changedCount;
    });

    state.login(QStringLiteral("filip"));
    state.login(QStringLiteral("filip"));

    EXPECT_TRUE(state.isLoggedIn());
    EXPECT_EQ(QStringLiteral("filip"), state.username());
    EXPECT_EQ(1, changedCount);
}

TEST(SessionStateTest, LoginWithDifferentUsernameUpdatesSession)
{
    SessionState state;
    int changedCount = 0;
    QObject::connect(&state, &SessionState::changed, [&changedCount] {
        ++changedCount;
    });

    state.login(QStringLiteral("filip"));
    state.login(QStringLiteral("student"));

    EXPECT_TRUE(state.isLoggedIn());
    EXPECT_EQ(QStringLiteral("student"), state.username());
    EXPECT_EQ(2, changedCount);
}

TEST(SessionStateTest, ClearLogsOutAndDoesNotEmitWhenAlreadyEmpty)
{
    SessionState state;
    int changedCount = 0;
    QObject::connect(&state, &SessionState::changed, [&changedCount] {
        ++changedCount;
    });

    state.clear();
    EXPECT_EQ(0, changedCount);

    state.login(QStringLiteral("filip"));
    state.clear();
    state.clear();

    EXPECT_FALSE(state.isLoggedIn());
    EXPECT_TRUE(state.username().isEmpty());
    EXPECT_EQ(2, changedCount);
}
