/**
 * @file main.cpp
 * @brief Uruchamia klienta Qt projektu PWChat.
 *
 * Plik odpowiada za start aplikacji, utworzenie kontrolerow, rejestracje
 * stron oraz polaczenie sygnalow UI z warstwa klienta backendowego.
 */
#include <QApplication>
#include <QMainWindow>
#include <QObject>
#include <QStackedWidget>

#include "src/components/NotificationComponent.h"
#include "src/controllers/AuthController.h"
#include "src/controllers/ChannelController.h"
#include "src/controllers/ConnectionController.h"
#include "src/controllers/MessageController.h"
#include "src/pages/ChannelListPage.h"
#include "src/pages/ChannelPage.h"
#include "src/pages/ConnectionPage.h"
#include "src/routing/RouteGuard.h"
#include "src/routing/Router.h"

namespace
{
/**
 * @brief Naklada prosty, minimalistyczny styl na cala aplikacje.
 * @param app Instancja aplikacji Qt.
 */
void applyAppStyle(QApplication &app)
{
    app.setStyle("Fusion");
    app.setStyleSheet(R"(
        QMainWindow, QWidget {
            background: #f5f6f8;
            color: #20242a;
        }
        QFrame#surfaceCard, QWidget#surfaceCard, QWidget#sidebarSurface, QWidget#statusSurface {
            background: #ffffff;
            border: 1px solid #d8dde4;
            border-radius: 10px;
        }
        QFrame#accentCard {
            background: #f1f5f9;
            border: 1px solid #d8dde4;
            border-radius: 10px;
        }
        QFrame#heroPanel {
            background: #ffffff;
            border: 1px solid #d8dde4;
            border-radius: 10px;
        }
        QLabel#eyebrow {
            color: #4e6b91;
            font-size: 12px;
            font-weight: 700;
        }
        QLabel#heroTitle {
            color: #20242a;
            font-size: 22px;
            font-weight: 700;
        }
        QLabel#heroBody {
            color: #5f6b7a;
            font-size: 14px;
        }
        QLabel#sectionTitle {
            color: #20242a;
            font-size: 16px;
            font-weight: 700;
        }
        QLabel#sectionBody, QLabel#headerContext, QLabel#inlineHelper, QLabel#channelMeta, QLabel#messageMeta {
            color: #66707c;
            font-size: 13px;
        }
        QLabel#headerTitle {
            color: #20242a;
            font-size: 18px;
            font-weight: 700;
        }
        QPushButton {
            background: #3b82f6;
            color: #ffffff;
            border: none;
            border-radius: 8px;
            padding: 8px 14px;
            font-weight: 600;
        }
        QPushButton:hover {
            background: #2563eb;
        }
        QPushButton:disabled {
            background: #c3cad5;
            color: #eef1f5;
        }
        QPushButton#secondaryButton {
            background: #ffffff;
            color: #334155;
            border: 1px solid #cbd5e1;
        }
        QPushButton#secondaryButton:hover {
            background: #f8fafc;
        }
        QPushButton#ghostButton {
            background: transparent;
            color: #3b82f6;
            padding-left: 0px;
            padding-right: 0px;
        }
        QPushButton#ghostButton:hover {
            color: #2563eb;
            background: transparent;
        }
        QLineEdit, QListWidget {
            background: #ffffff;
            border: 1px solid #cbd5e1;
            border-radius: 8px;
            padding: 8px 10px;
        }
        QLineEdit:focus {
            border: 1px solid #3b82f6;
        }
        QScrollArea {
            border: none;
            background: transparent;
        }
        QProgressBar {
            background: #e5e7eb;
            border: none;
            border-radius: 6px;
        }
        QProgressBar::chunk {
            background: #3b82f6;
            border-radius: 6px;
        }
        QWidget#errorSurface {
            background: #fef2f2;
            border: 1px solid #fecaca;
            border-radius: 10px;
        }
    )");
}

QColor colorForConnectionState(connection::ConnectionState state)
{
    switch (state) {
    case connection::ConnectionState::Connecting:
        return QColor("#1f6f78");
    case connection::ConnectionState::Connected:
        return QColor("#2f855a");
    case connection::ConnectionState::ConnectionLost:
        return QColor("#c53030");
    case connection::ConnectionState::Disconnected:
    default:
        return QColor("#f28c45");
    }
}

