#include "qbsonitemmodelsync_p.h"

/**
 * @class QEjdbCollectionSync manages bson document synchronisation with a
 * collection. Synchronize a qbsonmodel with a collection in database. Create,
 * update, remove bsonobjects. A row insert append the object into the database collection.
 */

/**
 * @brief QEjdbCollectionSync::QEjdbCollectionSync initialize a instance with
 * a database and an underlying model that caches the data.
 */
QEjdbCollectionSync::QEjdbCollectionSync(QEjdbDatabase db, QObject *parent)
    : m_db(db), m_qBsonItemModel(new QBsonItemModel(this)), QObject(parent)
{

    QObject::connect(m_qBsonItemModel, &QBsonItemModel::itemInserted,
                     this, &QEjdbCollectionSync::itemSave);
    QObject::connect(m_qBsonItemModel, &QBsonItemModel::itemRemoved,
                     this, &QEjdbCollectionSync::itemRemoved);
    QObject::connect(m_qBsonItemModel, &QBsonItemModel::itemUpdated,
                     this, &QEjdbCollectionSync::itemUpdated);

}

/**
 * @brief QEjdbCollectionSync::~QEjdbCollectionSync deletes bsonModel
 */
QEjdbCollectionSync::~QEjdbCollectionSync()
{
    QObject::disconnect(m_qBsonItemModel, &QBsonItemModel::itemInserted,
                     this, &QEjdbCollectionSync::itemSave);
    QObject::disconnect(m_qBsonItemModel, &QBsonItemModel::itemRemoved,
                     this, &QEjdbCollectionSync::itemRemoved);
    QObject::disconnect(m_qBsonItemModel, &QBsonItemModel::itemUpdated,
                     this, &QEjdbCollectionSync::itemUpdated);
    delete m_qBsonItemModel;
}

/**
 * @brief QEjdbCollectionSync::query Returns the query object. A query is a bson
 * structure to fetch and filter a collection of bson objects from database.
 * @see QEjdbCollectionSync::setQuery()
 */
QBsonObject QEjdbCollectionSync::query() const
{
    return m_query;
}

/**
 * @brief QEjdbCollectionSync::hints returns the hints. Hints are additional
 * options to control data fetching.
 */
QBsonObject QEjdbCollectionSync::hints() const
{
    return m_hints;
}

/**
 * @brief QEjdbCollectionSync::collection returns the collection. A collection
 * is a container for bson objects. So you have to define the collection.
 */
QString QEjdbCollectionSync::collection() const
{
    return m_collection;
}

/**
 * @brief QEjdbCollectionSync::model returns the bson model that manages the
 * bson objects.
 */
QBsonItemModel *QEjdbCollectionSync::model()
{
    return m_qBsonItemModel;
}

/**
 * @brief QEjdbCollectionSync::fetch get data from collection with query and
 * hints.
 */
void QEjdbCollectionSync::fetch()
{
    if (isDbValid()) {
        QEjdbResult result = m_db.query(m_collection, m_query, m_hints);
        m_qBsonItemModel->set(result.values());
    }
}

/**
 * @brief QEjdbCollectionSync::setQuery set a new query value.
 * @see QEjdbCollectionSync::query()
 */
void QEjdbCollectionSync::setQuery(QBsonObject query)
{
    m_query = query;
}

/**
 * @brief QEjdbCollectionSync::setHints set new hints.
 * @see QEjdbCollectionSync::hints()
 */
void QEjdbCollectionSync::setHints(QBsonObject hints)
{
    m_hints = hints;
}

/**
 * @brief QEjdbCollectionSync::setCollection set a new collection.
 */
void QEjdbCollectionSync::setCollection(QString collection)
{
    m_collection = collection;
}

/**
 * @brief QEjdbCollectionSync::itemRemoved remove item from db.
 */
void QEjdbCollectionSync::itemRemoved(int row, QBsonObject removedObject)
{
    Q_UNUSED(row)
    if (isDbValid()) {
        m_db.remove(m_collection, removedObject);
    }
}

/**
 * @brief QEjdbCollectionSync::itemInserted update the bsonobject at given row.
 */
void QEjdbCollectionSync::itemSave(int row)
{
    if (isDbValid()) {
        QBsonObject bsonObject = m_qBsonItemModel->row(row);
        m_db.save(m_collection, bsonObject);
    }
}

/**
 * @brief QEjdbCollectionSync::itemUpdated load a bson from db change it and
 * saves back.
 */
