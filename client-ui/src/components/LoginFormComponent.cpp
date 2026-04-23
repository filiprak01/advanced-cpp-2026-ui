#include "LoginFormComponent.h"

#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

LoginFormComponent::LoginFormComponent(QWidget *parent)
    : QWidget(parent)
    , m_usernameEdit(new QLineEdit(this))
    , m_passwordEdit(new QLineEdit(this))
    , m_loginButton(new QPushButton("Zaloguj", this))
    , m_helperLabel(new QLabel(this))
{
    auto *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(12);

    auto *titleLabel = new QLabel("Logowanie", this);
    titleLabel->setObjectName("sectionTitle");

    auto *subtitleLabel = new QLabel(
        "Wpisz dane konta, aby przejsc do panelu kanalow.",
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

    formLayout->addRow("Uzytkownik", m_usernameEdit);
    formLayout->addRow("Haslo", m_passwordEdit);

    m_helperLabel->setObjectName("inlineHelper");
    m_helperLabel->hide();

    rootLayout->addWidget(titleLabel);
    rootLayout->addWidget(subtitleLabel);
    rootLayout->addLayout(formLayout);
    rootLayout->addWidget(m_helperLabel);
    rootLayout->addWidget(m_loginButton);

    connect(m_loginButton, &QPushButton::clicked, this, &LoginFormComponent::submit);
    connect(m_usernameEdit, &QLineEdit::returnPressed, this, &LoginFormComponent::submit);
    connect(m_passwordEdit, &QLineEdit::returnPressed, this, &LoginFormComponent::submit);
}

LoginFormComponent::~LoginFormComponent() = default;

void LoginFormComponent::setBusy(bool busy)
{
    m_usernameEdit->setDisabled(busy);
    m_passwordEdit->setDisabled(busy);
    m_loginButton->setDisabled(busy);
}

void LoginFormComponent::submit()
{
    const QString username = m_usernameEdit->text().trimmed();
    const QString password = m_passwordEdit->text();

    if (username.isEmpty() || password.isEmpty())
    {
        m_helperLabel->setText("Uzupelnij nazwe uzytkownika i haslo.");
        m_helperLabel->show();
        return;
    }

    m_helperLabel->hide();
    emit loginRequested(username, password);
}
