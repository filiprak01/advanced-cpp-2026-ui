#pragma once

#include <QList>
#include <QObject>
#include <QStringList>

#include "services/ConnectionService.h"
#include "state/ChannelState.h"

/**
 * @brief Obsługuje operacje na kanałach po stronie UI.
 *
 * Aktualizuje ChannelState w odpowiedzi na zdarzenia synchronizacji
 * i zmian kanałów z backendu.
 */
class ChannelController : public QObject
{
    Q_OBJECT
   public:
    /**
     * @brief Konstruktor.
     * @param connectionService Serwis udostepniajacy ClientBackend.
     * @param parent Rodzic QObject/QWidget.
     */
    explicit ChannelController(ConnectionService* connectionService, QObject* parent = nullptr);
    ~ChannelController() override;

    ChannelState* state();
    const ChannelState* state() const;

   signals:
    void channelError(const QString &message);

   public slots:
    void selectChannel(int channelId);
    void createChannel(const QString &name, const QStringList &usernames);
    void deleteChannel(int channelId);

   private slots:
    void handleBackendResponse(const QString &message);

   private:
    /**
     * @brief Buduje model UI na podstawie obiektu kanalu z odpowiedzi serwera.
     *
     * Metoda zachowuje flage prywatnosci kanalu, poniewaz widoki uzywaja jej
     * do pokazania etykiety "Prywatny" albo "Otwarty".
     */
    ChannelData channelFromJson(const json &channel) const;
    QList<ChannelData> channelsFromJson(const json &channels) const;
    QStringList usernamesFromJson(const json &users) const;
    void updateAvailableUsernamesFromPayload(const json &payload);
    void upsertChannel(const ChannelData &channel);
    void removeChannel(int channelId);

    ConnectionService* m_connectionService{nullptr};
    ChannelState m_channelState;
};
