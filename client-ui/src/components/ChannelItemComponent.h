#pragma once

#include <QWidget>

#include "models/ChannelData.h"

class QLabel;
class QMouseEvent;

/**
 * @brief Pojedynczy element listy kanalow w sidebarze.
 *
 * Komponent pokazuje nazwe kanalu, liczbe uczestnikow oraz typ rozmowy.
 * Etykieta typu ma stala szerokosc, dzieki czemu dluzsze nazwy kanalow
 * moga sie zawijac bez zaslaniania informacji "Prywatny" albo "Otwarty".
 */
class ChannelItemComponent : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Tworzy element reprezentujacy jeden kanal.
     * @param channel Dane kanalu do wyswietlenia.
     * @param parent Widget rodzic.
     */
    explicit ChannelItemComponent(const ChannelData &channel, QWidget *parent = nullptr);
    ~ChannelItemComponent() override;

signals:
    void channelSelected(int channelId);

public slots:
    /**
     * @brief Podmienia dane reprezentowanego kanalu.
     * @param channel Nowe dane kanalu.
     */
    void setChannel(const ChannelData &channel);

    /**
     * @brief Oznacza element jako aktywny lub nieaktywny.
     * @param selected Flaga aktywnosci wizualnej.
     */
    void setSelected(bool selected);

protected:
    /**
     * @brief Reaguje na klikniecie mysza i wybiera kanal.
     * @param event Zdarzenie klikniecia.
     */
    void mousePressEvent(QMouseEvent *event) override;

private:
    /**
     * @brief Odswieza tekst, etykiete typu kanalu i stan zaznaczenia.
     */
    void refresh();

    ChannelData m_channel;
    QLabel *m_nameLabel;
    QLabel *m_metaLabel;
    QLabel *m_badgeLabel;
    bool m_selected{false};
};
