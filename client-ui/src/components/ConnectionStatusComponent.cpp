#include "ConnectionStatusComponent.h"

#include <QHBoxLayout>
#include <QLabel>

ConnectionStatusComponent::ConnectionStatusComponent(QWidget *parent)
    : QWidget(parent)
    , m_indicator(new QLabel(this))
    , m_statusLabel(new QLabel(this))
{
    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(14, 10, 14, 10);
    layout->setSpacing(10);

    auto *captionLabel = new QLabel("Polaczenie", this);
    captionLabel->setObjectName("eyebrow");

    m_indicator->setFixedSize(12, 12);
    m_indicator->setObjectName("connectionIndicator");
    m_statusLabel->setObjectName("sectionBody");

    layout->addWidget(captionLabel);
    layout->addStretch();
    layout->addWidget(m_indicator);
    layout->addWidget(m_statusLabel);

    setObjectName("statusSurface");
    setStatus("Tryb podgladu", QColor("#f28c45"));
}

ConnectionStatusComponent::~ConnectionStatusComponent() = default;

void ConnectionStatusComponent::setStatus(const QString &text, const QColor &color)
{
    m_statusLabel->setText(text);
    m_indicator->setStyleSheet(QString("background:%1;border-radius:6px;").arg(color.name()));
}
