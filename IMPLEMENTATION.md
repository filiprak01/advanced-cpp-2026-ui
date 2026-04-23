# Implementation Guide

Ordered steps to build the two Qt6 applications from empty skeletons to a fully working chat client and admin panel.

**[MILESTONE N]** — stop here, run the app, verify it works before continuing.
**[OPTIONAL]** — described in [ADDITIONAL.md](ADDITIONAL.md); implement any time after the preceding milestone.

---

## Qt Primer — rzeczy specyficzne dla Qt, które napotkasz podczas implementacji

Ten rozdział wyjaśnia mechanizmy Qt używane w całym projekcie.
Wróć tu zawsze, gdy napotkasz nieznaną składnię.

---

### `Q_OBJECT` — makro wymagane w każdej klasie używającej sygnałów/slotów

```cpp
class Router : public QObject {
    Q_OBJECT   // ← MUSI być w pierwszej linii ciała klasy
    // ...
};
```

**Co robi:** instruuje MOC (Meta Object Compiler) — narzędzie Qt — aby wygenerował
dodatkowy kod C++ dla tej klasy. Ten kod dostarcza mechanizm sygnałów/slotów,
`qobject_cast<>`, introspekcję i zarządzanie powiązaniami.
**Ważne:** bez `Q_OBJECT` klasa skompiluje się, ale sygnały i sloty nie będą działać.
CMake z `CMAKE_AUTOMOC ON` uruchamia MOC automatycznie przed kompilacją.

---

### Sygnały (`signals:`) i sloty (`public slots:`)

```cpp
class LoginFormComponent : public QWidget {
    Q_OBJECT
signals:
    // Deklaracja sygnału — bez implementacji w .cpp, tylko nagłówek
    void loginRequested(const QString &username, const QString &password);

public slots:
    // Slot — zwykła metoda, którą można podłączyć do sygnału
    void onLoginFailed(const QString &reason);
};
```

**Sygnał:**
- Deklaruje się go w sekcji `signals:` — **nie** implementujesz go w .cpp.
- Qt MOC generuje implementację automatycznie.
- Emitujesz go przez `emit loginRequested(user, pass);` — słowo kluczowe `emit`
  to makro Qt (rozwijane do niczego), służy wyłącznie czytelności kodu.

**Slot:**
- To zwykła metoda C++, dodatkowo oznaczona jako slot przez `public slots:`.
- Może być wywołany bezpośrednio jak każda inna metoda.
- Może też zostać podłączony do sygnału.

---

### `connect()` — łączenie sygnałów ze slotami

```cpp
// Składnia wskaźnikowo-typowa (Qt5/Qt6 — preferowana, sprawdzana w czasie kompilacji)
connect(źródło, &KlasaŹródło::nazwaSignału,
        cel,    &KlasaCelu::nazwaSlotu);

// Przykłady z projektu:
connect(m_loginForm, &LoginFormComponent::loginRequested,
        this,        &ConnectionPage::loginRequested);   // przekazanie sygnału dalej

connect(authController, &AuthController::loginSucceeded,
        [&]{ router->navigateTo("/channels"); });        // lambda jako slot
```

**Tryby połączenia (opcjonalny 5. argument):**
- `Qt::AutoConnection` (domyślny) — blokujące w tym samym wątku, kolejkowane między wątkami.
- `Qt::QueuedConnection` — zawsze kolejkowane; przydatne między wątkami.

---

### Zarządzanie pamięcią — hierarchia rodzic/dziecko

```cpp
auto *layout = new QVBoxLayout(this);   // 'this' = rodzic
auto *label  = new QLabel("Tekst", this);
layout->addWidget(label);
```

**Zasada:** gdy przekazujesz `parent` do konstruktora `QObject`/`QWidget`,
rodzic przejmuje własność. Kiedy rodzic zostanie zniszczony, automatycznie
niszczy wszystkie swoje dzieci — **nie wywołuj `delete` ręcznie na dzieciach**.
Wyjątek: widgety dodane do layoutu są automatycznie reparentowane do kontenera.

---

### Typy danych Qt używane w projekcie

| Typ Qt | Odpowiednik C++ / opis |
|--------|----------------------|
| `QString` | `std::string`, ale Unicode (UTF-16); używaj go wszędzie zamiast `std::string` w Qt API |
| `QList<T>` | `std::vector<T>` — sekwencja z szybkim dostępem losowym |
| `QHash<K,V>` | `std::unordered_map<K,V>` |
| `QUrl` | Strukturalny typ URL; tworzony przez `QUrl("ws://localhost:8080")` |
| `quint16` | `uint16_t` — liczba całkowita bez znaku 16-bit (używana dla numerów portów) |
| `QJsonDocument` | Parsuje i serializuje JSON; `QJsonDocument::fromJson(bytes)` |
| `QJsonObject` | Mapa klucz-wartość JSON; dostęp przez `obj["type"].toString()` |

