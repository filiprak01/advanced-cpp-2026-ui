#include "ChannelListPage.h"

#include "components/HeaderBarComponent.h"
#include "components/SidebarComponent.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

ChannelListPage::ChannelListPage(QWidget *parent)
    : QWidget(parent)
    , m_headerBar(new HeaderBarComponent(this))
    , m_sidebar(new SidebarComponent(this))
    , m_summaryLabel(new QLabel(this))
    , m_focusLabel(new QLabel(this))
    , m_channelNameInput(new QLineEdit(this))
    , m_channelUsersInput(new QLineEdit(this))
    , m_createChannelButton(new QPushButton("Utworz kanal", this))
{
    auto *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(22, 22, 22, 22);
    rootLayout->setSpacing(18);

    auto *contentLayout = new QHBoxLayout();
    contentLayout->setSpacing(18);

    auto *overviewCard = new QFrame(this);
    overviewCard->setObjectName("surfaceCard");
    auto *overviewLayout = new QVBoxLayout(overviewCard);
    overviewLayout->setContentsMargins(24, 24, 24, 24);
    overviewLayout->setSpacing(16);

    auto *eyebrowLabel = new QLabel("Kanaly", overviewCard);
    eyebrowLabel->setObjectName("eyebrow");

    auto *titleLabel = new QLabel("Wybierz kanal", overviewCard);
    titleLabel->setObjectName("heroTitle");
    titleLabel->setWordWrap(true);

    m_summaryLabel->setObjectName("heroBody");
    m_summaryLabel->setWordWrap(true);

    auto *focusCard = new QFrame(overviewCard);
    focusCard->setObjectName("accentCard");
    auto *focusCardLayout = new QVBoxLayout(focusCard);
    focusCardLayout->setContentsMargins(18, 18, 18, 18);
    focusCardLayout->setSpacing(10);

    auto *focusTitle = new QLabel("Akcje kanalow", focusCard);
    focusTitle->setObjectName("sectionTitle");
    m_focusLabel->setObjectName("sectionBody");
    m_focusLabel->setWordWrap(true);
    m_focusLabel->setText(
        "Utworz kanal publiczny, wybierz go z listy i zacznij pisac po synchronizacji.");
    focusCardLayout->addWidget(focusTitle);
    focusCardLayout->addWidget(m_focusLabel);

    auto *createCard = new QFrame(overviewCard);
    createCard->setObjectName("surfaceCard");
    auto *createLayout = new QVBoxLayout(createCard);
    createLayout->setContentsMargins(18, 18, 18, 18);
    createLayout->setSpacing(10);
    auto *createTitle = new QLabel("Nowy kanal", createCard);
    createTitle->setObjectName("sectionTitle");
    m_channelNameInput->setPlaceholderText("nazwa kanalu");
    m_channelUsersInput->setPlaceholderText("uczestnicy po przecinku (opcjonalnie)");
    createLayout->addWidget(createTitle);
    createLayout->addWidget(m_channelNameInput);
    createLayout->addWidget(m_channelUsersInput);
    createLayout->addWidget(m_createChannelButton);

    overviewLayout->addWidget(eyebrowLabel);
    overviewLayout->addWidget(titleLabel);
    overviewLayout->addWidget(m_summaryLabel);
    overviewLayout->addWidget(createCard);
    overviewLayout->addStretch();
    overviewLayout->addWidget(focusCard);

    m_headerBar->setTitle("PWChat");
    m_headerBar->setContext("Przeglad kanalow");
    m_headerBar->setBackVisible(false);

    contentLayout->addWidget(m_sidebar, 4);
    contentLayout->addWidget(overviewCard, 7);

    rootLayout->addWidget(m_headerBar);
    rootLayout->addLayout(contentLayout, 1);

    connect(m_headerBar, &HeaderBarComponent::logoutRequested, this, &ChannelListPage::logoutRequested);
    connect(m_sidebar, &SidebarComponent::channelSelected, this, &ChannelListPage::channelSelected);
    connect(m_createChannelButton, &QPushButton::clicked, this, [this] {
        const QString channelName = m_channelNameInput->text().trimmed();
        if (channelName.isEmpty()) {
            return;
        }

        QStringList usernames;
        for (const QString &username : m_channelUsersInput->text().split(',', Qt::SkipEmptyParts)) {
            const QString trimmed = username.trimmed();
            if (!trimmed.isEmpty() && !usernames.contains(trimmed)) {
                usernames.append(trimmed);
            }
        }

        emit createChannelRequested(channelName, usernames);
        m_channelNameInput->clear();
        m_channelUsersInput->clear();
    });
    connect(m_channelNameInput, &QLineEdit::returnPressed, m_createChannelButton, &QPushButton::click);
    connect(m_channelUsersInput, &QLineEdit::returnPressed, m_createChannelButton, &QPushButton::click);

    setChannels({});
}

ChannelListPage::~ChannelListPage() = default;

void ChannelListPage::setChannels(const QList<ChannelData> &channels)
{
    m_sidebar->setChannels(channels);
    m_summaryLabel->setText(
        channels.isEmpty()
            ? "Nie ma jeszcze kanalow. Utworz pierwszy, aby zaczac rozmowe."
            : QString("Masz %1 kanalow zsynchronizowanych z backendem.")
                  .arg(channels.size()));
}

void ChannelListPage::setCurrentUser(const QString &username)
{
    const QString displayName = username.isEmpty() ? "student" : username;
    m_headerBar->setContext(QString("Zalogowano jako %1").arg(displayName));
}

void ChannelListPage::setAvailableUsers(const QStringList &usernames)
{
    m_focusLabel->setText(
        usernames.isEmpty()
            ? "Najpierw zarejestruj uzytkownikow, potem wpisz ich nazwy w polu uczestnikow."
            : QString("Znani uzytkownicy: %1. Dodawaj uczestnikow, wpisujac nazwy po przecinku.")
                  .arg(usernames.join(", ")));
}
