#include "HeaderBarComponent.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

HeaderBarComponent::HeaderBarComponent(QWidget *parent)
    : QWidget(parent)
    , m_titleLabel(new QLabel("PWChat", this))
    , m_contextLabel(new QLabel("Klient", this))
    , m_backButton(new QPushButton("Wroc", this))
    , m_logoutButton(new QPushButton("Wyloguj", this))
{
    auto *rootLayout = new QHBoxLayout(this);
    rootLayout->setContentsMargins(20, 18, 20, 18);
    rootLayout->setSpacing(16);

    auto *textLayout = new QVBoxLayout();
    textLayout->setContentsMargins(0, 0, 0, 0);
    textLayout->setSpacing(2);
    textLayout->addWidget(m_titleLabel);
    textLayout->addWidget(m_contextLabel);

    m_titleLabel->setObjectName("headerTitle");
    m_contextLabel->setObjectName("headerContext");
    m_backButton->setObjectName("secondaryButton");
    m_logoutButton->setObjectName("secondaryButton");

    rootLayout->addWidget(m_backButton, 0, Qt::AlignVCenter);
    rootLayout->addLayout(textLayout);
    rootLayout->addStretch();
    rootLayout->addWidget(m_logoutButton, 0, Qt::AlignVCenter);

    setObjectName("surfaceCard");
    m_backButton->hide();

    connect(m_backButton, &QPushButton::clicked, this, &HeaderBarComponent::backRequested);
    connect(m_logoutButton, &QPushButton::clicked, this, &HeaderBarComponent::logoutRequested);
}

HeaderBarComponent::~HeaderBarComponent() = default;

void HeaderBarComponent::setTitle(const QString &title)
{
    m_titleLabel->setText(title);
}

void HeaderBarComponent::setContext(const QString &context)
{
    m_contextLabel->setText(context);
}

void HeaderBarComponent::setBackVisible(bool visible)
{
    m_backButton->setVisible(visible);
}

void HeaderBarComponent::setLogoutText(const QString &text)
{
    m_logoutButton->setText(text);
}
