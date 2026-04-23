#pragma once

#include <QList>
#include <QObject>
#include <QStringList>

#include "models/ChannelData.h"

/**
 * @brief Przechowuje listę kanałów i aktualnie wybrany kanał.
 *
 * Singleton. Emituje channelsUpdated() po zmianie listy
 * i currentChannelChanged() po wyborze kanału.
 */
class ChannelState : public QObject {
    Q_OBJECT
public:
    /**
     * @brief Konstruktor.
     * @param parent Rodzic QObject/QWidget.
     */
    explicit ChannelState(QObject *parent = nullptr);
    ~ChannelState() override;

    const QList<ChannelData> &channels() const;
    const QStringList &availableUsernames() const;
    int currentChannelId() const;
    ChannelData currentChannel() const;
    ChannelData channelById(int channelId) const;

public slots:
    void setChannels(const QList<ChannelData> &channels);
    void setAvailableUsernames(const QStringList &usernames);
    void setCurrentChannelId(int channelId);

signals:
    void channelsUpdated();
    void availableUsernamesUpdated();
    void currentChannelChanged(int channelId);

private:
    QList<ChannelData> m_channels;
    QStringList m_availableUsernames;
    int m_currentChannelId{-1};
};
