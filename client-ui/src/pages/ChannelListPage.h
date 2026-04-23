#pragma once

#include <QList>
#include <QStringList>
#include <QWidget>

#include "models/ChannelData.h"

class HeaderBarComponent;
class QLabel;
class QLineEdit;
class QPushButton;
class SidebarComponent;

/**
 * @brief Strona wyswietlajaca liste dostepnych kanalow.
 *
 * Strona laczy naglowek z panelem bocznym i prostym obszarem informacyjnym.
 * Jej zadaniem jest prezentacja listy kanalow i przekazanie wyboru dalej do
 * kontrolera lub routera.
 */
class ChannelListPage : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Tworzy strone listy kanalow.
     * @param parent Widget rodzic.
     */
    explicit ChannelListPage(QWidget *parent = nullptr);
    ~ChannelListPage() override;

signals:
    void channelSelected(int channelId);
    void createChannelRequested(const QString &name, const QStringList &usernames);
    void logoutRequested();

public slots:
    /**
     * @brief Ustawia liste kanalow pokazywana w sidebarze.
     * @param channels Lista kanalow do wyswietlenia.
     */
    void setChannels(const QList<ChannelData> &channels);

    /**
     * @brief Aktualizuje nazwe biezacego uzytkownika w warstwie opisu.
     * @param username Nazwa aktualnie zalogowanego uzytkownika.
     */
    void setCurrentUser(const QString &username);
    void setAvailableUsers(const QStringList &usernames);

private:
    HeaderBarComponent *m_headerBar;
    SidebarComponent *m_sidebar;
    QLabel *m_summaryLabel;
    QLabel *m_focusLabel;
    QLineEdit *m_channelNameInput;
    QLineEdit *m_channelUsersInput;
    QPushButton *m_createChannelButton;
};
