#include "SessionState.h"

SessionState::SessionState(QObject *parent)
    : QObject(parent)
{
}

SessionState::~SessionState() = default;

const QString &SessionState::username() const
{
    return m_username;
}

bool SessionState::isLoggedIn() const
{
    return m_isLoggedIn;
}

void SessionState::login(const QString &username)
{
    if (m_isLoggedIn && m_username == username) {
        return;
    }

    m_username = username;
    m_isLoggedIn = true;
    emit changed();
}

void SessionState::logout()
{
    clear();
}

void SessionState::clear()
{
    if (!m_isLoggedIn && m_username.isEmpty()) {
        return;
    }

    m_username.clear();
    m_isLoggedIn = false;
    emit changed();
}
