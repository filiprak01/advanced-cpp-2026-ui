#pragma once

#include <QList>
#include <QWidget>

#include "models/MessageData.h"

class QLabel;
class QScrollArea;
class QVBoxLayout;

/**
 * @brief Komponent renderujacy liste wiadomosci dla aktywnego kanalu.
 *
 * Klasa przechowuje aktualna liste danych wiadomosci i tworzy dla nich
 * odpowiednie komponenty wizualne w przewijanym kontenerze.
 */
class MessageListComponent : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Tworzy komponent listy wiadomosci.
     * @param parent Widget rodzic.
     */
    explicit MessageListComponent(QWidget *parent = nullptr);
    ~MessageListComponent() override;

public slots:
    /**
     * @brief Podmienia cala liste wiadomosci.
     * @param messages Nowa kolekcja wiadomosci.
     */
    void setMessages(const QList<MessageData> &messages);

    /**
     * @brief Dopisuje pojedyncza wiadomosc na koniec listy.
     * @param message Wiadomosc do dopisania.
     */
    void appendMessage(const MessageData &message);

    /**
     * @brief Usuwa wszystkie aktualnie wyswietlane wiadomosci.
     */
    void clearMessages();

private:
    /**
     * @brief Buduje od nowa zawartosc listy na podstawie `m_messages`.
     */
    void rebuildMessages();

    /**
     * @brief Przewija widok do ostatniej wiadomosci.
     */
    void scrollToBottom();

    QList<MessageData> m_messages;
    QScrollArea *m_scrollArea;
    QWidget *m_container;
    QVBoxLayout *m_messageLayout;
    QLabel *m_emptyLabel;
};
