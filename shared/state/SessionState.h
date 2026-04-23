#pragma once

#include <QObject>
#include <QString>

/**
 * @brief Przechowuje dane zalogowanego użytkownika.
 *
 * Zawiera nazwę użytkownika i flagę zalogowania.
 * Emituje changed() po zalogowaniu i wylogowaniu.
 */
class SessionState : public QObject {
    Q_OBJECT
public:
    /**
     * @brief Konstruktor.
     * @param parent Rodzic QObject/QWidget.
     */
    explicit SessionState(QObject *parent = nullptr);
    ~SessionState() override;

    const QString &username() const;
    bool isLoggedIn() const;

public slots:
    void login(const QString &username);
    void logout();
    void clear();

signals:
    void changed();

private:
    QString m_username;
    bool m_isLoggedIn{false};
};
