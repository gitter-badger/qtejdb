#include "qbsonvalue.h"
#include <QSharedData>
#include "qbsonobject.h"
#include "qbsonarray.h"
#include "qbsonoid.h"
#include <QVariant>
#include "qatomic.h"

class QBsonValueData {

public:

    QBsonValueData(QBsonValue::QBsonValueType t): type(t)
    {
        ref = 1;
    }

    QVariant v;

    const QBsonValue::QBsonValueType type;

    QAtomicInt ref;
};

/**
 * @brief QBsonValue construct a bsonvalue with type QBsonValueType::EMPTY
 */
QBsonValue::QBsonValue()
    : data(0)
{

}

/**
 * @brief QBsonValue::QBsonValue construct a bsonvalue with type
 * QBsonValue::INTEGER
 * @param value int value to store.
 */
QBsonValue::QBsonValue(int value)
    : data(new QBsonValueData(QBsonValue::Integer))
{
    data->v = value;
}

/**
 * @brief QBsonValue::QBsonValue construct a bsonvalue with type
 * QBsonValue::LONG
 * @param value long value to store.
 */
QBsonValue::QBsonValue(qlonglong value)
    : data(new QBsonValueData(QBsonValue::Long))
{
    data->v.setValue(value);
}

/**
 * @brief QBsonValue::QBsonValue construct a bsonvalue with type
 * QBsonValue::LONG
 * @param value long value to store.
 */
QBsonValue::QBsonValue(long value)
    : data(new QBsonValueData(QBsonValue::Long))
{
    data->v.setValue(value);
}

/**
 * @brief QBsonValue::QBsonValue construct a bsonvalue with type
 * QBsonValue::OBJECT
 * @param value object value to store.
 */
QBsonValue::QBsonValue(const QBsonObject &value)
    : data(new QBsonValueData(QBsonValue::Object))
{
    data->v.setValue(value);
}

/**
 * @brief QBsonValue::QBsonValue construct a bsonvalue with type
 * QBsonValue::STRING
 * @param value string value to store.
 */
QBsonValue::QBsonValue(const QString &value)
    : data(new QBsonValueData(QBsonValue::String))
{
    data->v = value;
}


/**
 * @brief QBsonValue construct a bsonvalue with type QBsonValue::BINARY
 * @param value binary value to store.
 */
QBsonValue::QBsonValue(const QByteArray &value):
    data(new QBsonValueData(QBsonValue::Binary))
{
    data->v = value;
}

/**
 * @brief QBsonValue construct a bsonvalue with type QBsonValue::Array
 * @param value array value to store.
 */
QBsonValue::QBsonValue(const QBsonArray &value):
    data(new QBsonValueData(QBsonValue::Array))
{
    data->v.setValue(value);
}

/**
 * @brief QBsonValue construct a bsonvalue with type QBsonValue::Bool
 * @param value bool value to store.
 */
QBsonValue::QBsonValue(bool value):
    data(new QBsonValueData(QBsonValue::Bool))
{
    data->v = value;
}

/**
 * @brief QBsonValue construct a bsonvalue with type QBsonValue::Id
 * @param value id value to store.
 */
QBsonValue::QBsonValue(const QBsonOid &value):
    data(new QBsonValueData(QBsonValue::Id))
{
    data->v.setValue(value);
}

/**
 * @brief QBsonValue construct a bsonvalue with type QBsonValue::STRING
 * @param value string value to store.
 */
QBsonValue::QBsonValue(const QLatin1String &value):
    data(new QBsonValueData(QBsonValue::String))
{
    data->v = value;
}

QBsonValue::QBsonValue(const char *value):
     data(new QBsonValueData(QBsonValue::String))
{
    data->v = QLatin1String(value);
}

/**
 * @brief QBsonValue construct a bsonvalue with type QBsonValue::DOUBLE
 * @param value double value to store.
 */
QBsonValue::QBsonValue(double value):
    data(new QBsonValueData(QBsonValue::Double))
{
    data->v = value;
}

/**
 * @brief QBsonValue construct a bsonvalue with type QBsonValue::DATETIME
 * @param value datetime value to store.
 */
QBsonValue::QBsonValue(const QDateTime &value):
    data(new QBsonValueData(QBsonValue::DateTime))
{
    data->v = value;
}

/**
 * @brief QBsonValue::QBsonValue copy constructor.
 * @param rhs
 */
QBsonValue::QBsonValue(const QBsonValue &rhs): data(rhs.data)
{
   data->ref.ref();
}

/**
 * @brief QBsonValue::operator = copy constructor.
 * @param rhs
 * @return
 */
QBsonValue &QBsonValue::operator=(const QBsonValue &rhs)
{
    qAtomicAssign<QBsonValueData>(data, rhs.data);
    return *this;
}

QBsonValue::~QBsonValue()
{
    // delete shared instance when not in used.
    if (!data->ref.deref())
        delete data;
}

QBsonValue::QBsonValueType QBsonValue::type() const
{
    return (data == 0)? QBsonValue::Empty : data->type;
}

