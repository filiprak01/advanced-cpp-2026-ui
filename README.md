# PWChat Client UI

Wersja: `1.0.0 MVP`

Repozytorium zawiera desktopowy interfejs użytkownika Qt dla projektu PWChat. UI korzysta z backendu klienta z repozytorium `advanced-cpp-2026`, a serwer pozostaje osobnym procesem uruchamianym w WSL/Linux.

## Zakres MVP

- Polski ekran logowania i rejestracji.
- Połączenie z serwerem `127.0.0.1:8091`.
- Automatyczne przejście do panelu kanałów po udanym logowaniu lub rejestracji.
- Lista kanałów i lista dostępnych użytkowników.
- Tworzenie kanałów z istniejącymi użytkownikami.
- Wejście do kanału i widok rozmowy.
- Wysyłanie i odbieranie wiadomości.
- Oznaczenie kanału jako `Otwarty` albo `Prywatny`.
- Proste komunikaty błędów i status połączenia.
- Minimalistyczny wygląd dopasowany do projektu studenckiego.

## Struktura

- `client-ui/` - właściwa aplikacja użytkownika.
- `client-ui/src/services/` - połączenie UI z backendem klienta.
- `client-ui/src/controllers/` - logika pośrednia między widokiem i usługami.
- `client-ui/src/pages/` - główne ekrany aplikacji.
- `client-ui/src/components/` - małe komponenty widoku.
- `client-ui/src/routing/` - proste przełączanie ekranów.
- `common/` - modele i stan współdzielony, który może zostać użyty też przez przyszły panel admina.
- `client-ui/src/services/` - usługi konkretnego klienta użytkownika, czyli część niewspółdzielona.
- `admin-ui/` - nie jest częścią bieżącego MVP; ewentualny panel admina zostaje rozszerzeniem.
- `tests/` - testy jednostkowe UI uruchamiane ręcznie.
- `config/` - konfiguracja połączenia z backendem.

## Wymagania

- Linux albo WSL z obsługą aplikacji graficznych.
- CMake `3.16+`.
- Kompilator C++20.
- Qt6: `Core`, `Widgets`, `Network`, `WebSockets`.
- Repozytorium backendu w katalogu obok: `../advanced-cpp-2026`.

Przykład instalacji na Ubuntu/WSL:

```bash
sudo apt update
sudo apt install g++ cmake qt6-base-dev qt6-websockets-dev doxygen graphviz
```

## Budowanie

Klient UI:

```bash
cmake --preset client-debug
cmake --build --preset client-debug
```

Opcjonalny preset rozszerzeniowy:

```bash
cmake --preset full-debug
cmake --build --preset full-debug
```

Jeśli katalog `admin-ui/` nie istnieje, preset zbuduje klienta i pokaże tylko ostrzeżenie CMake.

## Uruchomienie

Najpierw uruchom serwer z repozytorium backendu:

```bash
cd /home/filip/cpp/advanced-cpp-2026
./build/presets/server-only/server/PWChat
```

Potem uruchom UI:

```bash
cd /home/filip/cpp/advanced-cpp-2026-ui
QT_QPA_PLATFORM=xcb ./build/presets/client-debug/client-ui/client-ui
```

`QT_QPA_PLATFORM=xcb` pomaga w WSL, gdy Qt próbuje użyć niedostępnego pluginu Wayland.

## Konfiguracja

Plik `config/client.json` zawiera port `8091` i adres lokalny serwera. Wartość `serverIp` jest zapisana liczbowo jako `2130706433`, czyli `127.0.0.1`.

## Testy

Testy są ręczne i nie blokują normalnego developmentu:

```bash
cmake --preset client-debug
cmake --build --preset client-debug
ctest --test-dir build/ui-tests --output-on-failure
```

## Dokumentacja

```bash
./scripts/gen-docs.sh
```

Główne pliki opisowe:

- `OVERVIEW.md` - opis zaimplementowanych elementów.
- `SUMMARY.md` - opis plików UI.
- `DEADLINE_GUIDE.md` - plan i kolejność prac.
- `EXTENSION.md` - rozszerzenia po MVP.
- `../ALL_SUMMARY.md` - zbiorcze przygotowanie do obrony dla obu repozytoriów.

## Ograniczenia MVP

- Panel administratora jest rozszerzeniem, nie częścią MVP.
- UI nie obsługuje jeszcze wszystkich operacji backendu, np. pełnej edycji wiadomości i kanałów.
- Tworzenie kanału w UI jest uproszczone i można rozbudować je o wybór typu prywatny/otwarty.
- Natywne pakowanie aplikacji dla Windows/Linux jest planem po MVP.

## Najważniejsze punkty do obrony

- UI jest osobnym projektem Qt, ale korzysta z backendu klienta jako warstwy transportowej.
- Widoki emitują sygnały, kontrolery reagują, a stan aplikacji odświeża komponenty.
- Logika biznesowa pozostaje po stronie serwera.
- Minimalistyczny wygląd jest świadomym wyborem dla MVP i czytelnej prezentacji.
