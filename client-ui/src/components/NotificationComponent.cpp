#include "NotificationComponent.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QTimer>

NotificationComponent::NotificationComponent(QWidget *parent)
    : QWidget(parent)
    , m_messageLabel(new QLabel(this))
    , m_hideTimer(new QTimer(this))
{
    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(16, 12, 16, 12);
    layout->addWidget(m_messageLabel);

    setAttribute(Qt::WA_ShowWithoutActivating);
    setObjectName("notificationSurface");
    m_messageLabel->setWordWrap(true);
    m_hideTimer->setSingleShot(true);

    connect(m_hideTimer, &QTimer::timeout, this, &NotificationComponent::hide);
    hide();
}

NotificationComponent::~NotificationComponent() = default;

void NotificationComponent::showInfo(const QString &message)
{
    showMessage(message, "#1f6f78", "#144b52");
}

void NotificationComponent::showError(const QString &message)
{
    showMessage(message, "#a13d2d", "#7b2517");
}

void NotificationComponent::showMessage(
    const QString &message,
    const QString &background,
    const QString &border)
{
    m_messageLabel->setText(message);
    setStyleSheet(QString(
                      "#notificationSurface{background:%1;color:#f9f6f1;border:1px solid %2;"
                      "border-radius:14px;}"
                      "#notificationSurface QLabel{color:#f9f6f1;}")
                      .arg(background, border));
    adjustSize();
    reposition();
    show();
    raise();
    m_hideTimer->start(3000);
}

void NotificationComponent::reposition()
{
    if (parentWidget() == nullptr)
        return;

    const int x = parentWidget()->width() - width() - 24;
    move(qMax(24, x), 24);
}
