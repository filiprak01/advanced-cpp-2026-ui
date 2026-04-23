#pragma once
#include <QObject>

/**
 * @brief Wykonuje mutacje stanu w odpowiedzi na zdarzenia z EventMapper.
 *
 * Aktualizuje obiekty stanu (ChannelState, MessageState itp.)
 * na podstawie zdarzeń przekazanych przez UIEventDispatcher.
 */
class EventHandler : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Konstruktor.
     * @param parent Rodzic QObject/QWidget.
     */
    explicit EventHandler(QObject *parent = nullptr);
    ~EventHandler() override;
};
