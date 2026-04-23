#pragma once

#include <QWidget>

class QLabel;
class QTimer;

/**
 * @brief Prosty komponent powiadomien tymczasowych.
 *
 * NotificationComponent wyswietla krotkie komunikaty informacyjne lub bledy
 * nad glowna zawartoscia okna i samodzielnie ukrywa je po czasie.
 */
class NotificationComponent : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Tworzy komponent powiadomien.
     * @param parent Widget rodzic.
     */
    explicit NotificationComponent(QWidget *parent = nullptr);
    ~NotificationComponent() override;

public slots:
    /**
     * @brief Pokazuje komunikat informacyjny.
     * @param message Tresc komunikatu.
     */
    void showInfo(const QString &message);

    /**
     * @brief Pokazuje komunikat bledu.
     * @param message Tresc komunikatu.
     */
    void showError(const QString &message);

private:
    /**
     * @brief Wyswietla komunikat z zadana kolorystyka.
     * @param message Tresc komunikatu.
     * @param background Kolor tla.
     * @param border Kolor obramowania.
     */
    void showMessage(const QString &message, const QString &background, const QString &border);

    /**
     * @brief Ustawia polozenie komponentu wzgledem rodzica.
     */
    void reposition();

    QLabel *m_messageLabel;
    QTimer *m_hideTimer;
};
