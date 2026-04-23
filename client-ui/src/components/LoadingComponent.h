#pragma once

#include <QWidget>

class QLabel;
class QProgressBar;

/**
 * @brief Prosty komponent wskazujacy trwanie operacji asynchronicznej.
 */
class LoadingComponent : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Tworzy komponent ladowania.
     * @param parent Widget rodzic.
     */
    explicit LoadingComponent(QWidget *parent = nullptr);
    ~LoadingComponent() override;

public slots:
    /**
     * @brief Ustawia komunikat pokazywany podczas ladowania.
     * @param message Tresc komunikatu.
     */
    void setMessage(const QString &message);

private:
    QLabel *m_label;
    QProgressBar *m_progressBar;
};