---

### Layouty — rozmieszczanie widgetów

```cpp
// QVBoxLayout — elementy w pionie
auto *vbox = new QVBoxLayout(this);
vbox->addWidget(m_label);
vbox->addWidget(m_button);
vbox->addStretch();   // ← wypycha elementy ku górze (elastyczny odstęp)

// QHBoxLayout — elementy w poziomie
auto *hbox = new QHBoxLayout;
hbox->addWidget(m_input);
hbox->addWidget(m_sendButton);

// QFormLayout — dwie kolumny (etykieta | pole)
auto *form = new QFormLayout(this);
form->addRow("Nazwa użytkownika:", m_usernameEdit);
form->addRow("Hasło:", m_passwordEdit);
```

**Zasada:** zawsze przekazuj `this` jako rodzica top-level layoutu lub wywołaj
`setLayout(layout)`. Nie rób tego dla zagnieżdżonych layoutów (Qt robi to sam).

---

### `QStackedWidget` — wiele widoków w jednym miejscu

```cpp
auto *stack = new QStackedWidget(this);
stack->addWidget(loginPage);        // indeks 0
stack->addWidget(channelListPage);  // indeks 1

stack->setCurrentWidget(loginPage);       // pokaż po nazwie
stack->setCurrentIndex(1);               // lub po indeksie
```

Używany w `Router` (przełączanie stron) i wewnątrz `ConnectionPage`
(przełączanie między formularzem logowania a rejestracją).

---

### `QWebSocket` — komunikacja z backendem

```cpp
#include <QtWebSockets/QWebSocket>

QWebSocket m_socket;
m_socket.open(QUrl("ws://localhost:8080"));

// wysyłanie
m_socket.sendTextMessage(jsonString);

// odbieranie — przez sygnał:
connect(&m_socket, &QWebSocket::textMessageReceived,
        this, &WebSocketService::onTextReceived);
```

`QWebSocket` jest **asynchroniczny** — `open()` zwraca natychmiast.
Używaj sygnałów `connected()`, `disconnected()`, `errorOccurred()` do
reagowania na zmiany stanu.

---

### Singleton w Qt

```cpp
// SessionState.h
class SessionState : public QObject {
    Q_OBJECT
public:
    static SessionState &instance() {
        static SessionState s;   // inicjalizowany przy pierwszym wywołaniu
        return s;
    }
private:
    explicit SessionState(QObject *parent = nullptr);
};

// użycie
SessionState::instance().setSession(username, token, isAdmin);
```

Wzorzec używany przez `AppState`, `ConnectionState`, `SessionState`, `ChannelState`, `MessageState`.

---

### Komentarze Doxygen — jak dokumentować klasy w tym projekcie

```cpp
/**
 * @brief Zarządza nawigacją między stronami aplikacji.
 *
 * Router rejestruje widgety pod nazwami tras i przełącza między nimi
 * przez QStackedWidget. Emituje routeChanged() po każdej zmianie.
 *
 * @see RouteGuard
 */
class Router : public QObject {
    Q_OBJECT
public:
    /**
     * @brief Konstruktor.
     * @param stack  Stos widgetów zarządzany przez router.
     * @param parent Rodzic QObject.
     */
    explicit Router(QStackedWidget *stack, QObject *parent = nullptr);

    /**
     * @brief Rejestruje trasę pod podaną ścieżką.
     * @param route Ścieżka trasy, np. "/connection".
     * @param page  Widżet odpowiadający tej trasie.
     */
    void registerRoute(const QString &route, QWidget *page);
};
```

Wygeneruj dokumentację przez: `./scripts/gen-docs.sh`
Wynikowe pliki HTML znajdziesz w `docs/html/index.html`.

---

## Phase 0 — Environment & Build System

### Step 0.1 — Install dependencies

Follow the **Prerequisites** table in [README.md](README.md).

```bash
# Ubuntu/Debian quick install
sudo apt install cmake ninja-build \
    qt6-base-dev qt6-websockets-dev libqt6websockets6-dev \
    clang clang-format clang-tidy \
    python3 python3-pip
pip install pre-commit
```

### Step 0.2 — Initialize git and install pre-commit hooks

```bash
git init
git add .
pre-commit install        # installs .git/hooks/pre-commit
```