void QEjdbCollectionSync::itemUpdated(QString property, QVariant value, int row)
{
    if (isDbValid()) {
        QBsonObject obj = m_qBsonItemModel->row(row);
        QBsonObject loadedObj = m_db.load(m_collection, obj.oid());
        loadedObj.insert(property, QBsonValue::fromVariant(value));
        m_db.save(m_collection, loadedObj);
    }

}

/**
 * @class QEjdbArrayPropertySync
 * @brief QEjdbArrayPropertySync manages a array property.
 *
 * interface requirements
 * ======================
 * - set bson object and re load it from db
 * - set id and load it from db
 * - if propertyCollection is set then make a join over
 * property and propertyCollection
 * - save a property bson on a own collection only propertyCollection is set
 * - if propertyCollection not set the bson is saved as subobj of parent object
 *
 */

/**
 * @brief QEjdbArrayPropertySync::bsonObject returns bson object that contains
 * the array property stored in propertyName.
 */
QEjdbArrayPropertySync::QEjdbArrayPropertySync(QEjdbDatabase db, QObject *parent)
    : m_qBsonItemModel(new QBsonItemModel(this)), m_db(db), QObject(parent)
{
    QObject::connect(m_qBsonItemModel, &QBsonItemModel::itemInserted,
                     this, &QEjdbArrayPropertySync::itemInserted);
    QObject::connect(m_qBsonItemModel, &QBsonItemModel::itemRemoved,
                     this, &QEjdbArrayPropertySync::itemRemoved);
    QObject::connect(m_qBsonItemModel, &QBsonItemModel::itemUpdated,
                     this, &QEjdbArrayPropertySync::itemUpdated);
    QObject::connect(m_qBsonItemModel, &QBsonItemModel::itemMoved,
                     this, &QEjdbArrayPropertySync::itemMoved);
}

/**
 * @brief QEjdbArrayPropertySync::~QEjdbArrayPropertySync delete bsonmodel.
 */
QEjdbArrayPropertySync::~QEjdbArrayPropertySync()
{
    QObject::disconnect(m_qBsonItemModel, &QBsonItemModel::itemInserted,
                     this, &QEjdbArrayPropertySync::itemInserted);
    QObject::disconnect(m_qBsonItemModel, &QBsonItemModel::itemRemoved,
                     this, &QEjdbArrayPropertySync::itemRemoved);
    QObject::disconnect(m_qBsonItemModel, &QBsonItemModel::itemUpdated,
                     this, &QEjdbArrayPropertySync::itemUpdated);
    QObject::disconnect(m_qBsonItemModel, &QBsonItemModel::itemMoved,
                     this, &QEjdbArrayPropertySync::itemMoved);
    delete m_qBsonItemModel;
}

/**
 * @brief QEjdbCollectionSync::model returns the bson model that manages the
 * bson objects.
 */
QBsonItemModel *QEjdbArrayPropertySync::model()
{
   return m_qBsonItemModel;
}

QBsonObject QEjdbArrayPropertySync::bsonObject()
{
    return m_parentObject;
}

/**
 * @brief QEjdbArrayPropertySync::propertyName returns the property name under
 * which the array is stored.
 */
QString QEjdbArrayPropertySync::propertyName()
{
    return m_propertyName;
}

/**
 * @brief QEjdbArrayPropertySync::collection is the collection in which the
 * bson object is stored.
 */
QString QEjdbArrayPropertySync::collection()
{
    return m_collection;
}

/**
 * @brief QEjdbArrayPropertySync::propertyCollection returns property collection.
 * The collection in which the bsonobjects in property are stored.
 */
QString QEjdbArrayPropertySync::propertyCollection()
{
    return m_propertyCollection;
}

/**
 * @brief QEjdbArrayPropertySync::fetch fetch the object and add it to model.
 */
void QEjdbArrayPropertySync::fetch()
{
    if (isDbValid()) {

        QBsonObject found;
        if (isJoined()) {
            QBsonObject query;
            QBsonArray andQ;
            andQ.append(QBsonObject("_id", m_parentObject.oid()));
            query.append("$do", QBsonObject(
                                  m_propertyName, QBsonObject(
                                      "$join", m_propertyCollection)
                                  )
                        );
            query.append("$and", andQ);
            QEjdbResult result = m_db.query(m_collection, query);

            if (result.count() > 0) {
                found = result.first();
            }

        } else {
            found = m_db.load(m_collection, m_parentObject.oid());
        }

        if (found.hasOid()) {
            QBsonArray array = found.value(m_propertyName).toArray();
            m_qBsonItemModel->clear();
            foreach (QBsonValue val, array.values()) {
                if (val.isObject()) {
                    m_qBsonItemModel->append(val.toObject());
                }
            }
        }

    }
}

