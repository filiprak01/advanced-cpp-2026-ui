#pragma once

#include <QColor>
#include <QWidget>

class AuthToggleComponent;
class ConnectionStatusComponent;
class ErrorComponent;
class LoadingComponent;
class LoginFormComponent;
class RegistrationFormComponent;
class QStackedWidget;

/**
 * @brief Strona logowania i rejestracji klienta.
 *
 * Klasa sklada formularze autoryzacyjne, status polaczenia, komunikaty bledow
 * oraz wskaznik pracy. Strona nie wykonuje komunikacji sieciowej samodzielnie -
 * emituje jedynie intencje uzytkownika do kontrolera.
 */
class ConnectionPage : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Tworzy strone polaczenia i formularzy auth.
     * @param parent Widget rodzic.
     */
    explicit ConnectionPage(QWidget *parent = nullptr);
    ~ConnectionPage() override;

signals:
    void loginRequested(const QString &username, const QString &password);
    void registrationRequested(const QString &username, const QString &password);

public slots:
    /**
     * @brief Pokazuje komunikat bledu na stronie auth.
     * @param message Tresc bledu do wyswietlenia.
     */
    void showError(const QString &message);

    /**
     * @brief Czyści aktywny komunikat bledu.
     */
    void clearError();

    /**
     * @brief Przelacza strone w tryb zajety lub gotowy.
     * @param busy Flaga informujaca o trwajacej operacji.
     */
    void setBusy(bool busy);

    /**
     * @brief Ustawia tekst i kolor statusu polaczenia.
     * @param text Komunikat statusu.
     * @param color Kolor wskaznika statusu.
     */
    void setConnectionStatus(const QString &text, const QColor &color);

    /**
     * @brief Przelacza widok na formularz logowania.
     */
    void showLoginForm();

    /**
     * @brief Przelacza widok na formularz rejestracji.
     */
    void showRegistrationForm();

private:
    ConnectionStatusComponent *m_statusComponent;
    LoginFormComponent *m_loginForm;
    RegistrationFormComponent *m_registrationForm;
    AuthToggleComponent *m_authToggle;
    LoadingComponent *m_loadingComponent;
    ErrorComponent *m_errorComponent;
    QStackedWidget *m_formStack;
};