### Step 0.3 — Verify the skeleton builds

```bash
cmake -S . -B build/debug -DCMAKE_BUILD_TYPE=Debug -G Ninja
cmake --build build/debug
```

Expected: both `client-ui` and `admin-ui` binaries compile without warnings.

> ### ✅ [MILESTONE 0] — Both apps build and open an empty `QMainWindow`
> ```bash
> ./build/debug/client-ui/client-ui   # empty window titled "Chat Client"
> ./build/debug/admin-ui/admin-ui     # empty window titled "Chat Admin Panel"
> ```

---

## Phase 1 — Data Models (define before any service)

### Step 1.1 — Review `shared/models/`

The three model headers are already created:
- `shared/models/ChannelData.h`
- `shared/models/MessageData.h`
- `shared/models/UserData.h`

Extend the structs if your backend protocol uses additional fields (e.g., `createdAt`, `avatarUrl`).
No `.cpp` needed — plain structs.

---

## Phase 2 — Routing

Implement routing **before** pages so `main.cpp` can be wired properly.

### Step 2.1 — Implement `Router`

Files: `client-ui/src/routing/Router.h` / `.cpp`  *(repeat for `admin-ui`)*

> **Qt note — `QHash` i `QStringList`:**
> `QHash<QString, QWidget*>` to mapa tras → widgetów (jak `std::unordered_map`).
> `QStringList` to alias dla `QList<QString>` — używamy go tutaj jako stos historii.
> `QStackedWidget* m_stack` przechowujemy jako wskaźnik; Qt zarządza jego życiem
> (rodzic `QMainWindow` przejmuje własność).

Add to the header:
```cpp
#include <QHash>
#include <QStackedWidget>
#include <QStringList>

public:
    explicit Router(QStackedWidget *stack, QObject *parent = nullptr);

    void registerRoute(const QString &route, QWidget *page);
    void navigateTo(const QString &route);
    void goBack();

signals:
    void routeChanged(const QString &route);

private:
    QStackedWidget *m_stack;
    QHash<QString, QWidget *> m_routes;
    QStringList m_history;
```

Implement `navigateTo`: look up the widget in `m_routes`, call `m_stack->setCurrentWidget()`, push to `m_history`, emit `routeChanged`.
Implement `goBack`: pop the last entry from `m_history` and navigate to it.

### Step 2.2 — Implement `RouteGuard`

Files: `client-ui/src/routing/RouteGuard.h` / `.cpp`  *(repeat for `admin-ui`)*

> **Qt note — `std::function` jako typ składowej:**
> `QList<GuardFn>` to lista zamknięć/funktorów. `std::function<bool(const QString&)>`
> to standardowy C++ (nie Qt-specyficzny) — Qt dopuszcza jego użycie jako typ składowej.
> Lambdy przekazywane do `addGuard()` są wywoływane w `onRouteChanged`,
> który jest slotem podłączonym do `Router::routeChanged`.

```cpp
#include "Router.h"

public:
    using GuardFn = std::function<bool(const QString &route)>;
    explicit RouteGuard(Router *router, QObject *parent = nullptr);
    void addGuard(GuardFn fn);   // called in order; first false blocks navigation

private slots:
    void onRouteChanged(const QString &route);

private:
    Router *m_router;
    QList<GuardFn> m_guards;
    QString m_lastAllowedRoute;
```

Connect `Router::routeChanged` → `RouteGuard::onRouteChanged`.
In the slot, run all guards; if any returns `false`, call `m_router->navigateTo(m_lastAllowedRoute)`.

### Step 2.3 — Wire routing in `main.cpp` (client-ui)

Uncomment the routing block in `client-ui/main.cpp` and fill it in after Step 4.x when
`ConnectionPage` exists.

> *(No visible change yet — routing is infrastructure.)*

---

## Phase 3 — Connection Page & Auth UI (client-ui)

### Step 3.1 — Implement `LoadingComponent`

File: `client-ui/src/components/LoadingComponent.h` / `.cpp`

- Add a `QLabel *m_label` with text "Loading…" (or a `QProgressBar` in indeterminate mode).
- Public slot `setVisible(bool)` (or rely on `QWidget::setVisible`).

### Step 3.2 — Implement `ErrorComponent`

File: `client-ui/src/components/ErrorComponent.h` / `.cpp`

- `QLabel *m_message`, `QPushButton *m_dismissButton`.
- Public slot `showError(const QString &msg)`.
- Emit `dismissed()` when the button is clicked.

### Step 3.3 — Implement `ConnectionStatusComponent`