/**
 * @brief QEjdbArrayPropertySync::setPropertyCollection set a collection for
 * property. @see QEjdbArrayPropertySync::propertyCollection()
 */
void QEjdbArrayPropertySync::setPropertyCollection(QString propertyCollection)
{
    m_propertyCollection = propertyCollection;
}

/**
 * @brief QEjdbArrayPropertySync::setCollection set collection in which the bson
 * object is stored. @see QEjdbArrayPropertySync::collection()
 */
void QEjdbArrayPropertySync::setCollection(QString collection)
{
    m_collection = collection;
}

/**
 * @brief QEjdbArrayPropertySync::setBsonObject the bson object that contains
 * the array property. @see QEjdbArrayPropertySync::bsonObject()
 */
void QEjdbArrayPropertySync::setBsonObject(QBsonObject bsonObject, QString propertyName)
{
    m_parentObject = bsonObject;
    m_propertyName = propertyName;
}

/**
 * @brief QEjdbArrayPropertySync::itemRemoved slot to remove a object from
 * array property.
 */
void QEjdbArrayPropertySync::itemRemoved(int row, QBsonObject removedObject)
{
    Q_UNUSED(removedObject)
    if (isDbValid()) {
        QBsonArray array = getBsonArray();
        array.remove(row);
        m_db.save(m_collection, m_parentObject);
    }
}

/**
 * @brief QEjdbArrayPropertySync::itemInserted slot to insert a new bson object.
 */
void QEjdbArrayPropertySync::itemInserted(int row)
{
    if (isDbValid()) {
        QBsonObject newObject = m_qBsonItemModel->row(row);
        QBsonArray array = getBsonArray();
        if (row >= 0 && row <= array.size()) {
            if (isJoined()) {
                m_db.save(m_propertyCollection, newObject);
                array.insert(row, newObject.oid());
            } else {
                array.insert(row, newObject);
            }
            m_parentObject.insert(m_propertyName, array);
            m_db.save(m_collection, m_parentObject);
        }
    }
}

/**
 * @brief QEjdbArrayPropertySync::itemMoved slot to move a bson object on array.
 */
void QEjdbArrayPropertySync::itemMoved(int sourceRow, int destinationRow)
{
    m_parentObject = m_db.load(m_collection, m_parentObject.oid());
    QBsonArray array = getBsonArray();
    array.insert(destinationRow, array.take(sourceRow));
    saveBson(m_collection, m_parentObject);
}

/**
 * @brief QEjdbArrayPropertySync::itemUpdated slot to update a document. Saves
 * only object have a oid.
 */
void QEjdbArrayPropertySync::itemUpdated(QString property, QVariant value, int row)
{
    QBsonObject updateObject = reloadBson(m_propertyCollection, m_qBsonItemModel->row(row));
    updateObject.insert(property, QBsonValue::fromVariant(value));
    if (isJoined()) {
        saveBson(m_propertyCollection, updateObject);
    } else {
        QBsonArray array = getBsonArray();
        array.replace(row, updateObject);
        saveBson(m_collection, m_parentObject);
    }

}

/**
 * @brief QEjdbArrayPropertySync::loadBson Returns a bson by oid. If oid not valid
 * returns an empty bson.
 *
 */
QBsonObject QEjdbArrayPropertySync::loadBson(const QString &collection, const QBsonOid &oid)
{
    if (oid.isValid() && isDbValid()) {
        return m_db.load(collection, oid.toString());
    }
    return QBsonObject();
}

/**
 * @brief QEjdbArrayPropertySync::reloadBson loads a bson with _id from given bson
 * from database. If database is not valid or bson have no oid property returns
 * the given bson.
 */
QBsonObject QEjdbArrayPropertySync::reloadBson(
        const QString &collection, const QBsonObject &bson
) {
    if (bson.hasOid() && isDbValid()) {
        QBsonObject obj = m_db.load(collection, bson.oid());
        if (obj.hasOid()) return obj;
    }
    return bson;
}

/**
 * @brief QEjdbArrayPropertySync::saveBson save the given bson.
 */
void QEjdbArrayPropertySync::saveBson(const QString &collection, QBsonObject &bson)
{
    if (isDbValid()) {
        m_db.save(collection, bson);
    }
}
