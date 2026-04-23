#pragma once

#include <QColor>
#include <QWidget>

class QLabel;

/**
 * @brief Niewielki komponent informujacy o stanie polaczenia.
 *
 * Komponent wyswietla krotki tekst oraz wskaznik kolorystyczny, dzieki czemu
 * uzytkownik widzi, czy aplikacja jest polaczona, rozlaczona lub w trakcie
 * wykonywania operacji.
 */
class ConnectionStatusComponent : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Tworzy komponent statusu polaczenia.
     * @param parent Widget rodzic.
     */
    explicit ConnectionStatusComponent(QWidget *parent = nullptr);
    ~ConnectionStatusComponent() override;

public slots:
    /**
     * @brief Aktualizuje tekst i kolor statusu.
     * @param text Tresc statusu.
     * @param color Kolor wskaznika.
     */
    void setStatus(const QString &text, const QColor &color);

private:
    QLabel *m_indicator;
    QLabel *m_statusLabel;
};