Files: `client-ui/src/components/ConnectionStatusComponent.h` / `.cpp`
*(repeat for `admin-ui`)*

- `QLabel *m_statusLabel` with a colored dot (`QPixmap` or CSS `border-radius`).
- Public slot `setStatus(const QString &text, const QColor &color)`.

### Step 3.4 — Implement `LoginFormComponent`

File: `client-ui/src/components/LoginFormComponent.h` / `.cpp`

```cpp
// signals
void loginRequested(const QString &username, const QString &password);

// private members
QLineEdit *m_usernameEdit;
QLineEdit *m_passwordEdit;
QPushButton *m_loginButton;
```

> **Qt note — `QLineEdit`, `QPushButton`, `QFormLayout`:**
> `QLineEdit` to jednoliniowe pole tekstowe; odczytuj wartość przez `m_usernameEdit->text()`.
> Ustaw tryb hasła: `m_passwordEdit->setEchoMode(QLineEdit::Password)`.
> `QPushButton` emituje sygnał `clicked()` po kliknięciu — podłącz go lambdą:
> ```cpp
> connect(m_loginButton, &QPushButton::clicked, this, [this] {
>     if (!m_usernameEdit->text().isEmpty() && !m_passwordEdit->text().isEmpty())
>         emit loginRequested(m_usernameEdit->text(), m_passwordEdit->text());
> });
> ```
> `QFormLayout` tworzy siatkę etykieta–pole; dodaj do konstruktora przez `setLayout(form)`.

Use `QFormLayout`. Connect button `clicked` → validate (non-empty) → emit `loginRequested`.

### Step 3.5 — Implement `RegistrationFormComponent`

File: `client-ui/src/components/RegistrationFormComponent.h` / `.cpp`

Same structure as `LoginFormComponent` but with:
- Extra `QLineEdit *m_confirmPasswordEdit`.
- Validate passwords match before emitting `registrationRequested(username, password)`.

### Step 3.6 — Implement `AuthToggleComponent`

File: `client-ui/src/components/AuthToggleComponent.h` / `.cpp`

- A single `QPushButton` whose text alternates between "Switch to Register" / "Switch to Login".
- Emit `toggleToLogin()` / `toggleToRegister()` alternately on click.

### Step 3.7 — Implement `ConnectionPage` (client-ui)

File: `client-ui/src/pages/ConnectionPage.h` / `.cpp`

Compose the components inside a `QVBoxLayout`:
```
[ConnectionStatusComponent]
[QStackedWidget]
  → index 0: LoginFormComponent
  → index 1: RegistrationFormComponent
[AuthToggleComponent]
[LoadingComponent]   ← hidden by default
[ErrorComponent]     ← hidden by default
```

Forward signals:
```cpp
signals:
    void loginRequested(const QString &username, const QString &password);
    void registrationRequested(const QString &username, const QString &password);
```

Handle `AuthToggleComponent` signals to switch the inner stack.

### Step 3.8 — Register `ConnectionPage` route in `main.cpp`

```cpp
auto *router = new Router(stack, &window);
auto *connectionPage = new ConnectionPage(stack);
router->registerRoute("/connection", connectionPage);
router->navigateTo("/connection");
```

> ### ✅ [MILESTONE 1] — ConnectionPage renders with login/register toggle
> Run `./build/debug/client-ui/client-ui`.
> You should see the connection status bar, a login form, and a toggle button.
> Toggling should switch to the registration form and back.

---

## Phase 4 — WebSocket & Auth Services

### Step 4.1 — Implement `WebSocketService`

File: `shared/services/WebSocketService.h` / `.cpp`

> **Qt note — `QWebSocket` jako wartość (nie wskaźnik):**
> `QWebSocket m_socket;` — przechowujemy jako wartość, nie wskaźnik.
> `QWebSocket` dziedziczy z `QObject`, ale wyjątkowo można go trzymać jako
> składową wartościową, jeśli właściciel (`WebSocketService`) jest tworzony na stercie.
> Sygnały QWebSocket (`connected`, `disconnected`, `textMessageReceived`,
> `errorOccurred`) są asynchroniczne — wywoływane przez pętlę zdarzeń Qt (`QApplication::exec()`).
> Bez uruchomionej pętli zdarzeń WebSocket nie może działać.

Add `#include <QtWebSockets/QWebSocket>` and:

```cpp
public:
    void connectToServer(const QUrl &url);
    void disconnectFromServer();
    void sendText(const QString &message);

signals:
    void connected();
    void disconnected();
    void textReceived(const QString &message);
    void errorOccurred(const QString &errorString);

private:
    QWebSocket m_socket;
```

