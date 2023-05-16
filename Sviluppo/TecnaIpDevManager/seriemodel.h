#ifndef _SERIEMODEL_H
#define _SERIEMODEL_H

#include <QAbstractListModel>
#include "tp_view_object_helper.h"

class SerieModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(TP_VIEW_OBJECT_HELPER *list READ list WRITE setList NOTIFY listChanged)
    Q_INVOKABLE void setList(TP_VIEW_OBJECT_HELPER *list);

public:
    explicit SerieModel(QObject *parent = nullptr);

    enum {
        SelectionRole = Qt::UserRole,
        NomRole,
        SerieRole,
        IDRole
    };

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
//    bool setData(const QModelIndex &index, const QVariant &value,
//                int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    virtual QHash<int, QByteArray> roleNames() const override;

    TP_VIEW_OBJECT_HELPER *list() const;
    // void setList(TP_VIEW_OBJECT_HELPER *list);

signals:
    void listChanged();

private:
    TP_VIEW_OBJECT_HELPER *mList;

private slots:
    void onListChanged();
};

#endif // SERIEMODEL_H
