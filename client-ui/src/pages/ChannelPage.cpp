#include "ChannelPage.h"

#include "components/HeaderBarComponent.h"
#include "components/MessageInputComponent.h"
#include "components/MessageListComponent.h"
#include "components/SidebarComponent.h"
#include "components/UserListComponent.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

ChannelPage::ChannelPage(QWidget *parent)
    : QWidget(parent)
    , m_headerBar(new HeaderBarComponent(this))
    , m_sidebar(new SidebarComponent(this))
    , m_topicLabel(new QLabel(this))
    , m_channelSummaryLabel(new QLabel(this))
    , m_deleteChannelButton(new QPushButton("Usun kanal", this))
    , m_messageList(new MessageListComponent(this))
    , m_messageInput(new MessageInputComponent(this))
    , m_userList(new UserListComponent(this))
{
    auto *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(22, 22, 22, 22);
    rootLayout->setSpacing(18);

    auto *contentLayout = new QHBoxLayout();
    contentLayout->setSpacing(18);

    auto *chatCard = new QFrame(this);
    chatCard->setObjectName("surfaceCard");
    auto *chatLayout = new QVBoxLayout(chatCard);
    chatLayout->setContentsMargins(24, 24, 24, 24);
    chatLayout->setSpacing(16);

    m_topicLabel->setObjectName("heroTitle");
    m_topicLabel->setWordWrap(true);
    m_channelSummaryLabel->setObjectName("heroBody");
    m_channelSummaryLabel->setWordWrap(true);
    m_deleteChannelButton->setObjectName("secondaryButton");

    chatLayout->addWidget(m_topicLabel);
    chatLayout->addWidget(m_channelSummaryLabel);
    chatLayout->addWidget(m_deleteChannelButton, 0, Qt::AlignRight);
    chatLayout->addWidget(m_messageList, 1);
    chatLayout->addWidget(m_messageInput);

    m_headerBar->setTitle("PWChat");
    m_headerBar->setBackVisible(true);

    contentLayout->addWidget(m_sidebar, 4);
    contentLayout->addWidget(chatCard, 8);
    contentLayout->addWidget(m_userList, 4);

    rootLayout->addWidget(m_headerBar);
    rootLayout->addLayout(contentLayout, 1);

    connect(m_headerBar, &HeaderBarComponent::backRequested, this, &ChannelPage::backRequested);
    connect(m_headerBar, &HeaderBarComponent::logoutRequested, this, &ChannelPage::logoutRequested);
    connect(m_sidebar, &SidebarComponent::channelSelected, this, &ChannelPage::channelSelected);
    connect(m_messageInput, &MessageInputComponent::messageSent, this, &ChannelPage::messageSent);
    connect(m_deleteChannelButton, &QPushButton::clicked, this, [this] {
        if (m_currentChannel.id >= 0) {
            emit deleteChannelRequested(m_currentChannel.id);
        }
    });

    setCurrentChannel(ChannelData{});
}

ChannelPage::~ChannelPage() = default;

void ChannelPage::setChannels(const QList<ChannelData> &channels)
{
    m_sidebar->setChannels(channels);
}

void ChannelPage::setCurrentChannel(const ChannelData &channel)
{
    m_currentChannel = channel;
    m_sidebar->setCurrentChannel(channel.id);

    const QString channelName = channel.name.isEmpty() ? "Wybierz kanal" : channel.name;
    const QString topic = channel.topic.isEmpty()
        ? "Kanal zsynchronizowany z serwerem."
        : channel.topic;

    m_headerBar->setContext(channel.name.isEmpty() ? "Nie wybrano kanalu" : QString("Kanal: %1").arg(channel.name));
    m_topicLabel->setText(channel.name.isEmpty() ? "Wybierz kanal" : channelName);
    m_channelSummaryLabel->setText(
        channel.name.isEmpty()
            ? "Utworz albo wybierz kanal, aby wczytac wiadomosci."
            : QString("%1 uzytkownikow | %2 | %3")
                  .arg(channel.memberCount)
                  .arg(channel.isPrivate ? "prywatny" : "otwarty", topic));
    m_deleteChannelButton->setEnabled(!channel.name.isEmpty() && channel.id >= 0);
    m_deleteChannelButton->setVisible(!channel.name.isEmpty() && channel.id >= 0);
    m_messageInput->setPlaceholderText(
        channel.name.isEmpty() ? "Najpierw wybierz kanal..." : QString("Wiadomosc do %1").arg(channel.name));
}

void ChannelPage::setMessages(const QList<MessageData> &messages)
{
    m_messageList->setMessages(messages);
}

void ChannelPage::setUsers(const QList<UserData> &users)
{
    m_userList->setUsers(users);
}

void ChannelPage::appendMessage(const MessageData &message)
{
    m_messageList->appendMessage(message);
}
