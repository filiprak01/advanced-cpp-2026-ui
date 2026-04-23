#include "AuthService.h"

#include <exception>
#include <string>

namespace
{
QString localizeAuthError(const QString &message)
{
    const QString normalized = message.trimmed().toLower();

    if (normalized == QStringLiteral("unauthorized"))
    {
        return QStringLiteral("Niepoprawna nazwa uzytkownika lub haslo.");
    }

    if (normalized == QStringLiteral("invalid payload"))
    {
        return QStringLiteral("Niepoprawne dane formularza. Sprawdz dlugosc loginu i hasla.");
    }

    if (normalized == QStringLiteral("user_already_exists") ||
        normalized.contains(QStringLiteral("already exists")))
    {
        return QStringLiteral("Taki uzytkownik juz istnieje.");
    }

    if (normalized == QStringLiteral("invalid_username"))
    {
        return QStringLiteral("Niepoprawna nazwa uzytkownika.");
    }

    if (normalized == QStringLiteral("invalid_password"))
    {
        return QStringLiteral("Niepoprawne haslo.");
    }

    return message;
}
}  // namespace

AuthService::AuthService(ConnectionService* connectionService, QObject* parent)
    : QObject(parent)
{
    setConnectionService(connectionService);
}

AuthService::~AuthService() = default;

ConnectionService* AuthService::connectionService() const
{
    return m_connectionService;
}

void AuthService::setConnectionService(ConnectionService* connectionService)
{
    if (m_connectionService == connectionService) {
        return;
    }

    if (m_connectionService) {
        disconnect(m_connectionService, nullptr, this, nullptr);
    }

    m_connectionService = connectionService;

    if (m_connectionService) {
        connect(m_connectionService,
                &ConnectionService::backendResponse,
                this,
                &AuthService::handleBackendResponse);
    }
}

void AuthService::login(const QString &username, const QString &password)
{
    if (!m_connectionService) {
        emit authError(QStringLiteral("Usluga polaczenia nie jest dostepna."));
        return;
    }

    m_pendingLoginUsername = username;

    try {
        m_connectionService->backend().login(username.toStdString(), password.toStdString());
    } catch (const std::exception &ex) {
        emit authError(QString::fromUtf8(ex.what()));
    }
}

void AuthService::registerUser(const QString &username, const QString &password)
{
    if (!m_connectionService) {
        emit authError(QStringLiteral("Usluga polaczenia nie jest dostepna."));
        return;
    }

    m_pendingRegisterUsername = username;

    try {
        m_connectionService->backend().registerUser(username.toStdString(), password.toStdString());
    } catch (const std::exception &ex) {
        emit authError(QString::fromUtf8(ex.what()));
    }
}

void AuthService::handleBackendResponse(const QString &message)
{
    json response;
    try {
        response = json::parse(message.toStdString());
    } catch (const json::parse_error&) {
        return;
    }

    const std::string type = response.value("type", "");
    const std::string status = response.value("status", "");

    if (type == "error" || status == "error") {
        emit authError(errorMessageFromJson(response));
        return;
    }

    if (type == "login_response") {
        if (status != "ok") {
            emit authError(errorMessageFromJson(response));
            return;
        }

        const json payload = response.value("payload", json::object());
        const std::string username = payload.value("userName", m_pendingLoginUsername.toStdString());
        emit loginSucceeded(QString::fromStdString(username));
        m_pendingLoginUsername.clear();
        return;
    }

    if (type == "register_response") {
        if (status != "ok") {
            emit authError(errorMessageFromJson(response));
            return;
        }

        const json payload = response.value("payload", json::object());
        const std::string username = payload.value("userName", m_pendingRegisterUsername.toStdString());
        const QString registeredUsername = QString::fromStdString(username);
        emit registerSucceeded(registeredUsername);
        emit loginSucceeded(registeredUsername);
        m_pendingRegisterUsername.clear();
        return;
    }
}

QString AuthService::errorMessageFromJson(const json &response) const
{
    if (response.contains("payload") && response["payload"].is_object()) {
        const std::string reason = response["payload"].value("reason", "");
        if (!reason.empty()) {
            return localizeAuthError(QString::fromStdString(reason));
        }
    }

    const std::string message = response.value("message", "");
    if (!message.empty()) {
        return localizeAuthError(QString::fromStdString(message));
    }

    return QStringLiteral("Logowanie lub rejestracja nie powiodly sie.");
}
