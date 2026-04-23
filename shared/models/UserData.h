#pragma once

#include <QString>

/**
 * @brief Model uzytkownika przeznaczony do warstwy prezentacji.
 *
 * Struktura laczy informacje pochodzace z backendu z flagami potrzebnymi do
 * prezentacji listy uczestnikow i elementow administracyjnych.
 */
struct UserData
{
    int id{};                  ///< Identyfikator uzytkownika.
    QString username;          ///< Login lub nazwa wyswietlana.
    QString role;              ///< Rola prezentowana w UI.
    bool isAdmin{false};       ///< Flaga administratora.
    bool isOnline{false};      ///< Flaga obecnosci uzytkownika.
};
