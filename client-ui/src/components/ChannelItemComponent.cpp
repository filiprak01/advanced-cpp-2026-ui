#include "ChannelItemComponent.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QSizePolicy>
#include <QVBoxLayout>

ChannelItemComponent::ChannelItemComponent(const ChannelData &channel, QWidget *parent)
    : QWidget(parent)
    , m_channel(channel)
    , m_nameLabel(new QLabel(this))
    , m_metaLabel(new QLabel(this))
    , m_badgeLabel(new QLabel(this))
{
    auto *rootLayout = new QHBoxLayout(this);
    rootLayout->setContentsMargins(14, 12, 14, 12);
    rootLayout->setSpacing(12);

    auto *textLayout = new QVBoxLayout();
    textLayout->setContentsMargins(0, 0, 0, 0);
    textLayout->setSpacing(4);
    textLayout->addWidget(m_nameLabel);
    textLayout->addWidget(m_metaLabel);

    m_nameLabel->setObjectName("channelTitle");
    m_nameLabel->setWordWrap(true);
    m_nameLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_metaLabel->setObjectName("channelMeta");
    m_metaLabel->setWordWrap(true);
    m_badgeLabel->setObjectName("channelBadge");
    m_badgeLabel->setAlignment(Qt::AlignCenter);
    m_badgeLabel->setFixedWidth(92);

    rootLayout->addLayout(textLayout, 1);
    rootLayout->addWidget(m_badgeLabel, 0, Qt::AlignTop);

    setCursor(Qt::PointingHandCursor);
    refresh();
}

ChannelItemComponent::~ChannelItemComponent() = default;

void ChannelItemComponent::setChannel(const ChannelData &channel)
{
    m_channel = channel;
    refresh();
}

void ChannelItemComponent::setSelected(bool selected)
{
    m_selected = selected;
    refresh();
}

void ChannelItemComponent::mousePressEvent(QMouseEvent *event)
{
    emit channelSelected(m_channel.id);
    QWidget::mousePressEvent(event);
}

void ChannelItemComponent::refresh()
{
    m_nameLabel->setText(m_channel.name.isEmpty() ? QStringLiteral("Bez nazwy") : m_channel.name);

    const QString activity = m_channel.isActive ? "aktywny" : "archiwalny";
    const QString topic = m_channel.topic.isEmpty()
        ? (m_channel.isPrivate ? QStringLiteral("rozmowa prywatna") : QStringLiteral("kanal publiczny"))
        : m_channel.topic;
    m_metaLabel->setText(
        QString("%1 uczestnikow | %2 | %3").arg(m_channel.memberCount).arg(activity, topic));
    m_badgeLabel->setText(m_channel.isPrivate ? "Prywatny" : "Otwarty");

    const QString background = m_selected ? "#1f6f78" : "#f8f5ef";
    const QString titleColor = m_selected ? "#f8f5ef" : "#1f2a33";
    const QString metaColor = m_selected ? "#d5ecef" : "#66727d";
    const QString border = m_selected ? "#1f6f78" : "#d9d2c7";
    const QString badgeBackground = m_channel.isPrivate ? "#f97316" : "#dcfce7";
    const QString badgeColor = m_channel.isPrivate ? "#fff7ed" : "#166534";
    const QString badgeBorder = m_channel.isPrivate ? "#ea580c" : "#86efac";

    setStyleSheet(QString(
                      "ChannelItemComponent{background:%1;border:1px solid %2;border-radius:16px;}"
                      "QLabel#channelTitle{color:%3;font-size:16px;font-weight:700;}"
                      "QLabel#channelMeta{color:%4;}"
                      "QLabel#channelBadge{background:%5;color:%6;border:1px solid %7;"
                      "border-radius:10px;padding:4px 8px;font-weight:700;}")
                      .arg(background, border, titleColor, metaColor, badgeBackground, badgeColor, badgeBorder));
}
