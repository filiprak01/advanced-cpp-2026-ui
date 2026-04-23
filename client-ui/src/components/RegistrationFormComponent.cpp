#include "RegistrationFormComponent.h"

#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

RegistrationFormComponent::RegistrationFormComponent(QWidget *parent)
    : QWidget(parent)
    , m_usernameEdit(new QLineEdit(this))
    , m_passwordEdit(new QLineEdit(this))
    , m_confirmPasswordEdit(new QLineEdit(this))
    , m_registerButton(new QPushButton("Zarejestruj", this))
    , m_helperLabel(new QLabel(this))
{
    auto *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(12);

    auto *titleLabel = new QLabel("Rejestracja", this);
    titleLabel->setObjectName("sectionTitle");

    auto *subtitleLabel = new QLabel(
        "Utworz konto. Po poprawnej rejestracji zostaniesz przeniesiony do kanalow.",
        this);
    subtitleLabel->setObjectName("sectionBody");
    subtitleLabel->setWordWrap(true);

    auto *formLayout = new QFormLayout();
    formLayout->setLabelAlignment(Qt::AlignLeft);
    formLayout->setFormAlignment(Qt::AlignLeft | Qt::AlignTop);
    formLayout->setHorizontalSpacing(16);
    formLayout->setVerticalSpacing(12);

    m_usernameEdit->setPlaceholderText("nazwa uzytkownika");
    m_usernameEdit->setClearButtonEnabled(true);
    m_passwordEdit->setPlaceholderText("haslo");
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_confirmPasswordEdit->setPlaceholderText("powtorz haslo");
    m_confirmPasswordEdit->setEchoMode(QLineEdit::Password);

    formLayout->addRow("Uzytkownik", m_usernameEdit);
    formLayout->addRow("Haslo", m_passwordEdit);
    formLayout->addRow("Powtorz", m_confirmPasswordEdit);

    m_helperLabel->setObjectName("inlineHelper");
    m_helperLabel->hide();

    rootLayout->addWidget(titleLabel);
    rootLayout->addWidget(subtitleLabel);
    rootLayout->addLayout(formLayout);
    rootLayout->addWidget(m_helperLabel);
    rootLayout->addWidget(m_registerButton);

    connect(m_registerButton, &QPushButton::clicked, this, &RegistrationFormComponent::submit);
    connect(m_confirmPasswordEdit,
            &QLineEdit::returnPressed,
            this,
            &RegistrationFormComponent::submit);
}

RegistrationFormComponent::~RegistrationFormComponent() = default;

void RegistrationFormComponent::setBusy(bool busy)
{
    m_usernameEdit->setDisabled(busy);
    m_passwordEdit->setDisabled(busy);
    m_confirmPasswordEdit->setDisabled(busy);
    m_registerButton->setDisabled(busy);
}

void RegistrationFormComponent::submit()
{
    const QString username = m_usernameEdit->text().trimmed();
    const QString password = m_passwordEdit->text();
    const QString confirmPassword = m_confirmPasswordEdit->text();

    if (username.isEmpty() || password.isEmpty())
    {
        m_helperLabel->setText("Uzupelnij nazwe uzytkownika i haslo.");
        m_helperLabel->show();
        return;
    }

    if (password != confirmPassword)
    {
        m_helperLabel->setText("Hasla nie sa takie same.");
        m_helperLabel->show();
        return;
    }

    m_helperLabel->hide();
    emit registrationRequested(username, password);
}
