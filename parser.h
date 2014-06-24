#ifndef PARSER_H
#define PARSER_H

#include <QFile>

#include <QObject>

#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QVector>
#include <QThread>

#include "category.h"
#include "product.h"

#include "utility.h"
#include "scheduler.h"

class Parser : public QThread
{
    Q_OBJECT

    Scheduler* scheduler;

    int tid;

    QVector<Category> categories;
    QVector<Product> products;

public:
    Parser(int, Scheduler*);

    void run();

private:
    void dealWithCategories(QJsonDocument);
    void dealWithProducts(QJsonDocument);
    void dealWithLargeDoc(QFile);

signals:
    void finished(int);
};

#endif // PARSER_H
