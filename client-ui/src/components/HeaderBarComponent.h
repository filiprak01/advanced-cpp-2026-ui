#pragma once

#include <QWidget>

class QLabel;
class QPushButton;

/**
 * @brief Gorny pasek nawigacyjny wspoldzielony przez strony klienta.
 *
 * Komponent pokazuje tytul, kontekst aktualnego ekranu oraz przyciski powrotu
 * i wylogowania. Jest wielokrotnego uzytku na roznych ekranach aplikacji.
 */
class HeaderBarComponent : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Tworzy pasek naglowka.
     * @param parent Widget rodzic.
     */
    explicit HeaderBarComponent(QWidget *parent = nullptr);
    ~HeaderBarComponent() override;

signals:
    void backRequested();
    void logoutRequested();

public slots:
    /**
     * @brief Ustawia glowny tytul paska.
     * @param title Tytul ekranu.
     */
    void setTitle(const QString &title);

    /**
     * @brief Ustawia dodatkowy opis kontekstu.
     * @param context Tekst pomocniczy pod tytulem.
     */
    void setContext(const QString &context);

    /**
     * @brief Pokazuje lub ukrywa przycisk powrotu.
     * @param visible Flaga widocznosci przycisku.
     */
    void setBackVisible(bool visible);

    /**
     * @brief Zmienia etykiete przycisku wylogowania.
     * @param text Nowy napis przycisku.
     */
    void setLogoutText(const QString &text);

private:
    QLabel *m_titleLabel;
    QLabel *m_contextLabel;
    QPushButton *m_backButton;
    QPushButton *m_logoutButton;
};
