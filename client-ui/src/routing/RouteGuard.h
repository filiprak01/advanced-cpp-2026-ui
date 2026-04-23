#pragma once

#include <QList>
#include <QObject>
#include <QString>

#include <functional>

class Router;

/**
 * @brief Filtruje nawigacje na podstawie zarejestrowanych warunkow.
 *
 * RouteGuard pozwala zablokowac wejscie na wybrane trasy, na przyklad gdy
 * uzytkownik nie jest jeszcze zalogowany albo nie ma odpowiednich uprawnien.
 */
class RouteGuard : public QObject
{
    Q_OBJECT
public:
    using GuardFn = std::function<bool(const QString &route)>;

    /**
     * @brief Tworzy obiekt straznika tras.
     * @param router Router, ktorego zmiany tras beda obserwowane.
     * @param parent Obiekt rodzic w hierarchii QObject.
     */
    explicit RouteGuard(Router *router, QObject *parent = nullptr);
    ~RouteGuard() override;

    /**
     * @brief Dodaje kolejna funkcje walidujaca trase.
     * @param fn Funkcja zwracajaca `true`, gdy trasa jest dozwolona.
     */
    void addGuard(GuardFn fn);

signals:
    void navigationBlocked(const QString &route);

public slots:
    /**
     * @brief Reaguje na zmiane trasy i weryfikuje warunki dostepu.
     * @param route Trasa wskazana przez router.
     */
    void onRouteChanged(const QString &route);

private:
    Router *m_router;
    QList<GuardFn> m_guards;
    QString m_lastAllowedRoute;
    bool m_isReverting{false};
};
