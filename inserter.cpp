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
    }
}

bool Inserter::insert(Menu menu)
{
    QString sql = "INSERT INTO menu VALUES (\"%1\", \"%2\", %3, \"%4\")";

    sql = sql.arg(menu.getId())
             .arg(menu.getName())
             .arg(menu.getCategory().toInt())
             .arg(menu.getParentCategories().join(","));


    qDebug() << sql;

    query.exec(sql);
}

bool Inserter::insert(Merchandise merchandise)
{

}
