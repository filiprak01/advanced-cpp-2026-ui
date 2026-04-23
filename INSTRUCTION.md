# PWChat Client UI - instrukcja uruchomienia

Wersja: `1.0.0 MVP`

Ten projekt jest graficznym klientem Qt. Do dzialania potrzebuje uruchomionego serwera z repozytorium `advanced-cpp-2026`.

W tej instrukcji slowo "rozwiazanie" oznacza katalog build wygenerowany przez CMake, np. `build/presets/client-debug` albo `build/ui-tests`.

## 1. Wymagany uklad katalogow

Repozytoria powinny byc obok siebie:

```text
/home/filip/cpp/advanced-cpp-2026
/home/filip/cpp/advanced-cpp-2026-ui
```

UI linkuje backend klienta z repozytorium backendowego przez CMake, wiec taka struktura jest najprostsza.

## 2. Instalacja zaleznosci

```bash
cd /home/filip/cpp/advanced-cpp-2026-ui
chmod +x precondition.sh
./precondition.sh
```

Skrypt instaluje Qt6, CMake, kompilator, Doxygen, GoogleTest i biblioteki potrzebne do linkowania z backendem klienta.

## 3. Najpierw uruchom backend

W pierwszym terminalu:

```bash
cd /home/filip/cpp/advanced-cpp-2026
cmake --preset server-only
cmake --build --preset server-only
./build/presets/server-only/server/PWChat
```

Serwer powinien dzialac na:

```text
0.0.0.0:8091
```

## 4. Tworzenie rozwiazania CMake dla UI klienta

Konfiguracja:

```bash
cd /home/filip/cpp/advanced-cpp-2026-ui
cmake --preset client-debug
```

Budowanie:

```bash
cmake --build --preset client-debug
```

Plik wykonywalny klienta powstaje tutaj:

```bash
./build/presets/client-debug/client-ui/client-ui
```

## 5. Uruchomienie UI klienta w WSL

```bash
cd /home/filip/cpp/advanced-cpp-2026-ui
QT_QPA_PLATFORM=xcb ./build/presets/client-debug/client-ui/client-ui
```

`QT_QPA_PLATFORM=xcb` jest przydatne w WSL, gdy Qt probuje uzyc pluginu Wayland.

## 6. Tworzenie rozwiazania CMake dla testow UI

Ten projekt nie ma osobnego presetu testowego, dlatego testy UI konfiguruje sie jawnie przez `cmake -S . -B ...`.

Konfiguracja testow:

```bash
cd /home/filip/cpp/advanced-cpp-2026-ui
cmake -S . -B build/ui-tests -DCMAKE_BUILD_TYPE=Debug -DBUILD_UI_TESTS=ON -DBUILD_ADMIN_UI=OFF
```

Budowanie testow:

```bash
cmake --build build/ui-tests
```

Wypisanie wszystkich testow bez uruchamiania:

```bash
ctest --test-dir build/ui-tests -N
```

## 7. Uruchamianie wszystkich testow UI

```bash
cd /home/filip/cpp/advanced-cpp-2026-ui
ctest --test-dir build/ui-tests --output-on-failure --timeout 30
```

Oczekiwany stan po aktualnym MVP:

```text
30/30 tests passed
```

## 8. Uruchamianie testow unit wedlug modulow

Wszystkie obecne testy UI sa testami unit, wiec ta komenda uruchamia caly zestaw unit:

```bash
ctest --test-dir build/ui-tests --output-on-failure --timeout 30
```

Kontrolery UI:

```bash
ctest --test-dir build/ui-tests -R '^(ChannelControllerTest|MessageControllerTest)\.' --output-on-failure --timeout 30
```

Stan aplikacji:

```bash
ctest --test-dir build/ui-tests -R '^(ChannelStateTest|MessageStateTest|SessionStateTest)\.' --output-on-failure --timeout 30
```

Serwisy UI:

```bash
ctest --test-dir build/ui-tests -R '^AuthServiceTest\.' --output-on-failure --timeout 30
```

Pojedyncza grupa testow, przyklad dla stanu wiadomosci:

```bash
ctest --test-dir build/ui-tests -R '^MessageStateTest\.' --output-on-failure --timeout 30
```

## 9. Testy integracyjne UI

W aktualnym MVP UI nie ma osobnego zestawu testow integracyjnych. Komunikacja end-to-end jest weryfikowana glownie przez testy integracyjne backendu oraz reczne uruchomienie: serwer w jednym terminalu, UI w drugim.

Gdy dodasz testy integracyjne UI w przyszlosci, trzymaj je w `tests/integration` i uruchamiaj podobnym filtrem:

```bash
ctest --test-dir build/ui-tests -R 'Integration' --output-on-failure --timeout 30
```

## 10. Opcjonalny build z panelem administratora

Panel administratora jest rozszerzeniem poza MVP, ale target sie buduje.

Konfiguracja:

```bash
cd /home/filip/cpp/advanced-cpp-2026-ui
cmake --preset full-debug
```

Budowanie:

```bash
cmake --build --preset full-debug
```

Uruchomienie opcjonalnego panelu:

```bash
./build/presets/full-debug/admin-ui/admin-ui
```

## 11. Konfiguracja polaczenia

UI czyta konfiguracje z:

```text
config/client.json
```

Aktualny port:

```text
8091
```

Adres `serverIp` ma wartosc liczbowa `2130706433`, czyli `127.0.0.1`.

## 12. Dokumentacja Doxygen

```bash
cd /home/filip/cpp/advanced-cpp-2026-ui
./scripts/gen-docs.sh
```

Dokumentacja HTML powstaje tutaj:

```text
docs/html/index.html
```

Mozesz zobaczyc ostrzezenie Doxygena o jezyku `polish`. To ostrzezenie pochodzi z samego Doxygena i nie oznacza bledu w kodzie.

## 13. Najczestsze problemy

- Jesli UI pokazuje blad polaczenia, upewnij sie, ze serwer dziala na porcie `8091`.
- Jesli port backendu zmienisz w `advanced-cpp-2026/config/server.json`, zmien tez `advanced-cpp-2026-ui/config/client.json`.
- Jesli pojawi sie blad pluginu Wayland, uruchom UI z `QT_QPA_PLATFORM=xcb`.
- Jesli CMake nie widzi backendu, sprawdz, czy repozytorium `advanced-cpp-2026` jest obok `advanced-cpp-2026-ui`.
- Jesli po zmianach CMake zachowuje sie dziwnie, usun tylko katalog `build/` i skonfiguruj ponownie.
