#include "UserListComponent.h"

#include <QLabel>
#include <QListWidget>
#include <QVBoxLayout>

UserListComponent::UserListComponent(QWidget *parent)
    : QWidget(parent)
    , m_countLabel(new QLabel("Uczestnicy: 0", this))
    , m_listWidget(new QListWidget(this))
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(18, 18, 18, 18);
    layout->setSpacing(12);

    auto *titleLabel = new QLabel("Uczestnicy", this);
    titleLabel->setObjectName("sectionTitle");
    m_countLabel->setObjectName("sectionBody");

    layout->addWidget(titleLabel);
    layout->addWidget(m_countLabel);
    layout->addWidget(m_listWidget, 1);

    setObjectName("surfaceCard");
}

UserListComponent::~UserListComponent() = default;

void UserListComponent::setUsers(const QList<UserData> &users)
{
    m_listWidget->clear();
    m_countLabel->setText(QString("Uczestnicy: %1").arg(users.size()));

    for (const UserData &user : users)
    {
        QString role = user.role.isEmpty() ? (user.isAdmin ? "admin" : "member") : user.role;
        if (role == QStringLiteral("admin")) {
            role = QStringLiteral("administrator");
        } else if (role == QStringLiteral("member")) {
            role = QStringLiteral("uczestnik");
        }
        const QString status = user.isOnline ? "online" : "offline";
        m_listWidget->addItem(QString("%1 | %2 | %3").arg(user.username, role, status));
    }
}
