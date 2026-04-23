#pragma once

#include <QWidget>

class QLabel;
class QPushButton;

/**
 * @brief Komponent prezentujacy komunikaty bledow w warstwie UI.
 *
 * Klasa pozwala wyswietlic wiadomosc bledu i ukryc ja po akcji uzytkownika lub
 * po zmianie stanu strony.
 */
class ErrorComponent : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Tworzy komponent bledu.
     * @param parent Widget rodzic.
     */
    explicit ErrorComponent(QWidget *parent = nullptr);
    ~ErrorComponent() override;

signals:
    void dismissed();

public slots:
    /**
     * @brief Wyswietla nowy komunikat bledu.
     * @param message Tresc bledu.
     */
    void showError(const QString &message);

    /**
     * @brief Usuwa biezacy komunikat bledu.
     */
    void clearError();

private:
    QLabel *m_messageLabel;
    QPushButton *m_dismissButton;
};