In the constructor connect `m_socket` signals to your own signals.

### Step 4.2 — Implement `ConnectionService`

File: `shared/services/ConnectionService.h` / `.cpp`

Wraps `WebSocketService`. Holds `WebSocketService *m_ws`.

```cpp
public:
    void connect(const QString &host, quint16 port);
    void disconnect();

signals:
    void connectionEstablished();
    void connectionLost();
    void connectionFailed(const QString &reason);
```

Builds the `QUrl` and delegates to `WebSocketService`.

### Step 4.3 — Implement `ConnectionState`

File: `shared/state/ConnectionState.h` / `.cpp`

```cpp
public:
    static ConnectionState &instance();
    bool isConnected() const;
    void setConnected(bool connected);
    QString host() const;
    void setHost(const QString &host);

signals:
    void changed();

private:
    bool m_connected{false};
    QString m_host;
```

### Step 4.4 — Implement `ConnectionController` (client-ui)

File: `client-ui/src/controllers/ConnectionController.h` / `.cpp`

Holds `ConnectionService *m_service`. On `connectionEstablished` → update `ConnectionState` → emit signal to show auth as enabled. On `connectionFailed` → emit error string.

For now hard-code host/port (e.g., `localhost:8080`) — make it configurable later.

### Step 4.5 — Implement `AuthService`

File: `shared/services/AuthService.h` / `.cpp`

Holds `WebSocketService *m_ws` (injected). Sends JSON requests; parses JSON responses.

```cpp
public:
    void login(const QString &username, const QString &password);
    void registerUser(const QString &username, const QString &password);
    void logout();

signals:
    void loginSucceeded(const QString &token);
    void loginFailed(const QString &reason);
    void registrationSucceeded();
    void registrationFailed(const QString &reason);
```

Message format (adapt to your backend protocol):
```json
{"type": "auth/login", "username": "…", "password": "…"}
{"type": "auth/login_ok", "token": "…"}
{"type": "auth/login_fail", "reason": "…"}
```

### Step 4.6 — Implement `SessionState`

File: `shared/state/SessionState.h` / `.cpp`

```cpp
public:
    static SessionState &instance();
    bool isAuthenticated() const;
    bool isAdmin() const;
    QString username() const;
    QString token() const;
    void setSession(const QString &username, const QString &token, bool isAdmin);
    void clearSession();

signals:
    void sessionChanged();
```

### Step 4.7 — Implement `AuthController` (client-ui)

File: `client-ui/src/controllers/AuthController.h` / `.cpp`

```cpp
public slots:
    void onLoginRequested(const QString &username, const QString &password);
    void onRegistrationRequested(const QString &username, const QString &password);
    void onLogoutRequested();

signals:
    void loginSucceeded();
    void loginFailed(const QString &reason);
    void registrationSucceeded();
    void registrationFailed(const QString &reason);
```

On `AuthService::loginSucceeded` → `SessionState::setSession(...)` → emit `loginSucceeded`.

### Step 4.8 — Wire `ConnectionPage` to controllers in `main.cpp`

```cpp
// after creating router and connectionPage:
auto *authController = new AuthController(&window);
connect(connectionPage, &ConnectionPage::loginRequested,
        authController, &AuthController::onLoginRequested);
connect(authController, &AuthController::loginSucceeded,
        [&]{ router->navigateTo("/channels"); });
connect(authController, &AuthController::loginFailed,
        connectionPage, &ConnectionPage::showError);   // add this slot
```

> ### ✅ [MILESTONE 2] — Can connect to backend and log in
> Start your C++ backend, then run the client.
> Enter credentials → "Login" → if accepted, a route change is triggered.
> `ConnectionStatusComponent` should reflect the WebSocket state.

---

## Phase 5 — Event Pipeline

### Step 5.1 — Implement `EventMapper`

File: `shared/events/EventMapper.h` / `.cpp`

> **Qt note — parsowanie JSON:**
> Qt ma wbudowane wsparcie dla JSON w module `QtCore` — nie potrzebujesz zewnętrznej biblioteki.
> ```cpp
> #include <QJsonDocument>
> #include <QJsonObject>
> #include <QJsonArray>
>
> QJsonDocument doc = QJsonDocument::fromJson(text.toUtf8());
> QJsonObject   obj = doc.object();
> QString type = obj["type"].toString();
>
> // iteracja po tablicy
> for (const QJsonValue &v : obj["channels"].toArray()) {
>     QJsonObject ch = v.toObject();
>     ChannelData d;
>     d.id   = ch["id"].toInt();
>     d.name = ch["name"].toString();
>     channels.append(d);
> }
> ```
> `QJsonValue` może być null — zawsze sprawdź przez `obj.contains("key")` lub `isNull()`
> zanim wywołasz `.toInt()` / `.toString()`.

