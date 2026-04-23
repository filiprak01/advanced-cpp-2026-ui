#pragma once

#include <QWidget>

class QLabel;
class QLineEdit;
class QPushButton;

/**
 * @brief Formularz rejestracji nowego uzytkownika.
 *
 * Komponent sprawdza podstawowa zgodnosc danych i emituje sygnal utworzenia
 * konta po stronie kontrolera.
 */
class RegistrationFormComponent : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Tworzy formularz rejestracji.
     * @param parent Widget rodzic.
     */
    explicit RegistrationFormComponent(QWidget *parent = nullptr);
    ~RegistrationFormComponent() override;

signals:
    void registrationRequested(const QString &username, const QString &password);

public slots:
    /**
     * @brief Blokuje lub odblokowuje pola i przycisk formularza.
     * @param busy Flaga zajetosci.
     */
    void setBusy(bool busy);

private:
    /**
     * @brief Waliduje formularz i emituje sygnal rejestracji.
     */
    void submit();

    QLineEdit *m_usernameEdit;
    QLineEdit *m_passwordEdit;
    QLineEdit *m_confirmPasswordEdit;
    QPushButton *m_registerButton;
    QLabel *m_helperLabel;
};
