#include "ErrorComponent.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

ErrorComponent::ErrorComponent(QWidget *parent)
    : QWidget(parent)
    , m_messageLabel(new QLabel(this))
    , m_dismissButton(new QPushButton("Ukryj", this))
{
    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(14, 12, 14, 12);
    layout->setSpacing(12);

    m_messageLabel->setWordWrap(true);
    m_dismissButton->setObjectName("secondaryButton");

    layout->addWidget(m_messageLabel, 1);
    layout->addWidget(m_dismissButton, 0, Qt::AlignTop);

    setObjectName("errorSurface");
    hide();

    connect(m_dismissButton, &QPushButton::clicked, this, [this] {
        hide();
        emit dismissed();
    });
}

ErrorComponent::~ErrorComponent() = default;

void ErrorComponent::showError(const QString &message)
{
    m_messageLabel->setText(message);
    show();
}

void ErrorComponent::clearError()
{
    m_messageLabel->clear();
    hide();
}
