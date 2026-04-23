#pragma once

#include <QDateTime>
#include <QString>

/**
 * @brief Model wiadomosci przygotowany pod wyswietlanie w kliencie UI.
 *
 * Struktura przechowuje dane niezbedne do wyrenderowania jednej wiadomosci
 * oraz kilka flag pomocniczych istotnych dla logiki widoku.
 */
struct MessageData
{
    int id{};                     ///< Identyfikator wiadomosci.
    int channelId{};              ///< Identyfikator kanalu, do ktorego nalezy wiadomosc.
    QString senderName;           ///< Nazwa nadawcy widoczna w UI.
    QString content;              ///< Tresc wiadomosci.
    QDateTime timestamp;          ///< Znacznik czasu wyslania lub odebrania.
    bool isOwnMessage{false};     ///< Flaga okreslajaca, czy wiadomosc nalezy do biezacego uzytkownika.
    bool isEdited{false};         ///< Flaga informujaca, czy wiadomosc byla edytowana.
};
