#include "LoadingComponent.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QProgressBar>

LoadingComponent::LoadingComponent(QWidget *parent)
    : QWidget(parent)
    , m_label(new QLabel("Laczenie z serwerem...", this))
    , m_progressBar(new QProgressBar(this))
{
    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(12);

    m_progressBar->setRange(0, 0);
    m_progressBar->setTextVisible(false);
    m_progressBar->setFixedWidth(120);

    layout->addWidget(m_progressBar);
    layout->addWidget(m_label);
    layout->addStretch();

    hide();
}

LoadingComponent::~LoadingComponent() = default;

void LoadingComponent::setMessage(const QString &message)
{
    m_label->setText(message);
}
