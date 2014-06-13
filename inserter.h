#ifndef INSERTER_H
#define INSERTER_H

#include "category.h"
#include "product.h"

#include <QDebug>
#include <QDate>

#include <QVector>
#include <QString>
#include <QStringList>

#include <QSqlError>
#include <QSqlQuery>
#include <QSqlDatabase>

#include <QElapsedTimer>

class Inserter
{
    QElapsedTimer timer;

    QSqlDatabase db_connection;

    QSqlQuery query;

public:
    Inserter();

    void insert(QVector<Category>);

    void insert(QVector<Product>);
};

#endif // INSERTER_H
