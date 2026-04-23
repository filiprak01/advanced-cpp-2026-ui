#pragma once

#include <QWidget>

class QLabel;
class QLineEdit;
class QPushButton;

/**
 * @brief Formularz logowania klienta.
 *
 * Komponent zbiera dane uwierzytelniajace i emituje sygnal po poprawnym
 * wypelnieniu przez uzytkownika.
 */
class LoginFormComponent : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Tworzy formularz logowania.
     * @param parent Widget rodzic.
     */
    explicit LoginFormComponent(QWidget *parent = nullptr);
    ~LoginFormComponent() override;

signals:
    void loginRequested(const QString &username, const QString &password);

public slots:
    /**
     * @brief Blokuje lub odblokowuje pola i przycisk formularza.
     * @param busy Flaga zajetosci.
     */
    void setBusy(bool busy);

private:
    /**
     * @brief Waliduje pola i emituje sygnal logowania.
     */
    void submit();

    QLineEdit *m_usernameEdit;
    QLineEdit *m_passwordEdit;
    QPushButton *m_loginButton;
    QLabel *m_helperLabel;
};
