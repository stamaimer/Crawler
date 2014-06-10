#ifndef PARSER_H
#define PARSER_H

#include <QFile>

#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QThread>

#include "utility.h"
#include "scheduler.h"

class Parser : public QThread
{
    Scheduler* scheduler;

    int tid;

public:
    Parser(int,Scheduler*);

    void run();

private:
    void dealWithCategories(QJsonDocument);
    void dealWithProducts(QJsonDocument);
};

#endif // PARSER_H
