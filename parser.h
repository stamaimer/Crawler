#ifndef PARSER_H
#define PARSER_H

#include <QFile>

#include <QObject>

#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QThread>

#include "utility.h"
#include "scheduler.h"

class Parser : public QThread
{
    Q_OBJECT

    Scheduler* scheduler;

    int tid;

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
