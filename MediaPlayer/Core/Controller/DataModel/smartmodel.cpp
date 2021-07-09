#include "smartmodel.h"

SmartModel::SmartModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

int SmartModel::columnCount(const QModelIndex&) const
{
    return m_depth + 1;
}

int SmartModel::rowCount(const QModelIndex &) const
{
    return m_model ? m_flat.count() : 0;
}

QVariant SmartModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    auto model = m_flat[index.row()];

    switch(SmartRole(role)) {
    case SmartRole::TypeRole:
        return index.column() == model.depth ? model.type : QVariant();
    case SmartRole::IdRole:
        return model.rule->id();
    default:
        break;
    }

    return QVariant();
}

bool SmartModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

QHash<int, QByteArray> SmartModel::roleNames() const
{
    static QHash<int, QByteArray> ret = {{int(SmartRole::OpRole), "op"},
                                         {int(SmartRole::TypeRole), "type"},
                                         {int(SmartRole::FieldRole), "field"},
                                         {int(SmartRole::IdRole), "id"},

                                        };

    return ret;
}

SmartGroup* SmartModel::group() const
{
    return m_model.data();
}

void SmartModel::setModel(SmartGroupPointer g)
{
    clear();
    m_model = g;
    m_flat = toFlat(m_model);
    emit groupChanged();

    restore();
}

QList<SmartModel::Flat> SmartModel::toFlat(SmartGroupPointer g, int depth)
{
    static int j = 0;
    if(depth == 1)
        j = 0;
    m_depth = std::max(m_depth, depth);

    QList<SmartModel::Flat> ret;
    ret << Flat{depth-1, "group", g};
    j++;
    for(auto i = 0; i < g->count(); i++) {
        if(!(*g)[i].dynamicCast<SmartRule>().isNull()) {
            ret << Flat{depth, "rule", (*g)[i]};
            j++;
        }
        else {
            ret << toFlat((*g)[i].dynamicCast<SmartGroup>(), depth+1);
        }
    }

    qDebug()<<"Build"<<j<<ret.count();
    return ret;
}

void SmartModel::add(int row, bool group)
{
    clear();

    auto g = m_flat[row].rule.dynamicCast<SmartGroup>();
    if(!g.isNull()) {
        g->add(group);
    }

    m_flat = toFlat(m_model);

    emit groupChanged();

    restore();
}

bool SmartModel::remove(QString i)
{
    clear();

    auto count = m_flat.count();

    auto it = std::find_if(m_flat.begin(), m_flat.end(), [i](Flat f) {
            qDebug()<<f.rule->id()<<i<<(f.rule->id() == QUuid::fromString(i));
            return f.rule->id() == QUuid::fromString(i);
    });


    auto ret = (it != m_flat.end()) && it->rule->parent() ? it->rule->parent().dynamicCast<SmartGroup>()->remove(QUuid::fromString(i)) : false;
    qDebug()<<count<<m_flat.count()<<ret<<(it->rule->parent());
    m_flat.clear();
    m_flat = toFlat(m_model);
    emit groupChanged();

    restore();

    return ret && m_flat.count() == count -1;
}

void SmartModel::clear()
{
    beginRemoveRows(QModelIndex(), 0, rowCount());
    removeRows(0, rowCount() - 1);
    endRemoveRows();

    beginRemoveColumns(QModelIndex(), 0, columnCount());
    removeColumns(0, columnCount() - 1);
    endRemoveColumns();
}

void SmartModel::restore()
{
    beginInsertRows(QModelIndex(), 0, rowCount() + 1);
    insertRows(0, rowCount() + 1);
    endInsertRows();

    beginInsertColumns(QModelIndex(), 0, columnCount() + 1);
    insertColumns(0, columnCount() + 1);
    endInsertColumns();
}
