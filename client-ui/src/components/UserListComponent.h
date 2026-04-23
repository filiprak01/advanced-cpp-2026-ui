#pragma once

#include <QList>
#include <QWidget>

#include "models/UserData.h"

class QLabel;
class QListWidget;

/**
 * @brief Komponent wyswietlajacy liste uczestnikow aktywnego kanalu.
 *
 * Lista ma charakter prezentacyjny i moze byc zasilana danymi pochodzacymi z
 * backendu lub z lokalnego stanu aplikacji.
 */
class UserListComponent : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Tworzy komponent listy uzytkownikow.
     * @param parent Widget rodzic.
     */
    explicit UserListComponent(QWidget *parent = nullptr);
    ~UserListComponent() override;

public slots:
    /**
     * @brief Podmienia cala liste uzytkownikow.
     * @param users Lista uzytkownikow przypisanych do widoku.
     */
    void setUsers(const QList<UserData> &users);

private:
    QLabel *m_countLabel;
    QListWidget *m_listWidget;
};
