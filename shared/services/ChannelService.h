#pragma once
#include <QObject>

/**
 * @brief Operacje na kanałach: pobieranie listy, dołączanie, opuszczanie.
 *
 * Wysyła żądania JSON przez WebSocketService i odpowiedzi
 * są routowane z powrotem przez EventMapper.
 */
class ChannelService : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Konstruktor.
     * @param parent Rodzic QObject/QWidget.
     */
    explicit ChannelService(QObject *parent = nullptr);
    ~ChannelService() override;
};
