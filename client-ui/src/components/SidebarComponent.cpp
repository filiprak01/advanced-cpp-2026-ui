#include "SidebarComponent.h"

#include "ChannelItemComponent.h"

#include <QLabel>
#include <QLayoutItem>
#include <QScrollArea>
#include <QVBoxLayout>

SidebarComponent::SidebarComponent(QWidget *parent)
    : QWidget(parent)
    , m_countLabel(new QLabel("Kanaly: 0", this))
    , m_scrollArea(new QScrollArea(this))
    , m_listContainer(new QWidget(this))
    , m_listLayout(new QVBoxLayout())
{
    auto *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(18, 18, 18, 18);
    rootLayout->setSpacing(14);

    auto *titleLabel = new QLabel("Kanaly", this);
    titleLabel->setObjectName("sectionTitle");
    m_countLabel->setObjectName("sectionBody");

    m_listLayout->setContentsMargins(0, 0, 0, 0);
    m_listLayout->setSpacing(10);
    m_listLayout->addStretch();

    m_listContainer->setLayout(m_listLayout);
    m_scrollArea->setWidget(m_listContainer);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFrameShape(QFrame::NoFrame);

    rootLayout->addWidget(titleLabel);
    rootLayout->addWidget(m_countLabel);
    rootLayout->addWidget(m_scrollArea, 1);

    setObjectName("sidebarSurface");
    setMinimumWidth(300);
}

SidebarComponent::~SidebarComponent() = default;

void SidebarComponent::setChannels(const QList<ChannelData> &channels)
{
    m_channels = channels;
    m_countLabel->setText(QString("Kanaly: %1").arg(m_channels.size()));
    rebuildList();
}

void SidebarComponent::setCurrentChannel(int channelId)
{
    m_currentChannelId = channelId;
    rebuildList();
}

void SidebarComponent::rebuildList()
{
    while (QLayoutItem *item = m_listLayout->takeAt(0))
    {
        if (QWidget *widget = item->widget())
            widget->deleteLater();
        delete item;
    }

    if (m_channels.isEmpty())
    {
        auto *emptyLabel = new QLabel("Brak kanalow. Utworz kanal po prawej stronie.", this);
        emptyLabel->setWordWrap(true);
        emptyLabel->setObjectName("sectionBody");
        m_listLayout->addWidget(emptyLabel);
    }
    else
    {
        for (const ChannelData &channel : std::as_const(m_channels))
        {
            auto *channelItem = new ChannelItemComponent(channel, m_listContainer);
            channelItem->setSelected(channel.id == m_currentChannelId);
            connect(channelItem, &ChannelItemComponent::channelSelected, this, [this](int channelId) {
                setCurrentChannel(channelId);
                emit channelSelected(channelId);
            });
            m_listLayout->addWidget(channelItem);
        }
    }

    m_listLayout->addStretch();
}
