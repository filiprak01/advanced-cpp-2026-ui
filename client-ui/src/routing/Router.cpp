#include "Router.h"

#include <QStackedWidget>

Router::Router(QStackedWidget *stack, QObject *parent)
    : QObject(parent)
    , m_stack(stack)
{
}

Router::~Router() = default;

void Router::registerRoute(const QString &route, QWidget *page)
{
    if (route.isEmpty() || page == nullptr || !m_stack)
        return;

    m_routes.insert(route, page);
    if (m_stack->indexOf(page) == -1)
        m_stack->addWidget(page);
}

QString Router::currentRoute() const
{
    return m_currentRoute;
}

void Router::navigateTo(const QString &route)
{
    if (!m_stack || !m_routes.contains(route))
        return;

    QWidget *page = m_routes.value(route);
    if (page == nullptr)
        return;

    if (!m_currentRoute.isEmpty() && m_currentRoute != route && !m_isNavigatingBack)
        m_history.append(m_currentRoute);

    m_currentRoute = route;
    m_stack->setCurrentWidget(page);
    emit routeChanged(route);
}

void Router::goBack()
{
    if (m_history.isEmpty())
        return;

    const QString previousRoute = m_history.takeLast();
    m_isNavigatingBack = true;
    navigateTo(previousRoute);
    m_isNavigatingBack = false;
}
