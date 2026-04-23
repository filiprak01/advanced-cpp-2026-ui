#pragma once

#include <QWidget>

class QLineEdit;
class QPushButton;

/**
 * @brief Komponent odpowiedzialny za wprowadzanie nowej wiadomosci.
 *
 * Uzytkownik wpisuje tresc i wysyla ja przez klikniecie przycisku lub akcje
 * klawiszowa. Komponent nie zna identyfikatora kanalu ani warstwy sieciowej.
 */
class MessageInputComponent : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Tworzy pole wprowadzania wiadomosci.
     * @param parent Widget rodzic.
     */
    explicit MessageInputComponent(QWidget *parent = nullptr);
    ~MessageInputComponent() override;

signals:
    void messageSent(const QString &text);

public slots:
    /**
     * @brief Ustawia tekst podpowiedzi wewnatrz pola tekstowego.
     * @param text Tekst placeholdera.
     */
    void setPlaceholderText(const QString &text);

    /**
     * @brief Przelacza komponent w tryb zajety lub gotowy.
     * @param busy Flaga zajetosci.
     */
    void setBusy(bool busy);

private:
    /**
     * @brief Waliduje i emituje sygnal wyslania wiadomosci.
     */
    void submit();

    QLineEdit *m_input;
    QPushButton *m_sendButton;
};
