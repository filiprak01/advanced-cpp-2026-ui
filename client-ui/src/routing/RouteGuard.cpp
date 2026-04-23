#include "RouteGuard.h"

#include "Router.h"

RouteGuard::RouteGuard(Router *router, QObject *parent)
    : QObject(parent)
    , m_router(router)
{
    if (m_router != nullptr)
        connect(m_router, &Router::routeChanged, this, &RouteGuard::onRouteChanged);
}

RouteGuard::~RouteGuard() = default;

void RouteGuard::addGuard(GuardFn fn)
{
    m_guards.append(std::move(fn));
}

void RouteGuard::onRouteChanged(const QString &route)
{
    if (m_isReverting)
        return;

    for (const GuardFn &guard : std::as_const(m_guards))
    {
        if (guard && !guard(route))
        {
            emit navigationBlocked(route);
            if (m_router != nullptr && !m_lastAllowedRoute.isEmpty())
            {
                m_isReverting = true;
                m_router->navigateTo(m_lastAllowedRoute);
                m_isReverting = false;
            }
            return;
        }
    }

    m_lastAllowedRoute = route;
}
