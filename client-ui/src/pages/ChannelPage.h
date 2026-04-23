#pragma once

#include <QList>
#include <QWidget>

#include "models/ChannelData.h"
#include "models/MessageData.h"
#include "models/UserData.h"

class HeaderBarComponent;
class QLabel;
class MessageInputComponent;
class MessageListComponent;
class QPushButton;
class SidebarComponent;
class UserListComponent;

/**
 * @brief Glowna strona pracy w pojedynczym kanale.
 *
 * ChannelPage laczy sidebar z kanalami, liste wiadomosci, pole wprowadzania
 * oraz liste uczestnikow. Klasa odpowiada za prezentacje aktualnego stanu
 * wybranego kanalu, ale nie powinna przechowywac logiki komunikacyjnej.
 */
class ChannelPage : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Tworzy ekran pojedynczego kanalu.
     * @param parent Widget rodzic.
     */
    explicit ChannelPage(QWidget *parent = nullptr);
    ~ChannelPage() override;

signals:
    void logoutRequested();
    void backRequested();
    void channelSelected(int channelId);
    void deleteChannelRequested(int channelId);
    void messageSent(const QString &text);

public slots:
    /**
     * @brief Ustawia liste kanalow widoczna w sidebarze.
     * @param channels Lista kanalow dostepnych w UI.
     */
    void setChannels(const QList<ChannelData> &channels);

    /**
     * @brief Ustawia kanal, ktory ma byc wyswietlany jako aktywny.
     * @param channel Dane biezacego kanalu.
     */
    void setCurrentChannel(const ChannelData &channel);

    /**
     * @brief Podmienia cala liste wiadomosci wyswietlanych na ekranie.
     * @param messages Lista wiadomosci przypisana do aktywnego kanalu.
     */
    void setMessages(const QList<MessageData> &messages);

    /**
     * @brief Ustawia liste uzytkownikow widoczna w panelu bocznym.
     * @param users Lista uczestnikow kanalu.
     */
    void setUsers(const QList<UserData> &users);

    /**
     * @brief Dopisuje pojedyncza wiadomosc do widocznej listy.
     * @param message Wiadomosc do dodania.
     */
    void appendMessage(const MessageData &message);

private:
    HeaderBarComponent *m_headerBar;
    SidebarComponent *m_sidebar;
    QLabel *m_topicLabel;
    QLabel *m_channelSummaryLabel;
    QPushButton *m_deleteChannelButton;
    MessageListComponent *m_messageList;
    MessageInputComponent *m_messageInput;
    UserListComponent *m_userList;
    ChannelData m_currentChannel;
};