Receives raw JSON strings from `WebSocketService::textReceived`.
Parses the `"type"` field and emits typed signals:

```cpp
signals:
    void channelListReceived(const QList<ChannelData> &channels);
    void messageReceived(const MessageData &message);
    void historyReceived(int channelId, const QList<MessageData> &messages);
    void userListReceived(const QList<UserData> &users);
    void errorReceived(const QString &reason);
```

Use `QJsonDocument::fromJson` and `QJsonObject`.

### Step 5.2 — Implement `UIEventDispatcher`

File: `shared/events/UIEventDispatcher.h` / `.cpp`

Routes incoming typed events from `EventMapper` to the right service/state object.
Acts as a mediator — keeps services decoupled from each other.

```cpp
public:
    explicit UIEventDispatcher(EventMapper *mapper, QObject *parent = nullptr);

private slots:
    void onChannelListReceived(const QList<ChannelData> &channels);
    void onMessageReceived(const MessageData &msg);
    // …
```

### Step 5.3 — Implement `EventHandler`

File: `shared/events/EventHandler.h` / `.cpp`

Performs the actual state mutation triggered by dispatched events.
Updates `ChannelState`, `MessageState`, etc. in response.

---

## Phase 6 — Channel List Page (client-ui)

### Step 6.1 — Implement `ChannelService`

File: `shared/services/ChannelService.h` / `.cpp`

```cpp
public:
    void fetchChannels();
    void joinChannel(int channelId);
    void leaveChannel(int channelId);

signals:
    void channelsReceived(const QList<ChannelData> &channels);
    void joinedChannel(int channelId);
    void leftChannel(int channelId);
```

Sends JSON over `WebSocketService`; responses are routed back via `EventMapper`.

### Step 6.2 — Implement `ChannelState`

File: `shared/state/ChannelState.h` / `.cpp`

```cpp
public:
    static ChannelState &instance();
    QList<ChannelData> channels() const;
    void setChannels(const QList<ChannelData> &channels);
    int currentChannelId() const;
    void setCurrentChannel(int id);

signals:
    void channelsUpdated();
    void currentChannelChanged(int id);
```

### Step 6.3 — Implement `ChannelController` (client-ui)

File: `client-ui/src/controllers/ChannelController.h` / `.cpp`

Slot `fetchChannels()` calls `ChannelService::fetchChannels()`.
On `ChannelService::channelsReceived` → update `ChannelState`.

### Step 6.4 — Implement `ChannelItemComponent`

File: `client-ui/src/components/ChannelItemComponent.h` / `.cpp`

Custom `QWidget` showing channel name and member count.
Emits `channelSelected(int channelId)` on mouse press / click.

### Step 6.5 — Implement `HeaderBarComponent` (client-ui)

File: `client-ui/src/components/HeaderBarComponent.h` / `.cpp`

A horizontal bar with: app title label, current context label, logout button.
Emits `logoutRequested()`.

### Step 6.6 — Implement `SidebarComponent` (client-ui)

File: `client-ui/src/components/SidebarComponent.h` / `.cpp`

Vertical panel containing a `QScrollArea` of `ChannelItemComponent` items.
Public slot `setChannels(const QList<ChannelData> &)` rebuilds the list.
Emits `channelSelected(int)`.

### Step 6.7 — Implement `ChannelListPage`

File: `client-ui/src/pages/ChannelListPage.h` / `.cpp`

Layout: `HeaderBarComponent` top, `SidebarComponent` left, main area right (placeholder).
On `ChannelState::channelsUpdated` → call `sidebar->setChannels(...)`.
Emits `channelSelected(int)`.

### Step 6.8 — Register `/channels` route and wire in `main.cpp`

```cpp
auto *channelListPage = new ChannelListPage(stack);
router->registerRoute("/channels", channelListPage);

// after login:
connect(authController, &AuthController::loginSucceeded,
        [&]{ channelController->fetchChannels();
             router->navigateTo("/channels"); });
```

> ### ✅ [MILESTONE 3] — Channel list displays after login
> Log in → channel list page appears with the sidebar populated.
> Clicking a channel should (for now) just print to `qDebug`.

---

## Phase 7 — Channel Page & Messaging (client-ui)

### Step 7.1 — Implement `MessageService`

