#include "ConnectionService.h"

#include <cstdlib>
#include <filesystem>
#include <utility>
#include <vector>

namespace
{
ClientConfig buildDefaultClientConfig()
{
    return ClientConfig{};
}

std::vector<std::filesystem::path> buildConfigCandidates(const QString &configPath)
{
    std::vector<std::filesystem::path> candidates;

    if (!configPath.isEmpty())
    {
        candidates.emplace_back(configPath.toStdString());
    }

    if (const char* envPath = std::getenv("PWCHAT_CLIENT_CONFIG"))
    {
        if (envPath[0] != '\0')
        {
            candidates.emplace_back(envPath);
        }
    }

#ifdef CLIENT_UI_LOCAL_CLIENT_CONFIG_PATH
    candidates.emplace_back(CLIENT_UI_LOCAL_CLIENT_CONFIG_PATH);
#endif

#ifdef CLIENT_UI_BACKEND_CLIENT_CONFIG_PATH
    candidates.emplace_back(CLIENT_UI_BACKEND_CLIENT_CONFIG_PATH);
#endif

    candidates.emplace_back("config/client.json");
    candidates.emplace_back("../advanced-cpp-2026/config/client.json");
    return candidates;
}
}  // namespace

ConnectionService::ConnectionService(QObject* parent, QString configPath)
    : QObject(parent),
      m_configPath(std::move(configPath)),
      m_clientBackend(loadClientConfig(m_configPath)),
      m_lastKnownState(m_clientBackend.getConnectionState())
{
    m_polling_timer = new QTimer(this);
    m_polling_timer->setInterval(50);
    connect(m_polling_timer, &QTimer::timeout, this, &ConnectionService::pollBackend);
}

ConnectionService::~ConnectionService()
{
    m_clientBackend.disconnect();
}

ClientBackend &ConnectionService::backend()
{
    return m_clientBackend;
}

const ClientBackend &ConnectionService::backend() const
{
    return m_clientBackend;
}

QString ConnectionService::configPath() const
{
    return m_configPath;
}

bool ConnectionService::isConnected() const
{
    return m_clientBackend.isConnected();
}

connection::ConnectionState ConnectionService::connectionState() const
{
    return m_clientBackend.getConnectionState();
}

void ConnectionService::connectToServer()
{
    publishConnectionState(connection::ConnectionState::Connecting);

    try
    {
        m_clientBackend.connect();
        refreshConnectionState();
        pollBackend();
        m_polling_timer->start();
    }
    catch (const std::exception &ex)
    {
        refreshConnectionState();
        emit connectionError(QString::fromUtf8(ex.what()));
        return;
    }

    refreshConnectionState();
}

void ConnectionService::disconnectFromServer()
{
    if (m_polling_timer->isActive())
    {
        m_polling_timer->stop();
    }
    m_clientBackend.disconnect();
    refreshConnectionState();
}

void ConnectionService::refreshConnectionState()
{
    publishConnectionState(m_clientBackend.getConnectionState());
}

ClientConfig ConnectionService::loadClientConfig(const QString &configPath)
{
    for (const auto &path : buildConfigCandidates(configPath))
    {
        if (!std::filesystem::exists(path))
        {
            continue;
        }

        try
        {
            return ClientConfig().readFromFile(path.string());
        }
        catch (...)
        {
            continue;
        }
    }

    return buildDefaultClientConfig();
}

void ConnectionService::publishConnectionState(connection::ConnectionState state)
{
    if (m_lastKnownState == state)
    {
        return;
    }

    m_lastKnownState = state;
    emit connectionStateChanged(state);
}
void ConnectionService::pollBackend()
{
    refreshConnectionState();
    // get error messages and display if any
    for (const auto statusItem : m_clientBackend.drainStatus())
    {
        emit connectionError(QString::fromStdString(statusItem));
    }
    // get reposnses from the backend for json messages
    for (const auto &responseMessage : m_clientBackend.drainQueue())
    {
        emit backendResponse(QString::fromStdString(responseMessage.dump()));
    }
    const auto &connectionState = m_clientBackend.getConnectionState();
    if (connectionState == connection::ConnectionState::ConnectionLost ||
        connectionState == connection::ConnectionState::Disconnected)
    {
        m_polling_timer->stop();
    }
}
