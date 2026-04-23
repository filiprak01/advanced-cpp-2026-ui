#pragma once
#include <QObject>

/**
 * @brief Wspolny punkt rozbudowy dla globalnego stanu aplikacji.
 *
 * Klasa pozostaje lekka, poniewaz aktualny klient przechowuje stan w
 * wyspecjalizowanych klasach takich jak SessionState, ChannelState i
 * MessageState. AppState moze pozniej zebrac te obiekty w jednym miejscu.
 */
class AppState : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Konstruktor.
     * @param parent Rodzic QObject/QWidget.
     */
    explicit AppState(QObject *parent = nullptr);
    ~AppState() override;
};
