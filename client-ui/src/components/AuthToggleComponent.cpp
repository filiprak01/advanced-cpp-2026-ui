#include "AuthToggleComponent.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

AuthToggleComponent::AuthToggleComponent(QWidget *parent)
    : QWidget(parent)
    , m_promptLabel(new QLabel(this))
    , m_toggleButton(new QPushButton(this))
{
    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(8);
    layout->addWidget(m_promptLabel);
    layout->addWidget(m_toggleButton, 0, Qt::AlignLeft);
    layout->addStretch();

    m_toggleButton->setObjectName("ghostButton");

    connect(m_toggleButton, &QPushButton::clicked, this, [this] {
        if (m_loginMode)
            emit toggleToRegister();
        else
            emit toggleToLogin();
    });

    setLoginMode(true);
}

AuthToggleComponent::~AuthToggleComponent() = default;

void AuthToggleComponent::setLoginMode(bool loginMode)
{
    m_loginMode = loginMode;
    m_promptLabel->setText(loginMode ? "Nie masz konta?" : "Masz juz konto?");
    m_toggleButton->setText(loginMode ? "Przejdz do rejestracji" : "Przejdz do logowania");
}
