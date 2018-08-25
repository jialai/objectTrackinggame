#include "qdailog1.h"

QDailog1::QDailog1(QObject *parent)
    : QAbstractItemModel(parent)
{
}

QVariant QDailog1::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
}

QModelIndex QDailog1::index(int row, int column, const QModelIndex &parent) const
{
    // FIXME: Implement me!
}

QModelIndex QDailog1::parent(const QModelIndex &index) const
{
    // FIXME: Implement me!
}

int QDailog1::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    // FIXME: Implement me!
}

int QDailog1::columnCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    // FIXME: Implement me!
}

QVariant QDailog1::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    return QVariant();
}
