#pragma once

#include <QList>
#include <QString>
#include <QStringList>

/**
 * @brief Lekki model kanalu wykorzystywany przez warstwe UI.
 *
 * Struktura przechowuje dane przygotowane do prezentacji w interfejsie
 * uzytkownika. Czesciowo odzwierciedla model backendowy, ale zawiera tez pola
 * pomocnicze wyliczane lub konsumowane bezposrednio przez widoki Qt.
 */
struct ChannelData
{
    int id{};                 ///< Identyfikator kanalu.
    QString name;             ///< Nazwa widoczna w interfejsie.
    QString topic;            ///< Krotki opis lub temat kanalu.
    int memberCount{};        ///< Liczba aktywnych lub przypisanych czlonkow.
    int maxUsers{32};         ///< Limit uzytkownikow prezentowany w UI.
    bool isPrivate{false};    ///< Flaga okreslajaca kanal prywatny.
    bool isActive{true};      ///< Flaga aktywnosci kanalu.
    QStringList usernames;    ///< Lista nazw uzytkownikow przypisanych do kanalu.
    QList<int> messageIds;    ///< Lista identyfikatorow wiadomosci w kanale.
};