QList<UserData> usersFromChannel(const ChannelData &channel)
{
    QList<UserData> users;
    int nextId = 1;
    for (const QString &username : channel.usernames) {
        users.append(UserData{nextId++, username, QStringLiteral("member"), false, true});
    }
    return users;
}
} // namespace

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("PWChat - klient");
    app.setApplicationVersion("1.0.0");
    applyAppStyle(app);

    QMainWindow window;
    window.setWindowTitle("PWChat - klient");
    window.resize(1180, 760);

    auto *stack = new QStackedWidget(&window);
    window.setCentralWidget(stack);

    auto *notification = new NotificationComponent(stack);
    auto *connectionController = new ConnectionController(&window);
    auto *authController = new AuthController(connectionController, &window);
    auto *channelController = new ChannelController(connectionController->connectionService(), &window);
    auto *messageController = new MessageController(connectionController->connectionService(), &window);
    auto *router = new Router(stack, &window);
    auto *guard = new RouteGuard(router, &window);
    Q_UNUSED(guard);

    auto *connectionPage = new ConnectionPage(stack);
    auto *channelListPage = new ChannelListPage(stack);
    auto *channelPage = new ChannelPage(stack);

    QObject::connect(connectionController, &ConnectionController::stateChanged, &window, [&] {
        connectionPage->setBusy(connectionController->currentState() == connection::ConnectionState::Connecting);
        connectionPage->setConnectionStatus(connectionController->connectionText(),
                                            colorForConnectionState(connectionController->currentState()));

        if (connectionController->lastError().isEmpty()) {
            connectionPage->clearError();
        } else {
            connectionPage->showError(connectionController->lastError());
        }
    });

    QObject::connect(connectionPage,
                     &ConnectionPage::loginRequested,
                     authController,
                     &AuthController::login);
    QObject::connect(connectionPage,
                     &ConnectionPage::registrationRequested,
                     authController,
                     &AuthController::registerUser);
    QObject::connect(authController, &AuthController::authError, connectionPage, &ConnectionPage::showError);
    QObject::connect(authController, &AuthController::loginSucceeded, &window, [&](const QString &username) {
        connectionPage->setBusy(false);
        channelListPage->setCurrentUser(username);
        notification->showInfo("Zalogowano. Synchronizuje kanaly i wiadomosci.");
        router->navigateTo("/channels");
    });
    QObject::connect(authController, &AuthController::registerSucceeded, &window, [&](const QString &) {
        connectionPage->setBusy(false);
        notification->showInfo("Konto utworzone. Trwa przejscie do panelu kanalow.");
    });

    QObject::connect(channelController->state(), &ChannelState::channelsUpdated, &window, [&] {
        const QList<ChannelData> channels = channelController->state()->channels();
        channelListPage->setChannels(channels);
        channelPage->setChannels(channels);
        channelPage->setCurrentChannel(channelController->state()->currentChannel());
        channelPage->setUsers(usersFromChannel(channelController->state()->currentChannel()));
    });
    QObject::connect(channelController->state(), &ChannelState::availableUsernamesUpdated, &window, [&] {
        channelListPage->setAvailableUsers(channelController->state()->availableUsernames());
    });
    QObject::connect(channelController->state(), &ChannelState::currentChannelChanged, &window, [&](int channelId) {
        const ChannelData channel = channelController->state()->channelById(channelId);
        channelPage->setCurrentChannel(channel);
        channelPage->setUsers(usersFromChannel(channel));
        channelPage->setMessages(messageController->state()->messagesForChannel(channelId));
    });
    QObject::connect(messageController->state(), &MessageState::messagesUpdated, &window, [&](int channelId) {
        if (channelId == channelController->state()->currentChannelId()) {
            channelPage->setMessages(messageController->state()->messagesForChannel(channelId));
        }
    });
    QObject::connect(messageController, &MessageController::messageError, connectionPage, &ConnectionPage::showError);
    QObject::connect(channelController, &ChannelController::channelError, notification, &NotificationComponent::showError);

    QObject::connect(channelListPage,
                     &ChannelListPage::createChannelRequested,
                     channelController,
                     &ChannelController::createChannel);
    QObject::connect(channelListPage, &ChannelListPage::channelSelected, &window, [&](int channelId) {
        channelController->selectChannel(channelId);
        router->navigateTo("/channel");
    });
    QObject::connect(channelListPage, &ChannelListPage::logoutRequested, &window, [&] {
        authController->logout();
        connectionPage->setConnectionStatus("Sesja zakonczona", QColor("#f28c45"));
        notification->showInfo("Wrociles do ekranu logowania.");
        router->navigateTo("/connection");
    });

    QObject::connect(channelPage, &ChannelPage::channelSelected, &window, [&](int channelId) {
        channelController->selectChannel(channelId);
    });
    QObject::connect(channelPage,
                     &ChannelPage::deleteChannelRequested,
                     channelController,
                     &ChannelController::deleteChannel);
    QObject::connect(channelPage, &ChannelPage::backRequested, &window, [&] {
        router->navigateTo("/channels");
    });
    QObject::connect(channelPage, &ChannelPage::logoutRequested, &window, [&] {
        authController->logout();
        connectionPage->setConnectionStatus("Sesja zakonczona", QColor("#f28c45"));
        router->navigateTo("/connection");
    });
    QObject::connect(channelPage, &ChannelPage::messageSent, &window, [&](const QString &text) {
        messageController->sendMessage(channelController->state()->currentChannelId(), text);
    });

    router->registerRoute("/connection", connectionPage);
    router->registerRoute("/channels", channelListPage);
    router->registerRoute("/channel", channelPage);

    connectionPage->setConnectionStatus(connectionController->connectionText(),
                                        colorForConnectionState(connectionController->currentState()));
    router->navigateTo("/connection");

    window.show();
    return app.exec();
}
