#include "MessageItemComponent.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

MessageItemComponent::MessageItemComponent(const MessageData &message, QWidget *parent)
    : QWidget(parent)
    , m_message(message)
    , m_bubbleFrame(new QFrame(this))
    , m_metaLabel(new QLabel(m_bubbleFrame))
    , m_contentLabel(new QLabel(m_bubbleFrame))
{
    auto *rootLayout = new QHBoxLayout(this);
    rootLayout->setContentsMargins(0, 0, 0, 0);

    auto *bubbleLayout = new QVBoxLayout(m_bubbleFrame);
    bubbleLayout->setContentsMargins(16, 12, 16, 12);
    bubbleLayout->setSpacing(6);
    bubbleLayout->addWidget(m_metaLabel);
    bubbleLayout->addWidget(m_contentLabel);

    m_metaLabel->setObjectName("messageMeta");
    m_contentLabel->setObjectName("messageBody");
    m_contentLabel->setWordWrap(true);

    if (m_message.isOwnMessage)
        rootLayout->addStretch();

    rootLayout->addWidget(m_bubbleFrame, 0, m_message.isOwnMessage ? Qt::AlignRight : Qt::AlignLeft);

    if (!m_message.isOwnMessage)
        rootLayout->addStretch();

    refresh();
}

MessageItemComponent::~MessageItemComponent() = default;

void MessageItemComponent::setMessage(const MessageData &message)
{
    m_message = message;
    refresh();
}

void MessageItemComponent::refresh()
{
    const QString editedSuffix = m_message.isEdited ? " | edytowano" : "";
    const QString timeText = m_message.timestamp.isValid()
        ? m_message.timestamp.toString("HH:mm")
        : "teraz";

    m_metaLabel->setText(QString("%1 | %2%3").arg(m_message.senderName, timeText, editedSuffix));
    m_contentLabel->setText(m_message.content);

    const QString bubbleBackground = m_message.isOwnMessage ? "#1f6f78" : "#f8f5ef";
    const QString bubbleBorder = m_message.isOwnMessage ? "#1a5d64" : "#d9d2c7";
    const QString textColor = m_message.isOwnMessage ? "#f8f5ef" : "#1f2a33";
    const QString metaColor = m_message.isOwnMessage ? "#d5ecef" : "#66727d";

    m_bubbleFrame->setStyleSheet(QString(
                                     "QFrame{background:%1;border:1px solid %2;border-radius:18px;}"
                                     "QLabel#messageMeta{color:%3;font-weight:700;}"
                                     "QLabel#messageBody{color:%4;}")
                                     .arg(bubbleBackground, bubbleBorder, metaColor, textColor));
}
