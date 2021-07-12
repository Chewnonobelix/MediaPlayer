#include "smartplaylist.h"

QMultiMap<QString, QString> SmartPlaylist::s_ops = {{"number", "inferior"},
                                                    {"number", "einferior"},
                                                    {"number", "superior"},
                                                    {"number", "esuperior"},
                                                    {"number", "equal"},
                                                    {"number", "limit"}, //TODO
                                                    {"all", "equal"},
                                                    {"all", "inList"}, //TODO
                                                    {"pre", "not"}, //TODO
                                                    {"string", "contain"}, //TODO
                                                    {"string", "start"}, //TODO
                                                    {"string", "end"}, //TODO
                                                    {"string", "reg"}, //TODO
                                                    {"group", "and"},
                                                    {"group", "or"}
                                                   };

SmartPlaylist::SmartPlaylist()
{
    m_rules->add();
}

bool SmartPlaylist::isValid(MediaPointer m)
{
    if(m_rules->count() == 0)
        return true;

    if(m_expression.isNull())
        return false;

    m_rules->set(m);
    return m_expression->evaluate();
}

void SmartPlaylist::append(MediaPointer m, int p)
{
    if (isValid(m))
        PlayList::append(m, p);
    else
        removeAll(m);
}

void SmartPlaylist::onMediaChanged(MediaPointer m) {
    if (!m.isNull())
        append(m);
}

SmartGroupPointer SmartPlaylist::rules() const
{
    return m_rules;
}

SmartGroup* SmartPlaylist::rule() const
{
    return m_rules.data();
}

bool SmartPlaylist::setRules(SmartGroupPointer rule)
{
    auto ret = m_rules != rule;
    if(ret) {
        m_rules = rule;
        ret &= rebuild();
        emit rulesChanged();
    }
    return ret;
}

bool SmartPlaylist::rebuild()
{
    m_expression = m_rules->create();

    return !m_expression.isNull();
}