File: `shared/services/MessageService.h` / `.cpp`

```cpp
public:
    void fetchHistory(int channelId, int limit = 50, int offset = 0);
    void sendMessage(int channelId, const QString &text);

signals:
    void historyReceived(int channelId, const QList<MessageData> &messages);
    void messageReceived(const MessageData &msg);
```

### Step 7.2 — Implement `MessageState`

File: `shared/state/MessageState.h` / `.cpp`

```cpp
public:
    static MessageState &instance();
    QList<MessageData> messages(int channelId) const;
    void setHistory(int channelId, const QList<MessageData> &msgs);
    void appendMessage(const MessageData &msg);

signals:
    void messagesUpdated(int channelId);
```

### Step 7.3 — Implement `MessageItemComponent`

File: `client-ui/src/components/MessageItemComponent.h` / `.cpp`

`QWidget` with: username label (bold), timestamp label (small grey), message text label (word-wrap).
Constructor: `explicit MessageItemComponent(const MessageData &data, QWidget *parent = nullptr)`.

### Step 7.4 — Implement `MessageListComponent`

File: `client-ui/src/components/MessageListComponent.h` / `.cpp`

`QScrollArea` wrapping a `QWidget` with `QVBoxLayout`.

```cpp
public slots:
    void setMessages(const QList<MessageData> &messages);
    void appendMessage(const MessageData &msg);

private:
    void scrollToBottom();
```

> **Qt note — `QScrollArea` z dynamiczną zawartością:**
> `QScrollArea` samo w sobie nie ma layoutu — musisz osadzić w nim `QWidget`:
> ```cpp
> auto *container = new QWidget;
> m_layout = new QVBoxLayout(container);
> m_layout->addStretch();   // pcha wiadomości na dół
> setWidget(container);
> setWidgetResizable(true); // pozwala kontenerowi rozszerzać się
> ```
> Przewijanie na dół: `verticalScrollBar()->setValue(verticalScrollBar()->maximum())`.
> Wywołaj to przez `QTimer::singleShot(0, this, &MessageListComponent::scrollToBottom)`
> po `appendMessage` — timer z opóźnieniem 0 ms czeka na przeliczenie layoutu przez Qt.

### Step 7.5 — Implement `MessageInputComponent`

File: `client-ui/src/components/MessageInputComponent.h` / `.cpp`

`QLineEdit` + "Send" `QPushButton` in a `QHBoxLayout`.
Emit `messageSent(const QString &text)` on Enter key or button click.
Clear the input after emitting.

### Step 7.6 — Implement `UserListComponent` (client-ui)

File: `client-ui/src/components/UserListComponent.h` / `.cpp`

`QListWidget` in a `QVBoxLayout` with a "Users" header label.
Public slot `setUsers(const QList<UserData> &users)`.

### Step 7.7 — Implement `MessageController`

File: `client-ui/src/controllers/MessageController.h` / `.cpp`

```cpp
public slots:
    void onChannelSelected(int channelId);
    void onMessageSent(const QString &text);

private:
    int m_currentChannelId{-1};
    MessageService *m_service;
    MessageListComponent *m_list;   // injected
```

On `onChannelSelected` → call `fetchHistory` → populate `MessageListComponent`.
On `MessageService::messageReceived` → `MessageListComponent::appendMessage`.
On `onMessageSent` → `MessageService::sendMessage(m_currentChannelId, text)`.

### Step 7.8 — Implement `ChannelPage`

File: `client-ui/src/pages/ChannelPage.h` / `.cpp`

Layout:
```
[HeaderBarComponent]
[SidebarComponent] | [MessageListComponent]
                   | [MessageInputComponent]
                   | [UserListComponent]  ← right panel
```

`setChannel(int channelId)` — update header title, call `MessageController::onChannelSelected`.

### Step 7.9 — Register `/channel` route and wire in `main.cpp`

```cpp
auto *channelPage = new ChannelPage(stack);
router->registerRoute("/channel", channelPage);

connect(channelListPage, &ChannelListPage::channelSelected,
        [&](int id) {
            channelPage->setChannel(id);
            router->navigateTo("/channel");
        });
```

> ### ✅ [MILESTONE 4] — Can send and receive messages
> Log in → channel list → click a channel → messages load → type and send a message.

---

## Phase 8 — Admin Panel

### Step 8.1 — Implement `ConnectionPage` (admin-ui)

File: `admin-ui/src/pages/ConnectionPage.h` / `.cpp`

Identical structure to the client version but:
- Title/branding says "Admin Panel".
- On login success the `RouteGuard` must verify `SessionState::instance().isAdmin() == true` before allowing `/admin`.

