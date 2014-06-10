#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <QElapsedTimer>

#include <QMutex>

#include "preprocessor.h"
#include "requester.h"
#include "extractor.h"
#include "inserter.h"
#include "parser.h"

#define AMOUNT_OF_THREADS 20

class Scheduler
{
    QElapsedTimer timer;

    QMutex mutex;

    QVector<QString> files;

public:
    Scheduler();
    void exec();
};

#endif // SCHEDULER_H
