#include "inserter.h"

Inserter::Inserter()
{
    db_connection = QSqlDatabase::addDatabase("QMYSQL");

    db_connection.setDatabaseName("walmart");

    db_connection.setHostName("localhost");

    db_connection.setUserName("root");

    db_connection.setPassword("");

    if(!db_connection.open())
    {
        qDebug() << db_connection.lastError();

        exit(1);
    }

    query = new QSqlQuery();
}

void Inserter::insert(Menu menu)
{
    QString sql = "INSERT IGNORE INTO menu VALUES (\"%1\", \"%2\", %3, \"%4\")";

    sql = sql.arg(menu.getId())
             .arg(menu.getName())
             .arg(menu.getCategory())
             .arg(menu.getParentCategories());


    qDebug() << sql;

    if(!query->exec(sql))
    {
        qDebug() << query->lastError().text();

        exit(1);
    }
}

void Inserter::insert(Merchandise merchandise)
{
    QString sql = "INSERT IGNORE INTO merchan_info VALUES (%1, \"%2\", \"%3\", %4, %5, %6, %7, %8)";

    sql = sql.arg(merchandise.getId())
             .arg(merchandise.getURL())
             .arg(merchandise.getName())
             .arg(merchandise.getMSRP())
             .arg(merchandise.getPrice())
             .arg(merchandise.getStock())
             .arg(merchandise.getReviews())
             .arg(QDate::currentDate().toString(Qt::ISODate));

    qDebug() << sql;

    if(!query->exec(sql))
    {
        qDebug() << query->lastError().text();

        exit(1);
    }
}
