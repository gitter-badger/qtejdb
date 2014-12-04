#include "qbsonarray.h"
#include <QSharedData>
#include "qatomic.h"
#include <QLinkedList>
#include "qbsonvalue.h"

class QBsonArrayData {
public:
    QBsonArrayData() { ref = 1; }

    QAtomicInt ref;
    QList<QBsonValue> list;
};

/**
 * @brief QBsonArray::QBsonArray construct a empty QBsonArray
 */
QBsonArray::QBsonArray() : data(new QBsonArrayData)
{
}

/**
 * @brief QBsonArray::QBsonArray copy constructor
 * @param rhs
 */
QBsonArray::QBsonArray(const QBsonArray &rhs) : data(rhs.data)
{
    data->ref.ref();
}

/**
 * @brief QBsonArray::operator = copy a shared instance
 *
 * @param rhs source instance
 * @return this instace
 */
QBsonArray &QBsonArray::operator=(const QBsonArray &rhs)
{
    qAtomicAssign(data, rhs.data);
    return *this;
}

/**
 * @brief QBsonArray::~QBsonArray destruct instance and delete shared instance if not
 * used.
 */
QBsonArray::~QBsonArray()
{
    if (!data->ref.deref())
        delete data;
}

/**
 * @brief QBsonArray::append append a QBsonValue to the array.
 * @see QList::append()
 * @param value value to append
 *
 * @return Returns this QBsonArray.
 */
QBsonArray& QBsonArray::append(const QBsonValue &value)
{
    data->list.append(value);
    return *this;
}

/**
 * @brief QBsonArray::insert insert a QBsonValue on given index.
 * @see QList::insert()
 * @param i insertion index.
 * @param value value to insert.
 *
 * @return this QBsonArray.
 */
QBsonArray& QBsonArray::insert(int i, const QBsonValue &value)
{
    data->list.insert(i, value);
    return *this;
}

/**
 * @brief QBsonArray::value return QBsonValue at given index.
 * @see QList::at(int)
 * @param i index
 * @return
 */
QBsonValue QBsonArray::value(int i)
{
    return data->list.at(i);
}

/**
 * @brief QBsonArray::values return all values.
 * @return QList<QBsonValue>()
 */
QList<QBsonValue> QBsonArray::values() const
{
    return data->list;
}

int QBsonArray::size() const
{
    return data->list.size();
}