### Step 8.2 — Wire `RouteGuard` for admin in `admin-ui/main.cpp`

```cpp
auto *guard = new RouteGuard(router, &window);
guard->addGuard([](const QString &route) -> bool {
    if (route == "/admin")
        return SessionState::instance().isAdmin();
    return true;
});
```

### Step 8.3 — Implement `ChannelManagementComponent` (admin-ui)

File: `admin-ui/src/components/ChannelManagementComponent.h` / `.cpp`

`QTableWidget` or `QTableView` listing channels with columns: ID, Name, Members, Actions.
Action buttons per row: "Edit", "Delete".
Header toolbar: "New Channel" button.

```cpp
signals:
    void createChannelRequested();
    void deleteChannelRequested(int channelId);
    void editChannelRequested(int channelId);
```

Public slot `setChannels(const QList<ChannelData> &)` repopulates the table.

### Step 8.4 — Implement `UserService`

File: `shared/services/UserService.h` / `.cpp`

```cpp
public:
    void fetchUsers();
    void kickUser(int userId, int channelId);
    void banUser(int userId);

signals:
    void usersReceived(const QList<UserData> &users);
    void kickSucceeded(int userId);
    void banSucceeded(int userId);
```

### Step 8.5 — Implement `AdminController`

File: `admin-ui/src/controllers/AdminController.h` / `.cpp`

```cpp
public slots:
    void createChannel(const QString &name, const QString &description);
    void deleteChannel(int channelId);
    void kickUser(int userId, int channelId);
    void banUser(int userId);
```

Delegates to `ChannelService` and `UserService`.

### Step 8.6 — Implement `AdminDashboardPage`

File: `admin-ui/src/pages/AdminDashboardPage.h` / `.cpp`

`QTabWidget` with two tabs:
- **Channels** — `ChannelManagementComponent`
- **Users** — `UserListComponent` (admin variant with Kick/Ban buttons)

`HeaderBarComponent` at the top showing "Admin Panel" + logout.

Wire `AdminController` slots to component signals.

### Step 8.7 — Register routes in `admin-ui/main.cpp`

```cpp
router->registerRoute("/connection", connectionPage);
router->registerRoute("/admin", dashboardPage);
router->navigateTo("/connection");

connect(authController, &AuthController::loginSucceeded, [&]{
    if (SessionState::instance().isAdmin()) {
        adminController->fetchAll();          // fetch channels + users
        router->navigateTo("/admin");
    } else {
        router->navigateTo("/connection");    // guard will block, but be explicit
    }
});
```

> ### ✅ [MILESTONE 5] — Admin panel functional
> Log in as admin → dashboard loads with channel and user tables.
> Create/delete a channel; verify the client also reflects the change.

---

## Phase 9 — Polish (both apps)

### Step 9.1 — Implement `NotificationComponent`

Files: `client-ui/src/components/NotificationComponent.h` / `.cpp`
*(repeat for `admin-ui`)*

Overlay `QLabel` positioned at top-right of the main window, auto-hides after 3 s via `QTimer`.

```cpp
public slots:
    void showInfo(const QString &msg);
    void showError(const QString &msg);
```

### Step 9.2 — Implement `AppState`

File: `shared/state/AppState.h` / `.cpp`

Singleton aggregating all state objects:

```cpp
public:
    static AppState &instance();
    ConnectionState &connection();
    SessionState    &session();
    ChannelState    &channels();
    MessageState    &messages();
```

Refactor direct singleton calls, for example `SessionState::instance()`, so that they go through `AppState`.

> ### ✅ [MILESTONE 6] — Full end-to-end client + admin working

---

## [OPTIONAL] Steps

See [ADDITIONAL.md](ADDITIONAL.md) for full descriptions.

- [OPTIONAL] A.1 — Dark mode / QSS theming
- [OPTIONAL] A.2 — Message history local cache (SQLite / `QSqlDatabase`)
- [OPTIONAL] A.3 — System tray & desktop notifications
- [OPTIONAL] A.4 — File / image attachment sharing
- [OPTIONAL] A.5 — Emoji picker component
- [OPTIONAL] A.6 — Message reactions
- [OPTIONAL] A.7 — User avatars / profile pictures
- [OPTIONAL] A.8 — Channel search and filter
- [OPTIONAL] A.9 — Typing indicators
- [OPTIONAL] A.10 — Message editing and deletion
- [OPTIONAL] A.11 — Unread message counters per channel
- [OPTIONAL] A.12 — QML migration for a modern look
