#pragma once

#include <QHash>
#include <QObject>
#include <QPointer>
#include <QString>
#include <QStringList>

class QStackedWidget;
class QWidget;

/**
 * @brief Zarzadza przejsciami pomiedzy stronami klienta Qt.
 *
 * Router rejestruje strony pod nazwami tras i przechowuje prosta historie
 * nawigacji. Jego zadaniem jest zmiana aktualnego widoku w QStackedWidget oraz
 * poinformowanie pozostalych warstw o zmianie aktywnej trasy.
 */
class Router : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Tworzy router pracujacy na zadanym stosie widgetow.
     * @param stack Wspoldzielony QStackedWidget z ekranami aplikacji.
     * @param parent Obiekt rodzic w hierarchii QObject.
     */
    explicit Router(QStackedWidget *stack, QObject *parent = nullptr);
    ~Router() override;

    /**
     * @brief Rejestruje widget pod logiczna nazwa trasy.
     * @param route Nazwa lub sciezka trasy.
     * @param page Widget reprezentujacy strone.
     */
    void registerRoute(const QString &route, QWidget *page);

    /**
     * @brief Zwraca nazwe aktualnie wyswietlanej trasy.
     * @return Nazwa aktywnej trasy lub pusty napis.
     */
    QString currentRoute() const;

signals:
    void routeChanged(const QString &route);

public slots:
    /**
     * @brief Przelacza aplikacje na wskazana trase.
     * @param route Nazwa trasy, ktora ma zostac wyswietlona.
     */
    void navigateTo(const QString &route);

    /**
     * @brief Wraca do poprzednio odwiedzonej trasy.
     */
    void goBack();

private:
    QPointer<QStackedWidget> m_stack;
    QHash<QString, QWidget *> m_routes;
    QStringList m_history;
    QString m_currentRoute;
    bool m_isNavigatingBack{false};
};
