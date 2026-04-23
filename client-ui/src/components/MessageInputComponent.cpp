#include "MessageInputComponent.h"

#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>

MessageInputComponent::MessageInputComponent(QWidget *parent)
    : QWidget(parent)
    , m_input(new QLineEdit(this))
    , m_sendButton(new QPushButton("Wyslij", this))
{
    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(12);

    m_input->setPlaceholderText("wpisz wiadomosc...");
    m_input->setClearButtonEnabled(true);

    layout->addWidget(m_input, 1);
    layout->addWidget(m_sendButton);

    connect(m_sendButton, &QPushButton::clicked, this, &MessageInputComponent::submit);
    connect(m_input, &QLineEdit::returnPressed, this, &MessageInputComponent::submit);
}

MessageInputComponent::~MessageInputComponent() = default;

void MessageInputComponent::setPlaceholderText(const QString &text)
{
    m_input->setPlaceholderText(text);
}

void MessageInputComponent::setBusy(bool busy)
{
    m_input->setDisabled(busy);
    m_sendButton->setDisabled(busy);
}

void MessageInputComponent::submit()
{
    const QString text = m_input->text().trimmed();
    if (text.isEmpty())
        return;

    emit messageSent(text);
    m_input->clear();
}
