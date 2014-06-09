#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <QElapsedTimer>

#include "requester.h"
#include "extractor.h"
#include "parser.h"

class Scheduler
{
    QElapsedTimer timer;
public:
    Scheduler();
    void exec();
};

#endif // SCHEDULER_H
