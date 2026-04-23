#pragma once

#include <QWidget>

class QLabel;
class QPushButton;

/**
 * @brief Przelacznik pomiedzy formularzem logowania i rejestracji.
 *
 * Komponent pokazuje krotki tekst pomocniczy oraz przycisk, ktory emituje
 * odpowiedni sygnal zmiany trybu formularza.
 */
class AuthToggleComponent : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Tworzy przelacznik formularzy auth.
     * @param parent Widget rodzic.
     */
    explicit AuthToggleComponent(QWidget *parent = nullptr);
    ~AuthToggleComponent() override;

signals:
    void toggleToRegister();
    void toggleToLogin();

public slots:
    /**
     * @brief Ustawia aktualny tryb formularza.
     * @param loginMode `true`, gdy aktywny jest formularz logowania.
     */
    void setLoginMode(bool loginMode);

private:
    QLabel *m_promptLabel;
    QPushButton *m_toggleButton;
    bool m_loginMode{true};
};
