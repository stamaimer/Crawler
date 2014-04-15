#ifndef INSERTER_H
#define INSERTER_H

#include "menu.h"
#include "merchandise.h"

#include <QDate>

#include <QDebug>

#include <QVector>

#include <QString>

#include <QSqlError>
#include <QSqlQuery>
#include <QSqlDatabase>

class Inserter
{
    QSqlDatabase db_connection;

    QSqlQuery query;

public:
    Inserter();

    void insert(Menu);

    void insert(QVector<Merchandise>);
};

#endif // INSERTER_H
