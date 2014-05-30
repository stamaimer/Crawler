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

}

void Inserter::insert(QVector<Merchandise> merchandises)
{

}
