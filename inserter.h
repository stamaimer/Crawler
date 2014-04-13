#ifndef INSERTER_H
#define INSERTER_H

#include "menu.h"
#include "merchandise.h"

#include <QDebug>

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

    bool insert(Menu);

    bool insert(Merchandise);
};

#endif // INSERTER_H
