#include "ConnectionPage.h"

#include "components/AuthToggleComponent.h"
#include "components/ConnectionStatusComponent.h"
#include "components/ErrorComponent.h"
#include "components/LoadingComponent.h"
#include "components/LoginFormComponent.h"
#include "components/RegistrationFormComponent.h"

#include <QFrame>
#include <QLabel>
#include <QStackedWidget>
#include <QVBoxLayout>

ConnectionPage::ConnectionPage(QWidget *parent)
    : QWidget(parent)
    , m_statusComponent(new ConnectionStatusComponent(this))
    , m_loginForm(new LoginFormComponent(this))
    , m_registrationForm(new RegistrationFormComponent(this))
    , m_authToggle(new AuthToggleComponent(this))
    , m_loadingComponent(new LoadingComponent(this))
    , m_errorComponent(new ErrorComponent(this))
    , m_formStack(new QStackedWidget(this))
{
    auto *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(28, 28, 28, 28);
    rootLayout->setSpacing(18);

    auto *authCard = new QFrame(this);
    authCard->setObjectName("surfaceCard");
    auto *authLayout = new QVBoxLayout(authCard);
    authLayout->setContentsMargins(32, 32, 32, 32);
    authLayout->setSpacing(18);

    auto *titleLabel = new QLabel("PWChat", authCard);
    titleLabel->setObjectName("heroTitle");
    titleLabel->setAlignment(Qt::AlignCenter);

    auto *subtitleLabel = new QLabel("Klient czatu - logowanie i rejestracja", authCard);
    subtitleLabel->setObjectName("heroBody");
    subtitleLabel->setAlignment(Qt::AlignCenter);

    m_formStack->addWidget(m_loginForm);
    m_formStack->addWidget(m_registrationForm);

    authLayout->addWidget(titleLabel);
    authLayout->addWidget(subtitleLabel);
    authLayout->addWidget(m_statusComponent);
    authLayout->addWidget(m_formStack, 1);
    authLayout->addWidget(m_loadingComponent);
    authLayout->addWidget(m_errorComponent);
    authLayout->addWidget(m_authToggle);

    rootLayout->addStretch();
    rootLayout->addWidget(authCard, 0, Qt::AlignCenter);
    rootLayout->addStretch();
    authCard->setMinimumWidth(430);
    authCard->setMaximumWidth(560);

    connect(m_loginForm, &LoginFormComponent::loginRequested, this, &ConnectionPage::loginRequested);
    connect(m_registrationForm,
            &RegistrationFormComponent::registrationRequested,
            this,
            &ConnectionPage::registrationRequested);
    connect(m_authToggle, &AuthToggleComponent::toggleToRegister, this, &ConnectionPage::showRegistrationForm);
    connect(m_authToggle, &AuthToggleComponent::toggleToLogin, this, &ConnectionPage::showLoginForm);
    connect(m_errorComponent, &ErrorComponent::dismissed, this, &ConnectionPage::clearError);

    showLoginForm();
    clearError();
}

ConnectionPage::~ConnectionPage() = default;

void ConnectionPage::showError(const QString &message)
{
    m_errorComponent->showError(message);
}

void ConnectionPage::clearError()
{
    m_errorComponent->clearError();
}

void ConnectionPage::setBusy(bool busy)
{
    m_loginForm->setBusy(busy);
    m_registrationForm->setBusy(busy);
    m_loadingComponent->setVisible(busy);
}

void ConnectionPage::setConnectionStatus(const QString &text, const QColor &color)
{
    m_statusComponent->setStatus(text, color);
}

void ConnectionPage::showLoginForm()
{
    m_formStack->setCurrentWidget(m_loginForm);
    m_authToggle->setLoginMode(true);
    clearError();
}

void ConnectionPage::showRegistrationForm()
{
    m_formStack->setCurrentWidget(m_registrationForm);
    m_authToggle->setLoginMode(false);
    clearError();
}
