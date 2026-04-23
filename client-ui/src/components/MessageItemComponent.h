#pragma once

#include <QWidget>

#include "models/MessageData.h"

class QFrame;
class QLabel;

/**
 * @brief Pojedynczy widget reprezentujacy jedna wiadomosc czatu.
 *
 * Komponent odpowiada za prezentacje nadawcy, czasu i tresci wiadomosci.
 */
class MessageItemComponent : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Tworzy widok pojedynczej wiadomosci.
     * @param message Dane wiadomosci.
     * @param parent Widget rodzic.
     */
    explicit MessageItemComponent(const MessageData &message, QWidget *parent = nullptr);
    ~MessageItemComponent() override;

public slots:
    /**
     * @brief Podmienia dane wiadomosci i odswieza widok.
     * @param message Nowe dane wiadomosci.
     */
    void setMessage(const MessageData &message);

private:
    /**
     * @brief Odswieza zawartosc etykiet i styl komponentu.
     */
    void refresh();

    MessageData m_message;
    QFrame *m_bubbleFrame;
    QLabel *m_metaLabel;
    QLabel *m_contentLabel;
};
