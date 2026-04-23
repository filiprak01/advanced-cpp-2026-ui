# Additional Features

Optional enhancements for the advanced-cpp-2026-ui apps.  
Each item is listed in [IMPLEMENTATION.md](IMPLEMENTATION.md) under `[OPTIONAL]`.

---

## A.1 — Dark Mode / QSS Theming

Apply a `QStyleSheet` globally from a `.qss` file bundled as a Qt resource.

**What you need:**
- `resources/themes/dark.qss` and `resources/themes/light.qss`
- A `ThemeManager` singleton that calls `qApp->setStyleSheet(...)`.
- A toggle button in `HeaderBarComponent`.

**Why it matters:** desktop apps without theming feel outdated; Qt QSS gives you full CSS-like control.

**Dependency:** none (pure Qt).

---

## A.2 — Message History Local Cache (SQLite)

Cache received messages in a local SQLite database so previous messages load instantly without
a server round-trip.

**What you need:**
- Qt module: `Qt6::Sql` (add to `target_link_libraries`).
- `shared/services/CacheService.h` — wraps `QSqlDatabase` with `QSQLITE` driver.
- Schema: `CREATE TABLE messages (id INT, channel_id INT, author TEXT, text TEXT, ts TEXT)`.
- `MessageService` writes to cache on receive; reads from cache on startup before fetching from server.

**Why it matters:** improves perceived performance and allows offline message browsing.

---

## A.3 — System Tray & Desktop Notifications

Show a system-tray icon and native OS notifications for new messages when the window is minimized.

**What you need:**
- `QSystemTrayIcon` (already in `Qt6::Widgets`).
- `QSystemTrayIcon::showMessage(title, body, icon, msecs)`.
- Listen for `MessageState::messagesUpdated` when the window is not active.

**Why it matters:** standard feature of any desktop chat app.

---

## A.4 — File / Image Attachment Sharing

Allow users to send images or files through the chat.

**What you need:**
- `QFileDialog::getOpenFileName` triggered from a paperclip button in `MessageInputComponent`.
- Base64-encode small files (< 1 MB) and embed in the JSON message, or implement a separate
  HTTP upload endpoint on the server side.
- `MessageItemComponent` renders `QLabel` with `QPixmap` if the message contains an image payload.

**Why it matters:** significantly increases usefulness of the chat.

**Note:** requires backend support for binary/file messages.

---

## A.5 — Emoji Picker Component

A popup grid of emoji characters that inserts the selection into `MessageInputComponent`.

**What you need:**
- `EmojiPickerComponent` — a `QDialog` or floating `QWidget` with a `QGridLayout` of `QPushButton`
  labeled with Unicode emoji characters.
- A 😊 button next to the message input that shows/hides the picker.
- Emit `emojiSelected(const QString &emoji)` and append to `QLineEdit`.

**Dependency:** none beyond Qt. For a richer picker consider the
[QtEmoji](https://github.com/nicehash/QtEmoji) library.

---

## A.6 — Message Reactions

Allow users to react to messages with emoji (👍 👎 😂 etc.).

**What you need:**
- Extend `MessageData` with `QMap<QString, int> reactions`.
- Right-click context menu on `MessageItemComponent` → "Add Reaction" → mini emoji picker.
- Backend must support a `messages/react` event type.
- `MessageItemComponent` renders a row of reaction pills below the message text.

---

## A.7 — User Avatars / Profile Pictures

Display circular avatar images next to each message and in the user list.

**What you need:**
- `UserData::avatarUrl` field (add to the model).
- A reusable `AvatarWidget` that loads a `QPixmap` from a URL using `QNetworkAccessManager`,
  clips it to a circle with `QPainter` and a `QPainterPath`.
- Fallback: coloured circle with the first letter of the username.

**Dependency:** `Qt6::Network` (already linked).

---

## A.8 — Channel Search and Filter

A search bar above `SidebarComponent` that filters the channel list in real time.

**What you need:**
- `QLineEdit *m_searchEdit` in `SidebarComponent`.
- On text changed: filter the internal `QList<ChannelData>` and rebuild the widget list
  (or use a `QSortFilterProxyModel` if you migrate to `QListView`).

**Why it matters:** essential UX once the channel count exceeds ~10.

---

## A.9 — Typing Indicators

Show "Alice is typing…" when another user is actively composing a message.

**What you need:**
- Debounced `typing_start` / `typing_stop` events sent from `MessageInputComponent` on key presses.
- Backend broadcasts these to all channel members.
- `ChannelPage` shows a small label below `MessageListComponent` with the typing users.
- `QTimer` auto-hides the label after 5 s with no update.

---

## A.10 — Message Editing and Deletion

Allow users to edit or delete their own messages.

**What you need:**
- Right-click context menu on `MessageItemComponent` → "Edit" / "Delete" (only for own messages).
- Edit: replace the `QLabel` with a `QLineEdit`, confirm with Enter.
- Sends `messages/edit` / `messages/delete` events to the server.
- Server broadcasts the change; `EventMapper` handles `message_edited` / `message_deleted` event types.
- `MessageState` and `MessageListComponent` update in place.

---

## A.11 — Unread Message Counters Per Channel

Show a badge with the unread count on each `ChannelItemComponent` in the sidebar.

**What you need:**
- `ChannelData::unreadCount` field.
- `MessageState` tracks the last-read message ID per channel (persisted in local storage or
  `QSettings`).
- `ChannelItemComponent` renders a coloured badge `QLabel` when `unreadCount > 0`.
- Clear the counter when the user navigates to the channel.

---

## A.12 — QML Migration for Modern UI

Replace `QWidget`-based UI with `QtQuick` / QML for GPU-accelerated, animated, modern visuals.

**What you need:**
- Add `Qt6::Quick` and `Qt6::QuickControls2` to `target_link_libraries`.
- Create `.qml` files mirroring each page and component.
- Expose C++ models (state classes) to QML via `qmlRegisterSingletonInstance` or
  `QQmlContext::setContextProperty`.
- The `shared/` library stays unchanged — only the presentation layer changes.

**Effort:** high — treat as a full rewrite of the UI layer. Recommended after all milestones
are complete and the backend integration is stable.
