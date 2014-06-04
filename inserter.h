#ifndef INSERTER_H
#define INSERTER_H

#include "menu.h"
#include "merchandise.h"

#include <QObject>

#include <QDate>

#include <QDebug>

#include <QVector>
#include <QString>
#include <QStringList>

#include <QSqlError>
#include <QSqlQuery>
#include <QSqlDatabase>

#include <QElapsedTimer>

class Inserter : public QObject
{
    Q_OBJECT

    QElapsedTimer timer;

    QSqlDatabase db_connection;

    QSqlQuery query;

public:
    explicit Inserter(QObject *parent = 0);

    void insert(QVector<Menu>);

    void insert(QVector<Merchandise>);

signals:

public slots:

};

#endif // INSERTER_H
