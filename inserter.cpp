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

    query = QSqlQuery();
}

void Inserter::insert(Menu menu)
{
    QString sql = "INSERT IGNORE INTO menu VALUES (\"%1\", \"%2\", %3, \"%4\")";

    sql = sql.arg(menu.getId())
             .arg(menu.getName())
             .arg(menu.getCategory())
             .arg(menu.getParentCategories());

    if(!query.exec(sql))
    {
        qDebug() << query.lastError().text();

        qDebug() << query.lastQuery();

//        exit(1);
    }

//    qDebug() << sql;
}

void Inserter::insert(QVector<Merchandise> merchandises)
{
    QString sql_base = "INSERT IGNORE INTO merchan_base VALUES ";

    QString sql_info = "INSERT IGNORE INTO merchan_info VALUES ";

    QString pattern_base = "(%1)";

    QString pattern_info = "(%1, \"%2\", \"%3\", \"%4\", %5, %6, %7, %8, \"%9\")";

    QStringList rows_base;

    QStringList rows_info;

    for(Merchandise merchandise : merchandises)
    {
        rows_base << pattern_base.arg(merchandise.getId());

        rows_info << pattern_info.arg(merchandise.getId())
                                 .arg(merchandise.getURL())
                                 .arg(merchandise.getName())
                                 .arg(merchandise.getParent())
                                 .arg(merchandise.getMSRP())
                                 .arg(merchandise.getPrice())
                                 .arg(merchandise.getStock())
                                 .arg(merchandise.getReviews())
                                 .arg(QDate::currentDate().toString(Qt::ISODate));
    }

    sql_base = sql_base + rows_base.join(',');

    sql_info = sql_info + rows_info.join(',');

    if(!query.exec(sql_base))
    {
        qDebug() << query.lastError().text();

        qDebug() << query.lastQuery();

//        exit(1);
    }

    if(!query.exec(sql_info))
    {
        qDebug() << query.lastError().text();

        qDebug() << query.lastQuery();

//        exit(1);
    }


//    qDebug() << sql_base;

//    qDebug() << sql_info;

    qDebug() << merchandises.size() << "rows";
}
