#pragma once
#include <QObject>

/**
 * @brief Niskopoziomowa warstwa komunikacji WebSocket.
 *
 * Opakowuje QWebSocket i udostępnia sygnały connected(), disconnected(),
 * textReceived() oraz errorOccurred(). Wszystkie operacje są asynchroniczne.
 */
class WebSocketService : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Konstruktor.
     * @param parent Rodzic QObject/QWidget.
     */
    explicit WebSocketService(QObject *parent = nullptr);
    ~WebSocketService() override;
};
