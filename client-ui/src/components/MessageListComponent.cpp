#include "MessageListComponent.h"

#include "MessageItemComponent.h"

#include <QLabel>
#include <QLayoutItem>
#include <QScrollArea>
#include <QScrollBar>
#include <QTimer>
#include <QVBoxLayout>

MessageListComponent::MessageListComponent(QWidget *parent)
    : QWidget(parent)
    , m_scrollArea(new QScrollArea(this))
    , m_container(new QWidget(this))
    , m_messageLayout(new QVBoxLayout())
    , m_emptyLabel(new QLabel("Wiadomosci pojawia sie tutaj po wybraniu kanalu.", this))
{
    auto *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(0, 0, 0, 0);

    m_messageLayout->setContentsMargins(0, 0, 0, 0);
    m_messageLayout->setSpacing(12);
    m_messageLayout->addStretch();

    m_emptyLabel->setObjectName("sectionBody");
    m_emptyLabel->setAlignment(Qt::AlignCenter);
    m_emptyLabel->setWordWrap(true);

    m_container->setLayout(m_messageLayout);
    m_scrollArea->setWidget(m_container);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFrameShape(QFrame::NoFrame);

    rootLayout->addWidget(m_emptyLabel);
    rootLayout->addWidget(m_scrollArea);
}

MessageListComponent::~MessageListComponent() = default;

void MessageListComponent::setMessages(const QList<MessageData> &messages)
{
    m_messages = messages;
    rebuildMessages();
}

void MessageListComponent::appendMessage(const MessageData &message)
{
    m_messages.append(message);
    rebuildMessages();
}

void MessageListComponent::clearMessages()
{
    m_messages.clear();
    rebuildMessages();
}

void MessageListComponent::rebuildMessages()
{
    while (QLayoutItem *item = m_messageLayout->takeAt(0))
    {
        if (QWidget *widget = item->widget())
            widget->deleteLater();
        delete item;
    }

    m_emptyLabel->setVisible(m_messages.isEmpty());

    for (const MessageData &message : std::as_const(m_messages))
        m_messageLayout->addWidget(new MessageItemComponent(message, m_container));

    m_messageLayout->addStretch();
    scrollToBottom();
}

void MessageListComponent::scrollToBottom()
{
    QTimer::singleShot(0, this, [this] {
        if (m_scrollArea->verticalScrollBar() != nullptr)
        {
            auto *scrollBar = m_scrollArea->verticalScrollBar();
            scrollBar->setValue(scrollBar->maximum());
        }
    });
}
