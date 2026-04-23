#pragma once
#include <QObject>

/**
 * @brief Parsuje surowy JSON z WebSocket i emituje typowane sygnały.
 *
 * Odczytuje pole "type" z JSON i emituje odpowiedni sygnał,
 * np. channelListReceived(), messageReceived(), historyReceived().
 */
class EventMapper : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Konstruktor.
     * @param parent Rodzic QObject/QWidget.
     */
    explicit EventMapper(QObject *parent = nullptr);
    ~EventMapper() override;
};
