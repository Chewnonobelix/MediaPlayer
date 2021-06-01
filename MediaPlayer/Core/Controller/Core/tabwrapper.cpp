#include "tabwrapper.h"

QUuid TabWrapper::create()
{
    auto manager = new TabManager;
    m_model[manager->id()] = manager;

    connect(manager, &TabManager::clicked, this, &TabWrapper::setCurrentTab);

    return manager->id();
}

QPointer<TabManager> TabWrapper::get(QUuid id)
{
    return m_model.contains(id) ? m_model[id] : nullptr;
}

QPointer<TabManager> TabWrapper::get(QString id)
{
    return get(QUuid::fromString(id));
}

void TabWrapper::setCurrentTab(QString id)
{
    m_current = QUuid::fromString(id);
}

QPointer<TabManager>& TabWrapper::current()
{
    auto it = std::find_if(m_model.begin(), m_model.end(), [this](const QPointer<TabManager>& tab) {
        return tab->contains(m_current);
    });

    return *it;
}

void TabWrapper::setPlayer(QQmlComponent * p)
{
    auto tab = current();
    (*tab)[m_current].player = p;
}

void TabWrapper::setPlaylist(PlaylistPointer pl)
{
    auto tab = current();
    (*tab)[m_current].setPlaylist(pl);
}
