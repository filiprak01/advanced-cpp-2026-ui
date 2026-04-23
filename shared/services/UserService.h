#pragma once
#include <QObject>

/**
 * @brief Operacje administracyjne na użytkownikach.
 *
 * Udostępnia metody pobierania listy użytkowników, wyrzucania
 * (kick) i banowania — używane głównie przez panel admina.
 */
class UserService : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Konstruktor.
     * @param parent Rodzic QObject/QWidget.
     */
    explicit UserService(QObject *parent = nullptr);
    ~UserService() override;
};
