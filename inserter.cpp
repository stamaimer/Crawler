#include "inserter.h"

Inserter::Inserter()
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

void Inserter::insert(QVector<Category> categories)
{
    if(0 == categories.size())
    {
        return;
    }

    QString sql = "INSERT IGNORE INTO categories VALUES ";

    QString pattern = "(\"%1\", \"%2\", \"%3\")";

    QStringList rows;

    for(Category category : categories)
    {
        rows << pattern.arg(category.getId())
                       .arg(category.getName())
                       .arg(category.getPath());
    }

    sql = sql + rows.join(',');

    if(!query.exec(sql))
    {
        qDebug() << query.lastError().text();

        qDebug() << query.lastQuery();
    }
}

void Inserter::insert(QVector<Product> products)
{
    if(0 == products.size())
    {
        return;
    }

    static int total_count = 0;

    static int num_rows_affected = 0;

    total_count = total_count + products.size();

    QString sql_base = "INSERT IGNORE INTO products_base (id) VALUES ";

    QString sql_info = "INSERT IGNORE INTO products_info VALUES ";

    QString pattern_base = "(\"%1\")";

    QString pattern_info = "(\"%1\", \"%2\", \"%3\", \"%4\", \"%5\", %6, %7, %8, %9, \"%10\", \"%11\")";

    QStringList rows_base;

    QStringList rows_info;

    for(Product product : products)
    {
        rows_base << pattern_base.arg(product.getUPC());

        rows_info << pattern_info.arg(product.getSKU())
                                 .arg(product.getUPC())
                                 .arg(product.getURL())
                                 .arg(product.getName())
                                 .arg(product.getDate())
                                 .arg(product.getMSRP())
                                 .arg(product.getPrice())
                                 .arg(product.getStock())
                                 .arg(product.getReviews())
                                 .arg(product.getPath())
                                 .arg(QDate::currentDate().toString(Qt::ISODate));
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
