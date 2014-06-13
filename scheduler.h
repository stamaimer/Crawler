#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <QElapsedTimer>

#include <QMutex>

#include "preprocessor.h"
#include "requester.h"
#include "extractor.h"
#include "inserter.h"

#define AMOUNT_OF_THREADS 20

class Scheduler
{
    QElapsedTimer timer;

public:
    Scheduler();
    void exec();

    QMutex mutex;

    QVector<QString> files;
};

#endif // SCHEDULER_H
