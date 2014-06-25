#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <QElapsedTimer>

#include <QObject>

#include <QMutex>

#include "preprocessor.h"
#include "requester.h"
#include "extractor.h"
#include "inserter.h"

#define AMOUNT_OF_THREADS 5

class Parser;

class Scheduler : public QObject
{
    Q_OBJECT

    QElapsedTimer timer;

    Requester requester;

    Extractor extractor;

    PreProcessor pre_processor;

    Parser* parsers[AMOUNT_OF_THREADS];

public:

    Scheduler();

    void exec();

    Inserter* inserter;

    QMutex mutex;

    QVector<QString> files;

public slots:

    void finished(int);
};

#endif // SCHEDULER_H
