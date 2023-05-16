#include "seriemodel.h"
#include "tp_view_object_helper.h"

SerieModel::SerieModel(QObject *parent)
    :QAbstractListModel(parent)
    , mList(nullptr)
{
    mList = (TP_VIEW_OBJECT_HELPER*)parent;
    connect(this, SIGNAL(listChanged()), this, SLOT(onListChanged()));
}


int SerieModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid() || !mList )
        return 0;

    return mList->rowListCount();
}


QVariant SerieModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !mList)
        return QVariant();

    // const TP_VIEW_OBJECT *item = mList->getItem(index.row());
    switch (role) {
    case SelectionRole:
        return QVariant(mList->getIpAddr(index.row()));
    case NomRole:
        return QVariant(mList->getHostName(index.row()));
    case SerieRole:
        return QVariant(mList->getMacAddress(index.row()));
                    //PROBLEM HERE because item.serie is a pointer .... and cannot be handle with QVARIANT
    case IDRole:
        return QVariant(mList->getDHCPUsed(index.row()));

    }

    return QVariant();
}


Qt::ItemFlags SerieModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable; // FIXME: Implement me!
}

QHash<int, QByteArray> SerieModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[SelectionRole] = "selection";
    names[NomRole] = "nom";
    names[SerieRole] = "serie";
    names[IDRole] = "id";
    return names;
}

TP_VIEW_OBJECT_HELPER *SerieModel::list() const
{
    return mList;
}

void SerieModel::setList(TP_VIEW_OBJECT_HELPER *list)
{
    beginResetModel();

    if (mList)
        mList->disconnect(this);

    mList = list;

    if (mList) {
        connect(mList, &TP_VIEW_OBJECT_HELPER::preItemAppended, this, [=]() {
            const int index = mList->rowListCount();
            beginInsertRows(QModelIndex(), index, index);
        });
        connect(mList, &TP_VIEW_OBJECT_HELPER::postItemAppended, this, [=]() {
            endInsertRows();
        });
       }
    endResetModel();
}

void SerieModel::onListChanged()
{
    qDebug() << "SerieModel::onListChanged()";
    setList(mList);
}
