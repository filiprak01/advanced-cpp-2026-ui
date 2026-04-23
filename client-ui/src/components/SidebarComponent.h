#pragma once

#include <QList>
#include <QWidget>

#include "models/ChannelData.h"

class QLabel;
class QScrollArea;
class QVBoxLayout;

/**
 * @brief Panel boczny prezentujacy liste kanalow.
 *
 * SidebarComponent odbudowuje liste elementow kanalow na podstawie danych
 * przekazanych z zewnatrz i emituje sygnal po wyborze jednego z nich.
 */
class SidebarComponent : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Tworzy panel boczny.
     * @param parent Widget rodzic.
     */
    explicit SidebarComponent(QWidget *parent = nullptr);
    ~SidebarComponent() override;

signals:
    void channelSelected(int channelId);

public slots:
    /**
     * @brief Ustawia liste kanalow prezentowanych w sidebarze.
     * @param channels Dane kanalow do wyswietlenia.
     */
    void setChannels(const QList<ChannelData> &channels);

    /**
     * @brief Oznacza aktywny kanal w sidebarze.
     * @param channelId Identyfikator aktywnego kanalu.
     */
    void setCurrentChannel(int channelId);

private:
    /**
     * @brief Odbudowuje zawartosc listy kanalow.
     */
    void rebuildList();

    QList<ChannelData> m_channels;
    QLabel *m_countLabel;
    QScrollArea *m_scrollArea;
    QWidget *m_listContainer;
    QVBoxLayout *m_listLayout;
    int m_currentChannelId{-1};
};