/**
 * @brief QBsonValue::toString return the value as string.
 * @see QVariant::toString()
 * @return QString
 */
QString QBsonValue::toString() const
{
    switch(data->type) {
        case QBsonValue::Id:
            return toId().toString();
        default:
            return data->v.toString();
    }


}

/**
 * @brief QBsonValue::toDouble return the value as double.
 * @see QVariant::toDouble()
 * @return double
 */
double QBsonValue::toDouble() const
{
    return data->v.toDouble();
}

/**
 * @brief QBsonValue::toInt return the value as int.
 * @see QVariant::toInt()
 * @return int
 */
int QBsonValue::toInt() const
{
    return data->v.toInt();
}

/**
 * @brief QBsonValue::toLong return the value as long.
 * @see QVariant::toLongLong()
 * @return long
 */
long QBsonValue::toLong() const
{
    return data->v.toLongLong();
}

/**
 * @brief QBsonValue::toBinary() return the value as QByteArray.
 * @see QVariant::toByteArray()
 * @return QByteArray
 */
QByteArray QBsonValue::toBinary() const
{
    return data->v.toByteArray();
}

/**
 * @brief QBsonValue::toBsonObject() return the value as QBsonObject.
 * @see QVariant::value()
 * @return QBsonObject
 */
QBsonObject QBsonValue::toObject() const
{
    return data->v.value<QBsonObject>();
}

/**
 * @brief QBsonValue::toDateTime() return the value as QDateTime.
 * @see QVariant::toDateTime()
 * @return QDateTime
 */
QDateTime QBsonValue::toDateTime() const
{
    return data->v.toDateTime();
}

/**
 * @brief QBsonValue::toArray() return the value as QBsonArray.
 * @see QVariant::value()
 * @return QBsonArray
 */
QBsonArray QBsonValue::toArray() const
{
    return data->v.value<QBsonArray>();
}

/**
 * @brief QBsonValue::toBool return the value as bool.
 * @see QVariant::toBool()
 * @return bool
 */
bool QBsonValue::toBool() const
{
    return data->v.toBool();
}

/**
 * @brief QBsonValue::toId return the value as QBsonOid.
 * @see QVariant::value()
 * @return QBsonOid
 */
QBsonOid QBsonValue::toId() const
{
    return data->v.value<QBsonOid>();
}

bool QBsonValue::operator ==(const QBsonValue value) const
{
    return data->v == value.data->v;
}

/**
 * @brief QBsonValue::isObject returns true if type is QBsonValue::Object
 *
 * @return true if type is QBsonValue::Object otherwise false.
 */
bool QBsonValue::isObject() const
{
    return isValid(QBsonValue::Object);
}

/**
 * @brief QBsonValue::isArray returns true if type id QBsonValue::Array
 * @return true if type id QBsonValue::Array otherwise false.
 */
bool QBsonValue::isArray() const
{
    return isValid(QBsonValue::Array);
}

/**
 * @brief QBsonValue::isValid Returns true when the value is valid else false.
 * @return true when the value is valid else false.
 */
bool QBsonValue::isValid() const
{
    return data != 0;
}

/**
 * @brief QBsonValue::toVariant convert the value to a qvariant. if the value
 * could not convert a invalid qvariant is returned.
 *
 * @return QVariant()
 */
QVariant QBsonValue::toVariant() const
{
    return data->v;
}

/**
 * @brief QBsonValue::fromVariant converts a variant value to bsonvalue or
 * a default constructed QBsonValue if vaiant could not converted.
 * @param variant variant to convert
 * @return
 */
QBsonValue QBsonValue::fromVariant(QVariant variant)
{
    if (!variant.isValid()) return QBsonValue();
    switch (variant.type()) {
    case QVariant::Bool:
        return QBsonValue(variant.toBool());
    case QVariant::ByteArray:
        return QBsonValue(variant.toByteArray());
    case QVariant::String:
        return QBsonValue(variant.toString());
    case QVariant::LongLong:
        return QBsonValue(variant.toLongLong());
    case QVariant::Int:
        return QBsonValue(variant.toInt());
    case QVariant::Double:
        return QBsonValue(variant.toDouble());
    case QVariant::DateTime:
        return QBsonValue(variant.toDateTime());
    }
    if (variant.canConvert<QBsonObject>()) {
        return QBsonValue(variant.value<QBsonObject>());
    }
    if (variant.canConvert<QBsonOid>()) {
        return QBsonValue(variant.value<QBsonOid>());
    }
    if (variant.canConvert<QBsonArray>()) {
        return QBsonValue(variant.value<QBsonArray>());
    }

    return QBsonValue();
}

/**
 * @brief QBsonValue::isValid Returns true if the value is valid and given type.
 *
 * @param type value type
 *
 * @return true if the value is valid and given type.
 */
bool QBsonValue::isValid(QBsonValue::QBsonValueType type) const
{
    return isValid() && data->type == type;
}
