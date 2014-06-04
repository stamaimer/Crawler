#include "inserter.h"

Inserter::Inserter(QObject *parent) : QObject(parent)
{
    db_connection = QSqlDatabase::addDatabase("QMYSQL");

    db_connection.setDatabaseName("bestbuy");

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

void Inserter::insert(QVector<Menu> menus)
{
    QString sql = "INSERT IGNORE INTO menu VALUES ";

    QString pattern = "(\"%1\", \"%2\", \"%3\")";

    QStringList rows;

    for(Menu menu :menus)
    {
        rows << pattern.arg(menu.getId())
                       .arg(menu.getName())
                       .arg(menu.getPath());
    }

    sql = sql + rows.join(',');

    if(!query.exec(sql))
    {
        qDebug() << query.lastError().text();

        qDebug() << query.lastQuery();
    }
}

void Inserter::insert(QVector<Merchandise> merchandises)
{
    static int total_count = 0;

    static int num_rows_affected = 0;

    total_count = total_count + merchandises.size();

    QString sql_base = "INSERT IGNORE INTO merchan_base (id) VALUES ";

    QString sql_info = "INSERT IGNORE INTO merchan_info VALUES ";

    QString pattern_base = "(\"%1\")";

    QString pattern_info = "(\"%1\", \"%2\", \"%3\", \"%4\", \"%5\", %6, %7, %8, %9, \"%10\")";

    QStringList rows_base;

    QStringList rows_info;

    for(Merchandise merchandise : merchandises)
    {
        rows_base << pattern_base.arg(merchandise.getUPC());

        rows_info << pattern_info.arg(merchandise.getUPC())
                                 .arg(merchandise.getURL())
                                 .arg(merchandise.getSRC())
                                 .arg(merchandise.getName())
                                 .arg(merchandise.getDate())
                                 .arg(merchandise.getMSRP())
                                 .arg(merchandise.getPrice())
                                 .arg(merchandise.getStock())
                                 .arg(merchandise.getReviews())
                                 .arg(merchandise.getPath());
    }

    sql_base = sql_base + rows_base.join(',');

    sql_info = sql_info + rows_info.join(',');

    timer.start();

    if(!query.exec(sql_base))
    {
        qDebug() << query.lastError().text();

        qDebug() << query.lastQuery();
    }

    query.clear();

    if(!query.exec(sql_info))
    {
        qDebug() << query.lastError().text();

        qDebug() << query.lastQuery();
    }

    num_rows_affected = num_rows_affected + query.numRowsAffected();

    qDebug() << query.numRowsAffected() << "rows inserted," << timer.elapsed() / 1000 << "secs elapsed," << total_count << "rows in set," << num_rows_affected << "rows affected";

}
