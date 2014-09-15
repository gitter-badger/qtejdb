#include "tst_performance.h"
#include "qejdbdatabase.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QTest>
#include <QElapsedTimer>
#include <QDebug>

Tst_Performance::Tst_Performance(QObject *parent) :
    QObject(parent)
{
}

void Tst_Performance::initTestCase()
{
    QEjdbDatabase::removeDatabaseFiles(".", "test_db");
    QEjdbDatabase::addDatabase(".", "test_db", QEJDB::CREATE | QEJDB::WRITE
                             | QEJDB::LOCK_NB | QEJDB::TRUNCATE
                             | QEJDB::LOCK_NB | QEJDB::SYNC).open();
}

void Tst_Performance::cleanupTestCase()
{

    QEjdbDatabase::removeDatabase();
    QEjdbDatabase::removeDatabaseFiles(".", "test_db");
}

void Tst_Performance::tst_insQryTsd()
{
    QEjdbDatabase m_db = QEjdbDatabase::database();
    QEjdbCollection col = m_db.createCollection("testcollection");

    QString json = QString("{'name': 'test'\n 'num': '10'\n 'description': 'test test test test test test test'}");

    QElapsedTimer t;


    QJsonObject obj;
    obj.insert("name", QJsonValue("test"));
    obj.insert("name1", QJsonValue("test"));
    obj.insert("name2", QJsonValue("test"));
    obj.insert("name3", QJsonValue("test"));
    obj.insert("name4", QJsonValue("test"));
    obj.insert("name5", QJsonValue("test"));
    obj.insert("num", QJsonValue(10));
    obj.insert("num1", QJsonValue(10));
    obj.insert("num2", QJsonValue(10));
    obj.insert("num3", QJsonValue(10));
    obj.insert("num4", QJsonValue(10));
    obj.insert("num5", QJsonValue(10));
    obj.insert("description", QJsonValue("test test test test test test test test test test test test test test test test"));
    obj.insert("description1", QJsonValue("test test test test test test test test test test test test test test test test"));
    obj.insert("description2", QJsonValue("test test test test test test test test test test test test test test test test"));
    obj.insert("description3", QJsonValue("test test test test test test test test test test test test test test test test"));
    obj.insert("description4", QJsonValue("test test test test test test test test test test test test test test test test"));
    QJsonArray arr;
    arr.append("test1");
    arr.append("test2");
    arr.append("test3");
    arr.append("test4");
    obj.insert("array", arr);
    qDebug() << obj;
    int iteration = 10000;
    t.start();

    QString id;
    for (int j = 0; j < 5; j++) {

        for (int i = 0; i < iteration; i++) {
            obj.remove("_id");
            col.save(obj);
            if (i == 1) id = obj.value("_id").toString();
        }
        qDebug() << iteration << " inserts costs " << t.elapsed() << "ms per insert " << (double)t.elapsed()/iteration << "ms";
        t.restart();
    }

   /* QList<QJsonObject> result = m_db.query("testcollection", QEjdbCondition("name", QEjdbCondition::EQUALS, "test"));
    qDebug() << "found " << result.size() << " data";
    qDebug() << result.at(0);
    qDebug() << col.load(id);*/

}
