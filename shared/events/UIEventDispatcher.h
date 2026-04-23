#pragma once
#include <QObject>

/**
 * @brief Dystrybutor zdarzeń z backendu do odpowiednich serwisów i stanu.
 *
 * Działa jako mediator: odbiera zdarzenia z EventMapper
 * i przekazuje do ChannelState, MessageState itd.
 */
class UIEventDispatcher : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Konstruktor.
     * @param parent Rodzic QObject/QWidget.
     */
    explicit UIEventDispatcher(QObject *parent = nullptr);
    ~UIEventDispatcher() override;
};
