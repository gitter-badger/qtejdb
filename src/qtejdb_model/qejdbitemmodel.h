#ifndef QEJDBITEMMODEL_H
#define QEJDBITEMMODEL_H

#include <QObject>
#include <QAbstractItemModel>
#include "qejdbdatabase.h"
#include "qbsonobject.h"

class QEjdbItemModelWorker;
class QBsonModelData;

class QEjdbItemModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit QEjdbItemModel(QEjdbDatabase db = QEjdbDatabase(), QObject *parent = 0);

    ~QEjdbItemModel();

     QVariant data(const QModelIndex &index, int role) const;

     QModelIndex index(int row, int column,
                       const QModelIndex &parent = QModelIndex()) const;

     QModelIndex parent(const QModelIndex &index) const;

     int rowCount(const QModelIndex &parent = QModelIndex()) const;

     bool setData(const QModelIndex &index, const QVariant &value,
                  int role = Qt::EditRole);

     bool insertRows(int position, int rows,
                     const QModelIndex &parent = QModelIndex());

     bool removeRows(int position, int rows,
                     const QModelIndex &parent = QModelIndex());

     void setNames(QStringList names, QHash<QString, QString> namesMap);
     QVariant headerData(int section, Qt::Orientation orientation, int role) const;
     void setCollection(const QString &collectionName);

     QHash<int, QByteArray> roleNames() const;
signals:

public slots:

private:
    QEjdbDatabase m_db;



    QHash<int, QByteArray> m_roleNames;

    QHash<int, QByteArray> m_roleNamesMap;

    QEjdbItemModelWorker *m_modelWorker;

    QBsonModelData *getQBsonModelData(const QModelIndex &index) const;
    QBsonObject getQBson(const QModelIndex &index) const;

};

#endif // QEJDBITEMMODEL_H