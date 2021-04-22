#pragma once

#include <QAbstractTableModel>

#include "Model/playlist.h"

class ImageListModel: public QAbstractTableModel
{
    Q_OBJECT
    Q_DISABLE_COPY(ImageListModel)

private:
    enum class ImageListRole{DisplayRole = Qt::UserRole +1, RatingRole, FileRole, ExtensionRole, PathRole, CountRole, AddedRole, LastPlayRole, Fullpath, IndexRole};
    enum class TristateOrder{NoOrder, AscendingOrder, DescendingOrder};

    struct Column {
        QString name;
        ImageListRole role;
        TristateOrder order;
    };

    PlaylistPointer m_model;
    QList<Column> m_columns = {{"File", ImageListRole::FileRole, TristateOrder::NoOrder},
                               {"Path", ImageListRole::PathRole, TristateOrder::NoOrder},
                               {"Count", ImageListRole::CountRole, TristateOrder::NoOrder},
                               {"Added", ImageListRole::AddedRole, TristateOrder::NoOrder},
                               {"Last play", ImageListRole::LastPlayRole, TristateOrder::NoOrder},
                               {"Rating", ImageListRole::RatingRole, TristateOrder::NoOrder},
                               {"Ext", ImageListRole::ExtensionRole, TristateOrder::NoOrder}};
    TristateOrder nextOrder(TristateOrder);

public:
    ImageListModel() = default;

    void setPLaylist(PlaylistPointer);
    Q_INVOKABLE QStringList columnModel() const;

    Q_INVOKABLE void play(int);

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex& = QModelIndex()) const override;
    int columnCount(const QModelIndex& = QModelIndex()) const override;

    QVariant data(const QModelIndex&, int) const override;
    Q_INVOKABLE void sort(int);

    QHash<int, QByteArray> roleNames() const override;
};

