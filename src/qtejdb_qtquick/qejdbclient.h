#ifndef QEJDBCLIENT_H
#define QEJDBCLIENT_H

#include <QObject>
#include <QJSValue>
#include <QQmlParserStatus>

#include "qejdbdatabase.h"

class QEjdbClient : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

    Q_PROPERTY(QString uri READ uri WRITE setUri NOTIFY uriChanged)
    Q_PROPERTY(QString connectionName READ connectionName WRITE setConnectionName NOTIFY connectionNameChanged)
    Q_PROPERTY(bool autoCreateCollection READ autoCreateCollection WRITE setAutoCreateCollection NOTIFY autoCreateCollectionChanged)

public:
    explicit QEjdbClient(QObject *parent = 0);
    ~QEjdbClient();

    void classBegin();
    void componentComplete();

    Q_INVOKABLE QJSValue save(QString collectionName, const QJSValue &jsValue);
    Q_INVOKABLE QJSValue load(QString collectionName, QString uid);

    QString uri() const;
    QString connectionName() const;
    bool autoCreateCollection() const;

signals:

    void uriChanged(QString arg);
    void connectionNameChanged(QString arg);
    void autoCreateCollectionChanged(bool arg);

public slots:

    void connect();
    void disconnect();
    void setConnectionName(QString arg);
    void setUri(QString arg);
    void setAutoCreateCollection(bool arg);

private:

    /**
     * @brief m_connectionName is the key to get database connection
     * from dictionary @see QEjdbDatabase::addDatabase()
     */
    QString m_connectionName;

    /**
     * @brief m_uri is the connection string @see QEjdbDatabase::addDatabase()
     */
    QString m_uri;

    inline QEjdbDatabase database()
    {
        return QEjdbDatabase::database(m_connectionName);
    }

    /**
     * @brief checkCollection creates the collection of not exist.
     *
     * @param db reference to database
     * @param collection collection Name
     */
    inline void checkCollection(QEjdbDatabase &db, QString collection)
    {
        if (!db.isOpen() && db.containsCollection(collection)) {
            db.createCollection(collection);
        }
    }
    bool m_autoCreateCollection;
};

#endif // QEJDBCLIENT_H